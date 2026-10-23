#!/usr/bin/env python3
"""Independent binary cross-checks, branch validation, and streamed provenance."""
import collections,csv,hashlib,json,struct
from pathlib import Path
from inventory import ROOT,OUT,DATA,MachO,write_tsv
from boundaries import read_methods
from calls import stub_selector

def table(name):return list(csv.DictReader((OUT/name).open(),delimiter='\t'))

def main():
    b=MachO(ROOT/'binaries/ArcCore-1.170.0.arm64');app=MachO(ROOT/'binaries/Arc-1.168.0.arm64')
    binary_methods=[]
    va,sz,_=b.sections[('__DATA_CONST','__objc_classlist')]
    for a in range(va,va+sz,8):
        cls=b.pointer(a)
        for ptr in [cls,b.pointer(cls)]:
            ro=b.pointer(ptr+32)&~7
            binary_methods.extend(read_methods(b,b.pointer(ro+32)))
    va,sz,_=b.sections[('__DATA_CONST','__objc_catlist')]
    for a in range(va,va+sz,8):
        cat=b.pointer(a)
        for offset in [16,24]:binary_methods.extend(read_methods(b,b.pointer(cat+offset)))
    methods=table('all-methods-170.tsv');dump={r['method_entry_va']:r for r in methods}
    assert len(binary_methods)==len(methods)==4314
    assert len(dump)==len(methods)
    for r in binary_methods:
        d=dump[r['entry_va']]
        assert all(r[k]==d[k] for k in ['selector','type_encoding','imp']), (r,d)
    calls=table('used-calls.tsv');branches=0;binaryrows=0
    for r in calls:
        target=int(r['app_stub_va'],16)
        if not r['selector'].startswith('_'):
            assert stub_selector(app,target)[0]==r['selector']
        rowbranches=0
        for key,opcode in [('binary_bl_vas',0x94000000),('binary_tail_b_vas',0x14000000)]:
            for va in filter(None,r[key].split(';')):
                va=int(va,16);ins=struct.unpack('<I',app.read(va,4))[0]
                assert ins&0xfc000000==opcode
                imm=ins&0x3ffffff
                if imm&0x2000000:imm-=1<<26
                assert va+imm*4==target
                assert int(r['app_caller_min'],16)<=va<int(r['app_caller_end'],16)
                branches+=1;rowbranches+=1
        if rowbranches:binaryrows+=1
    # Preserve category shadowing explicitly: never collapse declarations into
    # one implementation merely because a dispatch key is the same.
    grouped=collections.defaultdict(list)
    for r in methods:
        if r['class_name'].startswith('Arc'):grouped[(r['class_name'],r['kind'],r['selector'])].append(r)
    collision=[]
    for (cls,kind,sel),rs in grouped.items():
        if len(rs)<2:continue
        collision.append(dict(class_name=cls,kind=kind,selector=sel,
            declarations_json=json.dumps([dict(category=r['category'],imp=r['imp'],type_encoding=r['type_encoding']) for r in rs]),
            runtime_resolution='category_attachment_required;static_metadata_does_not_observe_runtime_method_resolution'))
    write_tsv(OUT/'category-shadowing.tsv',collision)
    contract=table('contract.tsv');assert len(contract)==834
    assert len({(r['class_name'],r['category'],r['kind'],r['selector']) for r in contract})==834
    assert len({r['class_name'] for r in contract})==63
    assert all(r['pairing']=='binary_selector+class/category/kind/IMP' for r in contract)
    assert sum(not r['encoding_168'] for r in contract)==6
    cb=table('app-callbacks.tsv')
    for r in cb:
        found=[m for m in read_methods(app,int(r['list_va'],16)) if m['entry_va']==r['entry_va']]
        assert len(found)==1 and all(found[0][k]==r[k] for k in ['selector','type_encoding','imp'])
    reqs=table('decompile-requests.tsv')
    assert len(reqs)==65 and sum(r['ghidra_match']=='exact_start' for r in reqs)==64
    engine_runtime=[]
    if (OUT/'engine-runtime-calls.tsv').exists():
        engine_runtime=table('engine-runtime-calls.tsv');stubs=b.import_stubs()
        for r in engine_runtime:
            va=int(r['instruction_va'],16);ins=struct.unpack('<I',b.read(va,4))[0]
            assert ins&0xfc000000==(0x94000000 if r['branch']=='BL' else 0x14000000)
            imm=ins&0x3ffffff
            if imm&0x2000000:imm-=1<<26
            target=va+4*imm
            assert target==int(r['branch_target_va'],16)
            for island in filter(None,r['range_extension_islands'].split(';')):
                assert target==int(island,16)
                a,c,d=struct.unpack('<III',b.read(target,12))
                if a&0xfc000000==0x14000000:
                    imm=a&0x3ffffff
                    if imm&0x2000000:imm-=1<<26
                    target+=imm*4
                else:
                    assert a&0x9f00001f==0x90000010 and c&0xffc003ff==0x91000210 and d==0xd61f0200
                    imm=((a>>29)&3)|(((a>>5)&0x7ffff)<<2)
                    if imm&(1<<20):imm-=1<<21
                    target=(target&~4095)+(imm<<12)+((c>>10)&0xfff)
            assert target==int(r['stub_va'],16) and stubs[target]==r['import_symbol']
    # Evidence pinning uses bounded chunks, not whole-file reads. Hash only
    # inputs actually consumed, including the APP pseudocode rather than any
    # absent ENGINE pseudocode.
    inputs=[*DATA.glob('*.txt'),ROOT/'binaries/Arc-1.168.0.arm64',ROOT/'binaries/ArcCore-1.170.0.arm64',
        ROOT/'metadata/decomp/arccore-1.170.0-functions.tsv',ROOT/'app168/ghidra/functions.tsv',
        ROOT/'app168/dump/stubs.txt',ROOT/'app168/dump/swift-types.swift',ROOT/'app168/dump/symbol-map.tsv',
        *sorted((ROOT/'app168/ghidra/decomp').glob('*.c'))]
    engine_summary=OUT/'engine-evidence-summary.json'
    if engine_summary.exists():
        inputs.extend(Path(p) for p in json.load(engine_summary.open()).get('engine_source_files',[]))
        if (ROOT/'decompile/MANIFEST.json').exists():inputs.append(ROOT/'decompile/MANIFEST.json')
    provenance=[]
    for path in sorted(inputs):
        h=hashlib.sha256()
        with path.open('rb') as f:
            while chunk:=f.read(1024*1024):h.update(chunk)
        provenance.append(dict(path=str(path),bytes=path.stat().st_size,sha256=h.hexdigest()))
    write_tsv(OUT/'input-sha256.tsv',provenance)
    manifest=ROOT/'decompile/MANIFEST.json'
    if manifest.exists():
        expected={r['file']:r for r in json.load(manifest.open())}
        for r in provenance:
            if str(ROOT/'decompile')+'/' in r['path'] and Path(r['path']).name in expected:
                e=expected[Path(r['path']).name]
                assert r['bytes']==e['bytes'] and r['sha256']==e['sha256']
    uuids={}
    for name,image in [('app168',app),('core170',b)]:
        uuids[name]=next(d.hex() for cmd,d in image.commands if cmd==0x1b)
    summary=dict(validation='PASS',engine_binary_methods_independently_parsed=len(binary_methods),
        app_call_rows=len(calls),rows_with_binary_branch_evidence=binaryrows,validated_branch_instructions=branches,
        imported_declarations=len(contract),imported_dispatch_keys=len({(r['class_name'],r['kind'],r['selector']) for r in contract}),
        imported_classes=63,all_arc_category_shadow_keys=len(collision),app_callback_methods_binary_validated=len(cb),
        input_files_hashed=len(provenance),macho_uuid_hex=uuids,engine_runtime_import_calls_revalidated=len(engine_runtime),
        engine_pseudocode_input_present=(ROOT/'decompile').exists())
    (OUT/'validation.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary,indent=2))

if __name__=='__main__':main()
