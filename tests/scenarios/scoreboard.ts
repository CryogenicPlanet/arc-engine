// Read-only artifact integration. Never trusts diff-run's exit code as a scenario pass.
import { appendFileSync, closeSync, existsSync, openSync, readFileSync, readdirSync, renameSync, statSync, writeFileSync } from "node:fs";
import { join, relative, resolve } from "node:path";
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
const comparisonState=(name:string,directory:string,launch:string,comparisonDirectory=directory)=>{
  const golden=join(root,"results/golden",`arc-1.170-${name}`);
  const g=provenance(join(golden,launch,"record.jsonl")),o=provenance(join(directory,launch,"record.jsonl"));
  const diff=artifact(join(comparisonDirectory,`diff-${launch}.txt`));
  const scope=diff?.text.split("\n").find(line=>line.startsWith("caller attribution:"));
  const stale=diff&&[g.at,o.at].some(at=>at&&at>diff.at);
  return `${launch}: current files golden ${g.label}; ours ${o.label}<br>Saved diff scope: ${escape(scope??(diff?"scope not recorded by older comparator":"no comparison"))}${stale?"<br>STALE: transcript newer than diff":""}`;
};
// Explicitly reviewed fresh-profile standard-harness peers. Never scan arbitrary
// copies or promote their outcomes into golden acceptance. Renderer preserved-
// profile evidence remains separate below.
type Source={directory:string;label:string;comparisonDirectory?:string};
const peers:Record<string,Source[]>={
  "launch-slice":[{directory:"results/extensions-validation7/launch-slice",label:"fresh-profile peer, extensions validation7"},{directory:"logs/wp/profile.batch11/launch-slice-ours156",label:"fresh-profile peer, profile batch11"},{directory:"results/core-runtime/20261023T125302Z/launch-slice",label:"fresh isolated peer, core run6"}],
  "extensions-page-actions":[{directory:"results/extensions-validation7/extensions-page-actions",label:"fresh-profile peer, extensions validation7"}],
  "sessions-public-api":[{directory:"results/sessions/canonical-public-api-run1",label:"fresh isolated peer, sessions owner"}],
  "sessions-tab-restart":[{directory:"results/sessions/canonical-restart-ours-run1",label:"fresh isolated peer, sessions batch7"}],
  "tabs-navigation":[{directory:"results/core-runtime/20261023T125302Z/tabs-navigation",label:"fresh isolated peer, core run6"}],
  "renderer-public-api":[{directory:"results/ui-renderer-public-replay/run4-20261023T123944Z/harness",comparisonDirectory:"results/ui-renderer-public-replay/run4-20261023T123944Z",label:"fresh isolated standard peer, UI run4"}],
};
export function latestObservation(name:string) {
  const sources:Source[]=[{directory:`results/ours/${name}`,label:"diff-run output"},...(peers[name]??[])];
  return sources.flatMap(source=>{
    const directory=join(root,source.directory),stepsArtifact=artifact(join(directory,"steps.json"));
    if(!stepsArtifact)return[];
    let result:any;try{result=JSON.parse(stepsArtifact.text);}catch{return[];}
    if(result.scenario!==name||result.slug!=="ours156"||result.app!==resolve(root,"../arc/build/ArcModAgent-ours156.app")||result.recorded!==true||result.traced!==false||
      !Array.isArray(result.steps)||!result.steps.length||!result.steps.every((s:any)=>["pass","fail","inconclusive"].includes(s.outcome)))return[];
    return[{...source,directory,comparisonDirectory:join(root,source.comparisonDirectory??source.directory),stepsArtifact,result}];
  }).sort((a,b)=>a.stepsArtifact.at.localeCompare(b.stepsArtifact.at)).at(-1);
}
export function refresh() {
  const history = attempts();
  const lines = ["# Scenario scoreboard", "", `Updated ${new Date().toISOString()}. Accepted original-engine goldens only; candidates excluded.`, "",
    "Step passes are the latest completed fresh-profile replacement-engine harness observation, including failed/inconclusive steps in the denominator. Sources are canonical diff-run output and explicitly reviewed peers in scoreboard.ts; each selected source is labeled. Reset provenance: extensions validation7 disposable-data-resets.jsonl; profile batch11 archived batch.sh rename without seed; core run6 frozen runner's per-scenario disposable reset; sessions isolated standard runs (#137813/#138778); UI run4 profile-isolation.json plus empty-directory helper. Standard fields/source/tool hashes and paired captured-run diffs were independently reviewed. Renderer UI run4 stores steps/transcript under harness and its diff in the parent; report.json pins that relationship, with raw SHA independently verified. File dates choose the latest observation, not a claimed exact whole-build source attribution. Recorder labels describe current files; saved diff scope is the historical literal header. Exact first two lines are escaped for Markdown, for every available launch. A newer build failure does not refresh artifacts. Passing steps alone do not establish boundary parity. Missing diffs are explicit. Older preserved-profile renderer evidence remains separate.", "",
    "Overnight desktop state: LOCKED (loginwindow). Native UI and background media failures may be environmental; a difference against an unlocked golden alone does not establish a replacement-only defect. Morning ArcMod scripts remain unrecorded.", "",
    "Recorder migration: v1 lacks caller attribution; v2 records it. The new comparator narrows app-to-engine calls only when both transcripts carry attribution, while retaining callbacks and engine setters. It excludes direct test-driver API calls; those scenarios also require raw transcript gates and sidecar assertions. Compare counts only with the same scope. Older diffs are preserved verbatim and explicitly flagged when a newer golden or transcript makes them stale.", "",
    "| Scenario | Latest ours steps | Latest integrator attempt | Recorder / comparison scope per launch | First two diff lines per launch |", "| --- | --- | --- | --- | --- |"];
  for (const name of acceptedScenarios()) {
    const observation=latestObservation(name),dir=observation?.directory??join(root,"results/ours",name),comparisonDirectory=observation?.comparisonDirectory??dir;
    const stepsArtifact=observation?.stepsArtifact,result=observation?.result;
    const steps = result?.steps as {outcome: string}[] | undefined;
    const pass = steps?.filter(s => s.outcome === "pass").length;
    const stepText = steps ? `${pass}/${steps.length} pass; ${steps.filter(s=>s.outcome === "fail").length} fail, ${steps.filter(s=>s.outcome === "inconclusive").length} inconclusive (${stepsArtifact!.at})<br>[${escape(observation!.label)}](../../${relative(root,dir)}/steps.json)` : "no completed harness artifact";
    const latest = history.filter(a => a.scenario === name).at(-1);
    const attempt = latest ? `${latest.finished ?? latest.started}: ${latest.finished ? `exit ${latest.exit}; inspect log` : "queued/running"} [log](../../${latest.log})` : "not yet attempted by integrator";
    const golden = join(root, "results/golden", `arc-1.170-${name}`);
    const launches = files(golden).filter(n=>/^launch-\d+$/.test(n)).sort();
    const scopes = launches.map(l=>comparisonState(name,dir,l,comparisonDirectory)).join("<br><br>");
    const diffText = launches.map(l => {
      const diff = artifact(join(comparisonDirectory,`diff-${l}.txt`));
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
      diff?`Supplemental diff (${diff.at}), exact first two lines:<br>${diff.text.split("\n").slice(0,2).map(escape).join("<br>")}`:"Supplemental diff missing.",
      `Supplemental comparison state: ${comparisonState("renderer-public-api",supplemental,"launch-1")}.`);
  }
  const sessions=join(root,"results/sessions/canonical-public-api-run1"), sessionSteps=json(join(sessions,"steps.json"))?.steps;
  if(sessionSteps?.length) {
    const diff=artifact(join(sessions,"diff-launch-1.txt")),quit=sessionSteps.find((s:any)=>s.step==="public API agent quit");
    const stoppedAtClose=sessionSteps.some((s:any)=>s.step==="public close produces one synchronous main-thread callback"&&s.outcome!=="pass");
    lines.push("", "Sessions peer detail (Chirp #137813; counted once above if it is the latest fresh-profile observation): the sessions owner replayed the standard scenario in a fresh isolated disposable profile and retained its separate output directory. " +
      `[${sessionSteps.filter((s:any)=>s.outcome==="pass").length}/${sessionSteps.length} steps pass](../../results/sessions/canonical-public-api-run1/steps.json); shutdown ${escape(quit?.outcome??"missing")} (${escape(quit?.detail??"no shutdown artifact")}). `+
      (stoppedAtClose?"The synchronous-close assertion fails before restore is exercised; this is not evidence of a missing restore implementation. ":"Consult the step artifact for the exercised phases. ")+
      "[Raw timing comparison](../../results/sessions/canonical-public-api-run1/close-timing-comparison.json) distinguishes original inline close callbacks from the replacement's deferred callback. Direct driver calls remain outside default v2 diff scope; fixed sidecar/raw gates supply independent proof.", "",
      diff?`Supplemental sessions diff (${diff.at}), exact first two lines:<br>${diff.text.split("\n").slice(0,2).map(escape).join("<br>")}`:"Supplemental sessions diff missing.",
      `Sessions peer comparison state: ${comparisonState("sessions-public-api",sessions,"launch-1")}.`);
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
    const observation=latestObservation(name),dir=observation?.comparisonDirectory??join(root,"results/ours",name),steps=observation?.result.steps;
    if (steps?.length) { measured++; if (steps.every((s: any)=>s.outcome === "pass")) passing++; }
    if (files(dir).some(n=>/^diff-launch-\d+\.txt$/.test(n))) diffs++;
  }
  return `scenarios INTEGRATOR /Users/cryogenicplanet/general/arc-engine: ${names.length} accepted goldens; latest fresh-profile ours artifacts ${measured}/${names.length}, all-steps-pass ${passing}/${names.length}, diff artifacts ${diffs}/${names.length}; ${attempts().filter(a=>a.finished).length} integrator attempts completed. Verified peer sources labeled alongside diff-run outputs; exact lines/dates/build failures: tests/scenarios/SCOREBOARD.md. Shared diff-run lock +>=300s gaps; native recording deferred. No boundary parity claim.`;
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
