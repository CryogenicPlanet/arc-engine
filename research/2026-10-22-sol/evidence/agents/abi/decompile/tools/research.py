#!/usr/bin/env python3
"""Reproduce bounded ABI evidence; inputs opened read-only; no contiguous-size claims."""
import sys,csv,sqlite3,json,hashlib,re,collections,struct
from pathlib import Path
HERE=Path(__file__).resolve().parents[1]; ROOT=Path('/home/exedev/work/inputs')
sys.path.insert(0,str(HERE.parent/'scripts'))
from inventory import MachO,write_tsv
from disasm import disasm
DB=sqlite3.connect(f'file:{ROOT}/corpus-index.sqlite?mode=ro',uri=True); DB.row_factory=sqlite3.Row

def digest(path):
 h=hashlib.sha256()
 with path.open('rb') as f:
  for chunk in iter(lambda:f.read(1<<20),b''):h.update(chunk)
 return h.hexdigest()

def snapshot():
 rows=[]
 for manifest in [ROOT/'decompile/MANIFEST.json',*sorted((ROOT/'decompile-extra').glob('batch-*/MANIFEST.json'))]:
  assert (manifest.parent/'READY').exists()
  for r in json.loads(manifest.read_text()):
   p=manifest.parent/r['file']; sha=digest(p)
   assert p.stat().st_size==r['bytes'] and sha==r['sha256'],str(p)
   rows.append(dict(path=str(p),bytes=r['bytes'],sha256=sha,manifest=str(manifest),ready=(manifest.parent/'READY').read_text().strip()))
 write_tsv(HERE/'corpus-inventory.tsv',rows)
 stats=dict(manifests=len({x['manifest'] for x in rows}),unique_paths=len(rows),unique_shards=len({Path(x['path']).name for x in rows}),unique_hashes=len({x['sha256'] for x in rows}),indexed_bodies=DB.execute('select count(*) from functions').fetchone()[0],failure_placeholders=DB.execute('select count(*) from functions where failed!=0').fetchone()[0])
 (HERE/'corpus-summary.json').write_text(json.dumps(stats,indent=2)+'\n'); print(stats)

def audit():
 closure=[]; wanted=set()
 for line in (ROOT/'claude-handoff/results/arc-layer-functions.tsv').open():
  name,level,reason,size=line.rstrip('\n').split('\t')
  closure.append(dict(name=name,depth=int(level),reason=reason,handoff_reported_size=int(size))); wanted.add(name)
 matches=collections.defaultdict(list); previous=None; total=0; maxend=0; maxowner=''
 for line in (ROOT/'metadata/decomp/arccore-1.170.0-functions.tsv').open():
  p=re.split(r'\\t|\t',line.rstrip('\n')); va,end,size=int(p[0],16),int(p[1],16),int(p[2]);name=p[3];total+=1
  if previous is not None and previous['metadata_end']>va:previous['next_start_inside_span']=True
  previous=None
  normalized=name.rstrip(':')
  if normalized in wanted:
   row=dict(va=hex(va),metadata_name=name,metadata_end=end,metadata_size=size,metadata_span=end-va,next_start_inside_span=False,inside_prior_min_end=va<maxend,prior_span_owner=maxowner if va<maxend else '')
   matches[normalized].append(row);previous=row
  if end>maxend:maxend=end;maxowner=hex(va)
 rows=[]
 for row in closure:
  ms=matches[row['name']]
  if not ms:rows.append(dict(**row,resolution='no_exact_metadata_name'));continue
  for m in ms:
   r=DB.execute('select va,path,offset,length,failed,reported_size from functions where va=?',(int(m['va'],16),)).fetchone()
   flags=[]
   if row['name']=='FUN_0421f778':flags.append('known_merged_exclude')
   if m['metadata_span']>65536:flags.append('large_span_review')
   if m['next_start_inside_span']:flags.append('span_contains_next_entry_not_body_proof')
   if m['inside_prior_min_end']:flags.append('inside_prior_min_end_not_body_proof')
   if r and r['failed']:flags.append('export_failure')
   if r and m['metadata_size']!=r['reported_size']:flags.append('metadata_vs_export_size_diff')
   rows.append(dict(**row,**m,resolution='unique_name' if len(ms)==1 else 'ambiguous_metadata_name',exported=bool(r),export_path=r['path'] if r else '',export_offset=r['offset'] if r else '',export_text_bytes=r['length'] if r else '',export_reported_size=r['reported_size'] if r else '',flags=';'.join(flags)))
 write_tsv(HERE/'closure-audit.tsv',rows,list(dict.fromkeys(k for r in rows for k in r)))
 stats=dict(handoff_rows=len(closure),unique_names=len(wanted),resolved_rows=sum(bool(matches[r['name']]) for r in closure),ambiguous_names=sum(len(v)>1 for v in matches.values()),resolved_metadata_records=len(rows),depth_counts=dict(collections.Counter(r['depth'] for r in closure)),reason_counts=dict(collections.Counter(r['reason'] for r in closure)),metadata_rows=total,flags=dict(collections.Counter(f for r in rows for f in r.get('flags','').split(';') if f)),note='Do not sum any size column into code volume. Overlap flags are diagnostics, not proof of malformed semantics.')
 (HERE/'closure-summary.json').write_text(json.dumps(stats,indent=2)+'\n');print(stats)

def extract():
 rows=[]
 for line in (HERE/'targets.tsv').open():
  if line.startswith('va\t') or not line.strip():continue
  va,topic=line.rstrip('\n').split('\t');a=int(va,16)
  r=DB.execute('select * from functions where va=?',(a,)).fetchone()
  row=dict(va=va,topic=topic,status='no_exact_exported_header')
  if r:
   with Path(r['path']).open('rb') as f:f.seek(r['offset']); data=f.read(min(r['length'],32768))
   dest=HERE/'bodies'/f'{va}.c';dest.write_bytes(data)
   row.update(name=r['name'],status='export_failure' if r['failed'] else ('truncated' if len(data)!=r['length'] else 'complete_text_not_semantic_validation'),path=r['path'],offset=r['offset'],length=r['length'],extracted_length=len(data),sha256=hashlib.sha256(data).hexdigest(),local=str(dest))
  rows.append(row)
 write_tsv(HERE/'selected-bodies.tsv',rows,list(dict.fromkeys(k for r in rows for k in r)))
 # Update initial availability without modifying initial evidence.
 avail=[]
 for name in ['contract.tsv','decompile-requests.tsv','missing-engine-bodies.tsv']:
  for row in csv.DictReader((HERE.parent/'initial'/name).open(),delimiter='\t'):
   va=row.get('engine_va') or row.get('imp');r=DB.execute('select name,path,offset,length,failed from functions where va=?',(int(va,16),)).fetchone()
   avail.append(dict(initial_table=name,va=va,class_name=row.get('class_name',''),selector=row.get('selector',''),status='available_exact_header' if r and not r['failed'] else ('export_failed' if r else 'no_exact_header'),path=r['path'] if r else '',offset=r['offset'] if r else '',length=r['length'] if r else ''))
 write_tsv(HERE/'initial-requests-resolved.tsv',avail)
 print(dict(selected=len(rows),available=sum(r['status']!='no_exact_exported_header' for r in rows),initial_availability=dict(collections.Counter((r['initial_table'],r['status']) for r in avail))))

def binary():
 image=MachO(ROOT/'binaries/ArcCore-1.170.0.arm64');stubs=image.import_stubs()
 def resolve(target):
  islands=[]
  for _ in range(5):
   if target in stubs:return target,stubs[target],islands
   try:a,b,c=struct.unpack('<III',image.read(target,12))
   except ValueError:break
   if a&0xfc000000==0x14000000:
    n=a&0x3ffffff;n=n-(1<<26) if n&(1<<25) else n;islands.append(hex(target));target+=n*4;continue
   if a&0x9f00001f==0x90000010 and b&0xffc003ff==0x91000210 and c==0xd61f0200:
    n=((a>>29)&3)|(((a>>5)&0x7ffff)<<2);n=n-(1<<21) if n&(1<<20) else n
    islands.append(hex(target));target=(target&~4095)+(n<<12)+((b>>10)&0xfff);continue
   break
  return target,'',islands
 out=[];runtime=[]
 for line in (HERE/'binary-ranges.tsv').open():
  if line.startswith('va\t') or not line.strip():continue
  va,size,topic=line.rstrip('\n').split('\t');start=int(va,16)
  for a,raw,ins in disasm(image,start,int(size,0)):
   word=int.from_bytes(bytes.fromhex(raw),'little');target='';symbol='';islands=[]
   if word&0xfc000000 in (0x94000000,0x14000000):
    n=word&0x3ffffff;n=n-(1<<26) if n&(1<<25) else n;dest=a+n*4
    resolved,symbol,islands=resolve(dest);target=hex(dest)
    runtime.append(dict(range_va=va,topic=topic,instruction_va=hex(a),branch_target=target,resolved=hex(resolved),import_symbol=symbol,islands=';'.join(islands)))
   out.append(dict(range_va=va,topic=topic,instruction_va=hex(a),file_offset=hex(image.offset(a)),bytes=raw,instruction=ins.strip(),branch_target=target,import_symbol=symbol))
 write_tsv(HERE/'binary-instructions.tsv',out);write_tsv(HERE/'binary-branches.tsv',runtime)
 print(dict(instructions=len(out),branches=len(runtime)))

if __name__=='__main__':
 for cmd in sys.argv[1:] :globals()[cmd]()
