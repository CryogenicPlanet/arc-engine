export {};
// Separate process: synchronous agent CLI calls must not stall fixture responses during reload/redirect.
const scenario = await import(Bun.argv[2]!);
const server = Bun.serve({ hostname: "127.0.0.1", port: 0, fetch(req) {
  const fixture = scenario.fixture?.(req);
  if (fixture) return fixture;
  const path = new URL(req.url).pathname;
  if (path === "/a" || path === "/b") {
    const letter = path.slice(1).toUpperCase();
    return new Response(`<!doctype html><title>launch-slice ${letter}</title><body><div id="ls-marker-7f3a">fixture ${letter}</div>` +
      `<div style="width:200px;height:200px;background:rgb(12,140,200)"></div></body>`, { headers: { "content-type": "text/html" } });
  }
  return new Response("not found", { status: 404 });
} });
console.log(JSON.stringify({ port: server.port }));
// Do not leave a fixture listener if the driving harness is interrupted.
const parent = process.ppid;
setInterval(() => { try { process.kill(parent, 0); } catch { server.stop(true); process.exit(); } }, 1000);
