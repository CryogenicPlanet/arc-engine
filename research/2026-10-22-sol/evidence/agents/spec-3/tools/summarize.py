import json,re
from pathlib import Path
m=json.loads(Path('specs/evidence/metadata.json').read_text())
for cls in m['classes']:
 print('\n###',cls)
 for x in m['methods']:
  if x['class_name']!=cls:continue
  s=Path(f"specs/bodies/{int(x['imp'],16):08x}.txt").read_text()
  print(x['kind']+x['selector'], x['imp'], 'chars='+str(len(s)), ' '.join(re.findall(r'(?:FUN_[0-9a-f]+|_objc_[A-Za-z0-9_]+|"[^"\n]{2,100}")',s))[:800])
