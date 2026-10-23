#!/usr/bin/env bun
// Canonical permission fixture/actions with actual local AppKit controls.
// This test backend never claims external AX or visual acceptance.
// Run under the shared build/run lock; outputs never replace accepted goldens.
import { AgentCDP, verifyAgentEndpoint, executeCommand } from "../../../arc/tools/agent-cdp";
import { existsSync, lstatSync, mkdirSync, readFileSync, renameSync, writeFileSync } from "node:fs";
import { join, resolve } from "node:path";
import { summarize } from "../../tests/scenarios/coverage";

type Identity = { pid: number; ppid: number; born: string; command: string };
type Receipt = Record<string, unknown>;
type Step = {step: string; outcome: "pass" | "fail" | "inconclusive"; detail: string};
// BEGIN canonical opt-in read-only wait
// Clock injection supports semantic checks without launching the app or fixture.
type CanonicalPromptWaitObservation = {stage: string; purpose: "stage" | "before-reveal"; pollCount: number; elapsedMs: number; readyObserved: boolean};
async function waitCanonicalPromptInventory(poll: () => Promise<Receipt>, workDeadline: number,
    clock: {now: () => number; sleep: (ms: number) => Promise<unknown>}, first?: {row: Receipt; started: number}) {
  const started = first?.started ?? clock.now(), deadline = Math.min(started + 8000, workDeadline);
  let pollCount = 0, nextPollAt = started, readyObserved = false;
  for (let attempt = 0; attempt < 9; ++attempt) {
    let row: Receipt;
    if (attempt === 0 && first) { row = first.row; nextPollAt = started + 1000; }
    else {
      if (clock.now() >= workDeadline || clock.now() > deadline) break;
      if (nextPollAt > deadline) {
        await clock.sleep(Math.max(0, deadline - clock.now()));
        break;
      }
      await clock.sleep(Math.max(0, nextPollAt - clock.now()));
      if (clock.now() >= workDeadline || clock.now() > deadline) break;
      nextPollAt = clock.now() + 1000;
      row = await poll();
    }
    ++pollCount;
    if (clock.now() >= workDeadline || clock.now() > deadline) break;
    if (row.matchingGroupCount === 1 && row.exactMessageCount === 1 && row.notificationsIconCount === 1 && row.truncated === false) { readyObserved = true; break; }
  }
  // Exhausted global work authority cannot authorize a subsequent reveal/action.
  if (clock.now() >= workDeadline) throw new Error("canonical prompt observation exhausted work deadline; no subsequent native action");
  return {pollCount, elapsedMs: clock.now() - started, readyObserved};
}
// END canonical opt-in read-only wait

// Optional subset policy permits only stopped, already trusted baseline components.
type IdleGuardTarget = {targetId: string; type: string; url: string; browserContextId?: string};
function idleComponentSubset(targets: IdleGuardTarget[], baselineComponents: Map<string, IdleGuardTarget>,
    tombstones: Set<string>, fixtureTarget: string, fixtureURLs: Set<string>, baselinePages: Set<string>, blankURLs: Set<string>) {
  let fixtureMatchCount=0,unexpectedTargetCount=0,missingKnownServiceWorkerCount=0,missingKnownIdleEventPageCount=0,missingBackgroundPageCount=0;
  const newlyStopped: string[]=[];
  const sameComponent=(row: IdleGuardTarget, initial: IdleGuardTarget) => row.targetId===initial.targetId && row.type===initial.type && row.url===initial.url && row.browserContextId===initial.browserContextId;
  for (const row of targets) {
    // Tombstones precede every generic match, including browser and blank page.
    if (tombstones.has(row.targetId)) { ++unexpectedTargetCount; continue; }
    const initial=baselineComponents.get(row.targetId);
    if (initial) { if (sameComponent(row,initial)) continue; ++unexpectedTargetCount; continue; }
    if (row.targetId===fixtureTarget && row.type==="page" && fixtureURLs.has(row.url)) { ++fixtureMatchCount; continue; }
    if (row.type==="browser" && row.url==="") continue;
    if (row.type==="page" && baselinePages.has(row.targetId) && blankURLs.has(row.url)) continue;
    ++unexpectedTargetCount;
  }
  for (const [id,initial] of baselineComponents) {
    if (targets.some(row=>sameComponent(row,initial))) continue;
    if (initial.type==="service_worker") { ++missingKnownServiceWorkerCount; if (!tombstones.has(id)) newlyStopped.push(id); }
    else if (initial.type==="background_page" && initial.url==="chrome-extension://nkeimhogjdpnpccoofpliimaahmaaome/background.html") { ++missingKnownIdleEventPageCount; if (!tombstones.has(id)) newlyStopped.push(id); }
    else ++missingBackgroundPageCount;
  }
  const duplicateTargetIDs=new Set(targets.map(row=>row.targetId)).size!==targets.length;
  return {classification:{duplicateTargetIDs,missingKnownServiceWorkerCount,missingKnownIdleEventPageCount,missingBackgroundPageCount,unexpectedTargetCount,fixtureMatchCount},newlyStopped,
    valid:!duplicateTargetIDs && missingBackgroundPageCount===0 && unexpectedTargetCount===0 && fixtureMatchCount===1};
}

type Run = {fixtureSelectionDiagnostic?: Receipt; idleComponentStopCount?: number; targetGuardFailureClassifications?: Receipt[]; canonicalPromptWaitObservations?: CanonicalPromptWaitObservation[]; activationInvocationCount?: number; preparationInventoryID?: string; preparationInvocationCount?: number; preparationUncertain?: boolean; preparationReadyObserved?: boolean; preparationObservationElapsedMs?: number; preparationOutcomeConfirmed?: boolean; totalPhysicalActions?: number; revealAttemptCount?: number; revealUncertain?: boolean; revealPromptObserved?: boolean; revealOutcomeConfirmed?: boolean; revealBaselineInventoryID?: string; diagnosticInventories?: number; agent: string; pid?: number; errors: string[]; receipts: Receipt[]; steps: Step[];
  cleanup?: string; elapsedMs?: number; phase?: string; browserAliveOnFailure?: boolean;
  closeAcknowledged?: boolean; artifactBefore?: Record<string, string>; artifactAfter?: Record<string, string>; artifactsStable?: boolean; completedStages: string[]; uncertainMutation?: string; coverage?: unknown; profileIsolation?: {nonce: string; device: number; inode: number}};
const root = resolve(import.meta.dir, "../..");
const args = process.argv.slice(2);
const allowed = new Set(["--out", "--dylib", "--agents", "--port", "--record", "--scenario", "--diagnostic-only", "--reveal-prompt", "--foreground-launch", "--prepare-native-window", "--activate-native-app", "--site-center-ax-press", "--canonical-prompt-wait", "--fixture-selection-diagnostic", "--allow-idle-component-stop"]);
const options = new Map<string, string>();
for (let i = 0; i < args.length;) {
  const key = args[i]!;
  if (key === "--diagnostic-only" || key === "--reveal-prompt" || key === "--foreground-launch" || key === "--prepare-native-window" || key === "--activate-native-app" || key === "--site-center-ax-press" || key === "--canonical-prompt-wait" || key === "--fixture-selection-diagnostic" || key === "--allow-idle-component-stop") {
    if (options.has(key)) throw new Error(`duplicate ${key}`);
    options.set(key, "1"); ++i; continue;
  }
  const value = args[i + 1];
  if (!allowed.has(key) || options.has(key) || !value || value.startsWith("--")) throw new Error(`invalid option ${key}`);
  options.set(key, value); i += 2;
}
const diagnosticOnly = options.has("--diagnostic-only");
const revealPrompt = options.has("--reveal-prompt");
const foregroundLaunch = options.has("--foreground-launch");
const prepareNativeWindow = options.has("--prepare-native-window");
const activateNativeApp = options.has("--activate-native-app");
const siteCenterAXPress = options.has("--site-center-ax-press");
const canonicalPromptWait = options.has("--canonical-prompt-wait");
const fixtureSelectionDiagnostic = options.has("--fixture-selection-diagnostic");
const allowIdleComponentStop = options.has("--allow-idle-component-stop");
if (allowIdleComponentStop && diagnosticOnly) throw new Error("--allow-idle-component-stop requires normal mode");
if (fixtureSelectionDiagnostic && (diagnosticOnly || !canonicalPromptWait || revealPrompt || prepareNativeWindow || activateNativeApp || siteCenterAXPress)) throw new Error("--fixture-selection-diagnostic requires normal --canonical-prompt-wait without reveal/window/activation/modern strategies");
if (canonicalPromptWait && (diagnosticOnly || prepareNativeWindow || activateNativeApp)) throw new Error("--canonical-prompt-wait requires normal mode and excludes window preparation/activation");
if (siteCenterAXPress && (diagnosticOnly || !revealPrompt || prepareNativeWindow || activateNativeApp)) throw new Error("--site-center-ax-press requires normal --reveal-prompt and excludes window preparation/activation");
if (activateNativeApp && (diagnosticOnly || !prepareNativeWindow || !revealPrompt)) throw new Error("--activate-native-app requires normal --prepare-native-window and --reveal-prompt");
if (prepareNativeWindow && (diagnosticOnly || !revealPrompt)) throw new Error("--prepare-native-window requires --reveal-prompt and normal mode");
if (foregroundLaunch && (diagnosticOnly || !revealPrompt)) throw new Error("--foreground-launch requires --reveal-prompt and normal mode");
if (diagnosticOnly && revealPrompt) throw new Error("--diagnostic-only and --reveal-prompt are mutually exclusive");
if (!options.has("--out") || !options.has("--dylib")) {
  throw new Error("usage: bun arc/browser/ui_permission_controls_probe.ts --out NEW_DIR --dylib PATH [--agents trace156,ours156] [--port 9464] [--record 0|1] [--diagnostic-only] [--reveal-prompt] [--foreground-launch] [--prepare-native-window] [--activate-native-app] [--site-center-ax-press] [--canonical-prompt-wait] [--fixture-selection-diagnostic] [--allow-idle-component-stop]");
}
const out = resolve(options.get("--out")!);
const dylib = resolve(options.get("--dylib")!);
const agents = (options.get("--agents") ?? "trace156,ours156").split(",");
if (!agents.length || new Set(agents).size !== agents.length || agents.some(agent => !["trace156", "ours156"].includes(agent))) throw new Error("only exact disposable trace156/ours156 agents are allowed");
if (options.has("--record") && !["0", "1"].includes(options.get("--record")!)) throw new Error("--record must be 0 or 1");
const recorded = options.get("--record") !== "0";
const scenario = options.get("--scenario") ?? "permissions-deny";
if (!["permissions-deny", "cookies-storage-site-settings"].includes(scenario)) throw new Error("unsupported canonical permission scenario");
const expectedStages = scenario === "permissions-deny" ? ["deny"] : ["deny", "allow", "site-settings"];
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
const nonce = `arc-ui-permissions-${crypto.randomUUID()}`;
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
// Bind the exact accepted fixture authority. The shared diff currently leaves
// localhost ports literal; real server setup is fixed, transcripts are untouched.
const acceptedFixturePort = scenario === "permissions-deny" ? 50122 : 51436;
const canonicalBackend = `http://127.0.0.1:${fixturePort}`;
let frontend: ReturnType<typeof Bun.serve>;
try {
  frontend = Bun.serve({hostname: "127.0.0.1", port: acceptedFixturePort, async fetch(request) {
    const url = new URL(request.url);
    return fetch(canonicalBackend + url.pathname + url.search, {redirect: "manual"});
  }});
} catch (error) { await stopFixture(); throw error; }
const authority = `127.0.0.1:${acceptedFixturePort}`;
const base = `http://${authority}`;
const freshBase = `http://localhost:${acceptedFixturePort}`;
const fixtureURL = `${base}/a`;
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
    const requestPath = join(dir, "ui-permissions-request.json");
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
      {
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
      exec(["open", "-n", ...(foregroundLaunch ? [] : ["-g"]), "-a", app,
        "--env", `DYLD_INSERT_LIBRARIES=${recorded ? recorder + ":" : ""}${dylib}`,
        ...(recorded ? ["--env", `ARC_RECORD_PATH=${join(dir, "record.jsonl")}`] : []),
        "--env", `ARC_UI_PERMISSION_REQUEST=${requestPath}`, "--env", `ARC_UI_PERMISSION_NONCE=${nonce}`,
        "--env", `ARC_UI_PERMISSION_FIXTURE_AUTHORITY=${authority}`,
        ...(run.profileIsolation ? ["--env", `ARC_UI_PROFILE_ISOLATION_NONCE=${run.profileIsolation.nonce}`] : []),
        ...(diagnosticOnly ? ["--env", "ARC_UI_PERMISSION_DIAGNOSTIC_ONLY=1"] : ["--env", "ARC_UI_PERMISSION_ACTION_ENABLE=canonical-permissions-v1"]),
        ...(revealPrompt ? ["--env", "ARC_UI_PERMISSION_REVEAL_ENABLE=canonical-site-center-v1"] : []),
        ...(prepareNativeWindow ? ["--env", "ARC_UI_PERMISSION_KEY_WINDOW_ENABLE=canonical-key-window-v1"] : []),
        ...(activateNativeApp ? ["--env", "ARC_UI_PERMISSION_ACTIVATE_ENABLE=canonical-self-activate-v1"] : []),
        ...(siteCenterAXPress ? ["--env", "ARC_UI_PERMISSION_SITE_CENTER_AX_PRESS=canonical-site-center-ax-press-v1"] : []),
        "--env", `ARC_UI_PERMISSION_SCENARIO=${scenario}`, "--stdout", join(dir, "app.stdout.log"), "--stderr", join(dir, "app.stderr.log"), "--args",
        "--use-mock-keychain", "--no-default-browser-check", "--disable-sync", "--disable-background-networking",
        `--host-resolver-rules=${plist("ArcModHostResolverRules")}`, `--user-data-dir=${data}`,
        "--remote-debugging-address=127.0.0.1", `--remote-debugging-port=${port}`]);
      browser = await wait(() => {
        const matches = snapshot().filter(row => exact(row, exe));
        return matches.length === 1 ? matches[0] : undefined;
      }, 20000); run.pid = browser.pid;
      await wait(() => {
        const ready = json<{installed: boolean; pid: number; nonce: string; mainThread: boolean; actionCapabilityEnabled?: boolean; causalProfileIsolated?: boolean; diagnosticOnly?: boolean; revealCapabilityEnabled?: boolean; keyWindowPreparationEnabled?: boolean; nativeAppActivationEnabled?: boolean; siteCenterAXPressEnabled?: boolean}>(requestPath + ".ready.json");
        return ready.installed && ready.pid === browser!.pid && ready.nonce === nonce && ready.mainThread &&
          ready.actionCapabilityEnabled === !diagnosticOnly && (!diagnosticOnly || ready.diagnosticOnly === true) &&
          (!revealPrompt || ready.revealCapabilityEnabled === true) && (!prepareNativeWindow || ready.keyWindowPreparationEnabled === true) && (!activateNativeApp || ready.nativeAppActivationEnabled === true) && (!siteCenterAXPress || ready.siteCenterAXPressEnabled === true) && ready.causalProfileIsolated === true;
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
      const baseline = await inventoryTargets();
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
      const stoppedBaselineComponents = new Set<string>();
      const created = await client.request("Target.createTarget", {url: fixtureURL, background: true});
      if (typeof created.targetId !== "string") throw new Error("no owned canonical fixture target");
      const target = created.targetId;
      const verifyExclusiveFixtureTargets = async () => {
        const targets = await inventoryTargets();
        let exact = 0;
        const allowedFixtureURLs = new Set([fixtureURL, ...(scenario === "cookies-storage-site-settings" ? [`${freshBase}/a`] : [])]);
        const subset = idleComponentSubset(targets, baselineComponents, stoppedBaselineComponents, target, allowedFixtureURLs, baselinePages, blankURLs);
        const recordTargetGuardFailure = () => {
          (run.targetGuardFailureClassifications ??= []).push(subset.classification);
          writeFileSync(join(dir, "target-guard-failure.json"), JSON.stringify(subset.classification, null, 2), {mode: 0o600});
        };
        if (allowIdleComponentStop) {
          if (!subset.valid) { recordTargetGuardFailure(); throw new Error("trusted target subset guard refused; fixed classification saved"); }
          for (const id of subset.newlyStopped) stoppedBaselineComponents.add(id);
          run.idleComponentStopCount = stoppedBaselineComponents.size;
          return targets;
        }
        for (const row of targets) {
          if (row.targetId === target && row.type === "page" && allowedFixtureURLs.has(row.url)) { ++exact; continue; }
          if (row.type === "browser" && row.url === "") continue;
          if (row.type === "page" && baselinePages.has(row.targetId) && blankURLs.has(row.url)) continue;
          if (matchesBaselineComponent(row)) continue;
          recordTargetGuardFailure(); throw new Error("foreign page, popup or unexpected executable target; no modal action allowed");
        }
        if (new Set(targets.map(row => row.targetId)).size !== targets.length ||
            [...baselineComponents.keys()].some(id => !targets.some(row => row.targetId === id && matchesBaselineComponent(row)))) { recordTargetGuardFailure(); throw new Error("baseline component target changed; no native action allowed"); }
        if (exact !== 1) { recordTargetGuardFailure(); throw new Error("exact created fixture target is absent or ambiguous"); }
        return targets;
      };
      const evaluate = async (expression: string): Promise<unknown> => {
        const result = await client!.targetCommand(target, "Runtime.evaluate", {expression, returnByValue: true});
        if (result.exceptionDetails) throw new Error("canonical page evaluation threw");
        return (result.result as {value?: unknown} | undefined)?.value;
      };
      const pageIdentity = async () => await evaluate(`location.href===${JSON.stringify(fixtureURL)}&&document.title==='launch-slice A'&&!!document.getElementById('ls-marker-7f3a')`) === true;
      await wait(pageIdentity, 15000);
      run.steps.push({step: "fixture /a", outcome: "pass", detail: "canonical URL/title/marker"});
      let requestCount = 0;
      let activationSetupDeadline: number | undefined;
      let postRevealDiagnosticRequested = false;
      let fixtureSelectionRequested = false;
      let fixtureSelectionAcknowledged = false;
      const mutationIntents = new Set<string>();
      const verifyRevealProfileAndPID = () => {
        if (!browser || verifyAgentEndpoint(agent, port) !== browser.pid || !snapshot().some(row => same(row, browser!))) throw new Error("reveal captured process identity changed");
        const markerPath = join(applicationData, ".arc-ui-probe-profile.json");
        const directory = lstatSync(applicationData), markerFile = lstatSync(markerPath);
        const marker = json<{nonce: string; agent: string; device: number; inode: number}>(markerPath);
        if (!run.profileIsolation || !directory.isDirectory() || directory.isSymbolicLink() || !markerFile.isFile() || markerFile.isSymbolicLink() ||
            marker.nonce !== run.profileIsolation.nonce || marker.agent !== agent || marker.device !== directory.dev || marker.inode !== directory.ino ||
            directory.dev !== run.profileIsolation.device || directory.ino !== run.profileIsolation.inode) throw new Error("reveal fresh-profile marker/identity changed");
      };
      const pendingRevealFixture = async () => await evaluate(`location.href===${JSON.stringify(fixtureURL)}&&document.title==='launch-slice A'&&!!document.getElementById('ls-marker-7f3a')&&window.permissionResult==='pending'&&Notification.permission==='default'`) === true;
      const rpc = async (stage: string, operation: string): Promise<Receipt> => {
        if (++requestCount > 40) throw new Error("native request budget exceeded");
        if (!browser || verifyAgentEndpoint(agent, port) !== browser.pid || !snapshot().some(row => same(row, browser!))) throw new Error("native request captured-process identity changed");
        const id = `${nonce}-${requestCount}`;
        if (diagnosticOnly && operation !== "inventory") throw new Error("diagnostic-only driver refuses every native non-inventory operation");
        const includeFixtureSelection = fixtureSelectionDiagnostic && !fixtureSelectionRequested && stage === "deny" && operation === "inventory";
        let notificationNotStartedVerified = false;
        if (includeFixtureSelection) {
          verifyRevealProfileAndPID();
          notificationNotStartedVerified = await evaluate(`location.href===${JSON.stringify(fixtureURL)}&&document.title==='launch-slice A'&&!!document.getElementById('ls-marker-7f3a')&&typeof window.permissionResult==='undefined'&&Notification.permission==='default'`) === true;
          if (!notificationNotStartedVerified) throw new Error("selection diagnostic requires exact fixture/default permission before first Notification");
        }
        const causalFixtureVerified = includeFixtureSelection || diagnosticOnly || revealPrompt || !["inventory", "inspect-site-settings"].includes(operation);
        if (causalFixtureVerified) {
          const targets = await verifyExclusiveFixtureTargets();
          writeFileSync(join(dir, `${id}.targets.json`), JSON.stringify(targets, null, 2), {mode: 0o600});
        }
        let notificationPendingVerified = false;
        if (operation === "press-prompt-site-center") {
          if (!revealPrompt || diagnosticOnly || !run.revealBaselineInventoryID) throw new Error("reveal mode/baseline authority missing");
          notificationPendingVerified = await pendingRevealFixture();
          if (!notificationPendingVerified) throw new Error("reveal fixture is no longer the exact pending Notification request");
          verifyRevealProfileAndPID();
        }
        const keyWindowOperation = ["inventory-native-window", "activate-native-app", "prepare-native-window", "inspect-native-window"].includes(operation);
        let preparationNotificationPendingVerified = false;
        if (keyWindowOperation) {
          if (!prepareNativeWindow || diagnosticOnly) throw new Error("key-window setup requires explicit normal opt-in");
          preparationNotificationPendingVerified = await pendingRevealFixture();
          if (!preparationNotificationPendingVerified) throw new Error("key-window setup fixture changed or Notification is not pending/default");
          verifyRevealProfileAndPID();
        }
        const includeRevealDiagnostics = !diagnosticOnly && revealPrompt && operation === "inventory" && stage === "deny" &&
          run.revealAttemptCount === 1 && run.completedStages.length === 0 && run.receipts.at(-1)?.normalActionAttemptCount === 0 && !postRevealDiagnosticRequested;
        if (includeRevealDiagnostics) postRevealDiagnosticRequested = true;
        const request = {id, nonce, pid: browser.pid, stage, operation,
          causalFixtureVerified, isolatedProfileNonce: run.profileIsolation?.nonce,
          ...(includeRevealDiagnostics ? {includeRevealDiagnostics: true} : {}),
          ...(includeFixtureSelection ? {includeFixtureSelection: true, notificationNotStartedVerified} : {}),
          ...(keyWindowOperation ? {preparationNotificationPendingVerified, preparationInventoryID: run.preparationInventoryID} : {}),
          ...(operation === "press-prompt-site-center" ? {notificationPendingVerified, revealBaselineInventoryID: run.revealBaselineInventoryID} : {})};
        if (activateNativeApp && operation === "prepare-native-window" && (activationSetupDeadline === undefined || Date.now() >= activationSetupDeadline)) throw new Error("shared setup deadline expired after causal verification; refusing key mutation intent/request");
        if (!["inventory", "inspect-site-settings", "inventory-native-window", "inspect-native-window"].includes(operation)) {
          if (canonicalPromptWait && Date.now() >= workDeadline) throw new Error("canonical prompt wait exhausted work deadline after causal verification; refusing mutation intent");
          if (mutationIntents.has(operation)) throw new Error("mutation replay refused");
          mutationIntents.add(operation);
          run.uncertainMutation = run.preparationUncertain ? "prepare-native-window" : run.revealUncertain ? "press-prompt-site-center" : operation;
          writeFileSync(join(dir, `${id}.intent.json`), JSON.stringify({...request, retriesAllowed: false}), {mode: 0o600});
        }
        if (activateNativeApp && operation === "prepare-native-window" && (activationSetupDeadline === undefined || Date.now() >= activationSetupDeadline)) throw new Error("shared setup deadline expired before helper dispatch; refusing key request");
        if (canonicalPromptWait && Date.now() >= workDeadline) throw new Error("canonical prompt wait exhausted work deadline before helper dispatch");
        if (includeFixtureSelection) {
          verifyRevealProfileAndPID();
          if (await evaluate(`location.href===${JSON.stringify(fixtureURL)}&&document.title==='launch-slice A'&&!!document.getElementById('ls-marker-7f3a')&&typeof window.permissionResult==='undefined'&&Notification.permission==='default'`) !== true) throw new Error("pre-Notification selection scope changed before helper dispatch");
          verifyRevealProfileAndPID(); fixtureSelectionRequested = true;
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
        if (diagnosticOnly && (response.diagnosticOnly !== true || response.actionAttempted !== false || response.actionAttemptCount !== 0 || response.nativeActionsPerformed !== 0 || response.refusalReason)) throw new Error("diagnostic-only helper semantics/refusal mismatch");
        if (includeFixtureSelection) {
          const diagnostic = response.fixtureSelectionDiagnostic;
          if (response.readOnlyFixtureSelection !== true || response.fixtureSelectionScanCount !== 1 || response.fixtureSelectionFirstID !== id || response.actionAttempted !== false || response.actionAttemptCount !== 0 || response.nativeActionsPerformed !== 0 || response.refusalReason ||
              !diagnostic || typeof diagnostic !== "object" || Array.isArray(diagnostic) || (diagnostic as Receipt).readOnly !== true) throw new Error("one-shot pre-Notification selection diagnostic acknowledgment refused");
          run.fixtureSelectionDiagnostic = diagnostic as Receipt; fixtureSelectionAcknowledged = true;
        }
        if (includeRevealDiagnostics) {
          const diagnostic = response.revealDiagnostic;
          if (response.readOnlyRevealDiagnostics !== true || response.diagnosticOnly !== false || response.revealDiagnosticScanCount !== 1 || response.refusalReason ||
              !diagnostic || typeof diagnostic !== "object" || Array.isArray(diagnostic) ||
              (diagnostic as Receipt).diagnosticOnly !== true || (diagnostic as Receipt).functionalAcceptance !== false) throw new Error("single read-only post-reveal diagnostic acknowledgment missing or refused; no rescan");
        }
        if (prepareNativeWindow) {
          const prep = response.preparationInvocationCount, normal = response.normalActionAttemptCount, reveal = response.revealAttemptCount;
          const activation = activateNativeApp ? response.activationInvocationCount : 0;
          if (!Number.isInteger(activation) || (activation !== 0 && activation !== 1)) throw new Error("activation counter is invalid");
          if (activateNativeApp) run.activationInvocationCount = activation;
          if (!Number.isInteger(prep) || (prep !== 0 && prep !== 1) || !Number.isInteger(normal) || !Number.isInteger(reveal) ||
              response.totalPhysicalActions !== Number(activation) + Number(prep) + Number(normal) + Number(reveal) || response.totalPhysicalActions !== Number(activation) + Number(prep) + Number(response.actionAttemptCount)) throw new Error("preparation/physical counters disagree");
          run.preparationInvocationCount = prep;
          run.totalPhysicalActions = Number(response.totalPhysicalActions);
          if (keyWindowOperation && response.refusalReason) throw new Error("key-window preparation scope/refusal: " + String(response.refusalReason));
        }
        return response;
      };
      const readyStage = async (stage: string) => {
        if (canonicalPromptWait) {
          const observed = await waitCanonicalPromptInventory(() => rpc(stage, "inventory"), workDeadline, {now: Date.now, sleep});
          (run.canonicalPromptWaitObservations ??= []).push({stage, purpose: "stage", ...observed});
          if (observed.readyObserved) return;
          throw new Error(`canonical ${stage} native sheet not uniquely proven within bounded eight-second wait`);
        }
        for (let attempt = 0; attempt < 6; ++attempt) {
          const row = await rpc(stage, "inventory");
          if (row.matchingGroupCount === 1 && row.exactMessageCount === 1 && row.notificationsIconCount === 1 && row.truncated === false) return;
          await sleep(200);
        }
        throw new Error(`canonical ${stage} native sheet not uniquely proven`);
      };
      const press = async (stage: string, operation: string, count: number) => {
        const row = await rpc(stage, operation);
        if (row.actionAttempted !== true || row.actionAttemptCount !== count + (run.revealAttemptCount ?? 0) || row.nativeActionsPerformed !== count + (run.revealAttemptCount ?? 0) || !["performClick", "accessibilityPerformPress"].includes(String(row.nativeActionKind)) || row.refusalReason) throw new Error(`native ${operation} refused or unconfirmed`);
        if (revealPrompt && (row.normalActionAttemptCount !== count || row.revealAttemptCount !== (run.revealAttemptCount ?? 0))) throw new Error("normal/reveal native counters disagree");
        if (!run.revealUncertain && !run.preparationUncertain) delete run.uncertainMutation;
      };
      const assertPage = async (name: string, expression: string, identity = pageIdentity) => {
        await wait(async () => await evaluate(expression) === true, 5000);
        if (!await identity()) throw new Error("canonical page identity changed after action");
        run.steps.push({step: name, outcome: "pass", detail: diagnosticOnly ? "canonical page storage assertion; diagnostic only, no native control action" : "actual native controls followed by page assertion"});
      };
      const maybeRevealPrompt = async () => {
        const canonicalWaitStarted = canonicalPromptWait ? Date.now() : 0;
        const first = await rpc("deny", "inventory");
        if (first.truncated !== false) throw new Error("initial reveal inventory is incomplete");
        if (canonicalPromptWait && (Date.now() > Math.min(canonicalWaitStarted + 8000, workDeadline) || Date.now() >= workDeadline)) throw new Error("initial canonical inventory arrived after observation deadline; no reveal");
        if (first.matchingGroupCount === 1 && first.exactMessageCount === 1 && first.notificationsIconCount === 1) return;
        if (first.matchingGroupCount !== 0) throw new Error("initial permission scope is ambiguous; reveal forbidden");
        if (canonicalPromptWait) {
          const observed = await waitCanonicalPromptInventory(() => rpc("deny", "inventory"), workDeadline, {now: Date.now, sleep}, {row: first, started: canonicalWaitStarted});
          (run.canonicalPromptWaitObservations ??= []).push({stage: "deny", purpose: "before-reveal", ...observed});
          if (observed.readyObserved) return;
        }
        if (first.promptSiteCenterButtonCount !== 1 || first.revealFirstInventoryEligible !== true || typeof first.id !== "string" || first.revealFirstInventoryID !== first.id) throw new Error("first inventory has no unique eligible Site Control Center");
        run.revealBaselineInventoryID = first.id;
        run.revealUncertain = true;
        const row = await rpc("deny", "press-prompt-site-center");
        if (row.actionAttempted !== true || row.intentPersisted !== true || row.revealAttemptBudgetConsumed !== true || row.revealAttemptCount !== 1 || row.normalActionAttemptCount !== 0 || row.actionAttemptCount !== 1 || row.nativeActionsPerformed !== 1 || row.nativeMethodReturned !== true ||
            !["performClick", "accessibilityPerformPress"].includes(String(row.nativeActionKind)) || row.refusalReason ||
            (row.nativeActionKind === "accessibilityPerformPress" && typeof row.accessibilityPressReturn !== "boolean")) throw new Error("one reveal attempt refused or unconfirmed; no retry or fallback allowed");
        if (siteCenterAXPress && (row.nativeActionKind !== "accessibilityPerformPress" || typeof row.accessibilityPressReturn !== "boolean")) throw new Error("requested Site Control Center AX press strategy was not the actual attempted method");
        run.revealAttemptCount = 1;
      };
      const windowReady = (row: Receipt) => {
        const value = row.windowState;
        if (!value || typeof value !== "object" || Array.isArray(value)) return false;
        const state = value as Receipt;
        return state.truncated === false && state.uniqueSiteCenterCount === 1 && state.candidateWindowKnown === true && state.sameCandidateControl === true &&
          state.sameCandidateWindow === true && state.windowVisible === true && state.candidateScopeValid === true && state.keyOrMainReady === true &&
          ((state.windowIsKey === true && state.appKeyWindowMatches === true) || (state.windowIsMain === true && state.appMainWindowMatches === true));
      };
      run.phase = diagnosticOnly ? "canonical storage before read-only permission diagnostics" : "canonical storage and notification denial";
      if (diagnosticOnly) await verifyExclusiveFixtureTargets();
      await assertPage("cookie/local/session storage roundtrip", `(() => {
        document.cookie='scenario_cookie=c0ffee; path=/'; localStorage.setItem('scenario','41'); sessionStorage.setItem('scenario','93');
        return document.cookie.includes('scenario_cookie=c0ffee') && localStorage.getItem('scenario')==='41' && sessionStorage.getItem('scenario')==='93';
      })()`);
      if (diagnosticOnly) await verifyExclusiveFixtureTargets();
      if (fixtureSelectionDiagnostic) {
        await rpc("deny", "inventory");
        if (!fixtureSelectionAcknowledged) throw new Error("selection diagnostic did not complete before Notification request");
      }
      await evaluate("window.permissionResult='pending'; Notification.requestPermission().then(r=>window.permissionResult=r); 1");
      if (diagnosticOnly) {
        run.phase = "read-only local permission graph diagnostics";
        run.diagnosticInventories = 0;
        for (let attempt = 0; attempt < 6; ++attempt) {
          await rpc("deny", "inventory"); ++run.diagnosticInventories;
          if (attempt < 5) await sleep(200);
        }
        run.steps.push({step: "read-only permission diagnostics", outcome: "pass", detail: "Six bounded inventories; zero native control actions; no permission decision or functional acceptance claimed"});
      } else {
      if (prepareNativeWindow) {
        run.phase = "public same-window key preparation while Notification is pending";
        const initial = await rpc("deny", "inventory-native-window");
        if (initial.preparationCandidateCaptured !== true || typeof initial.preparationInventoryID !== "string" || initial.preparationInventoryID !== initial.id || initial.preparationInvocationCount !== 0) throw new Error("initial preparation inventory did not freeze one candidate");
        run.preparationInventoryID = initial.preparationInventoryID;
        run.preparationUncertain = true;
        const started = Date.now(), deadline = Math.min(started + 5000, workDeadline);
        if (activateNativeApp) activationSetupDeadline = deadline;
        try {
          if (activateNativeApp) {
            const activated = await rpc("deny", "activate-native-app");
            if (activated.preparationInventoryID !== run.preparationInventoryID || activated.activationInvocationCount !== 1 || activated.preparationInvocationCount !== 0 || activated.totalPhysicalActions !== 1 ||
                activated.actionAttempted !== true || activated.nativeActionKind !== "NSApplication.activate" || activated.nativeMethodReturned !== true ||
                activated.activationBudgetConsumed !== true || activated.activationIntentPersisted !== true) throw new Error("one public self-activation invocation is refused or unconfirmed");
            if (Date.now() >= deadline) throw new Error("shared five-second setup window expired after activation; no key invocation or fallback");
          }
          const prepared = await rpc("deny", "prepare-native-window");
          if (prepared.preparationInventoryID !== run.preparationInventoryID || prepared.preparationInvocationCount !== 1 || prepared.totalPhysicalActions !== 1 + (activateNativeApp ? 1 : 0) || prepared.actionAttempted !== true ||
              prepared.nativeActionKind !== "NSWindow.makeKeyWindow" || prepared.nativeMethodReturned !== true || prepared.preparationIntentPersisted !== true || prepared.preparationBudgetConsumed !== true) throw new Error("one public key-window invocation is refused or unconfirmed");
          let ready = false;
          while (Date.now() < deadline) {
            const inspected = await rpc("deny", "inspect-native-window");
            if (inspected.preparationInventoryID !== run.preparationInventoryID || inspected.actionAttempted !== false || inspected.preparationInvocationCount !== 1) throw new Error("read-only key-window observation identity/counter changed");
            if (Date.now() > deadline) break;
            if (windowReady(inspected)) { ready = true; break; }
            await sleep(Math.min(150, Math.max(0, deadline - Date.now())));
          }
          if (!ready) throw new Error("same known visible window did not become key or main within the five-second observation window; no fallback or further native actions");
          run.preparationReadyObserved = true;
        } finally { run.preparationObservationElapsedMs = Date.now() - started; }
      }
      if (revealPrompt) await maybeRevealPrompt();
      await readyStage("deny");
      if (run.revealAttemptCount === 1) run.revealPromptObserved = true;
      await press("deny", "press-deny", 1);
      await assertPage("deny notification prompt", "window.permissionResult==='denied'"); run.completedStages.push("deny");
      if (prepareNativeWindow) {
        if (run.preparationReadyObserved !== true) throw new Error("canonical denial lacks prior same-window readiness proof");
        run.preparationOutcomeConfirmed = true; run.preparationUncertain = false;
        if (!run.revealUncertain) delete run.uncertainMutation;
      }
      if (run.revealAttemptCount === 1 && run.revealPromptObserved === true) {
        run.revealOutcomeConfirmed = true; run.revealUncertain = false; delete run.uncertainMutation;
        run.steps.push({step: "optional prompt reveal", outcome: "pass", detail: "One public Site Control Center press, then newly qualified fixed-origin prompt and canonical denial page outcome"});
      }
      await assertPage("cookie deletion", "document.cookie='scenario_cookie=; Max-Age=0; path=/'; !document.cookie.includes('scenario_cookie=')");
      if (scenario === "cookies-storage-site-settings") {
        run.phase = "canonical fresh permission scope";
        const point = await evaluate(`(() => {const a=document.createElement('a');a.href=${JSON.stringify(freshBase + "/a")};a.textContent='Fresh permission origin';a.style='position:fixed;left:10px;top:10px';document.body.append(a);const r=a.getBoundingClientRect();return {x:r.x+r.width/2,y:r.y+r.height/2}})()`) as {x: number; y: number};
        await executeCommand(client, {name: "click", args: [target, String(point.x), String(point.y)]});
        await wait(async () => await evaluate(`location.origin===${JSON.stringify(freshBase)}`) === true, 10000);
        const freshIdentity = async () => await evaluate(`location.href===${JSON.stringify(freshBase + "/a")}&&document.title==='launch-slice A'&&!!document.getElementById('ls-marker-7f3a')`) === true;
        await evaluate("window.permissionResult='pending'; Notification.requestPermission().then(r=>window.permissionResult=r); 1");
        await assertPage("second origin starts with undecided permission", "Notification.permission==='default'", freshIdentity);
        await readyStage("allow"); await press("allow", "press-allow", 2);
        await assertPage("allow notification prompt", "window.permissionResult==='granted'", freshIdentity); run.completedStages.push("allow");
        run.phase = "canonical site settings";
        await wait(async () => {
          const row = await rpc("site-settings", "inventory");
          return row.truncated === false && row.ownerWindowAvailable === true && row.siteSettingsButtonCount === 1;
        }, 3000);
        await press("site-settings", "press-site-settings", 3);
        await wait(async () => {
          const row = await rpc("site-settings", "inspect-site-settings");
          return row.truncated === false && row.siteSettingsObserved === true && row.siteControlCenterInsecureSubtreeCount === 1;
        }, 5000);
        run.steps.push({step: "site settings reflects insecure HTTP fixture", outcome: "pass", detail: "actual native Site Control Center subtree contains Lock Off Insecure"});
        run.completedStages.push("site-settings");
      }
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
      writeFileSync(join(agentDir, "steps.json"), JSON.stringify({scenario, diagnosticOnly, functionalAcceptance: false, backend: diagnosticOnly ? "read-only local public AX diagnostics" : "local AppKit controls", steps: run.steps}, null, 2), {mode: 0o600});
      if (recorded) {
        try {
          const coverage = summarize(agentDir); run.coverage = coverage;
          if (!diagnosticOnly) {
            for (const error of coverage.errors) run.errors.push(`recorder summary: ${error}`);
            if (!coverage.accepted) run.errors.push("recorded scenario summary is not accepted");
          }
        } catch (error) { run.errors.push(`recorder summary: ${String(error)}`); }
      }
      writeFileSync(join(dir, "run.json"), JSON.stringify(run, null, 2), { mode: 0o600 });
    }
    if (stopAfterRun) break;
  }
} finally {
  frontend.stop(true);
  try { await stopFixture(); } catch (error) { runs.at(-1)?.errors.push(String(error)); }
}
const report = {nonce, scenario, backend: diagnosticOnly ? "read-only local public AX diagnostics" : "actual local AppKit controls", recorded,
  diagnosticOnly, revealPromptOptIn: revealPrompt, foregroundLaunch, prepareNativeWindow, activateNativeApp, siteCenterAXPress, canonicalPromptWait, fixtureSelectionDiagnostic, allowIdleComponentStop, functionalAcceptance: false, acceptanceClaimed: false,
  acceptedGoldenModified: false, visualAcceptance: false,
  runs: runs.map(run => ({agent: run.agent, pid: run.pid, errors: run.errors, steps: run.steps,
    phase: run.phase, ...(fixtureSelectionDiagnostic ? {fixtureSelectionDiagnostic: run.fixtureSelectionDiagnostic ?? null} : {}), ...(allowIdleComponentStop ? {idleComponentStopCount: run.idleComponentStopCount ?? 0} : {}), targetGuardFailureClassifications: run.targetGuardFailureClassifications ?? [], ...(canonicalPromptWait ? {canonicalPromptWaitObservations: run.canonicalPromptWaitObservations ?? []} : {}), activationInvocationCount: run.activationInvocationCount ?? 0, preparationInvocationCount: run.preparationInvocationCount ?? 0,
    totalPhysicalActions: run.totalPhysicalActions ?? null, preparationUncertain: run.preparationUncertain ?? false,
    preparationReadyObserved: run.preparationReadyObserved ?? false, preparationOutcomeConfirmed: run.preparationOutcomeConfirmed ?? false,
    preparationObservationElapsedMs: run.preparationObservationElapsedMs ?? null, revealAttemptCount: run.revealAttemptCount ?? 0, revealUncertain: run.revealUncertain ?? false,
    revealPromptObserved: run.revealPromptObserved ?? false, revealOutcomeConfirmed: run.revealOutcomeConfirmed ?? false, diagnosticInventories: run.diagnosticInventories ?? 0, completedStages: run.completedStages, uncertainMutation: run.uncertainMutation ?? null,
    coverage: run.coverage ?? null, profileIsolation: run.profileIsolation ?? null, browserAliveOnFailure: run.browserAliveOnFailure, artifactsStable: run.artifactsStable ?? false, cleanup: run.cleanup, elapsedMs: run.elapsedMs})),
  limitation: diagnosticOnly ? "Diagnostic collection only: canonical storage and first Notification.requestPermission are followed by six local inventories. No denial, grant, site settings, legacy action, engine completion or functional acceptance. Recorder coverage is observational and intentionally partial. Existing PID/profile/target guards, artifact hashing and cleanup still apply." : "Only the selected canonical permission sequence is exercised. The real fixture frontend uses its accepted fixed port; no output values are rewritten. Boundary diff and recorder noninterference are separate required checks; local controls do not establish external AX or visual acceptance."};
writeFileSync(join(out, "report.json"), JSON.stringify(report, null, 2), {mode: 0o600});
console.log(JSON.stringify(report, null, 2));
process.exitCode = runs.some(run => run.errors.length > 0 || run.revealUncertain === true || run.preparationUncertain === true ||
  (diagnosticOnly ? run.diagnosticInventories !== 6 || run.completedStages.length !== 0 : JSON.stringify(run.completedStages) !== JSON.stringify(expectedStages)) ||
  run.steps.some(step => step.outcome !== "pass")) ? 1 : 0;
