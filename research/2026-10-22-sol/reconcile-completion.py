#!/usr/bin/env python3
"""Research-VM recovery for original wrappers broken by an in-place harness update.

Never reruns research or assumes process absence means success. Only reconciles
the three original jobs after final CLI success plus fresh report/output proof.
"""
import hashlib
import json
import os
from pathlib import Path
import time

root=Path('/home/exedev/work')
for role in ('abi','cef','arc-layer'):
 job=role+'-initial'
 if (root/(job+'.done')).exists():continue
 pidfile=root/(job+'.pid')
 if not pidfile.exists():continue
 try:
  os.kill(int(pidfile.read_text()),0)
 except ProcessLookupError:pass
 else:
  print(job,'still alive; no reconciliation');continue
 launch=root/'logs'/('launch-'+job+'.log')
 if 'unexpected EOF' not in launch.read_text():
  print(job,'unrecognized wrapper outcome; investigate');continue
 log=root/'logs'/(job+'.jsonl')
 events=[json.loads(line) for line in log.open() if line.strip().startswith('{')]
 assert events[-1]['type']=='turn.completed', (job,'CLI did not complete')
 report=root/'agents'/role/'initial/report.md'
 final=root/'out'/(job+'.md')
 for path in (report,final):
  assert path.stat().st_size>0 and path.stat().st_mtime>pidfile.stat().st_mtime, path
 proof={'job':job,'cli_final_event':events[-1],'wrapper_error':launch.read_text(),
        'report_sha256':hashlib.sha256(report.read_bytes()).hexdigest(),
        'final_sha256':hashlib.sha256(final.read_bytes()).hexdigest(),
        'basis':'CLI completed and final/report written after job start, wrapper PID gone; bookkeeping failure reconciled without rerun'}
 (root/(job+'.completion-recovery.json')).write_text(json.dumps(proof,indent=2)+'\n')
 (root/(job+'.done')).touch()
 with (root/'STATUS').open('a') as f:
  f.write(time.strftime('%Y-%m-%dT%H:%M:%SZ',time.gmtime())+' JOB '+job+' DONE (reconciled; see '+job+'.completion-recovery.json)\n')
 print(job,'verified completed; recovery proof recorded')
