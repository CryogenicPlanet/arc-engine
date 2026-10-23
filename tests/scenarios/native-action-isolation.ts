import type { ScenarioContext } from "../launch-slice/run";
import { page, observed } from "./native";
export default async function scenario(c: ScenarioContext) {
  const first = c.launch();
  if (!await c.ready(first, "first native agent launch")) { await c.quit(first, "first quit"); return; }
  await page(c);
  const old = observed(c, n => n.identifier === "newWindow" && n.actions.includes("AXPress"));
  await c.quit(first, "first quit");
  const second = c.launch();
  if (await c.ready(second, "second native agent launch")) {
    let rejected = false;
    try { c.perform(old, "AXPress"); } catch (error) { rejected = String(error).includes("earlier agent process"); }
    c.assert("old PID native handle rejected after restart", rejected);
  }
  await c.quit(second, "second quit");
}
