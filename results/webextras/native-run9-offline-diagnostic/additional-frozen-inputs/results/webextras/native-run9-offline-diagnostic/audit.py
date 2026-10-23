import importlib.util,json,shutil,subprocess,tempfile
from pathlib import Path
ROOT=Path('/Users/cryogenicplanet/general/arc-engine'); OUT=ROOT/'results/webextras/native-run9-offline-diagnostic'; IMAGE='WebextrasNativeProbe.dylib'
TEMP=Path(tempfile.mkdtemp(prefix='webextras-native9-offline-')); SOURCE=TEMP/'source'; SNAP=TEMP/'frozen-comparator'
spec=importlib.util.spec_from_file_location('fi',ROOT/'arc/browser/webextras_fixture_inputs.py'); fi=importlib.util.module_from_spec(spec);spec.loader.exec_module(fi)
files=(*fi.FILES,'arc/browser/webextras_native_probe.ts','arc/browser/webextras_values_probe.sh','arc/browser/webextras_fixture_projection.ts','arc/browser/webextras_fixture_comparator.patch','arc/browser/webextras_fixture_driver_wiring.patch','arc/browser/webextras_native_current_run.sh','arc/browser/webextras_native_probe.m','arc/browser/webextras_media_clock_probe_test.py')
manifest={}
def freeze_copy(src,dst):
 before=fi.identity(src);dst.parent.mkdir(parents=True,exist_ok=True)
 with dst.open('xb') as output,src.open('rb') as inp:shutil.copyfileobj(inp,output)
 dst.chmod(0o444); after=fi.identity(src);frozen=fi.identity(dst)
 assert before==after and before['sha256']==frozen['sha256']
 return {'source':before,'frozen':frozen}
for relative in files:
 target=SOURCE/relative; target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(ROOT/relative,target)
 manifest[relative]=freeze_copy(ROOT/relative,OUT/'frozen-original-inputs'/relative)
# Apply the exact owned proposal only inside a new TEMP source tree.
proposal=SOURCE/'arc/browser/webextras_fixture_comparator.patch'
check=subprocess.run(['git','apply','--check',str(proposal)],cwd=SOURCE,text=True,capture_output=True)
assert check.returncode==0,check.stderr
applied=subprocess.run(['git','apply',str(proposal)],cwd=SOURCE,text=True,capture_output=True)
assert applied.returncode==0,applied.stderr
(OUT/'proposal-application.json').write_text(json.dumps({'check':check.returncode,'apply':applied.returncode,'stdout':applied.stdout,'stderr':applied.stderr},indent=2)+'\n')
state=fi.freeze(SOURCE,SNAP,IMAGE);assert state['capability']['verified'],state['capability']
for agent in ['trace156','ours156']:
 for name in ['record.jsonl','observations.json','binary-identity.json','media-clock.json','clock-evaluations.jsonl','app.log','registry.json','fixture-state.json','readiness.json']:
  src=ROOT/'results/webextras/native-run9'/agent/name
  manifest[f'{agent}/{name}']=freeze_copy(src,OUT/'frozen-recordings'/agent/name)
# Execute from the TEMP tree; preserve all bytes of the frozen comparator for later review.
shutil.copytree(SNAP,OUT/'archived-temp-comparator')
(OUT/'input-manifest.json').write_text(json.dumps({'scope':'OFFLINE DIAGNOSTIC ONLY; held patches not adopted; no runtime acceptance','temporarySource':str(SOURCE),'temporaryComparator':str(SNAP),'files':manifest,'comparisonInputs':state},indent=2)+'\n')
HARNESS=r'''
import {readFileSync,writeFileSync,mkdirSync} from 'node:fs';
import {PublicTraceProjection} from '__PROJECTION__';
const driver=readFileSync('__DRIVER__','utf8');
const validators=driver.slice(driver.indexOf('type RecordRow'),driver.indexOf('const root ='));
const semantic=driver.slice(driver.indexOf('const semantic ='),driver.indexOf('const golden ='));
const api=new Function('PublicTraceProjection',new Bun.Transpiler({loader:'ts'}).transformSync(validators+'\n'+semantic+'\nreturn {parseRecords,scanRecords,validateNesting,validateActionTrace,outcomeErrors,expectedSelectors,semantic,validateMediaServiceClockPhase,mediaServiceClockPhases,mediaClockPolicy,clockBracket,clockValue};'))(PublicTraceProjection);
const image='WebextrasNativeProbe.dylib';const metadata=readFileSync('__METADATA__','utf8');const root='__OUT__';
const require=(v:any,m:string)=>{if(!v)throw new Error(m)};
const runs:any={};
for(const agent of ['trace156','ours156']){
 const record=readFileSync(`${root}/frozen-recordings/${agent}/record.jsonl`);
 const observation=JSON.parse(readFileSync(`${root}/frozen-recordings/${agent}/observations.json`,'utf8'));
 const all=api.parseRecords(record);require(all[0]?.event.ev==='hooked'&&all[0]?.event.seq===0&&all[0]?.event.engine_classes>0,'hooked missing');
 require(all.filter((r:any)=>r.event.ev==='hooked').length===1,'hooked duplicate');require(new Set(all.map((r:any)=>r.event.seq)).size===all.length,'sequence duplicate');
 for(const {event} of all)if(['call','ret'].includes(event.ev))require(typeof event.from==='string'&&event.from.length>0&&event.from.trim()===event.from&&!/[\x00-\x1f]/.test(event.from),'bad caller');
 const full=api.scanRecords(all); // Existing parser: matched returns and all raw ancestry; lifecycle tail may remain open.
 const projection=new PublicTraceProjection(metadata,image);const held=new Map<string,string>();const actions:any[]=[];let calls=0,returns=0;
 mkdirSync(`${root}/action-recordings/${agent}`,{recursive:true});
 for(const original of observation.actions){
  const action=JSON.parse(JSON.stringify(original));const span=record.subarray(action.recordBegin,action.recordEnd);const rows=api.parseRecords(span,action.recordBegin);
  const projected=api.validateActionTrace(action,rows,record,projection,held);const fixture=rows.filter((r:any)=>r.event.dir==='->engine'&&r.event.from===image);
  const c=fixture.filter((r:any)=>r.event.ev==='call').length,t=fixture.filter((r:any)=>r.event.ev==='ret').length,e=api.expectedSelectors(action.operation).length;
  require(c===e&&t===e,`bad coverage ${agent}/${action.operation}`);calls+=c;returns+=t;
  writeFileSync(`${root}/action-recordings/${agent}/${action.actionKey??action.operation}.record.jsonl`,span);
  action.traceShape=projected;
  const rawShape=original.traceShape??[];
  actions.push({key:action.actionKey??action.operation,operation:action.operation,fixtureCounts:{call:c,ret:t},rawValidation:'passed including ambient stack/receiver/forwarding/actual ancestry',outcomeErrors:api.outcomeErrors(action),rawShapeEvents:rawShape.length,projectedEvents:projected.length,projectedShape:projected,semantic:api.semantic(action)});
 }
 require(actions.length===41&&calls===115&&returns===115,'incomplete whole-pair coverage');
 const clock=JSON.parse(readFileSync(`${root}/frozen-recordings/${agent}/media-clock.json`,'utf8'));
 require(clock.probeMode==='media-service-clock'&&JSON.stringify(clock.policy)===JSON.stringify(api.mediaClockPolicy)&&clock.phases.length===6,'wrong clock mode/policy/coverage');
 const clockPhases=[];
 for(let i=0;i<6;i++){
  const phase=api.mediaServiceClockPhases[i],report=clock.phases[i],measured=observation.actions.slice(11+i*5,16+i*5);
  require(report.phase===phase.name&&report.status==='complete'&&report.functional==='pass'&&JSON.stringify(report.attempts.map(({traceShape,...rest}:any)=>rest))===JSON.stringify(measured.map(({traceShape,...rest}:any)=>rest)),'phase receipts diverged');
  api.validateMediaServiceClockPhase(phase,measured);
  const release=report.release;
  require(release.operation==='clock-release'&&release.ok===true&&release.matches===1&&release.mainThread===true&&release.ownerTag===measured[4].ownerTag&&release.publicCalls.length===0&&release.timeline.length===0&&release.callbacks.length===0&&release.recordBegin===release.recordEnd&&release.recordBegin>=measured[4].recordEnd,'invalid phase release');
  const samples=measured.slice(2).map((a:any)=>({actionKey:a.actionKey,token:a.clock.heldToken,dtoTag:a.clock.dtoTag,currentTime:api.clockValue(a,'currentTime'),bracket:api.clockBracket(a),ticks:a.publicCalls.filter((c:any)=>c.selector==='currentTime').map((c:any)=>({before:c.clockBeforeTicks,after:c.clockAfterTicks}))}));
  clockPhases.push({phase:phase.name,functional:'pass',input:measured[2].positionReceipt.input,rate:phase.rate,heldSamples:samples,release:{id:release.id,recordBegin:release.recordBegin,recordEnd:release.recordEnd,zeroPublicCalls:true}});
 }
 const evaluations=readFileSync(`${root}/frozen-recordings/${agent}/clock-evaluations.jsonl`,'utf8').trim().split('\n').map((s:string)=>JSON.parse(s));
 require(evaluations.length===66&&evaluations.every((e:any,i:number)=>e.sequence===i+1&&e.status==='response'&&e.resultPresent===true&&!e.rawResult?.exceptionDetails&&BigInt(e.hostAfterNanos)>=BigInt(e.hostBeforeNanos)),'raw evaluation coverage/result/host bracket invalid');
 require(evaluations.every((e:any)=>!e.label.startsWith('activation')&&!/createElement|\.play\(|clock\.webm|fetch\(|XMLHttpRequest|AudioContext|new Audio/.test(e.expression)),'service-only lane includes activation/player/resource stimulus');
 require(evaluations.filter((e:any)=>e.label==='prepare').length===6,'wrong prepare count');

 runs[agent]={phase:observation.phase,cleanup:observation.cleanup,fullRows:all.length,callerValidation:'all calls/returns verified',fullRawAncestry:'all calls/returns scan passed; lifecycle tail may remain open',remainingLifecycleStack:Object.fromEntries([...full.stacks].map(([t,s]:any)=>[t,s.map((r:any)=>({seq:r.event.seq,cls:r.event.cls,sel:r.event.sel,from:r.event.from}))])),fixtureCounts:{call:calls,ret:returns},clockPhases,rawClockEvaluations:{count:evaluations.length,successful:true,activationStimulus:false,mediaAssetStimulus:false},actions};
}
function differences(a:any,b:any,path=''):any[]{
 if(JSON.stringify(a)===JSON.stringify(b))return [];
 if(a&&b&&typeof a==='object'&&typeof b==='object'&&Array.isArray(a)===Array.isArray(b)){
  return [...new Set([...Object.keys(a),...Object.keys(b)])].flatMap(k=>differences(a[k],b[k],path+'/'+k));
 }
 return [{path,golden:a===undefined?'<undefined>':a,ours:b===undefined?'<undefined>':b}];
}
const perAction=[];
for(let i=0;i<41;i++){
 const l=runs.trace156.actions[i],r=runs.ours156.actions[i];require(l.operation===r.operation&&l.key===r.key,'order mismatch');
 perAction.push({key:l.key,operation:l.operation,projectedShapeEqual:JSON.stringify(l.projectedShape)===JSON.stringify(r.projectedShape),projectedShapeDifferences:differences(l.projectedShape,r.projectedShape),semanticDifferences:differences(l.semantic,r.semantic)});
}
writeFileSync(`${root}/validation-and-projection.json`,JSON.stringify({scope:'held TEMP proposal offline diagnostic, not acceptance',runs,perAction},null,2));
console.log(JSON.stringify({runs:Object.fromEntries(Object.entries(runs).map(([a,r]:any)=>[a,{fullRows:r.fullRows,fixtureCounts:r.fixtureCounts,outcomeErrors:r.actions.flatMap((a:any)=>a.outcomeErrors)}])),perAction:perAction.map(a=>({operation:a.operation,shapeDiffs:a.projectedShapeDifferences.length,semanticDiffs:a.semanticDifferences.length}))}));
'''
for key,value in {'__PROJECTION__':SOURCE/'arc/browser/webextras_fixture_projection.ts','__DRIVER__':SOURCE/'arc/browser/webextras_native_probe.ts','__METADATA__':SNAP/'arc/tools/arccore-1.170.0-objc.tsv','__OUT__':OUT}.items():HARNESS=HARNESS.replace(key,str(value))
harness=TEMP/'offline.ts';harness.write_text(HARNESS);shutil.copyfile(harness,OUT/'offline-harness.ts')
r=subprocess.run(['bun',str(harness)],text=True,capture_output=True);(OUT/'validation.log').write_text(r.stdout+r.stderr);assert r.returncode==0,r.stdout+r.stderr
validated=json.loads((OUT/'validation-and-projection.json').read_text());comparisons=[]
for action in validated['perAction']:
 op=action['key'];count=5 if action['operation']=='clock-snapshot' else 3 if op=='subscription' else 1
 stdout,report=fi.compare(SNAP,IMAGE,OUT/'action-recordings/trace156'/f'{op}.record.jsonl',OUT/'action-recordings/ours156'/f'{op}.record.jsonl',count,OUT/f'{op}.diff.json')
 (OUT/f'{op}.diff.txt').write_text(stdout)
 comparisons.append({'key':op,'operation':action['operation'],'firstTwoLines':stdout.splitlines()[:2],'fixtureComparison':report['fixture_comparison'],'mismatchCounts':{k:len(report[k]) for k in fi.SECTIONS},'callbackSequence':report['callback_sequence']})
fi.verify(SNAP)
checks={}
for name,entry in manifest.items():
 source_path=Path(entry['source']['path']);frozen_path=Path(entry['frozen']['path'])
 current=fi.identity(source_path);frozen=fi.identity(frozen_path)
 checks[name]={'sourceUnchanged':current==entry['source'],'frozenUnchanged':frozen==entry['frozen'],'source':current,'frozen':frozen}
 assert checks[name]['sourceUnchanged'] and checks[name]['frozenUnchanged'],name
(OUT/'integrity-verification.json').write_text(json.dumps({'allVerified':True,'checks':checks},indent=2)+'\n')
result={'scope':'OFFLINE DIAGNOSTIC ONLY; shared comparator/active drivers unchanged; no acceptance claim','completeDiagnostic':True,'fixtureCounts':{a:r['fixtureCounts'] for a,r in validated['runs'].items()},'capability':state['capability'],'perActionComparator':comparisons,'perActionShapeAndSemanticDifferences':validated['perAction'],'clockPhases':{a:r['clockPhases'] for a,r in validated['runs'].items()},'rawClockEvaluations':{a:r['rawClockEvaluations'] for a,r in validated['runs'].items()}}
(OUT/'diagnostic-summary.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps({'diagnosticComplete':True,'out':str(OUT),'fixtureCounts':result['fixtureCounts'],'perAction':[{k:v for k,v in a.items() if k!='firstTwoLines'} for a in comparisons]},indent=2))
