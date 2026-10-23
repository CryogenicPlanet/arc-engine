"""Small direct app-call sample: BL instructions to three exact selector stubs.
Receiver association is NOT established by selector sends alone.
"""
from pathlib import Path
import struct,csv,bisect,array,json
from metadata import MachO,tsv
inp=Path('/home/exedev/work/inputs');out=Path('specs/evidence');m=MachO(inp/'binaries/Arc-1.168.0.arm64')
targets={0x1035b1f40:'attachToPaneHostView:',0x1035b6c00:'createPaneHostView',0x1035b8800:'detachFromPaneHostView:'}
for va,sel in targets.items():
 w,w2=struct.unpack('<II',m.vread(va,8));assert w&0x9f00001f==0x90000001;assert w2&0xffc003ff==0xf9400021
 imm=((w>>29)&3)|(((w>>5)&0x7ffff)<<2);imm=imm-(1<<21) if imm&(1<<20) else imm
 selref=(va&~4095)+(imm<<12)+(((w2>>10)&4095)*8);assert m.string(m.ptr(selref))==sel
starts=array.array('Q');ends=array.array('Q');names=[]
for l in open(inp/'app168/ghidra/functions.tsv'):
 a,b,size,n=l.rstrip('\n').split('\t',3);starts.append(int(a,16));ends.append(int(b,16));names.append(n)
rows=[]
for sec in m.sections:
 if sec['section']!='__text':continue
 for off in range(0,sec['size'],1024*1024):
  data=m.read(sec['file_offset']+off,min(1024*1024,sec['size']-off))
  for i,(w,) in enumerate(struct.iter_unpack('<I',data)):
   if w&0xfc000000 not in (0x94000000,0x14000000):continue
   pc=sec['va']+off+4*i;imm=w&0x3ffffff;imm=imm-(1<<26) if imm&(1<<25) else imm;dest=pc+4*imm
   if dest not in targets:continue
   j=bisect.bisect_right(starts,pc)-1
   rows.append(dict(image='Arc-1.168.0.arm64',call_va=hex(pc),call_file_offset=hex(m.offset(pc)),branch_word=hex(w),branch_kind='BL' if w&0xfc000000==0x94000000 else 'B_tail',selector_stub_va=hex(dest),selector=targets[dest],candidate_function_va=hex(starts[j]),candidate_function_name=names[j],within_saved_end_bound=pc<ends[j],receiver_class='unproven_selector_only',shard=f'0x{starts[j]>>20:x}.c'))
tsv(out/'app168-embedding-calls.tsv',list(rows[0]),rows)
print(json.dumps(rows,indent=2))

# Preserve only exact old app bodies for receiver-flow context; header+next-header delimiters.
entries={r['candidate_function_va'] for r in rows};p=inp/'app168/ghidra/decomp/0x1005.c';active=None;lines=[]
with open(p) as f:
 for line_no,line in enumerate(f,1):
  if line.startswith('// ==== '):
   va=line.split()[2];active=va if va in entries else None
  if active:lines.append(f'{line_no}: '+line)
(out/'app168-embedding-pseudocode.txt').write_text(''.join(lines))
