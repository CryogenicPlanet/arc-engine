import type { ScenarioContext } from "../launch-slice/run";
import { run, page, observed, set, press } from "./native";
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    await page(c); await page(c, "/b", c.EXPECT.bTitle);
    c.action(observed(c, n => n.identifier === "newTabMenuItem" && n.actions.includes("AXPress")));
    set(c, n => n.identifier === "commandBarTextField", "launch-slice");
    const suggestions = await c.waitFor(() => {
      const state = c.native(); const root = state.elements.find(n => n.identifier === "commandBarSuggestions");
      return root && state.elements.some(n => n.path.startsWith(root.path + ".") && /launch-slice [AB]/.test(n.label)) ? true : undefined;
    }, 8000);
    c.assert("command bar history suggestions", suggestions);
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
