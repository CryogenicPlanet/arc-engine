from pathlib import Path
import json,subprocess,os,tempfile,concurrent.futures,hashlib,datetime
root=Path('/Users/cryogenicplanet/general/arc-engine');p=root/'results/sessions/proposals/source-only-adoption-host-gates';m=json.loads((p/'manifest.json').read_text());stage=Path(m['stage']);sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest();entries=[]
for n in m['inputStageSHA256']:
 if not(stage/n).exists():continue
 paths=[root/'chromium/src'/n]
 if n.startswith('arc/'):paths.append(root/n)
 for path in paths:entries.append({'type':'file','name':str(path),'external-contents':str(stage/n)})
overlay=p/'overlay.json';overlay.write_text(json.dumps({'version':0,'case-sensitive':'true','roots':entries},indent=2)+'\n');env={**os.environ,'PATH':str(root/'depot_tools')+':'+os.environ['PATH']}
def run(kv):
 key,item=kv;clean=[];removed=[];skip=False
 for a in item['originalCommand']:
  if skip:removed.append(a);skip=False;continue
  if a in ['-o','-MF','-MT','-MQ','-serialize-diagnostics','-dependency-file']:removed.append(a);skip=True;continue
  if a in ['-c','-MMD','-MD','-MP']or a.startswith('-fcrash-diagnostics-dir='):removed.append(a);continue
  clean.append(a)
 clean+=['-ivfsoverlay',str(overlay),'-fsyntax-only','-Wno-unused-command-line-argument','-fcrash-diagnostics-dir='+tempfile.mkdtemp(prefix='session-host-gates-vfs-crash-',dir='/tmp')];res=subprocess.run(clean,cwd=item['cwd'],env=env,capture_output=True);log=p/(key+'.vfs.syntax.log');log.write_bytes(res.stdout+res.stderr);print(key,'VFS',res.returncode,'bytes',log.stat().st_size,flush=True);return key,{'command':clean,'removedFlags':removed,'cwd':item['cwd'],'exit':res.returncode,'log':str(log),'logSHA256':sha(log)}
with concurrent.futures.ThreadPoolExecutor(max_workers=4)as pool:results=dict(pool.map(run,m['compiler'].items()))
d={'scope':'Supplemental actual Siso flags with original logical source paths mapped through LLVM VFS overlay; syntax only, no outputs/build/runtime','overlaySHA256':sha(overlay),'manifestSHA256':sha(p/'manifest.json'),'compiler':results,'allExitZeroEmptyLogs':all(v['exit']==0 and Path(v['log']).stat().st_size==0 for v in results.values()),'finishedUTC':datetime.datetime.now(datetime.timezone.utc).isoformat()};(p/'vfs-syntax.json').write_text(json.dumps(d,indent=2)+'\n');print('PASS',d['allExitZeroEmptyLogs'])
