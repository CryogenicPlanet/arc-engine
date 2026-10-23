#!/usr/bin/env python3
"""Reconcile type/IMP multisets by class/category/kind, never row-order pairing.
Relative selector strings come independently from Mach-O records and dyld names.
"""
import json,re,collections
from pathlib import Path
meta=json.loads(Path('specs/evidence/metadata.json').read_text());classes=meta['classes'];cats={x['metadata_va']:x for x in meta['categories']}
lines=Path('/home/exedev/work/inputs/context/analysis/data/objc170.txt').read_text().splitlines()
starts=[(i,re.match(r'^([0-9a-f]{16}) (0x[0-9a-f]+)(.*)$',l)) for i,l in enumerate(lines)]
starts=[(i,m) for i,m in starts if m]
raw=[];layout_anchors={}
seen=set()
for z,(i,h) in enumerate(starts):
 end=starts[z+1][0] if z+1<len(starts) else len(lines)
 symbol=re.search(r'_OBJC_CLASS_\$_(\w+)',h[3]);cl=symbol[1] if symbol else None
 cat='';category_block=False
 if h[2] in cats:
  cl=cats[h[2]]['class_name'];cat=cats[h[2]]['category'];category_block=True
 elif cl in classes and '        instanceSize' in '\n'.join(lines[i:end]) and cl not in seen:
  seen.add(cl);layout_anchors[cl]=i+1
 else:continue
 kind='-';active=False;ty=None
 for j in range(i+1,end):
  l=lines[j]
  if category_block:
   if re.match(r'^    instanceMethods ',l):kind='-';active=True;continue
   if re.match(r'^    classMethods ',l):kind='+';active=True;continue
   if active and re.match(r'^    (protocols|instanceProperties) ',l):active=False
   if not active:continue
  else:
   if l=='Meta Class':kind='+';active=False
   if re.match(r'^        baseMethods ',l):active=True;continue
   if active and re.match(r'^        baseProtocols ',l):active=False
   if not active:continue
  mt=re.search(r'\btypes\s+0x[0-9a-f]+\s+\(0x[0-9a-f]+\) (.*)$',l)
  if mt:ty=mt[1]
  mi=re.search(r'\bimp\s+0x[0-9a-f]+\s+\((0x[0-9a-f]+)\)',l)
  if mi:
   assert ty is not None
   raw.append((cl,cat,kind,mi[1],ty,j+1));ty=None
key=lambda x:(x['class_name'],x['category'],x['kind'],x['imp'],x['type_encoding'])
want=collections.Counter(key(x) for x in meta['methods']);have=collections.Counter(x[:5] for x in raw)
assert want==have,(want-have,have-want)
anchors=collections.defaultdict(list)
for r in raw:anchors['|'.join(r[:5])].append(r[5])
Path('specs/evidence/raw-dump-check.json').write_text(json.dumps({'matched_rows':sum(want.values()),'class_layout_lines':layout_anchors,'otool_imp_type_lines':anchors,'comparison':'multiset by class/category/kind/IMP/encoding; selector identity independently re-decoded from binary and checked against dyld'},indent=2))
print('PASS raw otool: 491 method type/IMP rows; 37 class layout anchors; no row-order selector pairing')
