from pathlib import Path
import os,json,hashlib,subprocess,tempfile,time,signal,sys,shutil
ROOT=Path('/Users/cryogenicplanet/general/arc-engine');SCRIPT=Path('/tmp/arccore-sessions-adopt-source-only.py');EVIDENCE=ROOT/'results/sessions/proposals/source-only-adoption-transaction';sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest();baseinfo=json.loads((ROOT/'results/sessions/proposals/current-six-live-base-e973/manifest.json').read_text());results=[]
regen='''from pathlib import Path
import json,hashlib,sys
r=Path(__file__).parent;c=json.loads((r/'config.json').read_text());countfile=r/'regen-count';count=int(countfile.read_text())+1 if countfile.exists() else 1;countfile.write_text(str(count))
entries={}
for line in(r/'patches/MANIFEST').read_text().splitlines():
 if line.strip()and not line.startswith('#'):
  patch,n=line.split();entries.setdefault(patch,[]).append(n)
for patch,names in entries.items():
 text=''.join(n+':'+(hashlib.sha256((r/'chromium/src'/n).read_bytes()).hexdigest()if(r/'chromium/src'/n).exists()else'ABSENT')+'\\n'for n in names)
 (r/'patches'/patch).write_text(text)
if c.get('regenFailFirst') and count==2:sys.exit(3)
'''
runtime='''from pathlib import Path
import json,os,time,subprocess,sys
r=Path(__file__).parent;c=json.loads((r/'config.json').read_text());e=Path(os.environ['ARCCORE_SESSIONS_TRANSACTION_DIR']);(e/'runtime-ready.json').write_text(json.dumps({'pid':os.getpid(),'pgid':os.getpgrp()}))
if c.get('grandchild'):
 child=subprocess.Popen([sys.executable,'-c','import time;time.sleep(1.0)']);(e/'grandchild.json').write_text(json.dumps({'pid':child.pid,'at':time.monotonic()}))
if c.get('delay'):time.sleep(c['delay'])
x={'code':0,'cleanupVerified':not c.get('missingCleanup',False),'launch':{'exit':0,'passed':8 if c.get('launchFail')else9,'total':9,'coverageAccepted':True},'canonical':{'validationExit':1 if c.get('canonicalFail')else0,'noEarlierRegression':not c.get('canonicalFail',False),'scenarios':[{'name':n,'harnessExit':1,'passed':12,'failed':1,'inconclusive':0,'matchedPriorFailure':True}for n in ['sessions-public-api','sessions-tab-restart']]}}
Path(os.environ['ARCCORE_SESSIONS_TRANSACTION_RESULT']).write_text(json.dumps(x))
'''.replace('else9','else 9').replace('else0','else 0')
def setup(name,config):
 r=Path(tempfile.mkdtemp(prefix='sessions-adoption-mock-'+name+'-',dir='/tmp'));(r/'.sessions-transaction-mock').write_text('PRIVATE MOCK ONLY\n');(r/'logs/arccore-build.lock').mkdir(parents=True);(r/'logs/arccore-build.lock/pid').write_text(str(os.getpid()))
 for n,v in baseinfo['productionBaseline'].items():
  src=Path(v['path']);assert sha(src)==v['sha256'],('live baseline changed before mock setup',n);dest=r/n if n.startswith('arc/')else r/'chromium/src'/n;dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(src.read_bytes())
 for n in ['arc/browser/arc_browser_window.h','arc/BUILD.gn']:
  p=r/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/n).read_bytes())
 if config.get('foreignHeader'):
  p=r/'chromium/src/chrome/browser/arcmod/arcmod_hooks.h';text=p.read_text().replace('namespace arcmod {\n','namespace arcmod {\n// Preserved foreign owner hook.\ninline bool ForeignOwnerMarker() { return true; }\n',1);p.write_text(text)
 (r/'arc/api/foreign-owner.mm').write_text('// foreign owner unchanged\n');(r/'patches').mkdir();(r/'patches/MANIFEST').write_bytes((ROOT/'patches/MANIFEST').read_bytes());(r/'config.json').write_text(json.dumps(config));(r/'mock-regen.py').write_text(regen);(r/'mock-runtime.py').write_text(runtime);(r/'mock-syntax.py').write_text('import sys;sys.exit(0)\n');(r/'mock-official.py').write_text("print('check done: mock')\n")
 subprocess.run([sys.executable,r/'mock-regen.py'],check=True)
 pins={'paths':{str(r/'mock-runtime.py'):sha(r/'mock-runtime.py'),str(r/'mock-syntax.py'):sha(r/'mock-syntax.py')}};(r/'frozen-pins.json').write_text(json.dumps(pins));return r
for name,config in [('success-foreign-header',{'foreignHeader':True}),('launch-rollback',{'launchFail':True}),('canonical-rollback',{'canonicalFail':True}),('regen-rollback',{'regenFailFirst':True}),('signal-drain-rollback',{'delay':0.8}),('cleanup-hold',{'missingCleanup':True}),('grandchild-drain',{'grandchild':True})]:
 r=setup(name,config);run=r/'evidence';pre={}
 for n in ['arc/api/arc_sessions_restore.mm','chromium/src/chrome/browser/arcmod/arcmod_hooks.h','chromium/src/chrome/browser/ui/browser_live_tab_context.cc','chromium/src/chrome/browser/ui/browser_tabrestore.cc','chromium/src/chrome/browser/ui/browser_tabrestore.h','chromium/src/components/sessions/core/tab_restore_service_helper.cc','chromium/src/components/sessions/core/tab_restore_service_helper.h','patches/MANIFEST']:pre[n]=(r/n).read_bytes()
 oldpatches={str(p.relative_to(r)):p.read_bytes()for p in(r/'patches').glob('*.patch')};start=time.monotonic();log=EVIDENCE/('mock-'+name+'.log')
 with log.open('wb')as stream:
  command=[sys.executable,str(SCRIPT),'--mock-root',str(r),'--evidence-dir',str(run),'--frozen-runtime-pins',str(r/'frozen-pins.json'),'--frozen-runtime-pins-sha256',sha(r/'frozen-pins.json'),'--runtime-command',sys.executable,str(r/'mock-runtime.py'),'--pins','{runtime_pins}'];p=subprocess.Popen(command,stdout=stream,stderr=subprocess.STDOUT)
  sent=False;manual=False;child_alive_when_signal=False
  while p.poll()is None:
   assert time.monotonic()-start<20,(name,'mock timeout',log.read_text())
   if name=='signal-drain-rollback'and not sent and(run/'runtime-ready.json').exists():
    child=json.loads((run/'runtime-ready.json').read_text());assert child['pid']==child['pgid'];os.kill(child['pid'],0);p.send_signal(signal.SIGTERM);sent=True;time.sleep(.1);assert p.poll()is None;os.kill(child['pid'],0);child_alive_when_signal=True
   if name=='cleanup-hold'and not manual and(run/'transaction.json').exists():
    d=json.loads((run/'transaction.json').read_text())
    if d.get('status')=='HOLD'and 'cleanup' in d.get('holdReason','').lower():
     assert all((r/n).read_bytes()==v for n,v in pre.items());(run/'manual-cleanup-confirmation.json').write_text(json.dumps({'transactionId':d['transactionId'],'cleanupVerified':True,'verifiedBy':'private-mock-driver','evidence':'no app launched; fake runtime exited; source rollback verified'}));manual=True
   time.sleep(.03)
  rc=p.wait()
 d=json.loads((run/'transaction.json').read_text());success=name in ['success-foreign-header','grandchild-drain'];assert rc==(0 if success else 1),(name,rc,d.get('failure'))
 assert d['status']==('MOCK_APPLIED'if success else'ROLLED_BACK'),(name,d['status'],d.get('failure'))
 if not success:
  assert all((r/n).read_bytes()==v for n,v in pre.items()),name
  assert all((r/n).read_bytes()==v for n,v in oldpatches.items()),name
  assert not(r/'patches/0010-sessions.patch').exists(),name
 assert(r/'arc/api/foreign-owner.mm').read_text()=='// foreign owner unchanged\n'
 if config.get('foreignHeader'):assert 'ForeignOwnerMarker'in(r/'chromium/src/chrome/browser/arcmod/arcmod_hooks.h').read_text()
 if name=='signal-drain-rollback':assert sent and child_alive_when_signal and d['deferredSignals']==[signal.SIGTERM]
 if name=='cleanup-hold':assert manual
 if name=='grandchild-drain':assert time.monotonic()-json.loads((run/'grandchild.json').read_text())['at']>=0.95
 results.append({'name':name,'pass':True,'root':str(r),'evidence':str(run),'status':d['status'],'seconds':time.monotonic()-start,'logSHA256':sha(log)});print(name,'PASS',flush=True)
(EVIDENCE/'mock-results.json').write_text(json.dumps({'scope':'PRIVATE temporary roots only; fake lock/regen/syntax/runtime; no actual app/build/shared lock/production mutation','scriptSHA256':sha(SCRIPT),'results':results},indent=2)+'\n')
