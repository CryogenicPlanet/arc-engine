import type { ScenarioContext } from "../launch-slice/run";
import { run, page, press, windows } from "./native";
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    await page(c);
    const before = windows(c).length;
    c.assert("initial browser window", before === 1, `count=${before}`);
    press(c, /^New Window$/);
    c.assert("second native window", await c.waitFor(() => windows(c).length === 2 ? true : undefined, 10000));
    // Agent sessions intentionally redirect New Little Arc to an ordinary independent window.
    press(c, /^New Little Arc(?: Window)?$/);
    c.assert("agent Little Arc redirect", await c.waitFor(() => windows(c).length === 3 ? true : undefined, 10000));
  });
}
