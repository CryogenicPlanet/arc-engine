#!/usr/bin/env bash
# Invoke under flock /home/exedev/work/heavy.lock; sequential stdlib-only work.
set -euo pipefail
cd /home/exedev/work/agents/spec-3
mkdir -p specs/sources/cef specs/classes specs/evidence specs/bodies
if [[ ! -f specs/sources/cef/cef_types_mac.h ]]; then
  curl -fsSL https://raw.githubusercontent.com/chromiumembedded/cef/426920c33ef22551e0b8f48cf343d623ac25642d/include/internal/cef_types_mac.h -o specs/sources/cef/cef_types_mac.h
fi
if [[ ! -f specs/sources/cef/cef_browser.h ]]; then
  curl -fsSL https://raw.githubusercontent.com/chromiumembedded/cef/426920c33ef22551e0b8f48cf343d623ac25642d/include/cef_browser.h -o specs/sources/cef/cef_browser.h
fi
python3 tools/prepare.py
python3 tools/check_raw_dump.py
python3 tools/bridges.py
python3 tools/extra_evidence.py
python3 tools/inspect_leaves.py
python3 /home/exedev/work/read-function.py 0x1c391bc 0x1d9fb90 0x1b356b8 0x43abd34 0x43abdf8 > specs/evidence/string-url-converters.txt
python3 tools/summarize.py > specs/evidence/body-summary.txt
python3 tools/build_specs.py > specs/evidence/generation-summary.json
python3 tools/source_evidence.py
python3 tools/validate.py
