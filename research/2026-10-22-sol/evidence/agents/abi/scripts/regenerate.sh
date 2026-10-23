#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
mkdir -p initial
# No dependency installation. Each stage uses one Python process and one CPU.
# Lock acquisition is bounded so a busy sibling cannot stall us indefinitely.
for stage in inventory calls boundaries requests engine_evidence validate; do
  flock -w 60 /home/exedev/work/heavy.lock /usr/bin/time -v python3 "scripts/$stage.py" > "initial/$stage-run.log" 2>&1
done
