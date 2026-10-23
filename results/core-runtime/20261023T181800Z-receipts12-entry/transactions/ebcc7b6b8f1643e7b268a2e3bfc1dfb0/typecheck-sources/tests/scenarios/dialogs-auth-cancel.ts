import type { ScenarioContext } from "../launch-slice/run";
import { run, pressWhenReady } from "./native";
export { fixture } from "./dialogs-auth";
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const t = c.open(`${c.base}/auth`);
    c.assert("HTTP auth prompt", await c.waitFor(() => c.native().elements.some(n => n.subrole === "AXSecureTextField") ? true : undefined, 8000));
    await pressWhenReady(c, /^Cancel$/);
    c.assert("cancel displays fixed 401 response", await c.waitFor(() => c.evaluate(t, "document.body.innerText") === "fixture auth required" ? true : undefined, 8000));
  });
}
