#!/bin/bash
# Compare metadata-shaped native extension records on stock and current engines.
set -euo pipefail
E="$(cd "$(dirname "$0")/../.." && pwd)"
if [ "${1:-}" != --locked ]; then exec "$E/arc/tools/locked.sh" bash "$0" --locked; fi
cd "$E"
probe_dir="$(mktemp -d /tmp/arccore-extension-values.XXXXXX)"
trap 'rm -rf "$probe_dir"' EXIT
# Native record ABI includes Chromium's libc++ vector layout (__Cr), so compile
# and link with the same standard library as the framework.
python3 - "$probe_dir" <<'BUILD'
import os,shlex,subprocess,sys
from pathlib import Path
root=Path.cwd();cwd=root/'chromium/src/out/Base';tmp=Path(sys.argv[1])
env={**os.environ,'PATH':str(root/'depot_tools')+':'+os.environ['PATH']}
p=subprocess.Popen(['siso','query','commands','-C','.','obj/arc/arc_core/arc_browser.o'],cwd=cwd,env=env,stdout=subprocess.PIPE,stderr=subprocess.DEVNULL)
command=None
for line in p.stdout:
 text=line.decode(errors='replace')
 if ' -c ../../arc/api/arc_browser.mm ' in text:command=shlex.split(text);break
p.stdout.close();p.terminate();p.wait()
if not command:raise SystemExit('compile command not found')
args=[];i=0
while i<len(command):
 value=command[i]
 if value in ('-o','-MF'):i+=2;continue
 if value in ('-MMD',):i+=1;continue
 args.append(value.replace('../../arc/api/arc_browser.mm','../../arc/browser/extensions_values_probe.mm'));i+=1
args+=['-o',str(tmp/'probe.o')]
subprocess.run(args,cwd=cwd,check=True)
subprocess.run([command[0],'-fuse-ld=lld','-isysroot','sdk/xcode_links/MacOSX26.0.sdk','-nostdlib++',str(tmp/'probe.o'),str(cwd/'obj/buildtools/third_party/libc++/libc++.a'),str(cwd/'obj/buildtools/third_party/libc++abi/libc++abi.a'),'-framework','AppKit','-framework','Foundation','-o',str(tmp/'probe')],cwd=cwd,check=True)
BUILD
original="$E/../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore"
ours="$E/chromium/src/out/Base/Chromium.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore"
for name in original ours; do
  framework="$original"; [ "$name" != ours ] || framework="$ours"
  out="$E/results/extensions-values/$name"; mkdir -p "$out"
  "$probe_dir/probe" "$framework" > "$out/values.json"
  env ARC_RECORD_PATH="$out/record.jsonl" ARC_EXTENSIONS_RECORDER_LIBRARY="$E/tests/recorder/ArcEngineRecorder.dylib" "$probe_dir/probe" "$framework" > "$out/recorded-values.json"
done
python3 - <<'PY'
import json,subprocess
from pathlib import Path
root=Path('results/extensions-values')
value_classes={'ArcExtensionInfo','ArcExtensionCommand','ArcExtensionRuntimeErrorStackFrame','ArcExtensionRuntimeError','ArcExtensionToolbarAction'}
for engine in ('original','ours'):
 raw=json.loads((root/engine/'values.json').read_text())
 recorded=json.loads((root/engine/'recorded-values.json').read_text())
 if raw!=recorded:raise SystemExit(engine+': recorder changed values or metadata')
 calls=[json.loads(line) for line in (root/engine/'record.jsonl').read_text().splitlines() if line]
 observed={(r['cls'],r['sel']) for r in calls if r.get('ev')=='call'}
 # These NSObject selectors are deliberately excluded by ArcEngineRecorder;
 # the raw/recorded snapshots still compare their results and type encodings.
 recorder_skips={'init','description'}
 expected={(name,selector) for name,methods in raw['metadata'].items() if name in value_classes for selector in methods if selector not in recorder_skips}
 if expected-observed:raise SystemExit(engine+': missing recorder coverage '+repr(sorted(expected-observed)))
 print(engine+': all '+str(len(expected))+' value methods recorded without changing observations')
for name in ('values.json','recorded-values.json'):
 a=json.loads((root/'original'/name).read_text());b=json.loads((root/'ours'/name).read_text())
 if a!=b:
  for k in a.keys()|b.keys():
   if a.get(k)!=b.get(k):print('DIFFERENCE',k,repr(a.get(k)),repr(b.get(k)))
  raise SystemExit(1)
 print(name+': exact match')
with (root/'diff.txt').open('w') as out:
 subprocess.run(['python3','tests/recorder/diff.py',str(root/'original/record.jsonl'),str(root/'ours/record.jsonl'),'--json',str(root/'diff.json')],stdout=out,check=True)
print('\n'.join((root/'diff.txt').read_text().splitlines()[:2]))
PY
