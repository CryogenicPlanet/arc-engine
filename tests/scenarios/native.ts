import type { ScenarioContext, NativeNode } from "../launch-slice/run";
// Arc's sidebar tab buttons advertise only a custom "Name:press" action, which AX cannot perform (-25200, on
// Arc's own engine too). c.perform() then selects the tab through DevTools Target.activateTarget, so its
// receipt names the activated CDP target instead of the AX helper's pid/bundle/action/dispatched fields.
export type TabFallbackReceipt = { fallback: "Target.activateTarget"; targetId: string };
export const isTabFallback = (actionName: string | undefined, receipt: unknown): receipt is TabFallbackReceipt => {
  if (!actionName?.startsWith("Name:press") || !receipt || typeof receipt !== "object") return false;
  const r = receipt as Record<string, unknown>;
  return r.fallback === "Target.activateTarget" && typeof r.targetId === "string" && r.targetId.length > 0;
};
export function observed(c: ScenarioContext, match: (node: NativeNode) => boolean): NativeNode {
  const nodes = c.native().elements.filter(n => n.enabled && match(n));
  if (nodes.length !== 1) throw new Error(`expected one native control; observed ${nodes.length}`);
  return nodes[0]!;
}
export function press(c: ScenarioContext, label: RegExp) {
  const match = (n: NativeNode) => label.test(n.label) && n.actions.includes("AXPress");
  const state = c.native(), matches = state.elements.filter(match);
  if (matches.length === 1 && !matches[0]!.enabled) {
    const item = matches[0]!;
    const menu = state.elements.find(n => n.role === "AXMenuBarItem" && item.path.startsWith(n.path + "."));
    if (menu?.enabled && menu.actions.includes("AXPress")) c.action(menu);
    const refreshed = c.native();
    if (!refreshed.elements.some(n => n.enabled && match(n))) {
      // Use only the exact agent window's advertised native action when a command needs a key window.
      const bars = refreshed.elements.filter(n => n.role === "AXMenuBar");
      const active = refreshed.elements.filter(n => n.role === "AXMenu" && n.actions.includes("AXCancel") && !bars.some(b => n.path.startsWith(b.path + ".")));
      if (active.length === 1) c.perform(active[0]!, "AXCancel");
      const raised = c.native();
      const windows = raised.elements.filter(n => n.role === "AXWindow" && n.enabled && n.actions.includes("AXRaise"));
      if (windows.length === 1) c.perform(windows[0]!, "AXRaise");
    }
  }
  const node = observed(c, match);
  return c.action(node);
}
export function set(c: ScenarioContext, match: (node: NativeNode) => boolean, value: string) {
  return c.action(observed(c, n => n.valueSettable && match(n)), "set", value);
}
export const windows = (c: ScenarioContext) => c.native().elements.filter(n => n.role === "AXWindow");
export async function page(c: ScenarioContext, path = "/a", title = c.EXPECT.aTitle) {
  const t = c.open(`${c.base}${path}`);
  if (!await c.titleIs(t, title)) throw new Error(`fixture ${path} did not load`);
  return t;
}
export async function run(c: ScenarioContext, body: () => Promise<void>) {
  const l = c.launch();
  if (await c.ready(l, "launch")) await c.step("scenario actions", body);
  await c.quit(l, "quit");
}
export async function pressWhenReady(c: ScenarioContext, label: RegExp, ms = 8000) {
  if (!await c.waitFor(() => c.native().elements.some(n => n.enabled && label.test(n.label) && n.actions.includes("AXPress")) ? true : undefined, ms))
    throw new Error(`native control did not appear: ${label}`);
  return press(c, label);
}
export async function ensureNativeWindow(c: ScenarioContext) {
  if (c.native().elements.some(n => n.role === "AXWindow")) return;
  const state = c.native(), bars = state.elements.filter(n => n.role === "AXMenuBar");
  const menus = state.elements.filter(n => n.role === "AXMenu" && n.actions.includes("AXCancel") && !bars.some(b => n.path.startsWith(b.path + ".")));
  if (menus.length === 1) c.perform(menus[0]!, "AXCancel");
  if (c.native().elements.some(n => n.role === "AXWindow")) return;
  c.action(observed(c, n => n.identifier === "newWindow" && n.actions.includes("AXPress")));
  if (!await c.waitFor(() => c.native().elements.some(n => n.role === "AXWindow") ? true : undefined, 5000)) throw new Error("agent window not exposed after native New Window");
}
export async function nativePage(c: ScenarioContext, path = "/a", title = c.EXPECT.aTitle) {
  const target = await page(c, path, title);
  await ensureNativeWindow(c);
  const state = c.native();
  const controls = state.elements.filter(n => n.role === "AXButton" && n.label === title && n.enabled);
  if (controls.length !== 1) throw new Error(`fixture tab is ambiguous in native UI: ${controls.length} matches`);
  const control = controls[0]!, action = control.actions.find(a => a === "AXPress" || a.startsWith("Name:press"));
  if (!action) throw new Error("fixture tab has no advertised press action");
  c.perform(control, action);
  if (!await c.waitFor(() => c.native().elements.some(n => n.identifier === "contentPanelTextField" && n.value === `${c.base}${path}`) ? true : undefined, 5000)) throw new Error("native selected fixture URL did not match");
  const selected = c.native(), address = selected.elements.find(n => n.identifier === "contentPanelTextField" && n.value === `${c.base}${path}`)!;
  const owner = selected.elements.filter(n => n.role === "AXWindow" && address.path.startsWith(n.path + "."));
  if (owner.length === 1 && owner[0]!.actions.includes("AXRaise")) c.perform(owner[0]!, "AXRaise");
  return target;
}
