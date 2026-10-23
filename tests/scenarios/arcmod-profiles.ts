import type { ScenarioContext } from "../launch-slice/run";
import { run, nativePage, press } from "./native";

export const morningPlan = [
  "Create a profile named ArcCore Fixture Profile through observed enabled New Profile and its native editor.",
  "In the original profile set fixture-origin localStorage.profileMarker='arccore-original-7f3a'.",
  "Assign a disposable Space to ArcCore Fixture Profile using observed Change Profile controls.",
  "Open the same fixture origin there; assert profileMarker is null, then set 'arccore-created-7f3a'.",
  "Switch back to the original profile/Space; assert profileMarker is 'arccore-original-7f3a'.",
  "Relaunch the same disposable agent; assert both profile names and both isolated markers persist.",
];
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    await nativePage(c);
    // Observe the real local-mode gate, never make an account/profile mutation
    // using an unobserved private selector or a guessed Settings control.
    press(c, /^File$/);
    const items = c.native().elements.filter(n => n.role === "AXMenuItem" && n.label === "New Profile");
    c.assert("New Profile command exposed exactly once", items.length === 1);
    c.record("create and switch isolated profiles", "inconclusive", items.length === 1 && !items[0]!.enabled
      ? "BLOCKED: New Profile is disabled in this exact agent snapshot; ArcModLocalMenuAction does not force-enable profile creation. The original native validation reason is unknown."
      : "NEEDS MORNING CAPTURE: profile editor control labels and Change Profile selection must be observed before implementing this mutation; see morningPlan.");
  });
}
