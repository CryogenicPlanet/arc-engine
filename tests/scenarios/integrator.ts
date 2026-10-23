// Persistent rotation. One child diff-run at a time; no lock held during the five-minute cooldown.
import { mkdirSync, readFileSync, rmSync, writeFileSync } from "node:fs";
import { join, resolve } from "node:path";
import { attempts, refresh, summary } from "./scoreboard";
const root = resolve(import.meta.dir,"../..");
const lease = join(root,"logs/wp/scenarios-integrator.lock");
const alive = (pid: number) => {
  if (!Number.isSafeInteger(pid) || pid <= 0) return false;
  try { process.kill(pid,0); return true; } catch (error) { if ((error as NodeJS.ErrnoException).code === "ESRCH") return false; throw error; }
};
const activeChild = () => {
  const history = attempts(), finished = new Set(history.filter(a=>a.finished).map(a=>a.started));
  return history.find(a=>!finished.has(a.started) && (alive(a.runnerPid ?? 0) || alive(a.childPid ?? 0)));
};
if (activeChild()) throw new Error("previous integrator run still alive; inspect scenarios-scoreboard.jsonl before restarting");
for (;;) {
  try { mkdirSync(lease); break; } catch (error) {
    if ((error as NodeJS.ErrnoException).code !== "EEXIST") throw error;
    // Give a live process a bounded chance to finish writing its lease PID.
    let pid = 0;
    for (let i=0;i<10;i++) {
      try { pid=Number(readFileSync(join(lease,"pid"),"utf8")); } catch { /* interrupted/new lease */ }
      if (pid > 0) break;
      await Bun.sleep(250);
    }
    if (alive(pid)) throw new Error(`integrator already running: PID ${pid}`);
    let childPid = 0;
    try { childPid=Number(readFileSync(join(lease,"child-pid"),"utf8")); } catch { /* no active child */ }
    if (alive(childPid)) throw new Error(`previous integrator child still running: PID ${childPid}`);
    // A pid-less lease is reclaimable only with no other matching controller.
    const ps = Bun.spawnSync(["ps","-axo","pid=,command="]).stdout.toString();
    const other = ps.split("\n").some(line=>{
      const match=line.trim().match(/^(\d+)\s+(.+)$/);
      return match && Number(match[1]) !== process.pid && /(?:^|\/)bun\s+(?:.*\/)?(?:tests\/scenarios\/)?integrator\.ts(?:\s|$)/.test(match[2]!);
    });
    if (other || activeChild()) throw new Error("another integrator/controller is active; refusing stale lease recovery");
    rmSync(lease,{recursive:true,force:true});
  }
}
writeFileSync(join(lease,"pid"),String(process.pid));
let stop = false;
process.on("SIGTERM",()=>{ stop=true; }); process.on("SIGINT",()=>{ stop=true; });
const option = Bun.argv.indexOf("--chirp-helper");
const helper = option >= 0 ? Bun.argv[option+1] : undefined;
let lastPost = Date.now();
const post = async () => {
  if (!helper || Date.now()-lastPost < 3600000) return;
  const child = Bun.spawn(["python3",helper,"post",summary()],{cwd:root,stdout:"inherit",stderr:"inherit"});
  if (await child.exited === 0) lastPost=Date.now();
};
try {
  while (!stop) {
    const rows = attempts();
    const latest = rows.filter(r=>r.finished).at(-1);
    const next = latest?.finished ? Date.parse(latest.finished)+300000 : Date.now();
    refresh();
    while (!stop && Date.now()<next) {
      await Bun.sleep(Math.min(60000,next-Date.now())); await post();
    }
    if (stop) break;
    const child = Bun.spawn(["bun",join(import.meta.dir,"scoreboard.ts"),"--run-next"],{cwd:root,stdout:"inherit",stderr:"inherit"});
    writeFileSync(join(lease,"child-pid"),String(child.pid));
    // Lock acquisition may take hours; hourly summaries continue independently.
    let done = false;
    const ended = child.exited.then(()=>{ done=true; });
    while (!done) { await Promise.race([ended,Bun.sleep(60000)]); await post(); }
    rmSync(join(lease,"child-pid"),{force:true});
    await post();
  }
} finally { rmSync(lease,{recursive:true,force:true}); }
