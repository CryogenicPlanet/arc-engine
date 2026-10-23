import type { ScenarioContext } from "../launch-slice/run";
import { run, page } from "./native";
import { join } from "node:path";
export const chromiumArgs = ["--enable-unsafe-extension-debugging"];
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const client = await c.client();
    const loaded = await client.request("Extensions.loadUnpacked", { path: join(import.meta.dir, "extension-fixture") });
    c.assert("load local MV3 extension", typeof loaded.id === "string");
    const t = await page(c);
    c.assert("content script", await c.waitFor(() => c.evaluate(t, "document.documentElement.dataset.arcCoreScenario") === "extension-7f3a" ? true : undefined, 8000));
    c.assert("chrome.tabs query sees fixture tab", await c.waitFor(() => c.evaluate(t, "document.documentElement.dataset.arcCoreTabsProbe") === JSON.stringify({tabVisible:true}) ? true : undefined, 8000), String(c.evaluate(t, "document.documentElement.dataset.arcCoreTabsProbe")));
    c.assert("chrome.tabGroups query starts empty", await c.waitFor(() => c.evaluate(t, "document.documentElement.dataset.arcCoreGroupsProbe") === JSON.stringify({groupCount:0}) ? true : undefined, 8000), String(c.evaluate(t, "document.documentElement.dataset.arcCoreGroupsProbe")));
    // Open Arc's observed extension menu, then the fixture's own action.
    const menu = c.native().elements.filter(n => n.label === "Extensions Menu" && n.enabled && n.actions.includes("AXPress"));
    if (menu.length !== 1) throw new Error("native extension menu unavailable");
    c.action(menu[0]!);
    const state = c.native(); const action = state.elements.filter(n => n.enabled && /ArcCore fixture action/.test(n.label) && n.actions.includes("AXPress"));
    if (action.length !== 1) throw new Error(`extension toolbar action unavailable: ${action.length} observed matches`);
    c.action(action[0]!);
    c.assert("extension native popup", c.native().elements.some(n => /ArcCore fixture popup/.test(n.label)));
  });
}
