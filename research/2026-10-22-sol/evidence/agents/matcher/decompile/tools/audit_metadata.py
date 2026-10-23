#!/usr/bin/env python3
"""Audit ObjC metadata identities independently of Ghidra's chosen symbol name."""
import collections
import csv
import hashlib
import json
import re
import sqlite3
from pathlib import Path

ROOT=Path('/home/exedev/work/inputs')
OUT=Path(__file__).resolve().parents[1]
D=sqlite3.connect(f'file:{ROOT}/corpus-index.sqlite?mode=ro',uri=True)
D.execute('pragma cache_size=-8192')
closure={int(r['va'],16) for r in csv.DictReader((OUT/'reconstructed-closure.tsv').open(),delimiter='\t')}
old={int(r['va'],16) for r in csv.DictReader((OUT/'handoff-crosswalk.tsv').open(),delimiter='\t') if r['va']}
rows=[]
cls=category=None
for lineno,line in enumerate((ROOT/'context/analysis/data/dobjc170.txt').open(),1):
    m=re.search(r'@interface\s+(\w+)(?:\(([^)]+)\))?',line)
    if m: cls,category=m.group(1),m.group(2) or ''; continue
    if '@end' in line: cls=category=None;continue
    if not cls or not cls.startswith('Arc'):continue
    m=re.search(r'(0x[0-9A-Fa-f]+)\s+([+-])\[(\w+) (.+)\]',line)
    if not m:continue
    address,kind,owner,sel=m.groups()
    assert owner==cls
    va=int(address,16)
    rr=D.execute('select name,path,offset,length,failed from functions where va=?',(va,)).fetchone()
    rows.append((cls,category,kind,sel,hex(va),lineno,rr[0] if rr else '',va in old,va in closure,
                 rr[1] if rr else '',rr[2] if rr else '',rr[3] if rr else '',rr[4] if rr else ''))
with (OUT/'objc-closure-audit.tsv').open('w') as f:
    w=csv.writer(f,delimiter='\t',lineterminator='\n')
    w.writerow(['class','category','kind','selector','engine_va','dyld_line','indexed_name',
                'in_handoff_address_set','in_reconstructed_address_set','path','offset','C_bytes','failed'])
    w.writerows(rows)

# Compare saved min/end arithmetic against current export header counts without
# assuming either supplies a contiguous or independently correct body map.
outliers=[];differences=[]
for line in (ROOT/'metadata/decomp/arccore-1.170.0-functions.tsv').open():
    a,e,size,n=line.rstrip('\n').split('\t');v=int(a,16);s=int(size)
    if v not in old and s<=1048576:continue
    rr=D.execute('select reported_size,path,offset,length,failed from functions where va=?',(v,)).fetchone()
    if rr is None:continue
    hs,p,o,l,failed=rr
    if v in old and hs!=s:differences.append((a,n,s,hs))
    if s<=1048576:continue
    with open(p,'rb') as f:f.seek(o);b=f.read(l).decode('utf-8','replace')
    labels=[int(x,16) for x in re.findall(r'^\s*LAB_([0-9a-f]{8}):',b,re.M)]
    warns=re.findall(r'/\* WARNING[^*]*\*/',b)
    outliers.append((a,e,n,s,hs,l,failed,hex(min(labels,default=v)),hex(max(labels,default=v)),
                     json.dumps(warns[:12]),p,o,'known_excluded' if v==0x421f778 else
                     'suspect_merged_distant_labels' if max((abs(x-v) for x in labels),default=0)>1048576 else
                     'saved_span_unusable_not_body_corruption_proof'))
for filename,fields,data in [
    ('saved-span-outliers.tsv',['va','saved_end_exclusive','name','saved_span_not_coverage','export_header_size_not_coverage',
      'C_bytes','failed','first_defined_label','last_defined_label','warnings_json','path','offset','assessment'],outliers),
    ('handoff-size-disagreements.tsv',['va','exact_name','saved_size_not_coverage','export_header_size_not_coverage'],differences)]:
    with (OUT/filename).open('w') as f:
        w=csv.writer(f,delimiter='\t',lineterminator='\n');w.writerow(fields);w.writerows(data)
summary={
 'metadata_method_rows':len(rows),'metadata_classes':len({r[0] for r in rows}),
 'base_method_rows':sum(not r[1] for r in rows),'category_method_rows':sum(bool(r[1]) for r in rows),
 'distinct_method_implementation_VAs':len({r[4] for r in rows}),
 'metadata_rows_in_handoff_address_set':sum(r[7] for r in rows),
 'metadata_rows_outside_handoff_address_set':sum(not r[7] for r in rows),
 'distinct_metadata_VAs_outside_handoff':len({r[4] for r in rows if not r[7]}),
 'metadata_rows_with_no_indexed_body':sum(not r[6] for r in rows),
 'metadata_rows_outside_reconstructed_address_set':sum(not r[8] for r in rows),
 'saved_span_outliers_over_1MiB':len(outliers),'saved_size_vs_header_disagreements_in_handoff':len(differences),
 'limits':['counts are declared metadata entries, not app calls',
           'shared implementation addresses do not imply class/selector/category identity',
           'outlier spans and distant labels are quarantine/review signals, not a proven number of corrupt functions']}
(OUT/'metadata-audit.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps(summary,indent=2))
