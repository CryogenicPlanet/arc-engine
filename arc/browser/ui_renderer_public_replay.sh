#!/bin/bash
# Replay the existing renderer scenario for UI DTO coverage under the caller's
# shared lock. Never build, package, acquire a lock, or replace accepted goldens.
set -euo pipefail
umask 077
task_root="$(cd "$(dirname "$0")/../.." && pwd)"
if [ "$#" -ne 2 ] || [ "$1" != --out ]; then
  echo "usage: $0 --out ABSOLUTE_FRESH_OUTPUT_DIRECTORY" >&2
  exit 2
fi
exec python3 - "$task_root" "$2" <<'PY'
import datetime
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

root = Path(sys.argv[1]).resolve()
output_arg = Path(sys.argv[2])
if not output_arg.is_absolute():
    raise SystemExit('output must be absolute')
if '..' in output_arg.parts:
    raise SystemExit('output must not contain parent-directory traversal')
output = output_arg.absolute()
output_base = root / 'results/ui-renderer-public-replay'
if output == output_base or output_base not in output.parents:
    raise SystemExit('output must be a fresh child of ' + str(output_base))
if output.exists() or output.is_symlink():
    raise SystemExit('refusing existing output')
for parent in output.parents:
    if parent.is_symlink():
        raise SystemExit('refusing symlink output ancestry')

# This is a child of an already-owned batch, never another lock waiter.
lock_pid = int((root / 'logs/arccore-build.lock/pid').read_text().strip())
ancestor = os.getpid()
for _ in range(32):
    if ancestor == lock_pid:
        break
    if ancestor <= 1:
        raise SystemExit('existing shared-lock ancestry required')
    ancestor = int(subprocess.check_output(
        ['ps', '-p', str(ancestor), '-o', 'ppid='], timeout=2).strip())
if lock_pid <= 1 or ancestor != lock_pid:
    raise SystemExit('invalid shared-lock ancestry')

golden = root / 'results/golden/arc-1.170-renderer-public-api'
golden_record = golden / 'launch-1/record.jsonl'
if not json.loads((golden / 'coverage.json').read_text()).get('accepted'):
    raise SystemExit('renderer-public-api golden is not accepted')
if not golden_record.is_file():
    raise SystemExit('accepted golden transcript missing')
bun = shutil.which('bun')
if not bun or not Path(bun).is_absolute():
    raise SystemExit('absolute bun executable unavailable')

app = (root.parent / 'arc/build/ArcModAgent-ours156.app').resolve()
inputs = {
    root / 'arc/browser/ui_renderer_public_replay.sh',
    root / 'arc/browser/ui_isolate_probe_profiles.py',
    root / 'arc/browser/ui_boundary_diff.py',
    root / 'tests/launch-slice/run.ts',
    root / 'tests/scenarios/renderer-public-api.ts',
    root / 'tests/scenarios/renderer-public-api-driver.m',
    root / 'tests/scenarios/fixture-server.ts',
    root / 'tests/scenarios/native.ts',
    root / 'tests/scenarios/coverage.ts',
    root / 'tests/recorder/ArcEngineRecorder.m',
    root / 'tests/recorder/ArcEngineRecorder.dylib',
    root / 'tests/recorder/app_callbacks.inc',
    root / 'tests/recorder/diff.py',
    root / 'arc/tools/arccore-1.170.0-objc.tsv',
    root.parent / 'arc/tools/agent-cdp.ts',
    root.parent / 'arc/tools/agent-native.ts',
    app / 'Contents/MacOS/Arc',
    app / 'Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore',
    app / 'Contents/Info.plist',
    root / 'arc/wp/ui.gni',
}
inputs.update(root.glob('arc/api/arc_ui_*.mm'))
inputs.update(root.glob('arc/browser/ui_*.h'))
inputs.update(path for path in golden.rglob('*') if path.is_file())

def snapshot():
    result = {}
    current_inputs = inputs | set(root.glob('arc/api/arc_ui_*.mm')) | set(root.glob('arc/browser/ui_*.h'))
    current_inputs.update(path for path in golden.rglob('*') if path.is_file())
    for path in sorted(current_inputs):
        if not path.is_file():
            raise RuntimeError('required evidence input missing: ' + str(path))
        with path.open('rb') as stream:
            digest = hashlib.file_digest(stream, 'sha256').hexdigest()
        result[str(path.absolute())] = {'sha256': digest, 'bytes': path.stat().st_size}
    return result

before = snapshot()
output.mkdir(parents=True, mode=0o700)
output_base.chmod(0o700)
os.chmod(output, 0o700)
os.umask(0o077)

def save(name, value):
    path = output / name
    with path.open('x') as stream:
        json.dump(value, stream, indent=2)
        stream.write('\n')
    path.chmod(0o600)

save('evidence-before.json', before)
harness_output = output / 'harness'
journal = output / 'profile-isolation.json'
command = [sys.executable, str(root / 'arc/browser/ui_isolate_probe_profiles.py'),
           '--agents', 'ours156', '--journal', str(journal), '--', bun,
           str(root / 'tests/launch-slice/run.ts'), '--app', str(app),
           '--out', str(harness_output), '--record', '--scenario',
           'renderer-public-api', '--port', '9464']
report = {
    'scenario': 'renderer-public-api',
    'started_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
    'command': command,
    'full_harness_acceptance_claimed': False,
    'scope': 'Existing scenario replay plus explicit UI boundary comparison. '
             'Harness/quit failures remain failures; UI comparison does not waive them.',
    'paths': {'output': str(output), 'harness': str(harness_output),
              'journal': str(journal), 'golden': str(golden_record)},
    'errors': [],
}

try:
    with (output / 'isolation-harness.log').open('x') as log:
        result = subprocess.run(command, cwd=root, stdin=subprocess.DEVNULL,
                                stdout=log, stderr=subprocess.STDOUT)
    report['isolation_harness_exit'] = result.returncode
    if result.returncode:
        report['errors'].append('standard harness/isolation command exited ' + str(result.returncode))

    # Inspect this even after exit 1: the isolation wrapper restores on normal
    # harness failure and preserves unsafe/interrupted state for reconciliation.
    state = json.loads(journal.read_text()) if journal.is_file() else {}
    profiles = state.get('profiles', [])
    restored = (state.get('status') == 'restored' and len(profiles) == 1 and
                profiles[0].get('agent') == 'ours156' and
                profiles[0].get('phase') == 'restored')
    report['profile_journal_status'] = state.get('status', 'missing')
    report['profiles_restored'] = restored
    if not restored:
        report['errors'].append('profile restoration journal is not restored; preserve evidence/backups')

    steps_path = harness_output / 'steps.json'
    if steps_path.is_file():
        steps = json.loads(steps_path.read_text()).get('steps', [])
        report['nonpassing_steps'] = [step for step in steps if step.get('outcome') != 'pass']
        report['standard_steps_all_pass'] = bool(steps) and not report['nonpassing_steps']
        if not report['standard_steps_all_pass']:
            report['errors'].append('standard scenario contains missing or nonpassing steps (including quit)')
    else:
        report['errors'].append('standard steps.json missing')

    ours = harness_output / 'launch-1/record.jsonl'
    report['paths']['ours'] = str(ours)
    if ours.is_file():
        # Keep the complete original transcript and the shared default diff.
        raw_text = output / 'diff-launch-1.txt'
        raw_json = output / 'diff-launch-1.json'
        with raw_text.open('x') as stream:
            raw = subprocess.run([sys.executable, str(root / 'tests/recorder/diff.py'),
                                  str(golden_record), str(ours), '--json', str(raw_json)],
                                 cwd=root, stdin=subprocess.DEVNULL, stdout=stream,
                                 stderr=subprocess.STDOUT)
        report['raw_diff_exit'] = raw.returncode
        report['paths']['raw_diff'] = str(raw_text)
        report['raw_diff_header'] = raw_text.read_text().splitlines()[:2]
        if raw.returncode:
            report['errors'].append('shared default diff failed')
        boundary = output / 'boundary-launch-1'
        with (output / 'boundary-diff.log').open('x') as stream:
            scoped = subprocess.run([sys.executable, str(root / 'arc/browser/ui_boundary_diff.py'),
                                     '--golden', str(golden_record), '--ours', str(ours),
                                     '--out', str(boundary)], cwd=root,
                                    stdin=subprocess.DEVNULL, stdout=stream,
                                    stderr=subprocess.STDOUT)
        report['ui_boundary_diff_exit'] = scoped.returncode
        report['paths']['ui_diff'] = str(boundary / 'ui.txt')
        if scoped.returncode or not (boundary / 'ui.json').is_file():
            report['errors'].append('UI boundary comparison failed or produced no report')
        else:
            diff = json.loads((boundary / 'ui.json').read_text())
            sections = ('missing', 'extra', 'return_mismatch', 'arg_mismatch',
                        'count_mismatch', 'internal', 'internal_ours')
            clean = (not any(diff.get(key) for key in sections) and
                     diff.get('callback_sequence', {}).get('first_divergence') is None)
            report['ui_boundary_clean'] = clean
            report['ui_diff_header'] = (boundary / 'ui.txt').read_text().splitlines()[:2]
            if not clean:
                report['errors'].append('UI-scoped transcript differences remain')
    else:
        report['errors'].append('standard raw transcript missing')
except Exception as error:
    report['errors'].append(type(error).__name__ + ': ' + str(error))
finally:
    # An exception must never hide the isolation wrapper's restoration status.
    try:
        state = json.loads(journal.read_text()) if journal.is_file() else {}
        profiles = state.get('profiles', [])
        report['profile_journal_status'] = state.get('status', 'missing')
        report['profiles_restored'] = (state.get('status') == 'restored' and
            len(profiles) == 1 and profiles[0].get('agent') == 'ours156' and
            profiles[0].get('phase') == 'restored')
        if not report['profiles_restored'] and not any('restoration' in error for error in report['errors']):
            report['errors'].append('profile restoration unconfirmed; inspect journal before further runs')
    except Exception as error:
        report['profiles_restored'] = False
        report['errors'].append('profile journal unreadable: ' + str(error))
    try:
        after = snapshot()
        save('evidence-after.json', after)
        drift = [path for path in sorted(before.keys() | after.keys()) if before.get(path) != after.get(path)]
        report['evidence_drift'] = drift
        report['evidence_unchanged'] = not drift
        if drift:
            report['errors'].append('hashed source/golden/recorder/packaged evidence changed')
    except Exception as error:
        report['evidence_unchanged'] = False
        report['errors'].append('after-snapshot failed: ' + str(error))
    report['finished_utc'] = datetime.datetime.now(datetime.timezone.utc).isoformat()
    report['status'] = 'pass' if not report['errors'] else 'fail'
    save('report.json', report)
    # The standard harness creates driver artifacts too; keep all replay output
    # private after the process has exited, without modifying its contents.
    for path in output.rglob('*'):
        if path.is_symlink():
            continue
        path.chmod(0o700 if path.is_dir() else 0o600)

print('Renderer UI replay status: ' + report['status'])
print('Profile journal: ' + str(journal))
print('Report: ' + str(output / 'report.json'))
for key in ('raw_diff_header', 'ui_diff_header'):
    for line in report.get(key, []):
        print(key + ': ' + line)
raise SystemExit(0 if report['status'] == 'pass' else 1)
PY
