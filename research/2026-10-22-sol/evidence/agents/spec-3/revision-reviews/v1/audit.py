#!/usr/bin/env python3
"""Fresh read-only revision audit; writes only beside this script. No old reproducers."""
import collections, csv, ctypes, difflib, fcntl, hashlib, json, os, re, sqlite3, struct, subprocess
from pathlib import Path
HERE=Path(__file__).resolve().parent
os.sched_setaffinity(0,{min(os.sched_getaffinity(0))})
os.nice(10)
OUT=HERE/'evidence'; OUT.mkdir(exist_ok=True)
INPUT=Path('/home/exedev/work/inputs'); ORIGINAL=Path('/home/exedev/work/agents/spec-3/specs')
REVISED=Path('/home/exedev/work/agents/spec-3/revisions/v1'); PRIOR=Path('/home/exedev/work/agents/spec-2/classreview')
def digest(p):
 h=hashlib.sha256()
 with p.open('rb') as f:
  for b in iter(lambda:f.read(65536),b''):h.update(b)
 return h.hexdigest()
def table(p,rows,fields=None):
 with p.open('w') as f:
  w=csv.DictWriter(f,fieldnames=fields or list(rows[0]),delimiter='\t');w.writeheader();w.writerows(rows)
def rows(p):return list(csv.DictReader(p.open(),delimiter='\t'))
def snapshot():
 return {str(p):digest(p) for root in (ORIGINAL,PRIOR,REVISED) for p in sorted(root.rglob('*')) if p.is_file()}
class MachO:
 def __init__(self,path):
  self.f=path.open('rb');self.path=path;self.seg=[];self.sec={};self.commands=[]
  assert self.at(0,4)==b'\xcf\xfa\xed\xfe'
  pos=32
  for _ in range(struct.unpack('<I',self.at(16,4))[0]):
   cmd,n=struct.unpack('<II',self.at(pos,8));b=self.at(pos,n);self.commands.append((cmd,b))
   if cmd==0x19:
    vm,vs,off,fs=struct.unpack_from('<QQQQ',b,24);self.seg.append((vm,vs,off,fs))
    for j in range(struct.unpack_from('<I',b,64)[0]):
     s=72+80*j;self.sec[b[s:s+16].split(b'\0')[0].decode()]=(struct.unpack_from('<QQ',b,s+32),b[s:s+80])
   pos+=n
 def at(self,off,n):
  assert 0<=n<=65536;self.f.seek(off);b=self.f.read(n);assert len(b)==n;return b
 def offset(self,va,n=1):
  s=[s for s in self.seg if s[0]<=va and va+n<=s[0]+s[3]];assert len(s)==1,hex(va)
  return s[0][2]+va-s[0][0]
 def data(self,va,n):return self.at(self.offset(va,n),n)
 def ptr(self,va):
  v=int.from_bytes(self.data(va,8),'little')
  assert not v>>63,('unexpected bind pointer',hex(va))
  return (v&((1<<36)-1))|(((v>>36)&255)<<56)
 def text(self,va):
  out=b''
  while True:
   b=self.data(va+len(out),1)
   if b==b'\0':return out.decode()
   out+=b;assert len(out)<4096
 def section_ptrs(self,name):
  (va,n),_=self.sec[name];return [self.ptr(p) for p in range(va,va+n,8)]
 def classname(self,cl):return self.text(self.ptr((self.ptr(cl+32)&~7)+24))
 def methodlist(self,va,cl,cat,kind):
  if not va:return []
  flags,count=struct.unpack('<II',self.data(va,8));stride=flags&0xffff;out=[]
  for i in range(count):
   p=va+8+i*stride
   if flags>>31:
    a,b,c=struct.unpack('<iii',self.data(p,12));sel=p+a
    if not flags&0x40000000:sel=self.ptr(sel)
    ty=p+4+b;imp=p+8+c
   else:sel,ty,imp=[self.ptr(p+j) for j in (0,8,16)]
   out.append((cl,cat,kind,self.text(sel),self.text(ty),hex(imp)))
  return out

def decode_setup():
 lib=ctypes.CDLL('/usr/lib/x86_64-linux-gnu/libLLVM-20.so')
 for s in ('TargetInfo','TargetMC','Disassembler'):getattr(lib,'LLVMInitializeAArch64'+s)()
 lib.LLVMCreateDisasm.argtypes=[ctypes.c_char_p,ctypes.c_void_p,ctypes.c_int,ctypes.c_void_p,ctypes.c_void_p];lib.LLVMCreateDisasm.restype=ctypes.c_void_p
 ctx=lib.LLVMCreateDisasm(b'arm64-apple-macos',None,0,None,None);assert ctx
 lib.LLVMDisasmInstruction.argtypes=[ctypes.c_void_p,ctypes.c_void_p,ctypes.c_uint64,ctypes.c_uint64,ctypes.c_char_p,ctypes.c_size_t];lib.LLVMDisasmInstruction.restype=ctypes.c_size_t
 def decode(b,va):
  out=ctypes.create_string_buffer(256);buf=ctypes.create_string_buffer(b)
  n=lib.LLVMDisasmInstruction(ctx,buf,len(b),va,out,len(out))
  return ' '.join(out.value.decode().split()) if n==4 else 'UNDECODED (retained raw word; no instruction claim)'
 return decode

def main():
 baseline=snapshot();(OUT/'protected-before.json').write_text(json.dumps(baseline,indent=2)+'\n')
 checks={};orig=rows(ORIGINAL/'methods.tsv');rev=rows(REVISED/'methods.tsv')
 assigned=json.loads((INPUT/'context/class-assignments.json').read_text())['groups']['spec-3']
 classes=set(assigned['classes']);identity=lambda r:tuple(r[k] for k in ('class_name','category','kind','selector'))
 decl=lambda r:identity(r)+(r['type_encoding'],r['imp_va'])
 assert [decl(r) for r in orig]==[decl(r) for r in rev]
 assert len(rev)==len(set(map(identity,rev)))==assigned['method_rows']==491
 assert set(r['class_name'] for r in rev)==classes
 assert {p.stem for p in (REVISED/'classes').glob('*.md')}==classes
 im=MachO(INPUT/'binaries/ArcCore-1.170.0.arm64');binary=[]
 for cl in im.section_ptrs('__objc_classlist'):
  name=im.classname(cl)
  if name not in classes:continue
  for c,k in ((cl,'-'),(im.ptr(cl),'+')):binary+=im.methodlist(im.ptr((im.ptr(c+32)&~7)+32),name,'',k)
 for cat in im.section_ptrs('__objc_catlist'):
  if int.from_bytes(im.data(cat+8,8),'little')>>63:continue  # external class category bind
  cl=im.ptr(cat+8)
  if cl==0:continue
  name=im.classname(cl)
  if name not in classes:continue
  for off,k in ((16,'-'),(24,'+')):binary+=im.methodlist(im.ptr(cat+off),name,im.text(im.ptr(cat)),k)
 assert collections.Counter(binary)==collections.Counter(map(decl,rev))
 table(OUT/'decoded-methods.tsv',[dict(image=im.path.name,address_basis='unslid Mach-O VA',**dict(zip(('class_name','category','kind','selector','type_encoding','imp_va'),b))) for b in binary])
 dyld=[];cl=cat=''
 for line in (INPUT/'context/analysis/data/dobjc170.txt').open():
  m=re.search(r'@interface (\w+)(?:\(([^)]+)\))?',line)
  if m:cl,cat=m[1],m[2] or ''
  m=re.search(r'(0x[\da-fA-F]+)\s+([-+])\[([^ ]+) ([^]]+)\]',line)
  if m and cl in classes:
   assert cl==m[3];dyld.append((cl,cat,m[2],m[4],hex(int(m[1],16))))
 assert collections.Counter(dyld)==collections.Counter(identity(r)+(r['imp_va'],) for r in rev)
 # Compare all rows, exact rendered behavior, evidence status and links.
 matrices=[];links=0;diffs=[];changed_classes=[];rby={identity(r):r for r in rev}
 for cl in sorted(classes):
  p=REVISED/'classes'/f'{cl}.md';s=p.read_text()
  if s!=(ORIGINAL/'classes'/p.name).read_text():
   changed_classes.append(cl);diffs+=list(difflib.unified_diff((ORIGINAL/'classes'/p.name).read_text().splitlines(True),s.splitlines(True),fromfile='original/'+p.name,tofile='revision/'+p.name))
  for line in s.splitlines():
   if not re.match(r'^\| [^|]+ \| `[-+]',line):continue
   cells=line.split(' | ');category=cells[0][2:];kindsel=cells[1].strip('`');key=(cl,'' if category=='(base)' else category,kindsel[0],kindsel[1:]);r=rby[key]
   assert cells[2]==f"`{r['type_encoding']}`"
   assert cells[3].startswith('['+r['imp_va']+'](')
   assert cells[4]==r['behavior']
   assert cells[7].removesuffix(' |')=='; '.join(r[k] for k in ('confidence','body_status','semantic_status'))
   matrices.append(key)
  for link in re.findall(r'\]\(([^)]+)\)',s):
   if not link.startswith(('https:','http:','#')):assert (p.parent/link.split('#')[0]).exists(),(p,link);links+=1
 assert collections.Counter(matrices)==collections.Counter(map(identity,rev))
 (OUT/'class-changes.diff').write_text(''.join(diffs))
 oldby={identity(r):r for r in orig};changes=[dict(before=oldby[identity(r)],after=r) for r in rev if r!=oldby[identity(r)]]
 assert changes==[{k:x[k] for k in ('before','after')} for x in json.loads((REVISED/'revision-evidence/annotation-changes.json').read_text())]
 (OUT/'annotation-changes.json').write_text(json.dumps(changes,indent=2)+'\n')
 corrections=rows(REVISED/'corrections.tsv');findings=rows(PRIOR/'findings.tsv')
 assert len(corrections)==len(findings)==6
 for i,(c,f) in enumerate(zip(corrections,findings),1):
  assert c['finding_row']==str(i) and (c['class_name'],c['selector'])==(f['class'],f['selector'])
  assert f['claim'] in c['before'] and c['disposition']=='corrected'
  assert c['after']==rby[(f['class'],'','-',f['selector'])]['behavior']
 c=json.loads((REVISED/'coverage.json').read_text())
 for col,target in [('semantic_status','semantic_status_counts'),('body_status','body_status_counts'),('kind','method_kind_counts')]:assert c[target]==dict(collections.Counter(r[col] for r in rev))
 assert c['method_rows_with_open_questions']==sum(bool(r['unresolved_question']) for r in rev)
 for cl in classes:
  rr=[r for r in rev if r['class_name']==cl]
  assert c['per_class'][cl]==dict(method_rows=len(rr),statuses=dict(collections.Counter(r['semantic_status'] for r in rr)))
 checks['coverage']=dict(classes=len(classes),rows=len(rev),imps=len(set(r['imp_va'] for r in rev)),changed_rows=len(changes),changed_classes=changed_classes,links=links,kinds=dict(collections.Counter(r['kind'] for r in rev)),categories=dict(collections.Counter(r['category'] or '(base)' for r in rev)))
 # Frozen historic hashes and preservation of all copied evidence, not just samples.
 historic=json.loads((PRIOR/'evidence/reviewed-inputs.json').read_text())
 for name,h in historic.items():assert digest((PRIOR.parent/name).resolve())==h,name
 preserved=[]
 for folder in ('bodies','evidence','sources'):
  a={str(p.relative_to(ORIGINAL/folder)) for p in (ORIGINAL/folder).rglob('*') if p.is_file()}
  b={str(p.relative_to(REVISED/folder)) for p in (REVISED/folder).rglob('*') if p.is_file()};assert a==b
  for n in sorted(a):
   assert digest(ORIGINAL/folder/n)==digest(REVISED/folder/n)
   preserved.append(dict(path=folder+'/'+n,sha256=digest(ORIGINAL/folder/n)))
 assert digest(ORIGINAL/'report.md')==digest(REVISED/'original-report.md')
 table(OUT/'preserved-files.tsv',preserved)
 checks['preservation']=dict(historical_hashes=len(historic),copied_counts=dict(collections.Counter(x['path'].split('/')[0] for x in preserved)))
 # Re-decode every assigned class layout, and the bounded vtable/data/app citations.
 metadata=json.loads((REVISED/'evidence/metadata.json').read_text())['classes'];layouts=[]
 for cl in sorted(classes):
  r=metadata[cl];va=int(r['metadata_va'],16);ro=im.ptr(va+32)&~7
  assert im.classname(va)==cl and ro==int(r['ro_va'],16)
  assert struct.unpack('<II',im.data(ro+4,8))==(r['instance_start'],r['instance_size'])
  for field,off in [('ivars',48),('properties',64)]:
   ptr=im.ptr(ro+off);actual=[]
   if ptr:
    stride,n=struct.unpack('<II',im.data(ptr,8))
    for i in range(n):
     q=ptr+8+stride*i
     if field=='ivars':actual.append(dict(record_va=hex(q),offset=int.from_bytes(im.data(im.ptr(q),4),'little'),name=im.text(im.ptr(q+8)),type=im.text(im.ptr(q+16)),size=int.from_bytes(im.data(q+28,4),'little')))
     else:actual.append(dict(record_va=hex(q),name=im.text(im.ptr(q)),attributes=im.text(im.ptr(q+8))))
   assert actual==r[field],(cl,field)
   prose=(REVISED/'classes'/f'{cl}.md').read_text()
   for a in actual:
    text=(f"| {a['name']} | {a['offset']} | {a['size']} | `{a['type']}` | {a['record_va']} |" if field=='ivars' else f"| {a['name']} | `{a['attributes']}` | {a['record_va']} |")
    assert text in prose
  layouts.append(dict(image=im.path.name,address_basis='unslid Mach-O VA',class_name=cl,metadata_va=hex(va),ro_va=hex(ro),instance_size=r['instance_size'],ivars=len(r['ivars']),properties=len(r['properties'])))
 table(OUT/'layout-checks.tsv',layouts)
 for r in json.loads((REVISED/'evidence/vtable-prefixes.json').read_text()):assert im.ptr(int(r['slot_va'],16))==int(r['target_va'],16)
 app=MachO(INPUT/'binaries/Arc-1.168.0.arm64');appchecks=[]
 for r in rows(REVISED/'evidence/app-launch-calls.tsv'):
  va=int(r['call_va'],16);word=int.from_bytes(app.data(va,4),'little');assert word==int(r['bl_word'],16) and word>>26==0x25
  disp=word&0x3ffffff;disp=disp-(1<<26) if disp&(1<<25) else disp
  assert va+disp*4==int(r['stub_va'],16) and app.offset(va)==int(r['call_file_offset'],16)
  appchecks.append(dict(address_basis='unslid Mach-O app VA',**r))
 table(OUT/'app-call-checks.tsv',appchecks)
 artifact_count=0
 for line in (ORIGINAL/'evidence/artifacts.sha256').read_text().splitlines():
  h,n=line.split('  ',1);assert digest(ORIGINAL.parent/n)==h,n;artifact_count+=1
 checks['additional_citations']=dict(layouts=37,vtable_slots=16,app_BL_words=4,historical_artifact_hashes=artifact_count)
 # Read the original corpus ONLY via the bounded helper. Sequential groups cap capture memory.
 db=sqlite3.connect(f'file:{INPUT}/corpus-index.sqlite?mode=ro',uri=True)
 vas=set(int(r['imp_va'],16) for r in rev)|{int(r['va'],16) for r in rows(REVISED/'revision-evidence/body-provenance.tsv')}
 vas|={0x4338920,0x4404e6c,0x283ebf0,0x43e3394,0x44195b4,0x44197a8,0x441999c,0x4419b90}
 bodyrows=[];(OUT/'bodies').mkdir(exist_ok=True)
 for start in range(0,len(vas),40):
  batch=sorted(vas)[start:start+40]
  output=subprocess.run(['python3','/home/exedev/work/read-function.py',*map(hex,batch),'--max-bytes','32768'],check=True,capture_output=True).stdout
  entries=re.split(rb'(?=^(?:INDEX |NO_EXPORTED_BODY ))',output,flags=re.M)
  for entry in entries:
   if not entry:continue
   va=int(re.search(rb'0x[0-9a-f]+',entry)[0],16);path=OUT/'bodies'/f'{va:08x}.txt';path.write_bytes(entry)
   row=db.execute('select name,path,offset,length,failed from functions where va=?',(va,)).fetchone()
   if row is None:
    bodyrows.append(dict(va=hex(va),name='',source_shard='',offset='',C_bytes='',body_sha256='',state='NO_EXACT_EXPORTED_BODY'));continue
   name,shard,offset,n,failed=row;assert not failed and n<=32768
   body=entry.split(b'\n',1)[1];assert body[n:]==b'\n'
   body=body[:n];assert body.startswith(f'// ==== {hex(va)} '.encode())
   h=hashlib.sha256(body).hexdigest();copied=REVISED/'bodies'/f'{va:08x}.txt'
   if copied.exists():assert body in copied.read_bytes()
   fresh=REVISED/'revision-evidence'/f'{va:08x}.read-function.txt'
   if fresh.exists():assert fresh.read_bytes()==entry
   bodyrows.append(dict(va=hex(va),name=name,source_shard=shard,offset=offset,C_bytes=n,body_sha256=h,state='COMPLETE_BOUNDED_HELPER_READ'))
 table(OUT/'body-checks.tsv',[dict(image=im.path.name,address_basis='unslid Mach-O VA',**r) for r in bodyrows])
 prior_bodies={r['va']:r for r in rows(PRIOR/'evidence/body-checks.tsv')}
 for r in bodyrows:
  if r['va'] in prior_bodies:assert r['body_sha256']==prior_bodies[r['va']]['sha256']
 checks['bodies']=dict(exact_reads=sum(r['state']=='COMPLETE_BOUNDED_HELPER_READ' for r in bodyrows),missing_exact_headers=[r['va'] for r in bodyrows if r['state']=='NO_EXACT_EXPORTED_BODY'],assigned_imps=len(prior_bodies))
 # Validate every revision instruction byte and re-disassemble independently.
 decode=decode_setup();inst=rows(REVISED/'revision-evidence/instructions.tsv');decoded=[]
 for r in inst:
  va=int(r['va'],16);b=im.data(va,4)
  assert r['image']==im.path.name and int(r['file_offset'],16)==im.offset(va,4) and r['bytes']==b.hex()
  ins=decode(b,va);assert ins==r['instruction'],(r,ins)
  decoded.append(dict(image=im.path.name,address_basis='unslid Mach-O VA',va=hex(va),file_offset=hex(im.offset(va,4)),bytes=b.hex(),instruction=ins))
 table(OUT/'instructions.tsv',decoded)
 for va in (0x15ac0,0xfbc0):assert im.data(va,4)==b'\xc0\x03\x5f\xd6'
 for r in rows(REVISED/'revision-evidence/data.tsv'):
  va=int(r['va'],16);assert im.data(va,len(bytes.fromhex(r['bytes']))).hex()==r['bytes'] and im.offset(va)==int(r['file_offset'],16)
 assert im.text(im.ptr(0xe2a97b0))=='owner';assert im.text(im.ptr(0xe2a97b8))=='T@"ArcWebContents",W,N,V_owner'
 assert (im.ptr(0xdf6b728),im.ptr(0xdf6b730))==(0x4044e80,0x4044ec0)
 for r in json.loads((REVISED/'evidence/arm64-leaves.json').read_text()):
  va=int(r['imp'],16);words=r['words'];assert im.offset(va)==int(r['file_offset'],16)
  assert im.data(va,len(words)*4)==b''.join(int(w,16).to_bytes(4,'little') for w in words)
 # Mach-O symbol/indirect table resolution of selected veneers.
 syms=next(struct.unpack_from('<IIII',b,8) for cmd,b in im.commands if cmd==2)
 indirect=next(struct.unpack_from('<I',b,56)[0] for cmd,b in im.commands if cmd==0xb)
 (stubva,stubsize),sb=im.sec['__stubs'];first,stride=struct.unpack_from('<II',sb,68)
 for r in rows(REVISED/'revision-evidence/runtime-symbols.tsv'):
  va=int(r['veneer_va'],16);b=im.data(va,12);assert b.hex()==r['veneer_bytes']
  a,add,br=struct.unpack('<III',b);assert a&0x9f00001f==0x90000010 and add&0xffc003ff==0x91000210 and br==0xd61f0200
  disp=((a>>29)&3)|(((a>>5)&0x7ffff)<<2);disp=disp-(1<<21) if disp&(1<<20) else disp
  target=(va&~4095)+disp*4096+((add>>10)&4095);assert hex(target)==r['stub_va']
  assert stubva<=target<stubva+stubsize and (target-stubva)%stride==0
  idx=int.from_bytes(im.at(indirect+4*(first+(target-stubva)//stride),4),'little');assert idx<int(syms[1]) and idx==int(r['indirect_symbol_index'])
  off=int.from_bytes(im.at(syms[0]+idx*16,4),'little');name=im.at(syms[2]+off,min(512,syms[3]-off)).split(b'\0')[0].decode();assert name==r['symbol']
 checks['instructions']=dict(words=len(inst),undecoded=sum('UNDECODED' in r['instruction'] for r in inst),leaf_rows=9,runtime_symbols=6)
 # Exact source file hashes and windows, with explicit pinned revision.
 sourcechecks=[];excerpts=[]
 sources=json.loads((REVISED/'evidence/sources.json').read_text())+json.loads((REVISED/'revision-evidence/source-checks.json').read_text())
 for s in sources:
  assert s['commit']=={'Chromium':'d0c02bfad08ec92a6ffcf989e53cd995e6381a4c','CEF':'426920c33ef22551e0b8f48cf343d623ac25642d'}[s['repository']]
  p=Path(s['local_path']);p=p if p.is_absolute() else REVISED/'sources/cef'/Path(s['file']).name
  assert digest(p)==s['sha256'];lines=p.read_text().splitlines()
  windows=s.get('line_windows') or [(s['first_line'],s['last_line'])]
  for lo,hi in windows:excerpts.append(f"\n{s['repository']} {s['commit']} {p}:{lo}-{hi}\n"+'\n'.join(f'{i}: {lines[i-1]}' for i in range(lo,hi+1))+'\n')
  sourcechecks.append(dict(repository=s['repository'],commit=s['commit'],path=str(p),sha256=digest(p)))
 table(OUT/'source-checks.tsv',sourcechecks);(OUT/'source-excerpts.txt').write_text(''.join(excerpts))
 checks['source_manifests_checked']=len(sourcechecks)
 # Full binary image hashing is serialized under the existing read-only lock.
 fingerprints=json.loads((ORIGINAL/'evidence/input-sha256.json').read_text())
 with Path('/home/exedev/work/heavy.lock').open('rb') as lock:
  fcntl.flock(lock,fcntl.LOCK_EX)
  for rel,h in fingerprints.items():assert digest(INPUT/rel)==h,rel
 checks['input_hashes_checked']=fingerprints
 assert snapshot()==baseline,'protected files changed during audit'
 checks['protected_files_unchanged']=len(baseline)
 (OUT/'audit.json').write_text(json.dumps(checks,indent=2)+'\n');print(json.dumps(checks,indent=2))
if __name__=='__main__':main()
