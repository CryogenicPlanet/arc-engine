from pathlib import Path
import collections,hashlib,importlib.util,json,shutil,subprocess,sys
ROOT=Path('/Users/cryogenicplanet/general/arc-engine');OUT=Path(__file__).parent.resolve()
assert OUT==ROOT/'results/extensions-validation5/triple-normalizer-diagnostic'
# The parent created this directory with mkdir(exist_ok=False). Never overwrite
# an existing capture or any raw/source input when replaying this script.
assert not (OUT/'provenance.json').exists(), 'Refusing existing diagnostic capture'
assert not (OUT/'tool').exists(), 'Refusing existing private comparator'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
inputs={}
def snapshot(source,dest):
 source=ROOT/source;dest=OUT/dest
 assert not dest.exists(),f'Refusing overwrite: {dest}'
 h=sha(source);dest.parent.mkdir(parents=True,exist_ok=True);shutil.copy2(source,dest);assert sha(dest)==h
 inputs[str(dest.relative_to(OUT))]={'source':str(source.relative_to(ROOT)),'sha256':h}
 return dest
staged='logs/wp/extensions-normalization-after-fixture-epoch/v1/after/'
for relative in ('tests/recorder/diff.py','arc/DIFF-ALLOWLIST.md','arc/tools/arccore-1.170.0-objc.tsv','tests/recorder/app_callbacks.inc'):
 snapshot(staged+relative,'tool/'+relative)
assert sha(OUT/'tool/tests/recorder/diff.py')=='63c0055e6c44ff5b27c45d69bb91f55a471bcaa20761f27d780fcb1779b6eb08'
assert sha(OUT/'tool/arc/tools/arccore-1.170.0-objc.tsv')==sha(ROOT/'arc/tools/arccore-1.170.0-objc.tsv')
assert sha(OUT/'tool/tests/recorder/app_callbacks.inc')==sha(ROOT/'tests/recorder/app_callbacks.inc')
spec=importlib.util.spec_from_file_location('private_triple',OUT/'tool/tests/recorder/diff.py');tool=importlib.util.module_from_spec(spec);spec.loader.exec_module(tool)
classes={'ArcBrowserContextExtensions','ArcExtensionInfo','ArcExtensionCommand','ArcExtensionRuntimeError','ArcExtensionRuntimeErrorStackFrame','ArcExtensionToolbarAction'}
browser_selectors={'executeExtensionAction:','pinExtension:','unpinExtension:','showExtensionContextMenu:','moveExtension:targetIndex:','triggerExtensionOmniboxInput:inputText:disposition:'}
group_selectors={'browser:createTabGroupWithSessionIds:properties:completion:','browser:addTabsToGroupWithId:sessionIds:completion:','browser:removeTabsFromGroupWithSessionIds:completion:','browser:updateTabGroupWithId:properties:completion:','browser:moveTabGroupWithId:toNewIndex:completion:','browser:getTabGroupWithId:completion:','browser:getAllTabGroupsWithCompletion:','browser:extensionGroupIdForTabWithSessionId:'}
def owned_key(key):
 direction,kind,cls,sel=key
 if direction=='->engine':return cls in classes or (cls=='ArcBrowser' and sel in browser_selectors)
 return direction=='engine->app' and (('Extension' in sel) or sel in group_selectors or sel=='browserContextDidExtensionSystemReady:' or 'BrowserExtensionsLifeCycleManager' in cls)
def key(row):return tuple(row.get(x) for x in ('dir','kind','cls','sel'))
def method(k):return '%s %s%s %s'%(k[0],k[1],k[2],k[3])
def rows(path):return [json.loads(line) for line in path.read_text().splitlines()]
def version(events):
 boundary=[e for e in events if e.get('ev') in ('call','ret')]
 attributed=sum(isinstance(e.get('from'),str) and bool(e['from']) for e in boundary)
 return {'version':'v2-attributed' if boundary and attributed==len(boundary) else 'v1-or-mixed','boundary_rows':len(boundary),'attributed_rows':attributed,'missing_or_invalid_caller_rows':len(boundary)-attributed}
def selected(events,bycaller):
 return [e for e in events if not (bycaller and e.get('dir')=='->engine' and e.get('from')!='app' and not(e.get('from')=='engine' and e.get('sel','').startswith('set')))]
def session_trace(events,bycaller):
 aliases={};entries=[]
 for e in selected(events,bycaller):
  k=key(e);field=None;v=None
  if k==tool.SESSION_IDENTIFIER and e.get('ev')=='ret':field='return';v=e.get('ret')
  if k==tool.SESSION_GROUP_CALLBACK and e.get('ev')=='call' and isinstance(e.get('args'),list) and len(e['args'])>1:field='args[1]';v=e['args'][1]
  if field:
   if type(v) is int and 0<v<=0x7fffffff:
    if v not in aliases:aliases[v]=len(aliases)+1
    normalized={'native_session_id':aliases[v]}
   else:normalized={'native_session_id_literal':v}
   entries.append({'seq':e.get('seq'),'field':field,'method':method(k),'raw':v,'normalized':normalized,'from':e.get('from'),'self':e.get('self')})
 return {'distinct_positive_ids':len(aliases),'entries':entries}
summary={}
for scenario,launches in (('launch-slice',(1,2)),('extensions-page-actions',(1,))):
 for launch in launches:
  name=f'{scenario}/launch-{launch}'
  gp=snapshot(f'results/golden/arc-1.170-{scenario}/launch-{launch}/record.jsonl',f'{name}/golden.jsonl')
  op=snapshot(f'results/extensions-validation5/{scenario}/launch-{launch}/record.jsonl',f'{name}/ours.jsonl')
  raw={'golden':rows(gp),'ours':rows(op)};versions={side:version(data) for side,data in raw.items()}
  detail={'scope':'Historical recomparison, no new runtime or acceptance claim','recorder':versions}
  if scenario=='extensions-page-actions' and any(v['version']!='v2-attributed' for v in versions.values()):
   detail['status']='NOT COMPARED: current page-actions golden is v1-or-mixed; v1 fallback cannot provide paired caller-attributed coverage'
   summary[name]=detail;continue
  assert all(v['version']=='v2-attributed' for v in versions.values()),versions
  target=OUT/name/'diff.json';command=[sys.executable,'-B',str(OUT/'tool/tests/recorder/diff.py'),str(gp),str(op),'--json',str(target)]
  run=subprocess.run(command,capture_output=True,text=True);assert run.returncode==0,run.stderr
  (OUT/name/'diff.txt').write_text(run.stdout);(OUT/name/'diff.stderr.txt').write_text(run.stderr)
  first2=run.stdout.splitlines()[:2];(OUT/name/'first-two-lines.txt').write_text('\n'.join(first2)+'\n')
  assert 'caller attribution: on (app calls + engine setters)' in run.stdout
  report=json.loads(target.read_text());scope={};inventory={};callback_sequences={};session={}
  for side,p in (('golden',gp),('ours',op)):
   calls,callbacks=tool.load(p,True);retained=selected(raw[side],True)
   inventory[side]=[{'method':method(k),'calls':len(calls[k]),'records':calls[k]} for k in sorted(calls) if owned_key(k)]
   callback_sequences[side]=[{'seq':e.get('seq'),'method':method(key(e)),'args':e.get('args'),'ret':None} for e in retained if e.get('ev')=='call' and e.get('dir')=='engine->app' and owned_key(key(e))]
   session[side]=session_trace(raw[side],True)
  method_names={item['method'] for values in inventory.values() for item in values}
  for section,items in report.items():
   if isinstance(items,list):scope[section]=[item for item in items if item.get('method') in method_names]
  (OUT/name/'extension-owned-report.json').write_text(json.dumps(scope,indent=2))
  (OUT/name/'extension-owned-method-inventory.json').write_text(json.dumps(inventory,indent=2))
  (OUT/name/'extension-owned-callback-sequences.json').write_text(json.dumps(callback_sequences,indent=2))
  (OUT/name/'session-id-evidence.json').write_text(json.dumps(session,indent=2))
  public={k:v for k,v in scope.items() if k not in ('internal','internal_ours') and v}
  detail.update({'status':'Compared with caller attribution ON, no fixture-image opt-ins','first_two_lines':first2,'global_totals':{k:len(v) for k,v in report.items() if isinstance(v,list)},'global_callback_sequence':report['callback_sequence'],'extension_public_residuals':public,'extension_internal_classification':{k:scope.get(k,[]) for k in ('internal','internal_ours')},'extension_callback_counts':{k:len(v) for k,v in callback_sequences.items()},'extension_callback_method_order_equal':[r['method'] for r in callback_sequences['golden']]==[r['method'] for r in callback_sequences['ours']],'session_target_residuals':{k:[x for x in v if x.get('method') in (method(tool.SESSION_IDENTIFIER),method(tool.SESSION_GROUP_CALLBACK))] for k,v in report.items() if isinstance(v,list)}})
  summary[name]=detail
for dest,meta in inputs.items():
 assert sha(ROOT/meta['source'])==meta['sha256'],f'Source changed during diagnostic: {meta}'
 assert sha(OUT/dest)==meta['sha256'],f'Copied input changed: {dest}'
(OUT/'summary.json').write_text(json.dumps(summary,indent=2))
(OUT/'provenance.json').write_text(json.dumps({'scope':'Historical recomparison only; no new runtime, product improvement, acceptance, or fixture coverage claim','toolSHA256':sha(OUT/'tool/tests/recorder/diff.py'),'inputs':inputs,'all_source_and_copy_hashes_reverified':True,'fixture_images':[]},indent=2))
print(json.dumps(summary,indent=2))
