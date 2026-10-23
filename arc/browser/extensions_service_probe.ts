#!/usr/bin/env bun
// Supplemental paired public service probe. No AX/global input. Run only under
// the lead-owned build/run lock. Native helper calls public selectors and
// forwards the app's actual delegates; it never substitutes callback results.
import { AgentCDP, verifyAgentEndpoint } from "../../../arc/tools/agent-cdp";
import { existsSync, mkdirSync, readFileSync, renameSync, statSync, writeFileSync } from "node:fs";
import { join, resolve, sep } from "node:path";

const root = resolve(import.meta.dir, "../..");
const args = process.argv.slice(2);
const option = (name: string, fallback?: string) => {
  const index = args.indexOf(name);
  return index === -1 ? fallback : args[index + 1];
};
const requestedOut = option("--out");
if (!requestedOut) throw Error("usage: extensions_service_probe.sh run --out results/extensions-service-probe/UNIQUE [--agents trace156,ours156] [--port 9458]");
const out = resolve(requestedOut);
const outputRoot = join(root, "results/extensions-service-probe");
if (!out.startsWith(outputRoot + sep) || existsSync(out)) throw Error("--out must be a fresh child of results/extensions-service-probe");
const agents = option("--agents", "trace156,ours156")!.split(",");
if (!agents.length || new Set(agents).size !== agents.length || agents.some(agent => !["trace156", "ours156"].includes(agent))) throw Error("only trace156/ours156, once each");
const port = Number(option("--port", "9458"));
if (!Number.isInteger(port) || port < 1024 || port > 65535) throw Error("invalid CDP port");
const exec = (argv: string[]) => {
  const result = Bun.spawnSync(argv, { stdout: "pipe", stderr: "pipe" });
  if (!result.success) throw Error(`${argv[0]}: ${result.stderr.toString().trim()}`);
  return result.stdout.toString().trim();
};
const lockPID = Number(readFileSync(join(root, "logs/arccore-build.lock/pid"), "utf8").trim());
let ancestor = process.pid;
for (let depth = 0; ancestor !== lockPID && ancestor > 1 && depth < 20; ++depth)
  ancestor = Number(exec(["ps", "-p", String(ancestor), "-o", "ppid="]));
if (ancestor !== lockPID) throw Error("parent-owned shared lock required; this runner never acquires it");
const dylib = join(root, "logs/wp/extensions-service-probe-build/ExtensionsServiceProbe.dylib");
const recorder = join(root, "tests/recorder/ArcEngineRecorder.dylib");
for (const file of [dylib, recorder]) if (!existsSync(file)) throw Error(`missing ${file}`);
// Hash the actual binaries to be loaded, not their names or a packaging log.
// Source freshness is a bounded mtime check for this work package only; it is
// not a claim that all concurrent packages were included in the linked engine.
const fingerprint = (path: string) => {
  const before = statSync(path);
  const sha256 = exec(["/usr/bin/shasum", "-a", "256", path]).split(/\s+/)[0] ?? "";
  const after = statSync(path);
  if (!/^[a-f0-9]{64}$/.test(sha256) || before.size !== after.size || before.mtimeMs !== after.mtimeMs)
    throw Error(`file changed during fingerprint: ${path}`);
  return { path, sha256, size: after.size, mtimeMs: after.mtimeMs };
};
const imageFingerprintScript = join(root, "arc/browser/extensions_macho_fingerprint.py");
const imageFingerprint = (path: string) => {
  const raw = fingerprint(path);
  const image = JSON.parse(exec(["python3", imageFingerprintScript, path])) as { rawSHA256: string; imageSHA256: string; [key: string]: unknown };
  if (raw.sha256 !== image.rawSHA256 || !/^[a-f0-9]{64}$/.test(image.imageSHA256)) throw Error(`Mach-O changed during image fingerprint: ${path}`);
  return { ...raw, image };
};
const gniPath = join(root, "arc/wp/extensions.gni");
const gniText = readFileSync(gniPath, "utf8");
const sourceBlock = gniText.match(/wp_extensions_sources\s*=\s*\[([\s\S]*?)\]/)?.[1];
if (!sourceBlock) throw Error("cannot parse extensions.gni source list");
const sourcePaths = [...sourceBlock.matchAll(/"\/\/(arc\/[^"\n]+)"/g)].flatMap(match => match[1] ? [match[1]] : []);
sourcePaths.push("arc/wp/extensions.gni", "arc/browser/extensions_internal/BUILD.gn", "arc/browser/extensions_internal/manifest.json", "arc/browser/extensions_internal/background.js");
const sources = [...new Set(sourcePaths)].sort().map(path => fingerprint(join(root, path)));
if (sources.some(source => source.path.includes("_probe"))) throw Error("probe files accidentally entered product fingerprint list");
const builtEngine = imageFingerprint(join(root, "chromium/src/out/Base/Chromium.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore"));
const packagedEnginePath = (agent: string) => resolve(root, `../arc/build/ArcModAgent-${agent}.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`);
// Stock is a universal binary pinned in full below; only our thin arm64 build
// needs comparison across the known packaging re-sign operation.
const packagedEngines = { trace156: fingerprint(packagedEnginePath("trace156")), ours156: imageFingerprint(packagedEnginePath("ours156")) };
const selectedEngine = (agent: string) => {
  if (agent === "trace156") return packagedEngines.trace156;
  if (agent === "ours156") return packagedEngines.ours156;
  throw Error("unknown agent provenance");
};
const probeBinary = fingerprint(dylib);
const recorderBinary = fingerprint(recorder);
const imageFingerprintTool = fingerprint(imageFingerprintScript);
const frameworkInfo = (binary: { path: string }) => {
  const path = resolve(binary.path, "../Resources/Info.plist");
  return {
    ...fingerprint(path),
    values: Object.fromEntries(["CFBundleIdentifier", "CFBundleShortVersionString", "CFBundleVersion"].map(key =>
      [key, exec(["/usr/libexec/PlistBuddy", "-c", `Print :${key}`, path])])),
  };
};
const frameworkMetadata = {
  trace156: frameworkInfo(packagedEngines.trace156),
  ours156: frameworkInfo(packagedEngines.ours156),
  built: frameworkInfo(builtEngine),
};
const surfaceMetadata = fingerprint(join(root, "arc/tools/arccore-1.170.0-objc.tsv"));
const expectedClasses = readFileSync(surfaceMetadata.path, "utf8").split(/\r?\n/).map(line => line.split("\t")).flatMap(fields => fields[0] === "C" && fields[1] ? [fields[1]] : []);
if (expectedClasses.length < 100) throw Error("metadata class export preflight has an unexpectedly small class inventory");
const globalExports = new Set(exec(["/usr/bin/nm", "-gU", packagedEngines.ours156.path]).split(/\r?\n/).map(line => line.trim().split(/\s+/).at(-1)));
const exportPreflight = {
  expectedClassCount: expectedClasses.length,
  missingClasses: expectedClasses.filter(name => !globalExports.has("_OBJC_CLASS_$_" + name)),
  metadata: surfaceMetadata,
  packagedOursSHA256: packagedEngines.ours156.sha256,
};
const expectedStockSHA256 = "b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d";
const stockSiblingPath = resolve(root, "../arc/build/ArcModAgent-swap156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore");
const independentStockSibling = existsSync(stockSiblingPath) ? fingerprint(stockSiblingPath) : null;
const unchanged = (file: { path: string; size: number; mtimeMs: number }) => {
  const current = statSync(file.path); return current.size === file.size && current.mtimeMs === file.mtimeMs;
};
const provenance = {
  capturedAt: new Date().toISOString(), builtEngine, packagedEngines, probeBinary, recorderBinary, frameworkMetadata, imageFingerprintTool, sources, exportPreflight,
  expectedStockSHA256, independentStockSibling,
  traceMatchesVerifiedStock: packagedEngines.trace156.sha256 === expectedStockSHA256,
  siblingMatchesVerifiedStock: independentStockSibling ? independentStockSibling.sha256 === expectedStockSHA256 : null,
  sourcesUnchangedDuringFingerprint: sources.every(unchanged),
  packagedOursRawMatchesBuilt: packagedEngines.ours156.sha256 === builtEngine.sha256,
  packagedOursMatchesBuilt: packagedEngines.ours156.image.imageSHA256 === builtEngine.image.imageSHA256,
  ownProductSourcesNotNewerThanBuilt: sources.every(source => source.mtimeMs <= builtEngine.mtimeMs),
  scope: "Actual packaged binaries and extension-package sources/assets only. ArcMod re-signs nested binaries: raw hashes are retained, build/package equality hashes every pre-signature Mach-O byte except the signature locator and signature-dependent linkedit sizes. Mtime freshness is snapshot-specific and does not prove other concurrent packages are included.",
};
mkdirSync(out, { recursive: true, mode: 0o700 });
writeFileSync(join(out, "provenance.json"), JSON.stringify(provenance, null, 2));
if (!provenance.traceMatchesVerifiedStock || provenance.siblingMatchesVerifiedStock === false) throw Error("precondition failed: stock artifact SHA256 differs from independently verified stock engine; see provenance.json");
if (exportPreflight.missingClasses.length) throw Error(`precondition failed: packaged ours missing metadata Objective-C class exports: ${exportPreflight.missingClasses.join(", ")}; see provenance.json`);
if (!provenance.sourcesUnchangedDuringFingerprint) throw Error("precondition failed: extension product source changed during fingerprint; see provenance.json");
if (!provenance.packagedOursMatchesBuilt) throw Error("precondition failed: packaged ours Mach-O image differs from current out/Base engine beyond code-signature metadata; see provenance.json");
if (!provenance.ownProductSourcesNotNewerThanBuilt) throw Error("precondition failed: extension product sources/assets are newer than built engine; see provenance.json");

const json = (path: string): any => JSON.parse(readFileSync(path, "utf8"));
const sleep = (ms: number) => new Promise(resolve => setTimeout(resolve, ms));
const wait = async <T>(read: () => T | Promise<T>, timeout = 20000): Promise<NonNullable<T>> => {
  const deadline = Date.now() + timeout;
  let last: unknown;
  while (Date.now() < deadline) {
    try { const value = await read(); if (value) return value as NonNullable<T>; } catch (error) { last = error; }
    await sleep(100);
  }
  throw Error(`bounded wait ${timeout}ms${last ? `: ${last}` : ""}`);
};
const pids = (exe: string) => exec(["ps", "-axo", "pid=,command="]).split("\n").flatMap(line => {
  const match = line.match(/^\s*(\d+)\s+(.+)$/);
  const command = match?.[2];
  return command && (command === exe || command.startsWith(exe + " ")) ? [Number(match![1])] : [];
});
const alive = (pid: number) => Bun.spawnSync(["kill", "-0", String(pid)], { stderr: "ignore" }).success;
const token = `arc-extensions-service-probe-${crypto.randomUUID()}`;
const effects: any[] = [];
let currentAgent = "";
const server = Bun.serve({ hostname: "127.0.0.1", port: 0, async fetch(request) {
  const path = new URL(request.url).pathname;
  if (path === `/${token}/page`) return new Response(`<!doctype html><meta charset="utf-8"><title>Extensions service probe</title><main id="fixture" data-token="${token}">Local fixture</main>`, { headers: { "content-type": "text/html" } });
  const match = path.match(new RegExp(`^/${token}/events/([AB])$`));
  if (match && request.method === "POST") {
    try {
      const text = await request.text();
      if (text.length > 8192) return new Response("too large", { status: 413 });
      const value = JSON.parse(text);
      effects.push({ agent: currentAgent, label: match[1], value });
      return new Response("ok");
    } catch { return new Response("invalid JSON", { status: 400 }); }
  }
  return new Response("missing", { status: 404 });
} });
const fixtureURL = `http://127.0.0.1:${server.port}/${token}/page`;
const fixtures = join(out, "fixtures");
mkdirSync(fixtures, { recursive: true, mode: 0o700 });
// Same absolute A/B directories are used in both engines, yielding the same
// unpacked extension IDs without copying any production/internal signing key.
for (const label of ["A", "B"]) {
  const directory = join(fixtures, label); mkdirSync(directory);
  const endpoint = `http://127.0.0.1:${server.port}/${token}/events/${label}`;
  const manifest = {
    manifest_version: 3, name: `ArcCore service probe ${token} ${label}`, version: "1.2.3",
    description: `Local fixture ${label} π 東京`, homepage_url: "https://example.invalid/arc-extension-probe",
    permissions: ["tabs", "activeTab"], host_permissions: ["http://127.0.0.1/*"],
    background: { service_worker: "background.js" },
    action: { default_title: `Service fixture ${label}`, ...(label === "A" ? { default_popup: "popup.html" } : {}) },
    commands: { "probe-command": { suggested_key: { default: label === "A" ? "Alt+Shift+Y" : "Alt+Shift+U" }, description: `Command ${label} π` }, "_execute_action": { suggested_key: { default: label === "A" ? "Alt+Shift+J" : "Alt+Shift+K" } } },
    omnibox: { keyword: `arcserviceprobe${label.toLowerCase()}` }, icons: { "16": "icon.png", "32": "icon.png" },
  };
  writeFileSync(join(directory, "manifest.json"), JSON.stringify(manifest));
  writeFileSync(join(directory, "icon.png"), Buffer.from("iVBORw0KGgoAAAANSUhEUgAAAAIAAAACCAYAAABytg0kAAAAFElEQVR4nGMwTpv5/0ya8X8GGAMATsMJK9Ofv4oAAAAASUVORK5CYII=", "base64"));
  writeFileSync(join(directory, "background.js"), `const report=value=>fetch(${JSON.stringify(endpoint)},{method:'POST',body:JSON.stringify({...value,id:chrome.runtime.id})}).catch(()=>{});chrome.commands.onCommand.addListener((command,tab)=>report({kind:'command',command,tab:tab?{url:tab.url??null,idPresent:Number.isInteger(tab.id)}:null}));chrome.action.onClicked.addListener(tab=>report({kind:'action',tab:tab?{url:tab.url??null,idPresent:Number.isInteger(tab.id)}:null}));chrome.runtime.onInstalled.addListener(()=>report({kind:'installed'}));chrome.action.setBadgeText({text:'P'});`);
  writeFileSync(join(directory, "popup.html"), '<!doctype html><meta charset="utf-8"><title>Service fixture popup</title><style>body{margin:0;width:320px;height:160px;background:#334455;color:white}</style><main>Real extension popup</main><script src="popup.js"></script>');
  writeFileSync(join(directory, "popup.js"), `const report=value=>fetch(${JSON.stringify(endpoint)},{method:'POST',body:JSON.stringify({...value,id:chrome.runtime.id})}).catch(()=>{});report({kind:'popup-loaded',url:location.href});setTimeout(()=>{document.body.style.width='400px';document.body.style.height='220px';report({kind:'popup-resized'});},300);setTimeout(()=>{report({kind:'popup-close-requested'});window.close();},1400);`);
}
mkdirSync(join(fixtures, "invalid"));
writeFileSync(join(fixtures, "invalid/manifest.json"), '{"manifest_version":3,"name":');

type Action = { step: string; [key: string]: any };
type Run = { agent: string; actions: Action[]; errors: string[]; uncovered: string[]; cleanup?: string; pid?: number; diagnostics?: any[]; ownership?: any };
const runs: Run[] = [];
const callbackSelectors = new Set([
  "browserContext:didEnableExtension:", "browserContext:didDisableExtension:", "browserContext:didInstallExtension:", "browserContext:didUninstallExtension:",
  "browserContext:didFailLoadUnpacked:withManifestError:", "browserContext:didFail:withRuntimeError:", "browserContext:didFail:withManifestError:", "browserContext:didFail:withInternalError:", "browserContext:didChangePinnedExtensionOrder:", "browserContextDidExtensionSystemReady:",
  "browser:didOpenExtensionPopup:webContents:", "browser:didResizeExtensionPopup:size:", "browser:didCloseExtensionPopup:", "browser:didUpdateExtensionToolbarAction:", "browser:didUpdateExtensionToolbarActions:",
]);
const boundaryClasses = new Set(["ArcBrowserContextExtensions", "ArcExtensionInfo", "ArcExtensionCommand", "ArcExtensionRuntimeError", "ArcExtensionRuntimeErrorStackFrame", "ArcExtensionToolbarAction"]);
try {
  for (const agent of agents) {
    currentAgent = agent;
    const run: Run = { agent, actions: [], errors: [], uncovered: ["ArcBrowser pinExtension:/unpinExtension:/moveExtension:targetIndex: metadata only; executable behavior uncovered", "Positive legacy background-page JavaScript uncovered; MV3 negative path only"] }; runs.push(run);
    const dir = join(out, agent); mkdirSync(dir);
    const app = resolve(root, `../arc/build/ArcModAgent-${agent}.app`), exe = join(app, "Contents/MacOS/Arc");
    const plist = (key: string) => exec(["/usr/libexec/PlistBuddy", "-c", `Print :${key}`, join(app, "Contents/Info.plist")]);
    const request = join(dir, "extensions-service-request.json"), recordPath = join(dir, "record.jsonl");
    let pid: number | undefined, cdp: AgentCDP | undefined, target: string | undefined;
    let requestSequence = 0;
    const action = async (step: string, operation: string, extra: Record<string, any> = {}): Promise<Action> => {
      if (!pid || verifyAgentEndpoint(agent, port) !== pid) throw Error("owned endpoint changed");
      const id = `${token}-${++requestSequence}`;
      writeFileSync(request + ".tmp", JSON.stringify({ id, token, operation, ...extra })); renameSync(request + ".tmp", request);
      const response = await wait(() => { const value = json(request + ".response.json"); return value.id === id ? value : undefined; }, 17000);
      if (response.pid !== pid || response.mainThread !== true) throw Error("probe acknowledgment PID/thread mismatch");
      const result = { step, ...response }; run.actions.push(result);
      if (!result.ok) throw Error(`${step}: ${result.probeError}`);
      return result;
    };
    const assert = (condition: unknown, message: string) => { if (!condition) throw Error(message); };
    try {
      assert(sources.every(unchanged) && unchanged(builtEngine) && unchanged(selectedEngine(agent)) && unchanged(probeBinary) && unchanged(recorderBinary) && unchanged(imageFingerprintTool), "provenance changed before launch; rebuild/repackage and use a fresh output directory");
      assert(plist("ArcModAgentSession") === agent && plist("CFBundleIdentifier") === `company.thebrowser.Browser.mod.agent.${agent}` && plist("CFBundleName") === `ArcModAgent-${agent}`, "disposable bundle identity mismatch");
      assert(!pids(exe).length, "agent already running; refusing attach/termination");
      assert(!Bun.spawnSync(["/usr/sbin/lsof", "-nP", `-iTCP:${port}`, "-sTCP:LISTEN"], { stdout: "ignore", stderr: "ignore" }).success, "CDP port occupied");
      const agentData = join(process.env.HOME!, "Library/Application Support", `ArcModAgent-${agent}`);
      const data = join(agentData, "Agent Chromium Data");
      mkdirSync(data, { recursive: true, mode: 0o700 });
      exec(["open", "-n", "-g", "-a", app,
        "--env", `DYLD_INSERT_LIBRARIES=${dylib}:${recorder}`, "--env", `ARC_RECORD_PATH=${recordPath}`,
        "--env", `ARC_EXTENSIONS_PROBE_REQUEST=${request}`, "--env", `ARC_EXTENSIONS_PROBE_FIXTURES=${fixtures}`, "--env", `ARC_EXTENSIONS_PROBE_TOKEN=${token}`,
        "--stdout", join(dir, "app.log"), "--stderr", join(dir, "app.log"), "--args", "--use-mock-keychain", "--no-default-browser-check", "--disable-sync", "--disable-background-networking",
        `--host-resolver-rules=${plist("ArcModHostResolverRules")}`, `--user-data-dir=${data}`, "--remote-debugging-address=127.0.0.1", `--remote-debugging-port=${port}`]);
      pid = await wait(() => pids(exe)[0], 30000); run.pid = pid;
      await wait(() => {
        const ready = json(request + ".ready.json");
        const appReady = json(join(agentData, "ArcModAgentReady.json"));
        return ready.pid === pid && ready.installed && ready.mainThread &&
          appReady.pid === pid && appReady.ready === true && existsSync(recordPath);
      }, 60000);
      await wait(() => verifyAgentEndpoint(agent, port) === pid, 30000);
      cdp = await AgentCDP.connectAgent(agent, port);
      target = (await cdp.request("Target.createTarget", { url: fixtureURL })).targetId as string;
      assert(typeof target === "string", "missing owned local fixture tab");
      await wait(async () => { const value = await cdp!.targetCommand(target!, "Runtime.evaluate", { expression: `document.readyState==='complete'&&document.querySelector('#fixture')?.dataset.token===${JSON.stringify(token)}`, returnByValue: true }); return (value.result as any)?.value === true; });
      const registry = await wait(async () => {
        try { const value = await action(`registry-${requestSequence + 1}`, "registry"); return value.identity && value.ready ? value : undefined; } catch { return undefined; }
      }, 25000);
      registry.step = "registry";
      assert(registry.identity && registry.ready, "canonical ready service unavailable");
      assert(registry.browserMatches === 1 && registry.browserPresent, "probe precondition failed: missing or ambiguous real app browser; no selection/activation attempted");
      // Failed discovery attempts are retained separately, outside exercised method spans.
      const discoveries = run.actions.filter(value => value !== registry); run.actions = [registry];
      writeFileSync(join(dir, "discovery.json"), JSON.stringify(discoveries, null, 2));
      for (const label of ["A", "B"]) {
        const installed = await action(`install-${label}`, "install", { label });
        assert(installed.completions?.length === 1 && installed.completion?.result === 0 && installed.owned[label] === installed.completion.id, `${label}: public install completion/proven identity failed`);
      }
      const loaded = await action("snapshot-loaded", "snapshot");
      assert(loaded.list?.length === 2 && loaded.list.every((info: any) => info.enabled && info.version === "1.2.3" && !info.fromWebstore), "installed info/list mismatch");
      assert(loaded.commands?.filter((command: any) => command.commandName === "probe-command").length === 2, "missing real named command entries");
      assert(loaded.commands?.filter((command: any) => command.commandName === "_execute_action").length === 2, "missing real action command entries");
      const invalid = await action("install-invalid", "install-invalid");
      assert(invalid.completions?.length === 1 && invalid.completion?.result !== 0 && invalid.completion?.id === "", "invalid manifest unexpectedly installed");
      const checkPins = (value: Action, labels: string[]) => {
        const expected = [...new Set(labels)].map(label => value.owned[label]);
        const actual = value.pinsAfter.filter((id: string) => Object.values(value.owned).includes(id));
        assert(JSON.stringify(actual) === JSON.stringify(expected), `${value.step}: fixture pin order mismatch`);
        const untouched = value.pinsAfter.filter((id: string) => !Object.values(value.owned).includes(id));
        assert(JSON.stringify(untouched) === JSON.stringify(registry.baselinePins), `${value.step}: baseline pins changed`);
      };
      checkPins(await action("pins-AB-dedup", "pins", { labels: ["A", "B", "A"] }), ["A", "B"]);
      checkPins(await action("pins-BA", "pins", { labels: ["B", "A"] }), ["B", "A"]);
      checkPins(await action("pins-A", "pins", { labels: ["A"] }), ["A"]);
      const beforeCommand = effects.length;
      const command = await action("command-A", "command", { label: "A", command: "probe-command" });
      command.effects = await wait(() => { const events = effects.slice(beforeCommand).filter(event => event.agent === agent && event.label === "A" && event.value.kind === "command"); return events.length ? events : undefined; }, 8000);
      assert(command.effects[0].value.id === command.owned.A && command.effects[0].value.command === "probe-command", "named command effect not from installed fixture");
      assert(command.effects[0].value.tab?.url === fixtureURL && command.effects[0].value.tab?.idPresent === true, "probe precondition failed: named command selected a different browser/tab; no activation attempted");
      const beforeAction = effects.length;
      const clicked = await action("action-B", "action", { label: "B" });
      clicked.effects = await wait(() => { const events = effects.slice(beforeAction).filter(event => event.agent === agent && event.label === "B" && event.value.kind === "action"); return events.length ? events : undefined; }, 8000);
      assert(clicked.effects[0].value.id === clicked.owned.B, "action effect not from installed fixture");
      assert(clicked.effects[0].value.tab?.url === fixtureURL && clicked.effects[0].value.tab?.idPresent === true, "probe precondition failed: action selected a different browser/tab; no activation attempted");
      const js = await action("background-js-MV3", "background-js", { label: "A" });
      assert(js.value === false, "MV3 worker unexpectedly treated as background page");
      const disabled = await action("disable-A", "disable", { label: "A" });
      assert(disabled.infosAfter.A?.enabled === false, "disable did not reach registry");
      const disabledSnapshot = await action("snapshot-disabled", "snapshot");
      assert(disabledSnapshot.list?.some((info: any) => info.id === disabled.owned.A && info.enabled === false), "list omitted disabled fixture");
      const enabled = await action("enable-A", "enable", { label: "A" });
      assert(enabled.infosAfter.A?.enabled === true, "enable did not reach registry");
      const reloaded = await action("reload-A", "reload", { label: "A" });
      assert(reloaded.infosAfter.A?.enabled === true, "reload did not return to enabled state");
      const beforePopup = effects.length;
      const popup = await action("popup-A", "action", { label: "A" });
      popup.effects = effects.slice(beforePopup).filter(event => event.agent === agent && event.label === "A" && event.value.kind.startsWith("popup-"));
      const opened = popup.callbacks.some((event: any) => event.selector === "browser:didOpenExtensionPopup:webContents:");
      if (!opened) run.uncovered.push("popup: public action attempted, no real open callback within bounded presentation window");
      else {
        assert(popup.effects.some((event: any) => event.value.kind === "popup-loaded" && event.value.id === popup.owned.A), "popup opened without observed fixture document effect");
        if (!popup.callbacks.some((event: any) => event.selector === "browser:didResizeExtensionPopup:size:")) run.uncovered.push("popup: resize callback not observed");
        if (!popup.callbacks.some((event: any) => event.selector === "browser:didCloseExtensionPopup:")) run.uncovered.push("popup: close callback not observed");
      }
      const removed = await action("uninstall-A", "uninstall", { label: "A" });
      assert(removed.infosAfter.A === null, "uninstall left fixture installed");
      const cleanup = await action("cleanup", "cleanup");
      assert(cleanup.cleanupCertified === true && cleanup.outstandingInstalls?.length === 0 && Object.values(cleanup.infosAfter).every(value => value === null), "cleanup not certified; fixture installed or installation unresolved");
      assert(JSON.stringify(cleanup.pinsAfter) === JSON.stringify(registry.baselinePins), "cleanup did not restore baseline pins");
      run.cleanup = "fixture IDs absent; baseline pins restored";
    } catch (error) { run.errors.push(String(error)); }
    finally {
      if (pid && !run.cleanup) {
        try {
          const cleanup = await action("cleanup-after-error", "cleanup");
          if (cleanup.cleanupCertified !== true || cleanup.outstandingInstalls?.length !== 0) throw Error("cleanup not certified; unresolved install or fixture state");
          run.cleanup = "reconciled nonce fixtures absent; no outstanding installs; baseline pins restored";
        }
        catch (error) { run.errors.push(`fixture cleanup: ${error}`); }
      }
      if (cdp && target) try { await cdp.request("Target.closeTarget", { targetId: target }); } catch {}
      if (pid) {
        try {
          if (verifyAgentEndpoint(agent, port) !== pid) throw Error("owned endpoint changed");
          cdp ??= await AgentCDP.connectAgent(agent, port);
          await cdp.request("Browser.close").catch(() => {}); await wait(() => !alive(pid!), 12000);
        } catch {
          if (pids(exe).includes(pid)) Bun.spawnSync(["kill", "-TERM", String(pid)]);
          try { await wait(() => !alive(pid!), 12000); } catch { run.errors.push("launched PID did not exit"); }
        }
      }
      cdp?.close();
      if (existsSync(request + ".owned.json")) run.ownership = json(request + ".owned.json");
      if (run.ownership?.outstandingInstalls?.length) run.errors.push("outstanding installation callbacks remained at process termination; cleanup cannot be certified");
      run.diagnostics = existsSync(request + ".diagnostics.json") ? json(request + ".diagnostics.json") : [];
      const fixtureIDs = Object.values(run.ownership?.owned ?? {});
      const relevantDiagnostics = run.diagnostics!.filter(event => {
        const serialized = JSON.stringify(event); return serialized.includes(token) || fixtureIDs.some(id => serialized.includes(String(id)));
      });
      if (relevantDiagnostics.length) run.errors.push(`${relevantDiagnostics.length} late/unassigned/outside-context fixture callbacks or completions; inspect diagnostics`);
      if (!sources.every(unchanged) || !unchanged(builtEngine) || !unchanged(selectedEngine(agent)) || !unchanged(probeBinary) || !unchanged(recorderBinary) || !unchanged(imageFingerprintTool)) run.errors.push("provenance changed during run; snapshot cannot establish current-source parity");
      if (existsSync(recordPath)) {
        const raw = readFileSync(recordPath);
        for (const value of run.actions) {
          const begin = value.recordBegin, end = value.recordEnd;
          if (!Number.isInteger(begin) || !Number.isInteger(end) || begin < 0 || end < begin || end > raw.length ||
              (begin && raw[begin - 1] !== 10) || (end > begin && raw[end - 1] !== 10)) { run.errors.push(`${value.step}: invalid recorder span`); continue; }
          const slice = raw.subarray(begin, end); writeFileSync(join(dir, `${value.step}.raw.record.jsonl`), slice);
          // Restrict comparison to the exercised extension boundary. These DTO
          // getter calls include measurements by this probe, identically in
          // both engines; they are not claimed as independent app demand.
          const selected = slice.toString().split("\n").filter(Boolean).map(line => JSON.parse(line)).filter(event =>
            boundaryClasses.has(event.cls) || callbackSelectors.has(event.sel) ||
            (event.cls === "ArcBrowser" && event.sel === "executeExtensionAction:"));
          writeFileSync(join(dir, `${value.step}.record.jsonl`), selected.map(event => JSON.stringify(event)).join("\n") + (selected.length ? "\n" : ""));
        }
      }
      writeFileSync(join(dir, "observations.json"), JSON.stringify(run, null, 2));
      writeFileSync(join(dir, "effects.json"), JSON.stringify(effects.filter(event => event.agent === agent), null, 2));
    }
  }
} finally { server.stop(true); }

// Scope semantics to proven fixture IDs. Preserve array order, native scalars,
// error text, metadata, and callback order. Only transport PID/request IDs,
// recorder offsets, and the redundant engine label are excluded. Baseline pin
// preservation is checked independently against each engine's original state.
const semantic = (action: Action) => {
  const ids = Object.values(action.owned ?? {});
  const own = (id: any) => ids.includes(id);
  const scopeCallback = (event: any): any | null => {
    const payload = structuredClone(event.payload);
    if (event.selector === "browser:didUpdateExtensionToolbarActions:") {
      payload[0] = payload[0].filter((item: any) => own(item.id)); if (!payload[0].length) return null;
    } else if (event.selector === "browser:didUpdateExtensionToolbarAction:") { if (!own(payload[0]?.id)) return null; }
    else if (event.selector === "browserContext:didChangePinnedExtensionOrder:") payload[0] = payload[0].filter(own);
    else if (event.selector !== "browserContext:didFailLoadUnpacked:withManifestError:" && event.selector !== "browserContextDidExtensionSystemReady:" && !own(payload[0])) return null;
    return { selector: event.selector, payload, mainThread: event.mainThread, synchronous: event.synchronous };
  };
  const callbacks = (action.callbacks ?? []).map(scopeCallback).filter(Boolean);
  const keep = ["step", "operation", "ok", "probeError", "matches", "mainThread", "metadata", "ready", "identity", "hashStable", "browserPresent", "browserMatches", "completion", "completions", "outstandingInstalls", "cleanupCertified", "list", "commands", "infosAfter", "owned", "value"];
  const result: Record<string, any> = Object.fromEntries(keep.filter(key => key in action).map(key => [key, action[key]]));
  result.callbacks = callbacks;
  result.eventOrder = (action.events ?? []).flatMap((event: any) => {
    if (event.type === "completion") return [{ type: "completion", value: event.value }];
    const callback = scopeCallback(event.value);
    return callback ? [{ type: "delegate", value: callback }] : [];
  });
  result.fixturePinsAfter = (action.pinsAfter ?? []).filter(own);
  if (action.effects) result.effects = action.effects.map(({ label, value }: any) => ({ label, value }));
  return result;
};
const golden = runs.find(run => run.agent === "trace156"), ours = runs.find(run => run.agent === "ours156");
const differences: any[] = [], boundaryDiffs: any[] = [];
if (golden && ours) {
  const steps = new Set([...golden.actions, ...ours.actions].map(action => action.step));
  for (const step of steps) {
    const left = golden.actions.find(action => action.step === step), right = ours.actions.find(action => action.step === step);
    const a = left ? semantic(left) : null, b = right ? semantic(right) : null;
    writeFileSync(join(out, `${step}.semantic.json`), JSON.stringify({ golden: a, ours: b }, null, 2));
    if (JSON.stringify(a) !== JSON.stringify(b)) differences.push({ step, golden: a, ours: b });
    const leftPath = join(out, "trace156", `${step}.record.jsonl`), rightPath = join(out, "ours156", `${step}.record.jsonl`);
    if (!existsSync(leftPath) || !existsSync(rightPath)) { boundaryDiffs.push({ step, error: "missing scoped record" }); continue; }
    const diffPath = join(out, `${step}.diff.json`);
    const diff = Bun.spawnSync(["python3", join(root, "tests/recorder/diff.py"), leftPath, rightPath, "--json", diffPath], { stdout: "pipe", stderr: "pipe" });
    writeFileSync(join(out, `${step}.diff.txt`), diff.stdout);
    if (!diff.success) { boundaryDiffs.push({ step, error: diff.stderr.toString() }); continue; }
    const report = json(diffPath);
    const mismatches = ["missing", "extra", "return_mismatch", "arg_mismatch", "count_mismatch"].filter(key => report[key]?.length);
    if (report.callback_sequence?.first_divergence !== null) mismatches.push("callback_sequence");
    if (mismatches.length) boundaryDiffs.push({ step, mismatches });
  }
}
const complete = runs.length === agents.length && runs.every(run => !run.errors.length && !!run.cleanup);
const parity = golden && ours ? complete && !differences.length && !boundaryDiffs.length : null;
const notes = [
  "Supplemental probe only; canonical extensions scenario acceptance remains separate.",
  "Native metadata encodings and fixture DTO scalar/string/ordered-array observations compare exactly; image observations include NSImage dimensions and representation sizes, not pixel equivalence.",
  "Semantic comparison omits transport PID/request IDs, recorder offsets and engine labels; callback and pin arrays are scoped to proven fixture IDs. Each baseline pin order is independently restored and asserted.",
  "Scoped boundary records include probe-generated DTO getter measurements and real app callbacks; they are not claimed as independent app demand.",
  "Existing diff.py renumbers recording object identities and masks loopback ports, temporary folders and UUID strings. The exact supplemental semantic comparison applies none of those string masks.",
  "ArcBrowser pinExtension:, unpinExtension:, moveExtension:targetIndex: are metadata-only here; their executable behavior is uncovered. Context setPinnedExtensionOrder: is exercised separately.",
  "MV3 background-page JavaScript negative path only; positive legacy background-page execution is uncovered and no legacy fixture is enabled.",
  "Browser captures retain all weak live browser/delegate entries per context; ambiguous selection fails a probe precondition. Command/action effects must prove a real numeric tab ID and the exact nonce fixture URL; no activation is attempted.",
  "provenance.json requires all original metadata class exports and records raw engine/probe/recorder/source hashes plus framework versions. Build/package equality compares every pre-signature Mach-O byte except its signature locator and signature-dependent __LINKEDIT sizes; ArcMod ad-hoc re-signing changes those fields and the trailing signature. The guarded read-only fingerprint helper and its hash are recorded. All owned product sources must precede the build; this is snapshot-specific.",
  "Install attempts are journaled before invocation. Cleanup waits for all outstanding completions, reconciles exact nonce names/verified IDs from the actual installed-extension list, then certifies fixture absence and baseline pin restoration. Timeouts leave explicit unresolved ownership evidence rather than claiming cleanup.",
  "Callback operation attribution is captured under a synchronization lock at callback entry. Deferred late/unassigned/outside-context deliveries are retained in diagnostics; fixture-related diagnostics fail completion rather than being reassigned to a future request.",
  "Callbacks are observed within 800ms after method return/completion; popup actions use 3 seconds. Absence beyond those windows is not a synthesized callback result.",
];
writeFileSync(join(out, "summary.json"), JSON.stringify({ complete, parity, differences, boundaryDiffs, provenance, notes, runs }, null, 2));
console.log(JSON.stringify({ complete, parity, out, errors: runs.map(run => ({ agent: run.agent, errors: run.errors, uncovered: run.uncovered })) }));
process.exitCode = complete && parity !== false ? 0 : 1;
