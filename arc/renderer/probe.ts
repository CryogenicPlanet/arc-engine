#!/usr/bin/env bun
// Test-only direct boundary parity probe. Run ONLY through arc/tools/locked.sh.
// No existing data is deleted. Only trace156/ours156 apps and their own domains are used.
import { AgentCDP, verifyAgentEndpoint } from "../../../arc/tools/agent-cdp";
import { copyFileSync, constants, existsSync, mkdirSync, readFileSync, readdirSync, realpathSync, renameSync, statSync, writeFileSync } from "node:fs";
import { join, resolve } from "node:path";

const root = resolve(import.meta.dir, "../..");
const args = process.argv.slice(2);
const option = (name: string, fallback?: string): string | undefined => {
  const index = args.indexOf(name);
  return index < 0 ? fallback : args[index + 1];
};
if (!option("--out")) throw new Error("usage: probe.sh run --out DIR [--agents trace156,ours156] [--port 9447]");
const out = resolve(option("--out")!);
const agents = option("--agents", "trace156,ours156")!.split(",");
if (!agents.length || new Set(agents).size !== agents.length || agents.some(a => !["trace156", "ours156"].includes(a))) {
  throw new Error("only trace156 and ours156 are allowed, each at most once");
}
const port = Number(option("--port", "9447"));
if (!Number.isInteger(port) || port < 1024 || port > 65535) throw new Error("invalid CDP port");
const adblockIdleMs = Number(option("--idle-before-adblock-ms", "0"));
if (!Number.isInteger(adblockIdleMs) || adblockIdleMs < 0 || adblockIdleMs > 60000) {
  throw new Error("idle-before-adblock-ms must be an integer from 0 through 60000");
}
const adblockReadinessMs = Number(option("--adblock-readiness-ms", "60000"));
if (!Number.isInteger(adblockReadinessMs) || adblockReadinessMs < 1000 || adblockReadinessMs > 240000) {
  throw new Error("adblock-readiness-ms must be an integer from 1000 through 240000");
}
if (args.includes("--adblock-queue-edges") && (!args.includes("--adblock-toggle") || !args.includes("--adblock"))) {
  throw new Error("adblock-queue-edges requires adblock and adblock-toggle");
}
const lockPID = Number(readFileSync(join(root, "logs/arccore-build.lock/pid"), "utf8").trim());
let ancestor = process.pid;
for (let depth = 0; ancestor !== lockPID && ancestor > 1 && depth < 20; ++depth) {
  ancestor = Number(Bun.spawnSync(["ps", "-p", String(ancestor), "-o", "ppid="], { stdout: "pipe" }).stdout.toString().trim());
}
if (ancestor !== lockPID) throw new Error("run this probe under arc/tools/locked.sh; it never acquires the lock itself");
const dylib = resolve(option("--dylib", join(root, "logs/wp/renderer-probe-build/RendererProbe.dylib"))!);
const recorderSource = join(root, "tests/recorder/ArcEngineRecorder.dylib");
for (const path of [dylib, recorderSource]) if (!existsSync(path)) throw new Error(`missing ${path}`);
mkdirSync(out, { recursive: true });
// Preserve the exact recorder bytes for both apps even if another worker rebuilds the shared recorder.
const recorder = join(out, "ArcEngineRecorder.frozen.dylib");
copyFileSync(recorderSource, recorder, constants.COPYFILE_EXCL);
const sleep = (ms: number) => new Promise(r => setTimeout(r, ms));
const wait = async <T>(fn: () => T | Promise<T>, timeout = 20000): Promise<NonNullable<T>> => {
  const deadline = Date.now() + timeout;
  let error: unknown;
  while (Date.now() < deadline) {
    try { const result = await fn(); if (result) return result as NonNullable<T>; } catch (e) { error = e; }
    await sleep(150);
  }
  throw new Error(`timed out after ${timeout} ms${error ? `: ${String(error)}` : ""}`);
};
const json = (path: string): any => JSON.parse(readFileSync(path, "utf8"));
const exec = (argv: string[]) => {
  const result = Bun.spawnSync(argv, { stdout: "pipe", stderr: "pipe" });
  if (!result.success) throw new Error(`${argv[0]} failed: ${result.stderr.toString().trim()}`);
  return result.stdout.toString().trim();
};
const recorderHash = (path: string) => exec(["/usr/bin/shasum","-a","256",path]).split(/\s/,1)[0];
const frozenRecorderHash = recorderHash(recorder);
if (recorderHash(recorderSource) !== frozenRecorderHash) throw new Error("recorder changed during snapshot");
writeFileSync(join(out,"recorder-provenance.json"),JSON.stringify({source:recorderSource,
  frozen:recorder,sha256:frozenRecorderHash},null,2));
const artifactProvenance = (app: string, executable: string) => {
  const framework = join(app,"Contents/Frameworks/ArcCore.framework");
  const files = [executable,join(framework,"Versions/A/ArcCore"),dylib,recorder];
  const helpers=join(framework,"Helpers");
  if(existsSync(helpers))for(const name of readdirSync(helpers).sort()) {
    if(!name.endsWith(".app"))continue;
    const bundle=join(helpers,name);
    const binary=exec(["/usr/libexec/PlistBuddy","-c","Print :CFBundleExecutable",join(bundle,"Contents/Info.plist")]);
    files.push(join(bundle,"Contents/MacOS",binary));
  }
  return files.map(path=>{
    const realpath=realpathSync(path),stat=statSync(realpath);
    const sha256=exec(["/usr/bin/shasum","-a","256",realpath]).split(/\s/,1)[0];
    return {path,realpath,size:stat.size,mtimeMs:stat.mtimeMs,sha256};
  });
};
const processes = (): { pid: number; ppid: number; command: string }[] => exec(["ps", "-axo", "pid=,ppid=,command="])
  .split("\n").flatMap(line => {
    const match = line.match(/^\s*(\d+)\s+(\d+)\s+(.+)$/);
    return match ? [{ pid: Number(match[1]), ppid: Number(match[2]), command: match[3]! }] : [];
  });
const appPIDs = (exe: string) => processes().filter(p => p.command === exe || p.command.startsWith(exe + " ")).map(p => p.pid);
const alive = (pid: number) => Bun.spawnSync(["kill", "-0", String(pid)], { stderr: "ignore" }).success;
const token = `arc-renderer-probe-${crypto.randomUUID()}`;
const fixtureHost = args.includes("--test-host") ? "renderer-probe.test" : "127.0.0.1";
const rich = args.includes("--rich-extraction") ? `<main><h1>Heading</h1><p>Paragraph <strong>bold</strong> <em>emphasis</em>
  <a href="https://example.invalid/path">link</a></p><div style="display:none">hidden</div>
  <img alt="fixture image" width="16" height="16" src="data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='16' height='16'%3E%3Crect width='16' height='16'/%3E%3C/svg%3E">
  <ul><li>first</li><li>second</li></ul><ol><li>ordered</li></ol>
  <form name="test-form" action="/submit"><input name="text-field" value="public-fixture" placeholder="placeholder" required>
  <input type="password" name="password-field" value="fixture-only-password"><input type="checkbox" name="check-field" checked>
  <select name="select-field"><option value="a" selected>Alpha</option><option value="b" disabled>Beta</option></select>
  <textarea name="text-area">area text</textarea><button>Submit</button></form>
  <table><caption>fixture table</caption><tr><th>header</th></tr><tr><td>cell</td></tr></table></main>` : "";
const server = Bun.serve({ hostname: "127.0.0.1", port: 0, fetch(request) {
  const url = new URL(request.url);
  if (!url.pathname.startsWith(`/${token}/`)) return new Response("not found", { status: 404 });
  const frame = url.pathname.includes("/frame");
  if (url.pathname.endsWith("/blocked.js"))
    return new Response("globalThis.__arcResourceLoads=(globalThis.__arcResourceLoads||0)+1", { headers: {"content-type":"text/javascript"} });
  return new Response(`<!doctype html><meta charset="utf-8"><title>Renderer boundary fixture</title>
    <style>#probe{color:rgb(0,0,0)${args.includes("--cascade") ? "!important" : ""};background-color:rgb(255,255,255)}</style><div id="probe">fixture</div><div class="arc-probe-ad">ad fixture</div>
    ${frame ? "" : rich + `<iframe id="child" src="/${token}/frame"></iframe>`}`, { headers: { "content-type": "text/html" } });
} });
const base = `http://${fixtureHost}:${server.port}/${token}`;
const expression = `(() => {
  const inspect = d => { const e=d.getElementById('probe'); if(!e || d.readyState!=='complete') return null;
    const s=d.defaultView.getComputedStyle(e); return {color:s.color, background:s.backgroundColor,
      old:s.getPropertyValue('--arc-probe-old').trim(), current:s.getPropertyValue('--arc-probe-current').trim(),
      second:s.getPropertyValue('--arc-probe-second').trim()}; };
  const main=inspect(document); const frames=[...document.querySelectorAll('iframe')].map(f=>inspect(f.contentDocument));
  return main && frames.length && frames.every(Boolean) ? {main,frames} : null;
})()`;
type Observation = { step: string; styles: unknown };
type Run = { agent: string; pid?: number; observations: Observation[]; acknowledgments: unknown[]; cleanup?: string; error?: string };
const runs: Run[] = [];
let overallError: string | undefined;
try {
  for (const agent of agents) {
    const run: Run = { agent, observations: [], acknowledgments: [] };
    runs.push(run);
    const dir = join(out, agent);
    mkdirSync(dir, { recursive: true });
    const app = resolve(root, `../arc/build/ArcModAgent-${agent}.app`);
    const exe = join(app, "Contents/MacOS/Arc");
    const plist = (key: string) => exec(["/usr/libexec/PlistBuddy", "-c", `Print :${key}`, join(app, "Contents/Info.plist")]);
    const requestPath = join(dir, "renderer-request.json");
    let cdp: AgentCDP | undefined;
    let pid: number | undefined;
    let target: string | undefined;
    let provenanceBefore: ReturnType<typeof artifactProvenance> | undefined;
    try {
      if (plist("ArcModAgentSession") !== agent || plist("CFBundleIdentifier") !== `company.thebrowser.Browser.mod.agent.${agent}`) {
        throw new Error("agent bundle identity mismatch");
      }
      if (appPIDs(exe).length) throw new Error("agent already running; refusing to attach or terminate it");
      // Refuse a shared or occupied debug port before opening an app.
      if (Bun.spawnSync(["/usr/sbin/lsof", "-nP", `-iTCP:${port}`, "-sTCP:LISTEN"], { stdout: "ignore", stderr: "ignore" }).success) {
        throw new Error("CDP port is already occupied");
      }
      provenanceBefore=artifactProvenance(app,exe);
      writeFileSync(join(dir,"artifact-provenance-before.json"),JSON.stringify(provenanceBefore,null,2));
      const data = join(process.env.HOME!, "Library/Application Support", `ArcModAgent-${agent}`);
      const chromiumData = join(data, "Agent Chromium Data");
      mkdirSync(chromiumData, { recursive: true, mode: 0o700 });
      for (const key of ["cloudKitSyncEnabled", "firebaseSyncEnabled"]) {
        exec(["defaults", "write", `company.thebrowser.Browser.mod.agent.${agent}`, key, "-bool", "false"]);
      }
      exec(["open", "-n", "-g", "-a", app,
        "--env", `DYLD_INSERT_LIBRARIES=${dylib}:${recorder}`,
        "--env", `ARC_RENDERER_PROBE_REQUEST=${requestPath}`,
        "--env", `ARC_RECORD_PATH=${join(dir, "record.jsonl")}`,
        ...(args.includes("--adblock-enabled") ? ["--env", "ARC_RENDERER_PROBE_ENABLE_ADBLOCK=1"] : []),
        "--stdout", join(dir, "app.log"), "--stderr", join(dir, "app.log"), "--args",
        "--use-mock-keychain", "--no-default-browser-check", "--disable-sync", "--disable-background-networking",
        `--host-resolver-rules=MAP renderer-probe.test 127.0.0.1, ${plist("ArcModHostResolverRules")}`, `--user-data-dir=${chromiumData}`,
        "--remote-debugging-address=127.0.0.1", `--remote-debugging-port=${port}`]);
      pid = await wait(() => appPIDs(exe)[0], 30000);
      run.pid = pid;
      await wait(() => {
        const injected = json(requestPath + ".ready.json");
        const ready = json(join(data, "ArcModAgentReady.json"));
        return injected.pid === pid && injected.installed === true && ready.pid === pid && ready.ready === true;
      }, 60000);
      await wait(() => verifyAgentEndpoint(agent, port) === pid, 30000);
      cdp = await AgentCDP.connectAgent(agent, port);
      const created = await cdp.request("Target.createTarget", { url: base + "/a" });
      if (typeof created.targetId !== "string") throw new Error("no fixture target");
      target = created.targetId;
      let currentURL = base + "/a";
      const evaluate = async (source: string): Promise<any> => {
        const value = await cdp!.targetCommand(target!, "Runtime.evaluate", { expression: source, returnByValue: true, awaitPromise: true });
        if (value.exceptionDetails) throw new Error(`fixture JS failed: ${JSON.stringify(value.exceptionDetails)}`);
        return (value.result as any)?.value;
      };
      let number = 0;
      const action = async (operation: string, identifier?: string, stylesheet?: string, values?: string[], hostPattern?: string, scheme?: number, matrix?: number[]) => {
        if (verifyAgentEndpoint(agent, port) !== pid) throw new Error("endpoint PID changed");
        const id = `${token}-${++number}`;
        const sentAt = Date.now() / 1000;
        const request = { id, url: currentURL, operation, identifier, stylesheet, values, hostPattern, scheme, matrix };
        writeFileSync(requestPath + ".tmp", JSON.stringify(request));
        renameSync(requestPath + ".tmp", requestPath);
        const response = await wait(() => {
          const response = json(requestPath + ".response.json");
          return response.id === id ? response : undefined;
        });
        run.acknowledgments.push({ ...response, sentAt, receivedAt: Date.now()/1000,
          url: new URL(response.url).pathname.replace(token, "fixture") });
        if (response.pid !== pid || response.ok !== true || response.matches !== 1) throw new Error(`boundary action failed: ${JSON.stringify(response)}`);
        return response;
      };
      const snapshot = async (step: string) => {
        // Three identical samples after the API acknowledgement give Mojo + style/layout a bounded settling period.
        let last = "";
        let same = 0;
        const styles = await wait(async () => {
          const styles = await evaluate(expression);
          if (!styles) { same = 0; return undefined; }
          const serial = JSON.stringify(styles);
          same = serial === last ? same + 1 : 0;
          last = serial;
          return same >= 3 ? styles : undefined;
        });
        run.observations.push({ step, styles });
        if (step === "insert-one") {
          const capture = await cdp!.targetCommand(target!, "Page.captureScreenshot", {format:"png"});
          if (typeof capture.data !== "string") throw new Error("fixture screenshot missing");
          writeFileSync(join(dir,"styled.png"),Buffer.from(capture.data,"base64"));
        }
        console.log(`${agent}: ${step} ${JSON.stringify(styles)}`);
      };
      await action("inspect");
      if (args.includes("--checkpoint-gate") && agent === "trace156") {
        // Parent may attach the bounded exact-PID observer while this run keeps
        // the shared lock. No operation proceeds on an elapsed-time approval.
        writeFileSync(join(dir,"checkpoint-target.json"),JSON.stringify({pid,url:currentURL},null,2));
        console.log(`${agent}: waiting for exact-PID checkpoint gate`);
        const gate = await wait(() => {
          const gate = json(join(dir,"checkpoint-continue.json"));
          return gate.pid === pid && typeof gate.proceed === "boolean" ? gate : undefined;
        }, 120000);
        if (!gate.proceed) throw new Error("parent cancelled checkpoint observation");
      }
      await snapshot("baseline");
      if (args.includes("--extract")) await action("extract");
      if (args.includes("--extract-repeat")) {
        await evaluate("document.getElementById('probe').textContent='mutated fixture'");
        await action("extract");
        await evaluate("document.getElementById('probe').textContent='fixture'");
      }
      const baseline = run.observations[0]!.styles as any;
      if ([baseline.main, ...baseline.frames].some(s => s.color !== "rgb(0, 0, 0)" || s.background !== "rgb(255, 255, 255)" || s.old || s.current || s.second)) {
        throw new Error("fixture computed-style baseline failed");
      }
      const first = "#probe{color:rgb(19,53,91)!important;--arc-probe-old:old}";
      const replacement = "#probe{color:rgb(113,29,47)!important;--arc-probe-current:current}";
      await action("insert", "arc-renderer-probe-one", first); await snapshot("insert-one");
      await action("insert", "arc-renderer-probe-one", replacement); await snapshot("replace-same-identifier");
      await action("insert", "arc-renderer-probe-two", "#probe{background-color:rgb(7,101,23)!important;--arc-probe-second:second}");
      await snapshot("insert-second-identifier");
      await action("remove", "arc-renderer-probe-missing"); await snapshot("remove-missing");
      await action("remove", "arc-renderer-probe-one"); await snapshot("remove-one");
      await action("remove", "arc-renderer-probe-two"); await snapshot("remove-two");
      await action("insert", "arc-renderer-probe-one", first); await snapshot("insert-before-document-navigation");
      currentURL = base + "/b";
      await cdp.targetCommand(target, "Page.navigate", { url: currentURL });
      await wait(async () => (await evaluate("location.href")) === currentURL);
      await action("inspect"); await snapshot("after-document-navigation");
      await action("remove", "arc-renderer-probe-one"); await snapshot("remove-after-document-navigation");
      await action("insert", "arc-renderer-probe-one", replacement); await snapshot("insert-after-document-navigation");
      await evaluate(`(() => { const f=document.createElement('iframe'); f.id='dynamic'; f.src=${JSON.stringify(base + "/frame-dynamic")}; document.body.append(f); })()`);
      await wait(async () => (await evaluate("document.querySelectorAll('iframe').length")) === 2);
      await snapshot("new-subframe-after-insertion");
      await evaluate(`document.getElementById('child').src=${JSON.stringify(base + "/frame-navigated")}`);
      await wait(async () => (await evaluate("document.getElementById('child').contentWindow.location.href")) === base + "/frame-navigated");
      await snapshot("existing-subframe-navigation");
      await action("remove", "arc-renderer-probe-one"); await snapshot("remove-with-subframes");
      if (args.includes("--filters")) {
        // Public fixture: explicit CSS, canvas, SVG image, raster image and text.
        // The pixel output is separate from computed style, which filters do not change.
        const gradients = args.includes("--filter-gradients") ? [
          ["linear-gradient-red","linear-gradient(red,red)"],
          ["radial-gradient-red","radial-gradient(red,red)"],
          ["conic-gradient-red","conic-gradient(red,red)"],
        ] : [];
        await evaluate(`(() => {
          const host=document.createElement('div'); host.id='arc-filter-swatches';
          host.style='position:fixed;left:10px;top:10px;display:flex;gap:8px;z-index:2147483647;background:white;padding:8px';
          const add=(element,name)=>{element.dataset.filterSwatch=name; element.style.width='48px';element.style.height='48px';host.append(element);return element;};
          const rect=add(document.createElement('div'),'css-red');rect.style.background='rgb(255,0,0)';
          const white=add(document.createElement('div'),'css-white');white.style.background='rgb(255,255,255)';
          const canvas=add(document.createElement('canvas'),'canvas-red');canvas.width=48;canvas.height=48;
          const c=canvas.getContext('2d');c.fillStyle='rgb(255,0,0)';c.fillRect(0,0,48,48);
          const raster=add(document.createElement('img'),'raster-red');raster.src=canvas.toDataURL('image/png');
          const svg=add(document.createElement('img'),'svg-red');svg.src='data:image/svg+xml,%3Csvg xmlns="http://www.w3.org/2000/svg" width="48" height="48"%3E%3Crect width="48" height="48" fill="red"/%3E%3C/svg%3E';
          for (const [name,background] of ${JSON.stringify(gradients)}) {
            const gradient=add(document.createElement('div'),name);gradient.style.backgroundImage=background;
          }
          const text=add(document.createElement('span'),'text-red');text.textContent='TEST';text.style.color='red';text.style.font='bold 14px monospace';
          document.body.append(host);
        })()`);
        await wait(async()=>await evaluate("[...document.querySelectorAll('#arc-filter-swatches img')].every(i=>i.complete&&i.naturalWidth===48)"));
        const filters: unknown[] = [];
        const sample = async(step:string)=>{
          await sleep(400);
          const geometry=await evaluate(`({dpr:devicePixelRatio,width:innerWidth,height:innerHeight,swatches:[...document.querySelectorAll('[data-filter-swatch]')].map(e=>{const r=e.getBoundingClientRect();return {name:e.dataset.filterSwatch,x:r.x,y:r.y,width:r.width,height:r.height};})})`);
          const capture=await cdp!.targetCommand(target!,"Page.captureScreenshot",{format:"png"});
          if(typeof capture.data!=="string")throw new Error("filter screenshot missing");
          const filename="filter-"+step+".png";
          writeFileSync(join(dir,filename),Buffer.from(capture.data,"base64"));
          filters.push({step,geometry,screenshot:filename});
        };
        const identity=Array.from({length:20},(_,i)=>[0,6,12,18].includes(i)?1:0);
        await sample("baseline");
        await action("apply-color-filter",undefined,undefined,undefined,undefined,undefined,identity);await sample("identity");
        const grayscale=[...identity];grayscale[6]=0;
        await action("apply-color-filter",undefined,undefined,undefined,undefined,undefined,grayscale);await sample("grayscale");
        await action("remove-color-filter");await sample("removed");
        writeFileSync(join(dir,"filter-observations.json"),JSON.stringify(filters,null,2));
      }
      if (args.includes("--schemes")) {
        const schemes: unknown[] = [];
        const sample = async(step:string)=>{ await sleep(300); schemes.push({step,dark:await evaluate("matchMedia('(prefers-color-scheme:dark)').matches")}); };
        await sample("baseline");
        await action("apply-color-scheme",undefined,undefined,undefined,undefined,0); await sample("scheme0");
        await action("apply-color-scheme",undefined,undefined,undefined,undefined,1); await sample("scheme1");
        await action("remove-color-scheme"); await sample("removed");
        writeFileSync(join(dir,"scheme-observations.json"),JSON.stringify(schemes,null,2));
      }
      if (args.includes("--effects")) {
        const effects: unknown[] = [];
        const sample = async (step: string) => {
          effects.push({step, result: await evaluate(`({color:getComputedStyle(document.getElementById('probe')).color,
            js:window.__arcBoostProbe||0,dom:document.documentElement.dataset.arcBoostProbe||'',
            childJS:document.querySelector('iframe').contentWindow.__arcBoostProbe||0,
            childDOM:document.querySelector('iframe').contentDocument.documentElement.dataset.arcBoostProbe||''})`)});
        };
        await action("register-boost-css", "arc-renderer-probe-boost", "#probe{color:rgb(81,17,141)!important}",undefined,fixtureHost);
        await action("register-boost-js", "arc-renderer-probe-script", "window.__arcBoostProbe=(window.__arcBoostProbe||0)+1;document.documentElement.dataset.arcBoostProbe=String(window.__arcBoostProbe)",undefined,fixtureHost);
        await sleep(700); await sample("registered-existing-document");
        currentURL = base + "/effects";
        await cdp.targetCommand(target,"Page.navigate",{url:currentURL});
        await wait(async()=>await evaluate("location.href=== " + JSON.stringify(currentURL) + " && document.readyState==='complete'"));
        await sleep(700); await action("inspect"); await sample("registered-new-document");
        await action("unregister-boost-css", "arc-renderer-probe-boost");
        await action("unregister-boost-js", "arc-renderer-probe-script");
        await sleep(700); await sample("removed-existing-document");
        currentURL = base + "/effects-removed";
        await cdp.targetCommand(target,"Page.navigate",{url:currentURL});
        await wait(async()=>await evaluate("location.href=== " + JSON.stringify(currentURL) + " && document.readyState==='complete'"));
        await sleep(700); await action("inspect"); await sample("removed-new-document");
        writeFileSync(join(dir,"effects-observations.json"),JSON.stringify(effects,null,2));
      }
      if (args.includes("--adblock")) {
        const resources: unknown[] = [];
        const debug = await action("adblock-debug");
        console.log(`${agent}: adblock debug ${JSON.stringify(debug.info)}`);
        writeFileSync(join(dir,"adblock-debug.json"),JSON.stringify(debug,null,2));
        const load = async (step: string) => {
          const resource = base + "/blocked.js?" + crypto.randomUUID();
          const loaded = await evaluate(`new Promise(resolve=>{ const s=document.createElement('script');
            s.src=${JSON.stringify(resource)};s.onload=()=>resolve('loaded');s.onerror=()=>resolve('blocked');document.body.append(s); })`);
          const count = await action("blocked-count");
          const debug = await action("adblock-debug");
          const adDisplay = await evaluate("getComputedStyle(document.querySelector('.arc-probe-ad')).display");
          resources.push({step,loaded,count:count.count,debug:debug.info,adDisplay});
          console.log(`${agent}: adblock ${step} ${loaded}, count=${count.count}, adDisplay=${adDisplay}, debug=${JSON.stringify(debug.info)}`);
        };
        // Read startup completion evidence without changing task priority or attaching a debugger.
        const startupHistograms: unknown[] = [];
        const snapshotStartup = async (stage: string) => {
          try {
            startupHistograms.push({stage,wallClockMs:Date.now(),
              result:await cdp!.request("Browser.getHistograms",{query:"Startup.",delta:false})});
          } catch (error) {
            startupHistograms.push({stage,wallClockMs:Date.now(),error:String(error)});
          }
          writeFileSync(join(dir,"adblock-startup-histograms.json"),JSON.stringify(startupHistograms,null,2));
        };
        await load("baseline");
        await snapshotStartup("before-idle");
        if (adblockIdleMs) {
          console.log(`${agent}: idle ${adblockIdleMs} ms before block-list publication diagnostic`);
          await sleep(adblockIdleMs);
        }
        await snapshotStartup("before-set-block-lists");
        const networkRule = `||${fixtureHost}^$script`;
        const rules = `${networkRule}\n${fixtureHost}##.arc-probe-ad`;
        await action("set-block-lists", undefined, undefined, [rules]);
        let ready = false;
        const compileStart = Date.now();
        writeFileSync(join(dir,"adblock-publication-request.json"),JSON.stringify({wallClockMs:compileStart,
          readinessDeadlineMs:adblockReadinessMs,idleBeforeRequestMs:adblockIdleMs},null,2));
        let lastStartupSnapshot = compileStart;
        while (Date.now() - compileStart < adblockReadinessMs) {
          await sleep(500);
          const debug = await action("adblock-debug");
          if (Date.now()-lastStartupSnapshot >= 30000) {
            await snapshotStartup(`publication-wait-${Math.floor((Date.now()-compileStart)/1000)}s`);
            lastStartupSnapshot = Date.now();
          }
          if (/\/\* Result: (Success|Cached: [^*]+) \*\//.test(String(debug.info)) &&
              String(debug.info).includes(".arc-probe-ad{display:none!important;}")) {
            ready = true;
            break;
          }
        }
        await snapshotStartup(ready ? "publication-ready" : "publication-timeout");
        if (!ready) throw new Error(`block list readiness timed out after ${Date.now()-compileStart} ms`);
        writeFileSync(join(dir,"adblock-publication.json"),JSON.stringify({elapsedMs:Date.now()-compileStart},null,2));
        await sleep(1500); await load("rule-active");
        currentURL = base + "/c";
        await cdp.targetCommand(target, "Page.navigate", {url:currentURL});
        await wait(async()=>await evaluate("location.href=== " + JSON.stringify(currentURL) + " && document.readyState==='complete'"));
        await action("inspect"); await load("rule-active-new-document");
        if (args.includes("--adblock-toggle")) {
          await action("set-feature-11", undefined, undefined, undefined, undefined, 0);
          await sleep(500); await load("feature-disabled");
          await action("set-exclusions", undefined, undefined, [fixtureHost]);
          await sleep(500); await load("feature-disabled-excluded");
          await action("set-exclusions", undefined, undefined, []);
          await sleep(500); await load("feature-disabled-exclusions-cleared");
          await action("set-feature-11", undefined, undefined, undefined, undefined, 1);
          await sleep(500); await load("feature-reenabled");
          if (args.includes("--adblock-queue-edges")) {
            await action("set-exclusions-then-disable", undefined, undefined, [fixtureHost]);
            await sleep(500); await load("queued-exclusion-disabled");
            await action("set-feature-11", undefined, undefined, undefined, undefined, 1);
            await sleep(500); await load("cached-exclusion-on-reenable");
            await action("set-feature-11", undefined, undefined, undefined, undefined, 0);
            await action("set-exclusions", undefined, undefined, []);
            await sleep(500); await load("disabled-retains-installed-exclusion");
            await action("set-feature-11", undefined, undefined, undefined, undefined, 1);
            await sleep(500); await load("cached-clear-on-reenable");
          }
        }
        await action("set-exclusions", undefined, undefined, [fixtureHost]);
        await sleep(500); await load("exclude-ip-no-port");
        await action("set-exclusions", undefined, undefined, [`${fixtureHost}:${server.port}`]);
        await sleep(500); await load("exclude-ip-with-port");
        await action("set-exclusions", undefined, undefined, []);
        await sleep(500); await load("exclusions-cleared");
        await action("set-block-lists", undefined, undefined, [`${rules}\n${networkRule},badfilter`]);
        await sleep(1500); await load("badfilter");
        await action("set-block-lists", undefined, undefined, []);
        await sleep(1000); await load("lists-cleared");
        writeFileSync(join(dir, "adblock-observations.json"), JSON.stringify(resources,null,2));
      }
      await cdp.request("Target.closeTarget", { targetId: target });
      target = undefined;
    } catch (error) {
      run.error = String(error);
      console.error(`${agent}: ${run.error}`);
    } finally {
      if (pid) {
        try {
          if (verifyAgentEndpoint(agent, port) !== pid) throw new Error("endpoint PID changed before cleanup");
          cdp ??= await AgentCDP.connectAgent(agent, port);
          await cdp.request("Browser.close").catch(() => {});
          await wait(() => !alive(pid!), 15000);
          run.cleanup = "Browser.close; original launched PID exited";
        } catch {
          if (appPIDs(exe).includes(pid)) Bun.spawnSync(["kill", "-TERM", String(pid)]);
          try { await wait(() => !alive(pid!), 15000); run.cleanup = "SIGTERM of original launched PID; exited"; }
          catch { run.cleanup = "original launched PID still alive"; run.error ??= "cleanup failed"; }
        }
      }
      cdp?.close();
      if(provenanceBefore)try {
        const after=artifactProvenance(app,exe);
        writeFileSync(join(dir,"artifact-provenance-after.json"),JSON.stringify(after,null,2));
        if(JSON.stringify(after)!==JSON.stringify(provenanceBefore))run.error??="app/probe artifacts changed during the run";
      }catch(error){run.error??=`artifact provenance failed: ${String(error)}`;}
      writeFileSync(join(dir, "observations.json"), JSON.stringify(run, null, 2));
    }
  }
} catch (error) { overallError = String(error); }
finally { server.stop(true); }
const golden = runs.find(r => r.agent === "trace156");
const ours = runs.find(r => r.agent === "ours156");
const differences = golden && ours ? golden.observations.flatMap((entry, index) => {
  const observed = ours.observations[index];
  return JSON.stringify(entry) === JSON.stringify(observed) ? [] : [{ step: entry.step, golden: entry.styles, ours: observed?.styles ?? null }];
}) : [];
if (golden && ours && golden.observations.length !== ours.observations.length) differences.push({ step: "observation-count", golden: golden.observations.length, ours: ours.observations.length });
const complete = runs.length === agents.length && runs.every(r => !r.error && r.observations.length === 14 && !!r.cleanup);
const parity = golden && ours ? complete && differences.length === 0 : null;
const summary = { complete, parity, differences, runs, ...(overallError ? { error: overallError } : {}) };
writeFileSync(join(out, "summary.json"), JSON.stringify(summary, null, 2));
console.log(JSON.stringify({ complete, parity, differences: differences.length, out }));
process.exitCode = complete && parity !== false && !overallError ? 0 : 1;
