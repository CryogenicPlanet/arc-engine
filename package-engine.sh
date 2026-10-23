#!/bin/bash
# Put our ArcCore.framework (chromium/src/out/Base) into a copy of Arc 1.168.0 and build an ArcMod
# agent app from it.
#   ./package-engine.sh [agent-slug]   (default: ours156) -> ../arc/build/ArcModAgent-<slug>.app
set -euo pipefail
E="$(cd "$(dirname "$0")" && pwd)"
SLUG="${1:-ours156}"
STOCK="$E/swap/stock-1.168.0/Arc.app"
OUT="$E/swap/ours/Arc.app"
FRAMEWORK="$E/chromium/src/out/Base/Chromium.app/Contents/Frameworks/ArcCore.framework"
[ -d "$STOCK" ] || { echo "missing $STOCK (unpack ../arc/bin/Arc-1.168.0-88413.app.tar.xz there)"; exit 1; }
[ -d "$FRAMEWORK" ] || { echo "missing $FRAMEWORK (build first: ./build-babysit.sh)"; exit 1; }
rm -rf "$(dirname "$OUT")" && mkdir -p "$(dirname "$OUT")"
cp -cR "$STOCK" "$OUT"
rm -rf "$OUT/Contents/Frameworks/ArcCore.framework"
cp -cR "$FRAMEWORK" "$OUT/Contents/Frameworks/ArcCore.framework"
cd "$E/../arc" && bun tools/build-mod.ts --agent "$SLUG" --source "$OUT"
