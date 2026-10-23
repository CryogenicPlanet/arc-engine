#!/bin/bash
set -euo pipefail
cd /home/exedev/work/agents/arc-layer
/usr/bin/time -v python3 decompile/tools/audit_closure.py > decompile/audit-run.log 2> decompile/audit-resources.log
/usr/bin/time -v python3 decompile/tools/diagnostics.py > decompile/diagnostics-run.log 2> decompile/diagnostics-resources.log
/usr/bin/time -v python3 decompile/tools/collect_evidence.py > decompile/collect-run.log 2> decompile/collect-resources.log
/usr/bin/time -v python3 decompile/tools/validate.py > decompile/validation-run.log 2> decompile/validation-resources.log
