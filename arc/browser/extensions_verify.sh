#!/bin/bash
# Package-owned verification batch. Invoke only through arc/tools/locked.sh.
set -uo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$ROOT"
python3 - <<'PY' || exit $?
from pathlib import Path
import os, subprocess
lock_pid = int(Path('logs/arccore-build.lock/pid').read_text())
ancestor = os.getpid()
for _ in range(24):
    if ancestor == lock_pid:
        break
    if ancestor <= 1:
        raise SystemExit('parent-owned shared build/run lock required')
    ancestor = int(subprocess.check_output(['ps', '-p', str(ancestor), '-o', 'ppid='], text=True))
else:
    raise SystemExit('shared build/run lock owner not in ancestry')
PY
OUT="results/extensions-validation11"
SERVICE_CLI_OUT="results/extensions-service-probe/validation11-omnibox-field-trial-diagnostic"
if [ -e "$OUT" ]; then
  echo "refusing to overwrite $OUT" >&2
  exit 2
fi
for path in "$SERVICE_CLI_OUT"; do
  if [ -e "$path" ]; then
    echo "refusing to overwrite $path" >&2
    exit 2
  fi
done
mkdir -p "$OUT"
snapshot() {
  python3 - "$OUT" "$1" <<'PY'
from pathlib import Path
import hashlib, json, shutil, sys
out, stage = Path(sys.argv[1]), sys.argv[2]
paths = list(Path('arc/api').glob('arc_extensions_*.mm'))
paths += [Path(p) for p in (
    'arc/wp/extensions.gni', 'arc/browser/extensions_groups.mm',
    'arc/browser/extensions_values.h', 'arc/browser/extensions_context.h',
    'arc/browser/extensions_bridge.h', 'arc/browser/extensions_verify.sh',
    'arc/browser/extensions_service_probe.ts', 'arc/browser/extensions_service_probe.m',
    'arc/browser/extensions_service_probe.sh',
    'arc/browser/extensions_macho_fingerprint.py',
    'logs/wp/extensions-service-probe-build/ExtensionsServiceProbe.dylib',
    'tests/recorder/ArcEngineRecorder.dylib', 'tests/recorder/diff.py',
    'tests/launch-slice/run.ts', 'tests/scenarios/extensions-page-actions.ts')]
paths += list(Path('arc/browser/extensions_internal').glob('*'))
paths += [Path('arc/browser/extensions_internal_manifest.h')]
goldens = [p for scenario in ('launch-slice', 'extensions-page-actions')
           for p in Path(f'results/golden/arc-1.170-{scenario}').glob('launch-*/record.jsonl')]
paths += goldens
hashes = {str(p): hashlib.sha256(p.read_bytes()).hexdigest()
          for p in sorted(set(paths)) if p.is_file()}
if stage == 'before':
    for p in goldens:
        dest = out / 'golden' / p.relative_to('results/golden')
        dest.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(p, dest)
else:
    before = json.loads((out / 'snapshot-before.json').read_text())['sha256']
    changed = sorted(k for k in before.keys() | hashes.keys()
                     if before.get(k) != hashes.get(k))
    (out / 'changed-paths.json').write_text(json.dumps(changed, indent=2))
    if changed:
        print('Source/tool/golden snapshot changed; retained raw evidence:', ', '.join(changed))
(out / f'snapshot-{stage}.json').write_text(json.dumps({'sha256': hashes}, indent=2))
if stage != 'before' and changed:
    raise SystemExit(1)
PY
}
arc/browser/extensions_service_probe.sh build > "$OUT/helper-build.log" 2>&1 || exit $?
snapshot before || exit $?
reset_disposable_data() {
  python3 - "$OUT" "$1" "$2" <<'PY'
from pathlib import Path
from datetime import datetime, timezone
import json, shutil, subprocess, sys
out, agent, reason = Path(sys.argv[1]), sys.argv[2], sys.argv[3]
assert agent in ('trace156', 'ours156')
app = (Path.cwd() / f'../arc/build/ArcModAgent-{agent}.app').resolve()
plist = app / 'Contents/Info.plist'
def value(key):
    return subprocess.check_output(['/usr/libexec/PlistBuddy', '-c', f'Print :{key}', str(plist)], text=True).strip()
assert value('ArcModAgentSession') == agent
assert value('CFBundleIdentifier') == f'company.thebrowser.Browser.mod.agent.{agent}'
commands = subprocess.check_output(['ps', '-axo', 'command='], text=True).splitlines()
assert not any(c.startswith(str(app) + '/') for c in commands), 'owned app/helper remains running; refusing data reset'
data = Path.home() / f'Library/Application Support/ArcModAgent-{agent}'
assert not data.is_symlink(), 'refusing symlinked disposable data'
existed = data.exists()
if existed:
    shutil.rmtree(data)
assert not data.exists()
with (out / 'disposable-data-resets.jsonl').open('a') as journal:
    journal.write(json.dumps({'at': datetime.now(timezone.utc).isoformat(), 'agent': agent,
        'app': str(app), 'data': str(data), 'reason': reason, 'existed': existed,
        'noAppOrHelpers': True, 'dataAbsent': True,
        'scope': 'disposable filesystem reset after exit; not public API cleanup certification'}) + '\n')
PY
}
python3 arc/tools/gen_api_table.py || exit $?
# Another worker can have compiled a TU while it was being edited. Under this
# lock, mark our frozen TUs dirty so this build resolves their current contents.
touch arc/api/arc_extensions_*.mm arc/browser/extensions_groups.mm || exit $?
ARC_BUILD_FAIL_FAST=1 ./build-babysit.sh > "$OUT/build.log" 2>&1 || exit $?
./package-engine.sh ours156 > "$OUT/package.log" 2>&1 || exit $?
# No public-install operation changed; batch9 already retains its matching
# enable-policy restriction. This batch targets the new valid omnibox range.
service_status=0
reset_disposable_data trace156 before-command-line-phase || exit $?
reset_disposable_data ours156 before-command-line-phase || exit $?
arc/browser/extensions_service_probe.sh run --out "$SERVICE_CLI_OUT" --diagnostic-disable-field-trial-config --fixture-mode=command-line --browser-omnibox --agents trace156,ours156 --port 9458 > "$OUT/service-cli.log" 2>&1
cli_status=$?
echo "command-line service exit $cli_status"
scenario_status=0
for scenario in launch-slice extensions-page-actions; do
  reset_disposable_data ours156 "before-$scenario" || exit $?
  bun tests/launch-slice/run.ts --app "$ROOT/../arc/build/ArcModAgent-ours156.app" --out "$OUT/$scenario" --record --scenario "$scenario" > "$OUT/$scenario.log" 2>&1
  status=$?
  echo "$scenario harness exit $status"
  if [ "$status" -ne 0 ]; then scenario_status=$status; fi
  for golden in "$OUT"/golden/arc-1.170-"$scenario"/launch-*/record.jsonl; do
    launch="$(basename "$(dirname "$golden")")"
    ours="$OUT/$scenario/$launch/record.jsonl"
    if [ ! -f "$ours" ]; then
      echo "$scenario $launch: missing transcript"
      scenario_status=1
      continue
    fi
    python3 tests/recorder/diff.py "$golden" "$ours" --json "$OUT/$scenario/diff-$launch.json" > "$OUT/$scenario/diff-$launch.txt" || scenario_status=1
    echo "$scenario $launch"
    head -n 2 "$OUT/$scenario/diff-$launch.txt"
  done
done
snapshot after || exit $?
if [ "$service_status" -ne 0 ] || [ "$cli_status" -ne 0 ] || [ "$scenario_status" -ne 0 ]; then exit 1; fi
