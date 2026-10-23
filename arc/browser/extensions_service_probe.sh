#!/bin/bash
# Build never acquires the shared lock. Run requires a parent-owned lock.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
BUILD="$ROOT/logs/wp/extensions-service-probe-build"
case "${1:-}" in
  build)
    mkdir -p "$BUILD"
    xcrun clang -dynamiclib -fobjc-arc -Wall -Wextra -Werror -framework AppKit \
      "$ROOT/arc/browser/extensions_service_probe.m" -o "$BUILD/ExtensionsServiceProbe.dylib"
    codesign --force --sign - "$BUILD/ExtensionsServiceProbe.dylib"
    bun build "$ROOT/arc/browser/extensions_service_probe.ts" --target=bun --outdir "$BUILD" >/dev/null
    echo "$BUILD/ExtensionsServiceProbe.dylib"
    ;;
  run)
    shift
    exec bun "$ROOT/arc/browser/extensions_service_probe.ts" "$@"
    ;;
  *) echo "usage: $0 build | run --out DIR [--agents trace156,ours156] [--port 9458]" >&2; exit 2 ;;
esac
