#!/bin/bash
set -euo pipefail
cd /home/exedev/work/agents/spec-3
# Bounded reads: 505 indexed bodies plus selected ranges, a 3.5 MiB dump,
# streaming function metadata. Under 40 MiB RSS; no full-corpus scan/build.
python3 classreview/tools/review_checks.py
python3 classreview/tools/write_review.py
python3 classreview/tools/validate_review.py
