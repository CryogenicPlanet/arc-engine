#!/usr/bin/env python3
"""Build/package/run one unchanged accepted scenario in one verified shared lock slot.

No app/helper processes are killed here. Disposable profiles are renamed, never deleted. If the
harness leaves an owned process alive, recovery.json records the parked profile.
"""
import argparse
import datetime
import hashlib
import json
import os
from pathlib import Path
import plistlib
import re
import shutil
import signal
import subprocess
import sys
import time
import uuid

ROOT = Path(__file__).resolve().parents[2]
ARC = ROOT.parent / 'arc'
APP = ARC / 'build/ArcModAgent-ours156.app'
SCENARIOS = ('media-pip', 'media', 'pdf', 'devtools', 'launch-slice')
SECTIONS = ('missing', 'extra', 'return_mismatch', 'arg_mismatch', 'count_mismatch')


def positive(value):
    if not re.fullmatch(r'[1-9][0-9]*', value):
        raise argparse.ArgumentTypeError('must be a positive integer without leading zeros')
    return value


def require(condition, message):
    if not condition:
        raise RuntimeError(message)


def now():
    return datetime.datetime.now(datetime.timezone.utc).isoformat()


def write_json(path, value, exclusive=False):
    with path.open('x' if exclusive else 'w') as output:
        json.dump(value, output, indent=2)
        output.write('\n')


def identity(path):
    path = Path(path)
    real = path.resolve(strict=True)
    before = real.stat()
    digest = hashlib.sha256()
    with real.open('rb') as source:
        for block in iter(lambda: source.read(1024 * 1024), b''):
            digest.update(block)
    after = real.stat()
    fields = ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')
    require(path.resolve(strict=True) == real and all(getattr(before, key) == getattr(after, key) for key in fields),
            f'input changed while hashing: {path}')
    return dict(path=str(path), realpath=str(real), sha256=digest.hexdigest(), size=after.st_size,
                mtimeNs=after.st_mtime_ns, ctimeNs=after.st_ctime_ns, dev=after.st_dev, ino=after.st_ino)


def verify_lock():
    owner = int((ROOT / 'logs/arccore-build.lock/pid').read_text())
    ancestor = os.getpid()
    for _ in range(32):
        if ancestor == owner:
            return
        require(ancestor > 1, '--locked requires an ancestor holding the shared lock')
        ancestor = int(subprocess.check_output(['ps', '-p', str(ancestor), '-o', 'ppid='], text=True))
    raise RuntimeError('--locked requires an ancestor holding the shared lock')


def owned_processes():
    # Match executable prefixes at the beginning of argv, never arbitrary mentions
    # of the bundle in another worker's shell command. Include both framework aliases.
    main = APP / 'Contents/MacOS/Arc'
    framework = APP / 'Contents/Frameworks/ArcCore.framework'
    mains = {str(main), str(main.resolve())}
    frameworks = {str(framework) + '/', str(framework.resolve()) + '/'}
    output = subprocess.check_output(['ps', '-axo', 'pid=,command=', '-ww'], text=True)
    found = []
    for line in output.splitlines():
        parts = line.strip().split(None, 1)
        if len(parts) != 2:
            continue
        pid, command = parts
        matched = next((p for p in mains if command == p or command.startswith(p + ' ')), None)
        matched = matched or next((p for p in frameworks if command.startswith(p)), None)
        if matched:
            found.append({'pid': int(pid), 'executablePrefix': matched})
    return found


def require_stopped(stage):
    found = owned_processes()
    require(not found, f'{stage}: disposable ours156 main/framework processes still running: {found}')


def ts_dependencies(seeds):
    pending, result = list(seeds), set()
    pattern = re.compile(r'''\bfrom\s*["'](\.[^"']+)["']|\bimport\s*["'](\.[^"']+)["']''')
    while pending:
        path = pending.pop().resolve(strict=True)
        require(path.is_relative_to(ROOT / 'tests') or path.is_relative_to(ARC / 'tools'),
                f'unexpected harness dependency: {path}')
        if path in result:
            continue
        result.add(path)
        for match in pattern.finditer(path.read_text()):
            relative = match.group(1) or match.group(2)
            base = path.parent / relative
            options = (base, Path(str(base) + '.ts'), base / 'index.ts')
            target = next((p for p in options if p.is_file()), None)
            require(target is not None and target.suffix == '.ts', f'unresolved TS dependency: {path}: {relative}')
            pending.append(target)
    return result


def source_inputs(scenario):
    seeds = [ROOT / 'tests/launch-slice/run.ts', ROOT / f'tests/scenarios/{scenario}.ts',
             ROOT / 'tests/scenarios/fixture-server.ts']
    paths = ts_dependencies(seeds)
    paths.update(ROOT.glob('arc/api/arc_webextras_*.mm'))
    paths.update(ROOT.glob('arc/browser/webextras_*.h'))
    paths.update(ROOT / p for p in (
        'arc/wp/webextras.gni', 'arc/api/arc_internal.h', 'arc/browser/webextras_scenario_current_run.py',
        'arc/tools/locked.sh', 'arc/tools/gen_api_table.py', 'build-babysit.sh', 'package-engine.sh',
        'tests/recorder/ArcEngineRecorder.dylib', 'tests/recorder/ArcEngineRecorder.m',
        'tests/recorder/diff.py', 'tests/recorder/app_callbacks.inc', 'arc/tools/arccore-1.170.0-objc.tsv'))
    paths.update((ARC / 'tools/agent-native.swift', ARC / 'build/agent-native'))
    # NativeController otherwise rebuilds this helper during the measured scenario.
    require((ARC / 'build/agent-native').stat().st_mtime_ns >= (ARC / 'tools/agent-native.swift').stat().st_mtime_ns,
            'agent-native binary is stale; have its owner build it before queuing this run')
    return paths


def freeze(paths, output):
    manifest = {}
    for path in sorted(paths):
        source = identity(path)
        relative = path.relative_to(ROOT.parent)
        target = output / 'inputs' / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        with target.open('xb') as destination, Path(source['realpath']).open('rb') as origin:
            shutil.copyfileobj(origin, destination)
        target.chmod(0o444)
        frozen = identity(target)
        require(identity(path) == source and frozen['sha256'] == source['sha256'] and frozen['size'] == source['size'],
                f'input changed while freezing: {path}')
        manifest[str(path)] = {'source': source, 'frozen': frozen}
    return manifest


def verify_inputs(manifest, phase, stem):
    for path, item in manifest.items():
        require(identity(path) == item['source'], f'{phase}: live input changed: {path}')
        require(identity(item['frozen']['path']) == item['frozen'], f'{phase}: frozen input changed: {path}')
    write_json(Path(str(stem) + f'.source-{phase}.json'),
               {'capturedAt': now(), 'phase': phase, 'files': manifest}, exclusive=True)


def binary_identity():
    require(APP.is_dir() and not APP.is_symlink() and APP.resolve().parent == (ARC / 'build').resolve(), 'unexpected disposable app location')
    with (APP / 'Contents/Info.plist').open('rb') as source:
        plist = plistlib.load(source)
    require(plist.get('ArcModAgentSession') == 'ours156' and plist.get('CFBundleIdentifier') == 'company.thebrowser.Browser.mod.agent.ours156', 'unexpected packaged agent identity')
    framework = APP / 'Contents/Frameworks/ArcCore.framework'
    engine = framework / 'Versions/A/ArcCore'
    helpers = []
    for bundle in sorted((framework / 'Helpers').glob('*.app')):
        with (bundle / 'Contents/Info.plist').open('rb') as source:
            executable = plistlib.load(source)['CFBundleExecutable']
        helpers.append(identity(bundle / 'Contents/MacOS' / executable))
    require(helpers, 'no packaged framework helpers identified')
    return {'capturedAt': now(), 'main': identity(APP / 'Contents/MacOS/Arc'),
            'engine': identity(engine), 'helpers': helpers,
            'recorder': identity(ROOT / 'tests/recorder/ArcEngineRecorder.dylib')}


def run_command(argv, log, env=None):
    print(f'{now()} {log.name}', flush=True)
    with log.open('xb') as output:
        return subprocess.run(argv, cwd=ROOT, env=env, stdout=output, stderr=subprocess.STDOUT).returncode


def check_record(path):
    rows = [json.loads(line) for line in path.read_text().splitlines()]
    require(any(r.get('ev') == 'hooked' and r.get('engine_classes', 0) > 0 for r in rows), f'no recorder hook: {path}')
    require(any(r.get('ev') == 'call' for r in rows), f'no recorded calls: {path}')
    events = [r for r in rows if r.get('ev') in ('call', 'ret')]
    if any('from' in r for r in events):
        require(all(isinstance(r.get('from'), str) and r['from'] and r['from'].strip() == r['from'] for r in events),
                f'mixed/missing caller attribution: {path}')
        require(any(r['ev'] == 'call' and r.get('dir') == '->engine' and r['from'] == 'app' for r in events),
                f'no official app calls in attributed recording: {path}')
    return len(rows)


def restore_profile(state, output):
    profile, backup, fresh = (Path(state[k]) for k in ('profile', 'backup', 'preservedFresh'))
    if not state.get('prepared'):
        # A signal can arrive after rename(2) succeeds but before Python updates
        # prepared. Observe the saved directory's identity, not that flag alone.
        try:
            if state['originalExisted']:
                original_here = profile.is_dir() and not profile.is_symlink() and directory_identity(profile) == state['savedIdentity']
                parked_here = backup.is_dir() and not backup.is_symlink() and directory_identity(backup) == state['savedIdentity']
                if original_here and not os.path.lexists(backup):
                    state['status'] = 'setup failed before parking; saved directory verified at original path'
                    write_json(output / 'recovery.json', state)
                    return True
                require(parked_here and not os.path.lexists(profile),
                        'ambiguous interrupted parking; saved directory not exclusively at backup path')
                state['prepared'] = True
                state['parkingObserved'] = 'completed rename verified by saved dev/ino'
            else:
                require(not os.path.lexists(profile) and not os.path.lexists(backup),
                        'profile appeared during interrupted setup; refusing to move it')
                state['status'] = 'setup interrupted; original absence verified'
                write_json(output / 'recovery.json', state)
                return True
        except Exception as error:
            state['status'] = 'recovery-required: ' + str(error)
            write_json(output / 'recovery.json', state)
            return False
    # Wait without killing anything. Never restore while a framework helper can
    # still write into the fresh profile, even if the browser PID has exited.
    deadline = time.monotonic() + 30
    found = owned_processes()
    while found and time.monotonic() < deadline:
        time.sleep(0.5)
        found = owned_processes()
    state['remainingProcesses'] = found
    if found:
        state['status'] = 'recovery-required: owned processes remain; original profile stays parked'
        write_json(output / 'recovery.json', state)
        return False
    try:
        if state['originalExisted']:
            require(backup.is_dir() and not backup.is_symlink() and directory_identity(backup) == state['savedIdentity'],
                    'saved original directory identity changed; refusing restoration')
        require(not profile.is_symlink(), 'refusing symlink at disposable profile during restoration')
        if profile.exists():
            require(not fresh.exists() and not fresh.is_symlink(), 'fresh preservation path already exists')
            profile.rename(fresh)
        if state['originalExisted']:
            require(directory_identity(backup) == state['savedIdentity'] and not profile.exists(),
                    'original backup cannot be restored safely')
            backup.rename(profile)
        state['status'] = 'restored' if state['originalExisted'] else 'restored-original-absence'
        write_json(output / 'recovery.json', state)
        return True
    except Exception as error:
        state['status'] = 'recovery-required: ' + str(error)
        write_json(output / 'recovery.json', state)
        return False


def directory_identity(path):
    require(path.is_dir() and not path.is_symlink(), f'not a real profile directory: {path}')
    stat = path.stat()
    return {'dev': stat.st_dev, 'ino': stat.st_ino}


def park_profile(state, output):
    profile, backup = Path(state['profile']), Path(state['backup'])
    if state['originalExisted']:
        state['parkingIntent'] = True
        state['status'] = 'parking intent recorded; inspect saved dev/ino if interrupted'
        write_json(output / 'recovery.json', state)
        require(directory_identity(profile) == state['savedIdentity'], 'original profile changed before parking')
        profile.rename(backup)
        state['prepared'] = True
        require(directory_identity(backup) == state['savedIdentity'], 'original profile changed during parking')
    state['prepared'] = True
    state['status'] = 'original-parked; fresh profile belongs to this run'
    write_json(output / 'recovery.json', state)


def execute(args, output, stem):
    verify_lock()
    require(not output.exists() and not output.is_symlink() and not os.path.lexists(str(stem) + '.source-before.json'),
            'refusing prior output or source-before artifacts')
    require_stopped('before build/profile changes')
    output.mkdir(parents=True, exist_ok=False)
    stem.parent.mkdir(parents=True, exist_ok=True)
    summary = {'scenario': args.scenario, 'runNumber': args.run_number, 'startedAt': now(),
               'scope': 'canonical app-record scenario; default frozen comparator; no fixture override', 'errors': [], 'comparisons': []}
    profile_state = None
    manifest = None
    records = []
    def verify(phase):
        inventory = source_inputs(args.scenario) | set(golden.glob('launch-*/record.jsonl')) | {golden / 'steps.json', golden / 'coverage.json'}
        require({str(p) for p in inventory} == set(manifest), f'{phase}: input file inventory changed')
        verify_inputs(manifest, phase, stem)
    try:
        golden = ROOT / f'results/golden/arc-1.170-{args.scenario}'
        records = sorted(golden.glob('launch-*/record.jsonl'))
        require(records, 'current accepted golden has no launch records')
        steps = json.loads((golden / 'steps.json').read_text())
        coverage = json.loads((golden / 'coverage.json').read_text())
        require(steps.get('scenario') == args.scenario and steps.get('steps') and
                all(s.get('outcome') == 'pass' for s in steps['steps']) and coverage.get('accepted') is True,
                'current golden is not an accepted scenario')
        paths = source_inputs(args.scenario) | set(records) | {golden / 'steps.json', golden / 'coverage.json'}
        manifest = freeze(paths, output)
        write_json(output / 'input-manifest.json',
                   {'capturedAt': now(), 'scope': 'owned webextras production, arc_internal.h and execution inputs; not full repository/source provenance',
                    'files': manifest}, exclusive=True)
        verify('before')
        # The golden copies and comparator mirror retain their original relative layout.
        frozen_root = output / 'inputs' / ROOT.name
        frozen_golden = frozen_root / golden.relative_to(ROOT)
        comparator = frozen_root / 'tests/recorder/diff.py'
        frozen_steps = json.loads((frozen_golden / 'steps.json').read_text())
        frozen_coverage = json.loads((frozen_golden / 'coverage.json').read_text())
        require(frozen_steps.get('scenario') == args.scenario and frozen_steps.get('steps') and
                all(s.get('outcome') == 'pass' for s in frozen_steps['steps']) and frozen_coverage.get('accepted') is True and
                set(frozen_coverage.get('launches', [])) == {p.parent.name for p in records},
                'frozen golden is not an accepted scenario with matching launch coverage')
        require_stopped('before API generation/build')
        require(run_command([sys.executable, 'arc/tools/gen_api_table.py'], Path(str(stem) + '.api-table.log')) == 0, 'API table generation failed')
        build_env = dict(os.environ, ARC_BUILD_FAIL_FAST='1')
        build_status = run_command(['./build-babysit.sh'], Path(str(stem) + '.engine-build.log'), build_env)
        verify('after-build')
        require(build_status == 0, f'build failed (exit {build_status})')
        require_stopped('before packaging')
        require(run_command(['./package-engine.sh', 'ours156'], Path(str(stem) + '.package.log')) == 0, 'packaging failed')
        binaries = binary_identity()
        write_json(output / 'binary-identity.json', binaries, exclusive=True)
        verify('before-run')
        require_stopped('before fresh profile')
        profile = Path.home() / 'Library/Application Support/ArcModAgent-ours156'
        require(not profile.is_symlink() and (not profile.exists() or profile.is_dir()), 'unsafe disposable profile path')
        token = f'webextras-{args.scenario}-current-run{args.run_number}-{uuid.uuid4().hex}'
        backup = profile.with_name(profile.name + '.' + token + '.saved')
        fresh = profile.with_name(profile.name + '.' + token + '.fresh')
        require(not backup.exists() and not fresh.exists(), 'profile parking path collision')
        profile_state = {'profile': str(profile), 'backup': str(backup), 'preservedFresh': str(fresh),
                         'originalExisted': profile.exists(), 'prepared': False, 'parkingIntent': False, 'status': 'preparing',
                         'savedIdentity': directory_identity(profile) if profile.exists() else None,
                         'recovery': 'After every ours156 main/framework process exits, park the fresh profile and rename the saved original back. Never merge or delete either profile.'}
        write_json(output / 'recovery.json', profile_state)
        park_profile(profile_state, output)
        require_stopped('immediately before canonical harness')
        summary['harnessExit'] = run_command(
            ['bun', 'tests/launch-slice/run.ts', '--app', str(APP), '--record', '--out', str(output), '--scenario', args.scenario],
            Path(str(stem) + '.harness.log'))
        verify('after-run')
        # Detect launch-file replacement without comparing volatile access times.
        after_binaries = binary_identity()
        require(all(after_binaries[k] == binaries[k] for k in ('main', 'engine', 'helpers', 'recorder')), 'packaged executable/recorder changed during run')
        write_json(output / 'binary-identity-after-run.json', after_binaries, exclusive=True)
        require_stopped('after canonical harness, before transcript comparison')
        for record in records:
            launch = record.parent.name
            left = frozen_golden / launch / 'record.jsonl'
            right = output / launch / 'record.jsonl'
            if not right.exists():
                summary['errors'].append(f'{launch}: no transcript from our engine')
                continue
            check_record(left)
            check_record(right)
            verify(f'before-diff-{launch}')
            report = output / f'diff-{launch}.json'
            status = run_command([sys.executable, '-B', str(comparator), str(left), str(right), '--json', str(report)], output / f'diff-{launch}.txt')
            verify(f'after-diff-{launch}')
            require(status == 0 and report.is_file(), f'{launch}: frozen comparator failed')
            diff = json.loads(report.read_text())
            require(all(isinstance(diff.get(k), list) for k in SECTIONS) and isinstance(diff.get('callback_sequence'), dict), 'invalid comparator report')
            mismatch = {k: len(diff[k]) for k in SECTIONS}
            clean = not any(mismatch.values()) and diff['callback_sequence'].get('first_divergence') is None
            first = (output / f'diff-{launch}.txt').read_text().splitlines()[:2]
            summary['comparisons'].append({'launch': launch, 'clean': clean, 'mismatches': mismatch, 'firstTwoLines': first})
            print(launch + ': ' + '\n'.join(first), flush=True)
        actual = {p.name for p in output.glob('launch-*') if p.is_dir()}
        require(actual == {p.parent.name for p in records}, 'golden/ours launch sets differ')
        observed = json.loads((output / 'coverage.json').read_text())
        summary['runtimeAccepted'] = summary['harnessExit'] == 0 and observed.get('accepted') is True and observed.get('scenario') == args.scenario
    except (Exception, KeyboardInterrupt) as error:
        summary['errors'].append(f'{type(error).__name__}: {error}')
    finally:
        if manifest is not None:
            try:
                verify('final')
            except Exception as error:
                summary['errors'].append(f'final input verification: {error}')
        try:
            summary['profileRestored'] = restore_profile(profile_state, output) if profile_state is not None else True
        except Exception as error:
            summary['profileRestored'] = False
            if profile_state is not None:
                profile_state['status'] = f'recovery-required: could not verify safe restoration: {error}'
                write_json(output / 'recovery.json', profile_state)
        if not summary['profileRestored']:
            summary['errors'].append('profile recovery required; see recovery.json; no processes were killed by this runner')
        summary['finishedAt'] = now()
        summary['accepted'] = bool(summary.get('runtimeAccepted') and not summary['errors'] and summary['profileRestored'] and
                                   len(summary['comparisons']) == len(records) and all(c['clean'] for c in summary['comparisons']))
        write_json(output / 'runner-summary.json', summary)
    print(json.dumps(summary), flush=True)
    return 0 if summary['accepted'] else 1


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('--scenario', choices=SCENARIOS, required=True)
    parser.add_argument('--run-number', type=positive, required=True)
    parser.add_argument('--locked', action='store_true', help=argparse.SUPPRESS)
    args = parser.parse_args()
    output = ROOT / f'results/webextras/{args.scenario}-current-run{args.run_number}'
    stem = ROOT / f'logs/wp/webextras.{args.scenario}.current-run{args.run_number}'
    require(not output.exists() and not output.is_symlink() and not os.path.lexists(str(stem) + '.source-before.json'),
            'refusing prior output or source-before artifacts')
    if not args.locked:
        os.execv(str(ROOT / 'arc/tools/locked.sh'), [str(ROOT / 'arc/tools/locked.sh'), sys.executable, str(Path(__file__).resolve()),
                   '--locked', '--scenario', args.scenario, '--run-number', args.run_number])
    def interrupted(signum, frame):
        raise KeyboardInterrupt(f'received signal {signum}')
    signal.signal(signal.SIGTERM, interrupted)
    return execute(args, output, stem)


if __name__ == '__main__':
    try:
        sys.exit(main())
    except (RuntimeError, OSError, ValueError) as error:
        print(str(error), file=sys.stderr)
        sys.exit(2)
