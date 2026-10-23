import type { ScenarioContext } from "../launch-slice/run";
import { run, page, press } from "./native";
export const fixture = (req: Request) => new URL(req.url).pathname === "/restore"
  ? new Response("<!doctype html><title>ArcCore restore 7f3a</title><p>Restorable tab", {headers:{"content-type":"text/html"}}) : undefined;
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    await page(c);
    const t = await page(c, "/restore", "ArcCore restore 7f3a");
    const tab = c.native().elements.filter(n => n.label === "ArcCore restore 7f3a" && n.role === "AXButton" && n.enabled);
    if (tab.length !== 1 || !tab[0]!.actions.includes("AXPress")) throw new Error("restore tab cannot be selected through observed AXPress; native adapter lacks its custom press action");
    c.action(tab[0]!);
    c.assert("restore tab visibly selected", c.native().elements.some(n => n.role === "AXTextField" && n.identifier === "contentPanelTextField" && n.value === `${c.base}/restore`));
    press(c, /^Archive Tab$/);
    c.assert("native archive closes tab", await c.waitFor(() => !(c.cdp("list") as {id:string}[]).some(n => n.id === t) ? true : undefined, 8000));
    press(c, /^Restore Last Closed Tab$/);
    c.assert("native restore returns same document", await c.waitFor(() => (c.cdp("list") as {url:string;title:string}[]).some(n => n.url === `${c.base}/restore` && n.title === "ArcCore restore 7f3a") ? true : undefined, 8000));
  });
}
