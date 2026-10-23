import type { ScenarioContext } from "../launch-slice/run";
import { run, page, observed, set } from "./native";
// Keep suggestion results local and deterministic in both engines.
export const chromiumArgs = ["--host-resolver-rules=MAP * ~NOTFOUND, EXCLUDE localhost, EXCLUDE 127.0.0.1"];
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    await page(c);
    if (!c.native().elements.some(n => n.identifier === "commandBarTextField"))
      c.action(observed(c, n => n.identifier === "newTabMenuItem" && n.actions.includes("AXPress")));
    if (!await c.waitFor(() => c.native().elements.some(n => n.identifier === "commandBarTextField") ? true : undefined, 5000)) throw new Error("command bar field not exposed");
    set(c, n => n.identifier === "commandBarTextField", `${c.base}/a`);
    c.assert("exact native command bar input", c.native().elements.some(n => n.identifier === "commandBarTextField" && n.value === `${c.base}/a`));
    c.assert("local URL autocomplete result", await c.waitFor(() => {
      const state = c.native(); const root = state.elements.find(n => n.identifier === "commandBarSuggestions");
      return root && state.elements.some(n => n.path.startsWith(root.path + ".") && n.role === "AXStaticText" && n.value === `${c.base}/a`) ? true : undefined;
    }, 8000));
  });
}
