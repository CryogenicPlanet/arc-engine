// An address form and then a card form are typed into and submitted. Chrome's autofill offers to save them,
// which Arc's engine delivers as -[ArcWebContentsUIDelegate webContents:didSubmitAddressEntry:]
// (ArcAddressEntrySaveRequest) and webContents:didSubmitCreditCardInfo: (ArcCreditCardInfoSaveRequest).
// The transcript diff judges those callbacks; page steps check the form flow.
import type { ScenarioContext } from "../launch-slice/run";
import { run } from "./native";

const html = (title: string, body: string) =>
  new Response(`<!doctype html><title>${title}</title><body>${body}</body>`, { headers: { "content-type": "text/html", "cache-control": "no-store" } });

const ADDRESS: Array<[string, string, string]> = [
  ["n", "name", "Ada Lovelace"],
  ["a1", "address-line1", "1600 Amphitheatre Parkway"],
  ["c", "address-level2", "Mountain View"],
  ["s", "address-level1", "CA"],
  ["z", "postal-code", "94043"],
  ["t", "tel", "6505550123"],
  ["e", "email", "ada@example.com"],
];
const CARD: Array<[string, string, string]> = [
  ["cn", "cc-name", "Ada Lovelace"],
  ["cc", "cc-number", "4111111111111111"],
  ["cm", "cc-exp-month", "12"],
  ["cy", "cc-exp-year", "2031"],
];

const form = (action: string, fields: Array<[string, string, string]>) =>
  `<form method="post" action="${action}">` +
  fields.map(([id, ac]) => `<label>${ac} <input id="${id}" name="${ac}" autocomplete="${ac}"></label><br>`).join("") +
  `<button id="go" type="submit">Submit</button></form>`;

export const fixture = (request: Request) => {
  const path = new URL(request.url).pathname;
  if (path === "/address-7f3a") return html("autofill address", form("/address-7f3a/done", ADDRESS));
  if (path === "/address-7f3a/done" && request.method === "POST") return html("autofill address done", "<p>ok</p>");
  if (path === "/card-7f3a") return html("autofill card", form("/card-7f3a/done", CARD));
  if (path === "/card-7f3a/done" && request.method === "POST") return html("autofill card done", "<p>ok</p>");
  return undefined;
};

async function submit(c: ScenarioContext, path: string, title: string, fields: Array<[string, string, string]>) {
  const t = c.open(`${c.base}${path}`);
  c.assert(`${title} loaded`, await c.titleIs(t, title));
  for (const [id, , value] of fields) {
    c.evaluate(t, `document.getElementById(${JSON.stringify(id)}).focus(); 1`);
    c.cdp("type", t, value);
  }
  const typed = fields.every(([id, , value]) => c.evaluate(t, `document.getElementById(${JSON.stringify(id)}).value`) === value);
  c.assert(`${title} typed`, typed);
  c.evaluate(t, "document.getElementById('go').click(); 1");
  c.assert(`${title} submitted`, await c.titleIs(t, `${title} done`));
  // Give autofill time to decide and notify the app.
  await c.sleep(3000);
  c.cdp("close", t);
  await c.sleep(1000);
}

export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    await submit(c, "/address-7f3a", "autofill address", ADDRESS);
    await submit(c, "/card-7f3a", "autofill card", CARD);
  });
}
