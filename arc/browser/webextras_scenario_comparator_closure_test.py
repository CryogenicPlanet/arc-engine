#!/usr/bin/env python3
"""Held canonical comparator closure; private files/subprocesses only, no app or lock."""
import ast
import hashlib
import json
from pathlib import Path
import py_compile
import shutil
import subprocess
import sys
import tempfile
import types
import unittest

ROOT = Path(__file__).resolve().parents[2]
RUNNER = 'arc/browser/webextras_scenario_current_run.py'
FREEZER = 'arc/browser/webextras_fixture_inputs.py'
LEGACY_DIFF = ROOT/'results/webextras/native-run9/fixture-comparator/tests/recorder/diff.py'
SUPPORTED = ROOT/'logs/wp/webextras-boundary-arguments-callback-correction/staged'


def source_module(path):
    module = types.ModuleType('private_actual_source'); module.__file__ = str(path)
    exec(compile(path.read_bytes(), str(path), 'exec'), module.__dict__)
    return module


class ComparatorClosure(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix='webextras-default-closure-')
        self.base = Path(self.temp.name).resolve(); self.root = self.base/'source'; self.root.mkdir()
        self.actual_hash = hashlib.sha256((ROOT/RUNNER).read_bytes()).hexdigest()
        self.copy(RUNNER); self.copy(FREEZER)
        subprocess.run(['git','init','--quiet',str(self.root)],check=True,capture_output=True)
        for patch in ('webextras_scenario_resources_held.patch','webextras_scenario_comparator_closure_held.patch'):
            argv=['git','apply','--whitespace=error',str(ROOT/'arc/browser'/patch)]
            subprocess.run(argv[:2]+['--check']+argv[2:],cwd=self.root,check=True,capture_output=True)
            subprocess.run(argv,cwd=self.root,check=True,capture_output=True)
        self.runner=source_module(self.root/RUNNER)
        self.inputs=self.runner.load_comparison_inputs()
        for name in self.inputs.FILES:
            if name == 'tests/recorder/diff.py':
                target=self.root/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(LEGACY_DIFF,target)
            elif name != FREEZER: self.copy(name)
        self.commands=[]
        def run(argv, log):
            self.commands.append(argv)
            result=subprocess.run(argv,text=True,capture_output=True)
            Path(log).write_text(result.stdout+result.stderr)
            return result.returncode
        self.runner.run_command=run

    def tearDown(self):
        self.assertEqual(hashlib.sha256((ROOT/RUNNER).read_bytes()).hexdigest(),self.actual_hash)
        self.temp.cleanup()

    def copy(self,name):
        target=self.root/name;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(ROOT/name,target)

    def supported(self):
        for p in (SUPPORTED/'tests/recorder').glob('*.py'):
            shutil.copyfile(p,self.root/'tests/recorder'/p.name)

    def records(self):
        common=dict(thread='main',depth=0,dir='->engine',kind='-',cls='ArcMediaPosition',
                    sel='playbackRate',self='<ArcMediaPosition#1>',**{'from':'app'})
        for name,value in (('golden',1),('ours',2)):
            (self.base/name).write_text(''.join(json.dumps(row,separators=(',',':'))+'\n' for row in
                [dict(common,seq=1,ev='call',args=[]),dict(common,seq=2,ev='ret',ret=value)]))
        return self.base/'golden',self.base/'ours'

    def default_equal(self,count):
        snapshot=self.base/'canonical-comparator'
        state=self.inputs.freeze(self.root,snapshot,'WebextrasCanonicalCanary.dylib')
        self.assertEqual(len(state['files']),count)
        left,right=self.records(); report=self.base/'frozen.json'; log=self.base/'frozen.log'
        # The wrapper ignores fixture capability: original policy is required even for legacy support.
        self.assertEqual(self.runner.compare_records(self.inputs,snapshot,left,right,report,log),0)
        live_report=self.base/'live.json'
        live=subprocess.run([sys.executable,'-B',str(self.root/'tests/recorder/diff.py'),str(left),str(right),
                             '--json',str(live_report)],text=True,capture_output=True,check=True)
        self.assertEqual(json.loads(report.read_text()),json.loads(live_report.read_text()))
        self.assertEqual(log.read_text(),live.stdout+live.stderr)
        self.assertEqual(len(json.loads(report.read_text())['return_mismatch']),1)
        self.assertNotIn('--fixture-image',self.commands[0])
        self.assertEqual(self.inputs.verify(snapshot),state)
        return snapshot,state

    def test_legacy_four_input_default_report_unchanged_and_pending_is_not_gate(self):
        _,state=self.default_equal(4);self.assertFalse(state['capability']['verified'])

    def test_supported_seven_input_default_report_unchanged(self):
        self.supported();_,state=self.default_equal(7);self.assertTrue(state['capability']['verified'])

    def test_missing_helper_rejected_before_default_execution(self):
        self.supported();snapshot=self.base/'snapshot';self.inputs.freeze(self.root,snapshot,'Canary.dylib')
        (snapshot/'tests/recorder/download_identity.py').unlink()
        with self.assertRaises(ValueError):
            self.runner.compare_records(self.inputs,snapshot,*self.records(),self.base/'report',self.base/'log')
        self.assertEqual(self.commands,[])

    def test_frozen_tamper_during_comparison_rejected_after_execution(self):
        snapshot=self.base/'snapshot';self.inputs.freeze(self.root,snapshot,'Canary.dylib')
        def tamper(*_):
            path=snapshot/'tests/recorder/diff.py';path.chmod(0o644);path.write_bytes(path.read_bytes()+b'\n#tamper\n');return 0
        self.runner.run_command=tamper
        with self.assertRaisesRegex(ValueError,'changed'):
            self.runner.compare_records(self.inputs,snapshot,*self.records(),self.base/'report',self.base/'log')

    def test_helper_source_load_ignores_unchecked_pyc_and_rejects_source_drift(self):
        path=self.root/FREEZER;good=path.read_bytes();marker=self.base/'poison-executed'
        path.write_text(f'from pathlib import Path\nPath({str(marker)!r}).touch()\nraise RuntimeError("poison")\n')
        py_compile.compile(str(path),doraise=True,invalidation_mode=py_compile.PycInvalidationMode.UNCHECKED_HASH)
        path.write_bytes(good)
        self.assertEqual(self.runner.load_comparison_inputs().FILES,self.inputs.FILES)
        self.assertFalse(marker.exists())
        path.write_bytes(good+b'\n#not reviewed\n')
        with self.assertRaisesRegex(RuntimeError,'reviewed f605'):self.runner.load_comparison_inputs()

    def test_poison_bytecode_inside_snapshot_rejected_before_execution(self):
        snapshot=self.base/'snapshot';self.inputs.freeze(self.root,snapshot,'Canary.dylib')
        py_compile.compile(str(snapshot/'tests/recorder/diff.py'),doraise=True,
                           invalidation_mode=py_compile.PycInvalidationMode.UNCHECKED_HASH)
        with self.assertRaisesRegex(ValueError,'directory'):
            self.runner.compare_records(self.inputs,snapshot,*self.records(),self.base/'report',self.base/'log')
        self.assertEqual(self.commands,[])

    def test_aggregate_equals_strict_resource_then_closure_composition(self):
        other=self.base/'aggregate';p=other/RUNNER;p.parent.mkdir(parents=True);shutil.copyfile(ROOT/RUNNER,p)
        subprocess.run(['git','init','--quiet',str(other)],check=True,capture_output=True)
        subprocess.run(['git','apply','--whitespace=error',str(ROOT/'arc/browser/webextras_scenario_observation_held.patch')],
                       cwd=other,check=True,capture_output=True)
        self.assertEqual(p.read_bytes(),(self.root/RUNNER).read_bytes())

    def test_actual_source_inputs_admits_complete_comparator_closure(self):
        self.supported()
        # Extract this exact statement instead of constructing unrelated TS/native fixtures.
        function=next(n for n in ast.parse((self.root/RUNNER).read_bytes()).body if isinstance(n,ast.FunctionDef) and n.name=='source_inputs')
        statements=[n for n in function.body if isinstance(n,ast.Expr) and 'comparison_files' in ast.unparse(n)]
        self.assertEqual(len(statements),1);paths=set()
        exec(compile(ast.Module(body=statements,type_ignores=[]),'<actual-source-inventory>','exec'),
             dict(paths=paths,ROOT=self.root,load_comparison_inputs=self.runner.load_comparison_inputs))
        self.assertEqual(paths,{self.root/n for n in self.inputs.comparison_files(self.root)})
        self.assertEqual(len(paths),7)

    def test_executor_source_load_ignores_unchecked_runner_pyc_and_checks_pin(self):
        executor=source_module(ROOT/'arc/browser/webextras_initial_integration.py')
        path=self.base/'runner.py';good=b'VALUE = "genuine source"\n';path.write_bytes(good)
        marker=self.base/'runner-poison';path.write_text(f'from pathlib import Path\nPath({str(marker)!r}).touch()\n')
        py_compile.compile(str(path),doraise=True,invalidation_mode=py_compile.PycInvalidationMode.UNCHECKED_HASH)
        path.write_bytes(good);expected=hashlib.sha256(good).hexdigest()
        self.assertEqual(executor.source_module(path,expected).VALUE,'genuine source');self.assertFalse(marker.exists())
        path.write_bytes(b'VALUE="drift"\n')
        with self.assertRaisesRegex(RuntimeError,'sealed image'):executor.source_module(path,expected)


if __name__=='__main__':unittest.main()
