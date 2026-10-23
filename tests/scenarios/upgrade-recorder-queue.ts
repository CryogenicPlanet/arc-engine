// One courtesy-spaced pass over accepted CDP-only goldens; each helper takes the
// common lock separately. Native/morning scripts never enter this queue.
import {appendFileSync,closeSync,existsSync,openSync,readFileSync} from "node:fs";
import {join,resolve} from "node:path";
import {acceptedScenarios} from "./scoreboard";
import {acquireLease} from "../../../arc/tools/agent-lease";
if(!Bun.argv.includes("--lead-recorder-upgrade")) throw new Error("explicit lead recorder upgrade flag required");
const root=resolve(import.meta.dir,"../.."), journal=join(root,"logs/wp/scenarios-recorder-upgrade.jsonl");
const queueJournal=join(root,"logs/wp/scenarios-recorder-upgrade-queue.jsonl");
const ownership=acquireLease("scenarios-recorder-upgrade-queue",join(root,"logs/wp/scenarios-leases"));
// Installed-extension owner requested the next available slot (Chirp #137221).
const names=["launch-slice","extensions-page-actions","launch-slice-regression","tabs-navigation","downloads","pdf","renderer-public-api","media","media-pip","fullscreen","sessions-public-api"];
let stop=false;
process.on("SIGTERM",()=>{stop=true;});process.on("SIGINT",()=>{stop=true;});
const rows=(path=journal):any[]=>existsSync(path)?readFileSync(path,"utf8").split("\n").filter(Boolean).flatMap(line=>{try{return[JSON.parse(line)];}catch{return[];}}):[];
const alive=(pid:number)=>{if(!Number.isSafeInteger(pid)||pid<=0)return false;try{process.kill(pid,0);return true;}catch(error){if((error as NodeJS.ErrnoException).code==="ESRCH")return false;throw error;}};
try {
  const prior=rows(queueJournal),finished=new Set(prior.filter(row=>row.finished).map(row=>row.started));
  if(prior.some(row=>!finished.has(row.started)&&(alive(row.runnerPid??0)||alive(row.childPid??0))))
    throw new Error("prior upgrade runner alive; wait before restarting queue");
  const ps=Bun.spawnSync(["ps","-axo","pid=,command="]).stdout.toString();
  if(ps.split("\n").some(line=>/^\s*\d+\s+(?:(?:\S*\/)?bash\s+\S*locked\.sh\s+)?(?:\S*\/)?bun\s+\S*upgrade-recorder\.ts(?:\s|$)/.test(line)))
    throw new Error("upgrade helper already active; refusing orphan overlap");
  const recovery=prior.some(row=>!finished.has(row.started))?Date.now()+300000:0;
  for(const name of names) {
    if(stop)break;
    const recover=existsSync(join(root,`results/golden/arc-1.170-${name}.migration-previous`));
    if(!acceptedScenarios().includes(name)&&!recover){console.log(`not accepted; skipped ${name}`);continue;}
    const history=rows();
    if(!recover&&history.some(row=>row.scenario===name&&row.phase==="accepted v2")){console.log(`already upgraded; skipped ${name}`);continue;}
    const completions=[...history,...rows(queueJournal)].filter(row=>row.finished).map(row=>Date.parse(row.finished));
    const next=Math.max(completions.length?Math.max(...completions)+300000:Date.now(),recovery);
    while(!stop&&Date.now()<next)await Bun.sleep(Math.min(60000,next-Date.now()));
    if(stop)break;
    const at=new Date().toISOString(),path=`logs/wp/scenarios.recorder-v2.${name}.${at.replaceAll(/[:.]/g,"-")}.log`;
    appendFileSync(queueJournal,JSON.stringify({scenario:name,started:at,log:path})+"\n");
    const fd=openSync(join(root,path),"w");
    let status:number;
    try {
      const child=Bun.spawn(["bun",join(import.meta.dir,"upgrade-recorder.ts"),name,"--lead-recorder-upgrade"],{cwd:root,stdout:fd,stderr:fd});
      appendFileSync(queueJournal,JSON.stringify({scenario:name,started:at,log:path,runnerPid:process.pid,childPid:child.pid})+"\n");
      status=await child.exited;
    } finally {closeSync(fd);}
    const finished=new Date().toISOString();
    appendFileSync(queueJournal,JSON.stringify({scenario:name,started:at,finished,exit:status,log:path})+"\n");
    console.log(`${name}: exit ${status}; ${path}`);
    if(!Bun.spawnSync(["bun",join(import.meta.dir,"scoreboard.ts")],{cwd:root,stdout:"inherit",stderr:"inherit"}).success)
      throw new Error("post-upgrade scoreboard refresh failed");
    // Early helper failures may precede its migration journal. They still earn a
    // full courtesy gap, rather than immediately contending for the lock again.
    const end=Date.parse(finished)+300000;
    while(!stop&&Date.now()<end)await Bun.sleep(Math.min(60000,end-Date.now()));
  }
}finally{ownership.release();}
