import type { ScenarioContext } from "../launch-slice/run";
import { run } from "./native";
import { storageAndDeny } from "./cookies-storage-site-settings";
export default async function scenario(c: ScenarioContext) { await run(c, async () => { await storageAndDeny(c); }); }
