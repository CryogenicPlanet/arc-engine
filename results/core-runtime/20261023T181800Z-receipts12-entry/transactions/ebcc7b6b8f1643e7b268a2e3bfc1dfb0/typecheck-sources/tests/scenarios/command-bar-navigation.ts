import type { ScenarioContext } from "../launch-slice/run";
import { run, page, observed, set, ensureNativeWindow } from "./native";
export { chromiumArgs } from "./command-bar-autocomplete";
export async function ensureCommandBar(c: ScenarioContext) {
  if (c.native().elements.some(n => n.identifier === "commandBarTextField")) return;
  const initial = c.native().elements.filter(n => n.identifier === "newTabMenuItem" && n.enabled && n.actions.includes("AXPress"));
  if (initial.length === 1) c.action(initial[0]!);
  if (await c.waitFor(() => c.native().elements.some(n => n.identifier === "commandBarTextField") ? true : undefined, 1000)) return;
  await ensureNativeWindow(c);
  // Menu validation needs a key agent window on some cold background launches.
  const state = c.native(), bars = state.elements.filter(n => n.role === "AXMenuBar");
  const active = state.elements.filter(n => n.role === "AXMenu" && n.actions.includes("AXCancel") && !bars.some(b => n.path.startsWith(b.path + ".")));
  if (active.length === 1) c.perform(active[0]!, "AXCancel");
  const windows = c.native().elements.filter(n => n.role === "AXWindow" && n.actions.includes("AXRaise"));
  if (windows.length === 1) c.perform(windows[0]!, "AXRaise");
  if (!c.native().elements.some(n => n.identifier === "commandBarTextField"))
    c.action(observed(c, n => n.identifier === "newTabMenuItem" && n.actions.includes("AXPress")));
  if (!await c.waitFor(() => c.native().elements.some(n => n.identifier === "commandBarTextField") ? true : undefined, 5000)) throw new Error("command bar did not open");
}
export async function openFromCommandBar(c: ScenarioContext, destination: string, title: string) {
  await ensureCommandBar(c);
  set(c, n => n.identifier === "commandBarTextField", destination);
  c.perform(observed(c, n => n.identifier === "commandBarTextField" && n.actions.includes("AXConfirm")), "AXConfirm");
  const result = await c.waitFor(() => {
    const matches = (c.cdp("list") as {id:string;url:string;title:string}[]).filter(t => t.url === destination && t.title === title);
    return matches.length === 1 ? matches[0] : undefined;
  }, 10000);
  if (!result) throw new Error("native command-bar destination did not load uniquely");
  return result.id;
}
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    await page(c);
    await ensureCommandBar(c);
    const node = observed(c, n => n.identifier === "commandBarTextField");
    let rejected = false;
    try { c.perform(node, "ArccoreUnadvertisedAction"); } catch (error) { rejected = String(error).includes("unsupported_action"); }
    c.assert("unadvertised native action rejected", rejected);
    const t = await openFromCommandBar(c, `${c.base}/b`, c.EXPECT.bTitle);
    c.assert("native confirmed navigation", c.evaluate(t, "location.href") === `${c.base}/b` && await c.titleIs(t, c.EXPECT.bTitle));
  });
}
