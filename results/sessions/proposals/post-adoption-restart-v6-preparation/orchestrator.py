#!/usr/bin/env python3
"""PRIVATE prepared replay only; parent invokes via locked.sh. Unchanged v6, ours156 only."""
import argparse,hashlib,importlib.util,json,os,sys
from pathlib import Path
ROOT=Path('/Users/cryogenicplanet/general/arc-engine');HELPER=Path('/tmp/arccore-sessions-adoption-runtime-v1.py')
HELPER_SHA='0df88d889d64f38d46dc06216cc0c2ba87a04167854b05c1f566649a1a73a8a0'
OWN_SHA='178091e65d07409a00eeeb86101a7234afd44f00a3597512c2b1be0095118978'
PAYLOADS={'chromium/src/chrome/browser/ui/browser_live_tab_context.cc':'934d1d49e2f04d2790ecfd4ad05c65f4195e31455aab6aedcd957d75068ee35f','chromium/src/chrome/browser/ui/browser_tabrestore.cc':'f37fe417662955ca13ddbbe51ef88479b6843a90184a30752a75e23b3d74f827','chromium/src/chrome/browser/ui/browser_tabrestore.h':'1e7d4f5d3f3435cf4ee2a7893c632d1f64e0c75092c6e624a0abf6e3445039b4','chromium/src/components/sessions/core/tab_restore_service_helper.cc':'a3fdc9acb7906ad2e692574c7393baa551ac8ec5b9c94e1a015f547dc2b19bd0','chromium/src/components/sessions/core/tab_restore_service_helper.h':'5dd2e512e6b18e6015ff59b53b87dd6b1f11ab1c54dbc17ab549691714a8ed15'}
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--out',required=True);ap.add_argument('--pins',required=True);ap.add_argument('--pins-sha256',required=True);a=ap.parse_args();os.chdir(ROOT)
 assert sha(a.pins)==a.pins_sha256;raw=json.loads(Path(a.pins).read_text());frozen=raw['paths'];assert all(sha(n)==v for n,v in frozen.items());assert all(not Path(n).exists()for n in raw.get('absent',[]));assert frozen[str(HELPER)]==HELPER_SHA and sha(HELPER)==HELPER_SHA
 spec=importlib.util.spec_from_file_location('reviewed_sessions_runtime_v1',HELPER);h=importlib.util.module_from_spec(spec);spec.loader.exec_module(h) # main is never called
 h.LOCK=h.lock_snapshot();h.OUT=Path(a.out).absolute();h.real_parent(h.OUT.parent);assert str(h.OUT).startswith(str(ROOT/'results/sessions')+'/')and not h.OUT.exists()and not h.OUT.is_symlink();h.OUT.mkdir(mode=0o700)
 h.PINS=dict(frozen);h.STATUS={'scope':'Unchanged v6 supplemental closed-tab restart, ours156 only; not canonical acceptance','commands':[],'isolation':{},'scenarios':{},'complete':False,'pass':False};h.write(h.OUT/'frozen-inputs.json',raw)
 def capture(generated=False):
  names={str(p.relative_to(ROOT))for p in(ROOT/'arc').rglob('*')if p.is_file()and not p.is_symlink()and p.suffix in ({'.mm','.h','.cc','.gni','.gn','.mojom','.inc'}if generated else{'.mm','.h','.cc','.gni','.gn','.mojom'})};names.add('patches/MANIFEST')
  lines=[x.split()for x in(ROOT/'patches/MANIFEST').read_text().splitlines()if x.strip()and not x.lstrip().startswith('#')];assert all(len(x)==2 for x in lines)and len({x[1]for x in lines})==len(lines)
  names|={'chromium/src/'+n for _,n in lines}|{'patches/'+p for p,_ in lines};return{n:sha(ROOT/n)for n in sorted(names)}
 def payload_guards():
  assert sha(ROOT/'arc/api/arc_sessions_restore.mm')==OWN_SHA;assert all(sha(ROOT/n)==v for n,v in PAYLOADS.items())
  header=(ROOT/'chromium/src/chrome/browser/arcmod/arcmod_hooks.h').read_text();factory=header[header.index('// Runs after Chrome computes'):];factory=factory[:factory.index('\n}\n')+3];source=header[header.index('// Marks one actual historical'):];source=source[:source.index('\n}\n')+3]
  assert hashlib.sha256(factory.encode()).hexdigest()=='44353690cde79e52481e243e1f5ebcbac2c0b252fedeeb7528e338026c419e07';assert hashlib.sha256(source.encode()).hexdigest()=='14b7c14ac77404c507170bc44bff9a9d27950709e94c1c5b841c6926cd501e9c'
  assert header.count('inline SessionBrowserFactoryHook& GetSessionBrowserFactoryHook()')==header.count('inline SessionTabRestoreScopeHook& GetSessionTabRestoreScopeHook()')==1
 def check():
  h.require_lock();assert sha(a.pins)==a.pins_sha256;h.pins_check();assert all(not Path(n).exists()for n in raw.get('absent',[]));payload_guards();assert capture(False)==entry,'entry build-input drift';assert not h.INTERRUPTED,'deferred signal'
 payload_guards();entry=capture(False);assert all(n not in frozen or frozen[n]==v for n,v in entry.items());h.PINS.update(entry);h.write(h.OUT/'entry-build-inputs.json',entry)
 initial={str(p):h.inode(p)for p in [h.CACHE,h.PROFILE]};assert not h.census(),'ours156 app/helper already running';build_inputs=None;binary=None
 try:
  check();assert h.run(['python3','arc/tools/gen_api_table.py'],h.OUT/'generator.log','generator')==0;check();build_inputs=capture(True);h.PINS.update(build_inputs);h.write(h.OUT/'build-inputs.before.json',build_inputs);h.write(h.OUT/'inputs.expected.json',{'paths':h.PINS})
  for label,command in [('build',['/usr/bin/env','ARC_BUILD_FAIL_FAST=1','./build-babysit.sh']),('package',['./package-engine.sh','ours156']),('codesign',['codesign','--verify','--deep','--strict',str(h.APP)])]:
   check();assert capture(True)==build_inputs;assert h.run(command,h.OUT/(label+'.log'),label)==0;check()
  binary=h.record_frameworks('frameworks.before');rc=h.isolated(['env','ARC_SESSIONS_RESTART_AGENT=ours156','ARC_SESSIONS_RESTART_OUT='+str(h.OUT/'restart-v6'),'bun','/tmp/arccore-sessions-tab-restart-v6.ts'],h.OUT/'restart-v6.log','restart-v6',9464);assert rc==0
  result=json.loads((h.OUT/'restart-v6/result.json').read_text());assert result['agent']=='ours156'and result['pass']is True and result['contractPass']is True and result['phase']=='complete';assert type(result['pid1'])is int and type(result['pid2'])is int and result['pid1']!=result['pid2']
  assert result['before']==result['restoredHistory']and len(result['closed'])==2 and len(result['after']['records'])==2;assert len(result['shutdowns'])==2 and all(x['acknowledged']is True and x['exited']is True for x in result['shutdowns']);assert not result.get('cleanupSignal')and not result.get('cleanupError')and not result.get('ownedAppStillRunning')
  h.fatal_gate(h.OUT/'restart-v6');check();assert capture(True)==build_inputs;assert h.record_frameworks('frameworks.after')==binary;h.STATUS.update({'pass':True,'fixtureExit':rc,'fixtureContractPass':True,'pid1':result['pid1'],'pid2':result['pid2'],'canonicalAcceptance':False})
 except BaseException as error:h.STATUS.update({'pass':False,'error':type(error).__name__+': '+str(error)[:4096]})
 finally:
  h.wait_safe(initial,'post-adoption-restart-final-drain');h.STATUS['cleanupVerified']=True;h.STATUS['deferredSignals']=h.INTERRUPTED;h.STATUS['complete']=True
  if h.INTERRUPTED:h.STATUS['pass']=False
  after=capture(build_inputs is not None);h.write(h.OUT/'build-inputs.after.json',after);h.STATUS['buildInputInventoryStable']=after==(build_inputs if build_inputs is not None else entry)
  if not h.STATUS['buildInputInventoryStable']:h.STATUS['pass']=False
  h.write(h.OUT/'status.json',h.STATUS)
 return 0 if h.STATUS['pass']else 1
if __name__=='__main__':sys.exit(main())
