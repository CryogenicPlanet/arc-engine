#!/usr/bin/env python3
"""Independent identity, byte-range and artifact validation; no product tests."""
import csv,json,sys,re,sqlite3,hashlib,struct,collections
from pathlib import Path
sys.path.insert(0,str(Path(__file__).parent));from metadata_reader import MachO,tsv,digest
O=Path(__file__).resolve().parents[1];I=Path('/home/exedev/work/inputs');R=O.parent
assign=json.load(open(I/'context/class-assignments.json'))['groups']['spec-4'];rows=list(csv.DictReader(open(O/'methods.tsv'),delimiter='\t'));decl=list(csv.DictReader(open(O/'evidence/declared-methods.tsv'),delimiter='\t'))
key=lambda r:tuple(r[x] for x in ['class_name','category','kind','selector','type_encoding'])
assert collections.Counter(map(key,rows))==collections.Counter(map(key,decl));assert len(rows)==491
assert set(x.stem for x in (O/'classes').glob('*.md'))==set(assign['classes']);assert len(assign['classes'])==38
for r in rows:
 assert all(r[x] for x in ['class_name','kind','selector','type_encoding','imp_va','body_status','behavior','evidence','confidence','unresolved_question'])
 assert r['imp_va'] in r['evidence'];assert r['kind'] in '+-'
 doc=(O/'classes'/f"{r['class_name']}.md").read_text();assert f"`{r['kind']}{r['selector']}`" in doc;assert r['imp_va'] in doc
# Compare saved bodies against the immutable exact indexed ranges.
db=sqlite3.connect(f'file:{I}/corpus-index.sqlite?mode=ro',uri=True);indexes={**json.load(open(O/'evidence/body-index.json')),**json.load(open(O/'evidence/bridges.json'))};body_checks=[]
for va,info in indexes.items():
 res=db.execute('select name,path,offset,length,failed from functions where va=?',(int(va,16),)).fetchone();assert res
 name,path,offset,length,failed=res;assert not failed and length<=32768
 with open(path,'rb') as f:f.seek(offset);data=f.read(length)
 saved=(O/'bodies'/f'{int(va,16):x}.c').read_bytes();assert saved==data
 body_checks.append(dict(imp_va=va,path=path,offset=offset,length=length,sha256=hashlib.sha256(data).hexdigest()))
tsv(O/'evidence/body-range-validation.tsv',['imp_va','path','offset','length','sha256'],body_checks)
# Stream saved function metadata for assigned exact IMPs only. Never interpret bounds as contiguous byte maps.
imps=set(r['imp_va'] for r in rows);saved=[]
with open(I/'metadata/decomp/arccore-1.170.0-functions.tsv') as f:
 for line in f:
  p=line.rstrip('\n').split('\t')
  if p[0] in imps:
   body=(O/'bodies'/f'{int(p[0],16):x}.c').read_text();hdr=re.search(r'\((\d+) bytes\)',body)
   saved.append(dict(entry_va=p[0],saved_end_va=p[1],reported_body_bytes_not_span=p[2],qualified_name=p[3],export_header_reported_bytes=hdr[1] if hdr else '',sizes_disagree=bool(hdr and int(hdr[1])!=int(p[2]))))
assert len(saved)==len(imps)
tsv(O/'evidence/imp-saved-function-metadata.tsv',list(saved[0]),saved)
# Revalidate the one existing APP selector-stub call using APP segment mapping and raw BL word.
a=MachO(I/'binaries/Arc-1.168.0.arm64');call=0x1004ad718;off=a.offset(call);word=struct.unpack('<I',a.vread(call,4))[0];assert word>>26==0b100101
imm=word&0x3ffffff;imm=imm-(1<<26) if imm&(1<<25) else imm;target=call+4*imm;assert target==0x1035beda0
json.dump({'image':'Arc 1.168.0 arm64 app','call_va':hex(call),'call_file_offset':hex(off),'bl_word':hex(word),'stub_va':hex(target),'selector':'init:argv:profilePath:applicationID:isProduction:sessionRestorationPolicy:shouldLoadAllProfilesAtStartup:quitRunLoopOnTermination:userDefaultsSuiteName:','basis':'Existing initial/app168-launch-calls.tsv selector-stub association; BL and image mapping independently rechecked. Receiver class association remains old app pseudocode inference, not runtime proof.'},open(O/'evidence/direct-app-call.json','w'),indent=2)
# Snapshot metadata validation and source hashes used by this specification.
paths=['chrome/browser/profiles/profile.h','chrome/browser/ui/side_panel/side_panel_registry.h','components/history/core/browser/history_service.h','components/prefs/pref_service.h','components/prefs/pref_change_registrar.h','chrome/common/pref_names.h','components/omnibox/browser/omnibox_pref_names.h','components/password_manager/core/common/password_manager_pref_names.h','components/autofill/core/common/autofill_prefs.h','components/search_engines/template_url_service.h','components/sessions/core/session_types.h','components/performance_manager/public/user_tuning/prefs.h','chrome/browser/performance_manager/user_tuning/battery_saver_mode_manager.cc','content/public/browser/web_ui_message_handler.h','components/url_formatter/url_formatter.h','ui/base/models/menu_model.h','third_party/blink/public/common/page/page_zoom.h','DEPS']
sources=[dict(commit='d0c02bfad08ec92a6ffcf989e53cd995e6381a4c',source_path=p,local_path=str(I/'chromium-156'/p),sha256=digest(I/'chromium-156'/p)) for p in paths]
for p,rev in [('net/base/registry_controlled_domains/registry_controlled_domain.h','d0c02bfad08ec92a6ffcf989e53cd995e6381a4c'),('boringssl/curve25519.h','2ea3200c3a3ec176613ab7a0b9a4ffc6db846120')]:sources.append(dict(commit=rev,source_path=p,local_path=str(O/'sources'/p),sha256=digest(O/'sources'/p)))
tsv(O/'evidence/primary-sources.tsv',['commit','source_path','local_path','sha256'],sources)
# Ensure report-cited primary file/line references exist; semantic source identification stays human-audited.
notes=json.load(open(O/'evidence/class-notes.json'))
for n in notes.values():
 for text in n.values():
  for path,line in re.findall(r'`([^`]+):([0-9]+)(?:-[0-9]+)?`',text):
   if path.startswith('specs/sources/'):p=O/path.removeprefix('specs/')
   else:
    p=I/'chromium-156'/path
    if not p.is_file():p=O/'sources'/path
   assert p.is_file(),p;assert 0<int(line)<=sum(1 for _ in open(p)),(p,line)
cov=json.load(open(O/'coverage.json'));assert cov['actual_method_rows']==491 and cov['actual_class_specs']==38
result={'class_specs':38,'method_identities':491,'unique_imps':len(imps),'exact_indexed_bodies_validated':len(body_checks),'failed_or_truncated_saved_bodies':0,'raw_otool_type_imp_matches':json.load(open(O/'evidence/otool-validation.json'))['matching_imp_type_rows'],'saved_function_rows':len(saved),'saved_vs_export_size_disagreement_imps':sum(x['sizes_disagree'] for x in saved),'direct_app_bl_verified':True,'source_files_hashed':len(sources),'matrix_membership_complete':True,'limitations':'Validation proves metadata identity, evidence provenance and artifact integrity, not behavioral or ABI equivalence at runtime.'}
json.dump(result,open(O/'evidence/validation.json','w'),indent=2);print(json.dumps(result,indent=2))
