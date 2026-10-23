#!/bin/bash
# Supplemental malformed-Web-Store-ID verification only. Product unchanged since validation11.
# Invoke only through arc/tools/locked.sh; this does not rerun canonical scenarios.
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
OUT="results/extensions-webstore-negative-validation12"
SERVICE_CLI_OUT="results/extensions-service-probe/validation12-webstore-negative"
for path in "$OUT" "$SERVICE_CLI_OUT"; do
  if [ -e "$path" ]; then
    echo "refusing to overwrite $path" >&2
    exit 2
  fi
done
mkdir -p "$OUT"
# This batch changes the helper only. Require every product source/header/asset
# to match the prior compiled canonical batch before using a current artifact.
python3 - <<'GATE' || exit $?
from pathlib import Path
import hashlib, json
before=json.loads(Path('results/extensions-validation11/snapshot-after.json').read_text())['sha256']
paths=[p for p in before if p.startswith('arc/api/arc_extensions_') or
       p in ('arc/wp/extensions.gni','arc/browser/extensions_groups.mm',
             'arc/browser/extensions_values.h','arc/browser/extensions_context.h',
             'arc/browser/extensions_bridge.h','arc/browser/extensions_internal_manifest.h') or
       p.startswith('arc/browser/extensions_internal/')]
assert len(paths)>=12, 'unexpectedly small compiled package inventory'
changed=[p for p in paths if hashlib.sha256(Path(p).read_bytes()).hexdigest()!=before[p]]
assert not changed, f'product changed since compiled validation11: {changed}'
expected_helpers={
    'arc/browser/extensions_service_probe.m':'c715415d56337dad54b0ecebfb34809de2a7d4610855afb5a22fdd59b47eea9b',
    'arc/browser/extensions_service_probe.ts':'a506bd8516a7b6df15d5a5faa6f330747ab149db4e0cf00eef305433c3ad774c',
}
for path, expected in expected_helpers.items():
    assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==expected, f'reviewed negative WebStore helper required: {path}'
GATE
arc/browser/extensions_service_probe.sh build > "$OUT/helper-build.log" 2>&1 || exit $?
snapshot() {
  python3 - "$OUT" "$1" <<'SNAPSHOT'
from pathlib import Path
import hashlib,json,sys
out,stage=Path(sys.argv[1]),sys.argv[2]
prior=json.loads(Path('results/extensions-validation11/snapshot-after.json').read_text())['sha256']
paths=list(prior)+['arc/browser/extensions_webstore_negative_verify.sh',
    'logs/wp/extensions-service-probe-build/ExtensionsServiceProbe.dylib']
hashes={p:hashlib.sha256(Path(p).read_bytes()).hexdigest() for p in sorted(set(paths))}
(out/f'snapshot-{stage}.json').write_text(json.dumps({'sha256':hashes},indent=2))
if stage=='after':
    before=json.loads((out/'snapshot-before.json').read_text())['sha256']
    changed=sorted(p for p in before.keys()|hashes.keys() if before.get(p)!=hashes.get(p))
    (out/'changed-paths.json').write_text(json.dumps(changed,indent=2))
    if changed:raise SystemExit(f'input drift: {changed}')
SNAPSHOT
}
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
reset_disposable_data trace156 before-webstore-negative-phase || exit $?
reset_disposable_data ours156 before-webstore-negative-phase || exit $?
# Explicit field-trial diagnostic environment, identical on both engines.
# Runner independently checks current built/package image identity and source
# freshness; provenance names actual binaries, not the old validation11 artifact.
arc/browser/extensions_service_probe.sh run --out "$SERVICE_CLI_OUT" --fixture-mode=command-line --webstore-negative --diagnostic-disable-field-trial-config --agents trace156,ours156 --port 9458 > "$OUT/service-cli.log" 2>&1
status=$?
echo "WebStore negative service exit $status"
snapshot after || exit $?
exit "$status"
