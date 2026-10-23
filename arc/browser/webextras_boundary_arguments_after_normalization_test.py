#!/usr/bin/env python3
"""Run unchanged22 argument tests and normalization controls on TEMP full chain."""
import ast
import contextlib
import importlib.util
import io
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]
EVIDENCE = ROOT / 'logs/wp/webextras-boundary-arguments-after-normalization'
BASE_TEST = ROOT / 'arc/browser/webextras_boundary_arguments_test.py'
spec = importlib.util.spec_from_file_location('held_argument_test_bodies', BASE_TEST)
base = importlib.util.module_from_spec(spec)
spec.loader.exec_module(base)
CHAIN = [Path(p) for p in json.loads((EVIDENCE / 'initial-manifest.json').read_text())['chainPins'] if p.endswith('.patch') and not p.endswith('/webextras_boundary_arguments.patch')]
REBASE = ROOT / 'arc/browser/webextras_boundary_arguments_after_normalization.patch'
DOWNLOAD_FIXTURE = Path('/tmp/arc-profile-download-normalizer-after-chain/controls.py')
RECEIVER_FIXTURE = Path('/tmp/arc-profile-download-normalizer-after-chain/peer-tests/test_receivers.py')
GOOGLE_FIXTURE = Path('/tmp/arc-profile-download-normalizer-after-chain/peer-tests/test_google_integration.py')


def source_function(path, name, namespace):
    node = next(n for n in ast.parse(path.read_text()).body if isinstance(n, ast.FunctionDef) and n.name == name)
    exec(compile(ast.Module(body=[node], type_ignores=[]), str(path), 'exec'), namespace)
    return namespace[name]


class FullChainBoundaryArgumentTests(base.BoundaryArgumentTests):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory(prefix='webextras-boundary-arguments-fullchain-test-')
        cls.repo = Path(cls.temp.name)
        inputs = [*(ROOT / n for n in (*base.FILES, *base.PATCHES)), ROOT / 'arc/DIFF-ALLOWLIST.md', BASE_TEST, *CHAIN, REBASE, DOWNLOAD_FIXTURE, RECEIVER_FIXTURE, GOOGLE_FIXTURE]
        cls.pins = {str(p): base.digest(p) for p in inputs}
        for name in (*base.FILES, 'arc/DIFF-ALLOWLIST.md'):
            target = cls.repo / name
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT / name, target)
        cls.original = cls.repo / 'tests/recorder/diff.original.py'
        shutil.copyfile(cls.repo / base.FILES[0], cls.original)
        for proposal in CHAIN:
            subprocess.run(['git', 'apply', '--check', str(proposal)], cwd=cls.repo, capture_output=True, check=True)
            subprocess.run(['git', 'apply', str(proposal)], cwd=cls.repo, capture_output=True, check=True)
        cls.before = cls.repo / 'tests/recorder/diff.fixture.py'
        shutil.copyfile(cls.repo / base.FILES[0], cls.before)
        cls.before_sha = base.digest(cls.before)
        subprocess.run(['git', 'apply', '--check', str(REBASE)], cwd=cls.repo, capture_output=True, check=True)
        subprocess.run(['git', 'apply', str(REBASE)], cwd=cls.repo, capture_output=True, check=True)
        cls.module_path = str(cls.repo / 'tests/recorder')
        sys.path.insert(0, cls.module_path)
        spec = importlib.util.spec_from_file_location('held_argument_fullchain', cls.repo / base.FILES[0])
        cls.module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(cls.module)
        cls.counter = 0

    @classmethod
    def tearDownClass(cls):
        sys.path.remove(cls.module_path)
        super().tearDownClass()

    def test_existing_epoch_raw_prepass_with_new_argument_mismatch(self):
        def rows(epoch, title):
            return base.pair(None, 'engine', 'init:value:', 'ArcAutocompleteMatchAdditionalData', ['last visit since epoch', epoch]) + base.pair(epoch, 'app', 'value', 'ArcAutocompleteMatchAdditionalData', []) + base.pair(args=[title])
        report = self.success(rows('1792759966', 'left'), rows('1792759977', 'right'))
        self.assertFalse(report['return_mismatch'])
        self.assertEqual([x['method'] for x in report['arg_mismatch']], ['->engine -ArcWebContents setTitle:'])

    def test_existing_session_aliases_with_new_argument_mismatch(self):
        def rows(sid, title):
            return base.pair(sid, 'app', 'identifier', 'ArcSessionID', []) + base.pair(None, 'engine', 'browser:extensionGroupIdForTabWithSessionId:', 'ADK.ChromiumBrowser', ['<ArcBrowser#1>', sid], 'engine->app') + base.pair(args=[title])
        report = self.success(rows(101, 'left'), rows(707, 'right'))
        self.assertFalse(report['return_mismatch'])
        self.assertEqual([x['method'] for x in report['arg_mismatch']], ['->engine -ArcWebContents setTitle:'])

    def test_existing_download_raw_proof_aliases_with_new_argument_mismatch(self):
        fixture = source_function(DOWNLOAD_FIXTURE, 'fixture', {})
        left = fixture(['<ArcDownloadItem#1>', '<ArcDownloadItem#2>'], ['00000000-0000-4000-8000-000000000001'] * 2, ['<ArcWebContents#1>', '<ArcWebContents#2>'], [101, 101])
        right = fixture(['<ArcDownloadItem#9>'] * 2, ['00000000-0000-4000-8000-000000000003'] * 2, ['<ArcWebContents#9>'] * 2, [707, 707])
        report = self.success(left + base.pair(args=['left']), right + base.pair(args=['right']))
        self.assertFalse(report['return_mismatch'])
        self.assertEqual([x['method'] for x in report['arg_mismatch']], ['->engine -ArcWebContents setTitle:'])

    def omnibox_fixture(self, port=51880, reverse=False, offset=0):
        from urllib.parse import quote
        namespace = {'QUERY': 'autocomplete:cursorPosition:preventInlineAutocomplete:startFresh:keyword:error:completion:', 'URL': 'http://127.0.0.1:9000/synthetic-fixture'}
        fixture = source_function(RECEIVER_FIXTURE, 'fixture', namespace)
        namespace['renumber'] = source_function(RECEIVER_FIXTURE, 'renumber', namespace)
        google = source_function(GOOGLE_FIXTURE, 'fixture', {'FIXTURES': namespace, 'QUERY': namespace['QUERY'], 'quote': quote})
        return google(port, reverse, offset)

    def test_receiver_google_reloads_keep_existing_normalized_values(self):
        left_rows, right_rows = self.omnibox_fixture(reverse=True), self.omnibox_fixture(59037, offset=1000)
        # Keep the source-sensitive fixtures' positive native sequence IDs. The
        # generic22-case helper assigns IDs from zero for its unscoped examples.
        directory = self.repo / 'positive-sequence-reload-fixtures'
        directory.mkdir()
        left, right = directory / 'left', directory / 'right'
        for path, rows in [(left, left_rows), (right, right_rows)]:
            path.write_text(''.join(json.dumps(e) + '\n' for e in rows))
        flags = ('--omnibox-google-query', '--omnibox-receivers')
        baseline, old, old_bytes = self.compare(left, right, flags, script=self.before)
        same, same_report, same_bytes = self.compare(left, right, flags)
        self.assertEqual((same.returncode, same.stdout, same.stderr, same_bytes), (baseline.returncode, baseline.stdout, baseline.stderr, old_bytes))
        result, report, _ = self.compare(left, right, (*flags, base.FLAG))
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertEqual(report.pop('boundary_arguments_comparison'), base.META)
        self.assertEqual(report, old)
        self.assertEqual(report['omnibox_receivers']['status'], 'aligned')
        self.assertEqual(report['omnibox_google_query']['status'], 'applied')
        self.assertFalse(report['arg_mismatch'])

    def test_google_rejection_fallback_keeps_required_callers(self):
        left, right = self.omnibox_fixture(), self.omnibox_fixture(59037)
        query = next(e for e in left if e.get('ev') == 'call' and e.get('cls') == 'ArcBrowser' and e.get('sel', '').startswith('autocomplete:'))
        query['args'][1] = False  # Invalid query provenance, with otherwise valid caller labels.
        directory = self.repo / 'tracked-rejection'
        directory.mkdir()
        paths = []
        for name, rows in [('left', left), ('right', right)]:
            p = directory / name
            p.write_text(''.join(json.dumps(e) + '\n' for e in rows))
            paths.append(p)
        calls = []
        original = self.module.load
        def tracked(*args, **kwargs):
            calls.append(dict(kwargs))
            return original(*args, **kwargs)
        argv = ['diff.py', *map(str, paths), '--json', str(directory / 'report.json'), '--omnibox-google-query', '--omnibox-receivers', base.FLAG]
        with patch.object(self.module, 'load', tracked), patch.object(sys, 'argv', argv), contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit) as result:
                self.module.main()
        self.assertEqual(result.exception.code, 2)
        self.assertGreaterEqual(len(calls), 4)
        self.assertTrue(all(c.get('require_callers') is True for c in calls))
        report = json.loads((directory / 'report.json').read_text())
        self.assertEqual(report['boundary_arguments_comparison'], base.META)
        self.assertEqual(report['omnibox_google_query']['status'], 'rejected')


if __name__ == '__main__':
    unittest.main(verbosity=2)
