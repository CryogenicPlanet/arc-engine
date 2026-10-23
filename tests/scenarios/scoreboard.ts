// Read-only artifact integration. Never trusts diff-run's exit code as a scenario pass.
import { appendFileSync, closeSync, existsSync, openSync, readFileSync, readdirSync, renameSync, statSync, writeFileSync } from "node:fs";
import { join, resolve } from "node:path";
import { createHash } from "node:crypto";
const root = resolve(import.meta.dir, "../..");
const journal = join(root, "logs/wp/scenarios-scoreboard.jsonl");
type Attempt = { scenario: string; started: string; finished?: string; exit?: number; log: string; runnerPid?: number; childPid?: number; diffSHA256?: string; recorderSHA256?: string };
const json = (path: string): any => { try { return JSON.parse(readFileSync(path, "utf8")); } catch { return undefined; } };
const artifact = (path: string) => {
  try { return {text:readFileSync(path,"utf8"), at:statSync(path).mtime.toISOString()}; }
  catch (error) { if ((error as NodeJS.ErrnoException).code === "ENOENT") return undefined; throw error; }
};
const files = (path: string) => {
  try { return readdirSync(path); }
  catch (error) { if ((error as NodeJS.ErrnoException).code === "ENOENT") return []; throw error; }
};
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
const provenance = (path:string) => {
  const record=artifact(path);
  if(!record) return {label:"missing",at:undefined};
  try {
    const rows=record.text.trim().split("\n").map(line=>JSON.parse(line)),calls=rows.filter(row=>row.ev==="call");
    const attributed=calls.filter(row=>typeof row.from==="string"&&row.from.trim().length>0).length;
    return {label:calls.length===0?"no calls":attributed===0?"v1":attributed===calls.length?"v2":`mixed ${attributed}/${calls.length}`,at:record.at};
  }catch{return {label:"incomplete JSON",at:record.at};}
};
export function refresh() {
  const history = attempts();
  const lines = ["# Scenario scoreboard", "", `Updated ${new Date().toISOString()}. Accepted original-engine goldens only; candidates excluded.`, "",
    "Step passes are the latest completed replacement-engine harness artifact, including failed/inconclusive steps in the denominator. Diff lines are exact, escaped for Markdown, for every available launch. Dates expose older artifacts. A newer build failure does not refresh those artifacts. Passing steps alone do not establish boundary parity. Missing diffs are explicit.", "",
    "Overnight desktop state: LOCKED (loginwindow). Native UI and background media failures may be environmental; a difference against an unlocked golden alone does not establish a replacement-only defect. Morning ArcMod scripts remain unrecorded.", "",
    "Recorder migration: v1 lacks caller attribution; v2 records it. The new comparator narrows app-to-engine calls only when both transcripts carry attribution, while retaining callbacks and engine setters. It excludes direct test-driver API calls; those scenarios also require raw transcript gates and sidecar assertions. Compare counts only with the same scope. Older diffs are preserved verbatim and explicitly flagged when a newer golden or transcript makes them stale.", "",
    "| Scenario | Latest ours steps | Latest integrator attempt | Recorder / comparison scope per launch | First two diff lines per launch |", "| --- | --- | --- | --- | --- |"];
  for (const name of acceptedScenarios()) {
    const dir = join(root, "results/ours", name), stepsArtifact = artifact(join(dir,"steps.json"));
    let result: any;
    try { result=stepsArtifact && JSON.parse(stepsArtifact.text); } catch { /* harness has not finished its write */ }
    const steps = result?.steps as {outcome: string}[] | undefined;
    const pass = steps?.filter(s => s.outcome === "pass").length;
    const stepText = steps ? `${pass}/${steps.length} pass; ${steps.filter(s=>s.outcome === "fail").length} fail, ${steps.filter(s=>s.outcome === "inconclusive").length} inconclusive (${stepsArtifact!.at})` : "no completed harness artifact";
    const latest = history.filter(a => a.scenario === name).at(-1);
    const attempt = latest ? `${latest.finished ?? latest.started}: ${latest.finished ? `exit ${latest.exit}; inspect log` : "queued/running"} [log](../../${latest.log})` : "not yet attempted by integrator";
    const golden = join(root, "results/golden", `arc-1.170-${name}`);
    const launches = files(golden).filter(n=>/^launch-\d+$/.test(n)).sort();
    const scopes = launches.map(l=>{
      const g=provenance(join(golden,l,"record.jsonl")),o=provenance(join(dir,l,"record.jsonl"));
      const diff=artifact(join(dir,`diff-${l}.txt`));
      const scope=diff?.text.split("\n").find(line=>line.startsWith("caller attribution:"));
      const stale=diff&&[g.at,o.at].some(at=>at&&at>diff.at);
      return `${l}: golden ${g.label}; ours ${o.label}<br>${escape(scope??(diff?"scope not recorded by older comparator":"no comparison"))}${stale?"<br>STALE: transcript newer than diff":""}`;
    }).join("<br><br>");
    const diffText = launches.map(l => {
      const diff = artifact(join(dir,`diff-${l}.txt`));
      return diff ? `${l} (${diff.at}):<br>${diff.text.split("\n").slice(0,2).map(escape).join("<br>")}` : `${l}: no diff`;
    }).join("<br><br>");
    lines.push(`| [${name}](../../results/golden/arc-1.170-${name}/steps.json) | ${stepText} | ${attempt} | ${scopes} | ${diffText} |`);
  }
  const supplemental=join(root,"results/renderer-canonical-run2"), extraSteps=json(join(supplemental,"steps.json"))?.steps;
  if(extraSteps?.length) {
    const diff=artifact(join(supplemental,"diff-launch-1.txt"));
    const quit=extraSteps.find((s:any)=>s.step==="renderer public API agent quit");
    const functional=extraSteps.filter((s:any)=>s!==quit).every((s:any)=>s.outcome==="pass");
    lines.push("", "Supplemental renderer evidence (Chirp #136769; excluded from canonical counts above): the renderer owner ran the same scenario with the disposable profile preserved, rather than the fresh-profile diff-run wrapper. " +
      `[${extraSteps.filter((s:any)=>s.outcome==="pass").length}/${extraSteps.length} steps pass](../../results/renderer-canonical-run2/steps.json); functional checks ${functional?"pass":"include failures/inconclusives"}; shutdown ${escape(quit?.outcome??"missing")} (${escape(quit?.detail??"no shutdown artifact")}). `+
      "The ten fixture CSS request/return gates are separate from the owner's eight-class/136-row metadata probe. No whole-engine parity claim. " +
      `[Harness](../../logs/wp/renderer.canonical.run2.harness.log), [source freeze](../../logs/wp/renderer.canonical.source.freeze.run2.json).`, "",
      diff?`Supplemental diff (${diff.at}), exact first two lines:<br>${diff.text.split("\n").slice(0,2).map(escape).join("<br>")}`:"Supplemental diff missing.");
  }
  const path = join(import.meta.dir,"SCOREBOARD.md");
  const temporary = `${path}.${process.pid}.${crypto.randomUUID()}.tmp`;
  writeFileSync(temporary, lines.join("\n") + "\n"); renameSync(temporary,path);
  console.log(`scoreboard refreshed: ${acceptedScenarios().length} accepted scenarios`);
}
export function summary() {
  const names = acceptedScenarios();
  let measured = 0, passing = 0, diffs = 0;
  for (const name of names) {
    const dir = join(root,"results/ours",name), steps = json(join(dir,"steps.json"))?.steps;
    if (steps?.length) { measured++; if (steps.every((s: any)=>s.outcome === "pass")) passing++; }
    if (files(dir).some(n=>/^diff-launch-\d+\.txt$/.test(n))) diffs++;
  }
  return `scenarios INTEGRATOR /Users/cryogenicplanet/general/arc-engine: ${names.length} accepted goldens; ours harness artifacts ${measured}/${names.length}, all-steps-pass ${passing}/${names.length}, diff artifacts ${diffs}/${names.length}; ${attempts().filter(a=>a.finished).length} integrator attempts completed. Exact lines/dates/build failures: tests/scenarios/SCOREBOARD.md. Rotation uses shared diff-run lock with >=300s between runs; native recording deferred until unlocked desktop. No boundary parity claim.`;
}
if (import.meta.main) {
  if(Bun.argv.includes("--summary")) { console.log(summary()); process.exit(0); }
  if (Bun.argv.includes("--run-next")) {
    const hash=(path:string)=>{try{return createHash("sha256").update(readFileSync(join(root,path))).digest("hex");}catch{return undefined;}};
    const toolHashes=()=>({diffSHA256:hash("tests/recorder/diff.py"),recorderSHA256:hash("tests/recorder/ArcEngineRecorder.dylib")});
    const scenarios = acceptedScenarios(), history = attempts();
    // Least recently attempted first; new accepted scenarios enter the rotation automatically.
    const name = scenarios.sort((a,b) => (history.filter(x=>x.scenario===a).at(-1)?.started ?? "").localeCompare(history.filter(x=>x.scenario===b).at(-1)?.started ?? ""))[0];
    if (!name) throw new Error("no accepted scenarios");
    const start = new Date().toISOString(), log = `logs/wp/scenarios.scoreboard.${name}.${start.replaceAll(/[:.]/g,"-")}.log`;
    appendFileSync(journal,JSON.stringify({scenario:name,started:start,log,...toolHashes()})+"\n"); refresh();
    const fd = openSync(join(root,log),"w");
    let status: number;
    try {
      const child = Bun.spawn([join(root,"arc/tools/diff-run.sh"),name],{cwd:root,stdout:fd,stderr:fd});
      appendFileSync(journal,JSON.stringify({scenario:name,started:start,log,runnerPid:process.pid,childPid:child.pid})+"\n");
      status = await child.exited;
    } finally { closeSync(fd); }
    appendFileSync(journal,JSON.stringify({scenario:name,started:start,finished:new Date().toISOString(),exit:status,log,...toolHashes()})+"\n");
  }
  refresh();
}
