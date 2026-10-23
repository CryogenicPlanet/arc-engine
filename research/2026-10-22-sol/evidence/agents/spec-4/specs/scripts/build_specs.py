#!/usr/bin/env python3
"""Render conservative research specifications from revalidated metadata and curated review."""
import csv,json,re,sys,struct,collections,hashlib
from pathlib import Path
sys.path.insert(0,str(Path(__file__).parent));from metadata_reader import MachO,tsv
O=Path(__file__).resolve().parents[1];I=Path('/home/exedev/work/inputs');PIN='d0c02bfad08ec92a6ffcf989e53cd995e6381a4c'
rows=list(csv.DictReader(open(O/'evidence/declared-methods.tsv'),delimiter='\t'));layout=json.load(open(O/'evidence/layout.json'));bi=json.load(open(O/'evidence/body-index.json'));bridges=json.load(open(O/'evidence/bridges.json'))
manual={r['imp_va']:r for r in csv.DictReader(open(O/'evidence/manual-method-review.tsv'),delimiter='\t')};notes=json.load(open(O/'evidence/class-notes.json'))
m=MachO(I/'binaries/ArcCore-1.170.0.arm64'); audit=[]
cleanup={'00005810','00008790','00f01bc0','013efb40','040416c0','04b08a00','04b35898','04b087f0'}

def auto(r,s):
 va=r['imp'];sel=r['selector'];ivs=layout[r['class_name']]['ivars'];types=r['type_encoding']
 word,ret=struct.unpack('<II',m.vread(int(va,16),8));base=word&0xffc003ff
 if ret==0xd65f03c0 and base in (0xf9400000,0xb9400000,0x39400000,0x79400000,0xbd400000,0xfd400000):
  sz={0xf9400000:8,0xb9400000:4,0x39400000:1,0x79400000:2,0xbd400000:4,0xfd400000:8}[base];off=((word>>10)&0xfff)*sz
  iv=next((v for v in ivs if v['offset']==off),None);desc=f"Loads {sz} byte(s) from self+{hex(off)}"+(f" ({iv['name']})" if iv else '')+" and returns directly. Use this identity's type encoding; shared IMP names/types may belong to another class."
  audit.append(dict(imp_va=va,raw_8=m.vread(int(va,16),8).hex(),load_bytes=sz,self_offset=off,method_record_va=r['method_record_va']))
  return desc,'high-local','local_field_observed','Dynamic use and inherited lifetime constraints are not tested.'
 if sel=='.cxx_construct':
  stores=re.findall(r'\*\([^\n]+?\)\(param_1 \+ (0x[0-9a-f]+|\d+)\) = ([^;]+);',s)
  return 'Initializes native storage with explicit stores: '+('; '.join('self+'+a+'='+b for a,b in stores) or 'see body; receiver/native storage layout is shown in metadata')+'.','medium-local','local_body_observed','Construction of associated/cached services occurs elsewhere; no general thread guarantee is established.'
 if sel=='.cxx_destruct':
  weak=re.findall(r'_objc_destroyWeak\(param_1 \+ (0x[0-9a-f]+|\d+)',s)
  calls=sorted(set(re.findall(r'\bFUN_([0-9a-f]{8})\(',s))-cleanup)
  return ('Performs C++/ObjC cleanup of native storage'+('; destroys weak ivars at '+', '.join('self+'+x for x in weak) if weak else '')+('; invokes '+', '.join(hex(int(a,16)) for a in calls) if calls else '')+'. Shared control-block decrement/destruction paths are visible where present; this does not establish backend shutdown.'),'medium-local','local_body_observed','Verify backend detachment, callback cancellation and destruction order; helper internals are not all followed.'
 if sel.startswith('mutable') and 'return (void *)(param_1 +' in s:
  off=re.search(r'return \(void \*\)\(param_1 \+ ([^)]+)\)',s)[1]
  return f'Returns pointer to inline native value at self+{off} when optional is engaged; disengaged path calls 0x4b08a00.','high-local','local_field_observed','Pointer is borrowed from receiver storage; external use/retention is not analyzed.'
 if va in ['0x43df53c','0x440a2ac','0x43dfab0','0x43fe184','0x4414800','0x44402c8']:
  off={'0x43df53c':'0x8','0x440a2ac':'0x10','0x43dfab0':'0x30','0x43fe184':'0x20','0x4414800':'0x18','0x44402c8':'0x38'}[va]
  return f'Copies optional native shared handle from self+{off} into C++ return storage; increments control count when engaged, otherwise marks returned optional disengaged.','high-local','local_field_observed','Preserve C++ aggregate/optional return ABI; this is not an ObjC object-returning getter.'
 if va in ['0x43df57c']:
  return 'Assigns optional shared native handle at self+8, with control-count retention/release and explicit engagement transition at self+0x18.','medium-local','local_body_observed','Preserve C++ aggregate argument ABI and replacement cleanup semantics.'
 if 'shared_ptr<' in s and len(s)<1800 and 'return sVar' in s:
  return 'Returns copy of shared bridge/native handle and increments its control count; exact field offset is in the body and layout.','high-local','local_field_observed','Shared bridge ownership does not determine ownership of the ObjC delegate inside it.'
 if sel.startswith('set') and 'shared_ptr<' in s:
  return 'Assigns shared native bridge handle, incrementing new control count and releasing previous shared owner; identity is defined by metadata, despite shared Ghidra name.','medium-local','local_body_observed','Delegate slot ownership and registration/detachment must be checked separately.'
 if 'optional<' in s and '__return_storage_ptr__' in s:
  return 'Returns native optional value in C++ aggregate storage; copies strings/records or calls the visible copy helper when engaged and sets engagement accordingly.','medium-local','local_body_observed','Exact copy-helper semantics and ABI require independent validation.'
 if sel=='isEqual:' and 'isKindOfClass:' in s:
  return 'Checks receiver identity, nil and class compatibility; compares native optional engagement and underlying identity/value after extracting both handles.','medium-local','local_body_observed','Native equality/value interpretation and hash agreement need runtime edge-case checks.'
 if sel=='hash' and 'FUN_00018f3c' in s:
  return 'Extracts native identity and hashes eight bytes using 0x18f3c; absent optional takes contract-failure path.','medium-local','local_body_observed','Hash stability is not promised across processes/versions; check equality agreement.'
 if '_objc_storeWeak replaced' in s:
  return 'Stores ObjC argument into weak slot shown in body; Ghidra injection renders it as a plain assignment.','high-local','local_field_observed','No stronger lifetime or callback guarantee follows from this store.'
 if '_objc_loadWeakRetained replaced' in s:
  return 'Loads weak ObjC slot with loadWeakRetained/autorelease operations documented by Ghidra injection warnings.','high-local','local_field_observed','Do not infer a retained owner cycle from the apparent plain pointer load.'
 if sel.startswith('initWith') and '(292 bytes)' in s and 'plVar4[1]' in s:
  return 'Calls NSObject init, replaces engaged optional native handle, copies implementation/control pointers and increments shared control count.','medium-local','local_body_observed','Native implementation destruction, detachment and callback cancellation remain outside wrapper.'
 if sel.startswith('init') and 'NSObject::init' in s:
  if 'forBrowserContext:' in sel:
   return 'Initializes NSObject, copies native shared optional service handle and assigns browser-context owner slot; owner uses weak accessor/destructor evidence elsewhere in this class.','medium-local','local_body_observed','Verify full service detach/shutdown ordering with owner context.'
  return 'Initializes NSObject and populates metadata-described fields from parameters/native input using visible copy/conversion helpers. Exact stores and helpers are in saved IMP body.','medium-local','local_body_observed','Copy/retention helper semantics and invalid-input behavior are not fully verified.'
 # Exact scalar field expression is useful even if decompiler gives an unrelated ICF type.
 returns=re.findall(r'\breturn ([^;\n]+);',s)
 if len(returns)==1 and 'param_1 +' in returns[0] and 'FUN_' not in returns[0]:
  return 'Reads native field and returns expression '+returns[0]+'. Optional guards/traps in body must be preserved.','high-local','local_field_observed','Public enum meanings, validity and externally supplied state are not established by the field load.'
 if sel.startswith('set') and len(s)<950 and 'param_1 +' in s and 'param_3' in s:
  return 'Writes supplied value to native/ObjC field at offsets visible in body, checking optional engagement when present.','medium-local','local_body_observed','Object ownership is asserted only where explicit ARC helper evidence is recorded.'
 if ('FUN_01c391bc' in s or 'FUN_01d9fb90' in s) and ':' not in sel and len(s)<2200:
  encoding='byte string' if 'FUN_01c391bc' in s else 'UTF-16 string'
  return f'Converts native {encoding} to NSString through '+('0x1c391bc' if 'FUN_01c391bc' in s else '0x1d9fb90')+'; optional state and small/long string branches are visible in body.','medium-local','local_body_observed','Validate conversion errors, nil/empty distinction and native source lifetime.'
 # Record observed pref string rather than infer behavior from name.
 pref=re.search(r'"((?:download|search|credentials|autofill|omnibox)[^" ]*)"',s)
 if pref:
  action='Writes supplied Boolean via native preference service' if sel.startswith('set') else 'Reads native preference value'
  return action+' at key '+pref[1]+'. Profile/native handle checks are visible in body.','medium-local','local_body_observed','Policy precedence, notification ordering and persistence are not fully verified.'
 return 'Body is available; visible calls and metadata are recorded, but a stable behavioral contract has not been established.','unknown','unresolved','Recover behavior, native state transitions and error/callback handling from the listed IMP and callees before implementation.'

for r in rows:
 s=(O/'bodies'/f"{r['imp'][2:]}.c").read_text();va=r['imp'];beh,conf,sem,q=auto(r,s)
 if va in manual:
  beh=manual[va]['behavior'];q=manual[va]['unresolved_question'];conf='medium-reviewed';sem='bridge_reviewed_partial'
 r.update(imp_va=va,behavior=beh,confidence=conf,unresolved_question=q,semantic_status=sem)
 calls=sorted(set(re.findall(r'\bFUN_([0-9a-fA-F]{8})\s*\(',s)))
 named=sorted(set(re.findall(r'([A-Za-z_][\w]*(?:::[A-Za-z_][\w]*)+)\s*\(',s)))
 r['backend_callees']='; '.join(hex(int(v,16)) for v in calls if v.lower() not in cleanup) or 'field/indirect dispatch; see body'
 flags=[]
 if bi[va]['name'].split('::')[0]!=r['class_name']:flags.append('shared_or_category_symbol_identity; metadata takes precedence')
 if 'Could not recover jumptable' in s:flags.append('indirect-tail-dispatch warning')
 if 'Removing unreachable' in s:flags.append('unreachable-block warning')
 if va in ['0x44376b0','0x443be7c','0x4406140']:flags.append('malformed_recovered_prototype');r['body_status']='available_prototype_malformed'
 if va=='0x40b1230':
  flags.append('pseudocode_loses_return; raw ARM64 LDRB/RET proves field getter');r['body_status']='available_pseudocode_incorrect_raw_verified'
 if va=='0x4402480':
  flags.append('cleanup_before_vector_reads; lifetime semantics suspect');r['body_status']='available_semantics_suspect';r['confidence']='low-partial';r['semantic_status']='unresolved'
 r['decompiler_flags']='; '.join(flags)
 r['evidence']=f"ArcCore 1.170.0 IMP {va}; bodies/{va[2:]}.c; ObjC method record {r['method_record_va']}; dobjc170.txt:{r['dyld_line']}; evidence/body-index.json exact byte range"
 r['machine_record']=json.dumps({'method_record_va':r['method_record_va'],'imp_file_offset':r['imp_file_offset'],'dyld_line':int(r['dyld_line'])})

fields=['class_name','category','kind','selector','type_encoding','imp_va','body_status','behavior','backend_callees','evidence','confidence','semantic_status','unresolved_question','decompiler_flags','machine_record']
tsv(O/'methods.tsv',fields,rows);tsv(O/'evidence/accessor-instruction-checks.tsv',['imp_va','raw_8','load_bytes','self_offset','method_record_va'],audit)
ident=lambda r:tuple(r[k] for k in ['class_name','category','kind','selector'])
assert len(set(map(ident,rows)))==len(rows)
aliases=collections.defaultdict(list)
for r in rows:aliases[r['imp_va']].append('|'.join(ident(r)))
json.dump({k:v for k,v in aliases.items() if len(v)>1},open(O/'evidence/shared-imps.json','w'),indent=2)
overrides=[]
for c in layout:
 rs=[r for r in rows if r['class_name']==c];group=collections.defaultdict(list)
 for r in rs:group[(r['kind'],r['selector'])].append(r)
 for k,g in group.items():
  if len(g)>1:overrides.append({'class_name':c,'kind':k[0],'selector':k[1],'definitions':[{x:r[x] for x in ['category','imp_va','type_encoding']} for r in g]})
json.dump(overrides,open(O/'evidence/category-overrides.json','w'),indent=2)

def esc(s):return str(s).replace('|','&#124;').replace('\n',' ')
for c,d in layout.items():
 rs=[r for r in rows if r['class_name']==c];n=notes[c]
 lines=[f'# {c}', '',f"ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. {len(rs)} declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.", '',
 '## State and layout','',f"Superclass `{d['superclass']}`; instanceStart {d['instance_start']}, instanceSize {d['instance_size']} bytes; class metadata `{d['metadata_va']}`, class_ro `{d['ro_va']}`. Declared protocols: {d['protocols'] or 'none'}. Raw binary read and independent dyld reconciliation: [inventory evidence](../evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../evidence/layout.json). These are metadata facts, not an ownership inference.",'','| Ivar | Byte offset | Size | Type |','|---|---:|---:|---|']
 for v in d['ivars']:
  typ=v['type'];short=typ if len(typ)<160 else typ[:120]+'… (full encoding in layout.json)'
  lines.append(f"| `{v['name']}` | {v['offset']} | {v['size']} | `{esc(short)}` |")
 if not d['ivars']:lines.append('| No declared ivars | — | — | — |')
 lines+=['','## Lifecycle, ownership and callbacks','',n['lifecycle'],'','## Threading and caller/callee relations','',n['relations'],'','## Chrome / CEF route implications','',n['route'],'','## Method matrix','',
 'Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../methods.tsv).','',
 '| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |','|---|---|---|---|---|---|---|---|']
 for r in rs:
  conf='H' if r['confidence'].startswith('high') else ('U' if r['confidence'] in ['unknown','low-partial'] else 'M')
  lines.append(f"| {r['category'] or '(base)'} | `{r['kind']}{r['selector']}` | `{esc(r['type_encoding'])}` | [{r['imp_va']}](../bodies/{r['imp_va'][2:]}.c) | {esc(r['behavior'])} | {esc(r['backend_callees'])} | M `{r['method_record_va']}`; D {r['dyld_line']} | {conf}; {r['body_status']}; {r['semantic_status']} |")
 lines+=['','## Remaining consequential questions','',n['questions'],'','Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.']
 (O/'classes'/f'{c}.md').write_text('\n'.join(lines)+'\n')
coverage={'image':'ArcCore 1.170.0 arm64','chromium_version':'156.0.8078.48','chromium_commit':PIN,'assignment':'spec-4','assigned_class_count':38,'assigned_method_rows':491,'actual_class_specs':len(layout),'actual_method_rows':len(rows),'unique_method_identity_count':len(set(map(ident,rows))),'identity_key':['class_name','category','kind','selector'],'unique_imp_count':len(bi),'metadata_reconciliation':json.load(open(O/'evidence/inventory-validation.json')),'exported_body_rows':491,'export_failed_rows':0,'missing_rows':0,'truncated_rows':0,'body_status_counts':dict(collections.Counter(r['body_status'] for r in rows)),'semantic_status_counts':dict(collections.Counter(r['semantic_status'] for r in rows)),'semantically_unresolved_rows':sum(r['semantic_status']=='unresolved' for r in rows),'full_dynamic_semantic_verification_rows':0,'rows_with_unresolved_questions':len(rows),'partial_native_semantics_rows':sum(r['semantic_status']=='bridge_reviewed_partial' for r in rows),'all_rows_have_remaining_validation_questions':True,'deeper_bridge_bodies_read':len(bridges),'bridge_truncated_count':sum(v['truncated'] for v in bridges.values()),'bridge_failed_count':sum(v['failed'] for v in bridges.values()),'category_override_count':len(overrides),'category_overrides':overrides,'by_class':{c:{'methods':sum(r['class_name']==c for r in rows),'body_available':sum(r['class_name']==c for r in rows),'unresolved':sum(r['class_name']==c and r['semantic_status']=='unresolved' for r in rows),'kinds':dict(collections.Counter(r['kind'] for r in rows if r['class_name']==c))} for c in sorted(layout)},'limits':['Declared methods, not measured app calls','No old engine semantics or live macOS execution','No complete call closure; bridge bodies selected explicitly','No summed Ghidra bounds or rewrite/effort estimate','No claim CEF API absence from missing mirrored files','Shared IMP identity independently retained per metadata record','Corrupt prototypes and uncertain lifetime ordering explicitly flagged']}
json.dump(coverage,open(O/'coverage.json','w'),indent=2);print(json.dumps({k:coverage[k] for k in ['actual_class_specs','actual_method_rows','unique_imp_count','body_status_counts','semantic_status_counts','deeper_bridge_bodies_read']},indent=2))
