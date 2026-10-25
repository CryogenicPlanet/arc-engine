// Select a tab that was opened in the background. Arc's app shows a selected tab only once the engine reports
// it ready to display, so the tab has to become visible and paint. The tab is selected through DevTools
// (Target.activateTarget, like tabs-restore's fallback) with no native UI, so this also runs with the screen
// locked.
import type { ScenarioContext } from "../launch-slice/run";
import { page } from "./native";
export { fixture } from "./tabs-restore";
// A locked screen occludes every window, and a tab that has never been shown stays hidden while occluded.
// Counting occluded windows as visible keeps the result independent of the screen.
export const chromiumArgs = ["--disable-backgrounding-occluded-windows"];

export default async function scenario(c: ScenarioContext) {
  const l = c.launch();
  if (await c.ready(l, "launch")) {
    await c.step("select a background tab", async () => {
      const first = await page(c);
      const background = await page(c, "/restore", "ArcCore restore 7f3a");
      const state = (id: string) => String(c.evaluate(id, "document.visibilityState"));
      c.assert("first tab visible", state(first) === "visible", state(first));
      c.assert("background tab opens hidden", state(background) === "hidden", state(background));
      c.cdp("activate", background);
      const shown = await c.waitFor(() => (state(background) === "visible" ? true : undefined), 8000);
      c.assert("selected background tab becomes visible", shown, state(background));
      c.assert("first tab hidden after the switch", state(first) === "hidden", state(first));
      const frame = c.evaluate(background, `Promise.race([
        new Promise(done => requestAnimationFrame(() => done("painted"))),
        new Promise(done => setTimeout(() => done("no frame"), 3000))])`);
      c.assert("selected background tab paints", frame === "painted", String(frame));
    });
  }
  await c.quit(l, "quit");
}
