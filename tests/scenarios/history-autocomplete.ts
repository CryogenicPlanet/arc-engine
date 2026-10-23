import type { ScenarioContext } from "../launch-slice/run";
import { run, page, observed, set } from "./native";
export { chromiumArgs } from "./command-bar-autocomplete";
export async function autocomplete(c: ScenarioContext) {
  const visited = await page(c); await page(c, "/b", c.EXPECT.bTitle);
  c.cdp("close", visited);
  c.assert("visited fixture tab is closed before history query", !(c.cdp("list") as {id:string}[]).some(t => t.id === visited));
  // Fresh Arc can already have the command bar open. New Tab is a toggle in that state.
  if (!c.native().elements.some(n => n.identifier === "commandBarTextField"))
    c.action(observed(c, n => n.identifier === "newTabMenuItem" && n.actions.includes("AXPress")));
  if (!await c.waitFor(() => c.native().elements.some(n => n.identifier === "commandBarTextField") ? true : undefined, 5000)) throw new Error("command bar field not exposed");
  // URLs suppress remote search suggestions; the title must still come from the recorded local visit.
  set(c, n => n.identifier === "commandBarTextField", `${c.base}/a`);
  c.assert("command bar contains exact fixture URL", c.native().elements.some(n => n.identifier === "commandBarTextField" && n.value === `${c.base}/a`));
  c.assert("local fixture history autocomplete", await c.waitFor(() => {
    const state = c.native(); const root = state.elements.find(n => n.identifier === "commandBarSuggestions");
    return root && state.elements.some(n => n.path.startsWith(root.path + ".") && (n.label + " " + String(n.value)).includes(c.EXPECT.aTitle)) ? true : undefined;
  }, 8000));
}
export default async function scenario(c: ScenarioContext) { await run(c, () => autocomplete(c)); }
