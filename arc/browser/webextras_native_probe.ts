#!/usr/bin/env bun
// Test-only owned-app boundary probe; no AX/native readiness or global input.
import { AgentCDP, verifyAgentEndpoint } from "../../../arc/tools/agent-cdp";
import { existsSync, mkdirSync, readFileSync, renameSync, statSync, writeFileSync } from "node:fs";
import { join, resolve } from "node:path";

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
        ["cls", "kind", "sel", "self", "dir", "thread"].every(key => call.event[key] === event[key]) &&
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
  if (operation === "subscription") return ["subscribe:autoEnable:notification:", "execute:completion:error:", "unsubscribe:"];
  return [one[operation] ?? "execute:completion:error:"];
}
function validateActionTrace(action: any, rows: RecordRow[], record: Buffer): any[] {
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
    const cls = action.operation === "media-types" || action.operation.startsWith("pip-") ? "ArcWebContentsMediaSession" : "ArcWebContentsDevTools";
    const candidates = rows.filter(row => row.begin >= call.recordBegin && row.end <= call.recordEnd && row.event.ev === "call" &&
      row.event.dir === "->engine" && row.event.cls === cls && row.event.sel === call.selector && row.event.depth === mainDepth && row.event.thread === "main");
    assert(candidates.length === 1, `expected exactly one recorded ${cls}.${call.selector} invocation`);
    const entry = candidates[0]!;
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
    assert(callback?.forwarded === true && Number.isSafeInteger(callback.recordOffset) && Number.isSafeInteger(callback.recordAfterForwarding) &&
      callback.recordOffset >= action.recordBegin && callback.recordOffset <= returned.begin && returned.end <= callback.recordAfterForwarding &&
      callback.recordAfterForwarding <= action.recordEnd && entry.event.args?.[0] === publicRows[0]!.event.self, "incomplete or mismatched app callback forwarding");
  } else assert(!delegates.length, "unexpected display/close callback");
  // A complete ordered boundary shape closes diff.py's depth/order/object-identity gaps.
  const identities = new Map<string, string>();
  const counts = new Map<string, number>();
  const norm = (value: any): any => typeof value === "string" ? value.replace(/<([^<>]+)#(\d+)>/g, (full, cls) => {
    if (!identities.has(full)) { const count = (counts.get(cls) ?? 0) + 1; counts.set(cls, count); identities.set(full, `<${cls}#${count}>`); }
    return identities.get(full)!;
  }) : value;
  return rows.map(row => ({ ev:row.event.ev, thread:row.event.thread, depth:row.event.depth - (ambient.get(row.event.thread)?.length ?? 0), dir:row.event.dir,
    cls:row.event.cls, kind:row.event.kind, sel:row.event.sel, self:norm(row.event.self) }));
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

const root = resolve(import.meta.dir, "../..");
const args = process.argv.slice(2);
const option = (key: string, fallback?: string): string | undefined => {
  const index = args.indexOf(key);
  return index < 0 ? fallback : args[index + 1];
};
if (!option("--out")) throw new Error("usage: webextras_native_probe.sh run --out DIR [--agents trace156,ours156] [--port 9458]");
const out = resolve(option("--out")!);
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
const lockPID = Number(readFileSync(join(root, "logs/arccore-build.lock/pid"), "utf8").trim());
let ancestor = process.pid;
for (let depth = 0; ancestor !== lockPID && ancestor > 1 && depth < 20; ++depth) {
  ancestor = Number(exec(["ps", "-p", String(ancestor), "-o", "ppid="]));
}
if (ancestor !== lockPID) throw new Error("run under arc/tools/locked.sh; this driver never acquires or bypasses the lock");
const dylib = join(root, "logs/wp/webextras-native-probe-build/WebextrasNativeProbe.dylib");
const recorder = join(root, "tests/recorder/ArcEngineRecorder.dylib");
for (const file of [dylib, recorder]) if (!existsSync(file)) throw new Error(`missing ${file}`);
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
const server = Bun.serve({ hostname: "127.0.0.1", port: 0, fetch(request) {
  if (new URL(request.url).pathname !== fixturePath) return new Response("not found", { status: 404 });
  return new Response(`<!doctype html><meta charset="utf-8"><title>Webextras native boundary fixture</title>
    <main id="webextras-probe" data-nonce="${token}">Local inert boundary fixture</main>`,
    { headers: { "content-type": "text/html; charset=utf-8", "cache-control": "no-store" } });
} });
const fixtureURL = `http://127.0.0.1:${server.port}${fixturePath}`;
mkdirSync(out, { recursive: true });
type Run = { agent: string; pid?: number; actions: any[]; errors: string[]; cleanup?: string };
const runs: Run[] = [];
const operations = ["console-open", "frontend", "console-close", "panel-closed", "execute-evaluate", "execute-malformed", "execute-unknown", "subscription", "media-types", "pip-request-error", "pip-exit-error"];
try {
  for (const agent of agents) {
    const run: Run = { agent, actions: [], errors: [] };
    runs.push(run);
    const dir = join(out, agent);
    mkdirSync(dir, { recursive: true });
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
      readiness = { agent, beforeLaunch: readySnapshot(nativeReadyPath), launchStartedMs: Date.now() };
      writeFileSync(join(dir, "readiness.json"), JSON.stringify(readiness, null, 2));
      exec(["open", "-n", "-g", "-a", app,
        "--env", `DYLD_INSERT_LIBRARIES=${dylib}:${recorder}`,
        "--env", `ARC_WEBEXTRAS_PROBE_REQUEST=${requestPath}`,
        "--env", `ARC_WEBEXTRAS_PROBE_URL=${fixtureURL}`,
        "--env", `ARC_RECORD_PATH=${recordPath}`,
        "--stdout", join(dir, "app.log"), "--stderr", join(dir, "app.log"), "--args",
        "--use-mock-keychain", "--no-default-browser-check", "--disable-sync", "--disable-background-networking",
        `--host-resolver-rules=${plist("ArcModHostResolverRules")}`, `--user-data-dir=${chromiumData}`,
        "--remote-debugging-address=127.0.0.1", `--remote-debugging-port=${port}`]);
      pid = await wait(() => appPIDs(exe)[0], 30000);
      run.pid = pid;
      browserIdentity = processSnapshot().find(row => row.pid === pid && isExecutable(row, exe));
      if (!browserIdentity) throw new Error("launched browser identity unavailable");
      observeProcesses = trackHelpers;
      trackHelpers();
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
      await wait(() => verifyAgentEndpoint(agent, port) === pid, 30000);
      cdp = await AgentCDP.connectAgent(agent, port);
      const created = await cdp.request("Target.createTarget", { url: "about:blank" });
      if (typeof created.targetId !== "string") throw new Error("no owned fixture target");
      target = created.targetId;
      // Navigate only after hook readiness, guaranteeing a new observable setUrl: opportunity.
      await cdp.targetCommand(target, "Page.navigate", { url: fixtureURL });
      await wait(async () => {
        const result = await cdp!.targetCommand(target!, "Runtime.evaluate", {
          expression: `location.href === ${JSON.stringify(fixtureURL)} && document.readyState === 'complete' && document.querySelector('#webextras-probe')?.dataset.nonce === ${JSON.stringify(token)}`,
          returnByValue: true, awaitPromise: true,
        });
        return (result.result as any)?.value === true;
      });
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
      const action = async (operation: string) => {
        if (verifyAgentEndpoint(agent, port) !== pid) throw new Error("owned endpoint PID changed");
        const id = `${token}-${++sequence}`;
        writeFileSync(requestPath + ".tmp", JSON.stringify({ id, url: fixtureURL, operation, nonce: `${token}-${operation}` }));
        renameSync(requestPath + ".tmp", requestPath);
        const result = await wait(() => {
          const response = json(requestPath + ".response.json");
          return response.id === id ? response : undefined;
        }, 20000);
        if (result.pid !== pid || result.mainThread !== true) throw new Error("probe acknowledgment PID/thread mismatch");
        return result;
      };
      const registry = await wait(async () => {
        const response = await action("registry");
        return response.ok === true && response.matches === 1 ? response : undefined;
      });
      writeFileSync(join(dir, "registry.json"), JSON.stringify(registry, null, 2));
      for (const operation of operations) {
        const result = await action(operation);
        run.actions.push(result);
        if (!result.ok) run.errors.push(`${operation}: ${result.probeError}`);
        for (const issue of outcomeErrors(result)) run.errors.push(`${operation}: ${issue}`);
        if (operation === "frontend" && result.frontendPresent !== true) run.errors.push("frontend: expected actual frontend wrapper");
        if (operation === "media-types" && (!Array.isArray(result.types) || result.types.length || result.error !== null)) run.errors.push("media-types: inert fixture should have an empty successful type set");
        if (operation.startsWith("pip-") && (result.value !== false || result.error?.domain !== "company.thebrowser.Arc" || result.error?.code !== 6)) run.errors.push(`${operation}: expected false/Arc NSError6 on inert fixture`);
        if (result.matches !== 1 || result.ownerTag !== registry.ownerTag) run.errors.push(`${operation}: canonical wrapper changed`);
        console.log(`${agent}: ${operation}: ${result.ok ? "acknowledged" : result.probeError}`);
      }
      await cdp.request("Target.closeTarget", { targetId: target });
      target = undefined;
    } catch (error) { run.errors.push(String(error)); }
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
      if (existsSync(recordPath)) {
        const record = readFileSync(recordPath);
        try {
          const all = parseRecords(record);
          if (all[0]?.event.ev !== "hooked" || all[0]?.event.seq !== 0 || !(all[0]?.event.engine_classes > 0) || all.filter(row => row.event.ev === "hooked").length !== 1) {
            throw new Error("missing initial recorder hooked event");
          }
          assert(new Set(all.map(row => row.event.seq)).size === all.length, "duplicate recorder sequence IDs");
          // Shutdown may interrupt final lifecycle calls; every measured action is checked separately for complete nesting.
        } catch (error) { run.errors.push(`recorder: ${String(error)}`); }
        for (const action of run.actions) {
          try {
            const span = validSpan(record, action.recordBegin, action.recordEnd);
            const rows = parseRecords(span, action.recordBegin);
            action.traceShape = validateActionTrace(action, rows, record);
            writeFileSync(join(dir, `${action.operation}.record.jsonl`), span);
          } catch (error) { run.errors.push(`${action.operation}: ${String(error)}`); }
        }
      }
      else run.errors.push("recorder file missing at cleanup");
      writeFileSync(join(dir, "observations.json"), JSON.stringify(run, null, 2));
    }
  }
} finally { server.stop(true); }
const semantic = (action: any) => canonical({
  operation: action.operation, ok: action.ok, probeError: action.probeError, matches: action.matches,
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
const boundaryDiffs: unknown[] = [];
if (golden && ours) for (const operation of operations) {
  const left = join(out, "trace156", `${operation}.record.jsonl`);
  const right = join(out, "ours156", `${operation}.record.jsonl`);
  if (!existsSync(left) || !existsSync(right)) continue;
  const diffPath = join(out, `${operation}.diff.json`);
  const result = Bun.spawnSync(["python3", join(root, "tests/recorder/diff.py"), left, right, "--json", diffPath], { stdout: "pipe", stderr: "pipe" });
  writeFileSync(join(out, `${operation}.diff.txt`), result.stdout);
  if (!result.success) { boundaryDiffs.push({ operation, error: result.stderr.toString() }); continue; }
  const diff = json(diffPath);
  const mismatches = ["missing", "extra", "return_mismatch", "arg_mismatch", "count_mismatch"].filter(key => diff[key]?.length);
  if (diff.callback_sequence?.first_divergence !== null) mismatches.push("callback_sequence");
  if (mismatches.length) boundaryDiffs.push({ operation, mismatches });
}
const complete = runs.length === agents.length && runs.every(run => !run.errors.length && !!run.cleanup && run.actions.length === operations.length && run.actions.every(action => action.traceShape?.length > 0));
const parity = golden && ours ? complete && !differences.length && !boundaryDiffs.length : null;
writeFileSync(join(out, "summary.json"), JSON.stringify({ complete, parity, differences, boundaryDiffs, runs }, null, 2));
console.log(JSON.stringify({ complete, parity, out }));
process.exitCode = complete && parity !== false ? 0 : 1;
