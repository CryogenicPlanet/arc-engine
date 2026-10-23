// Engine-swap acceptance suite: runs the same local test pages in several ArcMod agent sessions
// (each on a different ArcCore engine) over CDP, and writes a side-by-side comparison.
//   bun suite.ts <out-dir> <slug>:<cdp-port> [<slug>:<cdp-port> ...]
// Sessions must already be running: tools/run-mod.sh --agent <slug> --background --cdp-port <port>
// (from ~/general/arc). Drives them only through ~/general/arc/tools/agent-cdp.ts.
import { existsSync, mkdirSync, readdirSync, rmSync, writeFileSync } from "node:fs";
import { homedir } from "node:os";
import { join } from "node:path";

const ARCMOD = join(homedir(), "general/arc");
const [outDir, ...sessionArgs] = Bun.argv.slice(2);
if (!outDir || sessionArgs.length === 0) throw new Error("usage: bun suite.ts <out-dir> <slug>:<port> ...");
const sessions = sessionArgs.map((arg) => {
  const [slug, port] = arg.split(":");
  if (!slug || !port) throw new Error(`bad session ${arg}`);
  return { slug, port };
});
mkdirSync(outDir, { recursive: true });

// A one-page PDF with correct xref offsets.
const pdf = ((): Buffer => {
  const objects = [
    "<< /Type /Catalog /Pages 2 0 R >>",
    "<< /Type /Pages /Kids [3 0 R] /Count 1 >>",
    "<< /Type /Page /Parent 2 0 R /MediaBox [0 0 300 144] /Contents 4 0 R /Resources << /Font << /F1 5 0 R >> >> >>",
    "<< /Length 44 >>\nstream\nBT /F1 18 Tf 20 70 Td (engine swap pdf) Tj ET\nendstream",
    "<< /Type /Font /Subtype /Type1 /BaseFont /Helvetica >>",
  ];
  let body = "%PDF-1.4\n";
  const offsets = objects.map((object, i) => {
    const offset = body.length;
    body += `${i + 1} 0 obj\n${object}\nendobj\n`;
    return offset;
  });
  const xref = body.length;
  body += `xref\n0 ${objects.length + 1}\n0000000000 65535 f \n${offsets.map((o) => `${String(o).padStart(10, "0")} 00000 n \n`).join("")}`;
  body += `trailer\n<< /Size ${objects.length + 1} /Root 1 0 R >>\nstartxref\n${xref}\n%%EOF\n`;
  return Buffer.from(body, "latin1");
})();

// Each check settles within 8 s; results land in window.__results (a Promise the runner awaits).
const basicPage = `<!doctype html><meta charset=utf-8><title>engine swap suite</title><h1>engine swap suite</h1>
<canvas id=c width=64 height=64></canvas><script>
window.__results = (async () => {
  const r = {};
  const t = async (k, f) => { try { r[k] = await Promise.race([f(), new Promise((_, j) => setTimeout(() => j(new Error("timeout")), 8000))]); } catch (e) { r[k] = "ERR " + (e && e.message || e); } };
  await t("chromium", async () => (await navigator.userAgentData.getHighEntropyValues(["fullVersionList"])).fullVersionList.filter(b => b.brand === "Chromium").map(b => b.version).join(""));
  await t("wasm", async () => { const bytes = new Uint8Array([0,97,115,109,1,0,0,0,1,7,1,96,2,127,127,1,127,3,2,1,0,7,7,1,3,97,100,100,0,0,10,9,1,7,0,32,0,32,1,106,11]); const { instance } = await WebAssembly.instantiate(bytes); return instance.exports.add(2, 3) === 5; });
  await t("worker", () => new Promise(res => { const w = new Worker(URL.createObjectURL(new Blob(["onmessage=e=>postMessage(e.data*2)"]))); w.onmessage = e => res(e.data === 42); w.postMessage(21); }));
  await t("localStorage", async () => { localStorage.setItem("k", "v"); return localStorage.getItem("k") === "v"; });
  await t("indexedDB", () => new Promise((res, rej) => { const q = indexedDB.open("swap-db", 1); q.onupgradeneeded = () => q.result.createObjectStore("s"); q.onerror = () => rej(q.error); q.onsuccess = () => { const tx = q.result.transaction("s", "readwrite"); tx.objectStore("s").put("v", "k"); tx.oncomplete = () => res(true); }; }));
  await t("cookie", async () => { document.cookie = "swap=1; path=/"; return document.cookie.includes("swap=1"); });
  await t("fetch", async () => (await (await fetch("/json")).json()).ok === true);
  await t("serviceWorker", async () => { const reg = await navigator.serviceWorker.register("/sw.js"); return !!reg; });
  await t("crypto", async () => !!(await crypto.subtle.generateKey({ name: "AES-GCM", length: 128 }, true, ["encrypt"])));
  await t("webgl2", async () => { const g = document.createElement("canvas").getContext("webgl2"); if (!g) return "no webgl2"; const e = g.getExtension("WEBGL_debug_renderer_info"); return e ? g.getParameter(e.UNMASKED_RENDERER_WEBGL) : "webgl2 ok"; });
  await t("webgpu", async () => { const a = navigator.gpu && await navigator.gpu.requestAdapter(); return a ? "adapter ok" : "no adapter"; });
  await t("codecs", async () => ["video/mp4; codecs=\\"avc1.42E01E\\"", "audio/mp4; codecs=\\"mp4a.40.2\\"", "video/webm; codecs=\\"vp9\\"", "video/mp4; codecs=\\"av01.0.05M.08\\"", "video/mp4; codecs=\\"hvc1.1.6.L93.B0\\""].map(c => c.split("\\"")[1] + ":" + (MediaSource.isTypeSupported(c) ? "y" : "n")).join(" "));
  await t("mediaRecorderVP9", async () => { const c = document.getElementById("c"); const x = c.getContext("2d"); let i = 0; const timer = setInterval(() => { x.fillStyle = i++ % 2 ? "red" : "blue"; x.fillRect(0, 0, 64, 64); }, 30); const rec = new MediaRecorder(c.captureStream(30), { mimeType: "video/webm;codecs=vp9" }); const chunks = []; rec.ondataavailable = e => chunks.push(e.data); rec.start(); await new Promise(r => setTimeout(r, 600)); rec.stop(); await new Promise(r => rec.onstop = r); clearInterval(timer); const v = document.createElement("video"); v.muted = true; v.src = URL.createObjectURL(new Blob(chunks, { type: "video/webm" })); await v.play(); await new Promise(r => setTimeout(r, 250)); return v.videoWidth + "x" + v.videoHeight + (v.currentTime > 0 ? " playing" : " stalled"); });
  await t("widevine", async () => { const cfg = [{ initDataTypes: ["cenc"], videoCapabilities: [{ contentType: "video/mp4; codecs=\\"avc1.42E01E\\"" }] }]; return (await navigator.requestMediaKeySystemAccess("com.widevine.alpha", cfg)).keySystem; });
  await t("webrtcDataChannel", async () => { const a = new RTCPeerConnection(), b = new RTCPeerConnection(); a.onicecandidate = e => e.candidate && b.addIceCandidate(e.candidate); b.onicecandidate = e => e.candidate && a.addIceCandidate(e.candidate); const ch = a.createDataChannel("x"); const got = new Promise(res => b.ondatachannel = e => e.channel.onmessage = m => res(m.data === "hi")); ch.onopen = () => ch.send("hi"); await a.setLocalDescription(await a.createOffer()); await b.setRemoteDescription(a.localDescription); await b.setLocalDescription(await b.createAnswer()); await a.setRemoteDescription(b.localDescription); return await got; });
  await t("notificationPermission", async () => Notification.permission);
  await t("clipboardApi", async () => typeof navigator.clipboard?.writeText === "function");
  return r;
})();
</script>`;

const downloads = join(homedir(), "Downloads");
const nonce = `${Date.now().toString(36)}`;
const server = Bun.serve({
  hostname: "127.0.0.1",
  port: 0,
  fetch(request) {
    const url = new URL(request.url);
    const html = (body: string) => new Response(body, { headers: { "content-type": "text/html; charset=utf-8" } });
    switch (url.pathname) {
      case "/": return html(basicPage);
      case "/json": return Response.json({ ok: true });
      case "/sw.js": return new Response("self.addEventListener('fetch', () => {});", { headers: { "content-type": "text/javascript" } });
      case "/test.pdf": return new Response(pdf, { headers: { "content-type": "application/pdf" } });
      case "/download": {
        const name = url.searchParams.get("name") ?? "arcmod-swap.txt";
        return new Response(`engine swap download ${name}\n`, { headers: { "content-type": "text/plain", "content-disposition": `attachment; filename="${name}"` } });
      }
      default: return new Response("not found", { status: 404 });
    }
  },
});
const base = `http://127.0.0.1:${server.port}`;

const cdp = (session: { slug: string; port: string }, ...args: string[]): unknown => {
  const result = Bun.spawnSync(["bun", join(ARCMOD, "tools/agent-cdp.ts"), "--agent", session.slug, "--port", session.port, ...args], { stdout: "pipe", stderr: "pipe" });
  if (!result.success) throw new Error(`agent-cdp ${args[0]} failed: ${result.stderr.toString().trim().split("\n").at(-1)}`);
  return JSON.parse(result.stdout.toString());
};
const field = (value: unknown, ...path: string[]): unknown =>
  path.reduce<unknown>((v, key) => (v !== null && typeof v === "object" ? (v as Record<string, unknown>)[key] : undefined), value);
const sleep = (ms: number) => new Promise((resolve) => setTimeout(resolve, ms));

type Row = Record<string, string>;
const runSession = async (session: { slug: string; port: string }): Promise<Row> => {
  const row: Row = {};
  const open = (path: string): string => {
    const id = field(cdp(session, "open", `${base}${path}`), "targetId");
    if (typeof id !== "string") throw new Error(`open ${path}: no target id`);
    return id;
  };
  try {
    const page = open("/");
    await sleep(1500);
    const results = field(cdp(session, "evaluate", page, "window.__results"), "result", "value");
    if (results !== null && typeof results === "object") for (const [k, v] of Object.entries(results)) row[k] = String(v);
    cdp(session, "screenshot", page, join(outDir, `${session.slug}-basic.png`));
    cdp(session, "close", page);
  } catch (e) { row["basic"] = `ERR ${(e as Error).message}`; }

  try {
    const page = open("/test.pdf");
    await sleep(3000);
    const viewer = field(cdp(session, "evaluate", page, "document.contentType + ' / embed=' + (document.querySelector('embed')?.getAttribute('type') ?? 'none')"), "result", "value");
    row["pdfViewer"] = String(viewer);
    cdp(session, "screenshot", page, join(outDir, `${session.slug}-pdf.png`));
    cdp(session, "close", page);
  } catch (e) { row["pdfViewer"] = `ERR ${(e as Error).message}`; }

  const name = `arcmod-swap-${session.slug}-${nonce}.txt`;
  try {
    const page = open(`/download?name=${name}`);
    let found = false;
    for (let i = 0; i < 20 && !found; i++) { await sleep(500); found = readdirSync(downloads).some((f) => f === name); }
    row["download"] = found ? "saved to ~/Downloads" : "not saved within 10 s";
    try { cdp(session, "close", page); } catch { /* download targets often close themselves */ }
  } catch (e) { row["download"] = `ERR ${(e as Error).message}`; }
  finally { const path = join(downloads, name); if (existsSync(path)) rmSync(path); }
  return row;
};

const rows: Record<string, Row> = {};
for (const session of sessions) rows[session.slug] = await runSession(session);
server.stop(true);

const keys = [...new Set(Object.values(rows).flatMap((row) => Object.keys(row)))];
const slugs = sessions.map((s) => s.slug);
const table = [
  `| check | ${slugs.join(" | ")} | same? |`,
  `| --- | ${slugs.map(() => "---").join(" | ")} | --- |`,
  ...keys.map((k) => {
    const values = slugs.map((s) => rows[s]?.[k] ?? "—");
    const same = new Set(values).size === 1 ? "yes" : "**differs**";
    return `| ${k} | ${values.map((v) => v.replaceAll("|", "\\|")).join(" | ")} | ${same} |`;
  }),
].join("\n");
const report = `# Engine-swap acceptance run ${new Date().toISOString()}\n\n${table}\n`;
writeFileSync(join(outDir, "report.md"), report);
writeFileSync(join(outDir, "results.json"), JSON.stringify(rows, null, 2));
console.log(report);
