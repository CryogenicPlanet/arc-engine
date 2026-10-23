#!/usr/bin/env python3
"""Read-only corpus inventory and bounded call-closure audit; never interprets extent as code size."""
import collections,csv,hashlib,json,pathlib,re,sqlite3
ROOT=pathlib.Path('/home/exedev/work/inputs'); OUT=pathlib.Path(__file__).resolve().parents[1]
db=sqlite3.connect(f'file:{ROOT}/corpus-index.sqlite?mode=ro',uri=True)
def write(name,fields,rows):
 with (OUT/name).open('w') as f:
  w=csv.DictWriter(f,fields,delimiter='\t');w.writeheader();w.writerows(rows)
def sha(p):
 h=hashlib.sha256()
 with p.open('rb') as f:
  for b in iter(lambda:f.read(1048576),b''):h.update(b)
 return h.hexdigest()
manifest=[]; files={}
for p in [ROOT/'decompile',*sorted((ROOT/'decompile-extra').glob('batch-*'))]:
 assert (p/'READY').exists()
 m=json.loads((p/'MANIFEST.json').read_text()) if (p/'MANIFEST.json').exists() else []
 expected={x['file']:x for x in m}
 for f in sorted(p.glob('*.c')):
  h=sha(f);e=expected.get(f.name)
  if e:assert e['sha256']==h and e['bytes']==f.stat().st_size
  manifest.append(dict(path=str(f),bytes=f.stat().st_size,sha256=h,manifest_verified=bool(e)))
  if f.name in files:assert files[f.name][1]==h
  files[f.name]=(str(f),h)
write('corpus-inventory.tsv',list(manifest[0]),manifest)
old=[]
for line in (ROOT/'claude-handoff/results/arc-layer-functions.tsv').open():
 name,depth,reason,extent=line.rstrip('\n').split('\t')
 old.append(dict(name=name,depth=int(depth),reason=reason,handoff_extent=int(extent)))
wanted={r['name'] for r in old}; meta=collections.defaultdict(list); arcmeta=[]; allspans=[]
for line in (ROOT/'metadata/decomp/arccore-1.170.0-functions.tsv').open():
 a,b,n,name=line.rstrip('\n').split('\t'); va=int(a,16);end=int(b,16)
 allspans.append((va,end))
 if name.rstrip(':') in wanted:meta[name.rstrip(':')].append((va,end,int(n),name))
 if name.startswith('Arc') or name.startswith('objc::Categories::Arc'):arcmeta.append((va,end,int(n),name))
rows=[]; suspicious=[]
for r in old:
 matches=meta[r['name']]; found=[]
 for va,end,n,name in matches:
  q=db.execute('select path,offset,length,failed,reported_size from functions where va=?',(va,)).fetchone()
  if q:
   path,off,length,failed,reported=q;found.append(va)
   flags=[]
   if va==0x421f778:flags.append('known_malformed_excluded')
   if n>65536:flags.append('metadata_extent_gt64KiB_review_not_proof')
   if n!=reported:flags.append('metadata_header_extent_disagreement')
   row={**r,'va':hex(va),'exact_name':name,'metadata_extent':n,'header_extent':reported,'body_C_bytes':length,'failed':failed,'path':path,'offset':off,'flags':';'.join(flags)}
   rows.append(row)
   if flags:suspicious.append(row)
 if not found:rows.append({**r,'va':'','exact_name':'','metadata_extent':'','header_extent':'','body_C_bytes':'','failed':'','path':'','offset':'','flags':'no_exported_body'})
write('handoff-audit.tsv',list(rows[0]),rows);write('extent-review.tsv',list(rows[0]),suspicious)
# Reconstruct only syntactic references, preserving VA identity and ObjC selector punctuation.
header=re.compile(rb'^// ==== (0x[0-9a-f]+)  (.+?)  \((\d+) bytes\)\s*$')
call=re.compile(rb'\b(FUN_([0-9a-f]{8}))\s*\(')
refs=re.compile(rb'\b(?:FUN_|LAB_)([0-9a-f]{8})\b')
exports={b'_ChromeMain',b'_ChromeAppModeStart_v8',b'_ChromeWebAppShortcutCopierMain',b'_SetLogMessageHandler',b'_CallWithEHFrame',b'_emitTraceEvent',b'_arcCategoryEnabledFlag',b'_ArcErrorDomain'}
seeds={};seed_calls={};seed_refs={};counts=collections.Counter();path_seeds=[];path_evidence=[]
def consume(va,name,body):
 counts['headers']+=1
 for source in sorted(set(re.findall(rb'\.\./\.\./arc/[A-Za-z0-9_./-]+',body))):
  path_evidence.append(dict(va=hex(va),name=name.decode(),source=source.decode()))
 reason=None
 if name.startswith(b'Arc') and b'::' in name:reason='arc-objc'
 elif name.startswith(b'objc::Categories::Arc'):reason='arc-category'
 elif name in exports:reason='export'
 elif b'../../arc/' in body or b'arc.mojom' in body:reason='arc-path-or-ipc'
 if reason:
  seeds[va]=(name.decode(),reason)
  # Exclude the own function's declaration by dropping everything before the first opening brace.
  content=body.split(b'{',1)[-1]
  seed_calls[va]={int(m[1],16) for m in call.findall(content)}
  seed_refs[va]={int(m,16) for m in refs.findall(content)}
  if reason=='arc-path-or-ipc':path_seeds.append(va)
for path,h in files.values():
 with open(path,'rb') as f:
  va=None;buf=[];name=None
  for line in f:
   m=header.match(line)
   if m:
    if va is not None:consume(va,name,b''.join(buf))
    va=int(m[1],16);name=m[2];buf=[]
   elif va is not None:buf.append(line)
  if va is not None:consume(va,name,b''.join(buf))
assert counts['headers']==db.execute('select count(*) from functions').fetchone()[0]
write('arc-source-body-evidence.tsv',['va','name','source'],path_evidence)
# Enforce a 1 MiB bound on subsequent individual reads; larger bodies remain unresolved.
def body(va):
 q=db.execute('select name,path,offset,length,failed from functions where va=?',(va,)).fetchone()
 if not q or q[-1] or q[3]>1048576:return None
 with open(q[1],'rb') as f:f.seek(q[2]);return f.read(q[3]).split(b'{',1)[-1]
excluded={0x421f778};depth={v:0 for v in seeds if v not in excluded};front=set(depth);edges=[]
for d in (1,2):
 nxt=set()
 for va in sorted(front):
  if d==1:targets=seed_calls[va]
  else:
   b=body(va);targets={int(m[1],16) for m in call.findall(b)} if b else set()
  for target in sorted(targets):
   edges.append(dict(caller=hex(va),callee=hex(target),edge='direct_FUN_call_text',caller_depth=d-1))
   if target not in depth and target not in excluded:nxt.add(target)
 for v in nxt:depth[v]=d
 front=nxt
closure=[]
for va,d in sorted(depth.items()):
 q=db.execute('select name,path,offset,length,failed from functions where va=?',(va,)).fetchone()
 closure.append(dict(va=hex(va),depth=d,reason=seeds.get(va,('','callee'))[1],name=q[0] if q else '',path=q[1] if q else '',offset=q[2] if q else '',C_bytes=q[3] if q else '',failed=q[4] if q else '',status='exported' if q else 'missing'))
write('audited-direct-closure.tsv',list(closure[0]),closure);write('direct-edges.tsv',list(edges[0]),edges)
refsrows=[]
for va in sorted(seeds):
 for v in sorted(seed_refs[va]-seed_calls[va]-{va}):refsrows.append(dict(caller=hex(va),reference=hex(v),kind='FUN_or_LAB_reference_not_direct_call'))
write('noncall-function-references.tsv',['caller','reference','kind'],refsrows)
# Span overlap is a coverage warning, not automatic semantic corruption.
maxend=0;overlap=0
for va,end in allspans:
 overlap+=va<maxend;maxend=max(maxend,end)
summary=dict(corpus=dict(unique_shards=len(files),copies=len(manifest),**counts),handoff=dict(rows=len(old),by_depth=dict(collections.Counter(r['depth'] for r in old)),by_reason=dict(collections.Counter(r['reason'] for r in old)),zero_extents=sum(r['handoff_extent']==0 for r in old),resolved_export_rows=sum(bool(r['va']) for r in rows),unresolved_names=sum(not r['va'] for r in rows),colon_stripped_names=sum(r['name']!=r['exact_name'] for r in rows if r['va']),metadata_header_disagreements=sum('disagreement' in r['flags'] for r in rows),review_rows=len(suspicious)),audited_syntax=dict(seed_reasons=dict(collections.Counter(r for n,r in seeds.values())),closure_depths=dict(collections.Counter(depth.values())),missing_exported_bodies=sum(r['status']=='missing' for r in closure),seed_nondirect_references=len(refsrows),source_path_functions=len({r['va'] for r in path_evidence}),distinct_arc_paths=len({r['source'] for r in path_evidence}),excluded=[hex(v) for v in excluded]),metadata=dict(rows=len(allspans),starts_inside_prior_max_span=overlap),scope='Syntactic discovery inventory. No proprietary code count, size, complete reachability, ABI or runtime proof.')
(OUT/'audit-summary.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary,indent=2))
