#!/usr/bin/env python3
"""Read-only checks of the revision; writes only its new validation artifacts."""
import collections, csv, datetime, fcntl, json, re, struct
from pathlib import Path
from recheck import HERE, INPUT, ORIGINAL, OUT, Image, sha

def readrows(path): return list(csv.DictReader(path.open(), delimiter='\t'))
def identity(r): return tuple(r[k] for k in ('class_name','category','kind','selector'))
def declaration(r): return identity(r)+(r['type_encoding'],r['imp_va'])

def main():
    checks = {}
    original = readrows(ORIGINAL/'methods.tsv'); rows = readrows(HERE/'methods.tsv')
    assert len(original) == len(rows) == 491
    assert [declaration(r) for r in original] == [declaration(r) for r in rows]
    assert len(set(map(identity,rows))) == 491
    assert [r['body_status'] for r in original] == [r['body_status'] for r in rows]
    assignment = json.loads((INPUT/'context/class-assignments.json').read_text())['groups']['spec-3']['classes']
    classes = {r['class_name'] for r in rows}
    assert len(classes) == 37 and classes == set(assignment)
    assert {p.stem for p in (HERE/'classes').glob('*.md')} == classes
    im = Image(INPUT/'binaries/ArcCore-1.170.0.arm64')
    # Confirm the raw pointer format used by this independent method decoder.
    pos=32; formats=set()
    for _ in range(struct.unpack_from('<I',im.read(0,32),16)[0]):
        cmd,n=struct.unpack('<II',im.read(pos,8)); b=im.read(pos,n)
        if cmd == 0x80000034:
            fix=struct.unpack_from('<I',b,8)[0]
            start=fix+struct.unpack_from('<I',im.read(fix,28),4)[0]
            count=struct.unpack('<I',im.read(start,4))[0]
            for i in range(count):
                rel=struct.unpack('<I',im.read(start+4+i*4,4))[0]
                if rel: formats.add(struct.unpack('<H',im.read(start+rel+6,2))[0])
        pos+=n
    assert formats == {2}
    actual=[]; classvas={}
    for va in im.pointers('__objc_classlist'):
        cl=im.name(va)
        if cl in classes:
            classvas[cl]=va
            for c,k in ((va,'-'),(im.ptr(va),'+')):
                ro=im.ptr(c+32)&~7
                actual+=im.methods(im.ptr(ro+32),cl,'',k)
    for va in im.pointers('__objc_catlist'):
        c=im.ptr(va+8)
        if c and im.name(c) in classes:
            cl=im.name(c);cat=im.string(im.ptr(va))
            for off,k in ((16,'-'),(24,'+')):
                actual+=im.methods(im.ptr(va+off),cl,cat,k)
    assert collections.Counter(actual)==collections.Counter(map(declaration,rows))
    assert len(actual)==491 and set(classvas)==classes
    dyld=[];cl=cat=''
    for line in (INPUT/'context/analysis/data/dobjc170.txt').open():
        m=re.search(r'@interface (\w+)(?:\(([^)]+)\))?',line)
        if m:cl,cat=m[1],m[2] or ''
        m=re.search(r'(0x[\da-fA-F]+)\s+([-+])\[([^ ]+) ([^]]+)\]',line)
        if m and cl in classes:
            assert cl==m[3];dyld.append((cl,cat,m[2],m[4],hex(int(m[1],16))))
    assert collections.Counter(dyld)==collections.Counter(identity(r)+(r['imp_va'],) for r in rows)
    checks['inventory'] = dict(classes=37,method_identities=491,distinct_imps=len({r['imp_va'] for r in rows}),
        kinds=dict(collections.Counter(r['kind'] for r in rows)),
        categories=dict(collections.Counter(r['category'] or '(base)' for r in rows)),
        unchanged_order_and_identity_type_imp=True,binary_and_dyld_redecoded=True)
    preserved=[]
    for directory in ('bodies','evidence','sources'):
        files={str(p.relative_to(ORIGINAL/directory)) for p in (ORIGINAL/directory).rglob('*') if p.is_file()}
        copied={str(p.relative_to(HERE/directory)) for p in (HERE/directory).rglob('*') if p.is_file()}
        assert copied==files,(directory,'file set mismatch')
        for name in sorted(files):
            a=ORIGINAL/directory/name;b=HERE/directory/name
            assert a.read_bytes()==b.read_bytes(),str(b)
            preserved.append(dict(path=str(b.relative_to(HERE)),sha256=sha(b)))
    assert (HERE/'original-report.md').read_bytes()==(ORIGINAL/'report.md').read_bytes()
    checks['preserved_file_counts']=dict(collections.Counter(p['path'].split('/')[0] for p in preserved))
    (OUT/'preservation.json').write_text(json.dumps(preserved,indent=2)+'\n')
    # Validate every rendered method against the revised TSV, not only row counts.
    bykey={identity(r):r for r in rows}; matrices=[];linkcount=0
    pattern=re.compile(r'^\| ([^|]+) \| `([-+])([^`]+)` \| `([^`]+)` \| \[(0x[\da-f]+)\]\(([^)]+)\) \| (.*?) \| (.*?) \| (.*?) \| (.*?) \|$')
    for cl in sorted(classes):
        p=HERE/'classes'/f'{cl}.md';text=p.read_text()
        for line in text.splitlines():
            m=pattern.match(line)
            if not m:continue
            cat='' if m[1]=='(base)' else m[1];key=(cl,cat,m[2],m[3]);r=bykey[key]
            assert (m[4],m[5])==(r['type_encoding'],r['imp_va']),key
            assert m[7]==r['behavior'],key
            assert m[10]==f"{r['confidence']}; {r['body_status']}; {r['semantic_status']}",key
            assert (p.parent/m[6]).resolve().is_file(),(key,m[6])
            matrices.append(key)
        for link in re.findall(r'\]\(([^)]+)\)',text):
            if link.startswith(('http:', 'https:', '#')): continue
            assert (p.parent/link.split('#')[0]).resolve().exists(),(p,link)
            linkcount+=1
    assert collections.Counter(matrices)==collections.Counter(map(identity,rows))
    checks['rendered_matrix_rows']=len(matrices);checks['class_relative_links']=linkcount
    c=json.loads((HERE/'coverage.json').read_text())
    assert c['assigned_classes']==c['expected_classes']==37
    assert c['declared_method_rows']==c['expected_method_rows']==c['unique_identities']==491
    assert c['distinct_imps']==433
    assert c['method_kind_counts']==dict(collections.Counter(r['kind'] for r in rows))
    assert c['category_method_counts']==dict(collections.Counter(r['category'] or '(base)' for r in rows))
    for column,target in [('body_status','body_status_counts'),('semantic_status','semantic_status_counts')]:
        assert c[target]==dict(collections.Counter(r[column] for r in rows))
    assert c['method_rows_with_open_questions']==sum(bool(r['unresolved_question']) for r in rows)
    assert c['unresolved_method_rows']==sum(r['semantic_status']=='unresolved' for r in rows)
    assert c['runtime_semantically_verified_method_rows']==c['full_backend_semantically_verified_method_rows']==0
    assert not c['ghidra_bound_sizes_summed'] and c['body_counts_are_presence_only']
    for cl in classes:
        rr=[r for r in rows if r['class_name']==cl]
        assert c['per_class'][cl]==dict(method_rows=len(rr),statuses=dict(collections.Counter(r['semantic_status'] for r in rr)))
    changes=json.loads((OUT/'annotation-changes.json').read_text())
    oldby={identity(r):r for r in original}
    changed={identity(r) for r in rows if r!=oldby[identity(r)]}
    assert changed=={identity(x['after']) for x in changes} and len(changed)==21
    for x in changes:
        assert x['before']==oldby[identity(x['before'])]
        assert x['after']==bykey[identity(x['after'])]
    assert c['revision_corrected_method_rows']==len(changed)
    checks['changed_method_annotations']=len(changed)
    checks['semantic_status_counts']=c['semantic_status_counts']
    checks['method_rows_with_open_questions']=c['method_rows_with_open_questions']
    findings=readrows(Path('/home/exedev/work/agents/spec-2/classreview/findings.tsv'))
    corrections=readrows(HERE/'corrections.tsv')
    assert list(corrections[0])==['finding_row','class_name','selector','disposition','before','after','evidence','remaining_unknown']
    assert len(findings)==len(corrections)==c['revision_review_findings']==6
    for i,(f,r) in enumerate(zip(findings,corrections),1):
        assert r['finding_row']==str(i)
        assert (r['class_name'],r['selector'])==(f['class'],f['selector'])
        assert f['claim'] in r['before'] and r['disposition']=='corrected'
        row=bykey[(f['class'],'','-',f['selector'])]
        assert r['after']==row['behavior'] and r['remaining_unknown'] and r['evidence']
        for link in re.findall(r'revision-evidence/[\w.-]+',r['evidence']): assert (HERE/link).is_file()
    checks['finding_data_rows_addressed']=len(corrections)
    # Recheck all original source hashes, using revised CEF copies where applicable.
    sources=json.loads((HERE/'evidence/sources.json').read_text())
    for s in sources:
        assert s['commit']=={'Chromium':'d0c02bfad08ec92a6ffcf989e53cd995e6381a4c','CEF':'426920c33ef22551e0b8f48cf343d623ac25642d'}[s['repository']]
        path=HERE/'sources/cef'/Path(s['file']).name if s['repository']=='CEF' else Path(s['local_path'])
        assert sha(path)==s['sha256']
    for s in json.loads((OUT/'source-checks.json').read_text()):assert sha(Path(s['local_path']))==s['sha256']
    checks['original_pinned_source_hashes']=len(sources)
    bodyprovenance=readrows(OUT/'body-provenance.tsv')
    for r in bodyprovenance:
        p=OUT/r['helper_output'];assert sha(p)==r['helper_output_sha256']
        result=p.read_bytes();assert result.startswith(f"INDEX {r['va']} ".encode())
        body=result.split(b'\n',1)[1]
        n=int(r['original_C_bytes']);assert body[n:]==b'\n' and len(body[:n])==n
        assert body[:n].startswith(f"// ==== {r['va']} ".encode())
        copied=HERE/'bodies'/f"{int(r['va'],16):08x}.txt"
        if copied.exists():assert body[:n] in copied.read_bytes()
        assert r['truncated']=='False'
    instructions=readrows(OUT/'instructions.tsv')
    for r in instructions:
        va=int(r['va'],16);assert r['image']=='ArcCore-1.170.0.arm64'
        assert im.offset(va,4)==int(r['file_offset'],16)
        assert im.data(va,4).hex()==r['bytes']
    for va in (0x15ac0,0xfbc0):assert im.data(va,4)==bytes.fromhex('c0035fd6')
    for r in json.loads((HERE/'evidence/arm64-leaves.json').read_text()):
        va=int(r['imp'],16);n=4*len(r['words'])
        assert im.offset(va,n)==int(r['file_offset'],16)
        assert struct.unpack('<'+'I'*len(r['words']),im.data(va,n))==tuple(int(w,16) for w in r['words'])
    assert im.string(im.ptr(0xe2a97b0))=='owner'
    assert im.string(im.ptr(0xe2a97b8))=='T@"ArcWebContents",W,N,V_owner'
    assert (im.ptr(0xdf6b728),im.ptr(0xdf6b730))==(0x4044e80,0x4044ec0)
    symbols={int(r['veneer_va'],16):r['symbol'] for r in readrows(OUT/'runtime-symbols.tsv')}
    assert symbols[0x84edd34]=='_objc_loadWeakRetained' and symbols[0x8b31a18]=='_objc_autoreleaseReturnValue'
    assert symbols[0x8869080]=='_objc_storeWeak' and symbols[0xad3a854]=='_objc_retainBlock'
    with Path('/home/exedev/work/heavy.lock').open('rb') as lock:
        fcntl.flock(lock,fcntl.LOCK_EX)
        assert sha(im.path)==json.loads((OUT/'image.json').read_text())['sha256']=='69276adaca785a46f2554e7fd18926e7fcdd6aba44ba64400a337427817db3dd'
    checks['new_bounded_helper_reads']=len(bodyprovenance)
    checks['new_instruction_words']=len(instructions)
    checks['explicitly_undecoded_words']=sum('UNDECODED' in r['instruction'] for r in instructions)
    checks['original_leaf_instruction_rows_rechecked']=9
    checks['scope']='Static artifact/provenance checks and bounded semantic correction; no runtime or replacement compatibility validation.'
    result=dict(result='PASS',utc=datetime.datetime.now(datetime.timezone.utc).isoformat(),checks=checks)
    (OUT/'validation.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,indent=2))

if __name__=='__main__':main()
