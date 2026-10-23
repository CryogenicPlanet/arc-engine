import type { ScenarioContext } from "../launch-slice/run";
import { run, press, set, observed } from "./native";
let attempts = 0, matched = false, usernameMatches = false, passwordMatches = false;
export function fixture(req: Request) {
  if (new URL(req.url).pathname === "/auth-state") return Response.json({attempts, matched, usernameMatches, passwordMatches});
  if (new URL(req.url).pathname !== "/auth") return;
  attempts++;
  const authorization = req.headers.get("authorization") ?? "";
  if (/^Basic /i.test(authorization)) {
    const credentials = atob(authorization.slice(6)).split(":");
    usernameMatches = credentials[0] === "fixture-user"; passwordMatches = credentials.slice(1).join(":") === "fixture-pass";
  }
  if (req.headers.get("authorization") === "Basic " + btoa("fixture-user:fixture-pass")) { matched = true; return new Response("<!doctype html><title>ArcCore auth fixture</title><p>Authenticated 7f3a", {headers:{"content-type":"text/html"}}); }
  return new Response("fixture auth required", {status:401, headers:{"www-authenticate":'Basic realm="ArcCore fixture 7f3a"'}});
}
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const t = c.open(`${c.base}/auth`);
    c.assert("HTTP auth prompt", await c.waitFor(() => c.native().elements.some(n => n.subrole === "AXSecureTextField") ? true : undefined, 8000));
    const sheet = c.native().elements.filter(n => n.role === "AXSheet");
    if (sheet.length !== 1) throw new Error("auth sheet not uniquely identified");
    set(c, n => n.role === "AXTextField" && n.subrole !== "AXSecureTextField" && n.path.startsWith(sheet[0]!.path + "."), "fixture-user");
    set(c, n => n.subrole === "AXSecureTextField", "fixture-pass");
    c.perform(observed(c, n => n.subrole === "AXSecureTextField" && n.actions.includes("AXConfirm")), "AXConfirm");
    await c.sleep(300);
    if (c.native().elements.some(n => n.role === "AXSheet")) press(c, /^(Sign [Ii]n|Log [Ii]n|OK)$/);
    const accepted = await c.titleIs(t, "ArcCore auth fixture");
    c.assert("HTTP auth accepted", accepted);
    if (!accepted) {
      const state = await (await fetch(`${c.base}/auth-state`)).json();
      c.record("auth fixture diagnostics", "inconclusive", JSON.stringify(state));
      c.native();
      // Never leave a repeated auth sheet blocking graceful cleanup after an assertion failure.
      press(c, /^Cancel$/);
    }
  });
}
