#!/usr/bin/env bash
# Checks out Google's Chromium 156.0.8078.48 (Arc 1.170.0's engine version) with macOS deps and hooks.
set -euo pipefail
E="$(cd "$(dirname "$0")" && pwd)"
step() { echo "$(date -u +%FT%TZ) $*" | tee -a "$E/STATUS"; }
[ -d "$E/depot_tools" ] || git clone -q https://chromium.googlesource.com/chromium/tools/depot_tools.git "$E/depot_tools"
export PATH="$E/depot_tools:$PATH"
mkdir -p "$E/chromium" && cd "$E/chromium"
cat >.gclient <<'GCLIENT'
solutions = [{
  "name": "src",
  "url": "https://chromium.googlesource.com/chromium/src.git",
  "managed": False,
  "custom_deps": {},
  "custom_vars": {},
}]
GCLIENT
step "gclient sync src@156.0.8078.48 (no history, with hooks)"
gclient sync --revision src@refs/tags/156.0.8078.48 --no-history -D -j 16 >"$E/logs/gclient-sync.log" 2>&1 \
  && step "SYNC DONE: $(du -sh "$E/chromium/src" | cut -f1), $(cat "$E/chromium/src/chrome/VERSION" | tr '\n' ' ')" \
  || { step "SYNC FAILED (logs/gclient-sync.log)"; exit 1; }
# Reapply our patches (a sync resets the tree). Already-applied patches are skipped.
for p in "$E"/patches/*.patch; do
  [ -e "$p" ] || continue
  if git -C "$E/chromium/src" apply --reverse --check "$p" 2>/dev/null; then step "patch already applied: $(basename "$p")"
  elif git -C "$E/chromium/src" apply "$p"; then step "patch applied: $(basename "$p")"
  else step "PATCH FAILED: $(basename "$p")"; exit 1; fi
done
