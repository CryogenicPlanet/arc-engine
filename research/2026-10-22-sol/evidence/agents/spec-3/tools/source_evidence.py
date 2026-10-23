#!/usr/bin/env python3
"""Pin and hash primary files actually cited; audit handoff scope, not function bounds."""
import csv,json,hashlib,collections,sqlite3
from pathlib import Path
ROOT=Path('/home/exedev/work/inputs');OUT=Path('specs')
def sha(p):
 h=hashlib.sha256()
 with p.open('rb') as f:
  for b in iter(lambda:f.read(1024*1024),b''):h.update(b)
 return h.hexdigest()
files={
 'base/time/time.h':[(537,548)],
 'components/omnibox/browser/autocomplete_match.h':[(860,902),(980,985),(1080,1094)],
 'components/download/public/common/download_item.h':[(190,217),(265,274),(474,505)],
 'components/sessions/core/session_types.h':[(30,110)],
 'base/trace_event/memory_dump_manager.h':[(80,110)],
 'ui/base/models/menu_model.h':[(216,228)],
 'content/public/browser/navigation_controller.h':[(565,592),(647,695)],
 'content/public/browser/reload_type.h':[(10,22)],
 'ui/base/page_transition_types.h':[(30,44),(110,135)],
}
manifest=[];excerpt=[]
for file,ranges in files.items():
 p=ROOT/'chromium-156'/file;lines=p.read_text().splitlines();manifest.append(dict(repository='Chromium',commit='d0c02bfad08ec92a6ffcf989e53cd995e6381a4c',file=file,local_path=str(p),sha256=sha(p),line_windows=ranges))
 for a,b in ranges:
  excerpt.append('\n'+file+f':{a}-{b}\n'+'\n'.join(f'{i+1}: {lines[i]}' for i in range(a-1,min(b,len(lines)))))
for file,ranges in [('include/internal/cef_types_mac.h',[(100,154)]),('include/cef_browser.h',[(80,120),(285,324),(659,675)])]:
 p=OUT/'sources/cef'/Path(file).name;lines=p.read_text().splitlines();manifest.append(dict(repository='CEF',commit='426920c33ef22551e0b8f48cf343d623ac25642d',file=file,local_path=str(p),sha256=sha(p),line_windows=ranges,public_url='https://raw.githubusercontent.com/chromiumembedded/cef/426920c33ef22551e0b8f48cf343d623ac25642d/'+file))
 for a,b in ranges:excerpt.append('\n'+file+f':{a}-{b}\n'+'\n'.join(f'{i+1}: {lines[i]}' for i in range(a-1,min(b,len(lines)))))
(OUT/'evidence/sources.json').write_text(json.dumps(manifest,indent=2));(OUT/'evidence/pinned-source-excerpts.txt').write_text('\n'.join(excerpt))
inputs=['binaries/ArcCore-1.170.0.arm64','binaries/Arc-1.168.0.arm64','context/analysis/data/objc170.txt','context/analysis/data/dobjc170.txt','context/class-assignments.json']
(OUT/'evidence/input-sha256.json').write_text(json.dumps({x:sha(ROOT/x) for x in inputs},indent=2))
hand=list(csv.reader(open(ROOT/'claude-handoff/results/arc-layer-functions.tsv'),delimiter='\t'))
(OUT/'evidence/handoff-audit.json').write_text(json.dumps({'path':str(ROOT/'claude-handoff/results/arc-layer-functions.tsv'),'sha256':sha(ROOT/'claude-handoff/results/arc-layer-functions.tsv'),'rows':len(hand),'call_depth_counts':dict(collections.Counter(x[1] for x in hand)),'producer':str(ROOT/'claude-handoff/scripts/arc_layer.py'),'producer_sha256':sha(ROOT/'claude-handoff/scripts/arc_layer.py'),'scope':'Bounded handoff labels only; columns are name/call_depth/seed_reason/reported_size (confirmed from producer). Column two is depth 0/1/2, not an address. Producer strips trailing colons and keys bodies/sizes by name; indirect calls and named non-Arc calls are not in its FUN-only direct-call closure. IMP lookup in this stage comes independently from raw Mach-O and exact corpus index. No closure completeness or proprietary-code volume inferred.'},indent=2))
db=sqlite3.connect(f'file:{ROOT}/corpus-index.sqlite?mode=ro',uri=True)
gaps=[]
for va in [0x44195b4,0x44197a8,0x441999c,0x4419b90]:
 assert not db.execute('select va from functions where va=?',(va,)).fetchone()
 gaps.append(dict(va=hex(va),expected_shard='0x44.c',origin='Decoded observer-bridge vtable prefix 0xda882d0, see vtable-prefixes.json',request='Bounded arm64 disassembly and branch-target/selector resolution at this exact target; indexed body absent does not establish thunk or failure.',question='Identify navigation callback selector, payload, weak-owner handling and ordering.'))
(OUT/'evidence/address-requests.json').write_text(json.dumps(gaps,indent=2))
print('Pinned 9 Chromium + 2 CEF files; streamed 5 input hashes; audited handoff',len(hand),'rows; recorded 4 callback address requests')
