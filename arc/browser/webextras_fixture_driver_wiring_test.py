#!/usr/bin/env python3
"""Owned driver wiring tests: temporary copies only; no engine/app/lock use."""
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
IMAGE = 'WebextrasNativeProbe.dylib'
spec = importlib.util.spec_from_file_location('fixture_inputs', ROOT/'arc/browser/webextras_fixture_inputs.py')
inputs = importlib.util.module_from_spec(spec)
spec.loader.exec_module(inputs)


def rows(count=1, value=1, image=IMAGE):
    output = []
    for index in range(count):
        common = dict(thread='main', depth=0, dir='->engine', kind='-', cls='ArcMediaPosition',
                      sel='playbackRate', self='<ArcMediaPosition#1>', **{'from': image})
        output.extend((dict(common, seq=index*2+1, ev='call', args=[]), dict(common, seq=index*2+2, ev='ret', ret=value)))
    return output


def adopt_patch(directory, patch):
    """Accept exactly an applicable baseline or a completely adopted patch."""
    command = ['git', 'apply', '--whitespace=error']
    forward = subprocess.run([*command, '--check', str(patch)], cwd=directory, capture_output=True)
    reverse = subprocess.run([*command, '--reverse', '--check', str(patch)], cwd=directory, capture_output=True)
    if (forward.returncode == 0) == (reverse.returncode == 0):
        raise ValueError('patch is partial, drifted, or ambiguous: '+str(patch))
    if forward.returncode == 0:
        subprocess.run([*command, str(patch)], cwd=directory, check=True, capture_output=True)
        return 'applied'
    return 'already-applied'


def adopt_current_driver(directory):
    """Recognize the exact clock-v1 delta, preserving strict historical adoption checks.

    Reverse only the owned explicit extension in TEMP, demand the full prior
    adopted driver's frozen SHA, verify historical wiring, then restore current
    bytes. Arbitrary/partial/future source drift is not an accepted extension.
    """
    patch = ROOT/'arc/browser/webextras_fixture_driver_wiring.patch'
    try:
        return adopt_patch(directory, patch)
    except ValueError:
        extension = ROOT/'arc/browser/webextras_media_clock_driver_extension.patch'
        subprocess.run(['git', 'apply', '--reverse', '--check', '--whitespace=error', str(extension)],
                       cwd=directory, check=True, capture_output=True)
        subprocess.run(['git', 'apply', '--reverse', '--whitespace=error', str(extension)],
                       cwd=directory, check=True, capture_output=True)
        try:
            original = directory/'arc/browser/webextras_native_probe.ts'
            if hashlib.sha256(original.read_bytes()).hexdigest() != '1a31eb68758d66f50e608479b164ac7ff9f825555af300c0d39512f2c10d929f':
                raise ValueError('clock-v1 reversal did not recover the exact adopted driver')
            if adopt_patch(directory, patch) != 'already-applied':
                raise ValueError('clock-v1 baseline did not have complete historical wiring')
        finally:
            subprocess.run(['git', 'apply', '--whitespace=error', str(extension)], cwd=directory, check=True, capture_output=True)
        return 'already-applied-with-media-clock-v1'


class WiringTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory(prefix='webextras-wiring-proposal-')
        cls.directory = Path(cls.temp.name)
        cls.source = cls.directory/'source'
        for relative in (*inputs.FILES, 'arc/browser/webextras_native_probe.ts', 'arc/browser/webextras_values_probe.sh',
                         'arc/browser/webextras_fixture_projection.ts'):
            target = cls.source/relative; target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT/relative, target)
        # Comparator adoption remains confined to this temporary repository tree.
        adopt_patch(cls.source, ROOT/'arc/browser/webextras_fixture_comparator.patch')
        adopt_current_driver(cls.source)
        cls.counter = 0

    @classmethod
    def tearDownClass(cls):
        cls.temp.cleanup()

    def snapshot(self, image=IMAGE):
        type(self).counter += 1
        path = self.directory/f'snapshot-{self.counter}'
        state = inputs.freeze(self.source, path, image)
        self.assertTrue(state['capability']['verified'], state['capability'])
        return path, state

    def transcripts(self, left, right):
        type(self).counter += 1
        directory = self.directory/f'records-{self.counter}'; directory.mkdir()
        paths = []
        for name, events in (('golden', left), ('ours', right)):
            path = directory/name
            path.write_text(''.join(json.dumps(event, separators=(',', ':'))+'\n' for event in events))
            paths.append(path)
        return *paths, directory/'report.json'

    def test_capability_requires_mismatch_and_exact_metadata(self):
        _, state = self.snapshot()
        report = state['capability']['report']
        self.assertEqual(report['fixture_comparison'], inputs.metadata(IMAGE, 1))
        self.assertEqual(len(report['return_mismatch']), 1)
        # Freeze the unmodified shared comparator: an ignored option/zero report is insufficient.
        raw = self.directory/'unmodified'
        for relative in inputs.FILES:
            target = raw/relative; target.parent.mkdir(parents=True, exist_ok=True); shutil.copyfile(ROOT/relative, target)
        state = inputs.freeze(raw, self.directory/'unsupported', IMAGE)
        self.assertFalse(state['capability']['verified'])
        self.assertEqual(state['capability']['status'], 'pending-fixture-comparison-support')

    def test_changed_public_return_is_overall_comparison_failure(self):
        snapshot, _ = self.snapshot()
        left, right, report_path = self.transcripts(rows(value=1), rows(value=2))
        _, report = inputs.compare(snapshot, IMAGE, left, right, 1, report_path)
        self.assertEqual(len(report['return_mismatch']), 1)

    def test_exact_13_native_and_72_values_coverage(self):
        for image, count in ((IMAGE, 13), ('probe', 72)):
            with self.subTest(image=image):
                snapshot, _ = self.snapshot(image)
                left, right, report_path = self.transcripts(rows(count, image=image), rows(count, image=image))
                _, report = inputs.compare(snapshot, image, left, right, count, report_path)
                self.assertEqual(report['fixture_comparison'], inputs.metadata(image, count))
                for wrong in (count-1, count+1):
                    with self.assertRaisesRegex(ValueError, 'coverage'):
                        inputs.compare(snapshot, image, left, right, wrong, report_path)

    def test_missing_mixed_zero_incomplete_evidence_fails(self):
        snapshot, _ = self.snapshot()
        missing = rows(); del missing[1]['from']
        wrong = rows(image='other.dylib')
        for bad in ([], missing, rows()+missing, wrong, rows()[:1], rows()[1:]):
            with self.subTest(bad=bad):
                left, right, report_path = self.transcripts(bad, rows())
                with self.assertRaises(ValueError):
                    inputs.compare(snapshot, IMAGE, left, right, 1, report_path)

    def test_each_frozen_input_tamper_fails(self):
        for relative in inputs.FILES:
            with self.subTest(relative=relative):
                snapshot, _ = self.snapshot()
                changed = snapshot/relative
                changed.chmod(0o644); changed.write_bytes(changed.read_bytes()+b'\n# tampered\n')
                left, right, report_path = self.transcripts(rows(), rows())
                with self.assertRaisesRegex(ValueError, 'frozen comparison input changed'):
                    inputs.compare(snapshot, IMAGE, left, right, 1, report_path)

    def test_source_changes_do_not_change_frozen_comparison(self):
        snapshot, state = self.snapshot()
        target = self.source/'tests/recorder/diff.py'
        saved = target.read_bytes(); target.write_bytes(saved+b'\n# source changed after snapshot\n')
        try:
            self.assertEqual(inputs.verify(snapshot), state)
            left, right, report_path = self.transcripts(rows(), rows())
            _, report = inputs.compare(snapshot, IMAGE, left, right, 1, report_path)
            self.assertFalse(report['return_mismatch'])
        finally:
            target.write_bytes(saved)

    def test_patched_drivers_parse_without_execution(self):
        result = subprocess.run(['bun', 'build', str(self.source/'arc/browser/webextras_native_probe.ts'), '--target=bun',
                                 '--external', '*', '--outdir', str(self.directory/'bundle')], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        subprocess.run(['bash', '-n', str(self.source/'arc/browser/webextras_values_probe.sh')], check=True)

    def test_driver_patch_baseline_adopted_and_partial_states(self):
        directory = self.directory/'adoption-states'
        relatives = ('arc/browser/webextras_native_probe.ts', 'arc/browser/webextras_values_probe.sh')
        for relative in relatives:
            target = directory/relative; target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(self.source/relative, target)
        patch = ROOT/'arc/browser/webextras_fixture_driver_wiring.patch'
        self.assertEqual(adopt_current_driver(directory), 'already-applied-with-media-clock-v1')
        # The historical adoption state matrix uses the exactly recovered prior
        # driver; every behavioral/projection/negative test uses CURRENT bytes.
        extension = ROOT/'arc/browser/webextras_media_clock_driver_extension.patch'
        subprocess.run(['git', 'apply', '--reverse', '--whitespace=error', str(extension)], cwd=directory, check=True, capture_output=True)
        self.assertEqual(adopt_patch(directory, patch), 'already-applied')
        subprocess.run(['git', 'apply', '--reverse', '--whitespace=error', str(patch)], cwd=directory, check=True, capture_output=True)
        baseline = {relative: (directory/relative).read_bytes() for relative in relatives}
        self.assertEqual(adopt_patch(directory, patch), 'applied')
        # One adopted driver and one baseline driver must not silently pass.
        (directory/relatives[1]).write_bytes(baseline[relatives[1]])
        with self.assertRaisesRegex(ValueError, 'partial, drifted, or ambiguous'):
            adopt_patch(directory, patch)
        shutil.copyfile(self.source/relatives[1], directory/relatives[1])
        native = directory/relatives[0]
        native.write_text(native.read_text().replace('import { PublicTraceProjection }', 'import { DriftedProjection }', 1))
        with self.assertRaisesRegex(ValueError, 'partial, drifted, or ambiguous'):
            adopt_patch(directory, patch)

    def test_unsupported_capability_measures_both_but_never_accepts(self):
        # Execute actual measurement and acceptance bodies with mocked actions.
        # This starts no process/engine and does not establish runtime coverage.
        driver = (self.source/'arc/browser/webextras_native_probe.ts').read_text()
        measurement = driver[driver.index('for (const operation of operations) {'):driver.index('      run.phase = "cdp-close-fixture";')]
        gate_start = driver.index('run.fixtureComparisonStatus = fixtureVerified')
        pending_gate = driver[gate_start:driver.index('          for (const row of all)', gate_start)]
        comparison_start = driver.index('const boundaryDiffs: unknown[] = [];')
        acceptance = driver[comparison_start:driver.index('writeFileSync(join(out, "summary.json")', comparison_start)]
        harness = self.directory/'unsupported-measurement.ts'
        harness.write_text(r'''
const mediaClock=false;
const fixtureVerified=false, fixtureImage='WebextrasNativeProbe.dylib';
const fixtureComparisonPending='pending-fixture-comparison-support';
const operations=['console-open','frontend','console-close','panel-closed','execute-evaluate','execute-malformed','execute-unknown','subscription','media-types','pip-request-error','pip-exit-error'];
const actionManifest=operations.map(operation=>({key:operation,operation}));
const agents=['trace156','ours156'], runs:any[]=[], differences:any[]=[];
const outcomeErrors=()=>[];
const require=(value:any,message:string)=>{if(!value)throw new Error(message);};
for(const agent of agents){
 const run:any={agent,actions:[],errors:[],cleanup:'mock cleanup'};runs.push(run);
 const registry={ownerTag:1};
 const action=async(operation:string)=>({operation,ok:true,ownerTag:1,matches:1,frontendPresent:true,types:[],value:false,
   error:operation.startsWith('pip-')?{domain:'company.thebrowser.Arc',code:6}:null,traceShape:[{mock:true}]});
 __MEASUREMENT__
 __PENDING_GATE__
}
const golden=runs[0],ours=runs[1],out='unused',comparisonHelper='unused',comparisonSnapshot='unused';
const join=(...parts:string[])=>parts.join('/');
const existsSync=()=>true;
const writeFileSync=()=>{throw new Error('unsupported comparator unexpectedly invoked');};
const expectedSelectors=()=>['unused'];
const json=()=>{throw new Error('unsupported comparator report unexpectedly loaded');};
__ACCEPTANCE__
require(runs.every(run=>run.actions.length===11),'pending capability aborted measurement');
require(runs.every(run=>run.errors.length===1&&run.fixtureComparisonStatus===fixtureComparisonPending),'pending gate lost');
require(complete===false&&parity===false,'unsupported fixture comparison falsely accepted');
require(boundaryDiffs.length===1&&fixtureComparison.fixtureImageOption===null,'pending metadata lost');
console.log('22 mocked acknowledgments; actual measurement/acceptance bodies retain pending false acceptance');
'''.replace('__MEASUREMENT__', measurement).replace('__PENDING_GATE__', pending_gate).replace('__ACCEPTANCE__', acceptance))
        result = subprocess.run(['bun', str(harness)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn('22 mocked acknowledgments', result.stdout)

    def test_saved_original_replay_and_public_projection(self):
        harness = self.directory/'projection.ts'
        harness.write_text(r'''
import {readFileSync} from 'node:fs';
import {PublicTraceProjection} from '__PROJECTION__';
const source = readFileSync('__DRIVER__', 'utf8');
const validators = source.slice(source.indexOf('type RecordRow'), source.indexOf('const root ='));
const code = new Bun.Transpiler({loader:'ts'}).transformSync(validators + '\nreturn {parseRecords,validateNesting,validateActionTrace,outcomeErrors};');
const api = new Function('PublicTraceProjection', code)(PublicTraceProjection);
const metadata = readFileSync('__METADATA__','utf8');
const image = 'WebextrasNativeProbe.dylib';
const require = (value:any, message:string) => {if(!value) throw new Error(message);};
const record = readFileSync('__RECORD__');
const observations = JSON.parse(readFileSync('__OBSERVATIONS__','utf8'));
const original = new PublicTraceProjection(metadata,image,false); // Saved v1 evidence, never accepted as v2 fixture evidence.
let count = 0;
for (const action of observations.actions) {
 const events = api.parseRecords(record.subarray(action.recordBegin,action.recordEnd),action.recordBegin);
 require(api.validateActionTrace(action,events,record).length>0,'legacy raw replay failed');
 require(api.validateActionTrace(action,events,record,original).length>0,'legacy projected replay failed');
 require(api.outcomeErrors(action).length===0,'saved outcome changed');
 ++count;
}
require(count===11,'all saved actions required');
const common = {thread:'main',kind:'-',dir:'->engine',cls:'ArcWebContentsDevTools',self:'<ArcWebContentsDevTools#10>',from:image};
const call = (sel:string, depth:number, extra:any={}) => ({...common,ev:'call',sel,depth,args:[],...extra});
const ret = (event:any) => {const copy={...event,ev:'ret',ret:null};delete copy.args;return copy;};
function fixture(helper:boolean, getter=true) {
 const root=call('openConsoleView',0);
 const internal=call('privateNativeHelper',1,{from:'engine'});
 const cb=call('webContentsDevTools:displayAs:',helper?2:1,{dir:'engine->app',cls:'ProbeDelegate',self:'<ProbeDelegate#2>',from:'engine',args:[root.self,3]});
 const child=call('playbackRate',helper?3:2,{cls:'ArcMediaPosition',self:'<ArcMediaPosition#30>',from:'app'});
 return [root,...(helper?[internal]:[]),cb,...(getter?[child,ret(child)]:[]),ret(cb),...(helper?[ret(internal)]:[]),ret(root)];
}
function parsed(events:any[]) {
 return api.parseRecords(Buffer.from(events.map((event,index)=>JSON.stringify({...event,seq:index+1})).join('\n')+'\n'));
}
function project(events:any[], context=new PublicTraceProjection(metadata,image)) {
 const rows=parsed(events);api.validateNesting(rows);return context.project(rows);
}
function fullAction(helper:boolean) {
 const root=call('openConsoleView',0);
 const internal=call('privateNativeHelper',1,{from:'engine'});
 const callback=call('webContentsDevTools:displayAs:',helper?2:1,{dir:'engine->app',cls:'ProbeDelegate',self:'<ProbeDelegate#2>',from:'engine',args:[root.self,3]});
 const events=[root,...(helper?[internal]:[]),callback,ret(callback),...(helper?[ret(internal)]:[]),ret(root)];
 const data=Buffer.from(events.map((event,index)=>JSON.stringify({...event,seq:index+1})).join('\n')+'\n');
 const parsed=api.parseRecords(data), begin=parsed.find(row=>row.event.sel===callback.sel&&row.event.ev==='call'), end=parsed.find(row=>row.event.sel===callback.sel&&row.event.ev==='ret');
 const selector='openConsoleView';
 const action={operation:'console-open',recordBegin:0,recordEnd:data.length,recordAfterReturn:data.length,
   publicCalls:[{selector,recordBegin:0,recordEnd:data.length}],
   callbacks:[{event:'display',publicSelector:selector,recordOffset:begin.begin,recordAfterForwarding:end.end,forwarded:true,beforePublicReturn:true,mainThread:true}],
   timeline:[{event:'publicCall',selector},{event:'display',selector,mainThread:true,beforePublicReturn:true},{event:'publicReturn',selector}]};
 return api.validateActionTrace(action,parsed,data,new PublicTraceProjection(metadata,image));
}
require(JSON.stringify(fullAction(false))===JSON.stringify(fullAction(true)),'full callback validator rejected legitimate helper ancestry');
const literal=call('initWithAction:value:',0,{cls:'ArcAccessibilityActionData',self:'<ArcAccessibilityActionData#4>',from:'app',args:[1,'<ptr>']});
require(project([literal,ret(literal)]).length===2,'literal string value mistaken for native pointer');
const native=call('initWithMediaPosition:',0,{cls:'ArcMediaPosition',self:'<ArcMediaPosition#4>',from:'app',args:['<ptr>']});
require(project([native,ret(native)]).length===0,'native pointer constructor was not excluded');
const plain=project(fixture(false));const wrapped=project(fixture(true));
require(JSON.stringify(plain)===JSON.stringify(wrapped),'internal helper changed public depth/identity shape');
const engineGetter=fixture(false);engineGetter[2].from='engine';engineGetter[3].from='engine';
require(project(engineGetter).length===plain.length-2,'engine getter was not excluded');
require(JSON.stringify(project(fixture(false,false)))!==JSON.stringify(plain),'missing public call hidden');
const changedOrder=fixture(false);changedOrder[0].sel='close';changedOrder.at(-1).sel='close';
require(JSON.stringify(project(changedOrder))!==JSON.stringify(plain),'changed public order/selector hidden');
const first=call('openConsoleView',0), second=call('close',0);
require(JSON.stringify(project([first,ret(first),second,ret(second)]))!==JSON.stringify(project([second,ret(second),first,ret(first)])),'cross-method call order hidden');
const changedReceiver=fixture(false);changedReceiver[0].self='<ArcWebContentsDevTools#11>';changedReceiver.at(-1).self=changedReceiver[0].self;
const left=new PublicTraceProjection(metadata,image);project(fixture(false),left);
const right=new PublicTraceProjection(metadata,image);project(fixture(false),right);
require(JSON.stringify(project(fixture(false),left))!==JSON.stringify(project(changedReceiver,right)),'cross-action receiver replacement hidden');
const malformed=fixture(false);malformed[1].depth=19;
let rejected=false;try{project(malformed);}catch{rejected=true;}require(rejected,'raw nesting skipped');
console.log('11 actual saved v1 action replays; helper/getter projection, public order/receiver/missing-call and raw nesting checks passed');
'''.replace('__PROJECTION__', str(self.source/'arc/browser/webextras_fixture_projection.ts'))
           .replace('__DRIVER__', str(self.source/'arc/browser/webextras_native_probe.ts'))
           .replace('__METADATA__', str(self.source/'arc/tools/arccore-1.170.0-objc.tsv'))
           .replace('__RECORD__', str(ROOT/'results/webextras/native-run3/trace156/record.jsonl'))
           .replace('__OBSERVATIONS__', str(ROOT/'results/webextras/native-run3/trace156/observations.json')))
        result = subprocess.run(['bun', str(harness)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn('11 actual saved v1 action replays', result.stdout)


if __name__ == '__main__':
    unittest.main(verbosity=2)
