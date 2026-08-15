#!/usr/bin/env bash
# Unicode-Hygiene-Gate: erkennt unsichtbare/gefaehrliche Unicode-Zeichen in Quelldateien.
#
# Erkennt und SCHEITERT (nie still entfernen):
#   - Zero-Width: U+200B ZWSP, U+2060 WJ, U+FEFF (BOM ausserhalb Dateianfang)
#   - Bidi-Steuerzeichen: U+202A..U+202E, U+2066..U+2069  (Trojan-Source, CVE-2021-42574)
#   - Tag-Zeichen: U+E0001, U+E0020..U+E007F (unsichtbare ASCII-Spiegel)
# Bewusst ERLAUBT in i18n-Locale-Dateien: U+200C ZWNJ / U+200D ZWJ
#   (legitim in Arabisch/Persisch bzw. Emoji-Sequenzen), ausserhalb i18n verboten.
#
# Statistische Wasserzeichen in Token-Mustern kann KEIN Zeichen-Scan finden;
# dieses Gate deckt ausschliesslich zeichenbasierte Traeger ab.
set -euo pipefail
cd "$(dirname "$0")/.."

FAIL=0

scan() {
  local label="$1" pattern="$2"; shift 2
  local hits
  hits=$(rg -n --no-heading "$pattern" "$@" \
      -g '*.{ts,vue,js,mjs,php,module,install,inc,profile,theme,yml,yaml,json,md,twig,css,scss,html}' \
      -g '!**/node_modules/**' -g '!**/vendor/**' -g '!**/.nuxt/**' -g '!**/.output/**' \
      2>/dev/null | head -20) || true
  if [ -n "$hits" ]; then
    echo "FAIL [$label]:"
    # Fundstellen zeigen, unsichtbare Zeichen als Codepoints sichtbar machen
    echo "$hits" | perl -CS -pe 's/([\x{200B}\x{200C}\x{200D}\x{2060}\x{FEFF}\x{202A}-\x{202E}\x{2066}-\x{2069}\x{E0001}\x{E0020}-\x{E007F}])/sprintf("<U+%04X>",ord($1))/ge'
    FAIL=1
  fi
}

ZW='[\x{200B}\x{2060}\x{FEFF}]'
BIDI='[\x{202A}-\x{202E}\x{2066}-\x{2069}]'
TAGS='[\x{E0001}\x{E0020}-\x{E007F}]'
JOINERS='[\x{200C}\x{200D}]'

TREES=(frontend/app frontend/server frontend/shared frontend/config frontend/pro-layer frontend/fastmap-layer frontend/i18n web/profiles/contrib/markaspot config scripts)
EXISTING=(); for t in "${TREES[@]}"; do [ -e "$t" ] && EXISTING+=("$t"); done

scan "zero-width"      "$ZW"      "${EXISTING[@]}"
scan "bidi-controls"   "$BIDI"    "${EXISTING[@]}"
scan "tag-characters"  "$TAGS"    "${EXISTING[@]}"
# Joiners ueberall ausser in den Locale-Baeumen pruefen
NONI18N=(); for t in "${EXISTING[@]}"; do [ "$t" = "frontend/i18n" ] || NONI18N+=("$t"); done
scan "joiners-outside-i18n" "$JOINERS" "${NONI18N[@]}"

if [ "$FAIL" -eq 0 ]; then
  echo "Unicode hygiene: clean (zero-width, bidi, tags, joiners-outside-i18n)"
else
  echo "Unicode hygiene: FINDINGS - siehe oben. Nichts wurde veraendert; Fundstellen manuell bereinigen."
fi
exit "$FAIL"
