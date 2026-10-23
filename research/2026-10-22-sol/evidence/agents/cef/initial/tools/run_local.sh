#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/../.."
export PYTHONDONTWRITEBYTECODE=1
flock /home/exedev/work/heavy.lock python3 initial/tools/inventory.py
flock /home/exedev/work/heavy.lock python3 initial/tools/app_calls.py
python3 initial/tools/decompile_requests.py
flock /home/exedev/work/heavy.lock python3 initial/tools/extract_arrived.py
python3 initial/tools/map_cef.py
python3 initial/tools/validate.py
