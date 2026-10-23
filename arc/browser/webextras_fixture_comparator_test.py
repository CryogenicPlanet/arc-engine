#!/usr/bin/env python3
"""Apply the owned proposal in a temporary repository; never edit shared diff.py."""
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
IMAGE = 'WebextrasNativeProbe.dylib'
SECTIONS = ('missing', 'extra', 'return_mismatch', 'arg_mismatch', 'count_mismatch')


def pair(value=1, caller=IMAGE, selector='webContents', cls='ArcWebContentsDevTools', direction='->engine', args=None):
    common = dict(thread='main', depth=1, dir=direction, cls=cls, kind='-', sel=selector, self=f'<{cls}#1>')
    if caller is not None:
        common['from'] = caller
    return [dict(common, ev='call', args=args or []), dict(common, ev='ret', ret=value)]


class ProposalTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temporary = tempfile.TemporaryDirectory(prefix='webextras-fixture-comparator-')
        cls.repo = Path(cls.temporary.name)
        for relative in ('tests/recorder/diff.py', 'tests/recorder/app_callbacks.inc', 'arc/tools/arccore-1.170.0-objc.tsv'):
            target = cls.repo / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT / relative, target)
        cls.before = cls.repo / 'tests/recorder/diff.before.py'
        shutil.copyfile(cls.repo / 'tests/recorder/diff.py', cls.before)
        subprocess.run(['patch', '-p1', '-i', str(ROOT / 'arc/browser/webextras_fixture_comparator.patch')],
                       cwd=cls.repo, check=True, capture_output=True)
        cls.counter = 0

    @classmethod
    def tearDownClass(cls):
        cls.temporary.cleanup()

    def compare(self, left, right, flags=(), before=False):
        type(self).counter += 1
        directory = self.repo / f'case-{self.counter}'
        directory.mkdir()
        paths = []
        for name, data in (('golden', left), ('ours', right)):
            if isinstance(data, Path):
                paths.append(data)
            else:
                path = directory / name
                path.write_text(''.join(json.dumps(dict(event, seq=index), separators=(',', ':')) + '\n'
                                        for index, event in enumerate(data)))
                paths.append(path)
        script = self.before if before else self.repo / 'tests/recorder/diff.py'
        report_path = directory / 'report.json'
        result = subprocess.run([sys.executable, '-B', str(script), *map(str, paths), '--json', str(report_path), *flags],
                                capture_output=True, text=True)
        report = json.loads(report_path.read_text()) if report_path.exists() else None
        return result, report

    def success(self, left, right, flags=()):
        result, report = self.compare(left, right, flags)
        self.assertEqual(result.returncode, 0, result.stderr)
        return report

    def test_explicit_fixture_return_difference_and_counts(self):
        report = self.success(pair(1), pair(2), ['--fixture-image', IMAGE])
        self.assertEqual(len(report['return_mismatch']), 1)
        metadata = report['fixture_comparison']
        self.assertEqual(metadata['mode'], 'exact-image')
        self.assertEqual(metadata['admitted_images'], [IMAGE])
        self.assertEqual(metadata['retained_fixture_events'], {side: {IMAGE: {'call': 1, 'ret': 1}} for side in ('golden', 'ours')})

    def test_default_excludes_fixture_and_preserves_output(self):
        left, right = pair(1), pair(2)
        baseline, old = self.compare(left, right, before=True)
        result, report = self.compare(left, right)
        self.assertEqual(result.stdout, baseline.stdout)
        self.assertEqual(report, old)
        self.assertNotIn('fixture_comparison', report)
        self.assertFalse(report['return_mismatch'])

    def test_engine_getter_excluded_but_engine_setter_retained(self):
        left = pair() + pair(1, 'engine') + pair(1, 'engine', 'setDelegate:')
        right = pair() + pair(2, 'engine') + pair(2, 'engine', 'setDelegate:')
        report = self.success(left, right, ['--fixture-image', IMAGE])
        self.assertEqual([r['method'].split()[-1] for r in report['return_mismatch']], ['setDelegate:'])
        self.assertEqual(report['fixture_comparison']['retained_fixture_events']['golden'][IMAGE]['call'], 1)

    def test_app_calls_and_callbacks_retained(self):
        left = pair() + pair(1, 'app') + pair(None, 'engine', 'didUpdate:', 'ProbeDelegate', 'engine->app', [1])
        right = pair() + pair(2, 'app') + pair(None, 'engine', 'didUpdate:', 'ProbeDelegate', 'engine->app', [2])
        report = self.success(left, right, ['--fixture-image', IMAGE])
        self.assertEqual(len(report['return_mismatch']), 1)
        self.assertEqual(len(report['arg_mismatch']), 1)
        self.assertEqual(report['callback_sequence']['golden'], 1)

    def test_exact_image_isolation_and_repeatable_opt_in(self):
        left = pair(1) + pair(1, IMAGE + '.other') + pair(1, 'probe', 'currentPanel:error:')
        right = pair(1) + pair(2, IMAGE + '.other') + pair(2, 'probe', 'currentPanel:error:')
        scoped = self.success(left, right, ['--fixture-image', IMAGE])
        self.assertFalse(scoped['return_mismatch'])
        both = self.success(left, right, ['--fixture-image', IMAGE, '--fixture-image', 'probe', '--fixture-image', IMAGE])
        self.assertEqual(len(both['return_mismatch']), 1)
        self.assertEqual(both['fixture_comparison']['admitted_images'], [IMAGE, 'probe'])
        self.assertEqual(both['fixture_comparison']['retained_fixture_events']['ours']['probe'], {'call': 1, 'ret': 1})

    def test_unknown_option_fails(self):
        for flag in ('--fixture-images', '--fixture-im', '--unknown'):
            with self.subTest(flag=flag):
                result, report = self.compare(pair(), pair(), [flag, IMAGE])
                self.assertEqual(result.returncode, 2)
                self.assertIn('unrecognized arguments', result.stderr)
                self.assertIsNone(report)

    def test_malformed_images_fail(self):
        for image in ('', '.', '..', '/tmp/probe', 'a/b', 'a\\b', '*', 'probe?', '[probe]', 'app', 'engine', ' probe', 'probe ', 'a\n', '--probe'):
            with self.subTest(image=image):
                result, report = self.compare(pair(), pair(), ['--fixture-image=' + image])
                self.assertEqual(result.returncode, 2)
                self.assertIsNone(report)

    def test_missing_mixed_and_invalid_callers_fail(self):
        for left, right in ((pair(caller=None), pair()), (pair(), pair(caller=None)),
                            (pair() + pair(caller=None), pair()),
                            (pair()[:1] + [dict(pair()[1], **{'from': ''})], pair()),
                            (pair()[:1] + [dict(pair()[1], **{'from': 3})], pair())):
            with self.subTest(left=left, right=right):
                result, report = self.compare(left, right, ['--fixture-image', IMAGE])
                self.assertEqual(result.returncode, 2)
                self.assertIn('requires caller attribution', result.stderr)
                self.assertIsNone(report)

    def test_empty_or_unattributed_fixture_transcript_fails(self):
        for rows in ([], [dict(ev='hooked', engine_classes=1)]):
            with self.subTest(rows=rows):
                result, report = self.compare(rows, pair(), ['--fixture-image', IMAGE])
                self.assertEqual(result.returncode, 2)
                self.assertIn('at least one complete admitted fixture', result.stderr)
                self.assertIsNone(report)

    def test_zero_admitted_evidence_fails(self):
        for rows in (pair(caller='other.dylib'), pair(caller='app'), pair(caller='engine')):
            with self.subTest(rows=rows):
                result, report = self.compare(rows, pair(), ['--fixture-image', IMAGE])
                self.assertEqual(result.returncode, 2)
                self.assertIn('at least one complete admitted fixture', result.stderr)
                self.assertIsNone(report)

    def test_admitted_fixture_pairs_must_match_exactly(self):
        for field, replacement in {'from':'probe', 'thread':'background', 'depth':2, 'kind':'+',
                                   'cls':'ArcMediaPosition', 'sel':'close', 'self':'<ArcWebContentsDevTools#2>'}.items():
            with self.subTest(field=field):
                rows = pair()
                rows[1][field] = replacement
                result, report = self.compare(rows, pair(), ['--fixture-image', IMAGE, '--fixture-image', 'probe'])
                self.assertEqual(result.returncode, 2)
                self.assertIsNone(report)
        for rows in (pair()[:1], pair()[1:]):
            result, report = self.compare(rows, pair(), ['--fixture-image', IMAGE])
            self.assertEqual(result.returncode, 2)
            self.assertIsNone(report)

    def test_admitted_fixture_payload_fields_are_required(self):
        for index, field in ((0, 'args'), (1, 'ret')):
            rows = pair()
            del rows[index][field]
            result, report = self.compare(rows, pair(), ['--fixture-image', IMAGE])
            self.assertEqual(result.returncode, 2)
            self.assertIsNone(report)

    def test_unrelated_ambient_tail_does_not_require_completeness(self):
        rows = pair() + pair(caller='app', selector='run')[:1]
        report = self.success(rows, rows, ['--fixture-image', IMAGE])
        self.assertFalse(report['return_mismatch'])

    def test_constructor_and_helper_classification_unchanged(self):
        constructor = pair(None, IMAGE, 'initWithSidePanel:', 'ArcBrowserSidePanel', args=['<ptr>'])
        report = self.success(constructor, [], ())
        # Both default-labelled sides are needed to enable the old caller policy.
        report = self.success(constructor + pair(), pair(), ['--fixture-image', IMAGE])
        self.assertEqual(len(report['internal']), 1)
        helpers = pair(1, IMAGE, 'webextrasPrivateHelper')
        public = pair(1, IMAGE, 'currentPanel:error:')
        report = self.success(pair(), pair() + helpers + public, ['--fixture-image', IMAGE])
        self.assertEqual(len(report['internal_ours']), 1)
        self.assertEqual(len(report['extra']), 1)
        self.assertTrue(report['extra'][0]['method'].endswith('currentPanel:error:'))

    def test_first_two_lines_unchanged_in_fixture_mode(self):
        baseline, old = self.compare(pair(1, 'app'), pair(2, 'app'), before=True)
        result, report = self.compare(pair(1), pair(2), ['--fixture-image', IMAGE])
        self.assertEqual(result.stdout.splitlines()[:2], baseline.stdout.splitlines()[:2])
        report.pop('fixture_comparison')
        self.assertEqual(report, old)

    def test_archived_legacy_diffs_unchanged(self):
        pairs = [
            ('results/webextras/values-run1/original/record.jsonl', 'results/webextras/values-run1/ours/record.jsonl'),
            ('results/golden/arc-1.170-media-pip/launch-1/record.jsonl', 'results/webextras/media-pip-run2/launch-1/record.jsonl'),
        ]
        for index, (original, ours) in enumerate(pairs):
            with self.subTest(original=original):
                sources = ROOT / original, ROOT / ours
                self.assertTrue(all(path.is_file() for path in sources), 'archived inputs are required')
                frozen = self.repo / f'archived-inputs-{index}'
                frozen.mkdir()
                left, right = frozen / 'golden.jsonl', frozen / 'ours.jsonl'
                for source, target in zip(sources, (left, right)):
                    shutil.copyfile(source, target)
                baseline, old = self.compare(left, right, before=True)
                result, report = self.compare(left, right)
                self.assertEqual(baseline.returncode, 0, baseline.stderr)
                self.assertEqual(result.returncode, 0, result.stderr)
                self.assertEqual(result.stdout, baseline.stdout)
                self.assertEqual(report, old)


if __name__ == '__main__':
    unittest.main(verbosity=2)
