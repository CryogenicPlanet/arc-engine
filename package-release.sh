#!/bin/bash
# Packages the release engine for a GitHub release: chromium/src/out/Release's ArcCore.framework with local
# symbols stripped (exports unchanged), plus Chromium's license and third-party credits.
#   ./package-release.sh v0.1.0   -> dist/ArcCore-<chromium>-arm64.zip and its .sha256
set -euo pipefail
E="$(cd "$(dirname "$0")" && pwd)"
TAG="${1:?usage: ./package-release.sh <tag>}"
SRC="$E/chromium/src"
FRAMEWORK="$SRC/out/Release/Chromium.app/Contents/Frameworks/ArcCore.framework"
CHROMIUM="$(sed -n 's/^MAJOR=//p;s/^MINOR=//p;s/^BUILD=//p;s/^PATCH=//p' "$SRC/chrome/VERSION" | paste -sd. -)"
[ -d "$FRAMEWORK" ] || { echo "missing $FRAMEWORK (build out/Release first)"; exit 1; }
[ -z "$(git -C "$E" status --porcelain --untracked-files=no)" ] || { echo "commit your changes first: the release must match a commit"; exit 1; }

STAGE="$E/dist/stage-$TAG"
ZIP="$E/dist/ArcCore-$CHROMIUM-arm64.zip"
rm -rf "$STAGE" "$ZIP" "$ZIP.sha256"
mkdir -p "$STAGE"
cp -R "$FRAMEWORK" "$STAGE/ArcCore.framework"
BINARY="$STAGE/ArcCore.framework/Versions/A/ArcCore"
before="$(nm -gU -arch arm64 "$BINARY" | awk '{print $3}' | sort | shasum -a 256)"
strip -x "$BINARY"
after="$(nm -gU -arch arm64 "$BINARY" | awk '{print $3}' | sort | shasum -a 256)"
[ "$before" = "$after" ] || { echo "strip changed the exported symbols"; exit 1; }
cp "$SRC/LICENSE" "$STAGE/LICENSE.chromium"
cp "$SRC/out/Release/gen/components/resources/about_credits.html" "$STAGE/CREDITS.html"
cat > "$STAGE/README.txt" <<EOF
Arc Engine $TAG: Arc's ArcCore.framework rebuilt from Chromium $CHROMIUM (arm64).
Source: https://github.com/CryogenicPlanet/arc-engine/tree/$TAG (commit $(git -C "$E" rev-parse HEAD))
Use it with ArcMod: bun tools/build-mod.ts --engine release
Chromium's license is in LICENSE.chromium and third-party notices are in CREDITS.html.
EOF
(cd "$STAGE" && ditto -c -k --sequesterRsrc . "$ZIP")
(cd "$E/dist" && shasum -a 256 "$(basename "$ZIP")" > "$(basename "$ZIP").sha256")
rm -rf "$STAGE"
du -h "$ZIP"
cat "$ZIP.sha256"
