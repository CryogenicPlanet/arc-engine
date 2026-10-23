#!/usr/bin/env bun
// Paired, read-only local-control capability probe. Run only under the shared
// build/run lock. This does not press controls or establish functional acceptance.
import { AgentCDP, verifyAgentEndpoint } from "../../../arc/tools/agent-cdp";
import { existsSync, mkdirSync, readFileSync, renameSync, writeFileSync } from "node:fs";
import { join, resolve } from "node:path";

type Identity = { pid: number; ppid: number; born: string; command: string };
type Inventory = { id: string; nonce: string; pid: number; mainThread: boolean;
  fixtureMarkerCount: number; fixtureContainerCount: number; fixtureButtonCount: number;
  fixtureNSButtonCount: number; fixtureAccessibilityPressResponderCount: number;
  nativeActionsPerformed: number; functionalAcceptance: boolean; truncated: boolean;
  nodes: unknown[]; foregroundBefore: number; foregroundAfter: number };
type Run = { agent: string; pid?: number; errors: string[]; observations: Inventory[];
  cleanup?: string; elapsedMs?: number; pageAlertScheduled?: boolean; phase?: string;
  browserAliveOnFailure?: boolean; reconnects?: number; closeAcknowledged?: boolean };
const root = resolve(import.meta.dir, "../..");
const args = process.argv.slice(2);
const allowed = new Set(["--out", "--dylib", "--agents", "--port"]);
const options = new Map<string, string>();
for (let i = 0; i < args.length; i += 2) {
  const key = args[i]!, value = args[i + 1];
  if (!allowed.has(key) || options.has(key) || !value || value.startsWith("--")) throw new Error(`invalid option ${key}`);
  options.set(key, value);
}
if (!options.has("--out") || !options.has("--dylib")) {
  throw new Error("usage: bun arc/browser/ui_control_inventory.ts --out NEW_DIR --dylib PATH [--agents trace156,ours156] [--port 9464]");
}
const out = resolve(options.get("--out")!);
const dylib = resolve(options.get("--dylib")!);
const agents = (options.get("--agents") ?? "trace156,ours156").split(",");
if (!agents.length || new Set(agents).size !== agents.length || agents.some(agent => !["trace156", "ours156"].includes(agent))) throw new Error("only exact disposable trace156/ours156 agents are allowed");
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
const nonce = `arc-ui-control-inventory-${crypto.randomUUID()}`;
const alertMessage = `Arc UI inventory ${nonce}`;
const fixturePath = `/${nonce}/fixture`;
const fixture = Bun.serve({ hostname: "127.0.0.1", port: 0, fetch(request) {
  if (new URL(request.url).pathname !== fixturePath) return new Response("not found", { status: 404 });
  return new Response(`<!doctype html><meta charset="utf-8"><title>UI inventory fixture</title><main>UI inventory fixture</main>
<script>window.inventoryNonce=${JSON.stringify(nonce)};window.inventoryAlertState='idle';
window.triggerInventoryAlert=()=>{if(window.inventoryAlertState!=='idle')throw Error('already scheduled');
window.inventoryAlertState='scheduled';setTimeout(()=>{window.inventoryAlertState='open';alert(${JSON.stringify(alertMessage)});window.inventoryAlertState='returned'},100);return window.inventoryNonce};</script>`,
    { headers: { "content-type": "text/html; charset=utf-8", "cache-control": "no-store" } });
} });
const fixtureURL = `http://127.0.0.1:${fixture.port}${fixturePath}`;
mkdirSync(out, { recursive: true, mode: 0o700 });
const runs: Run[] = [];
try {
  for (const agent of agents) {
    const run: Run = { agent, errors: [], observations: [], phase: "preflight", reconnects: 0 }; runs.push(run);
    const dir = join(out, agent); mkdirSync(dir, { mode: 0o700 });
    const app = resolve(root, `../arc/build/ArcModAgent-${agent}.app`);
    const exe = join(app, "Contents/MacOS/Arc");
    const prefix = join(app, "Contents/Frameworks") + "/";
    const plist = (key: string) => exec(["/usr/libexec/PlistBuddy", "-c", `Print :${key}`, join(app, "Contents/Info.plist")]);
    const requestPath = join(dir, "ui-control-inventory-request.json");
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
      const engine = join(app, "Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore");
      const exports = exec(["nm", "-gU", engine], 10000);
      const requiredClasses = readFileSync(join(root, "arc/tools/arccore-1.170.0-objc.tsv"), "utf8")
          .split("\n").filter(line => line.startsWith("C\t")).map(line => line.split("\t")[1]!);
      const missingClasses = requiredClasses.filter(name => !exports.includes(`_OBJC_CLASS_$_${name}\n`));
      if (missingClasses.length) throw new Error(`stale framework missing classes: ${missingClasses.join(", ")}`);
      if (snapshot().some(row => exact(row, exe) || row.command.startsWith(prefix))) throw new Error("agent browser/helpers already running; refusing attachment or termination");
      if (Bun.spawnSync(["/usr/sbin/lsof", "-nP", `-iTCP:${port}`, "-sTCP:LISTEN"], { stdout: "ignore", stderr: "ignore", timeout: 2000 }).success) throw new Error("CDP port occupied");
      const applicationData = join(process.env.HOME!, "Library/Application Support", `ArcModAgent-${agent}`);
      const data = join(applicationData, "Agent Chromium Data");
      mkdirSync(data, { recursive: true, mode: 0o700 });
      run.phase = "launch";
      launchRequested = true;
      exec(["open", "-n", "-g", "-a", app,
        "--env", `DYLD_INSERT_LIBRARIES=${dylib}`, "--env", `ARC_UI_INVENTORY_REQUEST=${requestPath}`,
        "--env", `ARC_UI_INVENTORY_NONCE=${nonce}`, "--stdout", "/dev/null", "--stderr", "/dev/null", "--args",
        "--use-mock-keychain", "--no-default-browser-check", "--disable-sync", "--disable-background-networking",
        `--host-resolver-rules=${plist("ArcModHostResolverRules")}`, `--user-data-dir=${data}`,
        "--remote-debugging-address=127.0.0.1", `--remote-debugging-port=${port}`]);
      browser = await wait(() => {
        const matches = snapshot().filter(row => exact(row, exe));
        return matches.length === 1 ? matches[0] : undefined;
      }, 20000); run.pid = browser.pid;
      await wait(() => {
        const ready = json<{installed: boolean; pid: number; nonce: string; mainThread: boolean}>(requestPath + ".ready.json");
        return ready.installed && ready.pid === browser!.pid && ready.nonce === nonce && ready.mainThread;
      }, 20000);
      await wait(() => {
        const ready = json<{ready: boolean; pid: number}>(join(applicationData, "ArcModAgentReady.json"));
        return ready.ready === true && ready.pid === browser!.pid;
      }, 35000);
      run.phase = "connect after app-ready";
      client = await wait(connect, 15000);
      if (Date.now() >= workDeadline) throw new Error("work deadline reached");
      run.phase = "create nonce fixture target";
      const created = await client.request("Target.createTarget", { url: fixtureURL });
      if (typeof created.targetId !== "string") throw new Error("no owned fixture target");
      const target = created.targetId;
      run.phase = "verify nonce fixture page";
      await wait(async () => {
        let result;
        try { result = await client!.targetCommand(target, "Runtime.evaluate", { expression: `location.href===${JSON.stringify(fixtureURL)}&&window.inventoryNonce===${JSON.stringify(nonce)}&&document.readyState==='complete'`, returnByValue: true }); }
        catch (error) {
          // Reattach only for idempotent reads on this exact existing fixture.
          // Never replay target creation or an uncertain alert/action request.
          if (!/closed|disconnect/.test(String(error)) || (run.reconnects ?? 0) >= 2 ||
              !snapshot().some(row => same(row, browser!))) throw error;
          run.reconnects = (run.reconnects ?? 0) + 1; client?.close(); client = await connect();
          return false;
        }
        return (result.result as {value?: unknown} | undefined)?.value === true;
      }, 8000);
      run.phase = "schedule actual page alert";
      const scheduled = await client.targetCommand(target, "Runtime.evaluate", { expression: "window.triggerInventoryAlert()", returnByValue: true });
      if ((scheduled.result as {value?: unknown} | undefined)?.value !== nonce) throw new Error("fixture did not schedule its actual alert");
      run.pageAlertScheduled = true;
      run.phase = "read local dialog inventory";
      for (let index = 0; index < 8 && Date.now() + 1500 < workDeadline; ++index) {
        if (verifyAgentEndpoint(agent, port) !== browser.pid) throw new Error("endpoint PID changed");
        const id = `${nonce}-${index}`;
        writeFileSync(requestPath + ".tmp", JSON.stringify({ id, nonce, pid: browser.pid, operation: "inventory" }), { mode: 0o600 });
        renameSync(requestPath + ".tmp", requestPath);
        const inventory = await wait(() => {
          const value = json<Inventory>(requestPath + ".response.json");
          return value.id === id ? value : undefined;
        }, 1500);
        if (inventory.pid !== browser.pid || inventory.nonce !== nonce || inventory.mainThread !== true || inventory.nativeActionsPerformed !== 0 || inventory.functionalAcceptance !== false || !Array.isArray(inventory.nodes) || inventory.nodes.length > 1000) throw new Error("inventory identity/bounds/semantics mismatch");
        run.observations.push(inventory);
        writeFileSync(join(dir, `inventory-${index}.json`), JSON.stringify(inventory, null, 2), { mode: 0o600 });
        if (inventory.fixtureContainerCount > 0 && inventory.fixtureButtonCount > 0) break;
        await sleep(250);
      }
    } catch (error) {
      run.errors.push(`${run.phase}: ${String(error)}`);
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
      // Never resolve the alert. Browser shutdown or exact captured process
      // termination performs cleanup without a synthetic dialog completion.
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
      client?.close(); run.elapsedMs = Date.now() - started;
      if (run.elapsedMs > 90000) run.errors.push("per-app 90-second budget exceeded; inspect cleanup diagnostics");
      writeFileSync(join(dir, "run.json"), JSON.stringify(run, null, 2), { mode: 0o600 });
    }
    if (stopAfterRun) break;
  }
} finally { fixture.stop(true); }
const report = { nonce, purpose: "Read-only local AppKit control capability inventory", functionalAcceptance: false,
  nativeActionsPerformed: 0, runs: runs.map(run => ({ agent: run.agent, pid: run.pid, errors: run.errors,
    phase: run.phase, browserAliveOnFailure: run.browserAliveOnFailure, reconnects: run.reconnects,
    pageAlertScheduled: run.pageAlertScheduled ?? false, cleanup: run.cleanup, elapsedMs: run.elapsedMs,
    observations: run.observations.length,
    capabilityStatus: run.observations.some(row => row.truncated) ? "inventory-truncated-inconclusive" :
      run.observations.some(row => row.fixtureContainerCount > 0 && row.fixtureButtonCount > 0) ? "candidate-controls-observed-unexercised" :
      run.observations.some(row => row.fixtureMarkerCount > 0) ? "fixture-observed-controls-unresolved" : "fixture-not-locally-observed-inconclusive",
    fixtureObserved: run.observations.some(row => row.fixtureMarkerCount > 0),
    possibleNSButtons: Math.max(0, ...run.observations.map(row => row.fixtureNSButtonCount)),
    possibleAccessibilityPressResponders: Math.max(0, ...run.observations.map(row => row.fixtureAccessibilityPressResponderCount)),
    truncated: run.observations.some(row => row.truncated) })),
  limitation: "This inventories method availability without invoking any controls. It does not establish pressability, real app-handler completion, AX equivalence, visual acceptance, or functional engine parity. Missing/ambiguous/truncated fixture controls are inconclusive." };
writeFileSync(join(out, "report.json"), JSON.stringify(report, null, 2), { mode: 0o600 });
console.log(JSON.stringify(report, null, 2));
process.exitCode = runs.some(run => run.errors.length > 0 || !run.pageAlertScheduled || run.observations.length === 0) ? 1 : 0;
