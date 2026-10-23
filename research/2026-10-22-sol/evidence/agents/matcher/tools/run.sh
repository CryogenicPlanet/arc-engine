#!/usr/bin/env bash
set -euo pipefail
cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.."
if [[ $# != 1 ]]; then
  echo 'Usage: bash tools/run.sh NEW_OUTPUT_DIRECTORY' >&2
  exit 2
fi
MATCH_OUT=$1
mkdir -p -- "$MATCH_OUT"
if [[ -e "$MATCH_OUT/evidence.sqlite" ]]; then
  echo 'Use a fresh output directory; existing evidence will not be overwritten.' >&2
  exit 2
fi
flock /home/exedev/work/heavy.lock /usr/bin/time -v python3 -u tools/engine_match.py \
  --arc /home/exedev/work/inputs/binaries/ArcCore-1.170.0.arm64 \
  --stock /home/exedev/work/inputs/binaries/ChromiumFramework-156.arm64 \
  --ghidra-tsv /home/exedev/work/inputs/metadata/decomp/arccore-1.170.0-functions.tsv \
  --objc-dyld /home/exedev/work/inputs/context/analysis/data/dobjc170.txt \
  --out "$MATCH_OUT" --controls 384 --sample 512 --requests 40 \
  > "$MATCH_OUT/run.log" 2> "$MATCH_OUT/resource.log"
flock /home/exedev/work/heavy.lock /usr/bin/time -v python3 -u tools/probe_entries.py \
  --arc /home/exedev/work/inputs/binaries/ArcCore-1.170.0.arm64 \
  --stock /home/exedev/work/inputs/binaries/ChromiumFramework-156.arm64 \
  --objc-dyld /home/exedev/work/inputs/context/analysis/data/dobjc170.txt \
  --out "$MATCH_OUT" > "$MATCH_OUT/entry-run.log" 2> "$MATCH_OUT/entry-resource.log"
python3 tools/test_matcher.py --out "$MATCH_OUT" > "$MATCH_OUT/unit-tests.log" 2>&1
flock /home/exedev/work/heavy.lock python3 tools/finalize.py --out "$MATCH_OUT" \
  > "$MATCH_OUT/artifact-validation.log" 2>&1
