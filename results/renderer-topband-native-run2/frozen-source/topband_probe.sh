#!/bin/bash
# Compile only without the shared lock; runtime requires an already-held lock.
set -euo pipefail
TOPBAND_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
TOPBAND_BUILD="$TOPBAND_ROOT/logs/wp/renderer-topband-probe-build"
case "${1:-}" in
  build)
    mkdir -p "$TOPBAND_BUILD"
    xcrun clang -dynamiclib -fobjc-arc -Wall -Wextra -Werror -framework Foundation \
      "$TOPBAND_ROOT/arc/renderer/topband_probe.m" -o "$TOPBAND_BUILD/TopBandProbe.dylib"
    bun build "$TOPBAND_ROOT/arc/renderer/topband_probe.ts" --target=bun --outdir "$TOPBAND_BUILD" >/dev/null
    echo "$TOPBAND_BUILD/TopBandProbe.dylib"
    ;;
  run)
    shift
    exec bun "$TOPBAND_ROOT/arc/renderer/topband_probe.ts" "$@"
    ;;
  *) echo "usage: $0 build | run --out NEW_DIR [--agents trace156] [--mode default|base|arc|both] [--live36-toggle]" >&2; exit 2 ;;
esac
