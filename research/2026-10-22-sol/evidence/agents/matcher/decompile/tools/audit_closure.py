#!/usr/bin/env python3
"""Read-only, bounded corpus/closure audit. No size field is treated as coverage."""
import collections
import csv
import hashlib
import json
import re
import sqlite3
from pathlib import Path

ROOT = Path('/home/exedev/work/inputs')
OUT = Path(__file__).resolve().parents[1]
DB = sqlite3.connect(f'file:{ROOT}/corpus-index.sqlite?mode=ro', uri=True)
DB.execute('PRAGMA cache_size=-8192')
CALL = re.compile(r'\b(FUN_[0-9a-f]{8})\s*\(')
STR = re.compile(r'"([^"\n]{6,220})"')
EXPORTS = {'_ChromeMain', '_ChromeAppModeStart_v8', '_ChromeWebAppShortcutCopierMain',
           '_SetLogMessageHandler', '_CallWithEHFrame', '_emitTraceEvent',
           '_arcCategoryEnabledFlag', '_ArcErrorDomain'}
KNOWN_BAD = 0x421f778

def write_tsv(name, fields, rows):
    with (OUT / name).open('w') as f:
        w = csv.writer(f, delimiter='\t', lineterminator='\n')
        w.writerow(fields)
        w.writerows(rows)

def digest(path):
    h = hashlib.sha256()
    with path.open('rb') as f:
        for b in iter(lambda: f.read(1024*1024), b''): h.update(b)
    return h.hexdigest()

def body(row):
    va, name, size, path, off, length, failed = row
    with open(path, 'rb') as f:
        f.seek(off)
        return f.read(length).decode('utf-8', 'replace')

inventory = []
for directory in [ROOT / 'decompile', *sorted((ROOT / 'decompile-extra').glob('batch-*'))]:
    assert (directory/'READY').is_file()
    for item in json.loads((directory/'MANIFEST.json').read_text()):
        path = directory / item['file']
        actual = digest(path)
        assert path.stat().st_size == item['bytes'] and actual == item['sha256'], path
        inventory.append((str(path), item['bytes'], actual))
write_tsv('inventory.tsv', ['path', 'bytes', 'sha256_verified'], inventory)

old = []
for line in (ROOT/'claude-handoff/results/arc-layer-functions.tsv').read_text().splitlines():
    n, d, r, s = line.split('\t')
    old.append((n, int(d), r, int(s)))
oldnames = {x[0] for x in old}
by_old = collections.defaultdict(list)
arc_by_trim = collections.defaultdict(list)
seeds = {}
meta = {}
seed_calls = {}
all_counts = collections.Counter()
warnings = []
fh = None
current = None
# Natural VA order means shard files are read sequentially, bounded by one body.
for row in DB.execute('SELECT * FROM functions ORDER BY va'):
    va, n, size, path, off, length, failed = row
    all_counts['headers'] += 1
    all_counts['failed'] += bool(failed)
    if current != path:
        if fh: fh.close()
        fh = open(path, 'rb'); current = path
    fh.seek(off)
    b = fh.read(length).decode('utf-8', 'replace')
    assert b.startswith(f'// ==== 0x{va:x}  {n}  ({size} bytes)'), (va, n)
    trimmed = n.rstrip(':')
    if trimmed in oldnames: by_old[trimmed].append(row)
    if re.match(r'^Arc[A-Za-z0-9_]*::', n):
        reason = 'arc-objc'
        arc_by_trim[trimmed].append((va,n))
    elif n in EXPORTS: reason = 'export'
    elif '../../arc/' in b or 'arc.mojom' in b: reason = 'arc-path'
    else: reason = None
    if reason:
        seeds[va] = reason
        meta[va] = row
        seed_calls[va] = set(CALL.findall(b.split('\n', 2)[2])) - {n}
    # These are triage flags only. Distant labels/jumptable addresses can also
    # be valid outlined/shared blocks. They do not prove malformed boundaries.
    if trimmed in oldnames:
        code_addresses = [int(x,16) for x in re.findall(r'^\s*(?:LAB_|switchD_)([0-9a-f]{8})[^\n]*:', b, re.M)]
        delta = max((abs(x-va) for x in code_addresses), default=0)
        warn = re.findall(r'/\* WARNING[^*]*\*/', b)
        if failed or delta > 0x100000 or 'Bad instruction' in b or va == KNOWN_BAD:
            warnings.append((hex(va), n, failed, size, length, hex(delta),
                             json.dumps(warn[:12]), 'excluded_known_malformed' if va==KNOWN_BAD else 'review_not_proof'))
if fh: fh.close()

# Reproduce name-normalization losses rather than silently adopting them.
crosswalk = []
for n,d,r,s in old:
    candidates = by_old[n]
    for row in candidates or [(None,None,None,None,None,None,None)]:
        va, exact, hs, path, off, length, failed = row
        crosswalk.append((n,d,r,s,hex(va) if va is not None else '', exact,hs,path,off,length,failed,
                          'missing' if not candidates else 'ambiguous_trim_collision' if len(candidates)>1 else 'unique'))
write_tsv('handoff-crosswalk.tsv', ['handoff_name','depth','reason','handoff_size_not_coverage',
          'va','exact_header_name','header_size_not_coverage','path','offset','C_bytes','failed','status'], crosswalk)
write_tsv('malformed-triage.tsv', ['va','name','failed','header_size_not_coverage','C_bytes',
          'max_label_distance','warnings_json','disposition'], warnings)

# Independent address-keyed reconstruction of the same syntactic 2-level rule.
# Known malformed seed is removed BEFORE expanding it. Direct FUN_ calls only;
# no claim to capture ObjC sends, virtual calls, callbacks, or function pointers.
depth = {va:0 for va in seeds if va != KNOWN_BAD}
front = list(depth)
edges = []
missing = {}
for d in (1,2):
    nxt = []
    for va in front:
        row = meta[va]
        calls = seed_calls[va] if va in seed_calls else set(CALL.findall(body(row).split('\n',2)[2])) - {row[1]}
        for call in sorted(calls):
            cv = int(call[4:],16)
            edges.append((hex(va), row[1], hex(cv), call, d))
            if cv == KNOWN_BAD: continue
            if cv not in meta:
                cr = DB.execute('SELECT * FROM functions WHERE va=?',(cv,)).fetchone()
                if cr is None:
                    missing[cv] = call
                    continue
                meta[cv] = cr
            if cv not in depth:
                depth[cv] = d; nxt.append(cv)
    front = nxt
write_tsv('reconstructed-edges.tsv', ['caller_va','caller_name','callee_va','callee_token','expansion_level'], edges)
write_tsv('missing-bodies.tsv', ['va','name','status'], [(hex(v),n,'no_indexed_body') for v,n in sorted(missing.items())])
rows = []
anchors = []
for va,d in sorted(depth.items(),key=lambda x:(x[1],x[0])):
    row = meta[va]; _,n,size,p,off,length,failed = row
    rows.append((hex(va),n,d,seeds.get(va,'callee'),size,length,failed,p,off))
    b = body(row)
    ss = list(dict.fromkeys(STR.findall(b)))
    paths = [s for s in ss if '../../' in s]
    salient = [s for s in ss if '../../' not in s and not s.startswith(('virtual ', 'std::'))]
    if paths or salient:
        anchors.append((hex(va),n,d,seeds.get(va,'callee'),json.dumps(paths),json.dumps(salient[:24])))
write_tsv('reconstructed-closure.tsv', ['va','name','syntactic_depth','seed_reason','header_size_not_coverage',
          'C_bytes','failed','path','offset'],rows)
write_tsv('closure-anchors.tsv',['va','name','depth','reason','paths_json','other_strings_json'],anchors)

old_unique = {r[0] for rows in by_old.values() for r in rows}
count = collections.Counter((d,seeds.get(v,'callee')) for v,d in depth.items())
summary = {
 'input_hashes':{str(p):digest(p) for p in [ROOT/'corpus-index.sqlite',ROOT/'metadata/decomp/arccore-1.170.0-functions.tsv',ROOT/'claude-handoff/results/arc-layer-functions.tsv',ROOT/'claude-handoff/scripts/arc_layer.py']},
 'input_corpus': dict(all_counts), 'manifest_entries_verified':len(inventory),
 'unique_shards':len({Path(p).name for p,_,_ in inventory}),
 'manifest_bytes_streamed':sum(n for _,n,_ in inventory),
 'handoff_rows':len(old),'handoff_reason_counts':dict(collections.Counter(r for n,d,r,s in old)),
 'handoff_size_sum_invalid_as_coverage':sum(s for n,d,r,s in old),
 'handoff_size_sum_excluding_known_bad_still_invalid':sum(s for n,d,r,s in old if n!='FUN_0421f778'),
 'zero_size_handoff_rows':sum(s==0 for n,d,r,s in old),
 'unresolved_handoff_names':[n for n,d,r,s in old if not by_old[n]],
 'ambiguous_handoff_names':{n:[hex(row[0]) for row in rr] for n,rr in by_old.items() if len(rr)>1},
 'distinct_addresses_represented_by_handoff':len(old_unique),
 'normalized_arc_method_collisions':{n:[(hex(v),name) for v,name in rr] for n,rr in arc_by_trim.items() if len(rr)>1},
 'exact_seed_counts':dict(collections.Counter(seeds.values())),
 'reconstructed_counts':{f'{d}:{r}':n for (d,r),n in count.items()},
 'reconstructed_addresses':len(depth),'syntactic_edges':len(edges),'missing_callee_bodies':len(missing),
 'new_addresses_vs_handoff': [hex(v) for v in sorted(set(depth)-old_unique)],
 'handoff_addresses_not_reconstructed':[hex(v) for v in sorted(old_unique-set(depth))],
 'malformed_triage_rows':len(warnings),
 'limits':['header machine sizes are not independently verified ranges',
           'syntactic FUN_ calls include possible decompiler artifacts and exclude all indirect dispatch',
           'known malformed 0x421f778 excluded before expansion; other flags require review',
           'no proprietary-code, edit count, ABI equivalence, or executable-coverage measure']}
(OUT/'audit.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps({k:v for k,v in summary.items() if k not in ('normalized_arc_method_collisions','ambiguous_handoff_names','new_addresses_vs_handoff','handoff_addresses_not_reconstructed')},indent=2))
