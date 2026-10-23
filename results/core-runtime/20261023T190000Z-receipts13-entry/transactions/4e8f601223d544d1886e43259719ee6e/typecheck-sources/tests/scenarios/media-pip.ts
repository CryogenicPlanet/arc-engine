import type { ScenarioContext } from "../launch-slice/run";
import { playback } from "./media";
import { run } from "./native";
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const t = await playback(c);
    const point = c.evaluate(t, `(() => { const b=document.createElement('button');b.textContent='PiP fixture';b.style='position:fixed;left:10px;top:10px;z-index:9999';
      b.onclick=()=>document.getElementById('scenario-video').requestPictureInPicture().catch(e=>window.pipError=String(e));document.body.append(b);
      const r=b.getBoundingClientRect();return {x:r.x+r.width/2,y:r.y+r.height/2}; })()`) as {x:number;y:number};
    c.cdp("click", t, String(point.x), String(point.y));
    c.assert("video enters PiP", await c.waitFor(() => c.evaluate(t, "document.pictureInPictureElement?.id") === "scenario-video" ? true : undefined, 8000), String(c.evaluate(t, "window.pipError")));
    c.native();
    c.evaluate(t, "document.exitPictureInPicture().then(()=>true)");
    c.assert("PiP exits", c.evaluate(t, "document.pictureInPictureElement===null") === true);
  });
}
