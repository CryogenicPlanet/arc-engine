#!/usr/bin/env python3
"""Extracted wrapper CLI/snapshot tests in temp trees; never takes a lock or runs apps/builds."""
import json
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
SOURCE = (ROOT/'arc/browser/webextras_native_current_run.sh').read_text()


class WrapperTests(unittest.TestCase):
    def test_strict_cli_default_clock_forwarding_and_rejections(self):
        # Actual parser and re-exec body; shell function replaces exec before it can
        # invoke locked.sh. There is no lock command/process in this test.
        parser = SOURCE[SOURCE.index('locked=0'):SOURCE.index('python3 - "$ROOT" <<')]
        shell = 'set -euo pipefail\nROOT=/unused\nexec(){ printf "EXEC<%s>\\n" "$@"; }\n' + parser + '\nprintf "STATE:%s:%s\\n" "$run_number" "$media_clock"\n'
        cases = [([], 'STATE:4:0', False), (['--run-number', '6', '--media-clock'], 'STATE:6:1', True),
                 (['--media-clock', '--run-number', '6'], 'STATE:6:1', True),
                 (['--locked', '--run-number', '6', '--media-clock'], 'STATE:6:1', True)]
        for args, expected, clock in cases:
            result = subprocess.run(['/bin/bash', '-c', shell, 'wrapper', *args], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            self.assertIn(expected, result.stdout)
            if '--locked' not in args:
                self.assertIn('EXEC</unused/arc/tools/locked.sh>', result.stdout)
                self.assertEqual('EXEC<--media-clock>' in result.stdout, clock)
        for args in (['--run-number'], ['--run-number','0'], ['--run-number','-1'], ['--run-number','6','--run-number','7'],
                     ['--media-clock','--media-clock'], ['--unknown'], ['--media-clock','true'], ['--locked','--locked']):
            result = subprocess.run(['/bin/bash', '-c', shell, 'wrapper', *args], capture_output=True, text=True)
            self.assertEqual(result.returncode, 2, (args, result.stderr))
            self.assertNotIn('EXEC<', result.stdout)

    def test_actual_snapshot_default_inventory_clock_assets_mode_and_tamper(self):
        body = re.search(r'python3 - "\$ROOT" "\$stem" "\$1" "\$media_clock" <<\'PY\'\n(.*?)\nPY\n', SOURCE, re.S).group(1)
        with tempfile.TemporaryDirectory(prefix='webextras-wrapper-snapshot-') as directory:
            root = Path(directory)
            paths = list(ROOT.glob('arc/api/arc_webextras_*.mm')) + [ROOT/path for path in (
                'arc/browser/webextras_media.h','arc/browser/webextras_values.h','arc/wp/webextras.gni',
                'arc/browser/webextras_native_probe.m','arc/browser/webextras_native_probe.ts','arc/browser/webextras_native_probe.sh',
                'arc/browser/webextras_fixture_inputs.py','arc/browser/webextras_fixture_projection.ts','arc/browser/webextras_native_current_run.sh',
                'arc/browser/webextras_media_clock_fixture.webm','arc/browser/webextras_media_clock_fixture.json')]
            for source in paths:
                target = root/source.relative_to(ROOT); target.parent.mkdir(parents=True, exist_ok=True); shutil.copyfile(source, target)
            def snapshot(stem, phase, mode):
                return subprocess.run(['python3','-',str(root),str(root/stem),phase,str(mode)], input=body, capture_output=True, text=True)
            for mode in (0, 1):
                stem = 'mode'+str(mode)
                self.assertEqual(snapshot(stem,'before',mode).returncode,0)
                self.assertEqual(snapshot(stem,'after-build',mode).returncode,0)
                state=json.loads((root/(stem+'.source-before.json')).read_text())
                self.assertEqual(state['probeMode'], 'media-clock' if mode else 'default')
                self.assertEqual(len(state['files']), 16 if mode else 14)
                self.assertEqual('arc/browser/webextras_media_clock_fixture.webm' in state['files'], bool(mode))
                self.assertNotEqual(snapshot(stem,'after-mode-change',1-mode).returncode,0)
            fixture=root/'arc/browser/webextras_media_clock_fixture.webm'
            fixture.write_bytes(fixture.read_bytes()+b'tampered')
            self.assertEqual(snapshot('mode0','after-fixture-tamper',0).returncode,0)
            self.assertNotEqual(snapshot('mode1','after-fixture-tamper',1).returncode,0)
            self.assertEqual(snapshot('json-only','before',1).returncode,0)
            metadata=root/'arc/browser/webextras_media_clock_fixture.json'
            metadata.write_bytes(metadata.read_bytes()+b'\n')
            self.assertNotEqual(snapshot('json-only','after-json-tamper',1).returncode,0)
            self.assertEqual(snapshot('mode0','after-json-tamper',0).returncode,0)
            source=root/'arc/browser/webextras_native_probe.ts';source.write_bytes(source.read_bytes()+b'\n//tamper\n')
            self.assertNotEqual(snapshot('mode0','after-source-tamper',0).returncode,0)

    def test_default_namespace_and_same_slot_pipeline_are_preserved(self):
        self.assertIn('run_number=4', SOURCE)
        self.assertIn('Refusing prior native-run$run_number artifacts', SOURCE)
        steps = ['source_snapshot before', 'python3 arc/tools/gen_api_table.py', 'ARC_BUILD_FAIL_FAST=1 ./build-babysit.sh',
                 './package-engine.sh ours156', 'bash arc/browser/webextras_native_probe.sh build', 'source_snapshot after-build',
                 'bash arc/browser/webextras_native_probe.sh run --out "$output" ${clock_args[@]+"${clock_args[@]}"}', 'source_snapshot after-run']
        offsets = [SOURCE.index(step) for step in steps]
        self.assertEqual(offsets, sorted(offsets))
        self.assertIn("assert ancestor == owner, '--locked requires an ancestor holding the shared lock'", SOURCE)
        result = subprocess.run(['/bin/bash','-n',str(ROOT/'arc/browser/webextras_native_current_run.sh')], capture_output=True,text=True)
        self.assertEqual(result.returncode,0,result.stderr)


if __name__ == '__main__':
    unittest.main(verbosity=2)
