// Profile lifecycle through ArcBrowserApplication's public API, driven in-process by
// profiles-public-api-driver.m (no native UI): create "Profile 7f3a", list, get, load the default profile's
// incognito context, reject an empty name and a missing profile, delete. Arc's engine creates the directory
// itself (Chrome's next "Profile N") and uses the name as the visible name; get/delete take the directory. The transcript diff judges the calls;
// the steps check the driver's results. Golden on Arc's engine, same driver.
import type { ScenarioContext } from "../launch-slice/run";
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { join } from "node:path";

type Result = Record<string, unknown> & { complete?: boolean };

export default async function scenario(c: ScenarioContext) {
  const directory = join(c.outDir, "profiles-driver");
  mkdirSync(directory, { recursive: true });
  const library = join(directory, "ProfilesPublicAPIDriver.dylib");
  const compile = Bun.spawnSync(["xcrun", "clang", "-dynamiclib", "-fobjc-arc", "-Wall", "-Wextra", "-Werror",
    "-framework", "Foundation", "-framework", "AppKit", join(import.meta.dir, "profiles-public-api-driver.m"), "-o", library]);
  if (!compile.success) throw new Error(`driver build failed: ${compile.stderr.toString()}`);
  const l = c.launch({ libraries: [library], environment: { ARC_SCENARIO_PROFILES_DIRECTORY: directory } });
  if (await c.ready(l, "launch")) {
    await c.step("profile lifecycle", async () => {
      writeFileSync(join(directory, "go"), "");
      const resultPath = join(directory, "result.json");
      const result = await c.waitFor(() => {
        if (!existsSync(resultPath)) return undefined;
        const r = JSON.parse(readFileSync(resultPath, "utf8")) as Result;
        return r.complete ? r : undefined;
      }, 60000);
      if (!result) throw new Error("driver did not complete");
      const named = (key: string) => (result[key] as string[] | undefined) ?? [];
      const created = String(result.created);
      c.assert("empty name rejected", (result.createEmptyName as { returned: boolean }).returned === false,
        JSON.stringify(result.createEmptyName));
      c.assert("create accepted", (result.create as { returned: boolean }).returned === true, JSON.stringify(result.create));
      c.assert("create completes with a new profile directory", created !== "nil" && created !== "Default", created);
      c.assert("listed with its visible name", named("namesAfterCreate").includes(`${created}=Profile 7f3a`),
        named("namesAfterCreate").join(", "));
      c.assert("get by directory returns it", result.get === created, String(result.get));
      c.record("get by visible name", "pass", String(result.getByName));
      c.assert("load default incognito", result.loadIncognito === "Default (incognito)", String(result.loadIncognito));
      c.assert("load missing gives nil", result.loadMissing === "nil", String(result.loadMissing));
      c.record("delete by visible name", "pass", JSON.stringify(result.deleteByName));
      c.record("delete by directory", "pass", JSON.stringify(result.delete));
      c.assert("delete missing rejected", (result.deleteMissing as { returned: boolean }).returned === false,
        JSON.stringify(result.deleteMissing));
      c.record("names after delete", "pass", named("namesAfterDelete").join(", "));
    });
  }
  await c.quit(l, "quit");
}
