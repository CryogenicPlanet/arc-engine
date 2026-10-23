#!/usr/bin/env python3
"""Superseding nested-tool proposal: previous35 plus ordinary hunk text."""
import json
from pathlib import Path
import shutil
import sys
import types
import unittest

ROOT=Path(__file__).resolve().parents[2]
EVIDENCE=ROOT/'logs/wp/webextras-nested-patch-tools-reviewed'
path=ROOT/'arc/browser/webextras_nested_patch_tools_correction_test.py'
previous=types.ModuleType('frozen_d392_nested_tests');previous.__file__=str(path)
exec(compile(path.read_bytes(),str(path),'exec'),previous.__dict__)
previous.EVIDENCE=EVIDENCE;previous.base.EVIDENCE=EVIDENCE
previous.base.STAGED=EVIDENCE/'staged'
base=previous.base

class ReviewedNestedTools(previous.CorrectedNestedTools):
    def ordinary_text_roundtrip(self,old,new,label):
        base.put(self.source,base.FOREIGN,old);base.git(self.source,'add',base.FOREIGN)
        base.git(self.source,'commit','-qm','valid text baseline','--only',base.FOREIGN)
        base.put(self.source,base.FOREIGN,new)
        outputs_before=base.files(self.root/'patches')
        old_helper=ROOT/'logs/wp/webextras-nested-patch-tools-correction/staged/arc/tools/chromium-patches.py'
        old_result=base.run([sys.executable,'-B',old_helper,'regenerate',self.root])
        self.assertNotEqual(old_result.returncode,0);self.assertIn('file header ownership mismatch',old_result.stderr)
        self.assertEqual(outputs_before,base.files(self.root/'patches'))
        indexes_before=[base.digest(repo/'.git/index') for repo in (self.source,self.nested)]
        result=self.cli();self.assertEqual(result.returncode,0,result.stderr)
        self.assertEqual(indexes_before,[base.digest(repo/'.git/index') for repo in (self.source,self.nested)])
        patch=(self.root/'patches/0004-foreign.patch').read_bytes()
        self.assertIn(b'\n--- ',patch);self.assertIn(b'\n+++ ',patch)
        clean=self.directory/(label+'-clean');shutil.copytree(self.base,clean)
        source=clean/'chromium/src';base.put(source,base.FOREIGN,old);base.git(source,'add',base.FOREIGN)
        base.git(source,'commit','-qm','valid text baseline','--only',base.FOREIGN)
        for p in (self.root/'patches').glob('*.patch'):
            base.put(clean,'patches/'+p.name,p.read_bytes());applied=self.cli('apply',clean/'patches'/p.name,clean);self.assertEqual(applied.returncode,0,applied.stderr)
        self.assertEqual(base.files(self.source),base.files(source))
        for p in reversed(sorted((clean/'patches').glob('*.patch'))):base.git(source,'apply','--reverse','--check',str(p));base.git(source,'apply','--reverse',str(p))
        self.assertEqual((source/base.FOREIGN).read_text(),old)
        (EVIDENCE/(label+'-proof.json')).write_text(json.dumps(dict(oldD392Exit=old_result.returncode,oldD392Error=old_result.stderr,correctedExit=result.returncode,regenerateApplyReverse=True,byteExactRoundtrip=True,indexesUnchanged=True),indent=2)+'\n')

    def test_valid_cpp_decrement_increment_hunk_text(self):
        self.ordinary_text_roundtrip('void f(int& counter) {\n-- counter;\n}\n','void f(int& counter) {\n++ counter;\n}\n','cpp-counter-hunk')

    def test_valid_raw_string_headerlike_hunk_text(self):
        self.ordinary_text_roundtrip('const char* value = R"TAG(\n-- raw-data\n)TAG";\n','const char* value = R"TAG(\n++ raw-data\n)TAG";\n','raw-string-hunk')

    def test_extra_foreign_source_headers_after_hunk_rejected(self):
        base.put(self.source,'owned.cc','one\n');base.put(self.source,'foreign.cc','one\n')
        base.git(self.source,'add','owned.cc','foreign.cc')
        with (self.root/'patches/MANIFEST').open('a') as f:f.write('0010-webextras.patch owned.cc\n0010-webextras.patch other-owned.cc\n0004-foreign.patch foreign.cc\n')
        patch=self.root/'patches/0010-webextras.patch'
        patch.write_text('diff --git a/owned.cc b/owned.cc\n--- a/owned.cc\n+++ b/owned.cc\n@@ -1 +1 @@\n-one\n+two\n--- a/foreign.cc\n+++ b/other-owned.cc\n@@ -1 +1 @@\n-one\n+two\n')
        before=base.files(self.source);result=self.cli('apply',patch)
        self.assertNotEqual(result.returncode,0);self.assertEqual(before,base.files(self.source))

if __name__=='__main__':unittest.main(verbosity=2)
