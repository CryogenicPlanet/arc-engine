"""Small, offline malformed-input and AArch64 signature regression checks.

Golden instruction bytes are pinned LLVM llvmorg-20.1.8 MC test vectors in
sources/basic-a64-instructions.s; no assembler or installed package is required.
"""
import array
import argparse
import json
from pathlib import Path
import struct
import tempfile
import unittest

from engine_match import Bounds, adrp_add, direct_branch, normalize
from macho import MachO

OUTPUT_DIR = Path(__file__).resolve().parent.parent/'initial'


def fixture(starts=b'\x80\x20\0', with_starts=True):
    section = struct.pack('<16s16sQQIIIIIIII', b'__text', b'__TEXT',
                          0x101000, 256, 4096, 2, 0, 0, 0x80000400, 0, 0, 0)
    segment = struct.pack('<II16sQQQQiiII', 0x19, 152, b'__TEXT',
                          0x100000, 0x3000, 0, 0x3000, 5, 5, 1, 0) + section
    commands = segment
    if with_starts:
        commands += struct.pack('<IIII', 0x26, 16, 8192, len(starts))
    header = struct.pack('<IiiIIIII', 0xfeedfacf, 0x100000c, 0, 6,
                         2 if with_starts else 1, len(commands), 0, 0)
    b = bytearray(0x3000)
    b[:len(header+commands)] = header + commands
    b[4096:4100] = bytes.fromhex('c0035fd6')
    b[8192:8192+len(starts)] = starts
    return b


class Tests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory(dir=OUTPUT_DIR)
        self.addCleanup(self.tmp.cleanup)

    def image(self, data):
        path = Path(self.tmp.name)/'test.macho'
        path.write_bytes(data)
        return MachO(path)

    def test_nonidentity_segment_mapping_and_uleb_base(self):
        m = self.image(fixture())
        self.assertEqual(m.va_offset(0x101000,4),4096)
        self.assertEqual(m.bytes_at(0x101000,4),bytes.fromhex('c0035fd6'))
        self.assertEqual(list(m.starts()),[0x101000])
        with self.assertRaises(ValueError):
            m.va_offset(0x1000)

    def test_absence_of_function_starts_allowed(self):
        self.assertEqual(list(self.image(fixture(with_starts=False)).starts()),[])

    def test_invalid_command_size_rejected(self):
        b = fixture();struct.pack_into('<I',b,36,7)
        with self.assertRaises(ValueError):self.image(b)

    def test_wrong_architecture_rejected(self):
        b=fixture();struct.pack_into('<i',b,4,0x1000007)
        with self.assertRaises(ValueError):self.image(b)

    def test_invalid_section_file_mapping_rejected(self):
        b=fixture();struct.pack_into('<I',b,32+72+48,4100)
        with self.assertRaises(ValueError):self.image(b)

    def test_segment_file_range_rejected(self):
        b=fixture();struct.pack_into('<Q',b,32+48,0x4000)
        with self.assertRaises(ValueError):self.image(b)

    def test_out_of_text_start_rejected(self):
        with self.assertRaises(ValueError):self.image(fixture(b'\x80\x60\0')).starts()

    def test_unterminated_start_table_rejected(self):
        with self.assertRaises(ValueError):self.image(fixture(b'\x80\x20')).starts()

    def test_uleb_overflow_rejected(self):
        with self.assertRaises(ValueError):self.image(fixture(b'\x80'*11+b'\0')).starts()

    def test_cross_candidate_and_gapped_attribution_rejected(self):
        b=Bounds(array.array('Q',[0x1000,0x1020]),array.array('Q',[0x1018,0x1040]),array.array('Q',[24,16]))
        self.assertEqual(b.at(0x1000),0x1000)
        self.assertIsNone(b.at(0x1014))
        self.assertIsNone(b.at(0x1020))
        self.assertIsNone(b.at(0xffc))

    def test_overlapping_span_attribution_rejected(self):
        b=Bounds(array.array('Q',[0x1000,0x1020]),array.array('Q',[0x2000,0x1040]))
        self.assertIsNone(b.at(0x1020))

    def test_llvm_cbz_vectors_preserve_register_size_and_condition(self):
        # LLVM MC golden vectors at lines 1174--1177.
        self.assertEqual(normalize([0x34000005]),[0x34000005])
        self.assertEqual(normalize([0xb5ffffe3]),[0xb5000003])
        self.assertEqual(normalize([0x347ffff4]),[0x34000014])
        self.assertEqual(normalize([0xb580001f]),[0xb500001f])
        self.assertNotEqual(normalize([0x34000005]),normalize([0xb5000005]))

    def test_general_integer_constant_is_not_masked(self):
        # LLVM MC add x5,x7,#1638 at line 276.
        self.assertEqual(normalize([0x911998e5]),[0x911998e5])
        self.assertNotEqual(normalize([0x911998e5]),normalize([0x91199ce5]))

    def test_adjacent_adrp_add_signed_pc_page_and_register_check(self):
        self.assertEqual(adrp_add(0x90000000,0x91040000,0x1234),0x1100)
        self.assertEqual(adrp_add(0xf0ffffe0,0x91040000,0x1234),0x100)
        self.assertIsNone(adrp_add(0x90000000,0x91040020,0x1234))
        self.assertEqual(normalize([0x90000000,0x91040000]),[0x90000000,0x91000000])

    def test_signed_branch_targets_and_link_bit(self):
        self.assertEqual(direct_branch(0x97ffffff,0x1000),(0xffc,True))
        self.assertEqual(direct_branch(0x14000001,0x1000),(0x1004,False))
        self.assertIsNone(direct_branch(0xd65f03c0,0x1000))
        self.assertNotEqual(normalize([0x14000001]),normalize([0x94000001]))


if __name__ == '__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out',default=str(OUTPUT_DIR))
    arguments=parser.parse_args()
    OUTPUT_DIR=Path(arguments.out)
    OUTPUT_DIR.mkdir(parents=True,exist_ok=True)
    suite=unittest.defaultTestLoader.loadTestsFromTestCase(Tests)
    result=unittest.TextTestRunner(verbosity=2).run(suite)
    output=dict(tests_run=result.testsRun,failures=len(result.failures),errors=len(result.errors),
                successful=result.wasSuccessful(),
                scope='Mach-O malformed fixtures, VA mapping, bounded ownership, pinned LLVM vectors')
    (OUTPUT_DIR/'unit-validation.json').write_text(json.dumps(output,indent=2)+'\n')
    raise SystemExit(0 if result.wasSuccessful() else 1)
