"""Pure AST-expression/path/hash controls: no executor import, construction or invocation."""
from pathlib import Path
from types import SimpleNamespace
import ast,json,hashlib,datetime
ROOT=Path('/Users/cryogenicplanet/general/arc-engine');OUT=Path(__file__).parent
V1=Path('/tmp/arccore-sessions-adopt-source-only.py');V2=Path('/tmp/arccore-sessions-adopt-source-only-v2.py');RUNNER=Path('/tmp/arccore-sessions-adoption-runtime-v1.py');RUNNER_SHA='0df88d889d64f38d46dc06216cc0c2ba87a04167854b05c1f566649a1a73a8a0';PINS=ROOT/'results/sessions/source-only-runtime-inputs-20261023T170004Z/pins.json'
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
def guard(path):
 tree=ast.parse(path.read_text());target=next(n for n in ast.walk(tree)if isinstance(n,ast.Assert)and isinstance(n.msg,ast.Constant)and n.msg.value=='runtime v1 missing from frozen table');return compile(ast.Expression(target.test),str(path)+':pin-identity-only','eval')
def pin_path(key):
 p=Path(key);return p.resolve()if p.is_absolute()else(ROOT/p).resolve()
# Evaluate only the exact isolated boolean guard extracted by AST, using a pure
# local pin_path helper. No executor definitions or code are imported/executed.
def accepted(expression,paths):return eval(expression,{'__builtins__':{'any':any},'self':SimpleNamespace(pin_path=pin_path),'RUNTIME_RUNNER':RUNNER,'RUNTIME_RUNNER_SHA':RUNNER_SHA,'raw':{'paths':paths}})
assert sha(V1)=='fb482e50efcd33fb7dcfacf353ecdcc17ce968815946801a89181ac51436484c';assert sha(V2)=='d6dea94745a287eb94b10be203b18793b6224edc53d55252c9d8f6529a5b7f88';assert sha(RUNNER)==RUNNER_SHA;assert RUNNER.resolve()==Path('/private/tmp/arccore-sessions-adoption-runtime-v1.py');assert RUNNER!=RUNNER.resolve()
v1=guard(V1);v2=guard(V2);actual=json.loads(PINS.read_text())['paths'];tests=[
 ('actual-frozen32-v1-reproduces-false',not accepted(v1,actual)),
 ('actual-frozen32-v2-accepts',accepted(v2,actual)),
 ('tmp-spelling-positive',accepted(v2,{str(RUNNER):RUNNER_SHA})),
 ('private-tmp-spelling-positive',accepted(v2,{str(RUNNER.resolve()):RUNNER_SHA})),
 ('wrong-hash-negative',not accepted(v2,{str(RUNNER):'0'*64})),
 ('wrong-existing-path-negative',not accepted(v2,{str(V1):RUNNER_SHA})),
 ('missing-runner-negative',not accepted(v2,{})),
 ('mixed-wrong-alias-hashes-negative',not accepted(v2,{str(RUNNER):'0'*64,str(RUNNER.resolve()):'1'*64})),
 ('v1-copy-still-exact',V1.read_bytes()==(ROOT/'results/sessions/proposals/source-only-adoption-transaction/transaction.py').read_bytes()),
 ('v2-copy-exact',V2.read_bytes()==(OUT/'transaction.py').read_bytes()),
 ('real-root-already-canonical',ROOT==ROOT.resolve()),
]
assert all(ok for _,ok in tests),tests
report={'scope':'Pure AST extracted single guard + actual path/hash controls only; no executor import/constructor/invocation/lock/live writes/runtime/apps/build/queue','v1SHA256':sha(V1),'v2SHA256':sha(V2),'runtimeRunnerSHA256':sha(RUNNER),'frozenPinsSHA256':sha(PINS),'actualResolvedRunner':str(RUNNER.resolve()),'tests':[{'name':n,'pass':bool(ok)}for n,ok in tests],'finishedUTC':datetime.datetime.now(datetime.timezone.utc).isoformat()};(OUT/'pin-identity-results.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'pass':True,'controls':len(tests),'v2SHA256':sha(V2)}))
