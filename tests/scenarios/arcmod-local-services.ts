import type { ScenarioContext } from "../launch-slice/run";
import { spawnSync } from "bun";
import { run, page } from "./native";

// Chromium's ArcModHostResolverRules path only. Native NSURLSession/DNS filtering
// needs an additional exact-process fixture; see MORNING.md.
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const tab = await page(c);
    for (const key of ["shouldShareUsageData", "arcMaxAutoOptInEnabled", "tidyTabsEnabled", "instantLinksEnabled", "askOnPageEnabled", "renameDownloadsEnabled"]) {
      const result = spawnSync(["defaults", "read", `company.thebrowser.Browser.mod.agent.${c.slug}`, key]);
      c.assert(`local-mode ${key} disabled`, result.success && result.stdout.toString().trim() === "0");
    }
    const client = await c.client();
    const failures: { errorText?: unknown; type?: unknown }[] = [];
    const responses: unknown[] = [];
    await client.targetCommand(tab, "Network.enable");
    const dispose = client.onEvent(event => {
      if (event.method === "Network.loadingFailed") failures.push(event.params);
      if (event.method === "Network.responseReceived" && (event.params.response as {url?:string})?.url === "https://api.arc.net/arccore-disposable-probe") responses.push(event.params);
    });
    try {
      // A fixed Arc-owned host listed in resources/local-service-policy.json.
      const result = await client.targetCommand(tab, "Page.navigate", {url:"https://api.arc.net/arccore-disposable-probe"});
      c.assert("Chromium service hostname fails resolution", result.errorText === "net::ERR_NAME_NOT_RESOLVED");
      c.assert("service failure event is DNS rejection", await c.waitFor(() => failures.some(e => e.type === "Document" && e.errorText === "net::ERR_NAME_NOT_RESOLVED") ? true : undefined, 5000));
      c.assert("blocked service receives no HTTP response", responses.length === 0);
      await client.targetCommand(tab, "Page.navigate", {url:`${c.base}/b`});
      c.assert("ordinary loopback browsing survives service rejection", await c.titleIs(tab, c.EXPECT.bTitle));
    } finally { dispose(); }
  });
}
