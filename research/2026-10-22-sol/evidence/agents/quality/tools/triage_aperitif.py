#!/usr/bin/env python3
"""Static fat/thin Mach-O metadata parser for the supplied SMALL dylib only."""
import collections,csv,hashlib,json,re,struct
from pathlib import Path
P=Path('/home/exedev/work/inputs/binaries/libaperitif.dylib');O=Path('initial');data=P.read_bytes();assert len(data)<1024*1024
S=lambda b:b.split(b'\0')[0].decode('utf-8','replace')
def uleb(b,o):
 v=s=0
 while True:
  x=b[o];o+=1;v|=(x&127)<<s
  if not x&128:return v,o
  s+=7;assert s<64
assert data[:4]==bytes.fromhex('cafebabe');n=struct.unpack_from('>I',data,4)[0];archs=[];symbolrows=[];strrows=[];cstrrows=[]
for i in range(n):
 cpu,sub,off,size,align=struct.unpack_from('>iiIII',data,8+i*20);b=data[off:off+size];assert len(b)==size;magic,c,st,ft,nc,cs,flags,res=struct.unpack_from('<IiiIIIII',b);assert magic==0xfeedfacf and c==cpu
 arch={0x1000007:'x86_64',0x100000c:'arm64'}[cpu];r={'architecture':arch,'cpu_subtype':sub,'fat_offset':off,'slice_bytes':size,'slice_sha256':hashlib.sha256(b).hexdigest(),'filetype':ft,'flags':hex(flags),'segments':[],'sections':[],'dependencies':[],'load_commands':[]};pos=32;sym=None;trie=None
 for j in range(nc):
  cmd,sz=struct.unpack_from('<II',b,pos);assert sz>=8 and pos+sz<=32+cs
  lc=b[pos:pos+sz];r['load_commands'].append({'cmd':hex(cmd),'offset_in_slice':pos,'bytes':sz})
  if cmd in (0xc,0xd,0x80000018,0x8000001f,0x80000023):
   no,timestamp,current,compat=struct.unpack_from('<4I',lc,8);v={'name':S(lc[no:]),'current':current,'compatibility':compat,'command':hex(cmd)}
   if cmd==0xd:r['install_name']=v
   else:r['dependencies'].append(v)
  elif cmd==0x19:
   sn,va,vs,fo,fs,mp,ip,ns,fl=struct.unpack_from('<16sQQQQiiII',lc,8);r['segments'].append({'name':S(sn),'va':hex(va),'vmsize':vs,'fileoff_in_slice':fo,'filesize':fs,'initprot':ip})
   for k in range(ns):
    sn,sg,a,z,fo2,al,ro,nr,sf,r1,r2,r3=struct.unpack_from('<16s16sQQIIIIIIII',lc,72+k*80);r['sections'].append({'name':S(sn),'segment':S(sg),'va':hex(a),'bytes':z,'fileoff_in_slice':fo2,'flags':hex(sf)})
  elif cmd==2:sym=struct.unpack_from('<4I',lc,8)
  elif cmd==0x1b:r['uuid']=lc[8:24].hex()
  elif cmd==0x80000033:trie=struct.unpack_from('<II',lc,8)
  elif cmd in (0x22,0x80000022):trie=struct.unpack_from('<II',lc,40)
  elif cmd==0x32:r['build_version']={'platform':struct.unpack_from('<I',lc,8)[0],'minos':hex(struct.unpack_from('<I',lc,12)[0]),'sdk':hex(struct.unpack_from('<I',lc,16)[0])}
  pos+=sz
 assert pos==32+cs
 imports=[];exports=[];locals=[]
 if sym:
  so,nn,stro,strsz=sym;assert so+nn*16<=len(b) and stro+strsz<=len(b)
  for k in range(nn):
   ix,nt,sec,desc,value=struct.unpack_from('<IBBHQ',b,so+k*16);name=S(b[stro+ix:stro+strsz]);typ='debug' if nt&0xe0 else ('import' if nt&0xe==0 else ('external_definition' if nt&1 and not nt&0x10 else 'local_or_private'))
   symbolrows.append([arch,typ,name,hex(value),hex(nt),sec,hex(desc),desc>>8])
   if typ=='import':imports.append(name)
   elif typ=='external_definition':exports.append(name)
   elif typ=='local_or_private':locals.append(name)
 r.update(symtab_imports=imports,symtab_external_definitions=exports,local_private_symbol_count=len(locals))
 ex=[]
 if trie and trie[1]:
  to,tn=trie;t=b[to:to+tn];assert len(t)==tn
  def walk(o,prefix,parents):
   assert o not in parents;terminal,p=uleb(t,o);q=p+terminal
   if terminal:
    fl,p2=uleb(t,p);record={'name':prefix,'flags':hex(fl)}
    if fl&8:ordinal,p2=uleb(t,p2);record.update(reexport_ordinal=ordinal,reexport_name=S(t[p2:q]))
    else:addr,p2=uleb(t,p2);record['address_or_offset']=hex(addr)
    ex.append(record)
   ch=t[q];q+=1
   for k in range(ch):
    e=t.index(0,q);edge=t[q:e].decode();child,q=uleb(t,e+1);walk(child,prefix+edge,parents|{o})
  walk(0,'',set())
 r['export_trie']=ex
 r['static_initializer_offsets']=[]
 for sec in r['sections']:
  if sec['name']=='__init_offsets':
   at=sec['fileoff_in_slice'];r['static_initializer_offsets']=[hex(v[0]) for v in struct.iter_unpack('<I',b[at:at+sec['bytes']])]
  if sec['name']=='__cstring':
   at=sec['fileoff_in_slice'];end=at+sec['bytes']
   while at<end:
    z=b.index(0,at,end)
    if z>at:cstrrows.append([arch,hex(off+at),hex(at),b[at:z].decode('utf-8','replace')])
    at=z+1
 for m in re.finditer(rb'[ -~]{6,}',b):
  loc=m.start();sec=next((s for s in r['sections'] if s['fileoff_in_slice']<=loc<s['fileoff_in_slice']+s['bytes']),None)
  strrows.append([arch,hex(off+loc),hex(loc),'' if not sec else sec['segment']+','+sec['name'],m.group().decode()])
 archs.append(r)
res={'file':str(P),'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest(),'architectures':archs,'scope':'Static load-command, symbol, export-trie and ASCII-string triage only; no execution or decompilation.'}
(O/'aperitif.json').write_text(json.dumps(res,indent=2)+'\n')
for fn,cols,rows in [('aperitif-symbols.tsv',['architecture','kind','symbol','value','n_type','section','n_desc','library_ordinal'],symbolrows),('aperitif-strings.tsv',['architecture','file_offset_fat','file_offset_slice','section','string'],strrows),('aperitif-cstrings.tsv',['architecture','file_offset_fat','file_offset_slice','string'],cstrrows)]:
 with (O/fn).open('w') as f:w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(cols);w.writerows(rows)
print(json.dumps({**{k:v for k,v in res.items() if k!='architectures'},'architectures':[{k:v for k,v in a.items() if k not in ('sections','load_commands','symtab_imports','symtab_external_definitions')} for a in archs]},indent=2))
