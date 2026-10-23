#!/usr/bin/env python3
"""Read-only evidence audit; streams C shards, uses local SQLite, no disassembly execution.
Run from role directory: flock /home/exedev/work/heavy.lock python3 tools/audit_export.py
"""
import array,bisect,collections,csv,hashlib,json,re,sqlite3
from pathlib import Path
I=Path('/home/exedev/work/inputs'); O=Path('initial'); O.mkdir(exist_ok=True)
def dump(n,x): (O/n).write_text(json.dumps(x,indent=2,sort_keys=True)+'\n')
def tsv(n,cols,rows):
 with (O/n).open('w') as f:
  w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(cols);w.writerows(rows)
D=sqlite3.connect(O/'audit.sqlite');D.execute('PRAGMA journal_mode=OFF');D.execute('PRAGMA synchronous=OFF');D.execute('PRAGMA cache_size=-24000');D.execute('PRAGMA temp_store=FILE')
D.executescript('DROP TABLE IF EXISTS meta; DROP TABLE IF EXISTS bodies; CREATE TABLE meta(va INTEGER PRIMARY KEY,end INTEGER,n INTEGER,name TEXT); CREATE TABLE bodies(va INTEGER PRIMARY KEY,name TEXT,scriptname TEXT,n INTEGER,path TEXT,line INTEGER,cbytes INTEGER,failed INTEGER,arcpath INTEGER,calls TEXT,warnings TEXT);')
starts=array.array('Q');ends=array.array('Q');ns=array.array('Q');names=[]
for line in (I/'metadata/decomp/arccore-1.170.0-functions.tsv').open():
 a,b,n,name=line.rstrip('\n').split('\t',3);a,b,n=int(a,16),int(b,16),int(n);starts.append(a);ends.append(b);ns.append(n)
 D.execute('INSERT INTO meta VALUES(?,?,?,?)',(a,b,n,name))
D.commit();print('metadata',len(starts),flush=True)
# Validate every manifest record, including repeated copies, but scan only one identical shard.
files={};manifest_rows=[];manifests=sorted([I/'decompile/MANIFEST.json',*I.glob('decompile-extra/batch-*/MANIFEST.json')]);mismatches=[]
for m in manifests:
 for r in json.loads(m.read_text()):
  p=m.parent/r['file']; h=hashlib.sha256();count=0
  with p.open('rb') as f:
   for block in iter(lambda:f.read(1024*1024),b''):h.update(block);count+=len(block)
  ok=(count==r['bytes'] and h.hexdigest()==r['sha256']);manifest_rows.append([str(m),str(p),count,h.hexdigest(),ok]);
  if not ok:mismatches.append(str(p))
  if p.name in files and files[p.name][1]!=h.hexdigest():raise ValueError('conflicting shard '+p.name)
  files.setdefault(p.name,(p,h.hexdigest()))
tsv('manifest-verification.tsv',['manifest','path','bytes','sha256','matches'],manifest_rows)
header=re.compile(rb'^// ==== (0x[0-9a-f]+)  (.+?)  \((\d+) bytes\)\r?$');call=re.compile(r'\b(FUN_[0-9a-f]{8})\s*\(')
failures=[];warning_totals=collections.Counter();body_sizes=[];header_disagree=[]
def save(h,buf,path,line):
 if h is None:return
 a,name,n=h;a=int(a,16);n=int(n);name=name.decode();body=b''.join(buf).decode('utf-8','replace');err=re.search(r'/\* decompile failed: ([\s\S]*?) \*/',body)
 if err:
  error=err.group(1).strip();low=error.lower()
  category=('timeout' if 'process: timeout' in low else
            'varnode_hash_nonunique' if 'unable to find unique hash for varnode' in low else
            'address_space_range' if 'memory range beyond end of address space' in low else
            'input_varnode_adjustment' if 'cannot properly adjust input varnodes' in low else
            'forced_merge_intersection' if 'forced merge caused intersection' in low else 'unclassified')
  failures.append([f'0x{a:x}',error,category,f'{path}:{line}'])
 warnings=collections.Counter(re.findall(r'/\* WARNING: ([^\n]*?)(?: \*/|$)',body));warning_totals.update(warnings)
 # Declaration of FUN itself is included, matching the handoff parser's behavior.
 calls=sorted(set(call.findall(body)));haspath='../../arc/' in body or 'arc.mojom' in body
 D.execute('INSERT INTO bodies VALUES(?,?,?,?,?,?,?,?,?,?,?)',(a,name,name.rstrip(':'),n,str(path),line,len(body.encode()),int(bool(err)),int(haspath),json.dumps(calls),json.dumps(warnings)))
for k,(p,_) in sorted(files.items()):
 with p.open('rb') as f:
  h=None;buf=[];hl=0
  for ln,line in enumerate(f,1):
   m=header.match(line.rstrip(b'\n'))
   if m:
    save(h,buf,p,hl);h=m.groups();buf=[];hl=ln
   elif h:buf.append(line)
  save(h,buf,p,hl)
 D.commit()
print('bodies scanned',D.execute('select count(*) from bodies').fetchone()[0],flush=True)
D.executescript('CREATE INDEX meta_names ON meta(name); CREATE INDEX body_names ON bodies(scriptname); CREATE INDEX body_raw_names ON bodies(name);')
tsv('failures.tsv',['va','error','category','evidence'],failures)
# Bounds are half-open enclosing spans, not contiguous actual bodies.
prior_end=0;inside=0;isolated=0;zero=0;not_equal=0;huge=collections.Counter();boundrows=[];bad=set();isolated_set=set()
for idx,(a,b,n) in enumerate(zip(starts,ends,ns)):
 within=a<prior_end;nexta=starts[idx+1] if idx+1<len(starts) else 2**64-1
 if within:inside+=1
 iso=not within and b<=nexta and b>a
 if iso:isolated+=1;isolated_set.add(a)
 if n==0:zero+=1
 if n!=b-a:not_equal+=1
 for threshold in [4096,65536,1048576,16777216]:huge[str(threshold)]+=n>=threshold
 if within or b>nexta or b<=a:bad.add(a)
 if n>=65536:boundrows.append([hex(a),hex(b),n,bisect.bisect_left(starts,b)-idx-1,int(within),int(iso)])
 prior_end=max(prior_end,b)
tsv('huge-bounds.tsv',['va','end','tsv_span','other_starts_inside','inside_prior_span','isolated'],boundrows)
# Max end overlap isn't body overlap. Compare stored body count to span explicitly.
bodyspan=D.execute('select count(*),sum(meta.n),sum(bodies.n),sum(meta.n!=bodies.n),sum(bodies.n>meta.n) from bodies join meta using(va)').fetchone()
missing=D.execute('select count(*) from meta left join bodies using(va) where bodies.va is null').fetchone()[0]
tsv('missing-headers.tsv',['va','name','tsv_span'],((hex(a),n,s) for a,n,s in D.execute('select meta.va,meta.name,meta.n from meta left join bodies using(va) where bodies.va is null')))
# Reproduce script dictionary overwrite and name stripping without keeping entire corpus in RAM.
D.executescript('DROP TABLE IF EXISTS scriptbodies; CREATE TABLE scriptbodies AS SELECT * FROM bodies WHERE rowid IN (SELECT max(rowid) FROM bodies GROUP BY scriptname); CREATE UNIQUE INDEX sb_name ON scriptbodies(scriptname);')
# Original scan is shard filename lexical; rowid=VA order is NOT scan order. Use path basename lexical + line.
# Rebuild winners correctly: order scanned file list, overwrite each shared normalized name.
D.execute('delete from scriptbodies')
for k,(p,_) in sorted(files.items()):
 D.execute('insert or replace into scriptbodies select * from bodies where path=? order by line',(str(p),))
D.commit()
exports={'_ChromeMain','_ChromeAppModeStart_v8','_ChromeWebAppShortcutCopierMain','_SetLogMessageHandler','_CallWithEHFrame','_emitTraceEvent','_arcCategoryEnabledFlag','_ArcErrorDomain'}
seed={};vabody={}
for va,name,arcpath in D.execute('select va,scriptname,arcpath from scriptbodies'):
 if re.match(r'^Arc[A-Za-z0-9_]*::',name):seed[name]='arc-objc'
 elif name in exports:seed[name]='export'
 elif arcpath:seed[name]='arc-path'
depth=dict.fromkeys(seed,0);frontier=list(seed)
for d in (1,2):
 nxt=[]
 for name in frontier:
  row=D.execute('select calls from scriptbodies where scriptname=?',(name,)).fetchone()
  for callee in json.loads(row[0]) if row else []:
   if callee not in depth:depth[callee]=d;nxt.append(callee)
 frontier=nxt
# The supplied table contains sizes from a name-keyed TSV dictionary; last VA wins.
handoff={}
for line in (I/'claude-handoff/results/arc-layer-functions.tsv').open():
 name,d,r,n=line.rstrip('\n').split('\t');handoff[name]=(int(d),r,int(n))
reproduced={};closure=[]
for name,d in sorted(depth.items(),key=lambda kv:(kv[1],kv[0])):
 m=D.execute('select va,n from meta where name=? order by va desc',(name,)).fetchall();bs=D.execute('select va,name,n,failed,cbytes,warnings from scriptbodies where scriptname=?',(name,)).fetchone()
 n=m[0][1] if m else 0;reason=seed.get(name,'callee');reproduced[name]=(d,reason,n)
 closure.append([name,d,reason,n,'' if not bs else hex(bs[0]),'' if not bs else bs[1],len(m),'' if not bs else bs[2],'' if not bs else bs[3],'' if not bs else int(bs[0] in bad),'' if not bs else bs[4],'' if not bs else bs[5]])
tsv('closure-audit.tsv',['script_name','depth','reason','name_join_tsv_span','body_va','raw_body_name','exact_tsv_name_matches','header_body_addresses','failed','bound_risk','C_bytes','warnings'],closure)
# Corpus warning evidence retains function addresses for structural triage, not semantic judgment.
tsv('warning-bodies.tsv',['va','name','body_addresses','C_bytes','warnings','evidence'],((hex(va),name,n,cb,w,f'{p}:{ln}') for va,name,n,cb,w,p,ln in D.execute("select va,name,n,cbytes,warnings,path,line from bodies where warnings!='{}'")))
tsv('largest-bodies.tsv',['va','name','header_body_addresses','C_bytes','evidence'],((hex(va),name,n,cb,f'{p}:{ln}') for va,name,n,cb,p,ln in D.execute('select va,name,n,cbytes,path,line from bodies order by n desc limit 100')))
summary={'manifest_files':len(manifests),'manifest_records':len(manifest_rows),'unique_shards':len(files),'verified_manifest_records':sum(r[-1] for r in manifest_rows),'manifest_mismatches':mismatches,'C_headers':D.execute('select count(*) from bodies').fetchone()[0],'failures':len(failures),'failure_categories':dict(collections.Counter(r[2] for r in failures)),'failure_errors':dict(collections.Counter(r[1] for r in failures)),'TSV_entries':len(starts),'headers_without_TSV':D.execute('select count(*) from bodies left join meta using(va) where meta.va is null').fetchone()[0],'TSV_without_C_header':missing,'TSV_span_sum_NOT_code_coverage':sum(ns),'bounds':{'starts_inside_prior_enclosing_span':inside,'isolated_positive_spans':isolated,'zero_spans':zero,'span_field_disagrees_end_minus_start':not_equal,'span_threshold_counts':dict(huge)},'body_span_join':dict(zip(['rows','TSV_span_sum','header_body_address_sum','disagree','header_body_larger_than_span'],bodyspan)),'warnings':dict(warning_totals),'handoff':{'supplied_rows':len(handoff),'reproduced_rows':len(reproduced),'exact_reproduction':handoff==reproduced,'missing_in_reproduction':sorted(set(handoff)-set(reproduced)),'new_in_reproduction':sorted(set(reproduced)-set(handoff)),'mismatched_rows':[[k,handoff[k],v] for k,v in reproduced.items() if k in handoff and handoff[k]!=v],'depth_reason_counts':dict(collections.Counter(f'{d}:{r}' for d,r,n in reproduced.values())),'zero_name_join_sizes':sum(n==0 for d,r,n in reproduced.values()),'raw_header_names':D.execute('select count(distinct name) from bodies').fetchone()[0],'script_normalized_names':D.execute('select count(*) from scriptbodies').fetchone()[0],'normalized_names_with_multiple_bodies':D.execute('select count(*) from (select scriptname from bodies group by scriptname having count(*)>1)').fetchone()[0],'trailing_colon_headers':D.execute("select count(*) from bodies where name!=scriptname").fetchone()[0],'rows_with_risky_bounds':sum(r[9]==1 for r in closure),'rows_with_no_body':sum(r[4]=='' for r in closure),'rows_with_failure':sum(r[8]==1 for r in closure),'closure_TSV_name_sum_NOT_code_coverage':sum(v[2] for v in reproduced.values())}}
dump('coverage.json',summary);print(json.dumps({k:v for k,v in summary.items() if k!='warnings'},indent=2),flush=True)
D.close()
