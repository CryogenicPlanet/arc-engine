#!/usr/bin/env python3
"""Disposable copied scripts/repos; no production Git, lock, tools or runtime.

Synthetic ACK/review/config, native/build/binary guards and lock/census seams
are TEST INPUTS. They never constitute a production epoch or tool-owner grant.
"""
import argparse
import ast
import copy
import hashlib
import importlib.util
import json
import os
import plistlib
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import types
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[2]
EVIDENCE = ROOT / 'logs/wp/webextras-nested-regeneration-racy'
ARTIFACTS = None
COMMANDS = []


def sha(path): return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def put(root, name, data):
    p = root / name; p.parent.mkdir(parents=True, exist_ok=True)
    p.write_bytes(data.encode() if isinstance(data, str) else data); return p


class Held(RuntimeError): pass
class NoAdmission(RuntimeError): pass


class Fixture:
    def __init__(self, racy=True):
        self.tmp = tempfile.TemporaryDirectory(prefix='webextras-adoption-fixture-')
        self.root = Path(self.tmp.name).resolve() / 'checkout'; self.root.mkdir()
        source = ROOT / 'arc/browser/webextras_generator_adoption.py'
        copied = put(self.root, 'arc/browser/webextras_generator_adoption.py', source.read_bytes())
        spec = importlib.util.spec_from_file_location('adoption_fixture_' + os.urandom(4).hex(), copied)
        self.m = importlib.util.module_from_spec(spec); spec.loader.exec_module(self.m)
        self.native = self.root / 'chromium/src'; self.native.mkdir(parents=True)
        for repo in (self.root, self.native):
            self.git(repo, 'init', '-q')
            for k, v in [('user.name', 'Disposable fixture'), ('user.email', 'fixture@invalid'), ('commit.gpgsign', 'false')]: self.git(repo, 'config', '--local', k, v)
            put(repo, 'README', 'fixture\n'); self.git(repo, 'add', 'README'); self.git(repo, 'commit', '-qm', 'fixture baseline')
        put(self.native, 'native.cc', 'void Original() {\n int value = 1;\n}\n')
        put(self.native, 'foreign.cc', 'void Foreign() {\n int value = 1;\n}\n')
        put(self.native, '.gitignore', 'out/\n')
        self.git(self.native, 'add', 'native.cc', 'foreign.cc', '.gitignore'); self.git(self.native, 'commit', '-qm', 'native baseline')
        put(self.native, 'native.cc', 'void Original() {\n int value = 2;\n}\n')
        put(self.native, 'foreign.cc', 'void Foreign() {\n int value = 3;\n}\n')
        if not racy:
            # Driver fault controls need to reach their intended phase despite
            # the separately retained copied-index/racy-stat helper blocker.
            # This is fixture-only timing separation, NOT helper qualification.
            for leaf in ('native.cc', 'foreign.cc'):
                os.utime(self.native / leaf, ns=(1_600_000_000_123456789, 1_600_000_000_123456789))
        put(self.root, 'patches/MANIFEST', '0001-native.patch native.cc\n0002-foreign.patch foreign.cc\n')
        for leaf, owner in [('native.cc', '0001-native.patch'), ('foreign.cc', '0002-foreign.patch')]:
            put(self.root, 'patches/' + owner, self.git(self.native, 'diff', '--no-color', '--no-ext-diff', '--no-textconv', '--no-renames', 'HEAD', '--', leaf))
        put(self.root, self.m.WRAPPER, (EVIDENCE / 'baseline/arc/tools/regen-patches.sh').read_bytes()).chmod(0o755)
        for file in ('regen-patches.sh', 'regen-chromium-patches.py'):
            put(self.root, 'inputs/' + file, (EVIDENCE / 'staged/arc/tools' / file).read_bytes())
        put(self.root, 'inputs/successor.patch', (ROOT / 'arc/browser/webextras_nested_regeneration_racy.patch').read_bytes())
        put(self.root, 'inputs/review.txt', 'SYNTHETIC fixture review; not approval\n')
        put(self.root, 'inputs/ack.txt', 'SYNTHETIC fixture ACK; not approval\n')
        put(self.root, 'inputs/parent-state.json', '{"fixture":"predecessors reaped; current held state"}\n')
        for name in self.m.PARSER_POLICY:
            put(self.root, name, (ROOT / name).read_bytes())
        for name in ['arc/tools/locked.sh', 'sync-chromium.sh']:
            put(self.root, name, (ROOT / name).read_bytes())
        files = ['arc/api/fixture.mm', 'arc/browser/fixture.h', 'arc/wp/fixture.gni', 'arc/BUILD.gn',
                 'build-babysit.sh', 'package-engine.sh', 'chromium/src/out/Base/args.gn']
        for name in files: put(self.root, name, 'MOCK live oracle input: ' + name + '\n')
        for framework in [self.root / 'chromium/src/out/Base/Chromium.app/Contents/Frameworks/ArcCore.framework',
                          self.root.parent / 'arc/build/ArcModAgent-ours156.app/Contents/Frameworks/ArcCore.framework']:
            for leaf in ['Versions/A/ArcCore', 'Versions/A/Resources/resources.pak', 'Helpers/Fixture.app/Contents/MacOS/Fixture']:
                put(framework, leaf, 'MOCK binary fixture\n')
            put(framework, 'Helpers/Fixture.app/Contents/Info.plist', plistlib.dumps({'CFBundleExecutable':'Fixture'}))
        app = self.root.parent / 'arc/build/ArcModAgent-ours156.app'
        put(app, 'Contents/MacOS/Arc', 'MOCK app\n'); put(app, 'Contents/Info.plist', plistlib.dumps({'CFBundleExecutable':'Arc'}))
        put(self.root, 'tests/recorder/ArcEngineRecorder.dylib', 'MOCK recorder\n')
        binary = self.m.binary_inventory(self.root)
        c = self.m.template(); c['status'] = 'SEALED'; c['repo'] = str(self.root)
        c.update(proposal='inputs/successor.patch', review='inputs/review.txt', parentState='inputs/parent-state.json')
        pins = c['pins']
        for name in [c['proposal'], c['review'], c['parentState'], 'inputs/ack.txt', 'arc/browser/webextras_generator_adoption.py',
                     'arc/tools/locked.sh', 'sync-chromium.sh', 'inputs/regen-patches.sh', 'inputs/regen-chromium-patches.py']:
            pins[name] = sha(self.root / name)
        for name in c['executables'].values(): pins[name] = sha(name)
        for name, image in c['tools'].items():
            image['blob'] = 'inputs/' + Path(name).name; image['after'] = pins[image['blob']]; image['identity'] = self.m.identity(self.root / name)
        c['ack'] = dict(reference='FIXTURE_ONLY_NOT_OWNER_GRANT', path='inputs/ack.txt', sha256=pins['inputs/ack.txt'],
                        tools={n: i['after'] for n, i in c['tools'].items()})
        guarded = files + self.m.binary_paths(binary) + ['patches/MANIFEST', 'patches/0001-native.patch', 'patches/0002-foreign.patch',
                           'chromium/src/native.cc', 'chromium/src/foreign.cc']
        c['guards'] = {p: self.m.identity(self.root / p) for p in guarded}
        c['productionInventory'] = {folder: sorted(str(p.relative_to(self.root)) for p in (self.root / folder).rglob('*')
                                                  if p.is_file() and p.suffix in ('.mm', '.cc', '.h', '.gn', '.gni'))
                                    for folder in ['arc/api', 'arc/browser', 'arc/wp']}
        c['patchInventory'] = ['patches/0001-native.patch', 'patches/0002-foreign.patch']
        c['binaryGuards'] = binary
        for name, repo in [('.', self.root), ('chromium/src', self.native)]:
            index = str((repo / '.git/index').relative_to(self.root)); snapshot = 'inputs/' + ('root' if name == '.' else 'native') + '.index'
            put(self.root, snapshot, (self.root / index).read_bytes()); pins[snapshot] = sha(self.root / snapshot)
            c['gitRepos'].append(dict(path=name, index=index, snapshot=snapshot, head=self.git(repo, 'rev-parse', 'HEAD').decode().strip(),
                                     identity=self.m.identity(self.root / index), logicalSeal=list(self.m.logical_index_seal((self.root / snapshot).read_bytes()))))
        for repo in c['gitRepos']:
            cwd = self.root if repo['path'] == '.' else self.root / repo['path']
            names = ['.git/config', '.git/info/attributes', '.git/info/exclude', '.gitattributes', '.gitignore']
            raw = self.git(cwd, 'config', '--null', '--show-origin', '--list')
            c['gitMetadata'][repo['path']] = dict(configSHA256=hashlib.sha256(raw).hexdigest(),
                paths={str((cwd / name).relative_to(self.root)):self.m.identity(cwd / name) for name in names})
        self.config = c; self.out = self.root / 'logs/wp/webextras-generator-adoption-private/runs/case'; self.out.mkdir(parents=True)
        self.m.write(self.out / 'config.json', c)
        self.m.write(self.out / 'launch.json', dict(configSHA256=sha(self.out / 'config.json'), nonce='fixture-launch-nonce'))
        self.mode = None; self.calls = []
        fixture = self
        class FakeBackend:
            groups = set()
            def __init__(self): self.receipts = {}
            def inventory(self): return []
            def command(self, argv, label, cwd):
                fixture.calls.append(label)
                if label == 'official-regeneration' and fixture.mode:
                    result = types.SimpleNamespace(returncode=fixture.mode(), stdout=b'', stderr=b'')
                else:
                    result = fixture.run(argv, cwd)
                self.receipts[label] = fixture.m.write(fixture.out / (label + '.child.json'), {'state': 'DRAINED', 'argv': argv, 'pgid': 987654, 'pid': 987654, 'exit': result.returncode})
                put(fixture.out, label + '.log', result.stdout + result.stderr)
                return result.returncode
        self.backend = FakeBackend()
        class Tx(self.m.Transaction):
            def lock_check(self):
                stamp = [100, 200, 300, 'fixture-protected-owner']
                if self.lock is not None: fixture.m.require(self.lock == stamp, 'fixture lock drift')
                self.lock = stamp
            def stopped(self):
                fixture.m.require(not getattr(fixture, 'survivor', False), 'fixture live group')
            def hold(self, reason): raise Held(str(reason))
        self.Tx = Tx

    def run(self, argv, cwd, env=None):
        cwd = Path(cwd).resolve(); assert cwd.is_relative_to(Path(self.tmp.name).resolve())
        result = subprocess.run([str(x) for x in argv], cwd=cwd, env=env or self.m.APP8, capture_output=True)
        COMMANDS.append(dict(argv=[str(x) for x in argv], cwd=str(cwd), exit=result.returncode))
        return result

    def git(self, cwd, *args):
        result = self.run(['/usr/bin/git', *args], cwd); assert result.returncode == 0, result.stderr
        return result.stdout

    def tx(self): return self.Tx(self.config, self.out, self.backend)

    def admission(self, mutate=None):
        guardian = self.tx()
        def parent(_):
            request = self.m.load(self.out / 'admission-request.json')
            admission = {k: request[k] for k in ('action', 'requestNonce', 'launchNonce', 'configSHA256', 'lock', 'guardianPID', 'heldState')}
            admission.update(decision='ADMIT', ready=True, requestSHA256=sha(self.out / 'admission-request.json'))
            if mutate: mutate(admission)
            self.m.write(self.out / 'admission.json', admission)
        with patch.object(self.m.time, 'sleep', side_effect=parent): guardian.await_admission()
        return guardian

    def worker(self, tx=None):
        tx = tx or self.tx()
        nonce = '1' * 32
        intent = dict(guardianPID=os.getpid(), workerNonce=nonce, configSHA256=sha(self.out / 'config.json'),
                      launchNonce='fixture-launch-nonce', requestSHA256=sha(self.out / 'admission-request.json'))
        self.m.write(self.out / 'worker-intent.json', intent)
        self.m.write(self.out / 'worker.json', {**intent, 'pid':os.getpid()})
        with patch.object(self.m.os, 'getppid', return_value=os.getpid()):
            return tx.execute(nonce)

    def execute(self):
        self.guardian = self.admission(); self.guardian.worker_nonce = '1' * 32
        code = self.worker()
        self.guardian.certify(types.SimpleNamespace(returncode=code, pid=os.getpid())); return code

    def close(self): self.tmp.cleanup()


class AdoptionTests(unittest.TestCase):
    def setUp(self): self.f = Fixture()
    def tearDown(self): self.f.close()

    def test_unsealed_refuses_without_processes_or_capture(self):
        c = self.f.m.template()
        with self.assertRaisesRegex(RuntimeError, 'UNSEALED'): self.f.m.validate(c, self.f.root)
        self.assertFalse(self.f.calls); self.assertFalse((self.f.out / 'tool-journal.json').exists())
        self.assertIsNone(c['ack']); self.assertIsNone(c['tools'][self.f.m.HELPER]['after'])

    def test_real_copied_successor_entire_adoption_and_same_guardian(self):
        self.assertEqual(self.f.execute(), 0)
        record = self.f.m.load(self.f.out / 'adopted.json'); self.assertTrue(record['laterEngineRollbackMustRetainTools'])
        self.assertEqual(set(record['postimages']), set(self.f.m.TOOLS))
        self.assertEqual(self.f.calls[-1], 'official-regeneration')
        self.assertEqual([self.f.m.load(p)['exit'] for p in self.f.out.glob('lease-*.json')], [0, 0, 0])
        self.assertTrue((self.f.out / 'release-safe.json').exists())
        self.assertEqual(self.f.guardian.preworker, self.f.m.load(self.f.out / 'guardian-preworker.json'))

    def test_no_parent_admission_never_starts_worker_or_oracle(self):
        tx = self.f.tx()
        with patch.object(self.f.m.time, 'sleep', side_effect=NoAdmission('no parent receipt')):
            with self.assertRaises(NoAdmission): tx.await_admission()
        self.assertFalse(self.f.calls); self.assertFalse((self.f.out / 'tool-journal.json').exists())
        self.assertFalse((self.f.out / 'lease-failure.json').exists()); tx.tools_before()

    def test_prewritten_and_stale_admissions_refuse(self):
        self.f.m.write(self.f.out / 'admission.json', {'ready': True})
        with self.assertRaisesRegex(RuntimeError, 'prewritten'): self.f.tx().await_admission()
        (self.f.out / 'admission.json').unlink()
        with self.assertRaisesRegex(RuntimeError, 'stale/malformed'):
            self.f.admission(lambda a: a.update(requestNonce='old-parent-reaped-run'))
        self.assertFalse(self.f.calls)

    def test_nonboolean_ready_and_wrong_action_refuse(self):
        for mutate in [lambda a: a.update(ready=1), lambda a: a.update(action='RUN_ENGINE')]:
            for name in ['admission.json', 'admission-request.json']:
                (self.f.out / name).unlink(missing_ok=True)
            with self.assertRaises(RuntimeError): self.f.admission(mutate)
        self.assertFalse(self.f.calls)

    def test_held_state_change_before_admission_preserved(self):
        def mutation(a): put(self.f.root, self.f.config['parentState'], 'parent reaped/refused; new held state')
        with self.assertRaisesRegex(RuntimeError, 'pin drift'): self.f.admission(mutation)
        self.assertFalse(self.f.calls); self.assertIn('parent reaped', (self.f.root / self.f.config['parentState']).read_text())

    def test_canonical_disagreement_refuses_before_tools(self):
        p = self.f.root / 'patches/0002-foreign.patch'; p.write_text('accepted different labels\n')
        self.f.config['guards']['patches/0002-foreign.patch'] = self.f.m.identity(p)
        self.f.m.write(self.f.out / 'config.json', self.f.config)
        self.f.m.write(self.f.out / 'launch.json', dict(configSHA256=sha(self.f.out / 'config.json'), nonce='fixture-launch-nonce'))
        self.assertEqual(self.f.execute(), 1); self.assertEqual(p.read_text(), 'accepted different labels\n')
        self.assertFalse((self.f.out / 'tool-journal.json').exists()); self.assertNotIn('official-regeneration', self.f.calls)

    def test_ordinary_generator_failure_reverts_only_tools(self):
        self.f.mode = lambda: 7
        self.assertEqual(self.f.execute(), 1)
        self.assertEqual(sha(self.f.root / self.f.m.WRAPPER), self.f.m.BEFORE_WRAPPER)
        self.assertFalse((self.f.root / self.f.m.HELPER).exists())
        leases = [self.f.m.load(p) for p in self.f.out.glob('lease-official-*.json')]
        self.assertEqual(leases[0]['exit'], 7); self.assertIn('CommandFailure', leases[0]['error'])
        self.assertEqual(self.f.calls.count('official-regeneration'), 1)

    def test_partial_publication_second_replace_failure_reverts(self):
        self.f.admission(); worker = self.f.tx(); original = self.f.m.os.replace
        def replace(a, b):
            if Path(b) == self.f.root / self.f.m.WRAPPER and '.adopt-' in Path(a).name:
                raise OSError('fixture second tool rename failure')
            return original(a, b)
        with patch.object(self.f.m.os, 'replace', side_effect=replace): self.assertEqual(self.f.worker(worker), 1)
        self.assertFalse((self.f.root / self.f.m.HELPER).exists()); self.assertEqual(sha(self.f.root / self.f.m.WRAPPER), self.f.m.BEFORE_WRAPPER)
        self.assertNotIn('official-regeneration', self.f.calls)

    def test_foreign_same_bytes_helper_replacement_is_preserved(self):
        self.f.admission(); worker = self.f.tx(); original = self.f.m.write; replaced = []
        def write(path, value):
            result = original(path, value)
            if Path(path).name == 'tool-journal.json' and len(value['published']) == 1 and not replaced:
                target = self.f.root / self.f.m.HELPER; foreign = target.with_name('foreign-copy')
                shutil.copyfile(target, foreign); foreign.replace(target); replaced.append(self.f.m.identity(target))
            return result
        with patch.object(self.f.m, 'write', side_effect=write):
            with self.assertRaisesRegex(Held, 'foreign tool'): self.f.worker(worker)
        self.assertEqual(self.f.m.identity(self.f.root / self.f.m.HELPER), replaced[0])
        self.assertEqual(sha(self.f.root / self.f.m.WRAPPER), self.f.m.BEFORE_WRAPPER)

    def test_foreign_native_patch_mode_and_binary_conflicts_hold_without_restore(self):
        for relative in ['chromium/src/foreign.cc', 'patches/0002-foreign.patch', 'chromium/src/out/Base/Chromium.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore']:
            f = Fixture()
            try:
                p = f.root / relative
                def fail(): p.write_text('FOREIGN preserved\n'); return 8
                f.mode = fail; f.admission()
                with self.assertRaises(Held): f.worker()
                self.assertEqual(p.read_text(), 'FOREIGN preserved\n'); self.assertFalse((f.out / 'adopted.json').exists())
            finally: f.close()
        self.f.mode = lambda: ((self.f.root / 'chromium/src/foreign.cc').chmod(0o755) or 8)
        self.f.admission()
        with self.assertRaises(Held): self.f.worker()
        self.assertEqual((self.f.root / 'chromium/src/foreign.cc').stat().st_mode & 0o777, 0o755)

    def test_raw_only_index_drift_latches_and_blocks_new_guardian(self):
        index = self.f.root / self.f.config['gitRepos'][1]['index']
        original = index.read_bytes()
        def drift():
            p = index.with_name('same-byte-foreign-index'); p.write_bytes(original); p.replace(index); return 7
        self.f.mode = drift; self.f.admission()
        with self.assertRaises(Held): self.f.worker()
        self.assertEqual(index.read_bytes(), original); self.assertTrue((self.f.out / 'lease-failure.json').exists())
        with self.assertRaisesRegex(RuntimeError, 'lease failure'): self.f.tx().clear()

    def test_failed_latch_storage_still_open_blocks_new_guardian(self):
        index = self.f.root / self.f.config['gitRepos'][1]['index']; original = index.read_bytes()
        self.f.mode = lambda: (index.write_bytes(original + b'foreign') or 7)
        self.f.admission(); actual_write = self.f.m.write
        def write(path, value):
            if Path(path).name == 'lease-failure.json': raise OSError('fixture latch write failure')
            return actual_write(path, value)
        with patch.object(self.f.m, 'write', side_effect=write):
            with self.assertRaises(Held): self.f.worker()
        self.assertTrue(any(self.f.m.load(p)['state'] == 'OPEN' for p in self.f.out.glob('lease-*.json')))
        with self.assertRaisesRegex(RuntimeError, 'OPEN'): self.f.tx().clear()

    def test_admission_drift_after_publication_holds(self):
        self.f.admission()
        def change(): self.f.m.write(self.f.out / 'admission.json', {'decision': 'REAPED_NOT_ADMITTED'}); return 0
        self.f.mode = change
        with self.assertRaisesRegex(Held, 'admission'): self.f.worker()
        self.assertFalse((self.f.out / 'adopted.json').exists())

    def test_commit_receipt_write_failure_never_rolls_back_or_certifies(self):
        self.f.admission(); original = self.f.m.write
        def write(path, value):
            if Path(path).name == 'adopted.json': raise OSError('fixture commit receipt failure')
            return original(path, value)
        with patch.object(self.f.m, 'write', side_effect=write):
            with self.assertRaises(Held): self.f.worker()
        self.assertEqual(sha(self.f.root / self.f.m.HELPER), self.f.config['tools'][self.f.m.HELPER]['after'])
        self.assertFalse((self.f.out / 'reverted.json').exists())

    def test_guardian_rejects_foreign_postimage_missing_completion_and_live_group(self):
        self.assertEqual(self.f.execute(), 0)
        p = self.f.root / self.f.m.HELPER; temp = p.with_name('foreign-identical'); shutil.copyfile(p, temp); temp.replace(p)
        with self.assertRaisesRegex(RuntimeError, 'postimage'): self.f.guardian.certify(types.SimpleNamespace(returncode=0, pid=os.getpid()))
        (self.f.out / 'completion.json').unlink()
        with self.assertRaises(RuntimeError): self.f.guardian.certify(types.SimpleNamespace(returncode=0, pid=os.getpid()))
        self.f.survivor = True
        with self.assertRaisesRegex(RuntimeError, 'live group'): self.f.guardian.certify(types.SimpleNamespace(returncode=0, pid=os.getpid()))

    def test_attributed_parser_exact_and_semantic_flags_protected(self):
        source = (self.f.root / 'arc/browser/webextras_generator_adoption.py').read_text()
        n = next(n for n in ast.parse(source).body if isinstance(n, ast.FunctionDef) and n.name == 'logical_index_seal')
        exact = ''.join(source.splitlines(True)[n.lineno - 1:n.end_lineno]); self.assertEqual(hashlib.sha256(exact.encode()).hexdigest(), self.f.m.PARSER_SHA)
        before = self.f.config['gitRepos'][1]['logicalSeal']
        self.f.git(self.f.native, 'update-index', '--assume-unchanged', 'native.cc')
        after = self.f.m.logical_index_seal((self.f.native / '.git/index').read_bytes()); self.assertNotEqual(list(after), before)
        with self.assertRaisesRegex(RuntimeError, 'index drift'): self.f.tx().indices()

    def test_split_shared_index_refused_without_git_probe(self):
        index = self.f.root / self.f.config['gitRepos'][1]['index']; put(index.parent, 'sharedindex.stale', b'preserve')
        with patch.object(self.f.m.subprocess, 'check_output', side_effect=AssertionError('no Git probe')):
            with self.assertRaisesRegex(RuntimeError, 'shared indices'): self.f.tx().indices(discover=False)
        self.assertEqual((index.parent / 'sharedindex.stale').read_bytes(), b'preserve')

    def test_canonical_never_calls_publishing_generator(self):
        self.f.admission(); tx = self.f.tx(); tx.restore_admission(); tx.worker_nonce = '1' * 32; self.f.m.write(self.f.out / 'worker-intent.json', {'workerNonce':tx.worker_nonce}); module = tx.helper()
        module.generate = module.regenerate = lambda *a: self.fail('publishing dry-run called')
        with patch.object(tx, 'helper', return_value=module):
            self.assertEqual(set(tx.lease('readonly-control', tx.canonical)), {'0001-native.patch', '0002-foreign.patch'})
        tx.tools_before(); self.assertNotIn('official-regeneration', self.f.calls)

    def test_wrong_successor_environment_and_scope_refuse(self):
        self.f.admission(); tx = self.f.tx(); tx.restore_admission()
        module = tx.helper(); module.ENV = {'PATH': '/untrusted'}
        # helper() validates its own pinned source. A changed source requires a new
        # owner grant/pin; never mutate the real successor or303 for this control.
        path = self.f.root / self.f.config['tools'][self.f.m.HELPER]['blob']; path.write_text(path.read_text() + '\nENV = {}\n')
        with self.assertRaisesRegex(RuntimeError, 'helper image drift'): tx.helper()
        c = copy.deepcopy(self.f.config); c['tools']['arc/tools/other.py'] = {}
        with self.assertRaisesRegex(RuntimeError, 'scope|environment'): self.f.m.validate(c, self.f.root)

    def test_complete_evidence_deletion_and_forged_prefix_cannot_release(self):
        self.assertEqual(self.f.execute(), 0)
        guardian = self.f.guardian; child = types.SimpleNamespace(pid=os.getpid(), returncode=0)
        for pattern in ['lease-official-*.json', 'official-regeneration.child.json']:
            path = next(self.f.out.glob(pattern)); saved = path.read_bytes(); path.unlink()
            with self.assertRaises(RuntimeError): guardian.certify(child)
            path.write_bytes(saved)
        path = self.f.out / 'completion.json'; row = self.f.m.load(path)
        row['commands'] = ['official-regeneration']; self.f.m.write(path, row)
        with self.assertRaisesRegex(RuntimeError, 'operation order'): guardian.certify(child)

    def test_wrong_worker_parent_nonce_pid_and_receipt_prevent_work(self):
        self.f.admission(); tx = self.f.tx(); tx.restore_admission()
        for nonce in [None, 'short']:
            with self.assertRaisesRegex(RuntimeError, 'guardian'): tx.worker_gate(nonce)
        nonce = '1' * 32
        with patch.object(self.f.m.os, 'getppid', return_value=-1):
            with self.assertRaisesRegex(RuntimeError, 'guardian'): tx.worker_gate(nonce)
        with patch.object(self.f.m.os, 'getppid', return_value=os.getpid()):
            with self.assertRaisesRegex(RuntimeError, 'missing receipt'): tx.worker_gate(nonce)
        self.assertFalse(self.f.calls)
        self.assertEqual(self.f.worker(), 0)
        self.f.guardian = self.f.tx(); self.f.guardian.restore_admission()
        self.f.guardian.preworker = self.f.m.load(self.f.out / 'guardian-preworker.json')
        self.f.guardian.worker_nonce = nonce
        with self.assertRaisesRegex(RuntimeError, 'handoff'):
            self.f.guardian.certify(types.SimpleNamespace(pid=-1, returncode=0))

    def test_required_binary_roles_and_helper_inventory_are_enforced(self):
        c = copy.deepcopy(self.f.config); c['binaryGuards']['built']['engine'] = str(self.f.root / 'arc/BUILD.gn')
        with self.assertRaisesRegex(RuntimeError, 'binary roles'): self.f.m.validate(c, self.f.root)
        self.f.admission(); framework = self.f.root / 'chromium/src/out/Base/Chromium.app/Contents/Frameworks/ArcCore.framework'
        put(framework, 'Helpers/Foreign.app/Contents/Info.plist', plistlib.dumps({'CFBundleExecutable':'Foreign'}))
        put(framework, 'Helpers/Foreign.app/Contents/MacOS/Foreign', 'FOREIGN preserved')
        with self.assertRaisesRegex(RuntimeError, 'binary roles|inventory drift'): self.f.tx().base_guard()
        self.assertEqual((framework / 'Helpers/Foreign.app/Contents/MacOS/Foreign').read_text(), 'FOREIGN preserved')

    def test_immediate_foreign_tool_before_official_spawn_is_preserved(self):
        self.f.admission(); tx = self.f.tx(); actual_lease = tx.lease
        def lease(label, operation):
            if label == 'official':
                target = self.f.root / self.f.m.HELPER; temp = target.with_name('foreign-tool')
                shutil.copyfile(target, temp); temp.replace(target)
            return actual_lease(label, operation)
        with patch.object(tx, 'lease', side_effect=lease):
            with self.assertRaisesRegex(Held, 'foreign'): self.f.worker(tx)
        self.assertNotIn('official-regeneration', self.f.calls)
        self.assertTrue((self.f.root / self.f.m.HELPER).exists())

    def test_real_hold_loop_never_releases_on_receipt_postproof_error(self):
        class StopTest(BaseException): pass
        writes = []
        def write(*args): writes.append(1); raise RuntimeError('fixture postproof')
        def sleep(*args):
            if len(writes) >= 3: raise StopTest()
        with patch.object(self.f.m, 'write', side_effect=write), patch.object(self.f.m.time, 'sleep', side_effect=sleep):
            with self.assertRaises(StopTest): self.f.m.hold_forever(self.f.out, 'uncertain')
        self.assertEqual(len(writes), 3)

    def test_actual_protected_main_ignores_signals_before_exec_existing_lock(self):
        m = self.f.m; c = self.f.out / 'config.json'
        class ExecObserved(BaseException): pass
        calls = []
        def signal_call(*args): calls.append(('signal', args))
        def exec_call(path, argv):
            calls.append(('exec', path, argv)); raise ExecObserved()
        argv = ['driver', '_protected', '--config', str(c), '--config-sha', sha(c), '--out', str(self.f.out)]
        with patch.object(m.sys, 'argv', argv), patch.object(m.signal, 'signal', side_effect=signal_call), \
             patch.object(m.os, 'execv', side_effect=exec_call):
            with self.assertRaises(ExecObserved): m.main()
        self.assertEqual([v[1] for v in calls[:-1]], [(v,m.signal.SIG_IGN) for v in m.SIGNALS])
        self.assertEqual(calls[-1][1], '/bin/bash'); self.assertEqual(calls[-1][2][1], str(self.f.root / 'arc/tools/locked.sh'))
        self.assertFalse(self.f.calls)

    def test_cancel_after_publish_reverts_only_tools_and_never_runs_official(self):
        self.f.admission(); tx = self.f.tx(); publish = tx.publish
        def cancel(): publish(); (self.f.out / 'cancel.request').touch()
        with patch.object(tx, 'publish', side_effect=cancel):
            self.assertEqual(self.f.worker(tx), 1)
        self.assertNotIn('official-regeneration', self.f.calls)

    def test_git_config_attribute_presence_absence_and_external_layout_controls(self):
        self.f.admission(); tx = self.f.tx(); tx.restore_admission(); self.assertTrue(tx.metadata())
        for relative in ['chromium/src/.gitattributes', 'chromium/src/.gitignore', 'chromium/src/.git/info/attributes',
                         'chromium/src/.git/info/exclude', 'chromium/src/.git/config']:
            path = self.f.root / relative; before = path.read_bytes() if path.exists() else None
            path.parent.mkdir(parents=True, exist_ok=True); path.write_bytes((before or b'') + b'\n# foreign metadata\n')
            with self.assertRaisesRegex(RuntimeError, 'metadata drift'): tx.metadata()
            if before is None: path.unlink()
            else: path.write_bytes(before)
            # Restoring bytes cannot restore physical identities. This is a test
            # reseal only; the production driver never restores Git metadata.
            for repo in self.f.config['gitRepos']:
                guard = self.f.config['gitMetadata'][repo['path']]['paths']
                if relative in guard: guard[relative] = self.f.m.identity(path)
        self.f.git(self.f.native, 'config', 'core.attributesFile', str(self.f.root / 'external.attributes'))
        with self.assertRaisesRegex(RuntimeError, 'unsupported'): tx.metadata()

    def test_late_config_drift_during_official_latches_hold_preserving_metadata(self):
        self.f.admission(); path = self.f.native / '.git/info/attributes'
        self.f.mode = lambda: (path.write_text('*.cc diff=FOREIGN\n') and 0)
        with self.assertRaisesRegex(Held, 'metadata'): self.f.worker()
        self.assertEqual(path.read_text(), '*.cc diff=FOREIGN\n')
        self.assertTrue((self.f.out / 'lease-failure.json').exists())
        self.assertFalse((self.f.out / 'adopted.json').exists())

    def test_saved_operation_hashes_reject_precompletion_tampering(self):
        self.f.admission(); tx = self.f.tx(); finish = tx.finish
        def tamper(state):
            path = self.f.out / 'official-regeneration.child.json'
            row = self.f.m.load(path); row['exit'] = 7; self.f.m.write(path, row)
            return finish(state)
        with patch.object(tx, 'finish', side_effect=tamper):
            with self.assertRaisesRegex(Held, 'proof changed'): self.f.worker(tx)
        self.assertFalse((self.f.out / 'release-safe.json').exists())

    def test_guardian_adopted_requires_success_and_crossbinding_even_if_rehashed(self):
        self.assertEqual(self.f.execute(), 0); guardian = self.f.guardian
        child = types.SimpleNamespace(pid=os.getpid(), returncode=0)
        cases = [('official-regeneration.child.json', 'exit', 7),
                 (self.f.m.load(self.f.out / 'completion.json')['leases']['official'], 'error', 'foreign error'),
                 (self.f.m.load(self.f.out / 'completion.json')['leases']['official'], 'label', 'canonical-before'),
                 ('official-regeneration.child.json', 'argv', ['OTHER'])]
        for name, key, value in cases:
            path = self.f.out / name; original = path.read_bytes()
            row = self.f.m.load(path); row[key] = value; self.f.m.write(path, row)
            complete = self.f.m.load(self.f.out / 'completion.json'); complete['evidence'][name] = sha(path)
            self.f.m.write(self.f.out / 'completion.json', complete)
            with self.assertRaises(RuntimeError): guardian.certify(child)
            path.write_bytes(original)

    def test_actual_guardian_main_admission_then_spawn_and_same_guardian_certify(self):
        m = self.f.m; events = []; tx = self.f.tx()
        def admit():
            events.append('admission'); tx.request_sha = 'request'; tx.config_sha = sha(self.f.out / 'config.json')
        tx.await_admission = admit
        child = types.SimpleNamespace(pid=987654, returncode=0, wait=lambda: events.append('reap'))
        def spawn(argv, **kwargs):
            self.assertEqual(events, ['admission'])
            self.assertTrue((self.f.out / 'worker-intent.json').exists()); self.assertIn('--worker-nonce', argv)
            events.append('spawn'); return child
        def certify(observed):
            self.assertIs(observed, child); self.assertEqual(events, ['admission', 'spawn', 'reap'])
            self.assertEqual(m.load(self.f.out / 'worker.json')['pid'], child.pid); events.append('certify-same')
        tx.certify = certify
        argv = ['driver', '_guardian', '--config', str(self.f.out / 'config.json'), '--config-sha', sha(self.f.out / 'config.json'), '--out', str(self.f.out)]
        with patch.object(m.sys, 'argv', argv), patch.object(m, 'Transaction', return_value=tx) as ctor, \
             patch.object(m.subprocess, 'Popen', side_effect=spawn), patch.object(m.signal, 'signal'):
            self.assertEqual(m.main(), 0)
        self.assertEqual(ctor.call_count, 1); self.assertEqual(events[-1], 'certify-same'); self.assertFalse(self.f.calls)

    def test_guardian_bootstrap_failure_enters_hold_before_any_spawn(self):
        m = self.f.m
        class HoldObserved(BaseException): pass
        argv = ['driver', '_guardian', '--config', str(self.f.out / 'config.json'), '--config-sha', 'wrong', '--out', str(self.f.out)]
        with patch.object(m.sys, 'argv', argv), patch.object(m, 'hold_forever', side_effect=HoldObserved()) as hold, \
             patch.object(m.subprocess, 'Popen', side_effect=AssertionError('must not spawn')):
            with self.assertRaises(HoldObserved): m.main()
        self.assertEqual(hold.call_count, 1); self.assertEqual(hold.call_args.args[0], self.f.out)

    def test_noncanonical_sealed_config_rejected_before_launch_or_lock(self):
        m = self.f.m; config = self.f.out / 'noncanonical.json'; config.write_text(json.dumps(self.f.config))
        output = self.f.out.parent / 'must-not-create'
        argv = ['driver', 'run', '--config', str(config), '--config-sha', sha(config), '--out', str(output)]
        with patch.object(m.sys, 'argv', argv), patch.object(m.subprocess, 'Popen', side_effect=AssertionError('no launch')):
            with self.assertRaisesRegex(RuntimeError, 'canonical JSON'): m.main()
        self.assertFalse(output.exists()); self.assertFalse(self.f.calls)

    def test_copied_successor_nonracy_control_also_uses_actual_oracles(self):
        fixture = Fixture(racy=False)
        try:
            self.assertEqual(fixture.execute(), 0)
            self.assertEqual(set(fixture.m.load(fixture.out / 'adopted.json')['canonical']), {'0001-native.patch', '0002-foreign.patch'})
        finally: fixture.close()

    def test_nested_owned_leaf_intermediate_attribute_and_ignore_are_guarded(self):
        f = self.f; m = f.m; relative = 'chromium/src/vendor/child'; nested = f.root / relative; nested.mkdir(parents=True)
        f.git(nested, 'init', '-q'); f.git(nested, 'config', 'user.name', 'Fixture'); f.git(nested, 'config', 'user.email', 'fixture@invalid')
        put(nested, 'inner/leaf.cc', 'fixture\n'); f.git(nested, 'add', 'inner/leaf.cc'); f.git(nested, 'commit', '-qm', 'nested baseline')
        index = relative + '/.git/index'; snapshot = 'inputs/nested.index'; put(f.root, snapshot, (f.root / index).read_bytes())
        f.config['pins'][snapshot] = sha(f.root / snapshot)
        f.config['gitRepos'].append(dict(path=relative, index=index, snapshot=snapshot, head=f.git(nested, 'rev-parse', 'HEAD').decode().strip(),
            identity=m.identity(f.root / index), logicalSeal=list(m.logical_index_seal((f.root / snapshot).read_bytes()))))
        names = ['.git/config', '.git/info/attributes', '.git/info/exclude', '.gitattributes', '.gitignore', 'inner/.gitattributes', 'inner/.gitignore']
        raw = f.git(nested, 'config', '--null', '--show-origin', '--list')
        f.config['gitMetadata'][relative] = dict(configSHA256=hashlib.sha256(raw).hexdigest(),
            paths={relative + '/' + name:m.identity(nested / name) for name in names})
        manifest = f.root / 'patches/MANIFEST'; manifest.write_text(manifest.read_text() + '0001-native.patch vendor/child/inner/leaf.cc\n')
        f.config['guards']['patches/MANIFEST'] = m.identity(manifest)
        m.write(f.out / 'config.json', f.config); m.write(f.out / 'launch.json', dict(configSHA256=sha(f.out / 'config.json'), nonce='fixture-launch-nonce'))
        f.admission(); tx = f.tx(); tx.restore_admission(); self.assertTrue(tx.metadata())
        for leaf in ['inner/.gitattributes', 'inner/.gitignore']:
            path = put(nested, leaf, '# FOREIGN preserved\n')
            with self.assertRaisesRegex(RuntimeError, 'metadata drift'): tx.metadata()
            self.assertEqual(path.read_text(), '# FOREIGN preserved\n'); path.unlink()

    def test_actual_helper_launcher_runtime_is_pinned_after_admission(self):
        self.f.admission(); tx = self.f.tx(); tx.restore_admission(); actual = self.f.run
        def wrong(argv, *args, **kwargs):
            if argv[0] == '/usr/bin/env': return types.SimpleNamespace(returncode=0,stdout=b'/untrusted/python3\n',stderr=b'')
            return actual(argv, *args, **kwargs)
        with patch.object(self.f, 'run', side_effect=wrong):
            with self.assertRaisesRegex(RuntimeError, 'runtime selection drift'): tx.runtime()
        self.assertEqual(self.f.calls, ['runtime-selection']); self.assertFalse((self.f.out / 'tool-journal.json').exists())

    def test_entire_backend_drain_survives_postspawn_receipt_failure(self):
        m = self.f.m; backend = m.Backend(self.f.out); child = types.SimpleNamespace(pid=12345, returncode=9, wait=lambda: 9)
        inventories = [[dict(pgid=12345, state='S')], []]; original = m.write
        def write(path, value):
            if value.get('state') == 'RUNNING': raise OSError('postspawn bookkeeping failure')
            return original(path, value)
        with patch.object(m.subprocess, 'Popen', return_value=child), patch.object(m, 'write', side_effect=write), \
             patch.object(backend, 'inventory', side_effect=inventories), patch.object(m.time, 'sleep'):
            with self.assertRaisesRegex(OSError, 'postspawn'): backend.command(['FIXTURE_NOT_EXECUTED'], 'drain-control', self.f.root)
        self.assertEqual(m.load(self.f.out / 'drain-control.child.json')['state'], 'SPAWNING')
        self.assertIn(12345, backend.groups)


def main():
    global ARTIFACTS
    p = argparse.ArgumentParser(); p.add_argument('--out', type=Path, required=True); args = p.parse_args()
    ARTIFACTS = args.out.resolve(); ARTIFACTS.mkdir(parents=True, exist_ok=True)
    before = {str(p): sha(p) for p in [ROOT / 'arc/tools/regen-patches.sh', ROOT / 'arc/browser/webextras_initial_integration.py',
                                     ROOT / 'arc/browser/webextras_nested_regeneration_lead_rebase_reviewed.patch']}
    result = unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromTestCase(AdoptionTests))
    assert before == {name: sha(name) for name in before}; assert not os.path.lexists(ROOT / 'arc/tools/regen-chromium-patches.py')
    (ARTIFACTS / 'results.json').write_text(json.dumps({'testsRun': result.testsRun, 'success': result.wasSuccessful(),
        'failures': len(result.failures), 'errors': len(result.errors), 'copiedFixtureCommands': COMMANDS,
        'productionPinsUnchanged': before, 'actualProductionOracleOrAdoption': False,
        'fixtureSuccessorHelper': sha(EVIDENCE / 'staged/arc/tools/regen-chromium-patches.py'),
        'limits': 'mock lock/census/current native-build-binary inputs + synthetic ACK/config; copied real successor/Git only in disposable repos; no production approval'}, indent=2) + '\n')
    return 0 if result.wasSuccessful() else 1


if __name__ == '__main__': sys.exit(main())
