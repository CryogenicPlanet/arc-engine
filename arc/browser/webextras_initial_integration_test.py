#!/usr/bin/env python3
"""Private temporary-root safety tests; no real lock, build, app or profile."""
import copy
import difflib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
from unittest.mock import patch

SPEC = importlib.util.spec_from_file_location('integration', Path(__file__).with_name('webextras_initial_integration.py'))
M = importlib.util.module_from_spec(SPEC); SPEC.loader.exec_module(M)


class Held(Exception):
    pass


class FakeBackend:
    def __init__(self, root, output, blobs):
        self.root, self.output, self.blobs = root, output, blobs
        self.groups = set(); self.commands = []; self.regen_failure = False

    def inventory(self):
        return [dict(pid=os.getpid(), ppid=os.getppid(), pgid=999, state='S', command='private-test')]

    def command(self, argv, label, cwd=None, env=None):
        self.commands.append((argv, label, env))
        (self.output / (label + '.log')).write_text('private mock command\n')
        if argv[0] == 'git' and argv[1] in ('init', 'apply'):
            return subprocess.run(argv, cwd=cwd, env=env, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE).returncode
        if argv[0] == 'git' and argv[1:3] == ['add', '-N']:
            assert env['GIT_INDEX_FILE'] != str(self.root / 'chromium/src/.git/index')
            Path(env['GIT_INDEX_FILE']).write_bytes(b'PRIVATE INDEX WITH OWN ITA\n')
            return 0
        if argv == ['reviewed-regen']:
            after = (self.root / 'arc/api/test.mm').read_text() == 'new\n'
            for name, values in self.blobs.items():
                value = values[1 if after else 0]
                if value is not None:
                    (self.root / name).write_bytes(value)
            if self.regen_failure and after:
                return 1
        return 0


class PrivateTransaction(M.Transaction):
    def hold(self, reason):
        raise Held(str(reason))

    def scenario(self, name, number, recovery=False):
        self.file_state('before' if recovery else 'after')
        self.seen.append((name, number, recovery))
        if not recovery and name == self.fail_scenario:
            raise RuntimeError('private scenario failure')
        return {'scenario': name, 'allStepsPass': True, 'noEarlierStepRegression': True, 'boundaryParity': 'NOT ASSERTED'}


class Fixture(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory(prefix='webextras-integration-test-')
        self.root = Path(self.temporary.name) / 'checkout'; self.root.mkdir()
        self.output = self.root / 'evidence'; self.output.mkdir()
        self.old_root = M.ROOT; M.ROOT = self.root; M.CANCELLED = False
        self.head = patch.object(M.subprocess, 'check_output', side_effect=lambda argv, **_: (
            '.git/index\n' if '--git-path' in argv else 'private-head\n')); self.head.start()
        self.home = patch.object(M.Path, 'home', return_value=Path(self.temporary.name) / 'home'); self.home.start()
        self.config = M.template(); self.config.update(status='SEALED', repo=str(self.root),
            acknowledgments={'hostGate': 'private-test-reviewed-gate', 'regenerationInterface': 'private-test-interface'})
        self.config['pins'] = {}; self.config['baselineFiles'] = {}; self.config['targets'] = {}
        self.images = {'arc/api/test.mm': (b'old\n', b'new\n'),
                       'chromium/src/native.cc': (b'old native\n', b'new native\n'),
                       'chromium/src/new.h': (None, b'new header\n')}
        for name, values in self.images.items():
            if values[0] is not None: self.put(name, values[0])
            blob = 'private-images/' + name
            self.put(blob, values[1]); self.config['pins'][blob] = M.sha(self.root / blob)
            self.config['targets'][name] = {'before': M.sha(self.root / name), 'after': M.sha(self.root / blob), 'blob': blob}
        before = b'0004-arcmod-hooks.patch native.cc\n'
        after = before + b'0010-webextras.patch new.h\n'
        self.put('patches/MANIFEST', before); self.put('private-images/MANIFEST', after)
        self.config['manifest'] = {'before': M.sha(self.root / 'patches/MANIFEST'),
            'after': M.sha(self.root / 'private-images/MANIFEST'), 'blob': 'private-images/MANIFEST'}
        self.config['pins']['private-images/MANIFEST'] = self.config['manifest']['after']
        self.outputs = {'patches/0004-arcmod-hooks.patch': (b'old patch\n', b'new patch\n'),
                        'patches/0010-webextras.patch': (None, b'new own patch\n')}
        self.config['patchOutputs'] = {}
        for name, values in self.outputs.items():
            if values[0] is not None: self.put(name, values[0])
            self.config['patchOutputs'][name] = {
                'before': M.sha(self.root / name), 'after': __import__('hashlib').sha256(values[1]).hexdigest()}
        self.config['patches'] = []
        for cwd, names in [('.', ['arc/api/test.mm']), ('chromium/src', ['native.cc', 'new.h'])]:
            text = ''
            for name in names:
                key = name if cwd == '.' else cwd + '/' + name
                old, new = self.images[key]
                text += ''.join(difflib.unified_diff((old or b'').decode().splitlines(True), new.decode().splitlines(True),
                    fromfile='a/' + name if old is not None else '/dev/null', tofile='b/' + name))
            path = 'proposals/own.patch' if cwd == '.' else 'proposals/native.patch'
            self.put(path, text.encode()); digest = M.sha(self.root / path); self.config['pins'][path] = digest
            self.config['patches'].append({'path': path, 'sha256': digest, 'cwd': cwd})
        for path in ('arc/tools/locked.sh', 'arc/browser/webextras_initial_integration.py',
                     'arc/browser/webextras_scenario_current_run.py', 'private-regen-tool', 'arc/BUILD.gn',
                     'arc/tools/gen_api_table.py', 'build-babysit.sh', 'package-engine.sh', 'chromium/src/out/Base/args.gn'):
            self.put(path, b'private mock only\n'); self.config['pins'][path] = M.sha(self.root / path)
        self.put('chromium/src/.git/index', b'PRIVATE INDEX\n')
        self.config['gitRepos'] = [{'path': 'chromium/src', 'index': 'chromium/src/.git/index',
            'sha256': M.sha(self.root / 'chromium/src/.git/index'), 'head': 'private-head',
            'environmentKey': 'GIT_INDEX_FILE', 'intentToAdd': ['new.h']}]
        self.config['regeneration'] = {'argv': ['reviewed-regen'], 'cwd': '.', 'env': {}}
        self.config['inventory'] = {'arc/api': ['arc/api/test.mm'], 'arc/browser': [], 'arc/wp': []}
        (self.root / 'arc/wp').mkdir()
        self.config['runNumbers'] = {k: i + 2 for i, k in enumerate((*M.SCENARIOS, 'recovery-launch-slice'))}
        for scenario in M.SCENARIOS:
            for n in ('steps.json', 'coverage.json', 'runner-summary.json', 'recovery.json', 'input-manifest.json',
                      'binary-identity.json', 'binary-identity-after-run.json'):
                name = f'results/webextras/{scenario}-current-run1/{n}'
                self.put(name, b'{}\n'); self.config['baselineFiles'][name] = M.sha(self.root / name)
            for name, value in [('steps.json', {'scenario': scenario, 'steps': [{'step': 'private', 'outcome': 'pass'}]}),
                                ('coverage.json', {'accepted': True}), ('launch-1/record.jsonl', {'ev': 'private'})]:
                name = f'results/golden/arc-1.170-{scenario}/' + name
                self.put(name, (json.dumps(value) + '\n').encode()); self.config['pins'][name] = M.sha(self.root / name)
        self.put('logs/arccore-build.lock/pid', str(os.getpid()).encode())
        M.write(self.output / 'protected.json', {'pid': os.getpid()})
        self.backend = FakeBackend(self.root, self.output, self.outputs)

    def put(self, name, data):
        path = self.root / name; path.parent.mkdir(parents=True, exist_ok=True); path.write_bytes(data)

    def tx(self):
        M.write(self.output / 'config.json', self.config)
        tx = PrivateTransaction(copy.deepcopy(self.config), self.output, self.backend)
        tx.seen = []; tx.fail_scenario = None
        return tx

    def tearDown(self):
        self.head.stop(); self.home.stop(); M.ROOT = self.old_root; M.CANCELLED = False; self.temporary.cleanup()

    def scenario_fixture(self, scenario='launch-slice', known_failure=False):
        output = self.root / f'results/webextras/{scenario}-current-run2'; output.mkdir(parents=True)
        baseline = self.root / f'results/webextras/{scenario}-current-run1'
        app = self.root.parent / 'arc/build/ArcModAgent-ours156.app'
        names = list(M.LAUNCH) if scenario == 'launch-slice' else ['launch', 'scenario actions', 'quit'] * (2 if scenario == 'devtools' else 1)
        rows = [{'step': n, 'outcome': 'inconclusive' if known_failure and n == 'scenario actions' else 'pass',
                 'detail': ('Browser.close; browser + 3 helpers exited' if 'quit' in n.lower()
                            else 'Error: expected one native control; observed 0' if known_failure and n == 'scenario actions' else 'ok')}
                for n in names]
        launches = ['launch-1', 'launch-2'] if scenario in ('devtools', 'launch-slice') else ['launch-1']
        steps = {'scenario': scenario, 'slug': 'ours156', 'recorded': True, 'app': str(app), 'steps': rows}
        coverage = {'scenario': scenario, 'accepted': not known_failure, 'errors': [], 'launches': launches}
        summary = {'scenario': scenario, 'runNumber': '2', 'errors': [], 'profileRestored': True,
                   'harnessExit': int(known_failure), 'runtimeAccepted': not known_failure,
                   'comparisons': [{'launch': x, 'clean': False} for x in launches], 'accepted': False}
        for n, value in [('steps.json', steps), ('coverage.json', coverage), ('runner-summary.json', summary)]:
            M.write(output / n, value); M.write(baseline / n, value)
        golden_rows = copy.deepcopy(rows)
        if scenario == 'devtools':
            golden_rows = [dict(step=n, outcome='pass') for n in ['launch', 'native developer frontend opens',
                'native developer frontend closes', 'quit', 'launch', 'inspect opens native developer frontend', 'quit']]
        M.write(self.root / f'results/golden/arc-1.170-{scenario}/steps.json', {'scenario': scenario, 'steps': golden_rows})
        M.write(self.root / f'results/golden/arc-1.170-{scenario}/coverage.json', {'accepted': True})
        profile = M.Path.home() / 'Library/Application Support/ArcModAgent-ours156'
        M.write(output / 'recovery.json', {'profile': str(profile), 'originalExisted': False,
                                         'remainingProcesses': [], 'status': 'restored-original-absence'})
        binary_paths = [app / 'Contents/MacOS/Arc', app / 'Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore',
                        self.root / 'tests/recorder/ArcEngineRecorder.dylib',
                        app / 'Contents/Frameworks/ArcCore.framework/Helpers/Private Helper.app/Contents/MacOS/Helper']
        identities = []
        for p in binary_paths:
            p.parent.mkdir(parents=True, exist_ok=True); p.write_bytes(b'private binary fixture\n')
            value = p.stat(); identities.append({'path': str(p), 'realpath': str(p.resolve()), 'sha256': M.sha(p),
                'dev': value.st_dev, 'ino': value.st_ino, 'size': value.st_size,
                'mtimeNs': value.st_mtime_ns, 'ctimeNs': value.st_ctime_ns})
        binaries = dict(zip(('main', 'engine', 'recorder', 'helpers'), identities[:3] + [[identities[3]]]))
        M.write(output / 'binary-identity.json', binaries); M.write(output / 'binary-identity-after-run.json', binaries)
        seed = self.root / 'seed'; seed.write_text('private input')
        frozen = output / 'inputs/seed'; frozen.parent.mkdir(); frozen.write_bytes(seed.read_bytes())
        manifest = {str(seed): {'source': {'sha256': M.sha(seed)}, 'frozen': {'path': str(frozen)}}}
        M.write(output / 'input-manifest.json', {'files': manifest})
        phases = ['before', 'after-build', 'before-run', 'after-run', 'final']
        for launch in launches:
            p = output / launch; p.mkdir(); (p / 'app.log').write_text('ordinary private app fixture\n')
            (p / 'record.jsonl').write_text(json.dumps({'ev': 'hooked', 'engine_classes': 2}) + '\n' +
                                          json.dumps({'ev': 'call', 'cls': 'ArcExample'}) + '\n')
            report = {k: [{'ordinaryGlobalDifference': True}] for k in M.SECTIONS}
            report['callback_sequence'] = {'first_divergence': {'ordinaryGlobalDifference': True}}
            M.write(output / ('diff-' + launch + '.json'), report)
            (output / ('diff-' + launch + '.txt')).write_text('dirty global comparator\nnot parity\n')
            phases += [f'{position}-diff-{launch}' for position in ('before', 'after')]
        stem = self.root / f'logs/wp/webextras.{scenario}.current-run2'
        for phase in phases: M.write(Path(str(stem) + '.source-' + phase + '.json'), {'phase': phase, 'files': manifest})
        return output, baseline

    def test_default_unsealed_refuses_without_operation(self):
        with self.assertRaisesRegex(RuntimeError, 'UNSEALED'):
            M.validate(M.template(), self.root)
        self.assertEqual(self.backend.commands, [])

    def test_complete_private_success_and_alternate_index(self):
        tx = self.tx(); self.assertEqual(tx.execute(), 0)
        self.assertEqual([r[0] for r in tx.seen], list(M.SCENARIOS))
        self.assertEqual(M.sha(self.root / 'chromium/src/.git/index'), self.config['gitRepos'][0]['sha256'])
        regeneration = [e for e in self.backend.commands if e[0] == ['reviewed-regen']]
        self.assertEqual(len(regeneration), 1)
        self.assertNotEqual(regeneration[0][2]['GIT_INDEX_FILE'], str(self.root / 'chromium/src/.git/index'))

    def test_candidate_failure_restores_absence_then_recovers_package(self):
        tx = self.tx(); tx.fail_scenario = 'devtools'; self.assertEqual(tx.execute(), 1)
        for name, values in self.images.items():
            self.assertEqual((self.root / name).read_bytes() if values[0] is not None else None, values[0])
            if values[0] is None: self.assertFalse((self.root / name).exists())
        self.assertFalse((self.root / 'patches/0010-webextras.patch').exists())
        self.assertEqual(tx.seen[-1], ('launch-slice', 7, True))
        self.assertEqual(json.loads((self.output / 'completion.json').read_text())['status'], 'ROLLED_BACK')
        self.assertEqual(Path(tx.indexes['GIT_INDEX_FILE']).read_bytes(), b'PRIVATE INDEX\n')

    def test_regen_failure_rolls_back_not_release_candidate(self):
        self.backend.regen_failure = True; tx = self.tx(); self.assertEqual(tx.execute(), 1)
        self.assertEqual(tx.seen, [('launch-slice', 7, True)])

    def test_foreign_target_causes_hold_without_clobber(self):
        tx = self.tx(); tx.prepare(); tx.publish(); tx.regenerate()
        self.put('arc/api/test.mm', b'foreign\n')
        with self.assertRaisesRegex(RuntimeError, 'foreign target'): tx.rollback()
        self.assertEqual((self.root / 'arc/api/test.mm').read_bytes(), b'foreign\n')

    def test_foreign_patch_causes_hold_without_clobber(self):
        tx = self.tx(); tx.prepare(); tx.publish(); tx.regenerate()
        self.put('patches/0004-arcmod-hooks.patch', b'foreign patch\n')
        with self.assertRaisesRegex(RuntimeError, 'foreign patch'): tx.rollback()
        self.assertEqual((self.root / 'patches/0004-arcmod-hooks.patch').read_bytes(), b'foreign patch\n')

    def test_real_index_drift_preserved_even_if_metadata_only(self):
        tx = self.tx(); tx.prepare(); tx.publish(); tx.regenerate()
        self.put('chromium/src/.git/index', b'FOREIGN INDEX METADATA\n')
        with self.assertRaisesRegex(RuntimeError, 'index drift'): tx.rollback()
        self.assertEqual((self.root / 'chromium/src/.git/index').read_bytes(), b'FOREIGN INDEX METADATA\n')

    def test_cancel_stops_next_phase_after_drain_and_rolls_back(self):
        original = self.backend.command
        def command(*args, **kwargs):
            result = original(*args, **kwargs)
            if args[0] == ['reviewed-regen'] and not (self.root / 'arc/api/test.mm').read_text().startswith('old'):
                (self.output / 'cancel.request').touch()
            return result
        self.backend.command = command
        tx = self.tx(); self.assertEqual(tx.execute(), 1)
        self.assertEqual(tx.seen, [('launch-slice', 7, True)])

    def test_partial_publication_journal_can_restore(self):
        tx = self.tx(); tx.prepare(); tx.mutated = True
        name = 'arc/api/test.mm'; M.atomic_bytes(self.root / name, b'new\n')
        tx.rollback(); self.assertEqual((self.root / name).read_bytes(), b'old\n')

    def test_wrong_ack_duplicate_run_or_missing_pin_refused(self):
        for mutate in (lambda c: c['acknowledgments'].update(hostGate=''),
                       lambda c: c['runNumbers'].update(devtools=2),
                       lambda c: c['pins'].pop('arc/tools/locked.sh')):
            config = copy.deepcopy(self.config); mutate(config)
            with self.assertRaises(RuntimeError): M.validate(config, self.root)

    def test_private_reconstruction_rejects_substituted_blob(self):
        self.put('private-images/arc/api/test.mm', b'different\n')
        value = M.sha(self.root / 'private-images/arc/api/test.mm')
        self.config['pins']['private-images/arc/api/test.mm'] = value
        self.config['targets']['arc/api/test.mm']['after'] = value
        with self.assertRaisesRegex(RuntimeError, 'do not reconstruct'): self.tx().prepare()
        self.assertEqual((self.root / 'arc/api/test.mm').read_bytes(), b'old\n')

    def test_child_receipt_write_failure_still_drains_group(self):
        class Child:
            pid = 12345; returncode = 0
            def wait(self): waits.append('wait'); return 0
        waits = []; census = iter([[dict(pgid=12345, state='S')], []])
        backend = M.Backend(self.root, self.output, self.output / 'cancel')
        backend.inventory = lambda: next(census)
        with patch.object(M.subprocess, 'Popen', return_value=Child()), \
             patch.object(M, 'write', side_effect=OSError('disk')), patch.object(M.time, 'sleep'):
            with self.assertRaises(OSError): backend.command(['private-placeholder'], 'receipt-failure')
        self.assertEqual(waits, ['wait', 'wait'])

    def test_launch_gate_accepts_dirty_global_diff_but_not_parity(self):
        output, baseline = self.scenario_fixture()
        result = M.scenario_gate(self.root, output, 'launch-slice', baseline, 1)
        self.assertTrue(result['allStepsPass']); self.assertIn('NOT ASSERTED', result['boundaryParity'])

    def test_devtools_exact_old_inconclusive_allowed(self):
        output, baseline = self.scenario_fixture('devtools', True)
        self.assertFalse(M.scenario_gate(self.root, output, 'devtools', baseline, 1)['allStepsPass'])
        value = json.loads((output / 'steps.json').read_text()); value['steps'][1]['detail'] = 'new different error'
        M.write(output / 'steps.json', value)
        with self.assertRaisesRegex(RuntimeError, 'step regression'):
            M.scenario_gate(self.root, output, 'devtools', baseline, 1)

    def test_devtools_full_improvement_allowed(self):
        output, baseline = self.scenario_fixture('devtools', True)
        value = json.loads((output / 'steps.json').read_text())
        golden = json.loads((self.root / 'results/golden/arc-1.170-devtools/steps.json').read_text())
        value['steps'] = [dict(step=x['step'], outcome='pass', detail=(
            'Browser.close; browser + 3 helpers exited' if x['step'] == 'quit' else 'ok')) for x in golden['steps']]
        M.write(output / 'steps.json', value)
        value = json.loads((output / 'runner-summary.json').read_text()); value.update(harnessExit=0, runtimeAccepted=True)
        M.write(output / 'runner-summary.json', value)
        value = json.loads((output / 'coverage.json').read_text()); value['accepted'] = True; M.write(output / 'coverage.json', value)
        self.assertTrue(M.scenario_gate(self.root, output, 'devtools', baseline, 1)['allStepsPass'])

    def test_gate_rejects_false_zero_missing_or_mutated_evidence(self):
        output, baseline = self.scenario_fixture()
        for name, mutation in [('coverage.json', lambda d: d.update(accepted=False)),
                               ('runner-summary.json', lambda d: d.update(profileRestored=False)),
                               ('steps.json', lambda d: d['steps'].pop()),
                               ('binary-identity-after-run.json', lambda d: d['engine'].update(sha256='0'*64)),
                               ('diff-launch-1.json', lambda d: d.pop('callback_sequence'))]:
            path = output / name; original = path.read_bytes(); value = json.loads(original); mutation(value); M.write(path, value)
            with self.assertRaises(RuntimeError, msg=name): M.scenario_gate(self.root, output, 'launch-slice', baseline, 1)
            path.write_bytes(original)
        (output / 'launch-1/record.jsonl').write_text('')
        with self.assertRaisesRegex(RuntimeError, 'uninstrumented'):
            M.scenario_gate(self.root, output, 'launch-slice', baseline, 1)

    def test_actual_binary_and_source_phase_drift_rejected(self):
        output, baseline = self.scenario_fixture()
        value = json.loads((output / 'binary-identity.json').read_text()); engine = Path(value['engine']['path'])
        engine.write_text('unexpected replacement')
        with self.assertRaisesRegex(RuntimeError, 'actual packaged binary drift'):
            M.scenario_gate(self.root, output, 'launch-slice', baseline, 1)

    def test_protected_owner_ignores_signals_before_exec(self):
        config = self.root / 'sealed.json'; M.write(config, self.config)
        M.write(self.output / 'launch.json', {'configSHA256': M.sha(config)})
        handlers = {s: M.signal.getsignal(s) for s in M.SIGNALS}; calls = []
        def execute(*args):
            self.assertTrue(all(M.signal.getsignal(s) == M.signal.SIG_IGN for s in M.SIGNALS)); calls.append(args)
            raise Held('private exec interception')
        try:
            with patch.object(M.sys, 'argv', ['executor', '_protected', '--config', str(config), '--config-sha', M.sha(config), '--out', str(self.output)]), \
                 patch.object(M.os, 'execv', side_effect=execute):
                with self.assertRaises(Held): M.main()
        finally:
            for s, handler in handlers.items(): M.signal.signal(s, handler)
        self.assertEqual(calls[0][1][1], str(self.root / 'arc/tools/locked.sh'))

    def test_profile_unknown_blocks_rollback_before_source_writes(self):
        tx = self.tx(); tx.prepare(); tx.publish(); tx.regenerate()
        M.write(self.output / 'private.child.json', {'pgid': 1234, 'argv': ['python3',
            str(self.root / 'arc/browser/webextras_scenario_current_run.py'), '--scenario', 'media', '--run-number', '4']})
        with self.assertRaisesRegex(RuntimeError, 'profile restoration unconfirmed'): tx.rollback()
        self.assertEqual((self.root / 'arc/api/test.mm').read_bytes(), b'new\n')

    def test_helper_survivor_prevents_rollback(self):
        tx = self.tx(); tx.prepare(); tx.publish(); tx.regenerate()
        row = dict(pid=123, ppid=1, pgid=999, state='S', command=str(self.root.parent /
                   'arc/build/ArcModAgent-ours156.app/Contents/Frameworks/ArcCore.framework/Helpers/Helper'))
        self.backend.inventory = lambda: [row]
        with self.assertRaisesRegex(RuntimeError, 'survivors'): tx.rollback()
        self.assertEqual((self.root / 'arc/api/test.mm').read_bytes(), b'new\n')

    def test_cancel_handler_does_not_raise_on_disk_failure(self):
        M.cancel_handler(self.root / 'nonexistent-parent/cancel')(15, None)
        self.assertTrue(M.CANCELLED)

    def test_guardian_holds_dead_worker_without_safe_completion(self):
        self.tx()  # Writes config evidence, without executing the transaction.
        config = self.root / 'sealed.json'; M.write(config, self.config)
        M.write(self.output / 'launch.json', {'configSHA256': M.sha(config), 'nonce': 'private-nonce'})
        class Child:
            pid = 123456; returncode = -9
            def wait(self): return self.returncode
        argv = ['executor', '_guardian', '--config', str(config), '--config-sha', M.sha(config), '--out', str(self.output)]
        with patch.object(M.sys, 'argv', argv), patch.object(M.subprocess, 'Popen', return_value=Child()), \
             patch.object(M, 'process_inventory', side_effect=self.backend.inventory), patch.object(M, 'Transaction', PrivateTransaction):
            with self.assertRaisesRegex(Held, 'guardian cannot certify release'): M.main()

    def test_unknown_cli_flags_rejected(self):
        with patch.object(M.sys, 'argv', ['executor', 'template', '--unknown']):
            with self.assertRaises(SystemExit) as caught: M.main()
        self.assertEqual(caught.exception.code, 2)

    def test_unjournalled_patch_output_rejected_after_regeneration(self):
        tx = self.tx(); tx.prepare(); tx.publish(); tx.regenerate()
        self.put('patches/foreign-new.patch', b'foreign\n')
        with self.assertRaisesRegex(RuntimeError, 'inventory drift'): tx.file_state('after')
        with self.assertRaisesRegex(RuntimeError, 'unjournalled'): tx.rollback()
        self.assertEqual((self.root / 'patches/foreign-new.patch').read_bytes(), b'foreign\n')

    def test_manifest_owner_without_output_journal_rejected(self):
        self.config['patchOutputs'].pop('patches/0010-webextras.patch')
        with self.assertRaisesRegex(RuntimeError, 'lacks journalled'): self.tx().prepare()

    def test_owned_new_header_allowed_but_foreign_new_header_rejected(self):
        name = 'arc/browser/new_peer.h'; blob = 'private-images/' + name
        self.put(blob, b'private new header\n')
        image = {'before': None, 'after': M.sha(self.root / blob), 'blob': blob}
        self.config['targets'][name] = image; self.config['pins'][blob] = image['after']
        tx = self.tx(); tx.pin_check()
        self.put(name, b'private new header\n'); tx.pin_check()
        self.put('arc/browser/foreign_peer.h', b'foreign new header\n')
        with self.assertRaisesRegex(RuntimeError, 'production inventory drift'): tx.pin_check()

    def test_standard_fatal_and_crash_markers_block_runtime_gate(self):
        output, baseline = self.scenario_fixture()
        path = output / 'launch-1/app.log'
        for marker in ['[123:456:20261023:FATAL:source.cc(20)] fatal without Check',
                       '[123:456:DFATAL:source.cc(20)] fatal', 'Check failed: value',
                       'Assertion failed: value', 'SIGABRT', 'SIGSEGV', 'Received signal 11',
                       'Thread 4 Crashed:', 'EXC_BAD_ACCESS', 'EXC_BAD_INSTRUCTION', 'Fatal error: stop']:
            path.write_text(marker + '\n')
            with self.assertRaisesRegex(RuntimeError, 'fatal app marker', msg=marker):
                M.scenario_gate(self.root, output, 'launch-slice', baseline, 1)

    def test_duplicate_or_reordered_coverage_launches_rejected(self):
        output, baseline = self.scenario_fixture()
        path = output / 'coverage.json'; value = json.loads(path.read_text())
        for launches in [['launch-1', 'launch-2', 'launch-2'], ['launch-2', 'launch-1']]:
            value['launches'] = launches; M.write(path, value)
            with self.assertRaisesRegex(RuntimeError, 'canonical launch set'):
                M.scenario_gate(self.root, output, 'launch-slice', baseline, 1)


if __name__ == '__main__':
    unittest.main()
