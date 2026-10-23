#!/usr/bin/env python3
"""Inherited real-Git round trips plus corrections to the frozen970 proposal."""
import json
import os
from pathlib import Path
import shutil
import sys
import types
import unittest
from unittest import mock

ROOT=Path(__file__).resolve().parents[2]
EVIDENCE=ROOT/'logs/wp/webextras-nested-patch-tools-correction'
original=ROOT/'arc/browser/webextras_nested_patch_tools_test.py'
base=types.ModuleType('frozen970_nested_tests');base.__file__=str(original)
exec(compile(original.read_bytes(),str(original),'exec'),base.__dict__)
base.STAGED=EVIDENCE/'staged';base.EVIDENCE=EVIDENCE

class CorrectedNestedTools(base.NestedTools):
    def test_outer_alternate_index_scoped_per_repo(self):
        alternate=self.root/'outer-alternate.index'
        shutil.copyfile(self.source/'.git/index',alternate)
        # The manifest hook addition is registered only in the transaction's
        # outer alternate index. The physical outer index must remain unchanged.
        base.git(self.source,'update-index','--force-remove',base.HOOK)
        before=[base.digest(repo/'.git/index') for repo in (self.source,self.nested)]
        alt_before=base.digest(alternate);patches_before=base.files(self.root/'patches')
        old=ROOT/'logs/wp/webextras-nested-patch-tools/staged/arc/tools/chromium-patches.py'
        with mock.patch.dict(os.environ,{'GIT_INDEX_FILE':str(alternate)}):
            unsupported=base.run([sys.executable,'-B',old,'regenerate',self.root])
            self.assertNotEqual(unsupported.returncode,0)
            self.assertEqual(patches_before,base.files(self.root/'patches'))
            result=self.cli();self.assertEqual(result.returncode,0,result.stderr)
        self.assertEqual(before,[base.digest(repo/'.git/index') for repo in (self.source,self.nested)])
        self.assertEqual(alt_before,base.digest(alternate))
        clean=self.directory/'alternate-roundtrip-clean';shutil.copytree(self.base,clean)
        for patch in (self.root/'patches').glob('*.patch'):
            base.put(clean,'patches/'+patch.name,patch.read_bytes())
            self.assertEqual(self.cli('apply',clean/'patches'/patch.name,clean).returncode,0)
        self.assertEqual(base.files(self.source),base.files(clean/'chromium/src'))
        (EVIDENCE/'outer-alternate-index-proof.json').write_text(json.dumps(dict(old970Exit=unsupported.returncode,old970Stderr=unsupported.stderr,correctedExit=result.returncode,actualIndexesUnchanged=True,alternateIndexUnchanged=True,manifestAdditionOnlyInAlternate=True,allFiveFrontendSourcesRoundtrip=True),indent=2)+'\n')

    def test_assume_unchanged_rejected(self):
        base.git(self.source,'update-index','--assume-unchanged',base.FOREIGN)
        self.reject('hidden/unsupported index flags')

    def test_skip_worktree_rejected(self):
        base.git(self.source,'update-index','--skip-worktree',base.FOREIGN)
        self.reject('hidden/unsupported index flags')

    def test_binary_source_rejected_before_output(self):
        base.put(self.source,base.FOREIGN,b'binary\x00foreign\n')
        self.reject('binary source is unsupported')

    def test_forced_color_disabled_and_patch_roundtrip(self):
        for repo in (self.source,self.nested):base.git(repo,'config','color.diff','always')
        self.assertEqual(self.cli().returncode,0)
        clean=self.directory/'color-roundtrip-clean';shutil.copytree(self.base,clean)
        for patch in (self.root/'patches').glob('*.patch'):
            self.assertNotIn(b'\x1b',patch.read_bytes());base.put(clean,'patches/'+patch.name,patch.read_bytes())
            result=self.cli('apply',clean/'patches'/patch.name,clean);self.assertEqual(result.returncode,0,result.stderr)
        self.assertEqual(base.files(self.source),base.files(clean/'chromium/src'))

    def test_different_diff_source_destination_rejected(self):
        patch=self.root/'patches/0010-webextras.patch'
        # Parent's actual Git behavior: plain mismatched headers can read a
        # foreign source even though --numstat names only the owned destination.
        base.put(self.source,'foreign.cc','one\n');base.git(self.source,'add','foreign.cc')
        with (self.root/'patches/MANIFEST').open('a') as f:f.write('0010-webextras.patch owned.cc\n0004-foreign.patch foreign.cc\n')
        patch.write_text('diff --git a/foreign.cc b/owned.cc\n--- a/foreign.cc\n+++ b/owned.cc\n@@ -1 +1 @@\n-one\n+two\n')
        source_before=base.files(self.source)
        self.assertEqual(base.run(['git','-C',self.source,'apply','--check',patch]).returncode,0)
        old=ROOT/'logs/wp/webextras-nested-patch-tools/staged/arc/tools/chromium-patches.py'
        private=self.directory/'header970-control';shutil.copytree(self.root,private)
        control=base.run([sys.executable,'-B',old,'apply',private,private/'patches/0010-webextras.patch'])
        self.assertEqual(control.returncode,0,control.stderr)
        result=self.cli('apply',patch);self.assertNotEqual(result.returncode,0);self.assertIn('diff source/destination ownership mismatch',result.stderr)
        self.assertEqual(source_before,base.files(self.source))
        (EVIDENCE/'header-ownership-proof.json').write_text(json.dumps(dict(gitCheckAcceptsMismatchedHeaders=True,old970ApplyExit=control.returncode,correctedApplyExit=result.returncode,correctedError=result.stderr,originalFilesUnchanged=True),indent=2)+'\n')

    def test_different_unified_source_rejected(self):
        patch=self.root/'patches/0010-webextras.patch'
        patch.write_text('diff --git a/'+base.NATIVE+' b/'+base.NATIVE+'\n--- a/'+base.FOREIGN+'\n+++ b/'+base.NATIVE+'\n@@ -1 +1 @@\n-one\n+two\n')
        result=self.cli('apply',patch);self.assertNotEqual(result.returncode,0);self.assertIn('file header ownership mismatch',result.stderr)

    def test_multiple_unified_file_headers_rejected(self):
        patch=self.root/'patches/0010-webextras.patch'
        p=base.NATIVE;patch.write_text('diff --git a/'+p+' b/'+p+'\n--- a/'+p+'\n+++ b/'+p+'\n--- a/'+base.FOREIGN+'\n+++ b/'+p+'\n@@ -1 +1 @@\n-one\n+two\n')
        self.assertIn('file header ownership mismatch',self.cli('apply',patch).stderr)

    def test_inherited_git_dir_rejected(self):
        with mock.patch.dict(os.environ,{'GIT_DIR':str(self.source/'.git')}):self.reject('inherited GIT_DIR/GIT_WORK_TREE is unsupported')

    def test_inherited_git_work_tree_rejected(self):
        with mock.patch.dict(os.environ,{'GIT_WORK_TREE':str(self.source)}):self.reject('inherited GIT_DIR/GIT_WORK_TREE is unsupported')

    def test_relative_outer_index_rejected(self):
        with mock.patch.dict(os.environ,{'GIT_INDEX_FILE':'relative.index'}):self.reject('outer GIT_INDEX_FILE must be absolute')

if __name__=='__main__':unittest.main(verbosity=2)
