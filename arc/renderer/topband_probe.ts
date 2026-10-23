#!/usr/bin/env bun
// Diagnostic only. Silence is not a nil callback or a parity result.
import { constants, copyFileSync, existsSync, mkdirSync, readFileSync, readdirSync, realpathSync, renameSync, statSync, writeFileSync } from "node:fs";
import { join, resolve } from "node:path";

const root = resolve(import.meta.dir, "../..");
const args = process.argv.slice(2);
const option = (name: string, fallback?: string) => {
  const index = args.indexOf(name);
  if (index < 0) return fallback;
  const value = args[index + 1];
  if (!value || value.startsWith("--")) throw new Error(`missing value for ${name}`);
  return value;
};
const outArg = option("--out");
if (!outArg) throw new Error("usage: topband_probe.sh run --out NEW_DIR [--agents trace156] [--mode default|base|arc|both] [--live36-toggle]");
const out = resolve(outArg);
const agents = option("--agents", "trace156")!.split(",");
if (!agents.length || new Set(agents).size !== agents.length || agents.some(a => !["trace156", "ours156"].includes(a))) throw new Error("invalid disposable agent list");
const mode = option("--mode", "default")!;
if (!["default", "base", "arc", "both"].includes(mode)) throw new Error("invalid mode");
const port = Number(option("--port", "9449"));
const observeMs = Number(option("--observe-ms", "1500"));
if (!Number.isInteger(port) || port < 1024 || port > 65535) throw new Error("invalid CDP port");
if (!Number.isInteger(observeMs) || observeMs < 500 || observeMs > 5000) throw new Error("observe-ms must be 500..5000");
const exec = (argv: string[]) => {
  const value = Bun.spawnSync(argv, { stdout: "pipe", stderr: "pipe" });
  if (!value.success) throw new Error(`${argv[0]} failed: ${value.stderr.toString().trim()}`);
  return value.stdout.toString().trim();
};
const lockPID = Number(readFileSync(join(root, "logs/arccore-build.lock/pid"), "utf8").trim());
let ancestor = process.pid;
for (let n = 0; ancestor !== lockPID && ancestor > 1 && n < 20; ++n) ancestor = Number(exec(["ps", "-p", String(ancestor), "-o", "ppid="]));
if (ancestor !== lockPID) throw new Error("run under arc/tools/locked.sh; this driver does not acquire a lock");
// Dynamic path keeps candidate compilation isolated. Canonical runtime uses
// the existing read-only agent CDP helper in the sibling arc checkout.
const { AgentCDP, verifyAgentEndpoint } = await import(resolve(root, "../arc/tools/agent-cdp.ts"));
type Client = InstanceType<typeof AgentCDP>;
const dylib = resolve(option("--dylib", join(root, "logs/wp/renderer-topband-probe-build/TopBandProbe.dylib"))!);
const recorderSource = join(root, "tests/recorder/ArcEngineRecorder.dylib");
for (const file of [dylib, recorderSource]) if (!existsSync(file)) throw new Error(`missing ${file}`);
mkdirSync(out, { recursive: true });
const recorder = join(out, "ArcEngineRecorder.frozen.dylib");
copyFileSync(recorderSource, recorder, constants.COPYFILE_EXCL);
const hash = (path: string) => exec(["/usr/bin/shasum", "-a", "256", path]).split(/\s/, 1)[0];
const recorderHash = hash(recorder);
if (hash(recorderSource) !== recorderHash) throw new Error("recorder changed during snapshot");
const save = (path: string, value: unknown) => writeFileSync(path, JSON.stringify(value, null, 2));
save(join(out, "recorder-provenance.json"), { source: recorderSource, frozen: recorder, sha256: recorderHash });
const provenance = (app: string, executable: string) => {
  const framework = join(app, "Contents/Frameworks/ArcCore.framework");
  const files = [executable, join(framework, "Versions/A/ArcCore"), dylib, recorder];
  const helpers = join(framework, "Helpers");
  for (const name of readdirSync(helpers).sort()) if (name.endsWith(".app")) {
    const bundle = join(helpers, name);
    const binary = exec(["/usr/libexec/PlistBuddy", "-c", "Print :CFBundleExecutable", join(bundle, "Contents/Info.plist")]);
    files.push(join(bundle, "Contents/MacOS", binary));
  }
  return files.map(path => {
    const realpath = realpathSync(path), stat = statSync(realpath);
    return { path, realpath, size: stat.size, mtimeMs: stat.mtimeMs, sha256: hash(realpath) };
  });
};
const appPIDs = (exe: string) => exec(["ps", "-axo", "pid=,command="]).split("\n").flatMap(line => {
  const match = line.match(/^\s*(\d+)\s+(.+)$/);
  return match && (match[2] === exe || match[2]!.startsWith(exe + " ")) ? [Number(match[1])] : [];
});
const alive = (pid: number) => Bun.spawnSync(["kill", "-0", String(pid)], { stderr: "ignore" }).success;
const sleep = (ms: number) => new Promise(r => setTimeout(r, ms));
const wait = async <T>(fn: () => T | Promise<T>, ms = 20000): Promise<NonNullable<T>> => {
  const deadline = Date.now() + ms;
  let error: unknown;
  while (Date.now() < deadline) {
    try { const result = await fn(); if (result) return result as NonNullable<T>; } catch (e) { error = e; }
    await sleep(100);
  }
  throw new Error(`timed out after ${ms} ms${error ? `: ${String(error)}` : ""}`);
};
const json = (path: string) => JSON.parse(readFileSync(path, "utf8"));
const token = `arc-topband-probe-${crypto.randomUUID()}`;
const fixture = (kind: string) => {
  const shell = `<meta charset="utf-8"><title>Top-band public callback fixture</title>
    <style>html{background:white}body{margin:0;min-height:3000px}#label{position:absolute;top:120px;left:40px;font:24px sans-serif}</style>
    <script>globalThis.__paints=[];globalThis.__lcp=[];
      new PerformanceObserver(l=>__paints.push(...l.getEntries().map(e=>({name:e.name,startTime:e.startTime})))).observe({type:'paint',buffered:true});
      new PerformanceObserver(l=>__lcp.push(...l.getEntries().map(e=>({startTime:e.startTime,size:e.size,tag:e.element?.tagName})))).observe({type:'largest-contentful-paint',buffered:true});</script>`;
  const redImage = "data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='400' height='64'%3E%3Crect width='400' height='64' fill='red'/%3E%3C/svg%3E";
  const band = "position:absolute;left:1px;right:1px;top:0;height:64px;background:red";
  let content = "";
  if (kind === "root-red") content = "<style>html{background:rgb(255,0,0)}</style>";
  else if (kind === "root-blue") content = "<style>html{background:rgb(0,0,255)}</style>";
  else if (kind === "interior" || kind === "scroll") content = `<div id="band" style="${band}"></div>`;
  else if (kind === "fullbleed") content = `<div id="band" style="${band};left:0;right:0"></div>`;
  else if (kind === "image") content = `<img id="band" src="${redImage}" style="${band};width:calc(100% - 2px)">`;
  else if (kind === "gradient") content = `<div id="band" style="${band};background:linear-gradient(red,red)"></div>`;
  else if (kind === "canvas") content = `<canvas id="band" width="400" height="64" style="${band};background:transparent;width:calc(100% - 2px)"></canvas>
    <script>const c=document.getElementById('band').getContext('2d');c.fillStyle='red';c.fillRect(0,0,400,64);</script>`;
  if (kind === "scroll") content += "<div style='position:absolute;left:1px;right:1px;top:64px;height:2900px;background:green'></div>";
  return "<!doctype html>" + shell + content + `<div id="label">Public top-band fixture: ${kind}</div>`;
};
const server = Bun.serve({ hostname: "127.0.0.1", port: 0, fetch(request) {
  const path = new URL(request.url).pathname;
  if (!path.startsWith(`/${token}/`)) return new Response("not found", { status: 404 });
  return new Response(fixture(path.split("/").at(-1)!), { headers: { "content-type": "text/html", "cache-control": "no-store" } });
} });
const base = `http://127.0.0.1:${server.port}/${token}`;
const runs: any[] = [];
try {
  for (const agent of agents) {
    const dir = join(out, agent);
    mkdirSync(dir, { recursive: true });
    const app = resolve(root, `../arc/build/ArcModAgent-${agent}.app`);
    const exe = join(app, "Contents/MacOS/Arc");
    const plist = (key: string) => exec(["/usr/libexec/PlistBuddy", "-c", `Print :${key}`, join(app, "Contents/Info.plist")]);
    const requestPath = join(dir, "topband-request.json");
    const run: any = { agent, mode, observeMs, stages: [], acknowledgments: [], harnessComplete: false, callbackActivity: false, parity: null };
    runs.push(run);
    let pid: number | undefined, cdp: Client | undefined, target: string | undefined;
    let currentURL = base + "/blank";
    let before: ReturnType<typeof provenance> | undefined;
    let armed = false, requestNumber = 0;
    const action = async (operation: string, extra: Record<string, unknown> = {}, restoration = false) => {
      if (!pid) throw new Error("no launched PID");
      if (restoration) {
        // The unique request file is already bound to our injected process.
        // Restore through its public API even if the CDP endpoint disconnected.
        if (operation !== "disarm" || !appPIDs(exe).includes(pid)) throw new Error("original app PID unavailable for restoration");
      } else if (!cdp || verifyAgentEndpoint(agent, port) !== pid) throw new Error("endpoint PID changed");
      const id = `${token}-${++requestNumber}`;
      const sentAt = Date.now() / 1000;
      writeFileSync(requestPath + ".tmp", JSON.stringify({ id, url: currentURL, operation, ...extra }));
      renameSync(requestPath + ".tmp", requestPath);
      const response = await wait(() => { const value = json(requestPath + ".response.json"); return value.id === id ? value : undefined; });
      run.acknowledgments.push({ ...response, sentAt, receivedAt: Date.now() / 1000 });
      if (response.pid !== pid || response.ok !== true) throw new Error(`probe action failed: ${JSON.stringify(response)}`);
      return response;
    };
    try {
      if (plist("ArcModAgentSession") !== agent || plist("CFBundleIdentifier") !== `company.thebrowser.Browser.mod.agent.${agent}`) throw new Error("agent identity mismatch");
      if (appPIDs(exe).length) throw new Error("agent already running; refusing attachment or termination");
      if (Bun.spawnSync(["/usr/sbin/lsof", "-nP", `-iTCP:${port}`, "-sTCP:LISTEN"], { stdout: "ignore", stderr: "ignore" }).success) throw new Error("debug port occupied");
      before = provenance(app, exe);
      if (before.find(item => item.path === recorder)?.sha256 !== recorderHash) throw new Error("frozen recorder changed before launch");
      save(join(dir, "artifact-provenance-before.json"), before);
      const data = join(process.env.HOME!, "Library/Application Support", `ArcModAgent-${agent}`);
      const chromiumData = join(data, "Agent Chromium Data");
      mkdirSync(chromiumData, { recursive: true, mode: 0o700 });
      for (const key of ["cloudKitSyncEnabled", "firebaseSyncEnabled"]) exec(["defaults", "write", `company.thebrowser.Browser.mod.agent.${agent}`, key, "-bool", "false"]);
      const flags = ["--use-mock-keychain", "--no-default-browser-check", "--disable-sync", "--disable-background-networking",
        `--host-resolver-rules=${plist("ArcModHostResolverRules")}`, `--user-data-dir=${chromiumData}`,
        "--remote-debugging-address=127.0.0.1", `--remote-debugging-port=${port}`,
        ...(["base", "both"].includes(mode) ? ["--enable-features=TopBandColorSampling"] : [])];
      save(join(dir, "launch.json"), { mode, flags, recorderHash, arc36BeforeContextRequested: ["arc", "both"].includes(mode) });
      exec(["open", "-n", "-g", "-a", app, "--env", `DYLD_INSERT_LIBRARIES=${recorder}:${dylib}`,
        "--env", `ARC_TOPBAND_PROBE_REQUEST=${requestPath}`, "--env", `ARC_RECORD_PATH=${join(dir, "record.jsonl")}`,
        ...(["arc", "both"].includes(mode) ? ["--env", "ARC_TOPBAND_PUBLIC36=1"] : []),
        "--stdout", join(dir, "app.log"), "--stderr", join(dir, "app.log"), "--args", ...flags]);
      pid = await wait(() => appPIDs(exe)[0], 30000);
      run.pid = pid;
      await wait(() => {
        const ready = json(requestPath + ".ready.json"), appReady = json(join(data, "ArcModAgentReady.json"));
        return ready.pid === pid && ready.installed && appReady.pid === pid && appReady.ready;
      }, 60000);
      run.surface = json(requestPath + ".ready.json").surface;
      if (run.surface.observerCallback !== "v36@0:8@16@24B32" || run.surface["setTopBandColorDelegate:"] !== "v24@0:8@16" ||
          ["r", "g", "b", "a"].some(key => run.surface[`ArcColor.${key}`] !== "C16@0:8")) throw new Error("top-band public metadata mismatch");
      await wait(() => verifyAgentEndpoint(agent, port) === pid, 30000);
      cdp = await AgentCDP.connectAgent(agent, port);
      const created = await cdp.request("Target.createTarget", { url: currentURL });
      if (typeof created.targetId !== "string") throw new Error("no fresh fixture target");
      target = created.targetId;
      const evaluate = async (expression: string) => {
        if (verifyAgentEndpoint(agent, port) !== pid) throw new Error("endpoint PID changed");
        const result = await cdp!.targetCommand(target!, "Runtime.evaluate", { expression, returnByValue: true, awaitPromise: true });
        if (result.exceptionDetails) throw new Error(`fixture JS failed: ${JSON.stringify(result.exceptionDetails)}`);
        return (result.result as any)?.value;
      };
      await wait(async () => await evaluate(`location.href===${JSON.stringify(currentURL)}&&document.readyState==='complete'`));
      // An uncertain acknowledgment can follow a successful installation.
      // Conservatively restore even when arm itself times out or reports error.
      armed = true;
      const arm = await action("arm");
      if (["arc", "both"].includes(mode) && !arm.arc36SetBeforeFirstObservedContextCall) throw new Error("requested pre-profile Arc36 injection was not observed");
      const sample = async (name: string) => {
        const start = Date.now();
        await sleep(observeMs);
        await evaluate("new Promise(r=>requestAnimationFrame(()=>requestAnimationFrame(r)))");
        const geometry = await evaluate(`({href:location.href,dpr:devicePixelRatio,width:innerWidth,height:innerHeight,scrollY,
          paints:__paints,lcp:__lcp,visibility:document.visibilityState,htmlBackground:getComputedStyle(document.documentElement).backgroundColor,
          band:(()=>{const e=document.getElementById('band');if(!e)return null;const r=e.getBoundingClientRect();const s=getComputedStyle(e);
            return {x:r.x,y:r.y,width:r.width,height:r.height,position:s.position,background:s.backgroundColor,backgroundImage:s.backgroundImage};})()})`);
        const capture = await cdp!.targetCommand(target!, "Page.captureScreenshot", { format: "png" });
        if (typeof capture.data !== "string") throw new Error("missing screenshot");
        const filename = `${String(run.stages.length + 1).padStart(2, "0")}-${name}.png`;
        writeFileSync(join(dir, filename), Buffer.from(capture.data, "base64"));
        const state = await action("drain");
        run.stages.push({ name, observationStartedMs: start, observationEndedMs: Date.now(), geometry, screenshot: filename, eventCount: state.eventCount, interference: state.interference });
        save(join(dir, "observations.json"), run);
        console.log(`${agent}: ${name}, callbacks=${state.eventCount}, interference=${state.interference}`);
      };
      const navigate = async (name: string, kind = name) => {
        await action("stage", { stage: name });
        currentURL = base + "/" + kind;
        await cdp!.targetCommand(target!, "Page.navigate", { url: currentURL });
        await wait(async () => await evaluate(`location.href===${JSON.stringify(currentURL)}&&document.readyState==='complete'&&[...document.images].every(i=>i.complete&&i.naturalWidth>0)`));
        await sample(name);
      };
      await navigate("root-red");
      await action("stage", { stage: "same-red" });
      await evaluate("document.documentElement.style.backgroundColor='rgb(255,0,0)'");
      await sample("same-red");
      await navigate("root-blue");
      for (const name of ["interior", "fullbleed", "image", "gradient", "canvas", "scroll"]) await navigate(name);
      await action("stage", { stage: "scroll-document-top" });
      await evaluate("scrollTo(0,400)");
      await wait(async () => (await evaluate("scrollY")) >= 300);
      await sample("scroll-document-top");
      await action("stage", { stage: "fixed-blue" });
      await evaluate("(()=>{const e=document.createElement('div');e.id='band';e.style='position:fixed;left:1px;right:1px;top:0;height:64px;background:blue;z-index:2';document.getElementById('band').remove();document.body.append(e);})()");
      await sample("fixed-blue");
      if (args.includes("--live36-toggle")) for (const enabled of [false, true]) {
        const stage = `live36-${enabled ? "on" : "off"}`;
        await action("stage", { stage });
        await action("feature36", { enabled });
        await evaluate(`document.getElementById('band').style.backgroundColor=${JSON.stringify(enabled ? "purple" : "orange")}`);
        await sample(stage);
      }
      await action("disarm");
      armed = false;
      run.restoreStatus = "public disarm acknowledged before target closure";
      await cdp.request("Target.closeTarget", { targetId: target });
      target = undefined;
      run.harnessComplete = true;
    } catch (error) {
      run.error = String(error);
      console.error(`${agent}: ${run.error}`);
    } finally {
      if (armed && pid) {
        run.restoreAttempts = [];
        for (let attempt = 0; armed && attempt < 2; ++attempt) {
          if (!alive(pid)) {
            armed = false;
            run.restoreStatus = "original process already exited; no live delegate remains";
            break;
          }
          try {
            await action("disarm", {}, true);
            armed = false;
            run.restoreStatus = "public disarm acknowledged during cleanup";
          } catch (error) { run.restoreAttempts.push(String(error)); }
        }
        if (armed) {
          run.restoreError = "public delegate restoration was not acknowledged; normal target/app closure skipped";
          run.harnessComplete = false;
        }
      }
      if (pid) {
        try {
          if (armed || run.restoreError) throw new Error("restoration unresolved; terminate only our disposable PID");
          if (verifyAgentEndpoint(agent, port) !== pid) throw new Error("endpoint changed before cleanup");
          cdp ??= await AgentCDP.connectAgent(agent, port);
          await cdp.request("Browser.close").catch(() => {});
          await wait(() => !alive(pid!), 15000);
          run.cleanup = "Browser.close; original launched PID exited";
        } catch {
          if (appPIDs(exe).includes(pid)) Bun.spawnSync(["kill", "-TERM", String(pid)]);
          try { await wait(() => !alive(pid!), 15000); run.cleanup = run.restoreError
            ? "SIGTERM of original launched PID after failed restoration; exited"
            : "SIGTERM of original launched PID; exited"; }
          catch { run.cleanupError = "original launched PID still alive"; run.harnessComplete = false; }
        }
      }
      cdp?.close();
      if (before) try {
        const after = provenance(app, exe);
        save(join(dir, "artifact-provenance-after.json"), after);
        if (JSON.stringify(before) !== JSON.stringify(after)) { run.provenanceError = "artifacts changed during run"; run.harnessComplete = false; }
      } catch (error) { run.provenanceError = String(error); run.harnessComplete = false; }
      if (existsSync(requestPath + ".events.json")) {
        const observed = json(requestPath + ".events.json");
        run.callbackCount = observed.events.length;
        run.callbackActivity = observed.events.length > 0;
        run.observationIntegrity = !observed.truncated && !observed.diagnostics.length && observed.events.every((event: any) => event.sourceMatches);
        run.arc36SetBeforeFirstObservedContextCall = observed.arc36SetBeforeFirstObservedContextCall;
      } else run.observationIntegrity = false;
      if (run.observationIntegrity !== true) run.harnessComplete = false;
      save(join(dir, "observations.json"), run);
    }
  }
} finally { server.stop(true); }
const complete = runs.length === agents.length && runs.every(run => run.harnessComplete && run.observationIntegrity === true && !run.error && !run.restoreError && !run.provenanceError && !!run.cleanup && !run.cleanupError);
const summary = { harnessComplete: complete, callbackActivity: runs.some(run => run.callbackActivity), parity: null,
  interpretation: "Diagnostic only. No callbacks means silence, not a nil color. Counts/crossfade/parity require original-engine review.", runs };
save(join(out, "summary.json"), summary);
console.log(JSON.stringify({ harnessComplete: complete, callbackActivity: summary.callbackActivity, parity: null, out }));
process.exitCode = complete ? 0 : 1;
