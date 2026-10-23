// Form submissions and the app's navigation policy: Arc's engine asks
// webContents:decidePolicyForNavigationAction: for script/link navigations but not for the POST login form in
// password-save. A GET form, a POST form and a script navigation separate "POST" from "form submission".
import type { ScenarioContext } from "../launch-slice/run";
import { run } from "./native";

const html = (title: string, body: string) =>
  new Response(`<!doctype html><title>${title}</title><body>${body}</body>`, { headers: { "content-type": "text/html", "cache-control": "no-store" } });

export const fixture = (request: Request) => {
  const url = new URL(request.url);
  if (url.pathname === "/forms-7f3a")
    return html("forms start",
      `<form id="g" method="get" action="/forms-7f3a/get"><input name="q" value="v"></form>` +
      `<form id="p" method="post" action="/forms-7f3a/post"><input name="q" value="v"></form>`);
  if (url.pathname === "/forms-7f3a/get") return html(`forms get ${url.searchParams.get("q")}`, "<p>get</p>");
  if (url.pathname === "/forms-7f3a/post" && request.method === "POST") return html("forms post", "<p>post</p>");
  if (url.pathname === "/forms-7f3a/script") return html("forms script", "<p>script</p>");
  return undefined;
};

export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const t = c.open(`${c.base}/forms-7f3a`);
    c.assert("start loaded", await c.titleIs(t, "forms start"));
    c.evaluate(t, "document.getElementById('g').submit(); 1");
    c.assert("GET form navigated", await c.titleIs(t, "forms get v"));
    c.evaluate(t, `location.href = ${JSON.stringify(`${c.base}/forms-7f3a`)}; 1`);
    c.assert("back to start", await c.titleIs(t, "forms start"));
    c.evaluate(t, "document.getElementById('p').submit(); 1");
    c.assert("POST form navigated", await c.titleIs(t, "forms post"));
    c.evaluate(t, `location.href = ${JSON.stringify(`${c.base}/forms-7f3a/script`)}; 1`);
    c.assert("script navigation", await c.titleIs(t, "forms script"));
  });
}
