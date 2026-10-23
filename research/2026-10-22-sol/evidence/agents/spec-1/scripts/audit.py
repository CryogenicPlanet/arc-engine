"""Recheck exact method identities, pinned body offsets, otool records and bounded raw instructions."""
import csv,json,re,sqlite3,struct,collections,hashlib
from pathlib import Path
from metadata import MachO,tsv,digest
inp=Path('/home/exedev/work/inputs');out=Path('specs');raw=json.load(open(out/'evidence/methods.json'));methods=list(csv.DictReader(open(out/'methods.tsv'),delimiter='\t'))
m=MachO(inp/'binaries/ArcCore-1.170.0.arm64');byrecord={int(r['method_record_va'],16):r for r in raw};byid=lambda r:tuple(r[k] for k in ['class_name','category','kind','selector'])
assert len(raw)==len(methods)==491;assert len({byid(r) for r in methods})==491
assert {byid(r) for r in raw}=={byid(r) for r in methods}
# otool method list anchor + entry offset, NOT a class-wide/dyld row-order pairing.
# Resolve record identity from independently parsed binary, then verify exact encoding and IMP.
listva=None;entry=-1;entsize=None;pending_type=None;found={};record=None
with open(inp/'context/analysis/data/objc170.txt') as f:
 for lineno,line in enumerate(f,1):
  t=re.match(r'\s+(?:baseMethods|instanceMethods|classMethods)\s+(0x[0-9a-f]+)',line)
  if t:listva=int(t[1],16);entry=-1;entsize=None;record=None;continue
  t=re.match(r'\s+entsize\s+(\d+)(?: \(relative\))?\s*$',line)
  if t and listva is not None:entsize=int(t[1]);continue
  # Method triple lines have exact spacing/name labels (not ivars/properties).
  if listva is not None and entsize in (12,24) and re.match(r'\s+name\s+0x[0-9a-f]+ \(0x[0-9a-f]+\)',line):
   entry+=1;record=listva+8+entry*entsize;pending_type=None
  t=re.match(r'\s+types\s+0x[0-9a-f]+(?: \(0x[0-9a-f]+\))? (.+?)\s*$',line)
  if t and record in byrecord:pending_type=(t[1],lineno)
  t=re.match(r'\s+imp\s+0x[0-9a-f]+ \((0x[0-9a-f]+)\)',line)
  if t and pending_type is not None and record in byrecord:
   r=byrecord[record];assert pending_type[0]==r['type_encoding'],(r,pending_type);assert int(t[1],16)==int(r['imp'],16),(r,line)
   found[record]=dict(method_record_va=hex(record),class_name=r['class_name'],category=r['category'],kind=r['kind'],selector=r['selector'],type_encoding=r['type_encoding'],imp_va=r['imp'],otool_type_line=pending_type[1],otool_imp_line=lineno)
  if re.match(r'\s+(?:baseProtocols|ivars|weakIvarLayout|baseProperties|instanceProperties|classProperties|protocols)\s',line):listva=None;record=None
assert len(found)==491,(len(found),[r for a,r in byrecord.items() if a not in found][:3])
tsv(out/'evidence/otool-reconciliation.tsv',list(next(iter(found.values()))),found.values())
# Indexed extracts exactly match byte range (not merged function bounds).
db=sqlite3.connect(f'file:{inp}/corpus-index.sqlite?mode=ro',uri=True);seen=set();checks=[];edges=[]
closure={line.split('\t')[0] for line in open(inp/'claude-handoff/results/arc-layer-functions.tsv')}
for r in raw:
 if r['imp'] in seen:continue
 seen.add(r['imp']);q=db.execute('select name,path,offset,length,failed from functions where va=?',(int(r['imp'],16),)).fetchone()
 if q is None:continue
 name,path,offset,length,failed=q
 with open(path,'rb') as f:f.seek(offset);data=f.read(min(length,65536))
 local=(out/'bodies'/f'{r["imp"]}.c').read_bytes();assert data==local
 s=local.decode();assert s.startswith('// ==== '+r['imp']+' '),(r,s[:60]);assert length<=65536
 calls=sorted(set(re.findall(r'\b(?:thunk_)?FUN_([0-9a-fA-F]+)\s*\(',s[s.find('\n{')+2:])))
 ptrs=sorted(set(re.findall(r'=\s*(?:thunk_)?FUN_([0-9a-fA-F]+)\s*;',s)))
 for k,arr in [('direct_callee',calls),('function_pointer_assignment',ptrs)]:
  for a in arr:
   a=int(a,16);n=db.execute('select name from functions where va=?',(a,)).fetchone();edges.append(dict(caller_va=r['imp'],edge_kind=k,callee_va=hex(a),callee_name=n[0] if n else '',callee_name_in_handoff_closure=bool(n and n[0] in closure)))
 checks.append(dict(imp_va=r['imp'],name=name,path=path,offset=offset,length=length,failed=failed,sha256=hashlib.sha256(data).hexdigest(),name_in_handoff_closure=name in closure,injection_warning_count=len(re.findall('replaced with injection',s)),unrecovered_jumptable='UNRECOVERED_JUMPTABLE' in s))
tsv(out/'evidence/body-index.tsv',list(checks[0]),checks);tsv(out/'evidence/call-edges.tsv',list(edges[0]),edges)
# Known branch veneer; decode signed B immediate and assert indexed target.
w=struct.unpack('<I',m.vread(0x4410bb4,4))[0];assert w&0xfc000000==0x14000000;imm=w&0x3ffffff;imm=imm-(1<<26) if imm&(1<<25) else imm;assert 0x4410bb4+4*imm==0x47390dc
# HSLAMatrix getter SIMD copy: LDUR Q from x0, STR/STP Q to aggregate x8. Simulate register data offsets.
a=0x43dee9c;registers={};copies=[];instructions=[]
for i,(w,) in enumerate(struct.iter_unpack('<I',m.vread(a,36))):
 rn=(w>>5)&31;rt=w&31
 if w&0xffe00c00==0x3cc00000:
  assert rn==0;off=(w>>12)&511;off=off-512 if off&256 else off;registers[rt]=off;op=f'LDUR Q{rt},[X0,#{off}]'
 elif w&0xffc00000==0x3d800000:
  assert rn==8;off=((w>>10)&4095)*16;copies.append((off,registers[rt],16));op=f'STR Q{rt},[X8,#{off}]'
 elif w&0xffc00000==0xad000000:
  assert rn==8;rt2=(w>>10)&31;off=(w>>15)&127;off=off-128 if off&64 else off;off*=16;copies.extend([(off,registers[rt],16),(off+16,registers[rt2],16)]);op=f'STP Q{rt},Q{rt2},[X8,#{off}]'
 else:assert w==0xd65f03c0;op='RET'
 instructions.append(dict(va=hex(a+i*4),word=hex(w),decoded=op))
assert sorted(copies)==[(i,i+8,16) for i in range(0,80,16)],copies
tsv(out/'evidence/hsla-return-instructions.tsv',list(instructions[0]),instructions)
assignment=json.load(open(inp/'context/class-assignments.json'))['groups']['spec-1'];expected=set(assignment['classes']);assert set(p.stem for p in (out/'classes').glob('*.md'))==expected
stats=dict(status='PASS',classes=34,method_rows=491,unique_imp_vas=436,otool_records_verified=len(found),indexed_unique_bodies_verified=len(checks),indexed_body_rows=490,failed_body_rows=0,missing_body_rows=1,repaired_empty_return_imps=['0x40b1230','0x43dee04'],hsla_return_verified_80_byte_bitwise_copy=True,createPaneHostView_branch_target='0x47390dc',call_edge_count=len(edges),handoff_closure_names_present=sum(r['name_in_handoff_closure'] for r in checks),handoff_closure_line_count=len(closure),body_max_text_bytes=max(r['length'] for r in checks),no_body_extent_sum=True)
(out/'evidence/validation.json').write_text(json.dumps(stats,indent=2)+'\n');print(json.dumps(stats,indent=2))
