#!/usr/bin/env bash
set -euo pipefail
cd /home/exedev/work/agents/spec-2
export PYTHONDONTWRITEBYTECODE=1
python3 tools/collect.py
python3 tools/otool_check.py
python3 tools/bridges.py
python3 tools/provenance.py
python3 tools/render.py
python3 tools/supplement.py
python3 tools/validate.py
python3 tools/write_report.py
