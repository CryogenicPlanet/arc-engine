// Smoke test on a disposable COPY of a real ArcMod profile (prepared by the lead; never the original).
// Observes only: launch, let Arc restore its spaces and tabs, stay up, quit cleanly, relaunch, quit.
// No navigation, typing or clicks, so the copy's sites only see what restoring the tabs loads.
import type { ScenarioContext } from "../launch-slice/run";

export default async function scenario(c: ScenarioContext) {
  for (const round of [1, 2]) {
    const l = c.launch();
    if (!await c.ready(l, `launch ${round}`)) {
      await c.quit(l, `quit ${round}`);
      return;
    }
    await c.sleep(20000);
    const targets = c.cdp("list") as Array<{ type?: string; url?: string }>;
    const pages = targets.filter(t => t.type === "page");
    c.record(`restored pages ${round}`, "pass", `${pages.length} page targets`);
    c.assert(`browser alive after 20 s ${round}`, l.pid() !== undefined && Array.isArray(targets));
    await c.quit(l, `quit ${round}`);
  }
}
