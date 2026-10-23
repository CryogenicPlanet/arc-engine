#!/usr/bin/env python3
"""Read-only evidence reconciliation; output is confined to this role directory.

Run under the existing heavy.lock. No other role's programs are imported/run.
This checks provenance and declarations, not semantic/runtime equivalence.
"""
import collections as C
import csv, hashlib, json, os, re, sqlite3, struct, time
from pathlib import Path

OUT = Path(__file__).resolve().parents[1]
WORK = OUT.parents[2]
INP = WORK / 'inputs'
AG = WORK / 'agents'
os.sched_setaffinity(0, {min(os.sched_getaffinity(0))})
START = time.monotonic()

def rows(p):
    with Path(p).open() as f:
        return list(csv.DictReader(f, delimiter='\t'))

def write(p, rs, fields=None):
    with (OUT / p).open('w') as f:
        w = csv.DictWriter(f, fields or list(rs[0]), delimiter='\t')
        w.writeheader(); w.writerows(rs)

def digest(p):
    h = hashlib.sha256()
    with Path(p).open('rb') as f:
        for b in iter(lambda: f.read(1024*1024), b''): h.update(b)
    return h.hexdigest()

def key(r): return tuple(r[k] for k in ('class_name','category','kind','selector'))

class Image:
    """Minimal bounded reader; explicitly checks chained pointer format 2."""
    def __init__(self, path):
        self.f = Path(path).open('rb'); self.segs=[]; self.sections={}; self.commands=[]; self.formats={}
        h=struct.unpack('<8I',self.at(0,32)); assert h[:2]==(0xfeedfacf,0x100000c)
        pos=32
        for _ in range(h[4]):
            cmd,n=struct.unpack('<II',self.at(pos,8)); assert 8<=n<=32768
            b=self.at(pos,n); self.commands.append(cmd)
            if cmd==0x19:
                name,va,vs,off,fs,_,_,ns,_=struct.unpack_from('<16sQQQQiiII',b,8)
                self.segs.append((name.rstrip(b'\0').decode(),va,vs,off,fs))
                for j in range(ns):
                    sec,seg,a,size,fo=struct.unpack_from('<16s16sQQI',b,72+80*j)
                    self.sections[(seg.rstrip(b'\0').decode(),sec.rstrip(b'\0').decode())]=(a,size,fo)
            if cmd==0x80000034:
                fo=struct.unpack_from('<I',b,8)[0]; starts=fo+struct.unpack('<I',self.at(fo+4,4))[0]
                count=struct.unpack('<I',self.at(starts,4))[0]; fmts=set()
                for j in range(count):
                    d=struct.unpack('<I',self.at(starts+4+j*4,4))[0]
                    if d:
                        fmt=struct.unpack('<H',self.at(starts+d+6,2))[0]
                        fmts.add(fmt); self.formats[self.segs[j][0]]=fmt
                assert fmts and fmts<={2,6},fmts
            pos+=n
        assert pos==32+h[5]
    def at(self,off,n):
        assert 0<=n<=32768
        self.f.seek(off); b=self.f.read(n); assert len(b)==n; return b
    def offset(self,va,n=1):
        for _,a,_,off,fs in self.segs:
            if a<=va and va+n<=a+fs: return off+va-a
        raise ValueError(hex(va))
    def read(self,a,n): return self.at(self.offset(a,n),n)
    def ptr(self,a):
        raw=struct.unpack('<Q',self.read(a,8))[0]
        if raw>>63: return None  # bound import; never interpret as rebase
        seg=next(s for s in self.segs if s[1]<=a<s[1]+s[4])
        fmt=self.formats.get(seg[0])
        if not fmt: return raw
        target=(raw&((1<<36)-1))|(((raw>>36)&255)<<56)
        return target+(next(s[1] for s in self.segs if s[0]=='__TEXT') if fmt==6 else 0)
    def string(self,a):
        chunks=[]
        for off in range(0,32768,256):
            b=self.read(a+off,256); chunks.append(b.split(b'\0')[0])
            if b'\0' in b: return b''.join(chunks).decode()
        raise ValueError('unterminated string')
    def section_ptrs(self,name):
        a,n,_=next(v for k,v in self.sections.items() if k[1]==name)
        for x in range(a,a+n,8): yield self.ptr(x)
    def method_list(self,a):
        if not a: return []
        flags,n=struct.unpack('<II',self.read(a,8)); stride=flags&0xffff
        assert stride in (12,24) and n<10000
        out=[]
        for i in range(n):
            m=a+8+stride*i
            if flags&0x80000000:
                s,t,imp=struct.unpack('<iii',self.read(m,12)); s+=m; t+=m+4; imp+=m+8
                if not flags&0x40000000: s=self.ptr(s)
            else: s,t,imp=[self.ptr(m+j) for j in (0,8,16)]
            out.append(dict(selector=self.string(s),type_encoding=self.string(t),imp_va=hex(imp),method_record_va=hex(m)))
        return out
    def inventory(self):
        methods=[]; classes={}
        for cl in self.section_ptrs('__objc_classlist'):
            ro=self.ptr(cl+32)&~7; name=self.string(self.ptr(ro+24))
            if not name.startswith('Arc'): continue
            classes[name]=(cl,ro)
            for a,kind in ((cl,'-'),(self.ptr(cl),'+')):
                r=self.ptr(a+32)&~7
                methods += [dict(class_name=name,category='',kind=kind,**m) for m in self.method_list(self.ptr(r+32))]
        for cat in self.section_ptrs('__objc_catlist'):
            cl=self.ptr(cat+8)
            if not cl: continue
            ro=self.ptr(cl+32)&~7; name=self.string(self.ptr(ro+24))
            if name not in classes: continue
            category=self.string(self.ptr(cat))
            for off,kind in ((16,'-'),(24,'+')):
                methods += [dict(class_name=name,category=category,kind=kind,**m) for m in self.method_list(self.ptr(cat+off))]
        return methods,classes

manifest=json.loads((WORK/'CLASS-REVISIONS-READY').read_text())
hashchecks=[]; specs=[]; classindex=[]; grouprows=[]; dispositions=[]; prose=[]
original_reviews={'spec-1':'spec-4','spec-2':'spec-1','spec-3':'spec-2','spec-4':'spec-3'}
for s in manifest['selected']:
    writer=s['writer']; root=Path(s['specs']); review=Path(s['review'])
    for domain in ('spec_sha256','review_sha256'):
        for p,h in s[domain].items():
            actual=digest(WORK/p); assert actual==h,(p,h,actual)
            hashchecks.append(dict(writer=writer,domain=domain,path=p,sha256=actual,status='PASS'))
    ms=rows(root/'methods.tsv'); assert len(ms)==len({key(r) for r in ms})==491
    old=rows(AG/writer/'specs/methods.tsv')
    assert [key(r)+(r['type_encoding'],r['imp_va']) for r in ms]==[key(r)+(r['type_encoding'],r['imp_va']) for r in old]
    cr=rows(review/'class-review.tsv'); crmap={r['class_name']:r for r in cr}
    assert set(crmap)==set(s['classes'])=={p.stem for p in (root/'classes').glob('*.md')}
    assert not rows(review/'findings.tsv')
    ledger=rows(root/'corrections.tsv'); disp=rows(review/'finding-dispositions.tsv')
    historic=rows(AG/original_reviews[writer]/'classreview/findings.tsv')
    assert len(ledger)==len(disp)==len(historic)
    assert [r['finding_row'] for r in ledger]==[str(i+1) for i in range(len(historic))]
    assert [r['finding_row'] for r in disp]==[str(i+1) for i in range(len(historic))]
    for l,d in zip(ledger,disp):
        dispositions.append(dict(writer=writer,original_reviewer=original_reviews[writer],**l,fresh_status=d['status'],fresh_review_path=str(review/'finding-dispositions.tsv')))
    for name in s['classes']:
        p=root/'classes'/f'{name}.md'; doc=p.read_text(); matrix=[]
        for ln in doc.splitlines():
            if not ln.startswith('| '): continue
            cols=[x.strip() for x in ln.split('|')[1:-1]]
            if len(cols)<4: continue
            ksel=cols[1].strip('`').strip()
            mat=re.fullmatch(r'([+-])\s*(\S+)',ksel)
            if not mat: continue
            imp=re.search(r'0x[0-9a-f]+',cols[3])
            if not imp: continue
            cat='' if cols[0] in ('base','(base)') else cols[0].strip('`')
            matrix.append((name,cat,mat[1],mat[2],cols[2].strip('`'),imp[0]))
        mm=[r for r in ms if r['class_name']==name]
        expect=[key(r)+(r['type_encoding'],r['imp_va']) for r in mm]
        assert C.Counter(matrix)==C.Counter(expect),(name,len(matrix),len(expect))
        # Read every class narrative separately from mechanically reconciled matrices/layouts.
        narrative='\n'.join(ln for ln in doc.splitlines() if not ln.startswith('|'))
        prose.append(f'\n### {writer}: {name}\n'+narrative+'\nFRESH REVIEW: '+crmap[name]['limits'])
        classindex.append(dict(class_name=name,writer=writer,reviewer=writer+'/revision-reviews/v1',status=crmap[name]['status'],path=str(p),revision='v1',original_reviewer=original_reviews[writer]+'/classreview',review_path=str(review/'class-review.tsv'),review_report=str(review/'report.md'),correction_ledger=str(root/'corrections.tsv'),spec_sha256=digest(p),review_sha256=digest(review/'class-review.tsv'),selection_manifest=str(WORK/'CLASS-REVISIONS-READY'),declared_methods=len(mm),unique_imps=len({r['imp_va'] for r in mm}),open_question_rows=sum(bool(r['unresolved_question']) for r in mm),synthesis_status='bounded_static_research;not_complete_implementation_contract'))
    specs+=ms
    grouprows.append(dict(writer=writer,classes=len(s['classes']),methods=len(ms),distinct_imps=len({r['imp_va'] for r in ms}),base=sum(not r['category'] for r in ms),category=sum(bool(r['category']) for r in ms),instance=sum(r['kind']=='-' for r in ms),class_methods=sum(r['kind']=='+' for r in ms),open_questions=sum(bool(r['unresolved_question']) for r in ms),explicit_unresolved=sum(r['semantic_status']=='unresolved' for r in ms),semantic_status=json.dumps(C.Counter(r['semantic_status'] for r in ms),sort_keys=True),body_status=json.dumps(C.Counter(r['body_status'] for r in ms),sort_keys=True)))
assert len(classindex)==len({r['class_name'] for r in classindex})==146
assert len(specs)==len({key(r) for r in specs})==1964
write('spec-index.tsv',sorted(classindex,key=lambda r:r['class_name']))
write('evidence/selection-hashes.tsv',hashchecks)
write('evidence/groups.tsv',grouprows)
write('evidence/correction-dispositions.tsv',dispositions)
(OUT/'evidence/class-narratives.md').write_text('\n'.join(prose))

image=Image(INP/'binaries/ArcCore-1.170.0.arm64')
binary,classes=image.inventory(); bykey={key(r):r for r in binary}
assert len(classes)==146 and len(binary)==len(bykey)==1964
for r in specs:
    b=bykey[key(r)]; assert (b['type_encoding'],b['imp_va'])==(r['type_encoding'],r['imp_va'])
    b['imp_file_offset']=hex(image.offset(int(b['imp_va'],16)))
write('evidence/binary-methods.tsv',sorted(binary,key=key))
protocol_impl=rows(AG/'protocols/initial/evidence/engine170-implementations.tsv')
assert {key(r)+(r['type_encoding'],r['imp']) for r in protocol_impl}=={key(r)+(r['type_encoding'],r['imp_va']) for r in specs}
old_impl=rows(AG/'protocols/initial/evidence/engine168-implementations.tsv')
assert {key(r):r['type_encoding'] for r in old_impl}=={key(r):r['type_encoding'] for r in specs}
layouts=[json.loads((AG/f'protocols/initial/evidence/engine{v}-layouts.json').read_text()) for v in ('168','170')]
# Same compared-field definition as the independently inspected parent script.
clean=lambda raw:{c:{k:([{ik:iv for ik,iv in i.items() if ik!='entry_va'} for i in v] if k=='ivars' else v) for k,v in record.items() if k not in ('image','class_va','ro_va')} for c,record in raw.items()}
assert clean(layouts[0])==clean(layouts[1])
assert len(layouts[0])==146 and sum(len(r['ivars']) for r in layouts[0].values())==351
assert all(type(i['offset']) is int for r in layouts[0].values() for i in r['ivars'])
assert all(i['name']!='_mouseInside' for raw in layouts for r in raw.values() for i in r['ivars'])
write('evidence/metadata-keys.tsv',[dict(class_name=k[0],category=k[1],kind=k[2],selector=k[3],type_encoding=r['type_encoding'],status='binary170+protocol168/170+selected_revision_agree') for k,r in sorted(bykey.items())])

db=sqlite3.connect(f'file:{INP}/corpus-index.sqlite?mode=ro',uri=True)
bodychecks=[]
for va in sorted({int(r['imp_va'],16) for r in specs}):
    b=db.execute('select name,path,offset,length,failed from functions where va=?',(va,)).fetchone()
    bodychecks.append(dict(va=hex(va),name=b[0] if b else '',path=b[1] if b else '',offset=b[2] if b else '',length=b[3] if b else '',failed=b[4] if b else '',status='indexed' if b else 'no_exact_header'))
assert len(bodychecks)==1566 and sum(r['status']=='no_exact_header' for r in bodychecks)==1
assert all(not r['failed'] for r in bodychecks if r['status']=='indexed')
write('evidence/method-body-availability.tsv',bodychecks)

corpus=rows(AG/'abi/decompile/corpus-inventory.tsv'); assert len(corpus)==199
assert len({r['path'] for r in corpus})==len({r['sha256'] for r in corpus})==199
header=re.compile(rb'^// ==== (0x[0-9a-f]+)  (.*?)  \((\d+) bytes\)\s*$')
headers=0; shardchecks=[]
for r in corpus:
    p=Path(r['path']); h=hashlib.sha256(); count=0
    with p.open('rb') as f:
        for ln in f:
            h.update(ln)
            if header.match(ln): count+=1
    assert p.stat().st_size==int(r['bytes']) and h.hexdigest()==r['sha256'],str(p)
    headers+=count; shardchecks.append(dict(path=str(p),bytes=p.stat().st_size,sha256=h.hexdigest(),headers=count,status='PASS'))
assert headers==567959 and db.execute('select count(*) from functions').fetchone()[0]==headers
assert db.execute('select sum(failed) from functions').fetchone()[0]==79
write('evidence/corpus-checks.tsv',shardchecks)

callrows=rows(AG/'abi/initial/used-calls.tsv'); app=Image(INP/'binaries/Arc-1.168.0.arm64')
branchchecks=[]
for r in callrows:
    for col,op in (('binary_bl_vas',0x94000000),('binary_tail_b_vas',0x14000000)):
        for va in filter(None,r[col].split(';')):
            a=int(va,16); w=struct.unpack('<I',app.read(a,4))[0]; assert w&0xfc000000==op
            imm=w&0x3ffffff
            if imm&0x2000000: imm-=1<<26
            target=a+4*imm; assert target==int(r['app_stub_va'],16)
            branchchecks.append(dict(image='Arc-1.168.0.arm64',caller=r['app_caller_va'],va=va,file_offset=hex(app.offset(a,4)),word=hex(w),target=hex(target),selector=r['selector'],receiver_proven='no'))
write('evidence/app-branches.tsv',branchchecks)

protocols=rows(AG/'protocols/initial/protocols.tsv'); callbacks=rows(AG/'protocols/initial/callbacks.tsv'); sends=rows(AG/'protocols/initial/evidence/engine-all-static-sends.tsv')
assert len(protocols)==26 and len(callbacks)==200
cef=rows(AG/'cef/decompile/cef-mapping.tsv'); assert len(cef)==len({(r['class_name'],r['objc_category'],r['kind'],r['selector']) for r in cef})==834
matcher=rows(AG/'matcher/initial/matches.tsv')
summary=dict(status='PASS',selection_manifest_sha256=digest(WORK/'CLASS-REVISIONS-READY'),selected_hash_checks=len(hashchecks),classes=len(classes),method_rows=len(specs),unique_imps=len(bodychecks),indexed_method_imps=1565,missing_exact_method_header='0x4410bb4;B to 0x47390dc',groups=grouprows,original_findings=len(dispositions),fresh_dispositions=dict(C.Counter(r['fresh_status'] for r in dispositions)),rows_with_open_questions=sum(bool(r['unresolved_question']) for r in specs),explicit_unresolved_rows=sum(r['semantic_status']=='unresolved' for r in specs),shards=len(corpus),shard_bytes=sum(int(r['bytes']) for r in corpus),headers=headers,failed=79,successful_candidates=headers-79,app_used_call_rows=len(callrows),app_call_evidence=dict(C.Counter(r['evidence_kind'] for r in callrows)),app_branch_records=len(branchchecks),app_unique_branch_vas=len({r['va'] for r in branchchecks}),protocols=len(protocols),protocol_identities=len(callbacks),static_sends=len(sends),cef_classifications=dict(C.Counter(r['category'] for r in cef)),matcher_classifications=dict(C.Counter((r['scope'],r['status']) for r in matcher)),seconds=round(time.monotonic()-START,3),limits='Declaration/provenance/branch checks. No complete semantic, receiver, runtime, C++ ABI or readiness certification.')
summary['matcher_classifications']={str(k):v for k,v in summary['matcher_classifications'].items()}
(OUT/'evidence/audit.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps(summary,indent=2))
