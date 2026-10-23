#!/usr/bin/env python3
"""Save bounded, exact byte-range body copies and verify selected app B/BL selector sends."""
import csv,hashlib,json,pathlib,sqlite3,struct,sys
O=pathlib.Path(__file__).resolve().parents[1];I=pathlib.Path('/home/exedev/work/inputs');sys.path.insert(0,str(O.parent/'scripts'))
from inventory import MachO
c=sqlite3.connect(f'file:{I}/corpus-index.sqlite?mode=ro',uri=True)
vas='43f598c 44378a0 4437a90 42df0b8 42e4000 43e6ec0 43e6ee0 43eaae8 42b2370 42b2430 42aae64 41c8 43e6548 43e6fc8 42b17e8 42b0300 43ec404 433260c 42b0850 42aa7ec 429608c 2d2f3cc 42b1ba8 42aaaa0 42aac34 42ab360 318b9bc 4332148 42b7974 42b1200 42b13c4 43eb374 43e8368 42b3d74 7bdc470 42b3f34 442548 4402548 43eb7a0 43e7048 42b32d0 42f8300 42b4cc4 42c5400 43e154c 42faa84 43f35bc 42d7064 2dba704 4346528 4346100 443739c 4440c4c 47390dc 4410bb8 4410cd0 4410ec0 4376580 4739114 47391c8 4737eb4 4738a30 4738ad4 43755a4 43a809c 43aa39c 43aaea8 442a794 442aef4'.split()
# Omit an accidental exploratory nonentry; requests separately retain only evidenced targets.
vas=[int(x,16) for x in vas if x!='442548']
rows=[]
for va in vas:
 q=c.execute('select name,path,offset,length,failed,reported_size from functions where va=?',(va,)).fetchone()
 if not q:continue
 name,path,off,n,failed,size=q
 assert n<=65536,(hex(va),n)
 with open(path,'rb') as f:f.seek(off);body=f.read(n)
 assert body.startswith(f'// ==== {va:#x}  '.encode())
 local=O/'bodies'/f'{va:#x}.c';local.write_bytes(body)
 rows.append(dict(va=hex(va),name=name,path=path,offset=off,C_bytes=n,failed=failed,header_size=size,body_sha256=hashlib.sha256(body).hexdigest(),copy=str(local.relative_to(O))))
def write(file,rows,fields=None):
 with (O/file).open('w') as f:w=csv.DictWriter(f,fields or list(rows[0]),delimiter='\t');w.writeheader();w.writerows(rows)
write('reviewed-body-index.tsv',rows)
# Source-backed naming comparisons: pinned mirror; no claim of whole-function equality.
source_specs=[('content/app/content_main.cc',189,347,'0x2d2f3cc','RunContentProcess','RunContentProcess'),('content/browser/browser_main_runner_impl.cc',158,185,'0x318b9bc','BrowserMainRunnerImpl::Shutdown','initialization_started_'),('chrome/browser/profiles/profile_manager.cc',1037,1099,'0x7bdc470','ProfileManager::CreateMultiProfileAsync','CreateMultiProfileAsync'),('content/browser/web_contents/web_contents_impl.cc',1628,1713,'0x2dba704','WebContentsImpl::CreateWithOpener','CreateWithOpener')]
sources=[]
for rel,start,end,va,name,token in source_specs:
 p=I/'chromium-156'/rel;b=p.read_bytes();assert token.encode() in b
 sources.append(dict(engine_va=va,proposed_name=name,confidence='high_identity_no_equivalence_claim',revision='d0c02bfad08ec92a6ffcf989e53cd995e6381a4c',path=str(p),start_line=start,end_line=end,sha256=hashlib.sha256(b).hexdigest(),url='https://chromium.googlesource.com/chromium/src/+/d0c02bfad08ec92a6ffcf989e53cd995e6381a4c/'+rel))
write('source-backed-identities.tsv',sources)
# B and BL to stubs: include tail sends in tiny Swift wrappers.
m=MachO(I/'binaries/Arc-1.168.0.arm64');sec=next(s for s in m.sections if s['section']=='__objc_stubs')
wanted={'run','nativeView','createPaneHostView','attachToPaneHostView:','detachFromPaneHostView:','createBrowser:withDelegate:forWindow:','createBrowserContext:isHidden:type:completion:error:'}
stubs={}
for va in range(sec['va'],sec['va']+sec['size'],32):
 w,w2=struct.unpack('<II',m.vread(va,8))
 if w&0x9f00001f!=0x90000001 or w2&0xffc003ff!=0xf9400021:continue
 imm=((w>>29)&3)|(((w>>5)&0x7ffff)<<2)
 if imm&(1<<20):imm-=1<<21
 ref=(va&~4095)+(imm<<12)+(((w2>>10)&0xfff)*8);sel=m.string(m.ptr(ref))
 if sel in wanted:stubs[va]=(sel,ref)
target={0x1004ae240,0x10054267c,0x1005426a0,0x1005426c4,0x1005426d8,0x1004ad0a0,0x1004af380};rows=[]
for l in (I/'app168/ghidra/functions.tsv').open():
 a,b,n,name=l.rstrip('\n').split('\t');start=int(a,16);end=int(b,16)
 if start not in target:continue
 for j,(w,) in enumerate(struct.iter_unpack('<I',m.vread(start,end-start))):
  if w&0x7c000000!=0x14000000:continue
  pc=start+4*j;imm=w&0x3ffffff
  if imm&(1<<25):imm-=1<<26
  dest=pc+imm*4
  if dest not in stubs:continue
  sel,ref=stubs[dest]
  rows.append(dict(app_function_va=hex(start),app_function=name,call_va=hex(pc),file_offset=hex(m.offset(pc)),instruction=hex(w),kind='BL' if w&0x80000000 else 'B_tail',stub_va=hex(dest),selector_ref_va=hex(ref),selector=sel,receiver_basis='old_app_pseudocode_flow_not_runtime_receiver_proof',app_shard='app168/ghidra/decomp/0x1005.c' if start>=0x100500000 else 'app168/ghidra/decomp/0x1004.c'))
assert set(r['selector'] for r in rows)==wanted
write('app168-essential-sends.tsv',rows)
# Precise entry recovery requests: no shard transfer is needed.
requests=[('0x43eb7a0','0x43e8368','context completion converts native result into ObjC values; callback cardinality and error mapping','0x43.c'),('0x42c18f0','0x42b1ba8','startup main-loop callback target; ordering of did-initialize notification','0x42.c'),('0x4429bcc','0x43aa39c','RenderFrameAgent dispatch/validation entry; schema and method ordinals','0x44.c'),('0x442ad24','0x43aaea8','RenderProcessAgent dispatch/validation entry; schema and method ordinals','0x44.c'),('0x43a8a50','0x43a809c','EffectsAgent binder target and lifetime','0x43.c'),('0x43a482c','0x43a809c','AdBlockerAgent binder target and lifetime','0x43.c')]
r=[]
for va,caller,why,shard in requests:
 assert c.execute('select 1 from functions where va=?',(int(va,16),)).fetchone() is None
 r.append(dict(target_va=va,address_kind='referenced_LAB_target_not_verified_function_entry',caller_va=caller,expected_shard=shard,corpus_status='shard_present_no_exact_exported_body',request='recover_function_boundary_and_bounded_disassembly_then_reexport',dependent_question=why))
for gap in csv.DictReader((O.parent/'initial/decompile-gap-requests.tsv').open(),delimiter='\t'):
 va=gap['address']
 assert c.execute('select 1 from functions where va=?',(int(va,16),)).fetchone() is None
 r.append(dict(target_va=va,address_kind='instruction_not_function_entry',caller_va='',expected_shard=gap['expected_shard'],corpus_status='shard_present_no_exact_exported_body_at_instruction',request='recover_exact_containing_body_ranges_and_reexport',dependent_question=gap['reason']))
r.append(dict(target_va='0x441d770',address_kind='exported_function_entry',caller_va='',expected_shard='0x44.c',corpus_status='shard_present_no_exact_exported_body',request='recover_bounded_disassembly_and_body',dependent_question='CallWithEHFrame imported C boundary exception/unwind contract; no new semantic conclusion here'))
write('additional-shards.tsv',r)
prior=[]
for req in csv.DictReader((O.parent/'initial/decompile-requests.tsv').open(),delimiter='\t'):
 q=c.execute('select name,path,offset,length,failed from functions where va=?',(int(req['address'],16),)).fetchone()
 prior.append(dict(va=req['address'],anchor=req['symbol_anchor'],status='failed' if q and q[-1] else 'present' if q else 'no_exact_exported_body',path=q[1] if q else '',offset=q[2] if q else '',C_bytes=q[3] if q else ''))
write('initial-request-availability.tsv',prior)
print(json.dumps(dict(saved_bodies=len(list((O/'bodies').glob('*.c'))),app_B_BL_sends=len(rows),source_backed_identities=len(sources),missing_entry_requests=len(r)),indent=2))
