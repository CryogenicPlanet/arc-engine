import json,re,sys
from pathlib import Path
m=json.loads(Path('specs/evidence/metadata.json').read_text())
for x in m['methods']:
 if x['class_name'] not in sys.argv[1:]:continue
 s=Path(f"specs/bodies/{int(x['imp'],16):08x}.txt").read_text()
 print('\n###', x['class_name'],x['category'],x['kind']+x['selector'],x['imp'])
 lines=s.splitlines();start=next((i for i,l in enumerate(lines) if l=='{'),0)
 # Drop local declarations to show operations; keep all if body has no blank delimiter.
 j=next((i for i in range(start+1,len(lines)) if lines[i]=='  '),None)
 if j is not None:start=j
 print('\n'.join(lines[start+1:])[:int(__import__('os').environ.get('LIMIT','10000'))])
