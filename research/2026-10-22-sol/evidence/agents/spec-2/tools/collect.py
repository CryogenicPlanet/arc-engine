#!/usr/bin/env python3
import sys,csv,json,sqlite3,struct,collections,re,hashlib
from pathlib import Path
sys.dont_write_bytecode=True
from macho_metadata import MachO,binary_objc,dyld_inventory
ROOT=Path('/home/exedev/work/inputs'); OUT=Path('specs/evidence')
assignment=json.load(open(ROOT/'context/class-assignments.json'))['groups']['spec-2']
classes=assignment['classes']; mach=MachO(ROOT/'binaries/ArcCore-1.170.0.arm64')
bc,cat,pro,methods=binary_objc(mach)
methods=[m for m in methods if m['class_name'] in classes]
initial=[m for m in csv.DictReader(open('../arc-layer/initial/objc-methods.tsv'),delimiter='\t') if m['class_name'] in classes]
dc,da,dp,dm=dyld_inventory(ROOT/'context/analysis/data/dobjc170.txt');dm=[m for m in dm if m['class_name'] in classes]
key=lambda x:tuple(x.get(k,'') for k in ('class_name','category','kind','selector','imp'))
assert collections.Counter(map(key,methods))==collections.Counter(map(key,dm))
assert collections.Counter((key(x),x['type_encoding'],x['method_record_va']) for x in methods)==collections.Counter((key(x),x['type_encoding'],x['method_record_va']) for x in initial)
assert len(methods)==491 and len({m['class_name'] for m in methods})==37
con=sqlite3.connect('file:'+str(ROOT/'corpus-index.sqlite')+'?mode=ro',uri=True)
for m in methods:
 row=con.execute('select name,path,offset,length,failed from functions where va=?',(int(m['imp'],16),)).fetchone()
 m['dyld_line']=next(x['dump_line'] for x in dm if key(x)==key(m));m['imp_file_offset']=hex(mach.offset(int(m['imp'],16)))
 if row:
  name,path,off,length,failed=row;m.update(body_name=name,body_path=path,body_offset=off,body_length=length,failed=failed)
  with open(path,'rb') as f:f.seek(off);b=f.read(min(length,100000))
  m['body']=b.decode('utf-8','replace');m['truncated']=length>100000
 else:m['body']='';m['failed']=None
layouts=[]
for c in bc:
 if c['class_name'] not in classes:continue
 va=int(c['metadata_va'],16);ro=mach.ptr(va+32)&~7
 c.update(ro_va=hex(ro),instance_start=struct.unpack('<I',mach.vread(ro+4,4))[0],instance_size=struct.unpack('<I',mach.vread(ro+8,4))[0])
 iv=mach.ptr(ro+48);ivars=[]
 if iv:
  stride,n=struct.unpack('<II',mach.vread(iv,8))
  for i in range(n):
   a=iv+8+i*stride;op=mach.ptr(a)
   ivars.append(dict(record_va=hex(a),offset=struct.unpack('<I',mach.vread(op,4))[0],name=mach.string(mach.ptr(a+8)),type=mach.string(mach.ptr(a+16)),alignment=struct.unpack('<I',mach.vread(a+24,4))[0],size=struct.unpack('<I',mach.vread(a+28,4))[0]))
 c['ivars']=ivars;layouts.append(c)
OUT.mkdir(parents=True,exist_ok=True)
json.dump(methods,open(OUT/'methods-bodies.json','w'),indent=2)
json.dump(layouts,open(OUT/'layouts.json','w'),indent=2)
for c in classes:
 ms=[m for m in methods if m['class_name']==c]
 with open(OUT/(c+'.txt'),'w') as f:
  f.write(json.dumps(next(x for x in layouts if x['class_name']==c),indent=2)+'\n')
  for m in ms:f.write('\nIDENTITY '+json.dumps({k:v for k,v in m.items() if k!='body'})+'\n'+m['body']+'\n')
validation=dict(classes=len(classes),methods=len(methods),unique_imps=len({m['imp'] for m in methods}),binary_vs_initial='exact identity/type/record match',binary_vs_dyld='exact class/category/kind/selector/IMP match',categories=[x for x in cat if x['class_name'] in classes],kind_counts=dict(collections.Counter(m['kind'] for m in methods)),body_index_counts=dict(collections.Counter('missing' if m['failed'] is None else 'failed' if m['failed'] else 'available' for m in methods)))
json.dump(validation,open(OUT/'inventory-validation.json','w'),indent=2);print(json.dumps(validation,indent=2))
