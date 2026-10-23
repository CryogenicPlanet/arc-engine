#!/usr/bin/env bun
// Test-only owned-app boundary probe; no AX/native readiness or global input.
import { PublicTraceProjection } from "./webextras_fixture_projection";
import { AgentCDP, verifyAgentEndpoint } from "../../../arc/tools/agent-cdp";
import { chmodSync, constants, copyFileSync, existsSync, mkdirSync, readdirSync, readFileSync, realpathSync, renameSync, statSync, writeFileSync } from "node:fs";
import { dirname, join, resolve } from "node:path";

type RecordRow = { event: any; begin: number; end: number };
function canonical(value: any): any {
  if (Array.isArray(value)) return value.map(canonical);
  if (value && typeof value === "object") return Object.fromEntries(Object.keys(value).sort().map(key => [key, canonical(value[key])]));
  return value;
}
function assert(condition: unknown, message: string): asserts condition {
  if (!condition) throw new Error(message);
}
function parseRecords(bytes: Buffer, base = 0): RecordRow[] {
  assert(bytes.length > 0 && bytes[bytes.length - 1] === 10, "empty or incomplete recorder data");
  let begin = 0;
  const rows: RecordRow[] = [];
  for (let end = bytes.indexOf(10); end >= 0; end = bytes.indexOf(10, begin)) {
    assert(end > begin, "blank recorder line");
    const event = JSON.parse(bytes.subarray(begin, end).toString("utf8"));
    assert(Number.isSafeInteger(event.seq) && event.seq >= 0, "invalid recorder sequence ID");
    rows.push({ event, begin: base + begin, end: base + end + 1 });
    begin = end + 1;
  }
  return rows;
}
function validSpan(record: Buffer, begin: any, end: any): Buffer {
  assert(Number.isSafeInteger(begin) && Number.isSafeInteger(end) && begin >= 0 && end > begin && end <= record.length &&
    (begin === 0 || record[begin - 1] === 10) && record[end - 1] === 10, "empty, unaligned, or invalid recorder span");
  return record.subarray(begin, end);
}
function scanRecords(rows: RecordRow[], initial = new Map<string, RecordRow[]>()): { matches: Map<number, RecordRow>; stacks: Map<string, RecordRow[]> } {
  const seen = new Set<number>();
  const stacks = new Map<string, RecordRow[]>([...initial].map(([thread, stack]) => [thread, [...stack]]));
  const matches = new Map<number, RecordRow>();
  for (const row of rows) {
    const event = row.event;
    assert(!seen.has(event.seq), `duplicate recorder ID ${event.seq}`); seen.add(event.seq);
    if (event.ev === "hooked") continue;
    assert(event.ev === "call" || event.ev === "ret", "unknown recorder event");
    assert(typeof event.thread === "string" && Number.isSafeInteger(event.depth) && event.depth >= 0 &&
      ["->engine", "engine->app"].includes(event.dir) && ["+", "-"].includes(event.kind) &&
      typeof event.cls === "string" && typeof event.sel === "string", "invalid boundary identity/depth");
    assert(event.kind === "+" ? event.self === null : typeof event.self === "string" && /^<[^<>]+#\d+>$/.test(event.self), "invalid boundary object ID");
    const stack = stacks.get(event.thread) ?? []; stacks.set(event.thread, stack);
    if (event.ev === "call") {
      assert(event.depth === stack.length && Array.isArray(event.args), `call nesting mismatch: ${event.sel}`);
      stack.push(row);
    } else {
      const call = stack.pop();
      assert(call && event.depth === stack.length && call.event.seq < event.seq &&
        ["cls", "kind", "sel", "self", "dir", "thread", "from"].every(key => call.event[key] === event[key]) &&
        Object.hasOwn(event, "ret"), `unmatched return: ${event.sel}`);
      matches.set(call.event.seq, row);
    }
  }
  return { matches, stacks };
}
function validateNesting(rows: RecordRow[], initial = new Map<string, RecordRow[]>()): Map<number, RecordRow> {
  const { matches, stacks } = scanRecords(rows, initial);
  assert([...new Set([...initial.keys(), ...stacks.keys()])].every(thread =>
    JSON.stringify((stacks.get(thread) ?? []).map(row => row.event.seq)) === JSON.stringify((initial.get(thread) ?? []).map(row => row.event.seq))),
    "incomplete call/callback tail or changed ambient stack");
  return matches;
}
function expectedSelectors(operation: string): string[] {
  const one: Record<string, string> = {
    "console-open":"openConsoleView", frontend:"webContents", "console-close":"close", "panel-closed":"currentPanel:error:",
    "media-types":"getMediaContentTypes:", "pip-request-error":"requestPictureInPicture:", "pip-exit-error":"exitPictureInPicture:error:",
  };
  if (operation === "clock-snapshot") return ["position", "playbackRate", "duration", "currentTime", "endOfMedia"];
  if (operation === "clock-held-time") return ["currentTime"];
  if (operation === "subscription") return ["subscribe:autoEnable:notification:", "execute:completion:error:", "unsubscribe:"];
  return [one[operation] ?? "execute:completion:error:"];
}
function validateActionTrace(action: any, rows: RecordRow[], record: Buffer, projection?: PublicTraceProjection, held = new Map<string, string>()): any[] {
  // ArcBrowserApplication.run is recorded around the lifetime of the native run loop.
  // Derive ambient stacks from the prefix; actions must restore those exact stacks.
  const ambient = action.recordBegin > 0 ? scanRecords(parseRecords(record.subarray(0, action.recordBegin))).stacks : new Map<string, RecordRow[]>();
  const mainDepth = ambient.get("main")?.length ?? 0;
  const matched = validateNesting(rows, ambient);
  const expected = expectedSelectors(action.operation);
  assert(Array.isArray(action.publicCalls) && JSON.stringify(action.publicCalls.map((call: any) => call.selector)) === JSON.stringify(expected), "missing/unexpected public invocation list");
  let priorEnd = action.recordBegin;
  const publicRows: RecordRow[] = [];
  for (const call of action.publicCalls) {
    validSpan(record, call.recordBegin, call.recordEnd);
    assert(call.recordBegin >= priorEnd && call.recordEnd <= action.recordEnd, "public invocation range/order invalid");
    priorEnd = call.recordEnd;
    const cls = action.operation.startsWith("clock-") ? (call.selector === "position" ? "ArcWebContentsMediaSession" : "ArcMediaPosition") : action.operation === "media-types" || action.operation.startsWith("pip-") ? "ArcWebContentsMediaSession" : "ArcWebContentsDevTools";
    const candidates = rows.filter(row => row.begin >= call.recordBegin && row.end <= call.recordEnd && row.event.ev === "call" &&
      row.event.dir === "->engine" && row.event.cls === cls && row.event.sel === call.selector && row.event.depth === mainDepth && row.event.thread === "main");
    assert(candidates.length === 1, `expected exactly one recorded ${cls}.${call.selector} invocation`);
    const entry = candidates[0]!;
    if (projection?.byCaller) assert(entry.event.from === projection.fixtureImage, "public call is not attributed to the selected fixture image");
    const returned = matched.get(entry.event.seq);
    assert(returned && returned.end <= call.recordEnd, `missing matching public return: ${call.selector}`);
    publicRows.push(entry);
  }
  assert(action.recordAfterReturn === action.publicCalls.at(-1).recordEnd, "missing/falsified final public return offset");
  for (const selector of expected) assert(rows.filter(row => row.event.ev === "call" && row.event.thread === "main" && row.event.depth === mainDepth && row.event.sel === selector && row.event.dir === "->engine").length === expected.filter(item => item === selector).length, `unexpected public invocation count: ${selector}`);
  let callIndex = -1;
  let callbackIndex = 0;
  let inPublic = false;
  for (const event of action.timeline ?? []) {
    if (event.event === "publicCall") { ++callIndex; inPublic = true; continue; }
    if (event.event === "publicReturn") { inPublic = false; continue; }
    const callback = action.callbacks?.[callbackIndex++];
    const call = action.publicCalls[callIndex];
    assert(callback && call && callback.event === event.event && callback.publicSelector === call.selector &&
      Number.isSafeInteger(callback.recordOffset) && callback.recordOffset >= call.recordBegin && callback.recordOffset <= action.recordEnd &&
      (inPublic ? callback.recordOffset <= call.recordEnd : callback.recordOffset >= call.recordEnd) &&
      (callback.recordOffset === 0 || record[callback.recordOffset - 1] === 10), "callback record offset/invocation order invalid");
  }
  assert(callbackIndex === action.callbacks?.length, "callback timeline is incomplete");
  const displaySelector = action.operation === "console-open" ? "webContentsDevTools:displayAs:" : action.operation === "console-close" ? "webContentsDevToolsDidClose:" : null;
  const delegates = rows.filter(row => row.event.ev === "call" && row.event.dir === "engine->app" && ["webContentsDevTools:displayAs:", "webContentsDevToolsDidClose:"].includes(row.event.sel));
  if (displaySelector) {
    assert(delegates.length === 1 && delegates[0]!.event.sel === displaySelector, "missing/unexpected display/close callback");
    const callback = action.callbacks?.find((event: any) => event.event === (action.operation === "console-open" ? "display" : "closed"));
    const entry = delegates[0]!;
    const returned = matched.get(entry.event.seq)!;
    const ancestors = projection ? scanRecords(rows.filter(row => row.end <= entry.begin), ambient).stacks.get("main") ?? [] : [];
    const underPublicCall = ancestors.some(row => row.event.seq === publicRows[0]!.event.seq);
    const validCallbackDepth = projection ? underPublicCall === callback?.beforePublicReturn :
      entry.event.depth === mainDepth + (callback?.beforePublicReturn ? 1 : 0);
    const recorderInsideHook = callback?.recordOffset <= entry.begin && returned.end <= callback?.recordAfterForwarding;
    const recorderOutsideHook = entry.end <= callback?.recordOffset && callback?.recordAfterForwarding <= returned.begin;
    assert(callback?.forwarded === true && Number.isSafeInteger(callback.recordOffset) && Number.isSafeInteger(callback.recordAfterForwarding) &&
      callback.recordOffset >= action.recordBegin && callback.recordOffset <= callback.recordAfterForwarding &&
      callback.recordAfterForwarding <= action.recordEnd && returned.end <= action.recordEnd &&
      (recorderInsideHook || recorderOutsideHook) && entry.event.thread === "main" &&
      validCallbackDepth &&
      entry.event.args?.[0] === publicRows[0]!.event.self, "incomplete or mismatched app callback forwarding");
  } else assert(!delegates.length, "unexpected display/close callback");
  if (action.operation.startsWith("clock-")) validateClockReceivers(action, rows, matched, publicRows, held);
  if (projection) return projection.project(rows);
  // Legacy replay only: live fixture comparisons use the frozen-metadata projection.
  // A complete ordered boundary shape closes diff.py's depth/order/object-identity gaps.
  const identities = new Map<string, string>();
  const counts = new Map<string, number>();
  const norm = (value: any): any => typeof value === "string" ? value.replace(/<([^<>]+)#(\d+)>/g, (full, cls) => {
    if (!identities.has(full)) { const count = (counts.get(cls) ?? 0) + 1; counts.set(cls, count); identities.set(full, `<${cls}#${count}>`); }
    return identities.get(full)!;
  }) : value;
  return rows.map(row => ({ ev:row.event.ev, thread:row.event.thread, depth:row.event.depth - (ambient.get(row.event.thread)?.length ?? 0), dir:row.event.dir,
    cls:row.event.cls, kind:row.event.kind, sel:row.event.sel, self:norm(row.event.self), from:row.event.from }));
}
function arcError(error: any, code: number): boolean {
  return error?.domain === "company.thebrowser.Arc" && error.code === code && typeof error.description === "string" &&
    error.userInfo && typeof error.userInfo === "object" && !Array.isArray(error.userInfo);
}
function outcomeErrors(action: any): string[] {
  const errors: string[] = [];
  const check = (value: unknown, message: string) => { if (!value) errors.push(message); };
  const callbacks = action.callbacks ?? [];
  for (const callback of callbacks) check(callback.mainThread === true && typeof callback.beforePublicReturn === "boolean", "callback thread/timing unverified");
  check(Array.isArray(action.timeline) && action.timeline.length > 0, "missing public/callback timeline");
  let active: string | undefined;
  let lastSelector: string | undefined;
  let expectedIndex = 0;
  for (const event of action.timeline ?? []) {
    if (event.event === "publicCall") {
      check(!active && event.selector === expectedSelectors(action.operation)[expectedIndex++], "public call timeline order invalid");
      active = event.selector; lastSelector = event.selector;
    } else if (event.event === "publicReturn") {
      check(!!active && active === event.selector, "public return precedes or mismatches invocation"); active = undefined;
    } else {
      check(["completion", "subscription", "display", "closed"].includes(event.event) && event.mainThread === true &&
        event.beforePublicReturn === !!active && event.selector === lastSelector, "callback timeline invocation/order mismatch");
    }
  }
  check(!active && expectedIndex === expectedSelectors(action.operation).length, "incomplete public timeline");
  check(JSON.stringify((action.timeline ?? []).filter((event: any) => !["publicCall", "publicReturn"].includes(event.event)).map((event: any) => event.event)) ===
    JSON.stringify(callbacks.map((event: any) => event.event)), "callback entry sequence missing from timeline");
  const calls = action.timeline?.filter((event: any) => event.event === "publicCall").map((event: any) => event.selector);
  const returns = action.timeline?.filter((event: any) => event.event === "publicReturn").map((event: any) => event.selector);
  check(JSON.stringify(calls) === JSON.stringify(expectedSelectors(action.operation)) && JSON.stringify(calls) === JSON.stringify(returns), "timeline missing public calls/returns");
  const completed = callbacks.filter((event: any) => event.event === "completion");
  const subscriptions = callbacks.filter((event: any) => event.event === "subscription");
  const asynchronous = ["execute-evaluate", "execute-unknown", "subscription"].includes(action.operation);
  check(completed.length === (asynchronous ? 1 : 0), "unexpected completion count");
  check(subscriptions.length === (action.operation === "subscription" ? 1 : 0), "unexpected subscription count");
  if (asynchronous) check(action.value === true && action.error === null, "command was not accepted without immediate error");
  if (action.operation === "execute-evaluate" || action.operation === "subscription") check(completed[0]?.error === null && completed[0]?.parsed?.result?.type === "number" && completed[0]?.parsed?.result?.value === 42, "evaluation did not complete with primitive 42");
  if (action.operation === "execute-unknown") {
    const callback = completed[0];
    check(callback?.message === null && arcError(callback?.error, 7), "unknown method did not complete with nil/NSError7");
    let dictionary: any;
    try { dictionary = JSON.parse(callback.error.description); } catch {}
    check(dictionary?.code === -32601 && typeof dictionary.message === "string" && dictionary.message.includes("ArcWebextrasMissing.noSuchMethod"), "unknown method error does not preserve whole CDP error dictionary");
  }
  if (action.operation === "execute-malformed") check(action.value === false && arcError(action.error, 2), "malformed params did not immediately fail with NSError2");
  if (action.operation === "panel-closed") check(action.value === false && arcError(action.error, 10), "closed panel did not immediately fail with NSError10");
  if (action.operation === "subscription") {
    const callback = subscriptions[0];
    check(Number.isSafeInteger(action.subscriptionToken) && callback?.subscriptionID === action.subscriptionToken && action.unsubscribeReturn === true, "subscription token/unsubscribe mismatch");
    check(callback?.error === null && callback?.parsed?.type === "log" && callback?.parsed?.args?.length === 1 &&
      callback.parsed.args[0].type === "string" && callback.parsed.args[0].value === action.nonce && typeof callback.parsed.timestamp === "number" &&
      Number.isFinite(callback.parsed.timestamp) && callback.parsed.timestamp > 0 && Number.isSafeInteger(callback.parsed.executionContextId) && callback.parsed.executionContextId > 0, "subscription payload/nonce/context invalid");
  }
  const expectedDelegate = action.operation === "console-open" ? "display" : action.operation === "console-close" ? "closed" : null;
  const delegates = callbacks.filter((event: any) => ["display", "closed"].includes(event.event));
  check(expectedDelegate ? delegates.length === 1 && delegates[0].event === expectedDelegate && delegates[0].forwarded === true : delegates.length === 0, "unexpected/incomplete delegate callback count");
  return errors;
}
function comparedMessage(event: any, action: any): any {
  if (event.event !== "subscription" || typeof event.message !== "string") return event.message;
  // Preserve serialization spelling/order/whitespace. Replace only intrinsic scalar contents.
  let text = event.message;
  if (typeof action.nonce === "string") text = text.split(JSON.stringify(action.nonce)).join('"<probe-nonce>"');
  text = text.replace(/("timestamp"\s*:\s*)-?(?:0|[1-9]\d*)(?:\.\d+)?(?:[eE][+-]?\d+)?(?=\s*[,}])/g, "$1<timestamp>");
  text = text.replace(/("executionContextId"\s*:\s*)-?(?:0|[1-9]\d*)(?=\s*[,}])/g, "$1<execution-context>");
  text = text.replace(/("scriptId"\s*:\s*")\d+("(?=\s*[,}]))/g, "$1<script-id>$2");
  return text;
}
function normalizedPayload(value: any, action: any): any {
  // Preserve raw payload in observations. Only intrinsic CDP metadata is normalized for comparison.
  const normalize = (item: any, key?: string): any => {
    if (typeof action.nonce === "string" && item === action.nonce) return "<probe-nonce>";
    if (key === "timestamp" && typeof item === "number") return "<timestamp>";
    if (key === "executionContextId" && Number.isSafeInteger(item)) return "<execution-context>";
    if (key === "scriptId" && typeof item === "string" && /^\d+$/.test(item)) return "<script-id>";
    if (Array.isArray(item)) return item.map(entry => normalize(entry));
    if (item && typeof item === "object") return Object.fromEntries(Object.entries(item).map(([name, entry]) => [name, normalize(entry, name)]));
    return item;
  };
  return normalize(value);
}

// Fixed before any runtime measurement. Both MediaPosition and TimeTicks use integer
// microseconds (media_position.cc; base/time/time_apple.mm). Four endpoint roundings
// allow 4 us * (1 + |rate|); no measured jitter is used to widen this bound.
const mediaClockPolicy = Object.freeze({ quantizationSeconds: 0.000004, maxGetterSeconds: 0.025,
  minHeldSpacingSeconds: 0.2, warmupMilliseconds: 250, heldMilliseconds: 400,
  fixtureSHA256: "fa0a68bc0d09eadfa7d1b199673cd865cc075d813b79cb1866f90af75916ee54" });
class ClockProbeInconclusive extends Error {
  constructor(message:string){super(message);this.name="ClockProbeInconclusive";}
}
function isClockInconclusive(error:unknown):boolean{return error instanceof ClockProbeInconclusive;}
const clockVisibilityLimitMilliseconds=5000;
async function clockBounded<T>(pending:Promise<T>,milliseconds:number):Promise<T>{
  let timer:ReturnType<typeof setTimeout>|undefined;
  try{return await Promise.race([pending,new Promise<T>((_resolve,reject)=>{timer=setTimeout(()=>reject(new ClockProbeInconclusive("actual visibility wait exceeded 5000ms")),milliseconds);})]);}
  finally{if(timer!==undefined)clearTimeout(timer);}
}
async function activateMediaClockTarget(target:string,io:{verify:()=>void;read:(label:string)=>Promise<any>;bring:()=>Promise<any>;save:(report:any)=>void;now:()=>number;sleep:(ms:number)=>Promise<void>}):Promise<any>{
  const report:any={target,method:"Page.bringToFront",status:"checking-identity",visibilityLimitMilliseconds:clockVisibilityLimitMilliseconds,samples:[]};io.save(report);
  const identity=(value:any)=>value?.urlMatches===true&&value?.nonceMatches===true;
  try{
    io.verify();report.before=await io.read("activation-identity");io.save(report);
    if(!identity(report.before))throw new ClockProbeInconclusive("owned fixture URL/nonce identity changed before activation");
    io.verify();report.status="activating";report.request={method:"Page.bringToFront",target,hostBeforeNanos:process.hrtime.bigint().toString()};io.save(report);
    report.ack=await io.bring();report.hostAfterAckNanos=process.hrtime.bigint().toString();io.save(report);
    const started=io.now(),deadline=started+clockVisibilityLimitMilliseconds;
    report.status="waiting-for-actual-visibility";io.save(report);
    for(let index=0;index<35;++index){
      if(io.now()>=deadline)break;
      io.verify();const remaining=deadline-io.now();if(remaining<=0)break;
      const sample=await clockBounded(io.read(`activation-visibility-${index}`),remaining);
      report.samples.push({...sample,hostElapsedMilliseconds:io.now()-started});io.save(report);
      if(io.now()>deadline)throw new ClockProbeInconclusive("actual visibility sample arrived after 5000ms");
      if(!identity(sample))throw new ClockProbeInconclusive("owned fixture URL/nonce identity changed during activation");
      if(sample.visibilityState==="visible"&&sample.hidden===false){report.status="visible";io.save(report);return report;}
      const delay=Math.min(150,deadline-io.now());if(delay>0)await io.sleep(delay);
    }
    throw new ClockProbeInconclusive("owned fixture did not become actually visible within 5000ms");
  }catch(error){report.status="inconclusive";report.error=String(error);report.errorDetail={name:error instanceof Error?error.name:"Error",message:String(error instanceof Error?error.message:error),stack:error instanceof Error?error.stack:null};io.save(report);throw new ClockProbeInconclusive(`clock setup inconclusive: ${String(error)}`);}
}
function clockIdentityJS(fixtureURL:string,nonce:string):string{
  return `({urlMatches:location.href===${JSON.stringify(fixtureURL)},nonceMatches:document.querySelector('#webextras-probe')?.dataset.nonce===${JSON.stringify(nonce)},visibilityState:document.visibilityState,hidden:document.hidden,performanceNow:performance.now()})`;
}
// Process disappearance alone does not prove natural termination. Inspect the
// actual owned log after cleanup, including the Browser.close path.
function runtimeLogOutcome(log:string|null,cleanup:string|undefined,launched:boolean):any{
  const matches:any[]=[];let count=0;
  if(log!==null)for(const [index,line]of log.split(/\r?\n/).entries()){
    if(/^\[[^\]\r\n]*:(?:FATAL|DFATAL)(?::|\])/.test(line)||/^(?:\[[^\]\r\n]*\]\s*)?Check failed:\s*\S/.test(line)||/^FATAL(?::|\s)/.test(line)){
      ++count;if(matches.length<20)matches.push({line:index+1,text:line.slice(0,4096),truncated:line.length>4096});
    }
  }
  const forced=cleanup?.startsWith("SIGTERM")===true;
  const errors:string[]=[];
  if(count)errors.push(`runtime: ${count} fatal/check failure log line(s); first at app.log:${matches[0].line}`);
  if(forced)errors.push("runtime: forced SIGTERM cleanup is not natural termination");
  if(launched&&log===null)errors.push("runtime: owned app.log missing after launch");
  if(launched&&!cleanup)errors.push("runtime: launched process cleanup unverified");
  return {status:count?"fatal":forced?"forced-cleanup":!launched?"not-launched":log===null?"log-missing":!cleanup?"cleanup-unverified":"no-fatal-observed",
    naturalCleanup:launched&&log!==null&&cleanup?.startsWith("Browser.close")===true&&!count,forcedCleanup:forced,logStatus:log===null?"missing":"scanned",fatalCount:count,fatals:matches,errors};
}
const mediaClockPhases = [
  { name: "service-video-paused-one", rate: 0, seek: 1.25, declared: false },
  { name: "service-video-playing-one", rate: 1, seek: 0.5, declared: false },
  { name: "service-video-playing-two", rate: 2, seek: 0.5, declared: false },
  { name: "service-video-paused-two", rate: 0, seek: 2.25, declared: false },
  { name: "service-independent-paused", rate: 0, seek: 2.25, declared: true },
  { name: "service-independent-playing", rate: 1.5, seek: 2.25, declared: true },
] as const;
// Separate public service-only lane: no player, resource loading, activation or visibility override.
const mediaServiceClockPhases = [
  { name:"service-only-paused-one", rate:0, base:1.25, issuedRate:1 },
  { name:"service-only-playing-one", rate:1, base:1.25, issuedRate:1 },
  { name:"service-only-playing-two", rate:2, base:1.25, issuedRate:2 },
  { name:"service-only-paused-two", rate:0, base:2.25, issuedRate:1 },
  { name:"service-only-declared-paused", rate:0, base:7.25, issuedRate:1.5 },
  { name:"service-only-declared-playing", rate:1.5, base:7.25, issuedRate:1.5 },
] as const;
function clockModeFromArgs(argv:string[]): "default"|"media-clock"|"media-service-clock"|"devtools-frontend-events" {
  const flags=argv.filter(arg=>arg==="--media-clock"||arg==="--media-service-clock"||arg==="--devtools-frontend-events");
  assert(flags.length<=1,"probe modes are mutually exclusive and cannot be repeated");
  return flags.length ? flags[0]!.slice(2) as "media-clock"|"media-service-clock"|"devtools-frontend-events" : "default";
}
function serviceClockObservationJS(fixtureURL:string,nonce:string):string {
  return `(()=>{const session=navigator.mediaSession;return {phase:window.__webextrasClockPhase,
    urlMatches:location.href===${JSON.stringify(fixtureURL)},nonceMatches:document.querySelector('#webextras-probe')?.dataset.nonce===${JSON.stringify(nonce)},
    mediaSessionPresent:!!session,performanceNow:performance.now(),visibilityState:document.visibilityState,hidden:document.hidden,
    playbackState:session?.playbackState,positionReceipt:window.__webextrasClockReceipt}})()`;
}
function mediaServiceClockPrepareJS(phase:typeof mediaServiceClockPhases[number],fixtureURL:string,nonce:string):string {
  return `(()=>{const phase=${JSON.stringify(phase)},progress=[];
    const mark=stage=>{progress.push({stage,performanceNow:performance.now()});window.__webextrasClockProgress={phase:phase.name,stage,progress};};
    try{mark('service-identity-before');
      const observed=(${serviceClockObservationJS(fixtureURL,nonce)});delete observed.positionReceipt;
      if(!observed.urlMatches||!observed.nonceMatches||!observed.mediaSessionPresent)throw Error('service clock fixture identity/API unavailable');
      mark('service-identity-after');
      const session=navigator.mediaSession;
      mark('playback-state-before');session.playbackState=phase.rate===0?'paused':'playing';mark('playback-state-after');
      const input={duration:20,position:phase.base,playbackRate:phase.issuedRate};
      const receipt={phase:phase.name,source:'public-service-only',observed,input,beforeSetPerformanceNow:performance.now(),playbackState:session.playbackState};
      mark('input-setter-before');session.setPositionState(input);receipt.afterSetPerformanceNow=performance.now();mark('input-setter-after');
      window.__webextrasClockPhase=phase.name;window.__webextrasClockReceipt=receipt;
      mark('complete');return {receipt,progress};
    }catch(error){window.__webextrasClockFailure={name:String(error?.name??'Error'),message:String(error?.message??error),stack:String(error?.stack??''),stage:window.__webextrasClockProgress?.stage};mark('failed');throw error;}
  })()`;
}
function isNilClockPosition(action:any):boolean {
  return action.operation==="clock-snapshot"&&action.ok===false&&action.probeError==="position did not return an actual ArcMediaPosition"&&
    action.publicCalls?.length===1&&action.publicCalls[0]?.selector==="position"&&action.publicCalls[0]?.resultClass===null;
}
function validateMediaServiceClockPhase(phase:typeof mediaServiceClockPhases[number],actions:any[]):void {
  assert(actions.length===5&&actions.every((action,index)=>action.actionKey===clockActionKey(phase.name,index)&&action.clockPhase===phase.name&&action.clockAttempt===index),"missing/reordered service clock attempts");
  if(actions.some(isNilClockPosition))throw new ClockProbeInconclusive("public service position DTO did not converge in the fixed attempts");
  assert(actions.every(action=>action.ok===true),"failed service clock receipt");
  assert(actions.slice(0,3).every(action=>action.operation==="clock-snapshot")&&actions.slice(3).every(action=>action.operation==="clock-held-time"),"wrong service clock operation sequence");
  const receipt=actions[2].positionReceipt,input=receipt?.input,state=phase.rate===0?"paused":"playing";
  assert(receipt?.phase===phase.name&&receipt.source==="public-service-only"&&receipt.playbackState===state&&
    input?.duration===20&&input.position===phase.base&&input.playbackRate===phase.issuedRate&&
    Number.isFinite(receipt.observed?.performanceNow)&&Number.isFinite(receipt.beforeSetPerformanceNow)&&Number.isFinite(receipt.afterSetPerformanceNow)&&
    receipt.observed.performanceNow<=receipt.beforeSetPerformanceNow&&receipt.beforeSetPerformanceNow<=receipt.afterSetPerformanceNow,
    "missing/incorrect issued public service receipt");
  const identity=(observation:any)=>assert(observation?.urlMatches===true&&observation.nonceMatches===true&&observation.mediaSessionPresent===true&&
    typeof observation.visibilityState==="string"&&typeof observation.hidden==="boolean"&&Number.isFinite(observation.performanceNow),"service fixture identity/API/actual observation missing");
  identity(receipt.observed);
  for(const action of actions){
    assert(JSON.stringify(canonical(action.positionReceipt))===JSON.stringify(canonical(receipt)),"service receipt changed between attempts");
    for(const observation of [action.domBefore,action.domAfter]){
      identity(observation);
      assert(observation.phase===phase.name&&observation.playbackState===state&&observation.performanceNow>=receipt.afterSetPerformanceNow&&
        JSON.stringify(canonical(observation.positionReceipt))===JSON.stringify(canonical(receipt)),"service public state/receipt changed");
    }
    clockBracket(action);
  }
  const epsilon=mediaClockPolicy.quantizationSeconds;
  // Fixed warmups are diagnostic receipts. Final snapshot and same retained DTO must converge.
  const measurement=actions[2];
  assert(clockValue(measurement,"playbackRate")===phase.rate&&clockValue(measurement,"duration")===20&&clockValue(measurement,"endOfMedia")===false,"service rate/duration/end did not converge on final fixed attempt");
  for(const action of actions.slice(2)){
    const value=clockValue(action,"currentTime");
    assert(Number.isFinite(value)&&value>0&&value<20&&value>=phase.base-epsilon,"service clock is outside issued base/unclamped interior");
    if(phase.rate===0)assert(Math.abs(value-phase.base)<=epsilon,"paused service clock differs from exact issued base");
  }
  clockDelta(measurement,actions[3],phase.rate);clockDelta(actions[3],actions[4],phase.rate);clockDelta(measurement,actions[4],phase.rate);
}
function clockActionKey(phase: string, index: number): string { return `clock-${phase}-${index}`; }
function clockValue(action: any, selector: string): any { return action.publicCalls?.find((call: any) => call.selector === selector)?.value; }
function clockBracket(action: any): { begin: number; end: number; value: number } {
  const call = action.publicCalls?.find((entry: any) => entry.selector === "currentTime");
  const info = action.clock;
  assert(info && Number.isSafeInteger(info.timebaseNumer) && info.timebaseNumer > 0 && Number.isSafeInteger(info.timebaseDenom) && info.timebaseDenom > 0, "missing mach timebase");
  assert(/^\d+$/.test(call?.clockBeforeTicks) && /^\d+$/.test(call?.clockAfterTicks), "missing exact mach ticks");
  const begin = BigInt(call.clockBeforeTicks), end = BigInt(call.clockAfterTicks);
  assert(end >= begin && Number.isFinite(call.value), "invalid native clock bracket/value");
  // Convert only relative ticks to Number. Absolute uint64 timestamps remain strings.
  const factor = info.timebaseNumer / info.timebaseDenom / 1e9;
  assert(Number(end - begin) * factor <= mediaClockPolicy.maxGetterSeconds, "native clock getter bracket too wide");
  return { begin: 0, end: Number(end - begin) * factor, value: call.value };
}
function clockDelta(first: any, second: any, rate: number): void {
  clockBracket(first); clockBracket(second);
  assert(first.clock.heldToken === second.clock.heldToken && first.clock.dtoTag === second.clock.dtoTag &&
    first.ownerTag === second.ownerTag && first.clock.timebaseNumer === second.clock.timebaseNumer && first.clock.timebaseDenom === second.clock.timebaseDenom,
    "held DTO/token/owner/timebase changed");
  const left = first.publicCalls.find((call: any) => call.selector === "currentTime");
  const right = second.publicCalls.find((call: any) => call.selector === "currentTime");
  const factor = first.clock.timebaseNumer / first.clock.timebaseDenom / 1e9;
  const low = Number(BigInt(right.clockBeforeTicks) - BigInt(left.clockAfterTicks)) * factor;
  const high = Number(BigInt(right.clockAfterTicks) - BigInt(left.clockBeforeTicks)) * factor;
  assert(low >= mediaClockPolicy.minHeldSpacingSeconds, "held samples insufficiently separated");
  const difference = right.value - left.value;
  const epsilon = mediaClockPolicy.quantizationSeconds * (1 + Math.abs(rate));
  assert(difference >= rate * low - epsilon && difference <= rate * high + epsilon, `held DTO slope does not match ${rate}: delta=${difference}, bracket=[${low},${high}]`);
}
function validateClockPhase(phase: typeof mediaClockPhases[number], actions: any[]): void {
  assert(actions.length === 5 && actions.every((action, index) => action.actionKey === clockActionKey(phase.name, index) && action.clockPhase === phase.name && action.clockAttempt === index && action.ok === true), "missing/reordered clock phase attempts");
  const measurement = actions[2];
  const receipt = measurement.positionReceipt;
  assert(receipt && receipt.phase === phase.name && receipt.source === (phase.declared ? "independent" : "DOM-video") &&
    Number.isFinite(receipt.beforeReadPerformanceNow) && Number.isFinite(receipt.observed?.performanceNow) && Number.isFinite(receipt.beforeSetPerformanceNow) && Number.isFinite(receipt.afterSetPerformanceNow) && receipt.beforeReadPerformanceNow <= receipt.observed.performanceNow &&
    receipt.observed.performanceNow <= receipt.beforeSetPerformanceNow && receipt.beforeSetPerformanceNow <= receipt.afterSetPerformanceNow,
    "missing/reordered exact public position-state receipt");
  const input = receipt.input;
  assert(input && Number.isFinite(input.position) && Number.isFinite(input.duration) && input.position > 0 && input.position < input.duration &&
    input.playbackRate === (phase.declared ? 1.5 : phase.rate || 1), "invalid issued public position input");
  assert(actions.every(action => JSON.stringify(canonical(action.positionReceipt)) === JSON.stringify(canonical(receipt))), "position receipt changed between attempts");
  assert(actions.slice(0, 3).every(action => action.operation === "clock-snapshot") && actions.slice(3).every(action => action.operation === "clock-held-time"), "wrong clock phase operation sequence");
  const rate = clockValue(measurement, "playbackRate"), duration = clockValue(measurement, "duration");
  assert(rate === phase.rate && Number.isFinite(duration) && duration > 0 && clockValue(measurement, "endOfMedia") === false, "rate/duration/end state did not converge on final fixed attempt");
  const epsilon = mediaClockPolicy.quantizationSeconds;
  for (const action of actions) {
    for (const dom of [action.domBefore, action.domAfter]) {
      if(dom?.visibilityState!=="visible"||dom?.hidden!==false)throw new ClockProbeInconclusive("clock fixture lost actual visible state");
      assert(dom && dom.phase === phase.name && dom.readyState >= 2 && dom.seeking === false && dom.ended === false && dom.loop === false && Number.isFinite(dom.currentTime) &&
        Number.isFinite(dom.duration) && dom.duration > 6 && typeof dom.performanceNow === "number", "unready/ended/looping or wrong-phase DOM fixture");
      assert(dom.paused === (phase.declared || phase.rate === 0) && dom.playbackRate === (phase.declared || phase.rate === 0 ? 1 : phase.rate), "DOM stimulus is not the declared paused/playing phase");
      assert(dom.playbackState === (phase.rate === 0 ? "paused" : "playing") && JSON.stringify(canonical(dom.positionReceipt)) === JSON.stringify(canonical(receipt)) && dom.performanceNow >= receipt.afterSetPerformanceNow, "DOM mediaSession stimulus changed");
      if (phase.declared || phase.rate === 0) assert(Math.abs(dom.currentTime - phase.seek) <= epsilon, "DOM paused seek changed");
      else assert(dom.currentTime > 0.25 && dom.currentTime < dom.duration - 0.25, "playing fixture reached a clamp boundary");
    }
    clockBracket(action);
  }
  assert(Math.abs(duration - input.duration) <= epsilon, "public duration differs from issued position state");
  if (phase.declared) assert(input.duration === 20 && input.position === 7.25 && receipt.observed.paused === true &&
    Math.abs(receipt.observed.currentTime - phase.seek) <= epsilon, "independent position state did not override the paused real video");
  else {
    assert(input.duration === receipt.observed.duration && input.position === receipt.observed.currentTime && input.playbackRate === receipt.observed.playbackRate &&
      receipt.observed.paused === (phase.rate === 0) && Math.abs(duration - measurement.domAfter.duration) <= epsilon,
      "position input was not read from the actual DOM video");
    if (phase.rate === 0) assert(Math.abs(input.position - phase.seek) <= epsilon, "DOM input did not use the distinct paused seek");
    else assert(actions.at(-1).domAfter.currentTime > measurement.domBefore.currentTime + 0.2, "actual video clock did not advance");
  }
  if (phase.rate === 0) for (const action of actions.slice(2)) assert(Math.abs(clockValue(action, "currentTime") - input.position) <= epsilon, "paused public clock does not match issued base input");
  for (const action of actions.slice(2)) {
    const position = clockValue(action, "currentTime");
    assert(Number.isFinite(position) && position >= input.position - epsilon && position > 0 && position < duration, "measured/held public clock is outside its issued base/unclamped interior");
  }
  clockDelta(measurement, actions[3], phase.rate);
  clockDelta(actions[3], actions[4], phase.rate);
  clockDelta(measurement, actions[4], phase.rate);
}
function validateClockReceivers(action: any, _rows: RecordRow[], matches: Map<number, RecordRow>, publicRows: RecordRow[], held: Map<string, string>): void {
  assert(action.clock && typeof action.clock.heldToken === "string" && Number.isSafeInteger(action.clock.dtoTag) && action.clock.dtoTag > 0 && !action.callbacks.length, "missing held clock identity or unexpected callback");
  if (action.operation === "clock-snapshot") {
    assert(action.clock.heldToken === action.id, "fresh position token is not its request ID");
    const result = matches.get(publicRows[0]!.event.seq)?.event.ret;
    assert(typeof result === "string" && /^<ArcMediaPosition#\d+>$/.test(result), "position return is not a recorded ArcMediaPosition");
    held.clear(); held.set(action.clock.heldToken, result);
    held.set("<dto-tag>", String(action.clock.dtoTag));
  }
  assert(held.get("<dto-tag>") === String(action.clock.dtoTag), "held DTO tag changed");
  const receiver = held.get(action.clock.heldToken);
  assert(receiver && action.publicCalls.every((call: any) => call.receiverClass === (call.selector === "position" ? "ArcWebContentsMediaSession" : "ArcMediaPosition")), "unrecognized DTO token or receiver class");
  for (let index = 0; index < publicRows.length; ++index) {
    const call = action.publicCalls[index], entry = publicRows[index]!.event;
    assert(entry.args.length === 0, "clock getter has arguments");
    if (call.selector === "position") { assert(call.abi === "@16@0:8" && call.resultClass === "ArcMediaPosition", "position ABI/result mismatch"); continue; }
    assert(entry.self === receiver, "clock getter did not use the same retained public DTO");
    assert(call.abi === (call.selector === "endOfMedia" ? "B16@0:8" : "d16@0:8"), "clock getter ABI mismatch");
    const value = matches.get(entry.seq)?.event.ret;
    if (call.selector === "endOfMedia") assert(typeof call.value === "boolean" && value === call.value, "recorded BOOL differs from sidecar");
    else {
      const rounding = call.value === 0 ? 0 : 0.500001 * 10 ** (Math.floor(Math.log10(Math.abs(call.value))) - 5);
      assert(Number.isFinite(call.value) && typeof call.recordValue === "string" && Number.isFinite(Number(call.recordValue)) &&
        Math.abs(Number(call.recordValue) - call.value) <= rounding && value === Number(call.recordValue), "recorded double differs from source-formatted sidecar");
    }
  }
  // Each call bracket is independently ordered, even for getters not used in slope tests.
  for (const call of action.publicCalls) assert(/^\d+$/.test(call.clockBeforeTicks) && /^\d+$/.test(call.clockAfterTicks) && BigInt(call.clockAfterTicks) >= BigInt(call.clockBeforeTicks), "invalid getter clock ticks");
}
function mediaClockPrepareJS(phase: typeof mediaClockPhases[number], videoURL: string): string {
  return `(async()=>{
    const phase=${JSON.stringify(phase)}, events=[],progress=[];
    const mark=stage=>{progress.push({stage,performanceNow:performance.now()});window.__webextrasClockProgress={phase:phase.name,stage,progress};};
    window.__webextrasClockFailure=null;
    try{
      mark('initial-query');let v=document.querySelector('#webextras-clock-video');
      if(!v){mark('initial-create-before');v=document.createElement('video');v.id='webextras-clock-video';v.muted=true;v.playsInline=true;v.loop=false;v.src=${JSON.stringify(videoURL)};document.body.append(v);mark('initial-create-after');
        mark('initial-metadata-before');await new Promise((resolve,reject)=>{const t=setTimeout(()=>reject(Error('metadata timeout')),5000);v.onloadedmetadata=()=>{clearTimeout(t);resolve()};v.onerror=()=>reject(Error('video load error'))});mark('initial-metadata-after');
        mark('initial-play-before');await v.play();mark('initial-play-after');v.pause();events.push('initial-real-play-pause'); }
      mark('phase-pause-before');v.pause();v.playbackRate=1;mark('phase-pause-after');
      mark('clear-before');navigator.mediaSession.setPositionState({});mark('clear-after');
      mark('seek-before');v.currentTime=phase.seek;
      if(v.seeking)await new Promise((resolve,reject)=>{const t=setTimeout(()=>reject(Error('seek timeout')),5000);v.addEventListener('seeked',()=>{clearTimeout(t);resolve()},{once:true})});mark('seek-after');
      if(!phase.declared&&phase.rate){mark('phase-play-before');v.playbackRate=phase.rate;await v.play();mark('phase-play-after');}
      mark('playback-state-before');navigator.mediaSession.playbackState=phase.rate===0?'paused':'playing';mark('playback-state-after');
      const beforeReadPerformanceNow=performance.now();
      const observed={performanceNow:performance.now(),currentTime:v.currentTime,duration:v.duration,playbackRate:v.playbackRate,paused:v.paused,ended:v.ended,seeking:v.seeking};
      const input=phase.declared?{duration:20,position:7.25,playbackRate:1.5}:{duration:observed.duration,position:observed.currentTime,playbackRate:observed.playbackRate};
      mark('input-setter-before');const beforeSetPerformanceNow=performance.now();navigator.mediaSession.setPositionState(input);const afterSetPerformanceNow=performance.now();mark('input-setter-after');
      const receipt={phase:phase.name,source:phase.declared?'independent':'DOM-video',beforeReadPerformanceNow,observed,input,beforeSetPerformanceNow,afterSetPerformanceNow};
      window.__webextrasClockPhase=phase.name;window.__webextrasClockReceipt=receipt;mark('complete');
      return {phase:phase.name,events,progress,at:performance.now(),videoDuration:v.duration,receipt};
    }catch(error){const stage=window.__webextrasClockProgress?.stage;
      window.__webextrasClockFailure={name:typeof error?.name==='string'?error.name:'Error',message:String(error?.message??error),stack:typeof error?.stack==='string'?error.stack:null,stage};mark('failed');throw error;}
  })()`;
}
// Persist the complete Runtime.evaluate response before accepting/rejecting it.
// The caller owns the exact fixture expression and an append-only local sink.
async function evaluateMediaClock(send: (expression:string)=>Promise<any>, save:(record:any)=>void, context:{phase:string;label:string;expression:string}): Promise<any> {
  const hostBeforeNanos=process.hrtime.bigint().toString();
  let result:any;
  try { result=await send(context.expression); }
  catch(error) {
    save({...context,hostBeforeNanos,hostAfterNanos:process.hrtime.bigint().toString(),status:"transport-error",
      error:{name:error instanceof Error?error.name:"Error",message:String(error instanceof Error?error.message:error),stack:error instanceof Error?error.stack:null}});
    throw error;
  }
  save({...context,hostBeforeNanos,hostAfterNanos:process.hrtime.bigint().toString(),status:"response",resultPresent:result!==undefined,rawResult:result??null});
  assert(result && !result.exceptionDetails && result.result?.value!==undefined, `clock DOM evaluation failed (${context.label}); raw CDP response preserved`);
  return result.result.value;
}
function mediaClockFailureDOMJS(fixtureURL:string,videoURL:string):string {
  return `(()=>{
    if(location.href!==${JSON.stringify(fixtureURL)})return {wrongFixture:true};
    const safe=fn=>{try{return fn()}catch(e){return {readError:{name:String(e?.name??'Error'),message:String(e?.message??e)}}}};
    const v=safe(()=>document.querySelector('#webextras-clock-video'));
    const ownURL=value=>value===${JSON.stringify(videoURL)}?value:(value?'[off-fixture URL]':value);
    return {performanceNow:performance.now(),bodyPresent:!!document.body,navigatorPresent:typeof navigator!=='undefined',mediaSessionPresent:safe(()=>!!navigator.mediaSession),
      playbackState:safe(()=>navigator.mediaSession?.playbackState),visibilityState:document.visibilityState,hidden:document.hidden,secureContext:isSecureContext,
      progress:window.__webextrasClockProgress??null,failure:window.__webextrasClockFailure??null,
      video:v&&typeof v==='object'&&!v.readError?{src:ownURL(v.src),currentSrc:ownURL(v.currentSrc),error:safe(()=>v.error?{code:v.error.code,message:v.error.message}:null),
        readyState:v.readyState,networkState:v.networkState,duration:Number.isFinite(v.duration)?v.duration:String(v.duration),currentTime:v.currentTime,muted:v.muted,defaultMuted:v.defaultMuted,
        paused:v.paused,seeking:v.seeking,ended:v.ended,loop:v.loop,videoWidth:v.videoWidth,videoHeight:v.videoHeight,canPlayVP9:safe(()=>v.canPlayType('video/webm;codecs="vp9"'))}:v,
      resourceEntries:safe(()=>performance.getEntriesByName(${JSON.stringify(videoURL)}).slice(-16).map(e=>({name:ownURL(e.name),initiatorType:e.initiatorType,startTime:e.startTime,responseStart:e.responseStart,responseEnd:e.responseEnd,duration:e.duration,transferSize:e.transferSize,encodedBodySize:e.encodedBodySize,decodedBodySize:e.decodedBodySize,responseStatus:e.responseStatus}))) };
  })()`;
}

const mediaClockDOMJS = `(()=>{const v=document.querySelector('#webextras-clock-video');return v&&({phase:window.__webextrasClockPhase,performanceNow:performance.now(),visibilityState:document.visibilityState,hidden:document.hidden,currentTime:v.currentTime,duration:v.duration,paused:v.paused,ended:v.ended,loop:v.loop,playbackRate:v.playbackRate,readyState:v.readyState,seeking:v.seeking,playbackState:navigator.mediaSession.playbackState,positionReceipt:window.__webextrasClockReceipt})})()`;

// Genuine frontend diagnostics use their own zero-public callback spans, never the fixture comparator.
const frontendEventPolicy=Object.freeze({waitMilliseconds:5000,pollMilliseconds:100,quietMilliseconds:200,maxCandidates:8});
const frontendCallbackSelectors=["webContentsDevTools:displayAs:","webContentsDevTools:inspectModeDidChange:","webContentsDevToolsDidClose:","webContentsDevToolsNeedsFocus:"];
function genuineFrontendURL(value:any):boolean {
  if(typeof value!=="string")return false;
  try{const url=new URL(value);return ["devtools:","chrome-devtools:"].includes(url.protocol)&&url.hostname==="devtools"&&/\/(?:bundled\/)?(?:devtools_app|inspector|js_app)\.html$/.test(url.pathname);}catch{return false;}
}
function frontendCandidates(before:any[],after:any[]):any[]{
  const prior=new Set(before.map(info=>info.targetId));
  const candidates=after.filter(info=>typeof info.targetId==="string"&&!prior.has(info.targetId)&&genuineFrontendURL(info.url));
  assert(new Set(candidates.map(info=>info.targetId)).size===candidates.length,"duplicate frontend target identity");
  assert(candidates.length<=frontendEventPolicy.maxCandidates,"too many new frontend targets");return candidates;
}
function frontendProducerJS(fixtureURL:string,kind:"read"|"inspect-on"|"inspect-off"|"dock-change"|"dock-restore",side?:string,primaryID?:string):string {
  return `(async()=>{
    const SDK=await import(new URL('./core/sdk/sdk.js',location.href).href),UI=await import(new URL('./ui/legacy/legacy.js',location.href).href),Common=await import(new URL('./core/common/common.js',location.href).href);
    const manager=SDK.TargetManager.TargetManager.instance();
    const action=UI.ActionRegistry.ActionRegistry.instance().getAction('elements.toggle-element-search'),dock=UI.DockController.DockController.instance();
    const read=()=>{const primary=manager.primaryPageTarget();return ({primaryURLMatches:primary?.inspectedURL()===${JSON.stringify(fixtureURL)},primaryTargetID:primary?.id(),suspended:manager.allTargetsSuspended(),
      actionEnabled:action.enabled(),actionToggled:action.toggled(),canDock:dock.canDock(),dockSide:dock.dockSide(),
      uaShadow:Common.Settings.Settings.instance().moduleSetting('show-ua-shadow-dom').get(),
      performanceNow:performance.now()});};
    const expectedPrimaryID=${JSON.stringify(primaryID??null)};
    const before=read();if(!before.primaryURLMatches||(expectedPrimaryID&&before.primaryTargetID!==expectedPrimaryID))throw Error('frontend primary target is not exact owned nonce URL/identity');
    const kind=${JSON.stringify(kind)};let acknowledged=null;
    if(kind==='inspect-on'||kind==='inspect-off'){
      if(before.suspended||!before.actionEnabled||before.actionToggled!==(kind==='inspect-off'))throw Error('genuine inspect action not ready or prior state changed');
      acknowledged=await action.execute();
    }else if(kind==='dock-change'||kind==='dock-restore'){
      const side=${JSON.stringify(side??null)};
      if(!before.canDock||!['right','bottom','left','undocked'].includes(side)||before.dockSide===side)throw Error('genuine docking change unavailable or unchanged');
      dock.setDockSide(side);acknowledged=true;
    }
    const after=read();if(!after.primaryURLMatches||(expectedPrimaryID&&after.primaryTargetID!==expectedPrimaryID))throw Error('frontend primary target changed during producer');
    return {kind,before,after,acknowledged};
  })()`;
}
async function frontendRuntimeEvaluation(send:(expression:string)=>Promise<any>,save:(value:any)=>void,context:{label:string;expression:string;targetID?:string;sessionID?:string}):Promise<any>{
  const hostBeforeNanos=process.hrtime.bigint().toString();let rawResult:any;
  try{rawResult=await send(context.expression);}catch(error){save({...context,status:"transport-error",hostBeforeNanos,hostAfterNanos:process.hrtime.bigint().toString(),error:String(error)});throw error;}
  save({...context,status:"response",hostBeforeNanos,hostAfterNanos:process.hrtime.bigint().toString(),rawResult:rawResult??null});
  if(rawResult?.exceptionDetails&&JSON.stringify(rawResult.exceptionDetails).includes("frontend primary target"))throw new ClockProbeInconclusive("genuine frontend primary target identity changed; raw exception preserved");
  assert(rawResult&&!rawResult.exceptionDetails&&rawResult.result?.value!==undefined,"frontend Runtime.evaluate failed; exact raw result preserved");return rawResult.result.value;
}
function validateFrontendStimulusReceipt(span:any):void{
  const producer=span.producer,ack=span.afterAcknowledgment?.after,before=producer?.before,after=producer?.after;
  assert(producer?.kind===span.kind&&producer.acknowledged===true&&span.afterAcknowledgment?.kind==="read"&&before?.primaryURLMatches&&after?.primaryURLMatches&&ack?.primaryURLMatches&&
    typeof before.primaryTargetID==="string"&&before.primaryTargetID.length>0&&before.primaryTargetID===after.primaryTargetID&&after.primaryTargetID===ack.primaryTargetID,"frontend stimulus acknowledgment/primary owner receipt mismatch");
  if(span.kind==="inspect-on"||span.kind==="inspect-off"){
    const on=span.kind==="inspect-on";
    assert(before.suspended===false&&before.actionEnabled===true&&before.actionToggled===!on&&after.actionToggled===on&&ack.actionToggled===on&&typeof before.uaShadow==="boolean"&&
      span.expectedSelector==="webContentsDevTools:inspectModeDidChange:"&&span.expectedMode===(on?(before.uaShadow?2:1):0),"genuine inspect action was a no-op or expected mode was altered");
  }else{
    assert(["dock-change","dock-restore"].includes(span.kind)&&before.canDock===true&&before.dockSide!==after.dockSide&&after.dockSide===ack.dockSide&&
      ["right","bottom","left","undocked"].includes(after.dockSide)&&span.expectedSelector==="webContentsDevTools:displayAs:"&&
      span.expectedMode===({undocked:1,left:2,right:3,bottom:4} as any)[after.dockSide]&&
      after.dockSide===(span.kind==="dock-change"?(before.dockSide==="right"?"bottom":"right"):span.initialDockSide),"genuine docking change/restore receipt mismatch");
  }
}
function frontendCallbackMetadata(text:string):Set<string>{
  const known=new Set<string>();
  for(const line of text.split("\n")){
    const entry=line.trim().match(/^\{"([^"\s]+)", "([^"\s]+)"\},?$/);if(!entry)continue;
    const [,native,selector]=entry;known.add(JSON.stringify([native,selector]));
    if(!native.startsWith("_TtC"))continue;
    let rest=native.slice(native.startsWith("_TtCC")?5:4),parts:string[]=[];
    while(rest.length){const length=rest.match(/^([1-9]\d*)/);if(!length){parts=[];break;}
      const count=Number(length[1]);rest=rest.slice(length[1].length);
      if(!Number.isSafeInteger(count)||count>rest.length){parts=[];break;}parts.push(rest.slice(0,count));rest=rest.slice(count);
    }
    if(parts.length>=2)known.add(JSON.stringify([parts.join("."),selector]));
  }
  assert(known.size>0,"frozen callback metadata is empty");return known;
}
function ownedFrontendCallback(event:any):boolean{
  return event.cls==="ADK.WebContentChromium"&&frontendCallbackSelectors.includes(event.sel);
}
const frontendRecorderAliasSourceSHA256="9ae39687fdebcf4115b8c974af1b799aeee65265185a2c512ff56e5ca287d81e";
function validateFrontendCallbackIdentity(event:any):void{
  assert(frontendCallbackSelectors.includes(event.selector)&&event.selectorIdentityValid===true,"frontend callback selector identity invalid");
  const direct=event.rawSelector===event.selector&&event.selectorIdentityKind==="public"&&event.recorderAliasVerified===false;
  const alias=event.rawSelector==="__arcrec_"+event.selector&&event.selectorIdentityKind==="recorder-alias"&&event.recorderAliasVerified===true&&event.recorderAliasSourceSHA256===frontendRecorderAliasSourceSHA256;
  assert(direct||alias,"frontend callback raw selector is neither installed public selector nor verified recorder alias");
  const chain=event.ownerClassChain;
  assert(event.ownerIdentityValid===true&&event.ownerReportedClass==="ArcWebContents"&&event.ownerRecordClass==="ArcWebContents"&&Array.isArray(chain)&&
    chain.length>0&&chain.length<32&&chain.every((name:any)=>typeof name==="string"&&name.length>0)&&chain[0]===event.ownerClass&&new Set(chain).size===chain.length,"frontend callback owner identity/base chain invalid");
  const exact=event.ownerClass==="ArcWebContents"&&event.ownerIdentityKind==="exact";
  const kvo=event.ownerClass==="NSKVONotifying_ArcWebContents"&&event.ownerIdentityKind==="kvo"&&chain[1]==="ArcWebContents";
  assert(exact||kvo,"frontend callback owner runtime class is not exact owner or verified direct KVO subclass");
  assert(event.abi===(frontendCallbackSelectors.slice(0,2).includes(event.selector)?"v32@0:8@16Q24":"v24@0:8@16"),"frontend callback ABI mismatch");
}
function validateFrontendCallbackEvidence(span:any,record:Buffer,serviceReceiver:string,ownerTag:number,fixtureImage:string,projection?:PublicTraceProjection,knownCallbacks?:Set<string>):any[]{
  assert(knownCallbacks instanceof Set,"frozen callback metadata required for callback-only span");
  assert(span?.ok===true,"frontend callback observation request failed");
  assert(span.ownerTag===ownerTag,"frontend callback observation owner identity changed");
  assert(span.publicCalls?.length===0&&span.timeline?.length===0,"frontend callback observation performed public work");
  const observation=span.observation;
  assert(observation?.spanID===span.spanID&&observation.ownerTag===ownerTag&&observation.forwarding===0&&observation.overflow===false&&observation.ignoredServiceCallbacks===0,"incomplete/overflowed/foreign-service callback observation");
  const rows=parseRecords(validSpan(record,observation.recordBegin,observation.recordEnd),observation.recordBegin);
  const ambient=observation.recordBegin?scanRecords(parseRecords(record.subarray(0,observation.recordBegin))).stacks:new Map<string,RecordRow[]>();
  const matched=validateNesting(rows,ambient);
  assert(rows.every(row=>row.event.from&&typeof row.event.from==="string"),"callback caller label missing");
  assert(!rows.some(row=>row.event.dir==="->engine"&&row.event.from===fixtureImage),"callback-only span contains fixture public calls");
  const allDelegates=rows.filter(row=>row.event.ev==="call"&&row.event.dir==="engine->app");
  for(const entry of allDelegates){
    const event=entry.event;
    assert(event.from==="engine"&&event.kind==="-"&&typeof event.thread==="string"&&event.thread.length>0,"callback has invalid engine origin/thread/kind");
    assert(knownCallbacks.has(JSON.stringify([event.cls,event.sel])),"callback identity absent from frozen callback metadata");
    assert(event.args.length===(event.sel.match(/:/g)??[]).length,"callback argument count differs from frozen selector");
  }
  const delegates=allDelegates.filter(row=>ownedFrontendCallback(row.event));
  const ambientDelegates=allDelegates.filter(row=>!ownedFrontendCallback(row.event));
  span.rawCallbackCounts={total:allDelegates.length,owned:delegates.length,ambient:ambientDelegates.length};
  span.ambientCallbackIdentities=ambientDelegates.map(row=>({cls:row.event.cls,selector:row.event.sel,thread:row.event.thread}));
  const callbacks=observation.callbacks;
  assert(delegates.length>0,`no owned DevTools callback evidence: total=${allDelegates.length}, ambient=${ambientDelegates.length}`);
  assert(Array.isArray(callbacks)&&callbacks.length>0&&delegates.length===callbacks.length,"owned callback journal/raw count mismatch");
  for(let index=0;index<callbacks.length;++index){
    const event=callbacks[index],entry=delegates[index],returned=matched.get(entry.event.seq),modeful=frontendCallbackSelectors.slice(0,2).includes(entry.event.sel);
    validateFrontendCallbackIdentity(event);
    assert(frontendCallbackSelectors.includes(entry.event.sel)&&entry.event.cls===event.delegateClass&&entry.event.kind==="-"&&entry.event.thread==="main"&&entry.event.from==="engine"&&returned&&returned.event.ret===null,"callback selector/class/thread/caller/return mismatch");
    assert(event.selector===entry.event.sel&&event.mainThread===true&&event.forwarded===true&&event.ownerTag===ownerTag&&event.ownerRecordClass==="ArcWebContents"&&event.serviceClass==="ArcWebContentsDevTools"&&event.ordinal===index+1,"callback service/owner/forwarding association mismatch");
    assert(entry.event.args?.length===(modeful?2:1)&&entry.event.args[0]===serviceReceiver&&Number.isSafeInteger(event.mode)&&event.mode>=0&&event.mode<=4&&(modeful?entry.event.args[1]===event.mode:event.mode===0),"callback argument/service/mode mismatch");
    const begin=event.recordOffset,end=event.recordAfterForwarding;
    assert(Number.isSafeInteger(begin)&&Number.isSafeInteger(end)&&begin>=observation.recordBegin&&begin<=end&&end<=observation.recordEnd&&
      (begin===0||record[begin-1]===10)&&(end===0||record[end-1]===10),"callback forwarding offsets unaligned/outside span");
    assert((begin<=entry.begin&&returned!.end<=end)||(entry.end<=begin&&end<=returned!.begin),"callback recorder inner/outer forwarding bracket mismatch");
    assert(/^\d+$/.test(event.nativeBeforeTicks)&&/^\d+$/.test(event.nativeAfterTicks)&&BigInt(event.nativeAfterTicks)>=BigInt(event.nativeBeforeTicks),"callback monotonic bracket invalid");
    const ancestors=scanRecords(rows.filter(row=>row.end<=entry.begin),ambient).stacks.get("main")??[];
    assert(event.forwardDepth===1+ancestors.filter(row=>row.event.dir==="engine->app"&&frontendCallbackSelectors.includes(row.event.sel)&&row.event.args?.[0]===serviceReceiver).length,"callback forwarding reentry depth mismatch");
  }
  const expected=callbacks.filter((event:any)=>event.selector===span.expectedSelector);
  assert(expected.length>0&&expected.every((event:any)=>event.mode===span.expectedMode),"declared producer missing callback or produced wrong mode");
  if(projection)return projection.project(rows);
  return allDelegates.map(row=>({cls:row.event.cls,selector:row.event.sel,args:ownedFrontendCallback(row.event)?["<canonical-service>",...row.event.args.slice(1)]:row.event.args,
    ret:matched.get(row.event.seq)!.event.ret,thread:row.event.thread,from:row.event.from,scope:ownedFrontendCallback(row.event)?"owned":"ambient"}));
}
function validateFrontendCallbackSpan(span:any,record:Buffer,serviceReceiver:string,ownerTag:number,fixtureImage:string,projection?:PublicTraceProjection,knownCallbacks?:Set<string>):any[]{
  validateFrontendStimulusReceipt(span);
  assert(span.quietComplete===true,"frontend callback observation did not establish the fixed quiet interval");
  return validateFrontendCallbackEvidence(span,record,serviceReceiver,ownerTag,fixtureImage,projection,knownCallbacks);
}
function frontendJournalSignature(observation:any):string{
  return JSON.stringify(canonical({callbacks:observation?.callbacks,forwarding:observation?.forwarding,ignored:observation?.ignoredServiceCallbacks,overflow:observation?.overflow}));
}
async function runGenuineFrontendEvents(io:any,before:any[],fixtureURL:string,ownerTag:number):Promise<any>{
  const report:any={policy:frontendEventPolicy,status:"discovering",spans:[],censusBefore:before};io.save(report);
  const sessions=new Map<string,string>();let chosen:any,inspectNeedsRestore=false,dockNeedsRestore=false,initialSide:string|undefined;
  const evaluate=async(candidate:any,kind:any,side?:string)=>{
    io.verify();let info:any;try{info=await io.request("Target.getTargetInfo",{targetId:candidate.targetId});}catch(error){throw new ClockProbeInconclusive(`genuine frontend target unavailable: ${String(error)}`);}
    if(info.targetInfo?.targetId!==candidate.targetId||info.targetInfo?.url!==candidate.url||!genuineFrontendURL(info.targetInfo.url))throw new ClockProbeInconclusive("frontend target identity changed");
    let session=sessions.get(candidate.targetId);
    if(!session){const attached=await io.request("Target.attachToTarget",{targetId:candidate.targetId,flatten:true});assert(typeof attached.sessionId==="string"&&attached.sessionId.length>0,"missing frontend session");session=attached.sessionId;sessions.set(candidate.targetId,session);}
    return frontendRuntimeEvaluation(expression=>{io.verify();return io.request("Runtime.evaluate",{expression,returnByValue:true,awaitPromise:true},session);},io.raw,{label:kind,targetID:candidate.targetId,sessionID:session,expression:frontendProducerJS(fixtureURL,kind,side,report.preflight?.after.primaryTargetID)});
  };
  const observe=async(kind:string,expectedSelector:string,expectedMode:number,side?:string)=>{
    const spanID=io.spanID(kind),identity={spanID,ownerTag,observationNonce:new URL(fixtureURL).pathname.split("/")[1]},span:any={spanID,kind,expectedSelector,expectedMode,...(initialSide?{initialDockSide:initialSide}:{}),status:"arming"};report.spans.push(span);io.save(report);
    const arm=await io.action("frontend-observe-arm",identity);span.arm=arm;
    assert(arm.ok===true&&arm.ownerTag===ownerTag,"frontend callback observation did not arm on canonical owner");
    const started=io.now();let failure:any,stableJournalSignature:string|undefined;
    try{
      span.producer=await evaluate(chosen,kind,side);
      assert(span.producer.acknowledged===true,"genuine frontend action did not acknowledge");
      try{span.afterAcknowledgment=await evaluate(chosen,"read");}catch(error){throw new ClockProbeInconclusive(`frontend owner unverified after acknowledgment: ${String(error)}`);}
      if(kind==="inspect-on"){inspectNeedsRestore=span.producer.after.actionToggled===true;assert(inspectNeedsRestore,"inspect action was a no-op");}
      if(kind==="inspect-off"){assert(span.producer.after.actionToggled===false,"inspect restore was a no-op");inspectNeedsRestore=false;}
      if(kind==="dock-change"){dockNeedsRestore=span.producer.after.dockSide!==initialSide;assert(span.producer.after.dockSide===side,"dock action was a no-op");}
      if(kind==="dock-restore"){assert(span.producer.after.dockSide===side,"dock restore was a no-op");dockNeedsRestore=false;}
      let journalSignature:string|undefined,lastJournalChange=io.now(),lastObservation:any;
      while(io.now()-started<frontendEventPolicy.waitMilliseconds){
        const sample=await io.action("frontend-observe-read",identity),sampledAt=io.now();span.samples??=[];span.samples.push({response:sample,hostElapsedMilliseconds:sampledAt-started});io.save(report);
        assert(sample.ok===true,"frontend observation read failed");
        const observation=sample.observation;lastObservation=observation;
        assert(Array.isArray(observation?.callbacks),"frontend callback journal callback array missing");
        for(const event of observation?.callbacks??[])validateFrontendCallbackIdentity(event);
        if(sampledAt-started>frontendEventPolicy.waitMilliseconds||observation?.expired){span.waitExpired=true;break;}
        const signature=frontendJournalSignature(observation);
        if(signature!==journalSignature){journalSignature=signature;lastJournalChange=sampledAt;}
        if(observation?.callbacks?.some((event:any)=>event.selector===expectedSelector&&event.mode===expectedMode)&&observation.forwarding===0&&sampledAt-lastJournalChange>=frontendEventPolicy.quietMilliseconds){span.quietComplete=true;stableJournalSignature=signature;span.quietElapsedMilliseconds=sampledAt-started;break;}
        if(observation?.expired)break;
        await io.sleep(frontendEventPolicy.pollMilliseconds);
      }
      if(!span.quietComplete){
        const callbacks=lastObservation?.callbacks??[];
        if(!callbacks.length)throw new ClockProbeInconclusive("frontend callback observation timed out: no app callback observed");
        if(!callbacks.some((event:any)=>event.selector===expectedSelector))throw new ClockProbeInconclusive("frontend callback observation timed out: declared selector absent");
        if(!callbacks.some((event:any)=>event.selector===expectedSelector&&event.mode===expectedMode))throw new ClockProbeInconclusive("frontend callback observation timed out: declared mode absent");
        throw new ClockProbeInconclusive("frontend callback observation timed out: fixed quiet interval not established");
      }
    }catch(error){failure=error;}
    finally{
      span.disarm=await io.action("frontend-observe-disarm",identity);
      Object.assign(span,span.disarm,{spanID,kind,expectedSelector,expectedMode});
      if(span.quietComplete&&frontendJournalSignature(span.observation)!==stableJournalSignature){span.quietComplete=false;span.quietInvalidatedAtDisarm=true;failure??=new ClockProbeInconclusive("frontend journal changed between stable sample and disarm; extra events preserved");}
      io.save(report);
    }
    if(failure)throw failure;
    assert(span.ok===true&&span.observation?.callbacks?.some((event:any)=>event.selector===expectedSelector&&event.mode===expectedMode),"genuine frontend producer has no matching callback in bounded observation");
    span.status="observed";io.save(report);
  };
  try{
    const started=io.now();
    while(io.now()-started<frontendEventPolicy.waitMilliseconds){
      io.verify();const after=await io.census();report.censusAfter=after;
      const candidates=frontendCandidates(before,after);report.candidates=candidates;
      const matches:any[]=[];let unresolved=false;
      for(const candidate of candidates){try{const state=await evaluate(candidate,"read");matches.push({candidate,state});}catch(error){report.discoveryErrors??=[];report.discoveryErrors.push({targetID:candidate.targetId,error:String(error)});unresolved=true;}}
      io.save(report);
      assert(matches.length<=1,"multiple genuine frontends match exact owned primary target");
      if(matches.length===1&&!unresolved){chosen=matches[0].candidate;report.preflight=matches[0].state;break;}
      await io.sleep(frontendEventPolicy.pollMilliseconds);
    }
    if(!chosen)throw new ClockProbeInconclusive("unique genuine frontend for exact owned nonce target unavailable");
    report.target=chosen;report.status="exercising";io.save(report);
    const state=report.preflight.after;
    if(typeof state.primaryTargetID!=="string"||!state.primaryTargetID)throw new ClockProbeInconclusive("frontend primary target identity unavailable");
    if(!state.primaryURLMatches||state.suspended||!state.actionEnabled||state.actionToggled!==false)throw new ClockProbeInconclusive("inspect action precondition unavailable");
    // Restore even if producer response/callback is lost after it applied a mutation.
    inspectNeedsRestore=true;
    await observe("inspect-on","webContentsDevTools:inspectModeDidChange:",state.uaShadow?2:1);
    await observe("inspect-off","webContentsDevTools:inspectModeDidChange:",0);
    if(!state.canDock){report.dockStatus="unsupported-canDock-false";throw new ClockProbeInconclusive("genuine frontend does not permit docking; no forced docking stimulus");}
    initialSide=state.dockSide;assert(["right","bottom","left","undocked"].includes(initialSide!),"unknown initial dock side");
    const side=initialSide==="right"?"bottom":"right";dockNeedsRestore=true;
    await observe("dock-change","webContentsDevTools:displayAs:",side==="right"?3:4,side);
    await observe("dock-restore","webContentsDevTools:displayAs:",({undocked:1,left:2,right:3,bottom:4} as any)[initialSide!],initialSide);
    report.status="observed";
  }catch(error){report.status=isClockInconclusive(error)?"inconclusive":"failed";report.error=String(error);}
  finally{
    if(chosen&&(inspectNeedsRestore||dockNeedsRestore)){
      report.restoration=[];
      if(inspectNeedsRestore)try{const read=await evaluate(chosen,"read");if(read.after.actionToggled)report.restoration.push(await evaluate(chosen,"inspect-off"));}catch(error){report.restoration.push({error:String(error)});}
      if(dockNeedsRestore&&initialSide)try{const read=await evaluate(chosen,"read");if(read.after.dockSide!==initialSide)report.restoration.push(await evaluate(chosen,"dock-restore",initialSide));}catch(error){report.restoration.push({error:String(error)});}
    }
    for(const sessionId of sessions.values())try{io.verify();await io.request("Target.detachFromTarget",{sessionId});}catch(error){report.detachErrors??=[];report.detachErrors.push(String(error));}
    if(report.detachErrors?.length){report.status="failed";report.error="frontend session detach failed";}
    io.save(report);
  }
  return report;
}

const root = resolve(import.meta.dir, "../..");
const args = process.argv.slice(2);
const option = (key: string, fallback?: string): string | undefined => {
  const index = args.indexOf(key);
  return index < 0 ? fallback : args[index + 1];
};
if (!option("--out")) throw new Error("usage: webextras_native_probe.sh run --out DIR [--agents trace156,ours156] [--port 9458]");
const out = resolve(option("--out")!);
const probeMode=clockModeFromArgs(args);
const mediaClock=probeMode==="media-clock",mediaServiceClock=probeMode==="media-service-clock",anyClock=mediaClock||mediaServiceClock;
const devToolsFrontendEvents=probeMode==="devtools-frontend-events";
const agents = option("--agents", "trace156,ours156")!.split(",");
if (!agents.length || new Set(agents).size !== agents.length || agents.some(agent => !["trace156", "ours156"].includes(agent))) {
  throw new Error("only trace156 and ours156, each at most once, are allowed");
}
const port = Number(option("--port", "9458"));
if (!Number.isInteger(port) || port < 1024 || port > 65535) throw new Error("invalid CDP port");
const exec = (argv: string[]) => {
  const result = Bun.spawnSync(argv, { stdout: "pipe", stderr: "pipe" });
  if (!result.success) throw new Error(`${argv[0]} failed: ${result.stderr.toString().trim()}`);
  return result.stdout.toString().trim();
};
// One immutable recorder image is shared by both agents, even if the shared source is rebuilt.
function recorderFileIdentity(path: string): any {
  const realpath = realpathSync(path);
  const before = statSync(realpath);
  const sha256 = exec(["/usr/bin/shasum", "-a", "256", realpath]).match(/^([0-9a-f]{64})\s/)?.[1];
  const after = statSync(realpath);
  assert(sha256 && realpathSync(path) === realpath && before.dev === after.dev && before.ino === after.ino &&
    before.size === after.size && before.mtimeMs === after.mtimeMs && before.ctimeMs === after.ctimeMs,
    "recorder changed while capturing identity");
  return { path, realpath, sha256, size: after.size, mtimeMs: after.mtimeMs, mtime: after.mtime.toISOString(),
    dev: after.dev, ino: after.ino, ctimeMs: after.ctimeMs };
}
function freezeRecorder(sourcePath: string, outputRoot: string): any {
  mkdirSync(outputRoot, { recursive: true });
  const source = recorderFileIdentity(sourcePath);
  const path = join(outputRoot, "ArcEngineRecorder.dylib");
  copyFileSync(source.realpath, path, constants.COPYFILE_EXCL);
  chmodSync(path, 0o444);
  const frozen = recorderFileIdentity(path);
  const after = recorderFileIdentity(sourcePath);
  assert(JSON.stringify(source) === JSON.stringify(after) && frozen.sha256 === source.sha256 && frozen.size === source.size,
    "recorder source changed while freezing image");
  const identity = { capturedAt: new Date().toISOString(), originalSourcePath: sourcePath, source, frozen };
  writeFileSync(join(outputRoot, "recorder-identity.json"), JSON.stringify(identity, null, 2));
  return identity;
}
function freezeFrontendRecorderPolicy(sourcePath:string,outputRoot:string):any{
  const source=recorderFileIdentity(sourcePath);
  assert(source.sha256===frontendRecorderAliasSourceSHA256,`frontend recorder policy source hash mismatch: expected ${frontendRecorderAliasSourceSHA256}, observed ${source.sha256}`);
  const path=join(outputRoot,"frontend-recorder-policy.m");copyFileSync(source.realpath,path,constants.COPYFILE_EXCL);chmodSync(path,0o444);
  const frozen=recorderFileIdentity(path);
  assert(frozen.sha256===source.sha256&&frozen.size===source.size&&JSON.stringify(recorderFileIdentity(sourcePath))===JSON.stringify(source),"frontend recorder policy source changed while freezing");
  const identity={capturedAt:new Date().toISOString(),policySourceSHA256:frontendRecorderAliasSourceSHA256,source,frozen,
    provenance:"Pinned audit source policy only; no source-to-recorder-dylib build provenance claim. Exact alias IMP/ABI is separately verified at callback receipt."};
  writeFileSync(join(outputRoot,"frontend-recorder-policy.json"),JSON.stringify(identity,null,2),{flag:"wx"});return identity;
}
function verifyFrontendRecorderPolicy(identity:any,phase:string):void{
  assert(identity.policySourceSHA256===frontendRecorderAliasSourceSHA256&&identity.source.sha256===frontendRecorderAliasSourceSHA256&&identity.frozen.sha256===frontendRecorderAliasSourceSHA256,"frontend recorder policy source pin invalid");
  assert(JSON.stringify(recorderFileIdentity(identity.source.path))===JSON.stringify(identity.source),`frontend recorder policy source changed ${phase}`);
  assert(JSON.stringify(recorderFileIdentity(identity.frozen.path))===JSON.stringify(identity.frozen),`frozen frontend recorder policy source changed ${phase}`);
}
function hasCallerLabels(rows: RecordRow[]): boolean {
  return rows.some(row => Object.hasOwn(row.event, "from"));
}
const fixtureComparisonPending = "pending fixture-comparison support: recorder caller labels exist, but shared diff.py has no verified explicit fixture-image option; its default diff drops probe calls";
// BEGIN WEBEXTRAS_RESOURCE_JOURNAL
function resourceCommand(arguments_:string[]):string {
  assert(JSON.stringify(recorderFileIdentity(resourceCheckerIdentity.source.path))===JSON.stringify(resourceCheckerIdentity.source),"resource checker source changed");
  assert(JSON.stringify(recorderFileIdentity(resourceCheckerIdentity.frozen.path))===JSON.stringify(resourceCheckerIdentity.frozen),"frozen resource checker changed");
  return exec(["python3","-B",resourceCheckerIdentity.frozen.path,...arguments_]);
}
function captureResourceIdentity(app:string,agent:string):any {
  const resources=JSON.parse(resourceCommand(["capture",agent,join(app,"Contents/Frameworks/ArcCore.framework/Versions/A/Resources")]));
  if(agent==="ours156"){
    assert(JSON.stringify(recorderFileIdentity(resourcePackageProofIdentity.path))===JSON.stringify(resourcePackageProofIdentity),"resource package proof changed");
    const proof=JSON.parse(resourceCommand(["verify-proof",resourcePackageProofIdentity.path]));
    assert(JSON.stringify(resources)===JSON.stringify(proof.packaged),"ours launch resources differ from held-slot package proof");
    assert(JSON.stringify(recorderFileIdentity(resourcePackageProofIdentity.path))===JSON.stringify(resourcePackageProofIdentity),"resource package proof changed while verifying");
  }
  return resources;
}
function verifyResourceJournal(run:any,dir:string):void {
  if(!run.binaryIdentity?.resources)return;
  try{
    const journalPath=join(dir,"binary-identity.json");
    assert(JSON.stringify(JSON.parse(readFileSync(journalPath,"utf8")).resources)===JSON.stringify(run.binaryIdentity.resources),"resource journal differs from captured launch identity");
    run.resourcesAfterCleanup=JSON.parse(resourceCommand(["verify-journal",journalPath]));
    assert(JSON.stringify(JSON.parse(readFileSync(journalPath,"utf8")).resources)===JSON.stringify(run.binaryIdentity.resources),"resource journal changed while verifying");
    writeFileSync(join(dir,"resources-after-cleanup.json"),JSON.stringify(run.resourcesAfterCleanup,null,2),{flag:"wx"});
    run.resourceIdentityStatus="verified-unchanged";
  }catch(error){run.resourceIdentityStatus="changed-or-unverifiable";run.errors.push(`resource identity after cleanup: ${String(error)}`);}
}
// END WEBEXTRAS_RESOURCE_JOURNAL

// The packaged app can be replaced by another worker while this run waits for the lock.
// Hash the actual launch files under the lock; a queued build timestamp proves no source provenance.
function captureBinaryIdentity(app: string, agent: string): any {
  const capturedAt = new Date().toISOString();
  const cache = new Map<string, any>();
  const identity = (path: string): any => {
    const realpath = realpathSync(path);
    if (cache.has(realpath)) return { ...cache.get(realpath), path };
    const before = statSync(realpath);
    const output = exec(["/usr/bin/shasum", "-a", "256", realpath]);
    const sha256 = output.match(/^([0-9a-f]{64})\s/)?.[1];
    const after = statSync(realpath);
    assert(sha256 && realpathSync(path) === realpath && before.dev === after.dev && before.ino === after.ino &&
      before.size === after.size && before.mtimeMs === after.mtimeMs, "binary changed while capturing identity");
    const value = { path, realpath, sha256, size: after.size, mtimeMs: after.mtimeMs, mtime: after.mtime.toISOString() };
    cache.set(realpath, value);
    return value;
  };
  const framework = join(app, "Contents/Frameworks/ArcCore.framework");
  const engine = identity(join(framework, "Versions/A/ArcCore"));
  const helpersPath = realpathSync(join(framework, "Helpers"));
  const helpers = readdirSync(helpersPath, { withFileTypes: true }).filter(entry => entry.isDirectory() && entry.name.endsWith(".app")).map(entry => {
    const bundle = join(helpersPath, entry.name);
    const name = exec(["/usr/libexec/PlistBuddy", "-c", "Print :CFBundleExecutable", join(bundle, "Contents/Info.plist")]);
    const executable = join(bundle, "Contents/MacOS", name);
    // Ordinary helpers dlopen a relative path; Aperitif variants use @executable_path.
    // Stream stdin because macOS strings treats parenthesized fat-binary paths as archive members.
    const scanned = Bun.spawnSync(["/usr/bin/strings", "-a", "-"], { stdin: Bun.file(executable), stdout: "pipe", stderr: "pipe" });
    assert(scanned.success, `helper engine path scan failed: ${entry.name}`);
    const relativeEngines = [...new Set(scanned.stdout.toString().split("\n").filter(line => /^(?:@executable_path\/)?(?:\.\.\/)+ArcCore$/.test(line)))];
    assert(relativeEngines.length === 1, `unrecognized helper engine path: ${entry.name}`);
    const relativePath = relativeEngines[0]!.replace(/^@executable_path\//, "");
    return { bundle, executable: identity(executable), engineRelativePath: relativeEngines[0],
      engine: identity(resolve(dirname(executable), relativePath)) };
  });
  assert(helpers.length > 0, "no packaged helper executables identified");
  return { agent, app, capturedAt, sourceSnapshot: "unknown", provenance: "Packaged launch-file identity only; source correspondence requires build/package/run in one lock slot.",
    appExecutable: identity(join(app, "Contents/MacOS/Arc")), engine, helpers,
    resources:captureResourceIdentity(app,agent),resourceCheckerIdentity,
    ...(agent==="ours156"?{resourcePackageProof:resourcePackageProofIdentity}:{}) };
}
const lockPID = Number(readFileSync(join(root, "logs/arccore-build.lock/pid"), "utf8").trim());
let ancestor = process.pid;
for (let depth = 0; ancestor !== lockPID && ancestor > 1 && depth < 20; ++depth) {
  ancestor = Number(exec(["ps", "-p", String(ancestor), "-o", "ppid="]));
}
if (ancestor !== lockPID) throw new Error("run under arc/tools/locked.sh; this driver never acquires or bypasses the lock");
const resourceCheckerSource=join(root,"arc/browser/webextras_resource_identity.py");
const resourceCheckerSourceIdentity=recorderFileIdentity(resourceCheckerSource);
const resourceCheckerFrozenPath=join(out,"resource-identity-checker.py");
mkdirSync(out,{recursive:true});copyFileSync(resourceCheckerSourceIdentity.realpath,resourceCheckerFrozenPath,constants.COPYFILE_EXCL);chmodSync(resourceCheckerFrozenPath,0o444);
const resourceCheckerIdentity={source:resourceCheckerSourceIdentity,frozen:recorderFileIdentity(resourceCheckerFrozenPath)};
assert(resourceCheckerIdentity.source.sha256===resourceCheckerIdentity.frozen.sha256&&JSON.stringify(recorderFileIdentity(resourceCheckerSource))===JSON.stringify(resourceCheckerSourceIdentity),"resource checker changed while freezing");
const resourcePackageProofPath=process.env.ARC_WEBEXTRAS_RESOURCE_PACKAGE_PROOF;
assert(typeof resourcePackageProofPath==="string"&&resourcePackageProofPath.length>0,"resource-aware driver requires held-slot package proof");
const resourcePackageProofIdentity=recorderFileIdentity(resourcePackageProofPath);
const dylib = join(root, "logs/wp/webextras-native-resources-probe-build/WebextrasNativeProbe.dylib");
const recorderSource = join(root, "tests/recorder/ArcEngineRecorder.dylib");
for (const file of [dylib, recorderSource]) if (!existsSync(file)) throw new Error(`missing ${file}`);
const recorderIdentity = freezeRecorder(recorderSource, out);
const recorder = recorderIdentity.frozen.path;
const frontendRecorderPolicy=devToolsFrontendEvents?freezeFrontendRecorderPolicy(join(root,"tests/recorder/ArcEngineRecorder.m"),out):undefined;
let mediaClockIdentity: any;
let mediaClockBytes: Buffer | undefined;
if (mediaClock) {
  const fixtureSource = join(root, "arc/browser/webextras_media_clock_fixture.webm");
  const provenanceSource = join(root, "arc/browser/webextras_media_clock_fixture.json");
  const source = recorderFileIdentity(fixtureSource);
  assert(source.sha256 === mediaClockPolicy.fixtureSHA256, "media clock fixture hash differs from fixed policy");
  const destination = join(out, "webextras-media-clock.webm");
  copyFileSync(source.realpath, destination, constants.COPYFILE_EXCL); chmodSync(destination, 0o444);
  const frozen = recorderFileIdentity(destination);
  assert(frozen.sha256 === source.sha256 && frozen.size === source.size && JSON.stringify(recorderFileIdentity(fixtureSource)) === JSON.stringify(source), "media clock fixture changed while freezing");
  const provenance = readFileSync(provenanceSource);
  writeFileSync(join(out, "webextras-media-clock-provenance.json"), provenance, { flag: "wx", mode: 0o444 });
  const provenanceFrozen = recorderFileIdentity(join(out, "webextras-media-clock-provenance.json"));
  const details = JSON.parse(provenance.toString());
  assert(details.sha256 === frozen.sha256 && details.bytes === frozen.size && details.ffprobe?.streams?.length === 1 &&
    details.ffprobe.streams[0].codec_type === "video" && Number(details.ffprobe.format.duration) > 6, "media clock provenance does not describe the fixed finite video-only asset");
  mediaClockIdentity = { source, frozen, provenanceIdentity:provenanceFrozen, provenance:details, policy: mediaClockPolicy };
  mediaClockBytes = readFileSync(destination);
  assert(new Bun.CryptoHasher("sha256").update(mediaClockBytes).digest("hex") === mediaClockPolicy.fixtureSHA256, "served media clock bytes differ from frozen fixture");
  writeFileSync(join(out, "media-clock-identity.json"), JSON.stringify(mediaClockIdentity, null, 2), { flag: "wx" });
}
const fixtureImage = "WebextrasNativeProbe.dylib";
const comparisonSnapshot = join(out, "fixture-comparator");
const comparisonInputs = JSON.parse(exec(["python3", "-B", join(root, "arc/browser/webextras_fixture_inputs.py"),
  "freeze", root, comparisonSnapshot, fixtureImage]));
const comparisonHelper = join(comparisonSnapshot, "arc/browser/webextras_fixture_inputs.py");
const comparisonMetadata = readFileSync(join(comparisonSnapshot, "arc/tools/arccore-1.170.0-objc.tsv"), "utf8");
const frontendKnownCallbacks=devToolsFrontendEvents?frontendCallbackMetadata(readFileSync(join(comparisonSnapshot,"tests/recorder/app_callbacks.inc"),"utf8")):undefined;
const fixtureVerified = comparisonInputs.capability?.verified === true;
let frontendComparison:any,frontendComparisonHelper:string|undefined;
if(devToolsFrontendEvents){
  const source=recorderFileIdentity(join(root,"arc/browser/webextras_frontend_comparison.py"));
  frontendComparisonHelper=join(out,"frontend-comparison.py");copyFileSync(source.realpath,frontendComparisonHelper,constants.COPYFILE_EXCL);chmodSync(frontendComparisonHelper,0o444);
  const frozen=recorderFileIdentity(frontendComparisonHelper);
  assert(source.sha256===frozen.sha256&&source.size===frozen.size&&JSON.stringify(recorderFileIdentity(source.path))===JSON.stringify(source),"frontend comparison helper changed while freezing");
  const invocation=Bun.spawnSync(["python3","-B",frontendComparisonHelper,"capability",comparisonSnapshot,fixtureImage],{stdout:"pipe",stderr:"pipe"});
  let capability:any;
  try{assert(invocation.success,"frontend capability helper failed");capability=JSON.parse(invocation.stdout.toString());}
  catch(error){capability={verified:false,status:"pending-callback-boundary-comparison-support",error:String(error)};}
  frontendComparison={source,frozen,capability,invocation:{exitCode:invocation.exitCode,stdout:invocation.stdout.toString(),stderr:invocation.stderr.toString()}};writeFileSync(join(out,"frontend-comparison-capability.json"),JSON.stringify(frontendComparison,null,2),{flag:"wx"});
}

const sleep = (ms: number) => new Promise(resolve => setTimeout(resolve, ms));
let observeProcesses: (() => void) | undefined;
const wait = async <T>(read: () => T | Promise<T>, timeout = 20000): Promise<NonNullable<T>> => {
  const end = Date.now() + timeout;
  let lastError: unknown;
  while (Date.now() < end) {
    try { observeProcesses?.(); const result = await read(); if (result) return result as NonNullable<T>; }
    catch (error) { lastError = error; }
    await sleep(150);
  }
  throw new Error(`timeout after ${timeout}ms${lastError ? `: ${String(lastError)}` : ""}`);
};
const json = (path: string): any => JSON.parse(readFileSync(path, "utf8"));
type ProcessIdentity = { pid: number; ppid: number; born: string; command: string };
const processSnapshot = (): ProcessIdentity[] => exec(["ps", "-axo", "pid=,ppid=,lstart=,command="]).split("\n").flatMap(line => {
  const match = line.match(/^\s*(\d+)\s+(\d+)\s+(\S+\s+\S+\s+\d+\s+\d{2}:\d{2}:\d{2}\s+\d{4})\s+(.+)$/);
  return match ? [{ pid: Number(match[1]), ppid: Number(match[2]), born: match[3]!, command: match[4]! }] : [];
});
const isExecutable = (row: ProcessIdentity, executable: string) => row.command === executable || row.command.startsWith(executable + " ");
const appPIDs = (exe: string): number[] => processSnapshot().filter(row => isExecutable(row, exe)).map(row => row.pid);
const sameProcess = (left: ProcessIdentity, right: ProcessIdentity) => left.pid === right.pid && left.born === right.born && left.command === right.command;
const token = `arc-webextras-probe-${crypto.randomUUID()}`;
const fixturePath = `/${token}/fixture`;
let mediaClockHTTPAgent:string|undefined;
const mediaClockHTTP:any[]=[];
let mediaClockHTTPTruncated=false;
function clockHTTP(request:Request,resource:string,response:Response,bytes:number):Response {
  if(mediaClock){
    if(mediaClockHTTP.length<256)mediaClockHTTP.push({agent:mediaClockHTTPAgent??null,hostNanos:process.hrtime.bigint().toString(),resource,
      method:["GET","HEAD"].includes(request.method)?request.method:"[other]",range:/^bytes=\d+-\d*$/.test(request.headers.get("range")??"")?request.headers.get("range"):request.headers.has("range")?"[invalid]":null,status:response.status,bytes});
    else mediaClockHTTPTruncated=true;
    writeFileSync(join(out,"media-clock-http.json"),JSON.stringify({limit:256,truncated:mediaClockHTTPTruncated,requests:mediaClockHTTP},null,2));
  }
  return response;
}
const server = Bun.serve({ hostname: "127.0.0.1", port: 0, fetch(request) {
  if (mediaClockBytes && new URL(request.url).pathname === `/${token}/clock.webm`) {
    if (!["GET", "HEAD"].includes(request.method)) return clockHTTP(request,"video",new Response(null, { status: 405 }),0);
    const bytes = mediaClockBytes, range = request.headers.get("range");
    let begin = 0, end = bytes.length - 1;
    if (range) {
      const match = /^bytes=(\d+)-(\d*)$/.exec(range);
      if (!match) return clockHTTP(request,"video",new Response(null, { status: 416, headers: { "content-range": `bytes */${bytes.length}` } }),0);
      begin = Number(match[1]); end = match[2] ? Math.min(Number(match[2]), end) : end;
      if (!Number.isSafeInteger(begin) || !Number.isSafeInteger(end) || begin < 0 || begin > end || begin >= bytes.length) return clockHTTP(request,"video",new Response(null, { status: 416, headers: { "content-range": `bytes */${bytes.length}` } }),0);
    }
    const headers: Record<string,string> = { "content-type":"video/webm", "accept-ranges":"bytes", "cache-control":"no-store", "content-length":String(end - begin + 1) };
    if (range) headers["content-range"] = `bytes ${begin}-${end}/${bytes.length}`;
    return clockHTTP(request,"video",new Response(request.method === "HEAD" ? null : bytes.subarray(begin, end + 1), { status:range ? 206 : 200, headers }),request.method === "HEAD" ? 0 : end-begin+1);
  }
  if (new URL(request.url).pathname !== fixturePath) return new Response("not found", { status: 404 });
  const fixtureBody=`<!doctype html><meta charset="utf-8"><title>Webextras native boundary fixture</title>
    <main id="webextras-probe" data-nonce="${token}">Local inert boundary fixture</main>`;
  const response=new Response(fixtureBody,{ headers: { "content-type": "text/html; charset=utf-8", "cache-control": "no-store" } });
  return clockHTTP(request,"fixture",response,request.method==="HEAD"?0:Buffer.byteLength(fixtureBody));
} });
const fixtureURL = `http://127.0.0.1:${server.port}${fixturePath}`;
mkdirSync(out, { recursive: true });
type Run = { agent: string; probeMode?:string; pid?: number; phase?: string; failure?: { phase?: string; message: string }; binaryIdentity?: any; recorderIdentity?: any; comparisonInputs?: any; fixturePublicCounts?: any; recordCallerLabels?: boolean; fixtureComparisonStatus?: string; actions: any[]; errors: string[]; cleanup?: string; runtimeStatus?:any; frontendEvents?:any;frontendRecorderPolicy?:any;frontendRecorderPolicyStatus?:string };
const runs: Run[] = [];
const operations = ["console-open", "frontend", "console-close", "panel-closed", "execute-evaluate", "execute-malformed", "execute-unknown", "subscription", "media-types", "pip-request-error", "pip-exit-error"];
const actionManifest = operations.map(operation => ({ key: operation, operation })).concat(anyClock ? (mediaServiceClock ? mediaServiceClockPhases : mediaClockPhases).flatMap(phase => Array.from({length:5}, (_, index) => ({key:clockActionKey(phase.name,index), operation:index < 3 ? "clock-snapshot" : "clock-held-time"}))) : []);
const expectedPairCount = actionManifest.reduce((count, entry) => count + expectedSelectors(entry.operation).length, 0);
try {
  for (const agent of agents) {
    const run: Run = { agent, ...((anyClock||devToolsFrontendEvents)?{probeMode}:{}), phase: "preflight", recorderIdentity, comparisonInputs, ...(devToolsFrontendEvents?{frontendRecorderPolicy}:{}), actions: [], errors: [] };
    if(devToolsFrontendEvents&&frontendComparison?.capability?.verified!==true)run.errors.push("pending-callback-boundary-comparison-support: explicit argument option/default policy canaries not verified");
    runs.push(run);
    const dir = join(out, agent);
    mkdirSync(dir, { recursive: true });
    if(mediaClock)mediaClockHTTPAgent=agent;
    const app = resolve(root, `../arc/build/ArcModAgent-${agent}.app`);
    const exe = join(app, "Contents/MacOS/Arc");
    const plist = (key: string) => exec(["/usr/libexec/PlistBuddy", "-c", `Print :${key}`, join(app, "Contents/Info.plist")]);
    const requestPath = join(dir, "webextras-request.json");
    const recordPath = join(dir, "record.jsonl");
    const data = join(process.env.HOME!, "Library/Application Support", `ArcModAgent-${agent}`);
    const nativeReadyPath = join(data, "ArcModAgentReady.json");
    const readySnapshot = (path: string): any => {
      try { return { exists: true, mtimeMs: statSync(path).mtimeMs, record: json(path) }; }
      catch (error) { return { exists: existsSync(path), error: String(error) }; }
    };
    let readiness: any;

    let pid: number | undefined;
    let cdp: AgentCDP | undefined;
    let target: string | undefined;
    let browserIdentity: ProcessIdentity | undefined;
    const ownedHelpers = new Map<number, ProcessIdentity>();
    const frameworkPrefix = join(app, "Contents/Frameworks") + "/";
    const trackHelpers = () => {
      if (!browserIdentity) return;
      const rows = processSnapshot();
      const current = rows.find(row => row.pid === browserIdentity!.pid);
      if (!current || !sameProcess(current, browserIdentity)) return;
      const descendants = new Set([browserIdentity.pid]);
      for (let changed = true; changed;) {
        changed = false;
        for (const row of rows) if (descendants.has(row.ppid) && !descendants.has(row.pid)) { descendants.add(row.pid); changed = true; }
      }
      for (const row of rows) if (row.pid !== browserIdentity.pid && descendants.has(row.pid) && row.command.startsWith(frameworkPrefix)) ownedHelpers.set(row.pid, row);
    };
    const ownedGone = () => {
      const rows = processSnapshot();
      const captured = [...(browserIdentity ? [browserIdentity] : []), ...ownedHelpers.values()];
      return captured.every(old => !rows.some(row => sameProcess(row, old)));
    };
    const terminateCaptured = (identities: ProcessIdentity[]) => {
      const rows = processSnapshot();
      for (const identity of identities) {
        if (rows.some(row => sameProcess(row, identity))) Bun.spawnSync(["kill", "-TERM", String(identity.pid)]);
      }
    };
    try {
      if (existsSync(recordPath) || existsSync(requestPath + ".ready.json")) throw new Error("output contains a prior run; choose a fresh --out directory");
      if (plist("ArcModAgentSession") !== agent || plist("CFBundleIdentifier") !== `company.thebrowser.Browser.mod.agent.${agent}` ||
          plist("CFBundleName") !== `ArcModAgent-${agent}`) throw new Error("agent bundle identity mismatch");
      if (processSnapshot().some(row => isExecutable(row, exe) || row.command.startsWith(frameworkPrefix))) throw new Error("agent browser or bundled helpers already running; refusing to attach or terminate them");
      if (Bun.spawnSync(["/usr/sbin/lsof", "-nP", `-iTCP:${port}`, "-sTCP:LISTEN"], { stdout: "ignore", stderr: "ignore" }).success) {
        throw new Error("CDP port occupied");
      }
      const chromiumData = join(data, "Agent Chromium Data");
      mkdirSync(chromiumData, { recursive: true, mode: 0o700 });
      for (const key of ["cloudKitSyncEnabled", "firebaseSyncEnabled"]) exec(["defaults", "write", `company.thebrowser.Browser.mod.agent.${agent}`, key, "-bool", "false"]);
      run.phase = "binary-identity";
      run.binaryIdentity = {...captureBinaryIdentity(app, agent),...(devToolsFrontendEvents?{frontendRecorderPolicy}: {})};
      writeFileSync(join(dir, "binary-identity.json"), JSON.stringify(run.binaryIdentity, null, 2));
      run.phase = "recorder-identity";
      assert(JSON.stringify(recorderFileIdentity(recorder)) === JSON.stringify(recorderIdentity.frozen), "frozen recorder changed before launch");
      exec(["python3", "-B", comparisonHelper, "verify", comparisonSnapshot]);
      if(devToolsFrontendEvents){
        verifyFrontendRecorderPolicy(frontendRecorderPolicy,"before launch");
        assert(JSON.stringify(recorderFileIdentity(frontendComparison.frozen.path))===JSON.stringify(frontendComparison.frozen),"frozen frontend comparison helper changed before launch");
      }
      if (mediaClock) {
        assert(JSON.stringify(recorderFileIdentity(mediaClockIdentity.frozen.path)) === JSON.stringify(mediaClockIdentity.frozen), "frozen media clock fixture changed before launch");
        assert(JSON.stringify(recorderFileIdentity(mediaClockIdentity.provenanceIdentity.path)) === JSON.stringify(mediaClockIdentity.provenanceIdentity), "frozen media clock provenance changed before launch");
      }
      run.phase = "launch";
      readiness = { agent, beforeLaunch: readySnapshot(nativeReadyPath), launchStartedMs: Date.now() };
      writeFileSync(join(dir, "readiness.json"), JSON.stringify(readiness, null, 2));
      exec(["open", "-n", "-g", "-a", app,
        "--env", `DYLD_INSERT_LIBRARIES=${dylib}:${recorder}`,
        "--env", `ARC_WEBEXTRAS_PROBE_REQUEST=${requestPath}`,
        "--env", `ARC_WEBEXTRAS_PROBE_URL=${fixtureURL}`,
        "--env", `ARC_RECORD_PATH=${recordPath}`,
        ...(anyClock ? ["--env", "ARC_WEBEXTRAS_MEDIA_CLOCK=1"] : []),
        ...(devToolsFrontendEvents ? ["--env", "ARC_WEBEXTRAS_FRONTEND_EVENTS=1"] : []),
        "--stdout", join(dir, "app.log"), "--stderr", join(dir, "app.log"), "--args",
        "--use-mock-keychain", "--no-default-browser-check", "--disable-sync", "--disable-background-networking",
        `--host-resolver-rules=${plist("ArcModHostResolverRules")}`, `--user-data-dir=${chromiumData}`,
        "--remote-debugging-address=127.0.0.1", `--remote-debugging-port=${port}`]);
      run.phase = "process-identity";
      pid = await wait(() => appPIDs(exe)[0], 30000);
      run.pid = pid;
      browserIdentity = processSnapshot().find(row => row.pid === pid && isExecutable(row, exe));
      if (!browserIdentity) throw new Error("launched browser identity unavailable");
      observeProcesses = trackHelpers;
      trackHelpers();
      run.phase = "native-readiness";
      await wait(() => {
        const injected = readySnapshot(requestPath + ".ready.json");
        const native = readySnapshot(nativeReadyPath);
        const fresh = native.exists === true && native.mtimeMs >= readiness.launchStartedMs &&
          (readiness.beforeLaunch.exists !== true || native.mtimeMs !== readiness.beforeLaunch.mtimeMs);
        const nativeMatches = native.record?.pid === pid && native.record?.agent === agent && native.record?.ready === true;
        const injectedMatches = injected.record?.installed === true && injected.record?.mainThread === true && injected.record?.pid === pid;
        readiness = { ...readiness, expectedPID: pid, injected, native, nativeFresh: fresh,
          nativeMatches, injectedMatches, accepted: fresh && nativeMatches && injectedMatches && existsSync(recordPath) };
        writeFileSync(join(dir, "readiness.json"), JSON.stringify(readiness, null, 2));
        return readiness.accepted;
      }, 30000);
      run.phase = "cdp-endpoint";
      await wait(() => verifyAgentEndpoint(agent, port) === pid, 30000);
      run.phase = "cdp-connect";
      cdp = await AgentCDP.connectAgent(agent, port);
      run.phase = "cdp-create-fixture";
      // Native and hook readiness precede creation, so navigation's setUrl: is observable.
      const created = await cdp.request("Target.createTarget", { url: fixtureURL });
      if (typeof created.targetId !== "string") throw new Error("no owned fixture target");
      target = created.targetId;
      run.phase = "cdp-fixture-eval";
      await wait(async () => {
        const result = await cdp!.targetCommand(target!, "Runtime.evaluate", {
          expression: `location.href === ${JSON.stringify(fixtureURL)} && document.readyState === 'complete' && document.querySelector('#webextras-probe')?.dataset.nonce === ${JSON.stringify(token)}`,
          returnByValue: true, awaitPromise: true,
        });
        return (result.result as any)?.value === true;
      });
      run.phase = "cdp-fixture-diagnostic";
      // Read-only diagnostic for locked-desktop lifecycle; raw values do not affect parity.
      if (verifyAgentEndpoint(agent, port) !== pid) throw new Error("owned endpoint PID changed before fixture diagnostic");
      try {
        const state = await cdp.targetCommand(target, "Runtime.evaluate", {
          expression: "({visibilityState:document.visibilityState,hidden:document.hidden,hasFocus:document.hasFocus(),width:innerWidth,height:innerHeight})",
          returnByValue: true,
        });
        writeFileSync(join(dir, "fixture-state.json"), JSON.stringify((state.result as any)?.value ?? { cdpResult: state }, null, 2));
      } catch (error) {
        writeFileSync(join(dir, "fixture-state.json"), JSON.stringify({ cdpError: String(error) }, null, 2));
      }
      let sequence = 0;
      const action = async (operation: string, extra: Record<string,any> = {}) => {
        if (verifyAgentEndpoint(agent, port) !== pid) throw new Error("owned endpoint PID changed");
        const id = `${token}-${++sequence}`;
        writeFileSync(requestPath + ".tmp", JSON.stringify({ id, url: fixtureURL, operation, nonce: `${token}-${operation}`, ...extra }));
        renameSync(requestPath + ".tmp", requestPath);
        const result = await wait(() => {
          const response = json(requestPath + ".response.json");
          return response.id === id ? response : undefined;
        }, 20000);
        if (result.pid !== pid || result.mainThread !== true) throw new Error("probe acknowledgment PID/thread mismatch");
        return result;
      };
      run.phase = "registry";
      const registry = await wait(async () => {
        const response = await action("registry");
        return response.ok === true && response.matches === 1 ? response : undefined;
      });
      writeFileSync(join(dir, "registry.json"), JSON.stringify(registry, null, 2));
      const frontendCensus=async()=>{
        if(verifyAgentEndpoint(agent,port)!==pid)throw new Error("owned endpoint PID changed before frontend census");
        const result=await cdp!.request("Target.getTargets");assert(Array.isArray(result.targetInfos),"missing target census");
        return result.targetInfos.map((info:any)=>({targetId:info.targetId,type:info.type,url:genuineFrontendURL(info.url)?info.url:info.url===fixtureURL?fixtureURL:"[other owned-app target URL]"}));
      };
      let frontendBefore:any[]=[];
      for (const operation of operations) {
        if(devToolsFrontendEvents&&operation==="console-open")frontendBefore=await frontendCensus();
        run.phase = `action:${operation}`;
        const result = await action(operation);
        run.actions.push(result);
        if (result.ok !== true) run.errors.push(`${operation}: ${result.probeError}`);
        for (const issue of outcomeErrors(result)) run.errors.push(`${operation}: ${issue}`);
        if (operation === "frontend" && result.frontendPresent !== true) run.errors.push("frontend: expected actual frontend wrapper");
        if (operation === "media-types" && (!Array.isArray(result.types) || result.types.length || result.error !== null)) run.errors.push("media-types: inert fixture should have an empty successful type set");
        if (operation.startsWith("pip-") && (result.value !== false || result.error?.domain !== "company.thebrowser.Arc" || result.error?.code !== 6)) run.errors.push(`${operation}: expected false/Arc NSError6 on inert fixture`);
        if (result.matches !== 1 || result.ownerTag !== registry.ownerTag) run.errors.push(`${operation}: canonical wrapper changed`);
        console.log(`${agent}: ${operation}: ${result.ok === true ? "acknowledged" : result.probeError}`);
        if(devToolsFrontendEvents&&operation==="console-open"){
          run.phase="genuine-frontend-events";
          let rawSequence=0,spanSequence=0;
          run.frontendEvents=await runGenuineFrontendEvents({verify:()=>{if(verifyAgentEndpoint(agent,port)!==pid)throw new Error("owned endpoint PID changed before frontend diagnostic");},
            census:frontendCensus,request:(method:string,params:any,sessionId?:string)=>cdp!.request(method,params,sessionId),action,
            raw:(record:any)=>writeFileSync(join(dir,"frontend-runtime.jsonl"),JSON.stringify({sequence:++rawSequence,...record})+"\n",{flag:"a"}),
            spanID:(kind:string)=>`${token}-frontend-${++spanSequence}-${kind}`,now:()=>performance.now(),sleep,
            save:(report:any)=>writeFileSync(join(dir,"frontend-events.json"),JSON.stringify(report,null,2))},frontendBefore,fixtureURL,registry.ownerTag);
          if(run.frontendEvents.status!=="observed")run.errors.push(`frontend-events: ${run.frontendEvents.error??run.frontendEvents.status}`);
        }
      }
      if (anyClock) {
        let evaluationSequence=0;
        const evaluate = async (expression: string,label:string) => evaluateMediaClock(async text=>{
          if (verifyAgentEndpoint(agent, port) !== pid) throw new Error("owned endpoint PID changed before clock stimulus");
          return cdp!.targetCommand(target!, "Runtime.evaluate", { expression:text, returnByValue:true, awaitPromise:true });
        }, record=>writeFileSync(join(dir,"clock-evaluations.jsonl"),JSON.stringify({sequence:++evaluationSequence,...record})+"\n",{flag:"a"}),{phase:run.phase!,label,expression});
        if(mediaClock){
        run.phase="clock-activation";
        await activateMediaClockTarget(target!,{
          verify:()=>{if(verifyAgentEndpoint(agent,port)!==pid)throw new Error("owned endpoint PID changed before clock activation");},
          read:label=>evaluate(clockIdentityJS(fixtureURL,token),label),
          bring:async()=>{if(verifyAgentEndpoint(agent,port)!==pid)throw new Error("owned endpoint PID changed before Page.bringToFront");return cdp!.targetCommand(target!,"Page.bringToFront",{});},
          save:report=>writeFileSync(join(dir,"clock-activation.json"),JSON.stringify(report,null,2)),now:()=>performance.now(),sleep,
        });
        }
        const clockReport: any = { probeMode, policy:mediaClockPolicy, ...(mediaClock?{fixture:mediaClockIdentity}:{}), lane:mediaServiceClock?"declared public SERVICE-position DTO clock only; no player/DOM alignment/fallback/visibility claim":"public service-position clock; implicit player fallback uncovered", laneB:"provisional original behavior until measured; no timing normalization", phases:[] };
        for (const phase of (mediaServiceClock?mediaServiceClockPhases:mediaClockPhases)) {
          run.phase = `clock-prepare:${phase.name}`;
          const videoURL=`http://127.0.0.1:${server.port}/${token}/clock.webm`;
          const phaseReport: any = { phase:phase.name,status:"preparing",attempts:[] };
          clockReport.phases.push(phaseReport);
          const savePhase=()=>writeFileSync(join(dir,"media-clock.json"),JSON.stringify(clockReport,null,2));
          savePhase(); // A failed initial preparation must leave a durable phase record.
          const failureDOM=async()=>{
            if(phaseReport.failureDiagnosticAttempted)return;
            phaseReport.failureDiagnosticAttempted=true;savePhase();
            try{phaseReport.failureDOM=await evaluate(mediaServiceClock?serviceClockObservationJS(fixtureURL,token):mediaClockFailureDOMJS(fixtureURL,videoURL),"failure-DOM");}
            catch(error){phaseReport.failureDOMError=String(error);}
            savePhase();
          };
          try{
          phaseReport.preparation=await evaluate(mediaServiceClock?mediaServiceClockPrepareJS(phase as typeof mediaServiceClockPhases[number],fixtureURL,token):mediaClockPrepareJS(phase as typeof mediaClockPhases[number],videoURL),"prepare");
          phaseReport.status="prepared";savePhase();
          const measured: any[] = [];
          let heldToken: string | undefined;
          for (let index = 0; index < 5; ++index) {
            if (index) await sleep(index < 3 ? mediaClockPolicy.warmupMilliseconds : mediaClockPolicy.heldMilliseconds);
            const operation = index < 3 ? "clock-snapshot" : "clock-held-time";
            run.phase = `action:${clockActionKey(phase.name,index)}`;
            const hostBeforeDOM = process.hrtime.bigint().toString();
            const domBefore = await evaluate(mediaServiceClock?serviceClockObservationJS(fixtureURL,token):mediaClockDOMJS,`attempt-${index}-before`);
            const hostBeforeRequest = process.hrtime.bigint().toString();
            const result = await action(operation, heldToken ? {heldToken} : {});
            const hostAfterResponse = process.hrtime.bigint().toString();
            result.domBefore = domBefore; result.domAfter = await evaluate(mediaServiceClock?serviceClockObservationJS(fixtureURL,token):mediaClockDOMJS,`attempt-${index}-after`);
            result.hostMonotonicNanos = { beforeDOM:hostBeforeDOM, beforeRequest:hostBeforeRequest,
              afterResponse:hostAfterResponse, afterDOM:process.hrtime.bigint().toString() };
            result.actionKey = clockActionKey(phase.name,index); result.clockPhase = phase.name; result.clockAttempt = index;
            result.positionReceipt = phaseReport.preparation.receipt;
            run.actions.push(result); measured.push(result); phaseReport.attempts.push(result);
            if (index < 3) heldToken = result.clock?.heldToken;
            if (result.ok !== true || result.matches !== 1 || result.ownerTag !== registry.ownerTag) run.errors.push(mediaServiceClock&&isNilClockPosition(result)?`${result.actionKey}: inconclusive nil public service position receipt`:`${result.actionKey}: failed clock receipt or canonical owner changed`);
            for (const issue of outcomeErrors(result)) run.errors.push(`${result.actionKey}: ${issue}`);
            writeFileSync(join(dir, "media-clock.json"), JSON.stringify(clockReport, null, 2));
          }
          try { if(mediaServiceClock)validateMediaServiceClockPhase(phase as typeof mediaServiceClockPhases[number],measured);else validateClockPhase(phase as typeof mediaClockPhases[number], measured); phaseReport.functional = "pass"; }
          catch (error) { phaseReport.functional = isClockInconclusive(error)?"inconclusive":"fail"; phaseReport.error = String(error); run.errors.push(`${phase.name}: ${String(error)}`); await failureDOM(); }
          const release = await action("clock-release", {heldToken});
          phaseReport.release = release;
          writeFileSync(join(dir, "media-clock.json"), JSON.stringify(clockReport, null, 2));
          if(mediaServiceClock&&!heldToken&&phaseReport.functional==="inconclusive")phaseReport.releaseStatus="no retained DTO after fixed inconclusive attempts";
          else assert(release.ok === true && release.publicCalls?.length === 0 && release.timeline?.length === 0, "held clock DTO release failed");
          phaseReport.status=phaseReport.functional==="inconclusive"?"inconclusive":"complete";savePhase();
          }catch(error){phaseReport.status=isClockInconclusive(error)?"inconclusive":"failed";phaseReport.error=String(error);savePhase();await failureDOM();throw error;}
        }
      }
      run.phase = "cdp-close-fixture";
      await cdp.request("Target.closeTarget", { targetId: target });
      target = undefined;
      run.phase = "complete";
    } catch (error) {
      run.failure = { phase: run.phase, message: String(error) };
      run.errors.push(`[${run.phase}] ${String(error)}`);
    }
    finally {
      if (pid && browserIdentity) {
        trackHelpers();
        let graceful = false;
        try {
          if (verifyAgentEndpoint(agent, port) !== pid) throw new Error("endpoint PID changed before cleanup");
          cdp ??= await AgentCDP.connectAgent(agent, port);
          await cdp.request("Browser.close").catch(() => {});
          await wait(ownedGone, 15000);
          graceful = true;
        } catch {
          terminateCaptured([browserIdentity]);
          try { await wait(ownedGone, 5000); }
          catch {
            // Only exact helpers captured as descendants while this browser was alive.
            terminateCaptured([...ownedHelpers.values()]);
            try { await wait(ownedGone, 15000); }
            catch { run.errors.push("captured owned browser/helpers did not exit"); }
          }
        }
        run.cleanup = `${graceful ? "Browser.close" : "SIGTERM of captured identities"}; ${ownedHelpers.size} owned helpers checked`;
      }
      observeProcesses = undefined;
      cdp?.close();
      verifyResourceJournal(run,dir);
      const ownedLog=join(dir,"app.log");
      run.runtimeStatus=runtimeLogOutcome(existsSync(ownedLog)?readFileSync(ownedLog,"utf8"):null,run.cleanup,pid!==undefined);
      run.errors.push(...run.runtimeStatus.errors);
      if (existsSync(recordPath)) {
        const record = readFileSync(recordPath);
        try {
          const all = parseRecords(record);
          run.recordCallerLabels = hasCallerLabels(all);
          run.fixtureComparisonStatus = fixtureVerified ? "verified-exact-image" : "pending-fixture-comparison-support";
          if (!fixtureVerified) run.errors.push(fixtureComparisonPending);
          for (const row of all) if (row.event.ev === "call" || row.event.ev === "ret") {
            const caller = row.event.from;
            assert(typeof caller === "string" && caller.length > 0 && caller.trim() === caller && !/[\x00-\x1f]/.test(caller), "missing/mixed caller attribution");
          }
          if (all[0]?.event.ev !== "hooked" || all[0]?.event.seq !== 0 || !(all[0]?.event.engine_classes > 0) || all.filter(row => row.event.ev === "hooked").length !== 1) {
            throw new Error("missing initial recorder hooked event");
          }
          assert(new Set(all.map(row => row.event.seq)).size === all.length, "duplicate recorder sequence IDs");
          // Shutdown may interrupt final lifecycle calls; every measured action is checked separately for complete nesting.
        } catch (error) { run.errors.push(`recorder: ${String(error)}`); }
        const projection = new PublicTraceProjection(comparisonMetadata, fixtureImage);
        let fixtureCalls = 0, fixtureReturns = 0;
        const heldClockReceivers = new Map<string,string>();
        for (const action of run.actions) {
          try {
            const span = validSpan(record, action.recordBegin, action.recordEnd);
            const rows = parseRecords(span, action.recordBegin);
            action.traceShape = validateActionTrace(action, rows, record, projection, heldClockReceivers);
            const fixtureRows = rows.filter(row => row.event.dir === "->engine" && row.event.from === fixtureImage);
            const calls = fixtureRows.filter(row => row.event.ev === "call").length;
            const returns = fixtureRows.filter(row => row.event.ev === "ret").length;
            const expected = expectedSelectors(action.operation).length;
            assert(calls === expected && returns === expected, "fixture public invocation coverage mismatch");
            fixtureCalls += calls; fixtureReturns += returns;
            writeFileSync(join(dir, `${action.actionKey ?? action.operation}.record.jsonl`), span);
          } catch (error) { run.errors.push(`${action.actionKey ?? action.operation}: ${String(error)}`); }
        }
        if(devToolsFrontendEvents&&run.frontendEvents){
          try{
            const open=run.actions.find(action=>action.operation==="console-open");
            const roots=parseRecords(validSpan(record,open.recordBegin,open.recordEnd),open.recordBegin).filter(row=>row.event.ev==="call"&&row.event.cls==="ArcWebContentsDevTools"&&row.event.sel==="openConsoleView"&&row.event.from===fixtureImage);
            assert(roots.length===1,"missing canonical service anchor for frontend callback spans");
            const spanKinds=["inspect-on","inspect-off","dock-change","dock-restore"],spanIDs=new Set<string>();let priorFrontendEnd=open.recordEnd;
            for(const [index,span] of run.frontendEvents.spans.entries()){
              try{
                assert(span.kind===spanKinds[index]&&!spanIDs.has(span.spanID)&&span.observation?.recordBegin>=priorFrontendEnd,"missing/reordered/overlapping frontend callback span");
                spanIDs.add(span.spanID);priorFrontendEnd=span.observation.recordEnd;
                span.traceShape=validateFrontendCallbackSpan(span,record,roots[0]!.event.self,open.ownerTag,fixtureImage,projection,frontendKnownCallbacks);
                const bytes=validSpan(record,span.observation.recordBegin,span.observation.recordEnd);
                writeFileSync(join(dir,`frontend-${span.kind}.callbacks.record.jsonl`),bytes);span.rawValidated=true;
              }catch(error){span.rawValidated=false;span.rawError=String(error);run.errors.push(`frontend-${span.kind} callback evidence: ${String(error)}`);}
            }
            run.frontendEvents.typedSpanCount=run.frontendEvents.spans.filter((span:any)=>span.rawValidated).length;
            run.frontendEvents.rawValidated=run.frontendEvents.spans.length===4&&run.frontendEvents.status==="observed"&&run.frontendEvents.spans.every((span:any)=>span.rawValidated);
            assert(run.frontendEvents.rawValidated,"incomplete genuine frontend stimulus/typed callback manifest");
          }catch(error){run.errors.push(`frontend callback evidence: ${String(error)}`);}
          writeFileSync(join(dir,"frontend-events.json"),JSON.stringify(run.frontendEvents,null,2));
        }
        run.fixturePublicCounts = { call: fixtureCalls, ret: fixtureReturns };
        if (run.actions.length === actionManifest.length && (fixtureCalls !== expectedPairCount || fixtureReturns !== expectedPairCount)) run.errors.push(`expected exactly ${expectedPairCount} admitted fixture calls and returns`);
        if (JSON.stringify(run.actions.map(action => ({key:action.actionKey ?? action.operation, operation:action.operation}))) !== JSON.stringify(actionManifest)) run.errors.push("missing/reordered action manifest");
      }
      else run.errors.push("recorder file missing at cleanup");
      if(devToolsFrontendEvents){
        try{verifyFrontendRecorderPolicy(frontendRecorderPolicy,"after measurement/cleanup");run.frontendRecorderPolicyStatus="verified-unchanged";}
        catch(error){run.frontendRecorderPolicyStatus="changed";run.errors.push(`frontend recorder policy: ${String(error)}`);}
      }
      writeFileSync(join(dir, "observations.json"), JSON.stringify(run, null, 2));
    }
  }
} finally { server.stop(true); }
const semantic = (action: any) => canonical({
  operation: action.operation,
  ...(action.operation.startsWith("clock-") ? { actionKey:action.actionKey, clockPhase:action.clockPhase, clockAttempt:action.clockAttempt,
    clockValues:action.publicCalls?.map((call:any)=>({selector:call.selector,value:call.value,abi:call.abi})) } : {}),
  ok: action.ok, probeError: action.probeError, matches: action.matches,
  mainThread: action.mainThread, frontendPresent: action.frontendPresent, frontendClass: action.frontendClass,
  types: action.types, value: action.value, error: action.error,
  unsubscribeReturn: action.unsubscribeReturn,
  publicCalls: action.publicCalls?.map((call: any) => call.selector),
  timeline: action.timeline, traceShape: action.traceShape,
  callbacks: (action.callbacks ?? []).map((event: any) => ({
    event: event.event, mode: event.mode, mainThread: event.mainThread,
    beforePublicReturn: event.beforePublicReturn, publicSelector: event.publicSelector, forwarded: event.forwarded,
    message: comparedMessage(event, action), parsed: normalizedPayload(event.parsed, action), error: event.error,
    subscriptionMatches: event.event === "subscription" ? event.subscriptionID === action.subscriptionToken : undefined,
  })),
});
const golden = runs.find(run => run.agent === "trace156");
const ours = runs.find(run => run.agent === "ours156");
const differences: unknown[] = [];
if (golden && ours) for (let index = 0; index < Math.max(golden.actions.length, ours.actions.length); ++index) {
  const left = golden.actions[index] ? semantic(golden.actions[index]) : null;
  const right = ours.actions[index] ? semantic(ours.actions[index]) : null;
  if (JSON.stringify(left) !== JSON.stringify(right)) differences.push({ golden: left, ours: right });
}
const frontendEventDifferences:any[]=[];
if(devToolsFrontendEvents&&golden&&ours){
  const shape=(run:Run)=>canonical({status:run.frontendEvents?.status,rawValidated:run.frontendEvents?.rawValidated,
    spans:run.frontendEvents?.spans.map((span:any)=>({kind:span.kind,expectedSelector:span.expectedSelector,expectedMode:span.expectedMode,
      callbacks:span.observation?.callbacks.map((event:any)=>({selector:event.selector,mode:event.mode,mainThread:event.mainThread,forwarded:event.forwarded,forwardDepth:event.forwardDepth,abi:event.abi})),traceShape:span.traceShape}))});
  if(JSON.stringify(shape(golden))!==JSON.stringify(shape(ours)))frontendEventDifferences.push({golden:shape(golden),ours:shape(ours)});
}
if(devToolsFrontendEvents&&golden&&ours){
  for(const kind of ["inspect-on","inspect-off","dock-change","dock-restore"]){
    const left=golden.frontendEvents?.spans.find((span:any)=>span.kind===kind),right=ours.frontendEvents?.spans.find((span:any)=>span.kind===kind);
    if(!left?.rawValidated||!right?.rawValidated){frontendEventDifferences.push({kind,status:"incomplete-typed-callback-evidence"});continue;}
    try{
      assert(JSON.stringify(recorderFileIdentity(frontendComparison.frozen.path))===JSON.stringify(frontendComparison.frozen),"frozen frontend comparator helper changed");
      const result=JSON.parse(exec(["python3","-B",frontendComparisonHelper!,"compare",comparisonSnapshot,fixtureImage,
        join(out,"trace156",`frontend-${kind}.callbacks.record.jsonl`),join(out,"ours156",`frontend-${kind}.callbacks.record.jsonl`),join(out,`frontend-${kind}.diff.json`)]));
      writeFileSync(join(out,`frontend-${kind}.diff.txt`),result.stdout);
      writeFileSync(join(out,`frontend-${kind}.comparison.json`),JSON.stringify(result,null,2));
      if(result.verified!==true||result.mismatches?.length)frontendEventDifferences.push({kind,status:result.status,mismatches:result.mismatches});
    }catch(error){frontendEventDifferences.push({kind,status:"pending-callback-boundary-comparison-support",error:String(error)});}
  }
}
const boundaryDiffs: unknown[] = [];
const fixtureComparison = { status: fixtureVerified ? "verified-exact-image" : "pending-fixture-comparison-support",
  fixtureImageOption: fixtureVerified ? fixtureImage : null, reason: fixtureVerified ? null : fixtureComparisonPending };
if (fixtureComparison.reason) boundaryDiffs.push({ status: fixtureComparison.status, error: fixtureComparison.reason });
if (golden && ours) for (const {key, operation} of actionManifest) {
  const left = join(out, "trace156", `${key}.record.jsonl`);
  const right = join(out, "ours156", `${key}.record.jsonl`);
  if (!existsSync(left) || !existsSync(right)) continue;
  const diffPath = join(out, `${key}.diff.json`);
  if (!fixtureVerified) continue; // A default app-only zero diff is not fixture evidence.
  const result = Bun.spawnSync(["python3", "-B", comparisonHelper, "compare", comparisonSnapshot, fixtureImage,
    left, right, String(expectedSelectors(operation).length), diffPath], { stdout: "pipe", stderr: "pipe" });
  writeFileSync(join(out, `${key}.diff.txt`), result.stdout);
  if (!result.success) { boundaryDiffs.push({ operation, ...(anyClock ? {actionKey:key} : {}), error: result.stderr.toString() }); continue; }
  const diff = json(diffPath);
  const mismatches = ["missing", "extra", "return_mismatch", "arg_mismatch", "count_mismatch"].filter(key => diff[key]?.length);
  if (diff.callback_sequence?.first_divergence !== null) mismatches.push("callback_sequence");
  if (mismatches.length) boundaryDiffs.push({ operation, ...(anyClock ? {actionKey:key} : {}), mismatches });
}
const complete = runs.length === agents.length && runs.every(run => !run.errors.length && (!devToolsFrontendEvents||run.frontendEvents?.rawValidated===true) && !!run.cleanup && run.actions.length === actionManifest.length && run.actions.every(action => action.traceShape?.length > 0));
const parity = golden && ours ? complete && !differences.length && !boundaryDiffs.length && (!devToolsFrontendEvents||!frontendEventDifferences.length) : null;
writeFileSync(join(out, "summary.json"), JSON.stringify({ complete, parity, recorderIdentity, comparisonInputs, fixtureComparison, ...(devToolsFrontendEvents?{probeMode,frontendEventPolicy,frontendRecorderPolicy,frontendComparison,frontendEventDifferences}:{}), ...(anyClock ? {probeMode, ...(mediaClock?{mediaClockIdentity}:{}), mediaClockPolicy, actionManifest, expectedPairCount} : {}), differences, boundaryDiffs, runs }, null, 2));
console.log(JSON.stringify({ complete, parity, fixtureComparison, out }));
process.exitCode = complete && parity !== false ? 0 : 1;
