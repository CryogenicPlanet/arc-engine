"""Read-only bounded original-image extraction; writes only this evidence directory."""
from pathlib import Path
import sys,runpy,struct,hashlib,json,datetime
out=Path(__file__).resolve().parent
script=Path('/tmp/arccore-sessions-persistence.py')
inputs=[Path('results/sessions/native-evidence/discard-completion-capture.md'),Path('results/sessions/native-evidence/discard-registration-accounting.md'),Path('results/sessions/native-evidence/discard-registration-accounting-manifest.json'),Path('results/sessions/native-evidence/discard-registration-accounting-disassembly.txt'),Path('arc/api/arc_sessions_restore.mm'),script]
def pin(p):
 data=p.read_bytes();return {'sha256':hashlib.sha256(data).hexdigest(),'size':len(data)}
before={str(p):pin(p) for p in inputs}
sys.argv=[str(script),'none'];d=runpy.run_path(str(script));b=d['b'];fo=d['fileoff'];base=d['base']
binary=Path(d['path']);bp=pin(binary)
assert bp['sha256']=='b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d'
ranges=[('finalizer bridge',0x4446b10,0x4446b74),('member invoker',0x42c29d0,0x42c2a2c),('captured-registration delivery',0x42c0840,0x42c0980),('outer manager guard',0x42c1740,0x42c17e0),('bridge installation fields',0x444112c,0x44411e4),('weak-load thunk',0x84edd34,0x84edd40),('weak-load import stub',0xc4a5b14,0xc4a5b20),('message thunk',0x80045ec,0x80045f8),('message import stub',0xc4a5a90,0xc4a5a9c),('release thunk',0x8003ddc,0x8003de8),('release import stub',0xc4a5a9c,0xc4a5aa8)]
text=[];rp=[]
for label,start,end in ranges:
 raw=bytes(b[fo(start):fo(start)+end-start]);rp.append({'name':label,'start':hex(start),'endExclusive':hex(end),'sha256':hashlib.sha256(raw).hexdigest()})
 text.append(f'## {label} [{start:x},{end:x})')
 text.extend(f'{i.address:08x}: {i.mnemonic:8} {i.op_str}' for i in d['md'].disasm(raw,start));text.append('')
(out/'original-disassembly.txt').write_text('\n'.join(text))
cur=base+32
for _ in range(d['ncmd']):
 cmd,size=struct.unpack_from('<II',b,cur)
 if cmd==0x80000034:
  off,sz=struct.unpack_from('<II',b,cur+8);fix=base+off;h=struct.unpack_from('<7I',b,fix);imports=fix+h[2];symbols=fix+h[3];assert h[5]==1
 cur+=size
facts={'vtableSlot':{},'selector':{},'imports':[]}
for name,va in [('vtableSlot',0xda890e0),('selector',0xe291310)]:
 raw=struct.unpack_from('<Q',b,fo(va))[0]; target=raw&0xfffffffff
 facts[name]={'address':hex(va),'raw':hex(raw),'target':hex(target)}
 if name=='selector':facts[name]['value']=d['cstring'](target)
for va in [0xd824118,0xd824000,0xd8240c0]:
 raw=struct.unpack_from('<Q',b,fo(va))[0];assert raw>>63;ordinal=raw&0xffffff;rec=struct.unpack_from('<I',b,imports+ordinal*4)[0];name=symbols+(rec>>9)
 facts['imports'].append({'address':hex(va),'raw':hex(raw),'ordinal':ordinal,'name':bytes(b[name:b.find(b'\0',name)]).decode()})
assert facts['vtableSlot']['target']=='0x4446b10'
assert facts['selector']['value']=='webContents:finalizeDiscardRequest:'
(out/'mapping.json').write_text(json.dumps(facts,indent=2)+'\n')
after={str(p):pin(p) for p in inputs};assert before==after
manifest={'createdUTC':datetime.datetime.now(datetime.timezone.utc).isoformat(),'scope':'Static bridge policy and bounded known invokers only; no nil-owner reachability/runtime or Root activation claim.','sourceBinary':{'path':str(binary),**bp},'inputs':before,'allInputsUnchangedAfterExtraction':True,'ranges':rp,'outputs':{p.name:pin(p) for p in [out/'original-disassembly.txt',out/'mapping.json']}}
(out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
print(json.dumps({'sourceSHA':before['arc/api/arc_sessions_restore.mm']['sha256'],'sourceBinarySHA':bp['sha256'],'frozenInputsUnchanged':True,'mapping':facts},indent=2))
