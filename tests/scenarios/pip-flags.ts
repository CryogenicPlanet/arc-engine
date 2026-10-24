// Arc's picture-in-picture policies on a playing video, set on the tab's ArcWebContents in-process by
// pip-flags-driver.m: skipUserActivationForPiP: lets the page enter PiP without a user gesture (Arc's auto-PiP
// when you leave a tab), ignoreDisablePiPAttribute: lets it enter PiP although the video has
// disablepictureinpicture. Requests are made from page script with no user gesture.
import type { ScenarioContext } from "../launch-slice/run";
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { join } from "node:path";
import { playback } from "./media";

const request = `document.getElementById('scenario-video').requestPictureInPicture().then(()=>'entered',e=>e.name)`;
const exit = `(document.pictureInPictureElement ? document.exitPictureInPicture() : Promise.resolve()).then(()=>document.pictureInPictureElement===null)`;

export default async function scenario(c: ScenarioContext) {
  const directory = join(c.outDir, "pip-flags-driver");
  mkdirSync(directory, { recursive: true });
  const library = join(directory, "PiPFlagsDriver.dylib");
  const compile = Bun.spawnSync(["xcrun", "clang", "-dynamiclib", "-fobjc-arc", "-Wall", "-Wextra", "-Werror",
    "-framework", "Foundation", "-framework", "AppKit", join(import.meta.dir, "pip-flags-driver.m"), "-o", library]);
  if (!compile.success) throw new Error(`driver build failed: ${compile.stderr.toString()}`);
  const l = c.launch({ libraries: [library], environment: { ARC_SCENARIO_PIP_FLAGS_DIRECTORY: directory } });
  let next = 1;
  const flag = async (selector: string, value: boolean, url: string) => {
    const n = next++;
    writeFileSync(join(directory, `cmd-${n}.json`), JSON.stringify({ selector, value, url }));
    const done = join(directory, `done-${n}.json`);
    const reply = await c.waitFor(() => existsSync(done) ? JSON.parse(readFileSync(done, "utf8")) as { ok: boolean } : undefined, 10000);
    if (!reply?.ok) throw new Error(`${selector} ${value}: ${JSON.stringify(reply)}`);
    // The policy travels to the renderer separately from DevTools; let it land.
    await c.sleep(500);
  };
  if (await c.ready(l, "launch")) {
    await c.step("pip policies", async () => {
      const t = await playback(c);
      const url = `${c.base}/a`;
      c.assert("no gesture: PiP refused", c.evaluate(t, request) === "NotAllowedError", String(c.evaluate(t, exit)));
      await flag("skipUserActivationForPiP:", true, url);
      c.assert("skip user activation: PiP without a gesture", c.evaluate(t, request) === "entered");
      c.assert("PiP exits", c.evaluate(t, exit) === true);
      c.evaluate(t, "document.getElementById('scenario-video').disablePictureInPicture=true; 1");
      await flag("ignoreDisablePiPAttribute:", false, url);
      const honored = c.evaluate(t, request);
      c.assert("disablepictureinpicture honored", honored === "InvalidStateError", String(honored));
      await flag("ignoreDisablePiPAttribute:", true, url);
      // The app turns skipUserActivationForPiP: off again whenever PiP closes; turn it back on.
      await flag("skipUserActivationForPiP:", true, url);
      const ignored = c.evaluate(t, request);
      c.assert("ignore attribute: PiP despite disablepictureinpicture", ignored === "entered", String(ignored));
      c.assert("PiP exits again", c.evaluate(t, exit) === true);
      await flag("skipUserActivationForPiP:", false, url);
      c.evaluate(t, "document.getElementById('scenario-video').disablePictureInPicture=false; 1");
      c.assert("skip off: gesture required again", c.evaluate(t, request) === "NotAllowedError");
    });
  }
  await c.quit(l, "quit");
}
