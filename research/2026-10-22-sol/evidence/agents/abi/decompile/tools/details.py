#!/usr/bin/env python3
"""Generate independently decodable pointer/selector/boundary evidence and app excerpts."""
import sys,json,struct,csv,re,bisect,array,hashlib
from pathlib import Path
OUT=Path(__file__).resolve().parents[1];ROOT=Path('/home/exedev/work/inputs')
sys.path.insert(0,str(OUT.parent/'scripts'))
from inventory import MachO,write_tsv
from calls import functions
b=MachO(ROOT/'binaries/ArcCore-1.170.0.arm64')
def u32(a):return struct.unpack('<I',b.read(a,4))[0]
def selector(adrp,ldr):
 a=u32(adrp);c=u32(ldr);assert a&0x9f00001f==0x90000008;assert c&0xffc003ff==0xf9400101
 n=((a>>29)&3)|(((a>>5)&0x7ffff)<<2);n=n-(1<<21) if n&(1<<20) else n
 slot=(adrp&~4095)+(n<<12)+((c>>10)&0xfff)*8;value=b.pointer(slot)
 return dict(adrp_va=hex(adrp),ldr_va=hex(ldr),selector_ref_va=hex(slot),selector_string_va=hex(value),selector=b.string(value))
sels=[selector(a,l) for a,l in [(0x40427fc,0x4042800),(0x4445aa8,0x4445aac),(0x4415c48,0x4415c4c),(0x4440f74,0x4440f78)]]
assert sels[0]['selector']=='browserContext:onDeterminingFilename:defaultFilename:completionTimeout:completion:'
write_tsv(OUT/'binary-selectors.tsv',sels)
ptrs=[]
for base,n in [(0xdd2bfb0,7),(0xdf6b728,3),(0xdf6bc88,3),(0xda804e0,4)]:
 for i in range(n):
  p=base+i*8;ptrs.append(dict(table_va=hex(base),slot_va=hex(p),target_va=hex(b.pointer(p))))
write_tsv(OUT/'binary-pointers.tsv',ptrs)
domain_export=0xdf6bb28;cf=b.pointer(domain_export);chars=b.pointer(cf+16);length=b.u64(cf+24)
(OUT/'error-domain.json').write_text(json.dumps(dict(export_va=hex(domain_export),cfstring_va=hex(cf),chars_va=hex(chars),length=length,value=b.read(chars,length).decode(),status_translation_va='0xc9d8148',status_translation_bytes=b.read(0xc9d8148,12).hex()),indent=2)+'\n')
# Large metadata regions are recorded as unusable contiguous ranges, not as code volume.
starts=array.array('Q');large=[]
for ln,line in enumerate((ROOT/'metadata/decomp/arccore-1.170.0-functions.tsv').open(),1):
 a,e,n,name=re.split(r'\\t|\t',line.rstrip('\n'));a=int(a,16);e=int(e,16);n=int(n);starts.append(a)
 if e-a>=1<<20:large.append(dict(va=hex(a),end=hex(e),span=e-a,metadata_body_bytes=n,name=name,metadata_line=ln))
for r in large:
 r['other_metadata_starts_inside']=bisect.bisect_left(starts,int(r['end'],16))-bisect.bisect_right(starts,int(r['va'],16))
 r['status']='reject_as_contiguous_function_region;body_correctness_requires_reanalysis'
write_tsv(OUT/'malformed-region-candidates.tsv',large)
# Small, targeted APP source extracts preserve the source locator and original text.
appwanted={0x1001066ec,0x100106810,0x100106c60,0x1004ae4a0,0x1004e9748,0x1004e9ac4,0x1004e9da8,0x1004f2640,0x1004f285c,0x1004f2a24,0x1004ec75c,0x100535984,0x100536028,0x10053605c,0x1004aefe4,0x10053b870}
rows=[]
with (OUT/'app-selected.c').open('w') as out:
 for shard in ['0x1001.c','0x1004.c','0x1005.c']:
  p=ROOT/'app168/ghidra/decomp'/shard
  for h,body in functions(p):
   if int(h['va'],16) not in appwanted:continue
   out.write(f"// SOURCE {p}:{h['header_line']}\n// ==== {h['va']} {h['name']}\n{body}\n")
   rows.append(dict(va=h['va'],name=h['name'],path=str(p),line=h['header_line'],body_sha256=hashlib.sha256(body.encode()).hexdigest()))
assert len(rows)==len(appwanted),(len(rows),len(appwanted))
write_tsv(OUT/'app-selected.tsv',rows)
# Pin every primary source used for semantic comparison.
revision='d0c02bfad08ec92a6ffcf989e53cd995e6381a4c'
files=['base/apple/call_with_eh_frame.h','base/apple/call_with_eh_frame.cc','base/apple/call_with_eh_frame_asm.S','components/download/public/common/download_item.h','ui/base/window_open_disposition.h','content/public/browser/devtools_agent_host_client.h','content/browser/devtools/devtools_agent_host_impl.cc']
rows=[]
for rel in files:
 p=ROOT/'chromium-156'/rel
 rows.append(dict(revision=revision,path=str(p),relative_path=rel,sha256=hashlib.sha256(p.read_bytes()).hexdigest(),url=f'https://chromium.googlesource.com/chromium/src/+/{revision}/{rel}'))
write_tsv(OUT/'primary-sources.tsv',rows)
print(dict(selectors=len(sels),pointer_entries=len(ptrs),large_metadata_regions=len(large),app_bodies=len(appwanted),source_files=len(files)))
