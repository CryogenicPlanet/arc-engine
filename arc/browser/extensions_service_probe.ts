#!/usr/bin/env bun
// Supplemental paired public service probe. No AX/global input. Run only under
// the lead-owned build/run lock. Native helper calls public selectors and
// forwards the app's actual delegates; it never substitutes callback results.
import { AgentCDP, verifyAgentEndpoint } from "../../../arc/tools/agent-cdp";
import { existsSync, mkdirSync, readFileSync, renameSync, realpathSync, statSync, writeFileSync } from "node:fs";
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
const fixtureMode = args.find(arg => arg.startsWith("--fixture-mode="))?.slice("--fixture-mode=".length) ?? option("--fixture-mode", "public-install")!;
if (!["public-install", "command-line"].includes(fixtureMode)) throw Error("fixture-mode must be public-install or command-line");
const browserPins = args.includes("--browser-pins");
if (browserPins && fixtureMode !== "command-line") throw Error("--browser-pins requires explicit --fixture-mode command-line");
const browserOmnibox = args.includes("--browser-omnibox");
if (browserOmnibox && fixtureMode !== "command-line") throw Error("--browser-omnibox requires explicit --fixture-mode command-line");
const diagnosticDisableFieldTrialConfig = args.includes("--diagnostic-disable-field-trial-config");
const fixtureEnvironmentArguments = ["--use-mock-keychain", "--no-default-browser-check", "--disable-sync", "--disable-background-networking", "--noerrdialogs",
  ...(diagnosticDisableFieldTrialConfig ? ["--disable-field-trial-config"] : [])];
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
  capturedAt: new Date().toISOString(), fixtureMode, browserPins, browserOmnibox, diagnosticDisableFieldTrialConfig, fixtureEnvironmentArguments, builtEngine, packagedEngines, probeBinary, recorderBinary, frameworkMetadata, imageFingerprintTool, sources, exportPreflight,
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
// Persist current stage observations rather than retaining the first transient
// ENOENT from a generic wait. A listener/helper alone is insufficient: the real
// PID-bound app-window readiness record must agree before any CDP mutation.
const waitForNativeReadiness = async (agent: string, pid: number, exe: string, agentData: string, request: string, recordPath: string, dir: string) => {
  const timeoutMs = 120000, started = performance.now(), events: any[] = [];
  const readStatus = (path: string): any => {
    try { return { path, exists: true, mtimeMs: statSync(path).mtimeMs, data: json(path) }; }
    catch (error) { return { path, exists: existsSync(path), error: String(error) }; }
  };
  let previous = "", checkpoint = 0, last: any;
  while (performance.now() - started < timeoutMs) {
    const processPIDs = pids(exe);
    const helper = readStatus(request + ".ready.json");
    const appWindow = readStatus(join(agentData, "ArcModAgentReady.json"));
    const recorderStatus = { exists: existsSync(recordPath), size: existsSync(recordPath) ? statSync(recordPath).size : 0 };
    const preconditions = processPIDs.length === 1 && processPIDs[0] === pid &&
      helper.data?.pid === pid && helper.data?.installed === true && helper.data?.mainThread === true &&
      appWindow.data?.pid === pid && appWindow.data?.agent === agent && appWindow.data?.ready === true &&
      recorderStatus.exists && recorderStatus.size > 0;
    let endpoint: any = { checked: false };
    if (preconditions) {
      try { endpoint = { checked: true, pid: verifyAgentEndpoint(agent, port) }; }
      catch (error) { endpoint = { checked: true, error: String(error) }; }
    }
    const satisfied = preconditions && endpoint.pid === pid;
    last = { processPIDs, helper, appWindow, recorder: recorderStatus, endpoint, satisfied };
    const signature = JSON.stringify({ processPIDs, helper, appWindow, recorder: { exists: recorderStatus.exists, hasContent: recorderStatus.size > 0 }, endpoint, satisfied }), now = performance.now();
    if (signature !== previous || now - checkpoint >= 1000) {
      events.push({ elapsedMs: Math.round(now - started), ...last }); previous = signature; checkpoint = now;
      writeFileSync(join(dir, "readiness.json"), JSON.stringify({ pid, agent, timeoutMs, satisfied, events }, null, 2));
    }
    if (satisfied) return { pid, agent, timeoutMs, satisfied: true, elapsedMs: Math.round(now - started), events };
    if (!processPIDs.includes(pid)) throw Error(`launched process exited before native readiness: ${JSON.stringify(last)}`);
    if (processPIDs.length !== 1) throw Error(`ambiguous disposable process ownership before readiness: ${JSON.stringify(last)}`);
    await sleep(250);
  }
  writeFileSync(join(dir, "readiness.json"), JSON.stringify({ pid, agent, timeoutMs, satisfied: false, events }, null, 2));
  throw Error(`native readiness precondition timed out after ${timeoutMs}ms; current stages: ${JSON.stringify(last)}`);
};
const booleanFields = new Set(["ok", "mainThread", "synchronous", "ready", "identity", "hashStable", "browserPresent", "cleanupCertified", "enabled", "hasAction", "fromWebstore", "isGlobal", "fromIncognito", "present", "adoptionComplete"]);
const requireNativeBooleanTypes = (value: any, path = "response") => {
  if (!value || typeof value !== "object") return;
  if (Array.isArray(value)) { value.forEach((item, index) => requireNativeBooleanTypes(item, `${path}[${index}]`)); return; }
  for (const [key, item] of Object.entries(value)) {
    if (booleanFields.has(key) && typeof item !== "boolean") throw Error(`${path}.${key}: expected JSON Boolean from declared native ABI, got ${JSON.stringify(item)}`);
    requireNativeBooleanTypes(item, `${path}.${key}`);
  }
};
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
  writeFileSync(join(directory, "background.js"), `const report=value=>fetch(${JSON.stringify(endpoint)},{method:'POST',body:JSON.stringify({...value,id:chrome.runtime.id})}).catch(()=>{});chrome.commands.onCommand.addListener(function(command){const args=Array.from(arguments);report({kind:'command',command,argumentCount:args.length,argumentTypes:args.map(value=>value===null?'null':typeof value),noTab:args.length<2||args[1]===undefined});});chrome.action.onClicked.addListener(tab=>report({kind:'action',tab:tab?{url:tab.url??null,idPresent:Number.isInteger(tab.id)}:null}));chrome.runtime.onInstalled.addListener(()=>report({kind:'installed'}));chrome.action.setBadgeText({text:'P'});` + (browserOmnibox ? `let actionSequence=0;const inventory=async()=>({tabs:(await chrome.tabs.query({})).map(tab=>({id:tab.id,windowId:tab.windowId,url:tab.url??null,active:tab.active,incognito:tab.incognito})),windows:(await chrome.windows.getAll()).map(window=>({id:window.id,type:window.type,incognito:window.incognito}))});chrome.action.onClicked.addListener(async tab=>{const sequence=++actionSequence;report({kind:"omnibox-preflight",nonce:${JSON.stringify(token)},sequence,tab:tab?{id:tab.id,windowId:tab.windowId,url:tab.url??null,active:tab.active,incognito:tab.incognito}:null,...await inventory()});});chrome.omnibox.onInputEntered.addListener(async function(input,disposition){const args=Array.from(arguments);report({kind:"omnibox",nonce:${JSON.stringify(token)},input,disposition,argumentCount:args.length,argumentTypes:args.map(value=>value===null?"null":typeof value),...await inventory()});});` : ""));
  writeFileSync(join(directory, "popup.html"), '<!doctype html><meta charset="utf-8"><title>Service fixture popup</title><style>body{margin:0;width:320px;height:160px;background:#334455;color:white}</style><main>Real extension popup</main><script src="popup.js"></script>');
  writeFileSync(join(directory, "popup.js"), `const report=value=>fetch(${JSON.stringify(endpoint)},{method:'POST',body:JSON.stringify({...value,id:chrome.runtime.id})}).catch(()=>{});report({kind:'popup-loaded',url:location.href});setTimeout(()=>{document.body.style.width='400px';document.body.style.height='220px';report({kind:'popup-resized'});},300);setTimeout(()=>{report({kind:'popup-close-requested'});window.close();},1400);`);
}
mkdirSync(join(fixtures, "invalid"));
writeFileSync(join(fixtures, "invalid/manifest.json"), '{"manifest_version":3,"name":');

const fixturePaths = ["A", "B"].map(label => realpathSync(join(fixtures,label)));
if (fixtureMode === "command-line" && fixturePaths.some(path => path.includes(","))) throw Error("command-line fixture paths cannot contain the load-extension delimiter");
const startupArguments = fixtureMode === "command-line" ? [`--load-extension=${fixturePaths.join(",")}`] : [];
const fixtureInputs = ["A", "B"].map(label => ({ label, path: realpathSync(join(fixtures,label)), name: `ArcCore service probe ${token} ${label}`, version: "1.2.3",
  files: ["manifest.json","background.js","popup.html","popup.js","icon.png"].map(file => fingerprint(join(fixtures,label,file))) }));
writeFileSync(join(out,"fixture-inputs.json"), JSON.stringify({fixtureMode, startupArguments, fixtures:fixtureInputs},null,2));
writeFileSync(join(out,"provenance.json"), JSON.stringify({...provenance, fixtureMode, startupArguments, fixtureInputs},null,2));
type Action = { step: string; [key: string]: any };
type Run = { agent: string; actions: Action[]; errors: string[]; uncovered: string[]; cleanup?: string; pid?: number; diagnostics?: any[]; ownership?: any; readiness?: any; policyPrecondition?: any; fixtureMode?: string; startupIntent?: any; reloadObservation?: any; startupCallbacks?: any[] };
const runs: Run[] = [];
const callbackSelectors = new Set([
  "browserContext:didEnableExtension:", "browserContext:didDisableExtension:", "browserContext:didInstallExtension:", "browserContext:didUninstallExtension:",
  "browserContext:didFailLoadUnpacked:withManifestError:", "browserContext:didFail:withRuntimeError:", "browserContext:didFail:withManifestError:", "browserContext:didFail:withInternalError:", "browserContext:didChangePinnedExtensionOrder:", "browserContextDidExtensionSystemReady:",
  "browser:didOpenExtensionPopup:webContents:", "browser:didResizeExtensionPopup:size:", "browser:didCloseExtensionPopup:", "browser:didUpdateExtensionToolbarAction:", "browser:didUpdateExtensionToolbarActions:",
]);
const requiredPaths: Record<string,string[]> = {
  registry:["metadata","ready","identity","hashStable","baselinePins"],
  "adopt-command-line":["metadata","ready","identity","hashStable","baselinePins","adopted","adoptionComplete","startupPending"],
  install:["completion","completions"], "install-invalid":["completion","completions"],
  snapshot:["list","commands"], pins:[],
  "browser-pin":["pinsBefore","browserPinSelector","browserPinABI","browserPinLabel","browserPinArguments","browserPinIdentityVerified","browserPinBaselineBeforePreserved","browserPinBaselineAfterPreserved"],
  "browser-unpin":["pinsBefore","browserPinSelector","browserPinABI","browserPinLabel","browserPinArguments","browserPinIdentityVerified","browserPinBaselineBeforePreserved","browserPinBaselineAfterPreserved"],
  "browser-move":["pinsBefore","browserPinSelector","browserPinABI","browserPinLabel","browserPinArguments","browserPinAnchorLabel","browserPinIdentityVerified","browserPinBaselineBeforePreserved","browserPinBaselineAfterPreserved"],
  "browser-omnibox":["browserOmniboxSelector","browserOmniboxABI","browserOmniboxArguments","browserOmniboxIdentityVerified","browserOmniboxFixtureURL","effects","omniboxPreflight","omniboxCDPBefore","omniboxCDPAfter"],
  command:["effects"], action:["callbacks"],
  "background-js":["value"], disable:[], enable:[], reload:[], uninstall:[], cleanup:["cleanupCertified","outstandingInstalls","startupPending"],
};
function validRequiredValue(path: string, value: any): boolean {
  const dictionary = (item: any): boolean => !!item && typeof item === "object" && !Array.isArray(item);
  const completion = (item: any): boolean => dictionary(item) && typeof item.id === "string" && Number.isInteger(item.result) && typeof item.mainThread === "boolean" && typeof item.synchronous === "boolean";
  const callback = (item: any): boolean => dictionary(item) && typeof item.selector === "string" && Array.isArray(item.payload) && typeof item.mainThread === "boolean" && (item.synchronous === null || typeof item.synchronous === "boolean");
  if (["ok","mainThread","ready","identity","hashStable","adoptionComplete","cleanupCertified","browserPinIdentityVerified","browserPinBaselineBeforePreserved","browserPinBaselineAfterPreserved","browserOmniboxIdentityVerified"].includes(path)) return typeof value === "boolean";
  if (["browserPinSelector","browserPinABI","browserPinLabel","browserPinAnchorLabel","browserOmniboxSelector","browserOmniboxABI","browserOmniboxFixtureURL"].includes(path)) return typeof value === "string" && value.length > 0;
  if (path === "browserOmniboxArguments") return Array.isArray(value) && value.length === 3 && typeof value[0] === "string" && typeof value[1] === "string" && Number.isSafeInteger(value[2]) && value[2] >= 0 && value[2] <= 11;
  if (["omniboxPreflight","omniboxCDPBefore","omniboxCDPAfter"].includes(path)) return dictionary(value);
  if (path === "browserPinArguments") return Array.isArray(value) && (value.length === 1 || value.length === 2) && typeof value[0] === "string" && (value.length === 1 || (Number.isSafeInteger(value[1]) && value[1] >= 0));
  if (path === "operation") return typeof value === "string" && Object.hasOwn(requiredPaths,value);
  if (path === "matches") return Number.isInteger(value) && value >= 0;
  if (["pinsBefore","pinsAfter","baselinePins","outstandingInstalls","startupPending"].includes(path)) return Array.isArray(value) && value.every(item=>typeof item === "string");
  if (["metadata","infosAfter","owned","adopted"].includes(path)) return dictionary(value);
  if (["list","commands","effects"].includes(path)) return Array.isArray(value);
  if (path === "callbacks") return Array.isArray(value) && value.every(callback);
  if (path === "completion") return completion(value);
  if (path === "completions") return Array.isArray(value) && value.every(completion);
  if (path === "events") return Array.isArray(value) && value.every(item=>dictionary(item) && (item.type === "completion" ? completion(item.value) : item.type === "delegate" && callback(item.value)));
  if (path === "value") return typeof value === "boolean";
  return false;
}
function requiredRows(action: Action) {
  const measured = semantic(action);
  const paths = ["operation","ok","matches","mainThread","infosAfter","pinsAfter","owned","callbacks","events",...(requiredPaths[action.operation] ?? [])];
  // Pins are scoped to fixture IDs in semantic comparison; the full native
  // baseline-preservation check is independently asserted during the run.
  const rows = paths.map(path => {
    const semanticPath = path === "pinsBefore" ? "fixturePinsBefore" : path === "pinsAfter" ? "fixturePinsAfter" : path === "events" ? "eventOrder" : path === "baselinePins" ? "fixtureBaselinePins" : path;
    const present = Object.prototype.hasOwnProperty.call(action,path);
    return {path, present, valid:present && validRequiredValue(path,action[path]), value:measured[semanticPath]};
  });
  return {step:action.step, operation:action.operation, basis:"actual helper-returned observations, not ArcEngineRecorder rows", complete:rows.every(row=>row.present && row.valid), rows};
}
const boundaryClasses = new Set(["ArcBrowserContextExtensions", "ArcExtensionInfo", "ArcExtensionCommand", "ArcExtensionRuntimeError", "ArcExtensionRuntimeErrorStackFrame", "ArcExtensionToolbarAction"]);
try {
  for (const agent of agents) {
    currentAgent = agent;
    const run: Run = { agent, fixtureMode, actions: [], errors: [], uncovered: [browserPins ? "ArcBrowser pinExtension:/unpinExtension:/moveExtension:targetIndex: optional phase selected but not completed; inspect actual per-operation evidence for partial coverage" : "ArcBrowser pinExtension:/unpinExtension:/moveExtension:targetIndex: metadata only; executable behavior uncovered", "Positive legacy background-page JavaScript uncovered; MV3 negative path only", browserOmnibox ? "ArcBrowser triggerExtensionOmniboxInput:inputText:disposition: optional phase selected but not completed; inspect actual per-operation evidence for partial coverage" : "ArcBrowser triggerExtensionOmniboxInput:inputText:disposition: metadata only; executable behavior uncovered"] }; runs.push(run);
    const dir = join(out, agent); mkdirSync(dir);
    const app = resolve(root, `../arc/build/ArcModAgent-${agent}.app`), exe = join(app, "Contents/MacOS/Arc");
    const plist = (key: string) => exec(["/usr/libexec/PlistBuddy", "-c", `Print :${key}`, join(app, "Contents/Info.plist")]);
    const request = join(dir, "extensions-service-request.json"), recordPath = join(dir, "record.jsonl");
    let pid: number | undefined, cdp: AgentCDP | undefined, target: string | undefined;
    let requestSequence = 0;
    let nativeReadinessSatisfied = false, installRequestsIssued = false, startupLoadIssued = false;
    const sampleFailedInstall = async (step: string, operation: string, error: unknown) => {
      const metadataPath = join(dir,`${step}.failed-install-sample.json`);
      const samplePath = join(dir,`${step}.failed-install.sample.txt`);
      const diagnostic: Record<string,any> = {step,operation,error:String(error),agent,pid:pid ?? null,
        requestedDurationSeconds:1,samplerTimeoutMs:30000,provenancePath:join(out,"provenance.json"),
        fixtureEnvironmentArguments:provenance.fixtureEnvironmentArguments,
        scope:"passive sample of this verified owned PID only; no debugger attach, global sample, callback substitution or dialog-cause assertion"};
      try {
        if (!nativeReadinessSatisfied || !pid || verifyAgentEndpoint(agent,port) !== pid) throw Error("owned PID/endpoint readiness verification failed; sample skipped");
        const inspect = () => ({command:exec(["/bin/ps","-p",String(pid),"-o","command="]),started:exec(["/bin/ps","-p",String(pid),"-o","lstart="])});
        const before = inspect();
        if (before.command !== exe && !before.command.startsWith(exe + " ")) throw Error("owned executable differs; sample skipped");
        diagnostic.before = before;
        diagnostic.provenanceUnchanged = sources.every(unchanged) && unchanged(builtEngine) && unchanged(selectedEngine(agent)) && unchanged(probeBinary) && unchanged(recorderBinary) && unchanged(imageFingerprintTool);
        const sampler = Bun.spawn(["/usr/bin/sample",String(pid),"1","1","-file",samplePath],{stdout:"pipe",stderr:"pipe",timeout:30000});
        const [exitCode,stdout,stderr] = await Promise.all([sampler.exited,new Response(sampler.stdout).text(),new Response(sampler.stderr).text()]);
        diagnostic.exitCode = exitCode; diagnostic.stdout = stdout; diagnostic.stderr = stderr;
        const after = inspect(); diagnostic.after = after;
        diagnostic.sameVerifiedProcess = after.command === before.command && after.started === before.started && verifyAgentEndpoint(agent,port) === pid;
        diagnostic.sample = existsSync(samplePath) ? fingerprint(samplePath) : null;
      } catch (sampleError) { diagnostic.sampleError = String(sampleError); }
      writeFileSync(metadataPath,JSON.stringify(diagnostic,null,2));
    };
    const action = async (step: string, operation: string, extra: Record<string, any> = {}): Promise<Action> => {
      if (!nativeReadinessSatisfied) throw Error("native PID-bound window/helper readiness required before probe request");
      if (!pid || verifyAgentEndpoint(agent, port) !== pid) throw Error("owned endpoint changed");
      const id = `${token}-${++requestSequence}`;
      if (operation === "install" || operation === "install-invalid") installRequestsIssued = true;
      writeFileSync(request + ".tmp", JSON.stringify({ id, token, operation, ...extra })); renameSync(request + ".tmp", request);
      try {
      const response = await wait(() => { const value = json(request + ".response.json"); return value.id === id ? value : undefined; }, 17000);
      if (response.pid !== pid || response.mainThread !== true) throw Error("probe acknowledgment PID/thread mismatch");
      const result = { step, ...response }; run.actions.push(result);
      requireNativeBooleanTypes(result);
      if (result.ok !== true) throw Error(`${step}: ${result.probeError}`);
      return result;
      } catch (error) {
        if (operation === "install" || operation === "install-invalid") await sampleFailedInstall(step,operation,error);
        throw error;
      }
    };
    const assert = (condition: unknown, message: string) => { if (!condition) throw Error(message); };
    try {
      assert(fixtureInputs.every(fixture => fixture.files.every(unchanged)),"generated fixture source changed before launch");
      assert(sources.every(unchanged) && unchanged(builtEngine) && unchanged(selectedEngine(agent)) && unchanged(probeBinary) && unchanged(recorderBinary) && unchanged(imageFingerprintTool), "provenance changed before launch; rebuild/repackage and use a fresh output directory");
      assert(plist("ArcModAgentSession") === agent && plist("CFBundleIdentifier") === `company.thebrowser.Browser.mod.agent.${agent}` && plist("CFBundleName") === `ArcModAgent-${agent}`, "disposable bundle identity mismatch");
      assert(!pids(exe).length, "agent already running; refusing attach/termination");
      assert(!Bun.spawnSync(["/usr/sbin/lsof", "-nP", `-iTCP:${port}`, "-sTCP:LISTEN"], { stdout: "ignore", stderr: "ignore" }).success, "CDP port occupied");
      const agentData = join(process.env.HOME!, "Library/Application Support", `ArcModAgent-${agent}`);
      const data = join(agentData, "Agent Chromium Data");
      mkdirSync(data, { recursive: true, mode: 0o700 });
      run.startupIntent = {fixtureMode, agent, app, executable:exe, startupArguments, fixtures:fixtureInputs, state:fixtureMode === "command-line" ? "startup-load-pending" : "public-install-not-issued"};
      writeFileSync(join(dir,"launch-intents.json"),JSON.stringify(run.startupIntent,null,2));
      startupLoadIssued = fixtureMode === "command-line";
      exec(["open", "-n", "-g", "-a", app,
        "--env", `DYLD_INSERT_LIBRARIES=${dylib}:${recorder}`, "--env", `ARC_RECORD_PATH=${recordPath}`,
        "--env", `ARC_EXTENSIONS_PROBE_REQUEST=${request}`, "--env", `ARC_EXTENSIONS_PROBE_FIXTURES=${fixtures}`, "--env", `ARC_EXTENSIONS_PROBE_TOKEN=${token}`, "--env", `ARC_EXTENSIONS_PROBE_FIXTURE_MODE=${fixtureMode}`, "--env", `ARC_EXTENSIONS_PROBE_BROWSER_PINS=${browserPins ? "1" : "0"}`, "--env", `ARC_EXTENSIONS_PROBE_BROWSER_OMNIBOX=${browserOmnibox ? "1" : "0"}`, "--env", `ARC_EXTENSIONS_PROBE_FIXTURE_URL=${fixtureURL}`,
        "--stdout", join(dir, "app.log"), "--stderr", join(dir, "app.log"), "--args", ...fixtureEnvironmentArguments,
        `--host-resolver-rules=${plist("ArcModHostResolverRules")}`, `--user-data-dir=${data}`, "--remote-debugging-address=127.0.0.1", `--remote-debugging-port=${port}`, ...startupArguments]);
      pid = await wait(() => pids(exe)[0], 30000); run.pid = pid;
      run.startupIntent.pid = pid; writeFileSync(join(dir,"launch-intents.json"),JSON.stringify(run.startupIntent,null,2));
      run.readiness = await waitForNativeReadiness(agent,pid,exe,agentData,request,recordPath,dir);
      nativeReadinessSatisfied = true;
      cdp = await AgentCDP.connectAgent(agent, port);
      target = (await cdp.request("Target.createTarget", { url: fixtureURL })).targetId as string;
      assert(typeof target === "string", "missing owned local fixture tab");
      await wait(async () => { const value = await cdp!.targetCommand(target!, "Runtime.evaluate", { expression: `document.readyState==='complete'&&document.querySelector('#fixture')?.dataset.token===${JSON.stringify(token)}`, returnByValue: true }); return (value.result as any)?.value === true; });
      const registry = await wait(async () => {
        try { const value = await action(`registry-${requestSequence + 1}`, fixtureMode === "command-line" ? "adopt-command-line" : "registry"); return value.identity && value.ready && (fixtureMode !== "command-line" || value.adoptionComplete === true) ? value : undefined; } catch { return undefined; }
      }, 25000);
      registry.step = "registry";
      assert(registry.identity && registry.ready, "canonical ready service unavailable");
      assert(registry.browserMatches === 1 && registry.browserPresent, "probe precondition failed: missing or ambiguous real app browser; no selection/activation attempted");
      // Failed discovery attempts are retained separately, outside exercised method spans.
      const discoveries = run.actions.filter(value => value !== registry); run.actions = [registry];
      writeFileSync(join(dir, "discovery.json"), JSON.stringify(discoveries, null, 2));
      if (fixtureMode === "command-line") {
        assert(registry.adoptionComplete === true && registry.startupPending?.length === 0 && Object.keys(registry.adopted ?? {}).length === 2,"startup fixture adoption unresolved");
        for (const fixture of fixtureInputs) {
          const info = registry.adopted[fixture.label];
          assert(info?.id === registry.owned[fixture.label] && info.name === fixture.name && info.version === fixture.version && info.fromWebstore === false,"adopted fixture ID/name/version proof failed");
        }
        run.startupIntent.state = "adopted-from-public-native-info"; run.startupIntent.actualIDs = registry.owned;
        writeFileSync(join(dir,"launch-intents.json"),JSON.stringify(run.startupIntent,null,2));
      }
      if (fixtureMode === "public-install") for (const label of ["A", "B"]) {
        const installed = await action(`install-${label}`, "install", { label });
        assert(installed.completions?.length === 1 && installed.completion?.result === 0 && installed.owned[label] === installed.completion.id, `${label}: public install completion/proven identity failed`);
      }
      const loaded = await action("snapshot-loaded", "snapshot");
      assert(loaded.list?.length === 2 && loaded.list.every((info: any) => typeof info.enabled === "boolean" && info.version === "1.2.3" && info.fromWebstore === false), "installed info/list typed metadata mismatch");
      const malformedInstall = async () => {
        const invalid = await action("install-invalid", "install-invalid");
        assert(invalid.completions?.length === 1 && invalid.completion?.result !== 0 && invalid.completion?.id === "", "invalid manifest unexpectedly installed");
      };
      if (fixtureMode === "public-install") await malformedInstall();
      // Preserve the installation result exactly, including an initially
      // disabled extension. Exercise the public enable operation explicitly;
      // never bypass unpacked-extension policy through devmode or CDP.
      for (const label of ["A", "B"]) await action(`enable-${label}-initial`, "enable", { label });
      const enabledInitial = await action("snapshot-enabled-initial", "snapshot");
      if (enabledInitial.list?.length !== 2 || !enabledInitial.list.every((info: any) => info.enabled === true)) {
        run.policyPrecondition = { step: "snapshot-enabled-initial", reason: "public enable did not produce two enabled fixtures", observed: enabledInitial.infosAfter };
        run.uncovered.push("commands, action effects, pin order and popup: public enable left fixture disabled; native policy precondition blocked further exercise");
        throw Error("probe policy precondition failed: public enable left a fixture disabled; initial and enable observations retained for per-operation parity");
      }
      assert(enabledInitial.commands?.filter((command: any) => command.commandName === "probe-command").length === 2, "missing real named command entries");
      assert(enabledInitial.commands?.filter((command: any) => command.commandName === "_execute_action").length === 0, "action shortcuts unexpectedly exposed as named commands");
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
      if (browserPins) {
        const phase = async (step: string, operation: string, label: string, expected: string[], anchorLabel?: string) => {
          const value = await action(step,operation,{label,...(anchorLabel ? {anchorLabel} : {})});
          assert(value.browserMatches === 1 && value.browserPinIdentityVerified === true &&
            value.browserPinBaselineBeforePreserved === true && value.browserPinBaselineAfterPreserved === true,
            `${step}: browser identity/baseline guard did not pass`);
          checkPins(value,expected);
          assert(Array.isArray(value.pinsBefore) && Array.isArray(value.browserPinArguments) &&
            value.browserPinArguments[0] === value.owned[label], `${step}: real browser arguments missing`);
          if (operation === "browser-move") {
            const index = value.browserPinArguments[1];
            assert(Number.isSafeInteger(index) && index >= 0 && index < value.pinsBefore.length &&
              value.pinsBefore[index] === value.owned[anchorLabel!], `${step}: target index is not the actual owned anchor`);
            const expectedFull = [...value.pinsBefore];
            const source = expectedFull.indexOf(value.owned[label]);
            assert(source >= 0,`${step}: moved fixture was not pinned`);
            expectedFull.splice(source,1); expectedFull.splice(index,0,value.owned[label]);
            assert(JSON.stringify(value.pinsAfter) === JSON.stringify(expectedFull),`${step}: full visible pin splice differs`);
          }
          return value;
        };
        const entryPins = await phase("browser-unpin-A-start","browser-unpin","A",[]);
        await phase("browser-pin-A","browser-pin","A",["A"]);
        await phase("browser-pin-A-again","browser-pin","A",["A"]);
        const both = await phase("browser-pin-B","browser-pin","B",["A","B"]);
        assert(both.pinsAfter.indexOf(both.owned.B) === both.pinsAfter.indexOf(both.owned.A) + 1,
          "browser pin fixtures are not adjacent; move precondition unproved");
        const observedToolbarIDs = both.callbacks.flatMap((event: any) => event.selector === "browser:didUpdateExtensionToolbarActions:" ? event.payload[0].map((item: any) => item.id) : event.selector === "browser:didUpdateExtensionToolbarAction:" ? [event.payload[0]?.id] : []);
        assert([both.owned.A,both.owned.B].every(id => observedToolbarIDs.includes(id)),
          "browser move precondition failed: no actual toolbar snapshots prove both fixture action models");
        await phase("browser-move-B-left","browser-move","B",["B","A"],"A");
        await phase("browser-move-B-same","browser-move","B",["B","A"],"B");
        await phase("browser-move-B-right","browser-move","B",["A","B"],"A");
        await phase("browser-unpin-A","browser-unpin","A",["B"]);
        await phase("browser-unpin-A-again","browser-unpin","A",["B"]);
        await phase("browser-unpin-B","browser-unpin","B",[]);
        const restoredPins = await phase("browser-pin-A-restore","browser-pin","A",["A"]);
        assert(JSON.stringify(restoredPins.pinsAfter) === JSON.stringify(entryPins.pinsBefore),
          "browser pin phase did not restore its full entry order");
        run.uncovered = run.uncovered.filter(value => !value.startsWith("ArcBrowser pinExtension:"));
        run.uncovered.push("Browser pin/unpin/move exercised only for enabled nonce-owned A/B; disabled/OTR/force-pinned/invalid-ID/index paths remain uncovered");
      }
      const beforeCommand = effects.length;
      const command = await action("command-A", "command", { label: "A", command: "probe-command" });
      command.effects = await wait(() => { const events = effects.slice(beforeCommand).filter(event => event.agent === agent && event.label === "A" && event.value.kind === "command"); return events.length ? events : undefined; }, 8000);
      assert(command.effects[0].value.id === command.owned.A && command.effects[0].value.command === "probe-command", "named command effect not from installed fixture");
      const commandPayload = command.effects[0].value;
      assert(Number.isSafeInteger(commandPayload.argumentCount) && commandPayload.argumentCount >= 1 &&
        Array.isArray(commandPayload.argumentTypes) && commandPayload.argumentTypes.length === commandPayload.argumentCount &&
        commandPayload.argumentTypes[0] === "string", "named command listener argument observations are malformed");
      // The native payload has only the command string. Preserve listener
      // argumentCount exactly for paired comparison, allowing an optional
      // undefined slot if the original engine's bindings pad its schema.
      assert(commandPayload.noTab === true && commandPayload.argumentTypes.slice(1).every((type: string) => type === "undefined"),
        "native command dispatch unexpectedly supplied a tab or other argument");
      const beforeAction = effects.length;
      const clicked = await action("action-B", "action", { label: "B" });
      clicked.effects = await wait(() => { const events = effects.slice(beforeAction).filter(event => event.agent === agent && event.label === "B" && event.value.kind === "action"); return events.length ? events : undefined; }, 8000);
      assert(clicked.effects[0].value.id === clicked.owned.B, "action effect not from installed fixture");
      assert(clicked.effects[0].value.tab?.url === fixtureURL && clicked.effects[0].value.tab?.idPresent === true, "probe precondition failed: action selected a different browser/tab; no activation attempted");
      if (browserOmnibox) {
        // All inventories below come from real action/omnibox extension listeners
        // or CDP observations. Neither listener creates tabs/windows or navigates.
        const proveInventory = (value: any, previous?: any) => {
          assert(value?.nonce === token && [clicked.owned.A,clicked.owned.B].includes(value.id),
            "omnibox inventory nonce/owned extension ID mismatch");
          assert(Array.isArray(value.tabs) && value.tabs.length === 1 && Array.isArray(value.windows) && value.windows.length === 1,
            "omnibox precondition requires exactly one actual tab/window; no activation or removal attempted");
          const tab = value.tabs[0], window = value.windows[0];
          assert(Number.isSafeInteger(tab.id) && Number.isSafeInteger(tab.windowId) && Number.isSafeInteger(window.id) &&
            tab.id >= 0 && tab.windowId >= 0 && window.id >= 0 && tab.windowId === window.id && tab.url === fixtureURL && tab.active === true && tab.incognito === false &&
            window.type === "normal" && window.incognito === false, "omnibox active tab/window is not the exact nonce fixture");
          if (value.tab) assert(value.tab.id === tab.id && value.tab.windowId === window.id && value.tab.url === fixtureURL &&
            value.tab.active === true && value.tab.incognito === false,"omnibox clicked tab differs from inventory");
          if (previous) assert(tab.id === previous.tabs[0].id && window.id === previous.windows[0].id,
            "omnibox changed actual tab/window identity");
        };
        const cdpFixture = async () => {
          assert(pid && verifyAgentEndpoint(agent,port) === pid,"omnibox endpoint PID changed");
          const page = await cdp!.targetCommand(target!,"Runtime.evaluate",{expression:`({url:location.href,token:document.querySelector('#fixture')?.dataset.token})`,returnByValue:true});
          const value = (page.result as any)?.value;
          assert(value?.url === fixtureURL && value?.token === token,"omnibox CDP target is no longer nonce fixture");
          const window = await cdp!.request("Browser.getWindowForTarget",{targetId:target});
          assert(typeof window.windowId === "number" && Number.isSafeInteger(window.windowId) && window.windowId >= 0,"omnibox CDP window identity missing");
          return {targetId:target!,windowId:window.windowId,fixtureURL:value.url,nonce:value.token};
        };
        const initialPreflights = await wait(()=>{
          const values=effects.slice(beforeAction).filter(event=>event.agent === agent && event.label === "B" && event.value.kind === "omnibox-preflight");
          return values.length ? values : undefined;
        },8000);
        assert(initialPreflights.length === 1,"initial B action must have exactly one genuine inventory report");
        clicked.omniboxPreflight = initialPreflights[0].value;
        proveInventory(initialPreflights[0].value);
        let previousSequence = initialPreflights[0].value.sequence;
        assert(Number.isSafeInteger(previousSequence) && previousSequence > 0,"initial B action sequence missing");
        const observedInputs: string[] = [];
        for (let disposition = 0; disposition <= 11; ++disposition) {
          const beforePreflight = effects.length;
          const preflight = await action(`omnibox-preflight-${disposition}`,"action",{label:"B"});
          preflight.effects = await wait(()=>{
            const values=effects.slice(beforePreflight).filter(event=>event.agent===agent && event.label==="B" && event.value.kind==="omnibox-preflight");
            return values.length ? values : undefined;
          },8000);
          assert(preflight.effects.length === 1 && preflight.effects[0].value.id === preflight.owned.B &&
            preflight.effects[0].value.sequence === previousSequence + 1,"omnibox preflight stale/duplicate/foreign B effect");
          const before = preflight.effects[0].value; previousSequence = before.sequence;
          preflight.omniboxPreflight = before;
          const realClicks = effects.slice(beforePreflight).filter(event=>event.agent===agent && event.label==="B" && event.value.kind==="action");
          assert(realClicks.length===1 && realClicks[0].value.id===preflight.owned.B && realClicks[0].value.tab?.url===fixtureURL &&
            realClicks[0].value.tab?.idPresent===true,"omnibox preflight ordinary B action effect missing or foreign");
          preflight.effects = effects.slice(beforePreflight).filter(event=>event.agent===agent && event.label==="B" && ["action","omnibox-preflight"].includes(event.value.kind));
          proveInventory(before,initialPreflights[0].value);
          const cdpBefore = await cdpFixture();
          assert(cdpBefore.windowId === before.windows[0].id,"omnibox CDP target belongs to another extension-reported window");
          const inputText = `${token}/omnibox/${disposition}/π 東京 🧭`, beforeInput = effects.length;
          assert(!observedInputs.includes(inputText),"omnibox input nonce reused"); observedInputs.push(inputText);
          const entered = await action(`omnibox-${disposition}`,"browser-omnibox",{label:"A",disposition,inputText,fixtureURL});
          entered.omniboxPreflight = before; entered.omniboxCDPBefore = cdpBefore;
          entered.effects = await wait(()=>{
            const values=effects.slice(beforeInput).filter(event=>event.agent===agent && event.label==="A" && event.value.kind==="omnibox" && event.value.input===inputText);
            return values.length ? values : undefined;
          },8000);
          assert(entered.browserMatches === 1 && entered.browserOmniboxIdentityVerified === true &&
            entered.browserOmniboxFixtureURL === fixtureURL &&
            JSON.stringify(entered.browserOmniboxArguments) === JSON.stringify([entered.owned.A,inputText,disposition]),"omnibox real browser identity/arguments missing");
          assert(entered.effects.length === 1,"omnibox duplicate input listener effects");
          const effect = entered.effects[0].value;
          assert(effect.id === entered.owned.A && effect.nonce === token && effect.input === inputText &&
            typeof effect.disposition === "string" && effect.argumentCount >= 2 && Number.isSafeInteger(effect.argumentCount) &&
            Array.isArray(effect.argumentTypes) && effect.argumentTypes.length === effect.argumentCount &&
            effect.argumentTypes[0] === "string" && effect.argumentTypes[1] === "string" &&
            effect.argumentTypes.slice(2).every((type:string)=>type === "undefined"),"omnibox listener payload is foreign or malformed");
          const expected = [2,9,10].includes(disposition) ? "newForegroundTab" : disposition === 3 ? "newBackgroundTab" : "currentTab";
          assert(effect.disposition === expected,"omnibox listener disposition differs from native three-string projection");
          proveInventory(effect,before);
          entered.omniboxCDPAfter = await cdpFixture();
          assert(JSON.stringify(entered.omniboxCDPAfter) === JSON.stringify(cdpBefore),"omnibox CDP nonce target/window changed");
          assert(JSON.stringify(entered.pinsAfter) === JSON.stringify(clicked.pinsAfter),"omnibox changed full pin order");
        }
        const actualInputs = effects.filter(event=>event.agent===agent && event.value.kind==="omnibox");
        assert(actualInputs.length === 12 && actualInputs.every(event=>event.label==="A" && event.value.id===clicked.owned.A && event.value.nonce===token && observedInputs.includes(event.value.input)) &&
          new Set(actualInputs.map(event=>event.value.input)).size===12,"omnibox phase has duplicate/foreign/stale input effects");
        run.uncovered = run.uncovered.filter(value=>!value.startsWith("ArcBrowser triggerExtensionOmniboxInput:"));
        run.uncovered.push("Omnibox valid Arc ordinals 0..11 observed only through three native onInputEntered strings; internal window/popup/OTR casts and independent activeTab permission changes remain unproved");
      }
      const js = await action("background-js-MV3", "background-js", { label: "A" });
      assert(js.value === false, "MV3 worker unexpectedly treated as background page");
      const disabled = await action("disable-A", "disable", { label: "A" });
      assert(disabled.infosAfter.A?.enabled === false, "disable did not reach registry");
      const disabledSnapshot = await action("snapshot-disabled", "snapshot");
      assert(disabledSnapshot.list?.some((info: any) => info.id === disabled.owned.A && info.enabled === false), "list omitted disabled fixture");
      const enabled = await action("enable-A", "enable", { label: "A" });
      assert(enabled.infosAfter.A?.enabled === true, "enable did not reach registry");
      if (fixtureMode === "public-install") {
        const reloaded = await action("reload-A", "reload", { label: "A" });
        assert(reloaded.infosAfter.A?.enabled === true, "reload did not return to enabled state");
      }
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
      if (fixtureMode === "command-line") {
        const reloaded = await action("reload-A", "reload", { label: "A" });
        assert(typeof reloaded.infosAfter.A?.enabled === "boolean", "command-line reload returned no typed enabled observation");
        run.reloadObservation = reloaded.infosAfter.A;
        // A stalled malformed install must not erase earlier positive CLI
        // observations; its real completion/error and cleanup stay strict.
        await malformedInstall();
      }
      const removed = await action("uninstall-A", "uninstall", { label: "A" });
      assert(removed.infosAfter.A === null, "uninstall left fixture installed");
      const cleanup = await action("cleanup", "cleanup");
      assert(cleanup.cleanupCertified === true && cleanup.outstandingInstalls?.length === 0 && Object.values(cleanup.infosAfter).every(value => value === null), "cleanup not certified; fixture installed or installation unresolved");
      assert(JSON.stringify(cleanup.pinsAfter) === JSON.stringify(registry.baselinePins), "cleanup did not restore baseline pins");
      run.cleanup = "fixture IDs absent; baseline pins restored";
    } catch (error) { run.errors.push(String(error)); }
    finally {
      if (pid && !run.cleanup && !installRequestsIssued && !startupLoadIssued) run.cleanup = "no fixture installation or pin mutation issued; native readiness/registry precondition failed";
      if (pid && !run.cleanup && (installRequestsIssued || startupLoadIssued)) {
        try {
          const cleanup = await action("cleanup-after-error", "cleanup");
          if (cleanup.cleanupCertified !== true || cleanup.outstandingInstalls?.length !== 0 || (fixtureMode === "command-line" && cleanup.startupPending?.length !== 0)) throw Error("cleanup not certified; unresolved install or fixture state");
          run.cleanup = "reconciled nonce fixtures absent; no outstanding installs; baseline pins restored";
        }
        catch (error) { run.errors.push(`fixture cleanup: ${error}`); }
      }
      if (cdp && target) try { await cdp.request("Target.closeTarget", { targetId: target }); } catch {}
      if (pid) {
        try {
          if (!nativeReadinessSatisfied) throw Error("native readiness never completed; terminate owned PID without CDP mutation");
          if (verifyAgentEndpoint(agent, port) !== pid) throw Error("owned endpoint changed");
          cdp ??= await AgentCDP.connectAgent(agent, port);
          await cdp.request("Browser.close").catch(() => {}); await wait(() => !alive(pid!), 12000);
        } catch {
          if (pids(exe).includes(pid)) Bun.spawnSync(["kill", "-TERM", String(pid)]);
          try { await wait(() => !alive(pid!), 12000); } catch { run.errors.push("launched PID did not exit"); }
        }
      }
      cdp?.close();
      if (!run.readiness && existsSync(join(dir, "readiness.json"))) run.readiness = json(join(dir, "readiness.json"));
      if (existsSync(request + ".owned.json")) run.ownership = json(request + ".owned.json");
      if (startupLoadIssued && (!run.ownership || run.ownership.startupPending?.length !== 0)) run.errors.push("command-line startup ownership unresolved; fixture cleanup cannot be certified even after process exit");
      if (run.ownership?.outstandingInstalls?.length) run.errors.push("outstanding installation callbacks remained at process termination; cleanup cannot be certified");
      run.diagnostics = existsSync(request + ".diagnostics.json") ? json(request + ".diagnostics.json") : [];
      const fixtureIDs = Object.values(run.ownership?.owned ?? {});
      const relevantDiagnostics = run.diagnostics!.filter(event => {
        const serialized = JSON.stringify(event); return (serialized.includes(token) || fixtureIDs.some(id => serialized.includes(String(id)))) && !(fixtureMode === "command-line" && event.kind === "startup-observation");
      });
      run.startupCallbacks = (run.diagnostics ?? []).filter(event => fixtureMode === "command-line" && event.kind === "startup-observation");
      writeFileSync(join(dir,"startup-callbacks.json"),JSON.stringify(run.startupCallbacks,null,2));
      if (!fixtureInputs.every(fixture => fixture.files.every(unchanged))) run.errors.push("generated fixture source changed during run");
      if (relevantDiagnostics.length) run.errors.push(`${relevantDiagnostics.length} late/unassigned/outside-context fixture callbacks or completions; inspect diagnostics`);
      if (!sources.every(unchanged) || !unchanged(builtEngine) || !unchanged(selectedEngine(agent)) || !unchanged(probeBinary) || !unchanged(recorderBinary) || !unchanged(imageFingerprintTool)) run.errors.push("provenance changed during run; snapshot cannot establish current-source parity");
      if (existsSync(recordPath)) {
        const raw = readFileSync(recordPath);
        for (const value of run.actions) {
          const begin = value.recordBegin, end = value.recordEnd;
          if (!Number.isInteger(begin) || !Number.isInteger(end) || begin < 0 || end < begin || end > raw.length ||
              (begin && raw[begin - 1] !== 10) || (end > begin && raw[end - 1] !== 10)) { run.errors.push(`${value.step}: invalid recorder span`); continue; }
          const slice = raw.subarray(begin, end); writeFileSync(join(dir, `${value.step}.raw.record.jsonl`), slice);
          // Preserve true caller labels. This projection and the default
          // comparator exclude some probe calls; raw v2 inputs retain them.
          // Retained app/engine events are supplementary. Fixture coverage
          // additionally requires a verified paired public-call inventory.
          const selected = slice.toString().split("\n").filter(Boolean).map(line => JSON.parse(line)).filter(event =>
            boundaryClasses.has(event.cls) || callbackSelectors.has(event.sel) ||
            (event.cls === "ArcBrowser" && event.sel === "executeExtensionAction:"));
          writeFileSync(join(dir, `${value.step}.record.jsonl`), selected.map(event => JSON.stringify(event)).join("\n") + (selected.length ? "\n" : ""));
          writeFileSync(join(dir, `${value.step}.retained-row-inventory.json`), JSON.stringify({
            fixtureMethodCoverage:"unavailable: scoped projection/default comparator exclude probe calls; raw v2 inputs retain them, but no fixture admission or verified paired call/return inventory is applied", fixtureMethodInventory:[],
            retainedRows:selected.map(({dir,ev,kind,cls,sel,from}:any)=>({dir,ev,kind,cls,sel,from})),
          },null,2));
        }
      }
      for (const observed of run.actions) writeFileSync(join(dir,`${observed.step}.required-rows.json`),JSON.stringify(requiredRows(observed),null,2));
      if (browserOmnibox) {
        const attempts = run.actions.filter(value=>value.operation === "browser-omnibox");
        const preflights = effects.filter(event=>event.agent===agent && event.value.kind==="omnibox-preflight");
        const expectedPreflights = run.actions.filter(value=>value.step==="action-B" || value.step.startsWith("omnibox-preflight-")).length;
        if (preflights.length!==expectedPreflights || preflights.some((event,index)=>event.label!=="B" || event.value.id!==run.ownership?.owned?.B ||
            event.value.nonce!==token || event.value.sequence!==index+1))
          run.errors.push("omnibox late/duplicate/foreign/missing genuine B preflight effects; partial evidence retained");
        const entered = effects.filter(event=>event.agent===agent && event.value.kind==="omnibox");
        const expected = attempts.map(value=>value.browserOmniboxArguments?.[1]).filter(value=>typeof value === "string");
        if (entered.length !== expected.length || new Set(entered.map(event=>event.value.input)).size !== entered.length ||
            entered.some(event=>event.label!=="A" || event.value.id!==run.ownership?.owned?.A || event.value.nonce!==token || !expected.includes(event.value.input)))
          run.errors.push("omnibox late/duplicate/foreign/missing actual listener effects; partial evidence retained");
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
function semantic(action: Action) {
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
  const keep = ["step", "operation", "ok", "probeError", "matches", "mainThread", "metadata", "ready", "identity", "hashStable", "browserPresent", "browserMatches", "completion", "completions", "outstandingInstalls", "cleanupCertified", "fixtureMode", "startupPending", "adoptionComplete", "adopted", "list", "commands", "infosAfter", "owned", "value", "browserPinSelector", "browserPinABI", "browserPinLabel", "browserPinArguments", "browserPinAnchorLabel", "browserPinIdentityVerified", "browserPinBaselineBeforePreserved", "browserPinBaselineAfterPreserved", "browserOmniboxSelector", "browserOmniboxABI", "browserOmniboxArguments", "browserOmniboxIdentityVerified", "browserOmniboxFixtureURL", "omniboxPreflight", "omniboxCDPBefore", "omniboxCDPAfter"];
  const result: Record<string, any> = Object.fromEntries(keep.filter(key => key in action).map(key => [key, action[key]]));
  result.callbacks = callbacks;
  result.eventOrder = (action.events ?? []).flatMap((event: any) => {
    if (event.type === "completion") return [{ type: "completion", value: event.value }];
    const callback = scopeCallback(event.value);
    return callback ? [{ type: "delegate", value: callback }] : [];
  });
  if ("pinsBefore" in action) result.fixturePinsBefore = action.pinsBefore.filter(own);
  result.fixturePinsAfter = (action.pinsAfter ?? []).filter(own);
  if ("baselinePins" in action) result.fixtureBaselinePins = action.baselinePins === null ? null : action.baselinePins.filter(own);
  if (action.effects) result.effects = action.effects.map(({ label, value }: any) => ({ label, value }));
  return result;
};
const golden = runs.find(run => run.agent === "trace156"), ours = runs.find(run => run.agent === "ours156");
const differences: any[] = [], boundaryDiffs: any[] = [], boundaryUnavailable: any[] = [], requiredRowDifferences: any[] = [], operationParity: any[] = [];
if (golden && ours) {
  if (fixtureMode === "command-line") {
    const startup = (run: Run) => (run.startupCallbacks ?? []).map(item => ({kind:item.kind,event:{selector:item.event.selector,payload:item.event.payload,mainThread:item.event.mainThread,synchronous:item.event.synchronous}}));
    const leftStartup = startup(golden), rightStartup = startup(ours);
    writeFileSync(join(out,"startup-callbacks.semantic.json"),JSON.stringify({golden:leftStartup,ours:rightStartup},null,2));
    if (JSON.stringify(leftStartup) !== JSON.stringify(rightStartup)) differences.push({step:"startup-callbacks",golden:leftStartup,ours:rightStartup});
  }
  const steps = new Set([...golden.actions, ...ours.actions].map(action => action.step));
  for (const step of steps) {
    const left = golden.actions.find(action => action.step === step), right = ours.actions.find(action => action.step === step);
    const a = left ? semantic(left) : null, b = right ? semantic(right) : null;
    writeFileSync(join(out, `${step}.semantic.json`), JSON.stringify({ golden: a, ours: b }, null, 2));
    if (JSON.stringify(a) !== JSON.stringify(b)) differences.push({ step, golden: a, ours: b });
    const leftRows = left ? requiredRows(left) : null, rightRows = right ? requiredRows(right) : null;
    writeFileSync(join(out,`${step}.required-rows.json`),JSON.stringify({golden:leftRows,ours:rightRows},null,2));
    if (!leftRows?.complete || !rightRows?.complete || JSON.stringify(leftRows) !== JSON.stringify(rightRows)) requiredRowDifferences.push({step,golden:leftRows,ours:rightRows});
    const leftPath = join(out, "trace156", `${step}.record.jsonl`), rightPath = join(out, "ours156", `${step}.record.jsonl`);
    if (!existsSync(leftPath) || !existsSync(rightPath)) { boundaryDiffs.push({ step, error: "missing scoped record" }); continue; }
    boundaryUnavailable.push({step,reason:"no verified paired fixture-call/return inventory: scoped projection/default comparator exclude probe calls; raw v2 inputs retain them, but retained comparison rows do not prove fixture method coverage"});
    if (!readFileSync(leftPath,"utf8").trim() && !readFileSync(rightPath,"utf8").trim()) continue;
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
if (golden && ours) {
  const steps = new Set([...golden.actions, ...ours.actions].map(action => action.step));
  for (const step of steps) operationParity.push({
    step, observedInBoth: golden.actions.some(action => action.step === step) && ours.actions.some(action => action.step === step),
    semanticEqual: !differences.some(value => value.step === step),
    requiredRowsEqual: !requiredRowDifferences.some(value => value.step === step),
    scopedBoundaryEqual: boundaryDiffs.some(value=>value.step===step) ? false : boundaryUnavailable.some(value=>value.step===step) ? null : true,
    boundaryMethodParity:null,
  });
}
const validObservation = (action: Action, run: Run) => action.ok === true && action.mainThread === true && action.pid === run.pid &&
  action.matches === 1 && Number.isInteger(action.recordBegin) && Number.isInteger(action.recordEnd) && action.recordEnd >= action.recordBegin && requiredRows(action).complete;
const comparedOperationCount = golden && ours ? golden.actions.filter(left => {
  const right = ours.actions.find(action => action.step === left.step);
  return right && validObservation(left,golden) && validObservation(right,ours);
}).length : 0;
const observedParity = golden && ours ? (differences.length || requiredRowDifferences.length ? false : comparedOperationCount > 0 ? true : null) : null;
const standardBoundaryParity = golden && ours ? (boundaryDiffs.length ? false : comparedOperationCount > 0 && !boundaryUnavailable.length ? true : null) : null;
// No verified paired fixture-caller inventory is applied. Never promote unrelated retained
// rows or an empty default diff to public API call/return proof.
const boundaryMethodParity = null;
const parity = golden && ours ? (!complete || observedParity === false || standardBoundaryParity === false ? false : boundaryMethodParity) : null;
const notes = [
  diagnosticDisableFieldTrialConfig ? "DIAGNOSTIC environment: both engines explicitly receive --disable-field-trial-config. This tests an explicit launch condition; it does not establish default startup behavior or integrate the lead-owned bootstrap policy." : "Default fixture environment: no explicit field-trial-config switch is supplied by this helper.",
  "Both disposable launches include Chromium's --noerrdialogs (base::switches::kNoErrorDialogs) as an explicit fixture environment precondition, suppressing modal install-error presentation while preserving native LoadErrorReporter errors/callbacks. This does not establish default hosted warning-presentation parity.",
  "Command-line mode is a separate fixture phase using explicit --load-extension paths, not public-install success evidence. Startup intents and exact fixture file hashes are recorded before launch; all real callbacks entering before adoption are independently retained as startup-observation diagnostics, including callbacks during incomplete discovery requests and compared separately without reassigning them to later requests; adopted IDs/names/versions come only from actual public native info. Readiness alone never resolves a missing startup fixture. CLI baseline pins exclude only adopted IDs. Positive command/pin/popup probes precede reload; the returned reload enabled state is preserved exactly because current Chromium may convert command-line loading to normal unpacked policy.",
  "Default public-install retains malformed-manifest installation before public enable. Separate CLI mode moves this error diagnostic after positive command/pin/popup and reload observations; the same ordering applies to both engines. Failed install requests trigger a bounded passive one-second sample only after exact owned PID/executable/endpoint verification, before cleanup. Sample failure remains a diagnostic; native completion/ownership assertions are unchanged.",
  "Supplemental probe only; canonical extensions scenario acceptance remains separate.",
  "Native metadata encodings and fixture DTO scalar/string/ordered-array observations compare exactly; image observations include NSImage dimensions and representation sizes, not pixel equivalence.",
  "Semantic comparison omits transport PID/request IDs, recorder offsets and engine labels; callback and pin arrays are scoped to proven fixture IDs. Each baseline pin order is independently restored and asserted.",
  "Raw v2 recordings retain probe-origin calls. The current scoped projection and default comparator exclude probe calls; no fixture admission or verified paired call/return inventory is applied. Exact helper semantic and required-row sidecars remain a separate evidence stream. Without that inventory, standardBoundaryParity and boundaryMethodParity remain null even if retained comparison rows are clean. Known mismatches remain false and dominate unavailable coverage. At least one valid paired helper observation is required for observedParity.",
  "Scoped boundary records preserve true caller labels and may include real app callbacks or app/engine calls. Their class/selector projection excludes some public calls; the default comparator also excludes probe origins. Full raw inputs are retained separately. No synthetic recorder rows, fake app attribution or caller-label removal is used.",
  "Existing diff.py renumbers recording object identities and masks loopback ports, temporary folders and UUID strings. The exact supplemental semantic comparison applies none of those string masks.",
  browserPins ? "Explicit --browser-pins CLI phase calls real browser pin/unpin/move selectors only on enabled nonce-owned A/B, with exact service/browser identity, Q target ABI, actual before/after ordered snapshots, untouched baseline assertions, and no synthetic callbacks; default sequence and original cleanup remain. Invalid/disabled/OTR/force-pinned paths remain uncovered." : "ArcBrowser pinExtension:, unpinExtension:, moveExtension:targetIndex: are metadata-only here; their executable behavior is uncovered. Context setPinnedExtensionOrder: is exercised separately when the native enable precondition permits it.",
  browserOmnibox ? "Explicit --browser-omnibox CLI phase uses real B-action preflight inventories then actual A omnibox calls for ordinals0..11. Both fixtures and exact browser/service are reverified each call; the sole actual nonce tab/window must retain identity. Input/nonce/listener strings and raw actual tab/window/CDP IDs remain in evidence/semantic comparison, so native SessionID differences can remain mismatches. Three event strings do not prove internal disposition casts/window behavior or an independent permission grant; v2 method inventory remains unknown." : "ArcBrowser triggerExtensionOmniboxInput:inputText:disposition: is metadata-only; executable behavior remains uncovered.",
  "MV3 background-page JavaScript negative path only; positive legacy background-page execution is uncovered and no legacy fixture is enabled.",
  "Browser captures retain all weak live browser/delegate entries per context; ambiguous selection fails a probe precondition. Action-B onClicked effects must prove a real numeric tab ID and the exact nonce fixture URL; no activation is attempted.",
  "Context command dispatch targets an enabled extension, supplies only its command string, sets user gesture, and has no target-tab or independent activeTab permission-grant coverage. Listener argumentCount/types and noTab are retained exactly; optional undefined binding padding is observed rather than invented or normalized.",
  "provenance.json requires all original metadata class exports and records raw engine/probe/recorder/source hashes plus framework versions. Build/package equality compares every pre-signature Mach-O byte except its signature locator and signature-dependent __LINKEDIT sizes; ArcMod ad-hoc re-signing changes those fields and the trailing signature. The guarded read-only fingerprint helper and its hash are recorded. All owned product sources must precede the build; this is snapshot-specific.",
  "Install attempts are journaled before invocation. Cleanup waits for all outstanding completions, reconciles exact nonce names/verified IDs from the actual installed-extension list, then certifies fixture absence and baseline pin restoration. Timeouts leave explicit unresolved ownership evidence rather than claiming cleanup.",
  "Callback operation attribution is captured under a synchronization lock at callback entry. Deferred late/unassigned/outside-context deliveries are retained in diagnostics; fixture-related diagnostics fail completion rather than being reassigned to a future request.",
  "PID-bound helper, actual app-window readiness (including agent identity), recorder and verified CDP process must all agree within 120 seconds before any probe request or CDP mutation. Current stage observations are persisted to readiness.json; startup failure issues no cleanup mutation when no install was attempted.",
  "Helper Boolean expressions emit Foundation Boolean singletons; DTO Boolean getters are invoked using their declared B/c metadata and reject unexpected encodings/noncanonical char values. The runner rejects numeric substitutes for Boolean fields; native false remains false.",
  "Initial installed enabled flags are retained exactly. Explicit public enable operations precede enabled-dependent exercise; policy blockage keeps per-operation observations/diffs but marks the overall probe incomplete and downstream coverage uncovered.",
  "Callbacks are observed within 800ms after method return/completion; popup actions use 3 seconds. Absence beyond those windows is not a synthesized callback result.",
];
writeFileSync(join(out, "summary.json"), JSON.stringify({ complete, parity, observedParity, standardBoundaryParity, boundaryMethodParity, comparedOperationCount, operationParity, differences, requiredRowDifferences, boundaryDiffs, boundaryUnavailable, fixtureMode, fixtureInputs, provenance, notes, runs }, null, 2));
console.log(JSON.stringify({ complete, parity, observedParity, standardBoundaryParity, boundaryMethodParity, comparedOperationCount, fixtureMode, out, errors: runs.map(run => ({ agent: run.agent, errors: run.errors, uncovered: run.uncovered })) }));
process.exitCode = complete && (!(golden && ours) || observedParity === true) && standardBoundaryParity !== false ? 0 : 1;
