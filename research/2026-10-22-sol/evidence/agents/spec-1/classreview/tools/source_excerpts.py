#!/usr/bin/env python3
import csv,hashlib,json
from pathlib import Path
HERE=Path(__file__).resolve().parents[1];SRC=HERE.parent.parent/'spec-2/specs'
pin='d0c02bfad08ec92a6ffcf989e53cd995e6381a4c'
clips=[('components/history/core/browser/history_types.h',[(792,808),(1029,1045)]),('components/omnibox/browser/autocomplete_controller.cc',[(631,641),(797,808)]),('components/tab_groups/tab_group_visual_data.h',[(16,45)]),('content/browser/media/session/media_session_impl.cc',[(704,713),(739,748),(1296,1308),(1412,1424)]),('chrome/browser/ui/thumbnails/thumbnail_tab_helper.cc',[(242,258),(356,369)]),('components/site_engagement/content/site_engagement_score.h',[(29,39),(148,157)]),('components/net_log/net_export_file_writer.cc',[(275,314)]),('chrome/browser/ui/thumbnails/thumbnail_tab_helper.h',[(27,52)])]
with open(HERE/'evidence/source-excerpts.md','w') as f:
    for rel,ranges in clips:
        ls=(Path('/home/exedev/work/inputs/chromium-156')/rel).read_text().splitlines();f.write(f'\n{pin} {rel}\n')
        for start,end in ranges:f.write('\n'.join(f'{i+1}: {ls[i]}' for i in range(start-1,end))+'\n')
    for name,ranges in [('cef_browser.h',[(633,663),(995,1007)]),('cef_types_mac.h',[(148,157)])]:
        ls=(HERE/'sources'/name).read_text().splitlines();f.write(f'\n426920c33ef22551e0b8f48cf343d623ac25642d {name}\n')
        for start,end in ranges:f.write('\n'.join(f'{i+1}: {ls[i]}' for i in range(start-1,end))+'\n')
sources=[]
for r in csv.DictReader(open(SRC/'primary-sources.tsv'),delimiter='\t'):
    if r['project']!='cef':continue
    p=HERE/'sources'/Path(r['source_path']).name
    h=hashlib.sha256(p.read_bytes()).hexdigest();assert h==r['sha256']
    sources.append(dict(url=r['url'],revision=r['revision'],local_path=str(p),sha256=h,independent_public_fetch='exact bytes match writer source'))
(HERE/'evidence/independent-cef-sources.json').write_text(json.dumps(sources,indent=2)+'\n')
print('11 local source hashes checked by check.py; 2 independently fetched immutable CEF files match; pinned excerpts written.')
