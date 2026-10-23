from pathlib import Path
import subprocess,os,shlex,json,hashlib,re,datetime,tempfile
root=Path('/Users/cryogenicplanet/general/arc-engine');out=root/'chromium/src/out/Base';evidence=root/'results/sessions/proposals/current-six-live-base-e973';evidence.mkdir(exist_ok=False,parents=True)
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
sessions_manifest=root/'results/sessions/proposals/current-composition-20261023T1423Z/manifest.json';prior_manifest=root/'results/sessions/proposals/current-six-compat-e973/manifest.json'
sm=json.loads(sessions_manifest.read_text())
sess={label:[('sessions:'+x['patch'],Path(sm['frozenPatches'][x['patch']]['path']),sm['frozenPatches'][x['patch']]['sha256'])for x in sm['alternatives'][label]['steps'][:6]]for label in ['source-only','source-policy']}
assert all(len(v)==6 for v in sess.values())
assert all('close-accounting' not in n for chain in sess.values()for n,p,h in chain)
patches={name:(path,digest)for name,path,digest in [x for chain in sess.values()for x in chain]}
for n,(p,h)in patches.items():assert sha(p)==h,(n,'patch hash drift')
def current(name):return root/name if name.startswith('arc/')else root/'chromium/src'/name
expected={'arc/api/arc_sessions_restore.mm':'e973b371ff612fcb85132ee992af900937f92fbc325d06a97ec9e4b17a9c668d','arc/api/arc_web_contents.mm':'47534304f93ff381dbc00e1a080d7425caac808c6dd679825677f93c97201927','arc/api/arc_internal.h':'70495f85414c996dc97262cc9670a9464451e9e215600c934cc71e46269a62a1','chrome/browser/arcmod/arcmod_hooks.h':'a6bb3a03f67ee0e66394ccad379916cce571cfddcfa0e880d76ac8a6ff6b14d2'}
assert not (root/'arc/browser/core_close_adapters.h').exists()
assert not (root/'arc/browser/core_close_state.h').exists()
for n,h in expected.items():assert sha(current(n))==h,(n,'requested base drift')
names=set(expected)
for path,h in patches.values():names.update(re.findall(r'^\+\+\+ b/(.+)$',path.read_text(),re.M))
base={n:current(n).read_bytes()if current(n).exists()else None for n in sorted(names)}
owned='arc/api/arc_sessions_restore.mm';currentOwned=base[owned].decode()
def region(s,a,b):return s[s.index(a):s.index(b,s.index(a))]
setter=region(currentOwned,'- (void)setNavigations:','- (NSString*)groupId');callback=region(currentOwned,'void QueueRestorationCallback(','\nclass ScopedSessionInsertion');discard=region(currentOwned,'@implementation ArcWebContents (SessionsDiscard)','@implementation SessionsRestoreIDs')
def guards(stage):
 s=(stage/owned).read_text();cb=region(s,'void QueueRestorationCallback(','\nclass ScopedSessionInsertion');st=region(s,'- (void)setNavigations:','- (NSString*)groupId');dc=region(s,'@implementation ArcWebContents (SessionsDiscard)','@implementation SessionsRestoreIDs')
 result={'setterByteExactCurrent':st==setter,'navigationNonemptyGuard':st.count('CHECK_GT(entries.size(), 0u);')==1,'arrayPublicationBeforeEnumeration':st.index('_navigations = navigations;')<st.index('for (ArcSessionNavigation* navigation in navigations)'),'noIncomingCountCallout':'navigations.count'not in st,'callbackByteExactCurrent':cb==callback,'currentBindingGuard':'[wrapper webContents]'in cb,'noOldWebContentsWeakCapture':'nativeContents'not in cb and 'contents->GetWeakPtr()'not in cb,'discardByteExactCurrent':dc==discard,'preciseSelectedAdapterLifetime':'__attribute__((objc_precise_lifetime)) SessionsDiscardAdapter* selectedAdapter = weakAdapter;'in dc,'selectedAdapterDelegate':'delegate = selectedAdapter.delegate;'in dc}
 assert all(result.values()),result
 return result
m={'scope':'PRIVATE live-base independence compatibility/owned syntax ONLY. Two original six sessions routes; NO core11, after-core11 variants, accounting, logical root or private core headers. No production/shared/test changes, object/nativeTU/build/link/app/runtime/lock/Chirp/credentials/adoption/functional/publicClose/acceptance claim.','startedUTC':datetime.datetime.now(datetime.timezone.utc).isoformat(),'inputManifests':{str(p):sha(p)for p in [sessions_manifest,prior_manifest]},'productionBaseline':{n:{'path':str(current(n)),'sha256':hashlib.sha256(data).hexdigest()if data is not None else None,'size':len(data)if data is not None else None}for n,data in base.items()},'frozenPatches':{n:{'path':str(p),'sha256':h}for n,(p,h)in patches.items()},'omittedAccounting':sm['frozenPatches']['close-accounting'],'routes':{}}
def save():(evidence/'manifest.json').write_text(json.dumps(m,indent=2)+'\n')
save()
for label,session_chain in sess.items():
 name=label+'-live-base-six';chain=session_chain;stage=Path(tempfile.mkdtemp(prefix='arccore-sessions-current-six-live-base-e973-'+label+'-',dir='/tmp'));subprocess.run(['git','init','-q',str(stage)],check=True)
 for n,data in base.items():
  if data is not None:p=stage/n;p.parent.mkdir(exist_ok=True,parents=True);p.write_bytes(data)
 route={'stage':str(stage),'order':[n for n,p,h in chain],'steps':[]};m['routes'][name]=route
 for idx,(n,p,h)in enumerate(chain,1):
  log=evidence/(name+'-'+str(idx)+'.apply.log');res=subprocess.run(['git','-C',str(stage),'apply','--check','--verbose',str(p)],capture_output=True);log.write_bytes(res.stdout+res.stderr);step={'patch':n,'applyCheckExit':res.returncode,'log':str(log),'beforeGuards':guards(stage)};route['steps'].append(step)
  if res.returncode:route['failurePatch']=n;route['failureDiagnostic']=(res.stdout+res.stderr).decode();break
  res=subprocess.run(['git','-C',str(stage),'apply',str(p)],capture_output=True);log.write_bytes(log.read_bytes()+res.stdout+res.stderr);step['applyExit']=res.returncode
  if res.returncode:route['failurePatch']=n;route['failureDiagnostic']=(res.stdout+res.stderr).decode();break
  step['afterGuards']=guards(stage);step['afterOwnedSHA256']=sha(stage/owned)
 route['complete']=len(route['steps'])==6 and all(x.get('applyExit')==0 for x in route['steps']);route['stagedFiles']={n:sha(stage/n)if(stage/n).exists()else None for n in base};route['finalGuards']=guards(stage);print(name,'complete',route['complete'],'steps',len(route['steps']),'failure',route.get('failurePatch'),flush=True);save()
complete=[(n,i)for n,i in m['routes'].items()if i['complete']]
if complete:
 env={**os.environ,'PATH':str(root/'depot_tools')+':'+os.environ['PATH']};q=subprocess.run(['siso','query','commands','-C','.','obj/arc/arc_core/arc_sessions_restore.o'],cwd=out,env=env,capture_output=True);m['sisoQueryExit']=q.returncode
 if q.returncode:(evidence/'siso-query-error.log').write_bytes(q.stderr)
 else:
  command=next(x for x in reversed(q.stdout.decode(errors='replace').splitlines())if 'clang'in x and ' -c 'in x and owned in x);original=shlex.split(command);(evidence/'owned-original-compiler-command.json').write_text(json.dumps(original,indent=2)+'\n')
  for name,item in complete:
   stage=Path(item['stage']);clean=[];removed=[];skip=False;replaced=0
   for a in original:
    if skip:removed.append(a);skip=False;continue
    if a in ['-o','-MF','-MT','-MQ','-serialize-diagnostics','-dependency-file']:removed.append(a);skip=True;continue
    if a in ['-c','-MMD','-MD','-MP']or a.startswith('-fcrash-diagnostics-dir='):removed.append(a);continue
    if a.endswith(owned):clean.append(str(stage/owned));replaced+=1
    else:clean.append(a)
   assert replaced==1;clean[1:1]=['-I'+str(stage)];crash=Path(tempfile.mkdtemp(prefix='arccore-sessions-current-six-live-base-e973-crash-',dir='/tmp'));clean+=['-fsyntax-only','-Wno-unused-command-line-argument','-fcrash-diagnostics-dir='+str(crash)];assert '-Werror'in clean and all(x not in clean for x in ['-c','-o','-MF','-MT','-MQ','-MMD','-MD','-dependency-file'])
   item['compilerCommand']=clean;item['removedOutputFlags']=removed;item['compilerCWD']=str(out);res=subprocess.run(clean,cwd=out,env=env,capture_output=True);log=evidence/(name+'.owned.syntax.log');log.write_bytes(res.stdout+res.stderr);item['syntaxExit']=res.returncode;item['syntaxLog']=str(log);item['syntaxLogSHA256']=sha(log);print(name,'owned syntax',res.returncode,'logbytes',log.stat().st_size,flush=True);save()
else:m['syntaxSkippedReason']='No complete six-patch route; syntax not run.'
m['productionBaselineUnchanged']={n:(current(n).read_bytes()if current(n).exists()else None)==data for n,data in base.items()};m['frozenPatchesUnchanged']={n:sha(p)==h for n,(p,h)in patches.items()};m['inputManifestsUnchanged']={str(p):sha(p)==h for p,h in [(Path(n),h)for n,h in m['inputManifests'].items()]};m['finishedUTC']=datetime.datetime.now(datetime.timezone.utc).isoformat();m['privateOnlyChecksPass']=all(m['productionBaselineUnchanged'].values())and all(m['frozenPatchesUnchanged'].values())and all(m['inputManifestsUnchanged'].values());save();print('PRODUCTION/FROZENUNCHANGED',m['privateOnlyChecksPass'],flush=True)
