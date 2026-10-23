#!/bin/bash
# Syntax-check one arc/ source with the real build's compiler flags (no lock, no build graph changes).
# A file already in arc/BUILD.gn uses its own compile command; a new file borrows the command of a sibling
# (arc/api/arc_web_contents.mm for .mm, arc/browser/arc_browser_window.mm otherwise), so new work can be
# checked before it joins the build. gn must have been generated once (any build does that).
#   arc/tools/check-file.sh arc/api/arc_browser.mm
set -euo pipefail
E="$(cd "$(dirname "$0")/../.." && pwd)"
rel="${1#arc/}"; obj="obj/arc/arc_core/$(basename "${rel%.*}").o"
cd "$E/chromium/src/out/Base"
query() { PATH="$E/depot_tools:$PATH" siso query commands -C . "$1" 2>/dev/null | tail -1 || true; }
cmd="$(query "$obj")"
if [ -z "$cmd" ]; then
  donor=arc_web_contents; donor_src=../../arc/api/arc_web_contents.mm
  case "$1" in *.cc) donor=arc_browser_window; donor_src=../../arc/browser/arc_browser_window.mm ;; esac
  cmd="$(query "obj/arc/arc_core/$donor.o")"
  [ -n "$cmd" ] || { echo "no compile command (run a build once)"; exit 1; }
  lang=""; case "$1" in *.cc) lang="-x c++ -fno-objc-arc" ;; esac
  cmd="${cmd//$donor_src/../../$1} $lang"
  # Don't overwrite the donor's dependency file.
  cmd="$(printf '%s' "$cmd" | sed -E 's/ -MD / /; s/ -MF [^ ]+ / /')"
fi
eval "$cmd -fsyntax-only -Wno-unused-command-line-argument" 2>&1 | grep -E 'error|warning' | head -40 || true
echo "check done: $1"
