import type { ScenarioContext } from "../launch-slice/run";
import { run, press } from "./native";
import { autocomplete } from "./history-autocomplete";
export { chromiumArgs } from "./command-bar-autocomplete";
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    await autocomplete(c);
    const oldTargets = new Set((c.cdp("list") as {id:string}[]).map(t => t.id));
    press(c, /^View History$/);
    const newTargets = (c.cdp("list") as {id:string;url:string}[]).filter(t => !oldTargets.has(t.id));
    if (newTargets.length !== 1 || !/^chrome:\/\/history\//.test(newTargets[0]!.url)) throw new Error("history page was not exposed as one new exact CDP target");
    const history = newTargets[0]!.id;
    const historyText = `(() => {const collect=r=>[...r.querySelectorAll('*')].map(e=>(e.shadowRoot?collect(e.shadowRoot):'')+(e.children.length?'':e.textContent)).join(' ');return collect(document)})()`;
    c.assert("fixture visit exists in history", String(c.evaluate(history, historyText)).includes("launch-slice A"));
    press(c, /^(Clear History|Clear Browsing Data)(?:…)?$/);
    press(c, /^(Clear data|Clear History)$/);
    c.assert("history cleared confirmation", await c.waitFor(() => !String(c.evaluate(history, historyText)).includes("launch-slice A") ? true : undefined, 8000));
  });
}
