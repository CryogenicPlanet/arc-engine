#!/usr/bin/env python3
"""Private real-Git tests of the bounded REGEN-ONLY tooling proposal."""
import json
import os
from pathlib import Path
import shutil
import sys
import tempfile
import types
import unittest
from unittest import mock

ROOT=Path(__file__).resolve().parents[2]
EVIDENCE=ROOT/'logs/wp/webextras-nested-regeneration'
STAGED=EVIDENCE/'staged'
old=ROOT/'arc/browser/webextras_nested_patch_tools_test.py'
fixture=types.ModuleType('frozen_fixture_utilities');fixture.__file__=str(old)
exec(compile(old.read_bytes(),str(old),'exec'),fixture.__dict__)
NESTED=fixture.NESTED;FRONTEND=fixture.FRONTEND;NATIVE=fixture.NATIVE;HOOK=fixture.HOOK;FOREIGN=fixture.FOREIGN
RECEIPT=fixture.RECEIPT
FOREIGN_BASE='void f(int& counter) {\n-- counter;\n}\nconst char* raw = R"TAG(\n-- raw-data\n)TAG";\nint foreign = 1;\n'
FOREIGN_EDIT=FOREIGN_BASE.replace('--','++').replace('foreign = 1','foreign = 2')

class NestedRegeneration(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=tempfile.TemporaryDirectory(prefix='webextras-regen-only-');cls.directory=Path(cls.temp.name);cls.base=cls.directory/'base'
        source=cls.base/'chromium/src';nested=source/NESTED
        for repo in (source,nested):
            repo.mkdir(parents=True,exist_ok=True);fixture.git(repo,'init','-q');fixture.git(repo,'config','user.name','Private fixture');fixture.git(repo,'config','user.email','fixture@example.invalid');fixture.git(repo,'config','commit.gpgsign','false')
            fixture.git(repo,'config','diff.ignoreSubmodules','dirty');fixture.git(repo,'config','diff.external','/definitely-not-a-diff-tool')
        for rel in FRONTEND:fixture.put(nested,rel,(RECEIPT/'base'/NESTED/rel).read_bytes())
        fixture.git(nested,'add','front_end');fixture.git(nested,'commit','-qm','pinned frontend fixture')
        fixture.put(source,NATIVE,(RECEIPT/'base'/NATIVE).read_bytes());fixture.put(source,FOREIGN,FOREIGN_BASE)
        fixture.git(source,'add','chrome');fixture.git(source,'update-index','--add','--cacheinfo','160000,'+fixture.git(nested,'rev-parse','HEAD')+','+NESTED);fixture.git(source,'commit','-qm','outer fixture')
        rows=['0010-webextras.patch '+NESTED+'/'+p for p in FRONTEND]
        rows+=['0010-webextras.patch '+NATIVE,'0004-foreign.patch '+HOOK,'0004-foreign.patch '+FOREIGN]
        fixture.put(cls.base,'patches/MANIFEST','\n'.join(rows)+'\n')
        fixture.put(cls.base,'patches/0004-foreign.patch','preserve foreign output\n');fixture.put(cls.base,'patches/0010-webextras.patch','preserve own output\n')
        for rel in ('arc/tools/regen-patches.sh','arc/tools/regen-chromium-patches.py'):fixture.put(cls.base,rel,(STAGED/rel).read_bytes())
        fixture.put(cls.base,'sync-chromium.sh',(ROOT/'sync-chromium.sh').read_bytes())

    @classmethod
    def tearDownClass(cls):
        (EVIDENCE/'test-commands.json').write_text(json.dumps(fixture.TRANSCRIPTS,indent=2)+'\n');cls.temp.cleanup()

    def setUp(self):
        self.root=self.directory/self.id().rsplit('.',1)[-1];shutil.copytree(self.base,self.root);self.source=self.root/'chromium/src';self.nested=self.source/NESTED
        for rel in FRONTEND:fixture.put(self.nested,rel,(RECEIPT/'intended-output'/NESTED/rel).read_bytes())
        fixture.put(self.source,NATIVE,(RECEIPT/'intended-output'/NATIVE).read_bytes());fixture.put(self.source,HOOK,(RECEIPT/'intended-output'/HOOK).read_bytes());fixture.git(self.source,'add','-N',HOOK)
        fixture.put(self.source,FOREIGN,FOREIGN_EDIT)

    def indexes(self):return [fixture.digest(p/'.git/index') for p in (self.source,self.nested)]

    def regen(self):return fixture.run([sys.executable,'-B',self.root/'arc/tools/regen-chromium-patches.py',self.root])

    def reject(self,message):
        before=fixture.files(self.root/'patches');indexes=self.indexes();result=self.regen()
        self.assertNotEqual(result.returncode,0);self.assertIn(message,result.stderr);self.assertEqual(before,fixture.files(self.root/'patches'));self.assertEqual(indexes,self.indexes())

    def original_sync_loop(self,root):
        script=(root/'sync-chromium.sh').read_text();start=script.index('for p in "$E"/patches/*.patch; do');end=script.index('\ndone',start)+len('\ndone')
        loop='set -euo pipefail\nE="$1"\nstep(){ printf "%s\\n" "$*"; }\n'+script[start:end]
        return fixture.run(['bash','-c',loop,'private-original-sync-loop',root])

    def clean_replay(self,label):
        clean=self.directory/(label+'-clean');shutil.copytree(self.base,clean)
        for p in (self.root/'patches').glob('*.patch'):fixture.put(clean,'patches/'+p.name,p.read_bytes())
        before=[fixture.digest(p/'.git/index') for p in (clean/'chromium/src',clean/'chromium/src'/NESTED)]
        result=self.original_sync_loop(clean);self.assertEqual(result.returncode,0,result.stderr)
        self.assertEqual(fixture.files(self.source),fixture.files(clean/'chromium/src'))
        self.assertEqual(before,[fixture.digest(p/'.git/index') for p in (clean/'chromium/src',clean/'chromium/src'/NESTED)])
        again=self.original_sync_loop(clean);self.assertEqual(again.returncode,0);self.assertEqual(again.stdout.count('already applied'),2)
        return clean

    def test_full_actual_sources_foreign_counter_rawtext_sync_roundtrip(self):
        before=self.indexes();expected=fixture.files(self.source)
        result=fixture.run(['bash',self.root/'arc/tools/regen-patches.sh']);self.assertEqual(result.returncode,0,result.stderr);self.assertEqual(before,self.indexes())
        patches=fixture.files(self.root/'patches');self.assertEqual(self.regen().returncode,0);self.assertEqual(patches,fixture.files(self.root/'patches'))
        clean=self.clean_replay('full');source=clean/'chromium/src'
        for p in reversed(sorted((clean/'patches').glob('*.patch'))):fixture.git(source,'apply','--reverse','--check',str(p));fixture.git(source,'apply','--reverse',str(p))
        self.assertEqual(fixture.files(self.base/'chromium/src'),fixture.files(source))
        (EVIDENCE/'roundtrip-proof.json').write_text(json.dumps(dict(frontendFiles=5,nativeFiles=1,foreignChange=True,indexedITA=True,cppCounterAndRawStringHunks=True,originalSyncUnchanged=True,originalSyncForwardReverseByteExact=True,indexesUnchanged=True,sourceHashes=expected),indent=2)+'\n')

    def test_outer_alternate_index_and_nested_actual_index(self):
        alternate=self.root/'outer-alternate.index';shutil.copyfile(self.source/'.git/index',alternate);fixture.git(self.source,'update-index','--force-remove',HOOK)
        before=self.indexes();alt=fixture.digest(alternate)
        with mock.patch.dict(os.environ,{'GIT_INDEX_FILE':str(alternate)}):result=self.regen();self.assertEqual(result.returncode,0,result.stderr)
        self.assertEqual(before,self.indexes());self.assertEqual(alt,fixture.digest(alternate));self.clean_replay('alternate')
        (EVIDENCE/'alternate-proof.json').write_text(json.dumps(dict(outerAdditionOnlyInAlternate=True,actualIndexesUnchanged=True,outerAlternateUnchanged=True,nestedUsesActualIndex=True,originalSyncByteExact=True),indent=2)+'\n')

    def test_nested_only_and_outer_only_patch(self):
        p=self.root/'patches/MANIFEST';p.write_text(p.read_text().replace('0010-webextras.patch '+NATIVE,'0004-foreign.patch '+NATIVE))
        self.assertEqual(self.regen().returncode,0)
        clean=self.directory/'nested-only-clean';shutil.copytree(self.base,clean);fixture.put(clean,'patches/MANIFEST',p.read_bytes())
        for patch in (self.root/'patches').glob('*.patch'):fixture.put(clean,'patches/'+patch.name,patch.read_bytes())
        self.assertEqual(self.original_sync_loop(clean).returncode,0);self.assertEqual(fixture.files(self.source),fixture.files(clean/'chromium/src'))

    def test_staged_and_unstaged_final_worktree(self):
        fixture.put(self.source,FOREIGN,'int foreign = 3;\n');fixture.git(self.source,'add',FOREIGN);fixture.put(self.source,FOREIGN,'int foreign = 4;\n')
        before=self.indexes();self.assertEqual(self.regen().returncode,0);self.assertEqual(before,self.indexes());self.clean_replay('staged')

    def test_forced_color_uses_plain_generated_diff(self):
        for repo in (self.source,self.nested):fixture.git(repo,'config','color.diff','always')
        self.assertEqual(self.regen().returncode,0)
        for p in (self.root/'patches').glob('*.patch'):self.assertNotIn(b'\x1b',p.read_bytes())
        self.clean_replay('color')

    def test_empty_indexed_outer_file(self):
        rel='chrome/browser/empty.cc';fixture.put(self.source,rel,b'');fixture.git(self.source,'add','-N',rel)
        with (self.root/'patches/MANIFEST').open('a') as f:f.write('0010-webextras.patch '+rel+'\n')
        self.assertEqual(self.regen().returncode,0);self.clean_replay('empty')

    def test_mode_only_generated_diff(self):
        fixture.put(self.source,NATIVE,(RECEIPT/'base'/NATIVE).read_bytes());(self.source/NATIVE).chmod(0o755)
        self.assertEqual(self.regen().returncode,0);self.assertIn(b'new mode 100755',(self.root/'patches/0010-webextras.patch').read_bytes());clean=self.clean_replay('mode')
        self.assertEqual((clean/'chromium/src'/NATIVE).stat().st_mode&0o777,0o755)

    def test_original_sync_partial_patch_fails_without_partial_mutation(self):
        self.assertEqual(self.regen().returncode,0);clean=self.directory/'partial-clean';shutil.copytree(self.base,clean)
        for p in (self.root/'patches').glob('*.patch'):fixture.put(clean,'patches/'+p.name,p.read_bytes())
        fixture.git(clean/'chromium/src','apply',str(clean/'patches/0004-foreign.patch'))
        fixture.put(clean/'chromium/src',NATIVE,(self.source/NATIVE).read_bytes());before=fixture.files(clean/'chromium/src')
        result=self.original_sync_loop(clean);self.assertNotEqual(result.returncode,0);self.assertEqual(before,fixture.files(clean/'chromium/src'))

    def test_binary_source(self):fixture.put(self.source,FOREIGN,b'a\0b');self.reject('binary source')
    def test_binary_head_generates_unsupported_diff(self):
        fixture.put(self.source,FOREIGN,b'binary\0baseline');fixture.git(self.source,'add',FOREIGN);fixture.git(self.source,'commit','-qm','binary baseline','--only',FOREIGN)
        fixture.put(self.source,FOREIGN,FOREIGN_EDIT);self.reject('generated binary/unsupported diff')
    def test_hidden_assume_unchanged(self):fixture.git(self.source,'update-index','--assume-unchanged',FOREIGN);self.reject('hidden/unsupported index flags')
    def test_hidden_skip_worktree(self):fixture.git(self.source,'update-index','--skip-worktree',FOREIGN);self.reject('hidden/unsupported index flags')
    def test_unknown_nested_untracked(self):fixture.put(self.nested,'unknown.ts','bad');self.reject('unsupported/untracked change')
    def test_unknown_outer_untracked(self):fixture.put(self.source,'unknown.cc','bad');self.reject('unsupported/untracked change')
    def test_manifest_listed_untracked(self):
        fixture.put(self.nested,'unknown.ts','bad')
        with (self.root/'patches/MANIFEST').open('a') as f:f.write('0010-webextras.patch '+NESTED+'/unknown.ts\n')
        self.reject('untracked/unsupported manifest input')
    def test_duplicate_ownership(self):
        with (self.root/'patches/MANIFEST').open('a') as f:f.write('0004-foreign.patch '+NATIVE+'\n')
        self.reject('duplicate manifest ownership')
    def test_path_escape(self):
        with (self.root/'patches/MANIFEST').open('a') as f:f.write('0010-webextras.patch ../escape.cc\n')
        self.reject('unsafe manifest path')
    def test_missing_foreign_source(self):(self.source/FOREIGN).unlink();self.reject('missing input')
    def test_symlink_source(self):
        p=self.nested/FRONTEND[0];p.unlink();p.symlink_to('/etc/hosts');self.reject('symlink input')
    def test_moved_nested_head(self):fixture.git(self.nested,'add','front_end');fixture.git(self.nested,'commit','-qm','unexpected');self.reject('frontend checkout HEAD differs')
    def test_moved_index_gitlink(self):
        fixture.git(self.nested,'add','front_end');fixture.git(self.nested,'commit','-qm','unexpected');fixture.git(self.source,'update-index','--cacheinfo','160000,'+fixture.git(self.nested,'rev-parse','HEAD')+','+NESTED);self.reject('frontend gitlink index/HEAD differs')
    def test_relative_alternate_index(self):
        with mock.patch.dict(os.environ,{'GIT_INDEX_FILE':'relative.index'}):self.reject('outer GIT_INDEX_FILE must be absolute')
    def test_inherited_git_dir(self):
        with mock.patch.dict(os.environ,{'GIT_DIR':str(self.source/'.git')}):self.reject('inherited GIT_DIR/GIT_WORK_TREE is unsupported')
    def test_inherited_git_work_tree(self):
        with mock.patch.dict(os.environ,{'GIT_WORK_TREE':str(self.source)}):self.reject('inherited GIT_DIR/GIT_WORK_TREE is unsupported')

    def test_source_drift_before_publish(self):
        wrapper=self.root/'fake-bin';wrapper.mkdir();actual=shutil.which('git');marker=self.root/'once'
        program='#!/usr/bin/env python3\nimport os,sys\nfrom pathlib import Path\nmarker=Path('+repr(str(marker))+')\n'
        program+='if "diff" in sys.argv and "HEAD" in sys.argv and not marker.exists():\n marker.write_text("done");Path('+repr(str(self.source/FOREIGN))+').write_text("int foreign = 99;\\n")\n'
        program+='os.execv('+repr(actual)+',['+repr(actual)+']+sys.argv[1:])\n';p=wrapper/'git';p.write_text(program);p.chmod(0o755)
        with mock.patch.dict(os.environ,{'PATH':str(wrapper)+os.pathsep+os.environ['PATH']}):self.reject('inputs changed during regeneration')

    def test_destination_patch_drift_not_overwritten(self):
        wrapper=self.root/'fake-output-bin';wrapper.mkdir();actual=shutil.which('git');marker=self.root/'output-once';patch=self.root/'patches/0004-foreign.patch'
        program='#!/usr/bin/env python3\nimport os,sys\nfrom pathlib import Path\nmarker=Path('+repr(str(marker))+')\n'
        program+='if "diff" in sys.argv and "HEAD" in sys.argv and not marker.exists():\n marker.write_text("done");Path('+repr(str(patch))+').write_text("external foreign patch update\\n")\n'
        program+='os.execv('+repr(actual)+',['+repr(actual)+']+sys.argv[1:])\n';p=wrapper/'git';p.write_text(program);p.chmod(0o755)
        own_before=(self.root/'patches/0010-webextras.patch').read_bytes();indexes=self.indexes()
        with mock.patch.dict(os.environ,{'PATH':str(wrapper)+os.pathsep+os.environ['PATH']}):result=self.regen()
        self.assertNotEqual(result.returncode,0);self.assertIn('inputs changed during regeneration',result.stderr)
        self.assertEqual(patch.read_text(),'external foreign patch update\n');self.assertEqual(own_before,(self.root/'patches/0010-webextras.patch').read_bytes());self.assertEqual(indexes,self.indexes())

if __name__=='__main__':unittest.main(verbosity=2)
