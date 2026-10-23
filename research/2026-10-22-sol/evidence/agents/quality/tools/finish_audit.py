#!/usr/bin/env python3
"""Derive review gates and compact cross-checks from independently scanned evidence."""
import array,bisect,collections,csv,hashlib,json,re,sqlite3,struct
from pathlib import Path
I=Path('/home/exedev/work/inputs');O=Path('initial');D=sqlite3.connect(O/'audit.sqlite');D.execute('PRAGMA cache_size=-12000')
def tsv(n,cols,rows):
 with (O/n).open('w') as f:w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(cols);w.writerows(rows)
def rows(n):return list(csv.DictReader((O/n).open(),delimiter='\t'))
def sha(p):
 h=hashlib.sha256()
 with p.open('rb') as f:
  for b in iter(lambda:f.read(1048576),b''):h.update(b)
 return h.hexdigest()
j=json.loads((O/'coverage.json').read_text());inv=json.loads((O/'inventory.json').read_text());cl=rows('closure-audit.tsv');ms=rows('method-coverage.tsv');cs=rows('class-coverage.tsv')
starts=array.array('Q');ends=array.array('Q')
for a,e in D.execute('select va,end from meta order by va'):starts.append(a);ends.append(e)
prior=0;selfover=set();isolated=set();priorover=set()
for idx,(a,b) in enumerate(zip(starts,ends)):
 if a<prior:priorover.add(a)
 if idx+1<len(starts) and b>starts[idx+1]:selfover.add(a)
 if a>=prior and (idx+1==len(starts) or b<=starts[idx+1]) and b>a:isolated.add(a)
 prior=max(prior,b)
unique_c_coverage={int(r['body_va'],16) for r in cl};seedvas={int(r['body_va'],16) for r in cl if r['depth']=='0'}
nameclasses={r['script_name'].split('::')[0] for r in cl if r['reason']=='arc-objc'}
summary={}
for level in ['all','seed','arc-objc','arc-path','callee']:
 cr=[r for r in cl if level=='all' or (level=='seed' and r['depth']=='0') or r['reason']==level];vas={int(r['body_va'],16) for r in cr}
 summary[level]={'rows':len(cr),'own_enclosing_span_crosses_later_start':len(vas&selfover),'entry_inside_prior_enclosing_span':len(vas&priorover),'isolated_enclosing_span':len(vas&isolated),'actual_header_body_count_differs_from_TSV_at_same_VA':sum(D.execute('select bodies.n!=meta.n from bodies join meta using(va) where va=?',(int(r['body_va'],16),)).fetchone()[0] for r in cr),'unrecovered_jumptable_bodies':sum('Could not recover jumptable' in r['warnings'] for r in cr),'type_propagation_unsettled_bodies':sum('Type propagation algorithm not settling' in r['warnings'] for r in cr)}
j['handoff']['structural_risks_by_group']=summary
j['handoff']['arc_named_seed_classes']=len(nameclasses)
j['handoff']['sum_after_excluding_0421f778_NOT_code_coverage']=sum(int(r['name_join_tsv_span']) for r in cl if r['body_va']!='0x421f778')
j['handoff']['all_rows_lack_explicit_VA_in_supplied_file']=True
j['metadata_methods']={'distinct_IMPs':len({m['engine170_va'] for m in ms}),'declarations_in_name_based_closure_by_IMP':sum(int(m['engine170_va'],16) in unique_c_coverage for m in ms),'distinct_IMPs_in_name_based_closure':len({int(m['engine170_va'],16) for m in ms}&unique_c_coverage),'declarations_in_Arc_named_seeds_by_IMP':sum(int(m['engine170_va'],16) in {int(r['body_va'],16) for r in cl if r['reason']=='arc-objc'} for m in ms)}
# Establish exact symbol aliases at shared IMPs and cross-name closure gaps.
byva=collections.defaultdict(list)
for m in ms:byva[m['engine170_va']].append(m)
tsv('shared-IMP-aliases.tsv',['va','declarations','declaration_keys','C_name','in_handoff_closure'],((v,len(mm),json.dumps([[m['class'],m['category'],m['kind'],m['selector']] for m in mm]),mm[0]['C_name_at_IMP'],int(int(v,16) in unique_c_coverage)) for v,mm in byva.items() if len(mm)>1))
tsv('methods-outside-closure.tsv',list(ms[0]),([m[k] for k in ms[0]] for m in ms if int(m['engine170_va'],16) not in unique_c_coverage))
# Warning signal counts are function-level, distinct from occurrences. Their absence isn't assurance.
warnsummary={}
for tag in ['bad instruction','unimplemented instructions','Could not recover jumptable','Type propagation algorithm not settling']:
 ct=0
 for (raw,) in D.execute("select warnings from bodies where warnings!='{}'"):
  if tag.lower() in raw.lower():ct+=1
 warnsummary[tag]=ct
j['warning_function_counts']=warnsummary
# Missing entries named thunks vs others; naming itself cannot prove Ghidra isThunk state.
j['missing_header_name_hints']=dict(zip(['thunk_prefix','other_name'],D.execute("select sum(meta.name like 'thunk_%'),sum(meta.name not like 'thunk_%') from meta left join bodies using(va) where bodies.va is null").fetchone()))
# Read one missing metadata method's branch plus export thunk using VA->segment mapping.
p=I/'binaries/ArcCore-1.170.0.arm64'
with p.open('rb') as f:
 hdr=f.read(32);nc,sz=struct.unpack_from('<II',hdr,16);lcs=f.read(sz);pos=0;segments=[];symexports=[]
 for z in range(nc):
  cmd,size=struct.unpack_from('<II',lcs,pos);lc=lcs[pos:pos+size]
  if cmd==0x19:
   sn,va,vs,fo,fs=struct.unpack_from('<16sQQQQ',lc,8);segments.append((va,fo,fs,sn.split(b'\0')[0].decode()))
   for k in range(struct.unpack_from('<I',lc,64)[0]):
    sn2,sg2,sa,ss=struct.unpack_from('<16s16sQQ',lc,72+k*80)
    if sn2.split(b'\0')[0]==b'__text' and sg2.split(b'\0')[0]==b'__TEXT':text_start,text_end=sa,sa+ss
  if cmd==0x80000033:trieoff,triesize=struct.unpack_from('<II',lc,8)
  elif cmd in (0x22,0x80000022):trieoff,triesize=struct.unpack_from('<II',lc,40)
  pos+=size
 def word(va):
  sg=next(s for s in segments if s[0]<=va and va+4<=s[0]+s[2]);off=sg[1]+va-sg[0];f.seek(off);return off,int.from_bytes(f.read(4),'little')
 prows=[]
 for v in [0x4410bb4,0x441d770]:
  off,w=word(v);target=None
  if w&0xfc000000==0x14000000:
   imm=w&0x3ffffff;imm=imm-(1<<26) if imm&(1<<25) else imm;target=v+imm*4
  prows.append([hex(v),hex(off),f'0x{w:08x}','B' if target is not None else 'other','' if target is None else hex(target)])
 f.seek(trieoff);tr=f.read(triesize)
 def uleb(o):
  v=s=0
  while True:
   x=tr[o];o+=1;v|=(x&127)<<s
   if not x&128:return v,o
   s+=7
 def walk(o,prefix,parents):
  assert o not in parents;n,p=uleb(o);q=p+n
  if n:symexports.append(prefix)
  count=tr[q];q+=1
  for k in range(count):e=tr.index(0,q);edge=tr[q:e].decode();child,q=uleb(e+1);walk(child,prefix+edge,parents|{o})
 walk(0,'',set())
tsv('branch-probes.tsv',['engine170_va','MachO_file_offset','instruction_word','decode','target_va'],prows)
actual=set(symexports);expected=set((I/'context/analysis/data/core170-exports.txt').read_text().splitlines());j['engine170_binary_export_trie_matches_supplied_names']=actual==expected;j['engine170_binary_export_trie_names']=len(actual)
j['bounds']['isolated_spans_within_TEXT_text']=sum(text_start<=a<b<=text_end for a,b in zip(starts,ends) if a in isolated)
j['bounds']['TEXT_text_VA_bounds']=[hex(text_start),hex(text_end)]
# Exact old-version unknowns are re-audited by identity, preserving the ABI agent's unknown status.
abi=Path('/home/exedev/work/agents/abi/initial');unknown=[]
for r in csv.DictReader((abi/'all-methods-168.tsv').open(),delimiter='\t'):
 if r['class_name'].startswith('Arc') and not r['type_encoding']:unknown.append(r)
if unknown:tsv('old-encoding-unknowns.tsv',list(unknown[0]),([r[k] for k in unknown[0]] for r in unknown))
j['old_Arc_encoding_unknowns_retained']=len(unknown)
# All assigned class inventories must be represented. Text presence is NOT a review gate.
specrows=[]
for r in cs:
 c,g=r['class'],r['spec_group'];base=Path('/home/exedev/work/agents')/g/'specs';md=list(base.glob(c+'.md'))
 specrows.append([c,g,r['declarations'],r['distinct_IMPs'],r['C_headers_at_IMPs'],r['failed_headers'],int(bool(md)),'' if not md else str(md[0]),'provisional until method-level behavior/state/threading/ownership/error/callback/native-hosting claims reviewed; IMP aliases and category kind identity retained'])
tsv('spec-expectations.tsv',['class','owner_group','expected_method_declarations','distinct_IMPs','headers_available','failed_headers','class_md_seen_snapshot','class_md_path','required_review'],specrows)
j['class_spec_expectations']={'classes':len(specrows),'methods':sum(int(x[2]) for x in specrows),'by_group':dict(collections.Counter(r[1] for r in specrows)),'snapshot_class_md_seen':sum(r[6] for r in specrows),'absence_of_spec_file_is_not_absence_of_analysis':True}
# Independently compare available spec inventories, including category/kind/IMP.
spec_checks=[]
for g,fn in [('spec-1','methods.json'),('spec-2','methods-bodies.json'),('spec-3','metadata.json'),('spec-4','declared-methods.tsv')]:
 p=Path('/home/exedev/work/agents')/g/'specs/evidence'/fn
 if not p.exists():spec_checks.append([g,str(p),'absent',0,0,0]);continue
 if p.suffix=='.tsv':rr=list(csv.DictReader(p.open(),delimiter='\t'))
 else:
  raw=json.loads(p.read_text());rr=raw['methods'] if isinstance(raw,dict) else raw
 def key(r):return (r['class_name'],r.get('category',''),r['kind'],r['selector'],int(str(r['imp']),0))
 seen=collections.Counter(key(r) for r in rr)
 expected=collections.Counter((m['class'],m['category'],m['kind'],m['selector'],int(m['engine170_va'],0)) for m in ms if m['spec_group']==g)
 spec_checks.append([g,str(p),'match' if seen==expected else 'different',len(rr),sum((expected-seen).values()),sum((seen-expected).values())])
tsv('spec-inventory-checks.tsv',['group','inventory_path','identity_status','method_rows','missing_expected','extra_rows'],spec_checks)
j['class_spec_expectations']['inventory_snapshot_checks']=spec_checks
# Source pin and hashes for reviewed primary documents, plus critical local audit inputs.
sourcepaths=[I/'context/lead-review.md',I/'context/ENGINE.md',I/'claude-handoff/scripts/arc_layer.py',I/'claude-handoff/results/arc-layer-functions.tsv',I/'metadata/decomp/arccore-1.170.0-functions.tsv',abi/'all-methods-168.tsv']
sourcepaths += sorted((I/'context/analysis/data').glob('*.txt'))
sourcepaths += [I/'chromium-156'/r for r in ['content/public/app/aperitif_mac.h','content/app/aperitif_mac.cc','chrome/app/chrome_exe_main_mac.c','chrome/browser/mac/aperitif_unittest.mm']]
sourcepaths += list(Path('sources').glob('*'))
sourcerows=[]
for p in sourcepaths:
 url=''
 if 'chromium-156' in str(p):url='https://chromium.googlesource.com/chromium/src/+/d0c02bfad08ec92a6ffcf989e53cd995e6381a4c/'+str(p).split('chromium-156/')[1]
 if p.name.startswith('xnu-11417.121.6-'):url='https://github.com/apple-oss-distributions/xnu/blob/xnu-11417.121.6/EXTERNAL_HEADERS/mach-o/'+p.name.split('xnu-11417.121.6-')[1]
 sourcerows.append([str(p),p.stat().st_size,sha(p),url])
tsv('source-provenance.tsv',['path','bytes','sha256','pinned_source_url'],sourcerows)
# Compact gate matrix distinguishes consistency pass from unresolved behavioral requirements.
gates=[
('export_presence','PASS','199 manifest records in 3 manifest files; all SHA256 and byte sizes verified','manifest-verification.tsv','Integrity/presence only'),
('export_counts','PASS','567959 unique VA C headers; 79 failure placeholders; 567880 candidates; 607294 TSV rows','coverage.json','Candidates are not validated functions'),
('failure_taxonomy','PASS','48 timeout; 24 nonunique-varnode hash; 3 range; 3 input-varnode; 1 forced-merge errors','failures.tsv','Never describe all 79 as timed out'),
('address_join','PASS','All C header VAs present in TSV; no name/order pairing used for metadata','method-coverage.tsv','Retain image version and VA basis'),
('function_span_integrity','FAIL','579666 entries inside earlier enclosing spans; 59181 span/body disagreements','huge-bounds.tsv; coverage.json','No contiguous body map, byte coverage or rewrite-size claim'),
('only_one_bad_region','FAIL','10 spans >=1MiB; 39 bad-instruction bodies; 5 unimplemented-instruction bodies','huge-bounds.tsv; warning-bodies.tsv','Review every consequential body, including seeds/callees'),
('handoff_reproduction','PASS','5846 exact row reproductions including sizes/reasons/depth','closure-audit.tsv','Mechanical consistency only'),
('handoff_address_identity','FAIL','No VA field in 5846 supplied rows; 524 zero sizes; 3 normalized-name collisions','closure-audit.tsv','Use address + raw name + metadata method identity'),
('handoff_is_complete_Arc_layer','FAIL','Two-hop anonymous-call regex excludes thunks, named calls, indirect callbacks and dispatch','methods-outside-closure.tsv','Never call this all proprietary code or rewrite scope'),
('class_inventory','PASS','146 Arc metadata/exported classes; 144 current Arc-named seed namespaces; 142 not reproduced','inventory.json','Keep metadata, named-function and export sets distinct'),
('all_class_spec_inventory','PASS','146 classes assigned exactly once; each group has 491 declarations','spec-expectations.tsv','Presence/availability is not completed behavioral spec'),
('imported_method_surface','PASS','834 declarations on 63 imports; 830 class/kind/selector keys','inventory.json','Do not count as 834 proven calls'),
('direct_app_usage','PROVISIONAL','ABI evidence includes branch-supported caller/selector records and separately reviewed receiver cases','/home/exedev/work/agents/abi/initial/used-calls.tsv','Quality audit did not repeat whole app branch scan; selector alone does not identify receiver'),
('older_encoding_equivalence','PROVISIONAL','6 older Arc encodings lack unique selector/type association','old-encoding-unknowns.tsv','No order-based pairing; no complete old-version ABI equivalence'),
('category_resolution','PROVISIONAL','Base/category identities share selectors with distinct IMPs; additionalData needs separate assessment','method-coverage.tsv','Never inherit mapping just by class/selector'),
('thunk_completion','PROVISIONAL','createPaneHostView missing C header has B target; _CallWithEHFrame also a B thunk','branch-probes.tsv','Follow target and validate contract, not prototype guessing'),
('body_semantics','PROVISIONAL','Successful candidates include unresolved jumps, injections, type propagation and unsupported instructions','warning-bodies.tsv','Pseudocode is not semantics, ownership or callback ABI proof'),
('aperitif_static_role','PASS','Both slices export AperitifCheckInitialized and import sandbox APIs; source describes early sandboxing','aperitif.json; aperitif-strings.tsv','Consistent with upstream role, not binary equivalence'),
('aperitif_exact_implementation','PROVISIONAL','Not included in engine Ghidra export; static metadata and strings only','aperitif.json','Additional bounded decompile/disassembly needed for exact behavior'),
('helper_executables','NOT_ANALYZED','Helper executables absent from copied binary inputs','/home/exedev/work/inputs/binaries','Launch/entitlement/load-order/helper contract remains a scope gap'),
('upstream_change_scope','PROVISIONAL','LTO/config/ICF and missing logging paths defeat difference-by-absence reasoning','/home/exedev/work/inputs/context/lead-review.md','Unmatched functions or missing paths do not prove proprietary code or unchanged Chromium'),
('CEF_native_hosting','PROVISIONAL','Pinned CEF/Chromium sources establish API context only','/home/exedev/work/inputs/context/lead-review.md','Validate parent_view Alloy restrictions, pane hosting and Chrome services'),
('replacement_runtime_compatibility','NOT_ESTABLISHED','No replacement built or executed in this research','report.md','No behavioral assurance or maintenance effort estimate'),
]
tsv('quality-gates.tsv',['gate','status','finding','evidence','synthesis_requirement'],gates)
(O/'coverage.json').write_text(json.dumps(j,indent=2,sort_keys=True)+'\n')
print(json.dumps({'closure_structure':summary,'metadata_methods':j['metadata_methods'],'old_unknowns':len(unknown),'class_specs':j['class_spec_expectations'],'binary_exports':len(actual),'export_trie_match':actual==expected,'warning_function_counts':warnsummary,'branch_probes':prows},indent=2))
