#!/bin/bash
# Real-profile smoke test on the release engine, on a disposable APFS clone of ArcMod's data.
# The original ~/Library/Application Support/ArcMod is only read. The copy is deleted at the end.
set -u
SC="${1:-real-profile-smoke}"
E=/Users/cryogenicplanet/general/arc-engine
SRC="$HOME/Library/Application Support/ArcMod"
DST="$HOME/Library/Application Support/ArcModAgent-realcopy"
cleanup() {
  rm -rf "$DST"
  defaults delete company.thebrowser.Browser.mod.agent.realcopy >/dev/null 2>&1 || true
  echo "copy deleted: $([ -e "$DST" ] && echo NO || echo yes)"
}
trap cleanup EXIT
if pgrep -f '/ArcMod.app/Contents/MacOS/Arc( |$)' >/dev/null; then echo "ArcMod is running; not copying"; exit 1; fi
cd "$E" || exit 1
ARC_ENGINE_OUT=out/Release ./package-engine.sh realcopy > logs/realcopy-package.log 2>&1 || { echo "package failed"; exit 1; }
rm -rf "$DST"
cp -cR "$SRC" "$DST" || { echo "copy failed"; exit 1; }
mv "$DST/User Data" "$DST/Agent Chromium Data" || { echo "rename failed"; exit 1; }
rm -rf results/realcopy
bun tests/launch-slice/run.ts --app /Users/cryogenicplanet/general/arc/build/ArcModAgent-realcopy.app \
  --out results/realcopy --scenario "$SC" > results/realcopy.log 2>&1
tail -12 results/realcopy.log
echo "crashes: $(cat results/realcopy/launch-*/app.log 2>/dev/null | grep -c 'Received signal')"
grep -h -E 'FATAL|Check failed' results/realcopy/launch-*/app.log 2>/dev/null | head -5
# The run's app logs can mention visited URLs; keep only the summary.
rm -f results/realcopy/launch-*/record.jsonl
