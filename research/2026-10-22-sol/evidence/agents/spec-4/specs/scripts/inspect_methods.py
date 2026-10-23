import csv,sys,re
from pathlib import Path
rows=list(csv.DictReader(open('specs/evidence/declared-methods.tsv'),delimiter='\t'))
for r in rows:
 if sys.argv[1:] and r['class_name'] not in sys.argv[1:]:continue
 s=Path('specs/bodies/'+r['imp'][2:]+'.c').read_text()
 print('\n###',r['class_name'],r['category'],r['kind']+r['selector'],r['imp'])
 if '--full' in sys.argv:print(s);continue
 for line in s.splitlines():
  if any(x in line for x in ['FUN_','::','"','param_1 +','_objc_','return ','param_3','param_4','param_5','param_6','param_7','param_8']):
   if not any(x in line for x in ['WARNING','Stack Size','LOAcquire','LORelease','00f01bc0','013efb40','00005810','_objc_retain_fixup','_objc_release_fixup','Function:']):print(line)
