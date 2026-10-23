#!/bin/bash
# Build one snapshot and verify the core CDP scenarios under the shared lock.
set -euo pipefail
engine_root="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$engine_root"
if [ "${1:-}" != --locked ]; then
  exec arc/tools/locked.sh bash arc/browser/core_scenarios.sh --locked
fi
run_root="results/core-runtime/$(date -u +%Y%m%dT%H%M%SZ)"
mkdir -p "$run_root"
python3 arc/tools/gen_api_table.py
snapshot_sources() {
  python3 - "$1" <<'PY'
from pathlib import Path
import hashlib, json, re, sys
sources = re.findall(r'"//([^"]+)"', Path('arc/wp/core.gni').read_text().split('wp_core_deps')[0])
sources += ['arc/wp/core.gni', 'arc/api/arc_internal.h']
Path(sys.argv[1]).write_text(json.dumps({p: hashlib.sha256(Path(p).read_bytes()).hexdigest()
                                      for p in sorted(sources)}, indent=2) + '\n')
PY
}
while true; do
  snapshot_sources "$run_root/sources-before.json"
  if ! ./build-babysit.sh >> "$run_root/build.log" 2>&1; then
    mkdir -p "$run_root/build-failure"
    for attempt_log in logs/build-base.attempt-*.log; do
      if [ -f "$attempt_log" ]; then cp "$attempt_log" "$run_root/build-failure/"; fi
    done
    echo "Build failed; evidence: $run_root"
    exit 1
  fi
  snapshot_sources "$run_root/sources-after.json"
  if cmp -s "$run_root/sources-before.json" "$run_root/sources-after.json"; then break; fi
  echo "Core sources changed during build; rebuilding before validation."
done
python3 - "$run_root" <<'PY'
from pathlib import Path
import json, shutil, sys
out = Path(sys.argv[1]) / 'sources'
for name in json.loads((Path(sys.argv[1]) / 'sources-after.json').read_text()):
    destination = out / name
    destination.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(name, destination)
PY
./package-engine.sh ours156 > "$run_root/package.log" 2>&1
framework="chromium/src/out/Base/Chromium.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore"
shasum -a 256 "$framework" > "$run_root/framework.sha256"
git diff --no-ext-diff -- arc/api/arc_browser.mm arc/api/arc_web_contents.mm \
  arc/api/arc_navigation.mm arc/api/arc_internal.h arc/browser arc/wp/core.gni > "$run_root/source.diff"
for scenario in launch-slice tabs-navigation; do
  # The only profile removed is this disposable agent's isolated test profile.
  rm -rf "$HOME/Library/Application Support/ArcModAgent-ours156"
  if [ "$scenario" = launch-slice ]; then
    bun tests/launch-slice/run.ts --app "$engine_root/../arc/build/ArcModAgent-ours156.app" \
      --out "$run_root/$scenario" --record > "$run_root/$scenario.log" 2>&1 || true
  else
    bun tests/launch-slice/run.ts --app "$engine_root/../arc/build/ArcModAgent-ours156.app" \
      --out "$run_root/$scenario" --record --scenario "$scenario" > "$run_root/$scenario.log" 2>&1 || true
  fi
  for golden in results/golden/arc-1.170-"$scenario"/launch-*/record.jsonl; do
    launch="$(basename "$(dirname "$golden")")"
    transcript="$run_root/$scenario/$launch/record.jsonl"
    if [ ! -f "$transcript" ]; then
      echo "$scenario/$launch: missing transcript"
      continue
    fi
    python3 tests/recorder/diff.py "$golden" "$transcript" \
      --json "$run_root/$scenario/diff-$launch.json" > "$run_root/$scenario/diff-$launch.txt"
    echo "$scenario/$launch"
    head -2 "$run_root/$scenario/diff-$launch.txt"
  done
done
echo "Evidence: $run_root"
