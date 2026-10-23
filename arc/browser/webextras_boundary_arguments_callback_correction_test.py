#!/usr/bin/env python3
"""Actual TEMP full-chain+87be+callback correction; no shared edits or runtime."""
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys
import unittest

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]
OLDER_TEST = ROOT / 'arc/browser/webextras_boundary_arguments_after_normalization_test.py'
spec = importlib.util.spec_from_file_location('held_fullchain_argument_tests', OLDER_TEST)
older = importlib.util.module_from_spec(spec)
spec.loader.exec_module(older)
CORRECTION = ROOT / 'arc/browser/webextras_boundary_arguments_callback_correction.patch'


def determining_filename(timeout, nested=False):
    item = '<ArcDownloadItem#1>'
    proof = older.base.pair('00000000-0000-4000-8000-000000000001', 'engine', 'id', 'ArcDownloadItem', [])
    args = ['<ArcBrowserContext#1>', item, '<ptr>', timeout, '<block>']
    cb = older.base.pair(None, 'engine', 'browserContext:onDeterminingFilename:defaultFilename:completionTimeout:completion:', 'ADK.BrowserContextDownloadDelegateBridge', args, 'engine->app')
    if nested:
        # Exercise nested selected app work while preserving the callback's literal filename.
        getter = older.base.pair(True, 'app', 'isReadyToDisplay', 'ArcWebContents', [], depth=1)
        cb = [cb[0], *getter, cb[1]]
    return proof + cb


class CallbackCorrectionTests(older.FullChainBoundaryArgumentTests):
    @classmethod
    def setUpClass(cls):
        super().setUpClass()
        cls.pins[str(CORRECTION)] = older.base.digest(CORRECTION)
        cls.pins[str(OLDER_TEST)] = older.base.digest(OLDER_TEST)
        cls.bad_87be = cls.repo / 'tests/recorder/diff.87be-before-correction.py'
        shutil.copyfile(cls.repo / older.base.FILES[0], cls.bad_87be)
        for command in [['git', 'apply', '--check', str(CORRECTION)], ['git', 'apply', str(CORRECTION)]]:
            subprocess.run(command, cwd=cls.repo, capture_output=True, check=True)
        spec = importlib.util.spec_from_file_location('held_fullchain_callback_corrected', cls.repo / older.base.FILES[0])
        cls.module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(cls.module)

    def test_exact_filename_literal_ptr_timeout_regression(self):
        left, right = determining_filename(15), determining_filename(16)
        default, default_report, _ = self.compare(left, right, script=self.bad_87be)
        bad, bad_report, _ = self.compare(left, right, (older.base.FLAG,), script=self.bad_87be)
        self.assertEqual((default.returncode, bad.returncode), (0, 0))
        self.assertEqual(len(default_report['arg_mismatch']), 1)
        self.assertEqual(bad_report['arg_mismatch'], [])  # Independently reproduce the87be defect.
        report = self.success(left, right)
        self.assertEqual(len(report['arg_mismatch']), 1)
        self.assertTrue(report['arg_mismatch'][0]['method'].endswith('browserContext:onDeterminingFilename:defaultFilename:completionTimeout:completion:'))
        self.assertIn('15', report['arg_mismatch'][0]['golden'])
        self.assertIn('16', report['arg_mismatch'][0]['ours'])
        self.assertFalse(report['return_mismatch'])
        corrected_default, _, corrected_bytes = self.compare(left, right)
        self.assertEqual(corrected_default.stdout, default.stdout)
        self.assertEqual(corrected_bytes, self.compare(left, right, script=self.bad_87be)[2])

    def test_filename_callback_with_nested_app_getter(self):
        report = self.success(determining_filename(15, True), determining_filename(16, True))
        self.assertEqual(len(report['arg_mismatch']), 1)
        self.assertFalse(report['return_mismatch'])

    def test_nested_payload_literal_ptr_does_not_hide_callback_difference(self):
        def rows(value):
            return older.base.callback(args=[{'filename': '<ptr>', 'nested': [1, {'timeout': value}]}])
        report = self.success(rows(15), rows(16))
        self.assertEqual(len(report['arg_mismatch']), 1)
        self.assertIn('"timeout": 15', report['arg_mismatch'][0]['golden'])
        self.assertIn('"timeout": 16', report['arg_mismatch'][0]['ours'])

    def test_callback_literal_ptr_argument_and_return_are_independent(self):
        left = older.base.callback(1, ['<ptr>', {'value': 15}])
        right = older.base.callback(2, ['<ptr>', {'value': 16}])
        report = self.success(left, right)
        self.assertEqual(len(report['arg_mismatch']), 1)
        self.assertEqual(len(report['return_mismatch']), 1)
        # Preserve existing default return-first precedence.
        old, old_report, old_bytes = self.compare(left, right, script=self.bad_87be)
        current, current_report, current_bytes = self.compare(left, right)
        self.assertEqual((current.returncode, current.stdout, current_bytes), (old.returncode, old.stdout, old_bytes))
        self.assertEqual(old_report['arg_mismatch'], [])

    def test_callback_first_argument_mismatch_exactly_once(self):
        left = older.base.callback(args=['<ptr>', 0]) + older.base.callback(args=['<ptr>', 15]) + older.base.callback(args=['<ptr>', 20])
        right = older.base.callback(args=['<ptr>', 0]) + older.base.callback(args=['<ptr>', 16]) + older.base.callback(args=['<ptr>', 21])
        report = self.success(left, right)
        self.assertEqual(len(report['arg_mismatch']), 1)
        self.assertEqual(report['arg_mismatch'][0]['call'], 1)
        self.assertEqual(report['callback_sequence']['golden'], 3)
        self.assertEqual(report['callback_sequence']['ours'], 3)


if __name__ == '__main__':
    unittest.main(verbosity=2)
