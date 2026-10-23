// WRITE-only morning adaptation of the frozen public current-editor AX route.
// Ready for morning native recording, runtime unverified. No AXConfirm,
// navigation, block-completion, parity, or graceful-shutdown success is inferred
// from the archived pair. command-bar-navigation remains separate coverage.
import type { ScenarioContext, NativeSnapshot } from "../launch-slice/run";
import { verifyAgentEndpoint } from "../../../arc/tools/agent-cdp";
import { existsSync, mkdirSync, readFileSync, readdirSync, realpathSync, renameSync, statSync, writeFileSync } from "node:fs";
import { join, resolve } from "node:path";
import { CommandBarSnapshotPending, validateCommandBarRaw } from "./arcmod-command-bar-raw";
export { chromiumArgs } from "./command-bar-autocomplete";
export const morningPlan = [
  "After unlock, use only the exact disposable native agent and an empty browser profile.",
  "Open one loopback fixture, observe or open New Tab once, and pin its sole full window.",
  "Call the owned actual current editor's advertised public AX setter exactly once.",
  "Require exact field readback and actual same-window fixture suggestions.",
  "Gate new app-origin exact autocomplete, paired true return and complete main raw stack.",
  "Use standard graceful quit; do not infer navigation, block completion or parity.",
];
const DIRECTORY="arcmod-command-bar-driver", IMAGE="ScenarioCommandBarDriver.dylib";
const SOURCE_SHA="fa990a89dab483d8453e0a54aa0e580bcce442e6cbb9ff5bf8ac22b7561fd4ff";
type Cursor={available:boolean;byteOffset:number;device:number;inode:number};
type Field={identifier:string;windowNumber:number;concreteNSTextField:boolean;descendant:boolean;hidden:boolean;secure:boolean;editable:boolean;enabled:boolean;valueState?:SafeValue};
type SafeValue={valueAllowed:boolean;equalsFixture:boolean;value?:string};
type Receipt={id:string;nonce:string;pid:number;bundle:string;fixtureURL:string;operation:string;mainThread:boolean;functionalAcceptance:boolean;engineCallsPerformed:number;noRetry:boolean;
  status:string;selectedWindowNumber:number;windowUnique:boolean;eligibleWindows:{windowNumber:number}[];candidateCount:number;fields:Field[];truncated:boolean;budgetExceeded:boolean;
  route?:string;routeOwnershipVerified?:boolean;setterReturned?:boolean;actionConsumed:boolean;actualSetterCalls:number;textSetterAttempts:number;nativeActionsPerformed:number;
  before?:SafeValue;after?:SafeValue;fieldReadback?:SafeValue;recorderCursorBeforeIntent?:Cursor;recorderCursorBeforeSetter?:Cursor;recorderCursorAfterSetter?:Cursor;
  suggestions?:{suggestionsRootCount:number;exactFixtureStaticTextObserved:boolean;exactFixtureStaticTextCount:number;truncated:boolean;budgetExceeded:boolean;functionalAcceptance:boolean}};
function readJSON<T>(path:string):T|undefined {
  try{return JSON.parse(readFileSync(path,"utf8")) as T;}
  catch(error){if((error as NodeJS.ErrnoException).code==="ENOENT")return;throw error;}
}
export default async function scenario(c:ScenarioContext) {
  const assert=(name:string,ok:unknown,detail="")=>{c.assert(name,ok,detail);if(!ok)throw new Error(name+(detail?": "+detail:""));};
  assert("morning command bar requires caller-aware standard recording",c.recorded&&!c.traced);
  assert("morning command bar uses exact disposable engine agent",["trace156","ours156"].includes(c.slug));
  assert("morning command bar starts with empty disposable browser storage",["User Data","Agent Chromium Data"].every(name=>!existsSync(join(c.data,name))||readdirSync(join(c.data,name)).length===0));
  const helper=resolve(import.meta.dir,"../../../arc/build/agent-native"),helperSource=resolve(import.meta.dir,"../../../arc/tools/agent-native.swift");
  assert("morning command bar has current native helper without auto-build",existsSync(helper)&&statSync(helper).mtimeMs>=statSync(helperSource).mtimeMs);
  const directory=join(c.outDir,DIRECTORY);
  assert("morning command bar driver output is fresh",!existsSync(directory)||readdirSync(directory).length===0);
  mkdirSync(directory,{recursive:true,mode:0o700});
  assert("morning command bar request directory is private",(statSync(directory).mode&0o077)===0);
  const request=join(directory,"action-request.json"),library=join(directory,IMAGE),nonce=`omnibox-action-${crypto.randomUUID()}`,url=`${c.base}/a`,bundle=realpathSync(c.app);
  const compile=Bun.spawnSync(["xcrun","clang","-dynamiclib","-fobjc-arc","-Wall","-Wextra","-Werror","-framework","Foundation","-framework","AppKit",join(import.meta.dir,"arcmod-command-bar-driver.m"),"-o",library],{stdout:"pipe",stderr:"pipe"});
  writeFileSync(join(directory,"compile.log"),compile.stdout.toString()+compile.stderr.toString());
  assert("morning command bar driver compiles with strict warnings",compile.success,compile.stderr.toString().slice(-1500));
  writeFileSync(join(directory,"provenance.json"),JSON.stringify({source:"/tmp/omnibox-commandbar-action-probe/probe.m",sourceSHA256:SOURCE_SHA,archivedPair:"logs/wp/omnibox.real-ui-pair2",adaptation:"Current-editor public NSAccessibility setter only, request directory confined below this outDir, ARC_SCENARIO launch options, exact-once mutation, complete live main raw proof. The original pair's runtime reports remain false and unchanged; its derived causal proof is not acceptance. No block-callback, navigation, parity or successful-quit inference."},null,2));
  const launch=c.launch({libraries:[library],environment:{ARC_SCENARIO_COMMAND_BAR_BUNDLE:bundle,ARC_SCENARIO_COMMAND_BAR_ROOT:realpathSync(c.outDir),ARC_SCENARIO_COMMAND_BAR_URL:url,ARC_SCENARIO_COMMAND_BAR_NONCE:nonce,ARC_SCENARIO_COMMAND_BAR_REQUEST:request}});
  const observations:Receipt[]=[];let proofBytes:Buffer|undefined,proofIdentity:Cursor|undefined;
  const rawPath=join(c.outDir,"launch-1","record.jsonl");
  try {
    if(!await c.ready(launch,"morning command bar agent launch"))return;
    const pid=launch.pid();assert("morning command bar exposes owned actual PID",pid!==undefined);
    const owner=()=>{if(launch.pid()!==pid||verifyAgentEndpoint(c.slug,c.port)!==pid)throw new Error("command bar PID/endpoint ownership changed");};
    // Only pure reads are retried. Invalid JSON/ownership errors propagate, and
    // no action is passed to this polling helper.
    const poll=async<T>(read:()=>T|undefined,ms:number):Promise<T|undefined>=>{
      const end=Date.now()+ms;
      while(Date.now()<end){owner();const value=read();owner();if(value!==undefined)return value;await c.sleep(250);}return;
    };
    const ready=await poll(()=>{
      const v=readJSON<{installed:boolean;pid:number;nonce:string;bundle:string;fixtureURL:string;mainThread:boolean;maximumSetterAttempts:number;engineCallsPerformed:number;actualSetterCalls:number}>(request+".ready.json");
      if(!v)return;
      if(v.pid!==pid||v.nonce!==nonce||v.bundle!==bundle||v.fixtureURL!==url)throw new Error("command bar ready identity mismatch");
      return v;
    },10000);
    assert("morning command bar driver ready in exact owned process",ready?.installed===true&&ready.mainThread===true&&ready.maximumSetterAttempts===1&&ready.engineCallsPerformed===0&&ready.actualSetterCalls===0);
    owner();const fixture=c.open(url);
    assert("morning command bar exact causal fixture is loaded",await c.titleIs(fixture,c.EXPECT.aTitle)&&c.evaluate(fixture,"location.href")===url);owner();
    const native=()=>{
      owner();const state=c.native() as NativeSnapshot&{bundle?:string;webContentExcluded?:boolean};
      if(state.pid!==pid||state.bundle!==`company.thebrowser.Browser.mod.agent.${c.slug}`||state.truncated!==false||state.webContentExcluded!==true)throw new Error("command bar native state identity/scope/truncation gap");
      return state;
    };
    let state=native(),fields=state.elements.filter(n=>n.identifier==="commandBarTextField");
    assert("morning command bar initial field is unambiguous",fields.length<=1);
    if(fields.length===0){
      const items=state.elements.filter(n=>n.identifier==="newTabMenuItem"&&n.role==="AXMenuItem"&&n.enabled&&n.actions.includes("AXPress"));
      assert("morning command bar observes one advertised New Tab action",items.length===1);
      owner();writeFileSync(join(directory,"new-tab-intent.json"),JSON.stringify({pid,nonce,identifier:"newTabMenuItem",maximumAttempts:1,retries:false}),{mode:0o600});
      // The handle is from the immediately preceding snapshot. No intervening
      // native snapshot invalidates it. Dispatch errors are never replayed.
      const result=c.action(items[0]!) as {receipt?:{pid?:number;bundle?:string;action?:string;dispatched?:boolean}};
      writeFileSync(join(directory,"new-tab-receipt.json"),JSON.stringify(result.receipt??null,null,2));owner();
      const receipt=result.receipt;
      assert("morning command bar New Tab dispatch has exact receipt",receipt!==undefined&&receipt.pid===pid&&receipt.bundle===`company.thebrowser.Browser.mod.agent.${c.slug}`&&receipt.action==="press"&&receipt.dispatched===true);
      const exposed=await poll(()=>{const value=native();return value.elements.some(n=>n.identifier==="commandBarTextField")?value:undefined;},5000);
      assert("morning command bar appears after sole New Tab attempt",exposed!==undefined);state=exposed!;
    }
    fields=state.elements.filter(n=>n.identifier==="commandBarTextField");
    assert("morning command bar has one enabled empty native field",fields.length===1&&fields[0]!.role==="AXTextField"&&fields[0]!.enabled&&fields[0]!.value==="");
    let sequence=0;
    const action=async(operation:"inventory"|"setFixtureURL"|"observeFixtureSuggestions",windowNumber?:number):Promise<Receipt>=>{
      owner();if(sequence>=8)throw new Error("command bar helper request budget exhausted");
      const id=`${nonce}-${++sequence}`;
      const value={id,operation,pid,nonce,fixtureURL:url,isolatedProfileVerified:true,freshProfileVerified:true,causalFixtureVerified:true,...(windowNumber===undefined?{}:{windowNumber})};
      writeFileSync(request+".tmp",JSON.stringify(value),{mode:0o600});renameSync(request+".tmp",request);
      writeFileSync(join(directory,`${sequence}-${operation}.request.json`),JSON.stringify(value,null,2),{mode:0o600});
      const response=await poll(()=>{
        const v=readJSON<Receipt>(request+".response.json");if(!v||v.id!==id)return;
        if(v.pid!==pid||v.nonce!==nonce||v.bundle!==bundle||v.fixtureURL!==url||v.operation!==operation)throw new Error("command bar response identity mismatch");
        return v;
      },10000);
      if(!response)throw new Error(`command bar ${operation} timed out; uncertain mutation will not be replayed`);
      observations.push(response);writeFileSync(join(directory,`${sequence}-${operation}.response.json`),JSON.stringify(response,null,2));
      if(response.mainThread!==true||response.functionalAcceptance!==false||response.engineCallsPerformed!==0||response.noRetry!==true)throw new Error("command bar helper public scope/main-thread contract mismatch");
      return response;
    };
    const inventory=await action("inventory");
    const ownedField=(receipt:Receipt,windowNumber:number)=>receipt.selectedWindowNumber===windowNumber&&receipt.windowUnique===true&&receipt.truncated===false&&receipt.budgetExceeded===false&&receipt.candidateCount===1&&receipt.fields.length===1&&receipt.fields[0]!.identifier==="commandBarTextField"&&receipt.fields[0]!.windowNumber===windowNumber&&receipt.fields[0]!.concreteNSTextField===true&&receipt.fields[0]!.descendant===true&&receipt.fields[0]!.hidden===false&&receipt.fields[0]!.secure===false&&receipt.fields[0]!.editable===true&&receipt.fields[0]!.enabled===true;
    const window=inventory.selectedWindowNumber;
    assert("morning command bar pins sole full window and owned empty field",Number.isSafeInteger(window)&&window>0&&inventory.eligibleWindows.length===1&&inventory.eligibleWindows[0]!.windowNumber===window&&ownedField(inventory,window)&&inventory.status==="inventory-unexercised"&&inventory.actualSetterCalls===0&&inventory.textSetterAttempts===0&&inventory.nativeActionsPerformed===0&&inventory.fields[0]!.valueState?.valueAllowed===true&&inventory.fields[0]!.valueState?.value==="");
    const setter=await action("setFixtureURL",window);
    const once=(v:Receipt)=>v.actualSetterCalls===1&&v.textSetterAttempts===1&&v.nativeActionsPerformed===1&&v.actionConsumed===true;
    const exact=(v:SafeValue|undefined)=>v?.valueAllowed===true&&v.equalsFixture===true&&v.value===url;
    assert("morning command bar performs sole owned current-editor public setter",ownedField(setter,window)&&setter.status==="public-setter-returned-unverified"&&setter.route==="current-editor"&&setter.routeOwnershipVerified===true&&setter.setterReturned===true&&once(setter)&&setter.before?.valueAllowed===true&&setter.before.value===""&&exact(setter.after));
    const intent=readJSON<Receipt>(request+".intent.json");
    assert("morning command bar persisted consumed intent before sole setter",intent?.id===setter.id&&intent.pid===pid&&intent.nonce===nonce&&intent.bundle===bundle&&intent.fixtureURL===url&&intent.operation==="setFixtureURL"&&intent.mainThread===true&&intent.functionalAcceptance===false&&intent.engineCallsPerformed===0&&intent.status==="intent-before-public-setter"&&intent.route==="current-editor"&&intent.routeOwnershipVerified===true&&intent.setterReturned===false&&intent.actionConsumed===true&&intent.actualSetterCalls===0&&intent.textSetterAttempts===0&&intent.nativeActionsPerformed===0&&intent.selectedWindowNumber===window);
    const cursor=setter.recorderCursorBeforeSetter;
    const cursorValid=(v:Cursor|undefined):v is Cursor=>v?.available===true&&Number.isSafeInteger(v.byteOffset)&&v.byteOffset>=0&&Number.isSafeInteger(v.device)&&Number.isSafeInteger(v.inode);
    const beforeIntent=setter.recorderCursorBeforeIntent,afterSetter=setter.recorderCursorAfterSetter;
    assert("morning command bar recorder cursors bound actual setter",cursorValid(cursor)&&cursorValid(beforeIntent)&&cursorValid(afterSetter)&&beforeIntent.byteOffset<=cursor.byteOffset&&cursor.byteOffset<=afterSetter.byteOffset&&[beforeIntent,afterSetter].every(v=>v.device===cursor.device&&v.inode===cursor.inode));
    let suggested=false;
    for(let i=0;i<6;i++){
      const observation=await action("observeFixtureSuggestions",window);
      assert(`morning command bar observation ${i+1} retains owned exact field`,observation.status==="observed-only-no-acceptance-assertion"&&ownedField(observation,window)&&once(observation)&&exact(observation.fieldReadback));
      const suggestions=observation.suggestions;
      assert(`morning command bar observation ${i+1} has complete same-window suggestion scan`,suggestions?.truncated===false&&suggestions.budgetExceeded===false&&suggestions.functionalAcceptance===false&&suggestions.suggestionsRootCount===1&&Number.isSafeInteger(suggestions.exactFixtureStaticTextCount)&&suggestions.exactFixtureStaticTextCount>=0);
      if(suggestions!.exactFixtureStaticTextObserved===true&&suggestions!.exactFixtureStaticTextCount>=1){suggested=true;break;}
      if(i<5)await c.sleep(1000);
    }
    assert("morning command bar exposes actual exact-URL suggestion descendants",suggested);
    const end=Date.now()+5000;let gate:ReturnType<typeof validateCommandBarRaw>|undefined,lastPending="";
    while(Date.now()<end){
      owner();const stat=statSync(rawPath);
      if(stat.dev!==cursor!.device||stat.ino!==cursor!.inode)throw new Error("command bar recorder file identity changed");
      const bytes=readFileSync(rawPath),afterRead=statSync(rawPath);
      if(afterRead.dev!==cursor!.device||afterRead.ino!==cursor!.inode||afterRead.size<bytes.length)throw new Error("command bar recorder file identity/size changed while reading");
      try{gate=validateCommandBarRaw(bytes,cursor!.byteOffset,url);proofBytes=bytes;proofIdentity=cursor;break;}
      catch(error){if(!(error instanceof CommandBarSnapshotPending))throw error;lastPending=error.message;}
      await c.sleep(250);
    }
    assert("morning command bar raw proves new app query true return and complete live main boundary",gate!==undefined,lastPending);
    writeFileSync(join(directory,"proof.boundary.raw.jsonl"),proofBytes!);
    writeFileSync(join(directory,"raw-gate.json"),JSON.stringify(gate,null,2));
    assert("morning command bar retains causal fixture without confirming navigation",c.evaluate(fixture,"location.href")===url&&await c.titleIs(fixture,c.EXPECT.aTitle));owner();
  } finally {
    writeFileSync(join(directory,"observations.json"),JSON.stringify(observations,null,2));
    // Standard harness failures, including required SIGTERM, remain failures.
    await c.quit(launch,"morning command bar standard graceful quit");
  }
  if(proofBytes){
    const stat=statSync(rawPath);
    assert("morning command bar proof remains exact prefix of unchanged recorder",stat.dev===proofIdentity?.device&&stat.ino===proofIdentity?.inode&&readFileSync(rawPath).subarray(0,proofBytes.length).equals(proofBytes));
  }
}
