#!/usr/bin/env python3
"""Distinct test-only correction; candidate22/helperd40 remain unchanged."""
import ast
import difflib
import hashlib
import json
from pathlib import Path
import time
import types

ROOT = Path(__file__).resolve().parents[2]
EVIDENCE = ROOT / 'logs/wp/webextras-nested-regeneration-racy-controls'
ORIGINAL_TEST = ROOT / 'arc/browser/webextras_nested_regeneration_racy_test.py'
ORIGINAL_SHA = '734a12c4b1eaacbace6340dc7e8b40fab7d67648de7e14aad36247f8db50c577'
SHA = lambda p: hashlib.sha256(p.read_bytes()).hexdigest()

CORRECTED_METHOD = '''    def test_old26_no_new_leaf_counterexample_and_add_n_behavior(self):
        for new_leaf in [False, True]:
            root = self.root / str(new_leaf)
            fixture = RacyFixture(root, new_leaf)
            module = load_helper(root, previous=True)
            original = module.shutil.copyfile
            actual_git = module.git
            timings = []
            def late(src, dst, *args, **kwargs):
                result = original(src, dst, *args, **kwargs)
                if Path(src).name == 'index':
                    stamp = Path(src).stat().st_mtime_ns + 3_000_000_000
                    os.utime(dst, ns=(stamp, stamp))
                return result
            def observed_git(repo, *args, **kwargs):
                env = module.REPO_ENV.get(str(repo))
                is_add_n = env and args[:2] == ('add', '--intent-to-add') and not kwargs.get('real', False)
                if is_add_n:
                    private = Path(env['GIT_INDEX_FILE'])
                    observation = dict(repo=str(repo), sourceCapturedMtimeNs=module.REAL_INDEX[str(repo)][1][5], privateMtimeBeforeAddNNs=private.stat().st_mtime_ns, wallTimeBeforeAddNNs=time.time_ns())
                result = actual_git(repo, *args, **kwargs)
                if is_add_n:
                    observation.update(privateMtimeAfterAddNNs=private.stat().st_mtime_ns, wallTimeAfterAddNNs=time.time_ns())
                    timings.append(observation)
                return result
            with patch.object(module.shutil, 'copyfile', side_effect=late), patch.object(module, 'git', side_effect=observed_git): module.regenerate(root)
            observed = fixture.output.read_bytes()
            # Old26 add-N behavior is descriptive: timestamp-second relations
            # can yield complete, partial, or no tracked-edit rescue. Candidate
            # full-byte assertions elsewhere are unchanged and remain mandatory.
            if not new_leaf:
                self.assertNotEqual(observed, fixture.expected)
                self.assertEqual(observed, b'')
            fixture.preserved()
            tracked = dict(outer=b'diff --git a/tracked.cc b/tracked.cc\\n' in observed, nested=b'diff --git a/third_party/devtools-frontend/src/tracked.ts b/third_party/devtools-frontend/src/tracked.ts\\n' in observed)
            outcome = 'full' if observed == fixture.expected else ('partial' if any(tracked.values()) else 'none')
            PROOFS.append(dict(case=self.id(), newLeafPrivateAddN=new_leaf, old26ReturnedSuccess=True, missingTrackedEdits=not all(tracked.values()), privateAddNRescueOutcome=outcome if new_leaf else 'not-applicable', trackedHunks=tracked, privateAddNTiming=timings, observedPatch=observed.decode(), expectedPatch=fixture.expected.decode(), observedBytes=len(observed), observedSha256=SHA(fixture.output), expectedBytes=len(fixture.expected), expectedSha256=hashlib.sha256(fixture.expected).hexdigest(), setup=fixture.setup, realPhysicalBeforeAfter=fixture.before, readAccessTimeBefore=fixture.access_before, readAccessTimeAfter=fixture.access_after))

'''


def corrected_source():
    assert SHA(ORIGINAL_TEST) == ORIGINAL_SHA
    source = ORIGINAL_TEST.read_text()
    start = source.index('    def test_old26_no_new_leaf_counterexample_and_add_n_behavior(self):\n')
    end = source.index('    def test_timestamp_precision_failure_refused_before_private_git(self):\n', start)
    corrected = source[:start] + CORRECTED_METHOD + source[end:]
    def without_old_control(text):
        tree = ast.parse(text)
        cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == 'RacyIndexes')
        cls.body = [n for n in cls.body if not isinstance(n, ast.FunctionDef) or n.name != 'test_old26_no_new_leaf_counterexample_and_add_n_behavior']
        return ast.dump(tree, include_attributes=False)
    assert without_old_control(source) == without_old_control(corrected)
    return source, corrected


def main():
    pins = json.loads((EVIDENCE / 'preserved-originals.json').read_text())
    for rel, digest in pins.items(): assert SHA(ROOT / rel) == digest, rel
    source, corrected = corrected_source()
    (EVIDENCE / 'corrected-test-body.py').write_text(corrected)
    (EVIDENCE / 'test-controls.diff').write_text(''.join(difflib.unified_diff(source.splitlines(True), corrected.splitlines(True), fromfile='frozen22/test.py', tofile='controls/test.py')))
    module = types.ModuleType('corrected_racy_controls')
    module.__file__ = str(ORIGINAL_TEST)
    exec(compile(corrected, str(ORIGINAL_TEST), 'exec'), module.__dict__)
    module.EVIDENCE = EVIDENCE
    module.time = time
    result = module.main()
    for rel, digest in pins.items(): assert SHA(ROOT / rel) == digest, rel
    (EVIDENCE / 'controls-preservation.json').write_text(json.dumps(dict(originalPinsUnchanged=pins, originalSourceSha256=ORIGINAL_SHA, correctedBodySha256=hashlib.sha256(corrected.encode()).hexdigest(), everyOtherASTNodeUnchanged=True, fullCandidateAssertionsUnchanged=True, mandatoryOld26NoNewLeafEmptyCounterexampleUnchanged=True, exitCode=result), indent=2) + '\n')
    return result


if __name__ == '__main__': raise SystemExit(main())
