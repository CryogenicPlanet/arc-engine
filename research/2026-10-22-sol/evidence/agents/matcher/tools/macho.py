"""Bounded thin arm64 Mach-O reader. Addresses are unslid virtual addresses.

No assumption that VA == file offset, or that a symbol/start defines a body.
Definitions: Apple xnu xnu-11417.121.6 EXTERNAL_HEADERS/mach-o/{loader,nlist}.h.
"""
import array
import collections
import hashlib
import os
import struct


def sha256_file(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        for b in iter(lambda: f.read(1024 * 1024), b''):
            h.update(b)
    return h.hexdigest()


class MachO:
    def __init__(self, path):
        self.path = str(path)
        self.fd = os.open(path, os.O_RDONLY)
        self.size = os.fstat(self.fd).st_size
        self.segments, self.sections, self.commands = [], [], []
        self.symtab, self.function_starts_command, self.dysymtab = None, None, None
        self.uuid = None
        hdr = self.read(0, 32)
        magic, cpu, subtype, ftype, nc, sc, flags, reserved = struct.unpack('<IiiIIIII', hdr)
        if magic != 0xfeedfacf or cpu != 0x100000c:
            raise ValueError('expected little-endian thin arm64 Mach-O')
        if nc > 100000 or sc > self.size - 32:
            raise ValueError('invalid load command bounds')
        self.header = dict(magic=hex(magic), cpu=hex(cpu), subtype=subtype,
                           filetype=ftype, ncmds=nc, sizeofcmds=sc, flags=hex(flags))
        off, limit = 32, 32 + sc
        for _ in range(nc):
            cmd, cs = struct.unpack('<II', self.read(off, 8))
            if cs < 8 or cs % 8 or off + cs > limit:
                raise ValueError('invalid load command extent')
            raw = self.read(off, cs)
            self.commands.append(dict(cmd=hex(cmd), offset=off, size=cs))
            if cmd == 0x19:
                if cs < 72:
                    raise ValueError('short segment')
                sn, va, vs, fo, fs, mp, ip, ns, fl = struct.unpack_from('<16sQQQQiiII', raw, 8)
                if 72 + ns * 80 != cs or fs > vs:
                    raise ValueError('invalid segment layout')
                self.check_range(fo, fs)
                sn = sn.rstrip(b'\0').decode('ascii')
                seg = dict(name=sn, va=va, vmsize=vs, fileoff=fo, filesize=fs,
                           maxprot=mp, initprot=ip)
                self.segments.append(seg)
                for i in range(ns):
                    v = struct.unpack_from('<16s16sQQIIIIIIII', raw, 72 + i * 80)
                    name, sg = (v[j].rstrip(b'\0').decode('ascii') for j in (0, 1))
                    a, n, o, align, relo, nr, sf = v[2:9]
                    if sg != sn or not (va <= a <= a + n <= va + vs):
                        raise ValueError('section outside segment VA range')
                    zerofill = sf & 0xff in (1, 0xc, 0x12)
                    if n and not zerofill:
                        self.check_range(o, n)
                        if not (fo <= o <= o + n <= fo + fs) or o - fo != a - va:
                            raise ValueError('section file mapping inconsistent with segment')
                    self.check_range(relo, nr * 8)
                    self.sections.append(dict(index=len(self.sections) + 1, segment=sg,
                        name=name, va=a, size=n, fileoff=o, flags=sf,
                        reserved1=v[9], reserved2=v[10],
                        zerofill=zerofill, executable=bool(ip & 4)))
            elif cmd == 0x2:
                if cs != 24 or self.symtab is not None:
                    raise ValueError('invalid/duplicate symtab')
                self.symtab = struct.unpack_from('<IIII', raw, 8)
                so, nn, st, ss = self.symtab
                self.check_range(so, nn * 16)
                self.check_range(st, ss)
            elif cmd == 0xb:
                if cs != 80 or self.dysymtab is not None:
                    raise ValueError('invalid/duplicate dysymtab')
                self.dysymtab = struct.unpack_from('<18I', raw, 8)
                self.check_range(self.dysymtab[12], self.dysymtab[13] * 4)
            elif cmd == 0x26:
                if cs != 16 or self.function_starts_command is not None:
                    raise ValueError('invalid/duplicate function starts')
                self.function_starts_command = struct.unpack_from('<II', raw, 8)
                self.check_range(*self.function_starts_command)
            elif cmd in (0x1d, 0x1e, 0x29, 0x2b, 0x2e, 0x80000033, 0x80000034):
                if cs != 16:
                    raise ValueError('invalid linkedit_data_command')
                self.check_range(*struct.unpack_from('<II', raw, 8))
            elif cmd == 0x1b:
                if cs != 24:
                    raise ValueError('invalid UUID command')
                self.uuid = raw[8:24].hex()
            off += cs
        if off != limit:
            raise ValueError('load command count/size mismatch')
        for key, length in [('va', 'vmsize'), ('fileoff', 'filesize')]:
            seq = sorted((s[key], s[key]+s[length]) for s in self.segments if s[length])
            if any(a[1] > b[0] for a, b in zip(seq, seq[1:])):
                raise ValueError('overlapping segment ranges')
        text = [s for s in self.segments if s['name'] == '__TEXT']
        if len(text) != 1:
            raise ValueError('expected one __TEXT segment')
        self.text_base = text[0]['va']

    def check_range(self, off, size):
        if off < 0 or size < 0 or off + size > self.size:
            raise ValueError(f'file range out of bounds: {off:x}+{size:x}')

    def read(self, off, size):
        self.check_range(off, size)
        b = os.pread(self.fd, size, off)
        if len(b) != size:
            raise ValueError('short read')
        return b

    def section(self, segment, name):
        found = [s for s in self.sections if (s['segment'], s['name']) == (segment, name)]
        if len(found) != 1:
            raise ValueError(f'expected one section {segment},{name}')
        return found[0]

    def va_offset(self, va, size=1):
        for s in self.segments:
            if s['va'] <= va and va + size <= s['va'] + s['filesize']:
                return s['fileoff'] + va - s['va']
        raise ValueError(f'VA {va:x}+{size:x} not file backed')

    def bytes_at(self, va, size):
        return self.read(self.va_offset(va, size), size)

    def in_text(self, va, size=1):
        s = self.section('__TEXT', '__text')
        return s['va'] <= va and va + size <= s['va'] + s['size']

    def starts(self):
        out = array.array('Q')
        if self.function_starts_command is None:
            return out
        raw = self.read(*self.function_starts_command)
        value = shift = 0
        va = self.text_base
        terminated = False
        for i, b in enumerate(raw):
            value |= (b & 127) << shift
            if shift > 63:
                raise ValueError('ULEB128 overflow')
            if b & 128:
                shift += 7
                continue
            if not value:
                if any(raw[i+1:]):
                    raise ValueError('nonzero data after function-start terminator')
                terminated = True
                break
            va += value
            if va % 4 or not self.in_text(va, 4):
                raise ValueError('function start outside aligned __text')
            out.append(va)
            value = shift = 0
        if not terminated:
            raise ValueError('unterminated function-start table')
        return out

    def symbols(self, with_index=False):
        if not self.symtab:
            return
        so, nn, st, ss = self.symtab
        cache = collections.OrderedDict()
        def name_at(idx):
            if idx >= ss:
                raise ValueError('symbol string offset out of bounds')
            chunks = []
            while idx < ss:
                pg, sub = divmod(idx, 1024*1024)
                if pg not in cache:
                    cache[pg] = self.read(st + pg*1024*1024, min(1024*1024, ss-pg*1024*1024))
                    if len(cache) > 8:
                        cache.popitem(last=False)
                else:
                    cache.move_to_end(pg)
                b = cache[pg]
                end = b.find(b'\0', sub)
                chunks.append(b[sub:end if end >= 0 else len(b)])
                if end >= 0:
                    return b''.join(chunks).decode('utf8', 'replace')
                idx = (pg+1)*1024*1024
            raise ValueError('unterminated symbol name')
        idx = 0
        for i in range(0, nn, 8192):
            b = self.read(so+i*16, min(8192, nn-i)*16)
            for strx, nt, ns, desc, value in struct.iter_unpack('<IBBHQ', b):
                if ns > len(self.sections) and not nt & 0xe0:
                    raise ValueError('invalid symbol section index')
                # Validate every name offset, even when no name is requested by caller.
                if strx >= ss:
                    raise ValueError('invalid symbol string index')
                row = (value, nt, ns, name_at(strx))
                yield (idx, *row) if with_index else row
                idx += 1

    def stubs(self, undefined_names):
        """Map exact S_SYMBOL_STUBS entry VA to validated indirect symbol name."""
        result = {}
        if not self.dysymtab:
            return result
        off, count = self.dysymtab[12:14]
        table = self.read(off, count * 4)
        for s in self.sections:
            if s['flags'] & 0xff != 8:
                continue
            stride = s['reserved2']
            if not stride or s['size'] % stride:
                raise ValueError('invalid stub stride')
            n = s['size'] // stride
            if s['reserved1'] + n > count:
                raise ValueError('stub indirect table out of bounds')
            for i in range(n):
                sym = struct.unpack_from('<I', table, (s['reserved1'] + i) * 4)[0]
                if not sym & 0xc0000000:
                    if sym >= self.symtab[1]:
                        raise ValueError('stub symbol index out of bounds')
                    if sym in undefined_names:
                        result[s['va'] + i * stride] = undefined_names[sym]
        return result

    def cstrings(self):
        s = self.section('__TEXT', '__cstring')
        tail, base = b'', s['va']
        for pos in range(0, s['size'], 1024*1024):
            b = tail + self.read(s['fileoff']+pos, min(1024*1024, s['size']-pos))
            start = 0
            while True:
                end = b.find(b'\0', start)
                if end < 0:
                    break
                if end > start:
                    yield base+start, b[start:end]
                start = end + 1
            tail, base = b[start:], base+start
            if len(tail) > 1024*1024:
                raise ValueError('unreasonably long C string')
        if tail:
            raise ValueError('unterminated __cstring')

    def describe(self):
        return dict(path=self.path, size=self.size, sha256=sha256_file(self.path),
            header=self.header, uuid=self.uuid, segments=self.segments, sections=self.sections,
            commands=self.commands, symtab=self.symtab,
            function_starts_command=self.function_starts_command,
            function_start_count=len(self.starts()))
