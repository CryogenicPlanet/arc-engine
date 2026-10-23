#!/usr/bin/env python3
"""Bounded indexed body views with independently saved raw snapshots and anchors."""
import hashlib,json,re,sqlite3,sys
from pathlib import Path
out=Path(__file__).resolve().parents[1]/'evidence/body-views';out.mkdir(exist_ok=True)
db=sqlite3.connect('file:/home/exedev/work/inputs/corpus-index.sqlite?mode=ro',uri=True);db.row_factory=sqlite3.Row
for arg in sys.argv[1:]:
    va=int(arg,0);e=db.execute('select * from functions where va=?',(va,)).fetchone()
    if not e:print('NO EXACT ENTRY',hex(va));continue
    e=dict(e);assert e['length']<=100000, 'request a smaller explicit view'
    with open(e['path'],'rb') as f:f.seek(e['offset']);b=f.read(e['length'])
    e['sha256']=hashlib.sha256(b).hexdigest();(out/(hex(va)+'.json')).write_text(json.dumps(e,indent=2)+'\n');(out/(hex(va)+'.c')).write_bytes(b)
    s=b.decode();print('\n'+s.splitlines()[0]+' '+e['path']+'@'+str(e['offset'])+'+'+str(e['length']))
    # Display only executable-statement portion, but preserve full source on disk.
    m=re.search(r'\n\{\n',s)
    print(s[m.end():] if m else s)
