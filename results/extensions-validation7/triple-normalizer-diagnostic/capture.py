from pathlib import Path
import collections,hashlib,importlib.util,json,os,shutil,subprocess,sys
ROOT=Path('/Users/cryogenicplanet/general/arc-engine');OUT=Path(__file__).parent.resolve()
assert OUT==ROOT/'results/extensions-validation7/triple-normalizer-diagnostic'
# The creator uses mkdir(exist_ok=False). This second exclusive guard also
# rejects rerunning the copied script before reading/copying any source.
with (OUT/'capture-exclusive.guard').open('x') as guard:guard.write('Private validation7 diagnostic; never overwrite/reuse this capture.\n')
assert not (OUT/'provenance.json').exists() and not (OUT/'tool').exists()
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
inputs={}
def snapshot(source,destination):
 source=ROOT/source;dest=OUT/destination;assert not dest.exists(),f'Refuse overwrite {dest}';before=source.stat();raw=source.read_bytes();digest=hashlib.sha256(raw).hexdigest();dest.parent.mkdir(parents=True,exist_ok=True);dest.write_bytes(raw);after=source.stat();assert before.st_size==after.st_size==len(raw) and before.st_mtime_ns==after.st_mtime_ns and sha(source)==digest,'source changed while capturing';assert sha(dest)==digest
 inputs[str(dest.relative_to(OUT))]={'source':str(source.relative_to(ROOT)),'sha256':digest,'size':len(raw),'source_mtime_ns':after.st_mtime_ns};return dest
staged='logs/wp/extensions-normalization-after-fixture-epoch/v1/after/'
for relative in ['tests/recorder/diff.py','arc/DIFF-ALLOWLIST.md','arc/tools/arccore-1.170.0-objc.tsv','tests/recorder/app_callbacks.inc']:
 snapshot(staged+relative,'tool/'+relative);snapshot(relative,'actual-shared-tool-snapshot/'+relative)
assert sha(OUT/'tool/tests/recorder/diff.py')=='63c0055e6c44ff5b27c45d69bb91f55a471bcaa20761f27d780fcb1779b6eb08'
for relative in ['arc/tools/arccore-1.170.0-objc.tsv','tests/recorder/app_callbacks.inc']:assert sha(OUT/'tool'/relative)==sha(ROOT/relative)
for relative in ['snapshot-before.json','snapshot-after.json']:snapshot('results/extensions-validation7/'+relative,'official-runtime-snapshots/'+relative)
spec=importlib.util.spec_from_file_location('private_triple_v7',OUT/'tool/tests/recorder/diff.py');tool=importlib.util.module_from_spec(spec);spec.loader.exec_module(tool)
classes={'ArcBrowserContextExtensions','ArcExtensionInfo','ArcExtensionCommand','ArcExtensionRuntimeError','ArcExtensionRuntimeErrorStackFrame','ArcExtensionToolbarAction'}
browser_selectors={'executeExtensionAction:','pinExtension:','unpinExtension:','showExtensionContextMenu:','moveExtension:targetIndex:','triggerExtensionOmniboxInput:inputText:disposition:'}
group_selectors={'browser:createTabGroupWithSessionIds:properties:completion:','browser:addTabsToGroupWithId:sessionIds:completion:','browser:removeTabsFromGroupWithSessionIds:completion:','browser:updateTabGroupWithId:properties:completion:','browser:moveTabGroupWithId:toNewIndex:completion:','browser:getTabGroupWithId:completion:','browser:getAllTabGroupsWithCompletion:','browser:extensionGroupIdForTabWithSessionId:'}
context_callbacks={'browserContext:didEnableExtension:','browserContext:didDisableExtension:','browserContext:didInstallExtension:','browserContext:didUninstallExtension:','browserContext:didFailLoadUnpacked:withManifestError:','browserContext:didFail:withRuntimeError:','browserContext:didFail:withManifestError:','browserContext:didFail:withInternalError:','browserContext:didChangePinnedExtensionOrder:','browserContextDidExtensionSystemReady:'}
def key(event):return tuple(event.get(field) for field in ['dir','kind','cls','sel'])
def method(value):return '%s %s%s %s'%(value[0],value[1],value[2],value[3])
def owned(value):
 direction,kind,cls,selector=value
 if direction=='->engine':return cls in classes or cls=='ArcBrowser' and selector in browser_selectors
 return direction=='engine->app' and ('Extension' in selector or selector in group_selectors or selector in context_callbacks or 'BrowserExtensionsLifeCycleManager' in cls)
def rows(path):return [json.loads(line) for line in path.read_text().splitlines()]
def selected(events):return [event for event in events if not(event.get('dir')=='->engine' and event.get('from')!='app' and not(event.get('from')=='engine' and event.get('sel','').startswith('set')))]
def version(events):
 boundary=[e for e in events if e.get('ev') in ['call','ret']];attributed=sum(isinstance(e.get('from'),str) and bool(e['from']) and e['from'].strip()==e['from'] and all(ord(c)>=32 for c in e['from']) for e in boundary);return {'version':'v2-attributed' if boundary and attributed==len(boundary) else 'v1-or-mixed','boundary_rows':len(boundary),'attributed_rows':attributed,'invalid_caller_rows':len(boundary)-attributed}
def session_trace(events):
 aliases={};entries=[];pending=0;unpaired=[]
 for e in selected(events):
  value=None;field=None;k=key(e)
  if k==tool.SESSION_IDENTIFIER:
   if e.get('ev')=='call':pending+=1
   if e.get('ev')=='ret':
    if not pending:unpaired.append(e);continue
    pending-=1;field='return';value=e.get('ret')
  if k==tool.SESSION_GROUP_CALLBACK and e.get('ev')=='call' and isinstance(e.get('args'),list) and len(e['args'])>1:field='args[1]';value=e['args'][1]
  if field:
   if type(value) is int and 0<value<=0x7fffffff:
    if value not in aliases:aliases[value]=len(aliases)+1
    normalized={'native_session_id':aliases[value]}
   else:normalized={'native_session_id_literal':value}
   entries.append({'seq':e.get('seq'),'field':field,'method':method(k),'raw':value,'normalized':normalized,'from':e.get('from'),'self':e.get('self')})
 return {'distinct_positive_ids':len(aliases),'entries':entries,'unpaired_identifier_returns':unpaired,'pending_identifier_calls':pending}
summary={};commands=[];expected_official=[116,57,54]
for index,(scenario,launch) in enumerate([('launch-slice',1),('launch-slice',2),('extensions-page-actions',1)]):
 name=f'{scenario}/launch-{launch}';dest=OUT/name
 gp=snapshot(f'results/extensions-validation7/golden/arc-1.170-{scenario}/launch-{launch}/record.jsonl',f'{name}/golden.jsonl');op=snapshot(f'results/extensions-validation7/{scenario}/launch-{launch}/record.jsonl',f'{name}/ours.jsonl')
 official_txt=snapshot(f'results/extensions-validation7/{scenario}/diff-launch-{launch}.txt',f'{name}/official-diff.txt');official_json=snapshot(f'results/extensions-validation7/{scenario}/diff-launch-{launch}.json',f'{name}/official-diff.json');official_first2=official_txt.read_text().splitlines()[:2];assert official_first2[0].startswith(f'methods differing: {expected_official[index]} ('),official_first2
 raw={'golden':rows(gp),'ours':rows(op)};versions={side:version(events) for side,events in raw.items()};assert all(value['version']=='v2-attributed' for value in versions.values()),versions
 command=[sys.executable,'-B',str(OUT/'tool/tests/recorder/diff.py'),str(gp),str(op),'--json',str(dest/'diff.json')];run=subprocess.run(command,capture_output=True,text=True);assert run.returncode==0,run.stderr;commands.append({'scenario':name,'argv':command,'exit':run.returncode})
 (dest/'diff.txt').write_text(run.stdout);(dest/'diff.stderr.txt').write_text(run.stderr);first2=run.stdout.splitlines()[:2];(dest/'first-two-lines.txt').write_text('\n'.join(first2)+'\n');assert 'caller attribution: on (app calls + engine setters)' in run.stdout
 report=json.loads((dest/'diff.json').read_text());inventory={};callback_sequences={};session={};rawaudit={};selection={}
 for side,path in [('golden',gp),('ours',op)]:
  calls,callbacks=tool.load(path,True);retained=selected(raw[side]);assert sum(len(records) for records in calls.values())==sum(row.get('ev')=='call' for row in retained)
  inventory[side]=[{'method':method(k),'calls':len(records),'records':records} for k,records in sorted(calls.items()) if owned(k)]
  callback_sequences[side]=[{'seq':e.get('seq'),'method':method(key(e)),'raw':e} for e in retained if e.get('ev')=='call' and e.get('dir')=='engine->app' and owned(key(e))]
  session[side]=session_trace(raw[side]);rawaudit[side]=[e for e in raw[side] if e.get('cls') in classes];selection[side]={'selected_call_rows':sum(e.get('ev')=='call' for e in retained),'selected_ret_rows':sum(e.get('ev')=='ret' for e in retained),'unselected_boundary_rows':sum(e.get('ev') in ['call','ret'] and e not in retained for e in raw[side])}
 names={item['method'] for values in inventory.values() for item in values};scope={section:[item for item in items if item.get('method') in names] for section,items in report.items() if isinstance(items,list)}
 for filename,value in [('extension-owned-report.json',scope),('extension-owned-method-inventory.json',inventory),('extension-owned-callback-sequences.json',callback_sequences),('session-id-evidence.json',session),('extension-class-raw-audit.json',rawaudit),('selection-counts.json',selection)]: (dest/filename).write_text(json.dumps(value,indent=2))
 summary[name]={'scope':'private comparison-policy diagnostic of unchanged validation7 raw captures; no new runtime or acceptance','recorder':versions,'fixture_images':[],'official_first_two_lines':official_first2,'official_methods_differing':expected_official[index],'diagnostic_first_two_lines':first2,'diagnostic_global_totals':{section:len(items) for section,items in report.items() if isinstance(items,list)},'global_callback_sequence':report['callback_sequence'],'extension_public_residuals':{section:items for section,items in scope.items() if section not in ['internal','internal_ours'] and items},'extension_internal_classification':{section:scope.get(section,[]) for section in ['internal','internal_ours']},'extension_callback_counts':{side:len(values) for side,values in callback_sequences.items()},'extension_callback_method_order_equal':[e['method'] for e in callback_sequences['golden']]==[e['method'] for e in callback_sequences['ours']],'session_target_residuals':{section:[item for item in items if item.get('method') in [method(tool.SESSION_IDENTIFIER),method(tool.SESSION_GROUP_CALLBACK)]] for section,items in report.items() if isinstance(items,list)}}
verification=[]
for dest,meta in inputs.items():
 source=ROOT/meta['source'];copy=OUT/dest;source_digest=sha(source);copy_digest=sha(copy);assert source_digest==copy_digest==meta['sha256'];assert source.stat().st_size==copy.stat().st_size==meta['size'];verification.append({'source':meta['source'],'copy':dest,'source_sha256':source_digest,'copy_sha256':copy_digest,'unchanged':True})
(OUT/'input-verification.json').write_text(json.dumps(verification,indent=2));(OUT/'commands.json').write_text(json.dumps(commands,indent=2));(OUT/'summary.json').write_text(json.dumps(summary,indent=2));(OUT/'provenance.json').write_text(json.dumps({'scope':'private historical validation7 re-diff; official116/57/54 unchanged; no new runtime/acceptance claim','comparator_sha256':sha(OUT/'tool/tests/recorder/diff.py'),'inputs':inputs,'all_source_and_copy_hashes_reverified':True,'fixture_images':[],'copied_raw_count':6,'copied_shared_tools_untouched':True},indent=2));print(json.dumps(summary,indent=2))
