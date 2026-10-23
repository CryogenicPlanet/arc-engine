#!/bin/bash
# Verify the exercised UI boundary against one freshly built engine snapshot.
# Shared engine build, packaging, agent data and app runs remain under one lock.
set -euo pipefail
E="$(cd "$(dirname "$0")/../.." && pwd)"
if [ "${1:-}" != --locked ]; then
  exec "$E/arc/tools/locked.sh" bash "$0" --locked "$@"
fi
shift
cd "$E"
reuse_built=false
if [ "${1:-}" = --reuse-built ]; then
  reuse_built=true
  shift
fi
if [ $# -eq 0 ]; then
  set -- dialogs dialogs-beforeunload context-menus cookies-storage-site-settings permissions-deny dialogs-auth-cancel
fi
for scenario in "$@"; do
  [[ "$scenario" =~ ^[a-z0-9-]+$ ]] || exit 2
  [ -f "results/golden/arc-1.170-$scenario/launch-1/record.jsonl" ] || {
    echo "No finalized golden: $scenario" >&2
    exit 2
  }
done
if [ "$reuse_built" = false ]; then
  python3 arc/tools/gen_api_table.py
  ./build-babysit.sh > logs/wp/ui.batch-build.log 2>&1 || {
    tail -8 logs/wp/ui.batch-build.log
    exit 1
  }
fi
python3 - <<'PY'
import datetime
import hashlib
import json
from pathlib import Path

engine = Path('chromium/src/out/Base/Chromium.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore')
sources = sorted(Path('arc/api').glob('arc_ui_*.mm')) + sorted(Path('arc/browser').glob('ui_*.h')) + [Path('arc/wp/ui.gni')]
late = [str(p) for p in sources if p.stat().st_mtime > engine.stat().st_mtime]
if late:
    raise SystemExit(f'UI sources newer than linked framework; rebuild required: {late}')
snapshot = {'framework': str(engine.resolve()),
            'linked_utc': datetime.datetime.fromtimestamp(engine.stat().st_mtime, datetime.timezone.utc).isoformat(),
            'sha256': hashlib.file_digest(engine.open('rb'), 'sha256').hexdigest(),
            'ui_sources': {str(p): hashlib.sha256(p.read_bytes()).hexdigest() for p in sources}}
Path('logs/wp/ui.snapshot.json').write_text(json.dumps(snapshot, indent=2) + '\n')
print('UI linked snapshot: ' + snapshot['linked_utc'] + ' ' + snapshot['sha256'], flush=True)
PY
./package-engine.sh ours156 > logs/wp/ui.batch-package.log 2>&1
for scenario in "$@"; do
  rm -rf "$HOME/Library/Application Support/ArcModAgent-ours156" "results/ours/$scenario"
  bun tests/launch-slice/run.ts --app "$E/../arc/build/ArcModAgent-ours156.app" \
    --out "results/ours/$scenario" --record --scenario "$scenario" \
    > "logs/wp/ui.$scenario.scenario.log" 2>&1 || true
  for golden in results/golden/arc-1.170-"$scenario"/launch-*/record.jsonl; do
    launch="$(basename "$(dirname "$golden")")"
    ours="results/ours/$scenario/$launch/record.jsonl"
    if [ ! -f "$ours" ]; then
      echo "$scenario $launch: no transcript; inspect logs/wp/ui.$scenario.scenario.log"
      continue
    fi
    python3 tests/recorder/diff.py "$golden" "$ours" \
      --json "results/ours/$scenario/diff-$launch.json" \
      > "results/ours/$scenario/diff-$launch.txt"
    echo "$scenario $launch"
    head -2 "results/ours/$scenario/diff-$launch.txt"
  done
done
