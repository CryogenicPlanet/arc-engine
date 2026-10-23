import type { ScenarioContext } from "../launch-slice/run";
import { run, page } from "./native";
import { withTargetFocus } from "../../../arc/tools/agent-cdp";
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const t = await page(c); const client = await c.client();
    const before = new Set(c.native().elements.filter(n => n.role === "AXMenu").map(n => n.path));
    await withTargetFocus(client, t, async () => {
      for (const type of ["mousePressed", "mouseReleased"]) await client.targetCommand(t, "Input.dispatchMouseEvent", {
        type, x: 80, y: 80, button: "right", buttons: type === "mousePressed" ? 2 : 0, clickCount: 1,
      });
    });
    const shown = await c.waitFor(() => {
      const s = c.native();
      const bars = s.elements.filter(n => n.role === "AXMenuBar");
      const menus = s.elements.filter(n => n.role === "AXMenu" && !before.has(n.path) && !bars.some(b => n.path.startsWith(b.path + ".")));
      return s.elements.some(n => n.label === "Reload" && n.enabled && n.actions.includes("AXPress") && menus.some(m => n.path.startsWith(m.path + "."))) ? true : undefined;
    }, 8000);
    c.assert("new native page context menu outside menu bar", shown);
    const state = c.native(); const bars = state.elements.filter(n => n.role === "AXMenuBar");
    const reload = state.elements.filter(n => n.label === "Reload" && n.enabled && n.actions.includes("AXPress") && !bars.some(b => n.path.startsWith(b.path + ".")));
    if (reload.length !== 1) throw new Error(`expected contextual Reload; observed ${reload.length}`);
    c.action(reload[0]!);
    c.assert("contextual reload commits", await c.waitFor(() => c.evaluate(t, "performance.getEntriesByType('navigation')[0]?.type") === "reload" ? true : undefined, 8000));
    c.assert("context menu dismissed", !c.native().elements.some(n => n.label === "Reload" && !bars.some(b => n.path.startsWith(b.path + "."))));
  });
}
