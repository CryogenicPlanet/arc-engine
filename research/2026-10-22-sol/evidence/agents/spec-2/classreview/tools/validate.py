#!/usr/bin/env python3
"""Validate all required review artifacts, inventory totals and reviewed snapshot hashes."""
import csv,hashlib,json,re,sqlite3
from pathlib import Path
O=Path('classreview');W=Path('../spec-3/specs');I=Path('/home/exedev/work/inputs')
def read(p):return list(csv.DictReader(p.open(),delimiter='\t'))
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
assigned=set(json.loads((I/'context/class-assignments.json').read_text())['groups']['spec-3']['classes'])
cr=read(O/'class-review.tsv');fs=read(O/'findings.tsv');coverage=json.loads((O/'evidence/coverage.json').read_text())
assert len(cr)==37 and {r['class_name'] for r in cr}==assigned
assert len(fs)==6 and all(r['class'] in assigned for r in fs)
assert all(set(r)=={'severity','class','selector','claim','exact evidence','required correction'} for r in fs)
for r in cr:assert (r['result']=='CORRECTION_REQUIRED')==any(f['class']==r['class_name'] for f in fs)
for r in read(O/'evidence/class-coverage.tsv'):assert digest(W/'classes'/f"{r['class_name']}.md")==r['spec_sha256']
assert coverage['declarations']==491 and coverage['distinct_imps']==433
assert len(read(O/'evidence/layout-checks.tsv'))==37
assert len(read(O/'evidence/vtable-checks.tsv'))==16
cites=read(O/'evidence/citations.tsv');assert not any(r['resolution']=='UNMAPPED' for r in cites)
missing=[r for r in cites if r['resolution']=='engine text; NO_EXACT_INDEXED_BODY']
assert {r['cited_va'] for r in missing}=={'0x44195b4','0x44197a8','0x441999c','0x4419b90'}
# Evidence line and method-record citations are checked without pairing dump rows by order.
writer=read(W/'methods.tsv');decoded=read(O/'evidence/decoded-methods.tsv');key=lambda r:tuple(r[k] for k in ('class_name','category','kind','selector'))
dec={key(r):r for r in decoded};dyld=(I/'context/analysis/data/dobjc170.txt').read_text().splitlines()
for r in writer:
 a=dec[key(r)];assert f"method_record_va {a['record_va']}" in r['evidence']
 m=re.search(r'dobjc170.txt:(\d+)',r['evidence']);assert m
 line=dyld[int(m[1])-1];assert f"{r['kind']}[{r['class_name']} {r['selector']}]" in line
 assert int(re.search(r'0x[\dA-Fa-f]+',line)[0],16)==int(r['imp_va'],16)
# Every saved body must still equal the same exact read-only indexed bytes.
db=sqlite3.connect(f'file:{I}/corpus-index.sqlite?mode=ro',uri=True)
for directory in ('bodies','backend'):
 for p in (O/directory).glob('*.c'):
  a=int(p.stem,16);path,off,n=db.execute('select path,offset,length from functions where va=?',(a,)).fetchone()
  with open(path,'rb') as f:f.seek(off);b=f.read(min(n,32768) if directory=='backend' else n)
  assert b==p.read_bytes(),p
# Save hashes of reviewed writer inputs and checker sources, bounded in size.
paths=[W/'methods.tsv',W/'coverage.json',W/'evidence/metadata.json',*sorted((W/'classes').glob('*.md')),*sorted((O/'tools').glob('*.py'))]
(O/'evidence/reviewed-inputs.json').write_text(json.dumps({str(p):digest(p) for p in paths},indent=2)+'\n')
summary=dict(result='PASS',classes=37,method_identities=491,distinct_IMPs=433,findings=6,missing_exact_callback_entries=4,manual_sample_rows=len(read(O/'evidence/manual-body-sample.tsv')),writer_class_spec_hashes_unchanged=True,all_body_snapshots_rechecked=True)
(O/'evidence/validation.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary,indent=2))
