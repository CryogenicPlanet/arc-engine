#!/usr/bin/env python3
"""Bounded read-only evidence extraction. Run under the shared heavy.lock."""
import bisect, collections, csv, hashlib, json, re, sqlite3
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'decompile'
INP = Path('/home/exedev/work/inputs')
DB = sqlite3.connect(f'file:{INP}/corpus-index.sqlite?mode=ro', uri=True)

def tsv(path, rows, fields=None):
    rows = list(rows)
    with path.open('w') as f:
        w = csv.DictWriter(f, fieldnames=fields or list(rows[0]), delimiter='\t')
        w.writeheader(); w.writerows(rows)

def digest(path):
    h = hashlib.sha256()
    with path.open('rb') as f:
        for b in iter(lambda: f.read(1024*1024), b''): h.update(b)
    return h.hexdigest()

manifests = [INP/'decompile/MANIFEST.json', *sorted(INP.glob('decompile-extra/batch-*/MANIFEST.json'))]
shards = []; hashes = {}
for manifest in manifests:
    assert (manifest.parent/'READY').exists()
    for r in json.loads(manifest.read_text()):
        path = manifest.parent/r['file']
        sha = digest(path)
        assert sha == r['sha256'] and path.stat().st_size == r['bytes'], path
        hashes[str(path)] = sha
        shards.append(dict(path=str(path), bytes=r['bytes'], sha256=sha, manifest=str(manifest)))
assert len({r['sha256'] for r in shards}) == 199
tsv(OUT/'evidence/shards.tsv', shards)

targets = {}
for filename in ['decompile-requests.tsv', 'decompile-followups.tsv']:
    for r in csv.DictReader((ROOT/'initial'/filename).open(), delimiter='\t'):
        targets[int(r['engine170_imp_va'],16)] = r['question']
extra = {
    0x7cd94c0:'Toolbar visibility operation', 0x7cd9ea0:'Pinned membership',
    0x7cd9f40:'Pinned order operation', 0x42b3f34:'Profile callback dispatch',
    0x42e712c:'Extension callback fan-out', 0x78b2e84:'Password saving delegate',
    0x7d64a84:'Password save statistics', 0x7d6427c:'Dismissal reason',
    0x40991c0:'Password UI state', 0x7d64470:'Password profile lookup',
    0x2d0fb40:'Permission additionalData base IMP alias', 0x440a85c:'Permission category dictionary',
    0x6846c88:'Compositor fallback transfer', 0x68549b0:'Native bounds forwarding',
    0x6854a88:'Native focus forwarding', 0x2bd55c0:'Native host update',
    0x22f2830:'Native window propagation', 0x836a940:'Password importer wrapper',
    0x473c4dc:'Native host construction', 0x473c9a8:'Native host validity',
    0x473cad8:'Native fallback selection', 0x473cf0c:'Native geometry adapter',
    0x473d7e4:'Native focus adapter', 0x42bdf88:'Restore implementation',
    0x42f8300:'Browser construction', 0x42bf5ac:'Profile callback binding',
    0x7bddaf0:'Profile callback destructor', 0x7bdba94:'Profile async bound invocation',
    0x43eb7a0:'ObjC profile completion', 0x43fec84:'ObjC extension completion',
    0x43e5fd4:'ObjC browser autocomplete completion: result conversion, invocation and executor',
    0x784a050:'Start extension install',
    0x43e1f80:'Actual app-facing browser autocomplete selector',
    0x42fbd80:'Actual browser autocomplete backend',
    0x43f5f60:'Context autocomplete factory category IMP',
    0x42c2f5c:'Context autocomplete factory backend',
    0x43fdd3c:'Pinned extension order ObjC adapter',
    0x786cf44:'Prompt creation slot in stock installer vtable',
    0x614600:'Return-zero replacement slot in unattended installer vtable',
    0x786cf60:'Normal installer vtable slot 11: recover install UI body and boundary',
    0x4315348:'Unattended installer vtable slot 11: recover custom install UI body and boundary',
}
targets.update(extra)
rows=[]; missing=[]
for va, question in sorted(targets.items()):
    r=DB.execute('SELECT name,path,offset,length,failed,reported_size FROM functions WHERE va=?',(va,)).fetchone()
    if not r:
        missing.append(dict(engine_va=hex(va),status='no_exact_export',question=question,
                            next_action='Inspect binary thunk/function boundary; corpus presence is not required for a thunk.'))
        continue
    name,path,offset,length,failed,size=r
    with open(path,'rb') as f:
        f.seek(offset); b=f.read(min(length,65536))
    output=OUT/'functions'/f'{va:x}.c'; output.write_bytes(b)
    body=b.decode(errors='replace')
    rows.append(dict(engine_va=hex(va),name=name,question=question,path=path,offset=offset,
                     pseudocode_bytes=length,extracted_bytes=len(b),truncated=int(length>len(b)),
                     failed=failed,reported_body_bytes=size,shard_sha256=hashes[path],
                     extracted_sha256=hashlib.sha256(b).hexdigest(),
                     named_FUN_refs=','.join(sorted(set(re.findall(r'\bFUN_([0-9a-f]{8})\b',body)))),
                     local_excerpt=str(output.relative_to(ROOT))))
tsv(OUT/'evidence/functions.tsv',rows)
tsv(OUT/'evidence/extraction-missing.tsv',missing,['engine_va','status','question','next_action'])
requests=[]
for r in missing:
    if r['engine_va']=='0x4410bb4': continue  # Resolved binary branch to 0x47390dc.
    r=dict(r,status='target_without_exact_export',next_action='Recover bounded binary disassembly and containing body; referenced LAB/vtable address need not be a saved function start. No missing shard is presumed.')
    requests.append(r)
tsv(OUT/'additional-shards.tsv',requests,['engine_va','status','question','next_action'])

# The historical closure is keyed by names stripped of trailing colons, not VAs.
closure=[]
for name,depth,reason,size in csv.reader((INP/'claude-handoff/results/arc-layer-functions.tsv').open(),delimiter='\t'):
    closure.append(dict(name=name,depth=int(depth),reason=reason,historical_reported_bytes=int(size)))
names={r['name'] for r in closure}; byname=collections.defaultdict(list); starts=[]; large=[]
for line in (INP/'metadata/decomp/arccore-1.170.0-functions.tsv').open():
    entry,end,size,name=line.rstrip('\n').split('\t',3)
    va=int(entry,16); starts.append(va)
    if int(size)>1048576: large.append((va,int(end,16),int(size),name))
    if name.rstrip(':') in names: byname[name.rstrip(':')].append((va,int(end,16),int(size),name))
starts.sort(); audit=[]
for r in closure:
    matches=byname[r['name']]
    for va,end,size,name in matches:
        x=DB.execute('SELECT length,failed,reported_size FROM functions WHERE va=?',(va,)).fetchone()
        interior=bisect.bisect_left(starts,end)-bisect.bisect_right(starts,va)
        flags=[]
        if va==0x421f778: flags.append('known_malformed_merged_body_excluded')
        if size>65536: flags.append('reported_body_gt_64KiB_review')
        if end-va>65536: flags.append('span_gt_64KiB_review')
        if x and x[0]>262144: flags.append('pseudocode_gt_256KiB_review')
        if x and x[1]: flags.append('export_failure')
        if not x: flags.append('no_exact_export')
        audit.append(dict(name=r['name'],depth=r['depth'],reason=r['reason'],engine_va=hex(va),
                          metadata_name=name,metadata_name_matches=len(matches),reported_body_bytes=size,
                          min_end_span=end-va,interior_metadata_starts=max(0,interior),
                          pseudocode_bytes=x[0] if x else '',flags=','.join(flags)))
tsv(OUT/'evidence/closure-audit.tsv',audit)
large_rows=[]
for va,end,size,name in large:
    r=DB.execute('SELECT path,offset,length,reported_size FROM functions WHERE va=?',(va,)).fetchone()
    labels=[]; path=''; length=''; export_size=''
    if r:
        path,offset,length,export_size=r
        with open(path,'rb') as f:f.seek(offset);b=f.read(min(length,262144))
        labels=sorted(set(int(s,16) for s in re.findall(rb'(?:LAB_|ram,0x|jumptable at 0x)([0-9a-f]{8})',b)))
        if va in (0x2b7940,0x16220b0):
            (OUT/f'evidence/malformed-prefix-{va:x}.c').write_bytes(b[:6000])
    far=[p for p in labels if abs(p-va)>1048576]
    large_rows.append(dict(engine_va=hex(va),name=name,metadata_reported_body_bytes=size,
                          metadata_span=end-va,export_header_body_bytes=export_size,pseudocode_bytes=length,
                          interior_metadata_starts=bisect.bisect_left(starts,end)-bisect.bisect_right(starts,va),
                          path=path,distant_labels=','.join(hex(p) for p in far[:10]),
                          status='exclude_from_size_and_semantic_attribution_pending_boundary_recovery'))
tsv(OUT/'evidence/malformed-regions.tsv',large_rows)
summary=dict(shard_records=len(shards),unique_shards=len(hashes),indexed_functions=DB.execute('SELECT count(*) FROM functions').fetchone()[0],
             extracted_functions=len(rows),missing_exact_exports=len(missing),truncated_excerpts=sum(r['truncated'] for r in rows),
             closure_rows=len(closure),closure_unique_names=len(names),
             closure_reason_counts=dict(collections.Counter(r['reason'] for r in closure)),
             closure_depth_counts=dict(collections.Counter(r['depth'] for r in closure)),
             closure_names_unmatched=[r['name'] for r in closure if not byname[r['name']]],
             closure_names_ambiguous=sum(len(byname[n])>1 for n in names),
             closure_address_rows=len(audit),closure_distinct_addresses=len({r['engine_va'] for r in audit}),
             closure_large_review_rows=sum('gt_' in r['flags'] for r in audit),
             closure_failure_rows=sum('export_failure' in r['flags'] for r in audit),
             global_metadata_bodies_over_1MiB=len(large_rows),
             scope='Name reconciliation and malformed-region screening only; no code-size sum or proprietary attribution')
(OUT/'evidence/analysis.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps(summary,indent=2))
