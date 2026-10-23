"""Private offline references only; no engine/build/lock/production changes."""
from pathlib import Path
import datetime,hashlib,json,os,platform,subprocess,sys
ROOT=Path('/Users/cryogenicplanet/general/arc-engine');OUT=Path(__file__).absolute().parent
NEW=OUT/'accepted-tool-root';OLD=OUT/'old-tool-root';PINS={}
EXPECTED={'tests/recorder/diff.py':'24437bed30fddc8b94ab5c9dcc7cd20e755b06c0b05a4d8620e487d3c7f612cf','tests/recorder/omnibox_receivers.py':'3af958f677a96eb982f8ede4486e272a1f2bc928a87cd2455f75fef0c55bcd86','tests/recorder/google_q.py':'67fb19cf77f3903461543e1be18b7352c7b3999552cfd672f629f8699eaea28c','tests/recorder/download_identity.py':'5318e64f44d9f7549227ab893185c083a34dd835b01be0ba5552d0940631ccea','arc/tools/arccore-1.170.0-objc.tsv':'9ba3d4750f15e80ca6836966af5ec50523ccfe5e0ed3c22faa31f8b347492cf5','tests/recorder/app_callbacks.inc':'52b3687bd7f574acb34f8c3c5fcf6750e0fc044c9bf159a6971531a952830748'}
V4='results/sessions/adoption-source-only-parent-v4';F8='f8df579e6ea23b4eb531f357fdfe0460fa57b563daf44f9d6858c73abf84b184'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def stamp(p):
 p=Path(p);s=p.stat();return {'sha256':sha(p),'bytes':s.st_size,'inode':s.st_ino,'mtimeNs':s.st_mtime_ns}
def freeze(name,dest=None,expected=None):
 src=ROOT/name
 if src.is_symlink():raise RuntimeError('symlink input '+name)
 before=stamp(src)
 if expected is not None and before['sha256']!=expected:raise RuntimeError('expected pin drift '+name)
 if dest is None:dest=OUT/'inputs'/name
 dest.parent.mkdir(parents=True,exist_ok=True)
 with dest.open('xb')as f:f.write(src.read_bytes())
 assert stamp(src)==before and sha(dest)==before['sha256'] and dest.stat().st_ino!=before['inode'],name
 if name not in PINS:PINS[name]={'origin':before,'copies':[]}
 PINS[name]['copies'].append(str(dest.relative_to(OUT)))
 return dest
def check():
 for name,v in PINS.items():
  assert stamp(ROOT/name)==v['origin'],'origin drift '+name
  for copy in v['copies']:assert sha(OUT/copy)==v['origin']['sha256'],'copy drift '+copy
for name,h in EXPECTED.items():freeze(name,NEW/name,h)
for name in ['arc/tools/arccore-1.170.0-objc.tsv','tests/recorder/app_callbacks.inc']:freeze(name,OLD/name,EXPECTED[name])
freeze(V4+'/inputs/'+F8,OLD/'tests/recorder/diff.py',F8)
for name in ['arc/DIFF-ALLOWLIST.md','arc/ARCHITECTURE.md',V4+'/inputs.expected.json',V4+'/input-snapshots.json',V4+'/status.json',V4+'/frameworks.before.json',V4+'/frameworks.after.json','results/sessions/canonical-source-only-adoption-review/review.md','results/sessions/canonical-source-only-adoption-review/review.json','results/sessions/canonical-source-only-adoption-review/diff-first-two-lines.txt']:freeze(name)
prior=json.loads((ROOT/'results/sessions/canonical-source-only-adoption-review/review.json').read_text())['inputPins']
expected=json.loads((ROOT/V4/'inputs.expected.json').read_text())['paths'];pairs=[]
for scenario,n in [('launch-slice',1),('launch-slice',2),('sessions-public-api',1),('sessions-tab-restart',1)]:
 label=scenario+'-'+str(n);base=V4+'/'+scenario
 row={'label':label,'scenario':scenario,'launch':n,'golden':V4+f'/goldens/{scenario}/launch-{n}/record.jsonl','ours':base+f'/launch-{n}/record.jsonl','oldText':base+f'/diff-launch-{n}.txt','oldJSON':base+f'/diff-launch-{n}.json'}
 for key in ['golden','ours','oldText','oldJSON']:
  name=row[key];assert sha(ROOT/name)==prior[name]['sha256'],'prior audit input drift '+name;freeze(name,expected=prior[name]['sha256'])
 assert sha(ROOT/row['golden'])==expected[f'results/golden/arc-1.170-{scenario}/launch-{n}/record.jsonl']
 pairs.append(row)
for scenario in ['launch-slice','sessions-public-api','sessions-tab-restart']:
 for n in ['steps.json','coverage.json']:freeze(V4+'/'+scenario+'/'+n)
 for name in [V4+'/'+scenario+'.harness.log']:freeze(name)
 for p in sorted((ROOT/V4/scenario).rglob('*.json')):
  name=str(p.relative_to(ROOT))
  if name not in PINS and 'launch-' not in p.name and 'diff-' not in p.name:freeze(name)
check()
(OUT/'frozen-inputs.json').write_text(json.dumps({'preparedUTC':datetime.datetime.now(datetime.timezone.utc).isoformat(),'inputs':PINS,'pairs':pairs,'metadataLimit':'v4 expected-map did not pin TSV/callback metadata. Copied accepted-current metadata hashes match the independent accepted reference; oldf8 same-metadata replay below tests effective output compatibility, not historical metadata provenance.'},indent=2)+'\n')
env=dict(os.environ);env.update(PYTHONPATH='',PYTHONDONTWRITEBYTECODE='1');env.pop('PYTHONSAFEPATH',None)
commands=[]
for tool,toolroot in [('old-f8-control',OLD),('accepted-first8',NEW)]:
 for row in pairs:
  check();dest=OUT/tool/row['label'];dest.mkdir(parents=True,exist_ok=False)
  argv=[sys.executable,'-B','-v',str(toolroot/'tests/recorder/diff.py'),str(OUT/'inputs'/row['golden']),str(OUT/'inputs'/row['ours']),'--json',str(dest/'diff.json')]
  with (dest/'diff.txt').open('xb')as stdout,(dest/'imports.stderr').open('xb')as stderr:
   done=subprocess.run(argv,cwd=OUT,env=env,stdin=subprocess.DEVNULL,stdout=stdout,stderr=stderr)
  assert done.returncode==0,(tool,row['label'],done.returncode)
  json.loads((dest/'diff.json').read_text());imports=(dest/'imports.stderr').read_text()
  assert "import 'omnibox_receivers'"not in imports and "import 'google_q'"not in imports,'unexpected opt-in'
  if tool=='accepted-first8':assert str(NEW/'tests/recorder/download_identity.py')in imports,'copied mandatory helper source not evidenced'
  exact=None
  if tool=='old-f8-control':
   exact=(dest/'diff.txt').read_bytes()==(OUT/'inputs'/row['oldText']).read_bytes() and (dest/'diff.json').read_bytes()==(OUT/'inputs'/row['oldJSON']).read_bytes()
   assert exact,'old output not reproduced '+row['label']
  receipt={'tool':tool,'pair':row['label'],'argv':argv,'cwd':str(OUT),'exit':done.returncode,'defaultCLI':True,'fixtureImages':[],'omniboxReceivers':False,'omniboxGoogleQuery':False,'compareBoundaryArguments':False,'oldSavedTXTandJSONByteExact':exact,'outputs':{n:stamp(dest/n)for n in ['diff.txt','diff.json','imports.stderr']}}
  (dest/'command.json').write_text(json.dumps(receipt,indent=2)+'\n');commands.append(receipt)
check()
(OUT/'execution.json').write_text(json.dumps({'scope':'Offline4historicalv4pairs:4accepted default CLIs plus4old-f8 controls; not a freshengine/runtime','interpreter':{'executable':sys.executable,'version':sys.version,'implementation':platform.python_implementation()},'commands':commands,'allOriginAndCopyPinsUnchanged':True,'optInsUsed':False},indent=2)+'\n')
heads=[]
for row in pairs:
 p=OUT/'accepted-first8'/row['label']/'diff.txt';heads.extend([row['label'],*p.read_text().splitlines()[:2],''])
(OUT/'diff-first-two-lines.txt').write_text('\n'.join(heads))
print('PASS8offlineCLIcommands;4oldcontrolsbyteexact;allinputscopiesunchanged')
for row in pairs:print(row['label'],(OUT/'accepted-first8'/row['label']/'diff.txt').read_text().splitlines()[:2])
