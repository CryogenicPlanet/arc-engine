// Read-only artifact integration. Never trusts diff-run's exit code as a scenario pass.
import { appendFileSync, closeSync, existsSync, openSync, readFileSync, readdirSync, renameSync, statSync, writeFileSync } from "node:fs";
import { join, resolve } from "node:path";
const root = resolve(import.meta.dir, "../..");
const journal = join(root, "logs/wp/scenarios-scoreboard.jsonl");
type Attempt = { scenario: string; started: string; finished?: string; exit?: number; log: string; runnerPid?: number; childPid?: number };
const json = (path: string): any => { try { return JSON.parse(readFileSync(path, "utf8")); } catch { return undefined; } };
export function acceptedScenarios() {
  return readdirSync(join(root, "results/golden")).filter(n => /^arc-1\.170-[a-z0-9-]+$/.test(n)).filter(n => {
    const dir = join(root, "results/golden", n), steps = json(join(dir, "steps.json"))?.steps;
    return steps?.length && steps.every((s: any) => s.outcome === "pass") &&
      (n === "arc-1.170-launch-slice" || json(join(dir, "coverage.json"))?.accepted === true);
  }).map(n => n.slice("arc-1.170-".length)).sort();
}
export const attempts = (): Attempt[] => existsSync(journal) ? readFileSync(journal, "utf8").split("\n").filter(Boolean).flatMap(l => {
  try { return [JSON.parse(l) as Attempt]; } catch { return []; } // An interrupted final journal append is not a completed attempt.
}) : [];
const escape = (s: string) => s.replaceAll("&", "&amp;").replaceAll("<", "&lt;").replaceAll(">", "&gt;").replaceAll("|", "&#124;").replaceAll("`", "&#96;");
export function refresh() {
  const history = attempts();
  const lines = ["# Scenario scoreboard", "", `Updated ${new Date().toISOString()}. Accepted original-engine goldens only; candidates excluded.`, "",
    "Step passes are the latest completed replacement-engine harness artifact, including failed/inconclusive steps in the denominator. Diff lines are exact, escaped for Markdown, for every available launch. Dates expose older artifacts. A newer build failure does not refresh those artifacts. Passing steps alone do not establish boundary parity. Missing diffs are explicit.", "",
    "| Scenario | Latest ours steps | Latest integrator attempt | First two diff lines per launch |", "| --- | --- | --- | --- |"];
  for (const name of acceptedScenarios()) {
    const dir = join(root, "results/ours", name), stepsPath = join(dir, "steps.json"), result = json(stepsPath);
    const steps = result?.steps as {outcome: string}[] | undefined;
    const pass = steps?.filter(s => s.outcome === "pass").length;
    const stepText = steps ? `${pass}/${steps.length} pass; ${steps.filter(s=>s.outcome === "fail").length} fail, ${steps.filter(s=>s.outcome === "inconclusive").length} inconclusive (${statSync(stepsPath).mtime.toISOString()})` : "no completed harness artifact";
    const latest = history.filter(a => a.scenario === name).at(-1);
    const attempt = latest ? `${latest.finished ?? latest.started}: ${latest.finished ? `exit ${latest.exit}; inspect log` : "queued/running"} [log](../../${latest.log})` : "not yet attempted by integrator";
    const golden = join(root, "results/golden", `arc-1.170-${name}`);
    const launches = readdirSync(golden).filter(n=>/^launch-\d+$/.test(n)).sort();
    const diffText = launches.map(l => {
      const file = join(dir, `diff-${l}.txt`);
      return existsSync(file) ? `${l} (${statSync(file).mtime.toISOString()}):<br>${readFileSync(file,"utf8").split("\n").slice(0,2).map(escape).join("<br>")}` : `${l}: no diff`;
    }).join("<br><br>");
    lines.push(`| [${name}](../../results/golden/arc-1.170-${name}/steps.json) | ${stepText} | ${attempt} | ${diffText} |`);
  }
  const path = join(import.meta.dir,"SCOREBOARD.md");
  writeFileSync(path + ".tmp", lines.join("\n") + "\n"); renameSync(path + ".tmp",path);
  console.log(`scoreboard refreshed: ${acceptedScenarios().length} accepted scenarios`);
}
export function summary() {
  const names = acceptedScenarios();
  let measured = 0, passing = 0, diffs = 0;
  for (const name of names) {
    const dir = join(root,"results/ours",name), steps = json(join(dir,"steps.json"))?.steps;
    if (steps?.length) { measured++; if (steps.every((s: any)=>s.outcome === "pass")) passing++; }
    if (existsSync(dir) && readdirSync(dir).some(n=>/^diff-launch-\d+\.txt$/.test(n))) diffs++;
  }
  return `scenarios INTEGRATOR /Users/cryogenicplanet/general/arc-engine: ${names.length} accepted goldens; ours harness artifacts ${measured}/${names.length}, all-steps-pass ${passing}/${names.length}, diff artifacts ${diffs}/${names.length}; ${attempts().filter(a=>a.finished).length} integrator attempts completed. Exact lines/dates/build failures: tests/scenarios/SCOREBOARD.md. Rotation uses shared diff-run lock with >=300s between runs; native recording deferred until unlocked desktop. No boundary parity claim.`;
}
if (import.meta.main) {
  if (Bun.argv.includes("--run-next")) {
    const scenarios = acceptedScenarios(), history = attempts();
    // Least recently attempted first; new accepted scenarios enter the rotation automatically.
    const name = scenarios.sort((a,b) => (history.filter(x=>x.scenario===a).at(-1)?.started ?? "").localeCompare(history.filter(x=>x.scenario===b).at(-1)?.started ?? ""))[0];
    if (!name) throw new Error("no accepted scenarios");
    const start = new Date().toISOString(), log = `logs/wp/scenarios.scoreboard.${name}.${start.replaceAll(/[:.]/g,"-")}.log`;
    appendFileSync(journal,JSON.stringify({scenario:name,started:start,log})+"\n"); refresh();
    const fd = openSync(join(root,log),"w");
    let status: number;
    try {
      const child = Bun.spawn([join(root,"arc/tools/diff-run.sh"),name],{cwd:root,stdout:fd,stderr:fd});
      appendFileSync(journal,JSON.stringify({scenario:name,started:start,log,runnerPid:process.pid,childPid:child.pid})+"\n");
      status = await child.exited;
    } finally { closeSync(fd); }
    appendFileSync(journal,JSON.stringify({scenario:name,started:start,finished:new Date().toISOString(),exit:status,log})+"\n");
  }
  refresh();
}
