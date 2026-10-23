import json,csv,hashlib,re
from pathlib import Path
from metadata import digest
p=Path('specs');rows=list(csv.DictReader(open(p/'methods.tsv'),delimiter='\t'));c=json.load(open(p/'coverage.json'))
required='class_name category kind selector type_encoding imp_va body_status behavior evidence confidence unresolved_question'.split()
assert all(all(k in r for k in required) for r in rows)
assert len(rows)==491;assert len({r['class_name'] for r in rows})==34
for cls,stats in c['by_class'].items():
 doc=(p/'classes'/f'{cls}.md').read_text();matrix=[l for l in doc.splitlines() if l.startswith('| (base) |') or l.startswith('| Extensions |')]
 assert len(matrix)==stats['method_rows'],(cls,len(matrix),stats)
 for r in rows:
  if r['class_name']==cls:assert r['imp_va'] in doc
for manifest in [p/'sources/manifest.json',p/'sources/cef-manifest.json']:
 for r in json.load(open(manifest)):
  if 'sha256' not in r:continue
  f=p/'sources'/('cef' if 'cef-manifest' in str(manifest) else 'chromium')/r['path'];assert digest(f)==r['sha256']
for f in (p/'evidence').glob('bridges-*.txt'):
 assert 'TRUNCATED:' not in f.read_text();assert 'NO_EXPORTED_BODY' not in f.read_text(),f
# Offline input provenance hashes; streamed, no mmap or cross-directory writes.
files=['binaries/ArcCore-1.170.0.arm64','binaries/Arc-1.168.0.arm64','context/analysis/data/dobjc170.txt','context/analysis/data/objc170.txt','context/class-assignments.json','corpus-index.sqlite','claude-handoff/results/arc-layer-functions.tsv']
manifest={f:digest(Path('/home/exedev/work/inputs')/f) for f in files};(p/'evidence/input-hashes.json').write_text(json.dumps(manifest,indent=2)+'\n')
print('FINAL PASS: 34 class matrices / 491 rows; pinned extracts/sources; bounded bridge bodies; input hashes')
