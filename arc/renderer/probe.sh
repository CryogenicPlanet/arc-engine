#!/bin/bash
# Build does not take the shared lock or launch apps. Run requires an already-held lock.
# arc/renderer/probe.sh build
# arc/tools/locked.sh arc/renderer/probe.sh run --out results/renderer-direct-parity
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
BUILD="$ROOT/logs/wp/renderer-probe-build"
case "${1:-}" in
  build)
    mkdir -p "$BUILD"
    xcrun clang -dynamiclib -fobjc-arc -Wall -Wextra -Werror -framework Foundation \
      "$ROOT/arc/renderer/probe.m" -o "$BUILD/RendererProbe.dylib"
    bun build "$ROOT/arc/renderer/probe.ts" --target=bun --outdir "$BUILD" >/dev/null
    echo "$BUILD/RendererProbe.dylib"
    ;;
  run)
    shift
    exec bun "$ROOT/arc/renderer/probe.ts" "$@"
    ;;
  *) echo "usage: $0 build | run --out DIR [--agents trace156,ours156] [--port 9447]" >&2; exit 2 ;;
esac
