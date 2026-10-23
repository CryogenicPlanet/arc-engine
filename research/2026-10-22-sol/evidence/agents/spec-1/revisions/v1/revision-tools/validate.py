#!/usr/bin/env python3
"""Validate revision inventory, immutable evidence, matrices and finding dispositions."""
import collections
import csv
import hashlib
import json
import os
from pathlib import Path
import re
import resource
import struct
import sys
import time

sys.dont_write_bytecode = True
from instructions import MachO
start = time.monotonic()
R = Path(__file__).resolve().parents[1]
O = Path('/home/exedev/work/agents/spec-1/specs')
V = Path('/home/exedev/work/agents/spec-4/classreview')
I = Path('/home/exedev/work/inputs')
def tsv(p):
    with p.open(newline='') as f: return list(csv.DictReader(f, delimiter='\t'))
def digest(p):
    h = hashlib.sha256()
    with p.open('rb') as f:
        for b in iter(lambda:f.read(1024*1024), b''): h.update(b)
    return h.hexdigest()
def identity(r): return tuple(r[k] for k in ['class_name','category','kind','selector'])
rows = tsv(R/'methods.tsv'); old = tsv(O/'methods.tsv')
assert len(rows) == len(old) == 491
assert len({identity(r) for r in rows}) == 491
immutable = ['class_name','category','kind','selector','type_encoding','imp_va','method_record_va','dyld_line']
assert [[r[k] for k in immutable] for r in rows] == [[r[k] for k in immutable] for r in old]
classes = sorted({r['class_name'] for r in rows})
assigned = json.loads((I/'context/class-assignments.json').read_text())['groups']['spec-1']['classes']
assert classes == sorted(assigned) and len(classes) == 34
assert sorted(p.stem for p in (R/'classes').glob('*.md')) == classes
assert sorted(p.name for p in (O/'classes').glob('*')) == sorted(p.name for p in (R/'classes').glob('*'))

# Check every declared method directly against its complete binary method list.
m = MachO('ArcCore-1.170.0.arm64')
assert digest(I/'binaries'/m.name) == json.loads((R/'evidence/input-hashes.json').read_text())['binaries/'+m.name]
def cstr(va):
    out = bytearray()
    for i in range(128):
        b = m.read(va+i*128, 128)
        if b'\0' in b: return (out+b.split(b'\0',1)[0]).decode()
        out.extend(b)
    raise ValueError('unbounded string')
binary = []
def methods(a, cls, cat, kind):
    if not a: return
    flags, n = struct.unpack('<II', m.read(a, 8))
    stride = flags & 65535
    assert stride in (12,24) and n < 10000
    for j in range(n):
        rec = a+8+j*stride
        if flags & 0x80000000:
            x,y,z = struct.unpack('<iii', m.read(rec,12))
            sel, ty, imp = rec+x, rec+4+y, rec+8+z
            if not flags & 0x40000000: sel = m.ptr(sel)
        else:
            sel,ty,imp = [m.ptr(rec+k) for k in (0,8,16)]
        binary.append(dict(class_name=cls, category=cat, kind=kind, selector=cstr(sel),
                           type_encoding=cstr(ty), imp_va=hex(imp), method_record_va=hex(rec)))
layouts = json.loads((R/'evidence/layout.json').read_text())
class_by_va = {}
for lay in layouts:
    cls = lay['class_name']; va = int(lay['metadata_va'],16)
    class_by_va[va] = cls
    ro = m.ptr(va+32) & ~7
    assert ro == int(lay['ro_va'],16) and cstr(m.ptr(ro+24)) == cls
    assert struct.unpack('<II',m.read(ro+4,8)) == (lay['instance_start'],lay['instance_size'])
    methods(m.ptr(ro+32),cls,'','-')
    meta = m.ptr(va); meta_ro = m.ptr(meta+32) & ~7
    methods(m.ptr(meta_ro+32),cls,'','+')
catva, catsize, _ = m.sections['__objc_catlist']
for j in range(catsize//8):
    cat = m.ptr(catva+j*8)
    rawclass = struct.unpack('<Q',m.read(cat+8,8))[0]
    if rawclass >> 63: continue
    cls = class_by_va.get(m.ptr(cat+8))
    if cls:
        name = cstr(m.ptr(cat))
        methods(m.ptr(cat+16),cls,name,'-')
        methods(m.ptr(cat+24),cls,name,'+')
fields = immutable[:-1]
assert sorted(tuple(x[k] for k in fields) for x in binary) == sorted(tuple(x[k] for k in fields) for x in rows)

# Preserved copied evidence and historical input snapshots must be byte-identical.
preserved = {}
for dirname in ['bodies','evidence','sources']:
    original_files = sorted(p.relative_to(O) for p in (O/dirname).rglob('*') if p.is_file())
    revision_files = sorted(p.relative_to(R) for p in (R/dirname).rglob('*') if p.is_file())
    assert original_files == revision_files
    for rel in original_files: assert digest(R/rel) == digest(O/rel), str(rel)
    preserved[dirname] = len(original_files)
assert (R/'original-report.md').read_bytes() == (O/'report.md').read_bytes()
assert (R/'artifacts.sha256').read_bytes() == (O/'artifacts.sha256').read_bytes()
for line in (O/'artifacts.sha256').read_text().splitlines():
    sha, path = line.split('  ',1)
    rel = Path(path).relative_to('specs')
    assert digest(O/rel) == sha, 'Original snapshot changed: '+str(rel)
for snapshot in tsv(V/'evidence/writer-snapshot.tsv'):
    assert digest(Path(snapshot['path'])) == snapshot['sha256']

lookup = {identity(r):r for r in rows}
matrix_count = 0
for cls in classes:
    seen = []
    p = R/'classes'/f'{cls}.md'
    for line in p.read_text().splitlines():
        if not line.startswith('| '): continue
        col = [v.strip() for v in line.split('|')[1:-1]]
        if len(col) != 9 or not col[1].startswith(('-','+')): continue
        ident = (cls, '' if col[0] == '(base)' else col[0], col[1][0], col[1][1:])
        r = lookup[ident]
        assert col[2].strip('`') == r['type_encoding'] and col[3] == r['imp_va']
        # Existing matrices display this explicit caveat for a blank TSV question.
        expected_question = r['unresolved_question'] or 'No additional local question; runtime parity untested.'
        assert col[4] == r['behavior'] and col[8] == expected_question, ident
        assert col[7] == f"{r['confidence']} / {r['semantic_status']} / {r['body_status']}"
        seen.append(ident)
    assert collections.Counter(seen) == collections.Counter(identity(r) for r in rows if r['class_name'] == cls)
    matrix_count += len(seen)
    for target in re.findall(r'\]\(([^)]+)\)', p.read_text()):
        if target.startswith(('http:','https:')): continue
        assert (p.parent/target.split('#')[0]).is_file(), target

findings = tsv(V/'findings.tsv'); corrections = tsv(R/'corrections.tsv')
assert len(findings) == len(corrections) == 4
changed = [(a,b) for a,b in zip(old,rows) if a != b]
assert len(changed) == 4
for n,(finding,fix) in enumerate(zip(findings,corrections),1):
    assert fix['finding_row'] == str(n) and fix['class_name'] == finding['class'] and fix['selector'] == finding['selector']
    matches = [(a,b) for a,b in changed if a['class_name'] == fix['class_name'] and a['selector'] == fix['selector']]
    assert len(matches) == 1
    a,b = matches[0]
    assert fix['before'] == a['behavior'] == finding['claim'] and fix['after'] == b['behavior']
    assert fix['disposition'] == 'corrected' and fix['evidence'] and fix['remaining_unknown']
assert not any(r['selector'] == '.cxx_destruct' and 'supplied value' in r['behavior'] for r in rows)
assert not any(r['type_encoding'].startswith('{optional') and 'traps when the backing optional is empty' in r['behavior'] for r in rows)
coverage = json.loads((R/'coverage.json').read_text())
for key,col in [('body_status','body_status'),('semantic_status','semantic_status')]:
    assert coverage[key] == dict(collections.Counter(r[col] for r in rows))
    for cls in classes:
        subset = [r for r in rows if r['class_name'] == cls]
        assert coverage['by_class'][cls][key] == dict(collections.Counter(r[col] for r in subset))
        assert coverage['by_class'][cls]['method_rows'] == len(subset)
counts = dict(assigned_class_count=34,generated_class_count=34,assigned_method_rows=491,actual_method_rows=491,
              method_identity_unique=len(lookup),base_method_rows=sum(not r['category'] for r in rows),
              category_method_rows=sum(bool(r['category']) for r in rows),class_method_rows=sum(r['kind']=='+' for r in rows),
              instance_method_rows=sum(r['kind']=='-' for r in rows),distinct_imp_vas=len({r['imp_va'] for r in rows}),
              body_available_rows=sum(r['body_status'].startswith('available') for r in rows),
              failed_rows=0,missing_indexed_body_rows=sum(r['body_status'].startswith('missing') for r in rows),
              semantic_unresolved_rows=sum(r['semantic_status']=='unresolved' for r in rows),
              rows_with_open_question=sum(bool(r['unresolved_question']) for r in rows))
for k,v in counts.items(): assert coverage[k] == v, (k,v)
assert coverage['full_end_to_end_semantics_verified'] == 0
assert coverage['chromium_commit'] == 'd0c02bfad08ec92a6ffcf989e53cd995e6381a4c'
assert coverage['cef_commit'] == '426920c33ef22551e0b8f48cf343d623ac25642d'
assert json.loads((R/'revision-evidence/finding-checks.json').read_text())['result'] == 'pass'
result = dict(result='pass', image=m.name, image_sha256=digest(I/'binaries'/m.name),
              address_space='unslid virtual addresses', counts=counts,
              complete_binary_method_records_checked=len(binary), matrix_rows_checked=matrix_count,
              original_findings=4,correction_dispositions=4,changed_annotation_rows=len(changed),
              preserved_files=preserved,original_snapshot_hashes='pass',relative_markdown_links='pass',
              semantic_status=coverage['semantic_status'],body_status=coverage['body_status'],
              resources=dict(cpu_affinity=list(os.sched_getaffinity(0)),max_rss_kib=resource.getrusage(resource.RUSAGE_SELF).ru_maxrss,
                             elapsed_seconds=round(time.monotonic()-start,3)),
              limits='Static inventory/integrity and targeted instruction checks; no runtime or full behavioral certification')
(R/'revision-evidence/validation.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
