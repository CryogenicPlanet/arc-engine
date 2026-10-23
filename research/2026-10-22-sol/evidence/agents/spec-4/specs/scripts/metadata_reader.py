#!/usr/bin/env python3
"""Streaming Arc-layer evidence inventory. Python stdlib only; no disassembler.

All addresses are unslid Mach-O VAs. The function table's third column is kept
verbatim as reported_body_bytes; its producer/contiguity is not assumed.
"""
import argparse
import array
import bisect
import collections
import csv
import hashlib
import json
import re
import struct
from pathlib import Path


def tsv(path, fields, rows):
    with open(path, 'w') as f:
        w = csv.DictWriter(f, fields, delimiter='\t', extrasaction='ignore')
        w.writeheader()
        w.writerows(rows)


def digest(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        for b in iter(lambda: f.read(1024 * 1024), b''):
            h.update(b)
    return h.hexdigest()


class MachO:
    def __init__(self, path):
        self.path = path
        self.f = open(path, 'rb')
        hdr = self.read(0, 32)
        assert struct.unpack_from('<I', hdr)[0] == 0xfeedfacf, path
        self.segments, self.sections = [], []
        self.commands = []
        self.bind_symbols = []
        self.pointer_formats = set()
        pos = 32
        for _ in range(struct.unpack_from('<I', hdr, 16)[0]):
            cmd, size = struct.unpack('<II', self.read(pos, 8))
            data = self.read(pos, size)
            self.commands.append(cmd)
            if cmd == 0x19:
                seg, va, vs, off, fs, mx, ini, ns, flags = struct.unpack_from('<16sQQQQiiII', data, 8)
                seg = seg.rstrip(b'\0').decode()
                self.segments.append(dict(segment=seg, va=va, virtual_size=vs, file_offset=off, file_size=fs))
                for j in range(ns):
                    sec, sg, a, sz, of, align, relo, nr, fl, r1, r2, r3 = struct.unpack_from('<16s16sQQIIIIIIII', data, 72 + j * 80)
                    self.sections.append(dict(segment=sg.rstrip(b'\0').decode(), section=sec.rstrip(b'\0').decode(),
                                              va=a, size=sz, file_offset=of, flags=fl))
            if cmd == 0x80000034:
                fixoff, fixsize = struct.unpack_from('<II', data, 8)
                ver, st, im, sy, count, imfmt, syfmt = struct.unpack('<7I', self.read(fixoff, 28))
                assert ver == 0 and imfmt == 1 and syfmt == 0
                for i in range(count):
                    raw = struct.unpack('<I', self.read(fixoff + im + i*4, 4))[0]
                    noff = fixoff + sy + (raw >> 9)
                    self.bind_symbols.append(self.read(noff, min(8192, path.stat().st_size-noff)).split(b'\0',1)[0].decode())
                startoff = fixoff + st
                scount = struct.unpack('<I', self.read(startoff, 4))[0]
                for i in range(scount):
                    rel = struct.unpack('<I', self.read(startoff + 4 + 4*i, 4))[0]
                    if rel:
                        self.pointer_formats.add(struct.unpack('<H', self.read(startoff + rel + 6, 2))[0])
            pos += size
        assert self.pointer_formats in ({2}, {6}), self.pointer_formats
        self.imagebase = min(s['va'] for s in self.segments if s['file_size'])

    def read(self, off, n):
        self.f.seek(off)
        b = self.f.read(n)
        assert len(b) == n, (off, n, len(b))
        return b

    def offset(self, va):
        for s in self.segments:
            if s['va'] <= va < s['va'] + s['file_size']:
                return s['file_offset'] + va - s['va']
        raise ValueError('unmapped VA ' + hex(va))

    def va(self, off):
        for s in self.segments:
            if s['file_offset'] <= off < s['file_offset'] + s['file_size']:
                return s['va'] + off - s['file_offset']
        raise ValueError('unmapped file offset ' + hex(off))

    def vread(self, va, n):
        return self.read(self.offset(va), n)

    def ptr(self, va):
        return self.decode_ptr(struct.unpack('<Q', self.vread(va, 8))[0])

    def bind_symbol(self, va):
        raw = struct.unpack('<Q', self.vread(va, 8))[0]
        return self.bind_symbols[raw & 0xffffff] if raw >> 63 else ''

    def decode_ptr(self, raw):
        # This image uses DYLD_CHAINED_PTR_64 (2). Raw on-disk pointers carry
        # chain-next bits; they are not runtime addresses. Imported binds have
        # no local target and are left unresolved (0). See pinned fixup header.
        if raw == 0 or raw >> 63:
            return 0
        target = raw & ((1 << 36) - 1)
        if self.pointer_formats == {6}:
            target += self.imagebase
        return target | (((raw >> 36) & 0xff) << 56)

    def string(self, va):
        if not va:
            return ''
        b = self.read(self.offset(va), min(8192, self.path.stat().st_size - self.offset(va)))
        return b.split(b'\0', 1)[0].decode('utf-8', 'replace')

    def section_ptrs(self, name):
        for s in self.sections:
            if s['section'] == name:
                for (p,) in struct.iter_unpack('<Q', self.read(s['file_offset'], s['size'])):
                    yield self.decode_ptr(p)

    def method_list(self, va):
        if not va:
            return []
        flags, n = struct.unpack('<II', self.vread(va, 8))
        small, direct = flags & 0x80000000, flags & 0x40000000
        stride = flags & 0xffff
        assert stride >= (12 if small else 24) and n < 10000, (hex(va), flags, n)
        result = []
        for i in range(n):
            a = va + 8 + i * stride
            if small:
                nm, ty, imp = struct.unpack('<iii', self.vread(a, 12))
                sel = a + nm
                if not direct:
                    sel = self.ptr(sel)
                ty, imp = a + 4 + ty, a + 8 + imp
            else:
                sel, ty, imp = (self.ptr(a + j) for j in (0, 8, 16))
            result.append(dict(selector=self.string(sel), type_encoding=self.string(ty),
                               imp=hex(imp), method_record_va=hex(a), selector_va=hex(sel)))
        return result


def dyld_inventory(path):
    classes, categories, protocols, methods = [], [], [], []
    owner, cat = '', ''
    with open(path) as f:
        for lineno, line in enumerate(f, 1):
            m = re.search(r'@interface (\w+)(?:\(([^)]+)\))?(?: : (\S+))?(?:\s+<([^>]+)>)?', line)
            if m:
                owner, cat, supercls, protos = m.groups()
                cat = cat or ''
                (categories if cat else classes).append(dict(class_name=owner, category=cat,
                      superclass=supercls or '', protocols=protos or '', dump_line=lineno))
            m = re.search(r'@protocol (\w+)\s*:\s*(.*)', line)
            if m:
                protocols.append(dict(protocol=m[1], inherited=m[2].strip(), dump_line=lineno))
            m = re.search(r'(0x[0-9A-Fa-f]+)\s+([-+])\[([^ ]+) ([^]]+)\]', line)
            if m:
                imp, kind, cls, sel = m.groups()
                assert cls == owner, (cls, owner)
                methods.append(dict(class_name=owner, category=cat, kind=kind, selector=sel,
                                    imp=hex(int(imp, 16)), dump_line=lineno))
    return classes, categories, protocols, methods


def binary_objc(mach):
    classes, categories, protocols, methods = [], [], [], []
    visited = set()

    def plist(va):
        if not va:
            return []
        n = mach.ptr(va)
        assert n < 1000
        out = []
        for i in range(n):
            p = mach.ptr(va + 8 + 8 * i)
            out.append(protocol(p))
        return out

    def protocol(va):
        name = mach.string(mach.ptr(va + 8))
        if va in visited:
            return name
        visited.add(va)
        inherited = plist(mach.ptr(va + 16))
        counts = {}
        for off, kind, optional in [(24, '-', False), (32, '+', False), (40, '-', True), (48, '+', True)]:
            ms = mach.method_list(mach.ptr(va + off))
            counts[('optional_' if optional else 'required_') + kind] = len(ms)
            for m in ms:
                methods.append(dict(class_name='', protocol=name, category='', kind=kind,
                                    optional=optional, **m))
        protocols.append(dict(protocol=name, inherited=','.join(inherited), metadata_va=hex(va), **counts))
        return name

    def class_name(va):
        if not va:
            return ''
        ro = mach.ptr(va + 32) & ~7
        return mach.string(mach.ptr(ro + 24))

    for va in mach.section_ptrs('__objc_classlist'):
        ro = mach.ptr(va + 32) & ~7
        name = class_name(va)
        classes.append(dict(class_name=name, metadata_va=hex(va), metadata_file_offset=hex(mach.offset(va)),
                            superclass=class_name(mach.ptr(va + 8)) or mach.bind_symbol(va+8).removeprefix('_OBJC_CLASS_$_'), protocols=','.join(plist(mach.ptr(ro + 40)))))
        for clsva, kind in [(va, '-'), (mach.ptr(va), '+')]:
            ro = mach.ptr(clsva + 32) & ~7
            for m in mach.method_list(mach.ptr(ro + 32)):
                methods.append(dict(class_name=name, category='', protocol='', kind=kind, **m))
    for va in mach.section_ptrs('__objc_catlist'):
        cat = mach.string(mach.ptr(va))
        cls = class_name(mach.ptr(va + 8)) or mach.bind_symbol(va+8).removeprefix('_OBJC_CLASS_$_')
        # Imported category targets have an unresolved null class pointer.
        categories.append(dict(class_name=cls, category=cat, metadata_va=hex(va), protocols=','.join(plist(mach.ptr(va + 32)))))
        for off, kind in [(16, '-'), (24, '+')]:
            for m in mach.method_list(mach.ptr(va + off)):
                methods.append(dict(class_name=cls, category=cat, protocol='', kind=kind, **m))
    for va in mach.section_ptrs('__objc_protolist'):
        protocol(va)
    return classes, categories, protocols, methods


def subsystem(text):
    s = text.lower()
    rules = [
        ('ipc_mojom', ['.mojom.', 'arc/mojom/', 'arc::mojom']),
        ('importer', ['importer', 'importsource', 'bookmarkimport']),
        ('ai_content_conversion', ['ai_service', 'aiservice', 'aiagent', 'aiconfig', 'aiinference', 'aidistil', 'classifier',
                                    'commandbar', 'nomic', 'content_conversion', 'contentconversion', 'htmltomarkdown', 'extractedpage', 'contentnode', 'pagecontentextraction', 'chat_manager']),
        ('ad_blocking_media', ['ad_block', 'blocklists', 'excludeddomains', 'platform_media', 'media_session', 'mediasession', 'mediaposition', 'mediaimage', 'widevine']),
        ('artifacts_ui', ['artifact']),
        ('extensions', ['extension', '/boost', 'boostide', 'sidepanel']),
        ('ui_controllers', ['/ui/', 'dialog', 'infobar', 'menumodel', 'menuitem', 'permissionrequest', 'keyevent', 'keyboard', 'selectoption', 'formcontrol', 'contextmenu', 'overscroll', 'topband', 'hslamat', 'arccolor', 'arcimage', 'displayname']),
        ('browser_application', ['browser_application', 'browserapplication', 'browser_runner', 'content_browser_client', 'arc_paths', 'local_state', 'localstate', 'feature_config']),
        ('browser_context', ['browser_context', 'browsercontext', 'history', 'cookie', 'bookmark', 'searchengine', 'autocomplete', 'sitesetting', 'siteengagement', 'mostvisited', 'annotatedvisit', 'visitcategory']),
        ('web_contents', ['web_contents', 'webcontents', 'navigation', 'backforward', 'render_frame_agent', 'translate_helper', '/blink/', 'content/public', 'content/browser', 'findresult', 'findtext', 'metatag']),
        ('browser', ['browser_impl', 'arcbrowser', 'session', 'tabgroup']),
        ('downloads_network', ['download', 'network', 'url_load', 'urlload', 'urlutilities', 'redirectentry']),
        ('security_credentials', ['spake', 'securecontext', 'logincredential', 'creditcard', 'addressentry', 'webauthn', 'devicetrust', 'unexportable', 'payment']),
        ('notifications', ['notification']),
        ('diagnostics_accessibility_performance', ['crash', 'metrics', 'histogram', 'breadcrumb', 'tracing', 'accessibility', 'performance', 'freezing', 'memory', 'cpuinfo', 'gpulog', 'posted_from_logger']),
    ]
    # Category spelling Extensions means ObjC API additions, not browser extensions.
    s = s.replace('_extensions_', '_').replace('(extensions)', '')
    for k, keys in rules:
        if any(x in s for x in keys):
            return k
    return 'other_named_arc'


def scan_strings(mach, evidence):
    """Scan every printable run with chunk overlap; retain only selected evidence."""
    rows, source_paths = [], set()
    with open(mach.path, 'rb') as f:
        tail, base = b'', 0
        while True:
            chunk = f.read(1024 * 1024)
            if not chunk:
                data = tail + b'\0'
            else:
                data = tail + chunk
            # Keep an unfinished printable run for next chunk.
            last = len(data)
            if chunk:
                while last and 32 <= data[last - 1] <= 126:
                    last -= 1
            complete, tail = data[:last], data[last:]
            for m in re.finditer(rb'[ -~]{4,}', complete):
                b = m.group()
                s = b.decode('ascii')
                for p in re.finditer(r'\.\./\.\./([A-Za-z0-9_./+-]+\.(?:cc|mm|h|m|cpp|c))', s):
                    source_paths.add(p[1])
                    if p[1].startswith('arc/'):
                        off = base + m.start() + p.start()
                        rows.append(dict(kind='source_path', text=p[0], normalized=p[1], file_offset=hex(off),
                                         va=hex(mach.va(off)), subsystem=subsystem(p[1])))
                if s in evidence or re.search(r'\barc(?:::|\.mojom\.)', s):
                    off = base + m.start()
                    rows.append(dict(kind='historical_string' if s in evidence else 'arc_namespace_string',
                                     text=s, normalized=s, file_offset=hex(off), va=hex(mach.va(off)), subsystem=subsystem(s)))
            base += last
            if not chunk:
                break
    return rows, source_paths


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--inputs', type=Path, default=Path('/home/exedev/work/inputs'))
    ap.add_argument('--out', type=Path, default=Path('initial'))
    args = ap.parse_args()
    out, inp = args.out, args.inputs
    out.mkdir(parents=True, exist_ok=True)
    binary = inp / 'binaries/ArcCore-1.170.0.arm64'
    mach = MachO(binary)
    tab = inp / 'metadata/decomp/arccore-1.170.0-functions.tsv'
    data = inp / 'context/analysis/data'
    selected, starts, ends, sizes = [], array.array('Q'), array.array('Q'), array.array('Q')
    totals, scope_counts = collections.Counter(), collections.Counter()
    for line in open(tab):
        a, b, size, name = line.rstrip('\n').split('\t', 3)
        a, b, size = int(a, 16), int(b, 16), int(size)
        assert not starts or starts[-1] < a
        starts.append(a); ends.append(b); sizes.append(size)
        totals['functions'] += 1
        totals['reported_body_bytes'] += size
        totals['bound_span_bytes'] += b - a
        totals['size_differs_from_span'] += size != b - a
        totals['name_prefixed_thunk'] += name.startswith('thunk_')
        # Exact scope boundary excludes NSKeyedArchiver and NSBezierPath arc* selectors.
        arc = re.match(r'^(?:thunk_)?(?:Arc\w*::|objc::Categories::Arc\w*_|arc::)', name)
        if not arc:
            continue
        category = name.startswith('objc::Categories::')
        kind = 'category_method' if category else ('cpp_namespace_function' if name.startswith('arc::') else 'objc_class_method_anchor')
        scope = name.rsplit('::', 1)[0]
        scope_counts[scope] += 1
        selected.append(dict(image='ArcCore-1.170.0.arm64', va=hex(a), end_bound=hex(b), reported_body_bytes=size,
                             bound_span_bytes=b-a, file_offset=hex(mach.offset(a)), name=name, scope=scope, kind=kind,
                             name_prefixed_thunk=name.startswith('thunk_'), subsystem=subsystem(scope), expected_shard=f'0x{a >> 20:x}.c'))
    fields = ['image', 'va', 'end_bound', 'reported_body_bytes', 'bound_span_bytes', 'file_offset', 'name', 'scope', 'kind', 'name_prefixed_thunk', 'subsystem', 'expected_shard']
    tsv(out / 'arc-functions.tsv', fields, selected)
    byva = {r['va']: r for r in selected}
    arc_stats = collections.Counter()
    for r in selected:
        arc_stats[r['kind']] += 1
        arc_stats['reported_body_bytes'] += r['reported_body_bytes']
        arc_stats['bound_span_bytes'] += r['bound_span_bytes']
    arc_stats['functions'] = len(selected)
    arc_stats['scopes'] = len(scope_counts)
    arc_stats['class_scopes'] = sum(not k.startswith(('objc::', 'arc::')) for k in scope_counts)
    arc_stats['category_scopes'] = sum(k.startswith('objc::Categories::') for k in scope_counts)

    dc, dcat, dp, dm = dyld_inventory(data / 'dobjc170.txt')
    bc, bcat, bp, bm = binary_objc(mach)
    assert {r['class_name'] for r in bc} == {r['class_name'] for r in dc}
    assert {r['protocol'] for r in bp} == {r['protocol'] for r in dp}
    assert {(r['class_name'],r['category']) for r in bcat} == {(r['class_name'],r['category']) for r in dcat}
    # Resolve imported category owner from dyld by (category, kind, selector, IMP).
    dlookup = {(r['category'], r['kind'], r['selector'], r['imp']): r['class_name'] for r in dm if r['category']}
    for r in bm:
        if r['category'] and not r['class_name']:
            r['class_name'] = dlookup.get((r['category'], r['kind'], r['selector'], r['imp']), '')
    key = lambda r: (r['class_name'], r['category'], r['kind'], r['selector'], r['imp'])
    binary_keys = {key(r) for r in bm if not r['protocol']}
    dyld_keys = {key(r) for r in dm}
    assert dyld_keys == binary_keys, (len(dyld_keys - binary_keys), len(binary_keys - dyld_keys))
    for r in bm:
        if r['imp'] != '0x0':
            r['imp_file_offset'] = hex(mach.offset(int(r['imp'], 16)))
            r['ghidra_anchor'] = byva.get(r['imp'], {}).get('name', '')
    arc_imp_methods = collections.defaultdict(list)
    for r in bm:
        if r['class_name'].startswith('Arc') and not r['protocol']:
            arc_imp_methods[r['imp']].append(r)
    for r in selected:
        r['primary_arc_objc_imp'] = r['va'] in arc_imp_methods
        r['instruction_thunk_pattern'] = ''
        r['thunk_destination_va'] = ''
        a = int(r['va'], 16)
        words = struct.unpack('<III', mach.vread(a, 12))
        if r['bound_span_bytes'] == 4 and words[0] & 0xfc000000 == 0x14000000:
            imm = words[0] & 0x3ffffff
            if imm & (1 << 25): imm -= 1 << 26
            r['instruction_thunk_pattern'] = 'B_immediate'
            r['thunk_destination_va'] = hex(a + (imm << 2))
        elif r['bound_span_bytes'] == 12 and words[0] & 0x9f000000 == 0x90000000 and words[2] & 0xfffffc1f == 0xd61f0000:
            imm = ((words[0] >> 29) & 3) | (((words[0] >> 5) & 0x7ffff) << 2)
            if imm & (1 << 20): imm -= 1 << 21
            page = (a & ~4095) + (imm << 12)
            if words[1] & 0xff000000 == 0x91000000 and (words[0]&31) == ((words[1]>>5)&31) and (words[1]&31) == ((words[2]>>5)&31):
                r['instruction_thunk_pattern'] = 'ADRP_ADD_BR'
                r['thunk_destination_va'] = hex(page + (((words[1]>>10)&0xfff) << (12 if words[1]&(1<<22) else 0)))
            elif words[1] & 0xffc00000 == 0xf9400000 and (words[0]&31) == ((words[1]>>5)&31) and (words[1]&31) == ((words[2]>>5)&31):
                r['instruction_thunk_pattern'] = 'ADRP_LDR_BR_indirect'
                r['thunk_destination_va'] = 'indirect_slot:' + hex(page + (((words[1]>>10)&0xfff)*8))
    fields += ['primary_arc_objc_imp','instruction_thunk_pattern','thunk_destination_va']
    tsv(out / 'arc-functions.tsv', fields, selected)
    arc_stats['distinct_name_strings'] = len({r['name'] for r in selected})
    arc_stats['primary_arc_objc_imps'] = sum(r['primary_arc_objc_imp'] for r in selected)
    arc_stats['instruction_thunk_patterns'] = sum(bool(r['instruction_thunk_pattern']) for r in selected)
    arc_stats['cpp_namespace_function'] = sum(r['kind']=='cpp_namespace_function' for r in selected)
    imp_anchors = []
    for line in open(tab):
        a,b,size,name = line.rstrip('\n').split('\t',3)
        if a in arc_imp_methods:
            ms=arc_imp_methods[a]
            imp_anchors.append(dict(imp=a,file_offset=hex(mach.offset(int(a,16))),ghidra_anchor=name,
                                    arc_named_anchor=a in byva,method_declarations=len(ms),
                                    methods=';'.join(r['kind']+'['+r['class_name']+('('+r['category']+')' if r['category'] else '')+' '+r['selector']+']' for r in ms)))
    assert len(imp_anchors)==len(arc_imp_methods)
    tsv(out/'objc-arc-imp-anchors.tsv',['imp','file_offset','ghidra_anchor','arc_named_anchor','method_declarations','methods'],imp_anchors)
    tsv(out / 'objc-classes.tsv', ['class_name', 'metadata_va', 'metadata_file_offset', 'superclass', 'protocols'], bc)
    tsv(out / 'objc-categories.tsv', ['class_name', 'category', 'metadata_va', 'protocols'], bcat)
    tsv(out / 'objc-protocols.tsv', ['protocol', 'inherited', 'metadata_va', 'required_-', 'required_+', 'optional_-', 'optional_+'], bp)
    tsv(out / 'objc-methods.tsv', ['class_name', 'category', 'protocol', 'kind', 'optional', 'selector', 'type_encoding', 'imp', 'imp_file_offset', 'method_record_va', 'selector_va', 'ghidra_anchor'], bm)
    imported = {s.strip().split('$_', 1)[1] for s in open(data / 'app168-imports.txt') if s.startswith('_OBJC_CLASS_$_')}
    ic, ica, ip, im = dyld_inventory(data / 'dobjc168.txt')
    surface = [r for r in im if r['class_name'] in imported]
    tsv(out / 'app168-imported-class-declared-methods.tsv', ['class_name', 'category', 'kind', 'selector', 'imp', 'dump_line'], surface)
    objc_stats = dict(classes=len(bc), arc_classes=sum(r['class_name'].startswith('Arc') for r in bc), categories=len(bcat),
                     arc_categories=sum(r['class_name'].startswith('Arc') for r in dcat), protocols=len(bp),
                     arc_protocols=sum(r['protocol'].startswith('Arc') for r in bp), declared_methods=len(dm),
                     arc_declared_methods=sum(r['class_name'].startswith('Arc') for r in dm),
                     arc_base_methods=sum(r['class_name'].startswith('Arc') and not r['category'] for r in dm),
                     arc_category_methods=sum(r['class_name'].startswith('Arc') and bool(r['category']) for r in dm),
                     arc_unique_imps=len({r['imp'] for r in dm if r['class_name'].startswith('Arc')}),
                     dyld_binary_key_match=len(dyld_keys), imported_app168_classes=len(imported),
                     imported_class_declared_method_rows_168=len(surface),
                     imported_class_declared_unique_methods_168=len({(r['class_name'],r['kind'],r['selector']) for r in surface}),
                     imported_class_base_method_rows_168=sum(not r['category'] for r in surface))
    objc_stats['dyld168_arc_classes'] = sum(r['class_name'].startswith('Arc') for r in ic)
    objc_stats['dyld168_arc_categories'] = sum(r['class_name'].startswith('Arc') for r in ica)
    objc_stats['dyld168_arc_declared_methods'] = sum(r['class_name'].startswith('Arc') for r in im)
    evidence_path = inp / 'context/results/arc-strings-not-upstream.txt'
    evidence = set(evidence_path.read_text().splitlines())
    strings, paths = scan_strings(mach, evidence)
    tsv(out / 'arc-strings.tsv', ['kind', 'text', 'normalized', 'file_offset', 'va', 'subsystem'], strings)
    (out / 'all-source-paths.txt').write_text('\n'.join(sorted(paths)) + '\n')
    tsv(out / 'segments.tsv', ['segment', 'va', 'virtual_size', 'file_offset', 'file_size'], mach.segments)
    tsv(out / 'sections.tsv', ['segment', 'section', 'va', 'size', 'file_offset', 'flags'], mach.sections)
    hashes = {}
    for p in [binary, tab, data/'dobjc170.txt', data/'objc170.txt', data/'dobjc168.txt', data/'app168-imports.txt', evidence_path,
              inp/'context/vm/ghidra/ExportFunctions.java', inp/'context/vm/ghidra/DecompileAll.java']:
        hashes[str(p.relative_to(inp))] = digest(p)
    stats = dict(function_table=totals, named_arc=arc_stats, objc=objc_stats,
                 strings=dict(historical_entries=len(evidence), historical_matched=len({r['text'] for r in strings if r['kind']=='historical_string'}),
                              source_paths_all=len(paths), arc_source_paths=len({r['normalized'] for r in strings if r['kind']=='source_path'}),
                              arc_namespace_strings=len({r['text'] for r in strings if r['kind']=='arc_namespace_string'})),
                 engine_sha256=hashes['binaries/ArcCore-1.170.0.arm64'], sha256=hashes,
                 load_commands=[hex(x) for x in mach.commands])
    stats['pointer_formats'] = sorted(mach.pointer_formats)
    (out / 'counts.json').write_text(json.dumps(stats, indent=2)+'\n')
    print(json.dumps({k:v for k,v in stats.items() if k not in ('sha256','load_commands')}, indent=2))


if __name__ == '__main__':
    main()
