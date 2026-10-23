#!/usr/bin/env python3
"""Independent bounded review inventory. No writer module imports; read-only evidence."""
import collections,csv,hashlib,json,re,sqlite3,struct
from pathlib import Path
ROOT=Path('/home/exedev/work'); OUT=Path(__file__).resolve().parents[1]; WR=ROOT/'agents/spec-1/specs'; IN=ROOT/'inputs'
CLASSES='ArcAddressEntrySaveRequest ArcAdhocHistogramSample ArcAdhocHistogramSamples ArcBackForwardListItem ArcBookmarkImportTargets ArcBrowserContextDownloads ArcBrowserContextExtensions ArcBrowserContextPageContentExtraction ArcCommandBarRankingCandidate ArcCommandBarRankingInput ArcCommandBarRankingResult ArcContentNode ArcExtensionRuntimeError ArcExtractedPageContent ArcFindResult ArcFormControlInfo ArcHSLAMatrix ArcHistogramSample ArcHungRendererDialog ArcImageRep ArcMediaImage ArcMediaPosition ArcMetrics ArcNavigation ArcPermissionRequest ArcSecureContext ArcSessionNavigation ArcSiteSettingInfo ArcTracingConfiguration ArcURLLoadCancellationToken ArcVisitCategory ArcWebContents ArcWebContentsPDFController ArcWebDialog'.split()
def table(path,fields,rows):
 with path.open('w') as f:
  w=csv.DictWriter(f,fields,delimiter='\t',extrasaction='ignore');w.writeheader();w.writerows(rows)
def sha(b):return hashlib.sha256(b).hexdigest()
from macho import Image
m=Image(IN/'binaries/ArcCore-1.170.0.arm64'); binary=[]; classes={}
for c in m.section('__objc_classlist'):
 name=m.name(c)
 if name not in CLASSES:continue
 ro=m.ptr(c+32)&~7;classes[name]=dict(class_va=hex(c),ro_va=hex(ro),instance_start=m.u32(ro+4),instance_size=m.u32(ro+8))
 for x,kind in ((c,'-'),(m.ptr(c),'+')):
  binary+=m.methods(m.ptr((m.ptr(x+32)&~7)+32),name,'',kind)
for c in m.section('__objc_catlist'):
 name=m.name(m.ptr(c+8))
 if name in CLASSES:
  for off,kind in ((16,'-'),(24,'+')):binary+=m.methods(m.ptr(c+off),name,m.string(m.ptr(c)),kind)
key=lambda r:tuple(r[k] for k in ('class_name','category','kind','selector'))
bmap={key(r):r for r in binary};assert len(bmap)==len(binary)==491
rows=list(csv.DictReader((WR/'methods.tsv').open(),delimiter='\t'));assert len(rows)==491
assert set(key(r) for r in rows)==set(bmap) and len(set(key(r) for r in rows))==491
# A second independent identity surface: raw dyld, preserving current category scope.
dyld={};cat=''
for line_no,line in enumerate((IN/'context/analysis/data/dobjc170.txt').open(),1):
 match=re.search(r'@interface (\w+)(?:\(([^)]+)\))?',line)
 if match:owner,cat=match[1],match[2] or ''
 match=re.search(r'(0x[0-9A-Fa-f]+)\s+([+-])\[([^ ]+) ([^]]+)\]',line)
 if match and match[3] in CLASSES:
  imp,kind,name,sel=match.groups();k=(name,cat,kind,sel);assert k not in dyld;dyld[k]=(hex(int(imp,16)),line_no)
assert set(dyld)==set(bmap)
db=sqlite3.connect(f'file:{IN}/corpus-index.sqlite?mode=ro',uri=True);db.execute('PRAGMA cache_size=-4096')
body_cache={};checks=[];citations=set();matrix_counts={};hashes=[]
for name in CLASSES:
 p=WR/'classes'/f'{name}.md';txt=p.read_text();hashes.append(dict(path=str(p),sha256=sha(p.read_bytes())))
 assert txt.startswith('# '+name+'\n')
 matrix=[]
 for line in txt.splitlines():
  if not line.startswith('| '):continue
  cells=[s.strip() for s in line.split('|')[1:-1]]
  if len(cells)==9 and cells[1][:1] in ('-','+'):
   cat,ks,encoding,imp,behavior,backend,evidence,status,unknown=cells
   matrix.append(((name,'' if cat=='(base)' else cat,ks[0],ks[1:]),encoding.strip('`'),imp,behavior))
 for k,enc,imp,beh in matrix:
  source=[r for r in rows if key(r)==k];assert len(source)==1
  r=source[0];assert (enc,imp,beh)==(r['type_encoding'],r['imp_va'],r['behavior']),(k,enc,imp)
 assert collections.Counter(k for k,*_ in matrix)==collections.Counter(k for k in bmap if k[0]==name)
 matrix_counts[name]=len(matrix)
 # Distinguish code-range-looking literals from small offsets and ObjC metadata VAs.
 for s in re.findall(r'0x[0-9a-fA-F]+',txt):
  a=int(s,16)
  if (0x10000<=a<0xc000000 and a!=0x8000000) or a in (0x5810,0x8790):citations.add(a)
for r in rows:
 k=key(r);b=bmap[k];a=int(r['imp_va'],16)
 for f in ('type_encoding','imp_va','method_record_va'):assert r[f]==b[f],(k,f)
 assert dyld[k][0]==r['imp_va'] and dyld[k][1]==int(r['dyld_line'])
 if a not in body_cache:
  x=db.execute('select name,path,offset,length,failed,reported_size from functions where va=?',(a,)).fetchone()
  if x:
   name,p,o,n,failed,size=x
   assert n<=65536,(hex(a),n)
   with open(p,'rb') as f:f.seek(o);raw=f.read(n)
   assert int(re.match(rb'// ==== (0x[0-9a-f]+)',raw)[1],16)==a
   assert (WR/'bodies'/f'{hex(a)}.c').read_bytes()==raw,(a,'writer extraction mismatch')
   (OUT/'bodies'/f'{hex(a)}.c').write_bytes(raw)
   body_cache[a]=dict(body_name=name,path=p,byte_offset=o,c_bytes=n,failed=failed,reported_addresses=size,sha256=sha(raw),warnings='; '.join(re.findall(r'[^\n]*(?:WARNING|does not return|Does not return|Subroutine does not return)[^\n]*',raw.decode())),status='indexed')
  else:body_cache[a]=dict(status='no indexed body')
 checks.append(dict(**{f:r[f] for f in ('class_name','category','kind','selector','imp_va','type_encoding','method_record_va')},dyld_line=dyld[k][1],binary_file_offset=hex(m.off(a)),**body_cache[a]))
# Match every cited code-looking address against exact index and TSV start. Do not treat bounds as bodies.
wanted=set(citations)|set(body_cache);tsvmap={}
with (IN/'metadata/decomp/arccore-1.170.0-functions.tsv').open() as f:
 for line in f:
  v=line.rstrip('\n').split('\t')
  try:a=int(v[0],16)
  except ValueError:continue
  if a in wanted:tsvmap[a]=v
crows=[]
for a in sorted(wanted):
 x=db.execute('select name,path,offset,length,failed,reported_size from functions where va=?',(a,)).fetchone()
 if x:
  name,p,o,n,failed,size=x
  with open(p,'rb') as f:f.seek(o);raw=f.read(min(n,65536))
  (OUT/'bodies'/f'{hex(a)}.c').write_bytes(raw)
  crows.append(dict(va=hex(a),status='indexed' if n<=65536 else 'TRUNCATED',name=name,path=p,offset=o,length=n,sha256=sha(raw),tsv=' | '.join(tsvmap.get(a,[]))))
 else:crows.append(dict(va=hex(a),status='no C header',tsv=' | '.join(tsvmap.get(a,[]))))
layout_checks=[]
for layout in json.loads((WR/'evidence/layout.json').read_text()):
 cls=layout['class_name']; fact=classes[cls];ro=int(fact['ro_va'],16)
 assert layout['metadata_va']==fact['class_va'] and layout['ro_va']==fact['ro_va']
 for k in ('instance_start','instance_size'):assert layout[k]==fact[k]
 va=m.ptr(ro+48);ivs=[]
 if va:
  stride,count=struct.unpack('<II',m.read(va,8));assert stride==32
  for j in range(count):
   rec=va+8+j*stride
   iv=dict(record_va=hex(rec),offset=m.u32(m.ptr(rec)),name=m.string(m.ptr(rec+8)),type=m.string(m.ptr(rec+16)),alignment=m.u32(rec+24),size=m.u32(rec+28))
   ivs.append(iv)
 assert ivs==layout['ivars'],cls
 va=m.ptr(ro+64);ps=[]
 if va:
  stride,count=struct.unpack('<II',m.read(va,8));assert stride==16
  for j in range(count):
   rec=va+8+j*stride;ps.append(dict(record_va=hex(rec),name=m.string(m.ptr(rec)),attributes=m.string(m.ptr(rec+8))))
 assert ps==layout['properties'],cls
 layout_checks.append(dict(class_name=cls,class_va=fact['class_va'],ro_va=fact['ro_va'],ivars=len(ivs),properties=len(ps),status='binary contents equal writer layout records'))
table(OUT/'evidence/layout-checks.tsv',list(layout_checks[0]),layout_checks)
summary=dict(image='ArcCore-1.170.0.arm64',chromium_commit='d0c02bfad08ec92a6ffcf989e53cd995e6381a4c',class_count=len(classes),method_rows=len(rows),identities=len(bmap),base_rows=sum(not r['category'] for r in rows),category_rows=sum(bool(r['category']) for r in rows),class_methods=sum(r['kind']=='+' for r in rows),distinct_imps=len(body_cache),indexed_imp_count=sum(x['status']=='indexed' for x in body_cache.values()),indexed_rows=sum(x['status']=='indexed' for x in checks),failed_rows=sum(x.get('failed',0) for x in checks),matrix_counts=matrix_counts,classes=classes,body_status=dict(collections.Counter(r['body_status'] for r in rows)),semantic_status=dict(collections.Counter(r['semantic_status'] for r in rows)),code_citations=len(crows),code_citation_no_header=[r['va'] for r in crows if r['status']=='no C header'])
provided=json.loads((WR/'coverage.json').read_text())
for k,v in [('assigned_class_count',summary['class_count']),('actual_method_rows',len(rows)),('method_identity_unique',len(bmap)),('base_method_rows',summary['base_rows']),('category_method_rows',summary['category_rows']),('distinct_imp_vas',len(body_cache)),('body_available_rows',summary['indexed_rows']),('semantic_status',summary['semantic_status']),('body_status',summary['body_status'])]:assert provided[k]==v,(k,provided[k],v)
table(OUT/'evidence/method-checks.tsv',list(checks[0]),checks)
table(OUT/'evidence/citation-checks.tsv',['va','status','name','path','offset','length','sha256','tsv'],crows)
table(OUT/'evidence/writer-snapshot.tsv',['path','sha256'],hashes+[dict(path=str(WR/'methods.tsv'),sha256=sha((WR/'methods.tsv').read_bytes())),dict(path=str(WR/'coverage.json'),sha256=sha((WR/'coverage.json').read_bytes()))])
(OUT/'evidence/coverage.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps(summary,indent=2))
