import type { ScenarioContext } from "../launch-slice/run";
import { run, page, press } from "./native";
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const t = await page(c);
    const p = c.evaluate(t, `(() => {const b=document.createElement('button');b.textContent='Install beforeunload';b.style='position:fixed;top:10px;left:10px';
      b.onclick=()=>window.onbeforeunload=e=>{e.preventDefault();e.returnValue='fixture';return 'fixture'};document.body.append(b);
      const r=b.getBoundingClientRect();return {x:r.x+r.width/2,y:r.y+r.height/2}})()`) as {x:number;y:number};
    c.cdp("click", t, String(p.x), String(p.y));
    c.evaluate(t, `setTimeout(()=>location.assign(${JSON.stringify(`${c.base}/b`)}),50);1`);
    const shown = await c.waitFor(() => c.native().elements.some(n => /^(Stay|Stay on Page|Cancel)$/.test(n.label) && n.enabled && n.actions.includes("AXPress")) ? true : undefined, 8000);
    c.assert("beforeunload prompt", shown);
    press(c, /^(Stay|Stay on Page|Cancel)$/);
    c.assert("stay cancels navigation", c.evaluate(t, "location.href") === `${c.base}/a`);
    c.evaluate(t, `setTimeout(()=>location.assign(${JSON.stringify(`${c.base}/b`)}),50);1`);
    await c.waitFor(() => c.native().elements.some(n => /^(Leave|Leave Page|OK)$/.test(n.label)) ? true : undefined, 8000);
    press(c, /^(Leave|Leave Page|OK)$/);
    c.assert("leave commits navigation", await c.titleIs(t, c.EXPECT.bTitle));
  });
}
