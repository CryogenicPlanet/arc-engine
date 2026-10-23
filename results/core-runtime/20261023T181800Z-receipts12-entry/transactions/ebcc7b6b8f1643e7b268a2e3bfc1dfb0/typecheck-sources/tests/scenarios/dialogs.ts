import type { ScenarioContext } from "../launch-slice/run";
import { run, page, press, set } from "./native";
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const t = await page(c);
    c.evaluate(t, "window.dialogResult='pending'; setTimeout(()=>{ alert('ArcCore alert 7f3a'); window.dialogResult='alert-returned' }, 50); 1");
    await c.sleep(400); press(c, /^OK$/);
    c.assert("alert native completion", c.evaluate(t, "window.dialogResult") === "alert-returned");
    c.evaluate(t, "setTimeout(()=>{window.dialogResult=confirm('ArcCore confirm 7f3a')},50); 1");
    await c.sleep(400); press(c, /^Cancel$/);
    c.assert("confirm cancel", c.evaluate(t, "window.dialogResult") === false);
    c.evaluate(t, "setTimeout(()=>{window.dialogResult=prompt('ArcCore prompt 7f3a','initial')},50); 1");
    await c.sleep(400); set(c, n => n.role === "AXTextField" && n.value === "initial", "fixed prompt reply"); press(c, /^OK$/);
    c.assert("prompt native text and completion", c.evaluate(t, "window.dialogResult") === "fixed prompt reply");
  });
}
