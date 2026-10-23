import type { ScenarioContext } from "../launch-slice/run";
import { run, page, press, set, observed } from "./native";
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    await page(c);
    press(c, /^Find(?: in Page)?…$/);
    set(c, n => n.identifier === "findInPageTextField", "fixture");
    c.assert("one native find match", await c.waitFor(() => {
      const result = c.native().elements.find(n => n.identifier === "findInPageMatchesTextBlock");
      return result && /^(1 of 1|1\/1|1 match)$/.test(String(result.value || result.label)) ? true : undefined;
    }, 8000));
    c.action(observed(c, n => n.identifier === "findInPageCloseButton" && n.actions.includes("AXPress")));
    c.assert("find closed", !c.native().elements.some(n => n.identifier === "findInPageTextField"));
  });
}
