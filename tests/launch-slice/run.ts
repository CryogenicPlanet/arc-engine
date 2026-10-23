// Launch-slice scenario: launch, open, navigate, store, close, quit, relaunch, read back, quit.
// Each step is asserted against fixed values (pass / fail / inconclusive). With --trace, each launch
// runs under lldb_trace.py so engine entries are recorded per launch.
//
//   bun tests/launch-slice/run.ts --app <path/ArcModAgent-<slug>.app> --out <dir> [--trace] [--port 9333]
import { spawn, spawnSync, type Subprocess } from "bun";
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { basename, join, resolve } from "node:path";

const ARCMOD = resolve(import.meta.dir, "../../../arc");
const argv = process.argv.slice(2);
const opt = (name: string): string | undefined => { const i = argv.indexOf(name); return i >= 0 ? argv[i + 1] : undefined; };
const app = resolve(opt("--app") ?? "");
const outDir = resolve(opt("--out") ?? "");
const port = opt("--port") ?? "9333";
const traced = argv.includes("--trace");
// --record: inject tests/recorder/ArcEngineRecorder.dylib and write each launch's app<->engine calls to
// launch-N/record.jsonl (for diffing our engine against Arc's).
const recorded = argv.includes("--record");
const recorderDylib = resolve(import.meta.dir, "../recorder/ArcEngineRecorder.dylib");
if (recorded && !existsSync(recorderDylib)) throw new Error(`build ${recorderDylib} first (see its header)`);
if (!existsSync(app) || !opt("--out")) throw new Error("usage: bun run.ts --app <ArcModAgent-x.app> --out <dir> [--trace] [--port N]");
mkdirSync(outDir, { recursive: true });

const plist = (key: string): string =>
  spawnSync(["/usr/libexec/PlistBuddy", "-c", `Print :${key}`, join(app, "Contents/Info.plist")]).stdout.toString().trim();
const slug = plist("ArcModAgentSession");
const appName = basename(app, ".app");
const domain = `company.thebrowser.Browser.mod.agent.${slug}`;
const data = join(process.env.HOME!, "Library/Application Support", appName);
const chromiumData = join(data, "Agent Chromium Data");

// Same switches as tools/run-mod.sh --agent <slug> --cdp-port <port>; Chromium's sandbox stays on.
const chromiumArgs = [
  "--use-mock-keychain", "--no-default-browser-check", "--disable-sync", "--disable-background-networking",
  `--host-resolver-rules=${plist("ArcModHostResolverRules")}`, `--user-data-dir=${chromiumData}`,
  "--remote-debugging-address=127.0.0.1", `--remote-debugging-port=${port}`,
];

// Fixtures. Expected values are fixed here, never taken from another run.
const EXPECT = {
  aTitle: "launch-slice A", bTitle: "launch-slice B", marker: "ls-marker-7f3a",
  cookie: "ls_cookie=c0ffee", localKey: "ls-local", localValue: "v-41", idbValue: "idb-93",
};
const page = (title: string, body: string) =>
  new Response(`<!doctype html><title>${title}</title><body><div id="${EXPECT.marker}">${body}</div>` +
    `<div style="width:200px;height:200px;background:rgb(12,140,200)"></div></body>`, { headers: { "content-type": "text/html" } });
const server = Bun.serve({
  hostname: "127.0.0.1", port: 0,
  fetch(req) {
    const path = new URL(req.url).pathname;
    if (path === "/a") return page(EXPECT.aTitle, "fixture A");
    if (path === "/b") return page(EXPECT.bTitle, "fixture B");
    return new Response("not found", { status: 404 });
  },
});
const base = `http://127.0.0.1:${server.port}`;

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
type Launch = { proc: Subprocess; pid: () => number | undefined };
const launch = (): Launch => {
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

const browserClose = async (): Promise<boolean> => {
  try {
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
  const pid = l.pid();
  if (!pid) return record(step, "inconclusive", "no browser pid");
  const helpers = children(pid);
  // Graceful exit through Chromium's own path (CDP Browser.close), so state such as cookies is flushed.
  // SIGTERM only if that doesn't end the process.
  const closed = await browserClose();
  let how = closed ? "Browser.close" : "SIGTERM (Browser.close failed)";
  if (!closed || !(await waitFor(() => (!alive(pid) ? true : undefined), 15000))) {
    if (closed) how = "SIGTERM (Browser.close did not exit in 15 s)";
    spawnSync(["kill", "-TERM", String(pid)]);
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

// Run 1: S1–S6.
const l1 = launch();
if (await ready(l1, "S1 launch")) {
  try {
    const t = open(`${base}/a`);
    const okA = await titleIs(t, EXPECT.aTitle) && evaluate(t, `!!document.getElementById(${JSON.stringify(EXPECT.marker)})`) === true;
    record("S2 open A", okA ? "pass" : "fail", `title=${String(evaluate(t, "document.title"))}`);

    // Script navigation: CDP Page.navigate times out on Arc background tabs (the page still commits).
    evaluate(t, `location.assign(${JSON.stringify(`${base}/b`)}); 1`);
    const navStart = Date.now();
    const okB = await titleIs(t, EXPECT.bTitle);
    const navMs = Date.now() - navStart;
    evaluate(t, "location.hash = 'frag'; 1");
    const frag = await waitFor(() => (evaluate(t, "location.hash") === "#frag" ? true : undefined), 5000);
    evaluate(t, "history.back(); 1");
    const back = await waitFor(() => (evaluate(t, "location.href") === `${base}/b` ? true : undefined), 5000);
    record("S3 navigate", okB && frag && back ? "pass" : "fail", `B=${okB} (${navMs} ms) frag=${Boolean(frag)} back=${Boolean(back)} title=${String(evaluate(t, "document.title"))} href=${String(evaluate(t, "location.href"))}`);

    const wrote = evaluate(t, `new Promise((res) => {
      document.cookie = ${JSON.stringify(EXPECT.cookie + "; Max-Age=86400; path=/")};
      localStorage.setItem(${JSON.stringify(EXPECT.localKey)}, ${JSON.stringify(EXPECT.localValue)});
      const r = indexedDB.open("ls-db", 1);
      r.onupgradeneeded = () => r.result.createObjectStore("s");
      r.onsuccess = () => { const tx = r.result.transaction("s", "readwrite"); tx.objectStore("s").put(${JSON.stringify(EXPECT.idbValue)}, "k");
        tx.oncomplete = () => res("ok"); tx.onerror = () => res("idb-error"); };
      r.onerror = () => res("open-error");
    })`);
    record("S4 store", wrote === "ok" ? "pass" : "fail", String(wrote));

    cdp("close", t);
    const t2 = open(`${base}/a`);
    record("S5 close + new tab", (await titleIs(t2, EXPECT.aTitle)) ? "pass" : "fail", "");
  } catch (e) { record("S2-S5", "inconclusive", (e as Error).message); }
}
await quit(l1, "S6 quit");

// Run 2: S7–S8 on the same data dir.
const l2 = launch();
if (await ready(l2, "S7 relaunch")) {
  try {
    const t = open(`${base}/a`);
    await titleIs(t, EXPECT.aTitle);
    const back = evaluate(t, `new Promise((res) => {
      const out = { cookie: document.cookie, local: localStorage.getItem(${JSON.stringify(EXPECT.localKey)}) };
      const r = indexedDB.open("ls-db", 1);
      r.onsuccess = () => { try { const g = r.result.transaction("s").objectStore("s").get("k");
        g.onsuccess = () => res(JSON.stringify({ ...out, idb: g.result })); } catch (e) { res(JSON.stringify({ ...out, idb: "missing" })); } };
      r.onerror = () => res(JSON.stringify({ ...out, idb: "open-error" }));
    })`);
    const got = JSON.parse(String(back)) as { cookie: string; local: string; idb: string };
    const ok = got.cookie.includes(EXPECT.cookie) && got.local === EXPECT.localValue && got.idb === EXPECT.idbValue;
    record("S7 readback", ok ? "pass" : "fail", String(back));
  } catch (e) { record("S7 readback", "inconclusive", (e as Error).message); }
}
await quit(l2, "S8 quit");

server.stop(true);
writeFileSync(join(outDir, "steps.json"), JSON.stringify({ app, slug, traced, recorded, port, steps }, null, 2));
const failed = steps.filter((s) => s.outcome !== "pass");
console.log(failed.length ? `${failed.length} step(s) not passing` : "all steps pass");
