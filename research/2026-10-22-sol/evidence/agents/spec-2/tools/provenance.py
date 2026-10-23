import hashlib,json,csv,re,collections
from pathlib import Path
R=Path('/home/exedev/work/inputs');O=Path('specs/evidence')
def sha(p):
 h=hashlib.sha256()
 with open(p,'rb') as f:
  for b in iter(lambda:f.read(1048576),b''):h.update(b)
 return h.hexdigest()
paths=[R/'context/analysis/data/objc170.txt',R/'context/analysis/data/dobjc170.txt',R/'context/class-assignments.json',R/'context/ENGINE.md',R/'context/lead-review.md',R/'claude-handoff/results/arc-layer-functions.tsv',Path('../arc-layer/initial/objc-methods.tsv'),Path('../arc-layer/scripts/inventory.py'),Path('tools/macho_metadata.py')]
json.dump({str(p):sha(p) for p in paths},open(O/'input-provenance.json','w'),indent=2)
# The handoff schema is name, call-depth, seed-reason, reported-body-bytes. Not an address table.
assignment=json.load(open(R/'context/class-assignments.json'))['groups']['spec-2']['classes'];rows=list(csv.reader(open(R/'claude-handoff/results/arc-layer-functions.tsv'),delimiter='\t'))
selected=[r for r in rows if r[0].split('::')[0] in assignment]
ms=json.load(open(O/'methods-bodies.json'));byname=collections.defaultdict(list)
for m in ms:byname[(m['class_name']+'::'+m['selector']).rstrip(':')].append(m['imp'])
matched=[dict(handoff_name=r[0],depth=r[1],reason=r[2],reported_body_bytes=r[3],metadata_imps=byname.get(r[0].rstrip(':'),[])) for r in selected]
audit=dict(schema=['name','depth','reason','reported_body_bytes'],rows=len(rows),assigned_class_named_rows=len(selected),assigned_named_rows_with_zero_reported_bytes=sum(r[3]=='0' for r in selected),addresses_inferred_only_for_FUN_names=True,explicit_per_row_addresses=False,known_excluded_malformed='FUN_0421f778 never read or used',matched=matched)
json.dump(audit,open(O/'handoff-audit.json','w'),indent=2)
srcs=[('chromium',p) for p in ['components/history/core/browser/history_types.h','components/omnibox/browser/autocomplete_controller.cc','components/tab_groups/tab_group_visual_data.h','components/net_log/net_export_file_writer.cc','components/net_log/net_export_file_writer.h','content/browser/media/session/media_session_impl.cc','chrome/browser/ui/thumbnails/thumbnail_tab_helper.h','chrome/browser/ui/thumbnails/thumbnail_tab_helper.cc','components/site_engagement/content/site_engagement_score.h']]+[('cef',p) for p in ['include/cef_browser.h','include/internal/cef_types_mac.h']]
with open('specs/primary-sources.tsv','w') as f:
 w=csv.writer(f,delimiter='\t');w.writerow(['project','revision','source_path','local_path','sha256','line_count','url'])
 for proj,p in srcs:
  rev='d0c02bfad08ec92a6ffcf989e53cd995e6381a4c' if proj=='chromium' else '426920c33ef22551e0b8f48cf343d623ac25642d'
  local=R/'chromium-156'/p if proj=='chromium' else Path('specs/sources/cef')/Path(p).name
  url=f'https://chromium.googlesource.com/chromium/src/+/{rev}/{p}' if proj=='chromium' else f'https://github.com/chromiumembedded/cef/blob/{rev}/{p}'
  w.writerow([proj,rev,p,str(local),sha(local),sum(1 for l in open(local)),url])
print('handoff rows',len(rows),'assigned named rows',len(selected),'assigned named rows with zero reported bytes',audit['assigned_named_rows_with_zero_reported_bytes'])
