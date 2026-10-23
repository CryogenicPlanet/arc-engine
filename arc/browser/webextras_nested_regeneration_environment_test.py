#!/usr/bin/env python3
"""Environment-only successor tests: copied tools/disposable repositories only.

The frozen 52 test method bodies are source-loaded unchanged. Their five PATH
fault injections are routed through a test-only runpy subprocess adapter because
the candidate correctly refuses caller PATH. No production tool is invoked.
"""
import ast
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import types
import unittest

ROOT = Path(__file__).resolve().parents[2]
EVIDENCE = ROOT / 'logs/wp/webextras-nested-regeneration-environment'
OLD = ROOT / 'logs/wp/webextras-nested-regeneration-lead-rebase-reviewed'
APP8 = dict(PATH='/usr/bin:/bin', LANG='C', LC_ALL='C', GIT_OPTIONAL_LOCKS='0',
            GIT_CONFIG_NOSYSTEM='1', GIT_CONFIG_SYSTEM='/dev/null',
            GIT_CONFIG_GLOBAL='/dev/null', GIT_ATTR_NOSYSTEM='1')
COMMANDS = []
ADAPTATIONS = []
SHA = lambda p: hashlib.sha256(p.read_bytes()).hexdigest()


def run(argv, cwd, env=None):
    argv = [str(a) for a in argv]
    assert Path(cwd).resolve().is_relative_to(PRIVATE), cwd
    result = subprocess.run(argv, cwd=cwd, env=dict(APP8, **(env or {})),
                            capture_output=True, text=True)
    COMMANDS.append(dict(argv=argv, cwd=str(cwd), exitCode=result.returncode,
                         stdout=result.stdout, stderr=result.stderr))
    return result


def checked(argv, cwd, env=None):
    result = run(argv, cwd, env)
    assert result.returncode == 0, (argv, result.stdout, result.stderr)
    return result.stdout.strip()


def install(root, tool):
    dest = root / 'arc/tools'
    dest.mkdir(parents=True, exist_ok=True)
    origin = EVIDENCE / 'staged/arc/tools' if tool == 'new' else OLD / ('baseline/arc/tools' if tool == 'dabc' else 'staged/arc/tools')
    for name in ['regen-patches.sh'] + ([] if tool == 'dabc' else ['regen-chromium-patches.py']):
        shutil.copy2(origin / name, dest / name)


def physical(p):
    s = p.lstat()
    return (SHA(p), s.st_dev, s.st_ino, s.st_mode, s.st_size, s.st_mtime_ns, s.st_ctime_ns)


def isolated_52():
    """Rebind all original fixture paths to a bounded private snapshot tree."""
    module = types.ModuleType('frozen_52_environment_replay')
    old_test = ROOT / 'arc/browser/webextras_nested_regeneration_lead_rebase_reviewed_test.py'
    module.__file__ = str(old_test)
    exec(compile(old_test.read_bytes(), str(old_test), 'exec'), module.__dict__)
    sandbox = PRIVATE / 'frozen-52-root'
    evidence = sandbox / 'logs/wp/webextras-nested-regeneration-lead-rebase-reviewed'
    for rel, origin in {
        'arc/tools/regen-patches.sh': EVIDENCE / 'baseline/arc/tools/regen-patches.sh',
        'sync-chromium.sh': ROOT / 'logs/wp/webextras-nested-patch-tools/inputs/sync-chromium.sh',
    }.items():
        module.fixture.put(sandbox, rel, origin.read_bytes())
    for rel in ['arc/tools/regen-patches.sh', 'arc/tools/regen-chromium-patches.py']:
        module.fixture.put(evidence / 'staged', rel, (EVIDENCE / 'staged' / rel).read_bytes())
    module.fixture.put(evidence, 'baseline/arc/tools/regen-patches.sh', (EVIDENCE / 'baseline/arc/tools/regen-patches.sh').read_bytes())
    proposal = sandbox / 'arc/browser/candidate.patch'
    module.fixture.put(sandbox, 'arc/browser/candidate.patch', (ROOT / 'arc/browser/webextras_nested_regeneration_environment.patch').read_bytes())
    receipt = sandbox / 'fixture-sources'
    for kind in ['base', 'intended-output']:
        for rel in [module.NATIVE, module.HOOK, *[module.NESTED + '/' + p for p in module.FRONTEND]]:
            module.fixture.put(receipt / kind, rel, (module.RECEIPT / kind / rel).read_bytes())
    pins = {'files': {r: SHA(sandbox / r) for r in ['arc/tools/regen-patches.sh', 'sync-chromium.sh', 'arc/browser/candidate.patch']},
            'absent': ['arc/tools/regen-chromium-patches.py']}
    (evidence / 'input-pins.json').write_text(json.dumps(pins))
    module.ROOT = sandbox
    module.EVIDENCE = evidence
    module.STAGED = evidence / 'staged'
    module.PROPOSAL = proposal
    module.RECEIPT = receipt
    module.fixture.ROOT = sandbox
    module.fixture.RECEIPT = receipt
    adapter = PRIVATE / 'fault_adapter.py'
    adapter.write_text('''import runpy,subprocess,sys
original=subprocess.run
helper,fake=sys.argv[1:]
def intercepted(argv,*args,**kwargs):
    if argv[0]=='git':argv=[fake,*argv[1:]]
    return original(argv,*args,**kwargs)
subprocess.run=intercepted
sys.argv=[helper]
runpy.run_path(helper,run_name='__main__')
''')

    def controlled(argv, cwd=None):
        argv = [str(a) for a in argv]
        env = dict(APP8, **module.fixture.GIT_CONTEXT)
        first = Path(os.environ['PATH'].split(os.pathsep)[0])
        fake = first / 'git'
        # Only the original suite's explicit private fault scripts qualify.
        if len(argv) == 3 and argv[1] == '-B' and argv[2].endswith('/arc/tools/regen-chromium-patches.py') and fake.is_file() and first.resolve().is_relative_to(PRIVATE):
            ADAPTATIONS.append(dict(helper=argv[2], fake=str(fake)))
            argv = [sys.executable, '-B', str(adapter), argv[2], str(fake)]
        result = run(argv, cwd or PRIVATE, env)
        module.fixture.TRANSCRIPTS.append(COMMANDS[-1])
        return result
    module.fixture.run = controlled
    return module


class EnvironmentContract(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.base = PRIVATE / 'environment-base'
        source = cls.base / 'chromium/src'
        source.mkdir(parents=True)
        checked(['git', 'init', '-q'], source)
        for key, value in [('user.name', 'Disposable fixture'), ('user.email', 'fixture@invalid'), ('commit.gpgsign', 'false')]:
            checked(['git', 'config', '--local', key, value], source)
        cls.original = 'TOP_A\n' + ''.join('padding_a_%02d\n' % i for i in range(15)) + 'MIDDLE_B\n' + ''.join('padding_b_%02d\n' % i for i in range(15)) + 'int value = 1;\n' + ''.join('tail_%02d\n' % i for i in range(8))
        (source / 'sample.cc').write_text(cls.original)
        (source / '.gitattributes').write_text('sample.cc diff=fixture\n')
        checked(['git', 'add', 'sample.cc', '.gitattributes'], source)
        checked(['git', 'commit', '-qm', 'private baseline'], source)
        (source / 'sample.cc').write_text(cls.original.replace('value = 1', 'value = 2'))
        (cls.base / 'patches').mkdir()
        (cls.base / 'patches/MANIFEST').write_text('0001-native.patch sample.cc\n')
        (cls.base / 'patches/0001-native.patch').write_text('entry output\n')

    def setUp(self):
        self.directory = PRIVATE / self.id().split('.')[-1]
        self.directory.mkdir()
        self.counter = 0

    def home(self, tag, *, xdg=False, attributes=None, default=False):
        home = self.directory / ('home-' + tag)
        home.mkdir()
        xd = home / 'xdg'
        config = xd / 'git/config' if xdg else home / '.gitconfig'
        config.parent.mkdir(parents=True, exist_ok=True)
        config.write_text('[diff "fixture"]\n xfuncname = ' + ('^TOP_A' if tag == 'A' else '^MIDDLE_B') + '\n')
        if attributes:
            attr = (xd / 'git/attributes' if xdg else home / '.config/git/attributes') if default else home / 'global-attributes'
            attr.parent.mkdir(parents=True, exist_ok=True)
            attr.write_text('sample.cc diff=fixture' + tag + '\n')
            if not default:
                config.write_text('[core]\n attributesFile = ' + str(attr) + '\n')
        return dict(HOME=str(home), XDG_CONFIG_HOME=str(xd)) if xdg else dict(HOME=str(home))

    def generate(self, tool='new', env=None, direct=False, no_local_attributes=False):
        self.counter += 1
        root = self.directory / ('run-' + str(self.counter))
        shutil.copytree(self.base, root)
        source = root / 'chromium/src'
        if no_local_attributes:
            (source / '.gitattributes').unlink()
            checked(['git', 'add', '.gitattributes'], source)
            checked(['git', 'commit', '-qm', 'private no local attributes'], source)
            # Local definitions isolate default-attribute loading from global config.
            for key, value in [('diff.fixtureA.xfuncname', '^TOP_A'), ('diff.fixtureB.xfuncname', '^MIDDLE_B')]:
                checked(['git', 'config', '--local', key, value], source)
        install(root, tool)
        before = [physical(source / p) for p in ['sample.cc', '.git/index']]
        argv = [sys.executable, '-B', root / 'arc/tools/regen-chromium-patches.py'] if direct else ['/bin/bash', root / 'arc/tools/regen-patches.sh']
        result = run(argv, root, env)
        self.assertEqual(result.returncode, 0, result.stderr + result.stdout)
        self.assertEqual(before, [physical(source / p) for p in ['sample.cc', '.git/index']])
        return (root / 'patches/0001-native.patch').read_bytes()

    def test_exact_eight_helper_environment_and_unchanged_semantics(self):
        old = (OLD / 'staged/arc/tools/regen-chromium-patches.py').read_text()
        new = (EVIDENCE / 'staged/arc/tools/regen-chromium-patches.py').read_text()
        module = types.ModuleType('environment_contract')
        exec(compile(new, '<copied-helper>', 'exec'), module.__dict__)
        self.assertEqual(module.ENV, APP8)
        # Delete only ENV assignment and its comment; every other AST node exact.
        def without_env(text):
            tree = ast.parse(text)
            tree.body = [n for n in tree.body if not isinstance(n, ast.Assign) or not any(isinstance(t, ast.Name) and t.id == 'ENV' for t in n.targets)]
            return ast.dump(tree, include_attributes=False)
        self.assertEqual(without_env(old), without_env(new))

    def test_fixed_app8_matches_copied_dabc(self):
        expected = self.generate('dabc')
        self.assertEqual(expected, self.generate())
        self.assertEqual(expected, self.generate(direct=True))

    def test_home_global_config_ignored(self):
        expected = self.generate('dabc')
        for tag in ['A', 'B']:
            env = self.home(tag)
            for direct in [False, True]: self.assertEqual(expected, self.generate(env=env, direct=direct))

    def test_xdg_global_config_ignored(self):
        expected = self.generate('dabc')
        for tag in ['A', 'B']:
            env = self.home(tag, xdg=True)
            for direct in [False, True]: self.assertEqual(expected, self.generate(env=env, direct=direct))

    def attributes(self, *, xdg=False, default=False):
        expected = self.generate('dabc', no_local_attributes=True)
        for tag in ['A', 'B']:
            env = self.home(tag, xdg=xdg, attributes=True, default=default)
            for direct in [False, True]: self.assertEqual(expected, self.generate(env=env, direct=direct, no_local_attributes=True))

    def test_global_attributes_file_ignored(self): self.attributes()
    def test_home_default_attributes_ignored(self): self.attributes(default=True)
    def test_xdg_default_attributes_ignored(self): self.attributes(default=True, xdg=True)

    def test_old_303_home_and_default_attribute_counterexamples(self):
        for attributes in [False, True]:
            data = []
            for tag in ['A', 'B']:
                # Distinct private homes for each subcase.
                sub = self.directory / ('attributes-' + str(attributes)); sub.mkdir(exist_ok=True)
                previous = self.directory; self.directory = sub
                env = self.home(tag, attributes=attributes, default=attributes)
                self.directory = previous
                data.append(self.generate('303', env=env, no_local_attributes=attributes))
            self.assertNotEqual(data[0], data[1])
            self.assertIn(b'@@ -30,7 +30,7 @@ TOP_A', data[0])
            self.assertIn(b'@@ -30,7 +30,7 @@ MIDDLE_B', data[1])

    def test_hostile_caller_git_configuration_and_path_ignored(self):
        expected = self.generate('dabc')
        fake = self.directory / 'bin'; fake.mkdir()
        marker = self.directory / 'must-not-run'
        for name in ['git', 'python3', 'dirname']:
            p = fake / name; p.write_text('#!/bin/sh\ntouch "' + str(marker) + '"\nexit 99\n'); p.chmod(0o755)
        hostile = self.directory / 'hostile.config'
        hostile.write_text('[core]\n worktree = /definitely-wrong-worktree\n')
        env = dict(PATH=str(fake), HOME=str(self.directory), XDG_CONFIG_HOME=str(self.directory),
                   GIT_CONFIG_SYSTEM=str(hostile), GIT_CONFIG_GLOBAL=str(hostile),
                   GIT_CONFIG_NOSYSTEM='0', GIT_ATTR_NOSYSTEM='0',
                   GIT_CONFIG_COUNT='1', GIT_CONFIG_KEY_0='core.worktree', GIT_CONFIG_VALUE_0='/wrong')
        for direct in [False, True]: self.assertEqual(expected, self.generate(env=env, direct=direct))
        self.assertFalse(marker.exists())

    def test_wrapper_discovery_ignores_home_worktree(self):
        home = self.directory / 'discovery-home'; home.mkdir()
        (home / '.gitconfig').write_text('[core]\n worktree = /definitely-wrong-worktree\n')
        self.assertEqual(self.generate('dabc'), self.generate(env=dict(HOME=str(home))))


def main():
    global PRIVATE
    protected = json.loads((EVIDENCE / 'preserved-303.json').read_text())
    protected.update(json.loads((EVIDENCE / 'input-pins.json').read_text()))
    for rel, digest in protected.items(): assert SHA(ROOT / rel) == digest, rel
    with tempfile.TemporaryDirectory(prefix='webextras-regen-environment-') as temp:
        PRIVATE = Path(temp).resolve()
        old_path = os.environ.get('PATH'); os.environ['PATH'] = APP8['PATH']
        old_output = os.environ.get('WEBEXTRAS_REGEN_TEST_EVIDENCE')
        os.environ['WEBEXTRAS_REGEN_TEST_EVIDENCE'] = str(EVIDENCE / 'replay52')
        try:
            module = isolated_52()
            suite = unittest.TestSuite([unittest.defaultTestLoader.loadTestsFromTestCase(module.LeadRebasedNestedRegeneration), unittest.defaultTestLoader.loadTestsFromTestCase(EnvironmentContract)])
            original_temporary_directory = tempfile.TemporaryDirectory
            def private_temporary_directory(*args, **kwargs):
                kwargs.setdefault('dir', PRIVATE)
                return original_temporary_directory(*args, **kwargs)
            # Preserve the frozen setUpClass body while bounding its directory.
            with module.mock.patch.object(tempfile, 'TemporaryDirectory', side_effect=private_temporary_directory):
                result = unittest.TextTestRunner(verbosity=2).run(suite)
            assert len(ADAPTATIONS) == 5, ADAPTATIONS
        finally:
            if old_path is None: os.environ.pop('PATH', None)
            else: os.environ['PATH'] = old_path
            if old_output is None: os.environ.pop('WEBEXTRAS_REGEN_TEST_EVIDENCE', None)
            else: os.environ['WEBEXTRAS_REGEN_TEST_EVIDENCE'] = old_output
    for rel, digest in protected.items(): assert SHA(ROOT / rel) == digest, rel
    (EVIDENCE / 'test-commands.json').write_text(json.dumps(COMMANDS, indent=2) + '\n')
    (EVIDENCE / 'test-adaptations.json').write_text(json.dumps(dict(frozen52MethodBodiesUnchanged=True, faultAdapterUses=ADAPTATIONS, preserved303=protected), indent=2) + '\n')
    return 0 if result.wasSuccessful() else 1


if __name__ == '__main__': sys.exit(main())
