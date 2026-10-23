#!/usr/bin/env python3
"""PRIVATE two-file generator adoption. No SEALED config or successor grant supplied.

No engine/build/app/scenario actions. Missing parent admission never authorizes work.
"""
import argparse
import ast
import hashlib
import json
import os
from pathlib import Path
import plistlib
import shutil
import signal
import stat
import struct
import subprocess
import sys
import tempfile
import time
import types
import uuid

ROOT = Path(__file__).resolve().parents[2]
WRAPPER = 'arc/tools/regen-patches.sh'
HELPER = 'arc/tools/regen-chromium-patches.py'
TOOLS = {WRAPPER: 0o755, HELPER: 0o644}
BEFORE_WRAPPER = 'dabcc5255e59c45d64555f8f1adea81e6fc2f329a22349fe2738513919f5b151'
APP8 = {'PATH': '/usr/bin:/bin', 'LANG': 'C', 'LC_ALL': 'C', 'GIT_OPTIONAL_LOCKS': '0',
        'GIT_CONFIG_NOSYSTEM': '1', 'GIT_CONFIG_SYSTEM': '/dev/null',
        'GIT_CONFIG_GLOBAL': '/dev/null', 'GIT_ATTR_NOSYSTEM': '1'}
PARSER_SHA = '14cf9aa8377da51031990cfb99868a7386c88aa2bbab5196f8bfb0c1a4b90024'
PARSER_POLICY = {'logs/wp/app-run6-vnav-integration-driver-v4/transaction.py': '444e2c0738c8d5c8e425cc2eb67c4cf2bdabe6ee7695009ec9935a7c921590cc',
 'logs/wp/app-run6-vnav-integration-driver-v4/parent-validation-classification.json': 'ec464fe306831bc2146b6070beffe6b33bbfe11e922b8eadff1c7742fe715417',
 'logs/wp/app-run6-vnav-integration-driver-v4-runtime/transaction.review.json': '27fa4631aa530882c02f995bae28db05f08a318848f305eb46cd6670a8ccf354',
 'logs/wp/app-run6-vnav-integration-driver-v4-runtime/freeze.json': 'ddb03b308063ac63a267bd48af2fd3200ebc8afaef6ecb5172f2566865d6fb34'}
SIGNALS = (signal.SIGINT, signal.SIGTERM, signal.SIGHUP)
CANCELLED = False
IDENTITY = {'sha256', 'dev', 'ino', 'size', 'mode', 'mtimeNs', 'ctimeNs'}


class CommandFailure(RuntimeError):
    def __init__(self, code):
        super().__init__('actual generator failed: ' + str(code))
        self.exit_code = code


def hold_forever(output, reason):
    while True:
        try:
            write(output / 'HOLD.json', {'reason': repr(reason), 'mustRetainLock': True})
        except BaseException:
            pass  # Receipt/post-proof failure cannot release an uncertain lock.
        time.sleep(1)


def require(value, message):
    if not value:
        raise RuntimeError(message)


# EXACT attributed App444e pure parser. No App driver import or execution.
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

def safe(root, name):
    require(isinstance(name, str) and name and not Path(name).is_absolute() and
            str(Path(name)) == name and not set(Path(name).parts) & {'.', '..'}, 'unsafe relative path')
    p = root
    for part in Path(name).parts:
        p = p / part
        require(not p.is_symlink(), 'symlink path: ' + name)
    return p


def identity(path):
    path = Path(path)
    if not os.path.lexists(path):
        return None
    before = path.lstat()
    require(stat.S_ISREG(before.st_mode), 'not a regular file: ' + str(path))
    digest = hashlib.sha256(path.read_bytes()).hexdigest()
    after = path.lstat()
    fields = ('st_dev', 'st_ino', 'st_size', 'st_mode', 'st_mtime_ns', 'st_ctime_ns')
    require(all(getattr(before, k) == getattr(after, k) for k in fields), 'unstable file: ' + str(path))
    return dict(sha256=digest, dev=after.st_dev, ino=after.st_ino, size=after.st_size,
                mode=stat.S_IMODE(after.st_mode), mtimeNs=after.st_mtime_ns, ctimeNs=after.st_ctime_ns)


def digest(path):
    value = identity(path)
    return value['sha256'] if value else None


def read_path(root, name):
    if not Path(name).is_absolute():
        return safe(root, name)
    path = Path(name)
    app = (root.parent / 'arc/build/ArcModAgent-ours156.app').resolve()
    require((path.is_relative_to(root) or path.is_relative_to(app)) and path.resolve() == path,
            'external/aliased read guard')
    return path


def binary_inventory(root):
    """Fixed built/framework + exact ours156 package roles; read-only discovery."""
    app = root.parent / 'arc/build/ArcModAgent-ours156.app'
    built = root / 'chromium/src/out/Base/Chromium.app/Contents/Frameworks/ArcCore.framework'
    packaged = app / 'Contents/Frameworks/ArcCore.framework'
    result = {}
    for role, framework in [('built', built), ('packaged', packaged)]:
        item = {name: str((framework / path).resolve()) for name, path in
                [('engine', 'Versions/A/ArcCore'), ('resources', 'Versions/A/Resources/resources.pak')]}
        helpers, plists = [], []
        for bundle in sorted((framework / 'Helpers').glob('*.app')):
            info = bundle / 'Contents/Info.plist'
            value = plistlib.loads(info.read_bytes())['CFBundleExecutable']
            require(isinstance(value, str) and Path(value).name == value, 'invalid helper executable')
            executable = (bundle / 'Contents/MacOS' / value).resolve()
            require(executable.is_relative_to(framework.resolve()), 'escaped framework helper')
            helpers.append(str(executable)); plists.append(str(info.resolve()))
        require(helpers, 'missing fixed-role framework helper inventory')
        item.update(helpers=helpers, helperPlists=plists)
        result[role] = item
    result['packaged'].update(main=str((app / 'Contents/MacOS/Arc').resolve()),
                             recorder=str((root / 'tests/recorder/ArcEngineRecorder.dylib').resolve()),
                             appPlist=str((app / 'Contents/Info.plist').resolve()))
    return result


def binary_paths(inventory):
    return [p for group in inventory.values() for value in group.values()
            for p in (value if isinstance(value, list) else [value])]


def write(path, value):
    """Durable receipts; an uncertain write must not become safe completion."""
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_name(path.name + '.new-' + uuid.uuid4().hex)
    try:
        with temporary.open('x') as stream:
            stream.write(json.dumps(value, indent=2) + '\n'); stream.flush(); os.fsync(stream.fileno())
        temporary.chmod(0o600); temporary.replace(path)
        fd = os.open(path.parent, os.O_RDONLY)
        try:
            os.fsync(fd)
        finally:
            os.close(fd)
        expected = hashlib.sha256((json.dumps(value, indent=2) + '\n').encode()).hexdigest()
        require(digest(path) == expected, 'receipt post-proof failed')
        return expected
    finally:
        temporary.unlink(missing_ok=True)


def load(path):
    before = identity(path)
    require(before is not None, 'missing receipt: ' + str(path))
    value = json.loads(Path(path).read_text())
    require(identity(path) == before, 'receipt changed while reading')
    return value


def template():
    return {'schema': 1, 'status': 'UNSEALED', 'repo': str(ROOT), 'pins': dict(PARSER_POLICY),
            'proposal': None, 'review': None, 'ack': None, 'parentState': None,
            'environment': dict(APP8), 'tools': {
                WRAPPER: {'before': BEFORE_WRAPPER, 'after': None, 'blob': None, 'mode': 0o755, 'identity': None},
                HELPER: {'before': None, 'after': None, 'blob': None, 'mode': 0o644, 'identity': None}},
            'guards': {}, 'productionInventory': {}, 'patchInventory': [], 'gitRepos': [],
            'binaryGuards': {'built': [], 'packaged': []}, 'gitMetadata': {},
            'executables': {'git': '/usr/bin/git', 'python': str(Path(sys.executable).resolve()),
                            'helperPython': '/usr/bin/python3',
                            'helperRuntime': str(Path('/Applications/Xcode.app/Contents/Developer/usr/bin/python3').resolve()),
                            'env': '/usr/bin/env', 'bash': '/bin/bash'},
            'note': 'Successor proposal/review/postimages/ACK and fresh epoch unset. No capture/adoption authorized.'}


def validate(config, root):
    require(__debug__ and not sys.flags.optimize, 'optimized Python unsupported')
    require(set(config) == set(template()), 'unknown/missing standalone config fields')
    require(config['schema'] == 1 and config['status'] == 'SEALED', 'UNSEALED; no lock or adoption')
    require(Path(config['repo']) == root.resolve(), 'wrong physical repository')
    require(config['environment'] == APP8 and set(config['tools']) == set(TOOLS), 'wrong fixed action/environment')
    source = Path(__file__).read_text()
    function = next(n for n in ast.parse(source).body if isinstance(n, ast.FunctionDef) and n.name == 'logical_index_seal')
    exact = ''.join(source.splitlines(True)[function.lineno - 1:function.end_lineno])
    require(hashlib.sha256(exact.encode()).hexdigest() == PARSER_SHA, 'attributed parser changed')
    pins = config['pins']; require(isinstance(pins, dict), 'invalid pins')
    require(not set(pins) & set(TOOLS), 'mutable tool also immutable pin')
    for name, expected in PARSER_POLICY.items():
        require(pins.get(name) == expected, 'parser provenance not pinned')
    for name in ('proposal', 'review', 'parentState'):
        item = config[name]
        require(isinstance(item, str) and item in pins, 'missing exact ' + name)
        safe(root, item)
    ack = config['ack']
    require(isinstance(ack, dict) and set(ack) == {'reference', 'path', 'sha256', 'tools'} and
            isinstance(ack['reference'], str) and ack['reference'].strip() and
            pins.get(ack['path']) == ack['sha256'], 'missing exact owner ACK')
    require(set(config['executables']) == {'git', 'python', 'helperPython', 'helperRuntime', 'env', 'bash'} and
            config['executables']['git'] == '/usr/bin/git' and config['executables']['helperPython'] == '/usr/bin/python3' and
            config['executables']['env'] == '/usr/bin/env' and config['executables']['bash'] == '/bin/bash' and
            config['executables']['python'] == str(Path(sys.executable).resolve()) and
            Path(config['executables']['helperRuntime']).is_absolute() and
            config['executables']['helperRuntime'] == str(Path('/Applications/Xcode.app/Contents/Developer/usr/bin/python3').resolve()), 'untrusted executables')
    for name, value in pins.items():
        path = Path(name) if Path(name).is_absolute() else safe(root, name)
        require(not path.is_absolute() or path.is_relative_to(root) or name in config['executables'].values(), 'external pin')
        require(isinstance(value, str) and len(value) == 64 and digest(path) == value, 'immutable pin drift: ' + name)
    require(all(name in pins for name in ('arc/browser/webextras_generator_adoption.py', 'arc/tools/locked.sh',
                                         'sync-chromium.sh', *config['executables'].values())), 'driver/tools not pinned')
    require(config['tools'][WRAPPER]['before'] == BEFORE_WRAPPER and config['tools'][HELPER]['before'] is None,
            'not direct-dabc/helper-absent adoption')
    for name, image in config['tools'].items():
        require(set(image) == {'before', 'after', 'blob', 'mode', 'identity'} and image['mode'] == TOOLS[name] and
                type(image['mode']) is int and image['after'] != image['before'] and
                isinstance(image['after'], str) and len(image['after']) == 64 and
                pins.get(image['blob']) == image['after'] and ack['tools'].get(name) == image['after'], 'unsealed tool result')
        safe(root, image['blob'])
        require((image['identity'] is None and image['before'] is None) or
                (set(image['identity']) == IDENTITY and image['identity']['sha256'] == image['before'] and
                 image['identity']['mode'] == image['mode']), 'invalid original tool identity')
    require(set(ack['tools']) == set(TOOLS), 'ACK target scope')
    require(config['guards'] and not set(config['guards']) & set(TOOLS), 'missing guards/tool overlap')
    for name, image in config['guards'].items():
        read_path(root, name); require(image is None or set(image) == IDENTITY, 'incomplete physical guard')
    require(set(config['productionInventory']) == {'arc/api', 'arc/browser', 'arc/wp'}, 'missing production inventory')
    require(config['patchInventory'] and all(p.startswith('patches/') and p.endswith('.patch') and
            p in config['guards'] for p in config['patchInventory']), 'missing all patch outputs')
    require('patches/MANIFEST' in config['guards'] and
            all(n in config['guards'] for n in ('arc/BUILD.gn', 'build-babysit.sh', 'package-engine.sh',
                                              'chromium/src/out/Base/args.gn')), 'missing native/build guard')
    require(config['binaryGuards'] == binary_inventory(root) and
            all(name in config['guards'] and config['guards'][name] is not None for name in binary_paths(config['binaryGuards'])),
            'incomplete/wrong fixed build or packaged binary roles')
    require(set(config['gitMetadata']) == {r['path'] for r in config['gitRepos']}, 'missing Git metadata contract')
    repos = config['gitRepos']
    require(repos and {r['path'] for r in repos} >= {'.', 'chromium/src'} and
            len({r['path'] for r in repos}) == len(repos), 'missing original root/Chromium indices')
    for repo in repos:
        require(set(repo) == {'path', 'index', 'head', 'snapshot', 'logicalSeal', 'identity'}, 'invalid original index contract')
        if repo['path'] != '.': safe(root, repo['path'])
        safe(root, repo['index']); snapshot = safe(root, repo['snapshot'])
        require(set(repo['identity']) == IDENTITY and pins.get(repo['snapshot']) == repo['identity']['sha256'] and
                snapshot != safe(root, repo['index']) and
                list(logical_index_seal(snapshot.read_bytes())) == repo['logicalSeal'] and
                isinstance(repo['head'], str) and len(repo['head']) == 40, 'invalid original index snapshot/seal')
    return config


def processes():
    text = subprocess.check_output(['/bin/ps', '-axo', 'pid=,ppid=,pgid=,stat=,command=', '-ww'], text=True)
    rows = []
    for line in text.splitlines():
        parts = line.strip().split(None, 4); require(len(parts) == 5, 'unknown process census')
        rows.append(dict(pid=int(parts[0]), ppid=int(parts[1]), pgid=int(parts[2]), state=parts[3], command=parts[4]))
    return rows


class Backend:
    def __init__(self, output):
        self.output, self.groups, self.receipts = output, set(), {}

    def inventory(self):
        return processes()

    def command(self, argv, label, cwd):
        receipt = self.output / (label + '.child.json')
        write(receipt, {'state': 'SPAWNING', 'argv': argv})
        with (self.output / (label + '.log')).open('xb') as stream:
            child = subprocess.Popen(argv, cwd=cwd, env=APP8, stdin=subprocess.DEVNULL,
                                     stdout=stream, stderr=subprocess.STDOUT, start_new_session=True)
            self.groups.add(child.pid)
            try:
                write(receipt, {'state': 'RUNNING', 'argv': argv, 'pid': child.pid, 'pgid': child.pid})
            finally:
                while True:
                    try:
                        child.wait()
                        if not any(r['pgid'] == child.pid and not r['state'].startswith('Z') for r in self.inventory()): break
                    except BaseException:
                        pass
                    time.sleep(.2)
            self.receipts[label] = write(receipt, {'state': 'DRAINED', 'argv': argv, 'pid': child.pid, 'pgid': child.pid, 'exit': child.returncode})
            return child.returncode


class Transaction:
    def __init__(self, config, output, backend=None):
        self.root, self.output = ROOT, Path(output)
        require(self.output.parent == self.root / 'logs/wp/webextras-generator-adoption-private/runs' and
                self.output.resolve() == self.output, 'output outside owned adoption runs')
        self.config = validate(config, self.root)
        self.config_sha = digest(self.output / 'config.json')
        require(load(self.output / 'config.json') == config, 'config receipt drift')
        self.launch = load(self.output / 'launch.json')
        require(self.launch['configSHA256'] == self.config_sha, 'launch/config mismatch')
        self.backend = backend or Backend(self.output)
        self.lock = None; self.admission = None; self.request = None; self.preworker = None
        self.attempted = False; self.committed = False; self.commit_attempted = False
        self.uncertain_publication = False
        self.owned = {}; self.prepared = {}; self.failed_lease = False; self.lease_active = False
        self.commands = []; self.lease_paths = {}; self.worker_identity = None; self.worker_nonce = None
        self.journal_sha = None; self.adopted_sha = None; self.reverted_sha = None; self.runtime_sha = None; self.evidence = {}

    def hold(self, reason):
        hold_forever(self.output, reason)

    def cancelled(self):
        require(not CANCELLED and not os.path.lexists(self.output / 'cancel.request'), 'cancelled')

    def lock_check(self):
        lock = self.root / 'logs/arccore-build.lock'; pid = lock / 'pid'
        require(not lock.is_symlink() and lock.is_dir() and not pid.is_symlink(), 'invalid protected lock')
        raw = pid.read_text(); owner = int(raw); rows = {r['pid']: r for r in self.backend.inventory()}
        current = os.getpid()
        for _ in range(64):
            if current == owner: break
            require(current in rows and current > 1, 'protected lock not ancestor'); current = rows[current]['ppid']
        else: raise RuntimeError('lock ancestry bound')
        require(load(self.output / 'protected.json')['pid'] == owner, 'wrong protected launcher')
        stamp = [lock.stat().st_dev, lock.stat().st_ino, pid.stat().st_ino, raw]
        require(self.lock is None or stamp == self.lock, 'lock identity drift'); self.lock = stamp

    def stopped(self):
        app = self.root.parent / 'arc/build/ArcModAgent-ours156.app/Contents'
        prefixes = (str(app / 'MacOS/Arc'), str((app / 'MacOS/Arc').resolve()),
                    str(app / 'Frameworks/ArcCore.framework') + '/', str((app / 'Frameworks/ArcCore.framework').resolve()) + '/')
        require(not any(not r['state'].startswith('Z') and (r['pgid'] in self.backend.groups or
                any(r['command'] == p or r['command'].startswith(p if p.endswith('/') else p + ' ') for p in prefixes))
                for r in self.backend.inventory()), 'live command/app/helper group')

    def clear(self):
        require(not self.failed_lease and not os.path.lexists(self.output / 'lease-failure.json'), 'irreversible lease failure')
        if not self.lease_active:
            for path in self.output.glob('lease-*.json'):
                if path.name == 'lease-failure.json': continue
                row = load(path)
                require(row.get('state') == 'CHECKED' and row.get('configSHA256') == self.config_sha and
                        row.get('launchNonce') == self.launch['nonce'] and
                        row.get('requestSHA256') == digest(self.output / 'admission-request.json') and
                        row.get('workerNonce') == load(self.output / 'worker-intent.json')['workerNonce'] and
                        row.get('before') == row.get('after') and row.get('before') and
                        row['before'] == {r['path']: {'identity': r['identity'], 'head': r['head'],
                                                     'logicalSeal': r['logicalSeal']} for r in self.config['gitRepos']} and
                        ((row.get('exit') == 0 and row.get('error') is None) or isinstance(row.get('error'), str)), 'OPEN/invalid lease')

    def base_guard(self):
        self.clear(); self.lock_check(); self.stopped()
        require(digest(self.output / 'config.json') == self.config_sha, 'config changed')
        for name, expected in self.config['pins'].items():
            require(digest(Path(name) if Path(name).is_absolute() else safe(self.root, name)) == expected, 'pin drift: ' + name)
        for name, expected in self.config['guards'].items():
            require(identity(read_path(self.root, name)) == expected, 'foreign source/patch/build drift: ' + name)
        require(binary_inventory(self.root) == self.config['binaryGuards'], 'binary/helper/alias inventory drift')
        require(sorted(str(p.relative_to(self.root)) for p in (self.root / 'patches').glob('*.patch')) ==
                sorted(self.config['patchInventory']), 'patch inventory drift')
        for folder, expected in self.config['productionInventory'].items():
            actual = sorted(str(p.relative_to(self.root)) for p in (self.root / folder).rglob('*')
                            if p.is_file() and p.suffix in ('.mm', '.cc', '.h', '.gn', '.gni'))
            require(actual == sorted(expected) and all(p in self.config['guards'] for p in actual), 'production inventory drift')

    def runtime_argv(self):
        return [self.config['executables']['env'], '-i', *[k + '=' + v for k,v in APP8.items()],
                self.config['executables']['helperPython'], '-c',
                'import os,sys; print(os.path.realpath(sys.executable))']

    def runtime(self):
        self.admission_guard(); self.cancelled()
        require(self.call(self.runtime_argv(), 'runtime-selection', self.root) == 0, 'runtime selection command failed')
        log = self.output / 'runtime-selection.log'
        selected = log.read_text().strip()
        require(selected == self.config['executables']['helperRuntime'], 'wrapper Python runtime selection drift')
        self.runtime_sha = write(self.output / 'runtime-selection.json',
            {'selectedRealpath': selected, 'logSHA256': digest(log), 'configSHA256': self.config_sha,
             'launchNonce': self.launch['nonce'], 'requestSHA256': self.request_sha, 'workerNonce': self.worker_nonce})

    def metadata(self):
        """Bounded direct-.git layout; effective config is hashed, never logged."""
        _, owners = self.helper().manifest(self.root)
        native = self.root / 'chromium/src'
        for repo in self.config['gitRepos']:
            cwd = self.root if repo['path'] == '.' else safe(self.root, repo['path'])
            gitdir = cwd / '.git'
            require(gitdir.is_dir() and not gitdir.is_symlink() and
                    safe(self.root, repo['index']) == gitdir / 'index', 'unsupported indirect Git metadata layout')
            require(not os.path.lexists(gitdir / 'config.worktree'), 'unsupported worktree Git config')
            names = {gitdir / leaf for leaf in ('config', 'info/attributes', 'info/exclude')}
            names.update(cwd / leaf for leaf in ('.gitattributes', '.gitignore'))
            for leaf in owners:
                path = native / leaf
                if not path.is_relative_to(cwd): continue
                # Each assigned leaf belongs to the deepest pinned direct repo.
                if any(other['path'] != repo['path'] and safe(self.root, other['path']).is_relative_to(cwd) and
                       (native / leaf).is_relative_to(safe(self.root, other['path']))
                       for other in self.config['gitRepos'] if other['path'] != '.'):
                    continue
                parent = path.parent
                while parent != cwd:
                    names.update(parent / item for item in ('.gitattributes', '.gitignore'))
                    parent = parent.parent
            raw = subprocess.check_output([self.config['executables']['git'], 'config', '--null', '--show-origin', '--list'],
                                          cwd=cwd, env=APP8, stderr=subprocess.PIPE)
            fields = raw.split(b'\0'); require(fields[-1] == b'' and len(fields) % 2 == 1, 'unsupported config-origin framing')
            for i in range(0, len(fields)-1, 2):
                origin, pair = fields[i], fields[i+1]
                key = pair.split(b'\n', 1)[0].lower()
                require(origin == ('file:' + str(gitdir / 'config')).encode() or origin == b'file:.git/config',
                        'unsupported external Git config origin')
                require(not key.startswith((b'include.', b'includeif.')) and key not in
                        (b'core.attributesfile', b'core.excludesfile'), 'unsupported included/external Git metadata')
            actual = {'configSHA256': hashlib.sha256(raw).hexdigest(),
                      'paths': {str(path.relative_to(self.root)): identity(path) for path in sorted(names)}}
            require(actual == self.config['gitMetadata'][repo['path']], 'Git config/attribute metadata drift')
        return True

    def indices(self, discover=True):
        result = {}
        for repo in self.config['gitRepos']:
            index = safe(self.root, repo['index'])
            require(not any(index.parent.glob('sharedindex.*')), 'shared indices unsupported')
            value = identity(index); require(value == repo['identity'], 'original raw/physical index drift')
            require(list(logical_index_seal(index.read_bytes())) == repo['logicalSeal'] and identity(index) == value, 'logical index drift')
            if discover:
                cwd = self.root if repo['path'] == '.' else safe(self.root, repo['path'])
                def query(*args):
                    return subprocess.check_output([self.config['executables']['git'], *args], cwd=cwd, env=APP8, text=True).strip()
                path = Path(query('rev-parse', '--git-path', 'index'))
                if not path.is_absolute(): path = cwd / path
                require(path.resolve() == index.resolve() and query('rev-parse', 'HEAD') == repo['head'], 'actual HEAD/index path drift')
                require(identity(index) == value, 'index changed during discovery')
            result[repo['path']] = {'identity': value, 'head': repo['head'], 'logicalSeal': repo['logicalSeal']}
        return result

    def tools_before(self):
        require(all(identity(safe(self.root, p)) == v['identity'] for p, v in self.config['tools'].items()), 'tool preimage drift')

    def admission_guard(self):
        self.base_guard()
        require(self.admission is not None and identity(self.output / 'admission.json') == self.admission, 'admission changed/missing')
        require(identity(safe(self.root, self.config['parentState'])) == self.request['heldState'], 'parent held-state changed')
        require(digest(self.output / 'admission-request.json') == self.request_sha, 'request changed')
        require(self.request['configSHA256'] == self.config_sha and self.request['launchNonce'] == self.launch['nonce'] and
                self.request['lock'] == self.lock and self.request['state'] == 'WAITING_FOR_PARENT_ADMISSION' and
                self.request['action'] == 'ADOPT_TWO_REGENERATOR_FILES', 'request binding changed')
        expected = {k: self.request[k] for k in ('action', 'requestNonce', 'launchNonce', 'configSHA256', 'lock', 'guardianPID', 'heldState')}
        expected.update(decision='ADMIT', ready=True, requestSHA256=self.request_sha)
        actual = load(self.output / 'admission.json')
        require(actual == expected and type(actual.get('ready')) is bool, 'parent admission binding')
        if self.worker_identity is not None:
            require(identity(self.output / 'worker.json') == self.worker_identity, 'worker binding changed')

    def await_admission(self):
        self.base_guard(); self.tools_before(); self.indices(discover=False)
        require(not os.path.lexists(self.output / 'admission.json'), 'prewritten admission rejected')
        self.request = {'state': 'WAITING_FOR_PARENT_ADMISSION', 'action': 'ADOPT_TWO_REGENERATOR_FILES',
                        'configSHA256': self.config_sha, 'launchNonce': self.launch['nonce'],
                        'requestNonce': uuid.uuid4().hex, 'lock': self.lock, 'guardianPID': os.getpid(),
                        'heldState': identity(safe(self.root, self.config['parentState'])),
                        'tools': {p: identity(safe(self.root, p)) for p in TOOLS},
                        'originalIndices': self.indices(discover=False)}
        write(self.output / 'admission-request.json', self.request)
        self.request_sha = digest(self.output / 'admission-request.json')
        expected = {'action': self.request['action'], 'decision': 'ADMIT', 'ready': True,
                    'requestSHA256': self.request_sha, 'requestNonce': self.request['requestNonce'],
                    'launchNonce': self.launch['nonce'], 'configSHA256': self.config_sha,
                    'lock': self.lock, 'guardianPID': os.getpid(), 'heldState': self.request['heldState']}
        while not os.path.lexists(self.output / 'admission.json'):
            self.cancelled(); self.base_guard(); self.tools_before(); self.indices(discover=False); time.sleep(.2)
        require(load(self.output / 'admission.json') == expected and
                type(load(self.output / 'admission.json').get('ready')) is bool, 'stale/malformed parent admission')
        self.admission = identity(self.output / 'admission.json')
        self.admission_guard(); self.tools_before(); self.indices(discover=False)
        self.preworker = {'requestSHA256': self.request_sha, 'admission': self.admission,
                          'configSHA256': self.config_sha, 'lock': self.lock}
        write(self.output / 'guardian-preworker.json', self.preworker)

    def restore_admission(self):
        # Worker loads immutable guardian proof; guardian never reconstructs itself.
        self.request = load(self.output / 'admission-request.json'); self.request_sha = digest(self.output / 'admission-request.json')
        proof = load(self.output / 'guardian-preworker.json')
        require(proof['requestSHA256'] == self.request_sha and proof['configSHA256'] == self.config_sha and
                proof['lock'] == self.request['lock'], 'worker admission binding')
        self.admission = proof['admission']; self.lock_check(); require(self.lock == proof['lock'], 'worker lock binding')
        self.admission_guard()

    def worker_gate(self, nonce):
        require(isinstance(nonce, str) and len(nonce) == 32 and os.getppid() == self.request['guardianPID'], 'wrong spawning guardian')
        self.worker_nonce = nonce
        intent = {'configSHA256': self.config_sha, 'launchNonce': self.launch['nonce'],
                  'requestSHA256': self.request_sha, 'guardianPID': self.request['guardianPID'], 'workerNonce': nonce}
        require(load(self.output / 'worker-intent.json') == intent, 'worker intent mismatch')
        while not os.path.lexists(self.output / 'worker.json'):
            self.admission_guard(); self.cancelled(); time.sleep(.1)
        require(load(self.output / 'worker.json') == {**intent, 'pid': os.getpid()}, 'wrong guardian-spawned worker')
        self.worker_identity = identity(self.output / 'worker.json')

    def call(self, argv, label, cwd):
        require(label not in self.commands, 'duplicate command')
        self.commands.append(label)
        code = self.backend.command(argv, label, cwd)
        self.evidence[label + '.child.json'] = self.backend.receipts[label]
        return code

    def lease(self, label, operation):
        self.clear(); self.admission_guard(); self.metadata(); before = self.indices()
        require(label not in self.lease_paths, 'duplicate lease')
        path = self.output / ('lease-' + label + '-' + uuid.uuid4().hex + '.json')
        self.lease_paths[label] = path.name
        receipt = {'state': 'OPEN', 'configSHA256': self.config_sha, 'launchNonce': self.launch['nonce'],
                   'requestSHA256': self.request_sha, 'workerNonce': self.worker_nonce, 'label': label, 'before': before}
        write(path, receipt); self.lease_active = True; code = None; error = None
        try:
            result = operation(); code = 0; return result
        except BaseException as failure:
            code = getattr(failure, 'exit_code', None); error = repr(failure); raise
        finally:
            try:
                self.metadata(); after = self.indices(); require(after == before, 'raw/physical lease drift')
                self.evidence[path.name] = write(path, {**receipt, 'state': 'CHECKED', 'after': after, 'exit': code, 'error': error})
            except BaseException as failure:
                self.failed_lease = True
                try: write(self.output / 'lease-failure.json', {'state': 'IRREVERSIBLE_HOLD', 'lease': path.name,
                           'configSHA256': self.config_sha, 'error': repr(failure)})
                finally: raise
            finally: self.lease_active = False

    def helper(self):
        image = self.config['tools'][HELPER]; path = safe(self.root, image['blob'])
        data = path.read_bytes(); require(hashlib.sha256(data).hexdigest() == image['after'], 'helper image drift')
        module = types.ModuleType('reviewed_generator_oracle'); module.__file__ = str(path)
        exec(compile(data, str(path), 'exec'), module.__dict__)
        require(module.ENV == APP8, 'successor helper not fixed App8')
        return module

    def canonical(self):
        """Read-only calculation. NEVER call publishing generate/regenerate."""
        helper = self.helper(); source = safe(self.root, 'chromium/src')
        raw, owners = helper.manifest(self.root)
        require(all('chromium/src/' + leaf in self.config['guards'] for leaf in owners), 'unguarded native leaf')
        repos = helper.repositories(source, owners)
        expected_repos = {'chromium/src' + ('/' + prefix if prefix else '') for prefix in repos}
        require(expected_repos <= {r['path'] for r in self.config['gitRepos']}, 'unguarded nested repository')
        original = helper.fingerprint(self.root, source, owners, repos)
        with tempfile.TemporaryDirectory(prefix='webextras-generator-oracle-') as directory:
            helper.private_indexes(repos, owners, Path(directory)); helper.validate_sources(source, owners, repos)
            private_before = {repo: digest(Path(env['GIT_INDEX_FILE'])) for repo, env in helper.REPO_ENV.items()}
            outputs = {}
            for patch in sorted(set(owners.values())):
                chunks, expected = [], set()
                for prefix, (repo, _) in sorted(repos.items()):
                    leaves = sorted(helper.location(leaf, repos)[1] for leaf, owner in owners.items()
                                    if owner == patch and helper.location(leaf, repos)[0] == prefix)
                    if not leaves: continue
                    lead = prefix + '/' if prefix else ''
                    rows = helper.git(repo, 'diff', '--name-status', '-z', '--no-color', '--no-ext-diff', '--no-textconv',
                                      '--no-renames', 'HEAD', '--', *leaves).stdout.split(b'\0')
                    require(rows[-1] == b'' and len(rows) % 2 == 1, 'bad canonical Git metadata')
                    for i in range(0, len(rows) - 1, 2):
                        status, leaf = rows[i].decode(), helper.path_name(rows[i + 1].decode())
                        require(status in ('A', 'M'), 'unsupported canonical source change')
                        if status == 'A': require(not helper.git(repo, 'ls-tree', '-z', 'HEAD', '--', leaf, real=True).stdout,
                                                  'HEAD-tracked leaf marked new')
                        expected.add(lead + leaf)
                    chunks.append(helper.git(repo, 'diff', '--no-color', '--no-ext-diff', '--no-textconv', '--no-renames',
                                             '--src-prefix=a/' + lead, '--dst-prefix=b/' + lead, 'HEAD', '--', *leaves).stdout)
                data = b''.join(chunks)
                if data: helper.generated_paths(source, data, patch, owners, expected)
                else: require(not expected, 'missing canonical paths')
                require((self.root / 'patches' / patch).read_bytes() == data, 'maintained patch differs from canonical: ' + patch)
                outputs[patch] = hashlib.sha256(data).hexdigest()
            helper.validate_sources(source, owners, {prefix: (repo, helper.entries(repo)) for prefix, (repo, _) in repos.items()})
            require(helper.fingerprint(self.root, source, owners, repos) == original and
                    (self.root / 'patches/MANIFEST').read_bytes() == raw, 'canonical inputs changed')
            require(all(digest(Path(helper.REPO_ENV[repo]['GIT_INDEX_FILE'])) == value for repo, value in private_before.items()),
                    'canonical private index changed')
        return outputs

    def prepare(self):
        self.admission_guard(); self.tools_before(); self.cancelled()
        private = self.output / 'reconstruction'; (private / 'arc/tools').mkdir(parents=True)
        (private / WRAPPER).write_bytes((self.root / WRAPPER).read_bytes())
        git = self.config['executables']['git']; proposal = str(safe(self.root, self.config['proposal']))
        for n, argv in enumerate(([git, 'init', '--quiet'], [git, 'apply', '--check', '--whitespace=error', proposal],
                                  [git, 'apply', '--whitespace=error', proposal])):
            require(self.call(argv, 'reconstruct-' + str(n), private) == 0, 'private reconstruction failed')
        require({str(p.relative_to(private)) for p in private.rglob('*') if p.is_file() and '.git' not in p.relative_to(private).parts} == set(TOOLS), 'proposal exceeds two-tool scope')
        for name, image in self.config['tools'].items():
            require(digest(private / name) == image['after'], 'reconstruction postimage mismatch')
        self.admission_guard(); self.tools_before()

    @staticmethod
    def same_prepared(actual, prepared):
        return actual is not None and {k: v for k, v in actual.items() if k != 'ctimeNs'} == {
            k: v for k, v in prepared.items() if k != 'ctimeNs'}

    def publish(self):
        self.admission_guard(); self.tools_before(); self.cancelled(); self.indices()
        self.attempted = True
        self.journal = {'state': 'ADOPTING', 'configSHA256': self.config_sha, 'launchNonce': self.launch['nonce'],
                        'requestSHA256': self.request_sha, 'prepared': {}, 'published': {}}
        self.journal_sha = write(self.output / 'tool-journal.json', self.journal)
        # Helper first; actual wrapper remains old until both prepared images exist.
        for name in (HELPER, WRAPPER):
            path = safe(self.root, name); image = self.config['tools'][name]
            temporary = path.with_name('.' + path.name + '.adopt-' + uuid.uuid4().hex)
            self.uncertain_publication = True
            with temporary.open('xb') as stream:
                stream.write(safe(self.root, image['blob']).read_bytes()); stream.flush(); os.fsync(stream.fileno())
            temporary.chmod(image['mode']); prepared = identity(temporary)
            require(prepared['sha256'] == image['after'], 'prepared bytes changed')
            self.prepared[name] = (temporary, prepared)
            self.journal['prepared'][name] = prepared
            self.journal_sha = write(self.output / 'tool-journal.json', self.journal)
            self.uncertain_publication = False
        for name in (HELPER, WRAPPER):
            self.admission_guard(); self.cancelled(); self.indices()
            require(all(identity(safe(self.root, p)) == value for p, value in self.owned.items()), 'foreign earlier tool postimage')
            path = safe(self.root, name); temporary, prepared = self.prepared[name]
            require(identity(path) == self.config['tools'][name]['identity'] and identity(temporary) == prepared, 'foreign tool before CAS')
            self.uncertain_publication = True
            try:
                os.replace(temporary, path)
            except OSError:
                if identity(path) == self.config['tools'][name]['identity'] and identity(temporary) == prepared:
                    self.uncertain_publication = False
                raise
            actual = identity(path)
            require(self.same_prepared(actual, prepared), 'foreign tool during CAS')
            self.owned[name] = actual; self.journal['published'][name] = actual
            self.journal_sha = write(self.output / 'tool-journal.json', self.journal)
            self.uncertain_publication = False
        self.tools_after()

    def tools_after(self):
        require(set(self.owned) == set(TOOLS) and all(identity(safe(self.root, p)) == v for p, v in self.owned.items()), 'foreign postimage tool')

    def cleanup_prepared(self):
        for path, expected in self.prepared.values():
            if os.path.lexists(path):
                require(identity(path) == expected, 'foreign temporary image'); path.unlink()

    def revert(self):
        self.admission_guard(); self.stopped(); self.indices(); self.metadata()
        restored = {}; restore_prepared = {}
        for name in (WRAPPER, HELPER):
            path = safe(self.root, name)
            if name not in self.owned:
                require(identity(path) == self.config['tools'][name]['identity'], 'foreign unpublished tool')
                restored[name] = identity(path); continue
            require(identity(path) == self.owned[name], 'foreign tool preserved during reversion')
            if name == HELPER:
                path.unlink(); restored[name] = None
            else:
                temporary = path.with_name('.' + path.name + '.revert-' + uuid.uuid4().hex)
                require(digest(self.output / 'wrapper.before') == BEFORE_WRAPPER, 'original wrapper backup changed')
                with temporary.open('xb') as stream:
                    stream.write((self.output / 'wrapper.before').read_bytes()); stream.flush(); os.fsync(stream.fileno())
                temporary.chmod(TOOLS[name])
                prepared = identity(temporary)
                write(self.output / 'reversion-prepared.json', prepared)
                restore_prepared[name] = prepared
                require(identity(path) == self.owned[name], 'foreign wrapper before reversion CAS')
                os.replace(temporary, path); actual = identity(path)
                require(self.same_prepared(actual, prepared) and actual['sha256'] == BEFORE_WRAPPER, 'reversion proof failed')
                restored[name] = actual
        self.cleanup_prepared(); self.base_guard(); self.indices()
        require(all((identity(safe(self.root, name)) or {}).get('sha256') == image['before'] for name, image in self.config['tools'].items()), 'tool reversion failed')
        self.reverted_sha = write(self.output / 'reverted.json', {'state': 'REVERTED_BEFORE_ADOPTION', 'configSHA256': self.config_sha,
                                 'tools': restored, 'prepared': restore_prepared})

    def finish(self, state):
        require(self.worker_identity == identity(self.output / 'worker.json'), 'worker binding changed')
        evidence = {}
        for label in self.commands:
            path = self.output / (label + '.child.json')
            require(load(path).get('state') == 'DRAINED', 'missing/undrained command proof')
            require(digest(path) == self.evidence[path.name], 'operation proof changed before completion')
            evidence[path.name] = self.evidence[path.name]
        for name in self.lease_paths.values():
            path = self.output / name; require(load(path).get('state') == 'CHECKED', 'missing lease proof')
            require(digest(path) == self.evidence[name], 'lease proof changed before completion')
            evidence[name] = self.evidence[name]
        write(self.output / 'completion.json', {'state': state, 'configSHA256': self.config_sha,
              'launchNonce': self.launch['nonce'], 'requestSHA256': self.request_sha,
              'admissionSHA256': self.admission['sha256'], 'engineJournalStarted': False,
              'workerIdentity': self.worker_identity, 'workerNonce': self.worker_nonce,
              'commands': self.commands, 'leases': self.lease_paths, 'evidence': evidence,
              'runtimeSHA256': self.runtime_sha, 'journalSHA256': self.journal_sha, 'adoptedSHA256': self.adopted_sha, 'revertedSHA256': self.reverted_sha})

    def execute(self, worker_nonce):
        try:
            self.restore_admission(); self.worker_gate(worker_nonce); self.runtime(); self.metadata(); self.prepare()
            before = self.lease('canonical-before', self.canonical)
            write(self.output / 'canonical-before.json', before)
            (self.output / 'wrapper.before').write_bytes((self.root / WRAPPER).read_bytes())
            self.publish()
            def official():
                self.admission_guard(); self.tools_after(); self.cancelled()
                code = self.call(['./arc/tools/regen-patches.sh'], 'official-regeneration', self.root)
                self.tools_after()
                if code != 0: raise CommandFailure(code)
                return code
            self.lease('official', official)
            after = self.lease('canonical-after', self.canonical)
            require(after == before, 'canonical output set changed')
            self.admission_guard(); self.indices(); self.tools_after(); self.cancelled(); self.cleanup_prepared()
            self.commit_attempted = True
            self.adopted_sha = write(self.output / 'adopted.json', {'state': 'PERSISTENT_GENERATOR_BASELINE_ADOPTED', 'configSHA256': self.config_sha,
                  'launchNonce': self.launch['nonce'], 'requestSHA256': self.request_sha, 'admissionSHA256': self.admission['sha256'],
                  'prepared': self.journal['prepared'], 'postimages': self.owned, 'canonical': after,
                  'laterEngineRollbackMustRetainTools': True})
            self.committed = True; self.finish('ADOPTED'); return 0
        except BaseException as error:
            write(self.output / 'failure.json', {'error': repr(error), 'attempted': self.attempted, 'commitAttempted': self.commit_attempted})
            if self.commit_attempted or self.failed_lease or self.uncertain_publication: self.hold(error)
            try:
                if self.attempted: self.revert(); self.finish('REVERTED')
                else:
                    self.admission_guard(); self.tools_before(); self.indices(); self.finish('REFUSED')
                return 1
            except BaseException as failure: self.hold(failure)

    def certify(self, child):
        self.admission_guard(); self.stopped(); self.indices(); self.metadata()
        require(load(self.output / 'guardian-preworker.json') == self.preworker, 'preworker proof changed')
        row = load(self.output / 'completion.json')
        require(all(row.get(k) == v for k, v in {'configSHA256': self.config_sha, 'launchNonce': self.launch['nonce'],
                        'requestSHA256': self.request_sha, 'admissionSHA256': self.admission['sha256']}.items()) and
                row.get('engineJournalStarted') is False and row['state'] in ('ADOPTED', 'REVERTED', 'REFUSED') and
                child.returncode == (0 if row['state'] == 'ADOPTED' else 1), 'unsafe worker completion')
        require(row['workerNonce'] == self.worker_nonce and row['workerIdentity'] == identity(self.output / 'worker.json') and
                load(self.output / 'worker.json') == {'pid': child.pid, 'guardianPID': os.getpid(), 'workerNonce': self.worker_nonce,
                    'configSHA256': self.config_sha, 'launchNonce': self.launch['nonce'], 'requestSHA256': self.request_sha}, 'worker handoff mismatch')
        commands = ['runtime-selection', 'reconstruct-0', 'reconstruct-1', 'reconstruct-2', 'official-regeneration']
        leases = ['canonical-before', 'official', 'canonical-after']
        require(isinstance(row['commands'], list) and row['commands'] == commands[:len(row['commands'])] and
                list(row['leases']) == leases[:len(row['leases'])], 'incomplete/unknown operation order')
        if row['state'] == 'ADOPTED':
            require(row['commands'] == commands and list(row['leases']) == leases, 'missing complete adoption operation proof')
        if row['state'] == 'REVERTED':
            require(len(row['commands']) >= 4 and row['leases'], 'missing reverted attempt proof')
        expected_files = {label + '.child.json' for label in row['commands']} | set(row['leases'].values())
        require(set(row['evidence']) == expected_files and
                {p.name for p in self.output.glob('*.child.json')} | {p.name for p in self.output.glob('lease-*.json')} == expected_files,
                'command/lease inventory missing or unexpected')
        for name, expected in row['evidence'].items():
            require(Path(name).name == name and expected is not None and digest(self.output / name) == expected, 'operation proof drift')
        for key, name in [('runtimeSHA256', 'runtime-selection.json'), ('journalSHA256', 'tool-journal.json'), ('adoptedSHA256', 'adopted.json'), ('revertedSHA256', 'reverted.json')]:
            require(digest(self.output / name) == row[key], 'transaction handoff receipt drift')
        command_argv = {'runtime-selection': self.runtime_argv(), 'reconstruct-0': [self.config['executables']['git'], 'init', '--quiet'],
                        'reconstruct-1': [self.config['executables']['git'], 'apply', '--check', '--whitespace=error', str(safe(self.root, self.config['proposal']))],
                        'reconstruct-2': [self.config['executables']['git'], 'apply', '--whitespace=error', str(safe(self.root, self.config['proposal']))],
                        'official-regeneration': ['./arc/tools/regen-patches.sh']}
        for label in row['commands']:
            proof = load(self.output / (label + '.child.json'))
            require(proof.get('argv') == command_argv[label] and type(proof.get('pid')) is int and
                    proof.get('pid') > 1 and proof.get('pgid') == proof['pid'], 'child command binding')
            if row['state'] == 'ADOPTED': require(proof.get('exit') == 0, 'unsuccessful adopted child')
        for label, name in row['leases'].items():
            require(name.startswith('lease-' + label + '-') and name.endswith('.json') and Path(name).name == name,
                    'lease filename binding')
            proof = load(self.output / name)
            require(proof.get('label') == label, 'lease label binding')
            if row['state'] == 'ADOPTED':
                require(proof.get('exit') == 0 and proof.get('error') is None, 'unsuccessful adopted lease')
        if row['state'] in ('ADOPTED', 'REVERTED'):
            runtime = load(self.output / 'runtime-selection.json')
            require(row['runtimeSHA256'] is not None and runtime == {
                'selectedRealpath': self.config['executables']['helperRuntime'],
                'logSHA256': digest(self.output / 'runtime-selection.log'), 'configSHA256':self.config_sha,
                'launchNonce':self.launch['nonce'], 'requestSHA256':self.request_sha, 'workerNonce':self.worker_nonce} and
                load(self.output / 'runtime-selection.child.json')['exit'] == 0,
                'runtime selection completion drift')
        self.clear()
        for path in self.output.glob('*.child.json'): require(load(path).get('state') == 'DRAINED', 'undrained command intent')
        if row['state'] == 'ADOPTED':
            record = load(self.output / 'adopted.json'); journal = load(self.output / 'tool-journal.json')
            require(record['state'] == 'PERSISTENT_GENERATOR_BASELINE_ADOPTED' and row['adoptedSHA256'] is not None and
                    record['configSHA256'] == self.config_sha and record['requestSHA256'] == self.request_sha and
                    record['admissionSHA256'] == self.admission['sha256'] and record['launchNonce'] == self.launch['nonce'] and
                    record['laterEngineRollbackMustRetainTools'] is True and record['prepared'] == journal['prepared'] and
                    set(record['postimages']) == set(TOOLS), 'adoption receipt binding')
            for name, image in self.config['tools'].items():
                prepared = record['prepared'][name]; actual = identity(safe(self.root, name))
                require(self.same_prepared(actual, prepared) and actual == record['postimages'][name] and
                        actual['sha256'] == image['after'] and actual['mode'] == image['mode'], 'foreign adoption postimage')
            require(load(self.output / 'canonical-before.json') == record['canonical'], 'oracle handoff mismatch')
            self.cancelled()
        elif row['state'] == 'REFUSED':
            self.tools_before(); require(not os.path.lexists(self.output / 'tool-journal.json'), 'refusal attempted publication')
        else:
            record = load(self.output / 'reverted.json')
            for name, image in self.config['tools'].items():
                actual = identity(safe(self.root, name))
                require(actual == record['tools'].get(name) and (actual or {}).get('sha256') == image['before'] and
                        (actual is None or actual['mode'] == image['mode']), 'unsafe reverted tools')
                if name in record['prepared']:
                    require(self.same_prepared(actual, record['prepared'][name]), 'reverted prepared inode mismatch')
                else:
                    require(actual == self.request['tools'][name] or actual is None, 'unproved reverted image')
        self.stopped(); self.lock_check()
        write(self.output / 'release-safe.json', {**row, 'workerExit': child.returncode, 'noBuildOrRuntime': True})


def cancel(path):
    def handler(*_):
        global CANCELLED
        CANCELLED = True
        try: path.touch(exist_ok=True)
        except OSError: pass
    return handler


def dispatch():
    parser = argparse.ArgumentParser(allow_abbrev=False)
    parser.add_argument('mode', choices=('template', 'validate', 'run', '_protected', '_guardian', '_worker'))
    parser.add_argument('--config', type=Path); parser.add_argument('--config-sha'); parser.add_argument('--out', type=Path)
    parser.add_argument('--worker-nonce')
    args = parser.parse_args()
    if args.mode == 'template': print(json.dumps(template(), indent=2)); return 0
    require(args.config is not None and digest(args.config) == args.config_sha, 'exact config SHA required')
    config = validate(load(args.config), ROOT)
    require(args.config.read_bytes() == (json.dumps(config, indent=2) + '\n').encode(),
            'sealed config must use exact canonical JSON bytes before launch')
    if args.mode == 'validate': return 0
    require(args.out is not None, 'fresh output required'); output = args.out.resolve()
    require(output.parent == ROOT / 'logs/wp/webextras-generator-adoption-private/runs', 'output outside owned adoption runs')
    safe(ROOT, str(output.relative_to(ROOT)))
    if args.mode == 'run':
        output.mkdir(parents=True, exist_ok=False); write(output / 'config.json', config)
        write(output / 'launch.json', {'configSHA256': args.config_sha, 'nonce': uuid.uuid4().hex})
        for signum in SIGNALS: signal.signal(signum, cancel(output / 'cancel.request'))
        child = subprocess.Popen([sys.executable, str(Path(__file__).resolve()), '_protected', '--config', str(args.config.resolve()),
                                  '--config-sha', args.config_sha, '--out', str(output)], start_new_session=True)
        return child.wait()
    require(load(output / 'launch.json')['configSHA256'] == args.config_sha, 'launch receipt mismatch')
    if args.mode == '_protected':
        for signum in SIGNALS: signal.signal(signum, signal.SIG_IGN)
        write(output / 'protected.json', {'pid': os.getpid()})
        os.execv('/bin/bash', ['bash', str(ROOT / 'arc/tools/locked.sh'), sys.executable, str(Path(__file__).resolve()),
                             '_guardian', '--config', str(args.config.resolve()), '--config-sha', args.config_sha, '--out', str(output)])
    for signum in SIGNALS: signal.signal(signum, cancel(output / 'cancel.request'))
    if args.mode == '_worker': return Transaction(config, output).execute(args.worker_nonce)
    transaction = None
    try:
        transaction = Transaction(config, output)
        transaction.await_admission()  # No worker or oracle before explicit POST-LOCK admission.
        require(not os.path.lexists(output / 'worker.json') and not os.path.lexists(output / 'worker-intent.json'), 'prewritten worker binding')
        transaction.worker_nonce = uuid.uuid4().hex
        intent = {'guardianPID': os.getpid(), 'workerNonce': transaction.worker_nonce, 'configSHA256': transaction.config_sha,
                  'launchNonce': transaction.launch['nonce'], 'requestSHA256': transaction.request_sha}
        write(output / 'worker-intent.json', intent)
        argv = [sys.executable, str(Path(__file__).resolve()), '_worker', '--config', str(args.config.resolve()),
                '--config-sha', args.config_sha, '--out', str(output), '--worker-nonce', transaction.worker_nonce]
        child = subprocess.Popen(argv, stdin=subprocess.DEVNULL, start_new_session=True)
        try: write(output / 'worker.json', {**intent, 'pid': child.pid})
        finally:
            while True:
                try: child.wait(); break
                except BaseException: pass
        transaction.backend.groups = {child.pid} | {load(p).get('pgid') for p in output.glob('*.child.json')}
        transaction.certify(child); return child.returncode
    except BaseException as error:
        if transaction is None: hold_forever(output, error)
        else: transaction.hold(error)


def main():
    # _guardian is reached ONLY after locked.sh acquired the protected lock.
    # Include bootstrap config/launch/output validation in its retain-on-error gate.
    if len(sys.argv) > 1 and sys.argv[1] == '_guardian':
        try:
            return dispatch()
        except BaseException as error:
            output = ROOT / 'logs/wp/webextras-generator-adoption-private/bootstrap-HOLD'
            try:
                supplied = Path(sys.argv[sys.argv.index('--out') + 1])
                if supplied.resolve() == supplied and supplied.parent == ROOT / 'logs/wp/webextras-generator-adoption-private/runs':
                    output = supplied
            except BaseException:
                pass
            hold_forever(output, error)
    return dispatch()


if __name__ == '__main__':
    try: sys.exit(main())
    except (RuntimeError, OSError, ValueError, KeyError, AssertionError) as error:
        print(str(error), file=sys.stderr); sys.exit(2)
