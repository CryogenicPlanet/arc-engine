// Explicit lead-granted recorder-v2 migration (Chirp #136425); ordinary recording
// still refuses immutable launch-slice. Never replaces an all-pass v1 with a failed run.
import { copyFileSync, cpSync, existsSync, lstatSync, readFileSync, readdirSync, renameSync, rmSync, writeFileSync, appendFileSync } from "node:fs";
import { createHash } from "node:crypto";
import { join, resolve } from "node:path";
import { acceptedScenarios } from "./scoreboard";
const root=resolve(import.meta.dir,"../.."), name=Bun.argv[2];
const allowed=new Set(["launch-slice","launch-slice-regression","tabs-navigation","downloads","pdf","media","media-pip","fullscreen","extensions-page-actions","renderer-public-api","sessions-public-api"]);
if (!name || !allowed.has(name) || !Bun.argv.includes("--lead-recorder-upgrade"))
  throw new Error("usage: upgrade-recorder.ts <accepted-CDP-scenario> --lead-recorder-upgrade [--locked]");
if (!Bun.argv.includes("--locked")) {
  const child=Bun.spawn([join(root,"arc/tools/locked.sh"),"bun",import.meta.path,name,"--lead-recorder-upgrade","--locked",...(Bun.argv.includes("--force")?["--force"]:[])],{cwd:root,stdout:"inherit",stderr:"inherit"});
  process.exitCode=await child.exited;
} else {
  const parent=Number(readFileSync(join(root,"logs/arccore-build.lock/pid"),"utf8"));
  let ancestor=process.pid;
  for (let depth=0;ancestor!==parent && ancestor>1 && depth<20;depth++)
    ancestor=Number(Bun.spawnSync(["ps","-p",String(ancestor),"-o","ppid="],{stdout:"pipe",stderr:"pipe"}).stdout.toString().trim());
  if (ancestor!==parent) throw new Error("recorder migration requires real shared-lock ancestry");
  const app=resolve(root,"../arc/build/ArcModAgent-trace156.app"), exe=join(app,"Contents/MacOS/Arc");
  const expression="^"+exe.replace(/[.*+?^${}()|[\]\\]/g,"\\$&")+"( |$)";
  if (Bun.spawnSync(["pgrep","-f",expression],{stdout:"pipe",stderr:"pipe"}).success) throw new Error("trace156 running; refusing profile reset");
  const golden=join(root,`results/golden/arc-1.170-${name}`);
  const stage=golden+".recorder-v2.candidate", snapshot=golden+".recorder-v1";
  const journal=join(root,"logs/wp/scenarios-recorder-upgrade.jsonl");
  const sha=(path:string)=>createHash("sha256").update(readFileSync(path)).digest("hex");
  const launches=(path:string)=>readdirSync(path).filter(n=>/^launch-\d+$/.test(n)).sort();
  const callerAware=(path:string)=>{
    try {
      const rows=readFileSync(path,"utf8").trim().split("\n").map(line=>JSON.parse(line));
      const calls=rows.filter(row=>row.ev==="call");
      return rows.some(row=>row.ev==="hooked" && row.engine_classes>0) && calls.length>0 &&
        calls.every(row=>typeof row.from==="string" && row.from.trim().length>0) &&
        calls.some(row=>row.dir==="->engine" && row.from==="app");
    } catch { return false; }
  };
  type Manifest=Record<string,string>;
  const manifest=(directory:string,exclude=""):Manifest=>{
    const result:Manifest={};
    const walk=(relative:string)=>{
      for(const file of readdirSync(join(directory,relative)).sort()) {
        const item=join(relative,file); if(item===exclude) continue;
        const stat=lstatSync(join(directory,item));
        if(stat.isDirectory()) walk(item);
        else if(stat.isFile()) result[item]=sha(join(directory,item));
        else throw new Error(`snapshot refuses non-regular artifact: ${item}`);
      }
    };
    walk(""); return result;
  };
  const equalManifest=(a:Manifest,b:Manifest)=>JSON.stringify(Object.entries(a).sort())===JSON.stringify(Object.entries(b).sort());
  const previous=golden+".migration-previous";
  // Recover before acceptedScenarios(): a process death between renames can hide canonical.
  if(existsSync(previous)) {
    if(!existsSync(golden)) renameSync(previous,golden);
    else {
      const marker=JSON.parse(readFileSync(join(golden,"recorder-upgrade.json"),"utf8"));
      if(marker.leadMessage!==136425 || !marker.artifacts || !equalManifest(marker.artifacts,manifest(golden,"recorder-upgrade.json")))
        throw new Error("unfinished promotion: canonical integrity unproven; both versions retained");
      for(const [launch,hash] of Object.entries(marker.originalHashes as Manifest)) {
        const old=join(previous,launch,"record.v1.jsonl");
        if(sha(existsSync(old)?old:join(previous,launch,"record.jsonl"))!==hash || sha(join(golden,launch,"record.v1.jsonl"))!==hash)
          throw new Error("unfinished promotion: v1 preservation unproven; both versions retained");
      }
      rmSync(previous,{recursive:true});
    }
  }
  if (!acceptedScenarios().includes(name)) throw new Error("only accepted canonical goldens can be upgraded");
  const oldLaunches=launches(golden);
  if (oldLaunches.length>0 && oldLaunches.every(launch=>callerAware(join(golden,launch,"record.jsonl"))) && !Bun.argv.includes("--force")) {
    console.log(`already caller-aware: ${name}; no rerecord needed`); process.exit(0);
  }
  const originalHashes=Object.fromEntries(oldLaunches.map(launch=>{
    const saved=join(golden,launch,"record.v1.jsonl");
    return [launch,sha(existsSync(saved)?saved:join(golden,launch,"record.jsonl"))];
  }));
  const start=new Date().toISOString();
  const log=(value:Record<string,unknown>)=>appendFileSync(journal,JSON.stringify({scenario:name,started:start,...value})+"\n");
  // Full first-version snapshot is write-once. Preserve app logs/steps/coverage as well as raw records.
  const manifestName="snapshot-manifest.json";
  if (!existsSync(snapshot)) {
    if(existsSync(join(golden,manifestName))) throw new Error("reserved snapshot manifest exists in canonical");
    const original=manifest(golden), partial=snapshot+`.partial-${crypto.randomUUID()}`;
    cpSync(golden,partial,{recursive:true,errorOnExist:true,force:false});
    if(!equalManifest(original,manifest(partial))) throw new Error(`incomplete preservation copy retained: ${partial}`);
    writeFileSync(join(partial,manifestName),JSON.stringify({leadMessage:136425,files:original},null,2));
    renameSync(partial,snapshot);
  }
  const savedManifest=JSON.parse(readFileSync(join(snapshot,manifestName),"utf8"));
  if(savedManifest.leadMessage!==136425 || !savedManifest.files || !equalManifest(savedManifest.files,manifest(snapshot,manifestName)))
    throw new Error("v1 full snapshot missing or corrupt; canonical retained");
  for (const launch of oldLaunches) {
    const saved=join(snapshot,launch,"record.v1.jsonl"), first=existsSync(saved)?saved:join(snapshot,launch,"record.jsonl");
    if (sha(first)!==originalHashes[launch]) throw new Error(`v1 snapshot drift: ${launch}`);
  }
  const recorderHash=sha(join(root,"tests/recorder/ArcEngineRecorder.dylib"));
  log({phase:"recording",originalHashes,recorderHash});
  rmSync(stage,{recursive:true,force:true});
  rmSync(join(process.env.HOME!,"Library/Application Support/ArcModAgent-trace156"),{recursive:true,force:true});
  const child=Bun.spawn(["bun",join(root,"tests/launch-slice/run.ts"),"--app",app,"--out",stage,"--record","--scenario",name],{cwd:root,stdout:"inherit",stderr:"inherit"});
  const code=await child.exited;
  let valid=false;
  try {
    const coverage=JSON.parse(readFileSync(join(stage,"coverage.json"),"utf8"));
    const steps=JSON.parse(readFileSync(join(stage,"steps.json"),"utf8"));
    valid=code===0 && coverage.accepted===true && steps.steps.length>0 && steps.steps.every((step:{outcome:string})=>step.outcome==="pass") && launches(stage).join()===oldLaunches.join() && recorderHash===sha(join(root,"tests/recorder/ArcEngineRecorder.dylib"));
    for (const launch of oldLaunches) {
      valid &&= callerAware(join(stage,launch,"record.jsonl"));
    }
  } catch { valid=false; }
  if (!valid) {
    log({finished:new Date().toISOString(),phase:"not accepted",exit:code,candidate:stage,originalHashes});
    console.error("v2 candidate not accepted; canonical v1 and full snapshot retained"); process.exitCode=1;
  } else {
    for (const launch of oldLaunches) {
      const old=join(golden,launch,"record.v1.jsonl");
      copyFileSync(existsSync(old)?old:join(golden,launch,"record.jsonl"),join(stage,launch,"record.v1.jsonl"));
      if (sha(join(stage,launch,"record.v1.jsonl"))!==originalHashes[launch]) throw new Error("v1 copy verification failed");
    }
    writeFileSync(join(stage,"recorder-upgrade.json"),JSON.stringify({leadMessage:136425,upgraded:new Date().toISOString(),originalHashes,recorderHash,artifacts:manifest(stage,"recorder-upgrade.json")},null,2));
    if (existsSync(previous)) throw new Error("unfinished migration backup exists; reconcile manually");
    renameSync(golden,previous);
    try { renameSync(stage,golden); } catch (error) { renameSync(previous,golden); throw error; }
    rmSync(previous,{recursive:true});
    log({finished:new Date().toISOString(),phase:"accepted v2",exit:0,originalHashes});
    console.log(`accepted recorder-v2 golden: ${golden}; original bytes retained as record.v1.jsonl + ${snapshot}`);
  }
}
