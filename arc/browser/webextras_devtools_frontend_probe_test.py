#!/usr/bin/env python3
"""Actual owned frontend probe body tests; no app, lock, shared build or runtime launch."""
from pathlib import Path
import json
import importlib.util
import subprocess
import tempfile
import unittest

ROOT=Path(__file__).resolve().parents[2]
DRIVER=ROOT/'arc/browser/webextras_native_probe.ts'
NATIVE=ROOT/'arc/browser/webextras_native_probe.m'


class FrontendProbeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp=tempfile.TemporaryDirectory(prefix='webextras-frontend-probe-test-')
        cls.directory=Path(cls.temp.name)

    @classmethod
    def tearDownClass(cls):
        cls.temp.cleanup()

    def bun(self,body):
        path=self.directory/(self._testMethodName+'.ts')
        path.write_text('''import {readFileSync} from 'node:fs';
const source=readFileSync(%s,'utf8');
const bodies=source.slice(source.indexOf('type RecordRow'),source.indexOf('const root ='));
const api=new Function(new Bun.Transpiler({loader:'ts'}).transformSync(bodies+`;return{frontendCandidates,genuineFrontendURL,frontendProducerJS,frontendRuntimeEvaluation,validateFrontendCallbackSpan,runGenuineFrontendEvents,frontendEventPolicy,clockModeFromArgs,validateFrontendCallbackIdentity,frontendCallbackMetadata};`))();
const callbacksMetadata=api.frontendCallbackMetadata(readFileSync("/Users/cryogenicplanet/general/arc-engine/tests/recorder/app_callbacks.inc",'utf8'));
const callbackIdentity=(selector:string)=>({rawSelector:selector,selectorIdentityValid:true,selectorIdentityKind:'public',recorderAliasVerified:false,
 ownerClass:'ArcWebContents',ownerRecordClass:'ArcWebContents',ownerReportedClass:'ArcWebContents',ownerIdentityValid:true,ownerIdentityKind:'exact',ownerClassChain:['ArcWebContents','NSObject'],
 abi:['webContentsDevTools:inspectModeDidChange:','webContentsDevTools:displayAs:'].includes(selector)?'v32@0:8@16Q24':'v24@0:8@16'});
const check=(v:any,message:string)=>{if(!v)throw Error(message)};
const rejects=(f:()=>void,message:string)=>{let bad=false;try{f()}catch{bad=true}check(bad,message)};
const execute=(expression:string,SDK:any,UI:any,Common:any)=>{
 const mocked=expression.replace("await import(new URL('./core/sdk/sdk.js',location.href).href)",'mockSDK').replace("await import(new URL('./ui/legacy/legacy.js',location.href).href)",'mockUI').replace("await import(new URL('./core/common/common.js',location.href).href)",'mockCommon');
 return new Function('mockSDK','mockUI','mockCommon','performance','return '+mocked)(SDK,UI,Common,{now:()=>1});
};
'''%json.dumps(str(DRIVER))+body)
        result=subprocess.run(['bun',str(path)],capture_output=True,text=True)
        self.assertEqual(result.returncode,0,result.stderr)

    def test_actual_genuine_producer_calls_actions_and_dock_and_rechecks_identity(self):
        self.bun(r'''
const fixture='http://127.0.0.1/owned/fixture',calls:any[]=[];let toggled=false,side='right',url=fixture,swapped=false;
const manager={primaryPageTarget:()=>({inspectedURL:()=>swapped?'http://wrong':url,id:()=> 'owned-page'}),allTargetsSuspended:()=>false};
const action={enabled:()=>true,toggled:()=>toggled,execute:async()=>{calls.push('execute');toggled=!toggled;return true}};
const dock={canDock:()=>true,dockSide:()=>side,setDockSide:(value:string)=>{calls.push(['dock',value]);side=value}};
const SDK={TargetManager:{TargetManager:{instance:()=>manager}}},UI={ActionRegistry:{ActionRegistry:{instance:()=>({getAction:(id:string)=>{check(id==='elements.toggle-element-search','wrong global action');return action}})}},DockController:{DockController:{instance:()=>dock}}},Common={Settings:{Settings:{instance:()=>({moduleSetting:(name:string)=>{check(name==='show-ua-shadow-dom','wrong setting');return{get:()=>false}}})}}};
for(const [kind,requested] of [['read',undefined],['inspect-on',undefined],['inspect-off',undefined],['dock-change','bottom'],['dock-restore','right']] as const){
 const result=await execute(api.frontendProducerJS(fixture,kind,requested),SDK,UI,Common);
 check(result.before.primaryURLMatches&&result.after.primaryURLMatches,'lost exact primary target');
 if(kind==='inspect-on')check(result.acknowledged===true&&result.after.actionToggled,'inspect did not use real action');
 if(kind==='inspect-off')check(!result.after.actionToggled,'inspect restore wrong');
 if(requested)check(result.after.dockSide===requested,'dock did not use real controller');
}
check(JSON.stringify(calls)===JSON.stringify(['execute','execute',['dock','bottom'],['dock','right']]),'producer extra/fake mutation');
url='http://wrong';let bad=false;try{await execute(api.frontendProducerJS(fixture,'inspect-on'),SDK,UI,Common)}catch{bad=true}check(bad&&calls.length===4,'wrong primary target mutated');
url=fixture;action.execute=async()=>{toggled=true;swapped=true;return true};bad=false;try{await execute(api.frontendProducerJS(fixture,'inspect-on'),SDK,UI,Common)}catch{bad=true}check(bad,'changed primary target accepted');
const expression=api.frontendProducerJS(fixture,'inspect-on');
for(const forbidden of ['Overlay.setInspectMode','invoke_setInspectMode','inspectModeChanged(','requestDockPosition(','DevToolsHost','setFocusEmulationEnabled','dispatchMouseEvent','createTarget'])check(!expression.includes(forbidden),'producer bypass '+forbidden);
''')

    def test_candidates_exact_new_internal_url_and_mode_exclusion(self):
        self.bun(r'''
const before=[{targetId:'old',url:'devtools://devtools/bundled/devtools_app.html'}];
const after=[...before,{targetId:'new',type:'other',url:'devtools://devtools/bundled/devtools_app.html?panel=console'},
 {targetId:'fake',url:'http://devtools/devtools_app.html'},{targetId:'remote',url:'https://host/devtools_app.html'}];
check(api.frontendCandidates(before,after).length===1&&api.frontendCandidates(before,after)[0].targetId==='new','first/old/fake target admitted');
rejects(()=>api.frontendCandidates(before,[...after,after[1]]),'duplicate candidate accepted');
rejects(()=>api.frontendCandidates([],Array.from({length:9},(_,index)=>({targetId:String(index),url:'devtools://devtools/bundled/devtools_app.html'}))),'unbounded candidates');
check(api.clockModeFromArgs(['--devtools-frontend-events'])==='devtools-frontend-events','frontend mode missing');
for(const flags of [['--devtools-frontend-events','--media-clock'],['--media-service-clock','--devtools-frontend-events'],['--devtools-frontend-events','--devtools-frontend-events']])rejects(()=>api.clockModeFromArgs(flags),'ambiguous mode accepted');
''')

    def test_actual_orchestration_positive_and_missing_ambiguous_noop_wrongmode_controls(self):
        self.bun(r'''
for(const failure of ['none','missing-target','ambiguous','no-callback','wrong-mode','noop','no-response','cannot-dock','target-replaced','late-callbacks','disarm-late','read-late','save-late','bad-alias','bad-owner','bad-abi']){
 const fixture='http://127.0.0.1/owned/fixture',candidate={targetId:'front',type:'other',url:'devtools://devtools/bundled/devtools_app.html?panel=console'};
 let now=0,toggled=false,side='right',observation:any,executeCount=0,replaced=false,readCount=0,saveDelayed=false;const raw:any[]=[],requests:any[]=[],saved:any[]=[];
 const callbacks=()=>observation?.callbacks;
 const emit=(selector:string,mode:number)=>{if(observation&&failure!=='no-callback'){
 const event:any={...callbackIdentity(selector),selector,mode:failure==='wrong-mode'?4:mode};
 if(failure==='bad-alias'){event.rawSelector='__arcrec___arcrec_'+selector;event.selectorIdentityValid=false;}
 if(failure==='bad-owner')event.ownerReportedClass='Foreign';
 if(failure==='bad-abi')event.abi='v32@0:8@16B24';callbacks().push(event);
 }};
 const manager={primaryPageTarget:()=>({inspectedURL:()=>fixture,id:()=> 'owned-page'}),allTargetsSuspended:()=>false};
 const action={enabled:()=>true,toggled:()=>toggled,execute:async()=>{executeCount++;if(failure!=='noop')toggled=!toggled;emit('webContentsDevTools:inspectModeDidChange:',toggled?1:0);return true}};
 const dock={canDock:()=>failure!=='cannot-dock',dockSide:()=>side,setDockSide:(value:string)=>{side=value;if(failure==='target-replaced')replaced=true;emit('webContentsDevTools:displayAs:',value==='right'?3:4)}};
 const SDK={TargetManager:{TargetManager:{instance:()=>manager}}},UI={ActionRegistry:{ActionRegistry:{instance:()=>({getAction:()=>action})}},DockController:{DockController:{instance:()=>dock}}},Common={Settings:{Settings:{instance:()=>({moduleSetting:()=>({get:()=>false})})}}};
 const io={verify:()=>{},now:()=>now,sleep:async(ms:number)=>{check(ms===100,'poll bound changed');now+=ms},spanID:(kind:string)=>kind,
  save:(value:any)=>{saved.push(structuredClone(value));if(failure==='save-late'&&!saveDelayed&&value.spans?.[0]?.samples?.length===2){saveDelayed=true;now+=200}},raw:(value:any)=>raw.push(value),
  census:async()=>failure==='missing-target'?[]:failure==='ambiguous'?[candidate,{...candidate,targetId:'front2'}]:[candidate],
  request:async(method:string,params:any,sessionId?:string)=>{requests.push({method,params,sessionId});
   if(method==='Target.getTargetInfo'){if(replaced)throw Error('original frontend destroyed');return{targetInfo:{...candidate,targetId:params.targetId}}}
   if(method==='Target.attachToTarget'){check(params.flatten===true,'session not flattened');return{sessionId:'session-'+params.targetId}}
   if(method==='Target.detachFromTarget')return{};
   check(method==='Runtime.evaluate'&&sessionId?.startsWith('session-'),'non-session/non-runtime stimulus');
   if(failure==='no-response')throw Error('mock missing response');
   return{result:{value:await execute(params.expression,SDK,UI,Common)}};
  },action:async(operation:string,extra:any)=>{
   if(operation==='frontend-observe-arm')observation={spanID:extra.spanID,callbacks:[],forwarding:0,recordBegin:0,recordEnd:1,overflow:false,startedAt:now};
   if(['read-late','save-late'].includes(failure)&&operation==='frontend-observe-read'){now=(++readCount)===1?4800:failure==='read-late'?5100:4900;}
   if(failure==='late-callbacks'&&operation==='frontend-observe-read'&&observation.spanID==='inspect-on'){for(const delay of [200,300])if(now-observation.startedAt>=delay&&!observation['late'+delay]){observation['late'+delay]=true;observation.callbacks.push({...callbackIdentity('webContentsDevToolsNeedsFocus:'),selector:'webContentsDevToolsNeedsFocus:',mode:0});}}
   if(failure==='disarm-late'&&operation==='frontend-observe-disarm'&&observation.spanID==='inspect-on')observation.callbacks.push({...callbackIdentity('webContentsDevToolsNeedsFocus:'),selector:'webContentsDevToolsNeedsFocus:',mode:0});
   const result={ok:true,ownerTag:1,publicCalls:[],timeline:[],observation:structuredClone(observation)};
   if(operation==='frontend-observe-disarm')observation=undefined;return result;
  }};
 const report=await api.runGenuineFrontendEvents(io,[],fixture,1);
 if(['none','late-callbacks'].includes(failure))check(report.status==='observed'&&report.spans.length===4&&executeCount===2&&!toggled&&side==='right'&&report.spans.every((span:any)=>span.status==='observed'),'genuine positive orchestration failed');
 else check(report.status!=='observed','bad diagnostic accepted: '+failure);
 if(failure==='no-callback')check(report.error.includes('no app callback observed'),'no-callback diagnostic is ambiguous');
 if(failure==='wrong-mode')check(report.error.includes('declared mode absent'),'wrong-mode diagnostic is ambiguous');
 for(const [name,message] of [['bad-alias','selector identity'],['bad-owner','owner identity'],['bad-abi','ABI mismatch']])if(failure===name)check(report.error.includes(message),'identity/type diagnostic is ambiguous');
 if(failure==='save-late')check(report.status==='inconclusive'&&saveDelayed&&report.spans[0].quietComplete!==true&&report.spans[0].samples.at(-1).hostElapsedMilliseconds===4900,'durable save manufactured quiet time');
 if(failure==='read-late')check(report.status==='inconclusive'&&report.spans[0].waitExpired&&report.spans[0].quietComplete!==true&&report.spans[0].samples.at(-1).hostElapsedMilliseconds===5100,'late response accepted or raw sample dropped');
 if(failure==='disarm-late')check(report.status==='inconclusive'&&report.spans[0].quietInvalidatedAtDisarm&&report.spans[0].quietComplete===false&&report.spans[0].observation.callbacks.length===2,'last-read/disarm callback discarded or claimed quiet');
 if(failure==='late-callbacks')check(report.spans[0].quietComplete&&report.spans[0].quietElapsedMilliseconds>=500&&report.spans[0].observation.callbacks.length===3,'late callback rounds truncated by first-match quiet timer');
 if(['missing-target','ambiguous','no-response'].includes(failure))check(executeCount===0,'unverified frontend mutated');
 check(!toggled&&(side==='right'||failure==='target-replaced'),'mutation not restored after '+failure);
 if(failure==='target-replaced')check(report.status==='inconclusive'&&report.restoration.some((entry:any)=>entry.error)&&report.spans.length===3,'destroyed frontend was substituted or restore invented');
 check(observation===undefined,'observation left armed');
 check(requests.every(request=>!request.method.startsWith('Overlay.')),'Overlay protocol substitute');
 if(failure==='no-response')check(raw.length>0&&raw.every(value=>value.status==='transport-error'),'missing response not persisted');
}
''')

    def test_actual_runtime_exception_saved_before_assertion(self):
        self.bun(r'''
const saved:any[]=[],raw={result:{subtype:'error',description:'real exception'},exceptionDetails:{text:'Uncaught',exception:{description:'exact original failure'}}};
let rejected=false;try{await api.frontendRuntimeEvaluation(async()=>raw,record=>saved.push(record),{label:'inspect-on',expression:'exact producer'})}catch{rejected=true}
check(rejected&&saved.length===1&&saved[0].rawResult===raw&&saved[0].expression==='exact producer','exception accepted/lost');
check(BigInt(saved[0].hostAfterNanos)>=BigInt(saved[0].hostBeforeNanos),'host bracket reversed');
''')

    def test_actual_callback_span_zero_public_inner_outer_and_negative_controls(self):
        self.bun(r'''
const image='WebextrasNativeProbe.dylib',service='<ArcWebContentsDevTools#9>',common={thread:'main',dir:'engine->app',kind:'-',cls:'ADK.WebContentChromium',self:'<ADK.WebContentChromium#8>',from:'engine'};
const header=JSON.stringify({ev:'hooked',seq:0,engine_classes:1})+'\n'+JSON.stringify({ev:'call',seq:1,thread:'main',depth:0,dir:'->engine',kind:'-',cls:'ArcBrowserApplication',sel:'run',self:'<ArcBrowserApplication#1>',args:[],from:'app'})+'\n';
function fixture(nested=false){
 const events:any[]=[{...common,ev:'call',seq:2,depth:1,sel:'webContentsDevTools:inspectModeDidChange:',args:[service,1]}];
 if(nested)events.push({...common,ev:'call',seq:3,depth:2,sel:'webContentsDevToolsNeedsFocus:',args:[service]},{...common,ev:'ret',seq:4,depth:2,sel:'webContentsDevToolsNeedsFocus:',ret:null});
 events.push({...common,ev:'ret',seq:nested?5:3,depth:1,sel:'webContentsDevTools:inspectModeDidChange:',ret:null});
 const lines=events.map(e=>JSON.stringify(e)+'\n');let offset=Buffer.byteLength(header);const rows=lines.map(line=>{const row={begin:offset,end:offset+Buffer.byteLength(line)};offset=row.end;return row});
 const callback=(index:number,end:number,depth:number)=>({...callbackIdentity(events[index].sel),selector:events[index].sel,mode:index?0:1,ownerTag:1,ownerClass:'ArcWebContents',serviceClass:'ArcWebContentsDevTools',delegateClass:'ADK.WebContentChromium',abi:index?'v24@0:8@16':'v32@0:8@16Q24',mainThread:true,ordinal:index?2:1,forwarded:true,forwardDepth:depth,recordOffset:rows[index].begin,recordAfterForwarding:rows[end].end,nativeBeforeTicks:'100',nativeAfterTicks:'200'});
 const record=Buffer.from(header+lines.join(''));
 return{record,span:{ok:true,quietComplete:true,ownerTag:1,spanID:'s',kind:'inspect-on',producer:{kind:'inspect-on',acknowledged:true,before:{primaryURLMatches:true,primaryTargetID:'owned',suspended:false,actionEnabled:true,actionToggled:false,uaShadow:false},after:{primaryURLMatches:true,primaryTargetID:'owned',actionToggled:true}},afterAcknowledgment:{kind:'read',after:{primaryURLMatches:true,primaryTargetID:'owned',actionToggled:true}},expectedSelector:'webContentsDevTools:inspectModeDidChange:',expectedMode:1,publicCalls:[],timeline:[],observation:{spanID:'s',ownerTag:1,recordBegin:Buffer.byteLength(header),recordEnd:record.length,callbacks:[callback(0,events.length-1,1),...(nested?[callback(1,2,2)]:[])],forwarding:0,overflow:false}},rows};
}
const good=fixture();api.validateFrontendCallbackSpan(good.span,good.record,service,1,image,undefined,callbacksMetadata);
const alias=fixture();Object.assign(alias.span.observation.callbacks[0],{rawSelector:'__arcrec_webContentsDevTools:inspectModeDidChange:',selectorIdentityKind:'recorder-alias',recorderAliasVerified:true,recorderAliasSourceSHA256:'9ae39687fdebcf4115b8c974af1b799aeee65265185a2c512ff56e5ca287d81e',ownerClass:'NSKVONotifying_ArcWebContents',ownerClassChain:['NSKVONotifying_ArcWebContents','ArcWebContents','NSObject'],ownerIdentityKind:'kvo'});api.validateFrontendCallbackSpan(alias.span,alias.record,service,1,image,undefined,callbacksMetadata);
const nested=fixture(true);api.validateFrontendCallbackSpan(nested.span,nested.record,service,1,image,undefined,callbacksMetadata);
const outer=fixture();outer.span.observation.callbacks[0].recordOffset=outer.rows[0].end;outer.span.observation.callbacks[0].recordAfterForwarding=outer.rows[1].begin;api.validateFrontendCallbackSpan(outer.span,outer.record,service,1,image,undefined,callbacksMetadata);
for(const change of [
 (x:any)=>x.span.observation.callbacks[0].rawSelector='__arcrec___arcrec_webContentsDevTools:inspectModeDidChange:',
 (x:any)=>x.span.observation.callbacks[0].rawSelector='__arcrec_webContentsDevTools:displayAs:',
 (x:any)=>x.span.observation.callbacks[0].selectorIdentityValid=false,
 (x:any)=>x.span.observation.callbacks[0].recorderAliasVerified=true,
 (x:any)=>{const e=x.span.observation.callbacks[0];e.rawSelector='__arcrec_'+e.selector;e.selectorIdentityKind='recorder-alias';e.recorderAliasVerified=true;e.recorderAliasSourceSHA256='unverified';},
 (x:any)=>x.span.observation.callbacks[0].ownerClass='NSKVONotifying_Foreign',
 (x:any)=>x.span.observation.callbacks[0].ownerClassChain=['ArcWebContents','ArcWebContents'],
 (x:any)=>x.span.observation.callbacks[0].ownerReportedClass='Foreign',
 (x:any)=>x.span.observation.callbacks[0].ownerIdentityValid=false,
 (x:any)=>x.span.observation.callbacks=[],(x:any)=>x.span.quietComplete=false,(x:any)=>x.span.publicCalls=[{}],(x:any)=>x.span.observation.recordBegin++,
 (x:any)=>x.span.observation.callbacks[0].recordOffset++, (x:any)=>x.span.observation.callbacks[0].recordAfterForwarding--,
 (x:any)=>x.span.observation.callbacks[0].forwarded=false,(x:any)=>x.span.observation.callbacks[0].mainThread=false,
 (x:any)=>x.span.observation.callbacks[0].ownerTag=2,(x:any)=>x.span.observation.callbacks[0].abi='v32@0:8@16B24',
 (x:any)=>x.span.observation.callbacks[0].forwardDepth=2,(x:any)=>x.span.expectedMode=2,
 (x:any)=>x.span.producer.after.actionToggled=false,(x:any)=>x.span.afterAcknowledgment.after.primaryTargetID='retarget',
 (x:any)=>x.record=Buffer.from(x.record.toString().replaceAll('"from":"engine"','"from":"app"')),
 (x:any)=>x.record=Buffer.from(x.record.toString().replaceAll(service,'<ArcWebContentsDevTools#2>')),
 (x:any)=>x.record=Buffer.from(x.record.toString().replaceAll('"dir":"engine->app"','"dir":"->engine"').replaceAll('"from":"engine"','"from":"'+image+'"')),
]){const x=fixture();change(x);rejects(()=>api.validateFrontendCallbackSpan(x.span,x.record,service,1,image,undefined,callbacksMetadata),'bad callback evidence accepted')}
''')

    def test_actual_frontend_mode_acceptance_keeps_baseline_and_pending_gate(self):
        source=DRIVER.read_text()
        start=source.index('const boundaryDiffs: unknown[] = [];')
        body=source[start:source.index('writeFileSync(join(out, "summary.json")',start)]
        self.bun(r'''
const devToolsFrontendEvents=true,anyClock=false,frontendEventDifferences=[],fixtureVerified=false;
const fixtureComparisonPending='pending fixture-comparison support: no explicit fixture mode',fixtureImage='WebextrasNativeProbe.dylib';
const agents=['trace156','ours156'],actionManifest=Array.from({length:11},(_,index)=>({key:String(index),operation:'baseline'}));
const runs=agents.map(agent=>({agent,actions:actionManifest.map(()=>({traceShape:[{}]})),errors:[fixtureComparisonPending],cleanup:'Browser.close',frontendEvents:{rawValidated:true}}));
const golden=runs[0],ours=runs[1],differences=[],out='unused',comparisonHelper='unused',comparisonSnapshot='unused';
const join=(...parts:string[])=>parts.join('/'),existsSync=()=>true,expectedSelectors=()=>['unused'];
const json=()=>{throw Error('unsupported comparator report loaded')},writeFileSync=()=>{throw Error('zero/public comparator invoked')};
__BODY__
check(complete===false&&parity===false&&boundaryDiffs.length===1,'pending fixture support accepted as parity');
const manifest=source.slice(source.indexOf('const actionManifest ='),source.indexOf('try {',source.indexOf('const actionManifest =')));
check(manifest.includes('.concat(anyClock ?')&&!manifest.includes('devToolsFrontendEvents'),'frontend observations added public fixture calls');
'''.replace('__BODY__',body))

    def test_native_actual_forwarding_reentry_disarm_lifetime_and_no_public_work(self):
        path=self.directory/'native.m'
        path.write_text('#import '+json.dumps(str(NATIVE))+r'''
@interface ArcWebContents:NSObject @end
@implementation ArcWebContents @end
@interface ArcWebContentsDevTools:NSObject @end
@implementation ArcWebContentsDevTools @end
@interface MockDelegate:NSObject
-(void)webContentsDevTools:(id)tools inspectModeDidChange:(NSUInteger)mode;
-(void)webContentsDevTools:(id)tools displayAs:(NSUInteger)mode;
-(void)webContentsDevToolsDidClose:(id)tools;
-(void)webContentsDevToolsNeedsFocus:(id)tools;
@end
@implementation MockDelegate
-(void)webContentsDevTools:(id)tools inspectModeDidChange:(NSUInteger)mode{(void)tools;(void)mode;}
-(void)webContentsDevTools:(id)tools displayAs:(NSUInteger)mode{(void)tools;(void)mode;}
-(void)webContentsDevToolsDidClose:(id)tools{(void)tools;}
-(void)webContentsDevToolsNeedsFocus:(id)tools{(void)tools;}
@end
static int inspectCount,focusCount,displayCount,closeCount;
static WebextrasProbeEntry* entry;static WebextrasProbeWork* work;
static NSDictionary* Identity(NSString* span){return @{@"spanID":span,@"ownerTag":@1,@"observationNonce":@"arc-webextras-probe-test",@"url":gExpectedURL};}
static void Check(BOOL value,NSString* message){if(!value){NSLog(@"%@",message);exit(1);}}
static void FocusIMP(id receiver,SEL selector,id tools){(void)receiver;(void)selector;(void)tools;++focusCount;}
static void InspectIMP(id receiver,SEL selector,id tools,NSUInteger mode){(void)selector;(void)mode;++inspectCount;
 FrontendFocus(receiver,@selector(webContentsDevToolsNeedsFocus:),tools);
 Check(FrontendObservationOperation(work,entry,Identity(@"a"),@"frontend-observe-disarm")!=nil,@"nested disarm succeeded while forwarding");}
static void DisplayIMP(id receiver,SEL selector,id tools,NSUInteger mode){(void)receiver;(void)selector;(void)tools;(void)mode;++displayCount;}
static void CloseIMP(id receiver,SEL selector,id tools){(void)receiver;(void)selector;(void)tools;++closeCount;}
int main(void){@autoreleasepool{
 gFrontendEvents=YES;gFrontendLock=[NSObject new];gRecord=@"/dev/null";gExpectedURL=@"http://127.0.0.1:1/arc-webextras-probe-test/fixture";
 gInspect=(IMP)InspectIMP;gFocus=(IMP)FocusIMP;gDisplay=(IMP)DisplayIMP;gClose=(IMP)CloseIMP;
 ArcWebContents* owner=[ArcWebContents new];ArcWebContentsDevTools* tools=[ArcWebContentsDevTools new];MockDelegate* delegate=[MockDelegate new];
 entry=[WebextrasProbeEntry new];entry.owner=owner;entry.tools=tools;entry.identifier=1;
 work=[WebextrasProbeWork new];work.response=[NSMutableDictionary dictionary];work.publicCalls=[NSMutableArray array];work.timeline=[NSMutableArray array];
 @autoreleasepool{
 Check(!FrontendObservationOperation(work,entry,Identity(@"a"),@"frontend-observe-arm"),@"arm failed");
 FrontendInspect(delegate,@selector(webContentsDevTools:inspectModeDidChange:),tools,1);
 FrontendDisplay(delegate,@selector(webContentsDevTools:displayAs:),tools,3);
 FrontendClosed(delegate,@selector(webContentsDevToolsDidClose:),tools);
 Check(inspectCount==1&&focusCount==1&&displayCount==1&&closeCount==1,@"original IMP not forwarded exactly once");
 Check(!FrontendObservationOperation(work,entry,Identity(@"a"),@"frontend-observe-read"),@"read failed");
 NSArray* callbacks=work.response[@"observation"][@"callbacks"];
 Check(callbacks.count==4&&[callbacks[0][@"forwardDepth"] isEqual:@1]&&[callbacks[1][@"forwardDepth"] isEqual:@2],@"nested callback depth lost");
 for(NSDictionary* callback in callbacks)Check([callback[@"forwarded"] boolValue]&&[callback[@"ownerTag"] isEqual:@1],@"forward return/association lost");
 Check(work.publicCalls.count==0&&work.timeline.count==0,@"observation performed public work");
 Check(FrontendObservationOperation(work,entry,Identity(@"stale"),@"frontend-observe-disarm")!=nil,@"stale disarm accepted");
 Check(!FrontendObservationOperation(work,entry,Identity(@"a"),@"frontend-observe-disarm")&&!gFrontendObservation,@"disarm failed");
 }
 @autoreleasepool{Check(!FrontendObservationOperation(work,entry,Identity(@"b"),@"frontend-observe-arm"),@"rearm failed");}
 __weak id weakOwner=owner;owner=nil;Check(!weakOwner,@"observation retained owner");
 @autoreleasepool{FrontendFocus(delegate,@selector(webContentsDevToolsNeedsFocus:),tools);}
 Check(focusCount==2&&gFrontendObservation.callbacks.count==0&&gFrontendObservation.ignored==1,@"dead owner event captured or original suppressed");
 entry.tools=nil;tools=nil;entry=nil;Check(!gFrontendObservation.entry,@"journal retained weak canonical entry");
 // Exercise actual Poll after all weak matches vanish, including an expired journal.
 gRequest=@__REQUEST__;gInstalled=YES;gWork=nil;gLastID=nil;gEntries=[NSMapTable weakToStrongObjectsMapTable];
 gFrontendObservation.deadline=NSDate.timeIntervalSinceReferenceDate-1;
 NSArray* badChanges=@[@{@"ownerTag":@2},@{@"ownerTag":@YES},@{@"ownerTag":@1.5},@{@"spanID":@"wrong"},@{@"observationNonce":@"wrong"}];int serial=0;
 for(NSDictionary* change in badChanges){NSMutableDictionary* request=[Identity(@"b") mutableCopy];[request addEntriesFromDictionary:change];request[@"id"]=[NSString stringWithFormat:@"bad-%d",++serial];request[@"operation"]=@"frontend-observe-disarm";WriteJSON(request,gRequest);Poll();
  NSDictionary* response=[NSJSONSerialization JSONObjectWithData:[NSData dataWithContentsOfFile:[gRequest stringByAppendingString:@".response.json"]] options:0 error:NULL];
  Check(![response[@"ok"] boolValue]&&gFrontendObservation&&[response[@"publicCalls"] count]==0,@"mismatched dead-owner cleanup accepted/called engine");}
 for(NSString* operation in @[@"frontend-observe-read",@"frontend-observe-disarm"]){NSMutableDictionary* request=[Identity(@"b") mutableCopy];request[@"id"]=operation;request[@"operation"]=operation;WriteJSON(request,gRequest);Poll();
  NSDictionary* response=[NSJSONSerialization JSONObjectWithData:[NSData dataWithContentsOfFile:[gRequest stringByAppendingString:@".response.json"]] options:0 error:NULL];
  Check([response[@"ok"] boolValue]&&[response[@"ownerTag"] isEqual:@1]&&[response[@"matches"] isEqual:@0]&&[response[@"observation"][@"expired"] boolValue]&&[response[@"publicCalls"] count]==0&&[response[@"timeline"] count]==0,@"actual Poll cannot clean expired dead-owner journal");}
 Check(!gFrontendObservation,@"actual Poll failed to release dead-owner journal");
 NSMutableDictionary* newArm=[Identity(@"c") mutableCopy];newArm[@"id"]=@"new-arm-after-loss";newArm[@"operation"]=@"frontend-observe-arm";WriteJSON(newArm,gRequest);Poll();
 NSDictionary* lost=[NSJSONSerialization JSONObjectWithData:[NSData dataWithContentsOfFile:[gRequest stringByAppendingString:@".response.json"]] options:0 error:NULL];
 Check(![lost[@"ok"] boolValue]&&!gFrontendObservation&&[lost[@"publicCalls"] count]==0,@"cleanup route admitted new arming without a live owner");
 puts("actual wrappers forward once; nested depth/active-disarm/weak owner/zero public work passed");
}return 0;}
'''.replace('__REQUEST__',json.dumps(str(self.directory/'webextras-request.json'))))
        binary=self.directory/'native'
        result=subprocess.run(['xcrun','clang','-fobjc-arc','-Wall','-Wextra','-Werror','-framework','Foundation','-framework','AppKit',str(path),'-o',str(binary)],capture_output=True,text=True)
        self.assertEqual(result.returncode,0,result.stderr)
        result=subprocess.run([str(binary)],capture_output=True,text=True)
        self.assertEqual(result.returncode,0,result.stderr)

    def test_native_actual_recorder_alias_interposition_and_real_kvo_identity(self):
        import hashlib
        recorder=(ROOT/'tests/recorder/ArcEngineRecorder.m').read_text()
        self.assertEqual(hashlib.sha256(recorder.encode()).hexdigest(),'9ae39687fdebcf4115b8c974af1b799aeee65265185a2c512ff56e5ca287d81e')
        keys=recorder[recorder.index('static NSString* ClassKey('):recorder.index('// Who called:')]
        forwarding=recorder[recorder.index('static void RecForwardInvocation('):recorder.index('static BOOL Skip(')]
        path=self.directory/'alias-kvo.m'
        path.write_text('#import '+json.dumps(str(NATIVE))+r"""
extern void _objc_msgForward(void);
static unsigned long long gSeq;static __thread int tDepth;
static NSMutableDictionary* gHooks;static NSMutableDictionary* gPrevForward;static NSMutableArray* emitted;
static NSString* Quote(NSString* value){NSData* data=[NSJSONSerialization dataWithJSONObject:@[value] options:0 error:NULL];NSString* json=[[NSString alloc]initWithData:data encoding:NSUTF8StringEncoding];return [json substringWithRange:NSMakeRange(1,json.length-2)];}
static NSString* ThreadName(void){return @"main";}
static NSString* Caller(void){return @"engine";}
static NSString* Describe(id object,int depth){(void)object;(void)depth;return @"\"<mock-object>\"";}
static NSString* DescribeValue(const char* type,void* buffer){if(*type=='Q')return [NSString stringWithFormat:@"%lu",*(unsigned long*)buffer];return @"\"<mock-object>\"";}
static void Emit(NSString* line){[emitted addObject:line];}
__KEYS__
__FORWARDING__
@interface ArcWebContents:NSObject
@property NSInteger observed;
@end
@implementation ArcWebContents @end
@interface ArcWebContentsDevTools:NSObject @end
@implementation ArcWebContentsDevTools @end
@interface AliasDelegate:NSObject
-(void)webContentsDevTools:(id)tools inspectModeDidChange:(NSUInteger)mode;
@end
static NSUInteger originalCalls;static SEL originalSelector;
@implementation AliasDelegate
-(void)webContentsDevTools:(id)tools inspectModeDidChange:(NSUInteger)mode{(void)tools;(void)mode;++originalCalls;originalSelector=_cmd;}
@end
@interface TestKVOObserver:NSObject @end
@implementation TestKVOObserver
-(void)observeValueForKeyPath:(NSString*)key ofObject:(id)object change:(NSDictionary*)change context:(void*)context{(void)key;(void)object;(void)change;(void)context;}
@end
static void Check(BOOL condition,NSString* message){if(!condition){NSLog(@"%@",message);exit(1);}}
static NSDictionary* Last(void){return gFrontendObservation.callbacks.lastObject;}
int main(void){@autoreleasepool{
 gFrontendEvents=YES;gFrontendLock=[NSObject new];gRecord=@"/dev/null";gExpectedURL=@"http://127.0.0.1:1/arc-webextras-probe-alias/fixture";
 ArcWebContents* owner=[ArcWebContents new];ArcWebContentsDevTools* tools=[ArcWebContentsDevTools new];AliasDelegate* delegate=[AliasDelegate new];TestKVOObserver* observer=[TestKVOObserver new];
 WebextrasProbeEntry* entry=[WebextrasProbeEntry new];entry.owner=owner;entry.tools=tools;entry.identifier=1;
 WebextrasProbeWork* work=[WebextrasProbeWork new];work.response=[NSMutableDictionary dictionary];work.publicCalls=[NSMutableArray array];work.timeline=[NSMutableArray array];
 NSDictionary* request=@{@"spanID":@"alias",@"ownerTag":@1,@"observationNonce":@"arc-webextras-probe-alias",@"url":gExpectedURL};
 Check(!FrontendObservationOperation(work,entry,request,@"frontend-observe-arm"),@"arm failed");
 Class cls=AliasDelegate.class;SEL public=@selector(webContentsDevTools:inspectModeDidChange:),alias=AliasFor(public);Method method=class_getInstanceMethod(cls,public);const char* types=method_getTypeEncoding(method);
 gInspect=method_getImplementation(method);
 // Install the actual probe wrapper, then recorder forwarding/alias around it.
 class_replaceMethod(cls,public,(IMP)FrontendInspect,types);
 Check(class_addMethod(cls,alias,(IMP)FrontendInspect,types),@"alias installation failed");
 gHooks=[NSMutableDictionary dictionary];gPrevForward=[NSMutableDictionary dictionary];emitted=[NSMutableArray array];gHooks[HookKey(cls,public)]=@"engine->app";
 class_replaceMethod(cls,@selector(forwardInvocation:),(IMP)RecForwardInvocation,"v24@0:8@16");
 class_replaceMethod(cls,public,(IMP)_objc_msgForward,types);
 [owner addObserver:observer forKeyPath:@"observed" options:0 context:NULL];
 Check(object_getClass(owner)!=ArcWebContents.class&&[owner class]==ArcWebContents.class,@"Foundation did not create genuine KVO subclass");
 [delegate webContentsDevTools:tools inspectModeDidChange:1];
 Check(originalCalls==1&&originalSelector==alias,@"original IMP not forwarded once with incoming recorder alias");
 NSDictionary* event=Last();Check([event[@"rawSelector"] isEqual:NSStringFromSelector(alias)]&&[event[@"selector"] isEqual:NSStringFromSelector(public)]&&[event[@"selectorIdentityValid"] boolValue]&&[event[@"recorderAliasVerified"] boolValue],@"actual recorder alias was not verified/preserved");
 Check([event[@"ownerIdentityKind"] isEqual:@"kvo"]&&[event[@"ownerIdentityValid"] boolValue]&&[event[@"ownerClass"] isEqual:@"NSKVONotifying_ArcWebContents"]&&[event[@"ownerClassChain"][1] isEqual:@"ArcWebContents"],@"genuine KVO runtime identity was not preserved/validated");
 Check(emitted.count==2&&[emitted[0] containsString:@"\"sel\":\"webContentsDevTools:inspectModeDidChange:\""]&&![emitted[0] containsString:@"__arcrec_"],@"extracted actual recorder did not retain canonical raw selector");
 // Invalid aliases still forward untouched exactly once, but cannot be evidence.
 FrontendInspect(delegate,NSSelectorFromString(@"__arcrec___arcrec_webContentsDevTools:inspectModeDidChange:"),tools,1);
 Check(originalCalls==2&&![Last()[@"selectorIdentityValid"] boolValue],@"arbitrary alias prefix accepted or forwarding lost");
 class_replaceMethod(cls,alias,gInspect,types);FrontendInspect(delegate,alias,tools,1);
 Check(originalCalls==3&&![Last()[@"selectorIdentityValid"] boolValue],@"alias with foreign saved IMP accepted");
 Class badABI=objc_allocateClassPair(cls,"AliasWrongABI",0);Check(class_addMethod(badABI,alias,(IMP)FrontendInspect,"v32@0:8@16B24"),@"bad ABI fixture install failed");objc_registerClassPair(badABI);FrontendInspect([badABI new],alias,tools,1);
 Check(originalCalls==4&&![Last()[@"selectorIdentityValid"] boolValue],@"alias with wrong encoding accepted");
 [owner removeObserver:observer forKeyPath:@"observed"];
 Check([FrontendOwnerIdentity(owner)[@"ownerIdentityKind"] isEqual:@"exact"],@"KVO removal not reflected");
 Class wrong=objc_allocateClassPair(ArcWebContents.class,"NSKVONotifying_ForeignOwner",0);objc_registerClassPair(wrong);id wrongOwner=[wrong new];
 Check(![FrontendOwnerIdentity(wrongOwner)[@"ownerIdentityValid"] boolValue],@"arbitrary KVO prefix subclass accepted");
 Class indirect=objc_allocateClassPair(wrong,"NSKVONotifying_ArcWebContents_Indirect",0);objc_registerClassPair(indirect);
 Check(![FrontendOwnerIdentity([indirect new])[@"ownerIdentityValid"] boolValue],@"indirect KVO-like subclass accepted");
 // A dishonest -class result alone cannot substitute for the actual direct base.
 Class impostor=objc_allocateClassPair(NSObject.class,"AliasForeignOwner",0);class_addMethod(impostor,@selector(class),imp_implementationWithBlock(^Class(id object){(void)object;return ArcWebContents.class;}),"#16@0:8");objc_registerClassPair(impostor);
 Check(![FrontendOwnerIdentity([impostor new])[@"ownerIdentityValid"] boolValue],@"foreign runtime class accepted");
 Class nilReport=objc_allocateClassPair(ArcWebContents.class,"AliasNilReportedClass",0);class_addMethod(nilReport,@selector(class),imp_implementationWithBlock(^Class(id object){(void)object;return Nil;}),"#16@0:8");objc_registerClassPair(nilReport);
 id nilOwner=[nilReport new];entry.owner=nilOwner;FrontendInspect(delegate,public,tools,1);
 Check(originalCalls==5&&![Last()[@"ownerIdentityValid"] boolValue]&&Last()[@"ownerReportedClass"]==NSNull.null,@"nil-reported class prevented forwarding or was accepted");
 puts("actual pinned recorder alias forwarding + genuine Foundation KVO + invalid alias/base controls passed");
}return 0;}
""".replace('__KEYS__',keys).replace('__FORWARDING__',forwarding))
        binary=self.directory/'alias-kvo'
        result=subprocess.run(['xcrun','clang','-fobjc-arc','-Wall','-Wextra','-Werror','-framework','Foundation','-framework','AppKit',str(path),'-o',str(binary)],capture_output=True,text=True)
        self.assertEqual(result.returncode,0,result.stderr)
        result=subprocess.run([str(binary)],capture_output=True,text=True)
        self.assertEqual(result.returncode,0,result.stderr)

    def test_pure_objective_c_actual_journal_json_booleans_and_old_source_negative(self):
        import hashlib
        old=ROOT/'logs/wp/webextras-native11-boolean-inputs/arc/browser/webextras_native_probe.m'
        self.assertEqual(hashlib.sha256(old.read_bytes()).hexdigest(),'0ee5a17c6f9fc9db13a2589edbdb1e7e487abfeef7793e619b246b3b5db51587')
        suffix=r"""
#import <CoreFoundation/CoreFoundation.h>
#ifdef __cplusplus
#error This regression must compile as pure Objective-C, not Objective-C++.
#endif
_Static_assert(_Generic((1 || 0), int: 1, default: 0), "C logical expression must be int");
@interface ArcWebContents:NSObject
@property NSInteger observed;
@end
@implementation ArcWebContents @end
@interface ForeignBooleanOwner:ArcWebContents @end
@implementation ForeignBooleanOwner
-(Class)class{return ArcWebContents.class;}
@end
@interface ArcWebContentsDevTools:NSObject @end
@implementation ArcWebContentsDevTools @end
@interface BooleanDelegate:NSObject
-(void)webContentsDevTools:(id)tools inspectModeDidChange:(NSUInteger)mode;
@end
static NSUInteger originalCount;static SEL forwardedSelector;
@implementation BooleanDelegate
-(void)webContentsDevTools:(id)tools inspectModeDidChange:(NSUInteger)mode{(void)tools;(void)mode;++originalCount;forwardedSelector=_cmd;}
@end
@interface BooleanKVOObserver:NSObject @end
@implementation BooleanKVOObserver
-(void)observeValueForKeyPath:(NSString*)path ofObject:(id)object change:(NSDictionary*)change context:(void*)context{(void)path;(void)object;(void)change;(void)context;}
@end
static void Check(BOOL condition,NSString* message){if(!condition){NSLog(@"%@",message);exit(1);}}
static NSDictionary* Reparsed(NSDictionary* value){NSError* error=nil;NSData* data=[NSJSONSerialization dataWithJSONObject:value options:0 error:&error];Check(data&&!error,@"JSON encode failed");NSDictionary* result=[NSJSONSerialization JSONObjectWithData:data options:0 error:&error];Check(result&&!error,@"JSON decode failed");return result;}
static void JSONBoolean(NSDictionary* dictionary,NSString* key,BOOL expected){
 for(NSDictionary* value in @[dictionary,Reparsed(dictionary)]){
  id object=value[key];Check(object&&CFGetTypeID((__bridge CFTypeRef)object)==CFBooleanGetTypeID(),[@"JSON boolean type required: " stringByAppendingString:key]);
  Check([object boolValue]==expected,[@"JSON boolean value incorrect: " stringByAppendingString:key]);
 }
}
static void Event(NSDictionary* value,BOOL selectorValid,BOOL ownerValid,BOOL alias,BOOL forwarded){
 JSONBoolean(value,@"selectorIdentityValid",selectorValid);JSONBoolean(value,@"ownerIdentityValid",ownerValid);
 JSONBoolean(value,@"recorderAliasVerified",alias);JSONBoolean(value,@"mainThread",YES);JSONBoolean(value,@"forwarded",forwarded);
 for(NSString* key in @[@"mode",@"ownerTag",@"ordinal",@"forwardDepth"]){id number=Reparsed(value)[key];Check(CFGetTypeID((__bridge CFTypeRef)number)!=CFBooleanGetTypeID(),[@"numeric count became boolean: " stringByAppendingString:key]);}
 Check([value[@"recorderAliasSourceSHA256"] isEqual:@"9ae39687fdebcf4115b8c974af1b799aeee65265185a2c512ff56e5ca287d81e"],@"recorder policy source identity changed");
}
int main(void){@autoreleasepool{
 gFrontendEvents=YES;gFrontendLock=[NSObject new];gRecord=@"/dev/null";gExpectedURL=@"http://127.0.0.1:1/arc-webextras-probe-bool/fixture";
 ArcWebContents* owner=[ArcWebContents new];ArcWebContentsDevTools* tools=[ArcWebContentsDevTools new];BooleanDelegate* delegate=[BooleanDelegate new];BooleanKVOObserver* observer=[BooleanKVOObserver new];
 WebextrasProbeEntry* entry=[WebextrasProbeEntry new];entry.owner=owner;entry.tools=tools;entry.identifier=1;
 WebextrasProbeWork* work=[WebextrasProbeWork new];work.response=[NSMutableDictionary dictionary];work.publicCalls=[NSMutableArray array];work.timeline=[NSMutableArray array];
 NSDictionary* request=@{@"spanID":@"bool",@"ownerTag":@1,@"observationNonce":@"arc-webextras-probe-bool",@"url":gExpectedURL};
 Check(!FrontendObservationOperation(work,entry,request,@"frontend-observe-arm"),@"arm failed");
 SEL public=@selector(webContentsDevTools:inspectModeDidChange:),alias=NSSelectorFromString(@"__arcrec_webContentsDevTools:inspectModeDidChange:");
 Class cls=BooleanDelegate.class;Method method=class_getInstanceMethod(cls,public);const char* types=method_getTypeEncoding(method);gInspect=method_getImplementation(method);
 class_replaceMethod(cls,public,(IMP)FrontendInspect,types);Check(class_addMethod(cls,alias,(IMP)FrontendInspect,types),@"alias setup failed");
 [delegate webContentsDevTools:tools inspectModeDidChange:1];Event(gFrontendObservation.callbacks.lastObject,YES,YES,NO,YES);
 [owner addObserver:observer forKeyPath:@"observed" options:0 context:NULL];
 Check(object_getClass(owner)!=ArcWebContents.class&&[owner class]==ArcWebContents.class,@"real KVO subclass absent");
 ((void(*)(id,SEL,id,NSUInteger))objc_msgSend)(delegate,alias,tools,1);Event(gFrontendObservation.callbacks.lastObject,YES,YES,YES,YES);
 Check([gFrontendObservation.callbacks.lastObject[@"ownerIdentityKind"] isEqual:@"kvo"]&&forwardedSelector==alias,@"KVO/alias identity or incoming selector lost");
 FrontendInspect(delegate,NSSelectorFromString(@"__arcrec___arcrec_webContentsDevTools:inspectModeDidChange:"),tools,1);Event(gFrontendObservation.callbacks.lastObject,NO,YES,NO,YES);
 ForeignBooleanOwner* foreign=[ForeignBooleanOwner new];entry.owner=foreign;FrontendInspect(delegate,public,tools,1);Event(gFrontendObservation.callbacks.lastObject,YES,NO,NO,YES);
 Check(originalCount==4,@"original IMP not forwarded exactly once per callback");entry.owner=owner;
 WebextrasFrontendObservation* captured=nil;NSMutableDictionary* unfinished=FrontendCallbackBegin(delegate,public,public,(IMP)FrontendInspect,tools,1,&captured);FrontendCallbackEnd(captured,unfinished,NO);Event(unfinished,YES,YES,NO,NO);
 Check(!FrontendObservationOperation(work,entry,request,@"frontend-observe-read"),@"read failed");JSONBoolean(work.response[@"observation"],@"expired",NO);JSONBoolean(work.response[@"observation"],@"overflow",NO);
 gFrontendObservation.deadline=NSDate.timeIntervalSinceReferenceDate-1;gFrontendObservation.overflow=YES;
 Check(!FrontendObservationOperation(work,entry,request,@"frontend-observe-read"),@"expired read failed");JSONBoolean(work.response[@"observation"],@"expired",YES);JSONBoolean(work.response[@"observation"],@"overflow",YES);
 Check(work.publicCalls.count==0&&work.timeline.count==0,@"journal performed public work");
 [owner removeObserver:observer forKeyPath:@"observed"];
 puts("pure Objective-C actual journal JSON booleans: public/alias/real-KVO/invalid-owner/false-forward/expired/overflow passed");
}return 0;}
"""
        for name,body,expected in [('current',NATIVE.read_text(),0),('old-0ee5',old.read_text(),1)]:
            # Extract the actual complete helper source into a .m translation
            # unit, with no altered flag expressions or serialization mocks.
            source=self.directory/('pure-json-'+name+'.m');source.write_text(body+suffix)
            binary=self.directory/('pure-json-'+name)
            command=['xcrun','clang','-x','objective-c','-std=gnu17','-fobjc-arc','-Wall','-Wextra','-Werror','-framework','Foundation','-framework','AppKit',str(source),'-o',str(binary)]
            result=subprocess.run(command,capture_output=True,text=True);self.assertEqual(result.returncode,0,result.stderr)
            result=subprocess.run([str(binary)],capture_output=True,text=True);self.assertEqual(result.returncode,expected,result.stderr)
            if expected:self.assertIn('JSON boolean type required: selectorIdentityValid',result.stderr)

    def test_frozen_callback_comparator_canaries_and_nested_values_are_not_dropped(self):
        def module(name,path):
            spec=importlib.util.spec_from_file_location(name,path)
            loaded=importlib.util.module_from_spec(spec);spec.loader.exec_module(loaded);return loaded
        inputs=module('fixture_input_test',ROOT/'arc/browser/webextras_fixture_inputs.py')
        comparator=module('callback_input_test',ROOT/'arc/browser/webextras_frontend_comparison.py')
        snapshot=self.directory/'callback-comparator'
        inputs.freeze(ROOT,snapshot,'WebextrasNativeProbe.dylib')
        with self.assertRaises(ValueError):comparator.capability(snapshot,'WrongProbe.dylib')
        support=comparator.capability(snapshot,'WebextrasNativeProbe.dylib')
        self.assertEqual(len(support['cases']),4)
        self.assertEqual(support['verified'],all(case['verified'] for case in support['cases']))
        literal=support['cases'][-1]
        self.assertEqual(literal['name'],'literal-filename-callback-argument')
        self.assertEqual(literal['expected'],dict(missing=0,extra=0,return_mismatch=0,arg_mismatch=1,count_mismatch=0))
        callbackRows=comparator.filename_canary_rows(15)
        right=self.directory/'literal-canary.jsonl'
        right.write_text(''.join(json.dumps(row)+'\n' for row in callbackRows))
        self.assertEqual(comparator.gate(right,'WebextrasNativeProbe.dylib',filename_canary=True)['callbacks'],1)
        with self.assertRaises(ValueError):comparator.gate(right,'WebextrasNativeProbe.dylib')
        for change in [lambda rows: rows[1].update(ret='unproven'),lambda rows: rows[2].update(args=['<ArcBrowserContext#1>','<ArcDownloadItem#1>','<ptr>',15]),lambda rows: rows[2].update(**{'from':'app'})]:
            bad=comparator.filename_canary_rows(15);change(bad);right.write_text(''.join(json.dumps(row)+'\n' for row in bad))
            with self.assertRaises(ValueError):comparator.gate(right,'WebextrasNativeProbe.dylib',filename_canary=True)
        for case in support['cases']:
            self.assertTrue(all(count['callbacks']==1 and count['fixturePublicCalls']==0 for count in case['typedCounts']))
            if case['verified']:
                self.assertEqual(case['observed'],case['expected'])
                self.assertEqual(case['metadata'],comparator.ARGUMENT_METADATA)
        left,right,report=[self.directory/name for name in ('callback-left.jsonl','callback-right.jsonl','callback-report.json')]
        original=comparator.canary_rows('getter','<ArcWebContents#118>')
        changed=comparator.canary_rows('getter',None)
        for path,rows in [(left,original),(right,changed)]:
            path.write_text(''.join(json.dumps(row,separators=(',',':'))+'\n' for row in rows))
        with self.assertRaises(ValueError):comparator.compare(snapshot,'WrongProbe.dylib',left,right,report)
        result=comparator.compare(snapshot,'WebextrasNativeProbe.dylib',left,right,report)
        self.assertEqual(len(result['report']['return_mismatch']),1)
        self.assertIn('return_mismatch',result['mismatches'])
        self.assertEqual(json.loads(report.read_text()),result['report'])
        self.assertEqual(result['verified'],support['verified'])
        if not support['verified']:
            self.assertEqual(result['status'],'pending-callback-boundary-comparison-support')
        self.assertTrue(result['stdout'].splitlines()[0].startswith('methods differing:'))
        self.assertTrue(result['stdout'].splitlines()[1].startswith('callback sequence:'))
        for bad in [[],[dict(row,**{'from':''}) for row in original],
                    [dict(row,**{'from':'app'}) if row['dir']=='engine->app' else row for row in original],
                    [row for row in original if row['dir']!='engine->app']]:
            right.write_text(''.join(json.dumps(row)+'\n' for row in bad))
            with self.assertRaises(ValueError):comparator.compare(snapshot,'WebextrasNativeProbe.dylib',left,right,report)
        # Cross-thread IDs may print out of global allocation order. Preserve
        # row order and require uniqueness, per-thread nesting and call<return.
        interleaved=[dict(original[0],seq=1),
                     dict(original[1],seq=4,thread='worker',depth=0),
                     dict(original[2],seq=5,thread='worker',depth=0),
                     dict(original[1],seq=2),dict(original[2],seq=3),dict(original[3],seq=6)]
        right.write_text(''.join(json.dumps(row)+'\n' for row in interleaved))
        self.assertEqual(comparator.gate(right,'WebextrasNativeProbe.dylib')['callbacks'],1)
        interleaved[1]['seq']=1
        right.write_text(''.join(json.dumps(row)+'\n' for row in interleaved))
        with self.assertRaises(ValueError):comparator.gate(right,'WebextrasNativeProbe.dylib')
        self.assertIn('--compare-boundary-arguments',(ROOT/'arc/browser/webextras_frontend_comparison.py').read_text())
        self.assertNotIn("'--fixture-image',",(ROOT/'arc/browser/webextras_frontend_comparison.py').read_text())

    def test_frozen_freezer_loader_never_executes_unmanifested_unchecked_bytecode(self):
        import py_compile
        def module(name,path):
            spec=importlib.util.spec_from_file_location(name,path)
            loaded=importlib.util.module_from_spec(spec);spec.loader.exec_module(loaded);return loaded
        inputs=module('cache_fixture_inputs',ROOT/'arc/browser/webextras_fixture_inputs.py')
        comparator=module('cache_callback_inputs',ROOT/'arc/browser/webextras_frontend_comparison.py')
        snapshot=self.directory/'source-only-frozen';inputs.freeze(ROOT,snapshot,'WebextrasNativeProbe.dylib')
        source=snapshot/'arc/browser/webextras_fixture_inputs.py'
        marker=self.directory/'unmanifested-cache-executed'
        evil=self.directory/'cache-payload.py'
        evil.write_text('from pathlib import Path\nPath('+repr(str(marker))+').write_text("executed")\ndef verify(snapshot): return {}\n')
        cache=Path(importlib.util.cache_from_source(str(source)));cache.parent.mkdir(parents=True,exist_ok=True)
        py_compile.compile(str(evil),cfile=str(cache),doraise=True,invalidation_mode=py_compile.PycInvalidationMode.UNCHECKED_HASH)
        try:comparator.verify(snapshot)
        except ValueError:pass  # Stricter freezer may reject the extra cache itself.
        self.assertFalse(marker.exists(),'source-only production loader executed unmanifested cache')
        module('proven_unsafe_cache_control',source)
        self.assertTrue(marker.exists(),'unchecked-hash negative control did not actually execute')

    def test_actual_frontend_recorder_source_freeze_and_changed_source_rejection(self):
        self.bun(r'''
const fs=await import('node:fs'),path=await import('node:path');
const guardSource=source.slice(source.indexOf('function recorderFileIdentity('),source.indexOf('function hasCallerLabels('));
const command=(argv:string[])=>{const result=Bun.spawnSync(argv,{stdout:'pipe',stderr:'pipe'});check(result.success,'identity command failed');return result.stdout.toString().trim()};
const guards=new Function('exec','realpathSync','statSync','copyFileSync','chmodSync','writeFileSync','mkdirSync','join','constants','assert','frontendRecorderAliasSourceSHA256',
 new Bun.Transpiler({loader:'ts'}).transformSync(guardSource+';return{freezeFrontendRecorderPolicy,verifyFrontendRecorderPolicy};'))(command,fs.realpathSync,fs.statSync,fs.copyFileSync,fs.chmodSync,fs.writeFileSync,fs.mkdirSync,path.join,fs.constants,check,'9ae39687fdebcf4115b8c974af1b799aeee65265185a2c512ff56e5ca287d81e');
const directory=__DIRECTORY__;fs.mkdirSync(directory);const original=__RECORDER__,local=path.join(directory,'source.m');fs.copyFileSync(original,local);
const out=path.join(directory,'valid');fs.mkdirSync(out);const identity=guards.freezeFrontendRecorderPolicy(local,out);
guards.verifyFrontendRecorderPolicy(identity,'before launch');check(fs.readFileSync(identity.frozen.path,'utf8')===fs.readFileSync(local,'utf8')&&fs.statSync(identity.frozen.path).mode%512===292,'policy source not frozen readonly');
check(identity.provenance.includes('no source-to-recorder-dylib'),'source-to-binary provenance overclaimed');
fs.appendFileSync(local,'changed');let message='';try{guards.verifyFrontendRecorderPolicy(identity,'before launch')}catch(error){message=String(error)}check(message.includes('policy source changed before launch'),'changed source accepted');
const bad=path.join(directory,'bad');fs.mkdirSync(bad);message='';try{guards.freezeFrontendRecorderPolicy(local,bad)}catch(error){message=String(error)}check(message.includes('policy source hash mismatch')&&!fs.existsSync(path.join(bad,'frontend-recorder-policy.m')),'wrong policy source accepted/preflight copy created');
const other=path.join(directory,'frozen-tamper');fs.mkdirSync(other);const frozen=guards.freezeFrontendRecorderPolicy(original,other);fs.chmodSync(frozen.frozen.path,0o644);fs.appendFileSync(frozen.frozen.path,'changed');
message='';try{guards.verifyFrontendRecorderPolicy(frozen,'after measurement/cleanup')}catch(error){message=String(error)}check(message.includes('frozen frontend recorder policy source changed'),'changed frozen source accepted');
check(source.includes('const frontendRecorderPolicy=devToolsFrontendEvents?freezeFrontendRecorderPolicy(')&&source.includes('verifyFrontendRecorderPolicy(frontendRecorderPolicy,"before launch")')&&source.includes('verifyFrontendRecorderPolicy(frontendRecorderPolicy,"after measurement/cleanup")'),'source pin not frontend-only/both-launches/final guard');
'''.replace('__DIRECTORY__',json.dumps(str(self.directory/'source-freeze'))).replace('__RECORDER__',json.dumps(str(ROOT/'tests/recorder/ArcEngineRecorder.m'))))

    def test_required_literal_canary_distinguishes_defective_and_corrected_real_comparators(self):
        import hashlib
        import shutil
        def module(name,path):
            spec=importlib.util.spec_from_file_location(name,path)
            loaded=importlib.util.module_from_spec(spec);spec.loader.exec_module(loaded);return loaded
        inputs=module('canary_fixture_inputs',ROOT/'arc/browser/webextras_fixture_inputs.py')
        comparator=module('canary_callback_inputs',ROOT/'arc/browser/webextras_frontend_comparison.py')
        evidence=ROOT/'logs/wp/webextras-boundary-arguments-callback-correction'
        for name,sha,expected in [('baseline-87be','14671717f532c81807404cfa26cd3b5a7d1622ca3e48326ce363f6ff7a294caa',False),
                                  ('staged','24437bed30fddc8b94ab5c9dcc7cd20e755b06c0b05a4d8620e487d3c7f612cf',True)]:
            source=evidence/name
            self.assertEqual(hashlib.sha256((source/'tests/recorder/diff.py').read_bytes()).hexdigest(),sha)
            temporary=self.directory/('callback-correction-'+name)
            shutil.copytree(source,temporary/'source')
            owned=temporary/'source/arc/browser/webextras_fixture_inputs.py';owned.parent.mkdir(parents=True,exist_ok=True);owned.write_bytes((ROOT/'arc/browser/webextras_fixture_inputs.py').read_bytes())
            snapshot=temporary/'frozen'
            state=inputs.freeze(temporary/'source',snapshot,'WebextrasNativeProbe.dylib')
            expected_files={'tests/recorder/diff.py','tests/recorder/app_callbacks.inc','arc/tools/arccore-1.170.0-objc.tsv','arc/browser/webextras_fixture_inputs.py',
                      'tests/recorder/download_identity.py','tests/recorder/google_q.py','tests/recorder/omnibox_receivers.py'}
            self.assertEqual(set(state['files']),expected_files)
            result=comparator.capability(snapshot,'WebextrasNativeProbe.dylib')
            self.assertEqual(inputs.verify(snapshot)['files'],state['files'])
            self.assertEqual(len(result['cases']),4)
            self.assertTrue(all(case['verified'] for case in result['cases'][:3]))
            self.assertEqual(result['cases'][3]['verified'],expected)
            self.assertEqual(result['verified'],expected)
            self.assertEqual(result['cases'][3]['observed'],dict(missing=0,extra=0,return_mismatch=0,arg_mismatch=int(expected),count_mismatch=0))

    def test_default_and_video_service_body_preservation(self):
        archive=ROOT/'logs/wp/webextras-native9-frontend-lane-inputs'
        old,current=(archive/'arc/browser/webextras_native_probe.ts').read_text(),DRIVER.read_text()
        for begin,end in [('const mediaClockPhases =','function clockModeFromArgs('),('function validateClockPhase(','function validateClockReceivers('),
                          ('function mediaClockPrepareJS(','// Persist the complete Runtime.evaluate response'),
                          ('function mediaServiceClockPrepareJS(','function isNilClockPosition('),('function validateMediaServiceClockPhase(','function clockActionKey('),
                          ('      if (anyClock) {','      run.phase = "cdp-close-fixture";')]:
            self.assertEqual(current[current.index(begin):current.index(end)],old[old.index(begin):old.index(end)])
        old,current=(archive/'arc/browser/webextras_native_probe.m').read_text(),NATIVE.read_text()
        for begin,end in [('static void Display(','static void Closed('),('static void Closed(','static const char* Unqualified('),('static WebextrasProbeWork* CallbackEntry(','// Off-main delivery')]:
            old_end=end
            if begin=='static void Closed(':end='// This wrapper observes the existing app IMP'
            self.assertEqual(current[current.index(begin):current.index(end)],old[old.index(begin):old.index(old_end)])
        source=DRIVER.read_text()
        self.assertIn('const operations = ["console-open", "frontend", "console-close", "panel-closed", "execute-evaluate", "execute-malformed", "execute-unknown", "subscription", "media-types", "pip-request-error", "pip-exit-error"];',source)
        self.assertIn('span.traceShape=validateFrontendCallbackSpan(',source)
        self.assertNotIn('callbacks.record.jsonl", String(expectedSelectors',source)
        self.assertIn('(!devToolsFrontendEvents||run.frontendEvents?.rawValidated===true)',source)


if __name__=='__main__':
    unittest.main(verbosity=2)
