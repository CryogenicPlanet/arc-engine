import sys
sys.dont_write_bytecode=True
import csv,json,struct,subprocess,importlib.util,collections,re
from pathlib import Path
ROOT=Path('/home/exedev/work/inputs');out=Path('specs')
s=importlib.util.spec_from_file_location('inv','../arc-layer/scripts/inventory.py');m=importlib.util.module_from_spec(s);s.loader.exec_module(m)
mach=m.MachO(ROOT/'binaries/ArcCore-1.170.0.arm64')
meta=json.loads((out/'evidence/metadata.json').read_text());imp={int(x['imp'],16) for x in meta['methods']}
rows=[]
for line in (ROOT/'metadata/decomp/arccore-1.170.0-functions.tsv').open():
 a=line.rstrip().split('\t')
 if int(a[0],16) in imp:rows.append(dict(zip(['va','end_reported','body_bytes_reported','ghidra_name'],a)))
m.tsv(out/'evidence/ghidra-imp-rows.tsv',list(rows[0]),rows)
vt=[]
for base in [0xda882d0,0xda87798]:
 for i in range(8):
  va=mach.ptr(base+i*8);vt.append({'table_va':hex(base),'slot':i,'slot_va':hex(base+i*8),'target_va':hex(va)})
(out/'evidence/vtable-prefixes.json').write_text(json.dumps(vt,indent=2))
requests={int(x['target_va'],16) for x in vt}|{0x4407b5c,0x4337e00,0x43c8b18,0x4405188,0x473dff4,0x473e20c,0x432dadc,0x432de10,0x432e02c,0x4402548,0x443181c,0x43359c8,0x4335ae8}
for va in sorted(requests):
 p=out/'bodies'/f'{va:08x}.txt'
 if p.exists():continue
 with p.open('w') as f:subprocess.run(['python3','/home/exedev/work/read-function.py',hex(va)],stdout=f,check=True)
# Verify actual BL instruction/target independently; do not infer receiver class from selectors.
app=m.MachO(ROOT/'binaries/Arc-1.168.0.arm64');ar=[]
for x in csv.DictReader(open('../arc-layer/initial/app168-launch-calls.tsv'),delimiter='\t'):
 if x['receiver_association']!='ArcBrowserApplication':continue
 va=int(x['call_va'],16);word=struct.unpack('<I',app.vread(va,4))[0]
 assert word>>26==0x25 and word==int(x['bl_word'],16)
 disp=word&0x3ffffff;disp=disp-(1<<26) if disp&(1<<25) else disp
 assert va+disp*4==int(x['stub_va'],16)
 x['verification']='pristine app instruction and decoded target agree; receiver association remains prior pseudocode inference'
 ar.append(x)
m.tsv(out/'evidence/app-launch-calls.tsv',list(ar[0]),ar)
print('Ghidra exact IMP starts',len(rows),'vtable prefix slots',len(vt),'app BLs',len(ar))
