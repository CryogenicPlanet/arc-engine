import type { ScenarioContext } from "../launch-slice/run";
import { verifyAgentEndpoint, type CDPResult } from "../../../arc/tools/agent-cdp";
import { existsSync, mkdirSync, readFileSync, readdirSync, renameSync, writeFileSync } from "node:fs";
import { createHash } from "node:crypto";
import { join } from "node:path";
import { snapshotSessionStorage } from "./sessions-tab-restart-storage";

// Ground truth: SHA256-frozen batch6 v6 public API probe, both engines pass its
// supplemental two-launch contract. Canonical acceptance still requires a fresh
// standard-recorder original run; no supplemental events are copied into it.
const PREFIX = "/arc-sessions-restart-7f3a/";
const IMAGE = "SessionsTabRestartDriver.dylib";
const PROVENANCE = {
  manifest:"results/sessions/batch6/fixtures.sha256.json",
  source:"/tmp/arccore-sessions-tab-restart-v6.mm",sha256:"26f3fcf448a9271400b67d00d43c9e42095caea6605945cb51a929e469fcd617",
  runner:"/tmp/arccore-sessions-tab-restart-v6.ts",runnerSHA256:"c6504e3a51dede8d35837c5fdd9dbeb70d0677eef07316cfe401ee98d3381fc7",
  dylib:"/tmp/arccore-sessions-tab-restart-v6.dylib",dylibSHA256:"27c95a13797463327a306379d26dbdaaf7b962d6c9046dcb989044850131ccf1",
  storage:"/tmp/arccore-sessions-storage-snapshot.ts",storageSHA256:"c07dbd0feb9a429ac39994aa936020ac0aa6c5dcf2457fd511abd0ff903824a2",
  observations:["results/sessions/tab-restart-original-run1","results/sessions/tab-restart-ours-run1"],
  scope:"Two individually closed tabs across a same-profile/new-PID restart; excludes full-window/open-session startup parity. Standard harness Browser.close shutdown is required and any SIGTERM fallback fails acceptance. Driver-origin calls remain in raw transcripts; default boundary diff excludes them, so exact driver-call/return gates and fixed sidecar assertions are independently required.",
};
export const fixture = (request:Request) => {
  const path=new URL(request.url).pathname;
  if(!path.startsWith(PREFIX))return;
  const page=path.slice(PREFIX.length);
  if(page==="sentinel/1"||page==="sentinel/2")return new Response("<!doctype html><title>Sentinel</title><h1>Sentinel</h1>",{headers:{"content-type":"text/html; charset=utf-8","cache-control":"no-store"}});
  const match=/^(tab1|tab2)\/(A|B|C)$/.exec(page);
  if(!match)return new Response("missing",{status:404});
  return new Response(`<!doctype html><title>Sessions ${match[1]} ${match[2]}</title><h1>${match[1]} history ${match[2]}</h1>`,{headers:{"content-type":"text/html; charset=utf-8","cache-control":"no-store"}});
};
type Event={type:string;main:boolean;insideAction:boolean;order:number;id?:string;url?:string};
type Tab={url:string;sourceID:string;restorationID:string;callbackID:string|null;closes:Event[];restores:Event[];
  oldInitialized:boolean;restoredInitialized:boolean;restoredURL:string|null;restoredLiveID:string|null;canRestore:boolean;closeRequested:boolean;restoreRequested:boolean};
type State={records:Tab[];events:Event[];contextKey:string;browserInitialized:boolean;sentinelInitialized:boolean;sentinelURL:string;simultaneousRestoredDistinct:boolean;recorderAliases:boolean;error:string|null};
type Response={id:string;operation:string;pid:number;nonce:string;main:boolean;ok?:boolean;error?:string;state:State};
type History={currentIndex:number;entries:{id:number;url:string;title:string}[]};
type Row={seq:number;ev:string;from?:string;dir?:string;cls?:string;sel?:string;self?:unknown;args?:unknown[];ret?:unknown;depth?:number;thread?:string;engine_classes?:number};
type Closed={url:string;sourceID:string;restorationID:string;contextKey:string};
const positiveID=(value:unknown):value is string=>typeof value==="string"&&/^[1-9][0-9]*$/.test(value)&&BigInt(value)<=2147483647n;
const same=(a:unknown,b:unknown)=>JSON.stringify(a)===JSON.stringify(b);

// Full unmodified JSONL is authoritative. Never drop the direct fixture calls
// merely because the default v2 comparator excludes their caller image.
function transcript(path:string,launch:number){
  const text=readFileSync(path,"utf8");
  if(!text.endsWith("\n"))throw new Error("incomplete recorder final line");
  const rows=text.trimEnd().split("\n").map((line,index)=>{try{return JSON.parse(line) as Row}catch{throw new Error(`malformed recorder line ${index+1}`)}});
  if(!rows.some(row=>row.ev==="hooked"&&Number(row.engine_classes)>0))throw new Error("recorder did not hook engine");
  const boundary=rows.filter(row=>row.ev==="call"||row.ev==="ret"),calls=boundary.filter(row=>row.ev==="call");
  if(!calls.length||!boundary.every(row=>typeof row.from==="string"&&row.from.trim().length>0&&row.from!=="?"))throw new Error("caller-aware recorder required; absent/mixed/unknown attribution");
  const sequences=new Set<number>();for(const row of rows){if(!Number.isSafeInteger(row.seq)||row.seq<0||sequences.has(row.seq))throw new Error("invalid/duplicate recorder sequence");sequences.add(row.seq);}
  const own=calls.filter(row=>row.from===IMAGE&&row.dir==="->engine");
  const fixture=own.filter(row=>row.cls===(launch===1?"ArcWebContents":"ArcBrowser")&&row.sel===(launch===1?"close":"restoreWebContentsWithSessionId:"));
  if(fixture.length!==2||fixture.some(row=>row.thread!=="main"))throw new Error("exact two main-thread driver close/restore calls required");
  // Pair each public driver call with exactly one later same-frame return. Do
  // not normalize labels/IDs or assume adjacent rows (callbacks may be nested).
  const paired=new Map<Row,Row>();
  for(const call of own){const next=calls.find(row=>row.seq>call.seq&&row.thread===call.thread&&row.depth===call.depth);
    const returns=boundary.filter(row=>row.ev==="ret"&&row.seq>call.seq&&(!next||row.seq<next.seq)&&row.from===call.from&&row.dir===call.dir&&row.cls===call.cls&&row.sel===call.sel&&same(row.self,call.self)&&row.depth===call.depth&&row.thread===call.thread);
    if(returns.length!==1)throw new Error(`driver call missing/ambiguous return ${call.cls}.${call.sel}`);paired.set(call,returns[0]!);
  }
  for(const [cls,sel,min,exact] of (launch===1?
    [["ArcWebContents","sessionId",2,false],["ArcBrowser","context",1,false],["ArcBrowserContext","id",1,false],["ArcBrowserContextID","directoryBasename",1,false],["ArcSessionID","identifier",4,false],["ArcBrowser","canRestoreWebContentsWithSessionId:",2,false]]:
    [["ArcSessionID","initWithID:",4,true],["ArcSessionID","identifier",4,false],["ArcBrowser","canRestoreWebContentsWithSessionId:",2,false]]) as [string,string,number,boolean][]){
    const count=own.filter(row=>row.cls===cls&&row.sel===sel).length;if(exact?count!==min:count<min)throw new Error(`nonvacuous public API gate failed ${cls}.${sel}: ${count}`);
  }
  const callbacks=calls.filter(row=>row.dir==="engine->app"&&row.sel===(launch===1?"webContents:didCloseWithSessionID:":"browser:didRestoreWebContents:withSessionId:")&&fixture.some(call=>same(call.self,row.args?.[0])));
  if(callbacks.length!==2||callbacks.some(row=>row.from!=="engine"||row.thread!=="main"))throw new Error("exact genuine engine callback for each direct fixture request required");
  const identifier=(label:unknown)=>{
    if(typeof label!=="string"||!/^<ArcSessionID#[0-9]+>$/.test(label))throw new Error("historical ID must retain raw ArcSessionID label");
    const values=own.filter(row=>row.cls==="ArcSessionID"&&row.sel==="identifier"&&same(row.self,label)).map(row=>paired.get(row)?.ret);
    if(!values.length||!values.every(value=>Number.isSafeInteger(value)&&Number(value)>0&&Number(value)<=2147483647&&value===values[0]))throw new Error("public identifier getter required for raw historical ID label");
    return values[0];
  };
  for(const callback of callbacks){const next=calls.find(row=>row.seq>callback.seq&&row.thread===callback.thread&&row.depth===callback.depth);
    const returns=boundary.filter(row=>row.ev==="ret"&&row.seq>callback.seq&&(!next||row.seq<next.seq)&&row.from===callback.from&&row.dir===callback.dir&&row.cls===callback.cls&&row.sel===callback.sel&&same(row.self,callback.self)&&row.depth===callback.depth&&row.thread===callback.thread);
    if(returns.length!==1)throw new Error("genuine callback missing/ambiguous recorder return");
  }
  for(const call of fixture){const matches=callbacks.filter(row=>same(row.args?.[0],call.self));
    // Both restored tabs use one browser; restore-ID arguments disambiguate it.
    const exact=launch===1?matches:matches.filter(row=>identifier(row.args?.[2])===identifier(call.args?.[0]));
    if(exact.length!==1)throw new Error("request/callback receiver or public historical identifier mismatch");
  }
  return {path,sha256:createHash("sha256").update(text).digest("hex"),launch,callCount:calls.length,ownDriverCalls:own.length,requiredRequestCalls:fixture,requiredCallbacks:callbacks,rawLabelsPreserved:true};
}

export default async function scenario(c:ScenarioContext){
  const assert=(name:string,value:unknown,detail="")=>{c.assert(name,value,detail);if(!value)throw new Error(name+(detail?": "+detail:""));};
  assert("restart scenario requires caller-aware standard recording",c.recorded&&!c.traced);
  assert("restart scenario requires exact disposable engine agent",["trace156","ours156"].includes(c.slug));
  // Parent recorder/diff wrapper must reset only this disposable profile. Refuse
  // stale storage so unrelated historic tabs cannot satisfy the barrier.
  assert("restart scenario begins with empty disposable browser storage",["User Data","Agent Chromium Data"].every(name=>!existsSync(join(c.data,name))||readdirSync(join(c.data,name)).length===0));
  const directory=join(c.outDir,"sessions-tab-restart-driver");mkdirSync(directory,{recursive:true,mode:0o700});
  writeFileSync(join(directory,"provenance.json"),JSON.stringify(PROVENANCE,null,2));
  const dylib=join(directory,IMAGE),compile=Bun.spawnSync(["xcrun","clang","-dynamiclib","-fobjc-arc","-Wall","-Wextra","-Werror","-framework","Foundation","-framework","AppKit",join(import.meta.dir,"sessions-tab-restart-driver.m"),"-o",dylib],{stdout:"pipe",stderr:"pipe"});
  writeFileSync(join(directory,"compile.log"),compile.stdout.toString()+compile.stderr.toString());
  assert("restart driver compiles with strict warnings",compile.success,compile.stderr.toString().slice(-1500));
  const nonce=`sessions-tab-restart-${crypto.randomUUID()}`,closed:Closed[]=[],observations:Response[]=[],histories:Record<string,unknown>={},pids:number[]=[],gates:unknown[]=[];
  const urls=["tab1","tab2"].map(tab=>`${c.base}${PREFIX}${tab}/B`);
  const one=(state:State,url:string)=>{const matches=state.records.filter(row=>row.url===url);if(matches.length!==1)throw new Error("exactly one fixture record required");return matches[0]!;};
  const sentinelState=(state:State,url:string)=>state.browserInitialized===true&&state.sentinelInitialized===true&&state.sentinelURL===url&&state.recorderAliases===true&&state.error===null;
  for(const number of [1,2]){
    const phase=join(directory,`launch-${number}`);mkdirSync(phase,{recursive:true,mode:0o700});const request=join(phase,"request.json");
    const launch=c.launch({libraries:[dylib],environment:{ARC_SCENARIO_RESTART_AGENT:c.slug,ARC_SCENARIO_RESTART_REQUEST:request,ARC_SCENARIO_RESTART_NONCE:nonce,ARC_SCENARIO_RESTART_BASE:c.base}});
    try{
      if(!await c.ready(launch,`restart launch ${number}`))return;
      const pid=launch.pid();assert(`restart launch ${number} exposes owned PID`,pid!==undefined);pids.push(pid!);
      const owner=()=>{if(launch.pid()!==pid||verifyAgentEndpoint(c.slug,c.port)!==pid)throw new Error("restart PID/endpoint ownership changed");};
      const ready=await c.waitFor(()=>{const r=JSON.parse(readFileSync(request+".ready.json","utf8")) as {pid:number;nonce:string;agent:string;bundle:string;installed:boolean;recorderAliases:boolean};return r.pid===pid&&r.nonce===nonce&&r.agent===c.slug&&r.bundle===`company.thebrowser.Browser.mod.agent.${c.slug}`&&r.installed===true&&r.recorderAliases===true?r:undefined;},10000);
      assert(`restart launch ${number} driver has genuine recorder aliases`,ready!==undefined);
      if(number===2)assert("restart uses a new actual app PID and unchanged disposable profile",pids.length===2&&pids[0]!==pids[1],c.data);
      let sequence=0;
      const action=async(operation:string,values:Record<string,unknown>={}):Promise<State>=>{
        owner();const id=`${nonce}-${number}-${++sequence}`;writeFileSync(request+".tmp",JSON.stringify({id,operation,pid,nonce,...values}),{mode:0o600});renameSync(request+".tmp",request);
        const response=await c.waitFor(()=>{const r=JSON.parse(readFileSync(request+".response.json","utf8")) as Response;return r.id===id&&r.operation===operation&&r.pid===pid&&r.nonce===nonce?r:undefined;},10000);
        if(!response)throw new Error(`${operation} timeout; mutating request will not be replayed`);owner();observations.push(response);writeFileSync(join(directory,"observations.json"),JSON.stringify(observations,null,2));
        if(response.ok!==true||response.main!==true||response.state.error!==null)throw new Error(`${operation}: ${response.error??response.state.error??"driver failed"}`);return response.state;
      };
      const until=async<T>(read:()=>Promise<T|undefined>,timeout=20000):Promise<T>=>{const end=Date.now()+timeout;while(Date.now()<end){const value=await read();if(value!==undefined)return value;await c.sleep(100);}throw new Error("bounded restart wait timed out");};
      const client=await c.client();
      const evaluate=async(target:string,expression:string)=>{owner();const r=await client.targetCommand(target,"Runtime.evaluate",{expression,returnByValue:true,awaitPromise:true});if(r.exceptionDetails)throw new Error("fixture evaluation exception");return (r.result as CDPResult)?.value;};
      const loaded=async(target:string,url:string)=>until(async()=>{
        owner();
        try{
          // Retry only this read-only document observation. New targets and
          // navigation can briefly lack their default document/context.
          const result=await client.targetCommand(target,"Runtime.evaluate",{expression:`document.readyState==='complete'&&location.href===${JSON.stringify(url)}`,returnByValue:true,awaitPromise:true});
          owner();
          if(result.exceptionDetails!==undefined){
            const details=result.exceptionDetails as {exception?:{description?:unknown}};
            if(typeof details.exception?.description==="string"&&/^ReferenceError: document is not defined(?:\n[\s\S]*)?$/.test(details.exception.description))return undefined;
            throw new Error("fixture document observation exception: "+JSON.stringify(result.exceptionDetails));
          }
          return (result.result as CDPResult)?.value===true?true:undefined;
        }catch(error){
          // A matching message never permits a changed agent/endpoint. Keep
          // transport, ownership, programming and nontransient page errors fatal.
          owner();
          if(error instanceof Error&&error.message.startsWith("CDP error: ")){
            let decoded:unknown;try{decoded=JSON.parse(error.message.slice("CDP error: ".length));}catch{throw error;}
            const protocol=decoded as {code?:unknown;message?:unknown}|null;
            if(protocol!==null&&typeof protocol==="object"&&protocol.code===-32000&&typeof protocol.message==="string"&&/^(?:Execution context was destroyed(?:, most likely because of a navigation)?|Cannot find (?:default execution context|context with specified id))\.?$/.test(protocol.message)){
              owner();return undefined;
            }
          }
          throw error;
        }
      });
      const create=async(url:string)=>{owner();const r=await client.request("Target.createTarget",{url,background:true});if(typeof r.targetId!=="string")throw new Error("fixture target missing");await loaded(r.targetId,url);return r.targetId;};
      const sentinelURL=`${c.base}${PREFIX}sentinel/${number}`,sentinel=await create(sentinelURL);
      const clean=(history:History)=>({index:history.currentIndex,pages:history.entries.map(e=>e.url.replace(c.base,"FIXTURE")),titles:history.entries.map(e=>e.title)});
      const expected=(tab:string)=>({index:1,pages:["A","B","C"].map(page=>`FIXTURE${PREFIX}${tab}/${page}`),titles:["A","B","C"].map(page=>`Sessions ${tab} ${page}`)});
      if(number===1){
        const targets:Record<string,string>={};
        for(const tab of ["tab1","tab2"]){const target=targets[tab]=await create(`${c.base}${PREFIX}${tab}/A`);
          for(const page of ["B","C"]){const url=`${c.base}${PREFIX}${tab}/${page}`;owner();await client.targetCommand(target,"Page.navigate",{url});await loaded(target,url);}
          const h=await client.targetCommand(target,"Page.getNavigationHistory") as unknown as History,middle=h.entries.filter(e=>e.url===`${c.base}${PREFIX}${tab}/B`);
          assert(`${tab} has one exact B history entry`,middle.length===1);await client.targetCommand(target,"Page.navigateToHistoryEntry",{entryId:middle[0]!.id});await loaded(target,`${c.base}${PREFIX}${tab}/B`);
          const before=clean(await client.targetCommand(target,"Page.getNavigationHistory") as unknown as History);assert(`${tab} has exact A B C history with B selected`,same(before,expected(tab)));histories[tab]={expected:expected(tab),before};
        }
        const captured=await action("capture",{urls,sentinelURL});assert("two source tabs have distinct generated public IDs in one live browser",captured.records.length===2&&captured.records.every(r=>positiveID(r.sourceID))&&new Set(captured.records.map(r=>r.sourceID)).size===2&&sentinelState(captured,sentinelURL));
        for(const url of urls){const state=await action("close",{url}),row=one(state,url);
          assert("individual close invokes exactly one inline main-thread callback "+url.split("/").at(-2),row.closes.length===1&&row.closes[0]?.main===true&&row.closes[0]?.insideAction===true&&positiveID(row.restorationID)&&row.restorationID!==row.sourceID&&row.oldInitialized===false&&row.canRestore===true&&sentinelState(state,sentinelURL));
          closed.push({url,sourceID:row.sourceID,restorationID:row.restorationID,contextKey:state.contextKey});
        }
        assert("two historical IDs are distinct and separate from both source IDs",new Set(closed.map(r=>r.restorationID)).size===2&&closed.every(r=>closed.every(other=>r.restorationID!==other.sourceID)));
        const beforeQuit=await action("snapshot");
        assert("genuine close callbacks retain exact capture and fixture order",same(beforeQuit.events.map(e=>e.type),["captured","closed","closed"])&&same(beforeQuit.events.filter(e=>e.type==="closed").map(e=>e.url),urls)&&beforeQuit.events.every((e,index)=>e.main===true&&e.order===index));
        assert("both closed tabs stay eligible and sentinel survives before restart",beforeQuit.records.length===2&&beforeQuit.records.every(r=>r.closes.length===1&&r.restores.length===0&&r.oldInitialized===false&&r.canRestore===true)&&sentinelState(beforeQuit,sentinelURL));
        const targetState=await client.request("Target.getTargets"),infos=targetState.targetInfos as {targetId:string}[];
        assert("both exact source targets close and first sentinel survives",Object.values(targets).every(t=>infos.every(i=>i.targetId!==t))&&infos.some(i=>i.targetId===sentinel));
        // Bounded presence only: SNSS header/version/hash, no payload decoding,
        // no force-flush/private ABI. Restoration below proves actual contents.
        const storage=await until(async()=>{owner();try{return snapshotSessionStorage(c.slug,closed[0]!.contextKey);}catch{return undefined;}},20000);
        assert("natural nonempty disposable Tabs storage exists before quit",storage.files.length>0);writeFileSync(join(directory,"storage-before-quit.json"),JSON.stringify(storage,null,2));writeFileSync(join(directory,"closed-tabs.json"),JSON.stringify(closed,null,2));
      }else{
        for(const old of closed)await action("resume",{sentinelURL,fixtureURL:old.url,restorationID:old.restorationID,sourceID:old.sourceID,contextKey:old.contextKey});
        const eligible=await until(async()=>{const s=await action("snapshot");return s.records.length===2&&s.records.every(r=>r.canRestore===true)?s:undefined;},30000);
        assert("both persisted historical IDs are eligible in new PID same context",sentinelState(eligible,sentinelURL)&&eligible.contextKey===closed[0]!.contextKey&&eligible.records.every(r=>r.restores.length===0));
        for(const old of closed){const requested=await action("restore",{url:old.url});assert("restart restore callback is not inline "+old.url.split("/").at(-2),one(requested,old.url).restores.length===0);
          await until(async()=>{const s=await action("status");return one(s,old.url).restores.length?s:undefined;});
          const s=await action("snapshot"),r=one(s,old.url);
          assert("restart restore reports exact historical ID once asynchronously on main "+old.url.split("/").at(-2),r.restores.length===1&&r.restores[0]?.main===true&&r.restores[0]?.insideAction===false&&r.callbackID===old.restorationID&&r.callbackID!==old.sourceID&&r.callbackID!==r.restoredLiveID&&positiveID(r.restoredLiveID)&&r.restoredInitialized===true&&r.restoredURL===old.url&&r.canRestore===false&&sentinelState(s,sentinelURL));
          const target=await until(async()=>{owner();const list=await client.request("Target.getTargets"),matches=(list.targetInfos as {targetId:string;type:string;url:string}[]).filter(i=>i.type==="page"&&i.url===old.url);if(matches.length>1)throw new Error("ambiguous restored fixture target");return matches.length===1?matches[0]!.targetId:undefined;});await loaded(target,old.url);
          const tab=old.url.split("/").at(-2)!,after=clean(await client.targetCommand(target,"Page.getNavigationHistory") as unknown as History);assert(`${tab} restored exact full A B C history with B selected`,same(after,expected(tab)));histories[tab]={...(histories[tab] as object),after};
        }
        const after=await action("snapshot");
        assert("genuine restore callbacks retain requested fixture order",same(after.events.map(e=>e.type),["resumed","resumed","restored","restored"])&&same(after.events.map(e=>e.url),[...urls,...urls])&&after.events.every((e,index)=>e.main===true&&e.order===index));
        assert("two simultaneously live restored wrappers retain distinct IDs and consume each historical ID once",after.records.length===2&&after.simultaneousRestoredDistinct===true&&sentinelState(after,sentinelURL)&&after.events.filter(e=>e.type==="resumed").length===2&&after.events.filter(e=>e.type==="restored").length===2&&after.events.every(e=>e.type!=="unexpected-restored")&&new Set(after.records.map(r=>r.restoredLiveID)).size===2&&after.records.every(r=>r.restores.length===1&&r.closes.length===0&&r.restoredInitialized===true&&r.restoredURL===r.url&&r.canRestore===false&&r.callbackID===closed.find(old=>old.url===r.url)?.restorationID));
        assert("new sentinel document survives both restores",await evaluate(sentinel,`location.href===${JSON.stringify(sentinelURL)}&&document.title==='Sentinel'`)===true);
      }
      writeFileSync(join(directory,"history.json"),JSON.stringify(histories,null,2));
    }finally{await c.quit(launch,`restart launch ${number} graceful quit`);}
    assert(`restart launch ${number} process actually exited`,Bun.spawnSync(["/bin/kill","-0",String(pids[number-1])],{stdout:"ignore",stderr:"ignore"}).success===false);
    const gate=transcript(join(c.outDir,`launch-${number}`,"record.jsonl"),number);gates.push(gate);writeFileSync(join(directory,"transcript-gates.json"),JSON.stringify(gates,null,2));assert(`restart launch ${number} nonvacuous raw driver calls returns and genuine callbacks recorded`,true);
    if(number===1){const storage=snapshotSessionStorage(c.slug,closed[0]!.contextKey);writeFileSync(join(directory,"storage-after-quit.json"),JSON.stringify(storage,null,2));}
  }
}
