#!/usr/bin/env bash
# Phase C: full Chromium 156.0.8078.48 checkout (with macOS deps) for reading and comparing upstream code.
set -euo pipefail
W=~/work
step() { echo "$(date -u +%FT%TZ) $*" | tee -a "$W/STATUS"; }
[ -d "$W/depot_tools" ] || git clone -q https://chromium.googlesource.com/chromium/tools/depot_tools.git "$W/depot_tools"
export PATH="$W/depot_tools:$PATH" DEPOT_TOOLS_UPDATE=0
mkdir -p "$W/chromium/full" && cd "$W/chromium/full"
cat >.gclient <<'GCLIENT'
solutions = [{
  "name": "src",
  "url": "https://chromium.googlesource.com/chromium/src.git",
  "managed": False,
  "custom_deps": {},
  "custom_vars": {"checkout_mac": True},
}]
target_os = ["mac"]
GCLIENT
step "chromium: gclient sync 156.0.8078.48 (no history, no hooks)"
gclient sync --revision src@refs/tags/156.0.8078.48 --no-history --nohooks -j 16 >"$W/logs/gclient-sync.log" 2>&1 \
  && step "PHASE C DONE: chromium checkout $(du -sh "$W/chromium/full/src" | cut -f1)" \
  || step "PHASE C FAILED (logs/gclient-sync.log)"
