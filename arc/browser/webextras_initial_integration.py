#!/usr/bin/env python3
"""Held initial integration executor. No sealed configuration is supplied.

The parent seals exact images/commands after review. This program never signals
apps, restores a Git index, accepts global boundary parity, or releases a HOLD.
"""
import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import signal
import stat
import subprocess
import sys
import time
import uuid

ROOT = Path(__file__).resolve().parents[2]
SCENARIOS = ('launch-slice', 'devtools', 'media', 'pdf', 'media-pip')
SIGNALS = (signal.SIGINT, signal.SIGTERM, signal.SIGHUP)
LAUNCH = ('S1 launch', 'S2 open A', 'S3 navigate', 'S4 store', 'S5 close + new tab',
          'S6 quit', 'S7 relaunch', 'S7 readback', 'S8 quit')
SECTIONS = ('missing', 'extra', 'return_mismatch', 'arg_mismatch', 'count_mismatch')
CANCELLED = False


def cancel_handler(path):
    def request(*_):
        global CANCELLED
        CANCELLED = True
        try:
            path.touch(exist_ok=True)
        except OSError:
            pass
    return request


def require(value, message):
    if not value:
        raise RuntimeError(message)


def sha(path):
    path = Path(path)
    require(not path.is_symlink(), 'symlink file: ' + str(path))
    if not path.exists():
        return None
    require(path.is_file(), 'nonregular file: ' + str(path))
    before = path.stat()
    digest = hashlib.sha256()
    with path.open('rb') as source:
        for block in iter(lambda: source.read(1024 * 1024), b''):
            digest.update(block)
    after = path.stat()
    require(all(getattr(before, n) == getattr(after, n) for n in
                ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')),
            'changed while hashing: ' + str(path))
    return digest.hexdigest()


def write(path, value):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_name(path.name + '.tmp-' + uuid.uuid4().hex)
    temporary.write_text(json.dumps(value, indent=2) + '\n')
    temporary.replace(path)


def relative(root, name):
    require(isinstance(name, str) and name and not Path(name).is_absolute()
            and '..' not in Path(name).parts, 'unsafe relative path')
    result = root / name
    require(result.resolve() == root.resolve() or result.parent.resolve().is_relative_to(root.resolve()), 'escaped root: ' + name)
    require(not result.is_symlink(), 'symlink target: ' + name)
    return result


def pin_path(root, name):
    path = Path(name)
    if not path.is_absolute():
        return relative(root, name)
    require(path.resolve().is_relative_to(root.resolve()) or
            path.resolve().is_relative_to((root.parent / 'arc').resolve()), 'external pin outside checkout/sibling Arc')
    require(not path.is_symlink(), 'symlink external pin')
    return path


def atomic_bytes(path, data, mode=0o644):
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_name(path.name + '.new-' + uuid.uuid4().hex)
    with temporary.open('xb') as output:
        output.write(data)
        output.flush()
        os.fsync(output.fileno())
    temporary.chmod(mode)
    temporary.replace(path)


def template():
    return {'schema': 1, 'status': 'UNSEALED', 'repo': str(ROOT),
            'acknowledgments': {'hostGate': '', 'regenerationInterface': ''},
            'pins': {}, 'inventory': {}, 'patches': [], 'targets': {},
            'manifest': {}, 'patchOutputs': {}, 'gitRepos': [],
            'regeneration': {'argv': [], 'cwd': '.', 'env': {}},
            'observationTool': {}, 'resourceChecks': {'candidate': {}, 'recovery': {}},
            'baselineFiles': {}, 'runNumbers': {},
            'note': 'No execution until parent seals reviewed exact images, tools and ACK evidence.'}


def validate(config, root):
    require(config.get('schema') == 1 and config.get('status') == 'SEALED',
            'UNSEALED: no lock, publication, build or app execution permitted')
    require(Path(config.get('repo', '')).resolve() == root.resolve(), 'wrong repository')
    require(all(isinstance(config.get('acknowledgments', {}).get(k), str)
                and config['acknowledgments'][k].strip()
                for k in ('hostGate', 'regenerationInterface')), 'missing reviewed ACK references')
    digest = re.compile(r'[0-9a-f]{64}')
    for key in ('pins', 'baselineFiles'):
        require(isinstance(config.get(key), dict) and config[key], 'missing ' + key)
        for name, value in config[key].items():
            pin_path(root, name)
            require(isinstance(value, str) and digest.fullmatch(value), 'invalid pin: ' + name)
    require(isinstance(config.get('targets'), dict) and config['targets'], 'empty targets')
    require('patches/MANIFEST' not in config['targets'], 'MANIFEST must use its separate journal')
    images = {**config['targets'], 'patches/MANIFEST': config.get('manifest', {})}
    observation_path = 'arc/browser/webextras_scenario_current_run.py'
    observation = config.get('observationTool', {})
    require(set(observation) == {'before', 'after', 'blob', 'patch'} and observation_path not in images,
            'explicit separate persistent observation-tool contract required')
    require(all(isinstance(observation.get(k), str) and digest.fullmatch(observation[k]) for k in ('before', 'after'))
            and observation['before'] != observation['after'], 'invalid observation-tool images')
    require(config['pins'].get(observation['blob']) == observation['after'] and observation['patch'] in config['pins'],
            'persistent tool proposal/images not pinned')
    require(observation_path not in config['pins'], 'persistent tool must use its separate two-phase image')
    for name, image in images.items():
        relative(root, name)
        require(set(image) == {'before', 'after', 'blob'}, 'invalid image: ' + name)
        require(image['before'] is None or digest.fullmatch(image['before']), 'invalid preimage')
        require(isinstance(image['after'], str) and digest.fullmatch(image['after']), 'invalid postimage')
        require(image['before'] != image['after'], 'unchanged publication target')
        relative(root, image['blob'])
    outputs = config.get('patchOutputs')
    require(isinstance(outputs, dict) and outputs, 'missing all regeneration output images')
    for name, image in outputs.items():
        require(name.startswith('patches/') and name.endswith('.patch'), 'invalid patch output')
        relative(root, name)
        require(set(image) == {'before', 'after'} and all(v is None or
                (isinstance(v, str) and digest.fullmatch(v)) for v in image.values()), 'invalid output image')
    require(config.get('patches'), 'missing reviewed ordered patch inventory')
    for patch in config['patches']:
        require(set(patch) == {'path', 'sha256', 'cwd'}, 'invalid patch inventory')
        relative(root, patch['path']); relative(root, patch['cwd'])
        require(config['pins'].get(patch['path']) == patch['sha256'], 'patch not pinned')
    require(config.get('inventory'), 'missing production inventory')
    require(set(config['inventory']) == {'arc/api', 'arc/browser', 'arc/wp'}, 'incomplete Arc production inventory')
    for folder, names in config['inventory'].items():
        require(isinstance(names, list) and len(names) == len(set(names)), 'invalid production inventory')
        require(all(n.startswith(folder + '/') and (n in images or n in config['pins']) for n in names),
                'production content not pinned')
    require(config.get('gitRepos'), 'missing index protection')
    for repo in config['gitRepos']:
        require(set(repo) == {'path', 'index', 'sha256', 'head', 'environmentKey', 'intentToAdd'}, 'invalid Git repository contract')
        relative(root, repo['path']); relative(root, repo['index'])
        require(digest.fullmatch(repo['sha256']) and re.fullmatch(r'[A-Z_][A-Z0-9_]*', repo['environmentKey'])
                and repo['environmentKey'] not in ('HOME', 'PATH', 'PWD', 'CODEX_HOME'), 'invalid sealed alternate-index interface')
        require(isinstance(repo['intentToAdd'], list), 'invalid intent-to-add inventory')
        for name in repo['intentToAdd']:
            target = str(Path(repo['path']) / name)
            relative(root, target)
            require(target in config['targets'] and config['targets'][target]['before'] is None,
                    'intent-to-add is not an exact newly owned target')
    require(len({r['environmentKey'] for r in config['gitRepos']}) == len(config['gitRepos']),
            'duplicate alternate-index environment interface')
    regeneration = config.get('regeneration', {})
    require(set(regeneration) == {'argv', 'cwd', 'env'} and regeneration['argv']
            and all(isinstance(x, str) and x for x in regeneration['argv']), 'unsealed regeneration argv')
    relative(root, regeneration['cwd'])
    require(isinstance(regeneration['env'], dict) and all(isinstance(k, str) and isinstance(v, str)
            for k, v in regeneration['env'].items()), 'invalid sealed regeneration environment')
    numbers = config.get('runNumbers', {})
    require(set(numbers) == {*SCENARIOS, 'recovery-launch-slice'}, 'incomplete runtime inventory')
    values = [numbers[k] for k in (*SCENARIOS, 'recovery-launch-slice')]
    require(all(type(v) is int and v > 1 for v in values) and values == sorted(set(values)),
            'run numbers must be distinct increasing integers above baseline1')
    runner = 'arc/browser/webextras_scenario_current_run.py'
    require(all(n in config['pins'] for n in ('arc/tools/locked.sh', 'arc/BUILD.gn',
                'arc/browser/webextras_initial_integration.py', 'arc/tools/gen_api_table.py',
                'build-babysit.sh', 'package-engine.sh', 'chromium/src/out/Base/args.gn')),
            'executor/build/lock/scenario runner not pinned')
    require(not (set(config['pins']) & {*images, *outputs}), 'mutable image also listed as immutable pin')
    require(all(config['pins'].get(x['blob']) == x['after'] for x in images.values()), 'postimage blob not pinned')
    for scenario in SCENARIOS:
        base = 'results/webextras/' + scenario + '-current-run1/'
        require(all(base + n in config['baselineFiles'] for n in
                    ('steps.json', 'coverage.json', 'runner-summary.json', 'recovery.json',
                     'input-manifest.json', 'binary-identity.json', 'binary-identity-after-run.json')),
                'baseline not frozen: ' + scenario)
        golden = root / ('results/golden/arc-1.170-' + scenario)
        golden_files = [golden / 'steps.json', golden / 'coverage.json', *golden.glob('launch-*/record.jsonl')]
        require(len(golden_files) > 2 and all(str(p.relative_to(root)) in config['pins'] for p in golden_files),
                'accepted golden artifacts not pinned: ' + scenario)
    checks = config.get('resourceChecks', {})
    require(set(checks) == {'candidate', 'recovery'}, 'both resource-check roles must be sealed')
    checker = 'arc/browser/webextras_frontend_resource_check.py'
    require(checker in config['pins'], 'resource checker not pinned')
    for role, check in checks.items():
        require(set(check) == {'argv', 'cwd', 'expectHostGate'} and check['expectHostGate'] in ('present', 'absent'),
                'invalid role-specific resource check')
        argv = check['argv']; relative(root, check['cwd'])
        require(isinstance(argv, list) and all(isinstance(a, str) for a in argv) and argv.count('{resource_output}') == 1
                and (checker in argv or str(root / checker) in argv), 'unsealed resource-check argv')
        require('--expect-host-gate' in argv and argv[argv.index('--expect-host-gate') + 1] == check['expectHostGate']
                and '--output' in argv and argv[argv.index('--output') + 1] == '{resource_output}', 'resource CLI/mode mismatch')
    return config


def process_inventory():
    output = subprocess.check_output(['ps', '-axo', 'pid=,ppid=,pgid=,stat=,command=', '-ww'], text=True)
    rows = []
    for line in output.splitlines():
        parts = line.strip().split(None, 4)
        require(len(parts) == 5, 'unparseable process inventory')
        rows.append(dict(pid=int(parts[0]), ppid=int(parts[1]), pgid=int(parts[2]), state=parts[3], command=parts[4]))
    return rows


class Backend:
    def __init__(self, root, output, cancel):
        self.root, self.output, self.cancel = root, output, cancel
        self.groups = set()

    def inventory(self):
        return process_inventory()

    def command(self, argv, label, cwd=None, env=None):
        with (self.output / (label + '.log')).open('xb') as log:
            child = subprocess.Popen(argv, cwd=cwd or self.root, env=env, stdin=subprocess.DEVNULL,
                                     stdout=log, stderr=subprocess.STDOUT, start_new_session=True)
            self.groups.add(child.pid)
            try:
                write(self.output / (label + '.child.json'), {'pid': child.pid, 'pgid': child.pid, 'argv': argv})
            finally:
                # Deferred signals cannot interrupt this wait or start rollback.
                while True:
                    try:
                        child.wait()
                        if not any(r['pgid'] == child.pid and not r['state'].startswith('Z') for r in self.inventory()):
                            break
                    except BaseException:
                        pass  # Failed census/bookkeeping cannot release a child group.
                    time.sleep(.2)
            return child.returncode


def scenario_gate(root, output, scenario, baseline, child_exit):
    summary = json.loads((output / 'runner-summary.json').read_text())
    steps = json.loads((output / 'steps.json').read_text())
    coverage = json.loads((output / 'coverage.json').read_text())
    prior_steps = json.loads((baseline / 'steps.json').read_text())
    prior = json.loads((baseline / 'runner-summary.json').read_text())
    golden = root / ('results/golden/arc-1.170-' + scenario)
    golden_steps = json.loads((golden / 'steps.json').read_text())
    golden_coverage = json.loads((golden / 'coverage.json').read_text())
    require(golden_steps.get('scenario') == scenario and golden_coverage.get('accepted') is True
            and golden_steps.get('steps') and all(x['outcome'] == 'pass' for x in golden_steps['steps']),
            'unaccepted golden step contract')
    require(type(child_exit) is int and child_exit in (0, 1), 'operational runner failure')
    require(type(summary.get('harnessExit')) is int and summary['harnessExit'] in (0, 1), 'invalid harness exit')
    require(summary.get('scenario') == scenario and steps.get('scenario') == scenario
            and coverage.get('scenario') == scenario, 'scenario identity mismatch')
    require(output == root / f"results/webextras/{scenario}-current-run{summary['runNumber']}", 'run-number receipt mismatch')
    require(summary.get('errors') == [] and summary.get('profileRestored') is True
            and coverage.get('errors') == [], 'runner/source/profile failure')
    rows = [(x['step'], x['outcome']) for x in steps['steps']]
    old = [(x['step'], x['outcome']) for x in prior_steps['steps']]
    all_pass = bool(rows) and all(x[1] == 'pass' for x in rows)
    full_pass = (all_pass and [x[0] for x in rows] == [x['step'] for x in golden_steps['steps']]
                 and summary.get('harnessExit') == 0 and summary.get('runtimeAccepted') is True
                 and coverage.get('accepted') is True)
    same = (rows == old and summary.get('harnessExit') == prior.get('harnessExit')
            and summary.get('runtimeAccepted') == prior.get('runtimeAccepted')
            and coverage.get('accepted') == json.loads((baseline / 'coverage.json').read_text()).get('accepted')
            and all(new.get('detail') == before.get('detail') for new, before in zip(steps['steps'], prior_steps['steps'])
                    if new['outcome'] != 'pass'))
    require(full_pass or same, 'new/earlier canonical step regression')
    if scenario == 'launch-slice':
        require(tuple(x[0] for x in rows) == LAUNCH and full_pass, 'launch not exact accepted9/9')
    require(steps.get('slug') == 'ours156' and steps.get('recorded') is True, 'wrong/unrecorded app receipt')
    require(Path(steps.get('app', '')).resolve() == (root.parent / 'arc/build/ArcModAgent-ours156.app').resolve(),
            'wrong disposable app')
    require(not any(x['step'] == 'cleanup quit' for x in steps['steps']), 'fallback quit used')
    quits = [x for x in steps['steps'] if 'quit' in x['step'].lower()]
    require(quits and all(x['outcome'] == 'pass' and re.fullmatch(
        r'Browser\.close; browser \+ \d+ helpers exited', x.get('detail', '')) for x in quits), 'missing natural-close receipts')
    canonical_launches = ['launch-1', 'launch-2'] if scenario in ('launch-slice', 'devtools') else ['launch-1']
    require(coverage.get('launches') == canonical_launches,
            'wrong canonical launch set')
    expected_launches = set(canonical_launches)
    require(expected_launches and expected_launches == {p.name for p in output.glob('launch-*') if p.is_dir()},
            'launch/coverage inventory mismatch')
    for launch in expected_launches:
        app_log = output / launch / 'app.log'
        require(app_log.is_file() and app_log.stat().st_size, 'missing app log')
        require(not re.search(r'(?:\[(?:[^\]\n]*:)?D?FATAL:|Check failed:|Assertion failed:|EXC_BAD_(?:ACCESS|INSTRUCTION)|'
                              r'\bSIG(?:ABRT|SEGV)\b|Received signal (?:5|6|10|11)\b|Thread \d+ Crashed:|^Fatal error:)',
                              app_log.read_text(errors='replace'), re.M), 'fatal app marker')
        record = output / launch / 'record.jsonl'
        events = [json.loads(x) for x in record.read_text().splitlines()]
        require(any(x.get('ev') == 'hooked' and x.get('engine_classes', 0) > 0 for x in events)
                and any(x.get('ev') == 'call' for x in events), 'empty/uninstrumented transcript')
        report = json.loads((output / ('diff-' + launch + '.json')).read_text())
        require(all(isinstance(report.get(k), list) for k in SECTIONS)
                and isinstance(report.get('callback_sequence'), dict), 'missing raw comparator evidence')
        require(len((output / ('diff-' + launch + '.txt')).read_text().splitlines()) >= 2, 'missing raw comparator text')
    before = json.loads((output / 'binary-identity.json').read_text())
    after = json.loads((output / 'binary-identity-after-run.json').read_text())
    require(all(before[k] == after[k] for k in ('main', 'engine', 'helpers', 'recorder', 'resources')), 'binary proof mismatch')
    app = root.parent / 'arc/build/ArcModAgent-ours156.app'
    require(Path(before['main']['path']) == app / 'Contents/MacOS/Arc'
            and Path(before['engine']['path']) == app / 'Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore'
            and Path(before['recorder']['path']) == root / 'tests/recorder/ArcEngineRecorder.dylib'
            and Path(before['resources']['path']) == app / 'Contents/Frameworks/ArcCore.framework/Versions/A/Resources/resources.pak'
            and before['helpers'] and all(Path(x['path']).is_relative_to(app / 'Contents/Frameworks/ArcCore.framework')
                                        for x in before['helpers']), 'wrong packaged binary inventory')
    for item in [before['main'], before['engine'], before['recorder'], before['resources'], *before['helpers']]:
        source = Path(item['path']); actual = source.resolve(strict=True).stat()
        require(str(source.resolve(strict=True)) == item['realpath'] and sha(source) == item['sha256']
                and all(getattr(actual, k) == item[v] for k, v in
                        (('st_dev', 'dev'), ('st_ino', 'ino'), ('st_size', 'size'),
                         ('st_mtime_ns', 'mtimeNs'), ('st_ctime_ns', 'ctimeNs'))), 'actual packaged binary drift')
    require(summary.get('comparisons') and len(summary['comparisons']) == len(expected_launches), 'incomplete comparisons')
    require({c.get('launch') for c in summary['comparisons']} == expected_launches, 'duplicated comparison inventory')
    recovery = json.loads((output / 'recovery.json').read_text())
    profile = Path.home() / 'Library/Application Support/ArcModAgent-ours156'
    require(Path(recovery['profile']) == profile and recovery.get('remainingProcesses') == []
            and recovery.get('status') in ('restored', 'restored-original-absence'), 'unconfirmed profile recovery receipt')
    if recovery['originalExisted']:
        require(directory_id(profile) == recovery['savedIdentity'], 'restored profile identity mismatch')
    else:
        require(directory_id(profile) is None, 'original profile absence not restored')
    manifest = json.loads((output / 'input-manifest.json').read_text())['files']
    require(manifest, 'empty source manifest')
    for name, item in manifest.items():
        require(sha(name) == item['source']['sha256'] == sha(item['frozen']['path']), 'child source/frozen input drift')
    stem = root / f"logs/wp/webextras.{scenario}.current-run{summary['runNumber']}"
    phases = ['before', 'after-build', 'before-run', 'after-run', 'final']
    phases += [f'{position}-diff-{launch}' for launch in expected_launches for position in ('before', 'after')]
    for phase in phases:
        receipt = json.loads(Path(str(stem) + '.source-' + phase + '.json').read_text())
        require(receipt.get('phase') == phase and receipt.get('files') == manifest, 'child source phase proof incomplete')
    return {'scenario': scenario, 'childExit': child_exit, 'harnessExit': summary['harnessExit'],
            'noEarlierStepRegression': True, 'allStepsPass': full_pass,
            'boundaryParity': 'NOT ASSERTED; raw default diffs retained',
            'quitEvidence': 'Browser.close receipt + process absence; OS normal-exit status not captured'}


def directory_id(path):
    require(not path.is_symlink(), 'symlink directory identity')
    if not path.exists():
        return None
    require(path.is_dir(), 'non-directory identity')
    value = path.stat()
    return {'dev': value.st_dev, 'ino': value.st_ino}


class Transaction:
    def __init__(self, config, output, backend=None):
        self.root, self.output, self.config = ROOT, Path(output), validate(config, ROOT)
        self.cancel = self.output / 'cancel.request'
        self.backend = backend or Backend(self.root, self.output, self.cancel)
        self.images = {**config['targets'], 'patches/MANIFEST': config['manifest']}
        self.mutated = False
        self.phase = 0
        self.current = 'before'
        self.lock = None
        self.indexes = {}
        self.roots = {}
        receipt = self.output / 'observation-tool.json'
        self.tool_adopted = (receipt.is_file() and json.loads(receipt.read_text()).get('status') == 'PERSISTENT_OBSERVATION_TOOL_ADOPTED')
        require(json.loads((self.output / 'config.json').read_text()) == config, 'config evidence mismatch')
        self.config_digest = sha(self.output / 'config.json')

    def cancelled(self):
        require(not CANCELLED and not self.cancel.exists(), 'cancellation requested; stop before next candidate phase')

    def hold(self, reason):
        # Parent owns process reaping/reconciliation. Never release uncertainty.
        while True:
            try:
                write(self.output / 'HOLD.json', {'reason': str(reason), 'mustRetainLock': True,
                                                'noNormalOSExitClaim': True})
            except OSError:
                pass
            time.sleep(1)

    def lock_check(self):
        lock = self.root / 'logs/arccore-build.lock'
        require(lock.is_dir() and not lock.is_symlink(), 'missing real outer lock')
        pidfile = lock / 'pid'; require(pidfile.is_file() and not pidfile.is_symlink(), 'invalid lock pid')
        raw = pidfile.read_text(); owner = int(raw)
        rows = {r['pid']: r for r in self.backend.inventory()}
        pid = os.getpid()
        for _ in range(64):
            if pid == owner:
                break
            require(pid in rows and pid > 1, 'lock owner not ancestor')
            pid = rows[pid]['ppid']
        else:
            raise RuntimeError('ancestry bound')
        protected = json.loads((self.output / 'protected.json').read_text())
        require(protected['pid'] == owner, 'lock owner is not protected launcher')
        stamp = [lock.stat().st_dev, lock.stat().st_ino, pidfile.stat().st_ino, raw]
        require(self.lock is None or stamp == self.lock, 'lock identity drift')
        self.lock = stamp

    def stopped(self):
        app = self.root.parent / 'arc/build/ArcModAgent-ours156.app'
        prefixes = {str(app / 'Contents/MacOS/Arc'), str((app / 'Contents/MacOS/Arc').resolve()),
                    str(app / 'Contents/Frameworks/ArcCore.framework') + '/',
                    str((app / 'Contents/Frameworks/ArcCore.framework').resolve()) + '/'}
        rows = self.backend.inventory()
        survivors = [r for r in rows if not r['state'].startswith('Z') and
                     (r['pgid'] in self.backend.groups or any(r['command'] == p or
                      r['command'].startswith(p if p.endswith('/') else p + ' ') for p in prefixes))]
        require(not survivors, 'owned phase/app/helper survivors: ' + repr(survivors))

    def pin_check(self):
        require(sha(self.output / 'config.json') == self.config_digest, 'sealed config evidence drift')
        for name, value in {**self.config['pins'], **self.config['baselineFiles']}.items():
            require(sha(pin_path(self.root, name)) == value, 'immutable input drift: ' + name)
        observation = self.config['observationTool']
        require(sha(self.root / 'arc/browser/webextras_scenario_current_run.py') == observation['after' if self.tool_adopted else 'before'],
                'persistent observation-tool drift')
        for folder, expected in self.config['inventory'].items():
            base = relative(self.root, folder)
            actual = sorted(str(p.relative_to(self.root)) for p in base.rglob('*')
                            if p.is_file() and p.suffix in ('.mm', '.cc', '.h', '.gn', '.gni')
                            and not (str(p.relative_to(self.root)) in self.images and
                                     self.images[str(p.relative_to(self.root))]['before'] is None))
            require(actual == sorted(expected), 'production inventory drift: ' + folder)

    def file_state(self, phase):
        self.lock_check(); self.pin_check()
        paths = {**self.images, **self.config['patchOutputs']}
        actual = {name: sha(relative(self.root, name)) for name in paths}
        require(all(actual[n] == v[phase] for n, v in paths.items()), 'source/patch phase drift')
        require({str(p.relative_to(self.root)) for p in (self.root / 'patches').glob('*.patch')} ==
                {n for n, image in self.config['patchOutputs'].items() if image[phase] is not None},
                'all-patch-output inventory drift')
        for repo in self.config['gitRepos']:
            index = relative(self.root, repo['index'])
            git_env = {k: v for k, v in os.environ.items() if k != 'GIT_INDEX_FILE'}
            git_env['GIT_OPTIONAL_LOCKS'] = '0'
            actual_index = subprocess.check_output(['git', 'rev-parse', '--git-path', 'index'],
                cwd=relative(self.root, repo['path']), env=git_env, text=True).strip()
            actual_index = Path(actual_index) if Path(actual_index).is_absolute() else relative(self.root, repo['path']) / actual_index
            require(actual_index.resolve() == index.resolve(), 'configured index is not the actual repository index')
            if sha(index) != repo['sha256']:
                # Preserve real staging even for suspected metadata-only refresh.
                write(self.output / 'index-drift.json', {'path': repo['index'], 'before': repo['sha256'],
                       'actual': sha(index), 'policy': 'HOLD; never restore real index'})
                diagnostic = subprocess.run(['git', 'ls-files', '--stage', '-v', '--debug'],
                    cwd=relative(self.root, repo['path']), env={**os.environ, 'GIT_OPTIONAL_LOCKS': '0'},
                    stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                (self.output / 'index-semantic-diagnostic.txt').write_bytes(diagnostic.stdout[:65536])
                raise RuntimeError('real index drift; semantic review required')
            head = subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=relative(self.root, repo['path']),
                                           env={**os.environ, 'GIT_OPTIONAL_LOCKS': '0'}, text=True).strip()
            require(head == repo['head'], 'Git HEAD drift')
        return actual

    def command(self, argv, label, cwd=None, env=None, rollback=False):
        if not rollback:
            self.cancelled()
        self.lock_check(); self.pin_check()
        self.phase += 1
        stem = f'{self.phase:03d}-{label}'
        write(self.output / (stem + '.inputs.json'), {'images': {n: sha(relative(self.root, n)) for n in
              {**self.images, **self.config['patchOutputs']}}, 'immutablePins': self.config['pins']})
        code = self.backend.command(argv, stem, cwd, env)
        self.lock_check(); self.pin_check()
        write(self.output / (stem + '.exit.json'), {'exit': code})
        if not rollback:
            self.cancelled()
        return code

    def prepare(self):
        self.file_state('before'); self.stopped(); self.cancelled()
        expected_outputs = set(self.config['patchOutputs'])
        actual_outputs = {str(p.relative_to(self.root)) for p in (self.root / 'patches').glob('*.patch')}
        require(actual_outputs == {n for n in expected_outputs if self.config['patchOutputs'][n]['before'] is not None},
                'incomplete all-patch-output journal')
        self.roots = {str(p): directory_id(p) for p in (
            Path.home() / 'Library/Application Support/ArcModAgent-ours156',
            Path.home() / 'Library/Caches/ArcModAgent-ours156')}
        write(self.output / 'directory-identities.json', self.roots)
        prior_ownership = None
        for phase, data in [('before', relative(self.root, 'patches/MANIFEST').read_text()),
                            ('after', relative(self.root, self.config['manifest']['blob']).read_text())]:
            ownership = {}
            for line in data.splitlines():
                if not line.strip() or line.lstrip().startswith('#'):
                    continue
                fields = line.split(); require(len(fields) == 2, 'malformed MANIFEST')
                patch, name = fields
                require(name not in ownership and Path(patch).name == patch and patch.endswith('.patch'), 'duplicate/unsafe ownership')
                ownership[name] = patch
                output = 'patches/' + patch
                require(output in self.config['patchOutputs'] and self.config['patchOutputs'][output][phase] is not None,
                        'MANIFEST owner lacks journalled patch image: ' + output)
                key = 'chromium/src/' + name
                relative(self.root, key)
                require(key in self.config['targets'] or key in self.config['pins'], 'MANIFEST input not guarded: ' + key)
            if prior_ownership is not None:
                require(all(ownership.get(n) == p for n, p in prior_ownership.items()), 'existing MANIFEST ownership changed')
            prior_ownership = ownership
            for name in self.config['targets']:
                if name.startswith('chromium/src/'):
                    leaf = name.removeprefix('chromium/src/')
                    if phase == 'after':
                        require(leaf in ownership, 'native target missing MANIFEST owner')
                        if self.config['targets'][name]['before'] is None:
                            require(ownership[leaf] == '0010-webextras.patch', 'new native target assigned outside own patch')
        for name, image in self.images.items():
            path = relative(self.root, name)
            if image['before'] is not None:
                destination = self.output / 'before' / name
                destination.parent.mkdir(parents=True, exist_ok=True)
                destination.write_bytes(path.read_bytes())
                write(self.output / 'modes' / (name.replace('/', '_') + '.json'), stat.S_IMODE(path.stat().st_mode))
            require(sha(relative(self.root, image['blob'])) == image['after'], 'private postimage drift')
        for repo in self.config['gitRepos']:
            target = self.output / ('alternate-index-' + str(len(self.indexes)))
            atomic_bytes(target, relative(self.root, repo['index']).read_bytes())
            self.indexes[repo['environmentKey']] = str(target)
        write(self.output / 'journal.json', {'images': self.images, 'outputs': self.config['patchOutputs'],
                                            'realIndexesNeverRestored': True})
        # Reconstruct the reviewed patch chain privately from the exact preimages.
        stage = self.output / 'stage'; stage.mkdir()
        for name, image in self.images.items():
            if image['before'] is not None:
                destination = stage / name; destination.parent.mkdir(parents=True, exist_ok=True)
                destination.write_bytes(relative(self.root, name).read_bytes())
        for i, patch in enumerate(self.config['patches']):
            cwd = stage / patch['cwd']; cwd.mkdir(parents=True, exist_ok=True)
            # A nested private repository prevents git apply discovering the live
            # checkout above the evidence directory and escaping the stage.
            if not (cwd / '.git').exists():
                require(self.command(['git', 'init', '--quiet', str(cwd)], f'private-init-{i}') == 0,
                        'private repository initialization failed')
            for check in (True, False):
                argv = ['git', 'apply', '--whitespace=error'] + (['--check'] if check else []) + [str(relative(self.root, patch['path']))]
                require(self.command(argv, f'private-{i}-{check}', cwd, {**os.environ, 'GIT_OPTIONAL_LOCKS': '0'}) == 0,
                        'private patch reconstruction failed')
        require(all(sha(stage / n) == image['after'] for n, image in self.config['targets'].items()),
                'ordered patches do not reconstruct reviewed target images')

    def publish(self):
        self.file_state('before'); self.cancelled()
        self.mutated = True
        write(self.output / 'publication.json', {'state': 'PUBLISHING', 'images': self.images})
        for name, image in self.images.items():
            path = relative(self.root, name)
            require(sha(path) == image['before'], 'foreign edit before publication')
            atomic_bytes(path, relative(self.root, image['blob']).read_bytes())
        self.current = 'after'

    def adopt_observation_tool(self):
        """Separately approved provenance tool survives an engine rollback."""
        self.file_state('before'); self.cancelled()
        image = self.config['observationTool']; path = self.root / 'arc/browser/webextras_scenario_current_run.py'
        before = path.read_bytes(); mode = stat.S_IMODE(path.stat().st_mode)
        private = self.output / 'observation-stage'; (private / 'arc/browser').mkdir(parents=True)
        (private / 'arc/browser/webextras_scenario_current_run.py').write_bytes(before)
        for argv, label in [(['git', 'init', '--quiet', str(private)], 'observation-private-init'),
                            (['git', 'apply', '--check', '--whitespace=error', str(relative(self.root, image['patch']))], 'observation-check'),
                            (['git', 'apply', '--whitespace=error', str(relative(self.root, image['patch']))], 'observation-reconstruct')]:
            require(self.command(argv, label, private) == 0, 'observation tool strict reconstruction failed')
        require(sha(private / 'arc/browser/webextras_scenario_current_run.py') == image['after'], 'observation tool postimage mismatch')
        (self.output / 'observation-tool.before').write_bytes(before)
        try:
            write(self.output / 'observation-tool.json', {'status': 'ADOPTING', **image})
            atomic_bytes(path, relative(self.root, image['blob']).read_bytes(), mode)
            require(sha(path) == image['after'], 'observation tool apply/post-proof failed')
            self.tool_adopted = True
            write(self.output / 'observation-tool.json', {'status': 'PERSISTENT_OBSERVATION_TOOL_ADOPTED', **image,
                  'engineRollbackRetainsThisOwnedReadOnlyTool': True})
        except BaseException:
            if sha(path) not in (image['before'], image['after']):
                self.hold('foreign observation-tool bytes; no engine publication authorized')
            atomic_bytes(path, before, mode); self.tool_adopted = False
            try:
                write(self.output / 'observation-tool.json', {'status': 'REVERTED_BEFORE_ENGINE_PUBLICATION', **image})
            except OSError:
                pass
            raise

    def regenerate(self, rollback=False):
        command = self.config['regeneration']
        env = {**os.environ, **command['env'], **self.indexes, 'GIT_OPTIONAL_LOCKS': '0'}
        if not rollback:
            for repo in self.config['gitRepos']:
                if repo['intentToAdd']:
                    add_env = {**os.environ, 'GIT_OPTIONAL_LOCKS': '0', 'GIT_INDEX_FILE': self.indexes[repo['environmentKey']]}
                    require(self.command(['git', 'add', '-N', '--', *repo['intentToAdd']], 'private-index-add-' + str(self.phase),
                            relative(self.root, repo['path']), add_env) == 0, 'alternate-index intent-to-add failed')
        require(self.command(command['argv'], 'rollback-regen' if rollback else 'regen',
                             relative(self.root, command['cwd']), env, rollback) == 0, 'regeneration failed')

    def scenario(self, scenario, number, recovery=False):
        self.file_state('before' if recovery else 'after')
        script = self.root / 'arc/browser/webextras_scenario_current_run.py'
        spec = importlib.util.spec_from_file_location('webextras_checked_runner', script)
        module = importlib.util.module_from_spec(spec); spec.loader.exec_module(module)
        expected = {pin_path(self.root, n).resolve(): h for n, h in self.config['pins'].items()}
        expected.update({relative(self.root, n).resolve(): image['before' if recovery else 'after']
                         for n, image in self.images.items()})
        expected[script.resolve()] = self.config['observationTool']['after']
        for path in module.source_inputs(scenario):
            require(path.resolve() in expected and sha(path) == expected[path.resolve()],
                    'undeclared/drifted canonical runner dependency before build: ' + str(path))
        dest = self.root / f'results/webextras/{scenario}-current-run{number}'
        require(not dest.exists(), 'runtime output already exists')
        argv = [sys.executable, str(script), '--locked', '--scenario', scenario, '--run-number', str(number)]
        code = self.command(argv, ('recovery-' if recovery else '') + scenario, rollback=recovery)
        self.stopped()
        self.file_state('before' if recovery else 'after')
        require(all(directory_id(Path(p)) == value for p, value in self.roots.items()),
                'profile/cache directory identity not restored; parent recovery required')
        result = scenario_gate(self.root, dest, scenario,
                               self.root / f'results/webextras/{scenario}-current-run1', code)
        result['resourceProof'] = self.check_resources(dest, scenario, number, recovery)
        write(self.output / (('recovery-' if recovery else '') + scenario + '.gate.json'), result)
        return result

    def check_resources(self, scenario_output, scenario, number, recovery=False):
        check = self.config['resourceChecks']['recovery' if recovery else 'candidate']
        directory = self.root / f'logs/wp/webextras.initial.resources.{scenario}.current-run{number}'
        require(not directory.exists(), 'resource checker output already exists')
        argv = [str(directory) if a == '{resource_output}' else a for a in check['argv']]
        require(self.command(argv, ('recovery-' if recovery else '') + 'resources-' + scenario,
                             relative(self.root, check['cwd']), rollback=recovery) == 0, 'resource checker failed')
        receipt = json.loads((directory / 'receipt.json').read_text())
        before = json.loads((scenario_output / 'binary-identity.json').read_text())['resources']
        after = json.loads((scenario_output / 'binary-identity-after-run.json').read_text())['resources']
        require(receipt.get('passed') is True and receipt.get('expectHostGate') == check['expectHostGate']
                and receipt.get('builtPackagedPakExactMatch') is True
                and receipt.get('builtPakSHA256') == receipt.get('runtimePakSHA256') == before['sha256'] == after['sha256']
                and sha(before['path']) == before['sha256'], 'resource proof not tied to runtime package')
        resources = receipt.get('resources', [])
        require(len(resources) == 3 and len({x.get('textualId') for x in resources}) == 3 and all(
            all(x.get(k) is True for k in ('generatedExactMatch', 'grdInputExactMatch', 'intermediateExactMatch'))
            for x in resources), 'incomplete decoded/generated resource proof')
        return {'receipt': str(directory / 'receipt.json'), 'sha256': sha(directory / 'receipt.json'),
                'runtimePakSHA256': before['sha256'], 'expectHostGate': check['expectHostGate']}

    def rollback(self):
        self.stopped(); self.lock_check(); self.pin_check()
        # Unknown bytes, including raw-only index changes, never authorize overwrite.
        for repo in self.config['gitRepos']:
            require(sha(relative(self.root, repo['index'])) == repo['sha256'], 'index drift before rollback')
        for name, image in self.images.items():
            require(sha(relative(self.root, name)) in (image['before'], image['after']), 'foreign target edit at rollback')
        for name, image in self.config['patchOutputs'].items():
            require(sha(relative(self.root, name)) in (image['before'], image['after']), 'foreign patch output at rollback')
        require({str(p.relative_to(self.root)) for p in (self.root / 'patches').glob('*.patch')} <=
                set(self.config['patchOutputs']), 'unjournalled patch output at rollback')
        # Confirm every started scenario restored its original profile before source recovery.
        for child in self.output.glob('*.child.json'):
            argv = json.loads(child.read_text())['argv']
            if str(self.root / 'arc/browser/webextras_scenario_current_run.py') in argv:
                name = argv[argv.index('--scenario') + 1]; number = argv[argv.index('--run-number') + 1]
                summary = self.root / f'results/webextras/{name}-current-run{number}/runner-summary.json'
                require(summary.is_file() and json.loads(summary.read_text()).get('profileRestored') is True,
                        'profile restoration unconfirmed; parent recovery required')
        for name, image in self.images.items():
            path = relative(self.root, name)
            if sha(path) == image['before']:
                continue
            if image['before'] is None:
                path.unlink()
            else:
                mode = json.loads((self.output / 'modes' / (name.replace('/', '_') + '.json')).read_text())
                atomic_bytes(path, (self.output / 'before' / name).read_bytes(), mode)
        # Remove candidate-only intent-to-add from PRIVATE indexes before the
        # restored MANIFEST is regenerated. The real indexes remain untouched.
        for repo in self.config['gitRepos']:
            atomic_bytes(Path(self.indexes[repo['environmentKey']]), relative(self.root, repo['index']).read_bytes())
        self.regenerate(rollback=True)
        # Regen cannot remove an originally absent, now unreferenced own patch.
        for name, image in self.config['patchOutputs'].items():
            path = relative(self.root, name)
            if image['before'] is None and path.exists():
                require(sha(path) == image['after'], 'unexpected orphan patch bytes')
                path.unlink()
        self.file_state('before')
        self.scenario('launch-slice', self.config['runNumbers']['recovery-launch-slice'], recovery=True)
        self.file_state('before'); self.stopped()

    def execute(self):
        try:
            self.prepare(); self.adopt_observation_tool(); self.publish(); self.regenerate()
            self.file_state('after')
            results = [self.scenario(s, self.config['runNumbers'][s]) for s in SCENARIOS]
            self.file_state('after'); self.stopped(); self.cancelled()
            write(self.output / 'completion.json', {'status': 'APPLIED', 'scenarios': results,
                  'boundaryParity': 'NOT ASSERTED', 'normalOSExitStatusCaptured': False, 'persistentObservationToolRetained': self.tool_adopted})
            return 0
        except BaseException as error:
            try:
                write(self.output / 'failure.json', {'error': repr(error), 'sourcePublished': self.mutated})
            except OSError:
                pass
            if not self.mutated:
                self.stopped()
                write(self.output / 'completion.json', {'status': 'REFUSED_BEFORE_PUBLICATION', 'persistentObservationToolRetained': self.tool_adopted})
                return 1
            try:
                self.rollback()
                write(self.output / 'completion.json', {'status': 'ROLLED_BACK', 'baselineLaunch9of9': True,
                      'normalOSExitStatusCaptured': False, 'persistentObservationToolRetained': self.tool_adopted})
                return 1
            except BaseException as recovery_error:
                self.hold(repr(recovery_error))


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('mode', choices=('template', 'validate', 'run', '_protected', '_guardian', '_worker'))
    parser.add_argument('--config', type=Path); parser.add_argument('--config-sha')
    parser.add_argument('--out', type=Path)
    args = parser.parse_args()
    if args.mode == 'template':
        print(json.dumps(template(), indent=2)); return 0
    require(args.config is not None and args.config_sha and sha(args.config) == args.config_sha,
            'exact parent-sealed config SHA required')
    config = validate(json.loads(args.config.read_text()), ROOT)
    if args.mode == 'validate':
        return 0
    require(args.out is not None, 'fresh output required')
    output = args.out.resolve()
    if args.mode == 'run':
        output.mkdir(parents=True, exist_ok=False)
        write(output / 'config.json', config)
        write(output / 'launch.json', {'nonce': uuid.uuid4().hex, 'configSHA256': args.config_sha})
        for signum in SIGNALS:
            signal.signal(signum, cancel_handler(output / 'cancel.request'))
        argv = [sys.executable, str(Path(__file__).resolve()), '_protected', '--config', str(args.config.resolve()),
                '--config-sha', args.config_sha, '--out', str(output)]
        child = subprocess.Popen(argv, start_new_session=True)
        return child.wait()
    require((output / 'launch.json').is_file() and
            json.loads((output / 'launch.json').read_text())['configSHA256'] == args.config_sha,
            'missing matching parent launch receipt')
    if args.mode == '_protected':
        for signum in SIGNALS:
            signal.signal(signum, signal.SIG_IGN)
        write(output / 'protected.json', {'pid': os.getpid()})
        os.execv('/bin/bash', ['bash', str(ROOT / 'arc/tools/locked.sh'), sys.executable,
                 str(Path(__file__).resolve()), '_guardian', '--config', str(args.config.resolve()),
                 '--config-sha', args.config_sha, '--out', str(output)])
    for signum in SIGNALS:
        signal.signal(signum, cancel_handler(output / 'cancel.request'))
    if args.mode == '_guardian':
        transaction = Transaction(config, output)
        try:
            transaction.lock_check()
            argv = [sys.executable, str(Path(__file__).resolve()), '_worker', '--config', str(args.config.resolve()),
                    '--config-sha', args.config_sha, '--out', str(output)]
            child = subprocess.Popen(argv, stdin=subprocess.DEVNULL, start_new_session=True)
            try:
                write(output / 'worker.json', {'pid': child.pid, 'nonce': json.loads((output / 'launch.json').read_text())['nonce']})
            finally:
                child.wait()
            # An unexpectedly dead worker cannot release the protected lock.
            completion = json.loads((output / 'completion.json').read_text())
            state = completion.get('status')
            require(state in ('APPLIED', 'ROLLED_BACK', 'REFUSED_BEFORE_PUBLICATION'), 'missing safe completion')
            require(child.returncode == (0 if state == 'APPLIED' else 1), 'worker/completion mismatch')
            transaction.backend.groups = {json.loads(p.read_text())['pgid'] for p in output.glob('*.child.json')}
            transaction.backend.groups.add(child.pid)
            transaction.file_state('after' if state == 'APPLIED' else 'before')
            transaction.stopped()
            roots = output / 'directory-identities.json'
            if roots.exists():
                require(all(directory_id(Path(p)) == value for p, value in json.loads(roots.read_text()).items()),
                        'guardian profile/cache identity mismatch')
            if state == 'APPLIED':
                transaction.cancelled()
                require([s.get('scenario') for s in completion['scenarios']] == list(SCENARIOS)
                        and all(s.get('noEarlierStepRegression') is True for s in completion['scenarios']),
                        'incomplete runtime release proof')
            if state == 'ROLLED_BACK':
                require(completion.get('baselineLaunch9of9') is True, 'missing baseline package recovery')
            write(output / 'release-safe.json', {'workerExit': child.returncode, 'state': state,
                  'normalOSExitStatusCaptured': False, 'nonce': json.loads((output / 'launch.json').read_text())['nonce']})
            return child.returncode
        except BaseException as error:
            transaction.hold('guardian cannot certify release: ' + repr(error))
    return Transaction(config, output).execute()


if __name__ == '__main__':
    try:
        sys.exit(main())
    except (RuntimeError, OSError, ValueError, KeyError) as error:
        print(str(error), file=sys.stderr)
        sys.exit(2)
