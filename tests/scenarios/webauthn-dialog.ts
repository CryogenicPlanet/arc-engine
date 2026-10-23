// A passkey request opens Chrome's WebAuthn dialog, a tab-modal views dialog (security key / phone / ...).
// Arc's app has no callback for it, so the engine must host Chrome's dialog over Arc's window. Steps check
// that the request starts, the engine survives while the dialog is up, and closing the tab ends it.
import type { ScenarioContext } from "../launch-slice/run";
import { run } from "./native";

export const fixture = (request: Request) =>
  new URL(request.url).pathname === "/webauthn-7f3a"
    ? new Response(`<!doctype html><title>webauthn dialog</title><button id="b">create</button>`,
        { headers: { "content-type": "text/html", "cache-control": "no-store" } })
    : undefined;

export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    // WebAuthn needs a valid RP ID; localhost is one, 127.0.0.1 isn't.
    const t = c.open(`${c.base.replace("127.0.0.1", "localhost")}/webauthn-7f3a`);
    c.assert("page loaded", await c.titleIs(t, "webauthn dialog"));
    c.evaluate(t, `window.webauthnState = 'pending';
      navigator.credentials.create({publicKey: {
        challenge: new Uint8Array(32), rp: {name: 'ArcCore 7f3a'},
        user: {id: new Uint8Array(16), name: 'arccore-7f3a', displayName: 'ArcCore 7f3a'},
        pubKeyCredParams: [{type: 'public-key', alg: -7}], timeout: 60000}})
        .then(() => { window.webauthnState = 'created'; }, e => { window.webauthnState = 'rejected:' + e.name; });
      1`);
    await c.sleep(3000);
    const state = String(c.evaluate(t, "window.webauthnState"));
    c.assert("engine alive with the passkey request open", state === "pending" || state.startsWith("rejected"), state);
    c.record("request state after 3 s", "pass", state);
    c.cdp("close", t);
    await c.sleep(1500);
  });
}
