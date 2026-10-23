import type { ScenarioContext, NativeNode } from "../launch-slice/run";
export function observed(c: ScenarioContext, match: (node: NativeNode) => boolean): NativeNode {
  const nodes = c.native().elements.filter(n => n.enabled && match(n));
  if (nodes.length !== 1) throw new Error(`expected one native control; observed ${nodes.length}`);
  return nodes[0]!;
}
export function press(c: ScenarioContext, label: RegExp) {
  const state = c.native();
  const matches = state.elements.filter(n => label.test(n.label) && n.actions.includes("AXPress"));
  if (matches.length === 1 && !matches[0]!.enabled) {
    const item = matches[0]!;
    const menu = state.elements.find(n => n.role === "AXMenuBarItem" && item.path.startsWith(n.path + "."));
    if (menu?.enabled && menu.actions.includes("AXPress")) c.action(menu);
  }
  const node = observed(c, n => label.test(n.label) && n.actions.includes("AXPress"));
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
