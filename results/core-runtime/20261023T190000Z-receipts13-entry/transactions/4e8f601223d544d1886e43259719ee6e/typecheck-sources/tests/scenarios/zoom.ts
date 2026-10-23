import type { ScenarioContext } from "../launch-slice/run";
import { run, nativePage, press } from "./native";
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const t = await nativePage(c);
    press(c, /^Zoom to Actual Size$/);
    c.assert("normalize to fixed 100% retina scale", c.evaluate(t, "devicePixelRatio") === 2);
    press(c, /^Zoom In$/);
    c.assert("native zoom reaches fixed 110%", await c.waitFor(() => Math.abs(Number(c.evaluate(t, "devicePixelRatio")) - 2.2) < 0.00001 ? true : undefined, 8000));
    press(c, /^Zoom Out$/);
    c.assert("zoom out restores fixed 100%", await c.waitFor(() => c.evaluate(t, "devicePixelRatio") === 2 ? true : undefined, 8000));
    press(c, /^Zoom In$/); press(c, /^Zoom to Actual Size$/);
    c.assert("reset restores fixed 100%", await c.waitFor(() => c.evaluate(t, "devicePixelRatio") === 2 ? true : undefined, 8000));
  });
}
