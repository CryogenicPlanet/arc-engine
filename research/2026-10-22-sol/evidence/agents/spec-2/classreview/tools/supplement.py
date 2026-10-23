#!/usr/bin/env python3
"""Supplement independent raw metadata/citation checks; imports audit in this directory."""
from audit import *
layout=json.loads((W/'evidence/metadata.json').read_text())['classes'];out=[]
for cl in sorted(classes):
 a=classvas[cl];r=layout[cl];ro=im.ptr(a+32)&~7
 assert int(r['metadata_va'],16)==a and int(r['ro_va'],16)==ro
 assert (r['instance_start'],r['instance_size'])==struct.unpack('<II',im.data(ro+4,8))
 for field,ptr_off in [('ivars',48),('properties',64)]:
  p=im.ptr(ro+ptr_off);items=[]
  if p:
   stride,n=struct.unpack('<II',im.data(p,8))
   for i in range(n):
    q=p+8+i*stride
    if field=='ivars':
     off=struct.unpack('<I',im.data(im.ptr(q),4))[0];nm=im.string(im.ptr(q+8));ty=im.string(im.ptr(q+16));size=struct.unpack('<I',im.data(q+28,4))[0]
     items.append(dict(record_va=hex(q),offset=off,name=nm,type=ty,size=size))
    else:items.append(dict(record_va=hex(q),name=im.string(im.ptr(q)),attributes=im.string(im.ptr(q+8))))
  assert items==r[field],(cl,field)
 text=(W/'classes'/f'{cl}.md').read_text()
 for v in r['ivars']:
  assert f"| {v['name']} | {v['offset']} | {v['size']} | `{v['type']}` | {v['record_va']} |" in text
 for v in r['properties']:
  assert f"| {v['name']} | `{v['attributes']}` | {v['record_va']} |" in text
 out.append(dict(class_name=cl,metadata_va=hex(a),ro_va=hex(ro),instance_size=r['instance_size'],ivars=len(r['ivars']),properties=len(r['properties']),result='PASS binary/layout JSON/class prose'))
table(O/'evidence/layout-checks.tsv',out)
# Decode bounded vtable prefixes, keeping absent exact targets absent.
out=[]
for r in json.loads((W/'evidence/vtable-prefixes.json').read_text()):
 a=int(r['slot_va'],16);t=im.ptr(a);assert t==int(r['target_va'],16)
 b=db.execute('select name,path,offset,length,failed from functions where va=?',(t,)).fetchone()
 out.append(dict(slot_va=hex(a),target_va=hex(t),file_offset=hex(im.off(a)),indexed_name=b[0] if b else '',exact_body_status='present' if b else 'NO_EXACT_INDEXED_BODY'))
table(O/'evidence/vtable-checks.tsv',out)
# All prose numeric citations: classify text exact-entry versus data/metadata/app images.
cites=[];backend=set()
textva,textn=im.secs['__text']
for cl in sorted(classes):
 text=(W/'classes'/f'{cl}.md').read_text()
 for token in sorted(set(re.findall(r'0x[0-9a-f]{6,}',text))):
  a=int(token,16);b=db.execute('select name,path,offset,length,failed from functions where va=?',(a,)).fetchone()
  if b:
   status='exact engine C header';backend.add(a)
  elif a>=0x100000000:status='separate app image citation'
  elif textva<=a<textva+textn:status='engine text; NO_EXACT_INDEXED_BODY'
  else:
   try:im.off(a);status='mapped engine data/metadata; no C header required'
   except ValueError:status='UNMAPPED'
  cites.append(dict(class_name=cl,cited_va=token,resolution=status,indexed_name=b[0] if b else ''))
table(O/'evidence/citations.tsv',cites)
# Also save bounded exact callees read during behavioral review, including methods not in prose.
backend|={int(x,16) for x in '42fa5c8 43e33c4 42fa9fc 43c8b18 42faa84 437b014 4335e08 432b4b8 4407b5c 42b2430 42b3d74 4354de4 473dff4 4354ea0 473e20c 44193c0 43680fc 4338920 4337e00 4402548 473f138 433edd0 433ee60 433ef10 441daa0 4332040 443181c 4335ae8 43359c8 42affa0 42b00f4 4366ac0 42fbd80 43363b4 42b1ba8'.split()}
(O/'backend').mkdir(exist_ok=True);bs=[]
for a in sorted(backend-set(bodies)):
 b=db.execute('select name,path,offset,length,failed from functions where va=?',(a,)).fetchone()
 if not b:continue
 name,path,off,n,failed=b
 with open(path,'rb') as f:f.seek(off);data=f.read(min(n,32768))
 (O/'backend'/f'{a:08x}.c').write_bytes(data)
 bs.append(dict(va=hex(a),name=name,path=path,byte_offset=off,total_C_bytes=n,saved_C_bytes=len(data),truncated=n>len(data),sha256_saved=hashlib.sha256(data).hexdigest()))
table(O/'evidence/backend-snapshots.tsv',bs)
# Reproduce writer status totals; status presence is not semantic correctness.
counts={k:dict(collections.Counter(r[k] for r in rows)) for k in ['body_status','semantic_status','confidence']}
assert counts['body_status']==coverage['body_status_counts']
assert counts['semantic_status']==coverage['semantic_status_counts']
(O/'evidence/status-counts.json').write_text(json.dumps(counts,indent=2)+'\n')
# Read/verify the four app BL words in the separate executable, without receiver inference.
app=Image(ROOT/'binaries/Arc-1.168.0.arm64');apps=[]
for r in csv.DictReader((W/'evidence/app-launch-calls.tsv').open(),delimiter='\t'):
 a=int(r['call_va'],16);word=struct.unpack('<I',app.data(a,4))[0]
 assert word==int(r['bl_word'],16) and word>>26==0b100101
 disp=word&0x3ffffff;disp=disp-(1<<26) if disp&(1<<25) else disp
 assert a+disp*4==int(r['stub_va'],16)
 assert app.off(a)==int(r['call_file_offset'],16)
 apps.append(dict(**r,independent_check='PASS BL word/target and translated offset; receiver type remains inferred'))
(O/'evidence/app-launch-input.json').write_text(json.dumps(apps,indent=2)+'\n')
print('PASS 37 binary layouts; 16 vtable slots; all numeric prose VAs classified; status totals reproduced')
