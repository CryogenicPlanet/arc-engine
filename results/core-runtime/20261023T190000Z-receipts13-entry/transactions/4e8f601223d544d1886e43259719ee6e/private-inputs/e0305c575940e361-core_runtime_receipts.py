"""Lock-free body of the existing build/package/launch+tabs regression. Outer lock required."""
from pathlib import Path
import argparse, datetime, hashlib, json, os, shutil, subprocess, sys

EXPECTED_STEPS = {
    'launch-slice': ['S1 launch', 'S2 open A', 'S3 navigate', 'S4 store',
                     'S5 close + new tab', 'S6 quit', 'S7 relaunch', 'S7 readback', 'S8 quit'],
    'tabs-navigation': ['T1 launch', 'T2 three tabs', 'T3 document navigation', 'T4 back',
                        'T5 forward', 'T6 reload', 'T7 same document', 'T8 redirect',
                        'T9 close exact tab', 'T10 open replacement tab', 'T11 quit'],
}
SUFFIXES = {'.mm', '.h', '.cc', '.gni', '.gn', '.inc', '.mojom'}

def digest(path):
    p = Path(path)
    if not p.exists():
        return None
    h = hashlib.sha256()
    with p.open('rb') as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b''):
            h.update(chunk)
    return h.hexdigest()

def dump(path, value):
    p = Path(path)
    p.parent.mkdir(parents=True, exist_ok=True)
    p.write_text(json.dumps(value, indent=2, sort_keys=True) + '\n')

def require_outer_lock(root):
    """Require the locked.sh PID to be a living ancestor, not merely an existing lock."""
    lock = root / 'logs/arccore-build.lock'
    owner = int((lock / 'pid').read_text().strip())
    pid = os.getpid()
    ancestors = set()
    while pid > 1 and pid not in ancestors:
        ancestors.add(pid)
        pid = int(subprocess.check_output(['ps', '-o', 'ppid=', '-p', str(pid)], text=True).strip())
    if owner not in ancestors:
        raise RuntimeError('must run inside the existing locked.sh ancestor; do not nest locks')
    return owner

def agent_processes(root):
    app = (root / '../arc/build/ArcModAgent-ours156.app').resolve()
    return [line.strip() for line in subprocess.check_output(['ps', '-axo', 'pid=,command='], text=True).splitlines()
            if str(app) + '/Contents/' in line]

def require_stopped(root):
    rows = agent_processes(root)
    if rows:
        raise RuntimeError('live exact ours156 agent processes; no cleanup/reset authorized by this runner: ' + repr(rows))

def regular(path):
    if path.is_symlink() or not path.is_file():
        raise RuntimeError('required regular input absent/symlink: ' + str(path))
    return path

def source_seal(root, expected):
    return {rel: digest(root / rel) for rel in expected}

def arc_seal(root):
    return {str(p.relative_to(root)): digest(p) for p in sorted((root / 'arc').rglob('*'))
            if p.is_file() and p.suffix in SUFFIXES}

def input_seal(root):
    inputs = {root / 'tests/scenarios' / n for n in
              ['launch-slice.ts', 'tabs-navigation.ts', 'coverage.ts', 'fixture-server.ts', 'native.ts']}
    inputs |= set(root.glob('tests/recorder/*.py'))
    inputs |= {root / r for r in ['tests/launch-slice/run.ts', 'tests/recorder/ArcEngineRecorder.dylib',
                                 'tests/recorder/app_callbacks.inc', 'arc/tools/arccore-1.170.0-objc.tsv',
                                 'arc/tools/gen_api_table.py', 'build-babysit.sh', 'package-engine.sh']}
    inputs |= set((root / '../arc/tools').glob('agent*.ts'))
    inputs |= set((root / '../arc/tools').glob('agent*.swift'))
    # Actual build-mod dependency closure: imports/resources, executed patchers, and compiled mods.
    sibling = root / '../arc'
    inputs |= {sibling / rel for rel in [
        'tools/build-mod.ts', 'tools/service-policy.ts', 'tools/shortcut-policy.ts',
        'tools/shortcut-coverage.ts', 'tools/shortcut-inventory.ts', 'tools/shortcut-generated-coverage.ts',
        'tools/patch-local-mode.ts', 'tools/insert-dylib.ts', 'tools/shortcut-bridge.ts',
        'tools/local-close-bridge.ts', 'tools/verify-foundation-abi.swift',
        'resources/local-service-policy.json', 'resources/local-shortcut-policy.json',
        'resources/shortcut-menu-routing.json', 'resources/shortcut-menu-inventory.json',
        'resources/shortcut-generated-inventory.json',
        'mods/hello/ArcMod.m', 'mods/local/ServicePolicy.m', 'mods/local/ShortcutPolicy.m',
        'mods/local/ServicePolicy.h', 'mods/local/ShortcutPolicy.h',
    ]}
    inputs |= {root / r for r in ['arc/tools/regen-patches.sh', 'arc/tools/locked.sh',
        'swap/stock-1.168.0/Arc.app/Contents/Info.plist',
        'swap/stock-1.168.0/Arc.app/Contents/MacOS/Arc',
        'swap/stock-1.168.0/Arc.app/Contents/Resources/Base.lproj/MainMenu.nib']}

    for rel in ['xcode.env', 'chromium/src/out/Base/args.gn', 'patches/sdk-compat-min']:
        if (root / rel).exists():
            inputs.add(root / rel)
    return {os.path.relpath(p.resolve(), root): digest(regular(p)) for p in sorted(inputs)}

def validate_record(path):
    rows = [json.loads(line) for line in path.read_text().splitlines() if line.strip()]
    if not rows or not all(isinstance(row, dict) for row in rows):
        raise RuntimeError('invalid recorder rows ' + str(path))
    if not any(row.get('ev') == 'hooked' and row.get('engine_classes', 0) > 0 for row in rows):
        raise RuntimeError('missing recorder hook ' + str(path))
    if not all(any(row.get('ev') == ev for row in rows) for ev in ['call', 'ret']):
        raise RuntimeError('empty call/return evidence ' + str(path))

def command(args, root, out, name, env=None):
    log = out / name
    log.parent.mkdir(parents=True, exist_ok=True)
    print('RUN', name, flush=True)
    with log.open('w') as f:
        code = subprocess.run(args, cwd=root, env=env, stdout=f, stderr=subprocess.STDOUT).returncode
    print(name, 'exit', code, flush=True)
    return code

def main():
    import re
    ap = argparse.ArgumentParser()
    ap.add_argument('--root', type=Path, required=True)
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('--source-manifest', type=Path, required=True)
    ap.add_argument('--scenarios', nargs='+', choices=list(EXPECTED_STEPS), default=list(EXPECTED_STEPS))
    ap.add_argument('--core-receipt-diagnostics', action='store_true')
    args = ap.parse_args()
    root, out = args.root.resolve(), args.out.resolve()
    require_outer_lock(root)
    # Explicit new directory: no result deletion or accidental previous-run reuse.
    out.mkdir(parents=True, exist_ok=False)
    expected = json.loads(args.source_manifest.read_text())
    app = (root / '../arc/build/ArcModAgent-ours156.app').resolve()
    profile = Path.home() / 'Library/Application Support/ArcModAgent-ours156'
    binary = app / 'Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore'
    framework = root / 'chromium/src/out/Base/Chromium.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore'
    functional = {}
    try:
        require_stopped(root)
        if profile.is_symlink():
            raise RuntimeError('refuse symlink disposable profile')
        if source_seal(root, expected) != expected:
            raise RuntimeError('expected native/Arc source base drift before generation')
        inputs = input_seal(root)
        dump(out / 'runtime-inputs.json', inputs)
        dump(out / 'sources-before.json', source_seal(root, expected))
        for sc in args.scenarios:
            golden = root / 'results/golden' / ('arc-1.170-' + sc)
            if not json.loads((golden / 'coverage.json').read_text()).get('accepted'):
                raise RuntimeError('unaccepted golden ' + sc)
            records = sorted(golden.glob('launch-*/record.jsonl'))
            if len(records) != (2 if sc == 'launch-slice' else 1):
                raise RuntimeError('wrong golden launch count ' + sc)
            for record in records:
                validate_record(record)
            shutil.copytree(golden, out / 'goldens' / sc)
        for rel in inputs:
            p = (root / rel).resolve()
            dest = (out / 'tools/sibling-arc' / p.relative_to((root / '../arc').resolve())
                    if rel.startswith('../') else out / 'tools' / rel)
            dest.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(p, dest)
        if command(['python3', 'arc/tools/gen_api_table.py'], root, out, 'generator.log'):
            raise RuntimeError('generator failed')
        arc_before = arc_seal(root)
        dump(out / 'arc-build-inputs-before.json', arc_before)
        dump(out / 'native-inputs-before.json', {r: h for r, h in expected.items() if r.startswith('chromium/src/')})
        for rel in arc_before.keys() | expected.keys():
            if (root / rel).is_file():
                dest = out / 'sources' / rel
                dest.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(root / rel, dest)
        def stable():
            if source_seal(root, expected) != expected:
                raise RuntimeError('native/Arc selected sources changed')
            if arc_seal(root) != arc_before:
                raise RuntimeError('Arc build/runtime source inventory changed')
            if input_seal(root) != inputs:
                raise RuntimeError('runtime/build input inventory changed')
        stable()
        if command(['./build-babysit.sh'], root, out, 'build.log', {**os.environ, 'ARC_BUILD_FAIL_FAST': '1'}):
            raise RuntimeError('build failed')
        stable()
        dump(out / 'arc-build-inputs-after.json', arc_seal(root))
        (out / 'framework.sha256').write_text(str(digest(regular(framework))) + '\n')
        if command(['./package-engine.sh', 'ours156'], root, out, 'package.log'):
            raise RuntimeError('package failed')
        packaged = digest(regular(binary))
        (out / 'packaged-framework.sha256').write_text(packaged + '\n')
        stable()
        for sc in args.scenarios:
            require_stopped(root)
            stable()
            if digest(binary) != packaged:
                raise RuntimeError('packaged binary changed before ' + sc)
            if profile.is_symlink():
                raise RuntimeError('refuse symlink profile')
            if profile.exists():
                shutil.rmtree(profile)
            harness_argv = ['bun', 'tests/launch-slice/run.ts', '--app', str(app), '--out', str(out / sc),
                            '--record', '--scenario', sc] + (['--core-receipt-diagnostics'] if args.core_receipt_diagnostics else [])
            dump(out / (sc + '.harness-argv.json'), harness_argv)
            code = command(harness_argv, root, out, sc + '.log')
            (out / (sc + '.harness-exit')).write_text(str(code) + '\n')
            require_stopped(root)
            logs = sorted((out / sc).glob('launch-*/app.log'))
            if len(logs) != (2 if sc == 'launch-slice' else 1):
                raise RuntimeError('missing app logs ' + sc)
            fatal = []
            for log in logs:
                hits = [{'line': i + 1, 'text': line} for i, line in enumerate(log.read_text(errors='replace').splitlines())
                        if re.search(r'FATAL|Check failed:|Received signal|EXC_BAD_ACCESS|terminating due to uncaught', line)]
                if hits:
                    fatal.append({'log': str(log.relative_to(out)), 'matches': hits})
            dump(out / (sc + '.fatal-signatures.json'), fatal)
            receipt = json.loads((out / sc / 'steps.json').read_text())
            steps = receipt['steps']
            labels_exact = [s.get('step') for s in steps] == EXPECTED_STEPS[sc]
            quits = [s for s in steps if s.get('step') in ('S6 quit', 'S8 quit', 'T11 quit')]
            wanted = ['S6 quit', 'S8 quit'] if sc == 'launch-slice' else ['T11 quit']
            exact_quit = ([s.get('step') for s in quits] == wanted and all(
                s.get('outcome') == 'pass' and re.fullmatch(r'Browser\.close; browser \+ \d+ helpers exited', s.get('detail', ''))
                for s in quits))
            dump(out / (sc + '.quit-receipts.json'), quits)
            coverage = json.loads((out / sc / 'coverage.json').read_text())
            passed = (code == 0 and labels_exact and all(s.get('outcome') == 'pass' for s in steps)
                      and exact_quit and not fatal and coverage.get('accepted') is True
                      and receipt.get('scenario') == sc and Path(receipt.get('app', '')).resolve() == app)
            functional[sc] = {'harness_exit': code, 'all_steps_pass': passed, 'exact_labels': labels_exact,
                              'exact_quit': bool(exact_quit), 'steps': steps}
            dump(out / 'functional-results.json', functional)
            for golden in sorted((out / 'goldens' / sc).glob('launch-*/record.jsonl')):
                ours = out / sc / golden.parent.name / 'record.jsonl'
                validate_record(ours)
                if command(['python3', str(out / 'tools/tests/recorder/diff.py'), str(golden), str(ours), '--json',
                            str(out / sc / ('diff-' + golden.parent.name + '.json'))], root, out,
                           sc + '/diff-' + golden.parent.name + '.txt'):
                    raise RuntimeError('raw comparator execution failed')
                print(sc, golden.parent.name, (out / sc / ('diff-' + golden.parent.name + '.txt')).read_text().splitlines()[:2], flush=True)
            stable()
            if digest(binary) != packaged:
                raise RuntimeError('packaged binary drift during ' + sc)
            if not passed:
                raise RuntimeError('functional/fatal/exact-quit gate failed ' + sc)
        require_stopped(root)
        stable()
        if digest(binary) != packaged:
            raise RuntimeError('packaged binary changed across run')
        dump(out / 'sources-final.json', source_seal(root, expected))
        dump(out / 'native-inputs-final.json', {r: digest(root / r) for r in expected if r.startswith('chromium/src/')})
        dump(out / 'arc-build-inputs-final.json', arc_seal(root))
        dump(out / 'runtime-inputs-after.json', input_seal(root))
        dump(out / 'completion.json', {'status': 'PASS', 'all_scenarios_pass': True, 'scenarios': args.scenarios,
             'output': str(out), 'packaged_sha256': packaged, 'runtime_inputs_stable': True,
             'selected_native_sources_stable': True, 'arc_sources_stable': True,
             'app_and_helpers_exited': True, 'no_fatal_signatures_in_app_logs': True,
             'quit_evidence': 'exact Browser.close receipts + PID/helper absence; OS normal-exit status not captured',
             'boundary_diff_status': 'raw reports retained; comparator exit is execution success, not parity'})
        return 0
    except BaseException as exc:
        dump(out / 'completion.json', {'status': 'FAIL', 'error': repr(exc), 'output': str(out),
                                     'live_agent_processes': agent_processes(root)})
        print('FAIL', repr(exc), out, flush=True)
        return 1

if __name__ == '__main__':
    sys.exit(main())
