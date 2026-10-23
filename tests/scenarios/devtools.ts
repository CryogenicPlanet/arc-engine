import type { ScenarioContext } from "../launch-slice/run";
import { run, page, press } from "./native";
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    await page(c);
    press(c, /^Toggle Console$/);
    const client = await c.client();
    c.assert("developer frontend opens", await c.waitFor(() => (c.cdp("list") as { url: string }[])
      .some(t => /^(devtools|chrome-devtools):/.test(t.url)) ? true : undefined, 8000));
    // Toggle uses Arc's menu route, rather than detaching the recorder's CDP connection.
    press(c, /^Toggle Console$/);
    const targets = await client.request("Target.getTargets");
    c.assert("developer frontend closes", !(targets.targetInfos as { url: string }[]).some(t => /^(devtools|chrome-devtools):/.test(t.url)));
  });
}
