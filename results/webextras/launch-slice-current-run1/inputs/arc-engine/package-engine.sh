#!/bin/bash
# Build an ArcMod agent app on our ArcCore (chromium/src/out/Base) through ArcMod's own builder:
# Arc 1.168.0's app layer + `build-mod.ts --engine <our ArcCore.framework>`.
#   ./package-engine.sh [agent-slug]   (default: ours156) -> ../arc/build/ArcModAgent-<slug>.app
set -euo pipefail
E="$(cd "$(dirname "$0")" && pwd)"
SLUG="${1:-ours156}"
STOCK="$E/swap/stock-1.168.0/Arc.app"
FRAMEWORK="$E/chromium/src/out/Base/Chromium.app/Contents/Frameworks/ArcCore.framework"
[ -d "$STOCK" ] || { echo "missing $STOCK (unpack ../arc/bin/Arc-1.168.0-88413.app.tar.xz there)"; exit 1; }
[ -d "$FRAMEWORK" ] || { echo "missing $FRAMEWORK (build first: ./build-babysit.sh)"; exit 1; }
cd "$E/../arc" && bun tools/build-mod.ts --agent "$SLUG" --source "$STOCK" --engine "$FRAMEWORK"
