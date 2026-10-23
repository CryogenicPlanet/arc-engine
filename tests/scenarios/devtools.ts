import type { ScenarioContext } from "../launch-slice/run";
import { run, page, press } from "./native";
const visible = (c: ScenarioContext) => c.native().elements.some(n => n.role === "AXWebArea" && n.label === "DevTools");
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    await page(c);
    press(c, /^Toggle Console$/);
    c.assert("native developer frontend opens", await c.waitFor(() => visible(c) ? true : undefined, 8000));
    press(c, /^Toggle Console$/);
    c.assert("native developer frontend closes", await c.waitFor(() => !visible(c) ? true : undefined, 8000));
  });
  await run(c, async () => {
    const t = await page(c);
    press(c, /^Inspect Element$/);
    c.cdp("click", t, "80", "80");
    c.assert("inspect opens native developer frontend", await c.waitFor(() => visible(c) ? true : undefined, 8000));
  });
}
