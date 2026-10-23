"""Review-only validation. All writes are beneath this review directory."""
import csv,hashlib,json,re,subprocess,sys
from pathlib import Path
from collections import Counter
OUT=Path('/home/exedev/work/agents/spec-2/revision-reviews/v1')
REV=Path('/home/exedev/work/agents/spec-2/revisions/v1')
PRIOR=Path('/home/exedev/work/agents/spec-1/classreview')
assert Path.cwd()==OUT
subprocess.run([sys.executable,str(OUT/'evidence/audit.py')],check=True,stdout=subprocess.DEVNULL)
def rows(p):return list(csv.DictReader(p.open(),delimiter='\t'))
def header(p):
 with p.open() as f:return next(csv.reader(f,delimiter='\t'))
def digest(p):
 h=hashlib.sha256()
 with p.open('rb') as f:
  for b in iter(lambda:f.read(65536),b''):h.update(b)
 return h.hexdigest()
findings=rows(PRIOR/'findings.tsv');dispositions=rows(OUT/'finding-dispositions.tsv')
assert header(OUT/'finding-dispositions.tsv')==['finding_row','status','evidence','remaining_unknown']
assert [r['finding_row'] for r in dispositions]==[str(i) for i in range(1,len(findings)+1)]
assert Counter(r['status'] for r in dispositions)=={'resolved':4,'resolved_with_limits':9}
assert all(all(r.values()) for r in dispositions)
assert header(OUT/'findings.tsv')==['severity','class_name','selector','claim','evidence','required_correction']
assert rows(OUT/'findings.tsv')==[]
assigned=json.loads(Path('/home/exedev/work/inputs/context/class-assignments.json').read_text())['groups']['spec-2']['classes']
classrows=rows(OUT/'class-review.tsv')
assert header(OUT/'class-review.tsv')==['class_name','status','evidence','limits']
assert Counter(r['class_name'] for r in classrows)==Counter(assigned)
assert all(all(r.values()) for r in classrows)
# Reproduce bounded read-helper evidence and raw instruction rendering without
# invoking any original/revision output-writing harness.
seen=set();read_count=0;negative=[]
for name in ['corrected-bodies.txt','analogous-bodies.txt','unchanged-samples.txt','followup-bodies.txt','monitor-imp.txt']:
 expected=(OUT/'evidence'/name).read_bytes()
 addresses=[]
 for line in expected.decode().splitlines():
  if line.startswith('INDEX '):
   va=line.split()[1];addresses.append(va);seen.add(va);read_count+=1
  if line.startswith('NO_EXPORTED_BODY '):
   va=line.split()[1];addresses.append(va);negative.append(va)
 actual=subprocess.check_output([sys.executable,'/home/exedev/work/read-function.py',*addresses,'--max-bytes','16384'])
 assert actual==expected,name
 assert b'TRUNCATED:' not in actual
assert read_count==len(seen)==58 and negative==['0x440db08']
assert subprocess.check_output([sys.executable,str(OUT/'evidence/disassemble.py')])==(OUT/'evidence/instructions.txt').read_bytes()
assert (OUT/'evidence/instructions.txt').read_bytes()==(REV/'revision-evidence/instructions.txt').read_bytes()
# All assigned classes have a newly retained local IMP sample. Shared IMPs can
# count for each metadata-proven declaring class; a function name is not identity.
methods=rows(REV/'methods.tsv')
assert {r['class_name'] for r in methods if r['imp_va'] in seen}==set(assigned)
# Finish by rechecking measured read-only artifact hashes.
baseline=json.loads((OUT/'evidence/start-hashes.json').read_text())
assert all(digest(Path(p))==h for p,h in baseline.items())
result=dict(status='PASS',review_classes=len(classrows),reviewed_original_finding_data_rows=len(dispositions),disposition_counts=dict(Counter(r['status'] for r in dispositions)),remaining_or_new_findings=0,bounded_successful_unique_body_reads=len(seen),sampled_assigned_classes=len(assigned),negative_exploratory_queries=negative,disassembly_reproduced_from_copied_binary=True,read_only_artifact_hashes_unchanged=len(baseline),limits='Static research review. Structural/provenance PASS does not establish runtime compatibility, sound C++ ABI or complete transitive semantics.')
(OUT/'evidence/final-validation.json').write_text(json.dumps(result,indent=2)+'\n')
for target in re.findall(r'\]\(([^)]+)\)',(OUT/'report.md').read_text()):
 if '://' not in target:assert (OUT/target.split('#')[0]).exists(),target
print(json.dumps(result,indent=2))
