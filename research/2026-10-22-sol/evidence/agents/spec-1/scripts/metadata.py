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


