#!/usr/bin/env python3
"""Independent bounded re-checks of consequential ABI findings and bridges."""
import collections,hashlib,json,sqlite3,struct
from pathlib import Path
from audit import BASE,INPUT,tsv,load
from macho import MachO
from disasm import disasm
def main():
    b=MachO(INPUT/'binaries/ArcCore-1.170.0.arm64');app=MachO(INPUT/'binaries/Arc-1.168.0.arm64');db=sqlite3.connect(f'file:{INPUT}/corpus-index.sqlite?mode=ro',uri=True);db.row_factory=sqlite3.Row
    ptrs=[]
    for base,n,label in [(0xdd2bfb0,7,'downloads bridge;7 bounded slots'),(0xda89030,80,'UI bridge;80 bounded slots,extent not certified')]:
        for i in range(n):ptrs.append(dict(label=label,table_va=hex(base),slot_va=hex(base+i*8),target_va=hex(b.pointer(base+i*8))))
    tsv(BASE/'evidence/focus-vtables.tsv',ptrs)
    wanted={0x43e6548,0x43eb374,0x42b1200,0x42b13c4,0x43e6ee0,0x42b2430,0x4044e80,0x4044ec0,0x43e8368,0x42b3d74,0x42b3f34,0x43eb7a0,0x4041fec,0x43ed9a8,0x404257c,0x4042610,0x404269c,0x4042728,0x40428ac,0x4440dec,0x4375b40,0x4445a58,0x444112c,0x4375cdc,0x441cccc,0x441c680,0x441c08c,0x4415520,0x435b114,0x4415fe0,0x441604c,0x44160b8,0x441610c}
    # The actual table was independently read, and constructor stores this
    # table address. Discovery does not imply every bounded slot belongs to it.
    wanted|={int(r['target_va'],16) for r in ptrs if 0x4400000<=int(r['target_va'],16)<0x4450000}
    bodies=[]
    for va in sorted(wanted):
        r=db.execute('select * from functions where va=?',(va,)).fetchone();row=dict(va=hex(va),status='no_exact_exported_header')
        if r:
            with Path(r['path']).open('rb') as f:f.seek(r['offset']);data=f.read(min(32768,r['length']))
            dest=BASE/'bodies'/f'focus-{va:#x}.c';dest.write_bytes(data)
            row.update(name=r['name'],path=r['path'],offset=r['offset'],length=r['length'],extracted_length=len(data),sha256=hashlib.sha256(data).hexdigest(),failed=r['failed'],local=str(dest),status='truncated' if len(data)<r['length'] else 'complete_text_not_boundary_certificate')
        bodies.append(row)
    tsv(BASE/'evidence/focus-body-index.tsv',bodies)
    sels=[]
    for adrp,ldr in [(0x40427fc,0x4042800),(0x4445aa8,0x4445aac),(0x4447ec4,0x4447ec8)]:
        x=struct.unpack('<I',b.read(adrp,4))[0];y=struct.unpack('<I',b.read(ldr,4))[0]
        assert x&0x9f00001f==0x90000008 and y&0xffc003ff==0xf9400101
        imm=((x>>29)&3)|(((x>>5)&0x7ffff)<<2);imm=imm-(1<<21) if imm&(1<<20) else imm
        slot=(adrp&~4095)+(imm<<12)+((y>>10)&4095)*8;sv=b.pointer(slot)
        sels.append(dict(adrp_va=hex(adrp),ldr_va=hex(ldr),selector_ref_va=hex(slot),selector_string_va=hex(sv),selector=b.string(sv)))
    assert sels[0]['selector']=='browserContext:onDeterminingFilename:defaultFilename:completionTimeout:completion:'
    assert sels[1]['selector']=='webContents:decidePolicyForNavigationAction:'
    tsv(BASE/'evidence/focus-selectors.tsv',sels)
    ins=[]
    ranges=[(0x4042728,0x23c),(0x4445a58,0xa4),(0x4041fec,0x1c0),(0x4440dec,0x340),(0x43e6ee0,0xe8),(0x4044e80,0x60),(0x441cccc,0x98),(0x441c08c,0x8c),(0x43eb374,0x34),(0x44462cc,0x1a4),(0x4446470,0x198),(0x4446608,0x2e8),(0x4446944,0x1cc),(0x4446bc8,0x21c),(0x4447e88,0x74)]
    stubs=b.import_stubs()
    def resolve(target):
        for _ in range(5):
            if target in stubs:return stubs[target]
            a,c,d=struct.unpack('<III',b.read(target,12))
            if a&0xfc000000==0x14000000:
                imm=a&0x3ffffff;imm=imm-(1<<26) if imm&(1<<25) else imm;target+=imm*4;continue
            if a&0x9f00001f==0x90000010 and c&0xffc003ff==0x91000210 and d==0xd61f0200:
                imm=((a>>29)&3)|(((a>>5)&0x7ffff)<<2);imm=imm-(1<<21) if imm&(1<<20) else imm;target=(target&~4095)+(imm<<12)+((c>>10)&0xfff);continue
            return ''
        return ''
    for start,n in ranges:
        for va,raw,text in disasm(b,start,n):
            x=struct.unpack('<I',bytes.fromhex(raw))[0];target='';symbol=''
            if x&0xfc000000 in [0x14000000,0x94000000]:
                imm=x&0x3ffffff;imm=imm-(1<<26) if imm&(1<<25) else imm;target=va+imm*4;symbol=resolve(target)
            ins.append(dict(image='engine170',range_start=hex(start),va=hex(va),file_offset=hex(b.offset(va,4)),raw=raw,disasm=text,branch_target=hex(target) if target else '',import_symbol=symbol))
    by={int(r['va'],16):r['disasm'] for r in ins}
    assert 'ucvtf' in by[0x40427b0] and 'd8, w24' in by[0x40427b0]
    assert 'cmp' in by[0x4445ac0] and '#12' in by[0x4445ac0]
    assert 'cmp' in by[0x4446dd0] and '#3' in by[0x4446dd0]
    assert 'csel' in by[0x4446dd4] and 'lo' in by[0x4446dd4]
    tsv(BASE/'evidence/focus-instructions.tsv',ins)
    appcalls=[]
    for call in [0x1004aa8c8,0x1004af488,0x1004ae53c,0x10053b8e0,0x1004af094,0x1004af114]:
        word=struct.unpack('<I',app.read(call,4))[0];assert word&0xfc000000==0x94000000
        imm=word&0x3ffffff;imm=imm-(1<<26) if imm&(1<<25) else imm;stub=call+imm*4
        a,c=struct.unpack('<II',app.read(stub,8));assert a&0x9f00001f==0x90000001 and c&0xffc003ff==0xf9400021
        imm=((a>>29)&3)|(((a>>5)&0x7ffff)<<2);imm=imm-(1<<21) if imm&(1<<20) else imm;slot=(stub&~4095)+(imm<<12)+((c>>10)&4095)*8
        appcalls.append(dict(image='app168',call_va=hex(call),file_offset=hex(app.offset(call,4)),raw=app.read(call,4).hex(),stub_va=hex(stub),selector_ref_va=hex(slot),selector=app.string(app.pointer(slot)),limit='selector_dispatch;receiver_and_execution_not_proven_by_branch'))
    tsv(BASE/'evidence/focus-app-calls.tsv',appcalls)
    source=Path('/home/exedev/work/agents/abi/sources/objc4-objc-runtime-new.h')
    (BASE/'sources/objc-runtime-new.h').write_bytes(source.read_bytes())
    tsv(BASE/'evidence/primary-sources.tsv',[dict(revision='fb265098298302243cd7eeaa1f63f0ba7786dd9a',path=str(BASE/'sources'/file),sha256=hashlib.sha256((BASE/'sources'/file).read_bytes()).hexdigest(),url=f'https://github.com/apple-oss-distributions/objc4/blob/fb265098298302243cd7eeaa1f63f0ba7786dd9a/runtime/{file}',role=role) for file,role in [('objc-runtime-new.h','protocol_t,class_ro_t,method_list_t layout'),('objc-runtime-new.mm','getExtendedTypesIndexesForMethod:4885-4924;no cross-dump ordering')]])
    print(dict(focus_bodies=len(bodies),missing=[r['va'] for r in bodies if r['status']=='no_exact_exported_header'],instructions=len(ins)))
if __name__=='__main__':main()
