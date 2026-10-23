import type { ScenarioContext } from "../launch-slice/run";

export const morningPlan = [
  "With a fixture tab selected in the exact agent PID, dispatch Cmd+T; assert commandBarTextField appears.",
  "Dispatch Cmd+L; assert selected fixture URL in commandBarTextField, then cancel with Escape.",
  "Dispatch Cmd+comma; assert exactly one Settings window, then close its observed AXCloseButton.",
  "Dispatch Cmd+W; assert the selected fixture target is gone and a sentinel tab remains.",
  "Dispatch Cmd+Shift+T; assert restored fixture URL/title. Dispatch Cmd+N; assert two full windows.",
  "Use the agent's policy for Cmd+Option+N: expect a full window; true Little Arc policy has separate coverage.",
];
export default async function scenario(c: ScenarioContext) {
  c.record("physical key shortcut parity", "inconclusive", "UNSUPPORTED: PID-bound native bridge exposes advertised AX actions, not native keyboard dispatch. CDP page key events or AX menu presses do not test NSApplication shortcut routing. Lead must add exact-process key dispatch; no System Events/global input fallback.");
}
