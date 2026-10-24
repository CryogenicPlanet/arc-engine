#!/bin/bash
# Build an ArcMod agent app on our ArcCore (chromium/src/out/Base) through ArcMod's own builder:
# Arc 1.168.0's app layer + `build-mod.ts --engine <our ArcCore.framework>`.
#   ./package-engine.sh [agent-slug]   (default: ours156) -> ../arc/build/ArcModAgent-<slug>.app
#   ARC_ENGINE_OUT=out/Release ./package-engine.sh rel156   (the release engine instead of out/Base)
set -euo pipefail
E="$(cd "$(dirname "$0")" && pwd)"
SLUG="${1:-ours156}"
# ArcMod is the parent repo when this is its engine/ submodule, else a sibling checkout (../arc).
ARCMOD="$E/../arc"
[ -f "$E/../tools/build-mod.ts" ] && ARCMOD="$E/.."
STOCK="$E/swap/stock-1.168.0/Arc.app"
FRAMEWORK="$E/chromium/src/${ARC_ENGINE_OUT:-out/Base}/Chromium.app/Contents/Frameworks/ArcCore.framework"
[ -d "$STOCK" ] || { echo "missing $STOCK (unpack ArcMod's bin/Arc-1.168.0-88413.app.tar.xz there)"; exit 1; }
[ -d "$FRAMEWORK" ] || { echo "missing $FRAMEWORK (build first: ./build-babysit.sh)"; exit 1; }
cd "$ARCMOD" && bun tools/build-mod.ts --agent "$SLUG" --source "$STOCK" --engine "$FRAMEWORK"
