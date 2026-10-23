#!/bin/bash
set -euo pipefail
export PYTHONDONTWRITEBYTECODE=1
python3 scripts/collect.py
bash scripts/collect_bridges.sh
python3 scripts/review_notes.py
python3 scripts/specify.py
python3 scripts/app_embedding.py
python3 scripts/audit.py
python3 scripts/report.py
python3 scripts/final_validate.py
