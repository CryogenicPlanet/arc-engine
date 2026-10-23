#!/usr/bin/env python3
"""Actual binary_identity extraction on a private disposable framework fixture."""
import ast
import hashlib
import json
from pathlib import Path
import plistlib
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]


class Resources(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory(prefix='webextras-resources-proposal-')
        self.base = Path(self.temporary.name); self.root = self.base / 'arc-engine'; self.root.mkdir()
        source = ROOT / 'arc/browser/webextras_scenario_current_run.py'
        self.source_sha = hashlib.sha256(source.read_bytes()).hexdigest(); self.original = source.read_text()
        stage = self.base / 'stage'; (stage / 'arc/browser').mkdir(parents=True)
        path = stage / source.relative_to(ROOT); path.write_text(self.original)
        subprocess.run(['git', 'init', '--quiet', str(stage)], check=True, capture_output=True)
        proposal = ROOT / 'arc/browser/webextras_scenario_resources_held.patch'
        for check in (True, False):
            subprocess.run(['git', 'apply', '--whitespace=error'] + (['--check'] if check else []) + [str(proposal)],
                           cwd=stage, check=True, capture_output=True)
        self.staged = path.read_text()
        self.app = self.base / 'arc/build/ArcModAgent-ours156.app'
        self.framework = self.app / 'Contents/Frameworks/ArcCore.framework'
        self.resources = self.framework / 'Versions/A/Resources/resources.pak'
        paths = [self.app / 'Contents/MacOS/Arc', self.framework / 'Versions/A/ArcCore',
                 self.resources, self.root / 'tests/recorder/ArcEngineRecorder.dylib',
                 self.framework / 'Helpers/Private Helper.app/Contents/MacOS/Helper']
        for p in paths: p.parent.mkdir(parents=True, exist_ok=True); p.write_bytes(b'private test bytes\n')
        for p, value in [(self.app / 'Contents/Info.plist', {'ArcModAgentSession': 'ours156',
                            'CFBundleIdentifier': 'company.thebrowser.Browser.mod.agent.ours156'}),
                         (self.framework / 'Helpers/Private Helper.app/Contents/Info.plist', {'CFBundleExecutable': 'Helper'})]:
            with p.open('wb') as output: plistlib.dump(value, output)

    def tearDown(self):
        self.assertEqual(hashlib.sha256((ROOT / 'arc/browser/webextras_scenario_current_run.py').read_bytes()).hexdigest(),
                         self.source_sha, 'live runner changed during private resource test')
        self.temporary.cleanup()

    def functions(self, source):
        names = {'require', 'now', 'identity', 'binary_identity'}
        nodes = [x for x in ast.parse(source).body if isinstance(x, ast.FunctionDef) and x.name in names]
        import datetime
        scope = dict(APP=self.app, ROOT=self.root, ARC=self.base / 'arc', hashlib=hashlib,
                     plistlib=plistlib, Path=Path, datetime=datetime)
        exec(compile(ast.Module(body=nodes, type_ignores=[]), '<actual-held-binary-identity>', 'exec'), scope)
        return scope

    def test_stable_identity_and_exact_resource_path(self):
        fn = self.functions(self.staged)['binary_identity']
        before, after = fn(), fn()
        self.assertTrue(all(before[k] == after[k] for k in ('main', 'engine', 'helpers', 'recorder', 'resources')))
        self.assertEqual(before['resources']['path'], str(self.resources))
        self.assertEqual(before['resources']['sha256'], hashlib.sha256(self.resources.read_bytes()).hexdigest())

    def test_resource_flip_detected_by_actual_new_key_guard(self):
        fn = self.functions(self.staged)['binary_identity']; before = fn()
        self.resources.write_bytes(b'changed resource bytes\n'); after = fn()
        guard = "all(after_binaries[k] == binaries[k] for k in ('main', 'engine', 'helpers', 'recorder', 'resources'))"
        self.assertIn(guard, self.staged)
        self.assertFalse(eval(guard, {'after_binaries': after, 'binaries': before}))

    def test_original_four_keys_miss_the_same_flip(self):
        fn = self.functions(self.original)['binary_identity']; before = fn()
        self.resources.write_bytes(b'changed resources ignored by old proof\n'); after = fn()
        self.assertNotIn('resources', before)
        self.assertTrue(all(before[k] == after[k] for k in ('main', 'engine', 'helpers', 'recorder')))

    def test_missing_or_symlink_resource_fails(self):
        fn = self.functions(self.staged)['binary_identity']
        self.resources.unlink()
        with self.assertRaisesRegex(RuntimeError, 'regular packaged resources'): fn()
        other = self.resources.with_name('other.pak'); other.write_bytes(b'other\n')
        self.resources.symlink_to(other)
        with self.assertRaisesRegex(RuntimeError, 'regular packaged resources'): fn()


if __name__ == '__main__':
    unittest.main()
