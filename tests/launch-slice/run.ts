// Launch-slice scenario: launch, open, navigate, store, close, quit, relaunch, read back, quit.
// Each step is asserted against fixed values (pass / fail / inconclusive). With --trace, each launch
// runs under lldb_trace.py so engine entries are recorded per launch.
//
//   bun tests/launch-slice/run.ts --app <path/ArcModAgent-<slug>.app> --out <dir> [--trace] [--port 9333]
import { summarize } from "../scenarios/coverage";
import { AgentCDP, verifyAgentEndpoint } from "../../../arc/tools/agent-cdp";
import { NativeController } from "../../../arc/tools/agent-native";
import { spawn, spawnSync, type Subprocess } from "bun";
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { basename, join, resolve } from "node:path";

const ARCMOD = resolve(import.meta.dir, "../../../arc");
const argv = process.argv.slice(2);
const opt = (name: string): string | undefined => { const i = argv.indexOf(name); return i >= 0 ? argv[i + 1] : undefined; };
const scenario = opt("--scenario") ?? "launch-slice";
if (!/^[a-z][a-z0-9-]*$/.test(scenario)) throw new Error("invalid scenario name");
const scenarioPath = resolve(import.meta.dir, `../scenarios/${scenario}.ts`);
if (!existsSync(scenarioPath)) throw new Error(`unknown scenario: ${scenario}`);
const scenarioModule = await import(scenarioPath);
if (typeof scenarioModule.default !== "function") throw new Error(`scenario ${scenario} has no default runner`);
const app = resolve(opt("--app") ?? "");
const outDir = resolve(opt("--out") ?? "");
const port = opt("--port") ?? "9333";
if (!/^\d+$/.test(port) || Number(port) < 1024 || Number(port) > 65535) throw new Error("invalid CDP port");
const traced = argv.includes("--trace");
// --record: inject tests/recorder/ArcEngineRecorder.dylib and write each launch's app<->engine calls to
// launch-N/record.jsonl (for diffing our engine against Arc's).
const recorded = argv.includes("--record");
const recorderDylib = resolve(import.meta.dir, "../recorder/ArcEngineRecorder.dylib");
if (recorded && !existsSync(recorderDylib)) throw new Error(`build ${recorderDylib} first (see its header)`);
if (!existsSync(app) || !opt("--out")) throw new Error("usage: bun run.ts --app <ArcModAgent-x.app> --out <dir> [--trace] [--port N]");
if (!/^ArcModAgent-[a-z][a-z0-9-]*\.app$/.test(basename(app))) throw new Error("only disposable ArcModAgent apps are allowed");
mkdirSync(outDir, { recursive: true });

const plist = (key: string): string =>
  spawnSync(["/usr/libexec/PlistBuddy", "-c", `Print :${key}`, join(app, "Contents/Info.plist")]).stdout.toString().trim();
const slug = plist("ArcModAgentSession");
const appName = basename(app, ".app");
const domain = `company.thebrowser.Browser.mod.agent.${slug}`;
if (!/^[a-z][a-z0-9-]*$/.test(slug) || appName !== `ArcModAgent-${slug}`) throw new Error("invalid agent bundle identity");
const data = join(process.env.HOME!, "Library/Application Support", appName);
const chromiumData = join(data, "Agent Chromium Data");

// Same switches as tools/run-mod.sh --agent <slug> --cdp-port <port>; Chromium's sandbox stays on.
const chromiumArgs = [
  "--use-mock-keychain", "--no-default-browser-check", "--disable-sync", "--disable-background-networking",
  `--host-resolver-rules=${plist("ArcModHostResolverRules")}`, `--user-data-dir=${chromiumData}`,
  "--remote-debugging-address=127.0.0.1", `--remote-debugging-port=${port}`,
  ...(scenarioModule.chromiumArgs ?? []),
];

// Fixtures. Expected values are fixed here, never taken from another run.
const EXPECT = {
  aTitle: "launch-slice A", bTitle: "launch-slice B", marker: "ls-marker-7f3a",
  cookie: "ls_cookie=c0ffee", localKey: "ls-local", localValue: "v-41", idbValue: "idb-93",
};
const fixtureProcess = spawn(["bun", join(import.meta.dir, "../scenarios/fixture-server.ts"), scenarioPath],
  { stdout: "pipe", stderr: "inherit" });
const fixtureReader = fixtureProcess.stdout.getReader();
const first = await fixtureReader.read();
if (first.done) throw new Error("fixture server failed to start");
const fixturePort = JSON.parse(new TextDecoder().decode(first.value)).port;
const base = `http://127.0.0.1:${fixturePort}`;

type Outcome = "pass" | "fail" | "inconclusive";
const steps: { step: string; outcome: Outcome; detail: string }[] = [];
const record = (step: string, outcome: Outcome, detail: string) => { steps.push({ step, outcome, detail }); console.log(`${step} ${outcome} ${detail}`); };
const sleep = (ms: number) => new Promise((r) => setTimeout(r, ms));

const cdp = (...args: string[]): unknown => {
  const r = spawnSync(["bun", join(ARCMOD, "tools/agent-cdp.ts"), "--agent", slug, "--port", port, ...args], { stdout: "pipe", stderr: "pipe" });
  if (!r.success) throw new Error(`agent-cdp ${args[0]}: ${r.stderr.toString().trim().split("\n").at(-1)}`);
  return JSON.parse(r.stdout.toString());
};
const value = (r: unknown): unknown => (r as { result?: { value?: unknown } })?.result?.value;
const evaluate = (target: string, expr: string): unknown => value(cdp("evaluate", target, expr));
const waitFor = async <T>(fn: () => T | undefined, ms: number): Promise<T | undefined> => {
  const end = Date.now() + ms;
  while (Date.now() < end) { try { const v = fn(); if (v !== undefined) return v; } catch { /* not ready yet */ } await sleep(250); }
  return undefined;
};

let launchNo = 0;
const launches: Launch[] = [];
type Launch = { proc: Subprocess; pid: () => number | undefined };
const launch = (): Launch => {
  const l = launchProcess(); launches.push(l); return l;
};
const launchProcess = (): Launch => {
  if (pgrepExe(join(app, "Contents/MacOS/Arc")).length) throw new Error("agent app already running; refusing duplicate launch");
  launchNo += 1;
  spawnSync(["defaults", "write", domain, "cloudKitSyncEnabled", "-bool", "false"]);
  spawnSync(["defaults", "write", domain, "firebaseSyncEnabled", "-bool", "false"]);
  mkdirSync(chromiumData, { recursive: true, mode: 0o700 });
  const dir = join(outDir, `launch-${launchNo}`);
  mkdirSync(dir, { recursive: true });
  if (traced) {
    const lldbPath = spawnSync(["lldb", "-P"]).stdout.toString().trim();
    const proc = spawn(["xcrun", "python3", join(import.meta.dir, "lldb_trace.py"), "--app", app, "--out", dir, "--", ...chromiumArgs],
      { env: { ...process.env, PYTHONPATH: lldbPath }, stdout: Bun.file(join(dir, "tracer.log")), stderr: Bun.file(join(dir, "tracer.err")) });
    const pidFile = join(dir, "browser.pid");
    return { proc, pid: () => (existsSync(pidFile) ? Number(readFileSync(pidFile, "utf8")) : undefined) };
  }
  // LaunchServices, like tools/run-mod.sh: a direct exec of the app crashed on its first tab open.
  const exe = join(app, "Contents/MacOS/Arc");
  const before = new Set(pgrepExe(exe));
  const log = join(dir, "app.log");
  const env = recorded
    ? ["--env", `DYLD_INSERT_LIBRARIES=${recorderDylib}`, "--env", `ARC_RECORD_PATH=${join(dir, "record.jsonl")}`]
    : [];
  const proc = spawn(["open", "-n", "-g", "-a", app, ...env, "--stdout", log, "--stderr", log, "--args", ...chromiumArgs]);
  let pid: number | undefined;
  return { proc, pid: () => (pid ??= pgrepExe(exe).find((p) => !before.has(p))) };
};
const pgrepExe = (exe: string): number[] =>
  spawnSync(["pgrep", "-f", `^${exe}( |$)`]).stdout.toString().trim().split("\n").filter(Boolean).map(Number);
const children = (pid: number): number[] =>
  spawnSync(["pgrep", "-P", String(pid)]).stdout.toString().trim().split("\n").filter(Boolean).map(Number);
const alive = (pid: number) => spawnSync(["kill", "-0", String(pid)]).success;

const browserClose = async (expectedPid: number): Promise<boolean> => {
  try {
    if (verifyAgentEndpoint(slug, Number(port)) !== expectedPid) return false;
    const { webSocketDebuggerUrl } = (await (await fetch(`http://127.0.0.1:${port}/json/version`)).json()) as { webSocketDebuggerUrl: string };
    const ws = new WebSocket(webSocketDebuggerUrl);
    await new Promise<void>((res, rej) => { ws.onopen = () => res(); ws.onerror = () => rej(new Error("ws")); });
    ws.send(JSON.stringify({ id: 1, method: "Browser.close" }));
    await sleep(500);
    try { ws.close(); } catch { /* browser may already be gone */ }
    return true;
  } catch { return false; }
};
const quit = async (l: Launch, step: string) => {
  client?.close(); client = undefined;
  const pid = l.pid();
  if (!pid) return record(step, "inconclusive", "no browser pid");
  const helpers = children(pid);
  // Graceful exit through Chromium's own path (CDP Browser.close), so state such as cookies is flushed.
  // SIGTERM only if that doesn't end the process.
  const closed = await browserClose(pid);
  let how = closed ? "Browser.close" : "SIGTERM (Browser.close failed)";
  if (!closed || !(await waitFor(() => (!alive(pid) ? true : undefined), 15000))) {
    if (closed) how = "SIGTERM (Browser.close did not exit in 15 s)";
    if (pgrepExe(join(app, "Contents/MacOS/Arc")).includes(pid)) spawnSync(["kill", "-TERM", String(pid)]);
  }
  const gone = await waitFor(() => (!alive(pid) && helpers.every((h) => !alive(h)) ? true : undefined), 15000);
  if (traced) await waitFor(() => (l.proc.exitCode !== null ? true : undefined), 15000);
  record(step, gone && how === "Browser.close" ? "pass" : "fail",
    `${how}; ${gone ? `browser + ${helpers.length} helpers exited` : `still alive: ${[pid, ...helpers].filter(alive).join(",")}`}`);
};

// Arc can crash if a page is created before its native window exists, so wait for the agent dylib's
// PID-bound readiness record (written once the sidebar exists), then for CDP.
const readyFile = join(data, "ArcModAgentReady.json");
const ready = async (l: Launch, step: string): Promise<boolean> => {
  const nativeReady = await waitFor(() => {
    const pid = l.pid();
    if (!pid || !existsSync(readyFile)) return undefined;
    const rec = JSON.parse(readFileSync(readyFile, "utf8")) as { pid?: number; ready?: boolean };
    return rec.ready === true && rec.pid === pid ? true : undefined;
  }, 60000);
  if (!nativeReady) { record(step, "inconclusive", "no readiness record for this pid within 60 s"); return false; }
  const ok = await waitFor(() => { const r = cdp("list"); return Array.isArray(r) ? r : undefined; }, 30000);
  record(step, ok ? "pass" : "inconclusive", ok ? "native ready, CDP target list available" : "CDP not ready within 30 s");
  return Boolean(ok);
};
const open = (url: string): string => {
  const id = (cdp("open", url) as { targetId?: string }).targetId;
  if (typeof id !== "string") throw new Error("open: no targetId");
  return id;
};
const titleIs = async (target: string, title: string, ms = 30000) =>
  (await waitFor(() => (evaluate(target, "document.title") === title ? true : undefined), ms)) === true;

const context = {
  app, slug, port: Number(port), outDir, data, base, EXPECT, launch, ready, quit, open, titleIs,
  evaluate, record, waitFor, cdp, sleep,
  assert(step: string, ok: unknown, detail = "") { record(step, ok ? "pass" : "fail", detail); },
  async client() { return client ??= await AgentCDP.connectAgent(slug, Number(port)); },
  native() {
    const state = nativeController.state();
    writeFileSync(join(outDir, `native-${++nativeNo}.json`), JSON.stringify({ ...state, elements: (() => {
      const nodes = state.elements as NativeNode[];
      const privateRoots = nodes.filter(n => /^(Apple|Services)$|Recent/i.test(n.label)).map(n => n.path);
      return nodes.filter(n => !privateRoots.some(root => n.path === root || n.path.startsWith(root + ".")));
    })() }, null, 2));
    return state as NativeSnapshot;
  },
  action(node: NativeNode, kind: "press" | "set" = "press", value?: string) {
    return nativeController.action(node.handle, kind, value);
  },
  async step(name: string, fn: () => Promise<void> | void) {
    try { await fn(); } catch (error) { record(name, "inconclusive", String(error)); }
  },
};
export type NativeNode = { handle: string; role: string; subrole: string; label: string; identifier: string;
  path: string; enabled: boolean; actions: string[]; valueSettable: boolean; value?: unknown };
export type NativeSnapshot = { pid: number; elements: NativeNode[]; truncated: boolean };
export type ScenarioContext = typeof context;
let client: AgentCDP | undefined;
let nativeNo = 0;
const nativeController = new NativeController(slug, Number(port));
try {
  await scenarioModule.default(context);
} catch (error) {
  record("scenario", "inconclusive", String(error));
} finally {
  client?.close();
  for (const l of launches) if (l.pid() && alive(l.pid()!)) await quit(l, "cleanup quit");
  fixtureReader.releaseLock(); fixtureProcess.kill(); await fixtureProcess.exited;
}
writeFileSync(join(outDir, "steps.json"), JSON.stringify({ scenario, app, slug, traced, recorded, port, steps }, null, 2));
const failed = steps.filter((s) => s.outcome !== "pass");
console.log(failed.length ? `${failed.length} step(s) not passing` : "all steps pass");
let valid = true;
if (recorded) {
  try { valid = summarize(outDir).accepted; } catch (error) { valid = false; console.error(`invalid recorder transcript: ${error}`); }
}
process.exitCode = failed.length || !valid ? 1 : 0;
