#!/usr/bin/env python3
"""Actual freezer import-closure tests; no app, shared comparator edit or import execution."""
import importlib.util
import json
from pathlib import Path
import py_compile
import shutil
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]
spec = importlib.util.spec_from_file_location('actual_fixture_freezer', ROOT/'arc/browser/webextras_fixture_inputs.py')
inputs = importlib.util.module_from_spec(spec); spec.loader.exec_module(inputs)
SUPPORTED = ROOT/'logs/wp/webextras-boundary-arguments-callback-correction/staged'
HELPERS = ('download_identity.py', 'google_q.py', 'omnibox_receivers.py')
IMAGE = 'WebextrasNativeProbe.dylib'


class ImportClosureTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix='webextras-comparator-closure-')
        self.root = Path(self.temp.name)
        self.source = self.root/'source'
        for relative in inputs.FILES:
            path = self.source/relative; path.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT/relative, path)
        self.counter = 0

    def tearDown(self):
        self.temp.cleanup()

    def supported(self):
        # Prepare complete SOURCE only. The actual freezer alone populates snapshots.
        for path in (SUPPORTED/'tests/recorder').glob('*.py'):
            shutil.copyfile(path, self.source/'tests/recorder'/path.name)

    def freeze(self):
        self.counter += 1
        snapshot = self.root/f'snapshot-{self.counter}'
        return snapshot, inputs.freeze(self.source, snapshot, IMAGE)

    def test_legacy_freeze_has_exact_four_inputs_and_original_pending_capability(self):
        snapshot, state = self.freeze()
        self.assertEqual(set(state['files']), set(inputs.FILES))
        self.assertEqual(state['comparator_closure'], {'entry': inputs.FILES[0], 'files': [inputs.FILES[0]]})
        self.assertFalse(state['capability']['verified'])
        self.assertEqual(state['capability'], inputs.capability(snapshot, IMAGE))
        self.assertEqual(inputs.verify(snapshot), state)

    def test_legacy_manifest_without_closure_field_remains_valid(self):
        snapshot, state = self.freeze(); del state['comparator_closure']
        manifest = snapshot/'comparison-inputs.json'; manifest.chmod(0o644); manifest.write_text(json.dumps(state))
        self.assertEqual(inputs.verify(snapshot), state)

    def test_actual_archived_legacy_four_file_manifest_still_valid(self):
        snapshot = ROOT/'results/webextras/native-run9/fixture-comparator'
        original = json.loads((snapshot/'comparison-inputs.json').read_text())
        self.assertEqual(len(original['files']), 4)
        self.assertEqual(inputs.verify(snapshot), original)

    def test_supported_freeze_exact_seven_including_function_local_imports(self):
        self.supported(); snapshot, state = self.freeze()
        expected = set(inputs.FILES) | {'tests/recorder/'+name for name in HELPERS}
        self.assertEqual(set(state['files']), expected)
        self.assertEqual(set(state['comparator_closure']['files']), {inputs.FILES[0], *('tests/recorder/'+name for name in HELPERS)})
        self.assertTrue(state['capability']['verified'], state['capability'])
        self.assertEqual(inputs.verify(snapshot), state)

    def test_supported_snapshot_passes_actual_frontend_four_canaries(self):
        self.supported(); snapshot, state = self.freeze()
        frontend = self.root/'frontend_comparison.py'
        shutil.copyfile(ROOT/'arc/browser/webextras_frontend_comparison.py', frontend)
        spec = importlib.util.spec_from_file_location('actual_frontend_capability', frontend)
        module = importlib.util.module_from_spec(spec); spec.loader.exec_module(module)
        result = module.capability(snapshot, IMAGE)
        self.assertTrue(result['verified'], result)
        self.assertEqual(len(result['cases']), 4)
        self.assertEqual([case['verified'] for case in result['cases']], [True]*4)
        self.assertEqual(len(state['files']), 7)

    def test_mutating_each_frozen_helper_fails(self):
        self.supported()
        for name in HELPERS:
            with self.subTest(name=name):
                snapshot, _ = self.freeze(); p = snapshot/'tests/recorder'/name
                p.chmod(0o644); p.write_bytes(p.read_bytes()+b'\n# changed\n')
                with self.assertRaises((ValueError, OSError)):
                    inputs.verify(snapshot)

    def test_removing_each_frozen_helper_fails(self):
        self.supported()
        for name in HELPERS:
            with self.subTest(name=name):
                snapshot, _ = self.freeze(); (snapshot/'tests/recorder'/name).unlink()
                with self.assertRaises((ValueError, OSError)):
                    inputs.verify(snapshot)

    def test_source_helper_changes_or_removal_do_not_change_frozen_snapshot(self):
        self.supported(); snapshot, state = self.freeze()
        for name in HELPERS:
            p = self.source/'tests/recorder'/name
            p.write_text('raise RuntimeError("live changed")\n'); p.unlink()
        self.assertEqual(inputs.verify(snapshot), state)
        self.assertEqual(inputs.capability(snapshot, IMAGE), state['capability'])

    def test_missing_function_local_helper_rejected_before_capability(self):
        self.supported()
        for name in HELPERS:
            p = self.source/'tests/recorder'/name; saved = p.read_bytes(); p.unlink()
            with patch.object(inputs, 'capability', side_effect=AssertionError('must not execute')):
                with self.assertRaisesRegex(ValueError, 'missing or unknown comparison dependency'):
                    self.freeze()
            p.write_bytes(saved)

    def test_transitive_function_local_imports_discovered_without_execution(self):
        self.supported()
        p = self.source/'tests/recorder/diff.py'; p.write_text(p.read_text()+'\ndef uncalled():\n    from extra_helper import x\n')
        (p.parent/'extra_helper.py').write_text('def nested():\n    import final_helper\nraise RuntimeError("not executed")\n')
        (p.parent/'final_helper.py').write_text('raise RuntimeError("not executed either")\n')
        snapshot, state = self.freeze()
        self.assertTrue(state['capability']['verified'])
        self.assertEqual(len(state['files']), 9)
        self.assertIn('tests/recorder/final_helper.py', state['files'])
        self.assertEqual(inputs.verify(snapshot), state)

    def test_unknown_dependencies_never_search_sys_path(self):
        p = self.source/'tests/recorder/diff.py'; p.write_text(p.read_text()+'\nimport outside_only\n')
        (self.root/'outside_only.py').write_text('x=1\n'); sys.path.insert(0, str(self.root))
        try:
            with self.assertRaisesRegex(ValueError, 'missing or unknown comparison dependency'):
                self.freeze()
        finally:
            sys.path.remove(str(self.root))

    def test_local_helper_symlink_rejected(self):
        self.supported(); p = self.source/'tests/recorder/google_q.py'
        outside = self.root/'outside.py'; outside.write_bytes(p.read_bytes()); p.unlink(); p.symlink_to(outside)
        with self.assertRaisesRegex(ValueError, 'symlink comparison input'):
            self.freeze()

    def test_base_input_symlink_rejected(self):
        p = self.source/'tests/recorder/app_callbacks.inc'; outside = self.root/'callbacks'; shutil.move(p, outside); p.symlink_to(outside)
        with self.assertRaisesRegex(ValueError, 'symlink comparison input'):
            self.freeze()

    def test_recorder_directory_symlink_rejected(self):
        directory = self.source/'tests/recorder'; outside = self.root/'recorder'; shutil.move(directory, outside); directory.symlink_to(outside, target_is_directory=True)
        with self.assertRaisesRegex(ValueError, 'symlink comparison input'):
            self.freeze()

    def test_source_and_snapshot_root_symlinks_rejected(self):
        link = self.root/'source-link'; link.symlink_to(self.source, target_is_directory=True)
        with self.assertRaisesRegex(ValueError, 'symlink comparison root'):
            inputs.freeze(link, self.root/'snapshot', IMAGE)
        target = self.root/'target'; target.mkdir(); link = self.root/'snapshot-link'; link.symlink_to(target, target_is_directory=True)
        with self.assertRaisesRegex(ValueError, 'symlink comparison root'):
            inputs.freeze(self.source, link, IMAGE)

    def test_frozen_helper_symlink_rejected(self):
        self.supported(); snapshot, _ = self.freeze(); p = snapshot/'tests/recorder/google_q.py'
        p.unlink(); p.symlink_to(self.source/'tests/recorder/google_q.py')
        with self.assertRaisesRegex(ValueError, 'symlink comparison input'):
            inputs.verify(snapshot)

    def test_package_relative_and_dynamic_imports_fail_closed(self):
        p = self.source/'tests/recorder/diff.py'; original = p.read_text()
        for statement in ['from .missing import x', 'import local_package.child', '__import__("hidden")',
                          'import importlib\nimportlib.import_module("hidden")',
                          'from importlib import import_module as load\nload("hidden")',
                          'from importlib.util import spec_from_file_location as load',
                          'from runpy import run_path as load', 'exec("import hidden")']:
            with self.subTest(statement=statement):
                p.write_text(original+'\n'+statement+'\n')
                with self.assertRaises(ValueError):
                    self.freeze()

    def test_stdlib_shadow_rejected(self):
        (self.source/'tests/recorder/json.py').write_text('raise RuntimeError("never execute")\n')
        with self.assertRaisesRegex(ValueError, 'shadows stdlib'):
            self.freeze()

    def test_source_mutation_during_copy_is_rejected(self):
        self.supported(); actual = inputs.shutil.copyfileobj; changed = False
        def copy(source, target, *args, **kwargs):
            nonlocal changed
            actual(source, target, *args, **kwargs)
            if not changed:
                changed = True
                p = self.source/'tests/recorder/google_q.py'; p.write_bytes(p.read_bytes()+b'\n# concurrent change\n')
        with patch.object(inputs.shutil, 'copyfileobj', copy), patch.object(inputs, 'capability', side_effect=AssertionError('must not execute')):
            with self.assertRaisesRegex(ValueError, 'comparison source changed while freezing'):
                self.freeze()

    def test_manifest_cannot_omit_helper_or_add_escape(self):
        self.supported()
        for kind in ['omit', 'escape', 'closure']:
            snapshot, state = self.freeze()
            if kind == 'omit': del state['files']['tests/recorder/google_q.py']
            elif kind == 'escape': state['files']['../outside.py'] = state['files'][inputs.FILES[0]]
            else: state['comparator_closure']['files'].remove('tests/recorder/google_q.py')
            p = snapshot/'comparison-inputs.json'; p.chmod(0o644); p.write_text(json.dumps(state))
            with self.assertRaisesRegex(ValueError, 'snapshot.*mismatch'):
                inputs.verify(snapshot)

    def test_unchecked_hash_bytecode_is_rejected_before_execution(self):
        self.supported(); snapshot, _ = self.freeze()
        source = self.root/'unmanifested.py'; source.write_text('raise RuntimeError("unmanifested cache executed")\n')
        cache = snapshot/'tests/recorder/__pycache__'/('download_identity.'+sys.implementation.cache_tag+'.pyc')
        cache.parent.mkdir()
        py_compile.compile(str(source), cfile=str(cache), doraise=True,
                           invalidation_mode=py_compile.PycInvalidationMode.UNCHECKED_HASH)
        for operation in [lambda: inputs.verify(snapshot), lambda: inputs.capability(snapshot, IMAGE)]:
            with self.assertRaisesRegex(ValueError, 'unexpected frozen import-directory entry'):
                operation()

    def test_flat_bytecode_extensions_packages_and_untracked_sources_rejected(self):
        self.supported()
        for name in ['download_identity.pyc', 'download_identity.so', 'download_identity.cpython-315-darwin.so',
                     'download_identity.pyd', 'json.pyc', 'unused.py', 'google_q', '__pycache__']:
            with self.subTest(name=name):
                snapshot, _ = self.freeze(); p = snapshot/'tests/recorder'/name
                if name in ['google_q', '__pycache__']: p.mkdir()
                else: p.write_bytes(b'untracked')
                with self.assertRaises((ValueError, OSError)):
                    inputs.verify(snapshot)

    def test_frozen_freezer_import_directory_cache_and_manifest_links_rejected(self):
        snapshot, _ = self.freeze(); (snapshot/'arc/browser/__pycache__').mkdir()
        with self.assertRaisesRegex(ValueError, 'unexpected frozen import-directory entry'):
            inputs.verify(snapshot)
        snapshot, _ = self.freeze(); p = snapshot/'comparison-inputs.json'; target = self.root/'manifest.json'
        p.rename(target); p.symlink_to(target)
        with self.assertRaisesRegex(ValueError, 'symlink comparison input'):
            inputs.verify(snapshot)


if __name__ == '__main__':
    unittest.main(verbosity=2)
