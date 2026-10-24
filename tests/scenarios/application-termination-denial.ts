// WRITE-only additional morning scenario. Ready for native recording, runtime
// UNVERIFIED; independent of the ten ArcMod scripts. Fail closed on unexpected
// application-level Quit confirmation, missing events/controls or uncertain AX.
// Public menu route: ../arc/ghidra/decomp/0x1002.c:29704 (userQuitApp:),
// :29318/:41115 (main dispatch), :29382 (NSApp terminate:). No private calls.
// Observed controls: results/golden/arc-1.170-dialogs-beforeunload/native-1.json
// contains _NS:988 Quit Arc and the _NS:87 sheet/title/body/Cancel/OK below.
// ArcBrowserApplication metadata: arc/tools/arccore-1.170.0-objc.tsv:370.
// Runtime.bindingCalled is a fixture sidecar observation, never a recorder
// callback/block claim. Page.javascriptDialog* are the actual CDP page events.
import type { ScenarioContext, NativeNode, NativeSnapshot } from "../launch-slice/run";
import { verifyAgentEndpoint, type CDPEvent } from "../../../arc/tools/agent-cdp";
import { nativeState } from "../../../arc/tools/agent-native";
import { isTabFallback } from "./native";
import { appendFileSync, chmodSync, existsSync, lstatSync, mkdirSync, readFileSync, readdirSync, realpathSync, statSync, writeFileSync } from "node:fs";
import { basename, join, resolve } from "node:path";
import { assertTerminationRawPrefix, validateTerminationPhases, type QuitPhase } from "./application-termination-denial-raw";

export const morningPlan = [
  "Fresh isolated trace156/ours156 profile; exact PID/executable/start/endpoint and one native fixture window.",
  "One trusted CDP button click installs a live beforeunload handler with sticky activation and document nonce.",
  "Native Quit Arc once; genuine exact-session beforeunload plus owned Leave site? sheet; Cancel once; same document remains responsive.",
  "Fresh Quit Arc once; second genuine handler/prompt; native OK once; browser and recorded helpers disappear within 15 seconds without scenario signals or Browser.close.",
  "Require two genuine main app public tryToTerminate:<block> call/return pairs; no block completion, numeric enum semantics, OS exit code or parity claim.",
];
export function fixture(req:Request) {
  if(new URL(req.url).pathname!=="/a")return;
  return new Response(`<!doctype html><meta charset="utf-8"><title>launch-slice A</title>
    <button id="termination-install" style="position:fixed;left:24px;top:24px;width:240px;height:60px">Install beforeunload</button>
    <div id="ls-marker-7f3a">fixture A</div><script>
    window.__terminationProbe={nonce:crypto.randomUUID(),installs:0,handlerCount:0,trusted:false};
    document.querySelector('#termination-install').onclick=function(event){
      const p=window.__terminationProbe;p.installs++;p.trusted=event.isTrusted;
      if(!event.isTrusted||p.installs!==1)return;
      window.onbeforeunload=function(event){
        p.handlerCount++;
        window.__arcTerminationEvent(JSON.stringify({nonce:p.nonce,url:location.href,handlerCount:p.handlerCount,installs:p.installs,trusted:p.trusted,sticky:navigator.userActivation.hasBeenActive}));
        event.preventDefault();event.returnValue='fixture';return 'fixture';
      };
    };</script>`,{headers:{"content-type":"text/html; charset=utf-8","cache-control":"no-store"}});
}
type Node=NativeNode&{fingerprint:string};
type Snapshot=Omit<NativeSnapshot,"elements">&{bundle?:string;webContentExcluded?:boolean;elements:Node[]};
type ProcessIdentity={pid:number;ppid:number;executable:string;start:string};
type ProcessObservationOutput={success:boolean;exitCode:number|null;signalCode?:unknown;stdout:string;stderr:string};
type Endpoint={webSocketDebuggerUrl?:unknown};
type Probe={nonce:string;installs:number;handlerCount:number;trusted:boolean;sticky:boolean;live:boolean;url:string;title:string;responsive:number};

/** Shutdown observations only. Pre-action owner()/identity() stay strict. A
 * known process may naturally disappear after the table read, before ps reads
 * lstart. Only ps's exact empty exit-1 result AND a fresh absent table permit
 * that race; a reused PID, malformed output, signal, timeout or tool error fails. */
export function observeTerminationIdentity(
  observed:Pick<ProcessIdentity,"pid"|"executable">, expected:ProcessIdentity,
  readStart:()=>ProcessObservationOutput, confirmAbsent:()=>boolean,
):"alive"|"gone" {
  if(observed.pid!==expected.pid||observed.executable!==expected.executable)throw new Error("owned executable/PID changed during termination observation");
  const result=readStart();
  if(!result.success){
    if(result.exitCode===1&&result.stdout===""&&result.stderr===""&&result.signalCode==null){
      if(!confirmAbsent())throw new Error("ps reported absent PID still present in fresh process table");
      return "gone";
    }
    throw new Error("termination start-time process observation failed");
  }
  const start=result.stdout.trim();
  if(result.exitCode!==0||result.signalCode!=null||result.stderr!==""||
    !/^(Sun|Mon|Tue|Wed|Thu|Fri|Sat)\s+(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)\s+\d{1,2}\s+\d{2}:\d{2}:\d{2}\s+\d{4}$/.test(start)||start!==expected.start)
    throw new Error("owned PID start time changed or malformed during termination observation");
  return "alive";
}

export default async function scenario(c:ScenarioContext) {
  const assert=(name:string,ok:unknown,detail="")=>{c.assert(name,ok,detail);if(!ok)throw new Error(name+(detail?": "+detail:""));};
  assert("termination denial uses caller-aware standard disposable recording",c.recorded&&!c.traced&&["trace156","ours156","rel156"].includes(c.slug));
  const expectedApp=resolve(import.meta.dir,`../../../arc/build/ArcModAgent-${c.slug}.app`);
  assert("termination denial uses exact agent app",c.app===expectedApp&&realpathSync(c.app)===expectedApp);
  const executable=join(expectedApp,"Contents/MacOS/Arc"),bundle=`company.thebrowser.Browser.mod.agent.${c.slug}`;
  const support=join(process.env.HOME!,"Library/Application Support"),profile=join(support,`ArcModAgent-${c.slug}`);
  assert("termination denial uses exact disposable profile",c.data===profile);
  const supportCanonical=realpathSync(support),profileCanonical=join(supportCanonical,`ArcModAgent-${c.slug}`);
  const noFollow=(path:string)=>{try{return lstatSync(path);}catch(error){if((error as NodeJS.ErrnoException).code==="ENOENT")return;throw error;}};
  let profileIdentity:{device:number;inode:number}|undefined;
  const inspectProfile=(phase:string,required:boolean,fresh=false)=>{
    assert(`${phase}: canonical profile parent unchanged`,realpathSync(support)===supportCanonical);
    const info=noFollow(profile);
    if(!info){assert(`${phase}: only an uncreated disposable profile may be absent`,!required&&profileIdentity===undefined);return;}
    assert(`${phase}: profile is a directory without symlink`,!info.isSymbolicLink()&&info.isDirectory());
    assert(`${phase}: profile has exact canonical parent`,realpathSync(profile)===profileCanonical);
    if(fresh)assert(`${phase}: no prior profile contents`,readdirSync(profile).every(name=>["User Data","Agent Chromium Data"].includes(name)));
    for(const name of ["User Data","Agent Chromium Data"]){
      const path=join(profile,name),child=noFollow(path);
      assert(`${phase}: ${name} absent or non-symlink directory`,child===undefined||(!child.isSymbolicLink()&&child.isDirectory()));
      if(fresh)assert(`${phase}: ${name} initially empty`,child===undefined||readdirSync(path).length===0);
    }
    const after=noFollow(profile);
    assert(`${phase}: no-follow directory identity stable`,after!==undefined&&!after.isSymbolicLink()&&after.isDirectory()&&after.dev===info.dev&&after.ino===info.ino);
    assert(`${phase}: original profile inode retained`,profileIdentity===undefined||(profileIdentity.device===info.dev&&profileIdentity.inode===info.ino));
    return {device:info.dev,inode:info.ino};
  };
  profileIdentity=inspectProfile("prelaunch",false,true);
  const directory=join(c.outDir,"application-termination-denial-evidence"),prior=noFollow(directory);
  assert("termination evidence directory is fresh and not a symlink",prior===undefined||(!prior.isSymbolicLink()&&prior.isDirectory()&&readdirSync(directory).length===0));
  mkdirSync(directory,{recursive:true,mode:0o700});chmodSync(directory,0o700);
  assert("termination evidence directory is confined to this output",realpathSync(directory)===join(realpathSync(c.outDir),"application-termination-denial-evidence"));
  const save=(name:string,value:unknown)=>writeFileSync(join(directory,name),JSON.stringify(value,null,2)+"\n",{mode:0o600});
  const journal=(value:unknown)=>appendFileSync(join(directory,"actions.jsonl"),JSON.stringify(value)+"\n",{mode:0o600});
  const helper=resolve(import.meta.dir,"../../../arc/build/agent-native"),source=resolve(import.meta.dir,"../../../arc/tools/agent-native.swift");
  assert("termination native helper is already built and current",existsSync(helper)&&statSync(helper).mtimeMs>=statSync(source).mtimeMs);
  const helperStat=statSync(helper);
  const helperCurrent=()=>{const now=statSync(helper);if(now.dev!==helperStat.dev||now.ino!==helperStat.ino||now.mtimeMs!==helperStat.mtimeMs||now.mtimeMs<statSync(source).mtimeMs)throw new Error("native helper changed/stale; auto-build forbidden");};
  const command=(args:string[])=>{
    const result=Bun.spawnSync(args,{stdout:"pipe",stderr:"pipe",timeout:3000});
    if(!result.success)throw new Error(`process observation failed: ${args[0]}`);
    const value=result.stdout.toString();if(value.length>2*1024*1024)throw new Error("process observation exceeds bound");return value;
  };
  const processTable=()=>command(["/bin/ps","-ww","-axo","pid=,ppid=,comm="]).trim().split("\n").map(line=>{
    const match=/^\s*(\d+)\s+(\d+)\s+(.+)$/.exec(line);if(!match)throw new Error("invalid process observation");
    return {pid:Number(match[1]),ppid:Number(match[2]),executable:match[3]!};
  });
  // A process that has exited but is not yet reaped by its parent stays in the process table as "<defunct>"
  // (no longer the owned executable). It has terminated; the owned-identity checks apply to live entries.
  const exited=(p:{executable:string})=>p.executable==="<defunct>";
  const identity=(p:ReturnType<typeof processTable>[number]):ProcessIdentity=>{
    const start=command(["/bin/ps","-p",String(p.pid),"-o","lstart="]).trim();
    if(!start)throw new Error("process start identity unavailable");return {...p,start};
  };
  const startTime=Date.now(),launch=c.launch();
  assert("termination launch reaches owned readiness",await c.ready(launch,"termination launch"));
  const pid=launch.pid();assert("termination launch has actual browser PID",pid!==undefined);
  const browser=identity(processTable().find(p=>p.pid===pid)??(()=>{throw new Error("browser absent");})());
  assert("termination browser has exact executable",browser.executable===executable);
  const owner=()=>{
    const p=processTable().find(p=>p.pid===pid);
    if(!p||p.executable!==browser.executable||identity(p).start!==browser.start||launch.pid()!==pid||verifyAgentEndpoint(c.slug,c.port)!==pid)throw new Error("termination PID/executable/start/endpoint ownership changed");
  };
  owner();profileIdentity=inspectProfile("launched",true);
  const endpoint=async()=>{
    owner();const response=await fetch(`http://127.0.0.1:${c.port}/json/version`,{signal:AbortSignal.timeout(3000)});
    if(!response.ok)throw new Error("owned endpoint version unavailable");const value=await response.json() as Endpoint;owner();
    if(typeof value.webSocketDebuggerUrl!=="string")throw new Error("owned endpoint lacks browser socket");
    const url=new URL(value.webSocketDebuggerUrl);
    if(url.protocol!=="ws:"||url.hostname!=="127.0.0.1"||url.port!==String(c.port)||!url.pathname.startsWith("/devtools/browser/"))throw new Error("owned endpoint socket identity mismatch");return value.webSocketDebuggerUrl;
  };
  const socket=await endpoint();
  const state=()=>{
    owner();helperCurrent();const s=c.native() as Snapshot;owner();
    if(s.pid!==pid||s.bundle!==bundle||s.truncated!==false||s.webContentExcluded!==true)throw new Error("termination native snapshot ownership/scope gap");return s;
  };
  const unique=(s:Snapshot,match:(n:Node)=>boolean,what:string)=>{const nodes=s.elements.filter(match);if(nodes.length!==1)throw new Error(`${what}: expected one observed node, got ${nodes.length}`);return nodes[0]!;};
  const under=(n:Node,parent:Node)=>n.path.startsWith(parent.path+".");
  const window=(s:Snapshot)=>{
    const values=s.elements.filter(n=>n.role==="AXWindow");
    if(values.length!==1||!values[0]!.identifier.startsWith("bigBrowserWindow-"))throw new Error("termination requires sole owned full browser window");return values[0]!;
  };
  let sequence=0,quitAttempts=0;
  const dispatch=(node:Node,kind:"press"|"perform"="press",value?:string)=>{
    owner();helperCurrent();
    if(!node.enabled||!node.actions.includes(kind==="press"?"AXPress":value!)||typeof node.fingerprint!=="string"||!node.fingerprint)throw new Error("native control has no enabled advertised exact action/fingerprint");
    const intent={sequence:++sequence,pid,identifier:node.identifier,label:node.label,path:node.path,fingerprint:node.fingerprint,kind,value,attempts:1,retries:false};
    journal({event:"intent",...intent});
    try{
      // The sidebar tab press goes through c.perform for its DevTools fallback (native.ts isTabFallback); that
      // receipt has no AX pid/bundle fields, and owner() re-verifies the endpoint PID. Menu actions only work in
      // the active app, so everything else brings the agent to the front first, as c.action does.
      const tabPress=kind==="perform"&&value!.startsWith("Name:press");
      if(!tabPress)c.front();
      // Exported direct action has a receipt only; it does not snapshot a dying
      // application after OK. The helper rechecks PID and fresh fingerprint.
      const receipt=(tabPress?c.perform(node,value!).receipt:nativeState(c.slug,c.port,{path:node.path,fingerprint:node.fingerprint,pid,kind,...(value===undefined?{}:{value})})) as Record<string,unknown>|undefined;
      journal({event:"receipt",...intent,receipt});
      if(tabPress&&isTabFallback(value,receipt)){owner();return;}
      if(!receipt||receipt.pid!==pid||receipt.bundle!==bundle||receipt.action!==kind||receipt.dispatched!==true)throw new Error("native dispatch receipt mismatch");
    }catch(error){journal({event:"uncertain-no-retry",...intent,error:String(error)});throw error;}
  };
  // Only observations repeat; action exceptions, malformed states and ownership
  // changes propagate. No generic action retry or swallowed mutation error.
  const poll=async<T>(read:()=>T|undefined,ms=8000)=>{const end=Date.now()+ms;while(Date.now()<end){owner();const v=read();owner();if(v!==undefined)return v;await c.sleep(100);}return;};
  let s=state();
  if(!s.elements.some(n=>n.role==="AXWindow")){
    dispatch(unique(s,n=>n.identifier==="newWindow"&&n.role==="AXMenuItem","sole New Window setup"));
    assert("termination sole browser window appears",await poll(()=>{const v=state();if(!v.elements.some(n=>n.role==="AXWindow"))return;window(v);return true;},5000));
  }
  const target=c.open(c.base+"/a");owner();
  assert("termination exact fixture A loads",await c.titleIs(target,c.EXPECT.aTitle)&&c.evaluate(target,"location.href")===c.base+"/a");owner();
  s=state();let win=window(s);
  const tab=unique(s,n=>under(n,win)&&n.role==="AXButton"&&n.label===c.EXPECT.aTitle,"owned fixture A tab");
  const tabAction=tab.actions.find(a=>a==="AXPress"||a.startsWith("Name:press"));assert("fixture A has advertised native selection",tabAction!==undefined);dispatch(tab,"perform",tabAction!);
  assert("termination native address selects fixture A",await poll(()=>{const v=state(),w=window(v);return v.elements.filter(n=>under(n,w)&&n.identifier==="contentPanelTextField"&&n.value===c.base+"/a").length===1?true:undefined;},5000));
  s=state();win=window(s);if(win.actions.includes("AXRaise"))dispatch(win,"perform","AXRaise");
  const client=await c.client();owner();
  const events:CDPEvent[]=[];
  const unsubscribe=client.onEvent(event=>{
    if(!["Page.javascriptDialogOpening","Page.javascriptDialogClosed","Runtime.bindingCalled","ArcMod.disconnected"].includes(event.method))return;
    if(event.method==="Runtime.bindingCalled"&&event.params.name!=="__arcTerminationEvent")return;
    events.push(event);appendFileSync(join(directory,"page-events.jsonl"),JSON.stringify(event)+"\n",{mode:0o600});
  });
  try{
    const info=(await client.request("Target.getTargetInfo",{targetId:target})).targetInfo as Record<string,unknown>;
    assert("termination pinned target is exact fixture page",info?.targetId===target&&info.type==="page"&&info.url===c.base+"/a");
    const attached=await client.request("Target.attachToTarget",{targetId:target,flatten:true}),session=attached.sessionId;
    assert("termination has explicit nonempty target session",typeof session==="string"&&session.length>0);
    const request=(method:string,params:Record<string,unknown>={})=>client.request(method,params,session as string);
    const contexts:CDPEvent[]=[];
    const stopContexts=client.onEvent(e=>{if(e.method==="Runtime.executionContextCreated"&&e.sessionId===session)contexts.push(e);});
    await request("Page.enable");await request("Runtime.enable");
    const tree=(await request("Page.getFrameTree")).frameTree as {frame?:{id?:string;url?:string}};
    const main=contexts.map(e=>e.params.context as {id?:number;auxData?:{isDefault?:boolean;frameId?:string}}).filter(v=>v?.auxData?.isDefault===true&&v.auxData.frameId===tree?.frame?.id);
    stopContexts();assert("termination owns unique main document context",tree?.frame?.url===c.base+"/a"&&main.length===1&&Number.isInteger(main[0]!.id));
    const contextId=main[0]!.id!;
    await request("Runtime.addBinding",{name:"__arcTerminationEvent",executionContextId:contextId});
    const evaluate=async<T>(expression:string):Promise<T>=>{
      owner();const result=await request("Runtime.evaluate",{expression,contextId,returnByValue:true,awaitPromise:true});owner();
      if(result.exceptionDetails!==undefined)throw new Error("owned page evaluation failed: "+JSON.stringify(result.exceptionDetails));
      return (result.result as {value:T}).value;
    };
    const readProbe=()=>evaluate<Probe>(`(()=>{const p=window.__terminationProbe;return {...p,live:typeof onbeforeunload==='function',sticky:navigator.userActivation.hasBeenActive,url:location.href,title:document.title,responsive:21*2}})()`);
    const before=await readProbe();assert("termination document begins with nonce and no installed handler",typeof before.nonce==="string"&&before.nonce.length>0&&before.installs===0&&before.handlerCount===0&&!before.live&&before.url===c.base+"/a"&&before.title===c.EXPECT.aTitle);
    const point=await evaluate<{x:number;y:number}>(`(()=>{const r=document.querySelector('#termination-install').getBoundingClientRect();return {x:r.x+r.width/2,y:r.y+r.height/2}})()`);
    assert("termination install button has finite positive coordinates",Number.isFinite(point.x)&&Number.isFinite(point.y)&&point.x>0&&point.y>0);
    await request("Emulation.setFocusEmulationEnabled",{enabled:true});
    try{
      await evaluate("new Promise(resolve=>requestAnimationFrame(()=>requestAnimationFrame(resolve)))");
      journal({event:"trusted-click-intent",target,session,point,attempts:1,retries:false});
      try{
        await request("Input.dispatchMouseEvent",{type:"mousePressed",x:point.x,y:point.y,button:"left",clickCount:1});
        await request("Input.dispatchMouseEvent",{type:"mouseReleased",x:point.x,y:point.y,button:"left",clickCount:1});
        journal({event:"trusted-click-returned",target,session});
      }catch(error){journal({event:"trusted-click-uncertain-no-retry",error:String(error)});throw error;}
      await evaluate("new Promise(resolve=>requestAnimationFrame(resolve))");
    }finally{await request("Emulation.setFocusEmulationEnabled",{enabled:false});}
    const installed=await readProbe();assert("trusted single click installs one live sticky-activated handler",installed.nonce===before.nonce&&installed.installs===1&&installed.handlerCount===0&&installed.trusted===true&&installed.sticky===true&&installed.live===true);
    save("ownership.json",{pid,browser,profile:profileCanonical,profileIdentity,app:c.app,bundle,port:c.port,socket,target,session,contextId,documentNonce:before.nonce,helper:{device:helperStat.dev,inode:helperStat.ino,mtimeMs:helperStat.mtimeMs},bindingObservation:"Fixture sidecar only; not a recorded engine callback or block-completion observation.",scope:"Public native Quit, genuine page dialog events and observed process absence; no OS exit-code/block-completion claim."});
    const rawPath=join(c.outDir,"launch-1","record.jsonl"),rawStat=statSync(rawPath),phases:QuitPhase[]=[];
    let completePrefix=Buffer.alloc(0);
    const rawSnapshot=()=>{
      const st=statSync(rawPath);if(st.dev!==rawStat.dev||st.ino!==rawStat.ino||st.size>64*1024*1024)throw new Error("termination recorder identity/size changed");
      const bytes=readFileSync(rawPath),after=statSync(rawPath);
      if(after.dev!==st.dev||after.ino!==st.ino||after.size<bytes.length)throw new Error("termination recorder changed while reading");
      assertTerminationRawPrefix(bytes,completePrefix);
      if(bytes.length>0&&bytes.at(-1)===10){completePrefix=Buffer.from(bytes);return bytes;}
      return;
    };
    const cursor=async()=>{const bytes=await poll(rawSnapshot,3000);assert("termination pre-action recorder has complete byte cursor",bytes!==undefined);return bytes!.length;};
    const recorded=new Map<number,ProcessIdentity>();
    const descendants=(phase:number)=>{
      owner();const table=processTable().filter(p=>!exited(p)),ids=new Set<number>([pid!]);
      for(let i=0;i<table.length;i++){const found=table.filter(p=>ids.has(p.ppid)&&!ids.has(p.pid));if(!found.length)break;for(const p of found)ids.add(p.pid);}
      const helpers=table.filter(p=>p.pid!==pid&&ids.has(p.pid));
      assert(`Quit ${phase} has actual nonempty descendant snapshot`,helpers.length>0);
      const values=helpers.map(p=>{
        if(!p.executable.startsWith(expectedApp+"/Contents/"))throw new Error("browser descendant has unexpected executable");
        const value=identity(p),prior=recorded.get(value.pid);
        if(prior&&(prior.executable!==value.executable||prior.start!==value.start))throw new Error("recorded helper PID reused before Quit");
        recorded.set(value.pid,value);return value;
      });
      save(`quit-${phase}-descendants.json`,{browser,helpers:values});owner();
    };
    const freshQuit=async(phase:number)=>{
      inspectProfile(`before Quit ${phase}`,true);
      let snap=state();window(snap);
      assert(`Quit ${phase} starts without any sheet`,!snap.elements.some(n=>n.role==="AXSheet"));
      let item=unique(snap,n=>n.role==="AXMenuItem"&&n.identifier==="_NS:988"&&n.label==="Quit Arc","exact Quit Arc");
      if(!item.enabled){
        const parent=unique(snap,n=>n.role==="AXMenuBarItem"&&under(item,n),"observed Quit parent menu");dispatch(parent);
        snap=state();window(snap);item=unique(snap,n=>n.role==="AXMenuItem"&&n.identifier==="_NS:988"&&n.label==="Quit Arc","fresh enabled Quit Arc");
      }
      descendants(phase);const start=await cursor();
      // Reacquire after all pre-Quit observation work. No uncertain Quit retry.
      snap=state();window(snap);item=unique(snap,n=>n.role==="AXMenuItem"&&n.identifier==="_NS:988"&&n.label==="Quit Arc","final fresh Quit Arc");
      if(++quitAttempts!==phase)throw new Error("Quit action count changed");dispatch(item);return start;
    };
    const pageEvents=()=>{
      for(const event of events){
        if(event.method.startsWith("Page.javascriptDialog")&&event.sessionId!==session)throw new Error("dialog event belongs to a different session");
        if(event.method==="Runtime.bindingCalled"&&(event.sessionId!==session||event.params.executionContextId!==contextId))throw new Error("beforeunload binding belongs to a different document");
      }
      const openings=events.filter(e=>e.method==="Page.javascriptDialogOpening"),closed=events.filter(e=>e.method==="Page.javascriptDialogClosed");
      if(openings.some(e=>e.params.type!=="beforeunload"||e.params.url!==c.base+"/a"))throw new Error("unexpected genuine JavaScript prompt");
      const bindings=events.filter(e=>e.method==="Runtime.bindingCalled").map(e=>JSON.parse(String(e.params.payload)) as Record<string,unknown>);
      if(bindings.some((v,i)=>v.nonce!==before.nonce||v.url!==c.base+"/a"||v.handlerCount!==i+1||v.installs!==1||v.trusted!==true||v.sticky!==true))throw new Error("beforeunload event document/count/activation mismatch");
      if(openings.length>2||closed.length>2||bindings.length>2)throw new Error("extra prompt/handler invocation");
      return {openings,closed,bindings};
    };
    const prompt=(phase:number)=>{
      const snap=state(),w=window(snap),sheets=snap.elements.filter(n=>n.role==="AXSheet");
      if(!sheets.length)return;
      if(sheets.length!==1||sheets[0]!.identifier!=="_NS:87"||!under(sheets[0]!,w))throw new Error("unexpected application Quit confirmation or foreign sheet; never dismiss");
      const sheet=sheets[0]!;
      unique(snap,n=>under(n,sheet)&&n.role==="AXStaticText"&&n.value==="Leave site?","exact Leave site title");
      unique(snap,n=>under(n,sheet)&&n.role==="AXStaticText"&&n.value==="Changes you made may not be saved.","exact Leave site body");
      const e=pageEvents();if(e.openings.length!==phase||e.bindings.length!==phase)return;
      if(e.closed.length!==phase-1)throw new Error("prompt phase has unexpected close event");
      return snap;
    };
    const start1=await freshQuit(1);
    assert("first native Quit produces genuine first beforeunload and owned prompt",await poll(()=>prompt(1))!==undefined);
    // Fresh snapshot/action selection, never reuse a handle from the polling UI.
    const cancelState=prompt(1);assert("first owned prompt remains before Cancel",cancelState!==undefined);
    const cancelSheet=unique(cancelState!,n=>n.role==="AXSheet"&&n.identifier==="_NS:87","first sheet");
    dispatch(unique(cancelState!,n=>under(n,cancelSheet)&&n.role==="AXButton"&&n.identifier==="action-button--999"&&n.label==="Cancel","exact Cancel"));
    assert("Cancel emits first false close and removes prompt",await poll(()=>{
      const e=pageEvents();if(e.closed.length!==1)return;
      if(e.closed[0]!.params.result!==false)throw new Error("first beforeunload close was not Cancel");
      return !state().elements.some(n=>n.role==="AXSheet")?true:undefined;
    })===true);
    const survived=await readProbe();
    assert("Cancel preserves same responsive nonce URL title live handler and count one",survived.nonce===before.nonce&&survived.url===c.base+"/a"&&survived.title===c.EXPECT.aTitle&&survived.installs===1&&survived.handlerCount===1&&survived.trusted===true&&survived.sticky===true&&survived.live===true&&survived.responsive===42);
    assert("Cancel preserves exact browser endpoint socket",await endpoint()===socket);owner();
    const end1=await cursor();phases.push({start:start1,end:end1});
    save("cancel-survival.json",{browser,endpoint:socket,probe:survived,events:pageEvents()});
    const start2=await freshQuit(2);
    assert("second native Quit produces second genuine handler and same owned prompt",await poll(()=>prompt(2))!==undefined);
    const leaveState=prompt(2);assert("second owned prompt remains before OK",leaveState!==undefined);
    const leaveSheet=unique(leaveState!,n=>n.role==="AXSheet"&&n.identifier==="_NS:87","second sheet");
    const leave=unique(leaveState!,n=>under(n,leaveSheet)&&n.role==="AXButton"&&n.identifier==="action-button--998"&&n.label==="OK","exact affirmative OK");
    const acceptedAt=Date.now();dispatch(leave); // Final native action: receipt only, no poststate.
    const identities=[browser,...recorded.values()],deadline=acceptedAt+15000;
    let allGone=false,firstObservedGoneAt:number|undefined;
    while(Date.now()<=deadline){
      pageEvents();const table=processTable().filter(p=>!exited(p));
      for(const p of table.filter(p=>identities.some(i=>i.pid===p.pid))){
        const original=identities.find(i=>i.pid===p.pid)!;
        // While a process is exiting, ps can no longer resolve its executable path and prints the kernel's short
        // process name (at most 16 characters, possibly parenthesized). That entry is still present, so it keeps
        // allGone false, but its path can no longer be compared; anything else is an identity change.
        if(p.executable!==original.executable){
          const short=p.executable.replace(/^\((.*)\)$/,"$1");
          journal({event:"exiting-process-name",pid:p.pid,observed:p.executable,expected:original.executable});
          if(!short||short.length>16||!basename(original.executable).startsWith(short))throw new Error(`owned executable changed during termination observation: ${p.pid} ${p.executable}`);
          continue;
        }
        observeTerminationIdentity(p,original,()=>{
          const result=Bun.spawnSync(["/bin/ps","-p",String(p.pid),"-o","lstart="],{stdout:"pipe",stderr:"pipe",timeout:3000});
          return {success:result.success,exitCode:result.exitCode,signalCode:result.signalCode,stdout:result.stdout.toString(),stderr:result.stderr.toString()};
        },()=>!processTable().some(current=>current.pid===p.pid));
      }
      allGone=identities.every(i=>!table.some(p=>p.pid===i.pid))&&!table.some(p=>p.executable.startsWith(expectedApp+"/Contents/"));
      if(allGone){firstObservedGoneAt=Date.now();break;}await c.sleep(100);
    }
    assert("affirmative native Quit removes browser and all recorded helpers within 15 seconds",allGone&&firstObservedGoneAt!==undefined&&firstObservedGoneAt<=deadline);
    const listener=Bun.spawnSync(["/usr/sbin/lsof","-nP",`-iTCP:${c.port}`,"-sTCP:LISTEN","-Fp"],{stdout:"pipe",stderr:"pipe",timeout:3000});
    assert("affirmative native Quit leaves no listener on the owned endpoint",listener.exitCode===1&&listener.stdout.length===0&&listener.stderr.length===0);
    // Drain already-delivered socket messages without demanding an unproved
    // final Page event before disconnection. No post-exit ownership/action call.
    await c.sleep(200);
    const finalEvents=pageEvents();
    assert("second closed result, if delivered before disconnect, is affirmative",finalEvents.closed.length===1||(finalEvents.closed.length===2&&finalEvents.closed[1]!.params.result===true));
    // Bounded Apple IPS observation, modeled on try-engine.sh:38 but restricted
    // to actual recorded PID+exact executable. Do not print/copy crash payloads.
    // Report publication can lag this observation; no OS exit-status inference.
    const crashObservation:{directory:string;freshCandidates:number;matching:string[]}[]=[];
    for(const path of [join(process.env.HOME!,"Library/Logs/DiagnosticReports"),"/Library/Logs/DiagnosticReports"]){
      const d=noFollow(path);if(!d)continue;
      if(d.isSymbolicLink()||!d.isDirectory())throw new Error("crash report observation directory unsafe");
      const names=readdirSync(path).filter(n=>/^(?:Arc|ArcModAgent-(?:trace156|ours156|rel156))(?:[ _-].*)?\.ips$/.test(n));
      if(names.length>2048)throw new Error("crash directory candidate count exceeds bound");
      const fresh=names.filter(n=>{const st=noFollow(join(path,n));if(!st||st.isSymbolicLink()||!st.isFile())throw new Error("crash candidate is not a regular file");return st.mtimeMs>=startTime-2000;});
      if(fresh.length>16)throw new Error("fresh crash candidate count exceeds bound");
      const matching:string[]=[];
      for(const name of fresh){
        const report=join(path,name),st=lstatSync(report);if(st.size>8*1024*1024)throw new Error("fresh crash report exceeds read bound");
        const text=readFileSync(report,"utf8"),newline=text.indexOf("\n");
        if(newline<0)throw new Error("fresh crash report header unavailable");
        const body=JSON.parse(text.slice(newline+1)) as {pid?:number;procPath?:string};
        if(identities.some(i=>body.pid===i.pid&&body.procPath===i.executable))matching.push(name);
      }
      crashObservation.push({directory:path,freshCandidates:fresh.length,matching});
    }
    save("crash-observation.json",{observedAt:new Date().toISOString(),crashObservation,scope:"Bounded existing Apple IPS files only; delayed reports and OS exit status unobserved."});
    assert("no matching fresh bounded Apple crash report at observation",crashObservation.every(v=>v.matching.length===0));
    const bytes=rawSnapshot();assert("final recorder has complete unchanged-file transcript",bytes!==undefined);
    phases.push({start:start2,end:bytes!.length});
    const gate=validateTerminationPhases(bytes!,phases);
    writeFileSync(join(directory,"proof.full.raw.jsonl"),bytes!,{mode:0o600});save("raw-gate.json",gate);
    assert("both actual Quit phases have exact genuine public call and paired numeric return",gate.phases.length===2);
    save("termination.json",{browser,recordedHelpers:[...recorded.values()],target,session,documentNonce:before.nonce,quitAttempts,acceptedAt,observedGoneAt:firstObservedGoneAt,proofRecordedAt:Date.now(),allGone,events:finalEvents,scenarioBrowserCloseCalls:0,scenarioTerminationSignals:0,osExitStatus:"unobserved",blockCompletion:"unobserved",runtimeAcceptance:"Only this recorded execution can establish the guarded two-Quit flow; source was written offline."});
    // No c.quit on the successful path: its Browser.close would contaminate
    // termination proof. Standard harness finally cleans a still-alive failure.
  }finally{unsubscribe();}
}
