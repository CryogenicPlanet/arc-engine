"""Bounded, explicitly selected bridge snapshots via immutable indexed reader."""
import subprocess,json
from pathlib import Path
vas='42b17e8 42b1ba8 42b2430 42b32d0 42b3804 42b3d74 42b6394 42fa5c8 42fa9fc 42faa84 42fbc80 42fbd80 42faed0 42fb100 4366ac0 4367748 4367a60 4367d74 43680fc 4333598 4333650 4354de4 4354ea0 4335e08 43363b4 432b4b8 432c5e4 430ddd0 430dbcc 433edd0 433ee60 433ef10 4337b04 4338920 443181c 43319d8 4331ae4 42affa0 42b00f4 4333fe4 4334078 433410c 42cba00 43eb534 43eb730 43ec404 437b014 4332a34 42bdee4'.split()
for s in vas:
 va=int(s,16);p=Path(f'specs/bodies/{va:08x}.txt')
 if p.exists():continue
 with p.open('w') as f:subprocess.run(['python3','/home/exedev/work/read-function.py',hex(va)],stdout=f,check=True)
print(len(vas),'explicit bridge requests')
