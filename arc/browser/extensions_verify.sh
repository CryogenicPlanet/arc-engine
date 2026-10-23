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
OUT="results/extensions-validation14"
SERVICE_CLI_OUT="results/extensions-service-probe/validation14-startup-init-delegate-repeat"
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
    'arc/browser/extensions_startup_init.patch',
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
# Require the reviewed product/helper pair before compilation or app work.
python3 - <<'GATE' || exit $?
from pathlib import Path
import hashlib
expected={'arc/api/arc_extensions_browser.mm': '03024142806d33fee96cfcf03b7ab1876556c2cdef9ac48c9acc98e81335e46e', 'arc/api/arc_extensions_context.mm': '475fedd79f2433006fd6d173ff18cdb411eb8247ac38a4832abc0e2fdc0dc30d', 'arc/api/arc_extensions_internal.mm': 'cf61b5c5baeb28691229b5f4c61ed60a0f2993d3a9c383be1952e2d305793db3', 'arc/api/arc_extensions_values.mm': '1a3994ea1ff0a38b7cf696a3d467b14da948479eb9ae418fcbb06db0dee33845', 'arc/browser/extensions_bridge.h': 'b541fadc210b57d9fdfc18205181904ce4344051182ac32fad5fd89811fba39f', 'arc/browser/extensions_context.h': '9a77800fb0751b6322b982df5a6f5a215a82c1f2e9c347b19a786fe14b3d5894', 'arc/browser/extensions_groups.mm': '4779348d2122a9f811f42474de2df1c8a60ad3ec0ea1689c44c6ab3f586812ab', 'arc/browser/extensions_internal/BUILD.gn': '3846d4debc646efd0d543c1ca7172624eaec3d8d2f1c90cf845f492f3fdf2988', 'arc/browser/extensions_internal/background.js': 'eef3ebab7af63e3a72df06882a8185732d3c6bf71c9575bd7a4ea9785baf01fd', 'arc/browser/extensions_internal/manifest.json': '0badd5b743eaf75d894b8abf1a8ac29ba4233123bd48217bf9ebf6b2ed6f6bd9', 'arc/browser/extensions_internal_manifest.h': '5044b52a016b9b47ea2fab9b74ec9105c0dfed811d47c1b6cf4aba40360cb7e6', 'arc/browser/extensions_values.h': '0626fc241700feebf62592b86e1659c644e7cf2a6a90f8d28e19187a484d2d74', 'arc/wp/extensions.gni': '9b7531a27300061d5b67aa9cc35371f078ef2e7ad8578a18fc2338f7d5368a8a', 'arc/browser/extensions_service_probe.m': 'bc3adca6b91dd92ac65046e63956109c0b2faff75db7f82be0685a30bb9c519c', 'arc/browser/extensions_service_probe.ts': 'ac5e55c85b27758222e69e25866340c6eae53f965408cb4f42a47a174d4f6eb9'}
for path, pinned in expected.items():
    assert hashlib.sha256(Path(path).read_bytes()).hexdigest()==pinned, f"reviewed validation13 input required: {path}"
GATE
# Apply this reviewed owned-file change only after the shared lock is held.
python3 - "$OUT" <<'APPLY_STARTUP' || exit $?
from pathlib import Path
from datetime import datetime, timezone
import hashlib, json, subprocess, sys
out=Path(sys.argv[1]); browser=Path('arc/api/arc_extensions_browser.mm')
patch=Path('arc/browser/extensions_startup_init.patch')
pre='03024142806d33fee96cfcf03b7ab1876556c2cdef9ac48c9acc98e81335e46e'
post='99496aeb55684aac98f6b83bed1a8505a9e89ac06dbb107bac7fc50ccc2dd728'
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
assert digest(patch)=='421323820a53dbb167a69519d00fbdc2bda51730bcbf8d8e6c52254cbdc5d0ec', 'reviewed startup patch changed'
original=browser.read_bytes(); assert hashlib.sha256(original).hexdigest()==pre, 'startup preimage changed'
(out/'startup-browser-before.mm').write_bytes(original)
subprocess.run(['git','apply','--check','--whitespace=error',str(patch)],check=True)
subprocess.run(['git','apply',str(patch)],check=True)
assert digest(browser)==post, 'startup postimage differs from reviewed source; no unrelated bytes overwritten'
with (out/'startup-official-check.log').open('w') as log:
    result=subprocess.run(['arc/tools/check-file.sh',str(browser)],stdout=log,stderr=subprocess.STDOUT)
receipt={'at':datetime.now(timezone.utc).isoformat(),'before':pre,'after':post,'patch':digest(patch),
         'officialCheckExit':result.returncode,'appliedUnderSharedLock':True,'rolledBack':False}
if result.returncode:
    assert digest(browser)==post, 'source changed during check; refusing to overwrite another writer'
    browser.write_bytes(original); assert digest(browser)==pre
    receipt['rolledBack']=True
(out/'startup-application.json').write_text(json.dumps(receipt,indent=2))
if result.returncode: raise SystemExit(result.returncode)
print('startup patch applied under shared lock; official check0; browser '+post)
APPLY_STARTUP
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
# enable-policy restriction. This batch targets same genuine app-delegate repetition during a real popup.
service_status=0
reset_disposable_data trace156 before-command-line-phase || exit $?
reset_disposable_data ours156 before-command-line-phase || exit $?
arc/browser/extensions_service_probe.sh run --out "$SERVICE_CLI_OUT" --diagnostic-disable-field-trial-config --fixture-mode=command-line --browser-delegate-repeat --agents trace156,ours156 --port 9458 > "$OUT/service-cli.log" 2>&1
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
