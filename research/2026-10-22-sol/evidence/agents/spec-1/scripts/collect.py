import csv,json,sqlite3,struct,re,collections
from pathlib import Path
from metadata import MachO,binary_objc,dyld_inventory,tsv,digest
root=Path('/home/exedev/work/inputs');out=Path('specs');classes=json.load(open(root/'context/class-assignments.json'))['groups']['spec-1']['classes'];want=set(classes)
m=MachO(root/'binaries/ArcCore-1.170.0.arm64');bc,bcat,bp,bm=binary_objc(m)
rows=[r for r in bm if r['class_name'] in want]
dc,dcat,dp,dm=dyld_inventory(root/'context/analysis/data/dobjc170.txt');dm=[r for r in dm if r['class_name'] in want]
key=lambda r:tuple(r.get(k,'') for k in ('class_name','category','kind','selector','imp'))
assert collections.Counter(map(key,rows))==collections.Counter(map(key,dm));assert len(rows)==491
old=list(csv.DictReader(open('../arc-layer/initial/objc-methods.tsv'),delimiter='\t'));assert collections.Counter(map(key,rows))==collections.Counter(map(key,[r for r in old if r['class_name'] in want]))
lines={key(r):r['dump_line'] for r in dm}
db=sqlite3.connect(f'file:{root}/corpus-index.sqlite?mode=ro',uri=True)
state=[]
for c in bc:
 if c['class_name'] not in want:continue
 va=int(c['metadata_va'],16);ro=m.ptr(va+32)&~7
 s=dict(c,ro_va=hex(ro),instance_start=struct.unpack('<I',m.vread(ro+4,4))[0],instance_size=struct.unpack('<I',m.vread(ro+8,4))[0],ivars=[],properties=[])
 iv=m.ptr(ro+48)
 if iv:
  es,n=struct.unpack('<II',m.vread(iv,8))
  for i in range(n):
   a=iv+8+es*i;op,np,tp,al,sz=struct.unpack('<QQQII',m.vread(a,32));op=m.ptr(a);np=m.ptr(a+8);tp=m.ptr(a+16)
   s['ivars'].append(dict(record_va=hex(a),offset=struct.unpack('<I',m.vread(op,4))[0],name=m.string(np),type=m.string(tp),size=sz,alignment=al))
 prop=m.ptr(ro+64)
 if prop:
  es,n=struct.unpack('<II',m.vread(prop,8))
  for i in range(n):
   a=prop+8+es*i;s['properties'].append(dict(record_va=hex(a),name=m.string(m.ptr(a)),attributes=m.string(m.ptr(a+8))))
 state.append(s)
for r in rows:
 r['dyld_line']=lines[key(r)];r['imp_file_offset']=hex(m.offset(int(r['imp'],16)))
 q=db.execute('select name,path,offset,length,failed from functions where va=?',(int(r['imp'],16),)).fetchone()
 r['body_status']='missing' if q is None else 'failed' if q[4] else 'available'
 if q:
  name,path,offset,length,failed=q;r.update(body_name=name,body_path=path,body_offset=offset,body_length=length)
  with open(path,'rb') as f:f.seek(offset);body=f.read(min(length,65536)).decode('utf8','replace')
  if length>65536:r['body_status']='oversized_unreviewed'
  (out/'bodies'/f'{r["imp"]}.c').write_text(body)
 else:r.update(body_name='',body_path='',body_offset='',body_length=0)
json.dump(state,open(out/'evidence/layout.json','w'),indent=2)
json.dump(rows,open(out/'evidence/methods.json','w'),indent=2)
tsv(out/'evidence/metadata.tsv',list(rows[0]),rows)
json.dump(dict(binary_sha256=digest(root/'binaries/ArcCore-1.170.0.arm64'),class_count=len(state),method_count=len(rows),dyld_tuples_verified=len(dm),initial_inventory_tuples_verified=len(rows),segments=m.segments),open(out/'evidence/reconciliation.json','w'),indent=2)
for c in classes:
 with open(out/'evidence'/f'{c}.txt','w') as f:
  f.write(json.dumps(next(s for s in state if s['class_name']==c),indent=2)+'\n')
  for r in rows:
   if r['class_name']!=c:continue
   f.write('\n### '+r['category']+' '+r['kind']+r['selector']+' '+r['imp']+' '+r['type_encoding']+' '+r['body_status']+'\n')
   p=out/'bodies'/f'{r["imp"]}.c'
   if p.exists():
    body=p.read_text();start=body.find('\n{');body=body[start+2:] if start>=0 else body
    body=re.sub(r'\n  (?:undefined\w*|long|ulong|uint|int|char|bool|float|double|code|void|id|NSObject|NS\w+|Arc\w+) [^;\n{}]+;','',body)
    f.write(body+'\n')
print('reconciled',len(state),len(rows),collections.Counter(r['body_status'] for r in rows),'unique IMP',len(set(r['imp'] for r in rows)))
