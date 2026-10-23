#!/usr/bin/env python3
"""Exercise held argument comparison against actual TEMP-applied module/CLI only."""
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]
IMAGE = 'WebextrasNativeProbe.dylib'
FLAG = '--compare-boundary-arguments'
META = {'mode': 'selected-boundary-methods', 'caller_attribution': True}
FILES = ('tests/recorder/diff.py', 'tests/recorder/app_callbacks.inc', 'arc/tools/arccore-1.170.0-objc.tsv')
PATCHES = ('arc/browser/webextras_fixture_comparator.patch', 'arc/browser/webextras_boundary_arguments.patch')


def pair(value=None, caller='app', selector='setTitle:', cls='ArcWebContents', args=None, direction='->engine', depth=0):
    common = dict(thread='main', depth=depth, dir=direction, kind='-', cls=cls, sel=selector, self=f'<{cls}#1>')
    if caller is not None:
        common['from'] = caller
    return [dict(common, ev='call', args=[] if args is None else args), dict(common, ev='ret', ret=value)]


def callback(value=None, args=None):
    return pair(value, 'engine', 'webContentsDevTools:displayAs:', 'ADK.WebContentsDevToolsDelegateBridge', args, 'engine->app')


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


class BoundaryArgumentTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory(prefix='webextras-boundary-arguments-test-')
        cls.repo = Path(cls.temp.name)
        cls.pins = {name: digest(ROOT / name) for name in (*FILES, *PATCHES)}
        for name in FILES:
            target = cls.repo / name
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT / name, target)
        cls.original = cls.repo / 'tests/recorder/diff.original.py'
        shutil.copyfile(cls.repo / FILES[0], cls.original)
        for index, patch in enumerate(PATCHES):
            subprocess.run(['git', 'apply', '--check', str(ROOT / patch)], cwd=cls.repo, capture_output=True, check=True)
            subprocess.run(['git', 'apply', str(ROOT / patch)], cwd=cls.repo, capture_output=True, check=True)
            if index == 0:
                cls.before = cls.repo / 'tests/recorder/diff.fixture.py'
                shutil.copyfile(cls.repo / FILES[0], cls.before)
        spec = importlib.util.spec_from_file_location('boundary_arguments_staged', cls.repo / FILES[0])
        cls.module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(cls.module)
        cls.counter = 0

    @classmethod
    def tearDownClass(cls):
        try:
            for name, before in cls.pins.items():
                if digest(ROOT / name) != before:
                    raise AssertionError('original input changed: ' + name)
        finally:
            cls.temp.cleanup()

    def compare(self, left, right, flags=(), script=None):
        type(self).counter += 1
        directory = self.repo / f'case-{self.counter}'
        directory.mkdir()
        paths = []
        for name, rows in [('golden', left), ('ours', right)]:
            p = directory / name
            if isinstance(rows, Path):
                shutil.copyfile(rows, p)
            elif isinstance(rows, str):
                p.write_text(rows)
            else:
                p.write_text(''.join(json.dumps(dict(r, seq=i), separators=(',', ':')) + '\n' for i, r in enumerate(rows)))
            paths.append(p)
        report_path = directory / 'report.json'
        result = subprocess.run([sys.executable, '-B', str(script or self.repo / FILES[0]), *map(str, paths), '--json', str(report_path), *flags], text=True, capture_output=True)
        report_bytes = report_path.read_bytes() if report_path.exists() else None
        return result, json.loads(report_bytes) if report_bytes else None, report_bytes

    def success(self, left, right, flags=(FLAG,)):
        result, report, _ = self.compare(left, right, flags)
        self.assertEqual(result.returncode, 0, result.stderr)
        if FLAG in flags:
            self.assertEqual(report['boundary_arguments_comparison'], META)
        return report

    def test_module_uses_existing_caller_filter_and_normalization(self):
        rows = pair(args=['http://127.0.0.1:1234/a']) + pair(1, 'engine', 'isReadyToDisplay')
        directory = self.repo / 'module-input'
        directory.write_text(''.join(json.dumps(r) + '\n' for r in rows))
        calls, callbacks = self.module.load(directory, True, require_callers=True)
        self.assertEqual(list(calls), [('->engine', '-', 'ArcWebContents', 'setTitle:')])
        self.assertEqual(calls[next(iter(calls))][0]['args'], '["http://*/a"]')
        self.assertEqual(callbacks, [])
        rows[0]['from'] = ''
        directory.write_text(''.join(json.dumps(r) + '\n' for r in rows))
        with self.assertRaisesRegex(ValueError, 'requires caller attribution'):
            self.module.load(directory, True, require_callers=True)

    def test_app_setter_argument_canary(self):
        report = self.success(pair(args=['left']), pair(args=['right']))
        self.assertEqual(report['arg_mismatch'], [{'method': '->engine -ArcWebContents setTitle:', 'call': 0, 'golden': '["left"]', 'ours': '["right"]'}])
        self.assertFalse(report['return_mismatch'])

    def test_engine_setter_arguments(self):
        report = self.success(pair(caller='engine', args=[1]), pair(caller='engine', args=[2]))
        self.assertEqual(len(report['arg_mismatch']), 1)

    def test_nested_callback_and_app_getter_return(self):
        a, b = callback(args=['<ArcWebContentsDevTools#3>', 3]), callback(args=['<ArcWebContentsDevTools#8>', 3])
        left = [a[0], *pair(True, 'app', 'isReadyToDisplay', depth=1), a[1]]
        right = [b[0], *pair(False, 'app', 'isReadyToDisplay', depth=1), b[1]]
        report = self.success(left, right)
        self.assertEqual(report['return_mismatch'][0]['method'], '->engine -ArcWebContents isReadyToDisplay')
        self.assertFalse(report['arg_mismatch'])

    def test_nested_callback_app_and_engine_setter_values(self):
        a, b = callback(args=[1]), callback(args=[2])
        left = [a[0], *pair(args=['a'], depth=1), *pair(caller='engine', selector='setIsLoading:', args=[True], depth=1), a[1]]
        right = [b[0], *pair(args=['b'], depth=1), *pair(caller='engine', selector='setIsLoading:', args=[False], depth=1), b[1]]
        report = self.success(left, right)
        self.assertEqual(len(report['arg_mismatch']), 3)
        self.assertIsNone(report['callback_sequence']['first_divergence'])

    def test_independent_first_argument_and_return_same_call(self):
        for left, right in [(pair(1, args=[1]), pair(2, args=[2])), (callback(1, [1]), callback(2, [2]))]:
            with self.subTest(left=left):
                report = self.success(left, right)
                self.assertEqual(len(report['return_mismatch']), 1)
                self.assertEqual(len(report['arg_mismatch']), 1)

    def test_first_argument_does_not_hide_later_return(self):
        left = callback(1, [1]) + callback(3, [3]) + callback(5, [5])
        right = callback(1, [2]) + callback(4, [4]) + callback(6, [6])
        report = self.success(left, right)
        self.assertEqual(report['arg_mismatch'][0]['call'], 0)
        self.assertEqual(report['return_mismatch'][0]['call'], 1)
        self.assertEqual(len(report['arg_mismatch']), 1)
        self.assertEqual(len(report['return_mismatch']), 1)
        baseline = self.success(left, right, flags=())
        self.assertEqual(len(baseline['arg_mismatch']), 1)
        self.assertFalse(baseline['return_mismatch'])

    def test_count_mismatch_remains_independent(self):
        report = self.success(pair(args=[1]) * 2, pair(args=[2]))
        self.assertEqual(report['count_mismatch'][0]['golden'], 2)
        self.assertEqual(len(report['arg_mismatch']), 1)

    def test_callback_only_without_fixture_image(self):
        report = self.success(callback(args=[1]), callback(args=[2]))
        self.assertEqual(len(report['arg_mismatch']), 1)
        self.assertNotIn('fixture_comparison', report)

    def test_fixture_mode_and_capability_metadata(self):
        report = self.success(pair(caller=IMAGE, args=[1]), pair(caller=IMAGE, args=[2]), (FLAG, '--fixture-image', IMAGE))
        self.assertEqual(len(report['arg_mismatch']), 1)
        self.assertEqual(report['fixture_comparison']['retained_fixture_events'], {s: {IMAGE: {'call': 1, 'ret': 1}} for s in ['golden', 'ours']})

    def test_engine_getters_and_unselected_images_remain_excluded(self):
        left = callback() + pair(1, 'engine', 'isReadyToDisplay', args=[1]) + pair(args=[1], caller='unselected.dylib')
        right = callback() + pair(2, 'engine', 'isReadyToDisplay', args=[2]) + pair(args=[2], caller='unselected.dylib')
        report = self.success(left, right)
        self.assertFalse(report['arg_mismatch'])
        self.assertFalse(report['return_mismatch'])

    def test_metadata_private_helpers_excluded_from_new_argument_scope(self):
        report = self.success(pair(args=[1], selector='webextrasPrivateHelper'), pair(args=[2], selector='webextrasPrivateHelper'))
        self.assertFalse(report['arg_mismatch'])

    def test_whole_method_native_pointer_constructor_rule_only(self):
        left = pair(args=['<ptr>', 1], cls='ArcBrowserSidePanel', selector='initWithSidePanel:')
        right = pair(args=['<ptr>', 2], cls='ArcBrowserSidePanel', selector='initWithSidePanel:')
        report = self.success(left, right)
        self.assertFalse(report['arg_mismatch'])
        report = self.success(left + pair(args=[1], cls='ArcBrowserSidePanel', selector='initWithSidePanel:'), right + pair(args=[2], cls='ArcBrowserSidePanel', selector='initWithSidePanel:'))
        self.assertEqual(len(report['arg_mismatch']), 1)
        # Other pointer-shaped contents are not a new exclusion.
        report = self.success(pair(args=['<opaque-pointer>', 1]), pair(args=['<opaque-pointer>', 2]))
        self.assertEqual(len(report['arg_mismatch']), 1)

    def test_constructor_return_behavior_and_classification_unchanged(self):
        left = pair(1, args=['<ptr>'], cls='ArcBrowserSidePanel', selector='initWithSidePanel:')
        right = pair(2, args=['<ptr>'], cls='ArcBrowserSidePanel', selector='initWithSidePanel:')
        report = self.success(left, right)
        self.assertEqual(len(report['return_mismatch']), 1)
        self.assertFalse(report['arg_mismatch'])
        report = self.success(left + callback(), callback())
        self.assertEqual(len(report['internal']), 1)

    def test_existing_object_and_volatile_normalization_unchanged(self):
        left = pair(args=['<ArcWebContents#7>', '127.0.0.1:1000', '/var/folders/a/path', '11111111-1111-1111-1111-111111111111'])
        right = pair(args=['<ArcWebContents#18>', '127.0.0.1:9999', '/var/folders/b/path', '22222222-2222-2222-2222-222222222222'])
        report = self.success(left, right)
        self.assertFalse(report['arg_mismatch'])

    def test_invalid_and_mixed_callers_fail_only_opt_in(self):
        for value in [None, '', 3, False, ' app', 'app ', 'a\n', 'a\x00']:
            with self.subTest(value=value):
                rows = pair(caller=value)
                result, report, _ = self.compare(rows, pair(), (FLAG,))
                self.assertEqual(result.returncode, 2)
                self.assertIn('requires caller attribution', result.stderr)
                self.assertIsNone(report)
        rows = pair() + pair(caller=None)
        result, report, _ = self.compare(rows, pair(), (FLAG,))
        self.assertEqual(result.returncode, 2)
        self.assertIsNone(report)
        for left, right in [(rows, pair()), (pair(caller=None), pair(caller=None))]:
            before, old, old_bytes = self.compare(left, right, script=self.before)
            after, current, current_bytes = self.compare(left, right)
            self.assertEqual((after.returncode, after.stdout, after.stderr, current_bytes), (before.returncode, before.stdout.replace(str(self.before), str(self.repo / FILES[0])), before.stderr, old_bytes))

    def test_empty_hooked_only_or_return_only_fail_closed(self):
        for rows in [[], [dict(ev='hooked', engine_classes=1)], pair()[1:]]:
            result, report, _ = self.compare(rows, callback(), (FLAG,))
            self.assertEqual(result.returncode, 2)
            self.assertIn('at least one attributed call', result.stderr)
            self.assertIsNone(report)

    def test_malformed_json_or_nonobject_fail_closed(self):
        for rows in ['not-json\n', '[]\n', 'null\n']:
            result, report, _ = self.compare(rows, callback(), (FLAG,))
            self.assertEqual(result.returncode, 2)
            self.assertIsNone(report)

    def test_unknown_or_abbreviated_option_fails(self):
        for flag in ['--compare-boundary-argument', '--compare-boundary', '--compare-boundary-arguments=yes', '--unknown']:
            result, report, _ = self.compare(pair(), pair(), (flag,))
            self.assertEqual(result.returncode, 2)
            self.assertIsNone(report)

    def test_unsupported_comparators_cannot_pass_argument_canary(self):
        for script in [self.original, self.before]:
            result, report, _ = self.compare(pair(args=[1]), pair(args=[2]), (FLAG,), script=script)
            supported = result.returncode == 0 and isinstance(report, dict) and report.get('boundary_arguments_comparison') == META and len(report.get('arg_mismatch', [])) == 1
            self.assertFalse(supported)

    def test_first_two_lines_and_report_keys_unchanged_when_disabled(self):
        rows = [(callback(1, [1]), callback(2, [2])), (pair(args=[1]), pair(args=[2])), (pair(1, caller=IMAGE), pair(2, caller=IMAGE))]
        for left, right in rows:
            for flags in [(), ('--fixture-image', IMAGE)] if left[0]['from'] == IMAGE else [()]:
                baseline, old, old_bytes = self.compare(left, right, flags, self.before)
                result, report, report_bytes = self.compare(left, right, flags)
                self.assertEqual(result.returncode, baseline.returncode)
                self.assertEqual(result.stdout, baseline.stdout)
                self.assertEqual(report_bytes, old_bytes)
                self.assertNotIn('boundary_arguments_comparison', report)

    def test_archived_default_full_stdout_and_json_bytes_unchanged(self):
        archives = [('results/webextras/values-run1/original/record.jsonl', 'results/webextras/values-run1/ours/record.jsonl'), ('results/golden/arc-1.170-media-pip/launch-1/record.jsonl', 'results/webextras/media-pip-run2/launch-1/record.jsonl')]
        for left, right in archives:
            before, old, old_bytes = self.compare(ROOT / left, ROOT / right, script=self.before)
            result, report, report_bytes = self.compare(ROOT / left, ROOT / right)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertEqual(result.stdout, before.stdout)
            self.assertEqual(report_bytes, old_bytes)


if __name__ == '__main__':
    unittest.main(verbosity=2)
