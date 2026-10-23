#!/usr/bin/env python3
"""Copied62 replay and real-Git racy fixtures; no production Git/tool action."""
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile
import types
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[2]
EVIDENCE = ROOT / 'logs/wp/webextras-nested-regeneration-racy'
PREVIOUS = ROOT / 'logs/wp/webextras-nested-regeneration-environment'
APP8 = dict(PATH='/usr/bin:/bin', LANG='C', LC_ALL='C', GIT_OPTIONAL_LOCKS='0',
            GIT_CONFIG_NOSYSTEM='1', GIT_CONFIG_SYSTEM='/dev/null',
            GIT_CONFIG_GLOBAL='/dev/null', GIT_ATTR_NOSYSTEM='1')
PREFIX = 'third_party/devtools-frontend/src'
COPY = '        shutil.copyfile(actual, private)\n'
TIMESTAMP = COPY + "        # The index timestamp is part of Git's racily-clean detection state.\n        os.utime(private, ns=(private.stat().st_atime_ns, identity[5]))\n        require(private.stat().st_mtime_ns == identity[5], 'private index timestamp not preserved')\n"
COMMANDS = []
PROOFS = []
SHA = lambda p: hashlib.sha256(p.read_bytes()).hexdigest()


def run(argv, cwd, env=None, okay=True):
    assert Path(cwd).resolve().is_relative_to(PRIVATE)
    result = subprocess.run([str(a) for a in argv], cwd=cwd, env=env or APP8, capture_output=True)
    COMMANDS.append(dict(argv=[str(a) for a in argv], cwd=str(cwd), exit=result.returncode,
                         stdout=result.stdout.decode(errors='replace'), stderr=result.stderr.decode(errors='replace')))
    if okay: assert result.returncode == 0, COMMANDS[-1]
    return result


def git(repo, *args, env=None): return run(['/usr/bin/git', *args], repo, env).stdout


def physical(p):
    digest = SHA(p)
    s = p.stat()
    # Real Git/ordinary reads update access-time on this filesystem. Never
    # restore it; preserve every identity/content/change-time field below.
    return dict(sha256=digest, **{k: getattr(s, k) for k in ['st_dev', 'st_ino', 'st_size', 'st_mode', 'st_uid', 'st_gid', 'st_mtime_ns', 'st_ctime_ns']})


def load_helper(root, previous=False):
    origin = PREVIOUS if previous else EVIDENCE
    dest = root / 'arc/tools/regen-chromium-patches.py'
    dest.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(origin / 'staged/arc/tools/regen-chromium-patches.py', dest)
    module = types.ModuleType('copied_racy_helper')
    module.__file__ = str(dest)
    exec(compile(dest.read_bytes(), str(dest), 'exec'), module.__dict__)
    return module


class RacyFixture:
    def __init__(self, root, new_leaf=False):
        self.root = root
        self.source = root / 'chromium/src'
        self.nested = self.source / PREFIX
        self.repos = [('', self.source), (PREFIX, self.nested)]
        self.leaves = {}
        self.setup = []
        for prefix, repo in self.repos:
            repo.mkdir(parents=True, exist_ok=True)
            git(repo, 'init', '-q')
            for key, value in [('user.name', 'Disposable racy fixture'), ('user.email', 'fixture@invalid'), ('commit.gpgsign', 'false')]:
                git(repo, 'config', '--local', key, value)
            leaf = 'tracked.cc' if not prefix else 'tracked.ts'
            self.leaves[prefix] = [leaf]
            (repo / leaf).write_text('const int tracked_value = 1;\n')
            git(repo, 'add', leaf)
            git(repo, 'commit', '-qm', 'disposable baseline')
        git(self.source, 'update-index', '--add', '--cacheinfo', '160000,' + git(self.nested, 'rev-parse', 'HEAD').decode().strip() + ',' + PREFIX)
        git(self.source, 'commit', '-qm', 'disposable known nested gitlink')
        for prefix, repo in self.repos:
            leaf = self.leaves[prefix][0]
            # Bounded setup retries only: require a genuine default-Git same-second
            # cached/index/working timestamp, never weaken the dirty-file oracle.
            for attempt in range(3):
                (repo / leaf).write_text('const int tracked_value = 1;\n')
                git(repo, 'add', leaf)
                (repo / leaf).write_text('const int tracked_value = 2;\n')
                debug = git(repo, 'ls-files', '--debug', '--', leaf).decode()
                cached = int(re.search(r'\bmtime: (\d+):', debug).group(1))
                current = (repo / leaf).stat().st_mtime_ns // 1_000_000_000
                indexed = (repo / '.git/index').stat().st_mtime_ns // 1_000_000_000
                self.setup.append(dict(prefix=prefix, attempt=attempt, cachedMtimeSeconds=cached, actualMtimeSeconds=current, indexMtimeSeconds=indexed))
                if cached == current == indexed: break
            else: raise AssertionError('could not construct bounded genuine same-second fixture')
            if new_leaf:
                new = 'new-owned.cc' if not prefix else 'new-owned.ts'
                (repo / new).write_text('const int added_value = 7;\n')
                self.leaves[prefix].append(new)
                assert not git(repo, 'ls-files', '--', new)
        (root / 'patches').mkdir(parents=True)
        rows = ['0001-racy.patch ' + (prefix + '/' if prefix else '') + leaf for prefix, _ in self.repos for leaf in self.leaves[prefix]]
        (root / 'patches/MANIFEST').write_text('\n'.join(rows) + '\n')
        self.output = root / 'patches/0001-racy.patch'
        self.output.write_bytes(b'original output\n')
        self.paths = [repo / '.git/index' for _, repo in self.repos] + [repo / leaf for prefix, repo in self.repos for leaf in self.leaves[prefix]]
        self.before = {str(p.relative_to(root)): physical(p) for p in self.paths}
        self.access_before = {str(p.relative_to(root)): p.stat().st_atime_ns for p in self.paths}
        self.expected = b''
        self.oracle_debug = []
        for number, (prefix, repo) in enumerate(self.repos):
            # Git itself supplies the exact patch oracle. For a new leaf only,
            # register it in an independently preserved-mtime PRIVATE index.
            env = dict(APP8)
            if new_leaf:
                index = root / ('oracle-' + str(number) + '.index')
                original = repo / '.git/index'
                shutil.copyfile(original, index)
                os.utime(index, ns=(index.stat().st_atime_ns, original.stat().st_mtime_ns))
                env['GIT_INDEX_FILE'] = str(index)
                git(repo, 'add', '--intent-to-add', '--', self.leaves[prefix][1], env=env)
                debug = git(repo, 'ls-files', '--debug', '--', self.leaves[prefix][0], env=env).decode()
                # A normal index rewrite must smudge this dirty racy entry.
                assert re.search(r'\bsize: 0\b', debug), debug
                self.oracle_debug.append(dict(prefix=prefix, debug=debug))
            lead = prefix + '/' if prefix else ''
            data = git(repo, 'diff', '--no-color', '--no-ext-diff', '--no-textconv', '--no-renames', '--src-prefix=a/' + lead, '--dst-prefix=b/' + lead, 'HEAD', '--', *sorted(self.leaves[prefix]), env=env)
            assert b'-const int tracked_value = 1;' in data and b'+const int tracked_value = 2;' in data, data
            if new_leaf: assert b'+const int added_value = 7;' in data, data
            self.expected += data
        self.preserved()

    def preserved(self):
        assert self.before == {str(p.relative_to(self.root)): physical(p) for p in self.paths}
        self.access_after = {str(p.relative_to(self.root)): p.stat().st_atime_ns for p in self.paths}


class RacyIndexes(unittest.TestCase):
    def setUp(self):
        self.root = PRIVATE / self.id().split('.')[-1]
        self.root.mkdir()

    def exercise(self, new_leaf=False, delayed=False):
        fixture = RacyFixture(self.root, new_leaf)
        module = load_helper(self.root)
        original = module.shutil.copyfile
        copied = []
        def copy(src, dst, *args, **kwargs):
            result = original(src, dst, *args, **kwargs)
            if Path(src).name == 'index':
                stamp = Path(src).stat().st_mtime_ns
                if delayed: os.utime(dst, ns=(stamp + 3_000_000_000, stamp + 3_000_000_000))
                copied.append(dict(source=str(src), sourceMtimeNs=stamp, immediatelyCopiedMtimeNs=Path(dst).stat().st_mtime_ns))
            return result
        private_first_reads = []
        actual_git = module.git
        def observed(repo, *args, **kwargs):
            env = module.REPO_ENV.get(str(repo))
            if env and not kwargs.get('real', False) and str(repo) not in [x['repo'] for x in private_first_reads]:
                private = Path(env['GIT_INDEX_FILE'])
                source = module.REAL_INDEX[str(repo)]
                assert private.stat().st_mtime_ns == source[1][5]
                private_first_reads.append(dict(repo=str(repo), args=args, privateMtimeNs=private.stat().st_mtime_ns, realMtimeNs=source[1][5]))
            return actual_git(repo, *args, **kwargs)
        with patch.object(module.shutil, 'copyfile', side_effect=copy), patch.object(module, 'git', side_effect=observed):
            module.regenerate(self.root)
        self.assertEqual(fixture.output.read_bytes(), fixture.expected)
        self.assertEqual(len(private_first_reads), 2)
        self.assertIn(b'+const int tracked_value = 2;', fixture.output.read_bytes())
        fixture.preserved()
        PROOFS.append(dict(case=self.id(), newLeafPrivateAddN=new_leaf, simulatedDelayedCopy=delayed, expectedSha256=SHA(fixture.output), expectedBytes=len(fixture.expected), copies=copied, firstPrivateReads=private_first_reads, setup=fixture.setup, oracleSmudgedEntries=fixture.oracle_debug, realPhysicalBeforeAfter=fixture.before, readAccessTimeBefore=fixture.access_before, readAccessTimeAfter=fixture.access_after))

    def test_outer_nested_same_second_without_new_leaf(self): self.exercise()
    def test_outer_nested_same_second_with_private_add_n(self): self.exercise(new_leaf=True)
    def test_delayed_copy_without_new_leaf(self): self.exercise(delayed=True)
    def test_delayed_copy_with_private_add_n(self): self.exercise(new_leaf=True, delayed=True)

    def test_old26_no_new_leaf_counterexample_and_add_n_behavior(self):
        for new_leaf in [False, True]:
            root = self.root / str(new_leaf)
            fixture = RacyFixture(root, new_leaf)
            module = load_helper(root, previous=True)
            original = module.shutil.copyfile
            def late(src, dst, *args, **kwargs):
                result = original(src, dst, *args, **kwargs)
                if Path(src).name == 'index':
                    stamp = Path(src).stat().st_mtime_ns + 3_000_000_000
                    os.utime(dst, ns=(stamp, stamp))
                return result
            with patch.object(module.shutil, 'copyfile', side_effect=late): module.regenerate(root)
            observed = fixture.output.read_bytes()
            if new_leaf:
                # On this real Git, add-N independently refreshes/smudges the
                # dirty entries. Record that rescue; do not invent a failure.
                self.assertEqual(observed, fixture.expected)
            else:
                self.assertNotEqual(observed, fixture.expected)
                self.assertEqual(observed, b'')
            fixture.preserved()
            PROOFS.append(dict(case=self.id(), newLeafPrivateAddN=new_leaf, old26ReturnedSuccess=True, missingTrackedEdits=not new_leaf, privateAddNRescuedTrackedEdits=new_leaf, observedBytes=len(observed), observedSha256=SHA(fixture.output), expectedBytes=len(fixture.expected), expectedSha256=hashlib.sha256(fixture.expected).hexdigest(), setup=fixture.setup, realPhysicalBeforeAfter=fixture.before, readAccessTimeBefore=fixture.access_before, readAccessTimeAfter=fixture.access_after))

    def test_timestamp_precision_failure_refused_before_private_git(self):
        fixture = RacyFixture(self.root)
        module = load_helper(self.root)
        original_copy = module.shutil.copyfile
        original_utime = module.os.utime
        actual_git = module.git
        private_calls = []
        def late(src, dst, *args, **kwargs):
            result = original_copy(src, dst, *args, **kwargs)
            if Path(src).name == 'index':
                stamp = Path(src).stat().st_mtime_ns + 3_000_000_000
                original_utime(dst, ns=(stamp, stamp))
            return result
        def observed(repo, *args, **kwargs):
            if module.REPO_ENV.get(str(repo)) and not kwargs.get('real', False): private_calls.append(args)
            return actual_git(repo, *args, **kwargs)
        with patch.object(module.shutil, 'copyfile', side_effect=late), patch.object(module.os, 'utime', return_value=None), patch.object(module, 'git', side_effect=observed):
            with self.assertRaisesRegex(ValueError, 'private index timestamp not preserved'): module.regenerate(self.root)
        self.assertEqual(private_calls, [])
        self.assertEqual(fixture.output.read_bytes(), b'original output\n')
        fixture.preserved()

    def test_source_delta_exactly_timestamp_fragment(self):
        old = (PREVIOUS / 'staged/arc/tools/regen-chromium-patches.py').read_text()
        new = (EVIDENCE / 'staged/arc/tools/regen-chromium-patches.py').read_text()
        self.assertEqual(old.count(COPY), 1)
        self.assertEqual(new, old.replace(COPY, TIMESTAMP))
        self.assertEqual((PREVIOUS / 'staged/arc/tools/regen-patches.sh').read_bytes(), (EVIDENCE / 'staged/arc/tools/regen-patches.sh').read_bytes())


def main():
    global PRIVATE
    pins = json.loads((EVIDENCE / 'preserved-predecessors.json').read_text())
    for rel, digest in pins.items(): assert SHA(ROOT / rel) == digest, rel
    path = ROOT / 'arc/browser/webextras_nested_regeneration_environment_test.py'
    source = path.read_text()
    old_name = 'webextras_nested_regeneration_environment.patch'
    assert source.count(old_name) == 1
    source = source.replace(old_name, 'webextras_nested_regeneration_racy.patch')
    replay = types.ModuleType('copied_62_racy_replay'); replay.__file__ = str(path)
    exec(compile(source, str(path), 'exec'), replay.__dict__)
    replay.EVIDENCE = EVIDENCE
    def structural_oracle(self):
        old = (PREVIOUS / 'staged/arc/tools/regen-chromium-patches.py').read_text()
        new = (EVIDENCE / 'staged/arc/tools/regen-chromium-patches.py').read_text()
        self.assertEqual(new, old.replace(COPY, TIMESTAMP))
        module = types.ModuleType('exact_environment'); exec(compile(new, '<copied-helper>', 'exec'), module.__dict__)
        self.assertEqual(module.ENV, APP8)
    # Only the old structural oracle changes: exact timestamp fragment replaces
    # its now-obsolete expectation that ENV is the sole permitted difference.
    replay.EnvironmentContract.test_exact_eight_helper_environment_and_unchanged_semantics = structural_oracle
    replay_result = replay.main()
    with tempfile.TemporaryDirectory(prefix='webextras-racy-real-git-') as temp:
        PRIVATE = Path(temp).resolve()
        version = run(['/usr/bin/git', '--version'], PRIVATE).stdout.decode().strip()
        result = unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromTestCase(RacyIndexes))
    for rel, digest in pins.items(): assert SHA(ROOT / rel) == digest, rel
    (EVIDENCE / 'racy-commands.json').write_text(json.dumps(COMMANDS, indent=2) + '\n')
    (EVIDENCE / 'racy-proofs.json').write_text(json.dumps(dict(scope='real Git in disposable outer+nested repos only', gitVersion=version, proofs=PROOFS, replay62StructuralOracleExplicitlyUpdated=True, predecessorPinsPreserved=pins), indent=2) + '\n')
    return 0 if replay_result == 0 and result.wasSuccessful() else 1


if __name__ == '__main__': sys.exit(main())
