// Record only disposable trace156, under the common lock. Publish the canonical golden only on all-pass.
import { existsSync, readFileSync, renameSync, rmSync } from "node:fs";
import { resolve, join } from "node:path";
const root = resolve(import.meta.dir, "../..");
const name = Bun.argv[2];
if (!name || !/^[a-z][a-z0-9-]*$/.test(name) || name === "launch-slice") throw new Error("usage: record-golden.ts <new-scenario-name>; default golden is immutable");
if (!existsSync(join(import.meta.dir, `${name}.ts`))) throw new Error("unknown scenario");
if (!Bun.argv.includes("--locked")) {
  const child = Bun.spawn([join(root, "arc/tools/locked.sh"), "bun", import.meta.path, name, "--locked"], { cwd: root, stdout: "inherit", stderr: "inherit" });
  process.exitCode = await child.exited;
} else {
  const app = resolve(root, "../arc/build/ArcModAgent-trace156.app");
  if (Bun.spawnSync(["pgrep", "-f", `^${app}/Contents/MacOS/Arc( |$)`]).success) throw new Error("trace156 already running; refusing profile deletion");
  const candidate = join(root, `results/golden/arc-1.170-${name}.candidate`);
  const accepted = join(root, `results/golden/arc-1.170-${name}`);
  rmSync(candidate, { recursive: true, force: true });
  rmSync(join(process.env.HOME!, "Library/Application Support/ArcModAgent-trace156"), { recursive: true, force: true });
  const child = Bun.spawn(["bun", join(root, "tests/launch-slice/run.ts"), "--app", app, "--out", candidate, "--record", "--scenario", name], { cwd: root, stdout: "inherit", stderr: "inherit" });
  const status = await child.exited;
  const coverage = join(candidate, "coverage.json");
  if (status === 0 && existsSync(coverage) && JSON.parse(readFileSync(coverage, "utf8")).accepted === true) {
    rmSync(accepted, { recursive: true, force: true }); renameSync(candidate, accepted);
    // Paths in runtime logs refer to the candidate location, which contains the same transcript bytes.
    console.log(`accepted golden: ${accepted}`);
  } else {
    console.error(`not accepted; diagnostic candidate retained at ${candidate}`);
    process.exitCode = 1;
  }
}
