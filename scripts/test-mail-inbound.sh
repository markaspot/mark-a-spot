#!/usr/bin/env bash
#
# Script: test-mail-inbound.sh
# Beschreibung: End-to-end integration test for markaspot_mail_inbound
#   (staging model, issue #467 rebuild).
#   Drives the fixture mails through the REAL pipeline:
#     GreenMail IMAP -> MailboxFetcher fetch -> queue -> MailIngestOrchestrator
#     -> STAGED inbound_mail entities (NO auto service request),
#   then promotes explicitly via InboundMailPromoter (the Open311 processor
#   contract) and asserts the full cycle against the live DB:
#     - ingestion stages mails and creates ZERO service_request nodes,
#     - a genuine reply (04) threads onto its staged mail,
#     - a spoofed reply (05) is isolated into its own staged mail,
#     - a malformed Date header (02) survives parsing (H2),
#     - staged attachments live in the access-restricted staging dir,
#     - promotion creates an unpublished SR with jurisdiction, source=email,
#       platform title, attached media; the staged file is RELOCATED out of
#       the staging dir and the mail releases its file reference,
#     - a reply arriving AFTER promotion (06, wave 2) becomes an
#       internal_remark on the promoted node, not a new mail or node.
# Version: 2.0.0 (rewritten for the staged->promote model; 1.x asserted the
#   retired auto-SR behavior)
#
# Idempotent and fully repeatable: every run resets GreenMail to a clean
# inbox and wipes prior email-source nodes + ALL inbound_mail entities, so it
# can be run back to back and always asserts on the same clean state.
#
# Prereqs:
#   - DDEV project "dev" running.
#   - GreenMail sidecar running (.ddev/docker-compose.greenmail.yaml) with the
#     /preload mount and tests/fixtures/mail-inbound/preload fixtures in place.
#   - markaspot_mail_inbound enabled (the script enables it if missing).
#
# Usage:  bash scripts/test-mail-inbound.sh
# Exit:   0 = all assertions PASS, 1 = first failing assertion (FAIL).

set -euo pipefail
IFS=$'\n\t'

# Konstanten
# Resolved at runtime (the DDEV project is "mark-a-spot", the folder is
# dev/ — do not hardcode either name).
readonly GREENMAIL_CONTAINER="$(docker ps --filter name=greenmail --format '{{.Names}}' | head -1)"
readonly MAILBOX_ID="greenmail-it"
readonly RECIPIENT="report@dev.local"
readonly JURISDICTION_GID=1
readonly IMAP_READY_TIMEOUT=60

# Logging
log_info()  { echo "[INFO]  $(date '+%H:%M:%S') - $*"; }
log_warn()  { echo "[WARN]  $(date '+%H:%M:%S') - $*" >&2; }
log_error() { echo "[ERROR] $(date '+%H:%M:%S') - $*" >&2; }

# ANSI for the final summary (only when stdout is a terminal).
if [[ -t 1 ]]; then
  C_GREEN=$'\033[0;32m'; C_RED=$'\033[0;31m'; C_RESET=$'\033[0m'
else
  C_GREEN=''; C_RED=''; C_RESET=''
fi

# Collected assertion results, printed in the summary.
declare -a ASSERTIONS=()

# fail <message>: print, record, and exit 1 immediately on a failing assertion.
fail() {
  ASSERTIONS+=("${C_RED}FAIL${C_RESET}  $1")
  echo
  echo "${C_RED}========================================${C_RESET}"
  echo "${C_RED}MAIL-INBOUND INTEGRATION TEST: FAIL${C_RESET}"
  echo "${C_RED}========================================${C_RESET}"
  local line
  for line in "${ASSERTIONS[@]}"; do
    echo "  $line"
  done
  echo
  log_error "First failing assertion: $1"
  exit 1
}

# pass <message>: record a passing assertion (printed in the summary at the end).
pass() {
  ASSERTIONS+=("${C_GREEN}PASS${C_RESET}  $1")
  log_info "OK: $1"
}

# sql <query>: run a single-value SQL query and trim whitespace.
sql() {
  ddev drush sql:query "$1" 2>/dev/null | tr -d '[:space:]'
}

# getval <multiline-output> <key>: extract "key=value" lines from php:eval output.
getval() {
  printf '%s\n' "$1" | sed -n "s/^$2=//p" | head -1
}

main() {
  log_info "Mark-a-Spot mail-inbound end-to-end integration test (staging model)"

  # --- 1. Ensure the module is enabled. ------------------------------------
  if ddev drush pm:list --status=enabled --field=name 2>/dev/null | grep -qx 'markaspot_mail_inbound'; then
    log_info "Module markaspot_mail_inbound is enabled."
  else
    log_warn "Module not enabled; enabling markaspot_mail_inbound."
    ddev drush en markaspot_mail_inbound -y
  fi

  # --- 2. Configure the mailbox idempotently (verified shape). -------------
  # Sets markaspot_mail_inbound.settings.mailboxes to a single GreenMail
  # mailbox. Written every run so the test is self-contained regardless of any
  # prior admin-form state. Uses the editable config so it persists.
  log_info "Configuring mailbox '${MAILBOX_ID}' to point at the GreenMail sidecar."
  ddev drush php:eval '
    $config = \Drupal::configFactory()->getEditable("markaspot_mail_inbound.settings");
    $config->set("mailboxes", [[
      "id" => "'"${MAILBOX_ID}"'",
      "label" => "GreenMail integration test",
      "enabled" => TRUE,
      "imap" => [
        "host" => "greenmail",
        "port" => 3143,
        "encryption" => "none",
        "username" => "'"${RECIPIENT}"'",
        "password" => "test",
        "folder" => "INBOX",
        "fetch_limit" => 50,
      ],
      "recipient_addresses" => ["'"${RECIPIENT}"'"],
      "jurisdiction_gid" => '"${JURISDICTION_GID}"',
      "default_category_tid" => 0,
      "sender_allowlist" => [],
      "sender_blocklist" => [],
    ]]);
    $config->save();
    echo "mailbox-configured\n";
  '

  # --- 3. Clean slate: prior email nodes, ALL inbound_mail entities, flood. -
  log_info "Deleting prior email-source nodes and inbound_mail entities."
  ddev drush php:eval '
    $nodeStorage = \Drupal::entityTypeManager()->getStorage("node");
    $nids = $nodeStorage->getQuery()
      ->condition("type", "service_request")
      ->condition("field_source", "email")
      ->accessCheck(FALSE)
      ->execute();
    if ($nids) {
      $nodeStorage->delete($nodeStorage->loadMultiple($nids));
      echo "deleted " . count($nids) . " prior email node(s)\n";
    }
    $mailStorage = \Drupal::entityTypeManager()->getStorage("inbound_mail");
    $ids = $mailStorage->getQuery()->accessCheck(FALSE)->execute();
    if ($ids) {
      $mailStorage->delete($mailStorage->loadMultiple($ids));
      echo "deleted " . count($ids) . " prior inbound_mail entitie(s)\n";
    }
  '
  # Clear the flood table too: a prior run registered the senders, and the
  # flood gate would otherwise throttle re-ingestion of the same addresses.
  ddev drush php:eval '\Drupal::database()->delete("flood")->condition("event", "markaspot_mail_inbound.sender")->execute(); echo "flood cleared\n";'

  # Baseline: total service_request count BEFORE ingestion. The staging model
  # guarantees ingestion itself creates ZERO nodes; only explicit promotion
  # may add exactly the promoted ones.
  local node_baseline
  node_baseline="$(sql "SELECT COUNT(*) FROM node_field_data WHERE type='service_request'")"
  log_info "Baseline service_request count: ${node_baseline}."

  # --- 4. Reset GreenMail to a clean, deterministically ordered inbox. -----
  # A restart re-reads /preload (proving the preload mount works and serving
  # the human desktop-client use-case). BUT GreenMail's preload load order
  # follows the host filesystem readdir order, which on APFS is a name hash,
  # not alphabetical, so the IMAP UID order (= queue FIFO order) it produces is
  # not deterministic and can put reply 04 before its parent 01. For the test
  # assertions we therefore purge that inbox and re-inject the fixtures over
  # plain SMTP in sorted filename order (01..05) via inject.php, which assigns
  # IMAP UIDs in strict arrival order. The full real pipeline is still
  # exercised: SMTP -> GreenMail IMAP store -> MailboxFetcher (IMAP) -> queue.
  log_info "Restarting GreenMail (${GREENMAIL_CONTAINER}) to reload preloaded fixtures."
  docker restart "${GREENMAIL_CONTAINER}" >/dev/null

  log_info "Waiting for GreenMail IMAP (greenmail:3143) to become reachable."
  local waited=0
  until ddev exec bash -c "cat </dev/null >/dev/tcp/greenmail/3143" >/dev/null 2>&1; do
    sleep 2
    waited=$((waited + 2))
    if (( waited >= IMAP_READY_TIMEOUT )); then
      fail "GreenMail IMAP did not become reachable within ${IMAP_READY_TIMEOUT}s."
    fi
  done
  log_info "GreenMail IMAP reachable after ${waited}s."
  # Brief settle so the preload finishes loading before we purge + re-inject.
  sleep 3

  log_info "Purging GreenMail and re-injecting wave-1 fixtures in deterministic order (01..05)."
  if ! ddev exec php tests/fixtures/mail-inbound/inject.php; then
    fail "Fixture injection failed (tests/fixtures/mail-inbound/inject.php)."
  fi

  # --- 5. Fetch + process the queue (wave 1). -------------------------------
  log_info "Fetching mailbox into the queue."
  ddev drush markaspot:mail-inbound:fetch
  log_info "Processing the markaspot_mail_inbound queue."
  ddev drush queue:run markaspot_mail_inbound

  # --- 6. Wave-1 assertions: staging, threading, isolation. ----------------
  log_info "Asserting wave-1 outcome (staging model) against the live database."

  # A. Ingestion created ZERO service_request nodes (the core of the staging
  #    model: an email is NOT immediately a service request).
  local node_after_ingest email_node_count
  node_after_ingest="$(sql "SELECT COUNT(*) FROM node_field_data WHERE type='service_request'")"
  email_node_count="$(sql "SELECT COUNT(*) FROM node__field_source WHERE field_source_value='email'")"
  if [[ "${node_after_ingest}" == "${node_baseline}" && "${email_node_count}" == "0" ]]; then
    pass "Ingestion created ZERO nodes (${node_baseline} -> ${node_after_ingest}, email-source 0): mails are staged, not auto-promoted."
  else
    fail "Ingestion changed the node count (${node_baseline} -> ${node_after_ingest}, email-source ${email_node_count}); the staging model must not auto-create service requests."
  fi

  # B-G. Staged entity state in one round trip.
  local wave1
  wave1="$(ddev drush php:eval '
    $storage = \Drupal::entityTypeManager()->getStorage("inbound_mail");
    $ids = $storage->getQuery()->accessCheck(FALSE)->execute();
    $byMsg = [];
    foreach ($storage->loadMultiple($ids) as $m) {
      $byMsg[(string) $m->get("message_id")->value] = $m;
    }
    $staged = array_filter($byMsg, fn($m) => $m->getState() === "staged");
    echo "total=" . count($byMsg) . "\n";
    echo "staged=" . count($staged) . "\n";
    $m1 = $byMsg["mas-it-01@example.com"] ?? NULL;
    echo "m1_state=" . ($m1 ? $m1->getState() : "missing") . "\n";
    echo "m1_jur=" . ($m1 ? $m1->getJurisdictionId() : -1) . "\n";
    echo "m1_has_reply=" . ($m1 && str_contains($m1->getBody(), "Nachtrag") ? "yes" : "no") . "\n";
    echo "m1_thread_has_04=" . ($m1 && in_array("mas-it-04@example.com", $m1->getThreadMessageIds(), TRUE) ? "yes" : "no") . "\n";
    echo "m1_has_spoof=" . ($m1 && str_contains($m1->getBody(), "uebernehme diese Meldung") ? "yes" : "no") . "\n";
    echo "m2_state=" . (isset($byMsg["mas-it-02@example.com"]) ? $byMsg["mas-it-02@example.com"]->getState() : "missing") . "\n";
    echo "m4_staged_own=" . (isset($byMsg["mas-it-04@example.com"]) ? "yes" : "no") . "\n";
    $m5 = $byMsg["mas-it-05@example.com"] ?? NULL;
    echo "m5_state=" . ($m5 ? $m5->getState() : "missing") . "\n";
    echo "m5_from=" . ($m5 ? $m5->getFromAddress() : "missing") . "\n";
    $m3 = $byMsg["mas-it-03@example.com"] ?? NULL;
    echo "m3_state=" . ($m3 ? $m3->getState() : "missing") . "\n";
    $fileIds = $m3 ? $m3->getAttachmentFileIds() : [];
    echo "m3_files=" . count($fileIds) . "\n";
    if ($fileIds !== []) {
      $file = \Drupal::entityTypeManager()->getStorage("file")->load(reset($fileIds));
      echo "m3_uri=" . ($file ? $file->getFileUri() : "missing") . "\n";
    }
  ')"

  # B. Exactly 4 staged mails: 01, 02, 03 are new reports, 05 is the spoofed
  #    reply isolated into its OWN staged mail. 04 threads, so no 5th entity.
  if [[ "$(getval "${wave1}" total)" == "4" && "$(getval "${wave1}" staged)" == "4" ]]; then
    pass "Exactly 4 staged inbound_mail entities (01, 02, 03, 05); reply 04 staged none."
  else
    fail "Expected 4 staged inbound_mail entities, got total=$(getval "${wave1}" total) staged=$(getval "${wave1}" staged)."
  fi

  # C. Genuine reply 04 threaded onto staged mail 01.
  if [[ "$(getval "${wave1}" m1_state)" == "staged" && "$(getval "${wave1}" m1_has_reply)" == "yes" && "$(getval "${wave1}" m1_thread_has_04)" == "yes" ]]; then
    pass "Reply 04 threaded onto staged mail 01 (body appended, thread id recorded)."
  else
    fail "Reply 04 did not thread onto staged mail 01 (state=$(getval "${wave1}" m1_state) reply=$(getval "${wave1}" m1_has_reply) thread=$(getval "${wave1}" m1_thread_has_04))."
  fi
  if [[ "$(getval "${wave1}" m4_staged_own)" == "no" ]]; then
    pass "Reply 04 created no inbound_mail of its own."
  else
    fail "Reply 04 wrongly created its own inbound_mail entity."
  fi

  # D. H1: spoofed reply 05 is ISOLATED: own staged mail, no content injection
  #    into mail 01.
  if [[ "$(getval "${wave1}" m5_state)" == "staged" && "$(getval "${wave1}" m5_from)" == "eve@evil.example" && "$(getval "${wave1}" m1_has_spoof)" == "no" ]]; then
    pass "H1: spoofed reply 05 isolated into its own staged mail; mail 01 carries no spoofed content."
  else
    fail "H1 broken: spoof isolation failed (m5_state=$(getval "${wave1}" m5_state) m5_from=$(getval "${wave1}" m5_from) injected=$(getval "${wave1}" m1_has_spoof))."
  fi

  # E. H2: malformed Date header (02) survived parsing and staged.
  if [[ "$(getval "${wave1}" m2_state)" == "staged" ]]; then
    pass "H2: mail 02 staged despite the malformed Date header."
  else
    fail "H2 broken: mail 02 (bad Date) is $(getval "${wave1}" m2_state); fallback_date parsing regressed."
  fi

  # F. Mail 01 carries the mailbox jurisdiction.
  if [[ "$(getval "${wave1}" m1_jur)" == "${JURISDICTION_GID}" ]]; then
    pass "Staged mail 01 carries jurisdiction ${JURISDICTION_GID}."
  else
    fail "Staged mail 01 expected jurisdiction ${JURISDICTION_GID}, got '$(getval "${wave1}" m1_jur)'."
  fi

  # G. Attachment of 03 staged in the access-restricted staging dir with a
  #    non-predictable filename (review HIGH 1).
  local m3_uri
  m3_uri="$(getval "${wave1}" m3_uri)"
  if [[ "$(getval "${wave1}" m3_files)" == "1" && "${m3_uri}" == *"mail-inbound-staged/"* ]] \
    && printf '%s' "${m3_uri}" | grep -qE '/mail-[0-9a-f]{32}\.'; then
    pass "Attachment of 03 staged in the restricted staging dir with a random filename (${m3_uri})."
  else
    fail "Attachment staging of 03 wrong (files=$(getval "${wave1}" m3_files) uri=${m3_uri})."
  fi

  # --- 7. Promotion: 03 (with attachment), then 01 (dialogue -> remark). ---
  log_info "Promoting staged mail 03 via InboundMailPromoter (Open311 processor contract)."
  local promo3
  promo3="$(ddev drush php:eval '
    $storage = \Drupal::entityTypeManager()->getStorage("inbound_mail");
    $ids = $storage->getQuery()->condition("message_id", "mas-it-03@example.com")->accessCheck(FALSE)->execute();
    $mail = $storage->load((int) reset($ids));
    $fileIds = $mail->getAttachmentFileIds();
    $tids = \Drupal::entityQuery("taxonomy_term")
      ->condition("vid", "service_category")
      ->exists("field_service_code")
      ->accessCheck(FALSE)->sort("tid")->range(0, 1)->execute();
    $tid = (int) reset($tids);
    $node = \Drupal::service("markaspot_mail_inbound.promoter")->promoteToServiceRequest($mail, $tid);
    echo "tid=" . $tid . "\n";
    echo "nid=" . $node->id() . "\n";
    echo "published=" . ($node->isPublished() ? 1 : 0) . "\n";
    echo "source=" . $node->get("field_source")->value . "\n";
    echo "msgid=" . $node->get("field_email_message_id")->value . "\n";
    echo "jur=" . (int) $node->get("field_jurisdiction")->target_id . "\n";
    echo "title=" . $node->getTitle() . "\n";
    echo "media_count=" . count(array_filter($node->get("field_request_media")->getValue())) . "\n";
    $media = \Drupal::entityTypeManager()->getStorage("media")->load((int) $node->get("field_request_media")->target_id);
    $file = $media ? \Drupal::entityTypeManager()->getStorage("file")->load((int) $media->get("field_media_image")->target_id) : NULL;
    echo "file_uri=" . ($file ? $file->getFileUri() : "missing") . "\n";
    echo "file_exists=" . ($file && file_exists($file->getFileUri()) ? "yes" : "no") . "\n";
    echo "mail_state=" . $mail->getState() . "\n";
    echo "mail_nid=" . (int) $mail->get("nid")->target_id . "\n";
    echo "mail_files=" . count($mail->getAttachmentFileIds()) . "\n";
  ')"

  local nid03
  nid03="$(getval "${promo3}" nid)"
  if [[ -n "${nid03}" && "${nid03}" =~ ^[0-9]+$ ]]; then
    pass "Mail 03 promoted to service request nid ${nid03} (category tid $(getval "${promo3}" tid))."
  else
    fail "Promotion of mail 03 created no node."
  fi

  # H. The promoted SR honors the platform contract: unpublished (moderation),
  #    jurisdiction, source=email, threading id, system-generated title.
  if [[ "$(getval "${promo3}" published)" == "0" \
    && "$(getval "${promo3}" jur)" == "${JURISDICTION_GID}" \
    && "$(getval "${promo3}" source)" == "email" \
    && "$(getval "${promo3}" msgid)" == "mas-it-03@example.com" \
    && "$(getval "${promo3}" title)" == "#"* ]]; then
    pass "Promoted SR ${nid03}: unpublished, jurisdiction ${JURISDICTION_GID}, source=email, platform title '$(getval "${promo3}" title)'."
  else
    fail "Promoted SR contract violated (published=$(getval "${promo3}" published) jur=$(getval "${promo3}" jur) source=$(getval "${promo3}" source) msgid=$(getval "${promo3}" msgid) title=$(getval "${promo3}" title))."
  fi

  # I. Attachment became media AND the file was RELOCATED out of the denied
  #    staging dir into the media scheme (residual fix: within-scheme move).
  local file_uri
  file_uri="$(getval "${promo3}" file_uri)"
  if [[ "$(getval "${promo3}" media_count)" -ge 1 && "${file_uri}" != *"mail-inbound-staged/"* && "$(getval "${promo3}" file_exists)" == "yes" ]]; then
    pass "Attachment became media; file relocated out of staging (${file_uri})."
  else
    fail "Attachment relocation failed (media=$(getval "${promo3}" media_count) uri=${file_uri} exists=$(getval "${promo3}" file_exists))."
  fi

  # J. The mail flipped to promoted, points at the node, and RELEASED its file
  #    reference (residual fix: no orphan refs at uninstall).
  if [[ "$(getval "${promo3}" mail_state)" == "promoted" && "$(getval "${promo3}" mail_nid)" == "${nid03}" && "$(getval "${promo3}" mail_files)" == "0" ]]; then
    pass "Mail 03 is promoted, points at nid ${nid03}, attachment reference released."
  else
    fail "Mail 03 post-promotion state wrong (state=$(getval "${promo3}" mail_state) nid=$(getval "${promo3}" mail_nid) files=$(getval "${promo3}" mail_files))."
  fi

  # K. Promote 01: citizen-quote description + conversation log as remark.
  # Since 89788c1b the description quotes ONLY the original citizen mail;
  # the pre-promotion dialogue (the threaded reply from 04) is preserved as
  # ONE internal_remark paragraph instead of leaking into the citizen-visible
  # body (product decision, 2026-06-11).
  log_info "Promoting staged mail 01 (carries the threaded reply from 04)."
  local promo1
  promo1="$(ddev drush php:eval '
    $storage = \Drupal::entityTypeManager()->getStorage("inbound_mail");
    $ids = $storage->getQuery()->condition("message_id", "mas-it-01@example.com")->accessCheck(FALSE)->execute();
    $mail = $storage->load((int) reset($ids));
    $tids = \Drupal::entityQuery("taxonomy_term")
      ->condition("vid", "service_category")
      ->exists("field_service_code")
      ->accessCheck(FALSE)->sort("tid")->range(0, 1)->execute();
    $node = \Drupal::service("markaspot_mail_inbound.promoter")->promoteToServiceRequest($mail, (int) reset($tids));
    $body = (string) $node->get("body")->value;
    echo "nid=" . $node->id() . "\n";
    echo "has_original=" . (str_contains($body, "Schlagloch") ? "yes" : "no") . "\n";
    echo "body_has_reply=" . (str_contains($body, "Nachtrag") ? "yes" : "no") . "\n";
    $remark_text = "";
    foreach ($node->get("field_internal_remark")->referencedEntities() as $p) {
      $remark_text .= (string) $p->get("field_internal_remark_text")->value . "\n";
    }
    echo "remark_has_label=" . (str_contains($remark_text, "E-Mail-Konversation aus dem Posteingang") ? "yes" : "no") . "\n";
    echo "remark_has_reply=" . (str_contains($remark_text, "Nachtrag") ? "yes" : "no") . "\n";
    echo "email=" . $node->get("field_e_mail")->value . "\n";
  ')"
  local nid01
  nid01="$(getval "${promo1}" nid)"
  if [[ -n "${nid01}" && "$(getval "${promo1}" has_original)" == "yes" && "$(getval "${promo1}" body_has_reply)" == "no" && "$(getval "${promo1}" remark_has_label)" == "yes" && "$(getval "${promo1}" remark_has_reply)" == "yes" && "$(getval "${promo1}" email)" == "anna.buerger@example.com" ]]; then
    pass "Mail 01 promoted (nid ${nid01}); description quotes only the original, dialogue preserved as internal remark; reporter stored."
  else
    fail "Promotion of mail 01 field mapping wrong (nid=${nid01} original=$(getval "${promo1}" has_original) body_reply=$(getval "${promo1}" body_has_reply) remark_label=$(getval "${promo1}" remark_has_label) remark_reply=$(getval "${promo1}" remark_has_reply) email=$(getval "${promo1}" email))."
  fi

  # --- 8. Wave 2: a reply arriving AFTER promotion. -------------------------
  # Mail 06 (same sender as 01) references mas-it-01, whose mail is now
  # promoted: the orchestrator must route it onto the promoted NODE as an
  # internal_remark (H1 sender check), not stage a new mail.
  log_info "Injecting wave-2 fixture (06, reply after promotion) and re-running fetch + queue."
  if ! ddev exec bash -c "FIXTURE_DIR=tests/fixtures/mail-inbound/wave2 php tests/fixtures/mail-inbound/inject.php"; then
    fail "Wave-2 fixture injection failed."
  fi
  ddev drush markaspot:mail-inbound:fetch
  ddev drush queue:run markaspot_mail_inbound

  local wave2
  wave2="$(ddev drush php:eval '
    $mailCount = count(\Drupal::entityQuery("inbound_mail")->condition("message_id", "mas-it-06@example.com")->accessCheck(FALSE)->execute());
    echo "m6_mails=" . $mailCount . "\n";
    $nodeCount = count(\Drupal::entityQuery("node")->condition("field_email_message_id", "mas-it-06@example.com")->accessCheck(FALSE)->execute());
    echo "m6_nodes=" . $nodeCount . "\n";
    $node = \Drupal::entityTypeManager()->getStorage("node")->load('"${nid01}"');
    $marker = "[mail-inbound:reply-id:" . hash("sha256", "mas-it-06@example.com") . "]";
    $found = "no";
    foreach ($node->get("field_internal_remark")->getValue() as $item) {
      $p = \Drupal::entityTypeManager()->getStorage("paragraph")->load($item["target_id"] ?? 0);
      if ($p && str_contains((string) $p->get("field_internal_remark_text")->value, $marker)) {
        $found = "yes";
        break;
      }
    }
    echo "remark_found=" . $found . "\n";
  ')"

  # L. Reply 06 staged nothing and created no node.
  if [[ "$(getval "${wave2}" m6_mails)" == "0" && "$(getval "${wave2}" m6_nodes)" == "0" ]]; then
    pass "Wave-2 reply 06 created no inbound_mail and no node."
  else
    fail "Wave-2 reply 06 leaked (mails=$(getval "${wave2}" m6_mails) nodes=$(getval "${wave2}" m6_nodes)); it must thread onto the promoted SR."
  fi

  # M. Reply 06 landed as an internal_remark on the promoted node 01.
  if [[ "$(getval "${wave2}" remark_found)" == "yes" ]]; then
    pass "Wave-2 reply 06 threaded onto promoted SR ${nid01} as an internal_remark (idempotency marker present)."
  else
    fail "Wave-2 reply 06 did not append an internal_remark to promoted SR ${nid01}."
  fi

  # N. Final tally: exactly the 2 explicit promotions added nodes.
  local node_final
  node_final="$(sql "SELECT COUNT(*) FROM node_field_data WHERE type='service_request'")"
  if [[ "${node_final}" == "$(( node_baseline + 2 ))" ]]; then
    pass "Node count ${node_baseline} -> ${node_final}: exactly the 2 explicit promotions."
  else
    fail "Node count drifted (${node_baseline} -> ${node_final}); expected exactly +2 from promotions."
  fi

  # --- 9. Green summary. ----------------------------------------------------
  echo
  echo "${C_GREEN}========================================${C_RESET}"
  echo "${C_GREEN}MAIL-INBOUND INTEGRATION TEST: PASS${C_RESET}"
  echo "${C_GREEN}========================================${C_RESET}"
  local line
  for line in "${ASSERTIONS[@]}"; do
    echo "  $line"
  done
  echo
  log_info "All assertions passed. Promoted: 03 -> nid ${nid03}, 01 -> nid ${nid01} (04 threaded pre-, 06 post-promotion)."
  exit 0
}

main "$@"
