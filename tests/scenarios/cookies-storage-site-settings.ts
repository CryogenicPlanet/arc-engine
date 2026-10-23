import type { ScenarioContext } from "../launch-slice/run";
import { run, page, press, pressWhenReady } from "./native";
export async function storageAndDeny(c: ScenarioContext) {
    const t = await page(c);
    c.assert("cookie/local/session storage roundtrip", c.evaluate(t, `(() => {
      document.cookie='scenario_cookie=c0ffee; path=/'; localStorage.setItem('scenario','41'); sessionStorage.setItem('scenario','93');
      return document.cookie.includes('scenario_cookie=c0ffee') && localStorage.getItem('scenario')==='41' && sessionStorage.getItem('scenario')==='93';
    })()`) === true);
    c.evaluate(t, "window.permissionResult='pending'; Notification.requestPermission().then(r=>window.permissionResult=r); 1");
    await pressWhenReady(c, /^(Block|Don’t Allow|Don't Allow|Deny|Not Now)$/);
    c.assert("deny notification prompt", await c.waitFor(() => c.evaluate(t, "window.permissionResult") === "denied" ? true : undefined, 8000), String(c.evaluate(t, "window.permissionResult")));
    c.assert("cookie deletion", c.evaluate(t, "document.cookie='scenario_cookie=; Max-Age=0; path=/'; !document.cookie.includes('scenario_cookie=')") === true);
    return t;
}
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const t = await storageAndDeny(c);
    // A denied origin cannot prompt again. Navigate the selected tab to a fixed fresh permission scope.
    const allowed = t, fresh = c.base.replace("127.0.0.1", "localhost");
    // Chromium's notification cooldown blocks scripted cross-origin re-prompt loops. A trusted link
    // navigation is required to clear it (permission_request_manager.cc:725).
    const point = c.evaluate(t, `(() => {const a=document.createElement('a');a.href=${JSON.stringify(fresh + "/a")};a.textContent='Fresh permission origin';a.style='position:fixed;left:10px;top:10px';document.body.append(a);const r=a.getBoundingClientRect();return {x:r.x+r.width/2,y:r.y+r.height/2}})()`) as {x:number;y:number};
    c.cdp("click", t, String(point.x), String(point.y));
    if (!await c.waitFor(() => c.evaluate(t, "location.origin") === fresh ? true : undefined, 10000)) throw new Error("second permission origin did not load");
    c.evaluate(allowed, "window.permissionResult='pending'; Notification.requestPermission().then(r=>window.permissionResult=r); 1");
    c.assert("second origin starts with undecided permission", c.evaluate(allowed, "Notification.permission") === "default");
    await pressWhenReady(c, /^Allow$/);
    c.assert("allow notification prompt", await c.waitFor(() => c.evaluate(allowed, "window.permissionResult") === "granted" ? true : undefined, 8000));
    press(c, /^Site Settings$/);
    c.assert("site settings reflects insecure HTTP fixture", await c.waitFor(() => {
      const state = c.native(); const control = state.elements.find(n => n.label === "Site Control Center");
      return control && state.elements.some(n => n.path.startsWith(control.path + ".") && n.label === "Lock Off Insecure") ? true : undefined;
    }, 8000));

  });
}
