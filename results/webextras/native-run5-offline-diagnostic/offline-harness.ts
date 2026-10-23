
import {readFileSync,writeFileSync,mkdirSync} from 'node:fs';
import {PublicTraceProjection} from '/var/folders/2j/z4115t_51pv7sxxrq1_j7_xh0000gn/T/webextras-native5-offline-fm8v10w0/source/arc/browser/webextras_fixture_projection.ts';
const driver=readFileSync('/var/folders/2j/z4115t_51pv7sxxrq1_j7_xh0000gn/T/webextras-native5-offline-fm8v10w0/source/arc/browser/webextras_native_probe.ts','utf8');
const validators=driver.slice(driver.indexOf('type RecordRow'),driver.indexOf('const root ='));
const semantic=driver.slice(driver.indexOf('const semantic ='),driver.indexOf('const golden ='));
const api=new Function('PublicTraceProjection',new Bun.Transpiler({loader:'ts'}).transformSync(validators+'\n'+semantic+'\nreturn {parseRecords,scanRecords,validateNesting,validateActionTrace,outcomeErrors,expectedSelectors,semantic};'))(PublicTraceProjection);
const image='WebextrasNativeProbe.dylib';const metadata=readFileSync('/var/folders/2j/z4115t_51pv7sxxrq1_j7_xh0000gn/T/webextras-native5-offline-fm8v10w0/frozen-comparator/arc/tools/arccore-1.170.0-objc.tsv','utf8');const root='/Users/cryogenicplanet/general/arc-engine/results/webextras/native-run5-offline-diagnostic';
const require=(v:any,m:string)=>{if(!v)throw new Error(m)};
const runs:any={};
for(const agent of ['trace156','ours156']){
 const record=readFileSync(`${root}/frozen-recordings/${agent}/record.jsonl`);
 const observation=JSON.parse(readFileSync(`${root}/frozen-recordings/${agent}/observations.json`,'utf8'));
 const all=api.parseRecords(record);require(all[0]?.event.ev==='hooked'&&all[0]?.event.seq===0&&all[0]?.event.engine_classes>0,'hooked missing');
 require(all.filter((r:any)=>r.event.ev==='hooked').length===1,'hooked duplicate');require(new Set(all.map((r:any)=>r.event.seq)).size===all.length,'sequence duplicate');
 for(const {event} of all)if(['call','ret'].includes(event.ev))require(typeof event.from==='string'&&event.from.length>0&&event.from.trim()===event.from&&!/[\x00-\x1f]/.test(event.from),'bad caller');
 const full=api.scanRecords(all); // Existing parser: matched returns and all raw ancestry; lifecycle tail may remain open.
 const projection=new PublicTraceProjection(metadata,image);const actions:any[]=[];let calls=0,returns=0;
 mkdirSync(`${root}/action-recordings/${agent}`,{recursive:true});
 for(const original of observation.actions){
  const action=JSON.parse(JSON.stringify(original));const span=record.subarray(action.recordBegin,action.recordEnd);const rows=api.parseRecords(span,action.recordBegin);
  const projected=api.validateActionTrace(action,rows,record,projection);const fixture=rows.filter((r:any)=>r.event.dir==='->engine'&&r.event.from===image);
  const c=fixture.filter((r:any)=>r.event.ev==='call').length,t=fixture.filter((r:any)=>r.event.ev==='ret').length,e=api.expectedSelectors(action.operation).length;
  require(c===e&&t===e,`bad coverage ${agent}/${action.operation}`);calls+=c;returns+=t;
  writeFileSync(`${root}/action-recordings/${agent}/${action.operation}.record.jsonl`,span);
  action.traceShape=projected;
  const rawShape=original.traceShape??[];
  actions.push({operation:action.operation,fixtureCounts:{call:c,ret:t},rawValidation:'passed including ambient stack/receiver/forwarding/actual ancestry',outcomeErrors:api.outcomeErrors(action),rawShapeEvents:rawShape.length,projectedEvents:projected.length,projectedShape:projected,semantic:api.semantic(action)});
 }
 require(actions.length===11&&calls===13&&returns===13,'incomplete whole-pair coverage');
 runs[agent]={phase:observation.phase,cleanup:observation.cleanup,fullRows:all.length,callerValidation:'all calls/returns verified',fullRawAncestry:'all calls/returns scan passed; lifecycle tail may remain open',remainingLifecycleStack:Object.fromEntries([...full.stacks].map(([t,s]:any)=>[t,s.map((r:any)=>({seq:r.event.seq,cls:r.event.cls,sel:r.event.sel,from:r.event.from}))])),fixtureCounts:{call:calls,ret:returns},actions};
}
function differences(a:any,b:any,path=''):any[]{
 if(JSON.stringify(a)===JSON.stringify(b))return [];
 if(a&&b&&typeof a==='object'&&typeof b==='object'&&Array.isArray(a)===Array.isArray(b)){
  return [...new Set([...Object.keys(a),...Object.keys(b)])].flatMap(k=>differences(a[k],b[k],path+'/'+k));
 }
 return [{path,golden:a===undefined?'<undefined>':a,ours:b===undefined?'<undefined>':b}];
}
const perAction=[];
for(let i=0;i<11;i++){
 const l=runs.trace156.actions[i],r=runs.ours156.actions[i];require(l.operation===r.operation,'order mismatch');
 perAction.push({operation:l.operation,projectedShapeEqual:JSON.stringify(l.projectedShape)===JSON.stringify(r.projectedShape),projectedShapeDifferences:differences(l.projectedShape,r.projectedShape),semanticDifferences:differences(l.semantic,r.semantic)});
}
writeFileSync(`${root}/validation-and-projection.json`,JSON.stringify({scope:'held TEMP proposal offline diagnostic, not acceptance',runs,perAction},null,2));
console.log(JSON.stringify({runs:Object.fromEntries(Object.entries(runs).map(([a,r]:any)=>[a,{fullRows:r.fullRows,fixtureCounts:r.fixtureCounts,outcomeErrors:r.actions.flatMap((a:any)=>a.outcomeErrors)}])),perAction:perAction.map(a=>({operation:a.operation,shapeDiffs:a.projectedShapeDifferences.length,semanticDiffs:a.semanticDifferences.length}))}));
