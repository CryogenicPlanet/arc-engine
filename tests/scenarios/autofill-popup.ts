// Chrome's autofill dropdown (the views popup Arc's engine shows for saved passwords, addresses and
// <datalist> options) is opened on a datalist field. Arc's app has no callback for it, so the steps check
// that the engine survives, that the page keeps working, and record the native windows while it is open.
import type { ScenarioContext } from "../launch-slice/run";
import { run } from "./native";

export const fixture = (request: Request) =>
  new URL(request.url).pathname === "/popup-7f3a"
    ? new Response(
        `<!doctype html><title>autofill popup</title><body>` +
          `<input id="f" list="dl" autocomplete="off"><datalist id="dl">` +
          `<option value="arccore-alpha-7f3a"><option value="arccore-beta-7f3a"><option value="other-7f3a"></datalist>`,
        { headers: { "content-type": "text/html", "cache-control": "no-store" } })
    : undefined;

export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const t = c.open(`${c.base}/popup-7f3a`);
    c.assert("datalist page loaded", await c.titleIs(t, "autofill popup"));
    const before = c.native().elements.filter(n => n.role === "AXWindow").length;
    c.evaluate(t, "document.getElementById('f').focus(); 1");
    c.cdp("type", t, "arccore");
    await c.sleep(1500);
    const after = c.native().elements.filter(n => n.role === "AXWindow").length;
    c.record("native windows with popup open", "pass", `${before} -> ${after}`);
    c.assert("page alive with popup open", c.evaluate(t, "document.getElementById('f').value") === "arccore");
    c.cdp("type", t, "-alpha");
    await c.sleep(500);
    c.assert("page alive after more typing", c.evaluate(t, "document.getElementById('f').value") === "arccore-alpha");
    c.cdp("close", t);
    await c.sleep(500);
  });
}
