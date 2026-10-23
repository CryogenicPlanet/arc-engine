#!/usr/bin/env python3
"""Independent export-header/byte verification and targeted contract assertions."""
import sys,csv,json,re,hashlib,sqlite3,struct
from pathlib import Path
OUT=Path(__file__).resolve().parents[1];ROOT=Path('/home/exedev/work/inputs')
sys.path.insert(0,str(OUT.parent/'scripts'))
from inventory import MachO,write_tsv

def table(n):return list(csv.DictReader((OUT/n).open(),delimiter='\t'))
selected=table('selected-bodies.tsv');wanted={int(r['va'],16) for r in selected};found={};headers=0
pat=re.compile(rb'^// ==== (0x[0-9a-f]+)  (.+)  \((\d+) bytes\)')
for r in table('corpus-inventory.tsv'):
 with Path(r['path']).open('rb') as f:
  for ln,line in enumerate(f,1):
   if not line.startswith(b'// ==== '):continue
   m=pat.match(line);assert m,(r['path'],ln)
   headers+=1;va=int(m[1],16)
   if va in wanted:
    assert va not in found
    found[va]=(r['path'],ln,m[2].decode())
for r in selected:
 va=int(r['va'],16)
 if r['status']=='no_exact_exported_header':assert va not in found
 else:
  assert found[va][0]==r['path'] and found[va][2]==r['name']
  with Path(r['path']).open('rb') as f:f.seek(int(r['offset']));data=f.read(int(r['length']))
  assert data==Path(r['local']).read_bytes() and hashlib.sha256(data).hexdigest()==r['sha256']
 assert r['status'] not in ['truncated','export_failure']
with (OUT/'selected-source-lines.tsv').open('w') as f:
 w=csv.writer(f,delimiter='\t');w.writerow(['va','path','header_line','name'])
 for va,(p,ln,name) in sorted(found.items()):w.writerow([hex(va),p,ln,name])
assert headers==567959
b=MachO(ROOT/'binaries/ArcCore-1.170.0.arm64')
ins=table('binary-instructions.tsv')
for r in ins:assert b.read(int(r['instruction_va'],16),4).hex()==r['bytes'] and hex(b.offset(int(r['instruction_va'],16)))==r['file_offset']
branches={r['instruction_va']:r for r in table('binary-branches.tsv')}
for pc,sym in [('0x43e6f1c','_objc_retainBlock'),('0x4042068','_objc_storeWeak'),('0x43eda20','_objc_storeWeak'),('0x404278c','_objc_loadWeakRetained'),('0x404281c','_objc_msgSend'),('0x4044ea0','_objc_retainBlock'),('0x4044ed8','_objc_release')]:assert branches[pc]['import_symbol']==sym
for pc,target in [('0x441d770','0x2e0238'),('0x4440fb0','0x4375b40'),('0x4415c18','0x4402548')]:assert branches[pc]['branch_target']==target
assert b.read(0x2e0240,8).hex()=='080840f900013fd6'
assert b.read(0x4415b74,4).hex()=='f40303aa' # NSError** saved from x3.
assert b.read(0x4445ac0,4).hex()=='1f3000f1' # cmp x0,#12.
assert b.read(0x40427b0,4).hex()=='0803631e' # ucvtf d8,w24.
assert b.read(0x4044e44,4).hex()=='020840f9' # block invoke pointer at +16.
assert b.read(0x83af740,4).hex()=='a0008052' # mov w0,#5.
assert json.loads((OUT/'error-domain.json').read_text())['value']=='company.thebrowser.Arc'
av=table('initial-requests-resolved.tsv');assert sum(x['initial_table']=='contract.tsv' and x['status']=='available_exact_header' for x in av)==834
assert len(table('additional-shards.tsv'))==6
assert {r['engine_va'] for r in table('additional-shards.tsv')}=={r['va'] for r in selected if r['status']=='no_exact_exported_header'}
closure=table('closure-audit.tsv');assert len({r['name'] for r in closure})==5846
assert sum(r['flags'].count('known_merged_exclude') for r in closure)==1
assert len(table('malformed-region-candidates.tsv'))==10
# Hash all direct evidence files consumed beyond already manifest-verified engine shards.
paths={ROOT/'corpus-index.sqlite',ROOT/'INDEX-READY',ROOT/'CHROMIUM-READY',ROOT/'context/ENGINE.md',ROOT/'context/lead-review.md',ROOT/'claude-handoff/scripts/arc_layer.py',ROOT/'claude-handoff/results/arc-layer-functions.tsv',ROOT/'metadata/decomp/arccore-1.170.0-functions.tsv',ROOT/'binaries/ArcCore-1.170.0.arm64',ROOT/'binaries/Arc-1.168.0.arm64'}
paths|={Path(r['path']) for r in table('app-selected.tsv')}
paths|={Path(r['path']) for r in table('primary-sources.tsv')}
paths|={OUT.parent/'initial'/n for n in ['report.md','contract.tsv','all-methods-170.tsv','decompile-requests.tsv','missing-engine-bodies.tsv','used-calls.tsv','app-callbacks.tsv','validation.json']}
rows=[]
for p in sorted(paths):
 h=hashlib.sha256()
 with p.open('rb') as f:
  for chunk in iter(lambda:f.read(1<<20),b''):h.update(chunk)
 rows.append(dict(path=str(p),sha256=h.hexdigest(),bytes=p.stat().st_size))
write_tsv(OUT/'input-sha256.tsv',rows)
result=dict(status='PASS',engine_headers_independently_streamed=headers,selected_functions_with_complete_text=len(found),missing_exact_headers=len(wanted)-len(found),binary_instructions_byte_checked=len(ins),contract_method_declarations_with_export_header=834,closure_unique_names=5846,large_metadata_ranges_rejected=10,curated_contract_findings=len(table('contracts.tsv')),limits='Static assertions validate provenance and selected instructions, not runtime ABI equivalence or all function boundaries.')
(OUT/'validation.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
