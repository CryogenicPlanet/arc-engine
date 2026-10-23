#!/usr/bin/env python3
"""Parent-only source-only sessions transaction. Execute ONLY via locked.sh.
No invocation has been performed on production by the author. See private evidence README.
"""
from pathlib import Path
import argparse, datetime, difflib, hashlib, json, os, re, shlex, signal, subprocess, sys, tempfile, time, uuid

REAL_ROOT=Path('/Users/cryogenicplanet/general/arc-engine')
OWN='arc/api/arc_sessions_restore.mm'
HEADER='chrome/browser/arcmod/arcmod_hooks.h'
NATIVE=['chrome/browser/ui/browser_live_tab_context.cc','chrome/browser/ui/browser_tabrestore.cc','chrome/browser/ui/browser_tabrestore.h','components/sessions/core/tab_restore_service_helper.cc','components/sessions/core/tab_restore_service_helper.h']
TARGETS=[OWN,HEADER]+NATIVE
REGEN_TOOL_SHA='d90b45cd225c14324df1fdf5661a38db338662a1efa0ff9da2c7bb4e0acf94be'
OWN_BASE='e973b371ff612fcb85132ee992af900937f92fbc325d06a97ec9e4b17a9c668d'
INPUT_MANIFEST=REAL_ROOT/'results/sessions/proposals/current-composition-20261023T1423Z/manifest.json'
INPUT_MANIFEST_SHA='06a3fdc38d6b262697dd45f2dac90cfc08f3dad098d357aa5adf7cb2f90f5a9b'
HOST_PATCH=REAL_ROOT/'results/sessions/proposals/source-only-adoption-host-gates/host-gating.patch'
HOST_PATCH_SHA='574e225c4e37e53ff4d77c84ffbcd22a861684fa9cfe5631a28ba89c5d274e03'
FINAL_HEADER=REAL_ROOT/'results/sessions/proposals/current-six-live-base-e973/manifest.json'
FINAL_HEADER_MANIFEST_SHA='610baed3db37286e72867b3ec805f3276de3311325e51f873dbc3b5a41bc940a'
RUNTIME_RUNNER=Path('/tmp/arccore-sessions-adoption-runtime-v1.py')
RUNTIME_RUNNER_SHA='0df88d889d64f38d46dc06216cc0c2ba87a04167854b05c1f566649a1a73a8a0'
FINAL_HEADER_SHA='8d528a44114eb7306b2bc7f8b83fb0e25d57ebcb3ecacf0620cfb5ca77b2e725'
COMPILERS={
 'helper':('obj/components/sessions/shared/tab_restore_service_helper.o','components/sessions/core/tab_restore_service_helper.cc'),
 'live':('obj/chrome/browser/ui/browser_live_tab_context_impl/browser_live_tab_context.o','chrome/browser/ui/browser_live_tab_context.cc'),
 'restore':('obj/chrome/browser/ui/browser_tabrestore_impl/browser_tabrestore.o','chrome/browser/ui/browser_tabrestore.cc'),
 'owned':('obj/arc/arc_core/arc_sessions_restore.o',OWN),
}

def digest(data):return hashlib.sha256(data).hexdigest()
def file_sha(path):return digest(Path(path).read_bytes())
def stamp():return datetime.datetime.now(datetime.timezone.utc).isoformat()
class Abort(RuntimeError):pass

def failure_text(error):return type(error).__name__+': '+str(error)[:4096]
def select_compile_command(raw,target,source):
 # Recursive Siso output may contain unrelated binary data. Filter BYTES using
 # exact output and source tokens; decode ONLY one uniquely selected argv line.
 def argument(line,flag):
  pattern=rb'(?:^|[ \t])'+re.escape(flag)+rb'[ \t]+("[^"\r\n]*"|\'[^\'\r\n]*\'|[^ \t\r\n]+)'
  values=re.findall(pattern,line)
  return [v[1:-1]if len(v)>=2 and v[:1]==v[-1:]and v[:1]in [b'"',b"'"]else v for v in values]
 expected_output=target.encode('utf-8');expected_source=source.encode('utf-8');selected=[]
 for line in raw.splitlines():
  if b'clang'not in line:continue
  outputs=argument(line,b'-o');sources=argument(line,b'-c')
  if outputs==[expected_output]and len(sources)==1 and(sources[0]==expected_source or sources[0].endswith(b'/'+expected_source)):selected.append(line)
 assert len(selected)==1,'Expected one exact Siso compile command for '+target+'; found '+str(len(selected))
 argv=shlex.split(selected[0].decode('utf-8',errors='strict'))
 assert argv.count('-o')==1 and argv[argv.index('-o')+1]==target
 assert argv.count('-c')==1 and(argv[argv.index('-c')+1]==source or argv[argv.index('-c')+1].endswith('/'+source))
 return argv

class Transaction:
 def __init__(self,args):
  self.args=args;self.mock=args.mock_root is not None;self.root=Path(args.mock_root).resolve()if self.mock else REAL_ROOT
  if self.mock:
   assert self.root!=REAL_ROOT and REAL_ROOT not in self.root.parents and (self.root/'.sessions-transaction-mock').read_text().strip()=='PRIVATE MOCK ONLY'
  self.pending=[];self.live=False;self.ever_published=False;self.generated_patch_images={};self.runtime_started=False;self.cleanup_verified=False;self.expected={};self.backup={};self.patch_backup={};self.manifest_before=None;self.manifest_after=None;self.orphan_patch=None
  self.id=uuid.uuid4().hex;self.evidence=Path(args.evidence_dir).resolve()if args.evidence_dir else self.root/'results/sessions/proposals/source-only-adoption-transaction'/('run-'+self.id)
  self.evidence.mkdir(parents=True,exist_ok=False);self.stage=self.evidence/'stage';self.stage.mkdir();self.state={'transactionId':self.id,'startedUTC':stamp(),'status':'PREPARING','mock':self.mock,'root':str(self.root),'events':[],'noRuntimeAcceptanceClaim':True};self.save()
  for sig in [signal.SIGINT,signal.SIGTERM,signal.SIGHUP]:signal.signal(sig,self.defer_signal)
 def defer_signal(self,sig,frame):self.pending.append(sig)
 def save(self):(self.evidence/'transaction.json').write_text(json.dumps(self.state,indent=2)+'\n')
 def event(self,name,**fields):self.state['events'].append({'at':stamp(),'event':name,**fields});self.save()
 def check_signal(self):
  if self.pending:raise Abort('Deferred signal(s): '+str(self.pending))
 def current(self,n):return self.root/n if n.startswith('arc/')else self.root/'chromium/src'/n
 def best_effort_save(self):
  try:self.save()
  except OSError:pass
 def drain_runtime_group(self,pgid):
  # The trusted runtime runner also verifies app/helper/profile cleanup. This
  # independently drains descendants that remain in its isolated process group.
  while True:
   try:
    q=subprocess.run(['ps','-axo','pid=,pgid=,stat='],capture_output=True,text=True,check=True)
    alive=[]
    for line in q.stdout.splitlines():
     fields=line.split()
     if len(fields)>=3 and int(fields[1])==pgid and not fields[2].startswith('Z'):alive.append(int(fields[0]))
    if not alive:return
   except (OSError,ValueError,subprocess.SubprocessError):pass
   time.sleep(0.2)
 def run(self,argv,label,cwd=None,env=None,runtime=False,rollback=False):
  self.event('child-start',label=label,argv=[str(x)for x in argv],runtime=runtime)
  log=self.evidence/(label+'.log');child=None;code=None
  with log.open('wb')as stream:
   try:
    child=subprocess.Popen([str(x)for x in argv],cwd=cwd or self.root,env=env,stdout=stream,stderr=subprocess.STDOUT,start_new_session=runtime)
    self.event('child-pid',label=label,pid=child.pid,isolatedSession=runtime)
   finally:
    # Even a bookkeeping failure after Popen cannot skip child drainage.
    if child is not None:
     code=child.wait()
     if runtime:self.drain_runtime_group(child.pid)
  self.event('child-exit',label=label,code=code,log=str(log),logSHA256=file_sha(log),deferredSignals=self.pending[:])
  if not rollback and not runtime:self.check_signal()
  return code,log
 def lock_check(self):
  lock=self.root/'logs/arccore-build.lock';assert lock.is_dir()and not lock.is_symlink();pidfile=lock/'pid';owner=int(pidfile.read_text().strip());assert owner>1
  ancestors=[];pid=os.getppid()
  while pid>1 and pid not in ancestors:
   ancestors.append(pid)
   if pid==owner:break
   out=subprocess.run(['ps','-o','ppid=','-p',str(pid)],capture_output=True,text=True,check=True).stdout.strip();pid=int(out)
  assert owner in ancestors,'lock pid is not an ancestor; invoke only through arc/tools/locked.sh';os.kill(owner,0);self.owner=owner;self.event('lock-verified',owner=owner,ancestors=ancestors)
 def read_safe(self,path):
  assert path.is_file()and not path.is_symlink(),f'Expected regular file: {path}';return path.read_bytes()
 def capture(self):
  self.lock_check();self.check_signal()
  if not self.mock:
   assert file_sha(self.root/'arc/tools/regen-patches.sh')==REGEN_TOOL_SHA,'official regen tool changed before transaction'
   self.state['regenTool']={'path':str(self.root/'arc/tools/regen-patches.sh'),'sha256':REGEN_TOOL_SHA}
  assert file_sha(self.current(OWN))==OWN_BASE,'owned source no longer e973; no silent rebase'
  self.backup={n:self.read_safe(self.current(n))for n in TARGETS};manifest=self.root/'patches/MANIFEST';self.manifest_before=self.read_safe(manifest)
  self.manifest_entries=self.parse_manifest(self.manifest_before)
  assert self.manifest_entries.get(HEADER)==['0004-arcmod-hooks.patch'],'native hooks header must remain in 0004'
  patches=set(p for values in self.manifest_entries.values()for p in values)|{'0010-sessions.patch'}
  self.patch_backup={n:(self.read_safe(self.root/'patches'/n)if(self.root/'patches'/n).exists()else None)for n in patches}
  for n,data in self.backup.items():p=self.evidence/'backup'/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(data)
  (self.evidence/'backup/MANIFEST').write_bytes(self.manifest_before)
  for n,data in self.patch_backup.items():
   if data is not None:p=self.evidence/'backup/maintained-patches'/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(data)
  self.state['preimages']={n:digest(v)for n,v in self.backup.items()};self.state['maintainedPatchPreimages']={n:digest(v)if v is not None else None for n,v in self.patch_backup.items()};self.state['manifestPreimageSHA256']=digest(self.manifest_before)
  self.broad_entry_pins={str(p.relative_to(self.root)):file_sha(p)for folder in ['arc/api','arc/wp']for p in(self.root/folder).rglob('*')if p.is_file()and not p.is_symlink()and p.suffix in ['.mm','.h','.cc','.gni','.gn']}
  
  if(self.root/'arc/BUILD.gn').exists():self.broad_entry_pins['arc/BUILD.gn']=file_sha(self.root/'arc/BUILD.gn')
  self.state['broadEntrySourcePins']=self.broad_entry_pins;self.load_runtime_pins();self.save()
 def pin_path(self,key):
  p=Path(key);return p.resolve()if p.is_absolute()else(self.root/p).resolve()
 def runtime_mutable_paths(self):
  paths={self.current(n).resolve()for n in TARGETS}|{(self.root/'patches/MANIFEST').resolve()}|{(self.root/'patches'/n).resolve()for n in self.patch_backup}
  paths|={(self.root/n).resolve()for n in self.broad_entry_pins}
  return paths
 def load_runtime_pins(self):
  p=Path(self.args.frozen_runtime_pins).resolve();assert file_sha(p)==self.args.frozen_runtime_pins_sha256,'frozen runtime pins file changed'
  raw=json.loads(p.read_text());assert isinstance(raw,dict)and isinstance(raw.get('paths'),dict)
  assert all(isinstance(n,str)and isinstance(h,str)and re.fullmatch(r'[0-9a-f]{64}',h)for n,h in raw['paths'].items())
  self.frozen_runtime_raw=raw;self.mutable_pin_paths=self.runtime_mutable_paths();self.state['frozenRuntimePins']={'path':str(p),'sha256':file_sha(p),'count':len(raw['paths'])}
  assert self.args.runtime_command.count('{runtime_pins}')==1,'runtime argv requires one standalone {runtime_pins} placeholder'
  if not self.mock:
   assert str(RUNTIME_RUNNER)in self.args.runtime_command and file_sha(RUNTIME_RUNNER)==RUNTIME_RUNNER_SHA,'runtime runner not reviewed pinned v1'
   assert any(self.pin_path(n)==RUNTIME_RUNNER.resolve() and h==RUNTIME_RUNNER_SHA for n,h in raw['paths'].items()),'runtime v1 missing from frozen table'
  self.verify_frozen_runtime_pins();(self.evidence/'frozen-runtime-pins.json').write_text(json.dumps(raw,indent=2)+'\n')
 def verify_frozen_runtime_pins(self):
  assert file_sha(self.args.frozen_runtime_pins)==self.args.frozen_runtime_pins_sha256,'frozen pin file drift'
  for n,h in self.frozen_runtime_raw['paths'].items():
   if self.pin_path(n)not in self.mutable_pin_paths:assert file_sha(self.pin_path(n))==h,('frozen runtime/tool/golden drift',n)
 def merge_runtime_pins(self):
  self.verify_frozen_runtime_pins();merged=dict(self.frozen_runtime_raw['paths']);replacements={}
  for n in TARGETS:replacements[str(self.current(n).relative_to(self.root))]=file_sha(self.current(n))
  replacements['patches/MANIFEST']=file_sha(self.root/'patches/MANIFEST')
  for n in self.patch_backup:
   path=self.root/'patches'/n
   if path.exists():replacements['patches/'+n]=file_sha(path)
  for n,h in self.broad_entry_pins.items():
   if n!=OWN:assert file_sha(self.root/n)==h,('entry source drift',n)
   replacements[n]=file_sha(self.root/n)
  replacements[str(HOST_PATCH)]=HOST_PATCH_SHA
  by_path={self.pin_path(n):h for n,h in replacements.items()}
  for n in list(merged):
   if self.pin_path(n)in by_path:merged[n]=by_path[self.pin_path(n)]
  merged.update(replacements);raw=dict(self.frozen_runtime_raw);raw['paths']=merged;target=self.evidence/'runtime-pins.json';target.write_text(json.dumps(raw,indent=2)+'\n');self.state['mergedRuntimePins']={'path':str(target),'sha256':file_sha(target),'paths':merged};self.save();return target
 def parse_manifest(self,data):
  result={}
  for line in data.decode().splitlines():
   if not line.strip()or line.lstrip().startswith('#'):continue
   fields=line.split();assert len(fields)==2,line;patch,n=fields;assert re.fullmatch(r'[A-Za-z0-9_.-]+\.patch',patch);assert not n.startswith('/')and '..'not in Path(n).parts
   result.setdefault(n,[]).append(patch)
  assert all(len(v)==1 for v in result.values()),'duplicate MANIFEST paths';return result
 def header_union(self,current,frozen):
  symbols=['SessionBrowserCreatedCallback','SessionBrowserFactoryHook','GetSessionBrowserFactoryHook','SessionTabRestoreScopeHook','GetSessionTabRestoreScopeHook']
  assert all(not re.search(r'\b'+n+r'\b',current)for n in symbols),'sessions hooks already exist or conflict'
  anchor='namespace arcmod {\n';assert current.count(anchor)==1
  factory=frozen[frozen.index('// Runs after Chrome computes'):frozen.index('// Marks one actual historical')].rstrip('\n')+'\n'
  source=frozen[frozen.index('// Marks one actual historical'):];source=source[:source.index('\n}\n')+3]
  assert digest(factory.encode())=='44353690cde79e52481e243e1f5ebcbac2c0b252fedeeb7528e338026c419e07'
  assert digest(source.encode())=='14b7c14ac77404c507170bc44bff9a9d27950709e94c1c5b841c6926cd501e9c'
  payload='\n'+factory+'\n\n'+source;assert digest(payload.encode())=='ea3bb3d66adde7f4a7622a6091199d11091a17d20ae26401ddd16380958f559d'
  result=current;additions=[]
  def insert(text,anchor,after=False):
   nonlocal result
   assert result.count(anchor)==1,(anchor,'nonunique insertion anchor');result=result.replace(anchor,(anchor+text if after else text+anchor),1);additions.append(text)
  for line,preferred,after in [('#include <cstdint>\n','#include <memory>\n',False),('#include "base/functional/callback_helpers.h"\n','#include "base/functional/callback.h"\n',True),('#include "base/memory/weak_ptr.h"\n','#include "base/no_destructor.h"\n',True)]:
   if line in result:assert result.count(line)==1;continue
   if preferred in result:insert(line,preferred,after)
   else:
    first=re.search(r'^#include .+\n',result,re.M);assert first;insert(line,first.group())
  forwards=''.join(line for line in ['class BrowserWindowInterface;\n','struct BrowserWindowCreateParams;\n']if line not in result)
  if forwards:insert(forwards,anchor)
  insert(payload,anchor,True)
  reverse=result
  for text in reversed(additions):assert reverse.count(text)==1;reverse=reverse.replace(text,'',1)
  assert reverse==current,'header union did not preserve all foreign bytes'
  self.state['headerUnion']={'insertedPayloadSHA256':digest(payload.encode()),'foreignBytesReverseExact':True,'additions':additions};return result
 def stage_changes(self):
  assert file_sha(INPUT_MANIFEST)==INPUT_MANIFEST_SHA and file_sha(HOST_PATCH)==HOST_PATCH_SHA and file_sha(FINAL_HEADER)==FINAL_HEADER_MANIFEST_SHA
  info=json.loads(INPUT_MANIFEST.read_text());steps=info['alternatives']['source-only']['steps'][:6];assert len(steps)==6
  inputs=[]
  for step in steps:
   v=info['frozenPatches'][step['patch']];p=Path(v['path']);assert file_sha(p)==v['sha256'];inputs.append((step['patch'],p,v['sha256']))
  self.state['inputPatches']={n:{'path':str(p),'sha256':h}for n,p,h in inputs};self.state['inputPatches']['host-gates']={'path':str(HOST_PATCH),'sha256':HOST_PATCH_SHA}
  for n,data in self.backup.items():p=self.stage/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(data)
  # Include read-only live dependencies for VFS checks, without publishing them.
  for n in ['arc/api/arc_internal.h','arc/api/arc_web_contents.mm','arc/browser/arc_browser_window.h']:
   data=self.read_safe(self.current(n));p=self.stage/n;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(data)
  self.state['readOnlyStageDependencies']={n:file_sha(self.current(n))for n in ['arc/api/arc_internal.h','arc/api/arc_web_contents.mm','arc/browser/arc_browser_window.h']}
  assert not any('core_close_'in n for n in [str(x)for x in self.stage.rglob('*')])
  subprocess.run(['git','init','-q',str(self.stage)],check=True)
  for idx,(name,p,h)in enumerate(inputs,1):
   sections=re.split(r'(?=^--- a/)',p.read_text(),flags=re.M);kept=[]
   for section in sections:
    if not section.strip():continue
    match=re.match(r'--- a/(.+)\n\+\+\+ b/(.+)\n',section);assert match and match.group(1)==match.group(2)
    n=match.group(1);assert n in TARGETS,(name,n,'unexpected patch target')
    if n!=HEADER:kept.append(section)
   patch=self.evidence/(f'input-{idx}-{name}.nonheader.patch');patch.write_text(''.join(kept));code,_=self.run(['git','-C',self.stage,'apply','--check','--verbose',patch],f'stage-{idx}-check');assert code==0
   code,_=self.run(['git','-C',self.stage,'apply',patch],f'stage-{idx}-apply');assert code==0
  code,_=self.run(['git','-C',self.stage,'apply','--check','--verbose',HOST_PATCH],'stage-host-check');assert code==0
  code,_=self.run(['git','-C',self.stage,'apply',HOST_PATCH],'stage-host-apply');assert code==0
  frozen_info=json.loads(FINAL_HEADER.read_text());frozen_path=Path(frozen_info['routes']['source-only-live-base-six']['stage'])/HEADER;assert file_sha(frozen_path)==FINAL_HEADER_SHA
  original_header=self.backup[HEADER].decode();new_header=self.header_union(original_header,frozen_path.read_text());(self.stage/HEADER).write_text(new_header);(self.evidence/'header-before.h').write_text(original_header);(self.evidence/'header-after.h').write_text(new_header);(self.evidence/'header-union.patch').write_text(''.join(difflib.unified_diff(original_header.splitlines(True),new_header.splitlines(True),fromfile='a/'+HEADER,tofile='b/'+HEADER)))
  self.expected={n:(self.stage/n).read_bytes()for n in TARGETS};assert all(v!=self.backup[n]for n,v in self.expected.items()),'expected seven changed targets'
  self.state['stagedSHA256']={n:digest(v)for n,v in self.expected.items()};self.check_guards();self.save()
 def check_guards(self):
  def region(s,a,b):return s[s.index(a):s.index(b,s.index(a))]
  old=self.backup[OWN].decode();new=self.expected[OWN].decode();regions=[('- (void)setNavigations:','- (NSString*)groupId'),('void QueueRestorationCallback(','\nclass ScopedSessionInsertion'),('@implementation ArcWebContents (SessionsDiscard)','@implementation SessionsRestoreIDs')]
  assert all(region(old,a,b)==region(new,a,b)for a,b in regions),'owned guard region changed'
  assert 'CoreClose'not in new and 'ArcSessionsConfigureRestorationPolicy'not in new
  self.state['ownedGuardRegionsByteExact']=True
 def syntax(self):
  if self.mock:
   code,_=self.run([sys.executable,self.root/'mock-syntax.py'],'mock-syntax');assert code==0;return
  mappings=[]
  for n in TARGETS+list(self.state['readOnlyStageDependencies']):
   paths=[self.root/'chromium/src'/n]
   if n.startswith('arc/'):paths.append(self.root/n)
   for path in paths:mappings.append({'type':'file','name':str(path),'external-contents':str(self.stage/n)})
  overlay=self.evidence/'overlay.json';overlay.write_text(json.dumps({'version':0,'case-sensitive':'true','roots':mappings},indent=2)+'\n');self.state['compiler']={};env={**os.environ,'PATH':str(self.root/'depot_tools')+':'+os.environ['PATH']};cwd=self.root/'chromium/src/out/Base'
  for key,(target,n)in COMPILERS.items():
   querylog=self.evidence/(key+'-query.log');code,_=self.run(['siso','query','commands','-C','.',target],key+'-query',cwd=cwd,env=env);assert code==0
   original=select_compile_command(querylog.read_bytes(),target,n);clean=[];removed=[];skip=False
   for a in original:
    if skip:removed.append(a);skip=False;continue
    if a in ['-o','-MF','-MT','-MQ','-serialize-diagnostics','-dependency-file']:removed.append(a);skip=True;continue
    if a in ['-c','-MMD','-MD','-MP']or a.startswith('-fcrash-diagnostics-dir='):removed.append(a);continue
    clean.append(a)
   clean+=['-ivfsoverlay',str(overlay),'-fsyntax-only','-Wno-unused-command-line-argument','-fcrash-diagnostics-dir='+str(self.evidence/('crash-'+key))];assert '-Werror'in clean and all(x not in clean for x in ['-c','-o','-MF','-MD','-MMD']);code,log=self.run(clean,key+'-syntax',cwd=cwd,env=env);self.state['compiler'][key]={'queryLogSHA256':file_sha(querylog),'original':original,'command':clean,'removedFlags':removed,'exit':code,'logSHA256':file_sha(log)};self.save();assert code==0 and log.stat().st_size==0,(key,'syntax failed/diagnostics')
 def make_manifest(self):
  text=self.manifest_before.decode();entries=self.parse_manifest(self.manifest_before);added=[]
  for n in NATIVE:
   if n not in entries:added.append('0010-sessions.patch '+n+'\n')
  if added:text+=(''if text.endswith('\n')else'\n')+''.join(added)
  self.manifest_after=text.encode();after=self.parse_manifest(self.manifest_after);assert all(len(after[n])==1 for n in [HEADER]+NATIVE);assert after[HEADER]==['0004-arcmod-hooks.patch']
  # An unreferenced/new own patch must have its preimage restored on rollback,
  # since regen will no longer visit it after MANIFEST is restored.
  if added and '0010-sessions.patch'not in [p for v in entries.values()for p in v]:self.orphan_patch='0010-sessions.patch'
  self.state['manifestAddedLines']=added;self.state['manifestPublishedSHA256']=digest(self.manifest_after);self.save()
 def verify_preimages(self):
  if not self.mock:assert file_sha(self.root/'arc/tools/regen-patches.sh')==REGEN_TOOL_SHA,'official regen tool changed during preparation'
  assert all(self.read_safe(self.current(n))==v for n,v in self.backup.items()),'target changed during preparation';assert self.read_safe(self.root/'patches/MANIFEST')==self.manifest_before,'MANIFEST changed during preparation'
  assert all(file_sha(self.current(n))==h for n,h in self.state['readOnlyStageDependencies'].items()),'read-only stage dependency changed'
  for v in self.state['inputPatches'].values():assert file_sha(v['path'])==v['sha256'],'frozen patch changed'
  assert self.patches_match(self.patch_backup),'maintained patches changed during preparation'
  self.verify_frozen_runtime_pins()
  assert all(file_sha(self.root/n)==h for n,h in self.broad_entry_pins.items()),'broad entry source drift before publication'
 def atomic_write(self,path,data):
  assert not path.is_symlink();mode=path.stat().st_mode&0o777 if path.exists()else 0o644;temp=path.with_name(path.name+'.sessions-tmp-'+self.id);temp.write_bytes(data);temp.chmod(mode);os.replace(temp,path)
 def publish(self):
  self.check_signal();self.verify_preimages();self.lock_check();self.live=True;self.ever_published=True;self.state['status']='PUBLISHING';self.save()
  for n,v in self.expected.items():self.atomic_write(self.current(n),v)
  self.atomic_write(self.root/'patches/MANIFEST',self.manifest_after);self.event('source-published')
 def patches_match(self,expected):
  try:return all((self.read_safe(self.root/'patches'/n)if(self.root/'patches'/n).exists()else None)==v for n,v in expected.items())
  except (OSError,AssertionError):return False
 def private_regen_environment(self):
  # Official regen may git add -N a foreign assigned/untracked file. Keep all
  # such intent-to-add entries in a copied index, never Chromium's real index.
  if self.mock:return None
  if not hasattr(self,'private_chromium_index'):
   self.lock_check();probe_env=dict(os.environ);probe_env.pop('GIT_INDEX_FILE',None);probe_env['GIT_OPTIONAL_LOCKS']='0'
   source_root=self.root/'chromium/src'
   result=subprocess.run(['git','-C',str(source_root),'rev-parse','--git-path','index'],env=probe_env,capture_output=True,text=True,check=True)
   index=Path(result.stdout.strip());assert str(index)and result.stdout.strip()
   self.actual_chromium_index=(index if index.is_absolute()else source_root/index).resolve()
   data=self.read_safe(self.actual_chromium_index);self.actual_chromium_index_sha=digest(data)
   self.private_chromium_index=self.evidence/'chromium-private.index'
   with self.private_chromium_index.open('xb')as stream:stream.write(data)
   self.private_chromium_index.chmod(0o600)
   assert self.read_safe(self.actual_chromium_index)==data,'real Chromium index changed during copy'
   self.state['chromiumPrivateIndex']={'actualPath':str(self.actual_chromium_index),'entrySHA256':self.actual_chromium_index_sha,'privatePath':str(self.private_chromium_index),'regenerations':[]};self.save()
  env=dict(os.environ);env['GIT_INDEX_FILE']=str(self.private_chromium_index);env['GIT_OPTIONAL_LOCKS']='0';return env
 def regenerate(self,rollback=False):
  if not rollback:assert self.patches_match(self.patch_backup),'maintained patch changed before initial regeneration'
  command=[sys.executable,self.root/'mock-regen.py']if self.mock else[self.root/'arc/tools/regen-patches.sh']
  regen_env=self.private_regen_environment()
  regen_index_before=None if self.mock else file_sha(self.actual_chromium_index)
  try:
   # Drain first and capture outputs before honoring a deferred signal.
   code,_=self.run(command,'rollback-regen'if rollback else'apply-regen',env=regen_env,rollback=True)
  finally:
   if not rollback:self.generated_patch_images={n:(self.read_safe(self.root/'patches'/n)if(self.root/'patches'/n).exists()else None)for n in self.patch_backup}
   if not self.mock:
    actual_sha=file_sha(self.actual_chromium_index);self.state['chromiumPrivateIndex']['regenerations'].append({'phase':'rollback'if rollback else'apply','actualBeforeSHA256':regen_index_before,'actualAfterSHA256':actual_sha,'privateSHA256':file_sha(self.private_chromium_index),'actualDiffersFromEntryBeforeInvocation':regen_index_before!=self.actual_chromium_index_sha});self.best_effort_save()
    assert actual_sha==regen_index_before,'real Chromium index drift during regeneration; refusing to overwrite foreign staging'
  if not rollback:self.check_signal()
  assert code==0,'patch regeneration failed'
 def official_check(self):
  command=[sys.executable,self.root/'mock-official.py']if self.mock else[self.root/'arc/tools/check-file.sh',OWN];code,log=self.run(command,'owned-official-check');assert code==0 and not re.search(r'\b(?:error|warning):',log.read_text(),re.I),'official check diagnostics/nonzero'
 def runtime(self):
  self.check_signal();pins=self.merge_runtime_pins();self.runtime_started=True;result=self.evidence/'runtime-result.json';env={**os.environ,'ARCCORE_SESSIONS_TRANSACTION_RESULT':str(result),'ARCCORE_SESSIONS_TRANSACTION_DIR':str(self.evidence),'ARCCORE_SESSIONS_TRANSACTION_ID':self.id};command=[str(pins)if x=='{runtime_pins}'else x for x in self.args.runtime_command];code,_=self.run(command,'runtime',env=env,runtime=True)
  data=json.loads(result.read_text())if result.exists()else{};self.state['runtimeResult']=data;self.cleanup_verified=data.get('cleanupVerified')is True;self.state['runtimeCleanupVerified']=self.cleanup_verified;self.save();assert code==0 and data.get('code')==0,'runtime command/code failed';assert self.cleanup_verified,'runtime returned without verified cleanup'
  launch=data.get('launch',{});assert launch.get('exit')==0 and launch.get('passed')==9 and launch.get('total')==9 and launch.get('coverageAccepted')is True,'launch not accepted 9/9'
  canonical=data.get('canonical',{});assert canonical.get('validationExit')==0 and canonical.get('noEarlierRegression')is True,'canonical regression validation failed'
  scenarios=canonical.get('scenarios');assert isinstance(scenarios,list)and {x.get('name')for x in scenarios}=={'sessions-public-api','sessions-tab-restart'},'canonical scenario records missing'
  for scenario in scenarios:
   assert all(type(scenario.get(k))is int and scenario[k]>=0 for k in ['harnessExit','passed','failed','inconclusive']),'missing canonical harness statuses'
  self.check_signal()
 def hold(self,reason,predicate):
  self.state['status']='HOLD';self.state['holdReason']=reason;self.best_effort_save();print('HOLDING SHARED LOCK:',reason,'evidence',self.evidence,flush=True)
  while not predicate():time.sleep(2)
  self.state['holdResolvedAt']=stamp();self.best_effort_save()
 def verify_cleanup_or_hold(self):
  if not self.runtime_started or self.cleanup_verified:return
  receipt=self.evidence/'manual-cleanup-confirmation.json'
  def confirmed():
   try:
    d=json.loads(receipt.read_text());return d.get('transactionId')==self.id and d.get('cleanupVerified')is True and isinstance(d.get('verifiedBy'),str)and bool(d['verifiedBy'].strip())and isinstance(d.get('evidence'),str)and bool(d['evidence'].strip())
   except (OSError,ValueError):return False
  self.hold('Runtime cleanup unverified. Independently verify helpers/profile/cache, then write nonce-bound manual-cleanup-confirmation.json; no app kill will be performed.',confirmed);self.cleanup_verified=True
 def rollback(self):
  if not self.live:return
  # Never overwrite an unexpected concurrent edit, even on one of our targets.
  def safe():
   try:return all(self.read_safe(self.current(n))in [self.backup[n],self.expected[n]]for n in TARGETS)and self.read_safe(self.root/'patches/MANIFEST')in [self.manifest_before,self.manifest_after]
   except (OSError,AssertionError):return False
  if not safe():self.hold('Unexpected delta in a transaction target or MANIFEST. Preserve foreign work and manually reconcile to recorded preimage/published image before rollback.',safe)
  patch_expected=self.generated_patch_images or self.patch_backup
  if not self.patches_match(patch_expected):self.hold('Unexpected delta in a maintained patch. Preserve foreign edits and reconcile to recorded preimage/generated image before rollback regeneration.',lambda:self.patches_match(patch_expected))
  for n,v in self.backup.items():self.atomic_write(self.current(n),v)
  self.atomic_write(self.root/'patches/MANIFEST',self.manifest_before)
  if self.orphan_patch:
   path=self.root/'patches'/self.orphan_patch;original=self.patch_backup[self.orphan_patch]
   allowed=[original,self.generated_patch_images.get(self.orphan_patch)]
   def orphan_safe():return (path.read_bytes()if path.exists()else None)in allowed
   if not orphan_safe():self.hold('Unexpected delta in unreferenced own generated patch; reconcile to preimage/generated image before rollback.',orphan_safe)
   if original is None:
    if path.exists():path.unlink()
   else:self.atomic_write(path,original)
  while True:
   try:self.regenerate(rollback=True);break
   except Exception as error:
    self.state['status']='ROLLBACK_REGEN_HOLD';self.state['rollbackError']=failure_text(error);self.save();print('HOLDING LOCK: rollback regen failed; retrying; inspect evidence',flush=True);time.sleep(5)
  assert all(self.read_safe(self.current(n))==v for n,v in self.backup.items());assert self.read_safe(self.root/'patches/MANIFEST')==self.manifest_before
  recovered={n:(self.read_safe(self.root/'patches'/n)if(self.root/'patches'/n).exists()else None)for n in self.patch_backup}
  deltas={n:{'beforeSHA256':digest(self.patch_backup[n])if self.patch_backup[n]is not None else None,'afterSHA256':digest(v)if v is not None else None}for n,v in recovered.items()if v!=self.patch_backup[n]}
  self.state['rollbackMaintainedPatchDeltas']=deltas;self.state['rollbackExactPatchPreimages']=not deltas;self.save()
  if deltas:
   receipt=self.evidence/'manual-rollback-patch-delta-confirmation.json'
   def reviewed():
    try:
     d=json.loads(receipt.read_text());return d.get('transactionId')==self.id and d.get('acceptedCurrentPatchHashes')=={n:v['afterSHA256']for n,v in deltas.items()}and bool(d.get('verifiedBy'))and bool(d.get('evidence'))and all((file_sha(self.root/'patches'/n)if(self.root/'patches'/n).exists()else None)==v['afterSHA256']for n,v in deltas.items())
    except (OSError,ValueError):return False
   self.hold('Regenerated maintained patches differ from entry preimages; review exact preserved deltas against already-live foreign sources and provide nonce-bound manual-rollback-patch-delta-confirmation.json. No foreign patch preimages will be blindly restored.',reviewed)
   self.state['rollbackReviewedForeignPatchDeltas']=deltas
  self.event('rollback-complete');self.live=False
 def execute(self):
  try:
   self.capture();self.stage_changes();self.syntax();self.make_manifest();self.publish();self.regenerate();self.official_check();self.runtime();self.verify_postimages();self.check_signal();self.state['status']='MOCK_APPLIED'if self.mock else'APPLIED';self.state['finishedUTC']=stamp();self.save();self.check_signal();return 0
  except BaseException as error:
   self.state['failure']=failure_text(error);self.state['deferredSignals']=self.pending[:];self.best_effort_save()
   while True:
    try:self.rollback();self.verify_cleanup_or_hold();break
    except BaseException as rollback_error:
     # Keep the process/lock alive and retry after external repair.
     self.state['status']='ROLLBACK_HOLD';self.state['rollbackError']=failure_text(rollback_error);self.best_effort_save();time.sleep(2)
   self.state['status']='ROLLED_BACK'if self.ever_published else'ABORTED_NO_WRITES';self.state['finishedUTC']=stamp();self.best_effort_save();return 1
 def verify_postimages(self):
  self.check_signal();self.verify_frozen_runtime_pins();assert all(file_sha(self.root/n)==h for n,h in self.broad_entry_pins.items()if n!=OWN),'broad entry source drift after runtime';assert self.patches_match(self.generated_patch_images),'post-runtime maintained-patch drift';assert all(self.read_safe(self.current(n))==v for n,v in self.expected.items()),'post-runtime source drift';assert self.read_safe(self.root/'patches/MANIFEST')==self.manifest_after,'post-runtime MANIFEST drift';self.lock_check();self.state['postimages']={n:file_sha(self.current(n))for n in TARGETS};self.state['maintainedPatchPostimages']={p.name:file_sha(p)for p in(self.root/'patches').glob('*.patch')};self.save()

def main():
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--evidence-dir');parser.add_argument('--mock-root');parser.add_argument('--frozen-runtime-pins',required=True);parser.add_argument('--frozen-runtime-pins-sha256',required=True);parser.add_argument('--runtime-command',nargs=argparse.REMAINDER,required=True);args=parser.parse_args();assert args.runtime_command,'runtime command argv required';tx=Transaction(args);code=tx.execute();print(json.dumps({'code':code,'status':tx.state['status'],'evidence':str(tx.evidence)}),flush=True);return code
if __name__=='__main__':sys.exit(main())
