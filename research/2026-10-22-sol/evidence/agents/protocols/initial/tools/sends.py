#!/usr/bin/env python3
"""Stream immutable corpus and corroborate bounded selector sends in binary.

Source matches alone are candidates. Binary sends require a tracked x1 load of
an actual selector and a direct branch (possibly through an island) to msgSend.
Only straight-line constant propagation; failures remain unknown. No execution.
"""
import collections,csv,hashlib,json,re,sqlite3,struct
from pathlib import Path
from audit import BASE,INPUT,tsv,load,sha
from macho import MachO
HDR=re.compile(rb'^// ==== (0x[0-9a-f]+)  (.+)  \((\d+) bytes\)')
SEND=re.compile(r'\b_objc_msgSend\s*\(\s*([^,;]+),\s*"([^"\n]+)"')
def scan():
    selectors={r['selector'] for r in load(BASE/'evidence/guide-identities.tsv')};rows=[];shards=[];headers=0
    for manifest in [INPUT/'decompile/MANIFEST.json',*sorted((INPUT/'decompile-extra').glob('batch-*/MANIFEST.json'))]:
        assert (manifest.parent/'READY').exists()
        for r in json.loads(manifest.read_text()):
            path=manifest.parent/r['file'];digest=hashlib.sha256();offset=0;h=None;window='';window_line=0;prev='';n=0
            with path.open('rb') as f:
                for ln,raw in enumerate(f,1):
                    digest.update(raw);n+=len(raw);m=HDR.match(raw)
                    if m:
                        headers+=1;h=dict(engine_va=m[1].decode(),engine_name=m[2].decode(),reported_bytes=int(m[3]),path=str(path),header_line=ln,header_offset=offset);window='';prev=''
                    elif h:
                        line=raw.decode('utf8','replace');prefix=len(window);text=window+line
                        for m in (SEND.finditer(text) if '_objc_msgSend' in text else ()):
                            if m.end()<=prefix or m[2] not in selectors:continue
                            posline=ln-text[m.start():].count('\n')+line.count('\n')
                            rows.append(dict(**h,selector=m[2],send_source_line=posline,receiver_expression=m[1],context=text[max(0,m.start()-100):min(len(text),m.end()+180)],status='pseudocode_selector_send;receiver_and_execution_unproven',boundary_limit='known_merged_reject' if h['engine_va']=='0x421f778' else 'export_boundary_not_certified'))
                        window=text[-2048:]
                    offset+=len(raw)
            assert n==r['bytes'] and digest.hexdigest()==r['sha256'],str(path)
            shards.append(dict(path=str(path),sha256=digest.hexdigest(),bytes=n,manifest=str(manifest)))
    assert len(shards)==199 and headers==567959
    tsv(BASE/'evidence/engine-source-sends.tsv',rows)
    tsv(BASE/'evidence/shards.tsv',shards)
    print(dict(headers=headers,shards=len(shards),source_rows=len(rows),selectors=len({r['selector'] for r in rows})),flush=True)

def corroborate():
    rows=load(BASE/'evidence/engine-source-sends.tsv');wanted={int(r['engine_va'],16) for r in rows if 0x4000000<=int(r['engine_va'],16)<0x4500000};meta={};nexts={};prior=None
    for ln,line in enumerate((INPUT/'metadata/decomp/arccore-1.170.0-functions.tsv').open(),1):
        a,e,size,name=re.split(r'\\t|\t',line.rstrip('\n'));a=int(a,16)
        if prior in wanted:nexts[prior]=a
        if a in wanted:meta[a]=(int(e,16),int(size),name,ln)
        prior=a
    b=MachO(INPUT/'binaries/ArcCore-1.170.0.arm64');stubs=b.import_stubs();resolve_cache={}
    def resolve(a):
        if a in resolve_cache:return resolve_cache[a]
        original=a
        for _ in range(5):
            if a in stubs:break
            try:x,y,z=struct.unpack('<III',b.read(a,12))
            except ValueError:break
            if x&0xfc000000==0x14000000:
                imm=x&0x3ffffff;imm=imm-(1<<26) if imm&(1<<25) else imm;a+=imm*4;continue
            if x&0x9f00001f==0x90000010 and y&0xffc003ff==0x91000210 and z==0xd61f0200:
                imm=((x>>29)&3)|(((x>>5)&0x7ffff)<<2);imm=imm-(1<<21) if imm&(1<<20) else imm;a=(a&~4095)+(imm<<12)+((y>>10)&0xfff);continue
            break
        result=(a,stubs.get(a,''));resolve_cache[original]=result;return result
    selectors={r['selector'] for r in load(BASE/'evidence/guide-identities.tsv')};sends=[];ranges=[];instructions=[]
    for start in sorted(wanted)+[-1]:
        region=start==-1
        if region:start=0x4000000;end=0x4500000;size=0;name='Arc_area_binary_scan_no_function_attribution';ln=''
        else:end,size,name,ln=meta[start]
        nxt=end if region else nexts.get(start,end);limit=min(end,nxt);length=limit-start
        # Strict upper bound from both saved end and next start. Does not certify
        # contiguous body membership; source match remains necessary.
        if not (0<length<=32768 or region):
            ranges.append(dict(start=hex(start),end=hex(limit),status='skipped_untrusted_or_large_span'));continue
        code=b.read(start,length);regs={};local=[]
        for off in range(0,length-3,4):
            va=start+off;x=struct.unpack_from('<I',code,off)[0];rd=x&31;rn=(x>>5)&31
            if x&0xfc000000 in (0x94000000,0x14000000):
                imm=x&0x3ffffff;imm=imm-(1<<26) if imm&(1<<25) else imm;target=va+imm*4;stub,sym=resolve(target)
                if sym=='_objc_msgSend' and 1 in regs:
                    val,slot,origin=regs[1]
                    try:sel=b.string(val)
                    except (ValueError,AssertionError):sel=''
                    if sel in selectors:
                        local.append(dict(engine_va='' if region else hex(start),engine_name=name,selector=sel,send_va=hex(va),branch_target=hex(target),stub_va=hex(stub),selector_ref_va=hex(slot) if slot else '',selector_string_va=hex(val),selector_load_va=hex(origin),evidence='binary_x1_selector+msgSend_branch;'+('region_scan_no_source_required' if region else 'source_candidate'),receiver_limit='dynamic_receiver_not_proven;not_runtime_invocation',metadata_line=ln))
                if x&0xfc000000==0x94000000:regs={k:v for k,v in regs.items() if k>=19}
                else:regs={}
                continue
            if x&0x9f000000 in (0x90000000,0x10000000):
                imm=((x>>29)&3)|(((x>>5)&0x7ffff)<<2);imm=imm-(1<<21) if imm&(1<<20) else imm
                val=((va&~4095)+(imm<<12)) if x&0x80000000 else va+imm;regs[rd]=(val,0,va);continue
            if x&0xffc00000==0x91000000:
                if rn in regs:regs[rd]=(regs[rn][0]+((x>>10)&4095),0,va)
                else:regs.pop(rd,None)
                continue
            if x&0xffc00000==0xf9400000:
                if rn in regs:
                    slot=regs[rn][0]+(((x>>10)&4095)*8)
                    try:regs[rd]=(b.pointer(slot),slot,va)
                    except (ValueError,AssertionError):regs.pop(rd,None)
                else:regs.pop(rd,None)
                continue
            if x&0xffe0ffe0==0xaa0003e0:
                rm=(x>>16)&31
                if rm in regs:regs[rd]=regs[rm]
                else:regs.pop(rd,None)
                continue
            # Conditional control flow and all unknown writes fail closed.
            if x&0xff000010==0x54000000 or x&0x7e000000 in (0x34000000,0x36000000):regs={};continue
            # Stores do not clobber tracked register values. Other forms may
            # produce false negatives; preserving unknown destinations is unsafe.
            if x&0x3b000000==0x39000000 and not x&0x00400000:continue
            if x&0xfffffc1f in (0xd65f0000,0xd61f0000,0xd63f0000):regs={};continue
            regs.pop(rd,None)
            # Paired loads can also write Rt2. Conservatively forget that field
            # for every unknown instruction to avoid retaining a stale selector.
            regs.pop((x>>10)&31,None)
        # Retain only selector matches supported by this same source function.
        source_selectors={r['selector'] for r in rows if int(r['engine_va'],16)==start}
        local=[r for r in local if region or r['selector'] in source_selectors];sends.extend(local)
        if local and not region:
            dest=BASE/'bodies'/f'engine-{start:#x}.bin';dest.write_bytes(code)
            ranges.append(dict(start=hex(start),end=hex(limit),length=length,file_offset=hex(b.offset(start,length)),sha256=hashlib.sha256(code).hexdigest(),local=str(dest),status='bounded_binary_candidate;min_end_not_body_map',sends=len(local)))
    dedup={}
    for r in sends:
        key=(r['send_va'],r['selector'])
        if key not in dedup:dedup[key]=r
    sends=list(dedup.values())
    tsv(BASE/'evidence/engine-binary-sends.tsv',sends)
    tsv(BASE/'evidence/engine-binary-ranges.tsv',ranges)
    print(dict(binary_sends=len(sends),binary_selectors=len({r['selector'] for r in sends}),source_candidate_functions=len(wanted)),flush=True)

def extracts():
    db=sqlite3.connect(f'file:{INPUT}/corpus-index.sqlite?mode=ro',uri=True);db.row_factory=sqlite3.Row
    binary=load(BASE/'evidence/engine-binary-sends.tsv');source=load(BASE/'evidence/engine-source-sends.tsv')
    # Preserve complete bounded bodies for Arc-area send candidates, including
    # failed binary corroboration; large bodies are explicit truncations.
    wanted={int(r['engine_va'],16) for r in source if 0x4000000<=int(r['engine_va'],16)<0x4500000};evidence=[]
    for va in sorted(wanted):
        r=db.execute('select * from functions where va=?',(va,)).fetchone();assert r
        with Path(r['path']).open('rb') as f:f.seek(r['offset']);data=f.read(min(r['length'],32768))
        dest=BASE/'bodies'/f'engine-{va:#x}.c';dest.write_bytes(data)
        evidence.append(dict(va=hex(va),name=r['name'],path=r['path'],offset=r['offset'],length=r['length'],extracted_length=len(data),failed=r['failed'],sha256=hashlib.sha256(data).hexdigest(),local=str(dest),status='truncated' if len(data)<r['length'] else 'complete_export_text;boundaries_unverified'))
    tsv(BASE/'evidence/engine-body-index.tsv',evidence)
if __name__=='__main__':scan();corroborate();extracts()
