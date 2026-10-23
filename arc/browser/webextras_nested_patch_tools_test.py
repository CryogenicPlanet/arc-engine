#!/usr/bin/env python3
"""Private actual-Git fixture for the held nested patch tooling proposal."""
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

ROOT=Path(__file__).resolve().parents[2]
EVIDENCE=ROOT/'logs/wp/webextras-nested-patch-tools'
STAGED=EVIDENCE/'staged'
RECEIPT=ROOT/'logs/wp/webextras-initial-frontend-current-base'
NESTED='third_party/devtools-frontend/src'
FRONTEND=['front_end/core/host/InspectorFrontendHostAPI.ts',
          'front_end/core/host/InspectorFrontendHostStub.ts',
          'front_end/devtools_compatibility.js',
          'front_end/panels/elements/InspectElementModeController.ts',
          'front_end/ui/legacy/DockController.ts']
NATIVE='chrome/browser/devtools/devtools_ui_bindings.cc'
HOOK='chrome/browser/arcmod/arcmod_hooks.h'
FOREIGN='chrome/browser/foreign.cc'
TRANSCRIPTS=[]

def run(argv,cwd=None):
    result=subprocess.run([str(a) for a in argv],cwd=cwd,env=dict(os.environ,GIT_OPTIONAL_LOCKS='0',LC_ALL='C'),capture_output=True,text=True)
    TRANSCRIPTS.append(dict(argv=[str(a) for a in argv],cwd=str(cwd) if cwd else None,exitCode=result.returncode,stdout=result.stdout,stderr=result.stderr))
    return result

def git(repo,*args):
    result=run(['git','-C',repo,*args]);assert result.returncode==0,result.stderr
    return result.stdout.strip()

def put(root,rel,data):
    path=root/rel;path.parent.mkdir(parents=True,exist_ok=True)
    path.write_bytes(data.encode() if isinstance(data,str) else data)

def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()

def files(root):
    return {str(p.relative_to(root)):digest(p) for p in root.rglob('*') if p.is_file() and '.git' not in p.parts}

class NestedTools(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=tempfile.TemporaryDirectory(prefix='webextras-nested-patch-tools-')
        cls.directory=Path(cls.temp.name);cls.base=cls.directory/'base';source=cls.base/'chromium/src';nested=source/NESTED
        for repo in (source,nested):
            repo.mkdir(parents=True,exist_ok=True);git(repo,'init','-q');git(repo,'config','user.name','Private fixture');git(repo,'config','user.email','fixture@example.invalid')
            git(repo,'config','diff.ignoreSubmodules','dirty');git(repo,'config','diff.external','/definitely-not-a-diff-tool')
        for rel in FRONTEND:put(nested,rel,(RECEIPT/'base'/NESTED/rel).read_bytes())
        git(nested,'add','front_end');git(nested,'commit','-qm','pinned frontend fixture')
        put(source,NATIVE,(RECEIPT/'base'/NATIVE).read_bytes());put(source,FOREIGN,'int foreign = 1;\n')
        git(source,'add','chrome');git(source,'update-index','--add','--cacheinfo','160000,'+git(nested,'rev-parse','HEAD')+','+NESTED);git(source,'commit','-qm','outer fixture with frontend gitlink')
        manifest=''.join('0010-webextras.patch '+NESTED+'/'+p+'\n' for p in FRONTEND)
        manifest+='0010-webextras.patch '+NATIVE+'\n0004-foreign.patch '+HOOK+'\n0004-foreign.patch '+FOREIGN+'\n'
        put(cls.base,'patches/MANIFEST',manifest);put(cls.base,'patches/0004-foreign.patch','preserve old foreign output\n');put(cls.base,'patches/0010-webextras.patch','preserve old own output\n')
        for rel in ('arc/tools/regen-patches.sh','arc/tools/chromium-patches.py','sync-chromium.sh'):put(cls.base,rel,(STAGED/rel).read_bytes())

    @classmethod
    def tearDownClass(cls):
        write=EVIDENCE/'test-commands.json';write.write_text(json.dumps(TRANSCRIPTS,indent=2)+'\n')
        cls.temp.cleanup()

    def setUp(self):
        self.root=self.directory/self.id().rsplit('.',1)[-1];shutil.copytree(self.base,self.root)
        self.source=self.root/'chromium/src';self.nested=self.source/NESTED
        self.index_before=[digest(repo/'.git/index') for repo in (self.source,self.nested)]
        for rel in FRONTEND:put(self.nested,rel,(RECEIPT/'intended-output'/NESTED/rel).read_bytes())
        put(self.source,NATIVE,(RECEIPT/'intended-output'/NATIVE).read_bytes());put(self.source,HOOK,(RECEIPT/'intended-output'/HOOK).read_bytes());git(self.source,'add','-N','--',HOOK)
        put(self.source,FOREIGN,'int foreign = 2;\n')

    def cli(self,operation='regenerate',patch=None,root=None):
        root=root or self.root
        return run([sys.executable,'-B',root/'arc/tools/chromium-patches.py',operation,root,*([patch] if patch else [])])

    def unchanged_indexes(self,before=None):
        self.assertEqual(before or self.before_indexes,[digest(repo/'.git/index') for repo in (self.source,self.nested)])

    def reject(self,expected):
        before=files(self.root/'patches');self.before_indexes=[digest(repo/'.git/index') for repo in (self.source,self.nested)]
        result=self.cli();self.assertNotEqual(result.returncode,0);self.assertIn(expected,result.stderr)
        self.assertEqual(before,files(self.root/'patches'));self.unchanged_indexes()

    def test_real_five_frontend_native_foreign_roundtrip(self):
        self.before_indexes=[digest(repo/'.git/index') for repo in (self.source,self.nested)]
        expected=files(self.source);self.assertEqual(self.cli().returncode,0);self.unchanged_indexes()
        first={p.name:p.read_bytes() for p in (self.root/'patches').glob('*.patch')}
        self.assertEqual(self.cli().returncode,0);self.unchanged_indexes();self.assertEqual(first,{p.name:p.read_bytes() for p in (self.root/'patches').glob('*.patch')})
        own=first['0010-webextras.patch'];self.assertEqual(own.count(b'\ndiff --git ')+1,6)
        for rel in FRONTEND:self.assertIn(('b/'+NESTED+'/'+rel).encode(),own)
        self.assertIn(b'int foreign = 2;',first['0004-foreign.patch'])
        clean=self.directory/'roundtrip-clean';shutil.copytree(self.base,clean)
        for name,data in first.items():put(clean,'patches/'+name,data)
        clean_source=clean/'chromium/src';clean_nested=clean_source/NESTED
        indexes=[digest(p/'.git/index') for p in (clean_source,clean_nested)]
        for name in sorted(first):
            patch=clean/'patches'/name
            result=self.cli('apply',patch,clean);self.assertEqual(result.returncode,0,result.stderr)
            again=self.cli('apply',patch,clean);self.assertEqual(again.returncode,0);self.assertIn('already applied',again.stdout)
        self.assertEqual(expected,files(clean_source));self.assertEqual(indexes,[digest(p/'.git/index') for p in (clean_source,clean_nested)])
        for name in reversed(sorted(first)):
            patch=clean/'patches'/name;git(clean_source,'apply','--reverse','--check',str(patch));git(clean_source,'apply','--reverse',str(patch))
        self.assertEqual(files(self.base/'chromium/src'),files(clean_source))
        (EVIDENCE/'roundtrip-proof.json').write_text(json.dumps(dict(files=expected,frontendFiles=5,nativeFiles=1,foreignFilesPreserved=1,intentToAdd=1,patchSHA256={k:hashlib.sha256(v).hexdigest() for k,v in first.items()},indexesUnchanged=True,forwardReverseByteExact=True),indent=2)+'\n')

    def test_staged_and_unstaged_worktree_bytes(self):
        put(self.source,FOREIGN,'int foreign = 3;\n');git(self.source,'add',FOREIGN);put(self.source,FOREIGN,'int foreign = 4;\n')
        self.before_indexes=[digest(repo/'.git/index') for repo in (self.source,self.nested)]
        self.assertEqual(self.cli().returncode,0);self.unchanged_indexes()
        patch=(self.root/'patches/0004-foreign.patch').read_text();self.assertIn('+int foreign = 4;',patch);self.assertNotIn('+int foreign = 3;',patch)

    def test_nested_only_patch_and_outer_only_patch(self):
        manifest=self.root/'patches/MANIFEST';manifest.write_text(manifest.read_text().replace('0010-webextras.patch '+NATIVE,'0004-foreign.patch '+NATIVE))
        self.assertEqual(self.cli().returncode,0)
        own=(self.root/'patches/0010-webextras.patch').read_bytes();self.assertEqual(own.count(b'\ndiff --git ')+1,5)
        clean=self.directory/'nested-only-clean';shutil.copytree(self.base,clean);put(clean,'patches/MANIFEST',manifest.read_bytes())
        for name in ('0004-foreign.patch','0010-webextras.patch'):
            put(clean,'patches/'+name,(self.root/'patches'/name).read_bytes())
            result=self.cli('apply',clean/'patches'/name,clean);self.assertEqual(result.returncode,0,result.stderr)
        self.assertEqual(files(self.source),files(clean/'chromium/src'))

    def test_source_drift_rejected_before_any_output(self):
        wrapper=self.root/'fake-bin';wrapper.mkdir();actual=shutil.which('git');marker=self.root/'drift-once'
        program='#!/usr/bin/env python3\nimport os,sys\nfrom pathlib import Path\n'
        program+='marker=Path('+repr(str(marker))+')\n'
        program+='if "diff" in sys.argv and "HEAD" in sys.argv and not marker.exists():\n'
        program+=' marker.write_text("done");Path('+repr(str(self.source/FOREIGN))+').write_text("int foreign = 99;\\n")\n'
        program+='os.execv('+repr(actual)+',['+repr(actual)+']+sys.argv[1:])\n'
        tool=wrapper/'git';tool.write_text(program);tool.chmod(0o755)
        with mock.patch.dict(os.environ,{'PATH':str(wrapper)+os.pathsep+os.environ['PATH']}):self.reject('inputs changed during regeneration')

    def test_untracked_outer_manifest_input_rejected(self):
        put(self.source,'new-known.cc','bad\n')
        with (self.root/'patches/MANIFEST').open('a') as f:f.write('0010-webextras.patch new-known.cc\n')
        self.reject('untracked/unsupported manifest input')

    def test_apply_unsafe_mode_rejected(self):
        self.assertEqual(self.cli().returncode,0)
        patch=self.root/'patches/0004-foreign.patch';data=patch.read_bytes();self.assertIn(b'new file mode 100644',data)
        patch.write_bytes(data.replace(b'new file mode 100644',b'new file mode 120000'))
        self.assertIn('unsupported patch file mode',self.cli('apply',patch).stderr)

    def test_sync_proposed_shell_syntax(self):
        self.assertEqual(run(['bash','-n',self.root/'sync-chromium.sh']).returncode,0)
        self.assertEqual(run(['bash',self.root/'arc/tools/regen-patches.sh']).returncode,0)
        clean=self.directory/'sync-loop-clean';shutil.copytree(self.base,clean)
        for patch in (self.root/'patches').glob('*.patch'):put(clean,'patches/'+patch.name,patch.read_bytes())
        script=(clean/'sync-chromium.sh').read_text();start=script.index('for p in "$E"/patches/*.patch; do');end=script.index('\ndone',start)+len('\ndone')
        loop='set -euo pipefail\nE="$1"\nstep(){ printf "%s\\n" "$*"; }\n'+script[start:end]
        result=run(['bash','-c',loop,'private-sync-loop',clean]);self.assertEqual(result.returncode,0,result.stderr)
        self.assertEqual(files(self.source),files(clean/'chromium/src'))
        again=run(['bash','-c',loop,'private-sync-loop',clean]);self.assertEqual(again.returncode,0);self.assertEqual(again.stdout.count('already applied'),2)

    def test_duplicate_manifest(self):
        with (self.root/'patches/MANIFEST').open('a') as f:f.write('0004-foreign.patch '+NATIVE+'\n')
        self.reject('duplicate manifest ownership')

    def test_manifest_escape(self):
        with (self.root/'patches/MANIFEST').open('a') as f:f.write('0010-webextras.patch ../escape.cc\n')
        self.reject('unsafe manifest path')

    def test_manifest_absolute(self):
        with (self.root/'patches/MANIFEST').open('a') as f:f.write('0010-webextras.patch /escape.cc\n')
        self.reject('unsafe manifest path')

    def test_unknown_nested_untracked(self):
        put(self.nested,'front_end/untracked.ts','bad\n');self.reject('unsupported/untracked change')

    def test_manifest_nested_untracked(self):
        put(self.nested,'front_end/untracked.ts','bad\n')
        with (self.root/'patches/MANIFEST').open('a') as f:f.write('0010-webextras.patch '+NESTED+'/front_end/untracked.ts\n')
        self.reject('untracked/unsupported manifest input')

    def test_unknown_outer_untracked(self):
        put(self.source,'unknown.cc','bad\n');self.reject('unsupported/untracked change')

    def test_unassigned_outer(self):
        put(self.source,'other.cc','base\n');git(self.source,'add','other.cc');self.reject('unassigned changed file')

    def test_missing_foreign_source(self):
        (self.source/FOREIGN).unlink();self.reject('missing input')

    def test_symlink_leaf(self):
        p=self.nested/FRONTEND[0];p.unlink();p.symlink_to('/etc/hosts');self.reject('symlink input')

    def test_symlink_parent(self):
        p=self.source/'chrome/browser';shutil.rmtree(p);p.symlink_to(self.nested);self.reject('symlink input')

    def test_nested_head_moved(self):
        git(self.nested,'add','front_end');git(self.nested,'commit','-qm','unexpected frontend head');self.reject('frontend checkout HEAD differs')

    def test_gitlink_index_moved(self):
        git(self.nested,'add','front_end');git(self.nested,'commit','-qm','unexpected frontend head');oid=git(self.nested,'rev-parse','HEAD')
        git(self.source,'update-index','--cacheinfo','160000,'+oid+','+NESTED);self.reject('frontend gitlink index/HEAD differs')

    def test_rename_rejected(self):
        git(self.source,'mv',FOREIGN,'chrome/browser/renamed.cc');self.reject('untracked/unsupported manifest input')

    def test_partial_patch_fails_without_writes(self):
        self.assertEqual(self.cli().returncode,0)
        clean=self.directory/'partial-clean';shutil.copytree(self.base,clean)
        patch=clean/'patches/0010-webextras.patch';patch.write_bytes((self.root/'patches/0010-webextras.patch').read_bytes())
        put(clean/'chromium/src',NATIVE,(self.source/NATIVE).read_bytes());before=files(clean/'chromium/src')
        result=self.cli('apply',patch,clean);self.assertNotEqual(result.returncode,0);self.assertEqual(before,files(clean/'chromium/src'))

    def test_apply_path_outside_manifest_rejected(self):
        self.assertEqual(self.cli().returncode,0)
        patch=self.root/'outside.patch';patch.write_bytes((self.root/'patches/0010-webextras.patch').read_bytes())
        self.assertIn('outside manifest directory',self.cli('apply',patch).stderr)

    def test_apply_patch_symlink_rejected(self):
        self.assertEqual(self.cli().returncode,0)
        patch=self.root/'patches/symlink.patch';patch.symlink_to('0010-webextras.patch')
        self.assertIn('outside manifest directory',self.cli('apply',patch).stderr)

    def test_apply_wrong_owner_rejected(self):
        self.assertEqual(self.cli().returncode,0)
        patch=self.root/'patches/0004-foreign.patch';patch.write_bytes((self.root/'patches/0010-webextras.patch').read_bytes())
        self.assertIn('patch ownership mismatch',self.cli('apply',patch).stderr)

if __name__=='__main__':unittest.main(verbosity=2)
