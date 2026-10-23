from pathlib import Path
import collections,datetime,hashlib,importlib.util,json,re,subprocess,sys

ROOT=Path('/Users/cryogenicplanet/general/arc-engine')
OUT=Path(__file__).resolve().parent
assert OUT==ROOT/'results/extensions-peer-bookmarks-audit'
with (OUT/'capture-exclusive.guard').open('x') as f:
    f.write('Exclusive peer bookmark audit; refuse overwrite/reuse.\n')
assert not (OUT/'provenance.json').exists() and not (OUT/'tool').exists()
started=datetime.datetime.now(datetime.UTC).isoformat()
inputs={}
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def snap(src,dst):
    source=ROOT/src;target=OUT/dst
    assert not target.exists(),target
    before=source.stat();data=source.read_bytes();digest=hashlib.sha256(data).hexdigest()
    target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(data)
    after=source.stat()
    assert before.st_size==after.st_size==len(data) and before.st_mtime_ns==after.st_mtime_ns
    assert sha(source)==sha(target)==digest
    inputs[dst]={'source':src,'sha256':digest,'size':len(data),
                 'before_mtime_ns':before.st_mtime_ns,'after_capture_mtime_ns':after.st_mtime_ns,
                 'before_inode':before.st_ino,'after_capture_inode':after.st_ino}
    return target

toolroot='results/extensions-validation7/triple-normalizer-diagnostic/tool/'
for p in ['tests/recorder/diff.py','arc/DIFF-ALLOWLIST.md',
          'arc/tools/arccore-1.170.0-objc.tsv','tests/recorder/app_callbacks.inc']:
    snap(toolroot+p,'tool/'+p)
    snap(p,'current-shared-policy/'+p)
assert sha(OUT/'tool/tests/recorder/diff.py')=='63c0055e6c44ff5b27c45d69bb91f55a471bcaa20761f27d780fcb1779b6eb08'
for side,source in [('golden','results/golden/arc-1.170-bookmarks-public-api'),
                    ('ours','results/ours/bookmarks-public-api')]:
    for relative in ['launch-1/record.jsonl','launch-1/app.log','coverage.json','steps.json',
                     'bookmarks-public-api-driver/provenance.json',
                     'bookmarks-public-api-driver/raw-gate.json',
                     'bookmarks-public-api-driver/ready.json',
                     'bookmarks-public-api-driver/compile.log']:
        snap(source+'/'+relative,'inputs/'+side+'/'+relative)
    provenance=json.loads((OUT/'inputs'/side/'bookmarks-public-api-driver/provenance.json').read_text())
    for field,hashfield in [('source','sha256'),('scenario','scenarioSHA256'),('gate','gateSHA256')]:
        p=snap(provenance[field],'driver-source/'+side+'/'+Path(provenance[field]).name)
        assert sha(p)==provenance[hashfield],(side,field)
for relative in ['diff-launch-1.txt','diff-launch-1.json']:
    snap('results/ours/bookmarks-public-api/'+relative,'official/'+relative)
for p in ['logs/wp/scenarios.scoreboard.bookmarks-public-api.2026-10-23T13-16-46-684Z.log',
          'logs/wp/scenarios.bookmarks-public-api.run1.state.json',
          'logs/wp/scenarios.bookmarks-public-api.golden.run1.log',
          'logs/wp/extensions-golden-coverage-20261023/bookmarks-renderer-v2-increment/snapshot.json']:
    snap(p,'peer-metadata/'+Path(p).name)
proposal_hashes={}
for p in ['arc/browser/extensions_profile_observer.patch','arc/browser/extensions_profile_observer.md',
          'logs/wp/extensions-profile-observer-proposal/v1/extensions_profile_observer.after-app.patch']:
    proposal_hashes[p]=sha(ROOT/p)

gp=OUT/'inputs/golden/launch-1/record.jsonl';op=OUT/'inputs/ours/launch-1/record.jsonl'
command=[sys.executable,'-B',str(OUT/'tool/tests/recorder/diff.py'),str(gp),str(op),
         '--json',str(OUT/'private-diff.json')]
run=subprocess.run(command,capture_output=True,text=True)
assert run.returncode==0 and not run.stderr,(run.returncode,run.stderr)
(OUT/'private-diff.txt').write_text(run.stdout);(OUT/'private-diff.stderr.txt').write_text(run.stderr)
(OUT/'command.json').write_text(json.dumps({'argv':command,'exit':run.returncode,'fixture_images':[]},indent=2)+'\n')
assert 'caller attribution: on (app calls + engine setters)' in run.stdout
spec=importlib.util.spec_from_file_location('peer_private_diff',OUT/'tool/tests/recorder/diff.py')
tool=importlib.util.module_from_spec(spec);spec.loader.exec_module(tool)
classes={'ArcBrowserContextExtensions','ArcExtensionInfo','ArcExtensionCommand',
         'ArcExtensionRuntimeError','ArcExtensionRuntimeErrorStackFrame','ArcExtensionToolbarAction'}
browser_selectors={'executeExtensionAction:','pinExtension:','unpinExtension:',
 'showExtensionContextMenu:','moveExtension:targetIndex:',
 'triggerExtensionOmniboxInput:inputText:disposition:'}
group_selectors={'browser:createTabGroupWithSessionIds:properties:completion:',
 'browser:addTabsToGroupWithId:sessionIds:completion:',
 'browser:removeTabsFromGroupWithSessionIds:completion:',
 'browser:updateTabGroupWithId:properties:completion:',
 'browser:moveTabGroupWithId:toNewIndex:completion:',
 'browser:getTabGroupWithId:completion:','browser:getAllTabGroupsWithCompletion:',
 'browser:extensionGroupIdForTabWithSessionId:'}
context_callbacks={'browserContext:didEnableExtension:','browserContext:didDisableExtension:',
 'browserContext:didInstallExtension:','browserContext:didUninstallExtension:',
 'browserContext:didFailLoadUnpacked:withManifestError:',
 'browserContext:didFail:withRuntimeError:','browserContext:didFail:withManifestError:',
 'browserContext:didFail:withInternalError:','browserContext:didChangePinnedExtensionOrder:',
 'browserContextDidExtensionSystemReady:'}
def key(e):return tuple(e.get(f) for f in ['dir','kind','cls','sel'])
def method(k):return '%s %s%s %s'%(k[0],k[1],k[2],k[3])
def owned(k):
    direction,kind,cls,selector=k
    if direction=='->engine':return cls in classes or cls=='ArcBrowser' and selector in browser_selectors
    return direction=='engine->app' and ('Extension' in selector or selector in group_selectors or
        selector in context_callbacks or 'BrowserExtensionsLifeCycleManager' in cls)
def retained(e):
    return not(e.get('dir')=='->engine' and e.get('from')!='app' and
               not(e.get('from')=='engine' and e.get('sel','').startswith('set')))
raw={side:[json.loads(line) for line in p.read_text().splitlines()]
     for side,p in [('golden',gp),('ours',op)]}
inventory={};callbacks={};session={};rawscope={};versions={};counts={}
for side,path in [('golden',gp),('ours',op)]:
    events=raw[side];selected=[e for e in events if retained(e)]
    boundary=[e for e in events if e.get('ev') in ['call','ret']]
    attributed=[e for e in boundary if isinstance(e.get('from'),str) and e['from'] and
                e['from'].strip()==e['from'] and all(ord(c)>=32 for c in e['from'])]
    versions[side]={'boundary_rows':len(boundary),'attributed_rows':len(attributed),
                    'all_boundary_rows_attributed':len(boundary)==len(attributed)}
    assert versions[side]['all_boundary_rows_attributed']
    calls,fullcallbacks=tool.load(path,True)
    assert sum(len(v) for v in calls.values())==sum(e.get('ev')=='call' for e in selected)
    inventory[side]=[{'method':method(k),'calls':len(v),'records':v}
                     for k,v in sorted(calls.items()) if owned(k)]
    callbacks[side]=[e for e in selected if e.get('ev')=='call' and
                    e.get('dir')=='engine->app' and owned(key(e))]
    rawscope[side]={'selected_owned_events':[e for e in selected if owned(key(e))],
                    'all_extension_class_events':[e for e in events if e.get('cls') in classes]}
    counts[side]={'selected_owned_call_rows':sum(e.get('ev')=='call' and owned(key(e)) for e in selected),
                  'owned_callback_calls':len(callbacks[side]),
                  'all_selected_call_rows':sum(e.get('ev')=='call' for e in selected),
                  'all_raw_extension_class_call_rows':sum(e.get('ev')=='call' and e.get('cls') in classes for e in events),
                  'excluded_caller_call_rows':dict(collections.Counter(e.get('from') for e in events if e.get('ev')=='call' and not retained(e)))}
    aliases={};sid=[];pending=collections.Counter();unpaired=[]
    for e in selected:
        k=key(e);field=None;value=None
        if k==tool.SESSION_IDENTIFIER:
            pendingkey=(e.get('thread'),e.get('self'))
            if e.get('ev')=='call':pending[pendingkey]+=1
            elif e.get('ev')=='ret':
                if not pending[pendingkey]:unpaired.append(e);continue
                pending[pendingkey]-=1;field='return';value=e.get('ret')
        if k==tool.SESSION_GROUP_CALLBACK and e.get('ev')=='call':
            assert isinstance(e.get('args'),list) and len(e['args'])>1
            field='args[1]';value=e['args'][1]
        if field:
            if type(value) is int and 0<value<=0x7fffffff:
                if value not in aliases:aliases[value]=len(aliases)+1
                normalized={'native_session_id':aliases[value]}
            else:normalized={'native_session_id_literal':value}
            sid.append({'seq':e.get('seq'),'method':method(k),'field':field,'raw':value,
                        'normalized':normalized,'self':e.get('self'),'from':e.get('from')})
    session[side]={'distinct_positive_ids':len(aliases),'entries':sid,
                  'unpaired_returns':unpaired,'pending_identifier_calls':sum(pending.values())}

private=json.loads((OUT/'private-diff.json').read_text());official=json.loads((OUT/'official/diff-launch-1.json').read_text())
names={x['method'] for values in inventory.values() for x in values}
scope=lambda report:{k:[x for x in v if x.get('method') in names] for k,v in report.items() if isinstance(v,list)}
officialscope=scope(official);privatescope=scope(private)
allowed={'return_mismatch':'->engine -ArcSessionID identifier',
         'arg_mismatch':'engine->app -ADK.ChromiumBrowser browser:extensionGroupIdForTabWithSessionId:'}
policy={}
assert set(official)==set(private)
for category in official:
    left,right=official[category],private[category]
    if category in allowed:
        target=allowed[category]
        assert [x for x in left if x.get('method')!=target]==[x for x in right if x.get('method')!=target]
        policy[category]={'official_target':[x for x in left if x.get('method')==target],
                          'private_target':[x for x in right if x.get('method')==target]}
    else:assert left==right,category
callbackkeys={side:[method(key(e)) for e in v] for side,v in callbacks.items()}
summary={
 'scope':'peer observation from copied completed bookmark captures, not extension source-frozen replay',
 'comparator_sha256':sha(OUT/'tool/tests/recorder/diff.py'),'fixture_images':[],
 'official_first_two_lines':(OUT/'official/diff-launch-1.txt').read_text().splitlines()[:2],
 'private_first_two_lines':run.stdout.splitlines()[:2],
 'official_global_totals':{k:len(v) for k,v in official.items() if isinstance(v,list)},
 'private_global_totals':{k:len(v) for k,v in private.items() if isinstance(v,list)},
 'official_extension_scope':officialscope,'private_extension_scope':privatescope,
 'extension_callback_counts':{side:len(v) for side,v in callbacks.items()},
 'extension_callback_method_order_equal':callbackkeys['golden']==callbackkeys['ours'],
 'extension_callback_method_order':callbackkeys,'raw_selection_counts':counts,'recorder':versions,
 'public_selected_method_inventory_equal':inventory['golden']==inventory['ours'],
 'profile_proposal_hashes':proposal_hashes,
 'renderer_canonical_ours_exists':(ROOT/'results/ours/renderer-public-api').exists(),
 'provenance_limit':'Run provenance pins ancestor fixture helper/scenario/gate references. No historical canonical helper source or own extension TU/compiler/framework binary epoch is established; do not infer equality with extension batches9/10.',
 'policy_limit':'Official stored diff is frozen. Current shared comparator/policy also frozen as observation; exact official generation command/hash not present in its stored report.',
}
for name,data in [('summary.json',summary),('extension-method-inventory.json',inventory),
                  ('extension-callback-sequence.json',callbacks),('session-id-evidence.json',session),
                  ('extension-raw-scope.json',rawscope),('policy-delta.json',policy)]:
    (OUT/name).write_text(json.dumps(data,indent=2)+'\n')
verification=[]
for dest,meta in inputs.items():
    source=ROOT/meta['source'];copy=OUT/dest;st=source.stat()
    assert sha(source)==sha(copy)==meta['sha256']
    assert st.st_size==meta['size'] and st.st_mtime_ns==meta['after_capture_mtime_ns']
    verification.append({'source':meta['source'],'copy':dest,'before_sha256':meta['sha256'],
                         'after_source_sha256':sha(source),'copy_sha256':sha(copy),'unchanged':True})
assert all(sha(ROOT/p)==h for p,h in proposal_hashes.items())
(OUT/'input-verification.json').write_text(json.dumps(verification,indent=2)+'\n')
(OUT/'provenance.json').write_text(json.dumps({'started_utc':started,
 'finished_utc':datetime.datetime.now(datetime.UTC).isoformat(),'inputs':inputs,
 'all_sources_and_copies_hash_verified':True,'source_frozen_extension_replay':False,
 'profile_proposals_unchanged':True,'fixture_images':[]},indent=2)+'\n')
print(json.dumps(summary,indent=2))
