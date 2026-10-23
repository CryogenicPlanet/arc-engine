#!/bin/bash
# Run a command while holding the ArcCore build/run lock (one out/Base and one test app on this machine).
#   arc/tools/locked.sh sh -c './build-babysit.sh && ./try-engine.sh 30'
LOCK="$(cd "$(dirname "$0")/../.." && pwd)/logs/arccore-build.lock"
mkdir -p "$(dirname "$LOCK")"
until mkdir "$LOCK" 2>/dev/null; do
  # Reclaim a lock whose owner died.
  owner="$(cat "$LOCK/pid" 2>/dev/null)"
  if [ -n "$owner" ] && ! kill -0 "$owner" 2>/dev/null; then rm -rf "$LOCK"; continue; fi
  sleep 5
done
echo $$ > "$LOCK/pid"
trap 'rm -rf "$LOCK"' EXIT
"$@"
