#!/usr/bin/env python3
import csv,json,collections
from pathlib import Path
O=Path(__file__).resolve().parents[1];p=Path('/home/exedev/work/inputs/claude-handoff/results/arc-layer-functions.tsv');rows=[s.rstrip('\n').split('\t') for s in p.open()];manifest={r[0]:r for r in rows};out=[]
for group,file in [('assigned_imp','body-index.json'),('selected_bridge','bridges.json')]:
 for va,r in json.load(open(O/'evidence'/file)).items():
  hit=manifest.get(r['name']);out.append(dict(group=group,va=va,indexed_name=r['name'],closure_name_present=bool(hit),closure_depth=hit[1] if hit else '',closure_reason=hit[2] if hit else ''))
with open(O/'evidence/closure-crosscheck.tsv','w') as f:
 w=csv.DictWriter(f,list(out[0]),delimiter='\t');w.writeheader();w.writerows(out)
s={'handoff_rows':len(rows),'depth_distribution':dict(collections.Counter(r[1] for r in rows)),'assigned_imps_in_handoff_by_indexed_name':sum(r['group']=='assigned_imp' and r['closure_name_present'] for r in out),'selected_bridges_in_handoff_by_name':sum(r['group']=='selected_bridge' and r['closure_name_present'] for r in out),'limits':'Second column is closure depth, not address. Name-only inventory crosscheck; exact addresses come from ObjC binary metadata/corpus index. Absent names may be shared IMP aliases or outside bounded closure, never proprietary-code or missing-body proof. No claim exhaustive seed/call-parser audit.'}
json.dump(s,open(O/'evidence/closure-crosscheck.json','w'),indent=2);print(json.dumps(s,indent=2))
