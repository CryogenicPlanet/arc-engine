#!/usr/bin/env bash
set -euo pipefail
cd /home/exedev/work/agents/cef
flock /home/exedev/work/heavy.lock python3 decompile/tools/analyze.py
flock /home/exedev/work/heavy.lock python3 decompile/tools/app_evidence.py
python3 decompile/tools/source_evidence.py
python3 decompile/tools/mappings.py
python3 decompile/tools/validate.py
