import type { ScenarioContext } from "../launch-slice/run";
import { run, page, press } from "./native";
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    await page(c);
    press(c, /^Import from Another Browser…$/);
    c.assert("import wizard offers supported sources", await c.waitFor(() => {
      const state = c.native(); const sheet = state.elements.find(n => n.role === "AXSheet");
      const children = sheet ? state.elements.filter(n => n.path.startsWith(sheet.path + ".")) : [];
      return ["CHROME", "SAFARI", "Next", "Maybe Later"].every(label => children.some(n => n.role === "AXButton" && n.label === label)) ? true : undefined;
    }, 8000));
    // The shipped wizard offers existing browser profiles, not an isolated HTML-file source.
    // Cancel rather than importing any of the user's browsing data into the fixture.
    press(c, /^Maybe Later$/);
    c.assert("import wizard cancelled", await c.waitFor(() => !c.native().elements.some(n => n.role === "AXSheet") ? true : undefined, 8000));
  });
}
