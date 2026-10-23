#!/usr/bin/env python3
"""Bounded random access to copied pseudocode by exact engine VA."""
import argparse
from pathlib import Path
import sqlite3

p=argparse.ArgumentParser(description=__doc__)
p.add_argument('addresses',nargs='+',type=lambda x:int(x,0))
p.add_argument('--max-bytes',type=int,default=32768)
a=p.parse_args()
assert 1<=a.max_bytes<=1048576, 'Use a bounded view, at most 1 MiB per function'
db=sqlite3.connect('file:/home/exedev/work/inputs/corpus-index.sqlite?mode=ro',uri=True)
for va in a.addresses:
 row=db.execute('SELECT name,path,offset,length,failed FROM functions WHERE va=?',(va,)).fetchone()
 if row is None:
  print(f'NO_EXPORTED_BODY 0x{va:x} (could be thunk/external, missing body or invalid entry)');continue
 name,path,offset,length,failed=row
 print(f'INDEX 0x{va:x} {name} path={path} offset={offset} C_bytes={length} failed={failed}')
 with Path(path).open('rb') as f:
  f.seek(offset);data=f.read(min(length,a.max_bytes))
 print(data.decode('utf-8','replace'))
 if length>a.max_bytes:print(f'TRUNCATED: viewed {a.max_bytes} of {length} pseudocode bytes; do not claim whole-body semantics')
