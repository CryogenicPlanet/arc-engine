#!/usr/bin/env python3
import bisect,collections,csv,json,pathlib,re,sqlite3,struct,sys
O=pathlib.Path(__file__).resolve().parents[1];I=pathlib.Path('/home/exedev/work/inputs')
sys.path.insert(0,str(O.parents[0]/'scripts'))
from inventory import MachO
c=sqlite3.connect(f'file:{I}/corpus-index.sqlite?mode=ro',uri=True)
meta=[]
for l in (I/'metadata/decomp/arccore-1.170.0-functions.tsv').open():
 a,b,n,name=l.rstrip('\n').split('\t');meta.append((int(a,16),int(b,16),int(n),name))
starts=[x[0] for x in meta]
def out(file,fields,rows):
 with (O/file).open('w') as f:w=csv.DictWriter(f,fields,delimiter='\t');w.writeheader();w.writerows(rows)
rows=[]
for va,end,n,name in meta:
 if n<1048576:continue
 q=c.execute('select path,offset,length,reported_size from functions where va=?',(va,)).fetchone()
 rows.append(dict(va=hex(va),name=name,metadata_extent=n,interior_saved_entries=bisect.bisect_left(starts,end)-bisect.bisect_right(starts,va),path=q[0] if q else '',offset=q[1] if q else '',C_bytes=q[2] if q else '',header_size=q[3] if q else '',finding='merged_extent_review_not_exclusive_code'))
out('merged-metadata-regions.tsv',list(rows[0]),rows)
# A high expansion ratio is a review heuristic, not proof of a bad boundary.
rows=[]
m=MachO(I/'binaries/ArcCore-1.170.0.arm64')
for va,name,size,path,off,length in c.execute('select va,name,reported_size,path,offset,length from functions where length>65536 and length>1024*reported_size'):
 with open(path,'rb') as f:f.seek(off);b=f.read(length)
 labels=[int(x,16) for x in re.findall(rb'LAB_([0-9a-f]{8})',b)]
 branches=[]
 for j,(w,) in enumerate(struct.iter_unpack('<I',m.vread(va,min(size,64)))):
  if w&0x7c000000==0x14000000:
   imm=w&0x3ffffff
   if imm&0x2000000:imm-=0x4000000
   branches.append(f'{va+4*j:#x}->{va+4*j+4*imm:#x}')
 rows.append(dict(va=hex(va),name=name,header_size=size,C_bytes=length,path=path,offset=off,label_min=hex(min(labels)) if labels else '',label_max=hex(max(labels)) if labels else '',initial_B_BL=';'.join(branches),first_instruction=hex(struct.unpack('<I',m.vread(va,4))[0]),next_entry=hex(starts[bisect.bisect_right(starts,va)]) if bisect.bisect_right(starts,va)<len(starts) else '',finding='extreme_expansion_review_no_semantics_accepted'))
out('malformed-body-review.tsv',list(rows[0]),rows)
print(json.dumps(dict(large_metadata_regions=len(list(csv.DictReader(open(O/'merged-metadata-regions.tsv'),delimiter='\t'))),extreme_expansion_bodies=len(rows)),indent=2))
