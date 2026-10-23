"""Reproducible bounded specifications: conservative body rules plus reviewed notes.
Rules expose local observations; they never identify anonymous functions as Chromium.
"""
import json,csv,re,struct,collections,sqlite3
from pathlib import Path
from metadata import MachO,tsv
out=Path('specs');inp=Path('/home/exedev/work/inputs')
rows=json.load(open(out/'evidence/methods.json'));layouts={r['class_name']:r for r in json.load(open(out/'evidence/layout.json'))};notes=json.load(open('scripts/reviewed.json'))
m=MachO(inp/'binaries/ArcCore-1.170.0.arm64');binary=[]

def body(r):
 p=out/'bodies'/f'{r["imp"]}.c';return p.read_text() if p.exists() else ''
def direct_calls(s):
 # Excludes comments/signature; literal callee syntax only. Pointer assignments are a separate closure edge.
 s=s[s.find('\n{')+2:];return list(dict.fromkeys('0x'+x.lstrip('0') for x in re.findall(r'\b(?:thunk_)?FUN_([0-9a-fA-F]+)\s*\(',s)))
def classify(r):
 s=body(r);sel=r['selector'];l=layouts[r['class_name']];va=int(r['imp'],16)
 w,w2=struct.unpack('<II',m.vread(va,8));rec=dict(imp_va=r['imp'],file_offset=r['imp_file_offset'],word0=hex(w),word1=hex(w2))
 # Unambiguous load from x0 to return register and RET. All offsets are encoded immediates.
 bases={0x39400000:('byte',1),0xb9400000:('32-bit',4),0xf9400000:('64-bit',8),0xbd400000:('float',4),0xfd400000:('double',8)}
 for base,(typ,scale) in bases.items():
  if w&0xffc003ff==base and w2==0xd65f03c0:
   offset=((w>>10)&4095)*scale;rec.update(operation='load_then_ret',field_offset=hex(offset),width=typ);binary.append(rec)
   field=next((v['name'] for v in l['ivars'] if v['offset']==offset),f'inline/native storage +{hex(offset)}')
   return f'Returns {typ} field {field} at self+{hex(offset)}; no backend call.', 'local_verified','high',''
 if not s:
  if w&0xfc000000==0x14000000:
   delta=w&0x3ffffff;delta=delta-(1<<26) if delta&(1<<25) else delta;target=va+delta*4;rec.update(operation='branch_veneer',target_va=hex(target));binary.append(rec)
   return f'Branch veneer to {hex(target)}; no separately exported body.', 'veneer_verified','high','Target lifetime and view contract require callee review.'
  return 'No indexed body; behavior unknown.', 'unresolved','unknown','Obtain exact body or bounded disassembly.'
 # Simple injected stores are local facts; ownership must come separately from property attrs/raw calls.
 core=s[s.find('\n{')+2:];core=re.sub(r'/\*.*?\*/','',core,flags=re.S).strip()
 if re.fullmatch(r'\*\([^\n]+\)\(param_1 \+ (?:0x[0-9a-f]+|\d+)\) = [^;]+;\s*return;\s*}',core):
  return 'Stores the supplied value in the declared backing field; see layout/property attributes for ownership declaration.', 'local_verified','high',''
 if sel=='.cxx_construct':
  return 'Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body.', 'local_verified','medium','Full C++ aggregate construction ABI remains to be validated.'
 if sel=='.cxx_destruct':
  desc='Destroys backing storage'
  if 'LOAcquire' in s:desc+=' with shared-count decrement and last-owner virtual destruction'
  if '_objc_destroyWeak' in s:desc+='; explicitly destroys weak ObjC storage'
  if re.search(r'\*\(undefined8 \*\).* = 0;',s):desc+='; clears object/backing fields'
  return desc+'. ARC injection may hide retain/release operations.', 'local_verified','medium','Indirect destructor targets and thread of destruction are not exhaustively recovered.'
 if sel.startswith('initWith') and any('optional<' in v['type'] and 'shared_ptr<' in v['type'] for v in l['ivars']) and 'NSObject::init' in s and 'lVar1 + 8) = *(long *)(lVar1 + 8) + 1' in s:
  return 'Calls super init; copies the supplied native shared peer into optional storage and increments its control count; returns nil if super init fails.', 'local_verified','high','Creation/detachment of the native Impl lies outside this wrapper.'
 if sel.startswith('mutable') and 'return (void *)(param_1 +' in s:
  offset=re.search(r'return \(void \*\)\(param_1 \+ ([^)]+)\)',s)[1]
  return f'Returns address of inline native value at self+{offset} when engaged; aborts on empty optional.', 'local_verified','high','Caller mutation and pointer escape lifetime must be audited.'
 if r['type_encoding'].startswith('{optional<') and '__return_storage_ptr__' in s and '*plVar1 = *plVar1 + 1' in s:
  return 'Returns native optional by value; empty stays empty, engaged shared peer increments its control count.', 'local_verified','high','C++ optional return/storage ABI needs cross-toolchain validation.'
 if 'FUN_01c391bc' in s or 'FUN_01d9fb90' in s:
  calls=direct_calls(s)
  allowed={'0x1c391bc','0x1d9fb90','0x5810','0x54420','0x60a60','0x7d0cf8'}
  if set(calls)<=allowed and sel not in ['supportedTranslateLanguages']:
   enc='UTF-8' if 'FUN_01c391bc' in s else 'UTF-16LE'
   return f'Converts stored/native {enc} string data to NSString via '+('0x1c391bc' if enc=='UTF-8' else '0x1d9fb90')+('; returns nil for absent optional string.' if re.search(r'IVar\d+ = 0;',s) else '.')+(' Traps if outer optional is empty.' if 'SoftwareBreakpoint(1' in s else ''), 'local_verified','medium','Native string validity and invalid-encoding behavior need runtime validation.'
 if ':' not in sel and re.search(r'return (?:\([^;]+\))?\*[^;]+;',s) and not direct_calls(s) and len(s)<1600:
  return 'Returns native/inline field'+('; traps when the backing optional is empty.' if 'SoftwareBreakpoint' in s else '.'), 'local_verified','medium','Producer semantics and enum meanings are not established by the field load.'
 return 'Indexed body available; local calls recorded, end-to-end behavior unresolved.', 'unresolved','low','Audit native preconditions, result/error shape, callbacks and downstream service behavior.'

methods=[]
for r in rows:
 behavior,semantic,confidence,question=classify(r);key='|'.join((r['class_name'],r['category'],r['kind'],r['selector']))
 n=notes.get('methods',{}).get(key)
 if n:behavior=n['behavior'];semantic=n.get('status','reviewed_partial');confidence=n.get('confidence','medium');question=n.get('question','Downstream closure, callback ordering and runtime parity remain unverified.')
 s=body(r);calls=direct_calls(s)
 sends=list(dict.fromkeys(re.findall(r'\"([A-Za-z_.][A-Za-z0-9_.:]*:)\"',s)))
 evidence=f'ArcCore 1.170.0 {r["imp"]}; metadata method_record {r["method_record_va"]}; dobjc170.txt:{r["dyld_line"]}; bodies/{r["imp"]}.c' if s else f'ArcCore 1.170.0 {r["imp"]}; metadata method_record {r["method_record_va"]}; dobjc170.txt:{r["dyld_line"]}; evidence/binary-checks.tsv'
 if n and n.get('evidence'):evidence+='; '+n['evidence']
 # Inlined empty-return output is a decompiler problem, even when repaired by bytes.
 body_status=r['body_status']
 if r['imp']=='0x4410bb4':body_status='missing_body_verified_branch_veneer'
 elif r['imp'] in ['0x40b1230','0x43dee04']:body_status='available_incoherent_return_repaired_by_binary'
 elif r['imp']=='0x43dee9c':body_status='available_incoherent_casts_repaired_by_binary'
 method=dict(class_name=r['class_name'],category=r['category'],kind=r['kind'],selector=r['selector'],type_encoding=r['type_encoding'],imp_va=r['imp'],body_status=body_status,behavior=behavior,evidence=evidence,confidence=confidence,unresolved_question=question,semantic_status=semantic,backend_callees=','.join(calls)+(' ; ObjC selectors '+','.join(sends) if sends else ''),method_record_va=r['method_record_va'],dyld_line=r['dyld_line'])
 methods.append(method)
tsv(out/'methods.tsv',list(methods[0]),methods);tsv(out/'evidence/binary-checks.tsv',sorted(set().union(*(r.keys() for r in binary))),binary)
json.dump([r for r in methods if r['semantic_status']=='unresolved'],open(out/'evidence/unresolved.json','w'),indent=2)
classes=json.load(open(inp/'context/class-assignments.json'))['groups']['spec-1']['classes']
esc=lambda s:str(s).replace('|','\\|').replace('\n',' ')
for c in classes:
 l=layouts[c];ms=[r for r in methods if r['class_name']==c];n=notes.get('classes',{}).get(c,{})
 lines=[f'# {c}', '',f'ArcCore 1.170.0 arm64. {len(ms)} declared rows; superclass `{l["superclass"]}`. Class metadata `{l["metadata_va"]}`, class_ro `{l["ro_va"]}`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.','',n.get('overview','Native value/wrapper surface; preserve the exact method identity and encoding matrix below.'),'','## State and layout','',f'Metadata instance start {l["instance_start"]}, size {l["instance_size"]} bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.','', '| Ivar | Offset | Bytes | Encoding | Metadata record |','|---|---:|---:|---|---|']
 for i in l['ivars']:lines.append(f'| `{i["name"]}` | {hex(i["offset"])} | {i["size"]} | `{esc(i["type"])}` | `{i["record_va"]}` |')
 lines+=['','Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):','']
 for p in l['properties']:lines.append(f'- `{p["name"]}`: `{esc(p["attributes"])}` at `{p["record_va"]}`.')
 lines+=['','## Lifecycle, ownership, callbacks and threading','',n.get('lifecycle','Initializer, native getter and destructor observations are listed at their exact IMPs below. Native peer creation and detachment are outside the bounded wrapper review. Do not infer retain/copy/weak semantics from a getter name. Pseudocode ARC injections suppress operations; metadata ownership attributes remain declarations rather than a runtime proof.'),'',n.get('threading','No thread-affinity contract is established by this review. No caller-thread guarantee follows from a synchronous-looking selector or a shared control block. Callback delivery, where present, needs an explicit caller/task-runner audit.'),'','## Relations and engine route','',n.get('route','The examined methods expose Arc-specific object/value representations. A replacement needs an adapter retaining the declared ObjC/C++ boundary; no direct CEF equivalence is established. Backend identity is unresolved unless a source-backed inference is explicitly cited below.'),'','## Method matrix','', 'Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.','', '| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |','|---|---|---|---|---|---|---|---|---|']
 for r in ms:
  ev=f'[{r["imp_va"]}](../bodies/{r["imp_va"]}.c); record {r["method_record_va"]}; dyld L{r["dyld_line"]}' if (out/'bodies'/f'{r["imp_va"]}.c').exists() else f'{r["imp_va"]}; record {r["method_record_va"]}; dyld L{r["dyld_line"]}; binary-checks.tsv'
  if '; evidence/' in r['evidence']:ev+='; '+r['evidence'].split('; evidence/',1)[1].split(';')[0].join(['evidence/',''])
  lines.append('| '+' | '.join(map(esc,[r['category'] or '(base)',r['kind']+r['selector'],'`'+r['type_encoding']+'`',r['imp_va'],r['behavior'],r['backend_callees'] or 'No direct anonymous call recorded / indirect possible',ev,r['confidence']+' / '+r['semantic_status']+' / '+r['body_status'],r['unresolved_question'] or 'No additional local question; runtime parity untested.']))+' |')
 lines+=['','## Remaining implementation questions','',n.get('questions','Validate caller-visible nil/error behavior, aggregate calling convention, peer lifetime and callback ordering on macOS. Direct app-call reachability is not measured by these declaration rows.'),'']
 (out/'classes'/f'{c}.md').write_text('\n'.join(lines))
byclass={c:dict(method_rows=len([r for r in methods if r['class_name']==c]),body_status=dict(collections.Counter(r['body_status'] for r in methods if r['class_name']==c)),semantic_status=dict(collections.Counter(r['semantic_status'] for r in methods if r['class_name']==c))) for c in classes}
combos=collections.defaultdict(list)
for r in methods:combos[(r['class_name'],r['kind'],r['selector'])].append(r)
coverage=dict(image='ArcCore-1.170.0.arm64',upstream_version='156.0.8078.48',chromium_commit='d0c02bfad08ec92a6ffcf989e53cd995e6381a4c',assigned_class_count=34,generated_class_count=len(classes),assigned_method_rows=491,actual_method_rows=len(methods),identity_fields=['class_name','category','kind','selector'],method_identity_unique=len({tuple(r[k] for k in ['class_name','category','kind','selector']) for r in methods}),base_method_rows=sum(not r['category'] for r in methods),category_method_rows=sum(bool(r['category']) for r in methods),class_method_rows=sum(r['kind']=='+' for r in methods),instance_method_rows=sum(r['kind']=='-' for r in methods),distinct_imp_vas=len({r['imp_va'] for r in methods}),body_available_rows=sum(r['body_status'].startswith('available') for r in methods),failed_rows=sum(r['body_status']=='failed' for r in methods),missing_indexed_body_rows=sum(r['body_status'].startswith('missing') for r in methods),body_status=dict(collections.Counter(r['body_status'] for r in methods)),semantic_status=dict(collections.Counter(r['semantic_status'] for r in methods)),semantic_unresolved_rows=sum(r['semantic_status']=='unresolved' for r in methods),rows_with_open_question=sum(bool(r['unresolved_question']) for r in methods),full_end_to_end_semantics_verified=0,direct_app_called_method_count=None,direct_app_selector_sample_count=3,direct_app_branch_sample_count=3,direct_app_runtime_receiver_classes_verified=0,body_extraction_limit_bytes=65536,by_class=byclass,base_category_collisions=[dict(class_name=k[0],kind=k[1],selector=k[2],rows=[{f:r[f] for f in ['category','imp_va','behavior']} for r in v]) for k,v in combos.items() if len(v)>1],limits=['No summed Ghidra bounds used','No complete proprietary closure claim','No dynamic macOS execution or ABI conformance','Local observations and source hypotheses are separate from verified downstream semantics'])
json.dump(coverage,open(out/'coverage.json','w'),indent=2)
print(json.dumps({k:v for k,v in coverage.items() if k not in ['by_class','base_category_collisions','limits']},indent=2))
