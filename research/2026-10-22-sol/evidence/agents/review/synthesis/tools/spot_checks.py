#!/usr/bin/env python3
"""Bounded independent synthesis checks. Only writes to this role's output."""
import ast, collections as C, csv, hashlib, json, os, re, sqlite3, struct, subprocess
from pathlib import Path

OUT=Path(__file__).resolve().parents[1]; WORK=OUT.parents[2]; I=WORK/'inputs'; A=WORK/'agents'
os.sched_setaffinity(0,{min(os.sched_getaffinity(0))})
# Reuse only our own bounded reader definition, with no audit top-level execution.
tree=ast.parse((OUT/'tools/audit.py').read_text())
exec(compile(ast.Module(body=[n for n in tree.body if isinstance(n,ast.ClassDef) and n.name=='Image'],type_ignores=[]),'<own Image>','exec'))
def rows(p):
    with Path(p).open() as f:return list(csv.DictReader(f,delimiter='\t'))
def sha(p):
    h=hashlib.sha256()
    with Path(p).open('rb') as f:
        for b in iter(lambda:f.read(1048576),b''):h.update(b)
    return h.hexdigest()
def write(name,rs):
    with (OUT/'evidence'/name).open('w') as f:
        w=csv.DictWriter(f,list(rs[0]),delimiter='\t');w.writeheader();w.writerows(rs)
def branch(b,a):
    w=struct.unpack('<I',b.read(a,4))[0]; assert w&0xfc000000 in (0x14000000,0x94000000)
    imm=w&0x3ffffff; imm=imm-(1<<26) if imm&(1<<25) else imm
    return a+4*imm

b=Image(I/'binaries/ArcCore-1.170.0.arm64'); old=Image(I/'binaries/ArcCore-1.168.0.arm64')
pins=[]
for name in ('Arc-1.168.0.arm64','ArcCore-1.168.0.arm64','ArcCore-1.170.0.arm64','ChromiumFramework-156.arm64','libaperitif.dylib'):
    p=I/'binaries'/name;pins.append(dict(image=name,bytes=p.stat().st_size,sha256=sha(p)))
write('image-pins.tsv',pins)
assert pins[2]['sha256']=='69276adaca785a46f2554e7fd18926e7fcdd6aba44ba64400a337427817db3dd'
instructions=[]
for path,va_col,raw_col,off_col in [(A/'abi/decompile/binary-instructions.tsv','instruction_va','bytes','file_offset'),(A/'protocols/initial/evidence/focus-instructions.tsv','va','raw','file_offset')]:
    rr=rows(path)
    for r in rr:
        a=int(r[va_col],16); raw=bytes.fromhex(r[raw_col]); assert b.read(a,len(raw))==raw
        assert b.offset(a,len(raw))==int(r[off_col],16)
    instructions.append(dict(path=str(path),records=len(rr),status='raw_bytes+mapped_offsets_equal'))
write('instruction-table-checks.tsv',instructions)
assert branch(b,0x4410bb4)==0x47390dc and branch(b,0x441d770)==0x2e0238
assert struct.unpack('<2I',b.read(0x2d0fb40,8))==(0xf9400400,0xd65f03c0)
assert b.read(0x15ac0,4)==bytes.fromhex('c0035fd6')
assert b.read(0x4445ac0,4)==bytes.fromhex('1f3000f1') # cmp x0,#12
assert b.read(0x4446dcc,12)==bytes.fromhex('680080523f0c00f12130889a')
eh=json.loads((A/'abi/decompile/ehframe-unwind.json').read_text())
for field,address in [('cie_bytes','cie'),('fde_bytes','fde'),('lsda_bytes','lsda')]:
    raw=bytes.fromhex(eh[field]); assert b.read(int(eh[address],16),len(raw))==raw
assert b.ptr(int(eh['personality_pointer_slot'],16))==int(eh['personality'],16)
checks=[]
for r in rows(I/'context/parent-encoding-check.tsv'):
    im=old if '1.168' in r['image'] else b
    m=int(r['method_entry'],16); ds,dt,di=struct.unpack('<iii',im.read(m,12))
    sv=m+ds
    if sv!=int(r['selector_va'],16):sv=im.ptr(sv)
    assert (sv,m+4+dt,m+8+di)==tuple(int(r[k],16) for k in ('selector_va','type_va','imp'))
    assert im.string(sv)==r['selector'] and im.string(m+4+dt)==r['encoding']
    checks.append(dict(**r,independent_result='PASS;entry_bytes_only;class_attribution_from_full_protocol_metadata_parser'))
write('older-entry-checks.tsv',checks)

db=sqlite3.connect(f'file:{I}/corpus-index.sqlite?mode=ro',uri=True)
taxonomy=C.Counter(); failures=[]
for va,path,off,n in db.execute('select va,path,offset,length from functions where failed=1'):
    assert n<=32768
    with open(path,'rb') as f:f.seek(off);data=f.read(n).decode()
    kind=next((s for s in ['process: timeout','Unable to find unique hash for varnode','Trying to construct memory range beyond end of address space','Cannot properly adjust input varnodes','Forced merge caused intersection'] if s in data),None)
    assert kind,(hex(va),data);taxonomy[kind]+=1
    failures.append(dict(va=hex(va),path=path,offset=off,length=n,error=kind))
write('failure-taxonomy.tsv',failures)

handoff=[]
for line in (I/'claude-handoff/results/arc-layer-functions.tsv').open():
    name,depth,reason,size=line.rstrip('\n').split('\t');handoff.append((name,depth,reason,int(size)))
wanted={r[0] for r in handoff}; candidates=C.defaultdict(list); metadata_count=span_sum=overlap=0; maximum=0
for line in (I/'metadata/decomp/arccore-1.170.0-functions.tsv').open():
    start,end,span,name=line.rstrip('\n').split('\t'); a=int(start,16); z=int(end,16)
    metadata_count+=1;assert z-a==int(span); span_sum+=int(span);overlap+=a<maximum;maximum=max(maximum,z)
    norm=name.rstrip(':')
    if norm in wanted:candidates[norm].append((a,name))
assert metadata_count==607294 and span_sum==858548775 and overlap==579666
expanded=[]; exported_names=C.Counter()
for name,_,_,_ in handoff:
    for a,exact in candidates[name]:
        rec=db.execute('select path,offset,length from functions where va=?',(a,)).fetchone()
        if rec:exported_names[name]+=1
        expanded.append(dict(normalized_name=name,metadata_va=hex(a),exact_name=exact,has_exact_export=bool(rec),path=rec[0] if rec else '',offset=rec[1] if rec else '',length=rec[2] if rec else ''))
assert len(expanded)==5859 and sum(len(v)>1 for v in candidates.values())==8
assert sum(r['has_exact_export'] for r in expanded)==5847 and sum(v>1 for v in exported_names.values())==1
write('closure-definitions.tsv',expanded)

sources=[]
for rec in rows(A/'cef/decompile/evidence/primary-sources.tsv'):
    assert sha(rec['source_path'])==rec['sha256']
    # Retain the original stage root when relocating its source index.
    excerpt=Path(rec['local_excerpt'])
    if not excerpt.is_absolute():excerpt=A/'cef/decompile'/excerpt
    assert excerpt.is_file(),excerpt
    rec['local_excerpt']=str(excerpt)
    rec['original_source_manifest']=str(A/'cef/decompile/evidence/primary-sources.tsv')
    sources.append(rec)
write('source-pins.tsv',sources)
official=[]
for file,root in [('cef-manifest.json','cef'),('chromium-manifest.json','chromium'),('cef-published-manifest.json','cef-published')]:
    path=A/'cef/initial/sources'/file
    records=json.loads(path.read_text())
    for r in records:
        if 'sha256' not in r:
            assert r.get('error') or r.get('status'),r
            continue  # explicitly failed fetch is not a source artifact/API absence
        target=path.parent/root/r['path']
        # Archived published headers use this folder name in the available snapshot.
        if not target.exists() and root=='cef-published':target=path.parent/'cef-published-156.0.7'/r['path']
        assert target.exists(),target
        assert sha(target)==r['sha256']
    official.append(dict(manifest=str(path),files=sum('sha256' in r for r in records),failed_fetch_records=sum('sha256' not in r for r in records),status='PASS'))
write('official-source-checks.tsv',official)
cefroot=A/'cef/initial/sources/cef'
assert 'refs/tags/156.0.8078.48' in (cefroot/'CHROMIUM_BUILD_COMPATIBILITY.txt').read_text()
assert 'Alloy style will always be used' in (cefroot/'include/internal/cef_types_mac.h').read_text()

addresses=[0x42b2430,0x43e6ee0,0x42fa5c8,0x4443ee0,0x4430bdc,0x4401f34,0x4402480,0x473cad8,0x6846c88,0x7bdc470,0x4415b60,0x435c8fc,0x435ca2c,0x4445a58,0x440a85c,0x2d0fb40,0x2e0238,0x957ca0c,0x957d304]
bodyrows=[]
for a in addresses:
    rec=db.execute('select name,path,offset,length,failed from functions where va=?',(a,)).fetchone();assert rec and not rec[-1] and rec[3]<=32768
    with open(rec[1],'rb') as f:f.seek(rec[2]);data=f.read(rec[3])
    p=OUT/'evidence'/f'body-{a:x}.c';p.write_bytes(data)
    # Mandated reader cross-check for every sampled original body.
    run=subprocess.run(['python3','-B',str(WORK/'read-function.py'),hex(a)],capture_output=True,check=True)
    assert b'TRUNCATED:' not in run.stdout and data in run.stdout
    bodyrows.append(dict(va=hex(a),name=rec[0],path=rec[1],offset=rec[2],length=rec[3],sha256=hashlib.sha256(data).hexdigest(),local=str(p),bounded_reader_equal=True))
write('sampled-bodies.tsv',bodyrows)

sends=rows(A/'protocols/initial/evidence/engine-all-static-sends.tsv')
for r in sends:
    assert b.string(b.ptr(int(r['selector_ref_va'],16)))==r['selector']
    actual=branch(b,int(r['send_va'],16))
    if r['branch_target']: assert actual==int(r['branch_target'],16)
    else:
        assert r['selector']=='webContents:updateDevToolsToolbox:'
        # Focus-only row omits target/stub columns. Resolve its range island
        # to the same msgSend stub corroborated by all other rows.
        x,y,z=struct.unpack('<III',b.read(actual,12))
        if x&0xfc000000==0x14000000: resolved=branch(b,actual)
        else:
            assert x&0x9f00001f==0x90000010 and y&0xffc003ff==0x91000210 and z==0xd61f0200
            d=((x>>29)&3)|(((x>>5)&0x7ffff)<<2)
            if d&(1<<20):d-=1<<21
            resolved=(actual&~4095)+(d<<12)+((y>>10)&4095)
        assert resolved==0xc4a5a90
assert any(r['selector']=='webContents:updateDevToolsToolbox:' and r['send_va']=='0x4447edc' for r in sends)
exports=[set((I/f'context/analysis/data/core{v}-exports.txt').read_text().splitlines()) for v in ('168','169','170')]
assert exports[0]==exports[1]==exports[2] and len(exports[0])==259
export_classes={s.split('_OBJC_CLASS_$_',1)[1] for s in exports[2] if s.startswith('_OBJC_CLASS_$_')}
assert len(export_classes)==146
results=json.loads((I/'acceptance-run/results/engine-swap/20261023T0501/results.json').read_text())
assert results['swap155']['webrtcDataChannel']==results['swap156']['webrtcDataChannel']=='ERR timeout'
summary=dict(status='PASS',image_hashes=len(pins),instruction_records=sum(r['records'] for r in instructions),older_raw_entries=len(checks),failures=dict(taxonomy),handoff_names=len(handoff),metadata_candidates=len(expanded),metadata_ambiguous_names=8,exported_body_resolved=5847,exported_body_ambiguous_names=1,metadata_rows=metadata_count,enclosing_span_sum_not_code_coverage=span_sum,starts_in_prior_enclosing_span=overlap,sampled_exact_original_bodies=len(bodyrows),protocol_selector_branch_checks=len(sends),source_anchors=len(sources),official_source_manifests=official,exports=259,exported_classes=146,acceptance_equal_error='webrtcDataChannel=ERR timeout in both sessions',limits='No runtime execution, complete behavioral proof, C++ ABI certification, source identity equality or rebuild feasibility demonstration.')
(OUT/'evidence/spot-checks.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary,indent=2))
