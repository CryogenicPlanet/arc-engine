#!/bin/bash
# Run from this agent directory, serialized by the caller's heavy.lock.
set -eu
/usr/bin/time -v python3 scripts/inventory.py > initial/inventory-run.log 2> initial/inventory-resource.log
/usr/bin/time -v python3 scripts/export_inventory.py > initial/export-run.log 2> initial/export-resource.log
/usr/bin/time -v python3 scripts/trace_strings.py > initial/trace-run.log 2> initial/trace-resource.log
/usr/bin/time -v python3 scripts/app_launch_evidence.py > initial/app-launch-run.log 2> initial/app-launch-resource.log
/usr/bin/time -v python3 scripts/validate.py > initial/validation-run.log 2> initial/validation-resource.log
cat initial/validation-run.log
python3 scripts/write_report.py
