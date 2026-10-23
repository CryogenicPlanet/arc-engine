#!/usr/bin/env python3
"""Stream app text once, preserving exact bounded callback-body byte ranges."""
import collections,hashlib,re
from audit import BASE,INPUT,load,tsv
H=re.compile(rb'^// ==== (0x[0-9a-f]+)  (.+)  \((\d+) bytes\)')
def main():
    guide={(r['protocol_name'],r['kind'],r['selector']) for r in load(BASE/'evidence/guide-identities.tsv')}
    impl=[r for r in load(BASE/'evidence/app168-implementations.tsv') if any((p,r['kind'],r['selector']) in guide for p in r['protocols'].split(';'))]
    wanted={int(r['imp'],16) for r in impl};rows=[]
    # These app helpers are explicitly reviewed for completion forwarding and
    # executor expectations; they are not declared protocol implementations.
    wanted.update([0x1004e9ac4,0x1004e9da8,0x10053605c,0x100536028,0x1004ec75c,0x1004f2640,0x1004f2a24,0x1004af380,0x1004ae4a0,0x1004e530c])
    for path in sorted((INPUT/'app168/ghidra/decomp').glob('*.c')):
        current=None;data=bytearray();offset=0;length=0
        def flush():
            if current is None:return
            dest=BASE/'bodies'/f"app-{current['va']}.c";dest.write_bytes(data)
            rows.append(dict(**current,path=str(path),length=length,extracted_length=len(data),sha256=hashlib.sha256(data).hexdigest(),local=str(dest),status='truncated' if length>len(data) else 'complete_export_text;boundaries_unverified'))
        with path.open('rb') as f:
            for ln,raw in enumerate(f,1):
                m=H.match(raw)
                if m:
                    flush();current=None;data=bytearray();length=0
                    if int(m[1],16) in wanted:current=dict(va=m[1].decode(),name=m[2].decode(),offset=offset,header_line=ln,reported_bytes=int(m[3]))
                if current:
                    length+=len(raw)
                    if len(data)<32768:data.extend(raw[:32768-len(data)])
                offset+=len(raw)
            flush()
    found={int(r['va'],16) for r in rows}
    from macho import MachO
    image=MachO(INPUT/'binaries/Arc-1.168.0.arm64')
    for va in sorted(wanted-found):
        raw=image.read(va,4)
        import struct
        word=struct.unpack('<I',raw)[0];target=''
        if word&0xfc000000==0x14000000:
            imm=word&0x3ffffff;imm=imm-(1<<26) if imm&(1<<25) else imm;target=hex(va+imm*4)
        rows.append(dict(va=hex(va),status='no_exact_exported_app_header;binary_branch_veneer' if target else 'no_exact_exported_app_header',instruction_bytes=raw.hex(),file_offset=hex(image.offset(va,4)),branch_target=target))
    tsv(BASE/'evidence/app-body-index.tsv',rows)
    print(dict(app_callback_implementations=len(impl),unique_callback_imps=len({r['imp'] for r in impl}),extracted=len(found),missing=len(wanted-found),truncated=sum(r['status']=='truncated' for r in rows)))
if __name__=='__main__':main()
