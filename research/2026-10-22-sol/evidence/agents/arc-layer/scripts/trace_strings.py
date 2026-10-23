#!/usr/bin/env python3
"""Conservative adjacent ARM64 ADRP+ADD string references and request inventory.

Only the two decoded instructions are evidence. Function attribution is a
nearest-entry / end-bound candidate, because saved Ghidra body ranges are absent.
No attempt to infer semantics, callback ownership, or all references is made.
"""
import argparse
import array
import bisect
import collections
import csv
import json
import re
import struct
from pathlib import Path
from inventory import MachO, digest, subsystem, tsv


def load(path):
    return list(csv.DictReader(open(path), delimiter='\t'))


def exact_strings(path, wanted):
    found = set()
    with open(path, 'rb') as f:
        tail = b''
        while True:
            b = f.read(1024*1024)
            data = tail + b if b else tail + b'\0'
            cut = len(data)
            if b:
                while cut and 32 <= data[cut-1] <= 126:
                    cut -= 1
            for m in re.finditer(rb'[ -~]{4,}', data[:cut]):
                s = m[0].decode('ascii')
                if s in wanted:
                    found.add(s)
            tail = data[cut:]
            if not b:
                break
    return found


PRIORITIES = {
    'browser_application': ('P0', 'launch-lifecycle candidate; verify required initialization and shutdown'),
    'browser_context': ('P0', 'profile/context candidate; verify creation and persistent state ownership'),
    'browser': ('P0', 'browser creation/ownership candidate'),
    'web_contents': ('P0', 'content lifecycle/navigation candidate'),
    'ipc_mojom': ('P1', 'IPC identity/registration candidate; required endpoints depend on launch call graph'),
    'ui_controllers': ('P1', 'Chrome-to-Arc UI hook candidate; reachability and fallback contract unknown'),
    'extensions': ('P1', 'extension integration; needed for feature parity, launch reachability unproven'),
    'downloads_network': ('P1', 'network/download integration; check profile and callback contracts'),
    'diagnostics_accessibility_performance': ('P1', 'observers may be installed at startup; inspect registration before deferring'),
    'security_credentials': ('P1', 'security and credential contracts; launch reachability unproven'),
    'notifications': ('P1', 'notification API; launch registration and delivery dependencies unknown'),
    'ai_content_conversion': ('P2', 'service feature candidate; not proven safe to omit during initialization'),
    'ad_blocking_media': ('P1', 'blocking/media integration; initialization and feature dependency unknown'),
    'artifacts_ui': ('P2', 'artifact UI/content feature; startup dependency and script-source policy unknown'),
    'importer': ('P2', 'import workflow candidate; launch/profile migration dependency unknown'),
    'other_named_arc': ('P2', 'unclassified Arc evidence; inspect before assigning criticality'),
}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--inputs', type=Path, default=Path('/home/exedev/work/inputs'))
    ap.add_argument('--out', type=Path, default=Path('initial'))
    args = ap.parse_args()
    out, inp = args.out, args.inputs
    engine = inp/'binaries/ArcCore-1.170.0.arm64'
    stock = inp/'binaries/ChromiumFramework-156.arm64'
    mach = MachO(engine)
    strings = load(out/'arc-strings.tsv')
    found = exact_strings(stock, {r['text'] for r in strings})
    for r in strings:
        r['present_as_exact_printable_run_in_stock'] = r['text'] in found
    tsv(out/'arc-strings.tsv', list(strings[0]), strings)
    targets = collections.defaultdict(list)
    for r in strings:
        targets[int(r['va'],16)].append(r)
    pages = {v & ~4095 for v in targets}
    refs = []
    for sec in mach.sections:
        if not (sec['flags'] & 0x80000000):
            continue
        # Read one MB plus the next instruction; never map the full binary.
        for off in range(0, sec['size'], 1024*1024):
            n = min(1024*1024 + 4, sec['size']-off)
            b = mach.read(sec['file_offset'] + off, n)
            words = array.array('I'); words.frombytes(b[:len(b)//4*4])
            for i in range(min(1024*1024//4, len(words)-1)):
                w, w2 = words[i], words[i+1]
                if w & 0x9f000000 != 0x90000000:  # ADRP
                    continue
                pc = sec['va'] + off + i*4
                imm = ((w >> 29) & 3) | (((w >> 5) & 0x7ffff) << 2)
                if imm & (1 << 20):
                    imm -= 1 << 21
                page = (pc & ~4095) + (imm << 12)
                if page not in pages:
                    continue
                # 64-bit ADD immediate, no flags, same source base register.
                if w2 & 0xff000000 != 0x91000000 or ((w2 >> 5) & 31) != (w & 31):
                    continue
                dst = page + (((w2 >> 10) & 0xfff) << (12 if w2 & (1 << 22) else 0))
                for r in targets.get(dst, []):
                    refs.append(dict(instruction_va=hex(pc), instruction_file_offset=hex(mach.offset(pc)),
                                     adrp_word=hex(w), add_word=hex(w2), string_va=hex(dst), string_file_offset=r['file_offset'],
                                     string_kind=r['kind'], text=r['text'], subsystem=r['subsystem'],
                                     stock_exact_string=r['present_as_exact_printable_run_in_stock']))
    starts, ends = array.array('Q'), array.array('Q')
    ftab = inp/'metadata/decomp/arccore-1.170.0-functions.tsv'
    for line in open(ftab):
        a,b,_,_ = line.rstrip('\n').split('\t',3)
        starts.append(int(a,16)); ends.append(int(b,16))
    indices = set()
    for r in refs:
        i = bisect.bisect_right(starts, int(r['instruction_va'],16))-1
        r['function_va'] = hex(starts[i]) if i >= 0 and int(r['instruction_va'],16) < ends[i] else ''
        r['function_attribution'] = 'nearest_entry_within_end_bound_not_body_verified' if r['function_va'] else 'no_nearest_entry_bound_match'
        if r['function_va']:
            indices.add(i)
    names = {}
    for i,line in enumerate(open(ftab)):
        if i in indices:
            a,b,size,name = line.rstrip('\n').split('\t',3)
            names[int(a,16)] = (name,int(b,16),int(size))
    for r in refs:
        r['function_name'] = names.get(int(r['function_va'],16), ('',))[0] if r['function_va'] else ''
    rf = ['instruction_va','instruction_file_offset','adrp_word','add_word','string_va','string_file_offset','string_kind','text','subsystem',
          'stock_exact_string','function_va','function_name','function_attribution']
    tsv(out/'string-xrefs.tsv', rf, refs)
    candidates = collections.defaultdict(list)
    for r in refs:
        if r['function_name'].startswith(('FUN_', 'thunk_FUN_')):
            candidates[r['function_va']].append(r)
    candidate_rows = []
    for va, rs in sorted(candidates.items(), key=lambda p:int(p[0],16)):
        name,end,size = names[int(va,16)]
        candidate_rows.append(dict(va=va, file_offset=hex(mach.offset(int(va,16))), name=name, expected_shard=f'0x{int(va,16)>>20:x}.c',
                                   reported_body_bytes=size, end_bound=hex(end), xref_count=len(rs),
                                   source_path_count=len({r['text'] for r in rs if r['string_kind']=='source_path'}),
                                   subsystem=','.join(sorted({r['subsystem'] for r in rs})),
                                   anchor_instruction_va=rs[0]['instruction_va'], evidence=rs[0]['text'],
                                   attribution='nearest_entry_within_end_bound_not_body_verified'))
    tsv(out/'unnamed-candidates.tsv', list(candidate_rows[0]), candidate_rows)

    named = load(out/'arc-functions.tsv')
    # Precise entry points requested first, then representative path-reference candidates.
    launch_selectors = {
        'ArcBrowserApplication::init:', 'ArcBrowserApplication::initialize:', 'ArcBrowserApplication::run',
        'ArcBrowserApplication::createBrowser:withDelegate:forWindow:',
        'ArcBrowserApplication::createBrowserContext:isHidden:type:completion:error:',
        'ArcBrowserApplication::loadBrowserContext:type:completion:', 'ArcBrowserApplication::quit',
        'ArcBrowserApplicationConfiguration::init:argv:profilePath:applicationID:isProduction:sessionRestorationPolicy:shouldLoadAllProfilesAtStartup:quitRunLoopOnTermination:userDefaultsSuiteName:',
        'ArcBrowser::initWithBrowser:', 'ArcBrowser::createWebContents:', 'ArcBrowser::takeOwnershipOfWebContents:',
        'ArcBrowserContext::initWithBrowserContext:', 'ArcWebContents::initWithWebContents:',
        'ArcWebContents::bootstrapRendering', 'ArcWebContents::bootstrapRenderingComplete',
        'ArcWebContents::setNavigationDelegate:', 'ArcWebContents::setUIDelegate:', 'ArcWebContents::setActionsDelegate:',
        'ArcWebContentsNavigationController::load:initiatorOrigin:transition:fromAddressBar:typedWithHttpScheme:',
        'ArcBrowserApplication::setNotificationCenter:', 'ArcBrowserApplication::setHistogramSamplesDelegate:',
        'ArcPerformanceController::setArcPerformanceObserver:', 'ArcCrashKeysReporter::setArcCrashKeysObserver:',
        'objc::Categories::ArcWebContents_Extensions_::createPaneHostView',
        'objc::Categories::ArcWebContents_Extensions_::nativeView',
        'objc::Categories::ArcWebContents_Extensions_::attachToPaneHostView:',
        'objc::Categories::ArcWebContents_Extensions_::detachFromPaneHostView:',
        'objc::Categories::ArcWebContents_Extensions_::close',
    }
    requests = {}
    def request(va, name, sub, reason, priority, anchor=''):
        row = dict(address=va, file_offset=hex(mach.offset(int(va,16))), expected_shard=f'0x{int(va,16)>>20:x}.c',
                   symbol_anchor=name, subsystem=sub, reason=reason, priority=priority, evidence_instruction_va=anchor,
                   image='ArcCore-1.170.0.arm64')
        if va not in requests or requests[va]['priority'] > priority:
            requests[va] = row
    for r in named:
        if r['name'] in launch_selectors:
            request(r['va'],r['name'],r['subsystem'],'ObjC boundary: inspect configuration, lifecycle, native object/delegate ownership and async completion contract','P0')
            dest=r.get('thunk_destination_va','')
            if dest.startswith('0x'):
                request(dest,'entry_'+dest,r['subsystem'],'direct branch target of '+r['name']+' at '+r['va']+'; native host view bridge','P0',r['va'])
    for r in load(out/'exported-interface.tsv'):
        if r['kind']=='function' and r['app168_import']=='True':
            request(r['va'],r['symbol'],'diagnostics_accessibility_performance',
                    'App-imported C function export; inspect calling convention, exception/trace contract and runtime reachability separately from ObjC scopes','P1')
    grouped = collections.defaultdict(list)
    for r in refs:
        if r['function_va']:
            grouped[(r['string_kind'],r['text'])].append(r)
    for (kind,text),rs in sorted(grouped.items()):
        if kind != 'source_path' and '.mojom.' not in text:
            continue
        # One function per path/endpoint gives a bounded initial transfer request.
        rs.sort(key=lambda r:(r['function_name'].startswith('thunk_'),int(r['function_va'],16)))
        r = rs[0]
        sub = r['subsystem']; priority,reason = PRIORITIES[sub]
        request(r['function_va'],r['function_name'],sub,f'{kind}: {text}; {reason}; nearest-entry attribution needs body-range confirmation',priority,r['instruction_va'])
    rows = sorted(requests.values(), key=lambda r:(r['priority'],int(r['address'],16)))
    tsv(out/'decompile-requests.tsv',['address','file_offset','expected_shard','symbol_anchor','subsystem','reason','priority','evidence_instruction_va','image'],rows)
    gaps = []
    for text in sorted({r['text'] for r in refs if r['string_kind']=='source_path'}):
        rs=[r for r in refs if r['string_kind']=='source_path' and r['text']==text]
        if not any(r['function_va'] for r in rs):
            r=rs[0]; pc=int(r['instruction_va'],16)
            gaps.append(dict(address=hex(pc),file_offset=r['instruction_file_offset'],expected_shard=f'0x{pc>>20:x}.c',
                             address_kind='instruction_not_function_entry',priority=PRIORITIES[r['subsystem']][0],
                             reason='Resolve exact Ghidra containing body ranges; shard is instruction-range hint, not confirmed function-entry shard: '+text))
    tsv(out/'decompile-gap-requests.tsv',['address','file_offset','expected_shard','address_kind','priority','reason'],gaps)
    tsv(out/'unattributed-xrefs.tsv',rf,[r for r in refs if not r['function_va']])
    subrows = []
    for sub in sorted(PRIORITIES):
        ns=[r for r in named if r['subsystem']==sub]
        ss=[r for r in strings if r['subsystem']==sub]
        xs=[r for r in refs if r['subsystem']==sub]
        priority,role=PRIORITIES[sub]
        subrows.append(dict(subsystem=sub,triage_priority=priority,launch_relevance=role,named_function_entries=len(ns),
                            named_reported_bytes=sum(int(r['reported_body_bytes']) for r in ns),
                            objc_class_scopes=len({r['scope'] for r in ns if r['kind']=='objc_class_method_anchor'}),
                            objc_category_scopes=len({r['scope'] for r in ns if r['kind']=='category_method'}),
                            arc_source_paths=len({r['normalized'] for r in ss if r['kind']=='source_path'}),
                            string_xrefs=len(xs),xref_function_candidates=len({r['function_va'] for r in xs if r['function_va']}),
                            example_named_anchor=ns[0]['name'] if ns else '',
                            example_path=next((r['normalized'] for r in ss if r['kind']=='source_path'),''),
                            example_xref_va=xs[0]['instruction_va'] if xs else ''))
    tsv(out/'subsystems.tsv', list(subrows[0]), subrows)
    stats=dict(engine_sha256=digest(engine),stock_sha256=digest(stock),string_xrefs=len(refs),
               xref_function_candidates=len({r['function_va'] for r in refs if r['function_va']}),
               unnamed_function_candidates=len(candidates),
               source_paths_referenced=len({r['text'] for r in refs if r['string_kind']=='source_path'}),
               unattributed_instruction_xrefs=sum(not r['function_va'] for r in refs),
               source_path_gap_requests=len(gaps),
               historical_exact_strings_in_stock=len({r['text'] for r in strings if r['kind']=='historical_string' and r['present_as_exact_printable_run_in_stock']}),
               historical_exact_strings_absent_stock=sorted({r['text'] for r in strings if r['kind']=='historical_string' and not r['present_as_exact_printable_run_in_stock']}),
               decompile_requests=len(rows),requested_shards=sorted({r['expected_shard'] for r in rows}),
               request_priorities=dict(collections.Counter(r['priority'] for r in rows)))
    (out/'xref-counts.json').write_text(json.dumps(stats,indent=2)+'\n')
    print(json.dumps({k:v for k,v in stats.items() if k!='historical_exact_strings_absent_stock'},indent=2))


if __name__=='__main__':
    main()
