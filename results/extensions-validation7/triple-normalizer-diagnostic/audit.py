from pathlib import Path
import hashlib,json,subprocess,sys
OUT=Path(__file__).parent;ROOT=OUT.parents[2]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
summary=json.loads((OUT/'summary.json').read_text());policy={}
for name in summary:
 directory=OUT/name;official=json.loads((directory/'official-diff.json').read_text());diagnostic=json.loads((directory/'diff.json').read_text());allowed={'return_mismatch':'->engine -ArcSessionID identifier','arg_mismatch':'engine->app -ADK.ChromiumBrowser browser:extensionGroupIdForTabWithSessionId:'}
 differences={}
 assert set(official)==set(diagnostic)
 for category in official:
  left,right=official[category],diagnostic[category]
  if category in allowed:
   excluded=allowed[category];assert [item for item in left if item.get('method')!=excluded]==[item for item in right if item.get('method')!=excluded];differences[category]={'official_target':[item for item in left if item.get('method')==excluded],'diagnostic_target':[item for item in right if item.get('method')==excluded]}
  else:assert left==right,category
 policy[name]={'only_difference_is_two_exact_session_fields':True,'count_mismatch_list_byte_values_unchanged':official['count_mismatch']==diagnostic['count_mismatch'],'full_callback_sequence_report_unchanged':official['callback_sequence']==diagnostic['callback_sequence'],'exact_target_changes':differences}
(OUT/'policy-delta.json').write_text(json.dumps(policy,indent=2))
page=OUT/'extensions-page-actions/launch-1';detail={}
for side in ['golden','ours']:
 events=[json.loads(line) for line in (page/(side+'.jsonl')).read_text().splitlines()];snapshots={};toolbar={};list_calls=[];pin_callbacks=[];active_pin=None
 for event in events:
  cls,selector,phase=event.get('cls'),event.get('sel'),event.get('ev')
  if event.get('dir')=='engine->app' and selector=='browserContext:didChangePinnedExtensionOrder:':
   if phase=='call':active_pin={'call':event,'nested_list_calls':[],'nested_info_return_rows':[]};pin_callbacks.append(active_pin)
   elif phase=='ret' and active_pin:active_pin['return']=event;active_pin=None
  if event.get('from')=='app' and cls=='ArcBrowserContextExtensions' and selector=='listExtensions':
   if phase=='call':list_calls.append({'call':event,'within_pin_callback_seq':active_pin['call']['seq'] if active_pin else None});
   elif phase=='ret':list_calls[-1]['return']=event
   if active_pin and phase=='call':active_pin['nested_list_calls'].append(event)
  if event.get('from')=='app' and cls in ['ArcExtensionInfo','ArcExtensionToolbarAction'] and phase=='ret':
   target=snapshots if cls=='ArcExtensionInfo' else toolbar;target.setdefault(event['self'],{}).setdefault(selector,[]).append({'seq':event['seq'],'value':event.get('ret')})
   if active_pin and cls=='ArcExtensionInfo':active_pin['nested_info_return_rows'].append(event)
 canonical_info=[{selector:[row['value'] for row in values] for selector,values in fields.items()} for fields in snapshots.values()]
 detail[side]={'list_calls':list_calls,'list_return_lengths':[len(item['return']['ret']) for item in list_calls],'info_objects':snapshots,'info_canonical_values':canonical_info,'toolbar_objects':toolbar,'pin_callbacks':pin_callbacks}
reference=detail['golden']['info_canonical_values'][0]
assert len(detail['golden']['info_canonical_values'])==4 and len(detail['ours']['info_canonical_values'])==5
assert all(item==reference for side in detail.values() for item in side['info_canonical_values'])
assert len(detail['golden']['pin_callbacks'])==1 and len(detail['ours']['pin_callbacks'])==2
extra=detail['ours']['pin_callbacks'][1];assert extra['call']['args'][1]==reference['id'][:1] and len(extra['nested_list_calls'])==1 and len(extra['nested_info_return_rows'])==10
(OUT/'page-state-audit.json').write_text(json.dumps({'all_emitted_info_snapshot_values_equal_exactly':True,'extra_ours_pin_callback_seq':extra['call']['seq'],'extra_pin_contains_one_list_and_ten_info_getter_returns':True,'sides':detail,'inference':'Extra page Info/list demand is downstream of the additional filled-pin callback: exactly one nested list call and one identical fixture snapshot. Feature-configuration causality is supported by the existing feature-state audit/source gates, not isolated by a new disabled-feature runtime experiment.'},indent=2))
# This intentionally fails before touching inputs/outputs: capture owns an
# exclusive guard. Keep the refusal evidence without rerunning comparisons.
before={str(path.relative_to(OUT)):sha(path) for path in OUT.rglob('*') if path.is_file()}
run=subprocess.run([sys.executable,'-B',str(OUT/'capture.py')],capture_output=True,text=True)
assert run.returncode!=0 and 'FileExistsError' in run.stderr
assert all(sha(OUT/path)==digest for path,digest in before.items())
(OUT/'rerun-refusal.txt').write_text(run.stderr)
provenance=json.loads((OUT/'provenance.json').read_text())
for dest,meta in provenance['inputs'].items():assert sha(ROOT/meta['source'])==sha(OUT/dest)==meta['sha256']
print(json.dumps({'three_comparisons':list(summary),'only_exact_session_fields_changed':True,'all_counts_and_callback_sequences_unchanged':True,'page_info_snapshots_equal':'4 golden / 5 ours, all emitted getter values identical','extra_pin_seq':extra['call']['seq'],'input_count':len(provenance['inputs']),'all_original_and_copied_hashes_reverified':True,'exclusive_rerun_refused':True},indent=2))
