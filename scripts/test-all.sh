#!/usr/bin/env bash
#
# Script: test-all.sh
# Beschreibung: Full Mark-a-Spot test suite for the dev project. Runs the
#   frontend gates (lint, i18n, Vitest), the Drupal PHPUnit + integration
#   suites, and the GreenMail-backed mail-inbound end-to-end integration test.
#   The CLAUDE.md "Before release or push" gate points here.
# Version: 1.0.0
#
# All sub-suites run from the host (the pnpm scripts shell into DDEV as needed).
# Each section is reported independently; a non-zero overall exit means at
# least one section failed. Run from anywhere - paths are absolute.
#
# Usage:  bash scripts/test-all.sh

set -uo pipefail
IFS=$'\n\t'

readonly SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
readonly FRONTEND_DIR="${PROJECT_ROOT}/frontend"
readonly GREENMAIL_CONTAINER="ddev-dev-greenmail"

if [[ -t 1 ]]; then
  C_GREEN=$'\033[0;32m'; C_RED=$'\033[0;31m'; C_YELLOW=$'\033[0;33m'; C_BOLD=$'\033[1m'; C_RESET=$'\033[0m'
else
  C_GREEN=''; C_RED=''; C_YELLOW=''; C_BOLD=''; C_RESET=''
fi

log_section() { echo; echo "${C_BOLD}==> $*${C_RESET}"; }

# Section results for the final summary.
declare -a RESULTS=()
OVERALL=0

# run_section <label> <command...>: run a sub-suite, record PASS/FAIL.
run_section() {
  local label="$1"; shift
  log_section "${label}"
  if "$@"; then
    RESULTS+=("${C_GREEN}PASS${C_RESET}  ${label}")
  else
    RESULTS+=("${C_RED}FAIL${C_RESET}  ${label}")
    OVERALL=1
  fi
}

# skip_section <label> <reason>: record a skipped sub-suite (does not fail).
skip_section() {
  RESULTS+=("${C_YELLOW}SKIP${C_RESET}  $1 (${2})")
  log_section "$1"
  echo "${C_YELLOW}SKIPPED: $2${C_RESET}"
}

# frontend_pnpm <script>: run a pnpm script from INSIDE the frontend dir.
# Corepack resolves the pnpm version from the CWD, not from --dir: invoked
# from outside the frontend (no packageManager field there) it falls back to
# its global lastKnownGood pnpm, which then refuses the frontend's pinned
# version ("pnpm does not switch versions when running under corepack").
frontend_pnpm() { (cd "${FRONTEND_DIR}" && pnpm run "$@"); }

# Frontend gates via the existing pnpm scripts.
run_section "Unicode hygiene"            bash "${SCRIPT_DIR}/check-unicode-hygiene.sh"
run_section "ESLint"                     frontend_pnpm lint
run_section "i18n coverage"              frontend_pnpm i18n:check:strict
run_section "Vitest (unit)"              frontend_pnpm test:unit
run_section "PHPUnit (Drupal)"           frontend_pnpm test:drupal
run_section "Endpoint + multi-tenant"    frontend_pnpm test:integration

# Mail inbound integration: requires the GreenMail sidecar. Skip gracefully
# (rather than failing the whole suite) when the container is not running, so
# the suite stays green on machines that have not opted into the sidecar.
log_section "Mail inbound integration"
if docker ps --format '{{.Names}}' 2>/dev/null | grep -qx "${GREENMAIL_CONTAINER}"; then
  if bash "${SCRIPT_DIR}/test-mail-inbound.sh"; then
    RESULTS+=("${C_GREEN}PASS${C_RESET}  Mail inbound integration")
  else
    RESULTS+=("${C_RED}FAIL${C_RESET}  Mail inbound integration")
    OVERALL=1
  fi
else
  RESULTS+=("${C_YELLOW}SKIP${C_RESET}  Mail inbound integration (greenmail sidecar not running)")
  echo "${C_YELLOW}SKIPPED: GreenMail sidecar '${GREENMAIL_CONTAINER}' is not running.${C_RESET}"
  echo "         Enable it with .ddev/docker-compose.greenmail.yaml + 'ddev restart',"
  echo "         then re-run. Standalone: bash scripts/test-mail-inbound.sh"
fi

# Summary.
echo
echo "${C_BOLD}========================================${C_RESET}"
echo "${C_BOLD}TEST SUITE SUMMARY${C_RESET}"
echo "${C_BOLD}========================================${C_RESET}"
for line in "${RESULTS[@]}"; do
  echo "  ${line}"
done
echo

if (( OVERALL == 0 )); then
  echo "${C_GREEN}All sections passed (skips do not count as failures).${C_RESET}"
else
  echo "${C_RED}One or more sections failed.${C_RESET}"
fi
exit "${OVERALL}"
