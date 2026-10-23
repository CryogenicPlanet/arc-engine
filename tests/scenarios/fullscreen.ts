import type { ScenarioContext } from "../launch-slice/run";
import { run, page } from "./native";
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const t = await page(c);
    const p = c.evaluate(t, `(() => {const b=document.createElement('button');b.textContent='Fullscreen fixture';b.style='position:fixed;left:10px;top:10px';
      b.onclick=()=>document.documentElement.requestFullscreen().catch(e=>window.fullscreenError=String(e));document.body.append(b);
      const r=b.getBoundingClientRect();return {x:r.x+r.width/2,y:r.y+r.height/2}})()`) as {x:number;y:number};
    c.cdp("click", t, String(p.x), String(p.y));
    c.assert("page enters fullscreen through delegate", await c.waitFor(() => c.evaluate(t, "document.fullscreenElement===document.documentElement") === true ? true : undefined, 8000), String(c.evaluate(t, "window.fullscreenError")));
    c.native();
    c.evaluate(t, "document.exitFullscreen().then(()=>true)");
    c.assert("fullscreen exited", c.evaluate(t, "document.fullscreenElement===null") === true);
  });
}
