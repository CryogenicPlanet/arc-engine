#!/usr/bin/env python3
"""Independent dyld inventory; address joins to local scan; no ordinal ObjC pairing."""
import collections,csv,hashlib,json,re,sqlite3,struct
from pathlib import Path
I=Path('/home/exedev/work/inputs');O=Path('initial');D=sqlite3.connect(O/'audit.sqlite')
def tsv(n,cols,rows):
 with (O/n).open('w') as f:w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(cols);w.writerows(rows)
def parse(v):
 methods=[];cl=set();categories=set();context=None
 for ln,line in enumerate((I/f'context/analysis/data/dobjc{v}.txt').open(),1):
  m=re.search(r'@interface (\w+)(?:\(([^)]+)\))?',line)
  if m:
   context=(m[1],m[2] or '');cl.add(m[1]);
   if m[2]:categories.add(context)
  if '@end' in line:context=None
  m=re.search(r'0x([0-9A-Fa-f]+)  ([+-])\[([^ ]+) (.+)\]',line)
  if m and context:
   assert context[0]==m[3];methods.append(dict(cls=context[0],category=context[1],kind=m[2],selector=m[4],va=int(m[1],16),line=ln))
 return cl,categories,methods
def thin_symbols(path):
 # Only load commands and a bounded symbol chunk; resolve strings by a small window.
 with path.open('rb') as f:
  h=f.read(32);magic,cpu,st,ft,nc,cs,flags,res=struct.unpack('<IiiIIIII',h);assert magic==0xfeedfacf
  lcs=f.read(cs);p=0;libs=[];sym=None;ex=None
  for j in range(nc):
   cmd,sz=struct.unpack_from('<II',lcs,p);lc=lcs[p:p+sz]
   if cmd in (0xc,0x80000018,0x8000001f,0x80000023):
    no=struct.unpack_from('<I',lc,8)[0];libs.append(lc[no:].split(b'\0')[0].decode())
   elif cmd==2:sym=struct.unpack_from('<4I',lc,8)
   elif cmd==0x80000033:ex=struct.unpack_from('<2I',lc,8)
   elif cmd in (0x22,0x80000022):ex=struct.unpack_from('<2I',lc,40)
   p+=sz
  out=[]
  if sym:
   so,nn,stro,ss=sym
   for k in range(0,nn,4096):
    f.seek(so+k*16);chunk=f.read(min(4096,nn-k)*16)
    for ix,nt,sec,desc,value in struct.iter_unpack('<IBBHQ',chunk):
     if nt&0xe0 or nt&0xe:continue
     ordinal=desc>>8
     if ordinal<1 or ordinal>len(libs) or 'ArcCore' not in libs[ordinal-1]:continue
     assert ix<ss;f.seek(stro+ix);s=f.read(min(ss-ix,2048));assert b'\0' in s
     out.append(s.split(b'\0')[0].decode())
  return out,libs
imports,libs=thin_symbols(I/'binaries/Arc-1.168.0.arm64');supplied=set((I/'context/analysis/data/app168-imports.txt').read_text().splitlines());assert set(imports)==supplied
impcls={n.split('$_')[1] for n in imports if n.startswith('_OBJC_CLASS_$_')}
versions={};methodrows=[];classrows=[];assignment=json.loads((I/'context/class-assignments.json').read_text());groups={c:g for g,o in assignment['groups'].items() for c in o['classes']}
exportsets={v:set((I/f'context/analysis/data/core{v}-exports.txt').read_text().splitlines()) for v in (168,169,170)}
for v in (168,170):
 classes,categories,methods=parse(v);arccl={c for c in classes if c.startswith('Arc')};am=[m for m in methods if m['cls'].startswith('Arc')];im=[m for m in am if m['cls'] in impcls];ec={n.split('$_')[1] for n in exportsets[v] if n.startswith('_OBJC_CLASS_$_')};em={n.split('$_')[1] for n in exportsets[v] if n.startswith('_OBJC_METACLASS_$_')}
 versions[v]={'metadata_all_classes':len(classes),'metadata_all_method_rows':len(methods),'arc_metadata_classes':len(arccl),'arc_categories':len([p for p in categories if p[0].startswith('Arc')]),'arc_method_rows':len(am),'arc_base_method_rows':sum(not m['category'] for m in am),'arc_category_method_rows':sum(bool(m['category']) for m in am),'imported_method_rows':len(im),'imported_base_rows':sum(not m['category'] for m in im),'imported_category_rows':sum(bool(m['category']) for m in im),'imported_dispatch_keys':len({(m['cls'],m['kind'],m['selector']) for m in im}),'exports':len(exportsets[v]),'exported_class_symbols':len(ec),'exported_metaclass_symbols':len(em),'export_classes_not_metadata':sorted(ec-arccl),'metadata_not_exported_classes':sorted(arccl-ec),'export_metaclass_class_symdiff':sorted(ec^em)}
 if v!=170:continue
 gclasses={r[0] for r in D.execute("select distinct substr(name,1,instr(name,'::')-1) from meta where name glob 'Arc*::*'")};bclasses={r[0] for r in D.execute("select distinct substr(name,1,instr(name,'::')-1) from bodies where name glob 'Arc*::*'")}
 versions[v].update(ghidra_Arc_namespaces=len(gclasses),ghidra_Arc_namespaces_not_metadata=sorted(gclasses-arccl),metadata_no_ghidra_namespace=sorted(arccl-gclasses),metadata_no_body_namespace=sorted(arccl-bclasses))
 for c in sorted(arccl):
  ms=[m for m in am if m['cls']==c];counts=collections.Counter()
  for m in ms:
   va=m['va'];meta=D.execute('select name,n,end from meta where va=?',(va,)).fetchone();body=D.execute('select name,n,failed,path,line,warnings from bodies where va=?',(va,)).fetchone();counts['declared']+=1;counts['distinct_IMPs_placeholder']=0
   if meta:counts['metadata_entry']+=1
   if body:counts['header']+=1;counts['failed']+=body[2];counts['warnings']+=body[5]!='{}'
   if body and meta:counts['header_span_disagreement']+=body[1]!=meta[1]
   methodrows.append([c,m['category'],m['kind'],m['selector'],hex(va),groups.get(c,''),int(c in impcls),int(c in ec),'' if not meta else meta[0],'' if not body else body[0],'' if not body else body[1],'' if not meta else meta[1],'' if not body else body[2],'' if not body else f'{body[3]}:{body[4]}','' if not body else body[5]])
  classrows.append([c,groups.get(c,''),int(c in impcls),int(c in ec),counts['declared'],len({m['va'] for m in ms}),counts['metadata_entry'],counts['header'],counts['failed'],counts['warnings'],counts['header_span_disagreement'],int(c in gclasses)])
tsv('method-coverage.tsv',['class','category','kind','selector','engine170_va','spec_group','app_imported_class','exported_class_symbol','TSV_name_at_IMP','C_name_at_IMP','header_body_addresses','TSV_span','failed','header_evidence','warnings'],methodrows)
tsv('class-coverage.tsv',['class','spec_group','app_imported_class','exported_class_symbol','declarations','distinct_IMPs','TSV_entries_at_IMPs','C_headers_at_IMPs','failed_headers','headers_with_warnings','header_span_disagreements','has_Arc_named_TSV_namespace'],classrows)
res={'scope':'dyld declarations independently parsed; app ArcCore imports independently read from pristine app nlist with dylib ordinal; TSV/C joins by exact engine170 IMP, never by name/order','app_imports':len(imports),'app_imported_classes':len(impcls),'app_imported_C_symbols':sorted(supplied-{f'_OBJC_CLASS_$_{c}' for c in impcls}),'app_import_list_binary_verified':True,'export_sets_equal_168_169_170':exportsets[168]==exportsets[169]==exportsets[170],'versions':versions,'class_assignments':{'classes':len(groups),'distinct_classes':len(set(groups)),'metadata_170_class_symdiff':sorted(set(groups)^{r[0] for r in classrows}),'group_counts':dict(collections.Counter(r[5] for r in methodrows))},'method_IMP_header_coverage':{'declarations':len(methodrows),'with_C_header':sum(r[9]!='' for r in methodrows),'without_C_header':sum(r[9]=='' for r in methodrows),'failed_C_headers':sum(r[12]==1 for r in methodrows),'distinct_IMPs':len({r[4] for r in methodrows})}}
(O/'inventory.json').write_text(json.dumps(res,indent=2)+'\n');print(json.dumps(res,indent=2))
