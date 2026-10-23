#!/usr/bin/env python3
"""Held initial integration executor. No sealed configuration is supplied.

The parent seals exact images/commands after review. This program never signals
apps, restores a Git index, accepts global boundary parity, or releases a HOLD.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import signal
import stat
import struct
import subprocess
import sys
import tempfile
import time
import types
import uuid

if not __debug__ or sys.flags.optimize:
    raise RuntimeError('optimized Python is unsupported; index assertions must remain active')

ROOT = Path(__file__).resolve().parents[2]
NODE = Path('/opt/homebrew/bin/node').resolve()
PYTHON = Path(sys.executable).resolve()
SCENARIOS = ('launch-slice', 'devtools', 'media', 'pdf', 'media-pip')
SIGNALS = (signal.SIGINT, signal.SIGTERM, signal.SIGHUP)
LAUNCH = ('S1 launch', 'S2 open A', 'S3 navigate', 'S4 store', 'S5 close + new tab',
          'S6 quit', 'S7 relaunch', 'S7 readback', 'S8 quit')
SECTIONS = ('missing', 'extra', 'return_mismatch', 'arg_mismatch', 'count_mismatch')
RESOURCE_IDS = {'DEVTOOLS_COMPATIBILITY_JS', 'PANELS_ELEMENTS_ELEMENTS_JS', 'UI_LEGACY_LEGACY_JS'}
RESOURCE_CHECKER = 'arc/browser/webextras_frontend_resource_checked.py'
RESOURCE_INPUTS = (RESOURCE_CHECKER, 'chromium/src/ui/base/resource/data_pack.cc',
                   'chromium/src/ui/base/resource/data_pack.h', 'chromium/src/ui/base/resource/resource_handle.h',
                   'chromium/src/tools/grit/grit/format/data_pack.py', 'chromium/src/tools/grit/grit/constants.py')
RESOURCE_POLICY = {
    RESOURCE_CHECKER: 'aee01b900c0bfaa527995f893167a525a1a9984f2f5e5e8f76370dc98ce99b74',
    'arc/browser/webextras_frontend_resource_checked_test.py': '0c887aa79737c142c78a5c1252ef2e44cdbc14f059bf7c78328ba24a858a455c',
    'arc/browser/webextras_frontend_resource_checked_notes.md': 'e8716c369c658c544c2c29b1adf5e76848e04198d3ae4f97160c8c3834b9a4e3',
    'chromium/src/ui/base/resource/data_pack.cc': '11dcac3de60b8f007c6cc1b1f4be4ca420e6e347bb825ebdcacb9c08405e1c57',
    'chromium/src/ui/base/resource/data_pack.h': '97e9285a0b748b673fd586c88d1b6a1c8ad8e5294aa27e3ab9eb58925d6bd8ef',
    'chromium/src/ui/base/resource/resource_handle.h': '3e18062fb8452ac6a0177e3e66ec434945caa9d0d2315a2df21bb8ac673b7e20',
    'chromium/src/tools/grit/grit/format/data_pack.py': '323cc5492ec6037a65408f0ea0413bd9d098a06b7c0216067e8abf7b64f09d4e',
    'chromium/src/tools/grit/grit/constants.py': '143062b300d5a64b0ba9f8bebc1b3e88ef880fe1e9298edd3b80f786a451d443',
    'logs/wp/webextras-frontend-resource-native-sanity/freeze.json': '18c51bd6d8832404cb35c34662ddc4baea349951ebd26cf5a95de8e9c082e960',
    'logs/wp/webextras-frontend-resource-sanity-review-worker/review.json': '2d22fc23ea5ed6027dde80f792c6c1be2b8453f472ccf428f9b82807d0f8fef5',
}
LOGICAL_INDEX_FUNCTION_SHA256 = '14cf9aa8377da51031990cfb99868a7386c88aa2bbab5196f8bfb0c1a4b90024'
LOGICAL_INDEX_POLICY = {'logs/wp/app-run6-vnav-integration-driver-v4/transaction.py': '444e2c0738c8d5c8e425cc2eb67c4cf2bdabe6ee7695009ec9935a7c921590cc',
 'logs/wp/app-run6-vnav-integration-driver-v4/parent-validation-classification.json': 'ec464fe306831bc2146b6070beffe6b33bbfe11e922b8eadff1c7742fe715417',
 'logs/wp/app-run6-vnav-integration-driver-v4-runtime/transaction.review.json': '27fa4631aa530882c02f995bae28db05f08a318848f305eb46cd6670a8ccf354',
 'logs/wp/app-run6-vnav-integration-driver-v4-runtime/freeze.json': 'ddb03b308063ac63a267bd48af2fd3200ebc8afaef6ecb5172f2566865d6fb34'}
CANCELLED = False
REGEN_TOOLS = {'arc/tools/regen-patches.sh': 0o755, 'arc/tools/regen-chromium-patches.py': 0o644}


def source_module(path, expected):
    """Load the exact sealed source image without consulting Python bytecode."""
    require(sha(path) == expected, 'module source differs from sealed image')
    source = path.read_bytes()
    require(hashlib.sha256(source).hexdigest() == expected, 'module changed while reading')
    module = types.ModuleType('webextras_checked_source')
    module.__file__ = str(path)
    exec(compile(source, str(path), 'exec'), module.__dict__)
    require(sha(path) == expected, 'module changed while executing source')
    return module


# Exact attributed copy: App444e pure function, lines282–351 (no driver import).
# Source/function/review pins above; bounded72-case plus focused split rejection.
def logical_index_seal(data):
    """Pure SHA-1 DIRC v2/v3/v4 logical entries; cache frames are opaque.

    Format: local Git2.50.1 gitformat-index.5. No helper imports, Git or I/O.
    Preserve ALL paths/modes/OIDs/stages/valid/ITA/skip bits. Ignore only stat,
    encoding/version and known optional cache metadata, never staging content.
    """
    assert isinstance(data, bytes) and 32 <= len(data) <= 1024 * 1024 * 1024, 'Unsupported index length'
    end = len(data) - 20
    assert hashlib.sha1(data[:end]).digest() == data[end:], 'Invalid SHA-1 index checksum'
    signature, version, count = struct.unpack_from('>4sII', data)
    assert signature == b'DIRC' and version in (2, 3, 4), 'Unsupported index format'
    assert count <= (end - 12) // 64, 'Index count exceeds frame'
    offset, previous_name, previous_key = 12, b'', None
    logical = hashlib.sha256()
    for _ in range(count):
        start = offset
        assert offset + 62 <= end, 'Truncated index entry'
        mode = struct.unpack_from('>I', data, offset + 24)[0]
        oid = data[offset + 40:offset + 60]
        flags = struct.unpack_from('>H', data, offset + 60)[0]
        assert mode in (0o100644, 0o100755, 0o120000, 0o160000), 'Unknown/sparse index mode'
        stage = (flags >> 12) & 3
        extended = 0
        offset += 62
        if flags & 0x4000:
            assert version >= 3 and offset + 2 <= end, 'Invalid extended index flags'
            extended = struct.unpack_from('>H', data, offset)[0]
            assert extended & ~0x6000 == 0, 'Unknown extended index flags'
            offset += 2
        remove = 0
        if version == 4:
            # OFS_DELTA variable-width number; remove bytes from prior path.
            assert offset < end, 'Missing v4 path prefix'
            byte = data[offset]; offset += 1
            remove = byte & 0x7f
            width = 1
            while byte & 0x80:
                assert offset < end and width < 10, 'Invalid v4 path prefix'
                byte = data[offset]; offset += 1; width += 1
                remove = ((remove + 1) << 7) | (byte & 0x7f)
            assert remove <= len(previous_name), 'v4 path prefix underflow'
        nul = data.find(b'\0', offset, end)
        assert nul >= 0, 'Unterminated index pathname'
        suffix = data[offset:nul]
        name = previous_name[:len(previous_name) - remove] + suffix if version == 4 else suffix
        assert name and not name.startswith(b'/') and not any(part in (b'', b'.', b'..', b'.git') for part in name.split(b'/')), 'Invalid index pathname'
        length = flags & 0x0fff
        assert length == min(len(name), 0x0fff), 'Index pathname length mismatch'
        key = (name, stage)
        assert previous_key is None or previous_key < key, 'Unsorted/duplicate index entry'
        previous_name, previous_key = name, key
        if version == 4:
            offset = nul + 1
        else:
            offset = start + ((nul + 1 - start + 7) // 8) * 8
            assert offset <= end and not any(data[nul:offset]), 'Invalid index entry padding'
        logical.update(struct.pack('>I', len(name)))
        logical.update(name); logical.update(struct.pack('>I', mode)); logical.update(oid)
        logical.update(bytes((stage, bool(flags & 0x8000), bool(extended & 0x2000), bool(extended & 0x4000))))
    seen = set()
    while offset < end:
        assert offset + 8 <= end, 'Truncated index extension'
        kind, size = struct.unpack_from('>4sI', data, offset)
        assert kind in (b'TREE', b'UNTR', b'EOIE', b'IEOT') and kind not in seen, 'Unsupported/duplicate index extension (split/sparse rejected)'
        seen.add(kind)
        assert size <= end - offset - 8, 'Index extension exceeds frame'
        offset += 8 + size
    assert offset == end, 'Invalid index frame end'
    return (count, logical.hexdigest())

def image_state(path):
    return sha(path), stat.S_IMODE(path.stat().st_mode) if path.exists() else None


def cancel_handler(path):
    def request(*_):
        global CANCELLED
        CANCELLED = True
        try:
            path.touch(exist_ok=True)
        except OSError:
            pass
    return request


def git_environment():
    env = {k: v for k, v in os.environ.items() if not k.startswith('GIT_')}
    env.update(GIT_OPTIONAL_LOCKS='0', GIT_CONFIG_GLOBAL='/dev/null', GIT_CONFIG_NOSYSTEM='1')
    return env


def patch_changed_paths(path, cwd, expected):
    """Use Git's metadata reader in disposable space, never an actual checkout."""
    data = path.read_bytes()
    require(hashlib.sha256(data).hexdigest() == expected, 'engine patch differs from sealed image')
    with tempfile.TemporaryDirectory(prefix='webextras-patch-paths-') as private:
        result = subprocess.run(['git', 'apply', '--numstat', '-z', '-'], input=data,
                                cwd=private, env=git_environment(), capture_output=True)
        summary = subprocess.run(['git', 'apply', '--summary', '-'], input=data,
                                 cwd=private, env=git_environment(), capture_output=True)
    require(result.returncode == 0 and result.stdout, 'unreadable/empty engine patch metadata')
    # apply --numstat reports only the destination for a rename. Refuse these
    # operations rather than accidentally admitting a renamed baseline source.
    require(summary.returncode == 0 and not any(line.lstrip().startswith((b'rename ', b'copy '))
            for line in summary.stdout.splitlines()), 'engine patch rename/copy is unsupported')
    paths = set()
    for row in result.stdout.split(b'\0')[:-1]:
        fields = row.decode().split('\t', 2)
        require(len(fields) == 3 and fields[2], 'unsupported engine patch rename metadata')
        name = Path(fields[2])
        require(not name.is_absolute() and '..' not in name.parts, 'unsafe engine patch path')
        paths.add(str(Path(cwd) / name))
    return paths


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


def engine_path(root, name):
    """Mutable engine paths may not traverse aliases to immutable inputs."""
    result = relative(root, name)
    current = root
    for part in Path(name).parts:
        current = current / part
        require(not current.is_symlink(), 'mutable engine path traverses symlink: ' + name)
    require(result.resolve() not in {(root / tool).resolve() for tool in REGEN_TOOLS},
            'engine path aliases shared regeneration baseline: ' + name)
    return result


def pin_path(root, name):
    path = Path(name)
    if not path.is_absolute():
        return relative(root, name)
    require(path.resolve().is_relative_to(root.resolve()) or
            path.resolve().is_relative_to((root.parent / 'arc').resolve()) or
            path in (NODE, PYTHON), 'external pin outside checkout/sibling Arc/fixed interpreters')
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
            'pins': {**RESOURCE_POLICY, **LOGICAL_INDEX_POLICY}, 'inventory': {}, 'patches': [], 'targets': {},
            'manifest': {}, 'patchOutputs': {}, 'gitRepos': [],
            'regeneration': {'argv': ['./arc/tools/regen-patches.sh'], 'cwd': '.', 'env': {},
                             'toolModes': dict(REGEN_TOOLS)},
            'observationTool': {}, 'resourceChecks': {role: {'argv': [str(PYTHON), '-B', RESOURCE_CHECKER,
                '--expect-host-gate', mode, '--output', '{resource_output}'], 'cwd': '.', 'expectHostGate': mode}
                for role, mode in [('candidate', 'present'), ('recovery', 'absent')]},
            'baselineFiles': {}, 'runNumbers': {},
            'note': 'No execution until parent seals reviewed exact images, tools and ACK evidence.'}


def validate(config, root):
    require(__debug__ and not sys.flags.optimize, 'optimized Python is unsupported')
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
    require(not set(REGEN_TOOLS) & set(images), 'shared regeneration baseline cannot be an engine target')
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
        engine_path(root, name)
        require(set(image) == {'before', 'after', 'blob', 'beforeMode', 'afterMode'}, 'invalid image: ' + name)
        require(image['before'] is None or digest.fullmatch(image['before']), 'invalid preimage')
        require(isinstance(image['after'], str) and digest.fullmatch(image['after']), 'invalid postimage')
        require(image['before'] != image['after'], 'unchanged publication target')
        require(type(image['afterMode']) is int and 0 <= image['afterMode'] <= 0o777 and
                ((image['before'] is None and image['beforeMode'] is None) or
                 (type(image['beforeMode']) is int and image['beforeMode'] == image['afterMode'])),
                'existing target mode must be preserved; new target mode must be sealed')
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
        engine_path(root, patch['cwd'])
        require(config['pins'].get(patch['path']) == patch['sha256'], 'patch not pinned')
        changed_paths = patch_changed_paths(relative(root, patch['path']), patch['cwd'], patch['sha256'])
        require(not set(REGEN_TOOLS) & changed_paths,
                'engine patch changes shared regeneration baseline')
        for name in changed_paths:
            engine_path(root, name)
    require(config.get('inventory'), 'missing production inventory')
    require(set(config['inventory']) == {'arc/api', 'arc/browser', 'arc/wp'}, 'incomplete Arc production inventory')
    for folder, names in config['inventory'].items():
        require(isinstance(names, list) and len(names) == len(set(names)), 'invalid production inventory')
        require(all(n.startswith(folder + '/') and (n in images or n in config['pins']) for n in names),
                'production content not pinned')
    require(config.get('gitRepos'), 'missing index protection')
    for repo in config['gitRepos']:
        require(set(repo) == {'path', 'index', 'sha256', 'head', 'logicalSeal', 'snapshot'},
                'invalid Git repository contract')
        relative(root, repo['path']); relative(root, repo['index'])
        require(isinstance(repo['sha256'], str) and digest.fullmatch(repo['sha256']) and
                isinstance(repo['head'], str) and repo['head'], 'invalid guard-only Git identity')
        seal = repo['logicalSeal']
        require(isinstance(seal, list) and len(seal) == 2 and type(seal[0]) is int and seal[0] >= 0 and
                isinstance(seal[1], str) and digest.fullmatch(seal[1]), 'invalid original logical index seal')
        snapshot = relative(root, repo['snapshot'])
        require(snapshot.resolve() != relative(root, repo['index']).resolve(), 'original snapshot must be separate from live index')
        raw_snapshot = snapshot.read_bytes()
        require(config['pins'].get(repo['snapshot']) == repo['sha256'] == hashlib.sha256(raw_snapshot).hexdigest() == sha(snapshot),
                'original raw index snapshot is not pinned')
        require(list(logical_index_seal(raw_snapshot)) == seal, 'original snapshot/seal disagree')
    require(len({r['path'] for r in config['gitRepos']}) == len(config['gitRepos']) and
            sum(r['path'] == 'chromium/src' for r in config['gitRepos']) == 1, 'unique outer Chromium guard required')
    require(all(config['pins'].get(name) == value for name, value in LOGICAL_INDEX_POLICY.items()),
            'attributed logical index source/review policy is not pinned')
    regeneration = config.get('regeneration', {})
    require(set(regeneration) == {'argv', 'cwd', 'env', 'toolModes'} and
            regeneration['argv'] == ['./arc/tools/regen-patches.sh'] and regeneration['cwd'] == '.' and
            regeneration['env'] == {}, 'regeneration must invoke fixed baseline wrapper from root without overrides')
    require(regeneration['toolModes'] == REGEN_TOOLS and
            all(type(v) is int for v in regeneration['toolModes'].values()) and
            all(name in config['pins'] for name in REGEN_TOOLS), 'shared regeneration baseline hashes/modes not sealed')
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
    checker = RESOURCE_CHECKER
    require(all(config['pins'].get(n) == digest for n, digest in RESOURCE_POLICY.items())
            and all(n in config['pins'] for n in RESOURCE_INPUTS), 'reviewed native resource policy not exactly pinned')
    require(str(NODE) in config['pins'], 'fixed resource decoder Node binary not pinned')
    require(str(PYTHON) in config['pins'], 'resource checker Python interpreter not pinned')
    for role, check in checks.items():
        require(set(check) == {'argv', 'cwd', 'expectHostGate'} and check['expectHostGate'] in ('present', 'absent'),
                'invalid role-specific resource check')
        require(check['expectHostGate'] == ('present' if role == 'candidate' else 'absent'),
                'initial integration requires candidate-present/recovery-absent resources')
        argv = check['argv']; relative(root, check['cwd'])
        require(check['cwd'] == '.' and argv == [str(PYTHON), '-B', checker, '--expect-host-gate',
                check['expectHostGate'], '--output', '{resource_output}'], 'resource CLI/mode mismatch')
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


def file_identity(path):
    """Content plus replacement/chmod identity, including across the hash read."""
    require(not path.is_symlink() and path.is_file(), 'missing regular identity file')
    fields = ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_mode')
    before = path.stat(); digest = sha(path); after = path.stat()
    require(all(getattr(before, key) == getattr(after, key) for key in fields), 'identity changed while hashing')
    return {'sha256': digest, **{key: getattr(after, key) for key in fields}}


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
        self.roots = {}
        self._lease_active = False
        self._lease_failed = False
        self.original_indexes = json.loads(json.dumps(config['gitRepos']))
        receipt = self.output / 'observation-tool.json'
        self.tool_adopted = (receipt.is_file() and json.loads(receipt.read_text()).get('status') == 'PERSISTENT_OBSERVATION_TOOL_ADOPTED')
        require(json.loads((self.output / 'config.json').read_text()) == config, 'config evidence mismatch')
        self.config_digest = sha(self.output / 'config.json')

    def cancelled(self):
        require(not CANCELLED and not self.cancel.exists(), 'cancellation requested; stop before next candidate phase')

    def regeneration_clear(self):
        require(not self._lease_failed and not os.path.lexists(self.output / 'regeneration-failure-latch.json'),
                'failed regeneration lease is latched; parent reconciliation required')
        if not self._lease_active:
            for path in self.output.glob('regeneration-lease-*.json'):
                require(not path.is_symlink() and path.is_file(), 'invalid regeneration ledger')
                receipt = json.loads(path.read_text())
                require(isinstance(receipt, dict) and set(receipt) == {'status', 'configSHA256', 'originals', 'before', 'after', 'exit', 'error'} and
                        receipt.get('status') == 'CHECKED' and
                        receipt.get('configSHA256') == self.config_digest and
                        receipt.get('originals') == self.original_indexes and
                        receipt.get('before') == receipt.get('after') and receipt.get('before'),
                        'incomplete/foreign regeneration ledger; no new lease or release permitted')
                before = receipt['before']
                require(isinstance(before, dict) and set(before) == {r['path'] for r in self.original_indexes},
                        'regeneration ledger repository coverage mismatch')
                require((receipt.get('exit') is None or type(receipt.get('exit')) is int) and
                        (receipt.get('error') is None or isinstance(receipt.get('error'), str)), 'invalid regeneration outcome')
                require((receipt.get('exit') == 0 and receipt.get('error') is None) or
                        (isinstance(receipt.get('error'), str) and receipt['error']), 'missing regeneration failure outcome')
                fields = {'sha256', 'st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns', 'st_mode'}
                for repo in self.original_indexes:
                    row = before[repo['path']]
                    require(isinstance(row, dict) and set(row) == {'identity', 'head', 'logicalSeal'} and
                            row['head'] == repo['head'] and row['logicalSeal'] == repo['logicalSeal'], 'regeneration original anchor mismatch')
                    value = row['identity']
                    require(isinstance(value, dict) and set(value) == fields and
                            isinstance(value['sha256'], str) and re.fullmatch(r'[0-9a-f]{64}', value['sha256']) and
                            all(type(value[key]) is int and value[key] >= 0 for key in fields - {'sha256'}) and
                            stat.S_ISREG(value['st_mode']), 'invalid raw/physical regeneration lease')

    def latch_regeneration_failure(self, error, lease_path):
        self._lease_failed = True
        path = self.output / 'regeneration-failure-latch.json'
        if not os.path.lexists(path):
            atomic_bytes(path, (json.dumps({'status': 'IRREVERSIBLE_REGENERATION_LEASE_FAILURE',
                'configSHA256': self.config_digest, 'lease': lease_path.name, 'error': repr(error),
                'originals': self.original_indexes, 'noAutomaticReleaseOrNewLease': True}, indent=2) + '\n').encode(), 0o600)

    def index_boundary(self):
        """Fresh raw leases may vary; the configured original logical seal cannot."""
        self.regeneration_clear(); self.stopped(); self.lock_check()
        require(sha(self.output / 'config.json') == self.config_digest and
                self.config['gitRepos'] == self.original_indexes, 'original index contract cannot be rebound')
        result = {}
        for repo in self.original_indexes:
            index = relative(self.root, repo['index'])
            try:
                require(not any(index.parent.glob('sharedindex.*')), 'shared index layouts are unsupported')
                identity = file_identity(index); raw = index.read_bytes()
                require(file_identity(index) == identity and hashlib.sha256(raw).hexdigest() == identity['sha256'],
                        'index changed while copying boundary image')
                # Parse before any index-reading Git. No ls-files probe can refresh it.
                seal = list(logical_index_seal(raw))
                require(seal == repo['logicalSeal'], 'original full logical index changed')
                git_env = git_environment()
                actual = subprocess.check_output(['git', 'rev-parse', '--git-path', 'index'],
                    cwd=relative(self.root, repo['path']), env=git_env, text=True).strip()
                actual = Path(actual) if Path(actual).is_absolute() else relative(self.root, repo['path']) / actual
                require(actual.resolve() == index.resolve(), 'configured index is not actual repository index')
                head = subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=relative(self.root, repo['path']),
                                               env=git_env, text=True).strip()
                require(head == repo['head'], 'original Git HEAD drift')
                require(file_identity(index) == identity, 'index changed during boundary checks')
                result[repo['path']] = {'identity': identity, 'head': head, 'logicalSeal': seal}
            except BaseException as error:
                diagnostic = repr(error)[:4096]
                write(self.output / 'index-drift.json', {'path': repo['index'], 'originalRawSHA256': repo['sha256'],
                      'originalLogicalSeal': repo['logicalSeal'], 'error': diagnostic, 'policy': 'HOLD; never restore or rebaseline index'})
                (self.output / 'index-semantic-diagnostic.txt').write_text(diagnostic + '\nNo index-reading Git probe was performed.\n')
                raise RuntimeError('index drift/unsupported original logical image: ' + diagnostic) from error
        return result

    def capture_preworker_release_state(self, config_path, config_sha):
        """Guardian-only evidence before the worker can attempt any phase."""
        self.regeneration_clear(); self.lock_check(); self.stopped()
        runner = self.root / 'arc/browser/webextras_scenario_current_run.py'
        own = self.root / 'arc/browser/webextras_initial_integration.py'
        require(sha(own) == self.config['pins']['arc/browser/webextras_initial_integration.py'], 'guardian source is not sealed')
        require(sha(config_path) == config_sha, 'parent config drift before worker')
        evidence = {'configPath': str(config_path), 'configSHA256': config_sha,
                    'configEvidenceSHA256': self.config_digest, 'lock': list(self.lock),
                    'launch': file_identity(self.output / 'launch.json'),
                    'protected': file_identity(self.output / 'protected.json'),
                    'own': file_identity(own), 'runner': file_identity(runner),
                    'roots': {str(p): directory_id(p) for p in (
                        Path.home() / 'Library/Application Support/ArcModAgent-ours156',
                        Path.home() / 'Library/Caches/ArcModAgent-ours156')}}
        require(evidence['own']['sha256'] == self.config['pins']['arc/browser/webextras_initial_integration.py'],
                'guardian source changed during preworker capture')
        require(evidence['runner']['sha256'] == self.config['observationTool']['before'], 'preworker runner is not before image')
        write(self.output / 'guardian-preworker.json', evidence)
        self.preworker = (evidence, file_identity(self.output / 'guardian-preworker.json'))

    def certify_refused_without_writes(self, completion, child):
        """An early refusal releases no claim about the now-foreign baseline."""
        self.regeneration_clear()
        require(type(child.returncode) is int and child.returncode == 1 and
                completion.get('status') == 'REFUSED_BEFORE_PUBLICATION' and
                completion.get('persistentObservationToolRetained') is False, 'not an exact early refusal')
        # Attempted/reverted observation phases keep the ordinary strict path.
        allowed = {'config.json', 'launch.json', 'protected.json', 'worker.json', 'guardian-preworker.json',
                   'failure.json', 'completion.json', 'index-drift.json', 'index-semantic-diagnostic.txt'}
        require(all(p.name in allowed and p.is_file() and not p.is_symlink() for p in self.output.iterdir()),
                'early refusal contains attempted/unknown phase evidence')
        evidence, identity = self.preworker
        require(file_identity(self.output / 'guardian-preworker.json') == identity and
                json.loads((self.output / 'guardian-preworker.json').read_text()) == evidence, 'preworker evidence drift')
        require(sha(Path(evidence['configPath'])) == evidence['configSHA256'] and
                sha(self.output / 'config.json') == self.config_digest == evidence['configEvidenceSHA256'], 'refusal config drift')
        for key in ('launch', 'protected'):
            require(file_identity(self.output / (key + '.json')) == evidence[key], 'refusal launch identity drift')
        launch = json.loads((self.output / 'launch.json').read_text())
        require(launch.get('configSHA256') == evidence['configSHA256'] and
                isinstance(launch.get('nonce'), str) and launch['nonce'], 'refusal launch mismatch')
        require(json.loads((self.output / 'worker.json').read_text()) == {'pid': child.pid, 'nonce': launch['nonce']},
                'refusal worker identity mismatch')
        failure = json.loads((self.output / 'failure.json').read_text())
        require(isinstance(failure, dict) and failure.get('sourcePublished') is False, 'refusal publication is uncertain')
        for name, key in [('webextras_initial_integration.py', 'own'), ('webextras_scenario_current_run.py', 'runner')]:
            require(file_identity(self.root / 'arc/browser' / name) == evidence[key], 'refusal trusted tool identity drift')
        require(all(directory_id(Path(p)) == value for p, value in evidence['roots'].items()), 'refusal profile/cache identity drift')
        self.lock_check(); require(self.lock == evidence['lock'], 'refusal lock drift'); self.stopped()
        return {'workerExit': 1, 'state': 'REFUSED_WITHOUT_WRITES_NOT_CURRENT_BASELINE',
                'workerState': completion['status'], 'persistentObservationToolRetained': False,
                'sourcePublished': False, 'currentBaselineAsserted': False, 'boundaryParity': 'NOT ASSERTED',
                'normalOSExitStatusCaptured': False, 'nonce': launch['nonce'],
                'preworkerEvidenceSHA256': identity['sha256']}

    def refresh_observation_tool_after_worker(self, completion):
        """Validate the worker's durable handoff without replacing this guardian."""
        self.regeneration_clear()
        require(sha(self.output / 'config.json') == self.config_digest, 'sealed config evidence drift')
        require(completion.get('status') in ('APPLIED', 'ROLLED_BACK', 'REFUSED_BEFORE_PUBLICATION'),
                'unknown worker completion')
        retained = completion.get('persistentObservationToolRetained')
        require(type(retained) is bool, 'missing worker observation-tool disposition')
        require(retained or completion.get('status') == 'REFUSED_BEFORE_PUBLICATION',
                'completed engine phase must retain the observation tool')
        image = self.config['observationTool']
        path = self.output / 'observation-tool.json'
        require(not path.is_symlink(), 'symlink observation-tool handoff')
        if not path.exists():
            require(not retained, 'retained tool lacks durable adoption receipt')
        else:
            receipt = json.loads(path.read_text())
            require(isinstance(receipt, dict) and all(receipt.get(k) == value for k, value in image.items()),
                    'observation-tool receipt differs from sealed images')
            expected = 'PERSISTENT_OBSERVATION_TOOL_ADOPTED' if retained else 'REVERTED_BEFORE_ENGINE_PUBLICATION'
            require(receipt.get('status') == expected, 'incomplete/unknown observation-tool handoff')
            if retained:
                require(receipt.get('engineRollbackRetainsThisOwnedReadOnlyTool') is True,
                        'observation-tool retention policy mismatch')
        runner = self.root / 'arc/browser/webextras_scenario_current_run.py'
        require(sha(runner) == image['after' if retained else 'before'], 'observation-tool handoff source mismatch')
        self.tool_adopted = retained

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
        self.regeneration_clear()
        require(sha(self.output / 'config.json') == self.config_digest, 'sealed config evidence drift')
        for name, value in {**self.config['pins'], **self.config['baselineFiles']}.items():
            require(sha(pin_path(self.root, name)) == value, 'immutable input drift: ' + name)
        observation = self.config['observationTool']
        require(sha(self.root / 'arc/browser/webextras_scenario_current_run.py') == observation['after' if self.tool_adopted else 'before'],
                'persistent observation-tool drift')
        require(all(image_state(relative(self.root, name)) == (self.config['pins'][name], mode)
                    for name, mode in self.config['regeneration']['toolModes'].items()), 'shared regeneration baseline mode drift')
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
        actual = {name: sha(engine_path(self.root, name)) for name in paths}
        require(all(actual[n] == v[phase] for n, v in paths.items()), 'source/patch phase drift')
        require(all(image_state(relative(self.root, n)) == (v[phase], v[phase + 'Mode'])
                    for n, v in self.images.items()), 'target/MANIFEST mode phase drift')
        require({str(p.relative_to(self.root)) for p in (self.root / 'patches').glob('*.patch')} ==
                {n for n, image in self.config['patchOutputs'].items() if image[phase] is not None},
                'all-patch-output inventory drift')
        self.index_boundary()
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
            path = engine_path(self.root, name)
            if image['before'] is not None:
                destination = self.output / 'before' / name
                destination.parent.mkdir(parents=True, exist_ok=True)
                destination.write_bytes(path.read_bytes())
                mode = stat.S_IMODE(path.stat().st_mode)
                require(mode == image['beforeMode'], 'journal mode differs from sealed preimage')
                write(self.output / 'modes' / (name.replace('/', '_') + '.json'), mode)
            require(sha(relative(self.root, image['blob'])) == image['after'], 'private postimage drift')
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
                require(self.command(['git', 'init', '--quiet', str(cwd)], f'private-init-{i}', env=git_environment()) == 0,
                        'private repository initialization failed')
            for check in (True, False):
                argv = ['git', 'apply', '--whitespace=error'] + (['--check'] if check else []) + [str(relative(self.root, patch['path']))]
                require(self.command(argv, f'private-{i}-{check}', cwd, git_environment()) == 0,
                        'private patch reconstruction failed')
        require(all(sha(stage / n) == image['after'] for n, image in self.config['targets'].items()),
                'ordered patches do not reconstruct reviewed target images')

    def publish(self):
        self.file_state('before'); self.cancelled()
        self.mutated = True
        write(self.output / 'publication.json', {'state': 'PUBLISHING', 'images': self.images})
        for name, image in self.images.items():
            path = engine_path(self.root, name)
            require(image_state(path) == (image['before'], image['beforeMode']), 'foreign bytes/mode before publication')
            atomic_bytes(path, relative(self.root, image['blob']).read_bytes(), image['afterMode'])
            require(image_state(path) == (image['after'], image['afterMode']), 'publication bytes/mode mismatch')
        self.current = 'after'

    def adopt_observation_tool(self):
        """Separately approved provenance tool survives an engine rollback."""
        self.file_state('before'); self.stopped(); self.cancelled()
        image = self.config['observationTool']; path = self.root / 'arc/browser/webextras_scenario_current_run.py'
        before = path.read_bytes(); mode = stat.S_IMODE(path.stat().st_mode)
        private = self.output / 'observation-stage'; (private / 'arc/browser').mkdir(parents=True)
        (private / 'arc/browser/webextras_scenario_current_run.py').write_bytes(before)
        for argv, label in [(['git', 'init', '--quiet', str(private)], 'observation-private-init'),
                            (['git', 'apply', '--check', '--whitespace=error', str(relative(self.root, image['patch']))], 'observation-check'),
                            (['git', 'apply', '--whitespace=error', str(relative(self.root, image['patch']))], 'observation-reconstruct')]:
            require(self.command(argv, label, private, git_environment()) == 0, 'observation tool strict reconstruction failed')
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
        self.regeneration_clear()
        command = self.config['regeneration']
        env = git_environment()
        before = self.index_boundary()
        path = self.output / f'regeneration-lease-{self.phase + 1:04d}.json'
        require(not os.path.lexists(path), 'regeneration lease name already exists')
        receipt = {'status': 'OPEN', 'configSHA256': self.config_digest, 'originals': self.original_indexes, 'before': before}
        write(path, receipt)
        self._lease_active = True
        code = None; failure = None
        try:
            code = self.command(command['argv'], 'rollback-regen' if rollback else 'regen',
                                relative(self.root, command['cwd']), env, rollback)
            require(code == 0, 'regeneration failed')
        except BaseException as error:
            failure = repr(error)
            raise
        finally:
            try:
                after = self.index_boundary()
                require(after == before, 'raw/physical index lease changed during owned regeneration')
                write(path, {**receipt, 'status': 'CHECKED', 'after': after, 'exit': code, 'error': failure})
            except BaseException as error:
                self.latch_regeneration_failure(error, path)
                raise
            finally:
                self._lease_active = False

    def scenario(self, scenario, number, recovery=False):
        self.file_state('before' if recovery else 'after')
        script = self.root / 'arc/browser/webextras_scenario_current_run.py'
        module = source_module(script, self.config['observationTool']['after'])
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
        require(len(resources) == 3 and {x.get('textualId') for x in resources} == RESOURCE_IDS and all(
            all(x.get(k) is True for k in ('generatedExactMatch', 'grdInputExactMatch', 'intermediateExactMatch'))
            for x in resources), 'incomplete decoded/generated resource proof')
        sanity = receipt.get('nativePackSanity', {})
        require(set(sanity) == {'intermediate', 'built', 'packaged'} and all(
                type(s.get('version')) is int and s['version'] == 5 and s.get('nativeLoaderChecksPassed') is True
                and s.get('writerLookupDeterminismPolicyPassed') is True for s in sanity.values()),
                'missing native V5 DataPack sanity proof')
        for name in (*RESOURCE_INPUTS, str(NODE)):
            path = pin_path(self.root, name)
            require(receipt.get('inputs', {}).get(str(path), {}).get('sha256') == self.config['pins'][name],
                    'resource receipt used unsealed parser/decoder bytes')
        return {'receipt': str(directory / 'receipt.json'), 'sha256': sha(directory / 'receipt.json'),
                'runtimePakSHA256': before['sha256'], 'expectHostGate': check['expectHostGate']}

    def rollback(self):
        self.stopped(); self.lock_check(); self.pin_check()
        # Unknown bytes, including raw-only index changes, never authorize overwrite.
        self.index_boundary()
        for name, image in self.images.items():
            require(image_state(engine_path(self.root, name)) in
                    ((image['before'], image['beforeMode']), (image['after'], image['afterMode'])),
                    'foreign target bytes/mode at rollback')
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
            path = engine_path(self.root, name)
            state = image_state(path)
            require(state in ((image['before'], image['beforeMode']), (image['after'], image['afterMode'])),
                    'foreign target bytes/mode before rollback write')
            if state == (image['before'], image['beforeMode']):
                continue
            if image['before'] is None:
                path.unlink()
            else:
                mode = json.loads((self.output / 'modes' / (name.replace('/', '_') + '.json')).read_text())
                require(mode == image['beforeMode'], 'rollback journal mode differs from sealed preimage')
                atomic_bytes(path, (self.output / 'before' / name).read_bytes(), mode)
        self.regenerate(rollback=True)
        # Regen cannot remove an originally absent, now unreferenced own patch.
        for name, image in self.config['patchOutputs'].items():
            path = engine_path(self.root, name)
            if image['before'] is None and path.exists():
                require(sha(path) == image['after'], 'unexpected orphan patch bytes')
                path.unlink()
        self.file_state('before')
        self.scenario('launch-slice', self.config['runNumbers']['recovery-launch-slice'], recovery=True)
        self.file_state('before'); self.stopped()

    def execute(self):
        try:
            self.adopt_observation_tool(); self.prepare(); self.publish(); self.regenerate()
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
            transaction.capture_preworker_release_state(args.config.resolve(), args.config_sha)
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
            transaction.stopped()
            transaction.regeneration_clear()
            if (state == 'REFUSED_BEFORE_PUBLICATION' and
                    completion.get('persistentObservationToolRetained') is False and
                    not os.path.lexists(output / 'observation-tool.json')):
                write(output / 'release-safe.json', transaction.certify_refused_without_writes(completion, child))
                return child.returncode
            transaction.refresh_observation_tool_after_worker(completion)
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
