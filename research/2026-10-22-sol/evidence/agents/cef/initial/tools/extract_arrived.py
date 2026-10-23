#!/usr/bin/env python3
"""Extract only requested engine functions and index direct named FUN calls.

Optional: never waits for engine shards. Verifies each opened shard against its
provided manifest, streams functions, and records real file/line evidence.
"""
import csv,hashlib,json,pathlib,re
from inventory import ROOT,INPUT,write_tsv,MachO
import struct
directory=INPUT/'decompile'
if not (directory/'MANIFEST.json').is_file():
    print('Engine shards absent; no waiting.')
    raise SystemExit(0)
manifest={r['file']:r for r in json.loads((directory/'MANIFEST.json').read_text())}
requests=list(csv.DictReader((ROOT/'decompile-requests.tsv').open(),delimiter='\t'))
wanted={int(r['engine170_imp_va'],16) for r in requests}
primary=wanted.copy()
FOLLOWUP={
 0x42b32d0:'Browser creation backend; recover concrete Browser/View ownership.',
 0x42b3d74:'Context creation backend; observed empty-name and null-callback checks. Follow profile service operation.',
 0x42c35e4:'Autocomplete backend; follow controller dispatch at 0x20cd850.',
 0x42d5d2c:'Password import backend; follow importer initialization at 0x836a940.',
 0x42e6790:'Extension installation backend; follow Web Store installer call at 0x786cd50.',
 0x42eb57c:'Pinned extension order backend; identify actual toolbar/pref object and observer behavior.',
 0x4739114:'Pane attach adapter checks exact WebContentsPaneHostView class and calls attachWebContentsHostView:.',
 0x47391c8:'Pane detach adapter checks exact WebContentsPaneHostView class and calls detachWebContentsHostView.',
 0x4376580:'Native WebContents host-view setup called before pane attach/nativeView.',
 0x42bdee4:'Session restoration backend; recover actual profile/session service.',
 0x47390dc:'Binary branch target of createPaneHostView thunk; inspect object construction.',
 0x4737eb4:'Concrete pane attach: geometry, screen, visibility, focus, subview constraints and ArcMacCoalescedResizing path.',
 0x4738a30:'Concrete pane detach state and subview lifetime.',
 0x4738bb0:'Concrete pane state sync: bounds, hidden state, scale, screen and refresh timing.',
 0x7d64988:'Password-save backend target; body absent from supplied shards.',
 0x7bdc470:'Profile creation backend target; identify actual profile service and callback contract.',
 0x786cd50:'Extension installer backend target; distinguish service/UI/policy implementation.',
 0x20cd850:'Autocomplete controller dispatch target; identify provider/service semantics.',
}
wanted.update(FOLLOWUP)
records={};read_shards=[];chunks=[]
for filename in sorted({f'0x{a>>20:x}.c' for a in wanted}):
    path=directory/filename
    if not path.is_file():continue
    with path.open('rb') as stream:sha=hashlib.file_digest(stream,'sha256').hexdigest()
    assert sha==manifest[filename]['sha256'],filename
    read_shards.append(dict(file=str(path),sha256=sha))
    address=None;start=0;block=[]
    def finish():
        if address not in wanted:return
        body=''.join(block)
        chunks.append(f'/* SOURCE {path}:{start} sha256 {sha} */\n'+body)
        calls=sorted({int(x,16) for x in re.findall(r'\bFUN_([0-9a-fA-F]+)\s*\(',body) if int(x,16)!=address})
        records[address]=dict(engine170_imp_va=hex(address),source=str(path),source_line=start,
            source_sha256=sha,extracted_lines=len(block),direct_named_FUN_targets=','.join(hex(x) for x in calls),
            next_shards_available=','.join(f'0x{x>>20:x}.c' for x in calls if (directory/f'0x{x>>20:x}.c').exists()),
            caveat='Decompiled direct-call syntax only; no inferred virtual-call targets, semantics, ABI or ownership proof.')
    for line_no,line in enumerate(path.open(errors='replace'),1):
        m=re.match(r'// ==== (0x[0-9a-f]+)\s+',line)
        if m:
            finish();address=int(m[1],16);start=line_no;block=[]
        if address in wanted:block.append(line)
    finish()
(ROOT/'evidence/arrived-requested-functions.c').write_text('\n'.join(chunks))
rows=[]
for r in requests:
    a=int(r['engine170_imp_va'],16)
    if a in records:rows.append(dict(status='extracted',**records[a]))
    else:rows.append(dict(status='exact_start_not_exported' if (directory/f'0x{a>>20:x}.c').exists() else 'requested_shard_absent',engine170_imp_va=hex(a),source='',source_line='',source_sha256='',extracted_lines='',direct_named_FUN_targets='',next_shards_available='',caveat='No waiting; request remains open.'))
write_tsv(ROOT/'evidence/arrived-requested-functions.tsv',rows)
followups=[]
for a,question in FOLLOWUP.items():
    followups.append(dict(engine170_imp_va=hex(a),question=question,
        status='extracted' if a in records else 'requested_shard_absent',
        source=records.get(a,{}).get('source',''),source_line=records.get(a,{}).get('source_line',''),
        source_sha256=records.get(a,{}).get('source_sha256',''),
        direct_named_FUN_targets=records.get(a,{}).get('direct_named_FUN_targets','')))
write_tsv(ROOT/'decompile-followups.tsv',followups)
# Ghidra export omits this four-byte thunk. Decode only the actual B instruction.
engine=MachO(INPUT/'binaries/ArcCore-1.170.0.arm64')
a=0x4410bb4;word=struct.unpack_from('<I',engine.mem,engine.offset(a))[0]
assert word&0xfc000000==0x14000000
imm=word&0x3ffffff
if imm&0x2000000:imm-=0x4000000
target=a+imm*4
assert target==0x47390dc
write_tsv(ROOT/'evidence/engine-branch-thunks.tsv',[
    dict(engine_imp_va=hex(a),instruction_word=hex(word),branch_target_va=hex(target),
         target_source=records[target]['source'],target_source_line=records[target]['source_line'],
         evidence='Binary B imm26 instruction, not an inferred Ghidra interval.')])
(ROOT/'evidence/arrived-shards.json').write_text(json.dumps(read_shards,indent=2)+'\n')
print(f'Extracted {len(primary & records.keys())}/{len(primary)} original requests and {len(FOLLOWUP.keys() & records.keys())}/{len(FOLLOWUP)} followups from {len(read_shards)} verified shards; one omitted thunk resolved from binary.')
