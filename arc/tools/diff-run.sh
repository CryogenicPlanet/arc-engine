#!/bin/bash
# Under the build lock: regenerate the stub table, build, package our engine, run a scenario on it with the
# boundary recorder, and diff each launch against the golden transcript from Arc's engine.
#   arc/tools/diff-run.sh [scenario=launch-slice]
# Output: results/ours/<scenario>/ (+ diff-launch-N.txt). Golden: results/golden/arc-1.170-<scenario>/.
set -uo pipefail
E="$(cd "$(dirname "$0")/../.." && pwd)"
SC="${1:-launch-slice}"
exec "$E/arc/tools/locked.sh" bash -c '
  set -uo pipefail
  E="'"$E"'"; SC="'"$SC"'"
  cd "$E"
  python3 arc/tools/gen_api_table.py
  ./build-babysit.sh > logs/build-arccore.log 2>&1 || { tail -3 logs/build-arccore.log; grep -h "error:" logs/build-base.attempt-*.log | sort -u | head -20; exit 1; }
  ./package-engine.sh ours156 > /dev/null 2>&1 || { echo "package failed"; exit 1; }
  rm -rf "$HOME/Library/Application Support/ArcModAgent-ours156" "results/ours/$SC"
  SCARG=""; [ "$SC" != launch-slice ] && SCARG="--scenario $SC"
  bun tests/launch-slice/run.ts --app "$E/../arc/build/ArcModAgent-ours156.app" --out "results/ours/$SC" --record $SCARG 2>&1 | tail -12
  for g in results/golden/arc-1.170-$SC/launch-*/record.jsonl; do
    n=$(basename "$(dirname "$g")")
    o="results/ours/$SC/$n/record.jsonl"
    [ -f "$o" ] || { echo "$n: no transcript from our engine"; continue; }
    python3 tests/recorder/diff.py "$g" "$o" --json "results/ours/$SC/diff-$n.json" > "results/ours/$SC/diff-$n.txt"
    echo "== $n: $(head -2 "results/ours/$SC/diff-$n.txt" | tr "\n" " ")"
  done
'
