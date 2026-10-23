#!/bin/bash
set -euo pipefail
cd /home/exedev/work/agents/protocols
export PYTHONDONTWRITEBYTECODE=1
python3 initial/tools/audit.py
python3 initial/tools/sends.py
python3 initial/tools/app_bodies.py
python3 initial/tools/focus.py
python3 initial/tools/compose.py
python3 initial/tools/requests.py
python3 initial/tools/validate.py
