#!/bin/bash
set -euo pipefail
cd /home/exedev/work/agents/quality
/usr/bin/time -v -o initial/export-resources.txt flock /home/exedev/work/heavy.lock python3 tools/audit_export.py > initial/audit-run.log 2>&1
flock /home/exedev/work/heavy.lock python3 tools/audit_inventory.py > initial/inventory-run.log 2>&1
flock /home/exedev/work/heavy.lock python3 tools/triage_aperitif.py > initial/aperitif-run.log 2>&1
flock /home/exedev/work/heavy.lock python3 tools/finish_audit.py > initial/finish-run.log 2>&1
flock /home/exedev/work/heavy.lock python3 tools/validate.py > initial/validation-run.log 2>&1
