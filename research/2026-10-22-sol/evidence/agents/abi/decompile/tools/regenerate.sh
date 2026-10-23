#!/bin/bash
set -eu
cd /home/exedev/work/agents/abi
python3 decompile/tools/research.py snapshot audit extract binary
python3 decompile/tools/details.py
python3 decompile/tools/unwind.py
python3 decompile/tools/findings.py
python3 decompile/tools/validate.py
