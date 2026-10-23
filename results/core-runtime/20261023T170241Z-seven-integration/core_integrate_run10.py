"""Seven reviewed patches only. Run under ONE ancestor locked.sh; parent decides retention."""
from pathlib import Path
import argparse, json, os, shutil, stat, subprocess, sys, time, uuid
from core_runtime_run10 import digest, dump, require_outer_lock, agent_processes, arc_seal, input_seal

MANIFEST_SHA = 'bb7354ba6fbb1869cd100bed2c190d72b2f920807f6b66437ff56e4b47b2347b'
STEPS_SHA = 'c270bb21a39f69598aa83cdd286569e8a0e9706ddb44b8f6ef19338714b016fb'
ORDER = [
 ('native', 'core_navigation_chromium.patch', '2f423c8f1a48cf8d93f44f26ade9f57c3f2903d01a12ee8b7bca0cf63af3c83b'),
 ('native', 'core_close_transport_chromium.patch', '41de166d504e1a4e671af035398ad8940adc23be8bad313af9813935d72d42e0'),
 ('native', 'core_hosted_empty_browser.patch', '95c46cc5e1118fda12c1b2aa1ff7f723201ff173086df51aa4a59fd54930e142'),
 ('owned', 'core_navigation_registration.after_progress.patch', 'ac1b3d7e775ad68231c135ee033ac0f366c2a845ca9835d4ba7b18483ca068ea'),
 ('owned', 'core_display_registration.patch', 'e87f4d5e91315168cbf97690397a95b9af09753b61c4c97e75c768e279e278ee'),
 ('owned', 'core_observer_registration.after_progress.patch', 'b222ff87de87a4ab5ab88fa56c2f355800d8f85852beeef83ebc4fd3aa1b4c90'),
 ('native', 'core_beforeunload_transport_chromium.patch', 'e57ccbd991e538d860ed62f496d4f40046afccc27fea3e54ca3fcff0dd170625'),
]

class Transaction:
    def __init__(self, args):
        self.root = args.root.resolve()
        self.native = self.root / 'chromium/src'
        self.preflight = args.preflight.resolve()
        self.timeout = args.review_timeout
        require_outer_lock(self.root)
        if digest(self.preflight / 'manifest.json') != MANIFEST_SHA or digest(self.preflight / 'step-hashes.json') != STEPS_SHA:
            raise RuntimeError('reviewed preflight evidence changed; parent must rebase/review runner')
        self.meta = json.loads((self.preflight / 'manifest.json').read_text())
        self.steps = json.loads((self.preflight / 'step-hashes.json').read_text())
        if [(e['kind'], e['name'], e['sha256']) for e in self.meta['patches']] != ORDER:
            raise RuntimeError('seven-patch order mismatch')
        if [(e['patch'], e['sha256']) for e in self.steps] != [(n, h) for _, n, h in ORDER]:
            raise RuntimeError('per-stage hash evidence mismatch')
        self.base = {self.relative(k): v for k, v in self.meta['base'].items()}
        self.target = {self.relative(k): v for k, v in self.meta['targets'].items()}
        self.selected = sorted({self.relative(e['kind'] + '/' + p) for e in self.meta['patches'] for p in e['paths']})
        if len(self.selected) != 11:
            raise RuntimeError('unexpected mutation path inventory')
        # No production writes, generator, git mutation, profile reset, or app operation before these guards.
        self.verify(self.base)
        for kind, name, sha in ORDER:
            for p in [self.root / 'arc/browser' / name, self.preflight / 'patches' / name]:
                if p.is_symlink() or digest(p) != sha:
                    raise RuntimeError('reviewed patch drift ' + str(p))
        if agent_processes(self.root):
            raise RuntimeError('exact ours156 app exists; preflight exits without mutation')
        self.tools_expected = {r: digest(self.root / r) for r in ['arc/tools/regen-patches.sh', 'arc/tools/locked.sh']}
        if any(h is None for h in self.tools_expected.values()):
            raise RuntimeError('missing integration tool')
        self.manifest = self.root / 'patches/MANIFEST'
        ownership = self.parse_manifest()
        # Regen rewrites every owner's output. Guard its complete native input set so a
        # later exact output rollback cannot orphan unselected foreign source edits.
        for rel in ownership:
            key = 'chromium/src/' + rel
            self.base.setdefault(key, digest(self.native / rel))
            self.target.setdefault(key, self.base[key])
        self.verify(self.base)
        self.out = args.out.resolve() if args.out else self.root / 'results/core-runtime' / (time.strftime('%Y%m%dT%H%M%SZ', time.gmtime()) + '-integration10-' + uuid.uuid4().hex[:8])
        self.out.mkdir(parents=True, exist_ok=False)
        self.allowed = {}
        self.snapshots = {}
        self.mutated = False
        self.manifest_allowed = {digest(self.manifest)}
        for rel in self.selected + ['patches/MANIFEST']:
            self.snapshot(rel)
        # All regeneration outputs are archived, but never blindly restored over other work.
        for rel in self.regen_outputs():
            self.snapshot(rel)
        self.output_state = self.regen_outputs()
        git_env = {**os.environ, 'GIT_OPTIONAL_LOCKS': '0'}
        index_name = subprocess.check_output(['git', 'rev-parse', '--git-path', 'index'], cwd=self.native, env=git_env, text=True).strip()
        self.index = Path(index_name)
        if not self.index.is_absolute():
            self.index = self.native / self.index
        if self.index.is_symlink() or not self.index.is_file():
            raise RuntimeError('real Chromium index is not a regular file')
        self.index_sha = digest(self.index)
        shutil.copy2(self.index, self.out / 'chromium-index.before')
        for p in [self.preflight / 'manifest.json', self.preflight / 'step-hashes.json', Path(__file__), Path(__file__).with_name('core_runtime_run10.py')]:
            shutil.copy2(p, self.out / p.name)
        for _, name, _ in ORDER:
            dest = self.out / 'patches' / name
            dest.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(self.preflight / 'patches' / name, dest)
        dump(self.out / 'snapshots.json', self.snapshots)
        dump(self.out / 'base-sources.json', self.base)
        dump(self.out / 'integration-tools-before.json', self.tools_expected)
        for rel in self.tools_expected:
            dest = self.out / 'tools-before' / rel
            dest.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(self.root / rel, dest)
        dump(self.out / 'candidate-sources.json', self.target)
        self.verify(self.base)

    def relative(self, key):
        kind, rel = key.split('/', 1)
        if kind not in ('native', 'owned') or Path(rel).is_absolute() or '..' in Path(rel).parts:
            raise RuntimeError('unsafe source key ' + key)
        return ('chromium/src/' if kind == 'native' else '') + rel

    def verify(self, expected):
        bad = {}
        for rel, sha in expected.items():
            p = self.root / rel
            actual = digest(p)
            if p.is_symlink() or (p.exists() and not p.is_file()) or actual != sha:
                bad[rel] = {'expected': sha, 'actual': actual}
        if bad:
            raise RuntimeError('source base drift; no overwrite/rebase: ' + json.dumps(bad))

    def snapshot(self, rel):
        if rel in self.snapshots:
            return
        p = self.root / rel
        if p.is_symlink() or (p.exists() and not p.is_file()):
            raise RuntimeError('refuse nonregular transaction path ' + rel)
        meta = {'sha256': digest(p), 'present': p.exists(), 'mode': stat.S_IMODE(p.stat().st_mode) if p.exists() else None}
        self.snapshots[rel] = meta
        if p.exists():
            dest = self.out / 'before' / rel
            dest.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(p, dest)

    def parse_manifest(self):
        ownership = {}
        for i, line in enumerate(self.manifest.read_text().splitlines(), 1):
            if not line.strip() or line.lstrip().startswith('#'):
                continue
            fields = line.split()
            if len(fields) != 2:
                raise RuntimeError('invalid MANIFEST line ' + str(i))
            patch, rel = fields
            if Path(patch).name != patch or not patch.endswith('.patch') or Path(rel).is_absolute() or '..' in Path(rel).parts:
                raise RuntimeError('unsafe MANIFEST entry')
            if rel in ownership:
                raise RuntimeError('duplicate MANIFEST ownership ' + rel)
            ownership[rel] = patch
        return ownership

    def regen_outputs(self):
        paths = set((self.root / 'patches').glob('*.patch'))
        paths |= {self.root / 'patches' / name for name in self.parse_manifest().values()}
        paths.add(self.root / 'patches/0010-core.patch')
        return {str(p.relative_to(self.root)): digest(p) for p in sorted(paths)}

    def run(self, args, log, cwd=None, env=None):
        dest = self.out / log
        if dest.exists():
            dest = dest.with_name(dest.name + '.attempt-' + uuid.uuid4().hex[:8])
        dest.parent.mkdir(parents=True, exist_ok=True)
        print('RUN', log, flush=True)
        with dest.open('w') as f:
            result = subprocess.run(args, cwd=cwd or self.root, env=env, stdout=f, stderr=subprocess.STDOUT)
        if result.returncode:
            raise RuntimeError(log + ' failed exit ' + str(result.returncode))

    def index_guard(self):
        if digest(self.index) != self.index_sha:
            raise RuntimeError('real Chromium index changed externally; never restoring it')

    def regenerate(self, label):
        self.verify(self.target if label == 'candidate' else self.base)
        self.verify(self.tools_expected)
        self.index_guard()
        if self.regen_outputs() != self.output_state:
            raise RuntimeError('foreign patch-output edits; no regeneration overwrite')
        ownership = self.parse_manifest()
        native_before = {r: digest(self.native / r) for r in ownership}
        # Copied index preserves existing semantics; require no staged changes for owned paths,
        # because ordinary git diff (used by regen-patches) excludes staged-only changes.
        alt = self.out / (label + '-' + uuid.uuid4().hex[:8] + '.alternate-index')
        shutil.copy2(self.out / 'chromium-index.before', alt)
        env = {**os.environ, 'GIT_INDEX_FILE': str(alt), 'GIT_OPTIONAL_LOCKS': '0'}
        staged = subprocess.check_output(['git', 'diff', '--cached', '--name-only', '-z', '--', *ownership], cwd=self.native, env=env)
        if staged:
            raise RuntimeError('staged native changes would be omitted by regen; parent must resolve without resetting index')
        try:
            self.run(['bash', 'arc/tools/regen-patches.sh'], label + '.regen.log', env=env)
        finally:
            # Only known regen output paths become our journal; unknown additions remain foreign.
            now = self.regen_outputs()
            for rel in self.output_state:
                self.output_state[rel] = now.get(rel)
        self.index_guard()
        self.verify(self.tools_expected)
        if {r: digest(self.native / r) for r in ownership} != native_before:
            raise RuntimeError('native manifest inputs changed during regen')
        dump(self.out / (label + '.patch-outputs.json'), self.output_state)

    def restore_patch_outputs(self):
        self.verify(self.base)  # Includes all original MANIFEST native inputs, not just this batch.
        # regen cannot remove an output whose owner disappeared from MANIFEST. Reconcile
        # baseline presence as well as bytes, notably the newly generated 0010-core.patch.
        baseline = {r: snap['sha256'] for r, snap in self.snapshots.items()
                    if r.startswith('patches/') and r.endswith('.patch')}
        while True:
            current = self.regen_outputs()
            collisions = {r: {'unexpected_output': h} for r, h in current.items() if r not in baseline}
            for rel, before in baseline.items():
                p = self.root / rel
                now = digest(p)
                if p.is_symlink() or (p.exists() and not p.is_file()) or (now != before and now != self.output_state.get(rel)):
                    collisions[rel] = {'before': before, 'now': now, 'last_owned': self.output_state.get(rel)}
            if not collisions:
                break
            self.pause('rollback-patch-output-collision', ['retry'], {'collisions': collisions,
                       'instruction': 'Preserve unknown/foreign outputs. Parent reconcile; no force deletion.'})
        self.verify(self.base)
        for rel, before in baseline.items():
            p = self.root / rel
            if digest(p) == before:
                continue
            snap = self.snapshots[rel]
            if snap['present']:
                shutil.copy2(self.out / 'before' / rel, p)
                p.chmod(snap['mode'])
            else:
                p.unlink()  # Originally absent, exact journaled output only.
        self.verify(self.base)
        self.output_state = baseline
        if self.regen_outputs() != baseline:
            raise RuntimeError('baseline patch output set not exact after reconciliation')
        dump(self.out / 'rollback-patch-outputs.json', baseline)

    def pause(self, phase, actions, context=None, default=None):
        nonce = uuid.uuid4().hex
        request = {'phase': phase, 'actions': actions, 'nonce': nonce, 'output': str(self.out),
                   'source_state': {r: digest(self.root / r) for r in self.base},
                   'runtime_completion_sha256': digest(self.out / 'candidate/completion.json'),
                   'final_sources_sha256': digest(self.out / 'final-sources.json'),
                   'context': context or {}, 'timeout_seconds': self.timeout if default else None,
                   'timeout_action': default or 'continue holding lock for parent'}
        req_path = self.out / 'requests' / (nonce + '.json')
        dump(req_path, request)
        req_sha = digest(req_path)
        self.last_request = request
        decision_path = self.out / 'decisions' / (nonce + '.json')
        marker = {'request': str(req_path), 'request_sha256': req_sha, 'decision': str(decision_path),
                  'phase': phase, 'allowed_actions': actions}
        dump(self.out / 'awaiting-parent-review.json', marker)
        print('AWAITING PARENT (outer lock held)', json.dumps(marker), flush=True)
        deadline = time.monotonic() + self.timeout if default else None
        while True:
            require_outer_lock(self.root)
            if decision_path.exists():
                try:
                    decision = json.loads(decision_path.read_text())
                    if decision.get('request_sha256') == req_sha and decision.get('action') in actions:
                        dump(self.out / 'decisions' / (nonce + '.accepted.json'), decision)
                        return decision['action']
                except (ValueError, OSError):
                    pass  # Parent may be atomically preparing its decision.
            if deadline and time.monotonic() >= deadline:
                dump(self.out / 'decisions' / (nonce + '.timeout.json'), {'action': default, 'request_sha256': req_sha})
                return default
            time.sleep(5)

    def stopped_or_hold(self, phase):
        while agent_processes(self.root):
            self.pause(phase, ['cleanup_complete'], {'live_agent_processes': agent_processes(self.root),
                       'instruction': 'Parent inspect and explicitly clean up exact app; runner sends no cleanup signals.'})

    def apply(self):
        self.verify(self.base)
        self.index_guard()
        for (kind, name, sha), step in zip(ORDER, self.steps):
            before = {self.relative(kind + '/' + r): h for r, h in step['base'].items()}
            after = {self.relative(kind + '/' + r): h for r, h in step['target'].items()}
            self.verify(before)
            patch = self.out / 'patches' / name
            if digest(patch) != sha:
                raise RuntimeError('archived patch changed')
            cwd = self.native if kind == 'native' else self.root
            env = {**os.environ, 'GIT_OPTIONAL_LOCKS': '0'}
            self.run(['git', 'apply', '--check', '--whitespace=error', str(patch)], name + '.check.log', cwd, env)
            # Journal possible before/after bytes before the first mutation. git apply without
            # --reject/--3way is atomic per patch; unknown partial bytes cause a parent hold.
            for rel in before:
                self.allowed.setdefault(rel, set()).update([before[rel], after[rel]])
            self.mutated = True
            self.run(['git', 'apply', '--whitespace=error', str(patch)], name + '.apply.log', cwd, env)
            self.verify(after)
            for rel, h in after.items():
                self.allowed[rel] = {h}
        self.verify(self.target)
        ownership = self.parse_manifest()
        if digest(self.manifest) not in self.manifest_allowed:
            raise RuntimeError('foreign MANIFEST edit before append')
        additions = sorted(r[len('chromium/src/'):] for r in self.selected
                           if r.startswith('chromium/src/') and r[len('chromium/src/'):] not in ownership)
        if additions:
            text = self.manifest.read_text()
            updated = text + ('' if text.endswith('\n') else '\n') + ''.join('0010-core.patch ' + r + '\n' for r in additions)
            self.manifest.write_text(updated)
            self.manifest_allowed = {digest(self.manifest)}
        self.parse_manifest()
        dump(self.out / 'manifest-additions.json', additions)
        self.regenerate('candidate')
        self.verify(self.target)
        self.index_guard()
        dump(self.out / 'applied-sources.json', self.target)

    def runtime(self, label, sources, scenarios):
        self.stopped_or_hold(label + '-live-apps-before-runtime')
        manifest_path = self.out / (label + '-runtime-sources.json')
        dump(manifest_path, sources)
        self.run([sys.executable, str(Path(__file__).with_name('core_runtime_run10.py')), '--root', str(self.root),
                  '--out', str(self.out / label), '--source-manifest', str(manifest_path), '--scenarios', *scenarios],
                 label + '.runner.log')
        completion = json.loads((self.out / label / 'completion.json').read_text())
        if completion.get('status') != 'PASS' or completion.get('all_scenarios_pass') is not True:
            raise RuntimeError(label + ' runtime not PASS')

    def rollback(self, reason):
        while True:
            try:
                return self.rollback_once(reason)
            except BaseException as exc:
                self.pause('rollback-unexpected-blocker', ['retry'], {'error': repr(exc),
                           'instruction': 'Lock remains held. Parent resolve blocker; no forced overwrite or cleanup.'})

    def rollback_once(self, reason):
        self.stopped_or_hold('failed-candidate-live-apps')
        # No source/file is overwritten or removed unless still exactly our last journaled bytes.
        while True:
            collision = {}
            for rel in self.selected:
                p = self.root / rel
                h = digest(p)
                baseline = self.snapshots[rel]['sha256']
                if p.is_symlink() or (p.exists() and not p.is_file()) or (h != baseline and h not in self.allowed.get(rel, set())):
                    collision[rel] = {'now': h, 'baseline': baseline, 'owned': list(self.allowed.get(rel, set()))}
            if digest(self.manifest) not in self.manifest_allowed | {self.snapshots['patches/MANIFEST']['sha256']}:
                collision['patches/MANIFEST'] = {'now': digest(self.manifest)}
            if not collision:
                break
            self.pause('rollback-source-collision', ['retry'], {'collisions': collision,
                       'instruction': 'Preserve foreign edits; parent reconcile to baseline or journaled bytes. No force restore.'})
        for rel in self.selected + ['patches/MANIFEST']:
            p = self.root / rel
            snap = self.snapshots[rel]
            if digest(p) == snap['sha256']:
                continue
            if snap['present']:
                shutil.copy2(self.out / 'before' / rel, p)
                p.chmod(snap['mode'])
                if rel in self.selected:
                    os.utime(p, None)  # Restored bytes must be newer than candidate objects.
            else:
                p.unlink()  # Exact newly-created file only; no recursive cleanup.
        self.manifest_allowed = {self.snapshots['patches/MANIFEST']['sha256']}
        self.verify(self.base)
        while True:
            try:
                self.regenerate('rollback')
                self.restore_patch_outputs()
                self.verify(self.base)
                self.index_guard()
                break
            except BaseException as exc:
                self.pause('rollback-regeneration-blocked', ['retry'], {'error': repr(exc),
                           'instruction': 'Resolve foreign regen outputs/index/unassigned paths; never reset real index.'})
        dump(self.out / 'rollback-sources.json', {r: digest(self.root / r) for r in self.base})
        dump(self.out / 'rollback-source-completion.json', {'status': 'source-restored-and-regenerated', 'cause': reason,
             'real_index_unchanged': True, 'runtime_recovery': 'pending baseline launch-slice build/package/9-step validation'})
        self.stopped_or_hold('before-baseline-package-recovery')
        label = 'baseline' if not (self.out / 'baseline').exists() else 'baseline-retry-' + uuid.uuid4().hex[:8]
        try:
            self.runtime(label, self.base, ['launch-slice'])
            self.verify(self.base)
            self.index_guard()
            if self.regen_outputs() != self.output_state or digest(self.manifest) not in self.manifest_allowed:
                raise RuntimeError('rollback artifacts changed during baseline recovery')
            dump(self.out / 'completion.json', {'status': 'ROLLED_BACK', 'candidate_failure': reason,
                 'source_restored': True, 'patches_regenerated': True, 'real_index_unchanged': True,
                 'baseline_launch_9_of_9': True, 'baseline_runtime_completion_sha256': digest(self.out / label / 'completion.json')})
        except BaseException as exc:
            self.stopped_or_hold('baseline-failure-live-apps')
            dump(self.out / 'baseline-recovery-failure.json', {'candidate_failure': reason, 'baseline_error': repr(exc),
                 'status': 'awaiting guarded parent acknowledgment; no final rollback/runtime proof yet'})
            self.verify(self.base)
            self.index_guard()
            if self.regen_outputs() != self.output_state or digest(self.manifest) not in self.manifest_allowed:
                raise RuntimeError('rollback artifacts drifted; parent reconciliation required')
            self.pause('baseline-recovery-failed', ['release_failed_baseline'], {'error': repr(exc),
                       'instruction': 'No clean/runtime claim. Parent must explicitly acknowledge failed baseline before lock release.'})
            self.stopped_or_hold('baseline-release-live-apps')
            self.verify(self.base)
            self.index_guard()
            if self.regen_outputs() != self.output_state or digest(self.manifest) not in self.manifest_allowed:
                raise RuntimeError('rollback artifacts changed during failed-baseline review')
            dump(self.out / 'completion.json', {'status': 'ROLLED_BACK_RUNTIME_UNPROVED', 'candidate_failure': reason,
                 'source_restored': True, 'patches_regenerated': True, 'real_index_unchanged': True,
                 'baseline_error': repr(exc), 'parent_acknowledged_failed_baseline': True})
        return 1

    def execute(self):
        try:
            self.apply()
            self.runtime('candidate', self.target, ['launch-slice', 'tabs-navigation'])
            self.verify(self.target)
            self.index_guard()
            if self.regen_outputs() != self.output_state:
                raise RuntimeError('patch outputs changed during validation')
            dump(self.out / 'final-sources.json', {r: digest(self.root / r) for r in self.base})
            dump(self.out / 'final-patches.json', self.output_state)
            decision = self.pause('candidate-functional-PASS', ['accept', 'rollback'],
                {'candidate_completion': str(self.out / 'candidate/completion.json'),
                 'patch_outputs_sha256': digest(self.out / 'final-patches.json'),
                 'manifest_sha256': digest(self.manifest), 'boundary_diffs': 'raw reports, parent review required'}, default='rollback')
            if decision != 'accept':
                return self.rollback('parent rollback or review timeout')
            self.stopped_or_hold('acceptance-live-apps')
            self.verify(self.target)
            self.index_guard()
            if self.regen_outputs() != self.output_state or digest(self.manifest) not in self.manifest_allowed:
                raise RuntimeError('patch/MANIFEST drift during review')
            if digest(self.out / 'candidate/completion.json') != self.last_request['runtime_completion_sha256'] or digest(self.out / 'final-sources.json') != self.last_request['final_sources_sha256']:
                raise RuntimeError('reviewed runtime/source evidence changed during parent review')
            if arc_seal(self.root) != json.loads((self.out / 'candidate/arc-build-inputs-final.json').read_text()):
                raise RuntimeError('Arc source inventory changed during review')
            if input_seal(self.root) != json.loads((self.out / 'candidate/runtime-inputs-after.json').read_text()):
                raise RuntimeError('runtime inputs changed during review')
            packaged = json.loads((self.out / 'candidate/completion.json').read_text())['packaged_sha256']
            binary = (self.root / '../arc/build/ArcModAgent-ours156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore').resolve()
            if digest(binary) != packaged:
                raise RuntimeError('packaged binary drift during review')
            dump(self.out / 'completion.json', {'status': 'APPLIED_PARENT_ACCEPTED', 'functional_launch': '9/9',
                 'functional_tabs': '11/11', 'sources_sha256': digest(self.out / 'final-sources.json'),
                 'runtime_completion_sha256': digest(self.out / 'candidate/completion.json'),
                 'real_index_unchanged': True, 'raw_boundary_parity': 'not asserted; reports require parent assessment'})
            return 0
        except BaseException as exc:
            dump(self.out / 'candidate-failure.json', {'error': repr(exc), 'mutated': self.mutated})
            if self.mutated:
                return self.rollback(repr(exc))
            dump(self.out / 'completion.json', {'status': 'ABORTED_BEFORE_MUTATION', 'error': repr(exc)})
            return 2

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--root', type=Path, required=True)
    ap.add_argument('--preflight', type=Path, default=Path('/tmp/core-seven-integration-preflight'))
    ap.add_argument('--out', type=Path)
    ap.add_argument('--review-timeout', type=int, default=1800)
    args = ap.parse_args()
    if args.review_timeout <= 0:
        ap.error('review timeout must be positive; timeout defaults to rollback')
    try:
        tx = Transaction(args)
    except BaseException as exc:
        print('ABORTED BEFORE PRODUCTION MUTATION:', repr(exc), flush=True)
        return 2
    print('TRANSACTION OUTPUT', tx.out, flush=True)
    return tx.execute()

if __name__ == '__main__':
    sys.exit(main())
