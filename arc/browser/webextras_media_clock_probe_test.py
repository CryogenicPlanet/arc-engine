#!/usr/bin/env python3
"""Standalone current-production extraction tests; no app, lock, engine build or private ABI."""
import importlib.util
import json
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
DRIVER = ROOT / 'arc/browser/webextras_native_probe.ts'
NATIVE = ROOT / 'arc/browser/webextras_native_probe.m'


class MediaClockTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory(prefix='webextras-media-clock-test-')
        cls.directory = Path(cls.temp.name)

    @classmethod
    def tearDownClass(cls):
        cls.temp.cleanup()

    def bun(self, body):
        path = self.directory / (self._testMethodName + '.ts')
        path.write_text('''import {readFileSync} from 'node:fs';
import {PublicTraceProjection} from %s;
const source=readFileSync(%s,'utf8');
const validators=source.slice(source.indexOf('type RecordRow'),source.indexOf('const root ='));
const api=new Function('PublicTraceProjection',new Bun.Transpiler({loader:'ts'}).transformSync(validators+`\nreturn {parseRecords,expectedSelectors,validateActionTrace,outcomeErrors,clockDelta,validateClockPhase,mediaClockPolicy,mediaClockPhases,clockActionKey,mediaClockPrepareJS,evaluateMediaClock,mediaClockFailureDOMJS,activateMediaClockTarget,clockIdentityJS,runtimeLogOutcome,isClockInconclusive,clockVisibilityLimitMilliseconds,clockModeFromArgs,mediaServiceClockPhases,mediaServiceClockPrepareJS,serviceClockObservationJS,validateMediaServiceClockPhase,isNilClockPosition};`))(PublicTraceProjection);
const check=(v:any,m:string)=>{if(!v)throw Error(m)};
const rejects=(fn:()=>void,m:string)=>{let caught=false;try{fn()}catch{caught=true}check(caught,m)};
''' % (json.dumps(str(ROOT / 'arc/browser/webextras_fixture_projection.ts')), json.dumps(str(DRIVER))) + body)
        result = subprocess.run(['bun', str(path)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        return result.stdout

    def test_saved_default_eleven_replay(self):
        self.bun('''
const record=readFileSync(%s), observations=JSON.parse(readFileSync(%s,'utf8'));
const metadata=readFileSync(%s,'utf8');
const projection=new PublicTraceProjection(metadata,'WebextrasNativeProbe.dylib',false);
check(observations.actions.length===11,'saved default action count');
let count=0;
for(const action of observations.actions){
 const rows=api.parseRecords(record.subarray(action.recordBegin,action.recordEnd),action.recordBegin);
 check(api.validateActionTrace(action,rows,record).length>0,'raw replay');
 check(api.validateActionTrace(action,rows,record,projection).length>0,'projected replay');
 check(api.outcomeErrors(action).length===0,'default outcome changed');
 count+=api.expectedSelectors(action.operation).length;
}
check(count===13,'default public pair count changed');
''' % tuple(json.dumps(str(ROOT / relative)) for relative in (
            'results/webextras/native-run3/trace156/record.jsonl',
            'results/webextras/native-run3/trace156/observations.json', 'arc/tools/arccore-1.170.0-objc.tsv')))

    def test_slope_quantization_and_negative_controls(self):
        self.bun('''
const sample=(ticks:bigint,value:number,token='kept',tag=1)=>({ownerTag:1,clock:{heldToken:token,dtoTag:tag,timebaseNumer:1,timebaseDenom:1},publicCalls:[{selector:'currentTime',value,clockBeforeTicks:String(ticks),clockAfterTicks:String(ticks+1000n)}]});
const epoch=9007199254740993000n; // beyond Number's exact uint64 range
for(const rate of [0,1,2,1.5]){
 const a=sample(epoch,3),b=sample(epoch+400000000n,3+rate*.4);
 api.clockDelta(a,b,rate);
 if(rate)rejects(()=>api.clockDelta(a,sample(epoch+400000000n,3),rate),'constant passed');
}
const a=sample(epoch,3), b=sample(epoch+400000000n,3.4);
rejects(()=>api.clockDelta(a,b,2),'wrong rate passed');
rejects(()=>api.clockDelta(a,{...b,clock:{...b.clock,heldToken:'stale'}},1),'stale token passed');
rejects(()=>api.clockDelta(a,{...b,clock:{...b.clock,dtoTag:2}},1),'replacement DTO passed');
rejects(()=>api.clockDelta(a,sample(epoch+1000000n,3.001),1),'short interval passed');
const wide=sample(epoch+400000000n,3.4);wide.publicCalls[0].clockAfterTicks=String(epoch+500000000n);
rejects(()=>api.clockDelta(a,wide,1),'wide getter passed');
const backwards=sample(epoch+400000000n,3.4);backwards.publicCalls[0].clockAfterTicks=String(epoch);
rejects(()=>api.clockDelta(a,backwards,1),'reversed bracket passed');
check(api.mediaClockPolicy.quantizationSeconds===.000004,'fixed policy widened');
''')

    def test_clock_trace_receiver_abi_counts_values_caller_and_offsets(self):
        self.bun('''
const image='WebextrasNativeProbe.dylib',metadata=readFileSync(%s,'utf8');
const common={thread:'main',depth:0,dir:'->engine',kind:'-',from:image};
const selectors=api.expectedSelectors('clock-snapshot');
function make(operation='clock-snapshot',id='one',token=id,receiver='<ArcMediaPosition#8>'){
 const calls:any[]=[],events:any[]=[],timeline:any[]=[];let offset=0;
 for(const selector of api.expectedSelectors(operation)){
  const cls=selector==='position'?'ArcWebContentsMediaSession':'ArcMediaPosition';
  const value=selector==='duration'?8.208:selector==='playbackRate'?1:selector==='endOfMedia'?false:1.25;
  const self=selector==='position'?'<ArcWebContentsMediaSession#2>':receiver;
  const call={...common,ev:'call',seq:events.length+1,cls,sel:selector,self,args:[]};
  const ret={...common,ev:'ret',seq:events.length+2,cls,sel:selector,self,ret:selector==='position'?receiver:value};
  const data=JSON.stringify(call)+'\\n'+JSON.stringify(ret)+'\\n';
  calls.push({selector,receiverClass:cls,abi:selector==='position'?'@16@0:8':selector==='endOfMedia'?'B16@0:8':'d16@0:8',resultClass:selector==='position'?'ArcMediaPosition':undefined,
    value:selector==='position'?undefined:value,recordValue:typeof value==='number'?String(value):undefined,clockBeforeTicks:'1000000000',clockAfterTicks:'1000001000',recordBegin:offset,recordEnd:offset+Buffer.byteLength(data)});
  timeline.push({event:'publicCall',selector},{event:'publicReturn',selector});offset+=Buffer.byteLength(data);events.push(call,ret);
 }
 const record=Buffer.from(events.map(e=>JSON.stringify(e)).join('\\n')+'\\n');
 return {action:{id,operation,ownerTag:1,clock:{heldToken:token,dtoTag:1,timebaseNumer:1,timebaseDenom:1},publicCalls:calls,callbacks:[],timeline,recordBegin:0,recordEnd:record.length,recordAfterReturn:record.length},record};
}
function validate(f:any,held=new Map<string,string>()){
 const rows=api.parseRecords(f.record);
 return api.validateActionTrace(f.action,rows,f.record,new PublicTraceProjection(metadata,image),held);
}
const held=new Map<string,string>(),a=make();validate(a,held);
check(api.outcomeErrors(a.action).length===0,'clock outcome rejected');
validate(make('clock-held-time','two','one'),held);
rejects(()=>validate(make('clock-held-time','two','stale'),held),'stale token trace passed');
rejects(()=>validate(make('clock-held-time','two','one','<ArcMediaPosition#9>'),held),'fresh DTO substituted');
for(const mutate of [
 (f:any)=>{f.action.publicCalls.pop()},
 (f:any)=>{f.action.publicCalls[1].abi='f16@0:8'},
 (f:any)=>{f.action.publicCalls[1].recordBegin++},
 (f:any)=>{f.action.publicCalls[1].recordValue='9'},
 (f:any)=>{f.action.publicCalls[1].value=9},
 (f:any)=>{f.action.publicCalls.reverse()},
 (f:any)=>{f.record=Buffer.from(f.record.toString().replace('\"ret\":1}','\"ret\":9}'))},
 (f:any)=>{f.record=Buffer.from(f.record.toString().replaceAll(image,'XebextrasNativeProbe.dylib'))},
]){const f=make();mutate(f);rejects(()=>validate(f),'malformed clock evidence passed')}
''' % json.dumps(str(ROOT / 'arc/tools/arccore-1.170.0-objc.tsv')))

    def test_all_six_phase_semantics_and_static_negative_controls(self):
        self.bun('''
for(const phase of api.mediaClockPhases){
 const declared=phase.declared,paused=declared||phase.rate===0;
 const observed={performanceNow:0,currentTime:phase.seek,duration:8.208,playbackRate:paused?1:phase.rate,paused,ended:false,seeking:false};
 const input=declared?{duration:20,position:7.25,playbackRate:1.5}:{duration:observed.duration,position:observed.currentTime,playbackRate:observed.playbackRate};
 const receipt={phase:phase.name,source:declared?'independent':'DOM-video',beforeReadPerformanceNow:0,observed,input,beforeSetPerformanceNow:0,afterSetPerformanceNow:0};
 const acts=Array.from({length:5},(_,index)=>{
  const elapsed=index<3?index*.25:.5+(index-2)*.4;
  const q=(declared?7.25:phase.seek)+phase.rate*elapsed;
  const dom={phase:phase.name,visibilityState:'visible',hidden:false,readyState:4,seeking:false,ended:false,loop:false,currentTime:paused?phase.seek:phase.seek+phase.rate*elapsed,duration:8.208,performanceNow:elapsed*1000,paused,playbackRate:paused?1:phase.rate,playbackState:phase.rate===0?'paused':'playing',positionReceipt:receipt};
  const token=index<3?'token'+index:'token2';
  const calls=(index<3?['position','playbackRate','duration','currentTime','endOfMedia']:['currentTime']).map(selector=>({selector,value:selector==='playbackRate'?phase.rate:selector==='duration'?(declared?20:8.208):selector==='endOfMedia'?false:q,clockBeforeTicks:String(1000000000+elapsed*1e9),clockAfterTicks:String(1000001000+elapsed*1e9)}));
  return {ok:true,ownerTag:1,positionReceipt:receipt,operation:index<3?'clock-snapshot':'clock-held-time',actionKey:api.clockActionKey(phase.name,index),clockPhase:phase.name,clockAttempt:index,clock:{heldToken:token,dtoTag:index<3?index+1:3,timebaseNumer:1,timebaseDenom:1},publicCalls:calls,domBefore:{...dom},domAfter:{...dom}};
 });
 api.validateClockPhase(phase,acts);
 const mutate=(fn:(x:any[])=>void)=>{const x=structuredClone(acts);fn(x);rejects(()=>api.validateClockPhase(phase,x),'bad phase passed')};
 mutate(x=>{x[2].publicCalls.find((c:any)=>c.selector==='endOfMedia').value=true});
 mutate(x=>{x[4].domAfter.loop=true});
 mutate(x=>{x[4].domAfter.ended=true});
 mutate(x=>{x[4].domAfter.visibilityState='hidden'});
 mutate(x=>{x[4].domAfter.hidden=true});
 mutate(x=>{for(const a of x)for(const c of a.publicCalls)if(c.selector==='currentTime')c.value+=100});
 mutate(x=>{x[2].domAfter.positionReceipt={input:{duration:20,position:7.25,playbackRate:0}}});
 mutate(x=>{for(const a of x){a.positionReceipt.input.position+=1}});
 mutate(x=>{x.reverse()});
 if(phase.rate)mutate(x=>{const value=x[2].publicCalls.find((c:any)=>c.selector==='currentTime').value;for(const a of x.slice(3))a.publicCalls[0].value=value});
 else mutate(x=>{x[2].publicCalls.find((c:any)=>c.selector==='currentTime').value=0});
}
const phase=api.mediaClockPhases[0],js=api.mediaClockPrepareJS(phase,'http://127.0.0.1/clock.webm');
check(!js.includes('playbackRate:0')&&js.includes('setPositionState({})'),'invalid rate0 stimulus or missing clear');
check(api.mediaClockPhases.length*17+13===115,'fixed opt-in count changed');
''')

    def test_actual_public_js_stimulus_receipt_uses_dom_read(self):
        self.bun('''
for(const phase of api.mediaClockPhases){
 const events:any[]=[],issued:any[]=[];let now=0;
 const video={muted:true,loop:false,seeking:false,ended:false,currentTime:0,duration:8.208,playbackRate:1,paused:true,
   pause(){this.paused=true;events.push('pause')},async play(){this.paused=false;this.currentTime+=.031;events.push('play')}};
 const session={playbackState:'none',setPositionState(input:any){events.push(Object.keys(input).length?'position':'clear');issued.push(structuredClone(input))}};
 const window:any={};
 const fn=new Function('document','navigator','window','performance','return '+api.mediaClockPrepareJS(phase,'http://unused/video.webm'));
 const preparation=await fn({querySelector:()=>video},{mediaSession:session},window,{now:()=>++now});
 const receipt=preparation.receipt;
 check(issued.length===2&&Object.keys(issued[0]).length===0,'missing explicit clear/report');
 check(JSON.stringify(issued[1])===JSON.stringify(receipt.input),'receipt is not exact issued public state');
 check(receipt.observed.currentTime===video.currentTime&&receipt.observed.duration===video.duration&&receipt.observed.playbackRate===video.playbackRate,'receipt is not real DOM read');
 check(receipt.beforeReadPerformanceNow<=receipt.observed.performanceNow&&receipt.observed.performanceNow<=receipt.beforeSetPerformanceNow&&receipt.beforeSetPerformanceNow<receipt.afterSetPerformanceNow,'receipt clock order');
 check(window.__webextrasClockReceipt===receipt&&receipt.phase===phase.name,'stored receipt identity');
 check(session.playbackState===(phase.rate===0?'paused':'playing'),'wrong effective state');
 if(phase.declared)check(receipt.input.position===7.25&&receipt.input.duration===20&&receipt.input.playbackRate===1.5&&video.paused,'independent input lost');
 else{check(receipt.input.position===video.currentTime&&receipt.input.duration===video.duration&&receipt.input.playbackRate===video.playbackRate,'DOM-fed public input wrong');
   if(phase.rate)check(receipt.input.position!==phase.seek&&events.indexOf('play')<events.lastIndexOf('position'),'playing start hardcoded or reported before actual play');}
 check(receipt.input.playbackRate!==0,'invalid rate0 input');
}
''')

    def test_actual_initial_create_metadata_play_and_failure_stage(self):
        self.bun('''
for(const fail of [false,true]){
 const events:any[]=[],issued:any[]=[];let now=0,created:any=null;
 const video:any={seeking:false,ended:false,currentTime:0,duration:8.208,playbackRate:1,paused:true,
   pause(){this.paused=true;events.push('pause')},async play(){events.push('play');if(fail)throw new Error('mock initial play failure');this.paused=false;this.currentTime+=.031}};
 const document={querySelector:()=>created,createElement:(name:string)=>{check(name==='video','wrong initial element');return video},body:{append:(v:any)=>{created=v;queueMicrotask(()=>v.onloadedmetadata())}}};
 const session={playbackState:'none',setPositionState(input:any){issued.push(structuredClone(input))}},window:any={};
 const phase=api.mediaClockPhases[0],fn=new Function('document','navigator','window','performance','return '+api.mediaClockPrepareJS(phase,'http://fixture/video.webm'));
 let failed=false;try{const result=await fn(document,{mediaSession:session},window,{now:()=>++now});check(result.receipt.input.position===phase.seek&&issued.length===2,'initial success did not reach exact receipt');}catch(error){failed=true}
 check(failed===fail,'initial failure state lost');
 check(created===video&&video.muted&&video.loop===false&&video.src==='http://fixture/video.webm','initial creation changed');
 const stages=window.__webextrasClockProgress.progress.map((x:any)=>x.stage);
 for(const stage of ['initial-create-before','initial-create-after','initial-metadata-before','initial-metadata-after','initial-play-before'])check(stages.includes(stage),'missing initial progress');
 if(fail){check(window.__webextrasClockFailure.name==='Error'&&window.__webextrasClockFailure.message==='mock initial play failure'&&window.__webextrasClockFailure.stage==='initial-play-before'&&typeof window.__webextrasClockFailure.stack==='string','failure identity/stage lost');check(issued.length===0&&stages.at(-1)==='failed','failure continued stimulus');}
 else for(const stage of ['initial-play-after','clear-before','clear-after','seek-before','seek-after','playback-state-before','playback-state-after','input-setter-before','input-setter-after','complete'])check(stages.includes(stage),'missing successful progress');
}
''')

    def test_actual_raw_evaluation_persists_before_exception_and_transport_failure(self):
        path = self.directory / 'raw-evaluations.jsonl'
        self.bun('''
const {appendFileSync}=await import('node:fs');
const path=__PATH__,save=(value:any)=>appendFileSync(path,JSON.stringify(value)+'\\n');
const context={phase:'clock-prepare:test',label:'prepare',expression:'exact fixture expression'};
const raw={result:{type:'object',subtype:'error',description:'Error: initial play failed'},exceptionDetails:{text:'Uncaught (in promise)',exception:{description:'Error: initial play failed'},stackTrace:{callFrames:[{functionName:'prepare',url:'http://fixture/fixture',lineNumber:1,columnNumber:2}]}}};
let rejected=false;try{await api.evaluateMediaClock(async()=>raw,save,context)}catch{rejected=true}
check(rejected,'raw exception accepted');
let records=readFileSync(path,'utf8').trim().split('\\n').map(JSON.parse);
check(records.length===1&&JSON.stringify(records[0].rawResult)===JSON.stringify(raw)&&records[0].expression===context.expression&&records[0].status==='response','raw exception missing before assertion');
check(BigInt(records[0].hostAfterNanos)>=BigInt(records[0].hostBeforeNanos),'raw monotonic bracket lost');
rejected=false;try{await api.evaluateMediaClock(async()=>{throw new TypeError('mock transport failure')},save,context)}catch{rejected=true}
records=readFileSync(path,'utf8').trim().split('\\n').map(JSON.parse);
check(rejected&&records.length===2&&records[1].status==='transport-error'&&records[1].error.name==='TypeError'&&records[1].error.message==='mock transport failure','transport failure missing');
const value=await api.evaluateMediaClock(async()=>({result:{value:{ok:true}}}),save,context);
check(value.ok===true&&readFileSync(path,'utf8').trim().split('\\n').length===3,'successful raw response missing');
'''.replace('__PATH__', json.dumps(str(path))))

    def test_failure_dom_snapshot_is_readonly_robust_and_redacts_foreign_urls(self):
        self.bun('''
const fixture='http://fixture/fixture',videoURL='http://fixture/video.webm';
const video={src:videoURL,currentSrc:'https://foreign.invalid/private-token',error:{code:4,message:'mock decode failure'},readyState:0,networkState:3,duration:NaN,currentTime:0,muted:true,defaultMuted:false,paused:true,seeking:false,ended:false,loop:false,videoWidth:0,videoHeight:0,canPlayType:()=>''};
const navigator:any={};Object.defineProperty(navigator,'mediaSession',{get(){throw new Error('mock unavailable API')}});
const fn=new Function('location','document','navigator','performance','window','isSecureContext','return '+api.mediaClockFailureDOMJS(fixture,videoURL));
const result=fn({href:fixture},{body:{},querySelector:()=>video,visibilityState:'hidden',hidden:true},navigator,{now:()=>10,getEntriesByName:(name:string)=>{check(name===videoURL,'off-fixture resource query');return[{name:videoURL,responseStatus:416}]}},{__webextrasClockProgress:{stage:'initial-metadata-before'},__webextrasClockFailure:{name:'Error',message:'video load error'}},true);
check(result.video.error.code===4&&result.video.duration==='NaN'&&result.video.networkState===3&&result.video.currentSrc==='[off-fixture URL]'&&result.mediaSessionPresent.readError.message==='mock unavailable API','diagnostic facts lost');
check(!JSON.stringify(result).includes('private-token'),'foreign URL leaked');
const wrong=fn({href:'https://foreign.invalid/private-token'},{},null,null,null,false);check(wrong.wrongFixture===true,'foreign page was inspected');
''')

    def test_actual_phase_is_saved_before_prepare_and_failure_snapshot_runs_once(self):
        source = DRIVER.read_text()
        start = source.index('        for (const phase of (mediaServiceClock?mediaServiceClockPhases:mediaClockPhases)) {')
        start += len('        for (const phase of (mediaServiceClock?mediaServiceClockPhases:mediaClockPhases)) {')
        end = source.index('\n        }\n      }\n      run.phase = "cdp-close-fixture";', start)
        body = source[start:end]
        self.bun('''
const {mediaClockPrepareJS,mediaClockFailureDOMJS,isClockInconclusive}=api;const mediaServiceClock=false;
for(const failDiagnostic of [false,true]){
 const writes:any[]=[],labels:string[]=[],run:any={},clockReport:any={phases:[]};
 const dir='unused',server={port:1234},token='owned-nonce',fixtureURL='http://127.0.0.1:1234/owned-nonce/fixture';
 const join=(...parts:string[])=>parts.join('/');
 const writeFileSync=(_path:string,text:string)=>writes.push(JSON.parse(text));
 const evaluate=async(_expression:string,label:string)=>{labels.push(label);if(label==='prepare'){check(writes.length>0&&writes[0].phases[0].status==='preparing'&&writes[0].phases[0].attempts.length===0,'prepare ran before durable phase');throw Error('mock setup exception')}
  check(label==='failure-DOM','unexpected diagnostic operation');if(failDiagnostic)throw Error('mock diagnostic exception');return{error:{code:4}}};
 const action=()=>{throw Error('native action ran after failed preparation')};
 let failed=false;
 try{for(const phase of [api.mediaClockPhases[0]]){__BODY__}}catch{failed=true}
 const phase=writes.at(-1).phases[0];
 check(failed&&JSON.stringify(labels)===JSON.stringify(['prepare','failure-DOM'])&&phase.status==='failed'&&phase.failureDiagnosticAttempted===true&&phase.attempts.length===0,'failed phase/one-shot diagnostic lost');
 check(failDiagnostic?phase.failureDOMError.includes('mock diagnostic exception'):phase.failureDOM.error.code===4,'failure diagnostic not persisted');
}
'''.replace('__BODY__', body))

    def test_actual_bounded_http_log_preserves_response_and_excludes_secrets(self):
        source = DRIVER.read_text()
        body = source[source.index('function clockHTTP('):source.index('const server = Bun.serve')]
        self.bun('''
const body=__BODY__;
for(const enabled of [false,true]){
 const writes:any[]=[],requests:any[]=[];
 const fn=new Function('mediaClock','mediaClockHTTP','mediaClockHTTPTruncated','mediaClockHTTPAgent','writeFileSync','join','out',new Bun.Transpiler({loader:'ts'}).transformSync(body)+';return clockHTTP')(enabled,requests,false,'trace156',(_path:string,text:string)=>writes.push(JSON.parse(text)),(...parts:string[])=>parts.join('/'),'unused');
 for(let index=0;index<260;index++){
  const request=new Request('http://fixture/owned-video?private-query',{headers:{range:index?'bytes=0-':'private-range-secret',authorization:'private-auth-secret',cookie:'private-cookie-secret'}}),response=new Response('bytes',{status:206});
  check(fn(request,'video',response,5)===response,'diagnostic changed response');
 }
 if(enabled){const last=writes.at(-1),text=JSON.stringify(last);check(last.requests.length===256&&last.limit===256&&last.truncated===true&&last.requests[0].range==='[invalid]'&&last.requests[1].range==='bytes=0-'&&last.requests[1].bytes===5,'HTTP bound/status/bytes lost');check(!text.includes('private-')&&!text.includes('http://'),'HTTP log leaked a URL/credential/header');}
 else check(writes.length===0&&requests.length===0,'default HTTP path logged diagnostics');
}
'''.replace('__BODY__', json.dumps(body)))

    def test_actual_exact_target_activation_one_call_identity_and_visibility_bounds(self):
        self.bun('''
const visible={urlMatches:true,nonceMatches:true,visibilityState:'visible',hidden:false,performanceNow:20};
const hidden={...visible,visibilityState:'hidden',hidden:true};
for(const kind of ['visible','hidden','wrong-pid','wrong-url','changed-url','late-visible','verify-crosses-deadline']){
 let now=0,brings=0,reads=0,checks=0;const saves:any[]=[];
 const io={verify:()=>{++checks;if(kind==='wrong-pid')throw Error('PID mismatch');if(kind==='verify-crosses-deadline'&&checks===3)now=5001},read:async(label:string)=>{++reads;if(label==='activation-identity')return kind==='wrong-url'?{...hidden,urlMatches:false}:hidden;
   if(kind==='changed-url')return{...hidden,nonceMatches:false};if(kind==='late-visible'){now=5001;return visible}return kind==='visible'?visible:hidden;},
   bring:async()=>{++brings;return{acknowledged:true}},save:(x:any)=>saves.push(structuredClone(x)),now:()=>now,sleep:async(ms:number)=>{check(ms<=150,'unbounded sample wait');now+=ms}};
 let report:any,error:any;try{report=await api.activateMediaClockTarget('exact-owned-target',io)}catch(e){error=e}
 check(brings===(['wrong-pid','wrong-url'].includes(kind)?0:1),'activation wrong target/count');
 if(kind==='visible')check(report.status==='visible'&&report.target==='exact-owned-target'&&report.method==='Page.bringToFront'&&report.ack.acknowledged&&report.samples.length===1,'genuine visible ack missing');
 else check(api.isClockInconclusive(error)&&saves.at(-1).status==='inconclusive','bad activation accepted');
 if(kind==='verify-crosses-deadline')check(reads===1,'visibility query started after deadline during PID verification');
 if(kind==='hidden')check(now===5000&&reads<=36&&saves.at(-1).samples.every((x:any)=>x.hostElapsedMilliseconds<=5000),'hidden readiness not bounded');
}
check(api.clockVisibilityLimitMilliseconds===5000,'visibility bound widened');
const expression=api.clockIdentityJS('http://owned/fixture','owned-nonce');
const result=new Function('location','document','performance','return '+expression)({href:'http://wrong/fixture'},{querySelector:()=>({dataset:{nonce:'owned-nonce'}}),visibilityState:'visible',hidden:false},{now:()=>1});
check(result.urlMatches===false&&result.nonceMatches===true,'URL comparison bypassed');
check(!expression.includes('Emulation')&&!source.includes('setDocumentVisibilityOverride')&&!source.includes('setFocusEmulationEnabled'),'visibility/focus override introduced');
''')

    def test_actual_runtime_fatal_noise_and_forced_cleanup_classification(self):
        fatal = next(line.rstrip() for line in (ROOT/'results/webextras/native-run7/ours156/app.log').read_text().splitlines()
                     if ':FATAL:chrome/browser/ui/tabs/tab_strip_model.cc:142]' in line)
        self.bun('''
const fatal=__FATAL__;
const bad=api.runtimeLogOutcome('startup\\n'+fatal+'\\nstack', 'Browser.close; 11 owned helpers checked',true);
check(bad.status==='fatal'&&bad.fatalCount===1&&bad.fatals[0].line===2&&bad.naturalCleanup===false&&bad.errors.length===1,'actual run7 fatal falsely natural');
const clean='INFO startup complete\\nmessage mentions FATAL: as a quoted example\\n[INFO] earlier Check failed: text was diagnostic\\n{"description":"Check failed: historical"}';
const good=api.runtimeLogOutcome(clean,'Browser.close; 8 owned helpers checked',true);
check(good.status==='no-fatal-observed'&&good.naturalCleanup===true&&good.errors.length===0,'noise classified fatal');
const checkOnly=api.runtimeLogOutcome('[123:456] Check failed: actual_condition','Browser.close; 8 owned helpers checked',true);
check(checkOnly.status==='fatal'&&!checkOnly.naturalCleanup,'check-only failure ignored');
const forced=api.runtimeLogOutcome(clean,'SIGTERM of captured identities; 8 owned helpers checked',true);
check(forced.status==='forced-cleanup'&&forced.forcedCleanup===true&&!forced.naturalCleanup&&forced.errors.length===1,'forced cleanup accepted');
const missing=api.runtimeLogOutcome(null,'Browser.close; 8 owned helpers checked',true);
check(missing.status==='log-missing'&&!missing.naturalCleanup&&missing.errors.length===1,'missing log falsely clean');
check(api.runtimeLogOutcome(null,undefined,false).status==='not-launched','preflight-only status wrong');
const scanStart=source.indexOf('const ownedLog=join(dir,"app.log")');
check(scanStart>source.indexOf('cdp?.close()')&&scanStart<source.indexOf('writeFileSync(join(dir, "observations.json")'),'scan not in completed cleanup path');
check(source.includes('run.errors.push(...run.runtimeStatus.errors)'),'runtime failures not included in acceptance errors');
const acceptance=source.slice(source.indexOf('const complete ='),source.indexOf('const parity ='));
const accepts=new Function('runs','agents','actionManifest','devToolsFrontendEvents',new Bun.Transpiler({loader:'ts'}).transformSync(acceptance)+';return complete');
const manifest=Array.from({length:11},(_,index)=>({key:String(index)}));
const run=(errors:any[])=>({errors,cleanup:'Browser.close',actions:manifest.map(()=>({traceShape:[{mock:true}]}))});
check(accepts([run(good.errors)],['ours156'],manifest,false)===true,'clean fixture acceptance changed');
check(accepts([run(bad.errors)],['ours156'],manifest,false)===false&&accepts([run(forced.errors)],['ours156'],manifest,false)===false,'fatal/forced runtime accepted as complete');

'''.replace('__FATAL__', json.dumps(fatal)))

    def test_actual_default_measurement_and_pending_comparator_acceptance(self):
        # Extract the current production measurement/gate/acceptance, not a copied condition.
        source = DRIVER.read_text()
        measurement = source[source.index('for (const operation of operations) {'):source.index('      run.phase = "cdp-close-fixture";')]
        gate_start = source.index('run.fixtureComparisonStatus = fixtureVerified')
        gate = source[gate_start:source.index('          for (const row of all)', gate_start)]
        start = source.index('const boundaryDiffs: unknown[] = [];')
        acceptance = source[start:source.index('writeFileSync(join(out, "summary.json")', start)]
        path = self.directory / 'default-pending.ts'
        path.write_text('''
const mediaClock=false,mediaServiceClock=false,anyClock=false,devToolsFrontendEvents=false,fixtureVerified=false,fixtureImage='WebextrasNativeProbe.dylib',fixtureComparisonPending='pending-fixture-comparison-support';
const operations=['console-open','frontend','console-close','panel-closed','execute-evaluate','execute-malformed','execute-unknown','subscription','media-types','pip-request-error','pip-exit-error'];
const actionManifest=operations.map(operation=>({key:operation,operation}));
const agents=['trace156','ours156'],runs:any[]=[],differences:any[]=[];
const outcomeErrors=()=>[];
for(const agent of agents){const run:any={agent,actions:[],errors:[],cleanup:'mock'};runs.push(run);const registry={ownerTag:1};
const action=async(operation:string)=>({operation,ok:true,ownerTag:1,matches:1,frontendPresent:true,types:[],value:false,error:operation.startsWith('pip-')?{domain:'company.thebrowser.Arc',code:6}:null,traceShape:[{mock:true}]});
''' + measurement + '\n' + gate + '''
}
const golden=runs[0],ours=runs[1],out='unused',comparisonHelper='unused',comparisonSnapshot='unused';
const join=(...parts:string[])=>parts.join('/'),existsSync=()=>true;
const writeFileSync=()=>{throw Error('unsupported comparator invoked')};
const expectedSelectors=()=>['unused'],json=()=>{throw Error('unsupported report loaded')};
''' + acceptance + '''
if(!runs.every(run=>run.actions.length===11&&run.errors.length===1)||complete!==false||parity!==false||boundaryDiffs.length!==1)throw Error('default or pending acceptance changed');
''')
        result = subprocess.run(['bun', str(path)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_actual_service_only_js_six_exact_inputs_without_media_or_activation(self):
        self.bun(r'''
const fixture='http://127.0.0.1/nonce/fixture',nonce='nonce';
check(source.includes('<main id="webextras-probe" data-nonce='),'mock nonce element differs from actual served HTML');
for(const phase of api.mediaServiceClockPhases){
 const issued:any[]=[],events:string[]=[],window:any={};let now=0;
 const session={_state:'none',get playbackState(){return this._state},set playbackState(state:string){events.push('state:'+state);this._state=state},setPositionState(input:any){events.push('input');issued.push(structuredClone(input))}};
 const document={querySelector:(selector:string)=>{check(selector==='#webextras-probe','service touched a media element');return{dataset:{nonce}}},visibilityState:'hidden',hidden:true,
   createElement:()=>{throw Error('service created media/DOM')},body:{append:()=>{throw Error('service changed body')}}};
 const fn=new Function('location','document','navigator','window','performance','return '+api.mediaServiceClockPrepareJS(phase,fixture,nonce));
 const result=fn({href:fixture},document,{mediaSession:session},window,{now:()=>++now});
 const receipt=result.receipt;
 check(JSON.stringify(issued)==JSON.stringify([{duration:20,position:phase.base,playbackRate:phase.issuedRate}]),'exact service API input changed');
 check(events.join(',')==='state:'+(phase.rate===0?'paused':'playing')+',input','state/setter order changed');
 check(receipt.source==='public-service-only'&&receipt.phase===phase.name&&receipt.input.playbackRate!==0&&receipt.playbackState===(phase.rate===0?'paused':'playing'),'invalid public service receipt');
 check(receipt.observed.visibilityState==='hidden'&&receipt.observed.hidden&&receipt.observed.urlMatches&&receipt.observed.nonceMatches&&receipt.observed.mediaSessionPresent,'actual hidden identity observation lost');
 check(receipt.observed.performanceNow<=receipt.beforeSetPerformanceNow&&receipt.beforeSetPerformanceNow<receipt.afterSetPerformanceNow,'service receipt clocks lost');
 check(window.__webextrasClockReceipt===receipt&&window.__webextrasClockPhase===phase.name,'receipt not stored exactly');
 const observation=new Function('location','document','navigator','window','performance','return '+api.serviceClockObservationJS(fixture,nonce))({href:fixture},document,{mediaSession:session},window,{now:()=>++now});
 check(observation.positionReceipt===receipt&&observation.phase===phase.name&&observation.playbackState===receipt.playbackState,'current receipt not observed');
 for(const bad of ['wrong-url','wrong-nonce','no-api']){
  const badWindow:any={};let calls=0;
  const badSession={playbackState:'none',setPositionState:()=>++calls};
  let failed=false;try{fn({href:bad==='wrong-url'?'http://off-fixture':fixture},{...document,querySelector:()=>({dataset:{nonce:bad==='wrong-nonce'?'wrong':nonce}})},{mediaSession:bad==='no-api'?undefined:badSession},badWindow,{now:()=>++now})}catch{failed=true}
  check(failed&&calls===0&&badWindow.__webextrasClockFailure.stage==='service-identity-before','bad service identity/API issued state');
 }
 const js=api.mediaServiceClockPrepareJS(phase,fixture,nonce)+api.serviceClockObservationJS(fixture,nonce);
 for(const forbidden of ['createElement',"#webextras-clock-video",'play()','pause()','canvas','Audio','Page.bringToFront','Emulation'])check(!js.includes(forbidden),'non-service stimulus: '+forbidden);
}
check(JSON.stringify(api.mediaServiceClockPhases.map((p:any)=>[p.rate,p.base,p.issuedRate]))===JSON.stringify([[0,1.25,1],[1,1.25,1],[2,1.25,2],[0,2.25,1],[0,7.25,1.5],[1.5,7.25,1.5]]),'six fixed service phases changed');
check(api.mediaServiceClockPhases.length*17+13===115,'service public pair count changed');
''')

    def test_service_clock_relational_validator_negatives_and_nil_inconclusive(self):
        self.bun(r'''
for(const phase of api.mediaServiceClockPhases){
 const state=phase.rate===0?'paused':'playing';
 const receipt={phase:phase.name,source:'public-service-only',playbackState:state,input:{duration:20,position:phase.base,playbackRate:phase.issuedRate},
  observed:{urlMatches:true,nonceMatches:true,mediaSessionPresent:true,performanceNow:0,visibilityState:'hidden',hidden:true},beforeSetPerformanceNow:1,afterSetPerformanceNow:2};
 const actions=Array.from({length:5},(_,index)=>{
  const elapsed=index<3?index*.25:.5+(index-2)*.4,position=phase.base+phase.rate*elapsed;
  const observed={...receipt.observed,performanceNow:elapsed*1000+3,phase:phase.name,playbackState:state,positionReceipt:receipt};
  const publicCalls=(index<3?['position','playbackRate','duration','currentTime','endOfMedia']:['currentTime']).map(selector=>({selector,
   value:selector==='playbackRate'?phase.rate:selector==='duration'?20:selector==='endOfMedia'?false:position,
   clockBeforeTicks:String(1000000000+elapsed*1e9),clockAfterTicks:String(1000001000+elapsed*1e9)}));
  return {ok:true,operation:index<3?'clock-snapshot':'clock-held-time',ownerTag:1,actionKey:api.clockActionKey(phase.name,index),clockPhase:phase.name,clockAttempt:index,
   clock:{heldToken:index<3?'token'+index:'token2',dtoTag:index<3?index+1:3,timebaseNumer:1,timebaseDenom:1},publicCalls,positionReceipt:receipt,domBefore:observed,domAfter:observed};
 });
 api.validateMediaServiceClockPhase(phase,actions);
 const mutate=(fn:(x:any[])=>void)=>{const x=structuredClone(actions);fn(x);rejects(()=>api.validateMediaServiceClockPhase(phase,x),'bad service phase passed')};
 mutate(x=>{x[2].publicCalls.find((c:any)=>c.selector==='duration').value=8.208});
 mutate(x=>{x[2].publicCalls.find((c:any)=>c.selector==='playbackRate').value=phase.rate+1});
 mutate(x=>{x[2].publicCalls.find((c:any)=>c.selector==='endOfMedia').value=true});
 mutate(x=>{for(const a of x)for(const c of a.publicCalls)if(c.selector==='currentTime')c.value+=100});
 mutate(x=>{x[4].clock.heldToken='stale'});mutate(x=>{x[4].clock.dtoTag=99});mutate(x=>{x[4].ownerTag=99});
 mutate(x=>{x[4].domAfter={...x[4].domAfter,nonceMatches:false}});
 mutate(x=>{x[4].domAfter={...x[4].domAfter,mediaSessionPresent:false}});
 mutate(x=>{x[4].domAfter={...x[4].domAfter,playbackState:'none'}});
 mutate(x=>{x[4].domAfter={...x[4].domAfter,positionReceipt:{input:{duration:8}}}});
 mutate(x=>{for(const a of x)a.positionReceipt.input.position+=1});mutate(x=>{x.reverse()});
 if(phase.rate)mutate(x=>{const q=x[2].publicCalls.find((c:any)=>c.selector==='currentTime').value;for(const a of x.slice(3))a.publicCalls[0].value=q});
 else mutate(x=>{x[2].publicCalls.find((c:any)=>c.selector==='currentTime').value=phase.base+.25});
 const nil=structuredClone(actions);nil[0]={...nil[0],ok:false,probeError:'position did not return an actual ArcMediaPosition',publicCalls:[{selector:'position',resultClass:null}]};
 let error:any;try{api.validateMediaServiceClockPhase(phase,nil)}catch(e){error=e}check(api.isClockInconclusive(error),'authentic nil receipt not inconclusive');
 const wrongClass={...nil[0],publicCalls:[{selector:'position',resultClass:'UnexpectedClass'}]};check(!api.isNilClockPosition(wrongClass),'wrong class treated as nil');
}
''')

    def test_actual_service_mode_orchestration_has_no_activation_or_resource_stimulus(self):
        source = DRIVER.read_text()
        body = source[source.index('      if (anyClock) {'):source.index('      run.phase = "cdp-close-fixture";')]
        self.bun(r'''
const {evaluateMediaClock,mediaClockPolicy,mediaServiceClockPhases,mediaClockPhases,clockActionKey,mediaServiceClockPrepareJS,serviceClockObservationJS,validateMediaServiceClockPhase,isNilClockPosition,isClockInconclusive}=api;
const assert=check;const mediaClock=false,mediaServiceClock=true,anyClock=true,probeMode='media-service-clock';
const dir='unused',server={port:1234},token='owned-nonce',fixtureURL='http://127.0.0.1:1234/owned-nonce/fixture',agent='original',port=12345,pid=1,target='owned-target';
const mediaClockIdentity=undefined,run:any={actions:[],errors:[]},registry={ownerTag:1},window:any={},writes:any[]=[];
const join=(...parts:string[])=>parts.join('/'),writeFileSync=(path:string,text:string)=>writes.push({path,body:JSON.parse(text)});
let now=0,sequence=0,tag=0,held:any,releaseCount=0,position:any;
const session={playbackState:'none',setPositionState(input:any){position={...input,at:now}}};
const document={querySelector:(selector:string)=>{check(selector==='#webextras-probe','media element queried');return{dataset:{nonce:token}}},visibilityState:'hidden',hidden:true,
 createElement:()=>{throw Error('created media')},body:{append:()=>{throw Error('appended media')}}};
const cdp={targetCommand:async(actualTarget:string,method:string,params:any)=>{check(actualTarget===target&&method==='Runtime.evaluate','activation/non-evaluation command issued');
 return{result:{value:await new Function('location','document','navigator','window','performance','return '+params.expression)({href:fixtureURL},document,{mediaSession:session},window,{now:()=>now})}}}};
const verifyAgentEndpoint=()=>pid,sleep=async(ms:number)=>{now+=ms},outcomeErrors=()=>[];
const action=async(operation:string,extra:any={})=>{
 if(operation==='clock-release'){check(extra.heldToken===held.token,'release token changed');held=undefined;releaseCount++;return{ok:true,publicCalls:[],timeline:[]}}
 const id='native-'+(++sequence);if(operation==='clock-snapshot')held={token:id,tag:++tag,input:{...position}};
 else check(extra.heldToken===held.token,'held action changed DTO token');
 const q=held.input.position+(session.playbackState==='paused'?0:held.input.playbackRate)*(now-held.input.at)/1000;
 const selectors=operation==='clock-snapshot'?['position','playbackRate','duration','currentTime','endOfMedia']:['currentTime'];
 return{id,operation,ok:true,matches:1,ownerTag:1,callbacks:[],timeline:[],clock:{heldToken:held.token,dtoTag:held.tag,timebaseNumer:1,timebaseDenom:1},
 publicCalls:selectors.map(selector=>({selector,value:selector==='currentTime'?q:selector==='duration'?20:selector==='playbackRate'?(session.playbackState==='paused'?0:held.input.playbackRate):false,
 clockBeforeTicks:String(1000000000+now*1e6),clockAfterTicks:String(1000001000+now*1e6)}))};
};
__BODY__
check(run.actions.length===30&&releaseCount===6&&run.errors.length===0,'service orchestration missing actions/releases or false failure');
check(run.actions.reduce((count:number,a:any)=>count+a.publicCalls.length,0)===102,'service clock public pair count wrong');
const phases=writes.filter((write:any)=>write.path.endsWith('media-clock.json')).at(-1).body.phases;
check(phases.length===6&&phases.every((phase:any)=>phase.functional==='pass'&&phase.status==='complete'&&phase.attempts.length===5),'six service phases did not pass actual branch');
const evaluations=writes.filter((write:any)=>write.path.endsWith('clock-evaluations.jsonl'));
check(evaluations.length===66&&evaluations.every((entry:any)=>entry.body.rawResult&&!entry.body.expression.includes('video')&&!entry.body.label.startsWith('activation')),'raw evaluations missing or service activated/loaded video');
'''.replace('__BODY__', body))

    def test_modes_mutually_exclusive_and_video_helpers_unchanged(self):
        self.bun(r'''
check(api.clockModeFromArgs([])==='default'&&api.clockModeFromArgs(['--media-clock'])==='media-clock'&&api.clockModeFromArgs(['--media-service-clock'])==='media-service-clock','mode selection wrong');
for(const flags of [['--media-clock','--media-service-clock'],['--media-service-clock','--media-clock'],['--media-service-clock','--media-service-clock'],['--media-clock','--media-clock']])rejects(()=>api.clockModeFromArgs(flags),'ambiguous mode accepted');
const measurement=source.slice(source.indexOf('      if (anyClock) {'),source.indexOf('      run.phase = "cdp-close-fixture";'));
const activationStart=measurement.indexOf('if(mediaClock){'),activationEnd=measurement.indexOf('const clockReport');
check(activationStart>=0&&activationEnd>activationStart&&measurement.slice(activationStart,activationEnd).includes('Page.bringToFront'),'video-only activation guard lost');
check(measurement.match(/Page.bringToFront/g)?.length===2,'activation path duplicated');
const manifestBody=source.slice(source.indexOf('const actionManifest ='),source.indexOf('try {',source.indexOf('const actionManifest =')));
for(const mode of ['default','media-clock','media-service-clock']){
 const operations=Array.from({length:11},(_,i)=>'op'+i);
 const fn=new Function('operations','anyClock','mediaServiceClock','mediaClockPhases','mediaServiceClockPhases','clockActionKey','expectedSelectors',new Bun.Transpiler({loader:'ts'}).transformSync(manifestBody)+';return actionManifest');
 const manifest=fn(operations,mode!=='default',mode==='media-service-clock',api.mediaClockPhases,api.mediaServiceClockPhases,api.clockActionKey,()=>[]);
 check(manifest.length===(mode==='default'?11:41),'mode action count changed');
}
''')
        archived = ROOT/'logs/wp/webextras-native8-service-lane-inputs/arc/browser/webextras_native_probe.ts'
        old, current = archived.read_text(), DRIVER.read_text()
        for begin, end in [('const mediaClockPhases =','// Separate public service-only lane:'),
                           ('function validateClockPhase(', 'function validateClockReceivers('),
                           ('function mediaClockPrepareJS(', '// Persist the complete Runtime.evaluate response'),
                           ('function activateMediaClockTarget(', 'function runtimeLogOutcome(')]:
            old_end = 'function clockActionKey(' if end.startswith('// Separate') else end
            self.assertEqual(current[current.index(begin):current.index(end)], old[old.index(begin):old.index(old_end)])

    def test_actual_native_retained_dto_stale_token_and_disabled_gate(self):
        # Include the actual helper routines. These ObjC mocks construct no native/C++ DTO.
        path = self.directory / 'native.m'
        path.write_text('#import ' + json.dumps(str(NATIVE)) + r'''
@interface ArcMediaPosition : NSObject
@end
@implementation ArcMediaPosition
-(double)playbackRate{return 1;}
-(double)duration{return 8.208;}
-(double)currentTime{return 1.25;}
-(BOOL)endOfMedia{return NO;}
@end
static __weak ArcMediaPosition* seen;
@interface MockMedia : NSObject
@end
@implementation MockMedia
-(id)position {ArcMediaPosition* value=[ArcMediaPosition new];seen=value;return value;}
@end
static WebextrasProbeWork* Work(void){WebextrasProbeWork* w=[WebextrasProbeWork new];w.publicCalls=[NSMutableArray array];w.timeline=[NSMutableArray array];w.response=[NSMutableDictionary dictionary];return w;}
static void Check(BOOL value,NSString* message){if(!value){NSLog(@"%@",message);exit(1);}}
int main(void){@autoreleasepool{
 WebextrasProbeEntry* entry=[WebextrasProbeEntry new];MockMedia* media=[MockMedia new];entry.media=media;
 WebextrasProbeWork* work=Work();
 Check(ClockOperation(work,entry,@{@"id":@"a"},@"clock-snapshot")!=nil,@"disabled operation accepted");
 Check(work.publicCalls.count==0,@"disabled operation called engine");gMediaClock=YES;
 @autoreleasepool{work=Work();Check(!ClockOperation(work,entry,@{@"id":@"a"},@"clock-snapshot"),@"snapshot rejected");Check(work.publicCalls.count==5,@"wrong snapshot count");}
 Check(seen!=nil,@"real receipt not strongly retained");
 work=Work();Check(!ClockOperation(work,entry,@{@"heldToken":@"a"},@"clock-held-time"),@"held sample rejected");Check(work.publicCalls.count==1,@"held call fetched fresh DTO");
 work=Work();Check(ClockOperation(work,entry,@{@"heldToken":@"stale"},@"clock-held-time")!=nil&&work.publicCalls.count==0,@"stale token called getter");
 WebextrasProbeEntry* other=[WebextrasProbeEntry new];other.media=media;
 Check(ClockOperation(work,other,@{@"heldToken":@"a"},@"clock-held-time")!=nil,@"cross-owner token accepted");
 work=Work();Check(ClockOperation(work,entry,@{@"heldToken":@"stale"},@"clock-release")!=nil&&seen!=nil,@"stale release dropped held object");
 Check(!ClockOperation(work,entry,@{@"heldToken":@"a"},@"clock-release"),@"release rejected");Check(seen==nil,@"released DTO retained");
 Check(ClockOperation(Work(),entry,@{@"heldToken":@"a"},@"clock-held-time")!=nil,@"released token accepted");
 puts("actual native snapshot5/held1/strong-retain/stale-owner/disabled/release cases passed");
}return 0;}
''')
        binary = self.directory / 'native'
        result = subprocess.run(['xcrun', 'clang', '-fobjc-arc', '-Wall', '-Wextra', '-Werror', '-framework', 'Foundation',
                                 '-framework', 'AppKit', str(path), '-o', str(binary)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        result = subprocess.run([str(binary)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn('strong-retain', result.stdout)


if __name__ == '__main__':
    unittest.main(verbosity=2)
