#!/usr/bin/env python3
"""Revision-only bounded static evidence. Never runs original reproducer scripts."""
import collections, csv, ctypes, fcntl, hashlib, json, re, sqlite3, struct, subprocess
from pathlib import Path

HERE = Path(__file__).resolve().parent
INPUT = Path('/home/exedev/work/inputs')
ORIGINAL = Path('/home/exedev/work/agents/spec-3/specs')
OUT = HERE / 'revision-evidence'

def sha(path):
    h = hashlib.sha256()
    with path.open('rb') as f:
        for block in iter(lambda: f.read(1024 * 1024), b''):
            h.update(block)
    return h.hexdigest()

def table(path, rows):
    with path.open('w') as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0]), delimiter='\t')
        w.writeheader(); w.writerows(rows)

class Image:
    """Bounded Mach-O segment translation and declared ObjC records."""
    def __init__(self, path):
        self.path = path; self.f = path.open('rb'); self.segments = []; self.sections = {}
        h = self.read(0, 32)
        assert struct.unpack_from('<I', h)[0] == 0xfeedfacf
        p = 32
        for _ in range(struct.unpack_from('<I', h, 16)[0]):
            cmd, n = struct.unpack('<II', self.read(p, 8)); b = self.read(p, n)
            if cmd == 0x19:
                va, vs, off, size = struct.unpack_from('<QQQQ', b, 24)
                self.segments.append((va, size, off))
                for i in range(struct.unpack_from('<I', b, 64)[0]):
                    s = 72 + i * 80
                    name = b[s:s+16].split(b'\0')[0].decode()
                    self.sections[name] = struct.unpack_from('<QQ', b, s+32)
            p += n
    def read(self, offset, n):
        assert 0 <= n <= 65536
        self.f.seek(offset); b = self.f.read(n); assert len(b) == n; return b
    def offset(self, va, n=1):
        for start, size, off in self.segments:
            if start <= va and va + n <= start + size: return off + va - start
        raise ValueError(hex(va))
    def data(self, va, n): return self.read(self.offset(va, n), n)
    def ptr(self, va):
        raw = struct.unpack('<Q', self.data(va, 8))[0]
        if not raw or raw >> 63: return 0
        # ArcCore chained DYLD_CHAINED_PTR_64 rebase targets.
        return (raw & 0xfffffffff) | (((raw >> 36) & 255) << 56)
    def string(self, va): return self.data(va, 4096).split(b'\0')[0].decode()
    def pointers(self, section):
        va, n = self.sections[section]
        return [self.ptr(p) for p in range(va, va+n, 8)]
    def name(self, va): return self.string(self.ptr((self.ptr(va+32) & ~7)+24))
    def methods(self, va, cl, cat, kind):
        if not va: return []
        flags, n = struct.unpack('<II', self.data(va, 8)); stride = flags & 65535; out = []
        for i in range(n):
            p = va + 8 + i * stride
            if flags & 0x80000000:
                a, b, c = struct.unpack('<iii', self.data(p, 12)); sel = p + a
                if not flags & 0x40000000: sel = self.ptr(sel)
                ty = p + 4 + b; imp = p + 8 + c
            else: sel, ty, imp = (self.ptr(p+j) for j in (0, 8, 16))
            out.append((cl, cat, kind, self.string(sel), self.string(ty), hex(imp)))
        return out

def disassembler():
    lib = ctypes.CDLL('/usr/lib/x86_64-linux-gnu/libLLVM-20.so')
    for name in ('LLVMInitializeAArch64TargetInfo', 'LLVMInitializeAArch64TargetMC', 'LLVMInitializeAArch64Disassembler'):
        getattr(lib, name)()
    lib.LLVMCreateDisasm.argtypes = [ctypes.c_char_p, ctypes.c_void_p, ctypes.c_int, ctypes.c_void_p, ctypes.c_void_p]
    lib.LLVMCreateDisasm.restype = ctypes.c_void_p
    ctx = lib.LLVMCreateDisasm(b'arm64-apple-macos', None, 0, None, None); assert ctx
    lib.LLVMDisasmInstruction.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_uint64, ctypes.c_uint64, ctypes.c_char_p, ctypes.c_size_t]
    lib.LLVMDisasmInstruction.restype = ctypes.c_size_t
    def decode(data, va):
        buf = ctypes.create_string_buffer(data); out = ctypes.create_string_buffer(256)
        if lib.LLVMDisasmInstruction(ctx, buf, len(data), va, out, len(out)) != 4:
            return 'UNDECODED (retained raw word; no instruction claim)'
        return ' '.join(out.value.decode().split())
    return decode

def capture():
    OUT.mkdir(exist_ok=True)
    im = Image(INPUT/'binaries/ArcCore-1.170.0.arm64')
    rows = list(csv.DictReader((ORIGINAL/'methods.tsv').open(), delimiter='\t'))
    vas = {int(r['imp_va'], 16) for r in rows if r['imp_va'] in ('0x15ac0','0xfbc0','0x43fe200','0x43fe218')}
    # All nearby literal accessor rows whose runtime annotations were erased.
    for r in rows:
        body = (HERE/'bodies'/f"{int(r['imp_va'],16):08x}.txt").read_text()
        if r['behavior'].startswith('Literal') and 'replaced with injection' in body:
            vas.add(int(r['imp_va'], 16))
    vas |= {0x43e1190,0x42fa5c8,0x43e6ee0,0x42b2430,0x440a1f8,0x473f138,
            0x4044e80,0x4044ec0,0x4337e00,0x42b3d74,0x7bdc470,0x473cad8,0x6846c88}
    manifest = []
    db = sqlite3.connect(f'file:{INPUT}/corpus-index.sqlite?mode=ro', uri=True)
    for va in sorted(vas):
        result = subprocess.run(['python3','/home/exedev/work/read-function.py',hex(va),'--max-bytes','32768'], check=True, capture_output=True).stdout
        dest = OUT/f'{va:08x}.read-function.txt'; dest.write_bytes(result)
        name, path, off, length, failed = db.execute('select name,path,offset,length,failed from functions where va=?',(va,)).fetchone()
        assert not failed and length <= 32768
        manifest.append(dict(image=im.path.name,address_basis='unslid Mach-O VA',va=hex(va),name=name,
                             source_shard=path,byte_offset=off,original_C_bytes=length,
                             helper_output=dest.name,helper_output_sha256=sha(dest),truncated=False))
    table(OUT/'body-provenance.tsv',manifest)
    decode = disassembler(); instructions = []
    # Sizes are bounded requested windows, not recovered function-boundary guarantees.
    windows = [(0x15ac0,4),(0xfbc0,4),(0x43fe200,24),(0x43fe218,12),(0x43e3394,48),
               (0x43e1190,224),(0x42fa5c8,1068),(0x43e6ee0,212),(0x42b2430,348),
               (0x440a1f8,180),(0x473f138,464),(0x4044e80,56),(0x4044ec0,52),
               (0x42b3d74,440),(0x473cad8,1044)]
    for start,n in windows:
        for va in range(start,start+n,4):
            b = im.data(va,4); word = int.from_bytes(b,'little'); target = ''
            if word >> 26 in (0b100101,0b000101):
                disp = word & 0x3ffffff
                if disp & 0x2000000: disp -= 1 << 26
                target = hex(va+disp*4)
            instructions.append(dict(image=im.path.name,address_basis='unslid Mach-O VA',window_start=hex(start),
                                     va=hex(va),file_offset=hex(im.offset(va,4)),bytes=b.hex(),
                                     instruction=decode(b,va),direct_branch_target=target))
    table(OUT/'instructions.tsv',instructions)
    data_rows = []
    for va, n, purpose in [(0xdf6b728,24,'termination callable copy/destructor/flags table'),
                           (0xe2a97b0,16,'owner property record: weak ArcWebContents; decoded strings checked separately')]:
        data_rows.append(dict(image=im.path.name,address_basis='unslid Mach-O VA',va=hex(va),
                              file_offset=hex(im.offset(va,n)),bytes=im.data(va,n).hex(),purpose=purpose))
    table(OUT/'data.tsv',data_rows)
    # Resolve only the selected ADRP/ADD/BR veneers through documented Mach-O
    # section indirect-symbol indices; never infer names from nearby functions.
    p = 32; stub = None; sym = None; indirect = None
    for _ in range(struct.unpack_from('<I',im.read(0,32),16)[0]):
        cmd,n = struct.unpack('<II',im.read(p,8)); b = im.read(p,n)
        if cmd == 2: sym = struct.unpack_from('<IIII',b,8)
        if cmd == 0xb: indirect = struct.unpack_from('<I',b,56)[0]
        if cmd == 0x19:
            for i in range(struct.unpack_from('<I',b,64)[0]):
                s = 72+i*80
                if b[s:s+16].split(b'\0')[0] == b'__stubs':
                    va,size = struct.unpack_from('<QQ',b,s+32)
                    first,stride = struct.unpack_from('<II',b,s+68)
                    stub = (va,size,first,stride)
        p += n
    assert sym and indirect and stub
    symbols = []
    for va in (0x84edd34,0x8b31a18,0x8869080,0xad3a854,0x8003ddc,0x800484c):
        a,b,c = struct.unpack('<III',im.data(va,12))
        assert a & 0x9f00001f == 0x90000010 and b & 0xffc003ff == 0x91000210 and c == 0xd61f0200
        disp = ((a>>5 & 0x7ffff)<<2) | (a>>29 & 3)
        if disp & (1<<20): disp -= 1<<21
        dest = (va & ~4095) + disp*4096 + ((b>>10)&4095)
        start,size,first,stride = stub
        assert start <= dest < start+size and (dest-start)%stride == 0
        symindex = struct.unpack('<I',im.read(indirect+4*(first+(dest-start)//stride),4))[0]
        assert symindex < sym[1]
        stringindex = struct.unpack_from('<I',im.read(sym[0]+symindex*16,16))[0]
        name = im.read(sym[2]+stringindex,min(512,sym[3]-stringindex)).split(b'\0')[0].decode()
        symbols.append(dict(image=im.path.name,address_basis='unslid Mach-O VA',veneer_va=hex(va),
                            file_offset=hex(im.offset(va,12)),veneer_bytes=im.data(va,12).hex(),
                            stub_va=hex(dest),indirect_symbol_index=symindex,symbol=name))
    table(OUT/'runtime-symbols.tsv',symbols)
    with Path('/home/exedev/work/heavy.lock').open('rb') as lock:
        fcntl.flock(lock,fcntl.LOCK_EX)
        image_hash = sha(im.path)
    (OUT/'image.json').write_text(json.dumps(dict(image=str(im.path),sha256=image_hash,
        address_basis='unslid Mach-O VA; every read translated by file-backed segment',
        segments=[dict(va=hex(v),file_bytes=n,file_offset=hex(o)) for v,n,o in im.segments],
        disassembler='/usr/lib/x86_64-linux-gnu/libLLVM-20.so; arm64-apple-macos'),indent=2)+'\n')
    print(f'Captured {len(manifest)} bounded original helper reads and {len(instructions)} instructions.')

if __name__ == '__main__': capture()
