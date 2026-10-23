// A login form is typed into and submitted; Chrome's password manager offers to save it, which Arc's
// engine delivers to the app as -[ArcWebContentsUIDelegate webContents:didSubmitCredentials:]
// (ArcLoginCredentialsSaveRequest). The transcript diff judges that callback; page steps check the form flow.
import type { ScenarioContext } from "../launch-slice/run";
import { run } from "./native";

const html = (title: string, body: string) =>
  new Response(`<!doctype html><title>${title}</title><body>${body}</body>`, { headers: { "content-type": "text/html", "cache-control": "no-store" } });

export const fixture = (request: Request) => {
  const path = new URL(request.url).pathname;
  if (path === "/login-7f3a")
    return html("password-save login",
      `<form id="f" method="post" action="/login-7f3a/done">` +
      `<input id="u" name="username" autocomplete="username"><input id="p" name="password" type="password" autocomplete="current-password">` +
      `<button id="s" type="submit">Sign in</button></form>`);
  if (path === "/login-7f3a/done" && request.method === "POST") return html("password-save done", "<p>signed in</p>");
  return undefined;
};

const typeInto = (c: ScenarioContext, t: string, id: string, text: string) => {
  c.evaluate(t, `document.getElementById(${JSON.stringify(id)}).focus(); 1`);
  c.cdp("type", t, text);
};

export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const t = c.open(`${c.base}/login-7f3a`);
    c.assert("login form loaded", await c.titleIs(t, "password-save login"));
    typeInto(c, t, "u", "arccore-user-7f3a");
    typeInto(c, t, "p", "arccore-pass-7f3a-Q9!");
    c.assert("fields typed", c.evaluate(t, "document.getElementById('u').value + '|' + document.getElementById('p').value.length") === "arccore-user-7f3a|21");
    c.evaluate(t, "document.getElementById('s').click(); 1");
    c.assert("form submitted", await c.titleIs(t, "password-save done"));
    // Give the password manager time to decide and notify the app.
    await c.sleep(3000);
    c.assert("tab alive after save offer", c.evaluate(t, "document.title") === "password-save done");
    // Closing the tab withdraws the pending offer (Arc's engine otherwise holds quit while the prompt is up).
    c.cdp("close", t);
    await c.sleep(1000);
  });
}
