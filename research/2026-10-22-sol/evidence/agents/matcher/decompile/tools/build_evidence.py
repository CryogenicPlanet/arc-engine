#!/usr/bin/env python3
"""Materialize exact evidence and validate anchors for manually reviewed findings.

Assertions check artifact identity and cited anchors, not behavioral equivalence.
"""
import csv
import hashlib
import json
import re
import sqlite3
import struct
import sys
from pathlib import Path

OUT=Path(__file__).resolve().parents[1]
ROOT=Path('/home/exedev/work/inputs')
SRC=ROOT/'chromium-156'
PIN='d0c02bfad08ec92a6ffcf989e53cd995e6381a4c'
sys.path.insert(0,str(OUT.parent/'tools'))
from macho import MachO
from macho import sha256_file
D=sqlite3.connect(f'file:{ROOT}/corpus-index.sqlite?mode=ro',uri=True)
D.execute('pragma cache_size=-8192')
closure={int(r['va'],16) for r in csv.DictReader((OUT/'reconstructed-closure.tsv').open(),delimiter='\t')}

specs=[
 (0x167ca68,'PrefService::SetUserPrefValue','high_source_correspondence','observed_flow_agrees',
  'components/prefs/pref_service.cc',556,583,['Trying to write an unregistered pref: '],
  'Find preference; diagnose missing pref; compare Value type; clone/move value into user store. No modification established.'),
 (0x2736440,'KeepAliveRegistry::Register','high_source_correspondence','observed_flow_agrees',
  'components/keep_alive_registry/keep_alive_registry.cc',133,159,['New state of the KeepAliveRegistry: '],
  'Increment origin and total counts, optionally restart counts; notify on state transitions; update crash key.'),
 (0x2dd6dc0,'KeepAliveRegistry::Unregister','high_source_correspondence','observed_flow_agrees',
  'components/keep_alive_registry/keep_alive_registry.cc',161,189,['New state of the KeepAliveRegistry: '],
  'Decrement counts, notify keepalive before restart transition, then log/update crash key. Diagnostic spacing differs; no behavioral edit established.'),
 (0x3222b84,'bookmarks::ScheduleCallbackOnBookmarkModelLoad','high_source_correspondence','observed_flow_agrees',
  'components/bookmarks/browser/bookmark_model_load_waiter.cc',73,94,['ScheduleCallbackOnBookmarkModelLoad'],
  'CHECK callback; loaded branch posts task with weak model; unloaded branch constructs observing waiter. Does not establish app callback ABI.'),
 (0x3a70080,'content::GpuProcessHost::CallOnUI','high_source_correspondence','observed_flow_agrees',
  'content/browser/gpu/gpu_process_host.cc',693,705,['kind != GPU_PROCESS_KIND_INFO_COLLECTION'],
  'Non-Windows info-collection check, capture kind/force_create/callback, post to UI runner. No modification established.'),
 (0x27de0,'TabStripModel::GetIndexOfWebContents','high_source_correspondence','observed_flow_agrees',
  'chrome/browser/ui/tabs/tab_strip_model.cc',1222,1232,['iVar1 = -1'],
  'Traverse tabs, compare stored WebContents pointer, return index or -1. Used to check ScheduleUIUpdate guard.'),
 (0x16b9d84,'BrowserUiController::ScheduleUIUpdate','high_source_correspondence','upstream_function_with_arc_inline_and_guard_candidate',
  'chrome/browser/ui/browser_ui_controller/browser_ui_controller.cc',112,181,['BrowserUiController::UpdateToolbar','ScheduleUIUpdate','arc::browser::ui::Window::ResetToolbarTabState'],
  'URL/load flags, dirty-bit coalescing and delayed task match source role. Arc Window diagnostics inline. Arc guard calls 0x27de0; pinned guard compares TabInterface browser ownership. Behavioral distinction unresolved.'),
 (0x384784c,'chrome::BrowserCommandController::BrowserCommandController','high_source_correspondence','upstream_function_with_arc_inline',
  'chrome/browser/ui/browser_command_controller.cc',341,458,['select_file_dialogs.allowed','bookmarks.editing_enabled','ExtensionStateObserver','arc/browser/ui/browser_commands.cc'],
  'Constructor, command-updater choice, pref registrars, inlined InitCommandState and Arc UI helper diagnostics. Source also uses BrowserActions; its presence alone is not an Arc modification.'),
 (0x435da28,'Arc DevTools full-page screenshot preflight (descriptive)','observed_behavior_no_exact_cpp_name','extension_candidate',
  'content/public/browser/gpu_utils.cc',1,180,['managed webcontents is null.','GetMaxTextureSize'],
  'Full-page wrapper checks managed contents then supplies callback to GPU CallOnUI with GetMaxTextureSize source location. Referenced pinned gpu_utils.cc/.h lack this helper. Candidate added helper, not proven exact patch or full contract.'),
 (0x40bf0c8,'GPU callback forwarding maximum-texture-size request (descriptive)','observed_behavior_no_exact_cpp_name','extension_candidate',
  'content/browser/gpu/gpu_process_host.cc',1,1532,['gpu_host_'],
  'Move captured callback, check host pointer, access service and dispatch virtual slot +0xd8. No exact upstream helper name or slot semantics established.'),
]

extra=[0x435db94,0x435dd94,0x4376fb0,0x4435570,0x4435624,0x4442be0,0x43e6ec0,
       0x42b1ba8,0x42aac34,0x2d0fb40,0x440a85c,0x4410bb8,0x4410cd0,0x4410ec0,
       0x47390dc,0x4739114,0x47391c8,0x957ca0c,0x957d304,0x31086d4]
evidence=[];byva={}
for va in [s[0] for s in specs]+extra:
    row=D.execute('select * from functions where va=?',(va,)).fetchone()
    assert row is not None
    _,n,size,path,off,length,failed=row
    assert not failed
    with open(path,'rb') as f:f.seek(off);b=f.read(length)
    assert b.startswith(f'// ==== 0x{va:x}  '.encode())
    dest=OUT/'bodies'/f'0x{va:x}.c';dest.write_bytes(b)
    record=dict(va=hex(va),name=n,path=path,offset=off,length=length,sha256=hashlib.sha256(b).hexdigest(),
                local_body=str(dest.relative_to(OUT)),in_syntactic_closure=va in closure)
    evidence.append(record);byva[va]=(record,b.decode())

edges=list(csv.DictReader((OUT/'reconstructed-edges.tsv').open(),delimiter='\t'))
findings=[]
sources={}
for va,name,confidence,status,file,start,end,anchors,behavior in specs:
    record,b=byva[va]
    for anchor in anchors:assert anchor in b,(hex(va),anchor)
    data=(SRC/file).read_bytes()
    sources[file]=dict(file=file,pin=PIN,sha256=hashlib.sha256(data).hexdigest(),bytes=len(data),
                       url=f'https://chromium.googlesource.com/chromium/src/+/{PIN}/{file}')
    findings.append(dict(**record,candidate_name=name,confidence=confidence,status=status,
                         source=dict(file=file,start_line=start,end_line=min(end,len(data.splitlines())),pin=PIN),
                         observed_anchors=anchors,behavior=behavior,
                         callers_in_two_level_graph=[e for e in edges if e['callee_va']==hex(va)]))
for file in ['content/public/browser/gpu_utils.h','content/browser/gpu/gpu_process_host.h',
             'ui/accessibility/platform/browser_accessibility_cocoa.mm','chrome/browser/app_controller_mac.mm']:
    data=(SRC/file).read_bytes()
    sources[file]=dict(file=file,pin=PIN,sha256=hashlib.sha256(data).hexdigest(),bytes=len(data),
                       url=f'https://chromium.googlesource.com/chromium/src/+/{PIN}/{file}')
for file in ['content/public/browser/gpu_utils.cc','content/public/browser/gpu_utils.h',
             'content/browser/gpu/gpu_process_host.cc','content/browser/gpu/gpu_process_host.h']:
    assert 'GetMaxTextureSize' not in (SRC/file).read_text()

# Native-host metadata entry omitted from the decompiler index is a tail branch.
m=MachO(ROOT/'binaries/ArcCore-1.170.0.arm64')
engine_sha=sha256_file(ROOT/'binaries/ArcCore-1.170.0.arm64')
assert engine_sha==json.loads((OUT.parent/'initial/images.json').read_text())['arc']['sha256']
va=0x4410bb4
raw=m.bytes_at(va,4);w=struct.unpack('<I',raw)[0]
assert w&0xfc000000==0x14000000
imm=w&0x3ffffff
if imm&(1<<25):imm-=1<<26
target=va+imm*4
assert target==0x47390dc
branch=dict(image='ArcCore-1.170.0.arm64',address_basis='unslid engine VA',va=hex(va),
            file_offset=hex(m.va_offset(va,4)),bytes=raw.hex(),instruction_word=hex(w),
            decode='AArch64 B immediate, signed imm26 times four',target=hex(target),
            target_body=byva[target][0],limitation='4-byte branch only; no general disassembler or body membership inferred')
(OUT/'native-host-branch.json').write_text(json.dumps(branch,indent=2)+'\n')
(OUT/'body-evidence.json').write_text(json.dumps(evidence,indent=2)+'\n')
(OUT/'naming-evidence.json').write_text(json.dumps(findings,indent=2)+'\n')
with (OUT/'source-references.tsv').open('w') as f:
    fields=['file','pin','sha256','bytes','url']
    wr=csv.DictWriter(f,fields,delimiter='\t',lineterminator='\n');wr.writeheader();wr.writerows(sources.values())

# Record exact app callers, maintaining app-image addresses and stub-only limits.
selectors={'setPromptForDownloadPath_', 'setSearchSuggestEnabled_', 'captureFullPage_',
           'createPaneHostView','attachToPaneHostView_','detachFromPaneHostView_','nativeView','run'}
app=[]
for shard in ['0x1004.c','0x1005.c']:
    fn=ROOT/'app168/ghidra/decomp'/shard
    header=None
    for no,line in enumerate(fn.open(),1):
        if line.startswith('// ==== '):header=line.strip()
        for sel in selectors:
            if re.search(r'objc_stub::'+re.escape(sel)+r'\b',line):
                app.append((str(fn),no,header,sel,line.strip(),'selector-stub direct call; receiver class not independently proven'))
with (OUT/'app-callers.tsv').open('w') as f:
    wr=csv.writer(f,delimiter='\t',lineterminator='\n');wr.writerow(['path','line','function_header','stub','call','limit']);wr.writerows(app)

requests=[
 (1,'0x42d62c0','callback invoked through stored LAB address','0x4376fb0','0x42',
  'Full-page completion adapter and image conversion/ownership cannot be closed',
  'Recover actual function/body membership and bounded disassembly, not another copy of existing shard'),
 (1,'0x435e5c4','callback invoked through stored LAB address','0x435db94','0x43',
  'Page.captureScreenshot response/error conversion and completion contract unresolved',
  'Recover actual function/body membership and bounded disassembly'),
 (2,'0x4075dc0','callback invoked through stored LAB address','0x435da28','0x40',
  'Texture-size callback adapter threading/weak receiver validity unresolved',
  'Recover actual function/body membership and bounded disassembly'),
 (2,'0x2b7940','body exported but suspect merged recovery','saved-span-outliers.tsv','0x2',
  'Cannot assign distant switch regions to this entry or use it for matching',
  'Actual ordered Ghidra AddressSet blocks, original switch targets, and bounded disassembly'),
 (2,'0x16220b0','body exported but suspect merged recovery','saved-span-outliers.tsv','0x16',
  'Cannot assign distant parsing blocks to this entry or use it for matching',
  'Actual ordered Ghidra AddressSet blocks and bounded disassembly of cross-region jumps'),
]
with (OUT/'additional-shards.tsv').open('w') as f:
    wr=csv.writer(f,delimiter='\t',lineterminator='\n');wr.writerow(['priority','engine_va','status','referrer','existing_shard','dependent_conclusion','requested_evidence'])
    for r in requests:
        if 'LAB' in r[2]:assert D.execute('select 1 from functions where va=?',(int(r[1],16),)).fetchone() is None
        wr.writerow(r)
audit=json.loads((OUT/'audit.json').read_text());metadata=json.loads((OUT/'metadata-audit.json').read_text())
assert audit['handoff_rows']==5846 and audit['unique_shards']==199
assert not audit['unresolved_handoff_names'] and audit['missing_callee_bodies']==0
assert metadata['metadata_method_rows']==1964 and metadata['metadata_rows_with_no_indexed_body']==1
(OUT/'validation.json').write_text(json.dumps(dict(
    status='passed_artifact_and_anchor_checks_not_semantic_equivalence',
    exact_bodies_materialized=len(evidence),curated_source_comparisons=len(findings),source_files_hashed=len(sources),
    app_callsite_rows=len(app),native_host_tail_branch=branch,
    corpus_headers=audit['input_corpus']['headers'],source_pin=PIN,engine_sha256=engine_sha,
    validation_scope=['manifest SHA and length checks in audit_closure.py',
                      'exact header/address, failed flag and complete byte-range body extraction',
                      'manual source anchors present; source hashes recorded',
                      'metadata identity audit independent of Ghidra symbol name',
                      'missing requests checked against read-only index',
                      'no dynamic execution, ABI proof, general binary equivalence, or patch count']),indent=2)+'\n')
print(f'Validated {len(evidence)} exact bodies, {len(findings)} curated comparisons, {len(sources)} pinned source files, {len(app)} app callsites; native-host B target {target:#x}.')
