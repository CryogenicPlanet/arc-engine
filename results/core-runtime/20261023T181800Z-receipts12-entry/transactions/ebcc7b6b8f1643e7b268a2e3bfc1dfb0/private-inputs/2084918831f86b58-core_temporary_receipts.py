"""PRIVATE candidate: exact temporary diagnostics, always restore, parent ACK required."""
from pathlib import Path
import argparse, hashlib, json, os, re, shutil, stat, subprocess, sys, time, uuid
from sealed_integrator11 import Transaction as Run11Transaction
from core_runtime_run10 import digest, dump, require_outer_lock, agent_processes, arc_seal, input_seal
from core_index_guard_run11 import git_environment, native_head, read_regular_bytes, parse_index

PREFLIGHT_SHA = '735fba0abf9e6446d7f4ead929b16a43f5053e257009885b9b2bf6b6a632c381'
HARNESS = 'tests/launch-slice/run.ts'
HEADER = 'chromium/src/content/public/browser/host_app_hooks.h'
ABSENT_HEADER = 'chromium/src/content/public/browser/host_receipt_diagnostics.h'
HARNESS_PATCH_SHA = '2324e5ab79c08102083eaf2cce03aa66c8ecde3cb05e6ed601fc60fc480cd0b7'
HARNESS_BASE = '023dbb692a149434fe1eb38424e9cb819bb9b556702cc61a8b9b9a867f630b1e'
HARNESS_TARGET = '1470d49708a1280fadda806ec6c09dd1b587d08cd8604fac3927df1a74e80a7a'


def safe_error(exc):
    try:
        return repr(exc)
    except BaseException:
        return 'exception could not be represented; parent inspection required'


def owner_ack(args):
    """Parent verifies actual ownership/approval; never infer ACK from request text."""
    if not args.ack_binding or not args.ack_binding_sha256:
        raise RuntimeError('HOLD: exact parent owner-ACK binding required; no approval inferred')
    if args.ack_binding.is_symlink():
        raise RuntimeError('HOLD: ACK binding symlink')
    binding_path = args.ack_binding.resolve()
    if binding_path.is_symlink() or not binding_path.is_file() or digest(binding_path) != args.ack_binding_sha256:
        raise RuntimeError('HOLD: parent ACK binding absent/changed/nonregular')
    b = json.loads(binding_path.read_text())
    wanted = {'status': 'OWNER_ACK_VERIFIED_BY_PARENT', 'request_seq': 143890,
              'approved_exact_hunk': True, 'source': HARNESS, 'base_sha256': HARNESS_BASE,
              'target_sha256': HARNESS_TARGET, 'patch_sha256': HARNESS_PATCH_SHA}
    if any(b.get(k) != v for k, v in wanted.items()) or b.get('owner_role') not in ('scenarios', 'lead'):
        raise RuntimeError('HOLD: ACK does not bind exact approved harness hunk')
    if type(b.get('ack_seq')) is not int or b['ack_seq'] <= 143890 or not b.get('owner_agent'):
        raise RuntimeError('HOLD: actual owner ACK receipt identity missing')
    raw_path = binding_path.parent / b.get('raw_receipt_file', '')
    # The raw receipt must be a sibling regular file, not a path out of the private approval archive.
    if (raw_path.parent != binding_path.parent or raw_path.is_symlink() or not raw_path.is_file()
            or digest(raw_path) != b.get('raw_receipt_sha256')):
        raise RuntimeError('HOLD: raw owner receipt absent/changed/nonregular')
    raw = json.loads(raw_path.read_text())
    if (raw.get('seq') != b['ack_seq'] or raw.get('agent') != b['owner_agent']
            or raw.get('topic') != 'projects/arc' or not isinstance(raw.get('body'), str) or not raw['body'].strip()):
        raise RuntimeError('HOLD: raw ACK identity does not match parent verification')
    return {str(binding_path): digest(binding_path), str(raw_path): digest(raw_path)}


def protocol_logs(directory, instrumented, scenarios):
    """Coverage gate only; no inferred native origins or fabricated cancellation events."""
    result = {}
    for sc in scenarios:
        logs = sorted((directory / sc).glob('launch-*/app.log'))
        if len(logs) != (2 if sc == 'launch-slice' else 1):
            raise RuntimeError('receipt log launch coverage missing ' + sc)
        result[sc] = []
        for log in logs:
            markers = [line for line in log.read_text(errors='replace').splitlines() if 'ARC_CORE_RECEIPT' in line]
            pattern = r'ARC_CORE_RECEIPT v=1 counter=(\d+) id=(\d+) phase=([a-z-]+) tid=(\d+) us=(\d+) native=(\d+) peer=(\d+) aux=(\d+) value=(\S+) bit=([01])\s*$'
            phases = {}
            for line in markers:
                match = re.search(pattern, line)
                if not match or any(int(match[i]) <= 0 for i in (1, 2, 4, 5)):
                    raise RuntimeError('malformed receipt marker ' + str(log))
                float(match[9])  # parse only; no scalar normalization or semantic inference
                phases[match[3]] = phases.get(match[3], 0) + 1
            if bool(markers) != instrumented:
                raise RuntimeError('receipt mode contamination/missing activation ' + str(log))
            result[sc].append({'path': str(log.relative_to(directory)), 'sha256': digest(log),
                               'receipt_markers': len(markers), 'phases_seen': phases, 'mode': 'instrumented' if instrumented else 'disabled'})
    dump(directory / 'receipt-log-coverage.json', result)
    return result


def typecheck_sources(root):
    paths = set((root / 'tests/scenarios').glob('*.ts'))
    paths.add(root / HARNESS)
    paths |= set((root / '../arc/tools').rglob('*.ts'))
    return {os.path.relpath(path.resolve(), root): digest(path) for path in sorted(paths)}


class Transaction(Run11Transaction):
    def __init__(self, args):
        # ACK rejection precedes lock/process/Git/source queries and all mutation.
        approval = owner_ack(args)
        self.root = args.root.resolve()
        self.native = self.root / 'chromium/src'
        self.preflight = args.preflight.resolve()
        if digest(self.preflight / 'manifest.json') != PREFLIGHT_SHA:
            raise RuntimeError('reviewed frozen preflight changed; fresh isolated rebase/review required')
        self.meta = json.loads((self.preflight / 'manifest.json').read_text())
        self.base = dict(self.meta['fixed_base'])
        self.target = dict(self.meta['fixed_target'])
        self.selected = sorted(self.meta['selected'])
        for rel, expected in self.meta['preserved_seven_proposals'].items():
            self.base[rel] = expected
            self.target[rel] = expected
        if (len(self.selected) != 5 or HEADER not in self.selected or HARNESS not in self.selected
                or self.base[HEADER] != '48f3aae4107e6880c8797cc7db11ac810cc8b7c7fba6d63a3d34db05b52a4ad3'
                or any(self.base[r] is None for r in self.selected)
                or ABSENT_HEADER not in self.base or self.base[ABSENT_HEADER] is not None
                or self.target.get(ABSENT_HEADER) is not None):
            raise RuntimeError('five existing paths/forbidden new header preflight invalid')
        self._private_hashes = dict(approval)
        folder = Path(__file__).resolve().parent
        for name, expected in self.meta['source_components'].items():
            path = folder / ('sealed_integrator11.py' if name == 'core_integrate_run11.py' else name)
            if digest(path) != expected:
                raise RuntimeError('sealed reusable component changed ' + name)
        if digest(folder / 'core_runtime_receipts.py') != self.meta['runtime_variant_sha256']:
            raise RuntimeError('reviewed fixed-boolean runtime changed')
        for path in [Path(__file__).resolve(), *(folder / n for n in ['sealed_integrator11.py', 'core_runtime_run10.py',
                'core_runtime_receipts.py', 'core_index_guard_run11.py', 'core_untr_run11.py']), self.preflight / 'manifest.json',
                self.preflight / 'accepted-patch-map.json', self.preflight / 'accepted-MANIFEST']:
            self._private_hashes[str(path)] = digest(path)
        for patch in self.meta['patches']:
            path = self.preflight / 'patches' / patch['name']
            if path.is_symlink() or digest(path) != patch['sha256']:
                raise RuntimeError('reviewed root-relative patch changed')
            self._private_hashes[str(path)] = patch['sha256']
        for rel, expected in self.meta['typecheck']['private_files'].items():
            path = folder / rel
            if path.is_symlink() or digest(path) != expected:
                raise RuntimeError('strict TypeScript private input changed')
            self._private_hashes[str(path)] = expected
        require_outer_lock(self.root)
        self.timeout = None  # no automatic decision releases an unsafe/failed baseline
        self.verify(self.base)
        if agent_processes(self.root):
            raise RuntimeError('exact ours156 survivor; no mutation or cleanup')
        self.tools_expected = {r: digest(self.root / r) for r in ['arc/tools/regen-patches.sh', 'arc/tools/locked.sh']}
        if any(v is None for v in self.tools_expected.values()):
            raise RuntimeError('missing transaction tools')
        self.verify(self.tools_expected)
        self.manifest = self.root / 'patches/MANIFEST'
        ownership = self.parse_manifest()
        native_wc = 'content/browser/web_contents/web_contents_impl.cc'
        if native_wc not in ownership or ownership.get(HEADER.removeprefix('chromium/src/')) != '0010-core.patch':
            raise RuntimeError('native WC/header MANIFEST ownership invalid')
        for rel in ownership:
            key = 'chromium/src/' + rel
            self.base.setdefault(key, digest(self.native / rel))
            self.target.setdefault(key, self.base[key])
        # Never snapshot arbitrary current regeneration bytes as an accepted baseline.
        self.accepted_patch_guard()
        self.arc_input_base = dict(self.meta['expected_arc_inputs'])
        if arc_seal(self.root) != self.arc_input_base:
            raise RuntimeError('reviewed accepted Core11 Arc input inventory drift; fresh rebase/review required')
        for rel, expected in self.arc_input_base.items():
            self.base.setdefault(rel, expected)
            self.target.setdefault(rel, self.base[rel])
        self.runtime_input_base = dict(self.meta['expected_runtime_inputs'])
        if input_seal(self.root) != self.runtime_input_base:
            raise RuntimeError('reviewed accepted Core11 runtime inputs drift; fresh rebase/review required')
        self.typecheck_input_guard(self.base)
        self.verify(self.base)
        self.out = args.out.resolve()
        self.out.mkdir(parents=True, exist_ok=False)
        self.allowed = {}
        self.snapshots = {}
        self.mutated = False
        self.manifest_allowed = {digest(self.manifest)}
        for rel in self.selected + ['patches/MANIFEST']:
            self.snapshot(rel)
        for rel in self.regen_outputs():
            self.snapshot(rel)
        self.output_state = self.regen_outputs()
        index_name = subprocess.check_output(['git', 'rev-parse', '--git-path', 'index'], cwd=self.native,
                                              env=git_environment(), text=True).strip()
        self.index = Path(index_name)
        if not self.index.is_absolute():
            self.index = self.native / self.index
        if self.index.is_symlink() or not self.index.is_file():
            raise RuntimeError('real Chromium index nonregular')
        self.native_head = native_head(self.native)
        self.index_image = parse_index(read_regular_bytes(self.index))
        self.index_sha = self.index_image.sha256
        (self.out / 'chromium-index.before').write_bytes(self.index_image.data)
        self.index_guard_serial = 0
        dump(self.out / 'chromium-index-before-fingerprint.json', {'native_HEAD': self.native_head, **self.index_image.proof()})
        self.index_guard()
        self.manifest_index_visibility('constructor')
        for path, expected in self._private_hashes.items():
            source = Path(path)
            dest = self.out / 'private-inputs' / (hashlib.sha256(path.encode()).hexdigest()[:16] + '-' + source.name)
            dest.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(source, dest)
        for rel in self.meta['typecheck']['source_inputs']:
            source = self.root / rel
            dest = (self.out / 'typecheck-sources/sibling-arc' / rel[7:] if rel.startswith('../arc/')
                    else self.out / 'typecheck-sources' / rel)
            dest.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(source, dest)
        dump(self.out / 'typecheck-source-inputs.json', self.meta['typecheck']['source_inputs'])
        dump(self.out / 'private-inputs.json', self._private_hashes)
        dump(self.out / 'snapshots.json', self.snapshots)
        dump(self.out / 'base-sources.json', self.base)
        dump(self.out / 'candidate-sources.json', self.target)
        dump(self.out / 'integration-tools-before.json', self.tools_expected)
        dump(self.out / 'runtime-inputs-before-transaction.json', self.runtime_input_base)
        self.check_private_inputs()
        self.verify(self.base)

    def manifest_index_visibility(self, label):
        # Adopted regen ignores inherited GIT_INDEX_FILE. Require every assigned
        # leaf already visible so its add-N fallback cannot write the real index.
        self.index_guard()
        ownership = self.parse_manifest()
        before = parse_index(read_regular_bytes(self.index))
        raw = subprocess.check_output(['git', 'ls-files', '--cached', '-z', '--', *sorted(ownership)],
                                      cwd=self.native, env=git_environment())
        visible = set(raw.decode().rstrip('\0').split('\0')) if raw else set()
        after = parse_index(read_regular_bytes(self.index))
        if before.data != after.data:
            raise RuntimeError('real index changed during read-only manifest visibility query')
        if visible != set(ownership):
            raise RuntimeError('assigned native leaf not exactly index-visible; no regeneration/add-N allowed')
        self.index_guard()
        dump(self.out / (label + '.manifest-index-visible.json'), {
            'all_assigned_leaves_visible': True, 'assigned': sorted(ownership),
            'full_index_bytes_unchanged': True, 'before': before.proof(), 'after': after.proof()})

    def regenerate(self, label):
        self.manifest_index_visibility(label + '-before-regeneration')
        return super().regenerate(label)

    def accepted_patch_guard(self):
        if self.manifest.is_symlink() or not self.manifest.is_file():
            raise RuntimeError('HOLD: baseline MANIFEST nonregular')
        accepted = self.meta['accepted_patch_baseline']
        if (digest(self.preflight / accepted['map_file']) != accepted['map_sha256']
                or digest(self.preflight / accepted['manifest_file']) != accepted['manifest_sha256']
                or digest(self.manifest) != accepted['manifest_sha256']
                or self.regen_outputs() != accepted['outputs']):
            raise RuntimeError('HOLD: accepted Core11 MANIFEST/patch map mismatch before baseline archive')
        for rel in accepted['outputs']:
            path = self.root / rel
            if path.is_symlink() or not path.is_file():
                raise RuntimeError('HOLD: accepted patch output is nonregular')

    def phase_input_guard(self, sources):
        expected_inputs = dict(self.runtime_input_base)
        expected_arc = dict(self.arc_input_base)
        for rel in self.selected:
            if rel in expected_inputs:
                expected_inputs[rel] = sources[rel]
            if rel in expected_arc:
                expected_arc[rel] = sources[rel]
        if input_seal(self.root) != expected_inputs or arc_seal(self.root) != expected_arc:
            raise RuntimeError('foreign runtime/Arc input inventory drift; HOLD')

    def check_private_inputs(self):
        for name, expected in self._private_hashes.items():
            path = Path(name)
            if path.is_symlink() or not path.is_file() or digest(path) != expected:
                raise RuntimeError('private approval/script/patch drift; HOLD ' + name)

    def run(self, args, log, cwd=None, env=None):
        self.check_private_inputs()
        dump(self.out / 'commands' / (uuid.uuid4().hex + '.json'),
             {'argv': [str(a) for a in args], 'cwd': str(cwd or self.root), 'log': log,
              'GIT_OPTIONAL_LOCKS': '0', 'GIT_INDEX_FILE': (env or {}).get('GIT_INDEX_FILE')})
        super().run(args, log, cwd, env)
        self.check_private_inputs()

    def pause(self, phase, actions, context=None, default=None):
        # Parent request/journal failure cannot become permission to leave mutated
        # sources or release a failed baseline. This is not a SIGKILL/power-loss guarantee.
        while True:
            try:
                return super().pause(phase, actions, context, default=None)
            except BaseException as exc:
                try:
                    print('HOLD: parent request unavailable; repair journal/lock context:', safe_error(exc),
                          file=sys.stderr, flush=True)
                except BaseException:
                    pass
                try:
                    time.sleep(5)
                except BaseException:
                    pass

    def rollback(self, reason):
        while True:
            try:
                return self.rollback_once(reason)
            except BaseException as exc:
                # pause itself is protected, including write/lock/interruption failures.
                self.pause('rollback-unexpected-blocker', ['retry'], {'error': safe_error(exc),
                           'instruction': 'Lock held; no overwrite/cleanup/release. Parent repair then retry.'})

    def apply(self):
        self.verify(self.base)
        self.index_guard()
        self.manifest_index_visibility('before-apply')
        for spec in self.meta['patches']:
            before = {r: self.meta['selected'][r]['base'] for r in spec['paths']}
            after = {r: self.meta['selected'][r]['target'] for r in spec['paths']}
            self.verify(before)
            patch = self.preflight / 'patches' / spec['name']
            if digest(patch) != spec['sha256']:
                raise RuntimeError('private patch changed')
            self.run(['git', 'apply', '--check', '--whitespace=error', str(patch)], spec['name'] + '.check.log', self.root)
            for rel in before:
                self.allowed.setdefault(rel, set()).update([before[rel], after[rel]])
            self.mutated = True
            self.run(['git', 'apply', '--whitespace=error', str(patch)], spec['name'] + '.apply.log', self.root)
            self.verify(after)
            for rel, h in after.items():
                self.allowed[rel] = {h}
        self.verify(self.target)
        if digest(self.manifest) not in self.manifest_allowed:
            raise RuntimeError('foreign MANIFEST changed')
        ownership = self.parse_manifest()
        native_header = HEADER.removeprefix('chromium/src/')
        if ownership.get(native_header) != '0010-core.patch':
            raise RuntimeError('existing header owner changed; no MANIFEST addition allowed')
        self.parse_manifest()
        self.regenerate('candidate')
        self.verify(self.target)
        self.index_guard()
        dump(self.out / 'applied-sources.json', self.target)

    def runtime(self, label, sources, scenarios, instrumented=False):
        self.stopped_or_hold(label + '-live-apps-before-runtime')
        self.verify(sources)
        self.phase_input_guard(sources)
        self.index_guard()
        manifest_path = self.out / (label + '-runtime-sources.json')
        dump(manifest_path, sources)
        argv = [sys.executable, str(Path(__file__).with_name('core_runtime_receipts.py')), '--root', str(self.root),
                '--out', str(self.out / label), '--source-manifest', str(manifest_path), '--scenarios', *scenarios]
        if instrumented:
            argv.append('--core-receipt-diagnostics')
        dump(self.out / (label + '-mode.json'), {'instrumented': instrumented, 'argv': argv,
             'default_disabled': not instrumented, 'raw_origin_parity': 'not asserted'})
        self.run(argv, label + '.runner.log')
        completion = json.loads((self.out / label / 'completion.json').read_text())
        if completion.get('status') != 'PASS' or completion.get('all_scenarios_pass') is not True:
            raise RuntimeError(label + ' runtime not PASS')
        protocol_logs(self.out / label, instrumented, scenarios)
        self.verify(sources)
        self.phase_input_guard(sources)
        self.index_guard()
        if self.regen_outputs() != self.output_state or digest(self.manifest) not in self.manifest_allowed:
            raise RuntimeError('patch/MANIFEST drift during runtime')

    def typecheck_input_guard(self, sources):
        expected = dict(self.meta['typecheck']['source_inputs'])
        expected[HARNESS] = sources[HARNESS]
        actual = typecheck_sources(self.root)
        if actual != expected:
            raise RuntimeError('strict TypeScript source inventory drift; fresh review required')
        for rel, sha in self.meta['typecheck']['private_files'].items():
            path = Path(__file__).parent / rel
            if path.is_symlink() or not path.is_file() or digest(path) != sha:
                raise RuntimeError('strict TypeScript compiler/type/config input drift')

    def run_strict_typecheck(self):
        self.verify(self.target)
        self.phase_input_guard(self.target)
        self.typecheck_input_guard(self.target)
        config = self.meta['typecheck']
        if str(self.root) != config['project_root']:
            raise RuntimeError('strict TypeScript config binds a different source root')
        folder = Path(__file__).parent
        argv = ['bun', str(folder / config['compiler']), '-p', str(folder / config['config'])]
        dump(self.out / 'strict-typescript-argv.json', argv)
        self.run(argv, 'strict-typescript.log', self.root)
        self.typecheck_input_guard(self.target)
        self.verify(self.target)
        self.phase_input_guard(self.target)

    def restoration_collision(self):
        bad = {}
        expected_typecheck = dict(self.meta['typecheck']['source_inputs'])
        expected_typecheck[HARNESS] = digest(self.root / HARNESS)
        if typecheck_sources(self.root) != expected_typecheck:
            bad['typecheck-source-inventory'] = {'instruction': 'foreign strict TypeScript input drift'}
        expected_inputs = dict(self.runtime_input_base)
        expected_arc = dict(self.arc_input_base)
        for rel in self.selected:
            if rel in expected_inputs:
                expected_inputs[rel] = digest(self.root / rel)
            if rel in expected_arc:
                expected_arc[rel] = digest(self.root / rel)
        if input_seal(self.root) != expected_inputs:
            bad['runtime-input-inventory'] = {'instruction': 'foreign runtime input addition/deletion/change'}
        if arc_seal(self.root) != expected_arc:
            bad['arc-input-inventory'] = {'instruction': 'foreign Arc input addition/deletion/change'}
        for rel, expected in self.runtime_input_base.items():
            if rel not in self.selected and digest(self.root / rel) != expected:
                bad['input:' + rel] = {'current': digest(self.root / rel), 'baseline': expected}
        for rel, expected in self.base.items():
            path = self.root / rel
            actual = digest(path)
            allowed = {expected} | self.allowed.get(rel, set()) if rel in self.selected else {expected}
            if path.is_symlink() or (path.exists() and not path.is_file()) or actual not in allowed:
                bad[rel] = {'current': actual, 'allowed': list(allowed)}
        if digest(self.manifest) not in self.manifest_allowed | {self.snapshots['patches/MANIFEST']['sha256']}:
            bad['patches/MANIFEST'] = {'current': digest(self.manifest)}
        if self.regen_outputs() != self.output_state:
            bad['patch-outputs'] = {'current': self.regen_outputs(), 'last_owned': self.output_state}
        return bad

    def rollback_once(self, reason):
        self.stopped_or_hold('temporary-survivors-before-restoration')
        while True:
            self.check_private_inputs()
            self.index_guard()
            self.verify(self.tools_expected)
            collisions = self.restoration_collision()
            if not collisions:
                break
            self.pause('restoration-source-or-foreign-collision', ['retry'], {'collisions': collisions,
                       'instruction': 'Preserve unknown work; parent reconcile, no forced cleanup/overwrite.'})
        for rel in self.selected + ['patches/MANIFEST']:
            path = self.root / rel
            snap = self.snapshots[rel]
            if digest(path) == snap['sha256']:
                continue
            if snap['present']:
                shutil.copy2(self.out / 'before' / rel, path)
                path.chmod(snap['mode'])
                if rel in self.selected:
                    os.utime(path, None)
            else:
                raise RuntimeError('selected paths must all have existing baseline snapshots; no deletion allowed')
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
                self.pause('restoration-regen-HOLD', ['retry'], {'error': safe_error(exc)})
        dump(self.out / 'restored-source-proof.json', {'status': 'SOURCE_RESTORED_RUNTIME_PENDING',
             'sources': {r: digest(self.root / r) for r in self.base}, 'patches': self.regen_outputs(), **self.index_result()})
        # A baseline failure never releases this lock through a failed-baseline action.
        while True:
            label = 'baseline' if not (self.out / 'baseline').exists() else 'baseline-retry-' + uuid.uuid4().hex[:8]
            try:
                self.runtime(label, self.base, ['launch-slice'], instrumented=False)
                self.stopped_or_hold('baseline-final-survivors')
                self.verify(self.base)
                self.index_guard()
                if self.regen_outputs() != self.output_state or digest(self.manifest) not in self.manifest_allowed:
                    raise RuntimeError('baseline restoration artifacts drifted')
                status = 'DIAGNOSTIC_COMPLETE_RESTORED' if self.phase_success else 'FAILED_RESTORED'
                dump(self.out / 'completion.json', {'status': status, 'cause': reason,
                     'source_restored': True, 'exact_patch_presence_restored': True, 'baseline_launch_9_of_9': True,
                     'baseline_completion_sha256': digest(self.out / label / 'completion.json'),
                     'normal_and_instrumented_9_and_11_passed': self.phase_success,
                     'raw_boundary_parity': 'not asserted', **self.index_result()})
                return 0 if self.phase_success else 1
            except BaseException as exc:
                self.stopped_or_hold('baseline-failure-survivors')
                dump(self.out / 'baseline-recovery-failure.json', {'error': safe_error(exc), 'status': 'HOLD_RETRY_ONLY'})
                self.pause('baseline-recovery-HOLD', ['retry'], {'error': safe_error(exc),
                           'instruction': 'No release/retain/cleanup action. Parent reconcile; baseline9 required.'})
                self.verify(self.base)
                self.index_guard()

    def execute(self):
        self.phase_success = False
        reason = 'normal and instrumented diagnostic phases completed; mandatory restoration'
        try:
            self.apply()
            self.run_strict_typecheck()
            self.runtime('normal', self.target, ['launch-slice', 'tabs-navigation'])
            self.runtime('instrumented', self.target, ['launch-slice', 'tabs-navigation'], instrumented=True)
            self.phase_success = True
        except BaseException as exc:
            reason = safe_error(exc)
            try:
                dump(self.out / 'diagnostic-failure.json', {'error': reason, 'mutated': self.mutated})
            except BaseException as journal_error:
                reason += '; diagnostic journal unavailable: ' + safe_error(journal_error)
        finally:
            # Independent of optional failure journaling and every phase exception.
            if self.mutated:
                restored_result = self.rollback(reason)
        if self.mutated:
            return restored_result
        dump(self.out / 'completion.json', {'status': 'ABORTED_BEFORE_MUTATION', 'error': reason})
        return 2


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--root', type=Path, required=True)
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('--preflight', type=Path, default=Path(__file__).with_name('preflight'))
    ap.add_argument('--ack-binding', type=Path)
    ap.add_argument('--ack-binding-sha256')
    args = ap.parse_args()
    try:
        tx = Transaction(args)
    except BaseException as exc:
        print('ABORTED BEFORE SOURCE MUTATION:', safe_error(exc), flush=True)
        return 2
    return tx.execute()


if __name__ == '__main__':
    sys.exit(main())
