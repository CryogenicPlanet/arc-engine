import type { ScenarioContext } from "../launch-slice/run";
import { existsSync, readFileSync, unlinkSync } from "node:fs";
import { join } from "node:path";
import { run, page } from "./native";
const name = "arccore-scenario-download-7f3a.txt", bytes = "ArcCore deterministic download 7f3a\n";
export function fixture(req: Request) {
  switch (new URL(req.url).pathname) {
    case "/download-page": return new Response(`<!doctype html><title>download fixture</title><a href="/attachment">Download fixture</a>`, { headers: { "content-type": "text/html" } });
    case "/attachment": return new Response(bytes, { headers: { "content-type": "text/plain", "content-disposition": `attachment; filename="${name}"` } });
  }
}
export default async function scenario(c: ScenarioContext) {
  const destination = join(process.env.HOME!, "Downloads", name);
  if (existsSync(destination)) throw new Error(`fixture destination already exists: ${destination}`);
  try { await run(c, async () => {
    const t = await page(c, "/download-page", "download fixture");
    c.cdp("click", t, "60", "15");
    c.assert("download completes with exact filename and bytes", await c.waitFor(() => existsSync(destination)
      && readFileSync(destination, "utf8") === bytes ? true : undefined, 15000));
    c.native(); // Persist the actual prompt/absence of prompt for the lead.
  }); } finally { if (existsSync(destination) && readFileSync(destination, "utf8") === bytes) unlinkSync(destination); }
}
