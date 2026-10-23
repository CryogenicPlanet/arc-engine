#!/usr/bin/env python3
"""Private real-Git tests of the bounded REGEN-ONLY tooling proposal."""
import json
import os
from pathlib import Path
import shutil
import hashlib
import subprocess
import sys
import tempfile
import types
import unittest
from unittest import mock

ROOT=Path(__file__).resolve().parents[2]
EVIDENCE=ROOT/'logs/wp/webextras-nested-regeneration-lead-rebase'
STAGED=EVIDENCE/'staged'
PROPOSAL=ROOT/'arc/browser/webextras_nested_regeneration_lead_rebase.patch'
OUTPUT=None
old=ROOT/'arc/browser/webextras_nested_patch_tools_test.py'
fixture=types.ModuleType('frozen_fixture_utilities');fixture.__file__=str(old)
exec(compile(old.read_bytes(),str(old),'exec'),fixture.__dict__)
# No fixture Git command may inherit the invoking agent's repository context.
# Poison controls opt in only through this explicit, disposable context map.
fixture.GIT_CONTEXT={}
def controlled_run(argv,cwd=None):
    env={k:v for k,v in os.environ.items() if not k.startswith('GIT_')}
    env.update(fixture.GIT_CONTEXT);env.update(GIT_OPTIONAL_LOCKS='0',LC_ALL='C')
    result=subprocess.run([str(a) for a in argv],cwd=cwd,env=env,capture_output=True,text=True)
    fixture.TRANSCRIPTS.append(dict(argv=[str(a) for a in argv],cwd=str(cwd) if cwd else None,exitCode=result.returncode,stdout=result.stdout,stderr=result.stderr))
    return result
fixture.run=controlled_run
NESTED=fixture.NESTED;FRONTEND=fixture.FRONTEND;NATIVE=fixture.NATIVE;HOOK=fixture.HOOK;FOREIGN=fixture.FOREIGN
RECEIPT=fixture.RECEIPT
FOREIGN_BASE='void f(int& counter) {\n-- counter;\n}\nconst char* raw = R"TAG(\n-- raw-data\n)TAG";\nint foreign = 1;\n'
FOREIGN_EDIT=FOREIGN_BASE.replace('--','++').replace('foreign = 1','foreign = 2')

class LeadRebasedNestedRegeneration(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=tempfile.TemporaryDirectory(prefix='webextras-lead-rebased-regen-');cls.directory=Path(cls.temp.name);cls.base=cls.directory/'base'
        global OUTPUT
        OUTPUT=Path(os.environ['WEBEXTRAS_REGEN_TEST_EVIDENCE']) if 'WEBEXTRAS_REGEN_TEST_EVIDENCE' in os.environ else cls.directory/'receipts'
        OUTPUT.mkdir(parents=True,exist_ok=True)
        cls.pins=json.loads((EVIDENCE/'input-pins.json').read_text())
        cls.reconstructed=cls.directory/'reconstructed-tool'
        fixture.put(cls.reconstructed,'arc/tools/regen-patches.sh',(EVIDENCE/'baseline/arc/tools/regen-patches.sh').read_bytes())
        for args in (['git','apply','--check',PROPOSAL],['git','apply',PROPOSAL]):
            result=fixture.run(args,cwd=cls.reconstructed)
            if result.returncode:raise AssertionError(result.stderr)
        for rel in ('arc/tools/regen-patches.sh','arc/tools/regen-chromium-patches.py'):
            if (cls.reconstructed/rel).read_bytes()!=(STAGED/rel).read_bytes():raise AssertionError('strict reconstruction differs: '+rel)
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
        for rel in ('arc/tools/regen-patches.sh','arc/tools/regen-chromium-patches.py'):fixture.put(cls.base,rel,(cls.reconstructed/rel).read_bytes())
        fixture.put(cls.base,'sync-chromium.sh',(ROOT/'sync-chromium.sh').read_bytes())

    @classmethod
    def tearDownClass(cls):
        try:
            for relative,digest in cls.pins['files'].items():
                if fixture.digest(ROOT/relative)!=digest:raise AssertionError('source pin changed: '+relative)
            for relative in cls.pins['absent']:
                if (ROOT/relative).exists():raise AssertionError('actual helper appeared: '+relative)
            for command in fixture.TRANSCRIPTS:
                for argument in command['argv']:
                    if argument in [str(ROOT/p) for p in ('arc/tools/regen-patches.sh','arc/tools/regen-chromium-patches.py','arc/tools/diff-run.sh','arc/tools/locked.sh')]:
                        raise AssertionError('production tool invoked')
            (OUTPUT/'test-commands.json').write_text(json.dumps(fixture.TRANSCRIPTS,indent=2)+'\n')
        finally:cls.temp.cleanup()

    def setUp(self):
        self.root=self.directory/self.id().rsplit('.',1)[-1];shutil.copytree(self.base,self.root);self.source=self.root/'chromium/src';self.nested=self.source/NESTED
        for rel in FRONTEND:fixture.put(self.nested,rel,(RECEIPT/'intended-output'/NESTED/rel).read_bytes())
        fixture.put(self.source,NATIVE,(RECEIPT/'intended-output'/NATIVE).read_bytes());fixture.put(self.source,HOOK,(RECEIPT/'intended-output'/HOOK).read_bytes())
        fixture.put(self.source,FOREIGN,FOREIGN_EDIT)

    def indexes(self):return [fixture.digest(p/'.git/index') for p in (self.source,self.nested)]

    def regen(self):return fixture.run([sys.executable,'-B',self.root/'arc/tools/regen-chromium-patches.py'])

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
        (OUTPUT/'roundtrip-proof.json').write_text(json.dumps(dict(frontendFiles=5,nativeFiles=1,foreignChange=True,realUnindexedLeafPrivateITA=True,cppCounterAndRawStringHunks=True,originalSyncUnchanged=True,originalSyncForwardReverseByteExact=True,indexesUnchanged=True,sourceHashes=expected),indent=2)+'\n')

    def test_poisoned_outer_alternate_index_ignored_nested_real_independent(self):
        alternate=self.root/'poison-alternate.index';alternate.write_bytes(b'not even a Git index')
        before=self.indexes();alt=fixture.digest(alternate)
        with mock.patch.dict(fixture.GIT_CONTEXT,{'GIT_INDEX_FILE':str(alternate)}):result=self.regen()
        self.assertEqual(result.returncode,0,result.stderr);self.assertEqual(before,self.indexes());self.assertEqual(alt,fixture.digest(alternate));self.clean_replay('poison-alternate')
        (OUTPUT/'alternate-proof.json').write_text(json.dumps(dict(inheritedAlternateIgnored=True,actualIndexesUnchanged=True,alternateUnchanged=True,privateNewLeafRegistration=True,nestedUsesIndependentRealIndexCopy=True,originalSyncByteExact=True),indent=2)+'\n')

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
    def test_manifest_assigned_regular_new_nested_leaf_private_registration(self):
        fixture.put(self.nested,'new-owned.ts','export const added = 1;\n')
        with (self.root/'patches/MANIFEST').open('a') as f:f.write('0010-webextras.patch '+NESTED+'/new-owned.ts\n')
        before=self.indexes();result=self.regen();self.assertEqual(result.returncode,0,result.stderr);self.assertEqual(before,self.indexes());self.clean_replay('new-nested')

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
    def test_relative_and_empty_alternate_index_ignored(self):
        before=self.indexes()
        for value in ('relative.index',''):
            with self.subTest(value=value),mock.patch.dict(fixture.GIT_CONTEXT,{'GIT_INDEX_FILE':value}):
                result=self.regen();self.assertEqual(result.returncode,0,result.stderr)
            self.assertEqual(before,self.indexes())

    def test_all_poisoned_git_environment_ignored_wrapper_and_helper(self):
        fake=self.root/'fake-context';fake.mkdir();index=fake/'index';index.write_bytes(b'foreign untouched index')
        env={'GIT_INDEX_FILE':str(index),'GIT_DIR':str(fake),'GIT_WORK_TREE':str(fake),'GIT_OBJECT_DIRECTORY':str(fake/'objects'),'GIT_COMMON_DIR':str(fake),'GIT_NAMESPACE':'wrong','GIT_PREFIX':'wrong/','GIT_CONFIG_COUNT':'1','GIT_CONFIG_KEY_0':'core.worktree','GIT_CONFIG_VALUE_0':str(fake),'GIT_TRACE':str(fake/'must-not-exist')}
        before=self.indexes();foreign=fixture.digest(index)
        for command in ([sys.executable,'-B',self.root/'arc/tools/regen-chromium-patches.py'],['bash',self.root/'arc/tools/regen-patches.sh']):
            with mock.patch.dict(fixture.GIT_CONTEXT,env):result=fixture.run(command,cwd=fake)
            self.assertEqual(result.returncode,0,result.stderr);self.assertEqual(before,self.indexes());self.assertEqual(foreign,fixture.digest(index));self.assertFalse((fake/'must-not-exist').exists())
        self.clean_replay('poison-environment')

    def test_valid_empty_inherited_index_cannot_mark_head_files_new(self):
        alternate=self.root/'valid-empty.index'
        with mock.patch.dict(fixture.GIT_CONTEXT,{'GIT_INDEX_FILE':str(alternate)}):fixture.git(self.source,'read-tree','--empty')
        before=self.indexes();foreign=fixture.digest(alternate)
        with mock.patch.dict(fixture.GIT_CONTEXT,{'GIT_INDEX_FILE':str(alternate)}):result=self.regen()
        self.assertEqual(result.returncode,0,result.stderr);self.assertEqual(before,self.indexes());self.assertEqual(foreign,fixture.digest(alternate))
        patch=(self.root/'patches/0010-webextras.patch').read_bytes();self.assertNotIn(b'new file mode',patch)
        self.clean_replay('valid-empty-alternate')

    def test_manifest_assigned_empty_new_outer_leaf_private_registration(self):
        rel='chrome/browser/new-empty.cc';fixture.put(self.source,rel,b'')
        with (self.root/'patches/MANIFEST').open('a') as f:f.write('0010-webextras.patch '+rel+'\n')
        before=self.indexes();result=self.regen();self.assertEqual(result.returncode,0,result.stderr);self.assertEqual(before,self.indexes());self.clean_replay('private-empty')

    def test_head_tracked_missing_outer_index_entry_refused(self):
        fixture.git(self.source,'update-index','--force-remove',FOREIGN)
        self.reject('HEAD-tracked manifest file missing from real index')

    def test_head_tracked_missing_nested_index_entry_refused(self):
        fixture.git(self.nested,'update-index','--force-remove',FRONTEND[0])
        self.reject('HEAD-tracked manifest file missing from real index')

    def test_existing_split_real_index_refused_without_shared_index_mutation(self):
        fixture.git(self.source,'update-index','--split-index')
        before={p.name:fixture.digest(p) for p in (self.source/'.git').glob('sharedindex.*')}
        self.reject('split real index is unsupported')
        self.assertEqual(before,{p.name:fixture.digest(p) for p in (self.source/'.git').glob('sharedindex.*')})

    def test_split_index_config_cannot_create_real_shared_index(self):
        for repo in (self.source,self.nested):fixture.git(repo,'config','core.splitIndex','true')
        before=self.indexes();result=self.regen();self.assertEqual(result.returncode,0,result.stderr);self.assertEqual(before,self.indexes())
        for repo in (self.source,self.nested):self.assertEqual(list((repo/'.git').glob('sharedindex.*')),[])

    def test_helper_rejects_caller_root_override(self):
        before=fixture.files(self.root/'patches');indexes=self.indexes()
        result=fixture.run([sys.executable,'-B',self.root/'arc/tools/regen-chromium-patches.py',self.directory/'foreign-root'])
        self.assertNotEqual(result.returncode,0);self.assertIn('unrecognized arguments',result.stderr);self.assertEqual(before,fixture.files(self.root/'patches'));self.assertEqual(indexes,self.indexes())

    def test_copied_script_rejects_non_checkout_root(self):
        shutil.rmtree(self.source/'.git');before=fixture.files(self.root/'patches');nested=fixture.digest(self.nested/'.git/index')
        result=fixture.run(['bash',self.root/'arc/tools/regen-patches.sh'])
        self.assertNotEqual(result.returncode,0);self.assertIn('not the real chromium/src checkout',result.stdout);self.assertEqual(before,fixture.files(self.root/'patches'));self.assertEqual(nested,fixture.digest(self.nested/'.git/index'))

    def test_failed_output_preparation_never_rewrites_existing_outputs(self):
        module=types.ModuleType('private_regen');module.__file__=str(self.root/'arc/tools/regen-chromium-patches.py')
        exec(compile(Path(module.__file__).read_bytes(),module.__file__,'exec'),module.__dict__)
        original=module.tempfile.NamedTemporaryFile;count=0
        def fail_second(*args,**kwargs):
            nonlocal count
            count+=1
            if count==2:raise OSError('injected prepare failure')
            return original(*args,**kwargs)
        before=fixture.files(self.root/'patches');identities={p.name:p.stat().st_ino for p in (self.root/'patches').glob('*.patch')};indexes=self.indexes()
        with mock.patch.object(module.tempfile,'NamedTemporaryFile',side_effect=fail_second):
            with self.assertRaisesRegex(OSError,'injected prepare failure'):module.regenerate(self.root)
        self.assertEqual(before,fixture.files(self.root/'patches'));self.assertEqual(indexes,self.indexes());self.assertEqual(identities,{p.name:p.stat().st_ino for p in (self.root/'patches').glob('*.patch')})

    def test_failed_second_publication_atomically_restores_only_first(self):
        module=types.ModuleType('private_regen');module.__file__=str(self.root/'arc/tools/regen-chromium-patches.py')
        exec(compile(Path(module.__file__).read_bytes(),module.__file__,'exec'),module.__dict__)
        original=module.os.replace;count=0
        def fail_second(*args,**kwargs):
            nonlocal count
            count+=1
            if count==2:raise OSError('injected replace failure')
            return original(*args,**kwargs)
        before=fixture.files(self.root/'patches');indexes=self.indexes();untouched=(self.root/'patches/0010-webextras.patch').stat().st_ino
        with mock.patch.object(module.os,'replace',side_effect=fail_second):
            with self.assertRaisesRegex(OSError,'injected replace failure'):module.regenerate(self.root)
        self.assertEqual(before,fixture.files(self.root/'patches'));self.assertEqual(indexes,self.indexes());self.assertEqual(untouched,(self.root/'patches/0010-webextras.patch').stat().st_ino);self.assertEqual(count,3)

    def test_copied_lead_script_demonstrates_real_add_n_difference(self):
        for rel in FRONTEND:fixture.put(self.nested,rel,(RECEIPT/'base'/NESTED/rel).read_bytes())
        manifest=self.root/'patches/MANIFEST';manifest.write_text('\n'.join(line for line in manifest.read_text().splitlines() if NESTED not in line)+'\n')
        fixture.git(self.source,'config','status.showUntrackedFiles','all')
        fixture.put(self.root,'arc/tools/regen-patches.sh',(EVIDENCE/'baseline/arc/tools/regen-patches.sh').read_bytes())
        before=self.indexes();result=fixture.run(['bash',self.root.resolve()/'arc/tools/regen-patches.sh'])
        self.assertEqual(result.returncode,0,result.stderr);self.assertNotEqual(before[0],self.indexes()[0]);self.assertEqual(before[1],self.indexes()[1])
        # Intentional negative control: only this copied historical tool writes
        # its disposable fixture index. The candidate never restores real indices.

    def test_tools_only_native_oracle_matches_copied_lead_before_after(self):
        for rel in FRONTEND:fixture.put(self.nested,rel,(RECEIPT/'base'/NESTED/rel).read_bytes())
        manifest=self.root/'patches/MANIFEST';manifest.write_text('\n'.join(line for line in manifest.read_text().splitlines() if NESTED not in line)+'\n')
        fixture.git(self.source,'add','-N',HOOK)
        candidate=(self.root/'arc/tools/regen-patches.sh').read_bytes()
        fixture.put(self.root,'arc/tools/regen-patches.sh',(EVIDENCE/'baseline/arc/tools/regen-patches.sh').read_bytes())
        before=self.indexes();result=fixture.run(['bash',self.root.resolve()/'arc/tools/regen-patches.sh']);self.assertEqual(result.returncode,0,result.stderr)
        native=fixture.files(self.source);outputs=fixture.files(self.root/'patches');sync=fixture.digest(self.root/'sync-chromium.sh');self.assertEqual(before,self.indexes())
        patch_metadata={p.name:(p.stat().st_ino,p.stat().st_mtime_ns,p.stat().st_ctime_ns) for p in (self.root/'patches').glob('*.patch')}
        fixture.put(self.root,'arc/tools/regen-patches.sh',candidate)
        result=fixture.run(['bash',self.root/'arc/tools/regen-patches.sh']);self.assertEqual(result.returncode,0,result.stderr)
        self.assertEqual(native,fixture.files(self.source));self.assertEqual(outputs,fixture.files(self.root/'patches'));self.assertEqual(before,self.indexes());self.assertEqual(sync,fixture.digest(self.root/'sync-chromium.sh'))
        self.assertEqual(patch_metadata,{p.name:(p.stat().st_ino,p.stat().st_mtime_ns,p.stat().st_ctime_ns) for p in (self.root/'patches').glob('*.patch')})
        (OUTPUT/'native-tools-only-oracle.json').write_text(json.dumps(dict(scope='private three native leaves/two patches; not actual production oracle',patches=outputs,nativeFiles=native,indexesUnchanged=True,sourceUnchanged=True,syncUnchanged=True,toolsOnlyReplacement=True),indent=2)+'\n')

    def test_head_guard_uses_git_metadata_not_headerlike_source_body(self):
        baseline='const char* raw = R"TAG(\n-- /dev/null\n)TAG";\n'
        fixture.put(self.source,FOREIGN,baseline);fixture.git(self.source,'add',FOREIGN);fixture.git(self.source,'commit','-qm','header-like raw string','--only',FOREIGN)
        fixture.put(self.source,FOREIGN,baseline.replace('-- /dev/null','++ b/'+FOREIGN))
        before=self.indexes();result=self.regen();self.assertEqual(result.returncode,0,result.stderr);self.assertEqual(before,self.indexes())
        self.assertIn(('--- /dev/null\n+++ b/'+FOREIGN+'\n').encode(),(self.root/'patches/0004-foreign.patch').read_bytes())

    def index_drift(self,replace_same_bytes):
        wrapper=self.root/'index-drift-bin';wrapper.mkdir();actual=shutil.which('git');marker=self.root/'index-drift-once';index=self.source/'.git/index';original=index.read_bytes()
        program='#!/usr/bin/env python3\nimport os,sys\nfrom pathlib import Path\nmarker=Path('+repr(str(marker))+');index=Path('+repr(str(index))+')\n'
        program+='if "diff" in sys.argv and "HEAD" in sys.argv and not marker.exists():\n marker.write_text("done")\n'
        if replace_same_bytes:program+=' replacement=index.with_name("external-replacement");replacement.write_bytes(index.read_bytes());os.replace(replacement,index)\n'
        else:program+=' index.write_bytes(index.read_bytes()+b"external-index-change")\n'
        program+='os.execv('+repr(actual)+',['+repr(actual)+']+sys.argv[1:])\n';tool=wrapper/'git';tool.write_text(program);tool.chmod(0o755)
        before=fixture.files(self.root/'patches');nested=fixture.digest(self.nested/'.git/index');inode=index.stat().st_ino
        with mock.patch.dict(os.environ,{'PATH':str(wrapper)+os.pathsep+os.environ['PATH']}):result=self.regen()
        self.assertNotEqual(result.returncode,0);self.assertIn('real index changed during regeneration',result.stderr);self.assertEqual(before,fixture.files(self.root/'patches'));self.assertEqual(nested,fixture.digest(self.nested/'.git/index'))
        if replace_same_bytes:self.assertEqual(original,index.read_bytes());self.assertNotEqual(inode,index.stat().st_ino)
        else:self.assertEqual(original+b'external-index-change',index.read_bytes())
        # Tool detects and leaves the external fixture index alone; no restoration.

    def test_real_index_byte_drift_is_not_adopted_or_restored(self):self.index_drift(False)
    def test_real_index_same_bytes_replacement_is_not_adopted_or_restored(self):self.index_drift(True)

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
