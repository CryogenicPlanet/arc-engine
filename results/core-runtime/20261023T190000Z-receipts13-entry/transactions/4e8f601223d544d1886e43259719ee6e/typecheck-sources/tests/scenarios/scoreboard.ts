// Read-only artifact integration. Never trusts diff-run's exit code as a scenario pass.
import { appendFileSync, closeSync, existsSync, openSync, readFileSync, readdirSync, renameSync, statSync, writeFileSync } from "node:fs";
import { join, relative, resolve } from "node:path";
import { createHash } from "node:crypto";
const root = resolve(import.meta.dir, "../..");
const journal = join(root, "logs/wp/scenarios-scoreboard.jsonl");
type Attempt = { scenario: string; started: string; finished?: string; exit?: number; log: string; runnerPid?: number; childPid?: number; diffSHA256?: string; recorderSHA256?: string; comparatorHelpersSHA256?: Record<string, string | null> };
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
const comparisonState=(name:string,directory:string,launch:string,comparisonDirectory=directory,diffPrefix="diff-")=>{
  const golden=join(root,"results/golden",`arc-1.170-${name}`);
  const g=provenance(join(golden,launch,"record.jsonl")),o=provenance(join(directory,launch,"record.jsonl"));
  const diff=artifact(join(comparisonDirectory,`${diffPrefix}${launch}.txt`));
  const scope=diff?.text.split("\n").find(line=>line.startsWith("caller attribution:"));
  const stale=diff&&[g.at,o.at].some(at=>at&&at>diff.at);
  return `${launch}: current files golden ${g.label}; ours ${o.label}<br>Saved diff scope: ${escape(scope??(diff?"scope not recorded by older comparator":"no comparison"))}${stale?"<br>STALE: transcript newer than diff":""}`;
};
// Explicitly reviewed fresh-profile standard-harness peers. Never scan arbitrary
// copies or promote their outcomes into golden acceptance. Renderer preserved-
// profile evidence remains separate below.
type Source={directory:string;label:string;comparisonDirectory?:string;diffPrefix?:string};
const peers:Record<string,Source[]>={
  "launch-slice":[{directory:"results/extensions-validation7/launch-slice",label:"fresh-profile peer, extensions validation7"},{directory:"logs/wp/profile.batch11/launch-slice-ours156",label:"fresh-profile peer, profile batch11"},{directory:"results/core-runtime/20261023T125302Z/launch-slice",label:"fresh isolated peer, core run6"},{directory:"results/core-runtime/20261023T142843Z-window-binding/launch-slice",label:"fresh isolated standard peer, core run7"},{directory:"results/core-runtime/20261023T145945Z-quit-suppression/launch-slice",label:"fresh isolated standard peer, core run8"},{"directory":"logs/wp/app-boundary-validation/run2s-gcd/launch-slice","comparisonDirectory":"logs/wp/app-boundary-validation/run2s-gcd","diffPrefix":"launch-slice-diff-","label":"fresh isolated standard peer, app GCD run2s"},{"directory":"results/core-runtime/20261023T172900Z-seven-integration11/candidate/launch-slice","label":"fresh isolated standard peer, core integration11"},{directory:"logs/wp/profile.batch17/launch-slice",label:"fresh isolated standard peer, profile batch17"}],
  "extensions-page-actions":[{directory:"results/extensions-validation7/extensions-page-actions",label:"fresh-profile peer, extensions validation7"},{"directory":"results/extensions-validation15-full-arc-freeze/extensions-page-actions","label":"fresh isolated standard peer, extensions integration15"}],
  "sessions-public-api":[{directory:"results/sessions/canonical-public-api-run1",label:"fresh isolated peer, sessions owner"}],
  "sessions-tab-restart":[{directory:"results/sessions/canonical-restart-ours-run1",label:"fresh isolated peer, sessions batch7"}],
  "renderer-boost-lifecycle":[{directory:"results/renderer-boost-document-start-hook-run4",label:"fresh isolated standard peer, renderer cache/document-start run4"}],
  "history-public-api":[{directory:"results/renderer-history-after-boost-run4",label:"fresh isolated standard peer, renderer cache/history run4"}],
  "downloads":[{directory:"logs/wp/profile.batch17/downloads",label:"fresh isolated standard peer, profile batch17"}],
  "windows":[{directory:"results/core-runtime/20261023T174200Z-windows/windows",label:"captured standard failure peer, core Windows run11"}],
  "tabs-navigation":[{directory:"results/core-runtime/20261023T125302Z/tabs-navigation",label:"fresh isolated peer, core run6"},{directory:"results/core-runtime/20261023T142843Z-window-binding/tabs-navigation",label:"fresh isolated standard peer, core run7"},{directory:"results/core-runtime/20261023T145945Z-quit-suppression/tabs-navigation",label:"fresh isolated standard peer, core run8"},{"directory":"results/core-runtime/20261023T172900Z-seven-integration11/candidate/tabs-navigation","label":"fresh isolated standard peer, core integration11"}],
  "dialogs":[{directory:"logs/wp/app-boundary-validation/run2m/dialogs-runtime",comparisonDirectory:"logs/wp/app-boundary-validation/run2m",diffPrefix:"dialogs-diff-",label:"fresh isolated standard peer, app run2m"}],
  "renderer-public-api":[{directory:"results/ui-renderer-public-replay/run4-20261023T123944Z/harness",comparisonDirectory:"results/ui-renderer-public-replay/run4-20261023T123944Z",label:"fresh isolated standard peer, UI run4"},{directory:"results/renderer-queued-rules-canonical-run2",label:"fresh isolated standard peer, renderer queued-rules run2"}],
};
export function latestObservation(name:string) {
  const sources:Source[]=[{directory:`results/ours/${name}`,label:"diff-run output"},...(peers[name]??[])];
  return sources.flatMap(source=>{
    const directory=join(root,source.directory),stepsArtifact=artifact(join(directory,"steps.json"));
    if(!stepsArtifact)return[];
    let result:any;try{result=JSON.parse(stepsArtifact.text);}catch{return[];}
    if(result.scenario!==name||result.slug!=="ours156"||result.app!==resolve(root,"../arc/build/ArcModAgent-ours156.app")||result.recorded!==true||result.traced!==false||
      !Array.isArray(result.steps)||!result.steps.length||!result.steps.every((s:any)=>["pass","fail","inconclusive"].includes(s.outcome)))return[];
    return[{...source,directory,comparisonDirectory:join(root,source.comparisonDirectory??source.directory),diffPrefix:source.diffPrefix??"diff-",stepsArtifact,result}];
  }).sort((a,b)=>a.stepsArtifact.at.localeCompare(b.stepsArtifact.at)).at(-1);
}
export function refresh() {
  const history = attempts();
  const lines = ["# Scenario scoreboard", "", `Updated ${new Date().toISOString()}. Accepted original-engine goldens only; candidates excluded.`, "",
    "Step passes are the latest completed fresh-profile replacement-engine harness observation, including failed/inconclusive steps in the denominator. Sources are canonical diff-run output and explicitly reviewed peers in scoreboard.ts; each selected source is labeled. Reset provenance: extensions validation7 disposable-data-resets.jsonl; profile batch11 archived batch.sh rename without seed; core run6/run7/run8 frozen runners' per-scenario disposable resets; app run2m reset with no prior directory or seed; sessions isolated standard runs (#137813/#138778); UI run4 profile-isolation.json plus empty-directory helper. Renderer queued-rules run2 has a reviewed fresh empty-profile reset/restore, sealed harness and phase inventories; its captured source seals do not identify the current whole build. Standard harness fields, reviewed source/tool seals and paired captured-run diffs were independently checked; peer evidence does not identify the current whole build. Renderer UI run4 stores steps/transcript under harness and its diff in the parent; report.json pins that relationship, with raw SHA independently verified. App run2m stores dialogs steps/transcript under dialogs-runtime and its reviewed dialogs-diff-launch-N files in the parent; that explicit prefix is used for headers, scope and counts. File dates choose the latest observation, not a claimed exact whole-build source attribution. Recorder labels describe current files; saved diff scope is the historical literal header. Exact first two lines are escaped for Markdown, for every available launch. A newer build failure does not refresh artifacts. Passing steps alone do not establish boundary parity. Missing diffs are explicit. Older preserved-profile renderer evidence remains separate.", "",
    "Overnight desktop state: LOCKED (loginwindow). Native UI and background media failures may be environmental; a difference against an unlocked golden alone does not establish a replacement-only defect. Morning ArcMod scripts remain unrecorded.", "",
    "Recorder migration: v1 lacks caller attribution; v2 records it. The new comparator narrows app-to-engine calls only when both transcripts carry attribution, while retaining callbacks and engine setters. It excludes direct test-driver API calls; those scenarios also require raw transcript gates and sidecar assertions. Compare counts only with the same scope. Older diffs are preserved verbatim and explicitly flagged when a newer golden or transcript makes them stale.", "",
    "Comparator versions also affect counts when caller scope is unchanged. Per-attempt comparator and recorder SHA256 values are saved at start/end in logs/wp/scenarios-scoreboard.jsonl when available. Future attempts also save all three fixed comparator-helper hashes (download_identity.py, google_q.py, omnibox_receivers.py), with missing files explicit null; older journals lack these helper pins. These endpoint hashes do not prove files stayed unchanged throughout a run. Reviewed peer versions remain in their retained receipts. Compare counts only with matching tools and scope; policy/tool changes are not engine-fix evidence.", "",
    "Additional reviewed captures: core integration11 supplies launch/tabs from its later-adopted transaction candidate directory; archived selected48/Arc122/native42/runtime56 inputs are bounded inventories, not a whole build seal. App GCD run2s uses parent-directory launch-slice-diff- files and a diagnostic timing preload. Extensions integration15 uses a fresh reset and matching canonical source/tool pins; it lacks a full packaged-bundle interval seal and historical pins for four extension-fixture files. Core Windows run11 is a failed standard capture with a runner-source disposable reset, no independent profile-tree reset receipt and missing final selected/native/Arc/runtime seals; all 50 AX snapshots have zero AXWindow roles. Its captured package differs from current. Renderer run4 supplies isolated Boost64/64 and history61/61 with unchanged fixed API gates; native originals remain58/58 and55/55. Boost totals vary with polling; history replaces one absent-profile row with seven fresh-directory checks. Source236/framework297/package1145 are endpoint inventories; three comparator dependencies are covered by release pins outside the runtime capsule, with hash-matched current source copies distinguished from historical archives. Profile batch17 supplies later standard launch/downloads captures with274 matching declared input endpoints and framework/frontend hashes. Disposable reset uses frozen rename/backup logic without an independent profile-tree receipt; inherited environment and complete runtime inventories are not sealed, and the additional profile download raw audit is separate from these functional observations. These are captured observations with retained review limits, not current whole-build certification.", "",
    "| Scenario | Latest ours steps | Latest integrator attempt | Recorder / comparison scope per launch | First two diff lines per launch |", "| --- | --- | --- | --- | --- |"];
  for (const name of acceptedScenarios()) {
    const observation=latestObservation(name),dir=observation?.directory??join(root,"results/ours",name),comparisonDirectory=observation?.comparisonDirectory??dir,diffPrefix=observation?.diffPrefix??"diff-";
    const stepsArtifact=observation?.stepsArtifact,result=observation?.result;
    const steps = result?.steps as {outcome: string}[] | undefined;
    const pass = steps?.filter(s => s.outcome === "pass").length;
    const stepText = steps ? `${pass}/${steps.length} pass; ${steps.filter(s=>s.outcome === "fail").length} fail, ${steps.filter(s=>s.outcome === "inconclusive").length} inconclusive (${stepsArtifact!.at})<br>[${escape(observation!.label)}](../../${relative(root,dir)}/steps.json)` : "no completed harness artifact";
    const latest = history.filter(a => a.scenario === name).at(-1);
    const attempt = latest ? `${latest.finished ?? latest.started}: ${latest.finished ? `exit ${latest.exit}; inspect log` : "queued/running"} [log](../../${latest.log})` : "not yet attempted by integrator";
    const golden = join(root, "results/golden", `arc-1.170-${name}`);
    const launches = files(golden).filter(n=>/^launch-\d+$/.test(n)).sort();
    const scopes = launches.map(l=>comparisonState(name,dir,l,comparisonDirectory,diffPrefix)).join("<br><br>");
    const diffText = launches.map(l => {
      const diff = artifact(join(comparisonDirectory,`${diffPrefix}${l}.txt`));
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
    const observation=latestObservation(name),dir=observation?.comparisonDirectory??join(root,"results/ours",name),steps=observation?.result.steps,diffPrefix=observation?.diffPrefix??"diff-";
    if (steps?.length) { measured++; if (steps.every((s: any)=>s.outcome === "pass")) passing++; }
    if (files(dir).some(n=>n.startsWith(diffPrefix)&&/^launch-\d+\.txt$/.test(n.slice(diffPrefix.length)))) diffs++;
  }
  return `scenarios INTEGRATOR /Users/cryogenicplanet/general/arc-engine: ${names.length} accepted goldens; latest fresh-profile ours artifacts ${measured}/${names.length}, all-steps-pass ${passing}/${names.length}, diff artifacts ${diffs}/${names.length}; ${attempts().filter(a=>a.finished).length} integrator attempts completed. Verified peer sources labeled alongside diff-run outputs; exact lines/dates/build failures: tests/scenarios/SCOREBOARD.md. Shared diff-run lock +>=300s gaps; native recording deferred. No boundary parity claim.`;
}
if (import.meta.main) {
  if(Bun.argv.includes("--summary")) { console.log(summary()); process.exit(0); }
  if (Bun.argv.includes("--run-next")) {
    const hash=(path:string)=>{try{return createHash("sha256").update(readFileSync(join(root,path))).digest("hex");}catch{return undefined;}};
    const toolHashes=()=>({diffSHA256:hash("tests/recorder/diff.py"),recorderSHA256:hash("tests/recorder/ArcEngineRecorder.dylib"),comparatorHelpersSHA256:Object.fromEntries(["download_identity.py","google_q.py","omnibox_receivers.py"].map(name=>[name,hash(`tests/recorder/${name}`)??null]))});
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
