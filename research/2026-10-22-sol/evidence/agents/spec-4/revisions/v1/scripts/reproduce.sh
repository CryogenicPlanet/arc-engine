#!/bin/bash
set -euo pipefail
cd /home/exedev/work/agents/spec-4
python3 specs/scripts/collect.py
python3 specs/scripts/check_otool.py
python3 - <<'PY'
import json,subprocess
p='specs/evidence/bridges.json'
subprocess.run(['python3','specs/scripts/bridges.py',*json.load(open(p)).keys()],check=True,stdout=open('specs/evidence/bridge-reproduction.txt','w'))
PY
python3 specs/scripts/audit_closure.py
python3 /home/exedev/work/read-function.py 0x4402480 0x42a43ac --max-bytes 4096 > specs/evidence/helper-delta-inspection.txt
python3 specs/scripts/class_notes.py
python3 specs/scripts/build_specs.py
python3 specs/scripts/validate.py
