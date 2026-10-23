#!/bin/bash
# Runs scenarios on the release-packaged agent app (ArcModAgent-rel156) and diffs against the goldens.
E=/Users/cryogenicplanet/general/arc-engine; cd "$E"
APP=/Users/cryogenicplanet/general/arc/build/ArcModAgent-rel156.app
for SC in "$@"; do
  rm -rf "$HOME/Library/Application Support/ArcModAgent-rel156" "results/release/$SC"
  SCARG=""; [ "$SC" != launch-slice ] && SCARG="--scenario $SC"
  bun tests/launch-slice/run.ts --app "$APP" --out "results/release/$SC" --record $SCARG > "results/release/$SC.log" 2>&1
  steps=$(grep -E 'all steps pass|step\(s\) not passing' "results/release/$SC.log" | tail -1)
  crash=$(cat results/release/$SC/launch-*/app.log 2>/dev/null | grep -c 'Received signal')
  diffs=""
  for g in results/golden/arc-1.170-$SC/launch-*/record.jsonl; do
    n=$(basename "$(dirname "$g")"); o="results/release/$SC/$n/record.jsonl"
    [ -f "$o" ] && diffs="$diffs $(python3 tests/recorder/diff.py "$g" "$o" | head -1 | grep -o 'missing [0-9]*, extra [0-9]*')"
  done
  echo "$SC: $steps; crashes=$crash;$diffs"
done
