#!/usr/bin/env python3
import sys,json,csv,sqlite3,re,struct,collections
from pathlib import Path
sys.path.insert(0,str(Path(__file__).parent))
from metadata_reader import MachO,binary_objc,dyld_inventory,tsv,digest
ROOT=Path('/home/exedev/work'); OUT=Path(__file__).resolve().parents[1]
inputs=ROOT/'inputs'; assign=json.load(open(inputs/'context/class-assignments.json'))['groups']['spec-4']; names=set(assign['classes'])
m=MachO(inputs/'binaries/ArcCore-1.170.0.arm64'); classes,cats,protos,allrows=binary_objc(m)
rows=[r for r in allrows if r['class_name'] in names and not r.get('protocol')]
_,_,_,dy=dyld_inventory(inputs/'context/analysis/data/dobjc170.txt'); dy=[r for r in dy if r['class_name'] in names]
initial=[r for r in csv.DictReader(open(ROOT/'agents/arc-layer/initial/objc-methods.tsv'),delimiter='\t') if r['class_name'] in names and not r['protocol']]
key=lambda r:tuple(r[k] for k in ['class_name','category','kind','selector','imp'])
assert collections.Counter(map(key,rows))==collections.Counter(map(key,dy))
assert collections.Counter(tuple(r[k] for k in ['class_name','category','kind','selector','type_encoding','imp']) for r in rows)==collections.Counter(tuple(r[k] for k in ['class_name','category','kind','selector','type_encoding','imp']) for r in initial)
assert len(rows)==assign['method_rows'] and len(set(r['class_name'] for r in rows))==len(names)
dylines={key(r):r['dump_line'] for r in dy}
conn=sqlite3.connect(f'file:{inputs}/corpus-index.sqlite?mode=ro',uri=True)
bs={};layout={}
for c in classes:
 if c['class_name'] not in names:continue
 va=int(c['metadata_va'],16);ro=m.ptr(va+32)&~7
 start,size=struct.unpack('<II',m.vread(ro+4,8)); ivars=[];il=m.ptr(ro+48)
 if il:
  stride,n=struct.unpack('<II',m.vread(il,8))
  for i in range(n):
   p=il+8+i*stride; offsetptr=m.ptr(p); name=m.string(m.ptr(p+8));typ=m.string(m.ptr(p+16));align,isz=struct.unpack('<II',m.vread(p+24,8));off=struct.unpack('<I',m.vread(offsetptr,4))[0]
   ivars.append(dict(name=name,type=typ,offset=off,size=isz,record_va=hex(p)))
 layout[c['class_name']]={**c,'ro_va':hex(ro),'instance_start':start,'instance_size':size,'ivars':ivars}
for r in rows:
 va=int(r['imp'],16); r['dyld_line']=dylines[key(r)];r['imp_file_offset']=hex(m.offset(va));res=conn.execute('select name,path,offset,length,failed from functions where va=?',(va,)).fetchone()
 if res:
  name,path,off,length,failed=res
  with open(path,'rb') as f:f.seek(off);raw=f.read(min(length,32768))
  text=raw.decode('utf-8','replace');(OUT/'bodies'/f'{va:x}.c').write_text(text)
  status='failed' if failed else ('truncated' if length>32768 else 'available')
  bs[r['imp']]=dict(va=r['imp'],name=name,path=path,offset=off,length=length,body_status=status,calls=sorted(set(re.findall(r'\bFUN_([0-9a-fA-F]+)\s*\(',text))))
 else:status='missing';bs[r['imp']]=dict(va=r['imp'],body_status=status)
 r['body_status']=status
fields=['class_name','category','kind','selector','type_encoding','imp','imp_file_offset','method_record_va','selector_va','dyld_line','body_status']
tsv(OUT/'evidence/declared-methods.tsv',fields,rows)
json.dump(layout,open(OUT/'evidence/layout.json','w'),indent=2);json.dump(bs,open(OUT/'evidence/body-index.json','w'),indent=2)
json.dump({'assigned_classes':len(names),'method_rows':len(rows),'unique_imps':len(bs),'by_body_status':dict(collections.Counter(r['body_status'] for r in rows)),'binary_equals_initial':True,'binary_equals_raw_dyld':True,'categories':[c for c in cats if c['class_name'] in names],'inputs_sha256':{str(p):digest(p) for p in [inputs/'binaries/ArcCore-1.170.0.arm64',inputs/'context/analysis/data/dobjc170.txt',inputs/'context/analysis/data/objc170.txt']}},open(OUT/'evidence/inventory-validation.json','w'),indent=2)
print(json.dumps({'classes':len(names),'methods':len(rows),'imps':len(bs),'status':dict(collections.Counter(r['body_status'] for r in rows))},indent=2))
