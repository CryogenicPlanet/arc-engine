#!/bin/bash
# Syntax-check one arc/ source with the real build's compiler flags (no lock, no build graph changes).
# The file must be listed in arc/BUILD.gn and gn must have been regenerated once (any build does that).
#   arc/tools/check-file.sh arc/api/arc_browser.mm
set -euo pipefail
E="$(cd "$(dirname "$0")/../.." && pwd)"
rel="${1#arc/}"; obj="obj/arc/arc_core/$(basename "${rel%.*}").o"
cd "$E/chromium/src/out/Base"
cmd="$(PATH="$E/depot_tools:$PATH" siso query commands -C . "$obj" 2>/dev/null | tail -1)"
[ -n "$cmd" ] || { echo "no compile command for $obj (add the file to arc/BUILD.gn, then run a build once)"; exit 1; }
eval "$cmd -fsyntax-only -Wno-unused-command-line-argument" 2>&1 | grep -E 'error|warning' | head -40 || true
echo "check done: $1"
