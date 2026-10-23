import type { ScenarioContext } from "../launch-slice/run";
import { run, page, press } from "./native";
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    await page(c); await page(c, "/b", c.EXPECT.bTitle);
    press(c, /^Split View Menu$/);
    press(c, /^(Add Split View|Add Right Split|Add Left Split|Split Right)$/);
    const state = c.native();
    c.assert("split view has two native web areas", state.elements.filter(n => n.role === "AXWebArea").length === 2);
    press(c, /^Close this Split Pane$/);
    c.assert("split removed", c.native().elements.filter(n => n.role === "AXWebArea").length === 1);
  });
}
