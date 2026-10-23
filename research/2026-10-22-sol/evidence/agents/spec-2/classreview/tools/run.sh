#!/bin/bash
set -eu
export PYTHONDONTWRITEBYTECODE=1
flock -w 300 /home/exedev/work/heavy.lock /usr/bin/time -v python3 classreview/tools/supplement.py > classreview/evidence/validation-run.log 2> classreview/evidence/validation-resources.log
python3 classreview/tools/review_notes.py >> classreview/evidence/validation-run.log
python3 classreview/tools/validate.py >> classreview/evidence/validation-run.log
