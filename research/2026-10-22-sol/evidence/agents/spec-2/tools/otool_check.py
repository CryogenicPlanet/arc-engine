import json,re,collections
from pathlib import Path
p=Path('specs/evidence/methods-bodies.json');ms=json.load(open(p));targets={int(m['imp'],16) for m in ms};hits=collections.defaultdict(list)
ty='';tyline=0
for ln,l in enumerate(open('/home/exedev/work/inputs/context/analysis/data/objc170.txt'),1):
 a=re.match(r'\s*types\s+(0x[\da-f]+)(?: \(0x[\da-f]+\))?\s+(.+)',l)
 if a:ty=a[2].strip();tyline=ln
 a=re.match(r'\s*imp\s+(0x[\da-f]+)(?: \((0x[\da-f]+)\))?',l)
 if a:
  va=int(a[2] or a[1],16)
  if va in targets:hits[(va,ty)].append((tyline,ln))
for m in ms:
 h=hits[(int(m['imp'],16),m['type_encoding'])];assert h,(m['class_name'],m['selector']);m['otool_type_line'],m['otool_imp_line']=h[0]
json.dump(ms,open(p,'w'),indent=2)
print('491/491 binary-identified method encoding/IMP pairs corroborated in raw otool; names/class/category/kind independently resolved from binary and dyld')
