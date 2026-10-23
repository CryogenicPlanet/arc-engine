#!/usr/bin/env python3
"""Native exe.dev push notifications for aggregate milestones and failed jobs.

One private journal per event prevents duplicate attempts. HTTP acceptance is
recorded separately from delivery; uncertain outcomes are not retried blindly.
"""
import argparse
import fcntl
import json
import os
from pathlib import Path
import re
import subprocess
import time

root=Path('/home/exedev/work')
journal=root/'notifications'
journal.mkdir(mode=0o700,exist_ok=True)

def notify(event,title,body):
    path=journal/(event+'.json')
    try: fd=os.open(path,os.O_WRONLY|os.O_CREAT|os.O_EXCL,0o600)
    except FileExistsError:return
    record={'event':event,'title':title,'body':body,'state':'attempting','at':time.time()}
    with os.fdopen(fd,'w') as f:json.dump(record,f)
    try:
        response=subprocess.run(['curl','-sS','--max-time','20','-w','\n%{http_code}',
            '-X','POST','-H','Content-Type: application/json','--data-binary','@-',
            'https://notify.int.exe.xyz/'],input=json.dumps({'title':title,'body':body}),
            text=True,capture_output=True)
        text,status=response.stdout.rsplit('\n',1)
        record.update(http_status=int(status),response=text[:3000],stderr=response.stderr[:1000])
        record['state']='accepted' if response.returncode==0 and 200<=int(status)<300 else 'failed_or_uncertain'
    except Exception as error:
        record.update(state='failed_or_uncertain',error=str(error))
    temporary=path.with_suffix('.tmp')
    temporary.write_text(json.dumps(record,indent=2)+'\n');os.chmod(temporary,0o600);temporary.replace(path)
    print(event,record['state'],flush=True)

def watch():
    with (root/'notification-watcher.lock').open('a') as lock:
        try:fcntl.flock(lock,fcntl.LOCK_EX|fcntl.LOCK_NB)
        except BlockingIOError:return
        (root/'notification-watcher.pid').write_text(str(os.getpid()))
        phases={
            'initial':([n+'-initial' for n in ('abi','cef','arc-layer','matcher')],'Initial engine research complete'),
            'decompile':([n+'-decompile' for n in ('abi','cef','arc-layer','matcher')],'Engine body analysis complete'),
            'specs':([n+'-specs' for n in ('spec-1','spec-2','spec-3','spec-4')],'Arc class specifications drafted'),
            'class-review':([n+'-classreview' for n in ('spec-1','spec-2','spec-3','spec-4')],'Arc class specification review complete'),
            'synthesis':(['review-synthesis'],'Arc engine report review complete'),
        }
        while True:
            status=(root/'STATUS').read_text() if (root/'STATUS').exists() else ''
            for match in re.finditer(r'^\S+ JOB ([a-z0-9-]+) FAILED.*$',status,re.M):
                job=match[1]
                notify('failed-'+job,'Arc research job needs attention: '+job,
                       'A Sol research job failed. Its logs and prior evidence are preserved on arcmod-research.')
            for phase,(jobs,title) in phases.items():
                if all((root/(job+'.done')).exists() for job in jobs):
                    notify('complete-'+phase,title,
                           'The Sol workers finished this stage on arcmod-research. Codex is collecting and reviewing the evidence for the full Arc engine report.')
            if (root/'review-synthesis.done').exists():return
            time.sleep(30)

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('action',choices=['watch','test'])
    if parser.parse_args().action=='test':
        notify('setup-test','Arc research notification test',
               'Testing the existing exe.dev notify integration for Sol research completion and failure milestones.')
    else:watch()
