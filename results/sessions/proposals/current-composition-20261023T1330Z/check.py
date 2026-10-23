from pathlib import Path
import subprocess,os,shlex,json,hashlib,re,datetime
root=Path('/Users/cryogenicplanet/general/arc-engine');out=root/'chromium/src/out/Base'
prefix=Path('/tmp/arccore-sessions-current-composition')
evidence=root/'results/sessions/proposals/current-composition-20261023T1330Z'
evidence.mkdir(parents=True,exist_ok=False)
env={**os.environ,'PATH':str(root/'depot_tools')+':'+os.environ['PATH']}
chains={
 'source-only':['native-source-hooks','source-registration','factory-native','factory-owned','tab-factory-native','tab-factory-owned'],
 'source-policy':['native-source-policy','source-policy-registration','factory-native-after-source-policy','factory-owned-after-source-policy','tab-factory-native','tab-factory-owned']}
patches={n:Path('/tmp/arccore-sessions-'+n+'.patch') for chain in chains.values() for n in chain}
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
owned='arc/api/arc_sessions_restore.mm';internal='arc/api/arc_internal.h'
assert sha(root/owned)=='b7c5c201a2395ecf9cd1bd736958432ed42076f2b970cdbaf627fd18e45e5206','owned baseline drift'
for manifest in ['/tmp/arccore-sessions-factory-SHA256SUMS','/tmp/arccore-sessions-tab-factory-SHA256SUMS']:
 for line in Path(manifest).read_text().splitlines():
  digest,path=line.split(None,1);assert sha(Path(path.strip()))==digest,path
names={owned,internal}
for p in patches.values():names.update(re.findall(r'^\+\+\+ b/(.+)$',p.read_text(),re.M))
def current(name):return root/name if name.startswith('arc/') else root/'chromium/src'/name
base={name:current(name).read_bytes() for name in sorted(names)}
manifest={'scope':'Compatibility and owned syntax only. No object/build/app/runtime/lock/shared source changes; frozen inputs unchanged.','startedAtUTC':datetime.datetime.now(datetime.timezone.utc).isoformat(),'baseline':{name:{'path':str(current(name)),'sha256':hashlib.sha256(data).hexdigest(),'size':len(data)} for name,data in base.items()},'frozenPatches':{n:{'path':str(p),'sha256':sha(p)} for n,p in patches.items()},'alternatives':{}}
(evidence/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
def guards(stage):
 s=(stage/owned).read_text();start=s.index('void QueueRestorationCallback(');end=s.index('\nclass ScopedSessionInsertion',start);body=s[start:end]
 result={'navigationNonemptyGuard':s.count('CHECK_GT(navigations.count, 0u);')==1,'currentBindingGuard':'[wrapper webContents]' in body,'noOldWebContentsWeakCapture':'nativeContents' not in body and 'contents->GetWeakPtr()' not in body,'policyDeclaration': 'void ArcSessionsConfigureRestorationPolicy(NSUInteger policy);' in (stage/internal).read_text()}
 assert result['navigationNonemptyGuard'] and result['currentBindingGuard'] and result['noOldWebContentsWeakCapture'],result
 return result
for label,chain in chains.items():
 stage=Path(str(prefix)+'-'+label+'-stage');stage.mkdir(exist_ok=False)
 for name,data in base.items():
  p=stage/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(data)
 subprocess.run(['git','init','-q',str(stage)],check=True)
 item={'stage':str(stage),'steps':[]};manifest['alternatives'][label]=item
 for index,n in enumerate(chain):
  checked=subprocess.run(['git','-C',str(stage),'apply','--check',str(patches[n])],capture_output=True,text=True)
  log=evidence/(label+'-'+str(index+1)+'-'+n+'.apply.log');log.write_text(checked.stdout+checked.stderr)
  step={'patch':n,'applyCheckExit':checked.returncode,'log':str(log),'beforeGuards':guards(stage)};item['steps'].append(step)
  if checked.returncode:break
  applied=subprocess.run(['git','-C',str(stage),'apply',str(patches[n])],capture_output=True,text=True);step['applyExit']=applied.returncode
  with log.open('a') as f:f.write(applied.stdout+applied.stderr)
  if applied.returncode:break
  step['afterGuards']=guards(stage)
  step['afterOwnedSHA256']=sha(stage/owned)
 item['complete']=len(item['steps'])==len(chain) and all(s.get('applyExit')==0 for s in item['steps'])
 item['stagedFiles']={name:sha(stage/name) for name in base}
 print(label,'applychain',item['complete'],flush=True)
 (evidence/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
# Read Siso's actual command graph, but keep only the requested owned compiler command.
q=subprocess.run(['siso','query','commands','-C','.','obj/arc/arc_core/arc_sessions_restore.o'],cwd=out,env=env,capture_output=True)
manifest['sisoQueryExit']=q.returncode
if q.returncode: (evidence/'siso-query-error.log').write_bytes(q.stderr)
else:
 command=next(x for x in reversed(q.stdout.decode(errors='replace').splitlines()) if 'clang' in x and ' -c ' in x and owned in x)
 original=shlex.split(command);(evidence/'owned-original-compiler-command.json').write_text(json.dumps(original,indent=2)+'\n')
 for label,item in manifest['alternatives'].items():
  if not item['complete']:continue
  stage=Path(item['stage']);clean=[];skip=False;sourceReplacements=0;removed=[]
  for a in original:
   if skip:removed.append(a);skip=False;continue
   if a in ['-o','-MF','-MT','-MQ','-serialize-diagnostics','-dependency-file']:
    removed.append(a);skip=True;continue
   if a in ['-c','-MMD','-MD','-MP'] or a.startswith('-fcrash-diagnostics-dir='):
    removed.append(a);continue
   if a.endswith(owned):clean.append(str(stage/owned));sourceReplacements+=1
   else:clean.append(a)
  assert sourceReplacements==1
  clean[1:1]=['-I'+str(stage)]
  crash=Path(str(prefix)+'-'+label+'-crashreports');crash.mkdir(exist_ok=False)
  clean+=['-fsyntax-only','-Wno-unused-command-line-argument','-fcrash-diagnostics-dir='+str(crash)]
  assert '-Werror' in clean and '-c' not in clean and '-o' not in clean and '-MF' not in clean
  item['compilerCommand']=clean;item['removedOutputFlags']=removed
  log=evidence/(label+'-owned.syntax.log')
  result=subprocess.run(clean,cwd=out,env=env,capture_output=True);log.write_bytes(result.stdout+result.stderr)
  item['syntaxExit']=result.returncode;item['syntaxLog']=str(log);item['syntaxLogSHA256']=sha(log)
  print(label,'owned syntax',result.returncode,'logbytes',log.stat().st_size,flush=True)
manifest['baselineUnchanged']={name:sha(current(name))==hashlib.sha256(data).hexdigest() for name,data in base.items()}
manifest['frozenPatchesUnchanged']={name:sha(p)==manifest['frozenPatches'][name]['sha256'] for name,p in patches.items()}
manifest['finishedAtUTC']=datetime.datetime.now(datetime.timezone.utc).isoformat()
manifest['pass']=q.returncode==0 and all(i['complete'] and i.get('syntaxExit')==0 for i in manifest['alternatives'].values()) and all(manifest['baselineUnchanged'].values()) and all(manifest['frozenPatchesUnchanged'].values())
(evidence/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
print('PASS',manifest['pass'],'EVIDENCE',evidence,flush=True)
