#!/usr/bin/env python3
"""Generate declared-identity specifications from re-decoded metadata and bounded review.
Automatic rows are literal local observations only; curated notes are separately editable.
"""
import csv,json,re,hashlib,collections,sqlite3
from pathlib import Path
from curated import METHODS
from class_notes import NOTES
ROOT=Path('/home/exedev/work/inputs'); OUT=Path('specs');PIN='d0c02bfad08ec92a6ffcf989e53cd995e6381a4c';CEF='426920c33ef22551e0b8f48cf343d623ac25642d'
meta=json.loads((OUT/'evidence/metadata.json').read_text());bs=json.loads((OUT/'evidence/body-index.json').read_text());rec=json.loads((OUT/'evidence/reconciliation.json').read_text())
assert set(meta['classes'])==set(NOTES)
leaves={ (x['class_name'],x['category'],x['kind'],x['selector']):x for x in json.loads((OUT/'evidence/arm64-leaves.json').read_text()) }
rawcheck=json.loads((OUT/'evidence/raw-dump-check.json').read_text())
def body(x):return Path(bs[x['imp']]['snapshot']).read_text()
def calls(s):return list(dict.fromkeys('0x'+v.lstrip('0') for v in re.findall(r'\bFUN_([0-9a-fA-F]+)\s*\(',s)))
RUNTIME={'0x5810','0x8790','0xf01bc0','0x13efb40','0x4b08a00','0x4b35898','0x4047380','0x40416c0','0x4b087f0'}
def simple(s):
 a=s.find('\n{');b=s.rfind('}');c=s[a+2:b].strip() if a>=0 else ''
 c=re.sub(r'/\*.*?\*/','',c,flags=re.S).strip()
 return c
# Data snippets expose actual operations, not inferred C++ callee names.
observations=[];rows=[]
for x in meta['methods']:
 s=body(x);c=simple(s);va=x['imp'][2:];allcalls=calls(s);backend=[v for v in allcalls if v not in RUNTIME]
 selectors=list(dict.fromkeys(re.findall(r'"([^"\n]+:)"',s)))
 flags=[]
 if 'Could not recover jumptable' in s:flags.append('indirect_dispatch')
 if 'Type propagation algorithm not settling' in s:flags.append('type_recovery_unstable')
 if 'unaff_' in s or 'in_x' in s or 'in_s' in s or 'in_register_' in s or 'extraout_' in s:flags.append('register_recovery_requires_review')
 if re.search(r'(?:\bint\b[^\n]*::(?:accept|close|pause)|sockaddr \*)',s):flags.append('incorrect_C_or_method_signature')
 if va in {'43e0194','442fe90','4416bec'}:flags.append('incorrect_C_or_method_signature')
 if 'TRUNCATED:' in s:flags.append('truncated')
 status='available_with_decompiler_limits' if flags else 'available'
 semantic='unresolved';confidence='low-unresolved';question='Native backend semantics, ownership and queue beyond literal wrapper operations remain unresolved.'
 custom=METHODS.get(va)
 if custom:
  behavior,semantic,question=custom;confidence='medium-local' if semantic=='local_review' else 'medium-partial'
 else:
  match=re.fullmatch(r'return ([^;]+);',c,re.S)
  if match and len(match[1])<180:
   behavior='Literal return: '+re.sub(r'\s+',' ',match[1])+'. Use this row metadata type; shared Ghidra name/type may differ.'
   semantic='literal_verified';confidence='high-local';question=''
  elif re.fullmatch(r'\*\([^\n]+\)\s*=\s*[^;]+;\s*return;',c,re.S) and len(c)<250:
   behavior='Literal store: '+re.sub(r'\s+',' ',c).removesuffix('return;').strip()
   if '_objc_storeStrong' in s:behavior+=' Runtime annotation identifies objc_storeStrong.'
   if '_objc_storeWeak' in s:behavior+=' Runtime annotation identifies objc_storeWeak.'
   semantic='literal_verified';confidence='high-local';question=''
  else:
   returns=re.findall(r'\breturn ([^;\n]+);',c)
   if len(returns)==1 and len(returns[0])<150 and '*' in returns[0] and not backend:
    behavior='Observed return expression: '+returns[0]+'. Guard/trap and native optional details are in exact body.'
    semantic='literal_partial';confidence='medium-local';question='Guard conditions and native field identity require context; no wider lifecycle/thread guarantee.'
   elif x['selector']=='.cxx_construct':
    behavior='Native construction body initializes local storage; literal stores are retained in method-observations.json. No default ownership/queue inference.'
    semantic='literal_partial';confidence='medium-local';question='Confirm nontrivial native default-constructor semantics before translating layout.'
   elif x['selector']=='.cxx_destruct':
    behavior='Native destruction body is available; cleanup/control-block/ObjC runtime operations must be interpreted with this class layout.'
    semantic='literal_partial';confidence='medium-local';question='Underlying resource destruction, observer cancellation and required sequence are not established by this wrapper cleanup.'
   else:
    behavior='Behavior unresolved beyond local wrapper evidence.'
    if backend:behavior+=' Direct bridge/helper calls: '+', '.join(backend[:6])+'.'
    if selectors:behavior+=' Observed selector operands: '+', '.join(selectors[:4])+'.'
    if not backend and not selectors:behavior+=' Native field or indirect-dispatch operations require further review.'
 leaf=leaves.get((x['class_name'],x['category'],x['kind'],x['selector']))
 if leaf and leaf['requires_semantic_warning']:
  behavior='Pseudocode loses the return value under incorrect void prototype. Pristine arm64 leaf: '+leaf['decoded_whitelist']+'. Field identity/type independently checked against this class metadata.'
  status='available_malformed_signature_recovered_leaf';semantic='instruction_verified';confidence='high-local';question=''
  flags.append('erased_return_value')
 if semantic=='unresolved' and ':' not in x['selector'] and not selectors:
  converter=next((v for v in ['0x1c391bc','0x1d9fb90'] if v in backend),None)
  if converter and len(backend)<=5:
   behavior='Converts native field-derived string data through '+converter+' (CFStringCreateWithBytes converter inspected in evidence/string-url-converters.txt). Exact source reads and optional/nil guards remain visible in body.'
   semantic='conversion_partial';confidence='medium-local';question='Native field semantics, optional/empty-string policy and producer invariants require method-specific validation.'
 # Literal code evidence is bounded and independent of handoff names.
 ops=[re.sub(r'\s+',' ',l.strip()) for l in c.splitlines() if ('return ' in l or ('param_1 +' in l and '=' in l) or 'SoftwareBreakpoint' in l or '_objc_' in l or 'FUN_' in l)]
 evidence=f"ArcCore-1.170.0.arm64 IMP {x['imp']}; {bs[x['imp']]['snapshot']}; corpus {bs[x['imp']]['index'][1]} byte {bs[x['imp']]['index'][2]} length {bs[x['imp']]['index'][3]}; ObjC method_record_va {x['method_record_va']}; dyld dobjc170.txt:{x['dyld_line']}"
 if leaf: evidence+='; specs/evidence/arm64-leaves.json (pristine instruction words)'
 rows.append(dict(class_name=x['class_name'],category=x['category'],kind=x['kind'],selector=x['selector'],type_encoding=x['type_encoding'],imp_va=x['imp'],body_status=status,behavior=behavior,evidence=evidence,confidence=confidence,unresolved_question=question,semantic_status=semantic,backend_callees=','.join(backend),decompiler_limits=','.join(flags)))
 observations.append(dict(class_name=x['class_name'],category=x['category'],kind=x['kind'],selector=x['selector'],imp_va=x['imp'],direct_calls=allcalls,selector_operands=selectors,literal_excerpts=ops,limits=flags))
(OUT/'evidence/method-observations.json').write_text(json.dumps(observations,indent=2))
with (OUT/'methods.tsv').open('w') as f:
 w=csv.DictWriter(f,fieldnames=list(rows[0]),delimiter='\t');w.writeheader();w.writerows(rows)
def esc(x):return str(x).replace('|','&#124;').replace('\n',' ')
for cls in sorted(meta['classes']):
 l=meta['classes'][cls];ms=[x for x in rows if x['class_name']==cls];mm=[x for x in meta['methods'] if x['class_name']==cls]
 lines=[f'# {cls}', '',f'ArcCore **1.170.0 arm64**, unslid engine VAs. {len(ms)} declared identities. Superclass `{l["superclass"]}`; class metadata `{l["metadata_va"]}`, class RO `{l["ro_va"]}`; raw otool `objc170.txt:{rawcheck['class_layout_lines'][cls]}`. Method identity is `(class, category, kind, selector)`; inherited methods are outside this declared inventory.', '',f'Chromium source comparisons use commit `{PIN}` (156.0.8078.48). CEF comparisons use `{CEF}`. Files/lines cited below are pinned source-backed comparisons; anonymous engine functions retain their VAs unless independently identified. Sources and hashes: [source manifest](../evidence/sources.json).','',NOTES[cls], '', 'State/layout below is independently decoded from Mach-O metadata; offsets are bytes from self. Encoded C++/ObjC ownership markers are declarations, not proof of runtime lifetime. Property `C` means declared copy, `&` strong, `W` weak; `N` nonatomic. See exact method bodies for observed operations.', '',f'Instance start `{l["instance_start"]}`, instance size `{l["instance_size"]}`. Protocol declarations: `{l["protocols"] or "none in class metadata"}`.', '', '| Ivar | Offset | Size | Type | Metadata record VA |','|---|---:|---:|---|---|']
 for v in l['ivars']:lines.append('| '+' | '.join(map(esc,[v['name'],v['offset'],v['size'],'`'+v['type']+'`',v['record_va']]))+' |')
 if not l['ivars']:lines.append('| No class ivars declared | — | — | — | — |')
 lines+=['','| Property | Raw attributes | Metadata record VA |','|---|---|---|']
 for v in l['properties']:lines.append('| '+' | '.join(map(esc,[v['name'],'`'+v['attributes']+'`',v['record_va']]))+' |')
 if not l['properties']:lines.append('| None declared | — | — |')
 catlist=[x for x in meta['categories'] if x['class_name']==cls]
 cats='; '.join(f"`{x['category']}` at `{x['metadata_va']}`" for x in catlist) or 'none'
 lines+=['',f'Categories in assignment: {cats}. No base/category selector collision was found in these assigned classes. Shared IMPs remain separate rows; their Ghidra display names are not receiver identities.','', 'The matrix records local pseudocode observations, not runtime verification. `literal_verified` means a literal leaf load/store/return matched its exact body, not full ABI or thread verification. `local_review`/`partial_review` are bounded human review. `unresolved` rows preserve declared identity and concrete evidence without inferred behavior. No callback queue is guaranteed unless class notes give an explicit source/address anchor.','', '| Category | +/- selector | Type encoding | IMP VA / exact body | Behavior | Backend/callees | Evidence | Confidence / status |','|---|---|---|---|---|---|---|---|']
 byid={(x['category'],x['kind'],x['selector']):x for x in mm}
 for r in ms:
  x=byid[(r['category'],r['kind'],r['selector'])];bf=f"../bodies/{int(r['imp_va'],16):08x}.txt"
  vals=[r['category'] or '(base)','`'+r['kind']+r['selector']+'`','`'+r['type_encoding']+'`',f"[{r['imp_va']}]({bf})",r['behavior'],r['backend_callees'] or 'No direct FUN call; see body for field/ObjC/indirect operations',f"metadata `{x['method_record_va']}`; dobjc170.txt:{x['dyld_line']}",f"{r['confidence']}; {r['body_status']}; {r['semantic_status']}"]
  lines.append('| '+' | '.join(map(esc,vals))+' |')
 qs=list(dict.fromkeys(r['unresolved_question'] for r in ms if r['unresolved_question']))
 lines+=['','Unresolved engineering questions (method-specific evidence above):','']
 lines += ['- '+q for q in qs] if qs else ['- ABI layout/register passing, producer/consumer integration and real app behavior have not been runtime-tested.']
 (OUT/'classes'/f'{cls}.md').write_text('\n'.join(lines)+'\n')
# Reconcile identities/categories/aliases separately from semantic review.
collisions=[]
for cls in meta['classes']:
 d=collections.defaultdict(list)
 for r in rows:
  if r['class_name']==cls:d[(r['kind'],r['selector'])].append(r)
 for k,v in d.items():
  if len(v)>1:collisions.append([cls,k,[x['category'] for x in v]])
coverage={'image':'ArcCore-1.170.0.arm64','address_basis':'unslid Mach-O engine VA; file offsets translated by segment map; app evidence uses distinct 0x100... image VAs','assigned_classes':len(meta['classes']),'expected_classes':37,'declared_method_rows':len(rows),'expected_method_rows':491,'unique_identities':len({(x['class_name'],x['category'],x['kind'],x['selector']) for x in rows}),'identity_reconciliation':rec['identity_reconciliation'],'method_kind_counts':dict(collections.Counter(x['kind'] for x in rows)),'category_method_counts':dict(collections.Counter((x['category'] or '(base)') for x in rows)),'distinct_imps':len(bs),'body_available_method_rows':sum(bs[x['imp_va']]['status']=='available_candidate' for x in rows),'failed_method_rows':sum(bs[x['imp_va']]['status']=='failed' for x in rows),'missing_method_rows':sum(bs[x['imp_va']]['status']=='missing' for x in rows),'body_status_counts':dict(collections.Counter(x['body_status'] for x in rows)),'decompiler_limit_flag_counts':dict(collections.Counter(f for x in rows for f in x['decompiler_limits'].split(',') if f)),'semantic_status_counts':dict(collections.Counter(x['semantic_status'] for x in rows)),'unresolved_method_rows':sum(x['semantic_status']=='unresolved' for x in rows),'method_rows_with_open_questions':sum(bool(x['unresolved_question']) for x in rows),'runtime_semantically_verified_method_rows':0,'full_backend_semantically_verified_method_rows':0,'base_category_collisions':collisions,'shared_imp_groups':len(rec['shared_imps']),'app168_independently_checked_BL_instructions':4,'app168_receiver_association':'prior pseudocode inference; no runtime receiver proof','malformed_erased_return_method_rows':sum('erased_return_value' in x['decompiler_limits'] for x in rows),'instruction_checked_leaf_method_rows':len(leaves),'body_counts_are_presence_only':True,'ghidra_bound_sizes_summed':False,'per_class':{cls:dict(method_rows=sum(x['class_name']==cls for x in rows),statuses=dict(collections.Counter(x['semantic_status'] for x in rows if x['class_name']==cls))) for cls in sorted(meta['classes'])}}
imp_snapshots={Path(x['snapshot']).name for x in bs.values()}
extra_snapshots=[p for p in (OUT/'bodies').glob('*.txt') if p.name not in imp_snapshots]
coverage['backend_snapshot_candidates']=sum(p.read_text().startswith('INDEX ') for p in extra_snapshots)
coverage['missing_exact_backend_entries']=sum(p.read_text().startswith('NO_EXPORTED_BODY ') for p in extra_snapshots)
coverage['backend_snapshot_count_is_not_semantic_review_count']=True
(OUT/'coverage.json').write_text(json.dumps(coverage,indent=2)+'\n')
print(json.dumps({k:v for k,v in coverage.items() if k!='per_class'},indent=2))
