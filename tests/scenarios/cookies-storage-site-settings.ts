import type { ScenarioContext } from "../launch-slice/run";
import { run, page, press } from "./native";
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const t = await page(c);
    c.assert("cookie/local/session storage roundtrip", c.evaluate(t, `(() => {
      document.cookie='scenario_cookie=c0ffee; path=/'; localStorage.setItem('scenario','41'); sessionStorage.setItem('scenario','93');
      return document.cookie.includes('scenario_cookie=c0ffee') && localStorage.getItem('scenario')==='41' && sessionStorage.getItem('scenario')==='93';
    })()`) === true);
    c.evaluate(t, "window.permissionResult='pending'; Notification.requestPermission().then(r=>window.permissionResult=r); 1");
    press(c, /^(Block|Don’t Allow|Don't Allow|Deny|Not Now)$/);
    c.assert("dismiss notification prompt without granting", await c.waitFor(() => c.evaluate(t, "window.permissionResult") === "default" ? true : undefined, 8000));
    c.evaluate(t, "window.permissionResult='pending'; Notification.requestPermission().then(r=>window.permissionResult=r); 1");
    press(c, /^Allow$/);
    c.assert("allow notification prompt", await c.waitFor(() => c.evaluate(t, "window.permissionResult") === "granted" ? true : undefined, 8000));
    press(c, /^Site Settings$/);
    c.assert("site settings exposes notification control", c.native().elements.some(n => /Notifications/.test(n.label)));
    c.assert("cookie deletion", c.evaluate(t, "document.cookie='scenario_cookie=; Max-Age=0; path=/'; !document.cookie.includes('scenario_cookie=')") === true);
  });
}
