#!/bin/bash
# Package the current ArcCore build, launch Arc's app on it with a fresh agent profile, and report:
# whether it's alive, the unimplemented engine calls in order, and the newest crash (Sentry or Apple).
#   ./try-engine.sh [seconds=25] [slug=ours156] [--keep]   (--keep leaves the app running)
set -uo pipefail
E="$(cd "$(dirname "$0")" && pwd)"
WAIT="${1:-25}"; SLUG="${2:-ours156}"; KEEP="${3:-}"
APP="ArcModAgent-$SLUG"
LOG="$E/../arc/build/$APP-run.log"
pkill -TERM -f "$APP.app/Contents/MacOS/Arc" 2>/dev/null; sleep 1
"$E/package-engine.sh" "$SLUG" >/dev/null 2>&1 || { echo "package failed"; exit 1; }
rm -rf "$HOME/Library/Application Support/$APP"
: > "$LOG"
STAMP="$(mktemp)"
(cd "$E/../arc" && tools/run-mod.sh --agent "$SLUG" --background --cdp-port 9334 >/dev/null 2>&1)
sleep "$WAIT"
if pgrep -f "$APP.app/Contents/MacOS/Arc" >/dev/null; then
  echo "ALIVE after ${WAIT}s; helpers: $(pgrep -f "$APP.app/Contents/Frameworks" | wc -l | tr -d ' ')"
else
  echo "EXITED within ${WAIT}s"
fi
echo "--- unimplemented (in call order, ArcCpuInfo omitted)"
grep -o 'unimplemented: .*' "$LOG" | grep -v ArcCpuInfo | sed 's/unimplemented: //' | head -60
echo "--- fatal lines"
grep -E 'FATAL|Fatal error|Check failed' "$LOG" | cut -c1-240 | head -5
echo "--- newest crash since launch"
ENV=$(find "$HOME/Library/Caches/io.sentry" -name '*.json' -newer "$STAMP" 2>/dev/null | sort | tail -1)
[ -n "$ENV" ] && python3 - "$ENV" <<'EOF'
import json, sys
for line in open(sys.argv[1], 'rb').read().split(b'\n'):
    try: d = json.loads(line)
    except Exception: continue
    for v in (d.get('exception') or {}).get('values', []):
        print('sentry:', v.get('type'), '|', (v.get('value') or '')[:200].replace('\n', ' '))
        for f in v.get('stacktrace', {}).get('frames', [])[::-1][:8]:
            print('   ', (f.get('package') or '').split('/')[-1], f.get('function'), f.get('instruction_addr'))
EOF
IPS=$(find "$HOME/Library/Logs/DiagnosticReports" -name 'Arc-*.ips' -newer "$STAMP" 2>/dev/null | sort | tail -1)
[ -n "$IPS" ] && python3 - "$IPS" <<'EOF'
import json, sys
txt = open(sys.argv[1]).read(); first, rest = txt.split('\n', 1); d = json.loads(rest)
print('ips:', d.get('exception'))
imgs = d['usedImages']; th = d['threads'][d['faultingThread']]
for fr in th['frames'][:10]:
    print('   ', imgs[fr['imageIndex']].get('name'), hex(fr['imageOffset']), fr.get('symbol', ''))
EOF
[ "$KEEP" = "--keep" ] || pkill -TERM -f "$APP.app/Contents/MacOS/Arc" 2>/dev/null
true
