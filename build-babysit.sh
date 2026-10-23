#!/usr/bin/env bash
# Builds out/Base (Chromium.app) with preflight checks and automatic recovery from known failures.
# Every retry is incremental. Writes STATUS lines ending in "BUILD DONE" or "BUILD FAILED: <reason>".
set -uo pipefail
E="$(cd "$(dirname "$0")" && pwd)"
SRC="$E/chromium/src"
# Optional: build with a specific Xcode (e.g. /Applications/Xcode-26.5.app) without changing the system default.
[ -f "$E/xcode.env" ] && . "$E/xcode.env"
[ -n "${ARC_ENGINE_XCODE:-}" ] && export DEVELOPER_DIR="$ARC_ENGINE_XCODE/Contents/Developer"
export PATH="$E/depot_tools:$PATH"
step() { echo "$(date -u +%FT%TZ) $*" | tee -a "$E/STATUS"; }

preflight() {
  xcode-select -p | grep -q '/Xcode.app/' || { step "preflight: full Xcode is not selected"; return 1; }
  if ! xcrun -f metal >/dev/null 2>&1; then
    step "preflight: installing Xcode Metal toolchain"
    xcodebuild -downloadComponent MetalToolchain >>"$E/logs/preflight.log" 2>&1
    xcrun -f metal >/dev/null 2>&1 || { step "preflight: Metal toolchain install failed"; return 1; }
  fi
  xcrun --show-sdk-path >/dev/null 2>&1 || { step "preflight: macOS SDK not found"; return 1; }
  local need have; need=$(sed -n "s/^ *mac_sdk_official_version = \"\(.*\)\"/\1/p" "$SRC/build/config/mac/mac_sdk.gni"); have=$(xcrun --show-sdk-version)
  # patches/sdk-compat-min lowers the floor when the compat patches for an older SDK are applied.
  local floor; floor=$need; [ -f "$E/patches/sdk-compat-min" ] && floor=$(cat "$E/patches/sdk-compat-min")
  [ "$(printf "%s\n%s\n" "$floor" "$have" | sort -V | head -1)" = "$floor" ] || { step "preflight: macOS SDK $have is older than Chromium's $need (Xcode at $(xcode-select -p 2>/dev/null)${DEVELOPER_DIR:+, DEVELOPER_DIR=$DEVELOPER_DIR}); install a newer Xcode"; return 1; }
  [ "$have" = "$need" ] || [ "$(printf "%s\n%s\n" "$need" "$have" | sort -V | head -1)" = "$need" ] || step "preflight: building with SDK $have (Chromium targets $need; compat patches applied)"
  local free_gb; free_gb=$(df -g "$E" | awk 'NR==2 {print $4}')
  [ "$free_gb" -ge 80 ] || { step "preflight: only ${free_gb} GB free (need 80)"; return 1; }
  ulimit -n "$(ulimit -Hn)" 2>/dev/null || ulimit -n 10240
  # Always regenerate: picks up a changed Xcode/SDK (xcode.env) and is cheap when nothing changed.
  (cd "$SRC" && gn gen out/Base >>"$E/logs/gn-gen.log" 2>&1) || { step "preflight: gn gen failed"; return 1; }
}

jobs=14 unknown_retries=0
for attempt in 1 2 3 4 5 6; do
  preflight || { step "BUILD FAILED: preflight"; exit 1; }
  log="$E/logs/build-base.attempt-$attempt.log"
  step "build attempt $attempt: autoninja -j $jobs chrome (nice 10)"
  start=$(date +%s)
  if (cd "$SRC" && nice -n 10 autoninja -C out/Base -j "$jobs" chrome >"$log" 2>&1); then
    step "BUILD DONE (attempt $attempt, $(( ($(date +%s) - start) / 60 )) min): $(du -sh "$SRC/out/Base/Chromium.app" | cut -f1) Chromium.app"
    exit 0
  fi
  tail_text=$(tail -200 "$log")
  if grep -q "missing Metal Toolchain" <<<"$tail_text"; then
    step "attempt $attempt: Metal toolchain missing; reinstalling and resuming"
  elif grep -q -E "No space left on device" <<<"$tail_text"; then
    step "BUILD FAILED: disk full (see $log)"; exit 1
  elif grep -q -E "signal: killed|Killed: 9|exit=137|out of memory|Cannot allocate memory|LLVM ERROR: out of memory" <<<"$tail_text"; then
    jobs=$(( jobs * 2 / 3 )); [ "$jobs" -ge 4 ] || jobs=4
    step "attempt $attempt: memory pressure; resuming with -j $jobs"
  elif grep -q -E "Too many open files" <<<"$tail_text"; then
    step "attempt $attempt: too many open files; raised limit, resuming"
  elif [ "$unknown_retries" -lt 1 ]; then
    unknown_retries=$(( unknown_retries + 1 ))
    step "attempt $attempt: unrecognized failure; retrying once (see $log)"
  else
    step "BUILD FAILED: repeated unrecognized failure (see $log)"; exit 1
  fi
done
step "BUILD FAILED: retries exhausted"; exit 1
