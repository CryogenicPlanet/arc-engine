// Extension check for the engine-swap suite: loads ./test-extension through CDP (needs the session
// launched with --enable-unsafe-extension-debugging), then opens a local page and checks that the
// content script ran.  bun ext-test.ts <slug>:<port> [...]
import { homedir } from "node:os";
import { join } from "node:path";

const extensionPath = join(import.meta.dir, "test-extension");
const server = Bun.serve({ hostname: "127.0.0.1", port: 0, fetch: () => new Response("<title>ext</title><p>ext test", { headers: { "content-type": "text/html" } }) });

const browserCall = async (port: string, method: string, params: object): Promise<unknown> => {
  const version = (await (await fetch(`http://127.0.0.1:${port}/json/version`)).json()) as { webSocketDebuggerUrl: string };
  const ws = new WebSocket(version.webSocketDebuggerUrl);
  await new Promise((resolve, reject) => { ws.onopen = resolve; ws.onerror = reject; });
  const reply = await new Promise<unknown>((resolve) => {
    ws.onmessage = (event) => { const msg = JSON.parse(String(event.data)); if (msg.id === 1) resolve(msg); };
    ws.send(JSON.stringify({ id: 1, method, params }));
  });
  ws.close();
  return reply;
};
const cdp = (slug: string, port: string, ...args: string[]): unknown => {
  const r = Bun.spawnSync(["bun", join(homedir(), "general/arc/tools/agent-cdp.ts"), "--agent", slug, "--port", port, ...args], { stdout: "pipe", stderr: "pipe" });
  if (!r.success) throw new Error(r.stderr.toString().trim().split("\n").at(-1));
  return JSON.parse(r.stdout.toString());
};

for (const arg of Bun.argv.slice(2)) {
  const [slug, port] = arg.split(":") as [string, string];
  const loaded = (await browserCall(port, "Extensions.loadUnpacked", { path: extensionPath })) as { result?: { id?: string }; error?: { message?: string } };
  const id = loaded.result?.id;
  let content = "n/a";
  if (id) {
    const page = (cdp(slug, port, "open", `http://127.0.0.1:${server.port}/`) as { targetId: string }).targetId;
    await new Promise((r) => setTimeout(r, 1500));
    content = String((cdp(slug, port, "evaluate", page, "document.documentElement.dataset.arcmodExt ?? 'not run'") as { result: { value: unknown } }).result.value);
    cdp(slug, port, "close", page);
  }
  console.log(`${slug}: loadUnpacked ${id ? `ok (${id})` : `FAILED: ${loaded.error?.message}`}, content script: ${content}`);
}
server.stop(true);
