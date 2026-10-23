#!/usr/bin/env bash
# Points the engine build at a newer, side-by-side Xcode whose macOS SDK satisfies Chromium, without
# touching the system default. Prints NEEDS-USER lines for steps that need sudo. Exit 0 = ready to build.
set -uo pipefail
E="$(cd "$(dirname "$0")" && pwd)"
SRC="$E/chromium/src"
need=$(sed -n 's/^ *mac_sdk_official_version = "\(.*\)"/\1/p' "$SRC/build/config/mac/mac_sdk.gni")
pick=""
for app in /Applications/Xcode*.app; do
  [ -d "$app/Contents/Developer" ] || continue
  have=$(DEVELOPER_DIR="$app/Contents/Developer" xcrun --show-sdk-version 2>/dev/null) || continue
  [ "$(printf '%s\n%s\n' "$need" "$have" | sort -V | head -1)" = "$need" ] || continue
  case "$have" in 26.*) pick="$app"; break ;; esac   # stay on the 26.x SDK line Chromium 156 targets
done
[ -n "$pick" ] || { echo "no Xcode with a 26.x SDK >= $need yet"; exit 2; }
export DEVELOPER_DIR="$pick/Contents/Developer"
echo "using $pick (SDK $(xcrun --show-sdk-version), $(xcodebuild -version | tr '\n' ' '))"
xcodebuild -license check >/dev/null 2>&1 || { echo "NEEDS-USER: sudo DEVELOPER_DIR=$DEVELOPER_DIR xcodebuild -license accept"; exit 3; }
xcodebuild -checkFirstLaunchStatus >/dev/null 2>&1 || { echo "NEEDS-USER: sudo DEVELOPER_DIR=$DEVELOPER_DIR xcodebuild -runFirstLaunch"; exit 3; }
xcrun -f metal >/dev/null 2>&1 || xcodebuild -downloadComponent MetalToolchain >>"$E/logs/preflight.log" 2>&1
xcrun -f metal >/dev/null 2>&1 || { echo "Metal toolchain install failed for $pick"; exit 4; }
printf 'ARC_ENGINE_XCODE=%s\n' "$pick" >"$E/xcode.env"
echo "ready: wrote xcode.env"
