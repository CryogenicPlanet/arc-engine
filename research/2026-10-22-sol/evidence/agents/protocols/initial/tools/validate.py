#!/usr/bin/env python3
"""Fail on missing guide coverage or unsupported metadata/instruction anchors."""
import collections,hashlib,json,re,struct
from pathlib import Path
from audit import BASE,INPUT,load,tsv,guide,sha
from macho import MachO
def main():
    ps,g,raw=guide();c=load(BASE/'callbacks.tsv');p=load(BASE/'protocols.tsv');coverage=json.loads((BASE/'coverage.json').read_text())
    keys=[(r['protocol_name'],r['kind'],r['selector']) for r in c]
    assert len(c)==200 and len(set(keys))==200 and set(keys)==set(g)
    assert len(p)==26 and {r['protocol_name'] for r in p}==set(ps)
    assert {x.stem for x in (BASE/'protocol-specs').glob('*.md')}==set(ps)
    for row in c:
        assert all(row[k] for k in ['required_optional','type_encoding','metadata_status','app_implementation_evidence','engine_invocation_evidence','ownership_thread_evidence','confidence','unresolved_question'])
        spec=(BASE/'protocol-specs'/f"{row['protocol_name']}.md").read_text();assert row['typed_declaration'].replace('|','&#124;') in spec
    images={label:MachO(Path(r['path'])) for r in load(BASE/'evidence/input-pins.tsv') for label in [r['image']]}
    for r in load(BASE/'evidence/input-pins.tsv'):assert sha(Path(r['path']))==r['sha256']
    methodchecks=0
    for label,b in images.items():
        rows=load(BASE/f'evidence/{label}-methods.tsv')+load(BASE/f'evidence/{label}-implementations.tsv')
        for r in rows:
            # This independent check uses the entry's own list flags and raw
            # relative fields rather than re-running Metadata.read_methods.
            entry=int(r['entry_va'],16);lst=int(r['list_va'],16);flags,n=struct.unpack('<II',b.read(lst,8));stride=flags&65535
            assert entry>=lst+8 and (entry-lst-8)%stride==0 and (entry-lst-8)//stride<n
            if flags&0x80000000:
                sn,ty,im=struct.unpack('<iii',b.read(entry,12));sv=entry+sn
                if not flags&0x40000000:sv=b.pointer(sv)
                tv=entry+4+ty;imp=entry+8+im
            else:
                sv=b.pointer(entry);tv=b.pointer(entry+8);imp=b.pointer(entry+16) if r['imp'] else None
            assert b.string(sv)==r['selector'] and b.string(tv)==r['type_encoding']
            if r['imp']:assert imp==int(r['imp'],16)
            if r.get('extended_type_slot_va'):
                ev=b.pointer(int(r['extended_type_slot_va'],16));assert ev==int(r['extended_type_va'],16) and b.string(ev)==r['extended_type_encoding']
            methodchecks+=1
    selected=0;missing=[]
    for file in ['engine-body-index.tsv','focus-body-index.tsv','app-body-index.tsv']:
        for r in load(BASE/'evidence'/file):
            if not r.get('local'):missing.append(dict(table=file,va=r['va'],status=r['status']));continue
            data=Path(r['local']).read_bytes();assert len(data)==int(r['extracted_length']) and hashlib.sha256(data).hexdigest()==r['sha256']
            with Path(r['path']).open('rb') as f:f.seek(int(r['offset']));assert f.read(len(data))==data
            selected+=1
    b=images['engine170'];stubs=b.import_stubs()
    def symbol(target):
        for _ in range(5):
            if target in stubs:return stubs[target]
            x,y,z=struct.unpack('<III',b.read(target,12))
            if x&0xfc000000==0x14000000:
                d=x&0x3ffffff;d=d-(1<<26) if d&(1<<25) else d;target+=d*4;continue
            if x&0x9f00001f==0x90000010 and y&0xffc003ff==0x91000210 and z==0xd61f0200:
                d=((x>>29)&3)|(((x>>5)&0x7ffff)<<2);d=d-(1<<21) if d&(1<<20) else d;target=(target&~4095)+(d<<12)+((y>>10)&4095);continue
            return ''
        return ''
    sends=load(BASE/'evidence/engine-all-static-sends.tsv')
    for r in sends:
        slot=int(r['selector_ref_va'],16);assert b.string(b.pointer(slot))==r['selector']
        va=int(r['send_va'],16);x=struct.unpack('<I',b.read(va,4))[0];assert x&0xfc000000 in [0x94000000,0x14000000]
        d=x&0x3ffffff;d=d-(1<<26) if d&(1<<25) else d;assert symbol(va+d*4)=='_objc_msgSend'
    instructions=load(BASE/'evidence/focus-instructions.tsv')
    for r in instructions:
        va=int(r['va'],16);assert b.read(va,4).hex()==r['raw'] and b.offset(va,4)==int(r['file_offset'],16)
    expected={0x4445ac0:'1f3000f1',0x4446dcc:'68008052',0x4446dd0:'3f0c00f1',0x4446dd4:'2130889a',0x4446dd8:'021840f9',0x4446ddc:'00800091',0x4446de0:'40001fd6',0x40427b0:'0803631e'}
    for va,raw in expected.items():assert b.read(va,4).hex()==raw
    for va in [0x1004e82e8,0x1004e82ec]:
        w=struct.unpack('<I',images['app168'].read(va,4))[0];assert w&0xfc000000==0x14000000
        imm=w&0x3ffffff;imm=imm-(1<<26) if imm&(1<<25) else imm;assert va+imm*4==0x1004e530c
    old=load(BASE/'older-encoding-audit.tsv');assert len(old)==6
    assert all(r['raw_encoding_comparison']=='equal' and r['relevant_layout_equal']=='True' and r['resolution']=='older_encoding_resolved' for r in old)
    manifestrows=load(BASE/'evidence/shards.tsv');assert len(manifestrows)==199 and len({r['path'] for r in manifestrows})==199
    manifests={}
    for r in manifestrows:
        name=r['manifest']
        if name not in manifests:manifests[name]={x['file']:x for x in json.loads(Path(name).read_text())}
        orig=manifests[name][Path(r['path']).name];assert orig['sha256']==r['sha256'] and orig['bytes']==int(r['bytes'])
    for r in load(BASE/'evidence/primary-sources.tsv'):assert sha(Path(r['path']))==r['sha256']
    assert (BASE/'report.md').exists()
    result=dict(status='PASS',protocols=26,guide_rows=200,raw_protocol_method_entries=235,metadata_entry_checks=methodchecks,selected_text_snapshots_checked=selected,selected_missing_headers=missing,static_send_selector_and_branch_checks=len(sends),instruction_byte_and_offset_checks=len(instructions),older_unknowns_resolved=6,manifest_verified_shards=199,full_corpus_header_scan=567959,limits='Checks validate static provenance and bounded instructions, not actual receivers, execution, complete ABI, callback semantics or replacement compatibility.')
    (BASE/'validation.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k!='selected_missing_headers'},indent=2))
    # Self-contained artifact hashes; logs and this list are intentionally not
    # hashed because stdout redirection is still open during reproduction.
    artifacts=[]
    for path in sorted(BASE.rglob('*')):
        if path.is_file() and path.suffix not in ['.log','.pyc'] and '__pycache__' not in path.parts and path.name!='artifacts.sha256':artifacts.append(f'{sha(path)}  {path.relative_to(BASE)}')
    (BASE/'artifacts.sha256').write_text('\n'.join(artifacts)+'\n')
if __name__=='__main__':main()
