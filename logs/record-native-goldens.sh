#!/bin/bash
# Records the native-UI parity goldens on Arc's engine (trace156), one scenario at a time.
cd /Users/cryogenicplanet/general/arc-engine || exit 1
mkdir -p logs/native-goldens
for SC in "$@"; do
  bun tests/scenarios/record-golden.ts "$SC" > "logs/native-goldens/$SC.log" 2>&1
  echo "$SC: exit=$? $(grep -E 'all steps pass|step\(s\) not passing|accepted golden|not accepted' "logs/native-goldens/$SC.log" | tail -2 | tr '\n' ' ')"
done
