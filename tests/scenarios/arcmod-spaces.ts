import type { ScenarioContext } from "../launch-slice/run";
import { run, nativePage, press } from "./native";

export const morningPlan = [
  "Create a Space named ArcCore Fixture Space via enabled observed New Space… and its editor.",
  "Assert sidebarSpaceName equals ArcCore Fixture Space, then open fixture B there.",
  "Use observed Previous Space; assert the original Space and original fixture A selection return.",
  "Use observed Next Space; assert ArcCore Fixture Space and fixture B return.",
  "Relaunch the disposable agent and assert both Space names and per-Space tabs persist.",
];
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    await nativePage(c);
    press(c, /^Spaces$/);
    const items = c.native().elements.filter(n => n.role === "AXMenuItem" && n.label === "New Space…");
    c.assert("New Space command exposed exactly once", items.length === 1);
    c.record("create and switch Spaces", "inconclusive", items.length === 1 && !items[0]!.enabled
      ? "BLOCKED: New Space… is disabled in this exact agent snapshot; ArcModLocalMenuAction does not force-enable Space creation. The original native validation reason is unknown."
      : "NEEDS MORNING CAPTURE: Space editor controls must be observed before implementing the mutation; see morningPlan.");
  });
}
