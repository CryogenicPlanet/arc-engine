#!/usr/bin/env python3
"""Render evidence-bounded research specs; heuristics only describe local syntax, never name unknown backends."""
import csv,json,re,collections,hashlib
from pathlib import Path
from notes import NOTES
from method_notes import OVERRIDES
O=Path('specs');E=O/'evidence';I=Path('/home/exedev/work/inputs')
ms=json.load(open(E/'methods-bodies.json'));ls={r['class_name']:r for r in json.load(open(E/'layouts.json'))};pr={(r['class_name'],r['selector']):r for r in json.load(open(E/'imp-prefixes.json'))}
a=json.load(open(I/'context/class-assignments.json'))['groups']['spec-2'];assert set(NOTES)==set(a['classes'])
fields=['class_name','category','kind','selector','type_encoding','imp_va','body_status','behavior','evidence','confidence','unresolved_question','semantic_status','backend_callees','method_record_va','dyld_line','otool_type_line','otool_imp_line','body_path','body_offset','body_length']
sigart={('ArcURLLoadConfiguration','timeout'),('ArcAIServiceConfiguration','timeout'),('ArcColor','r'),('ArcColor','color'),('ArcWebLoginDialog','isProxyLogin'),('ArcWebLoginDialog','close'),('ArcWebContentsMediaSession','setVolume:error:'),('ArcAnnotatedVisit','toAnnotatedVisit')}
def compact(s):return re.sub(r'\s+',' ',s).strip()
def details(m):
 b=m['body'];c=b[b.find('\n{'):];key=(m['class_name'],m['selector']);p=pr[key]
 calls=list(dict.fromkeys(re.findall(r'\b(FUN_[0-9a-f]+)\s*\(',c)))
 refs=list(dict.fromkeys(re.findall(r'\bFUN_[0-9a-f]+\b',c)))
 backend=', '.join(x.replace('FUN_','0x') for x in calls if x not in ['FUN_00005810','FUN_00008790','FUN_00f01bc0','FUN_013efb40']) or 'local state / indirect operation; see body'
 if len(backend)>220:backend=backend[:217]+'... (full body indexed)'
 offsets=list(dict.fromkeys(re.findall(r'param_1 \+ (0x[0-9a-f]+|\d+)',c)))
 loc=', '.join(hex(int(x,0)) for x in offsets[:8]);guard=' Engagement guard/trap is visible.' if ('SoftwareBreakpoint' in c or 'FUN_04b08a00' in c) else ''
 if key in OVERRIDES:return (*OVERRIDES[key], 'medium','bounded_review',backend)
 if m['selector'].startswith('mutable') and 'return (void *)(param_1 + 8)' in c or m['selector']=='mutableAIInferenceConfiguration':
  return ('Returns interior native storage at self+8 only when optional is engaged; empty state calls nonreturning 0x4b08a00.', 'Borrowed pointer lifetime and mutation rules at callers.', 'medium','local_verified',backend)
 if key==('ArcAnnotatedVisit','toAnnotatedVisit'):
  return ('Returns owned native record: allocates/copies present payload via 0x4336aac; absent payload creates default 0x150-byte record. Ghidra reports type propagation not settling.', 'Hidden return ABI and nested deep-copy semantics require disassembly/consumer validation.', 'medium','bounded_review',backend)
 if key==('ArcExtensionToolbarAction','icon'):
  return ('Returns nil when native image storage absent; otherwise converts through 0x2a02ccc.', 'Image representation, scale and ownership.', 'medium','bounded_review',backend)
 if m['class_name']=='ArcWebContentsDevTools' and m['selector'] in ['inspectElementAt:','openInspectView','openConsoleView','openNetworkView','currentPanel:error:']:
  be={'inspectElementAt:':'Forwards supplied point to 0x43584f8, which checks live target/frame state and invokes an indirect inspection operation.','openInspectView':'Calls 0x4357738, opens elements panel if needed and schedules a native inspect action.','openConsoleView':'Calls 0x4357aa0, which forwards console panel string to 0x4357078.','openNetworkView':'Calls 0x4357b40, which forwards network panel string to 0x4357078.','currentPanel:error:':'Captures result callback, invokes 0x4357be0 and converts immediate status; backend rejects unopened window and requests DevToolsAPI.currentPanel.'}[m['selector']]
  return (be, 'Coordinate units, native UI/panel lifecycle, callback payload/queue and failure behavior beyond inspected bridge.', 'medium','bounded_review',backend)
 if 'leaf_load' in p:
  d=p['leaf_load'];off=d['offset'];name=next((v['name'] for v in ls[m['class_name']]['ivars'] if v['offset']==off),'inline/native field')
  return (f'Reads {d["width_bits"]}-bit {"floating" if d["fp"] else "register"} value from self+{off:#x} ({name}); exact load+RET. Interpret result using this metadata encoding.', '', 'high','local_verified', 'scalar field; machine words in imp-prefixes.json')
 if m['selector'].startswith('.cxx_construct'):
  if m['imp']=='0x15ac0':return ('Shared return-only IMP; constructor leaves state unchanged and x0 self available.','', 'high','local_verified','raw return-only shared IMP')
  return ('Initializes native storage/engagement fields at self offsets '+(loc or 'shown in body')+'; returns self.','Nested constructor/default policy beyond the shown writes.','medium','local_observation',backend)
 if m['selector']=='.cxx_destruct':
  if m['imp']=='0x15ac0':return ('Shared return-only IMP; no field cleanup.','', 'high','local_verified','raw return-only shared IMP')
  mode='Releases shared control-block state and invokes terminal virtual destruction when counts reach zero' if 'LOAcquire' in c else 'Cleans local native/object state using shown stores and cleanup helpers'
  return (mode+'; self offsets '+(loc or 'in body')+'.','Nested cleanup or ObjC ownership fixups beyond inspected wrapper.','medium','local_observation',backend)
 if m['selector'].startswith('init'):
  return ('Initializes object/native state'+(' after NSObject init' if 'NSObject::init' in b else '')+'; conversion/helper calls and assignment branches are preserved in exact body.','Validate native copy/move ownership, defaults and input validation in referenced helper(s).','medium','local_observation',backend)
 if m['selector'].startswith('isEqual'):
  return ('Checks same-object identity, compatible ObjC class, then compares unwrapped native handle pointers; no content equality established.','Native-handle validity/empty-state and object identity policy across wrappers.','medium','bounded_review',backend)
 if m['selector']=='hash':
  return ('Requires engaged native handle and hashes its pointer through 0x18f3c.','Consistency across wrapper instances and invalid native handle state.','medium','bounded_review',backend)
 if m['class_name']=='ArcAIService' and m['category']=='Extensions':
  if m['selector']=='isAvailable':beh='Copies native handle, consults availability through 0x1c5f40, returns low-bit Boolean and releases temporary handle.';q='Availability definition and sequence contract.'
  elif m['selector'] in ['waitForDebuggerToAttach:','crash']:beh='Copies native handle and forwards operation to '+backend+'; wrapper does not establish successful effect.';q='Service-side debugger/crash behavior and failure policy.'
  else:beh='Copies native service handle, marshals inputs and captures supplied callback(s) into native callable state, then forwards to '+backend+'.';q='Exact result/error adapter, callback ownership/once-versus-repeated behavior, delivery queue and unavailable/cancellation behavior.'
  return (beh,q,'medium','bounded_review',backend)
 if m['class_name']=='ArcWebContentsMediaSession' and ':' in m['selector'] and 'FUN_04402548' in c:
  return ('Requires engaged native session; forwards native action/input then converts returned status to BOOL and optional NSError via 0x4402548.','Action support, numeric units/range, native error branch and service callback thread.','medium','bounded_review',backend)
 if 'FUN_01c391bc' in c and ':' not in m['selector'] and len(c)<2300:
  return ('Converts native byte-string value to ObjC string using 0x1c391bc (CFStringCreateWithBytes encoding 0x08000100). Self offsets: '+loc+'.'+guard,'Producer meaning, invalid-string behavior and snapshot freshness.','medium','local_observation',backend)
 if 'FUN_01d9fb90' in c and ':' not in m['selector'] and len(c)<2300:
  return ('Converts native UTF-16 storage to ObjC string using 0x1d9fb90 (length × 2; CF encoding 0x14000100). Self offsets: '+loc+'.'+guard,'Producer meaning and invalid-string/absence behavior.','medium','local_observation',backend)
 if 'FUN_043abd34' in c and ':' not in m['selector'] and len(c)<2000:
  return ('Converts native URL/string storage through 0x43abd34; self offsets '+loc+'.'+guard,'URL parsing/normalization and invalid/empty URL result semantics.','medium','local_observation',backend)
 # Short field operations: quote recovered expression rather than invent a semantic name.
 statements=[compact(x) for x in c.splitlines() if ('return ' in x or ('param_1 +' in x and ' = ' in x and x.strip().endswith(';')))]
 statements=[x for x in statements if x!='return;' and not x.startswith('/*')]
 if len(c)<850 and not calls and statements:
  return ('Local field operation: '+'; '.join(statements[:4])+guard,'Enum/value domain and caller ownership beyond this local operation.' if ':' in m['selector'] or 'SoftwareBreakpoint' in c else '', 'medium','local_verified',backend)
 if m['selector'].startswith('set'):
  return ('Assigns/replaces local native or ObjC state; self offsets '+(loc or 'in exact body')+'. Calls '+backend+'.'+guard,'Copy/move/retain details, nested assignment and any cache invalidation not established beyond wrapper.','medium','local_observation',backend)
 if 'optional<' in m['type_encoding'] or ('__return_storage_ptr__' in b):
  return ('Copies native aggregate into return storage, with engagement/copy branches in body. Self offsets '+(loc or 'in exact body')+'.'+guard,'Native aggregate ABI, nested copy ownership and empty-state behavior at consumer.','medium','local_observation',backend)
 return ('Unresolved beyond local wrapper: fields '+(loc or 'in body')+'; direct helper calls '+backend+'.'+guard,'Identify helper/indirect callee and validate full behavior before implementing.','low','unresolved',backend)
rows=[]
for m in ms:
 beh,q,conf,sem,backend=details(m);key=(m['class_name'],m['selector'])
 # Every behavioral row has its own exact engine VA, independently of aliases.
 ev=f'ArcCore-1.170.0 IMP {m["imp"]}; corpus {m["body_path"]}@{m["body_offset"]}+{m["body_length"]}; metadata method_record_va {m["method_record_va"]}; dobjc170.txt:{m["dyld_line"]}; objc170.txt:{m["otool_type_line"]},{m["otool_imp_line"]}'
 if key in OVERRIDES or 'leaf_load' in pr[key]:ev+='; imp-prefixes.json' if 'leaf_load' in pr[key] or key in sigart else ''
 body_status='available_signature_artifact' if key in sigart else 'available'
 if m['failed']:body_status='failed';sem='unresolved';conf='low';q='Recover failed body.'
 if not m['body']:body_status='missing';sem='unresolved';conf='low';q='Obtain missing exact IMP body.'
 rows.append(dict(class_name=m['class_name'],category=m['category'],kind=m['kind'],selector=m['selector'],type_encoding=m['type_encoding'],imp_va=m['imp'],body_status=body_status,behavior=beh,evidence=ev,confidence=conf,unresolved_question=q,semantic_status=sem,backend_callees=backend,method_record_va=m['method_record_va'],dyld_line=m['dyld_line'],otool_type_line=m['otool_type_line'],otool_imp_line=m['otool_imp_line'],body_path=m['body_path'],body_offset=m['body_offset'],body_length=m['body_length']))
with open(O/'methods.tsv','w') as f:w=csv.DictWriter(f,fieldnames=fields,delimiter='\t');w.writeheader();w.writerows(rows)
# Shared implementations remain distinct metadata identities.
groups=collections.defaultdict(list)
for m in ms:groups[m['imp']].append({k:m[k] for k in ['class_name','category','kind','selector','type_encoding','method_record_va']})
json.dump({k:v for k,v in groups.items() if len(v)>1},open(E/'shared-imps.json','w'),indent=2)
collisions=[]
for cl in a['classes']:
 rr=[r for r in rows if r['class_name']==cl];lay=ls[cl];note=NOTES[cl]
 cats=sorted({r['category'] for r in rr if r['category']});base={(r['kind'],r['selector']) for r in rr if not r['category']}
 collisions.extend([(cl,r['category'],r['kind'],r['selector']) for r in rr if r['category'] and (r['kind'],r['selector']) in base])
 def cell(s):return str(s).replace('|','&#124;').replace('\n',' ')
 text=f'# {cl}\n\nArcCore **1.170.0 arm64**, Chromium **156.0.8078.48**. All addresses are unslid engine VAs. {len(rr)} declared instance methods; no declared class methods. Categories: {", ".join(cats) or "none"}. Metadata identity includes category and +/- kind.\n\n'
 text+=f'## State and layout\n\nSuperclass `{lay["superclass"]}`; metadata class `{lay["metadata_va"]}`, class_ro `{lay["ro_va"]}`; instanceStart {lay["instance_start"]}, instanceSize {lay["instance_size"]} bytes. These are metadata sizes, not estimates of work or function extents. Full native encodings: [layouts.json](../evidence/layouts.json).\n\n| Offset | Size | Ivar | Encoding (long encodings abbreviated here) | Metadata record VA |\n|---:|---:|---|---|---|\n'
 for v in lay['ivars']:text+=f'| {v["offset"]:#x} | {v["size"]} | `{v["name"]}` | `{cell(v["type"][:130]+("…" if len(v["type"])>130 else ""))}` | {v["record_va"]} |\n'
 text+='\n'+note[0]+'\n\n## Lifecycle, callbacks and relations\n\n'+note[1]+'\n\n'
 text+='Threading: no explicit queue dispatch or caller-thread assertion was established in these inspected IMPs/selected bridges. Atomic reference-count instructions do not establish a safe concurrent API. Callback queue/reentrancy remains unverified; source API requirements below are requirements of candidate backends, not measured Arc scheduling.\n\n'
 text+='## Backend implications\n\n'+note[2]+'\n\nAll Chromium file/line citations use commit **d0c02bfad08ec92a6ffcf989e53cd995e6381a4c**. All CEF citations use **426920c33ef22551e0b8f48cf343d623ac25642d**. Domain comparisons and source-backed inferences are labeled; unnamed Arc functions are not assigned an upstream identity solely from names.\n\n'
 text+='## Method matrix\n\n`base` means the class method list, not a category. Type encodings are copied from binary records. `verified` below is local behavior only; `bounded_review` follows a selected bridge; `local_observation` remains partial; `unresolved` needs semantic recovery. Signature artifacts are explicitly recorded and do not mean a failed export. Full exact bodies and metadata anchors: [class body evidence](../evidence/'+cl+'.txt), [indexed machine-readable bodies](../evidence/methods-bodies.json), [method rows](../methods.tsv), [bridge bodies](../evidence/bridges.json).\n\n'
 text+='| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / direct callees | Evidence | Confidence / status |\n|---|---|---|---|---|---|---|---|\n'
 for r in rr:
  evidence=f'IMP {r["imp_va"]}; record {r["method_record_va"]}; dyld L{r["dyld_line"]}; otool L{r["otool_type_line"]}'
  text+='| '+' | '.join(cell(x) for x in [r['category'] or 'base',r['kind']+' '+r['selector'],'`'+r['type_encoding']+'`',r['imp_va'],r['behavior'],r['backend_callees'],evidence,r['confidence']+' / '+r['body_status']+' / '+r['semantic_status']])+' |\n'
 text+='\n## Remaining consequential questions\n\n'+note[3]+' Additional row-level gaps are in `unresolved_question` in methods.tsv. No direct Arc 1.168 app-call count is asserted for this class; declarations and returned-object reachability are separate evidence.\n'
 (O/'classes'/f'{cl}.md').write_text(text)
coverage=dict(image='ArcCore-1.170.0.arm64',address_basis='unslid Mach-O virtual addresses; offsets translated via LC_SEGMENT_64',upstream_commit='d0c02bfad08ec92a6ffcf989e53cd995e6381a4c',cef_commit='426920c33ef22551e0b8f48cf343d623ac25642d',assigned_classes=len(a['classes']),assigned_declared_methods=a['method_rows'],actual_classes=len(ls),actual_method_rows=len(rows),unique_metadata_identities=len({tuple(r[k] for k in ['class_name','category','kind','selector']) for r in rows}),unique_imps=len(groups),instance_methods=len(rows),class_methods=0,category_counts=dict(collections.Counter(r['class_name']+'('+r['category']+')' for r in rows if r['category'])),base_category_selector_collisions=collisions,body_available=sum(bool(m['body']) and not m['failed'] for m in ms),body_failed=sum(bool(m['failed']) for m in ms),body_missing=sum(not m['body'] for m in ms),body_truncated=sum(m['truncated'] for m in ms),body_malformed_established=0,body_quality_not_proven_by_export=True,body_status_counts=dict(collections.Counter(r['body_status'] for r in rows)),semantic_status_counts=dict(collections.Counter(r['semantic_status'] for r in rows)),unresolved_rows=sum(bool(r['unresolved_question']) for r in rows),explicitly_semantically_unresolved_rows=sum(r['semantic_status']=='unresolved' for r in rows),fully_verified_methods=0,local_semantic_verification_only=True,raw_two_instruction_load_ret_rows=sum('leaf_load' in x for x in pr.values()),selected_bridge_bodies=len(json.load(open(E/'bridges.json'))),direct_app_calls_analyzed=0,direct_app_calls_scope='not measured in this stage; zero analyzed does not mean zero calls',inventory_reconciliation='491 rows agree binary vs initial identity/type/record, binary vs dyld identity/IMP; all encoding/IMP pairs corroborated in otool',per_class={cl:dict(method_rows=sum(r['class_name']==cl for r in rows),body_available=sum(m['class_name']==cl and bool(m['body']) and not m['failed'] for m in ms),semantic_status_counts=dict(collections.Counter(r['semantic_status'] for r in rows if r['class_name']==cl))) for cl in a['classes']})
json.dump(coverage,open(O/'coverage.json','w'),indent=2)
print(json.dumps({k:v for k,v in coverage.items() if k!='per_class'},indent=2))
