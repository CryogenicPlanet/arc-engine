#!/usr/bin/env python3
"""Cross-check IMP/type pairs without ever positional-pairing class/category methods."""
import re,csv,collections,json
from pathlib import Path
O=Path(__file__).resolve().parents[1];rows=list(csv.DictReader(open(O/'evidence/declared-methods.tsv'),delimiter='\t'));imps=set(r['imp'] for r in rows);found=collections.defaultdict(set);last=None
p=Path('/home/exedev/work/inputs/context/analysis/data/objc170.txt')
with p.open() as f:
 for n,line in enumerate(f,1):
  # Capture the WHOLE type encoding, including spaces in C++ template names.
  t=re.match(r'\s+types\s+0x[0-9a-f]+(?:\s+\(0x[0-9a-f]+\))?\s+(.+?)\s*$',line)
  if t:last=(t[1],n)
  imp=re.match(r'\s+imp\s+.*\((0x[0-9a-f]+)\)',line)
  if imp and last and hex(int(imp[1],16)) in imps:found[hex(int(imp[1],16))].add(last)
missing=[r for r in rows if r['type_encoding'] not in {t for t,n in found[r['imp']]}]
with open(O/'evidence/otool-anchors.tsv','w') as f:
 w=csv.writer(f,delimiter='\t');w.writerow(['class_name','category','kind','selector','imp_va','method_record_va','otool_type_line','type_encoding'])
 for r in rows:
  matches=sorted(n for t,n in found[r['imp']] if t==r['type_encoding'])
  if matches:w.writerow([r['class_name'],r['category'],r['kind'],r['selector'],r['imp'],r['method_record_va'],','.join(map(str,matches)),r['type_encoding']])
result={'method_rows':len(rows),'matching_imp_type_rows':len(rows)-len(missing),'missing':missing,'limit':'Raw otool independently corroborates IMP/type pairs only; owner/category/kind are established by direct binary metadata and raw dyld identity reconciliation, not a positional join.'}
json.dump(result,open(O/'evidence/otool-validation.json','w'),indent=2);print(json.dumps(result,indent=2));assert not missing
