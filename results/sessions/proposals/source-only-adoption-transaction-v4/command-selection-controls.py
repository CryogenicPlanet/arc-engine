"""Pure isolated parser/error formatter controls; no executor import/invocation or query/compiler calls."""
from pathlib import Path
import ast,json,hashlib,re,shlex
ROOT=Path('/Users/cryogenicplanet/general/arc-engine');OUT=Path(__file__).parent;SCRIPT=Path('/tmp/arccore-sessions-adopt-source-only-v4.py');RAW=ROOT/'results/sessions/proposals/source-only-adoption-transaction-v3/run-parent-v3/helper-query.log';sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest();tree=ast.parse(SCRIPT.read_text());functions=[n for n in tree.body if isinstance(n,ast.FunctionDef)and n.name in ['select_compile_command','failure_text']];assert len(functions)==2;ns={'re':re,'shlex':shlex};exec(compile(ast.Module(body=functions,type_ignores=[]),str(SCRIPT)+':isolated-parser-only','exec'),ns);select=ns['select_compile_command'];failure=ns['failure_text'];target='obj/components/sessions/shared/tab_restore_service_helper.o';source='components/sessions/core/tab_restore_service_helper.cc';raw=RAW.read_bytes();assert sha(RAW)=='2ec2cf5e16b6c6d544dec727ef5b91ae3fb33c591db36d98fe1e065c3a157158'
whole_invalid=False
try:raw.decode('utf-8',errors='strict')
except UnicodeDecodeError:whole_invalid=True
assert whole_invalid
argv=select(raw,target,source);assert argv[argv.index('-o')+1]==target and argv[argv.index('-c')+1].endswith('/'+source);line=next(x for x in raw.splitlines()if b'clang'in x and b' -c 'in x and b'-o '+target.encode()in x);assert argv==shlex.split(line.decode('utf-8',errors='strict'));checks=[]
def record(name,ok):assert ok,name;checks.append({'name':name,'pass':True})
def rejects(data,error_type):
 try:select(data,target,source)
 except error_type:return True
 return False
record('actual binary-noisy 20MB query selects exact unique command',True)
record('unrelated invalid UTF8 accepted unchanged argv',select(b'\xff\xfe\n'+raw+b'\nnoise\x80\xff\n',target,source)==argv)
record('selected invalid UTF8 rejected strictly',rejects(line+b' -DINVALID=\xff\n',UnicodeDecodeError))
record('missing target rejected',rejects(b'noise\xff\n',AssertionError))
record('ambiguous duplicate exact commands rejected',rejects(line+b'\n'+line+b'\n',AssertionError))
record('output prefix lookalike rejected',rejects(line.replace(b'-o '+target.encode(),b'-o '+target.encode()+b'.other'),AssertionError))
record('wrong source same output rejected',rejects(line.replace(source.encode(),b'components/sessions/core/other.cc'),AssertionError))
quoted=line.replace(b'-o '+target.encode(),b'-o "'+target.encode()+b'"').replace(b'-c ../../'+source.encode(),b'-c "../../'+source.encode()+b'"');record('quoted exact source and target accepted',select(quoted,target,source)==argv)
error=UnicodeDecodeError('utf-8',b'A'*20_000_000+b'\xff',20_000_000,20_000_001,'invalid start byte');serialized=failure(error);record('UnicodeDecodeError formatting excludes entire raw payload',serialized.startswith('UnicodeDecodeError: ')and len(serialized)<4200 and 'A'*100 not in serialized)
long=failure(RuntimeError('x'*20000));record('long error text bounded to type plus4096 message chars',long=='RuntimeError: '+'x'*4096)
record('v3 actual failure raw query unchanged',sha(RAW)=='2ec2cf5e16b6c6d544dec727ef5b91ae3fb33c591db36d98fe1e065c3a157158')
result={'scope':'Pure AST isolated command-selector/error-formatter only; reads preserved actual helper-query bytes; no executor import/constructor/entrypoint/livequery/compiler/index/lock/runtimequeue','scriptSHA256':sha(SCRIPT),'rawQuery':str(RAW),'rawBytes':len(raw),'rawSHA256':sha(RAW),'wholeRawStrictUTF8Fails':whole_invalid,'selectedArgvSHA256':hashlib.sha256(json.dumps(argv).encode()).hexdigest(),'selectedArgc':len(argv),'controls':checks};(OUT/'command-selection-results.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({'pass':True,'controls':len(checks),'scriptSHA256':sha(SCRIPT)}))
