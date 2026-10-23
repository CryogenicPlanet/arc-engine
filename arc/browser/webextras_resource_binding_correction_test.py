#!/usr/bin/env python3
"""Actual frozen TS/checker regression in private fixture; no app/build/lock."""
import ast
import hashlib
import json
from pathlib import Path
import sys
import types
import unittest
sys.dont_write_bytecode=True
ROOT=Path(__file__).resolve().parents[2]
EVIDENCE=ROOT/'logs/wp/webextras-native-resources-binding-correction'
ORIGINAL=ROOT/'arc/browser/webextras_resource_identity_test.py'
assert hashlib.sha256(ORIGINAL.read_bytes()).hexdigest()=='7de06d892da768a76cb24b4c577ab1c3645be0ff7d4330b22ba3679c77191673'

def replay(source, old):
    tree=ast.parse(ORIGINAL.read_bytes())
    func=next(n for c in tree.body if isinstance(c,ast.ClassDef) for n in c.body if isinstance(n,ast.FunctionDef) and n.name=='test_actual_typescript_capture_cleanup_and_journal_binding')
    assignment=next(n for n in func.body if isinstance(n,ast.Assign) and any(isinstance(t,ast.Name) and t.id=='body' for t in n.targets))
    s=assignment.value.value
    before="const exec=(argv:string[])=>{const p=Bun.spawnSync(argv,{stdout:'pipe',stderr:'pipe'});if(!p.success)throw Error(p.stderr.toString());return p.stdout.toString().trim()};"
    after="let transientSwap=false;const exec=(argv:string[])=>{let prior:string|undefined;if(transientSwap&&argv[3]==='verify-journal'){prior=readFileSync(argv[4],'utf8');writeFileSync(argv[4],JSON.stringify({resources:changed}));}const p=Bun.spawnSync(argv,{stdout:'pipe',stderr:'pipe'});if(prior!==undefined)writeFileSync(argv[4],prior);if(!p.success)throw Error(p.stderr.toString());return p.stdout.toString().trim()};"
    assert s.count(before)==1;s=s.replace(before,after)
    anchor="writeFileSync(FROZEN,readFileSync(FROZEN,'utf8')+'\\n# mutation\\n');"
    control="""const swapDirectory=join(DIR,'transient-journal');mkdirSync(swapDirectory);writeFileSync(join(swapDirectory,'binary-identity.json'),JSON.stringify({resources}));transientSwap=true;
const swap:any={binaryIdentity:{resources},errors:[]};tested.verifyResourceJournal(swap,swapDirectory);transientSwap=false;
assert(JSON.stringify(swap.resourcesAfterCleanup)!==JSON.stringify(resources),'control did not return changed identity');
assert(JSON.stringify(JSON.parse(readFileSync(join(swapDirectory,'binary-identity.json'),'utf8')).resources)===JSON.stringify(resources),'control did not restore journal');
"""
    control+=('assert(swap.resourceIdentityStatus===\'verified-unchanged\'&&swap.errors.length===0,\'old frozen false-pass control changed\');\n' if old else 'assert(swap.resourceIdentityStatus===\'changed-or-unverifiable\'&&swap.errors.length===1&&swap.errors[0].includes(\'verified resource result differs from captured launch identity\'),\'changed verifier result accepted\');\nassert(!Bun.file(join(swapDirectory,\'resources-after-cleanup.json\')).size,\'failed result wrote verified receipt\');\n')
    assert s.count(anchor)==1;assignment.value=ast.Constant(s.replace(anchor,control+anchor))
    for n in func.body:
        if isinstance(n,ast.Assign) and any(isinstance(t,ast.Name) and t.id=='source_path' for t in n.targets):n.value=ast.Call(func=ast.Name(id='Path',ctx=ast.Load()),args=[ast.Constant(str(source))],keywords=[])
    ast.fix_missing_locations(tree)
    name='binding_old' if old else 'binding_corrected';module=types.ModuleType(name);module.__file__=str(ORIGINAL);sys.modules[name]=module
    exec(compile(tree,str(ORIGINAL),'exec'),module.__dict__)
    suite=unittest.TestSuite([module.ResourceIdentityTests('test_actual_typescript_capture_cleanup_and_journal_binding')])
    result=unittest.TextTestRunner(verbosity=2).run(suite)
    assert result.wasSuccessful()
    return {'source':str(source),'sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'expectedOldFalsePass':old,'passed':True}

if __name__=='__main__':
    results=[replay(EVIDENCE/'baseline.ts',True),replay(EVIDENCE/'staged/arc/browser/webextras_native_resources_probe.ts',False)]
    print(json.dumps(results,indent=2))
