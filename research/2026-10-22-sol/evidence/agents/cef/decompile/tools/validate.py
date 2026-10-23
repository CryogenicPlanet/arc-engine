#!/usr/bin/env python3
"""Validate evidence provenance, call-reference chains and the bounded mapping overlay."""
import csv,hashlib,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];OUT=ROOT/'decompile'
sys.path.insert(0,str(ROOT/'initial/tools'))
from inventory import MachO
fs=list(csv.DictReader((OUT/'evidence/functions.tsv').open(),delimiter='\t'))
bodies={int(r['engine_va'],16):(ROOT/r['local_excerpt']).read_text() for r in fs}
for r in fs:
 b=(ROOT/r['local_excerpt']).read_bytes()
 assert hashlib.sha256(b).hexdigest()==r['extracted_sha256']
 assert b.startswith(f"// ==== {r['engine_va']}  ".encode())
 assert not int(r['truncated']) and not int(r['failed'])
edges=[(0x43e8368,0x42b3d74),(0x42b3d74,0x7bdc470),(0x43e1f80,0x42fbd80),
       (0x42fbd80,0x20cd850),(0x42c35e4,0x20cd850),(0x43f5f60,0x42c2f5c),
       (0x43fd304,0x42e6790),(0x42e6790,0x786cd50),(0x42e6790,0x784a050),
       (0x43fdd3c,0x42eb57c),(0x42eb57c,0x7cd94c0),(0x42eb57c,0x7cd9ea0),
       (0x42eb57c,0x7cd9f40),(0x443f038,0x7d64988),(0x7d64988,0x78b2e84),
       (0x4410cd0,0x4739114),(0x4737eb4,0x473cad8),(0x473cad8,0x6846c88)]
for caller,callee in edges:assert f'FUN_{callee:08x}' in bodies[caller],(hex(caller),hex(callee))
anchors={0x7bdc470:['CreateMultiProfileAsync','chrome/browser/profiles/profile_manager.cc'],
         0x786cd50:['WebstoreInstallWithPrompt','webstore_install_with_prompt.cc'],
         0x20cd850:['AutocompleteController::Start','Omnibox.ProviderTime2.','Omnibox.Start.WantAsyncMatches'],
         0x7cd94c0:['extensions.pinned_extensions'],
         0x78b2e84:['passwords_account_storage_used'],
         0x6846c88:['render_widget_host_view_mac.mm','CATransaction::begin','CATransaction::commit'],
         0x440a85c:['NSMutableDictionary::dictionary','NSNumber::numberWithUnsignedInteger_','NSMutableDictionary::copy']}
for va,words in anchors.items():
 for word in words:assert word in bodies[va],(hex(va),word)
engine=MachO(Path('/home/exedev/work/inputs/binaries/ArcCore-1.170.0.arm64'))
word=struct.unpack_from('<I',engine.mem,engine.offset(0x4410bb4))[0]
assert word==0x140ca14a
assert 0x4410bb4+((word&0x3ffffff)<<2)==0x47390dc
getter=struct.unpack_from('<2I',engine.mem,engine.offset(0x2d0fb40))
assert getter==(0xf9400400,0xd65f03c0),[hex(w) for w in getter]
vtable_rows=[]
for slot in range(15):
 normal=engine.ptr(0xd9a2988+slot*8);unattended=engine.ptr(0xda7dfc0+slot*8)
 vtable_rows.append(dict(slot=slot,normal_table_va='0xd9a2988',normal_target=hex(normal),unattended_table_va='0xda7dfc0',unattended_target=hex(unattended),differs=int(normal!=unattended)))
assert [r['slot'] for r in vtable_rows if r['differs']]==[9,11]
assert vtable_rows[9]['unattended_target']=='0x614600'
with (OUT/'evidence/installer-vtables.tsv').open('w') as f:
 w=csv.DictWriter(f,fieldnames=list(vtable_rows[0]),delimiter='\t');w.writeheader();w.writerows(vtable_rows)
sources=list(csv.DictReader((OUT/'evidence/primary-sources.tsv').open(),delimiter='\t'))
for r in sources:
 assert hashlib.sha256(Path(r['source_path']).read_bytes()).hexdigest()==r['sha256']
mapping=list(csv.DictReader((OUT/'cef-mapping.tsv').open(),delimiter='\t'))
keys=[(r['class_name'],r['objc_category'],r['kind'],r['selector']) for r in mapping]
assert len(keys)==len(set(keys))==834
assert len({r['class_name'] for r in mapping})==63
assert sum(r['category']=='confirmed_chrome_internal_dependency' for r in mapping)==3
app=json.loads((OUT/'evidence/app-validation.json').read_text())
assert app['installExtensionFromWebstore:unattended:completion:']['binary_BL_calls']==4
assert app['createBrowserContext:isHidden:type:completion:error:']['binary_BL_calls']==1
assert app['autocomplete:cursorPosition:preventInlineAutocomplete:startFresh:keyword:error:completion:']['binary_BL_calls']==1
summary=dict(status='PASS',engine_excerpts=len(fs),verified_named_reference_edges=len(edges),
             primary_source_anchors=len(sources),imported_declared_methods=len(mapping),imported_classes=63,
             permission_getter_words=[hex(w) for w in getter],pane_creation_thunk_word=hex(word),
             limitations='Static provenance and evidence consistency only; no runtime, ABI, callback-executor or source-equivalence proof')
(OUT/'evidence/validation.json').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps(summary,indent=2))
