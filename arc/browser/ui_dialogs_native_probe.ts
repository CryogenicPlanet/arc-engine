#!/usr/bin/env bun
// Canonical dialogs fixture/actions with actual local AppKit controls.
// This test backend never claims external AX or visual acceptance.
// Run under the shared build/run lock; outputs never replace accepted goldens.
import { AgentCDP, verifyAgentEndpoint, executeCommand } from "../../../arc/tools/agent-cdp";
import { existsSync, lstatSync, mkdirSync, readFileSync, renameSync, writeFileSync } from "node:fs";
import { join, resolve } from "node:path";
import { summarize } from "../../tests/scenarios/coverage";

type Identity = { pid: number; ppid: number; born: string; command: string };
type Receipt = Record<string, unknown>;
type Step = {step: string; outcome: "pass" | "fail" | "inconclusive"; detail: string};
type Run = {agent: string; pid?: number; errors: string[]; receipts: Receipt[]; steps: Step[];
  cleanup?: string; elapsedMs?: number; phase?: string; browserAliveOnFailure?: boolean;
  closeAcknowledged?: boolean; artifactBefore?: Record<string, string>; artifactAfter?: Record<string, string>; artifactsStable?: boolean; completedStages: string[]; uncertainMutation?: string; coverage?: unknown; profileIsolation?: {nonce: string; device: number; inode: number}};
const root = resolve(import.meta.dir, "../..");
const args = process.argv.slice(2);
const allowed = new Set(["--out", "--dylib", "--agents", "--port", "--record", "--scenario"]);
const options = new Map<string, string>();
for (let i = 0; i < args.length; i += 2) {
  const key = args[i]!, value = args[i + 1];
  if (!allowed.has(key) || options.has(key) || !value || value.startsWith("--")) throw new Error(`invalid option ${key}`);
  options.set(key, value);
}
if (!options.has("--out") || !options.has("--dylib")) {
  throw new Error("usage: bun arc/browser/ui_dialogs_native_probe.ts --out NEW_DIR --dylib PATH [--agents trace156,ours156] [--port 9464] [--record 0|1]");
}
const out = resolve(options.get("--out")!);
const dylib = resolve(options.get("--dylib")!);
const agents = (options.get("--agents") ?? "trace156,ours156").split(",");
if (!agents.length || new Set(agents).size !== agents.length || agents.some(agent => !["trace156", "ours156"].includes(agent))) throw new Error("only exact disposable trace156/ours156 agents are allowed");
if (options.has("--record") && !["0", "1"].includes(options.get("--record")!)) throw new Error("--record must be 0 or 1");
const recorded = options.get("--record") !== "0";
const scenario = options.get("--scenario") ?? "dialogs";
if (!["dialogs", "dialogs-beforeunload", "dialogs-auth-cancel"].includes(scenario)) throw new Error("unsupported canonical dialog scenario");
const expectedStages = scenario === "dialogs" ? ["alert", "confirm", "prompt"] : scenario === "dialogs-beforeunload" ? ["beforeunload-stay", "beforeunload-leave"] : ["auth-cancel"];
const recorder = join(root, "tests/recorder/ArcEngineRecorder.dylib");
if (recorded && !existsSync(recorder)) throw new Error("existing boundary recorder dylib missing");
const port = Number(options.get("--port") ?? "9464");
if (!Number.isInteger(port) || port < 1024 || port > 65535) throw new Error("invalid CDP port");
if (!existsSync(dylib)) throw new Error("inventory dylib missing; build it first");
if (existsSync(out)) throw new Error("output directory already exists; choose a fresh directory");
const exec = (argv: string[], timeout = 2500): string => {
  const result = Bun.spawnSync(argv, { stdout: "pipe", stderr: "pipe", timeout });
  if (!result.success) throw new Error(`${argv[0]} failed: ${result.stderr.toString().trim()}`);
  return result.stdout.toString().trim();
};
// Never acquire, replace, or bypass another worker's build/run lock.
const lockPID = Number(readFileSync(join(root, "logs/arccore-build.lock/pid"), "utf8").trim());
if (!Number.isInteger(lockPID) || lockPID <= 1) throw new Error("invalid shared-lock owner");
let ancestor = process.pid;
for (let depth = 0; ancestor !== lockPID && ancestor > 1 && depth < 20; ++depth) ancestor = Number(exec(["ps", "-p", String(ancestor), "-o", "ppid="]));
if (ancestor !== lockPID) throw new Error("run under arc/tools/locked.sh; this probe never acquires or bypasses the lock");
const json = <T>(path: string): T => JSON.parse(readFileSync(path, "utf8")) as T;
const snapshot = (): Identity[] => exec(["ps", "-axo", "pid=,ppid=,lstart=,command="]).split("\n").flatMap(line => {
  const match = line.match(/^\s*(\d+)\s+(\d+)\s+(\S+\s+\S+\s+\d+\s+\d{2}:\d{2}:\d{2}\s+\d{4})\s+(.+)$/);
  return match ? [{ pid: Number(match[1]), ppid: Number(match[2]), born: match[3]!, command: match[4]! }] : [];
});
const exact = (row: Identity, exe: string) => row.command === exe || row.command.startsWith(exe + " ");
const same = (a: Identity, b: Identity) => a.pid === b.pid && a.born === b.born && a.command === b.command;
const sleep = (ms: number) => new Promise(resolveSleep => setTimeout(resolveSleep, ms));
const nonce = `arc-ui-dialogs-${crypto.randomUUID()}`;
// Exact canonical fixture implementation, kept in a separate process.
const fixtureProcess = Bun.spawn(["bun", join(root, "tests/scenarios/fixture-server.ts"),
  join(root, `tests/scenarios/${scenario}.ts`)], {stdout: "pipe", stderr: "pipe"});
const stopFixture = async () => {
  try { fixtureProcess.kill(); } catch { /* already exited */ }
  if (await Promise.race([fixtureProcess.exited.then(() => true), sleep(2000).then(() => false)])) return;
  try { fixtureProcess.kill("SIGKILL"); } catch { /* check exit below */ }
  if (!await Promise.race([fixtureProcess.exited.then(() => true), sleep(2000).then(() => false)])) throw new Error("owned fixture child survived bounded TERM/KILL cleanup");
};
let fixturePort: number;
try {
  const reader = fixtureProcess.stdout.getReader();
  const first = await Promise.race([reader.read(), sleep(5000).then(() => {throw new Error("fixture startup timeout");})]);
  if (first.done) throw new Error("canonical fixture exited before startup");
  fixturePort = Number(JSON.parse(new TextDecoder().decode(first.value)).port);
  if (!Number.isInteger(fixturePort) || fixturePort < 1024 || fixturePort > 65535) throw new Error("invalid fixture port");
  reader.releaseLock();
} catch (error) { await stopFixture(); throw error; }
const authority = `127.0.0.1:${fixturePort}`;
const base = `http://${authority}`;
const fixtureURL = `${base}/${scenario === "dialogs-auth-cancel" ? "auth" : "a"}`;
mkdirSync(out, { recursive: true, mode: 0o700 });
const runs: Run[] = [];
try {
  for (const agent of agents) {
    const run: Run = { agent, errors: [], receipts: [], steps: [], completedStages: [], phase: "preflight" }; runs.push(run);
    const agentDir = join(out, agent); mkdirSync(agentDir, { mode: 0o700 });
    const dir = join(agentDir, "launch-1"); mkdirSync(dir, { mode: 0o700 });
    const app = resolve(root, `../arc/build/ArcModAgent-${agent}.app`);
    const exe = join(app, "Contents/MacOS/Arc");
    const engine = join(app, "Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore");
    const artifacts = [exe, engine, dylib, ...(recorded ? [recorder] : [])];
    const artifactHashes = () => Object.fromEntries(artifacts.map(path => {
      const hash = exec(["shasum", "-a", "256", path], 10000).split(/\s/)[0]!;
      if (!/^[a-f0-9]{64}$/.test(hash)) throw new Error("invalid artifact hash");
      return [path, hash];
    }));
    const prefix = join(app, "Contents/Frameworks") + "/";
    const plist = (key: string) => exec(["/usr/libexec/PlistBuddy", "-c", `Print :${key}`, join(app, "Contents/Info.plist")]);
    const requestPath = join(dir, "ui-dialogs-request.json");
    let browser: Identity | undefined, client: AgentCDP | undefined;
    let launchRequested = false, stopAfterRun = false;
    const helpers = new Map<number, Identity>();
    // At most 65 seconds of work and 25 seconds of cleanup per launched app.
    const started = Date.now(), workDeadline = started + 65000, totalDeadline = started + 90000;
    const track = () => {
      if (!browser) return;
      const rows = snapshot();
      if (!rows.some(row => same(row, browser!))) return;
      const descendants = new Set([browser.pid]);
      let changed = true;
      while (changed) {
        changed = false;
        for (const row of rows) if (descendants.has(row.ppid) && !descendants.has(row.pid)) { descendants.add(row.pid); changed = true; }
      }
      for (const row of rows) if (row.pid !== browser.pid && descendants.has(row.pid) && row.command.startsWith(prefix)) helpers.set(row.pid, row);
    };
    const gone = () => {
      const rows = snapshot();
      return [...(browser ? [browser] : []), ...helpers.values()].every(old => !rows.some(row => same(row, old)));
    };
    const wait = async <T>(fn: () => T | Promise<T>, timeout: number, deadline = workDeadline): Promise<NonNullable<T>> => {
      const until = Math.min(Date.now() + timeout, deadline);
      let last: unknown;
      while (Date.now() < until) {
        try { track(); const value = await fn(); if (value) return value as NonNullable<T>; } catch (error) { last = error; }
        await sleep(Math.min(150, Math.max(0, until - Date.now())));
      }
      throw new Error(`bounded wait expired${last ? `: ${String(last)}` : ""}`);
    };
    const terminate = (identities: Identity[]) => {
      const rows = snapshot();
      for (const old of identities) if (rows.some(row => same(row, old))) Bun.spawnSync(["kill", "-TERM", String(old.pid)], { timeout: 1000 });
    };
    const connect = async () => {
      if (!browser || verifyAgentEndpoint(agent, port) !== browser.pid) throw new Error("endpoint PID mismatch");
      const response = await fetch(`http://127.0.0.1:${port}/json/version`, { redirect: "error", signal: AbortSignal.timeout(2000) });
      const version = await response.json() as { webSocketDebuggerUrl?: string };
      if (typeof version.webSocketDebuggerUrl !== "string") throw new Error("browser websocket absent");
      return AgentCDP.connectSocket(version.webSocketDebuggerUrl, port, 2000);
    };
    try {
      if (plist("ArcModAgentSession") !== agent || plist("CFBundleIdentifier") !== `company.thebrowser.Browser.mod.agent.${agent}` || plist("CFBundleName") !== `ArcModAgent-${agent}`) throw new Error("exact agent bundle identity mismatch");
      const exports = exec(["nm", "-gU", engine], 10000);
      const requiredClasses = readFileSync(join(root, "arc/tools/arccore-1.170.0-objc.tsv"), "utf8")
          .split("\n").filter(line => line.startsWith("C\t")).map(line => line.split("\t")[1]!);
      const missingClasses = requiredClasses.filter(name => !exports.includes(`_OBJC_CLASS_$_${name}\n`));
      if (missingClasses.length) throw new Error(`stale framework missing classes: ${missingClasses.join(", ")}`);
      if (snapshot().some(row => exact(row, exe) || row.command.startsWith(prefix))) throw new Error("agent browser/helpers already running; refusing attachment or termination");
      if (Bun.spawnSync(["/usr/sbin/lsof", "-nP", `-iTCP:${port}`, "-sTCP:LISTEN"], { stdout: "ignore", stderr: "ignore", timeout: 2000 }).success) throw new Error("CDP port occupied");
      run.artifactBefore = artifactHashes();
      writeFileSync(join(dir, "artifacts-before.json"), JSON.stringify(run.artifactBefore, null, 2), {mode: 0o600});
      const applicationData = join(process.env.HOME!, "Library/Application Support", `ArcModAgent-${agent}`);
      if (scenario !== "dialogs") {
        const isolatedNonce = process.env.ARC_UI_PROFILE_ISOLATION_NONCE;
        if (!isolatedNonce || !/^arc-ui-profile-[a-f0-9]{32}$/.test(isolatedNonce)) throw new Error("generic modal requires isolated profile wrapper");
        const markerPath = join(applicationData, ".arc-ui-probe-profile.json");
        const directory = lstatSync(applicationData), markerFile = lstatSync(markerPath);
        const marker = json<{nonce: string; agent: string; device: number; inode: number}>(markerPath);
        if (!directory.isDirectory() || directory.isSymbolicLink() || !markerFile.isFile() || markerFile.isSymbolicLink() ||
            marker.nonce !== isolatedNonce || marker.agent !== agent || marker.device !== directory.dev || marker.inode !== directory.ino) throw new Error("isolated profile marker/identity mismatch");
        run.profileIsolation = {nonce: isolatedNonce, device: directory.dev, inode: directory.ino};
      }
      const data = join(applicationData, "Agent Chromium Data");
      mkdirSync(data, { recursive: true, mode: 0o700 });
      exec(["defaults", "write", `company.thebrowser.Browser.mod.agent.${agent}`, "cloudKitSyncEnabled", "-bool", "false"]);
      exec(["defaults", "write", `company.thebrowser.Browser.mod.agent.${agent}`, "firebaseSyncEnabled", "-bool", "false"]);
      run.phase = "launch";
      launchRequested = true;
      exec(["open", "-n", "-g", "-a", app,
        "--env", `DYLD_INSERT_LIBRARIES=${recorded ? recorder + ":" : ""}${dylib}`,
        ...(recorded ? ["--env", `ARC_RECORD_PATH=${join(dir, "record.jsonl")}`] : []),
        "--env", `ARC_UI_DIALOG_REQUEST=${requestPath}`, "--env", `ARC_UI_DIALOG_NONCE=${nonce}`,
        "--env", `ARC_UI_DIALOG_FIXTURE_AUTHORITY=${authority}`,
        ...(run.profileIsolation ? ["--env", `ARC_UI_PROFILE_ISOLATION_NONCE=${run.profileIsolation.nonce}`] : []),
        "--env", "ARC_UI_DIALOG_ACTION_ENABLE=canonical-dialogs-v1", "--env", `ARC_UI_DIALOG_SCENARIO=${scenario}`, "--stdout", join(dir, "app.stdout.log"), "--stderr", join(dir, "app.stderr.log"), "--args",
        "--use-mock-keychain", "--no-default-browser-check", "--disable-sync", "--disable-background-networking",
        `--host-resolver-rules=${plist("ArcModHostResolverRules")}`, `--user-data-dir=${data}`,
        "--remote-debugging-address=127.0.0.1", `--remote-debugging-port=${port}`]);
      browser = await wait(() => {
        const matches = snapshot().filter(row => exact(row, exe));
        return matches.length === 1 ? matches[0] : undefined;
      }, 20000); run.pid = browser.pid;
      await wait(() => {
        const ready = json<{installed: boolean; pid: number; nonce: string; mainThread: boolean; actionCapabilityEnabled?: boolean; causalProfileIsolated?: boolean}>(requestPath + ".ready.json");
        return ready.installed && ready.pid === browser!.pid && ready.nonce === nonce && ready.mainThread && ready.actionCapabilityEnabled === true && (scenario === "dialogs" || ready.causalProfileIsolated === true);
      }, 20000);
      await wait(() => {
        const ready = json<{ready: boolean; pid: number}>(join(applicationData, "ArcModAgentReady.json"));
        return ready.ready === true && ready.pid === browser!.pid;
      }, 35000);
      run.phase = "connect after app-ready";
      client = await wait(connect, 15000);
      if (recorded) await wait(() => readFileSync(join(dir, "record.jsonl"), "utf8").split("\n").some(line => {
        try { const row = JSON.parse(line); return row.ev === "hooked" && row.engine_classes > 0; } catch { return false; }
      }), 5000);
      run.steps.push({step: "launch", outcome: "pass", detail: "PID-bound app/helper/CDP ready"});
      type TargetInfo = {targetId: string; type: string; url: string; browserContextId?: string};
      const inventoryTargets = async (): Promise<TargetInfo[]> => {
        const result = await client!.request("Target.getTargets");
        if (!Array.isArray(result.targetInfos) || result.targetInfos.length > 64) throw new Error("target inventory missing or unbounded");
        return result.targetInfos.map((row: unknown) => {
          const target = row as TargetInfo;
          if (!target || typeof target.targetId !== "string" || typeof target.type !== "string" || typeof target.url !== "string" || (target.browserContextId !== undefined && typeof target.browserContextId !== "string")) throw new Error("malformed target inventory");
          return target;
        });
      };
      const blankURLs = new Set(["", "about:blank", "chrome://newtab/", "chrome://new-tab-page/", "arc://newtab/"]);
      const baseline = scenario === "dialogs" ? [] : await inventoryTargets();
      // Built-in Hangouts, Contextual Tasks and Arc Internal Extension
      // can have these startup targets in a fresh profile. Only exact baseline
      // identities are allowed, never new/replaced extension targets or pages.
      const components = new Map([
        ["chrome-extension://nkeimhogjdpnpccoofpliimaahmaaome/background.html", "background_page"],
        ["chrome-extension://glbjnfimcajjenihimblfaponejbkoph/background.js", "service_worker"],
        ["chrome-extension://ldciclpbkaobfbicjebamaapbfplfjen/background.js", "service_worker"],
      ]);
      const isComponent = (row: TargetInfo) => components.get(row.url) === row.type;
      writeFileSync(join(dir, "target-baseline.json"), JSON.stringify(baseline, null, 2), {mode: 0o600});
      if (new Set(baseline.filter(isComponent).map(row => row.url)).size !== baseline.filter(isComponent).length ||
          new Set(baseline.map(row => row.targetId)).size !== baseline.length ||
          baseline.some(row => !(row.type === "page" && blankURLs.has(row.url)) && !(row.type === "browser" && row.url === "") && !isComponent(row))) throw new Error("isolated process has foreign or unexpected executable targets; no fixture action allowed");
      const baselinePages = new Set(baseline.filter(row => row.type === "page").map(row => row.targetId));
      const baselineComponents = new Map(baseline.filter(isComponent).map(row => [row.targetId, {...row}]));
      const matchesBaselineComponent = (row: TargetInfo) => {
        const initial = baselineComponents.get(row.targetId);
        return initial !== undefined && row.type === initial.type && row.url === initial.url &&
            row.browserContextId === initial.browserContextId;
      };
      const created = await client.request("Target.createTarget", {url: fixtureURL, background: true});
      if (typeof created.targetId !== "string") throw new Error("no owned canonical fixture target");
      const target = created.targetId;
      const verifyExclusiveFixtureTargets = async () => {
        const targets = await inventoryTargets();
        let exact = 0;
        const allowedFixtureURLs = new Set([fixtureURL, ...(scenario === "dialogs-beforeunload" ? [`${base}/b`] : []), ...(scenario === "dialogs-auth-cancel" ? [""] : [])]);
        for (const row of targets) {
          if (row.targetId === target && row.type === "page" && allowedFixtureURLs.has(row.url)) { ++exact; continue; }
          if (row.type === "browser" && row.url === "") continue;
          if (row.type === "page" && baselinePages.has(row.targetId) && blankURLs.has(row.url)) continue;
          if (matchesBaselineComponent(row)) continue;
          throw new Error("foreign page, popup or unexpected executable target; no modal action allowed");
        }
        if (new Set(targets.map(row => row.targetId)).size !== targets.length ||
            [...baselineComponents.keys()].some(id => !targets.some(row => row.targetId === id && matchesBaselineComponent(row)))) throw new Error("baseline component target changed; no native action allowed");
        if (exact !== 1) throw new Error("exact created fixture target is absent or ambiguous");
        return targets;
      };
      const evaluate = async (expression: string): Promise<unknown> => {
        const result = await client!.targetCommand(target, "Runtime.evaluate", {expression, returnByValue: true});
        if (result.exceptionDetails) throw new Error("canonical page evaluation threw");
        return (result.result as {value?: unknown} | undefined)?.value;
      };
      const pageIdentity = async () => await evaluate(`location.href===${JSON.stringify(fixtureURL)}&&document.title==='launch-slice A'&&!!document.getElementById('ls-marker-7f3a')`) === true;
      if (scenario !== "dialogs-auth-cancel") {
        await wait(pageIdentity, 15000);
        run.steps.push({step: "fixture /a", outcome: "pass", detail: "canonical URL/title/marker"});
      }
      let requestCount = 0;
      const mutationIntents = new Set<string>();
      const rpc = async (stage: string, operation: string): Promise<Receipt> => {
        if (++requestCount > 28) throw new Error("native request budget exceeded");
        if (!browser || verifyAgentEndpoint(agent, port) !== browser.pid || !snapshot().some(row => same(row, browser!))) throw new Error("native request captured-process identity changed");
        const id = `${nonce}-${requestCount}`;
        const causalFixtureVerified = operation !== "inventory" && scenario !== "dialogs";
        if (causalFixtureVerified) {
          const targets = await verifyExclusiveFixtureTargets();
          writeFileSync(join(dir, `${id}.targets.json`), JSON.stringify(targets, null, 2), {mode: 0o600});
        }
        const request = {id, nonce, pid: browser.pid, stage, operation,
          causalFixtureVerified, isolatedProfileNonce: run.profileIsolation?.nonce};
        if (operation !== "inventory") {
          if (mutationIntents.has(operation)) throw new Error("mutation replay refused");
          mutationIntents.add(operation); run.uncertainMutation = operation;
          writeFileSync(join(dir, `${id}.intent.json`), JSON.stringify({...request, retriesAllowed: false}), {mode: 0o600});
        }
        writeFileSync(requestPath + ".tmp", JSON.stringify(request), {mode: 0o600});
        renameSync(requestPath + ".tmp", requestPath);
        const response = await wait(() => {
          const row = json<Receipt>(requestPath + ".response.json");
          return row.id === id ? row : undefined;
        }, 2000);
        run.receipts.push(response);
        writeFileSync(join(dir, `${id}.response.json`), JSON.stringify(response, null, 2), {mode: 0o600});
        if (response.nonce !== nonce || response.pid !== browser.pid || response.mainThread !== true || response.functionalAcceptance !== false) throw new Error("native response identity/semantics mismatch");
        if (causalFixtureVerified && (response.causalProfileIsolated !== true || response.causalFixtureVerified !== true)) throw new Error("helper causal ownership verification missing");
        return response;
      };
      const readyStage = async (stage: string) => {
        for (let attempt = 0; attempt < 6; ++attempt) {
          const row = await rpc(stage, "inventory");
          if (row.matchingSheetCount === 1 && row.markerCount === 1 && row.titleCount === 1 && row.truncated === false && row.pruned === false) return;
          await sleep(200);
        }
        throw new Error(`canonical ${stage} native sheet not uniquely proven`);
      };
      const press = async (stage: string, operation: string, count: number) => {
        const row = await rpc(stage, operation);
        if (row.actionAttempted !== true || row.actionAttemptCount !== count || row.nativeActionsPerformed !== count || row.refusalReason) throw new Error(`native ${operation} refused or unconfirmed`);
        delete run.uncertainMutation;
      };
      const assertPage = async (name: string, expression: string, identity = pageIdentity) => {
        await wait(async () => await evaluate(expression) === true, 5000);
        if (!await identity()) throw new Error("canonical page identity changed after action");
        run.steps.push({step: name, outcome: "pass", detail: "actual native controls followed by page assertion"});
      };
      if (scenario === "dialogs") {
        // Expressions, order, labels, text and page assertions match dialogs.ts6-14.
        run.phase = "canonical alert";
        await evaluate("window.dialogResult='pending'; setTimeout(()=>{ alert('ArcCore alert 7f3a'); window.dialogResult='alert-returned' }, 50); 1");
        await sleep(400); await readyStage("alert"); await press("alert", "press-alert-ok", 1);
        await assertPage("alert native completion", "window.dialogResult==='alert-returned'"); run.completedStages.push("alert");
        run.phase = "canonical confirm";
        await evaluate("setTimeout(()=>{window.dialogResult=confirm('ArcCore confirm 7f3a')},50); 1");
        await sleep(400); await readyStage("confirm"); await press("confirm", "press-confirm-cancel", 2);
        await assertPage("confirm cancel", "window.dialogResult===false"); run.completedStages.push("confirm");
        run.phase = "canonical prompt";
        await evaluate("setTimeout(()=>{window.dialogResult=prompt('ArcCore prompt 7f3a','initial')},50); 1");
        await sleep(400); await readyStage("prompt");
        const edited = await rpc("prompt", "set-prompt-text");
        if (edited.textEditAttempted !== true || edited.textEditAttemptCount !== 1 || edited.textReadbackMatchesFixedReply !== true || edited.refusalReason) throw new Error("native prompt text edit refused or unconfirmed");
        delete run.uncertainMutation;
        await press("prompt", "press-prompt-ok", 3);
        await assertPage("prompt native text and completion", "window.dialogResult==='fixed prompt reply'"); run.completedStages.push("prompt");
      } else if (scenario === "dialogs-beforeunload") {
        run.phase = "canonical beforeunload stay";
        const point = await evaluate(`(() => {const b=document.createElement('button');b.textContent='Install beforeunload';b.style='position:fixed;top:10px;left:10px';
          b.onclick=()=>window.onbeforeunload=e=>{e.preventDefault();e.returnValue='fixture';return 'fixture'};document.body.append(b);
          const r=b.getBoundingClientRect();return {x:r.x+r.width/2,y:r.y+r.height/2}})()`) as {x: number; y: number};
        await executeCommand(client, {name: "click", args: [target, String(point.x), String(point.y)]});
        await evaluate(`setTimeout(()=>location.assign(${JSON.stringify(`${base}/b`)}),50);1`);
        await readyStage("beforeunload-stay");
        run.steps.push({step: "beforeunload prompt", outcome: "pass", detail: "actual canonical native sheet proved"});
        await press("beforeunload-stay", "press-beforeunload-cancel", 1);
        await assertPage("stay cancels navigation", `location.href===${JSON.stringify(`${base}/a`)}`);
        run.completedStages.push("beforeunload-stay");
        run.phase = "canonical beforeunload leave";
        await evaluate(`setTimeout(()=>location.assign(${JSON.stringify(`${base}/b`)}),50);1`);
        await readyStage("beforeunload-leave");
        await press("beforeunload-leave", "press-beforeunload-ok", 2);
        const destinationIdentity = async () => await evaluate(`location.href===${JSON.stringify(`${base}/b`)}&&document.title==='launch-slice B'&&!!document.getElementById('ls-marker-7f3a')`) === true;
        await assertPage("leave commits navigation", "document.title==='launch-slice B'", destinationIdentity);
        run.completedStages.push("beforeunload-leave");
      } else {
        run.phase = "canonical HTTP auth cancel";
        await readyStage("auth-cancel");
        run.steps.push({step: "HTTP auth prompt", outcome: "pass", detail: "actual title/message/secure-field native sheet proved"});
        await press("auth-cancel", "press-auth-cancel", 1);
        const cancelledIdentity = async () => await evaluate(`location.href===${JSON.stringify(fixtureURL)}&&document.body.innerText==='fixture auth required'`) === true;
        await assertPage("cancel displays fixed 401 response", "document.body.innerText==='fixture auth required'", cancelledIdentity);
        run.completedStages.push("auth-cancel");
      }
    } catch (error) {
      run.errors.push(`${run.phase}: ${String(error)}`);
      run.steps.push({step: run.phase ?? "scenario actions", outcome: "inconclusive", detail: String(error)});
      if (browser) { try { run.browserAliveOnFailure = snapshot().some(row => same(row, browser!)); } catch {} }
    }
    finally {
      if (!browser && launchRequested) {
        // Reconcile a partially successful open before deciding whether another
        // app can run. Only the exact executable absent at preflight qualifies.
        try {
          browser = await wait(() => {
            const matches = snapshot().filter(row => exact(row, exe));
            return matches.length === 1 ? matches[0] : undefined;
          }, 2500, Math.min(totalDeadline, Date.now() + 2500));
          run.pid = browser.pid;
        } catch {
          try {
            const leftovers = snapshot().filter(row => exact(row, exe) || row.command.startsWith(prefix));
            if (leftovers.length) {
              run.errors.push("launch ownership unresolved; refusing another paired launch or uncertain termination");
              stopAfterRun = true;
            }
          } catch {
            run.errors.push("identity snapshot unavailable after launch; refusing further launches");
            stopAfterRun = true;
          }
        }
      }
      // Cleanup never resolves a dialog or invokes a completion block. Browser
      // shutdown or exact captured process termination removes pending UI.
      if (browser) {
        try {
          track();
          client ??= await connect();
          run.closeAcknowledged = await client.request("Browser.close").then(() => true, () => false);
          await wait(gone, 7000, Math.min(totalDeadline, Date.now() + 7000));
          run.cleanup = `Captured browser/helpers exited; close request ${run.closeAcknowledged ? "acknowledged" : "not acknowledged"}`;
        } catch {
          terminate([browser]);
          try { await wait(gone, 4000, Math.min(totalDeadline, Date.now() + 4000)); }
          catch { terminate([...helpers.values()]); }
          try { await wait(gone, 7000, totalDeadline); run.cleanup = "SIGTERM of exact captured identities; owned browser/helpers exited"; }
          catch { run.errors.push("exact captured browser/helpers survived bounded cleanup"); stopAfterRun = true; }
        }
      }
      client?.close();
      if (run.artifactBefore) {
        try {
          run.artifactAfter = artifactHashes();
          run.artifactsStable = JSON.stringify(run.artifactBefore) === JSON.stringify(run.artifactAfter);
          writeFileSync(join(dir, "artifacts-after.json"), JSON.stringify(run.artifactAfter, null, 2), {mode: 0o600});
          if (!run.artifactsStable) { run.errors.push("app/engine/probe/recorder artifact changed during run"); stopAfterRun = true; }
        } catch (error) { run.errors.push(`artifact verification: ${String(error)}`); stopAfterRun = true; }
      }
      run.elapsedMs = Date.now() - started;
      if (run.elapsedMs > 90000) run.errors.push("per-app 90-second budget exceeded; inspect cleanup diagnostics");
      run.steps.push({step: "quit", outcome: run.closeAcknowledged && run.cleanup?.startsWith("Captured browser/helpers exited;") ? "pass" : "inconclusive", detail: run.cleanup ?? "cleanup unresolved"});
      writeFileSync(join(agentDir, "steps.json"), JSON.stringify({scenario, backend: "local AppKit controls", steps: run.steps}, null, 2), {mode: 0o600});
      if (recorded) {
        try {
          const coverage = summarize(agentDir); run.coverage = coverage;
          for (const error of coverage.errors) run.errors.push(`recorder summary: ${error}`);
          if (!coverage.accepted) run.errors.push("recorded scenario summary is not accepted");
        } catch (error) { run.errors.push(`recorder summary: ${String(error)}`); }
      }
      writeFileSync(join(dir, "run.json"), JSON.stringify(run, null, 2), { mode: 0o600 });
    }
    if (stopAfterRun) break;
  }
} finally {
  try { await stopFixture(); } catch (error) { runs.at(-1)?.errors.push(String(error)); }
}
const report = {nonce, scenario, backend: "actual local AppKit controls", recorded,
  acceptedGoldenModified: false, visualAcceptance: false,
  runs: runs.map(run => ({agent: run.agent, pid: run.pid, errors: run.errors, steps: run.steps,
    phase: run.phase, completedStages: run.completedStages, uncertainMutation: run.uncertainMutation ?? null,
    coverage: run.coverage ?? null, profileIsolation: run.profileIsolation ?? null, browserAliveOnFailure: run.browserAliveOnFailure, artifactsStable: run.artifactsStable ?? false, cleanup: run.cleanup, elapsedMs: run.elapsedMs})),
  limitation: "Only the selected canonical dialog sequence is exercised. Boundary diff and recorder noninterference are separate required checks; local controls do not establish external AX or visual acceptance."};
writeFileSync(join(out, "report.json"), JSON.stringify(report, null, 2), {mode: 0o600});
console.log(JSON.stringify(report, null, 2));
process.exitCode = runs.some(run => run.errors.length > 0 || JSON.stringify(run.completedStages) !== JSON.stringify(expectedStages) || run.steps.some(step => step.outcome !== "pass")) ? 1 : 0;
