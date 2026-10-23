import sqlite3,json,sys,re
from pathlib import Path
out=Path(__file__).resolve().parents[1];d=sqlite3.connect('file:/home/exedev/work/inputs/corpus-index.sqlite?mode=ro',uri=True)
p=out/'evidence/bridges.json';allrows=json.load(open(p)) if p.exists() else {}
for arg in sys.argv[1:]:
 va=int(arg,16);res=d.execute('select name,path,offset,length,failed from functions where va=?',(va,)).fetchone()
 if not res:print('MISSING',hex(va));continue
 name,path,offset,length,failed=res
 with open(path,'rb') as f:f.seek(offset);raw=f.read(min(length,32768))
 s=raw.decode('utf-8','replace');(out/'bodies'/f'{va:x}.c').write_text(s)
 allrows[hex(va)]={'name':name,'path':path,'offset':offset,'length':length,'failed':failed,'truncated':length>32768}
 print('\n###',hex(va),name,'length',length,'failed',failed)
 for line in s.splitlines():
  if any(x in line for x in ['FUN_','"','_objc_','return ']) and not any(x in line for x in ['WARNING','Stack Size','00f01bc0','013efb40','00005810','00008790','_objc_retain_fixup','_objc_release_fixup','Function:']):print(line)
json.dump(allrows,open(p,'w'),indent=2)
