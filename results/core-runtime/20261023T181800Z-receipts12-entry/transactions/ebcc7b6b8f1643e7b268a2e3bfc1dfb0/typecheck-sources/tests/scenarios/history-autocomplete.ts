import { existsSync, readFileSync } from "node:fs";
import { join } from "node:path";
import type { ScenarioContext } from "../launch-slice/run";
import { run, page, observed, set } from "./native";
import { openFromCommandBar, ensureCommandBar } from "./command-bar-navigation";
export { chromiumArgs } from "./command-bar-autocomplete";
export async function autocomplete(c: ScenarioContext) {
  await page(c, "/b", c.EXPECT.bTitle);
  const visited = await openFromCommandBar(c, `${c.base}/a`, c.EXPECT.aTitle);
  c.cdp("close", visited);
  c.assert("visited fixture tab is closed before history query", !(c.cdp("list") as {id:string}[]).some(t => t.id === visited));
  await ensureCommandBar(c);
  // URLs suppress remote search suggestions; the title must still come from the recorded local visit.
  set(c, n => n.identifier === "commandBarTextField", `${c.base}/a`);
  c.assert("command bar contains exact fixture URL", c.native().elements.some(n => n.identifier === "commandBarTextField" && n.value === `${c.base}/a`));
  c.assert("local fixture URL visible in suggestions", await c.waitFor(() => {
    const state = c.native(); const root = state.elements.find(n => n.identifier === "commandBarSuggestions");
    return root && state.elements.some(n => n.path.startsWith(root.path + ".") && n.value === `${c.base}/a`) ? true : undefined;
  }, 8000));
  // Arc displays only the URL for this match. The recorder proves this result came from HistoryURL,
  // instead of treating a synthetic URL or an open-tab suggestion as a history hit.
  const transcript = join(c.outDir, "launch-1/record.jsonl");
  if (!existsSync(transcript)) throw new Error("history provenance requires --record");
  c.assert("HistoryURL result is the deletable closed fixture visit", await c.waitFor(() => {
    const rows = readFileSync(transcript, "utf8").split("\n").filter(line => line.endsWith("}")).map(line => JSON.parse(line)) as Record<string, any>[];
    const returns = rows.filter(r => r.ev === "ret");
    const refs = new Set<string>();
    for (const row of returns) if (row.cls === "ArcAutocompleteResult" && row.sel === "perProviderMatches")
      for (const ref of row.ret?.HistoryURL ?? []) refs.add(ref);
    return [...refs].some(ref => returns.some(r => r.self === ref && r.sel === "destinationUrlSpec" && r.ret === `${c.base}/a`)
      && returns.some(r => r.self === ref && r.sel === "deletable" && r.ret === true)) ? true : undefined;
  }, 8000));
}
export default async function scenario(c: ScenarioContext) { await run(c, () => autocomplete(c)); }
