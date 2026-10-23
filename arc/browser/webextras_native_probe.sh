#!/bin/bash
# Build only: no shared Chromium build or app launch. Run must already hold the shared lock.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
BUILD="$ROOT/logs/wp/webextras-native-probe-build"
case "${1:-}" in
  build)
    mkdir -p "$BUILD"
    xcrun clang -dynamiclib -fobjc-arc -Wall -Wextra -Werror -framework Foundation -framework AppKit \
      "$ROOT/arc/browser/webextras_native_probe.m" -o "$BUILD/WebextrasNativeProbe.dylib"
    bun build "$ROOT/arc/browser/webextras_native_probe.ts" --target=bun --outdir "$BUILD" >/dev/null
    echo "$BUILD/WebextrasNativeProbe.dylib"
    ;;
  run)
    shift
    exec bun "$ROOT/arc/browser/webextras_native_probe.ts" "$@"
    ;;
  *) echo "usage: $0 build | run --out DIR [--agents trace156,ours156] [--port 9458]" >&2; exit 2 ;;
esac
