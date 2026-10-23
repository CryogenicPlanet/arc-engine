import type { ScenarioContext } from "../launch-slice/run";

export const morningPlan = [
  "Build a disposable session retaining Little Arc behavior without changing any human bundle/data.",
  "Open fixture A through an exact-agent external-URL path and assert one littleBrowserWindow-* panel.",
  "Open fixture B through the same path and assert the same sole panel now owns fixture B.",
  "Change panel profile to a second disposable profile; assert exactly one panel after native replacement.",
  "Compare boundary transcripts against the stock-engine disposable equivalent.",
];
export default async function scenario(c: ScenarioContext) {
  c.record("single Little Arc parity", "inconclusive", "UNSUPPORTED: ArcModAgent session marker disables singleton policy and redirects New Little Arc to full BrowserWindow; lead must add an isolated opt-in harness first. See MORNING.md.");
}
