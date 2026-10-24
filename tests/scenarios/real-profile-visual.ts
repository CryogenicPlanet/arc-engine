// Visual check on a disposable COPY of a real ArcMod profile (prepared by logs/run-realcopy.sh; never the
// original): launch, let Arc restore its spaces and tabs, bring it to the front and capture each of its windows,
// open Settings and capture it, quit, relaunch and capture again. Screenshots stay in the run's output
// directory (gitignored) for review. No navigation or typing.
import type { ScenarioContext } from "../launch-slice/run";
import { join } from "node:path";
import { press, windows } from "./native";

function windowIds(c: ScenarioContext): string {
  const tool = join(c.outDir, "window-ids");
  const build = Bun.spawnSync(["xcrun", "swiftc", "-O", join(import.meta.dir, "window-ids.swift"), "-o", tool]);
  if (!build.success) throw new Error(`window-ids build failed: ${build.stderr.toString()}`);
  return tool;
}

function capture(c: ScenarioContext, pid: number, label: string) {
  const list = Bun.spawnSync([windowIds(c), String(pid)]).stdout.toString().trim().split("\n").filter(Boolean);
  list.forEach((line, i) => {
    const [id, size, ...title] = line.split(" ");
    const file = join(c.outDir, `${label}-${i + 1}.png`);
    const shot = Bun.spawnSync(["screencapture", "-x", "-o", `-l${id}`, file]);
    c.record(`screenshot ${label} ${i + 1}`, shot.success ? "pass" : "fail", `${size} ${title.join(" ")} -> ${file}`);
  });
  if (!list.length) c.record(`screenshot ${label}`, "fail", "no on-screen windows");
}

function front(pid: number) {
  Bun.spawnSync(["osascript", "-e", `tell application "System Events" to set frontmost of (first process whose unix id is ${pid}) to true`]);
}

export default async function scenario(c: ScenarioContext) {
  for (const round of [1, 2]) {
    const l = c.launch();
    if (!await c.ready(l, `launch ${round}`)) {
      await c.quit(l, `quit ${round}`);
      return;
    }
    await c.sleep(15000);
    const pid = l.pid()!;
    front(pid);
    await c.sleep(2000);
    const pages = (c.cdp("list") as Array<{ url?: string }>).length;
    c.record(`restored pages ${round}`, "pass", `${pages} page targets`);
    capture(c, pid, `main-${round}`);
    if (round === 1) {
      await c.step("settings", async () => {
        const before = windows(c).length;
        press(c, /^Settings…$/);
        await c.waitFor(() => windows(c).length > before ? true : undefined, 8000);
        await c.sleep(1500);
        capture(c, pid, "settings");
      });
    }
    await c.quit(l, `quit ${round}`);
  }
}
