// CDP-only subset. The extensions candidate still owns tabGroups and the native toolbar popup.
import type { ScenarioContext } from "../launch-slice/run";
import { run, page } from "./native";
import { join } from "node:path";
export const chromiumArgs = ["--enable-unsafe-extension-debugging"];
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const client = await c.client();
    const loaded = await client.request("Extensions.loadUnpacked", {path:join(import.meta.dir,"extension-fixture")});
    c.assert("load local MV3 extension", typeof loaded.id === "string");
    const t = await page(c);
    c.assert("content script fixed marker", await c.waitFor(() => c.evaluate(t,"document.documentElement.dataset.arcCoreScenario") === "extension-7f3a" ? true : undefined,8000));
    c.assert("chrome.tabs query sees sender fixture tab", await c.waitFor(() => c.evaluate(t,"document.documentElement.dataset.arcCoreTabsProbe") === JSON.stringify({tabVisible:true}) ? true : undefined,8000));
    c.cdp("close",t);
    c.assert("fixture tab closed", await c.waitFor(() => !(c.cdp("list") as {id:string}[]).some(row=>row.id===t) ? true : undefined,5000));
  });
}
