from pathlib import Path
import json,hashlib,subprocess,os,tempfile,difflib,shlex,datetime,concurrent.futures,shutil
root=Path('/Users/cryogenicplanet/general/arc-engine');out=root/'chromium/src/out/Base';evidence=root/'results/sessions/proposals/source-only-adoption-host-gates';evidence.mkdir(parents=True,exist_ok=False);sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest();input_manifest=root/'results/sessions/proposals/current-six-live-base-e973/manifest.json';m0=json.loads(input_manifest.read_text());route=m0['routes']['source-only-live-base-six'];oldstage=Path(route['stage']);stage=Path(tempfile.mkdtemp(prefix='arccore-sessions-source-only-host-gates-',dir='/tmp'));subprocess.run(['git','init','-q',str(stage)],check=True)
for n,h in route['stagedFiles'].items():
 if h is not None:
  assert sha(oldstage/n)==h;npath=stage/n;npath.parent.mkdir(parents=True,exist_ok=True);npath.write_bytes((oldstage/n).read_bytes())
name='components/sessions/core/tab_restore_service_helper.cc';before=(stage/name).read_text();s=before;s=s.replace('#include "base/check_op.h"\n','#include "base/check_op.h"\n#include "base/command_line.h"\n',1)
old='  const auto weak_self = weak_factory_.GetWeakPtr();\n';new='  const bool is_host_app =\n      base::CommandLine::ForCurrentProcess()->HasSwitch("arcmod-host-app");\n  base::WeakPtr<TabRestoreServiceHelper> weak_self;\n  if (is_host_app) {\n    weak_self = weak_factory_.GetWeakPtr();\n  }\n';assert s.count(old)==2;s=s.replace(old,new)
s=s.replace('if (!weak_self)', 'if (is_host_app && !weak_self)')
s=s.replace('      if (!context) {\n        context_creation_cancelled = true;', '      if (is_host_app && !context) {\n        context_creation_cancelled = true;',1)
a='''        auto current_entry = GetEntryIteratorById(id);
        if (!context || current_entry == entries_.end() ||
            current_entry->get() != &entry) {
          context_creation_cancelled = true;
          break;
        }
''';b='''        if (is_host_app) {
          auto current_entry = GetEntryIteratorById(id);
          if (!context || current_entry == entries_.end() ||
              current_entry->get() != &entry) {
            context_creation_cancelled = true;
            break;
          }
        }
''';assert a in s;s=s.replace(a,b,1)
s=s.replace('  if (context_creation_cancelled) {','  if (is_host_app && context_creation_cancelled) {',1)
a='''  const SessionID source_id = tab.id;
  // ROOT TAB application requests may enter embedder code during creation.
  // Descendant/group/split routing remains native and outside this request scope.
  const auto source_entry = GetEntryIteratorById(source_id);
  const Tab* expected_root_tab =
      session_restore_type == tab_restore::TAB && source_entry != entries_.end() &&
              source_entry->get() == &tab && (*source_entry)->type == tab_restore::TAB
          ? &tab
          : nullptr;
  if (live_tab) {
    *live_tab = nullptr;
  }
  LiveTab* restored_tab = nullptr;
''';b='''  std::optional<SessionID> source_id;
  const Tab* expected_root_tab = nullptr;
  if (is_host_app) {
    source_id = tab.id;
    // ROOT TAB application requests may enter embedder code during creation.
    // Descendant/group/split routing remains native and outside this request scope.
    const auto source_entry = GetEntryIteratorById(*source_id);
    if (session_restore_type == tab_restore::TAB &&
        source_entry != entries_.end() && source_entry->get() == &tab &&
        (*source_entry)->type == tab_restore::TAB) {
      expected_root_tab = &tab;
    }
    if (live_tab) {
      *live_tab = nullptr;
    }
  }
  LiveTab* restored_tab;
''';assert a in s;s=s.replace(a,b,1)
a='''      if (!weak_self || !context) {
        return nullptr;
      }
      if (expected_root_tab) {
        auto current_entry = GetEntryIteratorById(source_id);
        if (current_entry == entries_.end() ||
            current_entry->get() != expected_root_tab) {
          return nullptr;
        }
      }
''';b='''      if (is_host_app) {
        if (!weak_self || !context) {
          return nullptr;
        }
        if (expected_root_tab) {
          auto current_entry = GetEntryIteratorById(*source_id);
          if (current_entry == entries_.end() ||
              current_entry->get() != expected_root_tab) {
            return nullptr;
          }
        }
      }
''';assert a in s;s=s.replace(a,b,1)
(stage/name).write_text(s);(evidence/'before.cc').write_text(before);(evidence/'after.cc').write_text(s);patch=''.join(difflib.unified_diff(before.splitlines(True),s.splitlines(True),fromfile='a/'+name,tofile='b/'+name));(evidence/'host-gating.patch').write_text(patch)
res=subprocess.run(['git','-C',str(oldstage),'apply','--check','--verbose',str(evidence/'host-gating.patch')],capture_output=True);(evidence/'apply-check.log').write_bytes(res.stdout+res.stderr);assert res.returncode==0
# The source-only six-stage is immutable. Apply the additive patch only to a separate fresh copy for proof.
proof=Path(tempfile.mkdtemp(prefix='arccore-sessions-source-only-host-gates-apply-',dir='/tmp'));subprocess.run(['git','init','-q',str(proof)],check=True);f=proof/name;f.parent.mkdir(parents=True,exist_ok=True);f.write_text(before);res=subprocess.run(['git','-C',str(proof),'apply','--check',str(evidence/'host-gating.patch')],capture_output=True);assert res.returncode==0;res=subprocess.run(['git','-C',str(proof),'apply',str(evidence/'host-gating.patch')],capture_output=True);assert res.returncode==0 and f.read_text()==s
m={'scope':'PRIVATE additive host gating only against exact current source-only six stage. No production/native/shared/tests/MANIFEST/app/build/lock/Chirp changes; no policy/accounting/root or acceptance claim.','startedUTC':datetime.datetime.now(datetime.timezone.utc).isoformat(),'inputManifest':{'path':str(input_manifest),'sha256':sha(input_manifest)},'inputStage':str(oldstage),'inputStageSHA256':route['stagedFiles'],'stage':str(stage),'changedFile':name,'beforeSHA256':hashlib.sha256(before.encode()).hexdigest(),'afterSHA256':sha(stage/name),'patchSHA256':sha(evidence/'host-gating.patch'),'applyCheckExit':0,'privateApplyByteExact':True,'productionBaseline':m0['productionBaseline'],'originalPatches':m0['frozenPatches'],'compiler':{}}
files={'helper':('obj/components/sessions/shared/tab_restore_service_helper.o',name),'live':('obj/chrome/browser/ui/browser_live_tab_context_impl/browser_live_tab_context.o','chrome/browser/ui/browser_live_tab_context.cc'),'restore':('obj/chrome/browser/ui/browser_tabrestore_impl/browser_tabrestore.o','chrome/browser/ui/browser_tabrestore.cc'),'owned':('obj/arc/arc_core/arc_sessions_restore.o','arc/api/arc_sessions_restore.mm')};env={**os.environ,'PATH':str(root/'depot_tools')+':'+os.environ['PATH']}
def compile_one(key,value):
 target,n=value;q=subprocess.run(['siso','query','commands','-C','.',target],cwd=out,env=env,capture_output=True);q.check_returncode();original=shlex.split(next(x for x in reversed(q.stdout.decode(errors='replace').splitlines())if 'clang'in x and ' -c 'in x and n in x));clean=[];removed=[];skip=False;count=0
 for a in original:
  if skip:removed.append(a);skip=False;continue
  if a in ['-o','-MF','-MT','-MQ','-serialize-diagnostics','-dependency-file']:removed.append(a);skip=True;continue
  if a in ['-c','-MMD','-MD','-MP']or a.startswith('-fcrash-diagnostics-dir='):removed.append(a);continue
  if a.endswith(n):clean.append(str(stage/n));count+=1
  else:clean.append(a)
 assert count==1;clean[1:1]=['-I'+str(stage)];clean+=['-fsyntax-only','-Wno-unused-command-line-argument','-fcrash-diagnostics-dir='+tempfile.mkdtemp(prefix='arccore-session-host-gates-crash-',dir='/tmp')];assert '-Werror'in clean and '-c'not in clean and '-o'not in clean;res=subprocess.run(clean,cwd=out,env=env,capture_output=True);log=evidence/(key+'.syntax.log');log.write_bytes(res.stdout+res.stderr);print(key,'syntax',res.returncode,'bytes',log.stat().st_size,flush=True);return key,{'source':n,'target':target,'queryExit':q.returncode,'originalCommand':original,'command':clean,'removedFlags':removed,'cwd':str(out),'exit':res.returncode,'log':str(log),'logSHA256':sha(log)}
with concurrent.futures.ThreadPoolExecutor(max_workers=4)as pool:
 for key,result in pool.map(lambda kv:compile_one(*kv),files.items()):m['compiler'][key]=result
m['productionInputsUnchanged']={n:(sha(Path(v['path']))if Path(v['path']).exists()else None)==v['sha256']for n,v in m['productionBaseline'].items()};m['originalPatchesUnchanged']={n:sha(v['path'])==v['sha256']for n,v in m['originalPatches'].items()};m['inputManifestUnchanged']=sha(input_manifest)==m['inputManifest']['sha256'];m['originalStageUnchanged']={n:(sha(oldstage/n)if(oldstage/n).exists()else None)==h for n,h in route['stagedFiles'].items()};m['stagedOnlyHelperChanged']={n:(sha(stage/n)==(m['afterSHA256']if n==name else h))for n,h in route['stagedFiles'].items()if h is not None};m['finishedUTC']=datetime.datetime.now(datetime.timezone.utc).isoformat();m['pass']=all(v['exit']==0 for v in m['compiler'].values())and all(m['productionInputsUnchanged'].values())and all(m['originalPatchesUnchanged'].values())and all(m['originalStageUnchanged'].values())and all(m['stagedOnlyHelperChanged'].values())and m['inputManifestUnchanged'];(evidence/'manifest.json').write_text(json.dumps(m,indent=2)+'\n');shutil.copyfile(__file__,evidence/'check.py');print('PASS',m['pass'],'patch',m['patchSHA256'],flush=True)
