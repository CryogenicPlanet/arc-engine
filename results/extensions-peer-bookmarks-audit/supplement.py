from pathlib import Path
import datetime,hashlib,json,subprocess,sys

ROOT=Path('/Users/cryogenicplanet/general/arc-engine');OUT=Path(__file__).resolve().parent
with (OUT/'supplement-exclusive.guard').open('x') as f:f.write('Exclusive compiler/policy supplement.\n')
inputs={}
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def snap(p,d):
    source=ROOT/p;target=OUT/d;assert not target.exists()
    before=source.stat();data=source.read_bytes();target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(data)
    after=source.stat();digest=sha(target)
    assert before.st_size==after.st_size==len(data) and before.st_mtime_ns==after.st_mtime_ns and sha(source)==digest
    inputs[d]={'source':p,'sha256':digest,'size':len(data),'mtime_ns':after.st_mtime_ns,
               'mtime_utc':datetime.datetime.fromtimestamp(after.st_mtime_ns/1e9,datetime.UTC).isoformat()}
for p in ['tests/scenarios/bookmarks-public-api.ts','tests/scenarios/bookmarks-public-api-driver.m',
          'tests/scenarios/bookmarks-public-api-raw.ts']:
    snap(p,'current-canonical-observation/'+p)
for side,p in [('golden','results/golden/arc-1.170-bookmarks-public-api'),
               ('ours','results/ours/bookmarks-public-api')]:
    snap(p+'/bookmarks-public-api-driver/BookmarksPublicAPIDriver.dylib',
         'peer-driver-binaries/'+side+'/BookmarksPublicAPIDriver.dylib')
for p in ['logs/wp/profile.batch11/bookmark-fixture/manifest.json',
          'logs/wp/profile.batch11/bookmark-fixture/build.py',
          'logs/wp/profile.batch11/bookmark-fixture/compile.log']:
    snap(p,'ancestor-fixture-metadata/'+Path(p).name)
command=[sys.executable,'-B',str(OUT/'current-shared-policy/tests/recorder/diff.py'),
         str(OUT/'inputs/golden/launch-1/record.jsonl'),str(OUT/'inputs/ours/launch-1/record.jsonl'),
         '--json',str(OUT/'official-policy-reproduction.json')]
r=subprocess.run(command,capture_output=True,text=True)
assert r.returncode==0 and not r.stderr
(OUT/'official-policy-reproduction.txt').write_text(r.stdout)
(OUT/'official-policy-reproduction.stderr.txt').write_text(r.stderr)
same=json.loads((OUT/'official-policy-reproduction.json').read_text())==json.loads((OUT/'official/diff-launch-1.json').read_text())
assert same
assert r.stdout.splitlines()[:2]==(OUT/'official/diff-launch-1.txt').read_text().splitlines()[:2]
for d,m in inputs.items():assert sha(ROOT/m['source'])==sha(OUT/d)==m['sha256']
report={'inputs':inputs,'all_sources_and_copies_reverified':True,
 'official_reproduction_command':command,'official_reproduction_exit':r.returncode,
 'official_json_equal':same,'official_first_two_lines_equal':True,
 'official_reproduction_comparator_sha256':sha(OUT/'current-shared-policy/tests/recorder/diff.py'),
 'compiler_policy_observation':{
  'canonical_current_command':'xcrun clang -dynamiclib -fobjc-arc -Wall -Wextra -Werror -framework Foundation -framework AppKit tests/scenarios/bookmarks-public-api-driver.m -o <run-owned library>',
  'run_compile_logs_empty':all((OUT/'inputs'/s/'bookmarks-public-api-driver/compile.log').stat().st_size==0 for s in ['golden','ours']),
  'run_steps_record_compile_pass':True,
  'exact_run_compile_argv_or_compiler_identity_recorded':False,
  'ancestor_fixture_compile_log_is_not_canonical_run_compile_log':True,
  'run_provenance_source_fields_are_ancestor_fixture_references':True,
  'current_canonical_sources_are_observation_only_not_historical_source_freeze':True,
  'extension_TU_framework_source_epoch_proven':False}}
(OUT/'compiler-policy-supplement.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({k:v for k,v in report.items() if k!='inputs'},indent=2))
