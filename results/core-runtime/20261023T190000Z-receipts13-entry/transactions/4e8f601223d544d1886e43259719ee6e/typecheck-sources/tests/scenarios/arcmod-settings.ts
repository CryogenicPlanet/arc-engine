import type { ScenarioContext } from "../launch-slice/run";
import { run, nativePage, press, observed, windows } from "./native";

// Morning only: all native handles belong to the harness's exact agent PID.
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const tab = await nativePage(c);
    const initial = windows(c);
    c.assert("one initial browser window", initial.length === 1);
    if (initial.length !== 1) throw new Error("Settings test requires one disposable browser window");
    const browser = initial[0]!.identifier;
    if (!browser.startsWith("bigBrowserWindow-")) throw new Error("Initial window is not the disposable full browser window");
    press(c, /^Settings…$/);
    const opened = await c.waitFor(() => {
      const current = windows(c);
      return current.length === 2 && current.filter(n => n.identifier !== browser).length === 1 ? true : undefined;
    }, 8000);
    c.assert("Settings opens one additional native window", opened);
    if (!opened) throw new Error("Settings window not uniquely exposed; no close will be attempted");
    const state = c.native();
    const settings = state.elements.filter(n => n.role === "AXWindow" && n.identifier !== browser);
    if (settings.length !== 1) throw new Error("Settings window became ambiguous");
    const path = settings[0]!.path;
    c.action(observed(c, n => n.path.startsWith(path + ".") && n.subrole === "AXCloseButton" && n.actions.includes("AXPress")));
    c.assert("Settings closes while browser remains", await c.waitFor(() => {
      const current = windows(c);
      return current.length === 1 && current[0]!.identifier === browser ? true : undefined;
    }, 8000));
    c.assert("fixture tab survives Settings close", await c.titleIs(tab, c.EXPECT.aTitle));
  });
}
