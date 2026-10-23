#!/usr/bin/env python3
"""Read exact corpus ranges, save only beneath this role, record review selections."""
import sys,sqlite3,json
from pathlib import Path
out=Path(__file__).resolve().parents[1];(out/'bodies').mkdir(exist_ok=True)
db=sqlite3.connect('file:/home/exedev/work/inputs/corpus-index.sqlite?mode=ro',uri=True)
for arg in sys.argv[1:]:
 va=int(arg,16);r=db.execute('select name,path,offset,length,failed from functions where va=?',(va,)).fetchone()
 if not r:print('NO EXACT ENTRY',hex(va));continue
 name,path,offset,length,failed=r
 assert length<100000 and not failed
 with open(path,'rb') as f:f.seek(offset);b=f.read(length)
 (out/'bodies'/f'{va:x}.c').write_bytes(b)
 with open(out/'body-read-log.jsonl','a') as f:f.write(json.dumps(dict(va=hex(va),name=name,path=path,offset=offset,length=length))+'\n')
 print(b.decode())
