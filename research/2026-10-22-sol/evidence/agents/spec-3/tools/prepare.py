#!/usr/bin/env python3
"""Re-decode read-only Mach-O metadata; verify identity against dyld and prior inventory.
Extract exact indexed IMPs with the supplied bounded reader. No binary modification.
"""
import sys
sys.dont_write_bytecode=True
import csv,json,importlib.util,struct,subprocess,sqlite3,collections,re
from pathlib import Path
ROOT=Path('/home/exedev/work/inputs'); OUT=Path('specs')
spec=importlib.util.spec_from_file_location('inventory','/home/exedev/work/agents/arc-layer/scripts/inventory.py')
m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
assigned=json.loads((ROOT/'context/class-assignments.json').read_text())['groups']['spec-3']
classes=set(assigned['classes']);mach=m.MachO(ROOT/'binaries/ArcCore-1.170.0.arm64')
cs,cats,ps,ms=m.binary_objc(mach);ms=[x for x in ms if x['class_name'] in classes]
identity=lambda x:(x['class_name'],x['category'],x['kind'],x['selector'])
prior=list(csv.DictReader(open('../arc-layer/initial/objc-methods.tsv'),delimiter='\t'));prior={identity(x):x for x in prior if x['class_name'] in classes}
dyld=m.dyld_inventory(ROOT/'context/analysis/data/dobjc170.txt')[3];dyld={identity(x):x for x in dyld if x['class_name'] in classes}
assert len(ms)==assigned['method_rows']==491 and len(classes)==37
assert set(map(identity,ms))==set(prior)==set(dyld)
for x in ms:
 assert x['imp']==prior[identity(x)]['imp']==dyld[identity(x)]['imp']
 assert x['type_encoding']==prior[identity(x)]['type_encoding']
 x['dyld_line']=dyld[identity(x)]['dump_line']
 x['imp_file_offset']=hex(mach.offset(int(x['imp'],16)))
layouts={}
for c in cs:
 if c['class_name'] not in classes:continue
 va=int(c['metadata_va'],16);ro=mach.ptr(va+32)&~7
 layout={**c,'ro_va':hex(ro),'instance_start':struct.unpack('<I',mach.vread(ro+4,4))[0],'instance_size':struct.unpack('<I',mach.vread(ro+8,4))[0],'ivars':[],'properties':[]}
 iv=mach.ptr(ro+48)
 if iv:
  stride,n=struct.unpack('<II',mach.vread(iv,8))
  for i in range(n):
   a=iv+8+i*stride;off=mach.ptr(a)
   layout['ivars'].append(dict(record_va=hex(a),offset=struct.unpack('<I',mach.vread(off,4))[0],name=mach.string(mach.ptr(a+8)),type=mach.string(mach.ptr(a+16)),size=struct.unpack('<I',mach.vread(a+28,4))[0]))
 pr=mach.ptr(ro+64)
 if pr:
  stride,n=struct.unpack('<II',mach.vread(pr,8))
  for i in range(n):
   a=pr+8+i*stride
   layout['properties'].append(dict(record_va=hex(a),name=mach.string(mach.ptr(a)),attributes=mach.string(mach.ptr(a+8))))
 layouts[c['class_name']]=layout
(OUT/'evidence/metadata.json').write_text(json.dumps({'classes':layouts,'categories':[x for x in cats if x['class_name'] in classes],'methods':ms},indent=2))
db=sqlite3.connect(f'file:{ROOT}/corpus-index.sqlite?mode=ro',uri=True)
bs={}
for va in sorted({int(x['imp'],16) for x in ms}):
 row=db.execute('select name,path,offset,length,failed from functions where va=?',(va,)).fetchone()
 file=OUT/'bodies'/f'{va:08x}.txt'
 if not file.exists():
  with file.open('w') as f:subprocess.run(['python3','/home/exedev/work/read-function.py',hex(va)],stdout=f,check=True)
 bs[hex(va)]={'index':row,'snapshot':str(file),'status':('missing' if not row else 'failed' if row[4] else 'oversized_unresolved' if row[3]>32768 else 'available_candidate')}
(OUT/'evidence/body-index.json').write_text(json.dumps(bs,indent=2))
summary={'assigned_classes':len(classes),'declared_method_rows':len(ms),'distinct_imps':len(bs),'class_methods':sum(x['kind']=='+' for x in ms),'category_rows':sum(bool(x['category']) for x in ms),'identity_reconciliation':'491/491 equal to independently re-decoded Mach-O and dyld class/category/kind/selector/IMP; types equal initial binary inventory','body_status_by_method':dict(collections.Counter(bs[x['imp']]['status'] for x in ms)),'shared_imps':{va:[identity(x) for x in ms if x['imp']==va] for va in bs if sum(x['imp']==va for x in ms)>1}}
(OUT/'evidence/reconciliation.json').write_text(json.dumps(summary,indent=2));print(json.dumps(summary,indent=2))
