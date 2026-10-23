import type { ScenarioContext } from "../launch-slice/run";
import { verifyAgentEndpoint, type CDPResult } from "../../../arc/tools/agent-cdp";
import { mkdirSync, readFileSync, renameSync, writeFileSync } from "node:fs";
import { join } from "node:path";

const PREFIX = "/arc-sessions-probe-7f3a/";
export const fixture = (request: Request) => {
  const path = new URL(request.url).pathname;
  if (!path.startsWith(PREFIX)) return;
  const page = path.slice(PREFIX.length);
  if (!["A", "B", "C", "sentinel"].includes(page)) return new Response("not found", {status:404});
  return new Response(`<!doctype html><title>Sessions ${page}</title><h1>Sessions history ${page}</h1>`,
    {headers:{"content-type":"text/html; charset=utf-8", "cache-control":"no-store"}});
};
type DriverEvent = {type:string; main:boolean; insideAction:boolean};
type State = {
  selected?:boolean; knownFixtures?:number; sourceIDPresent?:boolean; sentinelSharesBrowser?:boolean;
  recorderForwardingBefore?:boolean; recorderForwardingAfter?:boolean;
  closedCount:number; restoreCount:number; events:DriverEvent[];
  oldInitializedAfterClose?:boolean; browserInitializedAfterClose?:boolean; sentinelInitializedAfterClose?:boolean;
  restorationIDPresent?:boolean; restorationMatchesSourceID?:boolean; canRestoreAfterClose?:boolean;
  canRestoreBeforeRestore?:boolean; restoreCallbackInline?:boolean;
  oldInitializedAfterRestore?:boolean; browserInitializedAfterRestore?:boolean; sentinelInitializedAfterRestore?:boolean;
  restoredInitialized?:boolean; restoredIsNewWrapper?:boolean; restoredURL?:string;
  callbackIDPresent?:boolean; callbackUsesSourceID?:boolean; callbackUsesRestorationID?:boolean; canRestoreAfterRestore?:boolean;
};
type Response = {id:string; operation:string; pid:number; nonce:string; main:boolean; ok?:boolean; error?:string; state:State};
type History = {currentIndex:number; entries:{id:number; url:string; title:string}[]};
type Transcript = {ev?:string; dir?:string; cls?:string; sel?:string;from?:string};

export default async function scenario(c: ScenarioContext) {
  const assert = (name:string, pass:unknown, detail = "") => {
    c.assert(name, pass, detail); if (!pass) throw new Error(name + (detail ? `: ${detail}` : ""));
  };
  assert("public API scenario requires standard boundary recording", c.recorded && !c.traced);
  assert("public API scenario uses disposable engine test agent", ["trace156", "ours156"].includes(c.slug));
  const directory = join(c.outDir,"sessions-driver");
  mkdirSync(directory,{recursive:true,mode:0o700});
  const dylib = join(directory,"SessionsPublicAPIDriver.dylib");
  const compile = Bun.spawnSync(["xcrun","clang","-dynamiclib","-fobjc-arc","-Wall","-Wextra","-Werror",
    "-framework","Foundation","-framework","AppKit",join(import.meta.dir,"sessions-public-api-driver.m"),"-o",dylib],
    {stdout:"pipe",stderr:"pipe"});
  writeFileSync(join(directory,"compile.log"),compile.stdout.toString()+compile.stderr.toString());
  assert("public API driver compiles with strict warnings", compile.success, compile.stderr.toString().slice(-1500));
  const nonce = `sessions-public-api-${crypto.randomUUID()}`;
  const fixtureURL = `${c.base}${PREFIX}B`;
  const launch = c.launch({libraries:[dylib],environment:{
    ARC_SCENARIO_SESSIONS_DIRECTORY:directory, ARC_SCENARIO_SESSIONS_NONCE:nonce,
    ARC_SCENARIO_SESSIONS_AGENT:c.slug, ARC_SCENARIO_SESSIONS_URL:fixtureURL,
  }});
  const observations:Response[] = [];
  try {
    if (!await c.ready(launch,"public API agent launch")) return;
    const pid = launch.pid();
    assert("public API launch exposes an owned PID", pid !== undefined);
    const owner = () => {
      if (launch.pid() !== pid || verifyAgentEndpoint(c.slug,c.port) !== pid) throw new Error("public API agent PID/endpoint changed");
    };
    const ready = await c.waitFor(() => {
      const value = JSON.parse(readFileSync(join(directory,"ready.json"),"utf8")) as {
        pid?:number; nonce?:string; agent?:string; installed?:boolean; recorderAliases?:boolean;
      };
      return value.pid === pid && value.nonce === nonce && value.agent === c.slug && value.installed === true && value.recorderAliases === true ? value : undefined;
    },10000);
    assert("public API driver ready with current PID and recorder aliases", ready !== undefined);
    let sequence = 0;
    const action = async (operation:string):Promise<State> => {
      owner();
      const id = `${nonce}-${++sequence}`;
      const request = join(directory,"request.json");
      writeFileSync(request+".tmp",JSON.stringify({id,operation,pid,nonce,url:fixtureURL}),{mode:0o600});
      renameSync(request+".tmp",request);
      const response = await c.waitFor(() => {
        const value = JSON.parse(readFileSync(join(directory,"response.json"),"utf8")) as Response;
        return value.id === id && value.pid === pid && value.nonce === nonce && value.operation === operation ? value : undefined;
      },10000);
      if (!response) throw new Error(`public API ${operation} response timed out; action will not be replayed`);
      owner(); observations.push(response);
      writeFileSync(join(directory,"observations.json"),JSON.stringify(observations,null,2));
      if (response.ok !== true || response.main !== true) throw new Error(`public API ${operation}: ${response.error ?? "not main-thread/success"}`);
      return response.state;
    };
    const cachedUntil = async (predicate:(state:State)=>boolean):Promise<State> => {
      const end = Date.now()+10000;
      while (Date.now()<end) {
        const state = await action("status"); if (predicate(state)) return state;
        await c.sleep(100);
      }
      throw new Error("public API cached callback wait timed out");
    };
    const client = await c.client();
    const evaluate = async (target:string, expression:string) => {
      owner(); const result = await client.targetCommand(target,"Runtime.evaluate",{expression,awaitPromise:true,returnByValue:true});
      if (result.exceptionDetails) throw new Error("fixture evaluation failed");
      return (result.result as CDPResult)?.value;
    };
    const loaded = async (target:string, destination:string) => {
      const end = Date.now()+10000;
      while (Date.now()<end) {
        if (await evaluate(target,`document.readyState==='complete'&&location.href===${JSON.stringify(destination)}`)) return;
        await c.sleep(100);
      }
      throw new Error("exact fixture document did not load");
    };
    const create = async (page:string) => {
      owner(); const url = `${c.base}${PREFIX}${page}`;
      const result = await client.request("Target.createTarget",{url,background:true});
      if (typeof result.targetId !== "string") throw new Error("fixture has no exact target ID");
      await loaded(result.targetId,url); return result.targetId;
    };
    const sentinel = await create("sentinel");
    const target = await create("A");
    for (const page of ["B","C"]) {
      owner(); const url = `${c.base}${PREFIX}${page}`;
      await client.targetCommand(target,"Page.navigate",{url}); await loaded(target,url);
    }
    const history = await client.targetCommand(target,"Page.getNavigationHistory") as unknown as History;
    const middle = history.entries.filter(entry=>entry.url === fixtureURL);
    assert("fixture B history entry is unique", middle.length === 1);
    await client.targetCommand(target,"Page.navigateToHistoryEntry",{entryId:middle[0]!.id});
    await loaded(target,fixtureURL);
    const clean = (value:History) => ({index:value.currentIndex,
      pages:value.entries.map(entry=>entry.url.replace(c.base,"FIXTURE")), titles:value.entries.map(entry=>entry.title)});
    const expected = {index:1,pages:["A","B","C"].map(page=>`FIXTURE${PREFIX}${page}`),titles:["A","B","C"].map(page=>`Sessions ${page}`)};
    const before = clean(await client.targetCommand(target,"Page.getNavigationHistory") as unknown as History);
    assert("fixture has exact A B C history with B selected", JSON.stringify(before) === JSON.stringify(expected));
    const selected = await action("select");
    assert("driver observes exact fixture/sentinel and opaque source ID", selected.selected === true && selected.knownFixtures === 2 && selected.sourceIDPresent === true && selected.sentinelSharesBrowser === true && selected.recorderForwardingBefore === true);
    const close = await action("close");
    assert("public close produces one synchronous main-thread callback", close.closedCount === 1 && close.events.length === 1 && close.events[0]?.type === "closed" && close.events[0]?.main === true && close.events[0]?.insideAction === true);
    const closed = await action("closed-snapshot");
    assert("closed wrapper becomes uninitialized and historical ID restorable", closed.oldInitializedAfterClose === false && closed.browserInitializedAfterClose === true && closed.sentinelInitializedAfterClose === true && closed.restorationIDPresent === true && closed.restorationMatchesSourceID === false && closed.canRestoreAfterClose === true);
    const afterClose = await client.request("Target.getTargets");
    assert("public close removes exact target and preserves sentinel", (afterClose.targetInfos as {targetId:string}[]).every(info=>info.targetId !== target) && (afterClose.targetInfos as {targetId:string}[]).some(info=>info.targetId === sentinel));
    const restoredRequest = await action("restore");
    assert("restore starts restorable and does not invoke callback inline", restoredRequest.canRestoreBeforeRestore === true && restoredRequest.restoreCallbackInline === false && restoredRequest.restoreCount === 0);
    const callback = await cachedUntil(state=>state.restoreCount !== 0);
    assert("one asynchronous main-thread restore callback follows close", callback.restoreCount === 1 && callback.events.length === 2 && callback.events[1]?.type === "restored" && callback.events[1]?.main === true && callback.events[1]?.insideAction === false);
    const restored = await action("restored-snapshot");
    assert("restored wrapper uses requested historical ID and consumes restore", restored.oldInitializedAfterRestore === false && restored.browserInitializedAfterRestore === true && restored.sentinelInitializedAfterRestore === true && restored.restoredInitialized === true && restored.restoredIsNewWrapper === true && restored.restoredURL === fixtureURL && restored.callbackIDPresent === true && restored.callbackUsesRestorationID === true && restored.callbackUsesSourceID === false && restored.canRestoreAfterRestore === false && restored.recorderForwardingAfter === true);
    const targets = await client.request("Target.getTargets");
    const matches = (targets.targetInfos as {targetId:string;type:string;url:string}[]).filter(info=>info.type === "page" && info.url === fixtureURL);
    assert("restore creates exactly one new fixture target", matches.length === 1 && matches[0]?.targetId !== target && matches[0]?.targetId !== sentinel);
    const restoredTarget = matches[0]!.targetId;
    await loaded(restoredTarget,fixtureURL);
    const after = clean(await client.targetCommand(restoredTarget,"Page.getNavigationHistory") as unknown as History);
    assert("restored history matches fixed A B C B contract", JSON.stringify(after) === JSON.stringify(expected));
    assert("sentinel page survives public close and restore", await evaluate(sentinel,`location.href===${JSON.stringify(`${c.base}${PREFIX}sentinel`)}&&document.title==='Sessions sentinel'`) === true);
    writeFileSync(join(directory,"history.json"),JSON.stringify({expected,before,after},null,2));
    // Do not let startup-only recorder coverage promote this API scenario. These
    // genuine app callbacks must still cross the canonical recorder forwarding.
    const rows = readFileSync(join(c.outDir,"launch-1","record.jsonl"),"utf8").trim().split("\n").map(line=>JSON.parse(line) as Transcript);
    const calls = rows.filter(row=>row.ev === "call"),attributed=calls.some(row=>Object.hasOwn(row,"from"));
    if(attributed)assert("caller-aware transcript fully attributes fixture calls",calls.length>0&&calls.every(row=>typeof row.from==="string"&&row.from.trim().length>0));
    const fixtureCalls=attributed?calls.filter(row=>row.from==="SessionsPublicAPIDriver.dylib"):calls;
    assert("boundary recorder observes public close and restore API", fixtureCalls.some(row=>row.dir === "->engine" && row.cls === "ArcWebContents" && row.sel === "close") && fixtureCalls.some(row=>row.dir === "->engine" && row.cls === "ArcBrowser" && row.sel === "restoreWebContentsWithSessionId:"));
    assert("boundary recorder observes genuine close and restore callbacks", calls.filter(row=>row.dir === "engine->app" && row.sel === "webContents:didCloseWithSessionID:").length === 1 && calls.filter(row=>row.dir === "engine->app" && row.cls === "ADK.ChromiumBrowser" && row.sel === "browser:didRestoreWebContents:withSessionId:").length === 1);
  } finally {
    writeFileSync(join(directory,"observations.json"),JSON.stringify(observations,null,2));
    await c.quit(launch,"public API agent quit");
  }
}
