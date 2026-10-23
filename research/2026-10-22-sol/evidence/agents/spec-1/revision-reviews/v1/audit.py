"""Fresh read-only revision inventory; outputs confined beside this script.

Mach-O layout interpretation was cross-checked against the completed review's
read-only macho.py; neither reviewer nor writer programs are imported/executed.
"""
import collections, csv, ctypes, difflib, hashlib, json, os, re, sqlite3, struct, subprocess
from pathlib import Path

os.sched_setaffinity(0, {min(os.sched_getaffinity(0))})
W = Path('/home/exedev/work')
OUT = Path(__file__).resolve().parent
REV = W/'agents/spec-1/revisions/v1'
OLD = W/'agents/spec-1/specs'
PRIOR = W/'agents/spec-4/classreview'
IN = W/'inputs'
EV = OUT/'evidence'
EV.mkdir(exist_ok=True)

def digest(p):
    h = hashlib.sha256()
    with p.open('rb') as f:
        for b in iter(lambda: f.read(65536), b''): h.update(b)
    return h.hexdigest()

def table(p, rows, fields=None):
    with p.open('w') as f:
        wr = csv.DictWriter(f, fields or list(rows[0]), delimiter='\t')
        wr.writeheader(); wr.writerows(rows)

def readtsv(p):
    with p.open() as f: return list(csv.DictReader(f, delimiter='\t'))

def snap():
    return {str(p): digest(p) for root in (OLD, PRIOR, REV) for p in sorted(root.rglob('*')) if p.is_file()}

class Image:
    def __init__(self):
        self.f = (IN/'binaries/ArcCore-1.170.0.arm64').open('rb')
        header = self.at(0, 32)
        assert struct.unpack_from('<I', header)[0] == 0xfeedfacf
        self.segments, self.sections, self.commands = [], {}, {}
        pos = 32
        for _ in range(struct.unpack_from('<I', header, 16)[0]):
            cmd, size = struct.unpack('<II', self.at(pos, 8)); d = self.at(pos, size)
            self.commands[cmd] = d
            if cmd == 0x19:
                name, va, vs, off, fs, _, _, ns, _ = struct.unpack_from('<16sQQQQiiII', d, 8)
                self.segments.append((name.rstrip(b'\0').decode(), va, fs, off))
                for i in range(ns):
                    name, _, a, n, off = struct.unpack_from('<16s16sQQI', d, 72+80*i)
                    self.sections[name.rstrip(b'\0').decode()] = (a, n)
            pos += size
        self.fix = struct.unpack_from('<I', self.commands[0x80000034], 8)[0]
        self.fix_header = struct.unpack('<7I', self.at(self.fix, 28))
        _, starts, _, _, _, _, _ = self.fix_header
        base = self.fix+starts
        formats = set()
        for i in range(struct.unpack('<I', self.at(base, 4))[0]):
            delta = struct.unpack('<I', self.at(base+4+4*i, 4))[0]
            if delta: formats.add(struct.unpack('<H', self.at(base+delta+6, 2))[0])
        assert formats == {2}

    def at(self, off, n):
        self.f.seek(off); d = self.f.read(n); assert len(d) == n; return d
    def off(self, a, n=1):
        for _, start, size, off in self.segments:
            if start <= a and a+n <= start+size: return a-start+off
        raise ValueError(hex(a))
    def read(self, a, n): return self.at(self.off(a, n), n)
    def u32(self, a): return struct.unpack('<I', self.read(a, 4))[0]
    def ptr(self, a):
        raw = struct.unpack('<Q', self.read(a, 8))[0]
        if raw >> 63: return 0  # Imported class/category target, no local class record.
        return (raw & ((1 << 36)-1)) | (((raw >> 36) & 255) << 56)
    def string(self, a): return self.read(a, 16384).split(b'\0', 1)[0].decode()
    def section(self, name):
        a, n = self.sections[name]; return [self.ptr(a+i) for i in range(0, n, 8)]
    def ro(self, cls): return self.ptr(cls+32) & ~7
    def name(self, cls): return self.string(self.ptr(self.ro(cls)+24)) if cls else ''
    def methods(self, a, name, category, kind):
        if not a: return []
        flags, count = struct.unpack('<II', self.read(a, 8)); stride = flags & 65535
        assert stride in (12, 24) and count < 10000
        rows = []
        for i in range(count):
            rec = a+8+stride*i
            if flags & 0x80000000:
                x, y, z = struct.unpack('<iii', self.read(rec, 12)); sel = rec+x
                if not flags & 0x40000000: sel = self.ptr(sel)
                ty, imp = rec+4+y, rec+8+z
            else: sel, ty, imp = [self.ptr(rec+j) for j in (0, 8, 16)]
            rows.append(dict(class_name=name, category=category, kind=kind,
                             selector=self.string(sel), type_encoding=self.string(ty),
                             imp_va=hex(imp), method_record_va=hex(rec)))
        return rows

def main():
    baseline = snap()
    (EV/'baseline-sha256.json').write_text(json.dumps(baseline, indent=2)+'\n')
    manifests = []
    for root, filename in ((OLD, 'artifacts.sha256'), (REV, 'revision-artifacts.sha256')):
        for line in (root/filename).read_text().splitlines():
            want, rel = line.split(None, 1)
            p = (root.parent if root == OLD else root)/rel.strip()
            assert digest(p) == want, p
            manifests.append(dict(manifest=str(root/filename), path=str(p), sha256=want))
    table(EV/'manifest-checks.tsv', manifests)
    for r in readtsv(PRIOR/'evidence/writer-snapshot.tsv'):
        assert digest(Path(r['path'])) == r['sha256'], r
    same = []
    for subdir in ('bodies', 'evidence', 'sources'):
        oldfiles = sorted(p.relative_to(OLD) for p in (OLD/subdir).rglob('*') if p.is_file())
        assert set(oldfiles) == {p.relative_to(REV) for p in (REV/subdir).rglob('*') if p.is_file()}
        for rel in oldfiles:
            assert digest(OLD/rel) == digest(REV/rel), rel
            same.append(dict(path=str(rel), sha256=digest(REV/rel)))
    table(EV/'preserved-evidence.tsv', same)
    classes = json.loads((IN/'context/class-assignments.json').read_text())['groups']['spec-1']['classes']
    assert {p.stem for p in (REV/'classes').glob('*.md')} == set(classes)
    rows, before = readtsv(REV/'methods.tsv'), readtsv(OLD/'methods.tsv')
    key = lambda r: tuple(r[k] for k in ('class_name','category','kind','selector'))
    assert list(map(key, rows)) == list(map(key, before)) and len(set(map(key, rows))) == 491
    changed = []
    for a, b in zip(before, rows):
        for f in ('type_encoding','imp_va','method_record_va','dyld_line'): assert a[f] == b[f]
        if a != b: changed.append(dict(class_name=b['class_name'], selector=b['selector'], changed_fields=','.join(k for k in b if a[k]!=b[k]), before=a['behavior'], after=b['behavior']))
    table(EV/'changed-methods.tsv', changed)
    assert len(changed) == 4
    diff = []
    for p in sorted((REV/'classes').glob('*.md')):
        rel = p.relative_to(REV)
        diff.extend(difflib.unified_diff((OLD/rel).read_text().splitlines(True), p.read_text().splitlines(True), fromfile=str(OLD/rel), tofile=str(p)))
    (EV/'class-changes.diff').write_text(''.join(diff))
    im = Image(); binary = []; classmap = {}
    for cls in im.section('__objc_classlist'):
        name = im.name(cls)
        if name not in classes: continue
        ro = im.ro(cls); classmap[name] = (cls, ro)
        for x, kind in ((cls,'-'), (im.ptr(cls),'+')):
            binary.extend(im.methods(im.ptr(im.ro(x)+32), name, '', kind))
    for cat in im.section('__objc_catlist'):
        name = im.name(im.ptr(cat+8))
        if name in classes:
            for off, kind in ((16,'-'), (24,'+')):
                binary.extend(im.methods(im.ptr(cat+off), name, im.string(im.ptr(cat)), kind))
    assert len(binary) == len(rows) == 491 and set(classmap) == set(classes)
    bmap = {key(r):r for r in binary}; assert set(bmap) == set(map(key, rows))
    for r in rows:
        for field in bmap[key(r)]: assert r[field] == bmap[key(r)][field], (key(r),field)
    dyld = {}
    owner = cat = ''
    for no, line in enumerate((IN/'context/analysis/data/dobjc170.txt').open(), 1):
        found = re.search(r'@interface (\w+)(?:\(([^)]+)\))?', line)
        if found: owner, cat = found[1], found[2] or ''
        found = re.search(r'(0x[0-9A-Fa-f]+)\s+([+-])\[([^ ]+) ([^]]+)\]', line)
        if found and found[3] in classes:
            imp, kind, name, sel = found.groups(); k=(name,cat,kind,sel)
            assert k not in dyld; dyld[k]=(hex(int(imp,16)),no)
    assert set(dyld) == set(bmap)
    for r in rows: assert dyld[key(r)] == (r['imp_va'],int(r['dyld_line']))
    layouts = json.loads((REV/'evidence/layout.json').read_text())
    for layout in layouts:
        cls, ro = classmap[layout['class_name']]
        assert (layout['metadata_va'],layout['ro_va'],layout['instance_start'],layout['instance_size']) == (hex(cls),hex(ro),im.u32(ro+4),im.u32(ro+8))
        ivars, props = [], []
        a = im.ptr(ro+48)
        if a:
            stride,n = struct.unpack('<II',im.read(a,8)); assert stride==32
            for i in range(n):
                rec=a+8+stride*i
                ivars.append(dict(record_va=hex(rec),offset=im.u32(im.ptr(rec)),name=im.string(im.ptr(rec+8)),type=im.string(im.ptr(rec+16)),alignment=im.u32(rec+24),size=im.u32(rec+28)))
        a = im.ptr(ro+64)
        if a:
            stride,n = struct.unpack('<II',im.read(a,8)); assert stride==16
            for i in range(n):
                rec=a+8+stride*i
                props.append(dict(record_va=hex(rec),name=im.string(im.ptr(rec)),attributes=im.string(im.ptr(rec+8))))
        assert ivars==layout['ivars'] and props==layout['properties']
    matrix_counts={}
    for cls in classes:
        cells=[]
        for line in (REV/'classes'/f'{cls}.md').read_text().splitlines():
            if not line.startswith('| '): continue
            cols=[c.strip() for c in line.split('|')[1:-1]]
            if len(cols)!=9 or cols[1][:1] not in ('-','+'): continue
            cat,ks,enc,imp,beh,backend,ev,status,question=cols
            k=(cls,'' if cat=='(base)' else cat,ks[0],ks[1:]); cells.append(k)
            r=next(r for r in rows if key(r)==k)
            assert (enc.strip('`'),imp,beh,backend,status,question)==(r['type_encoding'],r['imp_va'],r['behavior'],(r['backend_callees'] or 'No direct anonymous call recorded / indirect possible').strip(),f"{r['confidence']} / {r['semantic_status']} / {r['body_status']}",r['unresolved_question'] or 'No additional local question; runtime parity untested.'),k
        assert collections.Counter(cells)==collections.Counter(key(r) for r in rows if r['class_name']==cls)
        matrix_counts[cls]=len(cells)
    db=sqlite3.connect(f'file:{IN}/corpus-index.sqlite?mode=ro',uri=True)
    db.execute('PRAGMA cache_size=-4096')
    checks=[]
    for va in sorted({int(r['imp_va'],16) for r in rows}):
        x=db.execute('select name,path,offset,length,failed from functions where va=?',(va,)).fetchone()
        if not x:
            assert va==0x4410bb4
            checks.append(dict(imp_va=hex(va),status='no exported body',name='',path='',offset='',length='',sha256='')); continue
        name,path,off,n,failed=x; assert not failed and n<=65536
        with open(path,'rb') as f: f.seek(off); body=f.read(n)
        assert int(re.match(rb'// ==== (0x[0-9a-f]+)',body)[1],16)==va
        assert body==(REV/'bodies'/f'{hex(va)}.c').read_bytes()
        checks.append(dict(imp_va=hex(va),status='exact original bytes',name=name,path=path,offset=off,length=n,sha256=hashlib.sha256(body).hexdigest()))
    table(EV/'body-identity-checks.tsv',checks)
    table(EV/'binary-method-checks.tsv',binary)
    cov=json.loads((REV/'coverage.json').read_text())
    computed=dict(assigned_class_count=len(classes),generated_class_count=len(classmap),actual_method_rows=len(rows),method_identity_unique=len(bmap),base_method_rows=sum(not r['category'] for r in rows),category_method_rows=sum(bool(r['category']) for r in rows),class_method_rows=sum(r['kind']=='+' for r in rows),instance_method_rows=sum(r['kind']=='-' for r in rows),distinct_imp_vas=len(checks),body_available_rows=sum(r['imp_va']!='0x4410bb4' for r in rows),body_status=dict(collections.Counter(r['body_status'] for r in rows)),semantic_status=dict(collections.Counter(r['semantic_status'] for r in rows)))
    for k,v in computed.items(): assert cov[k]==v,(k,cov[k],v)
    original_findings=readtsv(PRIOR/'findings.tsv'); corr=readtsv(REV/'corrections.tsv')
    assert len(original_findings)==len(corr)==4
    for i,(f,c) in enumerate(zip(original_findings,corr),1):
        assert (str(i),f['class'],f['selector'],f['claim'])==(c['finding_row'],c['class_name'],c['selector'],c['before'])
        r=next(r for r in rows if r['class_name']==c['class_name'] and r['selector']==c['selector'])
        assert c['after']==r['behavior'] and c['remaining_unknown']==r['unresolved_question']
    sources=[]
    for filename,project,pin in [('manifest.json','chromium','d0c02bfad08ec92a6ffcf989e53cd995e6381a4c'),('cef-manifest.json','cef','426920c33ef22551e0b8f48cf343d623ac25642d')]:
        for entry in json.loads((REV/'sources'/filename).read_text()):
            if 'sha256' not in entry: continue  # Historical fetch failures do not prove absence.
            assert entry['commit']==pin
            p=REV/'sources'/project/entry['path']; assert digest(p)==entry['sha256']
            sources.append(dict(project=project,pin=pin,path=str(p),sha256=digest(p)))
    extra=IN/'chromium-156/content/browser/renderer_host/render_widget_host_view_mac.mm'
    sources.append(dict(project='chromium',pin='d0c02bfad08ec92a6ffcf989e53cd995e6381a4c',path=str(extra),sha256=digest(extra)))
    assert sources[-1]['sha256']=='17b9ac1271936dc3dcc2840544c556edf680cd5c54642115c5c8c24fd6b650df'
    table(EV/'source-hash-checks.tsv',sources)
    summary=dict(result='pass',coverage=computed,matrix_counts=matrix_counts,changed_method_rows=len(changed),original_findings=len(original_findings),unchanged_evidence_counts=dict(collections.Counter(Path(r['path']).parts[0] for r in same)),manifest_entries=len(manifests),body_checks=len(checks),source_hash_checks=len(sources),snapshot_files=len(baseline),address_identity='ArcCore 1.170.0 arm64, unslid virtual addresses; Chromium 156.0.8078.48')
    (EV/'audit.json').write_text(json.dumps(summary,indent=2)+'\n')
    assert baseline==snap(),'read-only snapshots changed during audit'
    print(json.dumps(summary,indent=2))

if __name__=='__main__': main()
