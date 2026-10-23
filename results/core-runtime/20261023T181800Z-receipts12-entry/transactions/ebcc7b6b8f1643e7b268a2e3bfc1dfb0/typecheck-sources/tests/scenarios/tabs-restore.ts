import type { ScenarioContext } from "../launch-slice/run";
import { run, page, press, ensureNativeWindow } from "./native";
export const fixture = (req: Request) => new URL(req.url).pathname === "/restore"
  ? new Response("<!doctype html><title>ArcCore restore 7f3a</title><p>Restorable tab", {headers:{"content-type":"text/html"}}) : undefined;
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    await page(c);
    const t = await page(c, "/restore", "ArcCore restore 7f3a");
    await ensureNativeWindow(c);
    await c.waitFor(() => c.native().elements.some(n => n.label === "ArcCore restore 7f3a" && n.role === "AXButton") ? true : undefined, 5000);
    const tab = c.native().elements.filter(n => n.label === "ArcCore restore 7f3a" && n.role === "AXButton" && n.enabled);
    if (tab.length !== 1) throw new Error("restore tab not uniquely observed");
    const action = tab[0]!.actions.find(a => a === "AXPress" || a.startsWith("Name:press"));
    if (!action) throw new Error("restore tab has no advertised press action");
    c.perform(tab[0]!, action);
    const selected = await c.waitFor(() => c.native().elements.some(n => n.role === "AXTextField" && n.identifier === "contentPanelTextField" && n.value === `${c.base}/restore`) ? true : undefined, 5000);
    c.assert("restore tab visibly selected", selected);
    if (!selected) throw new Error("refusing to archive an unselected target");
    press(c, /^Archive Tab$/);
    c.assert("native archive closes tab", await c.waitFor(() => !(c.cdp("list") as {id:string}[]).some(n => n.id === t) ? true : undefined, 8000));
    press(c, /^Restore Last Closed Tab$/);
    c.assert("native restore returns same document", await c.waitFor(() => (c.cdp("list") as {url:string;title:string}[]).some(n => n.url === `${c.base}/restore` && n.title === "ArcCore restore 7f3a") ? true : undefined, 8000));
  });
}
