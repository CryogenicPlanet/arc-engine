import type { ScenarioContext } from "../launch-slice/run";
export const fixture = (req: Request) => new URL(req.url).pathname === "/redirect"
  ? new Response(null, { status: 302, headers: { location: "/b" } }) : undefined;
export default async function run(c: ScenarioContext) {
  const l = c.launch();
  if (await c.ready(l, "T1 launch")) await c.step("tabs-navigation", async () => {
    const a = c.open(`${c.base}/a`), b = c.open(`${c.base}/b`), third = c.open(`${c.base}/a`);
    c.assert("T2 three tabs", await c.titleIs(a, c.EXPECT.aTitle) && await c.titleIs(b, c.EXPECT.bTitle)
      && await c.titleIs(third, c.EXPECT.aTitle) && new Set([a, b, third]).size === 3);
    c.evaluate(a, `location.assign(${JSON.stringify(`${c.base}/b`)}); 1`);
    c.assert("T3 document navigation", await c.titleIs(a, c.EXPECT.bTitle));
    c.evaluate(a, "history.back(); 1");
    c.assert("T4 back", await c.titleIs(a, c.EXPECT.aTitle));
    c.evaluate(a, "history.forward(); 1");
    c.assert("T5 forward", await c.titleIs(a, c.EXPECT.bTitle));
    c.evaluate(a, "sessionStorage.setItem('reload-count', '41'); location.reload(); 1");
    c.assert("T6 reload", await c.waitFor(() => c.evaluate(a, "performance.getEntriesByType('navigation')[0]?.type") === "reload" ? true : undefined, 10000)
      && await c.titleIs(a, c.EXPECT.bTitle) && c.evaluate(a, "sessionStorage.getItem('reload-count')") === "41");
    c.evaluate(a, "location.hash = 'scenario-fragment'; 1");
    c.assert("T7 same document", await c.waitFor(() => c.evaluate(a, "location.hash") === "#scenario-fragment" ? true : undefined, 5000));
    c.evaluate(a, `location.assign(${JSON.stringify(`${c.base}/redirect`)}); 1`);
    c.assert("T8 redirect", await c.waitFor(() => c.evaluate(a, "location.href") === `${c.base}/b` ? true : undefined, 10000), String(c.evaluate(a, "location.href")));
    c.cdp("close", third);
    const targets = c.cdp("list") as { id: string }[];
    c.assert("T9 close exact tab", !targets.some(t => t.id === third) && targets.some(t => t.id === a) && targets.some(t => t.id === b));
    const reopened = c.open(`${c.base}/a`);
    c.assert("T10 open replacement tab", reopened !== third && await c.titleIs(reopened, c.EXPECT.aTitle));
  });
  await c.quit(l, "T11 quit");
}
