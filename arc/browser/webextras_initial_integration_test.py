#!/usr/bin/env python3
"""Private temporary-root safety tests; no real lock, build, app or profile."""
import copy
import difflib
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import types
import unittest
from unittest.mock import patch

MODULE_PATH = Path(__file__).with_name('webextras_initial_integration.py')
M = types.ModuleType('integration'); M.__file__ = str(MODULE_PATH)
exec(compile(MODULE_PATH.read_bytes(), str(MODULE_PATH), 'exec'), M.__dict__)


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
        if M.RESOURCE_CHECKER in argv:
            directory = Path(argv[argv.index('--output') + 1]); directory.mkdir(parents=True)
            M.write(directory / 'receipt.json', self.resource_receipt)
            return 0
        if argv[0] == 'git' and argv[1] in ('init', 'apply'):
            return subprocess.run(argv, cwd=cwd, env=env, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE).returncode
        if argv == ['./arc/tools/regen-patches.sh']:
            assert not any(k.startswith('GIT_') and k not in ('GIT_OPTIONAL_LOCKS', 'GIT_CONFIG_GLOBAL', 'GIT_CONFIG_NOSYSTEM') for k in env)
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
        self.old_node = M.NODE; M.NODE = self.root / 'private-node'
        self.old_python = M.PYTHON; M.PYTHON = self.root / 'private-python'
        self.old_policy = M.RESOURCE_POLICY; M.RESOURCE_POLICY = {}
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
            self.config['targets'][name] = {'before': M.sha(self.root / name), 'after': M.sha(self.root / blob), 'blob': blob,
                'beforeMode': 0o644 if values[0] is not None else None, 'afterMode': 0o644}
        before = b'0004-arcmod-hooks.patch native.cc\n'
        after = before + b'0010-webextras.patch new.h\n'
        self.put('patches/MANIFEST', before); self.put('private-images/MANIFEST', after)
        self.config['manifest'] = {'before': M.sha(self.root / 'patches/MANIFEST'),
            'after': M.sha(self.root / 'private-images/MANIFEST'), 'blob': 'private-images/MANIFEST',
            'beforeMode': 0o644, 'afterMode': 0o644}
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
                     'arc/tools/gen_api_table.py', 'build-babysit.sh', 'package-engine.sh', 'chromium/src/out/Base/args.gn',
                     'arc/browser/webextras_frontend_resource_checked.py', 'arc/browser/webextras_frontend_resource_checked_test.py',
                     'chromium/src/tools/grit/grit/format/data_pack.py', 'chromium/src/tools/grit/grit/constants.py', 'private-node', 'private-python'):
            self.put(path, b'private mock only\n'); self.config['pins'][path] = M.sha(self.root / path)
        for path in self.old_policy:
            if path not in self.config['pins']:
                self.put(path, b'private policy fixture\n'); self.config['pins'][path] = M.sha(self.root / path)
        M.RESOURCE_POLICY = {n: self.config['pins'][n] for n in self.old_policy}
        self.config['pins'][str(M.NODE)] = self.config['pins'].pop('private-node')
        self.config['pins'][str(M.PYTHON)] = self.config['pins'].pop('private-python')
        runner = 'arc/browser/webextras_scenario_current_run.py'
        blob = 'private-images/observation.py'; self.put(blob, b'new private observation only\n')
        proposal = 'proposals/observation.patch'
        self.put(proposal, ''.join(difflib.unified_diff(['private mock only\n'], ['new private observation only\n'],
                 fromfile='a/' + runner, tofile='b/' + runner)).encode())
        self.config['observationTool'] = {'before': self.config['pins'].pop(runner), 'after': M.sha(self.root / blob),
                                          'blob': blob, 'patch': proposal}
        self.config['pins'][blob] = M.sha(self.root / blob); self.config['pins'][proposal] = M.sha(self.root / proposal)
        self.config['resourceChecks'] = {role: {'argv': [str(M.PYTHON), '-B', M.RESOURCE_CHECKER,
            '--expect-host-gate', mode, '--output', '{resource_output}'], 'cwd': '.', 'expectHostGate': mode}
            for role, mode in [('candidate', 'present'), ('recovery', 'absent')]}
        self.put('chromium/src/.git/index', b'PRIVATE INDEX\n')
        self.config['gitRepos'] = [{'path': 'chromium/src', 'index': 'chromium/src/.git/index',
            'sha256': M.sha(self.root / 'chromium/src/.git/index'), 'head': 'private-head'}]
        for name, mode in M.REGEN_TOOLS.items():
            self.put(name, b'private already-adopted generator fixture\n'); (self.root/name).chmod(mode)
            self.config['pins'][name] = M.sha(self.root/name)
        self.config['regeneration'] = {'argv': ['./arc/tools/regen-patches.sh'], 'cwd': '.', 'env': {}, 'toolModes': dict(M.REGEN_TOOLS)}
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
        self.head.stop(); self.home.stop(); M.ROOT = self.old_root; M.NODE = self.old_node; M.PYTHON = self.old_python
        M.RESOURCE_POLICY = self.old_policy; M.CANCELLED = False; self.temporary.cleanup()

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
                        app / 'Contents/Frameworks/ArcCore.framework/Helpers/Private Helper.app/Contents/MacOS/Helper',
                        app / 'Contents/Frameworks/ArcCore.framework/Versions/A/Resources/resources.pak']
        identities = []
        for p in binary_paths:
            p.parent.mkdir(parents=True, exist_ok=True); p.write_bytes(b'private binary fixture\n')
            value = p.stat(); identities.append({'path': str(p), 'realpath': str(p.resolve()), 'sha256': M.sha(p),
                'dev': value.st_dev, 'ino': value.st_ino, 'size': value.st_size,
                'mtimeNs': value.st_mtime_ns, 'ctimeNs': value.st_ctime_ns})
        binaries = dict(zip(('main', 'engine', 'recorder', 'helpers', 'resources'), identities[:3] + [[identities[3]], identities[4]]))
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

    def test_complete_private_success_and_guard_only_indexes(self):
        tx = self.tx(); self.assertEqual(tx.execute(), 0)
        self.assertEqual([r[0] for r in tx.seen], list(M.SCENARIOS))
        self.assertEqual(M.sha(self.root / 'chromium/src/.git/index'), self.config['gitRepos'][0]['sha256'])
        regeneration = [e for e in self.backend.commands if e[0] == ['./arc/tools/regen-patches.sh']]
        self.assertEqual(len(regeneration), 1)
        self.assertNotIn('GIT_INDEX_FILE', regeneration[0][2])
        self.assertFalse(any(e[0][:3] == ['git', 'add', '-N'] for e in self.backend.commands))
        self.assertFalse(list(self.output.glob('alternate-index-*')))

    def test_candidate_failure_restores_absence_then_recovers_package(self):
        tx = self.tx(); tx.fail_scenario = 'devtools'; self.assertEqual(tx.execute(), 1)
        for name, values in self.images.items():
            self.assertEqual((self.root / name).read_bytes() if values[0] is not None else None, values[0])
            if values[0] is None: self.assertFalse((self.root / name).exists())
        self.assertFalse((self.root / 'patches/0010-webextras.patch').exists())
        self.assertEqual(tx.seen[-1], ('launch-slice', 7, True))
        self.assertEqual(json.loads((self.output / 'completion.json').read_text())['status'], 'ROLLED_BACK')
        self.assertEqual((self.root/'chromium/src/.git/index').read_bytes(), b'PRIVATE INDEX\n')
        for name, mode in M.REGEN_TOOLS.items():
            self.assertEqual(M.image_state(self.root/name), (self.config['pins'][name], mode))
        self.assertEqual(M.sha(self.root / 'arc/browser/webextras_scenario_current_run.py'), self.config['observationTool']['after'])
        self.assertTrue(json.loads((self.output / 'completion.json').read_text())['persistentObservationToolRetained'])

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
            if args[0] == ['./arc/tools/regen-patches.sh'] and not (self.root / 'arc/api/test.mm').read_text().startswith('old'):
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
        original_popen = M.subprocess.Popen
        def popen(argv, *args, **kwargs):
            return Child() if '_worker' in argv else original_popen(argv, *args, **kwargs)
        with patch.object(M.sys, 'argv', argv), patch.object(M.subprocess, 'Popen', side_effect=popen), \
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
        image.update(beforeMode=None, afterMode=0o644)
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

    def test_observation_tool_postwrite_failure_reverted_before_engine(self):
        original = M.atomic_bytes
        tool = self.root / 'arc/browser/webextras_scenario_current_run.py'
        def interrupted(path, data, mode=0o644):
            original(path, data, mode)
            if path == tool and data == b'new private observation only\n':
                raise OSError('private interrupted after observation rename')
        tx = self.tx()
        with patch.object(M, 'atomic_bytes', side_effect=interrupted):
            self.assertEqual(tx.execute(), 1)
        self.assertFalse(tx.mutated); self.assertEqual(M.sha(tool), self.config['observationTool']['before'])
        self.assertEqual((self.root / 'arc/api/test.mm').read_bytes(), b'old\n')

    def test_resource_proof_bound_to_pak_and_explicit_candidate_recovery_modes(self):
        output, _ = self.scenario_fixture()
        # These private artifacts are constructed BEFORE the fixture is sealed.
        for section in ('pins', 'baselineFiles'):
            for name in self.config[section]: self.config[section][name] = M.sha(M.pin_path(self.root, name))
        tx = self.tx(); tx.prepare(); tx.adopt_observation_tool()
        digest = json.loads((output / 'binary-identity.json').read_text())['resources']['sha256']
        receipt = {'passed': True, 'expectHostGate': 'present', 'builtPackagedPakExactMatch': True,
                   'builtPakSHA256': digest, 'runtimePakSHA256': digest,
                   'nativePackSanity': {role: dict(version=5, nativeLoaderChecksPassed=True, writerLookupDeterminismPolicyPassed=True)
                                        for role in ('intermediate', 'built', 'packaged')},
                   'resources': [dict(textualId=n, generatedExactMatch=True, grdInputExactMatch=True,
                                      intermediateExactMatch=True) for n in M.RESOURCE_IDS]}
        receipt['inputs'] = {str(M.pin_path(self.root, n)): {'sha256': self.config['pins'][n]} for n in (*M.RESOURCE_INPUTS, str(M.NODE))}
        self.backend.resource_receipt = receipt
        self.assertEqual(tx.check_resources(output, 'launch-slice', 2)['expectHostGate'], 'present')
        for i, mutate in enumerate([lambda r: r.update(runtimePakSHA256='0'*64),
                                    lambda r: r.update(expectHostGate='absent'),
                                    lambda r: r['resources'][0].update(generatedExactMatch=False),
                                    lambda r: r['inputs'].pop(str(M.NODE)),
                                    lambda r: r['nativePackSanity'].pop('packaged'),
                                    lambda r: r['nativePackSanity']['intermediate'].update(nativeLoaderChecksPassed=False),
                                    lambda r: r['nativePackSanity']['built'].update(writerLookupDeterminismPolicyPassed=False)], 10):
            self.backend.resource_receipt = copy.deepcopy(receipt); mutate(self.backend.resource_receipt)
            with self.assertRaises(RuntimeError): tx.check_resources(output, 'launch-slice', i)
        self.backend.resource_receipt = copy.deepcopy(receipt); self.backend.resource_receipt['expectHostGate'] = 'absent'
        self.assertEqual(tx.check_resources(output, 'launch-slice', 20, recovery=True)['expectHostGate'], 'absent')

    def test_missing_resource_key_rejects_new_runtime_even_with_old_baseline(self):
        output, baseline = self.scenario_fixture()
        for name in ('binary-identity.json', 'binary-identity-after-run.json'):
            value = json.loads((output / name).read_text()); value.pop('resources'); M.write(output / name, value)
        with self.assertRaises((RuntimeError, KeyError)):
            M.scenario_gate(self.root, output, 'launch-slice', baseline, 1)

    def test_git_environment_never_inherits_actual_repository_routing(self):
        with patch.dict(M.os.environ, {'GIT_INDEX_FILE': '/private/foreign-index', 'GIT_DIR': '/private/foreign-git',
                                       'GIT_WORK_TREE': '/private/foreign-worktree', 'GIT_CONFIG_PARAMETERS': "'core.worktree=/private/foreign'",
                                       'GIT_CONFIG_COUNT': '1', 'GIT_CONFIG_KEY_0': 'core.worktree', 'GIT_CONFIG_VALUE_0': '/private/foreign'}):
            env = M.git_environment()
        self.assertNotIn('GIT_INDEX_FILE', env)
        self.assertNotIn('GIT_DIR', env); self.assertNotIn('GIT_WORK_TREE', env)
        self.assertNotIn('GIT_CONFIG_PARAMETERS', env); self.assertNotIn('GIT_CONFIG_COUNT', env)
        self.assertNotIn('GIT_CONFIG_KEY_0', env); self.assertNotIn('GIT_CONFIG_VALUE_0', env)
        self.assertEqual(env['GIT_OPTIONAL_LOCKS'], '0')

    def test_resource_cli_cannot_hide_pinned_helper_after_python_c(self):
        config = copy.deepcopy(self.config)
        config['resourceChecks']['candidate']['argv'][1:1] = ['-c', 'print("unrelated program")']
        with self.assertRaisesRegex(RuntimeError, 'CLI/mode mismatch'): M.validate(config, self.root)

    def test_initial_resource_role_modes_cannot_be_swapped_even_with_matching_cli(self):
        for role, mode in [('candidate', 'absent'), ('recovery', 'present')]:
            config = copy.deepcopy(self.config)
            config['resourceChecks'][role]['expectHostGate'] = mode
            argv = config['resourceChecks'][role]['argv']
            argv[argv.index('--expect-host-gate') + 1] = mode
            with self.assertRaisesRegex(RuntimeError, 'candidate-present/recovery-absent'):
                M.validate(config, self.root)

    def executable_regen_target(self):
        name = 'arc/browser/owned-executable-fixture.sh'
        before, after = b'#!/bin/sh\n# old private fixture\n', b'#!/bin/sh\n# reviewed private fixture\n'
        self.put(name, before); (self.root / name).chmod(0o755)
        blob = 'private-images/regen-patches.sh'; self.put(blob, after)
        self.config['targets'][name] = dict(before=M.sha(self.root / name), after=M.sha(self.root / blob),
            blob=blob, beforeMode=0o755, afterMode=0o755)
        self.config['pins'][blob] = M.sha(self.root / blob)
        proposal = 'proposals/executable-regen.patch'
        self.put(proposal, ''.join(difflib.unified_diff(before.decode().splitlines(True), after.decode().splitlines(True),
                 fromfile='a/' + name, tofile='b/' + name)).encode())
        digest = M.sha(self.root / proposal); self.config['pins'][proposal] = digest
        self.config['patches'].append(dict(path=proposal, sha256=digest, cwd='.'))
        return self.root / name, before, after

    def test_real_0755_publication_preserves_script_and_new_0644_helper(self):
        path, before, after = self.executable_regen_target()
        tx = self.tx(); self.assertEqual(tx.execute(), 0)
        self.assertEqual(path.read_bytes(), after); self.assertEqual(path.stat().st_mode & 0o777, 0o755)
        self.assertEqual((self.root / 'chromium/src/new.h').stat().st_mode & 0o777, 0o644)
        self.assertEqual((self.root / 'patches/MANIFEST').stat().st_mode & 0o777, 0o644)
        self.assertEqual(json.loads((self.output / 'modes/arc_browser_owned-executable-fixture.sh.json').read_text()), 0o755)

    def test_real_0755_candidate_failure_rolls_back_script_bytes_and_mode(self):
        path, before, after = self.executable_regen_target()
        tx = self.tx(); tx.fail_scenario = 'devtools'; self.assertEqual(tx.execute(), 1)
        self.assertEqual(path.read_bytes(), before); self.assertEqual(path.stat().st_mode & 0o777, 0o755)
        self.assertEqual(json.loads((self.output / 'completion.json').read_text())['status'], 'ROLLED_BACK')

    def test_mode_drift_blocks_publication_phase_acceptance_and_rollback_CAS(self):
        path, before, after = self.executable_regen_target()
        tx = self.tx(); tx.prepare(); path.chmod(0o644)
        with self.assertRaisesRegex(RuntimeError, 'mode phase drift'): tx.publish()
        self.assertEqual(path.read_bytes(), before); self.assertFalse(tx.mutated)
        path.chmod(0o755); tx.publish(); tx.regenerate(); path.chmod(0o644)
        with self.assertRaisesRegex(RuntimeError, 'mode phase drift'): tx.file_state('after')
        with self.assertRaisesRegex(RuntimeError, 'bytes/mode at rollback'): tx.rollback()
        self.assertEqual(path.read_bytes(), after); self.assertEqual(path.stat().st_mode & 0o777, 0o644)

    def test_missing_or_changed_sealed_mode_is_rejected(self):
        path, before, after = self.executable_regen_target()
        for mutate in [lambda image: image.pop('beforeMode'), lambda image: image.update(afterMode=0o644),
                       lambda image: image.update(beforeMode=True, afterMode=True)]:
            config = copy.deepcopy(self.config); mutate(config['targets']['arc/browser/owned-executable-fixture.sh'])
            with self.assertRaises(RuntimeError): M.validate(config, self.root)

    def worker_for_guardian(self):
        worker = PrivateTransaction(copy.deepcopy(self.config), self.output, self.backend)
        worker.seen = []; worker.fail_scenario = None
        return worker

    def assert_same_guardian_handoff(self, guardian, phase, retained):
        completion = json.loads((self.output / 'completion.json').read_text())
        self.assertEqual(completion['persistentObservationToolRetained'], retained)
        if retained:
            # Actual old path: this pre-worker instance still expects before570.
            with self.assertRaisesRegex(RuntimeError, 'persistent observation-tool drift'):
                guardian.file_state(phase)
        stamp = guardian.lock; groups = guardian.backend.groups
        digest = guardian.config_digest
        guardian.stopped(); guardian.refresh_observation_tool_after_worker(completion)
        self.assertIs(guardian.lock, stamp); self.assertIs(guardian.backend.groups, groups)
        self.assertEqual(guardian.config_digest, digest)
        self.assertIs(guardian.tool_adopted, retained)
        guardian.file_state(phase)

    def test_same_preworker_guardian_accepts_applied_persistent_tool(self):
        guardian = self.tx(); guardian.lock_check(); self.assertFalse(guardian.tool_adopted)
        worker = self.worker_for_guardian(); self.assertEqual(worker.execute(), 0)
        self.assertFalse(guardian.tool_adopted)
        self.assert_same_guardian_handoff(guardian, 'after', True)

    def test_same_preworker_guardian_accepts_rollback_retaining_tool(self):
        guardian = self.tx(); guardian.lock_check(); worker = self.worker_for_guardian()
        worker.fail_scenario = 'devtools'; self.assertEqual(worker.execute(), 1)
        self.assertEqual(json.loads((self.output / 'completion.json').read_text())['status'], 'ROLLED_BACK')
        self.assert_same_guardian_handoff(guardian, 'before', True)

    def test_same_preworker_guardian_accepts_refused_engine_retaining_tool(self):
        guardian = self.tx(); guardian.lock_check(); worker = self.worker_for_guardian()
        with patch.object(worker, 'prepare', side_effect=RuntimeError('private prepublication failure')):
            self.assertEqual(worker.execute(), 1)
        self.assert_same_guardian_handoff(guardian, 'before', True)

    def test_same_preworker_guardian_accepts_refused_reverted_tool(self):
        guardian = self.tx(); guardian.lock_check(); worker = self.worker_for_guardian()
        original = M.atomic_bytes; failed = False
        def interrupted(path, data, mode=0o644):
            nonlocal failed
            original(path, data, mode)
            if path == self.root / 'arc/browser/webextras_scenario_current_run.py' and not failed:
                failed = True; raise OSError('private post-rename failure')
        with patch.object(M, 'atomic_bytes', side_effect=interrupted):
            self.assertEqual(worker.execute(), 1)
        self.assertEqual(json.loads((self.output / 'observation-tool.json').read_text())['status'],
                         'REVERTED_BEFORE_ENGINE_PUBLICATION')
        self.assert_same_guardian_handoff(guardian, 'before', False)

    def test_same_preworker_guardian_accepts_refused_before_any_tool_receipt(self):
        guardian = self.tx(); guardian.lock_check(); worker = self.worker_for_guardian()
        with patch.object(worker, 'adopt_observation_tool', side_effect=RuntimeError('private refusal before tool')):
            self.assertEqual(worker.execute(), 1)
        self.assertFalse((self.output / 'observation-tool.json').exists())
        self.assert_same_guardian_handoff(guardian, 'before', False)

    def test_guardian_receipt_refresh_rejects_uncertain_or_inconsistent_handoffs(self):
        guardian = self.tx(); guardian.lock_check(); worker = self.worker_for_guardian()
        worker.adopt_observation_tool()
        path = self.output / 'observation-tool.json'; good = json.loads(path.read_text())
        completion = dict(status='REFUSED_BEFORE_PUBLICATION', persistentObservationToolRetained=True)
        cases = [None, {**good, 'status': 'ADOPTING'}, {**good, 'status': 'unknown'},
                 {**good, 'before': '0' * 64}, {**good, 'after': '0' * 64},
                 {**good, 'patch': 'other'}, {**good, 'blob': 'other'},
                 {**good, 'engineRollbackRetainsThisOwnedReadOnlyTool': False},
                 {**good, 'status': 'REVERTED_BEFORE_ENGINE_PUBLICATION'}]
        for receipt in cases:
            with self.subTest(receipt=receipt):
                M.write(path, receipt)
                with self.assertRaises(RuntimeError): guardian.refresh_observation_tool_after_worker(completion)
                self.assertFalse(guardian.tool_adopted)
        M.write(path, good)
        for bad in [dict(status='APPLIED', persistentObservationToolRetained=False),
                    dict(status='ROLLED_BACK', persistentObservationToolRetained=False),
                    dict(status='unknown', persistentObservationToolRetained=True), dict(status='APPLIED'),
                    dict(status='APPLIED', persistentObservationToolRetained=1),
                    dict(status='REFUSED_BEFORE_PUBLICATION', persistentObservationToolRetained=False)]:
            with self.assertRaises(RuntimeError): guardian.refresh_observation_tool_after_worker(bad)
            self.assertFalse(guardian.tool_adopted)
        runner = self.root / 'arc/browser/webextras_scenario_current_run.py'; after = runner.read_bytes()
        runner.write_bytes(b'unknown private runner\n')
        with self.assertRaisesRegex(RuntimeError, 'handoff source mismatch'):
            guardian.refresh_observation_tool_after_worker(completion)
        runner.write_bytes(after); path.unlink()
        with self.assertRaisesRegex(RuntimeError, 'lacks durable adoption'):
            guardian.refresh_observation_tool_after_worker(completion)

    def test_actual_main_reuses_preworker_guardian_after_worker_drain(self):
        guardian = self.tx(); self.assertFalse(guardian.tool_adopted)
        digest = M.sha(self.output / 'config.json')
        M.write(self.output / 'launch.json', {'configSHA256': digest, 'nonce': 'private-main'})
        fixture = self
        class Child:
            pid = 424242
            returncode = None
            def wait(self):
                fixture.assertFalse(guardian.tool_adopted)
                self.returncode = fixture.worker_for_guardian().execute()
                fixture.assertFalse(guardian.tool_adopted)
                return self.returncode
        argv = ['executor', '_guardian', '--config', str(self.output / 'config.json'),
                '--config-sha', digest, '--out', str(self.output)]
        original_popen = M.subprocess.Popen
        def popen(argv, *args, **kwargs):
            return Child() if '_worker' in argv else original_popen(argv, *args, **kwargs)
        with patch.object(M, 'Transaction', return_value=guardian) as factory, \
             patch.object(M.subprocess, 'Popen', side_effect=popen), patch.object(M.sys, 'argv', argv), \
             patch.object(M.signal, 'signal'):
            self.assertEqual(M.main(), 0)
        factory.assert_called_once(); self.assertTrue(guardian.tool_adopted)
        self.assertEqual(guardian.backend.groups, {424242})
        self.assertEqual(json.loads((self.output / 'release-safe.json').read_text())['state'], 'APPLIED')

    def test_actual_main_does_not_refresh_handoff_before_known_groups_are_quiet(self):
        guardian = self.tx(); digest = M.sha(self.output / 'config.json')
        M.write(self.output / 'launch.json', {'configSHA256': digest, 'nonce': 'private-live-group'})
        fixture = self; completed = False
        class Child:
            pid = 424242
            returncode = None
            def wait(self):
                nonlocal completed
                self.returncode = fixture.worker_for_guardian().execute()
                completed = True
                return self.returncode
        original_inventory = self.backend.inventory
        def inventory():
            return original_inventory() + ([dict(pid=424242, ppid=os.getpid(), pgid=424242,
                    state='S', command='private-surviving-phase')] if completed else [])
        original_popen = M.subprocess.Popen
        def popen(argv, *args, **kwargs):
            return Child() if '_worker' in argv else original_popen(argv, *args, **kwargs)
        argv = ['executor', '_guardian', '--config', str(self.output / 'config.json'),
                '--config-sha', digest, '--out', str(self.output)]
        with patch.object(M, 'Transaction', return_value=guardian), patch.object(self.backend, 'inventory', side_effect=inventory), \
             patch.object(M.subprocess, 'Popen', side_effect=popen), patch.object(M.sys, 'argv', argv), \
             patch.object(M.signal, 'signal'):
            with self.assertRaisesRegex(Held, 'survivors'): M.main()
        self.assertFalse(guardian.tool_adopted)
        self.assertFalse((self.output / 'release-safe.json').exists())

    def early_refusal_fixture(self):
        guardian = self.tx(); digest = M.sha(self.output / 'config.json')
        M.write(self.output / 'launch.json', {'configSHA256': digest, 'nonce': 'private-early-refusal'})
        guardian.capture_preworker_release_state(self.output / 'config.json', digest)
        M.write(self.output / 'worker.json', {'pid': 424242, 'nonce': 'private-early-refusal'})
        completion = dict(status='REFUSED_BEFORE_PUBLICATION', persistentObservationToolRetained=False)
        M.write(self.output / 'completion.json', completion)
        M.write(self.output / 'failure.json', {'error': 'private earlier worker drift', 'sourcePublished': False})
        child = types.SimpleNamespace(pid=424242, returncode=1)
        guardian.backend.groups.add(child.pid)
        return guardian, completion, child

    def test_early_refusal_certifies_stale_source_and_index_without_current_baseline_claim(self):
        guardian, completion, child = self.early_refusal_fixture()
        self.put('chromium/src/native.cc', b'foreign accepted source\n')
        self.put('chromium/src/.git/index', b'foreign staging stays\n')
        with self.assertRaises(RuntimeError): guardian.file_state('before')
        certificate = guardian.certify_refused_without_writes(completion, child)
        self.assertEqual(certificate['state'], 'REFUSED_WITHOUT_WRITES_NOT_CURRENT_BASELINE')
        self.assertIs(certificate['currentBaselineAsserted'], False)
        self.assertIs(certificate['normalOSExitStatusCaptured'], False)
        self.assertEqual((self.root / 'chromium/src/.git/index').read_bytes(), b'foreign staging stays\n')
        self.assertEqual(self.backend.commands, [])

    def test_actual_same_preworker_guardian_releases_only_no_write_foreign_refusal(self):
        guardian = self.tx(); digest = M.sha(self.output / 'config.json')
        M.write(self.output / 'launch.json', {'configSHA256': digest, 'nonce': 'private-early-main'})
        fixture = self
        class Child:
            pid = 424242
            returncode = None
            def wait(self):
                fixture.assertTrue((fixture.output / 'guardian-preworker.json').is_file())
                fixture.put('chromium/src/native.cc', b'foreign accepted before worker\n')
                fixture.put('chromium/src/.git/index', b'foreign index before worker\n')
                self.returncode = fixture.worker_for_guardian().execute()
                return self.returncode
        original_popen = M.subprocess.Popen
        def popen(argv, *args, **kwargs):
            return Child() if '_worker' in argv else original_popen(argv, *args, **kwargs)
        argv = ['executor', '_guardian', '--config', str(self.output / 'config.json'),
                '--config-sha', digest, '--out', str(self.output)]
        with patch.object(M, 'Transaction', return_value=guardian) as factory, \
             patch.object(M.subprocess, 'Popen', side_effect=popen), patch.object(M.sys, 'argv', argv), \
             patch.object(M.signal, 'signal'):
            self.assertEqual(M.main(), 1)
        factory.assert_called_once()
        self.assertEqual(json.loads((self.output / 'release-safe.json').read_text())['state'],
                         'REFUSED_WITHOUT_WRITES_NOT_CURRENT_BASELINE')
        self.assertFalse((self.output / 'observation-tool.json').exists())
        self.assertEqual(self.backend.commands, [])

    def refusal_negative_cases(self, cases):
        for name, mutate in cases:
            with self.subTest(name=name):
                fixture = Fixture('runTest'); fixture.setUp()
                try:
                    guardian, completion, child = fixture.early_refusal_fixture()
                    mutate(fixture, guardian, completion, child)
                    with self.assertRaises((RuntimeError, ValueError, KeyError, OSError, AttributeError)):
                        guardian.certify_refused_without_writes(completion, child)
                    self.assertFalse((fixture.output / 'release-safe.json').exists())
                finally:
                    fixture.tearDown()

    def test_actual_main_index_only_refusal_allows_read_only_diagnostic_and_preserves_index(self):
        guardian = self.tx(); digest = M.sha(self.output / 'config.json')
        M.write(self.output / 'launch.json', {'configSHA256': digest, 'nonce': 'private-index-only'})
        fixture = self
        class Child:
            pid = 424242
            returncode = None
            def wait(self):
                fixture.put('chromium/src/.git/index', b'foreign staging only\n')
                self.returncode = fixture.worker_for_guardian().execute()
                return self.returncode
        original_popen = M.subprocess.Popen
        def popen(argv, *args, **kwargs):
            return Child() if '_worker' in argv else original_popen(argv, *args, **kwargs)
        argv = ['executor', '_guardian', '--config', str(self.output/'config.json'),
                '--config-sha', digest, '--out', str(self.output)]
        with patch.object(M, 'Transaction', return_value=guardian), patch.object(M.subprocess, 'Popen', side_effect=popen), \
             patch.object(M.sys, 'argv', argv), patch.object(M.signal, 'signal'):
            self.assertEqual(M.main(), 1)
        self.assertTrue((self.output/'index-drift.json').is_file())
        self.assertTrue((self.output/'index-semantic-diagnostic.txt').is_file())
        self.assertEqual((self.root/'chromium/src/.git/index').read_bytes(), b'foreign staging only\n')
        self.assertEqual(json.loads((self.output/'release-safe.json').read_text())['state'],
                         'REFUSED_WITHOUT_WRITES_NOT_CURRENT_BASELINE')
        self.assertEqual(self.backend.commands, [])

    def test_early_refusal_rejects_attempted_or_unknown_output_evidence(self):
        names = ['publication.json', 'observation-tool.json', '001-any.inputs.json', '001-any.child.json',
                 '001-any.log', 'observation-stage/anything', 'observation-tool.before',
                 'directory-identities.json', 'before/anything', 'modes/anything', 'unknown.json']
        self.refusal_negative_cases([(name, lambda f, g, c, p, n=name: f.put('evidence/' + n, b'{}')) for name in names])

    def test_early_refusal_rejects_config_nonce_worker_completion_and_failure_ambiguity(self):
        cases = [('config', lambda f,g,c,p: f.put('evidence/config.json', b'{}')),
                 ('launch nonce', lambda f,g,c,p: M.write(f.output/'launch.json', {'nonce':'changed'})),
                 ('worker pid', lambda f,g,c,p: M.write(f.output/'worker.json', {'pid':424243,'nonce':'private-early-refusal'})),
                 ('snapshot', lambda f,g,c,p: M.write(f.output/'guardian-preworker.json', {})),
                 ('worker exit', lambda f,g,c,p: setattr(p, 'returncode', 0)),
                 ('wrong status', lambda f,g,c,p: c.update(status='APPLIED')),
                 ('retained', lambda f,g,c,p: c.update(persistentObservationToolRetained=True)),
                 ('falsey retained', lambda f,g,c,p: c.update(persistentObservationToolRetained=0)),
                 ('failure absent', lambda f,g,c,p: (f.output/'failure.json').unlink()),
                 ('failure falsey', lambda f,g,c,p: M.write(f.output/'failure.json', {'sourcePublished':0})),
                 ('failure uncertain', lambda f,g,c,p: M.write(f.output/'failure.json', {})),
                 ('failure published', lambda f,g,c,p: M.write(f.output/'failure.json', {'sourcePublished':True}))]
        self.refusal_negative_cases(cases)

    def test_early_refusal_rejects_equal_byte_replacement_chmod_and_trusted_source_change(self):
        def replace_runner(f,g,c,p):
            path=f.root/'arc/browser/webextras_scenario_current_run.py'
            other=path.with_suffix('.new'); other.write_bytes(path.read_bytes()); other.replace(path)
        self.refusal_negative_cases([
            ('equal-byte runner replacement', replace_runner),
            ('runner chmod', lambda f,g,c,p: (f.root/'arc/browser/webextras_scenario_current_run.py').chmod(0o755)),
            ('own trusted source', lambda f,g,c,p: f.put('arc/browser/webextras_initial_integration.py', b'changed trusted source')),
            ('own chmod', lambda f,g,c,p: (f.root/'arc/browser/webextras_initial_integration.py').chmod(0o755)),
            ('launch symlink', lambda f,g,c,p: ((f.output/'launch.json').unlink(), (f.output/'launch.json').symlink_to(f.output/'config.json'))),
            ('dangling phase symlink', lambda f,g,c,p: (f.output/'publication.json').symlink_to(f.output/'missing'))])

    def test_early_refusal_rejects_profile_cache_lock_or_surviving_group_changes(self):
        def root_create(f,g,c,p):
            Path(next(iter(g.preworker[0]['roots']))).mkdir(parents=True)
        def change_lock(f,g,c,p):
            path=f.root/'logs/arccore-build.lock/pid'
            other=path.with_suffix('.new'); other.write_bytes(path.read_bytes()); other.replace(path)
        def surviving(f,g,c,p):
            prior=g.backend.inventory
            g.backend.inventory=lambda: prior()+[dict(pid=p.pid,ppid=os.getpid(),pgid=p.pid,state='S',command='private-survivor')]
        def app_surviving(f,g,c,p,helper=False):
            prior=g.backend.inventory
            app=f.root.parent/'arc/build/ArcModAgent-ours156.app'
            command=str(app/('Contents/Frameworks/ArcCore.framework/Helper' if helper else 'Contents/MacOS/Arc'))
            g.backend.inventory=lambda: prior()+[dict(pid=424243,ppid=os.getpid(),pgid=111,state='S',command=command)]
        self.refusal_negative_cases([('new profile root',root_create),('lock replacement',change_lock),('worker group live',surviving),
            ('owned app live',app_surviving),('owned helper live',lambda f,g,c,p: app_surviving(f,g,c,p,True))])

    def test_early_refusal_existing_profile_and_cache_identities_are_preworker_evidence(self):
        for suffix in ('Application Support', 'Caches'):
            path=Path.home()/'Library'/suffix/'ArcModAgent-ours156'; path.mkdir(parents=True)
        guardian, completion, child = self.early_refusal_fixture()
        self.assertTrue(all(value is not None for value in guardian.preworker[0]['roots'].values()))
        guardian.certify_refused_without_writes(completion, child)
        cache=Path.home()/'Library/Caches/ArcModAgent-ours156'; cache.rename(cache.with_name('saved'))
        cache.mkdir()
        with self.assertRaisesRegex(RuntimeError, 'profile/cache'): guardian.certify_refused_without_writes(completion, child)

    def test_guard_only_contract_rejects_old_caller_index_and_override_interfaces(self):
        for key, value in [('environmentKey', 'GIT_INDEX_FILE'), ('intentToAdd', ['new.h']), ('existingIntentToAdd', {})]:
            config=copy.deepcopy(self.config); config['gitRepos'][0][key]=value
            with self.assertRaisesRegex(RuntimeError, 'Git repository contract'): M.validate(config, self.root)
        for key, value in [('argv', ['bash', 'unreviewed-generator']), ('cwd', 'chromium/src'),
                           ('env', {'GIT_INDEX_FILE':'ignored'}), ('env', {'CUSTOM_GENERATOR_ROOT':'ignored'}),
                           ('toolModes', {**M.REGEN_TOOLS, 'arc/tools/regen-patches.sh':0o644})]:
            config=copy.deepcopy(self.config); config['regeneration'][key]=value
            with self.assertRaises(RuntimeError): M.validate(config, self.root)

    def test_baseline_tool_target_and_disguised_patch_changed_paths_are_rejected(self):
        for name in M.REGEN_TOOLS:
            config=copy.deepcopy(self.config)
            config['targets'][name]=copy.deepcopy(config['targets']['arc/api/test.mm'])
            with self.assertRaisesRegex(RuntimeError, 'cannot be an engine target'): M.validate(config, self.root)
            for cwd, leaf in [('.',name),('arc','tools/'+Path(name).name)]:
                text=''.join(difflib.unified_diff(['old\n'],['new\n'],fromfile='a/'+leaf,tofile='b/'+leaf))
                path='proposals/innocent-name.patch'; self.put(path,text.encode())
                config=copy.deepcopy(self.config); digest=M.sha(self.root/path); config['pins'][path]=digest
                config['patches'].append(dict(path=path,sha256=digest,cwd=cwd))
                with self.assertRaisesRegex(RuntimeError, 'patch changes shared regeneration'): M.validate(config,self.root)
            path='proposals/renamed-baseline.patch'
            self.put(path,('diff --git a/'+name+' b/arc/browser/disguised.sh\nsimilarity index 100%\nrename from '+name+
                           '\nrename to arc/browser/disguised.sh\n').encode())
            config=copy.deepcopy(self.config); digest=M.sha(self.root/path); config['pins'][path]=digest
            config['patches'].append(dict(path=path,sha256=digest,cwd='.'))
            with self.assertRaisesRegex(RuntimeError, 'rename/copy is unsupported'): M.validate(config,self.root)

    def test_immutable_tool_hash_mode_and_retention_survive_engine_recovery(self):
        before={n:M.image_state(self.root/n) for n in M.REGEN_TOOLS}
        tx=self.tx(); tx.fail_scenario='devtools'; self.assertEqual(tx.execute(),1)
        self.assertEqual({n:M.image_state(self.root/n) for n in M.REGEN_TOOLS},before)
        self.assertFalse(any(n in json.loads((self.output/'journal.json').read_text())['images'] for n in M.REGEN_TOOLS))
        regeneration=[entry for entry in self.backend.commands if entry[0]==['./arc/tools/regen-patches.sh']]
        self.assertEqual(len(regeneration),2)
        self.assertTrue(all(not any(k.endswith('INDEX_FILE') for k in entry[2]) for entry in regeneration))
        for name in M.REGEN_TOOLS:
            path=self.root/name; oldmode=path.stat().st_mode&0o777
            path.chmod(0o600)
            with self.assertRaisesRegex(RuntimeError,'baseline mode drift'): tx.pin_check()
            path.chmod(oldmode)
            data=path.read_bytes(); path.write_bytes(data+b'foreign\n')
            with self.assertRaisesRegex(RuntimeError,'immutable input drift'): tx.rollback()
            self.assertEqual(path.read_bytes(),data+b'foreign\n'); path.write_bytes(data)

    def test_real_copied_generator_candidate_and_recovery_use_persistent_baseline(self):
        # This test executes only a copied reviewed helper in a disposable checkout.
        source=self.root/'chromium/src'; nested=source/'third_party/devtools-frontend/src'
        shutil.rmtree(source/'.git'); nested.mkdir(parents=True)
        commands=[]
        def git(repo,*args):
            result=subprocess.run(['git','-C',str(repo),*args],env=M.git_environment(),capture_output=True)
            commands.append(dict(argv=['git', '-C', str(repo), *args],exit=result.returncode,
                                 stdout=result.stdout.decode(),stderr=result.stderr.decode()))
            self.assertEqual(result.returncode,0,result.stderr.decode()); return result.stdout.decode().strip()
        for repo in (source,nested):
            git(repo,'init','--quiet'); git(repo,'config','user.name','Private integration fixture')
            git(repo,'config','user.email','fixture@example.invalid'); git(repo,'config','commit.gpgsign','false')
        (nested/'frontend.ts').write_bytes(b'original frontend\n')
        git(nested,'add','frontend.ts'); git(nested,'commit','-qm','nested base')
        (source/'native.cc').write_bytes(b'original native\n')
        git(source,'add','--all'); git(source,'commit','-qm','outer base')
        self.put('chromium/src/native.cc',b'old native\n')
        (nested/'frontend.ts').write_bytes(b'old frontend\n')
        foreign='chromium/src/foreign_hooks.h'; self.put(foreign,b'immutable unindexed manifest leaf\n')
        self.config['pins'][foreign]=M.sha(self.root/foreign)
        name='chromium/src/third_party/devtools-frontend/src/frontend.ts'; blob='private-images/frontend.ts'
        self.put(blob,b'new frontend\n'); self.config['pins'][blob]=M.sha(self.root/blob)
        self.config['targets'][name]=dict(before=M.sha(self.root/name),after=M.sha(self.root/blob),blob=blob,beforeMode=0o644,afterMode=0o644)
        proposal='proposals/frontend.patch'; self.put(proposal,b'--- a/frontend.ts\n+++ b/frontend.ts\n@@ -1 +1 @@\n-old frontend\n+new frontend\n')
        self.config['pins'][proposal]=M.sha(self.root/proposal)
        self.config['patches'].append(dict(path=proposal,sha256=M.sha(self.root/proposal),cwd='chromium/src/third_party/devtools-frontend/src'))
        before=b'0004-arcmod-hooks.patch native.cc\n0004-arcmod-hooks.patch foreign_hooks.h\n0004-arcmod-hooks.patch third_party/devtools-frontend/src/frontend.ts\n'
        self.put('patches/MANIFEST',before); self.put('private-images/MANIFEST',before+b'0010-webextras.patch new.h\n')
        self.config['manifest'].update(before=M.sha(self.root/'patches/MANIFEST'),after=M.sha(self.root/'private-images/MANIFEST'))
        self.config['pins']['private-images/MANIFEST']=self.config['manifest']['after']
        evidence=MODULE_PATH.resolve().parents[2]/'logs/wp/webextras-nested-regeneration-lead-rebase-reviewed'
        frozen=json.loads((evidence/'freeze.json').read_text())
        for name,mode in M.REGEN_TOOLS.items():
            data=(evidence/'staged'/name).read_bytes()
            self.assertEqual(__import__('hashlib').sha256(data).hexdigest(),frozen['adoptionFiles'][name])
            self.put(name,data); (self.root/name).chmod(mode); self.config['pins'][name]=M.sha(self.root/name)
        tools={n:M.file_identity(self.root/n) for n in M.REGEN_TOOLS}
        indices={str(repo/'.git/index'):M.file_identity(repo/'.git/index') for repo in (source,nested)}
        def real_regen():
            r=subprocess.run(['./arc/tools/regen-patches.sh'],cwd=self.root,env=M.git_environment(),capture_output=True)
            commands.append(dict(argv=['./arc/tools/regen-patches.sh'],cwd=str(self.root),exit=r.returncode,
                                 stdout=r.stdout.decode(),stderr=r.stderr.decode()))
            self.assertEqual(r.returncode,0,r.stderr.decode())
        real_regen(); old_outputs={str(p.relative_to(self.root)):p.read_bytes() for p in (self.root/'patches').glob('*.patch')}
        before_images={n:(self.root/n).read_bytes() if image['before'] else None for n,image in self.config['targets'].items()}
        for name,image in self.config['targets'].items(): self.put(name,(self.root/image['blob']).read_bytes())
        self.put('patches/MANIFEST',(self.root/self.config['manifest']['blob']).read_bytes()); real_regen()
        after_outputs={str(p.relative_to(self.root)):p.read_bytes() for p in (self.root/'patches').glob('*.patch')}
        for name,data in before_images.items():
            if data is None: (self.root/name).unlink()
            else: self.put(name,data)
        self.put('patches/MANIFEST',before); (self.root/'patches/0010-webextras.patch').unlink(); real_regen()
        self.assertEqual({str(p.relative_to(self.root)):p.read_bytes() for p in (self.root/'patches').glob('*.patch')},old_outputs)
        self.config['patchOutputs']={name:dict(before=__import__('hashlib').sha256(old_outputs[name]).hexdigest() if name in old_outputs else None,
            after=__import__('hashlib').sha256(data).hexdigest()) for name,data in after_outputs.items()}
        self.config['gitRepos']=[dict(path=str(repo.relative_to(self.root)),index=str((repo/'.git/index').relative_to(self.root)),
            sha256=M.sha(repo/'.git/index'),head=git(repo,'rev-parse','HEAD')) for repo in (source,nested)]
        self.head.stop()  # All remaining Git discovery stays inside these real PRIVATE repos.
        original=self.backend.command
        def command(argv,label,cwd=None,env=None):
            if argv==['./arc/tools/regen-patches.sh']:
                self.backend.commands.append((argv,label,env)); r=subprocess.run(argv,cwd=cwd,env=env,capture_output=True)
                commands.append(dict(argv=argv,cwd=str(cwd),label=label,exit=r.returncode,
                                     stdout=r.stdout.decode(),stderr=r.stderr.decode()))
                (self.output/(label+'.log')).write_bytes(r.stdout+r.stderr); return r.returncode
            return original(argv,label,cwd,env)
        self.backend.command=command
        tx=self.tx(); tx.fail_scenario='devtools'; self.assertEqual(tx.execute(),1)
        self.assertEqual({str(p.relative_to(self.root)):p.read_bytes() for p in (self.root/'patches').glob('*.patch')},old_outputs)
        self.assertEqual({n:M.file_identity(Path(n)) for n in indices},indices)
        self.assertEqual({n:M.file_identity(self.root/n) for n in tools},tools)
        self.assertEqual(M.sha(self.root/foreign),self.config['pins'][foreign])
        self.assertEqual(tx.seen[-1],('launch-slice',7,True))
        if os.environ.get('WEBEXTRAS_INTEGRATION_TEST_EVIDENCE'):
            destination=Path(os.environ['WEBEXTRAS_INTEGRATION_TEST_EVIDENCE'])
            M.write(destination/'copied-generator-proof.json',dict(privateOnly=True,productionToolInvoked=False,
                toolHashes={n:v['sha256'] for n,v in tools.items()},toolIdentitiesPreserved=True,
                realFixtureIndicesBefore=indices,realFixtureIndicesAfter={n:M.file_identity(Path(n)) for n in indices},
                outerAndNestedRepositories=True,unindexedForeignManifestLeafUnchanged=True,
                candidateAndRecoveryRealHelper=True,originalOutputsRestored=True,
                expectedBefore={n:__import__('hashlib').sha256(v).hexdigest() for n,v in old_outputs.items()},
                expectedCandidate={n:__import__('hashlib').sha256(v).hexdigest() for n,v in after_outputs.items()},commands=commands))



if __name__ == '__main__':
    unittest.main()
