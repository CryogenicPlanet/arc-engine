#!/usr/bin/env python3
"""Private filesystem/actual TS resource functions; no engine, app or lock runs."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]
CHECKER = ROOT/'arc/browser/webextras_resource_identity.py'
spec = importlib.util.spec_from_file_location('resource_proposal', CHECKER)
api = importlib.util.module_from_spec(spec); spec.loader.exec_module(api)


class ResourceIdentityTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix='webextras-resource-fixture-')
        self.root = Path(self.temp.name)
        self.built = self.root/'built'; self.built.mkdir()
        (self.built/'resources.pak').write_bytes(b'private fixture, not a real pak')
        (self.built/'chrome_100_percent.pak').write_bytes(b'scale one')
        (self.built/'nested').mkdir(); (self.built/'nested/asset.js').write_text('fixture')
        self.packaged = self.root/'app/Contents/Frameworks/ArcCore.framework/Versions/A/Resources'
        shutil.copytree(self.built, self.packaged)
        # A private ancestor PID fixture, never the shared lock or its directory.
        self.pid_file = self.root/'fixture-owner-pid'; self.pid_file.write_text(str(os.getpid()))

    def tearDown(self):
        self.temp.cleanup()

    def proof(self):
        return api.package_proof(api.capture(self.built, 'built-ours156'), self.packaged, self.pid_file)

    def test_copy_equality_is_content_not_inode_or_timestamp(self):
        built = api.capture(self.built, 'built-ours156')
        copied = api.capture(self.packaged, 'ours156')
        self.assertEqual(api.content(built), api.content(copied))
        self.assertNotEqual(built['files']['resources.pak']['st_ino'], copied['files']['resources.pak']['st_ino'])
        proof = self.proof(); self.assertEqual(api.verify_proof(proof), proof)
        self.assertEqual(proof['packaged']['observedPaks'], ['resources.pak', 'chrome_100_percent.pak'])

    def test_original_role_has_no_chromium_layout_requirement(self):
        original = self.root/'original'; original.mkdir(); (original/'old-layout.data').write_text('original')
        snapshot = api.capture(original, 'trace156')
        self.assertEqual(snapshot['observedPaks'], [])
        self.assertEqual(api.verify_identity(snapshot), snapshot)
        with self.assertRaisesRegex(ValueError, 'role must be ours156'):
            api.package_proof(snapshot, self.packaged, self.pid_file)

    def test_missing_or_empty_main_pak_fails_ours_only(self):
        (self.built/'resources.pak').unlink()
        with self.assertRaisesRegex(ValueError, 'resources.pak is missing'):
            api.capture(self.built, 'built-ours156')
        self.assertEqual(api.capture(self.built, 'trace156')['observedPaks'], ['chrome_100_percent.pak'])
        (self.built/'resources.pak').touch()
        with self.assertRaisesRegex(ValueError, 'resources.pak is missing or empty'):
            api.capture(self.built, 'ours156')

    def test_scale_and_nested_files_are_not_silently_ignored(self):
        proof = self.proof()
        (self.packaged/'chrome_100_percent.pak').unlink()
        with self.assertRaises(ValueError): api.verify_proof(proof)
        with self.assertRaisesRegex(ValueError, 'bytes/inventory differ'): self.proof()
        shutil.copyfile(self.built/'chrome_100_percent.pak', self.packaged/'chrome_100_percent.pak')
        (self.packaged/'nested/new.js').write_text('new')
        with self.assertRaisesRegex(ValueError, 'bytes/inventory differ'): self.proof()
        (self.packaged/'nested/new.js').unlink()
        (self.packaged/'nested/asset.js').write_text('changed')
        with self.assertRaisesRegex(ValueError, 'bytes/inventory differ'): self.proof()

    def test_symlinks_and_special_entries_fail_without_following(self):
        (self.built/'link').symlink_to(self.packaged/'resources.pak')
        with self.assertRaisesRegex(ValueError, 'symlink resource entry'): api.capture(self.built, 'ours156')
        (self.built/'link').unlink(); (self.root/'root-link').symlink_to(self.built, target_is_directory=True)
        with self.assertRaisesRegex(ValueError, 'direct directory'): api.capture(self.root/'root-link', 'ours156')
        os.mkfifo(self.built/'pipe')
        with self.assertRaisesRegex(ValueError, 'nonregular resource entry'): api.capture(self.built, 'ours156')

    def test_same_length_time_restoration_and_inode_replacement_fail_identity(self):
        path = self.built/'resources.pak'; original = api.capture(self.built, 'ours156'); old = path.stat()
        path.write_bytes(b'x'*old.st_size); os.utime(path, ns=(old.st_atime_ns, old.st_mtime_ns))
        with self.assertRaisesRegex(ValueError, 'identity changed'): api.verify_identity(original)
        current = api.capture(self.built, 'ours156'); data = path.read_bytes(); old = path.stat()
        path.unlink(); path.write_bytes(data); os.utime(path, ns=(old.st_atime_ns, old.st_mtime_ns))
        with self.assertRaisesRegex(ValueError, 'identity changed'): api.verify_identity(current)

    def test_mutation_during_hash_is_rejected(self):
        digest = api.hashlib.file_digest; changed = False
        def mutate(source, algorithm):
            nonlocal changed
            result = digest(source, algorithm)
            if not changed:
                changed = True; (self.built/'resources.pak').write_text('changed concurrently')
            return result
        with patch.object(api.hashlib, 'file_digest', mutate):
            with self.assertRaisesRegex(ValueError, 'changed'): api.capture(self.built, 'ours156')

    def test_actual_cli_journal_and_no_overwrite(self):
        state = api.capture(self.packaged, 'ours156'); journal = self.root/'binary-identity.json'
        journal.write_text(json.dumps({'resources': state}))
        command = [sys.executable, '-B', str(CHECKER), 'verify-journal', str(journal)]
        self.assertEqual(subprocess.run(command, capture_output=True).returncode, 0)
        (self.packaged/'nested/asset.js').write_text('mutation')
        self.assertEqual(subprocess.run(command, capture_output=True).returncode, 2)
        manifest = self.root/'identity.json'; api.write_new(manifest, state)
        with self.assertRaises(FileExistsError): api.write_new(manifest, state)
        self.assertEqual(subprocess.run([sys.executable, '-B', str(CHECKER), '--unknown'], capture_output=True).returncode, 2)

    def test_actual_wrapper_parser_and_expanded_source_guards_without_lock(self):
        wrapper = (ROOT/'arc/browser/webextras_native_resources_current_run.sh').read_text()
        parser = wrapper[wrapper.index('locked=0'):wrapper.index('python3 - "$ROOT" <<')]
        shell = 'set -euo pipefail\nROOT=/unused\nexec(){ printf "EXEC<%s>\\n" "$@"; }\n'+parser
        result = subprocess.run(['/bin/bash', '-c', shell, 'fixture', '--run-number', '13', '--devtools-frontend-events'], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn('EXEC</unused/arc/tools/locked.sh>', result.stdout)
        self.assertIn('EXEC<13>', result.stdout); self.assertIn('EXEC<--devtools-frontend-events>', result.stdout)
        for flags in [['--unknown'], ['--run-number', '0'], ['--run-number', '13', '--run-number', '14']]:
            result = subprocess.run(['/bin/bash', '-c', shell, 'fixture', *flags], capture_output=True, text=True)
            self.assertEqual(result.returncode, 2); self.assertNotIn('EXEC<', result.stdout)
        body = re.search(r'python3 - "\$ROOT" "\$stem" "\$1" "\$media_clock" "\$media_service_clock" "\$frontend_events" <<\'PY\'\n(.*?)\nPY\n', wrapper, re.S).group(1)
        # All source files are copied into a private tree. No script build/run or
        # lock branch is reached; this executes the actual source-snapshot body.
        fixture = self.root/'repository'; fixture.mkdir()
        paths = [*ROOT.glob('arc/api/arc_webextras_*.mm')]
        paths += [ROOT/n for n in (
            'arc/browser/webextras_media.h', 'arc/browser/webextras_values.h', 'arc/wp/webextras.gni',
            'arc/browser/webextras_native_probe.m', 'arc/browser/webextras_native_probe.ts', 'arc/browser/webextras_native_probe.sh',
            'arc/browser/webextras_fixture_inputs.py', 'arc/browser/webextras_fixture_projection.ts',
            'arc/browser/webextras_native_current_run.sh', 'arc/browser/webextras_frontend_comparison.py',
            'arc/browser/webextras_native_resources_current_run.sh', 'arc/browser/webextras_native_resources_probe.ts',
            'arc/browser/webextras_native_resources_probe.sh', 'arc/browser/webextras_resource_identity.py', 'package-engine.sh')]
        for source in paths:
            target = fixture/source.relative_to(ROOT); target.parent.mkdir(parents=True, exist_ok=True); shutil.copyfile(source, target)
        packager = self.root/'arc/tools/build-mod.ts'; packager.parent.mkdir(parents=True); packager.write_text('private packager source fixture')
        def snapshot(phase):
            return subprocess.run([sys.executable, '-', str(fixture), str(self.root/'guard'), phase, '0', '0', '1'], input=body, capture_output=True, text=True)
        self.assertEqual(snapshot('before').returncode, 0)
        self.assertEqual(snapshot('after-build').returncode, 0)
        state = json.loads((self.root/'guard.source-before.json').read_text())
        self.assertEqual(len(state['files']), 21)
        self.assertIn('../arc/tools/build-mod.ts', state['files'])
        self.assertIn('arc/browser/webextras_native_resources_probe.ts', state['files'])
        packager.write_text('changed packager source fixture')
        self.assertNotEqual(snapshot('after-run').returncode, 0)

    def test_actual_typescript_capture_cleanup_and_journal_binding(self):
        proof = self.proof(); proof_path = self.root/'proof.json'; api.write_new(proof_path, proof)
        frozen = self.root/'frozen-checker.py'; shutil.copyfile(CHECKER, frozen)
        original = self.root/'reference/Contents/Frameworks/ArcCore.framework/Versions/A/Resources'
        original.mkdir(parents=True); (original/'different-original.layout').write_text('reference')
        source_path = ROOT/'arc/browser/webextras_native_resources_probe.ts'
        body = r'''
import {readFileSync,writeFileSync,statSync,realpathSync,mkdirSync} from 'node:fs';
import {join} from 'node:path';
const exec=(argv:string[])=>{const p=Bun.spawnSync(argv,{stdout:'pipe',stderr:'pipe'});if(!p.success)throw Error(p.stderr.toString());return p.stdout.toString().trim()};
const assert=(v:any,m:string)=>{if(!v)throw Error(m)};
const source=readFileSync(SOURCE,'utf8');
const identity=source.slice(source.indexOf('function recorderFileIdentity('),source.indexOf('function freezeRecorder('));
const block=source.slice(source.indexOf('// BEGIN WEBEXTRAS_RESOURCE_JOURNAL'),source.indexOf('// END WEBEXTRAS_RESOURCE_JOURNAL'));
const api=new Function('readFileSync','writeFileSync','statSync','realpathSync','join','exec','assert','resourceCheckerIdentity','resourcePackageProofIdentity',new Bun.Transpiler({loader:'ts'}).transformSync(identity+block+';return{captureResourceIdentity,verifyResourceJournal,recorderFileIdentity};'));
const basic=new Function('statSync','realpathSync','exec','assert',new Bun.Transpiler({loader:'ts'}).transformSync(identity+';return recorderFileIdentity;'))(statSync,realpathSync,exec,assert);
const checker={source:basic(CHECKER),frozen:basic(FROZEN)},proof=basic(PROOF);
const tested=api(readFileSync,writeFileSync,statSync,realpathSync,join,exec,assert,checker,proof);
const reference=tested.captureResourceIdentity(REFERENCE,'trace156');assert(reference.observedPaks.length===0,'original layout forced to Chromium');
const resources=tested.captureResourceIdentity(APP,'ours156');
const directory=DIR;mkdirSync(directory);writeFileSync(join(directory,'binary-identity.json'),JSON.stringify({resources}));
const run:any={binaryIdentity:{resources},errors:[]};tested.verifyResourceJournal(run,directory);assert(run.resourceIdentityStatus==='verified-unchanged'&&run.errors.length===0,'clean identity failed');
writeFileSync(RESOURCE,'changed resources');
const bad:any={binaryIdentity:{resources},errors:['earlier measurement failure']};tested.verifyResourceJournal(bad,directory);assert(bad.resourceIdentityStatus==='changed-or-unverifiable'&&bad.errors.length===2&&bad.errors[0]==='earlier measurement failure','failure swallowed existing measurement/cleanup evidence');
// A mutable journal cannot replace the actual in-memory launch identity.
const changed=JSON.parse(exec(['python3','-B',FROZEN,'capture','ours156',RESOURCE_ROOT]));
writeFileSync(join(directory,'binary-identity.json'),JSON.stringify({resources:changed}));
const rewrite:any={binaryIdentity:{resources},errors:[]};tested.verifyResourceJournal(rewrite,directory);assert(rewrite.errors.length===1&&rewrite.errors[0].includes('journal differs from captured'),'rewritten journal trusted');
writeFileSync(FROZEN,readFileSync(FROZEN,'utf8')+'\n# mutation\n');let rejected=false;try{tested.captureResourceIdentity(REFERENCE,'trace156')}catch{rejected=true}assert(rejected,'frozen checker changed unnoticed');
console.log('PASS actual TS resource capture, original-role separation, cleanup error and immutable launch-journal binding');
'''
        replacements = {'SOURCE': str(source_path), 'CHECKER': str(CHECKER), 'FROZEN': str(frozen), 'PROOF': str(proof_path),
                        'REFERENCE': str(self.root/'reference'), 'APP': str(self.root/'app'), 'DIR': str(self.root/'journal'),
                        'RESOURCE_ROOT': str(self.packaged), 'RESOURCE': str(self.packaged/'resources.pak')}
        # Replace token identifiers only, keeping source byte literals intact.
        body = re.sub(r'\b('+ '|'.join(sorted(replacements, key=len, reverse=True)) +r')\b', lambda match: json.dumps(replacements[match[0]]), body)
        script = self.root/'actual-resource-functions.ts'; script.write_text(body)
        result = subprocess.run(['bun', str(script)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == '__main__':
    unittest.main(verbosity=2)
