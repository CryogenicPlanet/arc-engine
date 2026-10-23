#!/bin/bash
set -euo pipefail
cd /home/exedev/work/agents/spec-1
# Bounded lock acquisition prevents an unrelated long job from causing an indefinite wait.
flock -w 60 /home/exedev/work/heavy.lock /usr/bin/time -v python3 classreview/tools/check.py > classreview/validation-run.log 2> classreview/validation-resource.log
python3 classreview/tools/raw_check.py > classreview/raw-run.log
python3 classreview/tools/source_excerpts.py > classreview/source-run.log
python3 classreview/tools/write_review.py > classreview/review-run.log
python3 classreview/tools/validate_review.py
