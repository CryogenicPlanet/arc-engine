// Persistent rotation. One child diff-run at a time; no lock held during the five-minute cooldown.
import { mkdirSync, readFileSync, renameSync, rmSync, writeFileSync } from "node:fs";
import { join, resolve } from "node:path";
import { attempts } from "./scoreboard";
import { acquireLease } from "../../../arc/tools/agent-lease";
const root = resolve(import.meta.dir,"../..");
const lease = join(root,"logs/wp/scenarios-integrator.lock");
// Reuse the established kernel flock helper. The persistent inode must never be unlinked.
const ownership = acquireLease("scenarios-integrator",join(root,"logs/wp/scenarios-leases"));
const alive = (pid: number) => {
  if (!Number.isSafeInteger(pid) || pid <= 0) return false;
  try { process.kill(pid,0); return true; } catch (error) { if ((error as NodeJS.ErrnoException).code === "ESRCH") return false; throw error; }
};
const activeChild = () => {
  const history = attempts(), finished = new Set(history.filter(a=>a.finished).map(a=>a.started));
  return history.find(a=>!finished.has(a.started) && (alive(a.runnerPid ?? 0) || alive(a.childPid ?? 0)));
};
if (activeChild()) throw new Error("previous integrator run still alive; inspect scenarios-scoreboard.jsonl before restarting");
// Cover a parent crash immediately after spawn, before its child PID journal write.
const ps = Bun.spawnSync(["ps","-axo","pid=,command="]).stdout.toString();
const otherRunner = ps.split("\n").some(line=>{
  const match=line.trim().match(/^(\d+)\s+(.+)$/);
  return match && Number(match[1]) !== process.pid && /(?:^|\/)bun\s+(?:.*\/)?(?:tests\/scenarios\/)?scoreboard\.ts\s+--run-next(?:\s|$)/.test(match[2]!);
});
if (otherRunner) throw new Error("previous scoreboard runner still active; refusing overlapping rotation");
const history = attempts(), finished = new Set(history.filter(a=>a.finished).map(a=>a.started));
const recoveryCooldown = history.some(a=>!finished.has(a.started)) ? Date.now()+300000 : 0;
mkdirSync(lease,{recursive:true});
writeFileSync(join(lease,"pid"),String(process.pid));
let stop = false;
process.on("SIGTERM",()=>{ stop=true; }); process.on("SIGINT",()=>{ stop=true; });
const option = Bun.argv.indexOf("--chirp-helper");
const helper = option >= 0 ? Bun.argv[option+1] : undefined;
const postState = join(root,"logs/wp/scenarios-scoreboard-posted.json");
// Render with the current reporting source even when the overnight controller
// remains alive across a lead integration. This never starts a comparison.
const refresh = () => {
  const result=Bun.spawnSync(["bun",join(import.meta.dir,"scoreboard.ts")],{cwd:root,stdout:"inherit",stderr:"inherit"});
  if(!result.success) throw new Error("scoreboard refresh failed");
};
const summary = () => {
  const result=Bun.spawnSync(["bun",join(import.meta.dir,"scoreboard.ts"),"--summary"],{cwd:root,stdout:"pipe",stderr:"inherit"});
  if(!result.success) throw new Error("scoreboard summary failed");
  return result.stdout.toString().trim();
};
let lastPost = 0;
try { const saved=JSON.parse(readFileSync(postState,"utf8")); if (Number.isFinite(saved.postedAt)) lastPost=saved.postedAt; } catch { /* first startup */ }
const post = async () => {
  if (!helper || Date.now()-lastPost < 3600000) return;
  refresh();
  const child = Bun.spawn(["python3",helper,"post",summary()],{cwd:root,stdout:"inherit",stderr:"inherit"});
  if (await child.exited === 0) {
    lastPost=Date.now();
    const temporary=`${postState}.${process.pid}.tmp`;
    writeFileSync(temporary,JSON.stringify({postedAt:lastPost})); renameSync(temporary,postState);
  }
};
try {
  while (!stop) {
    const rows = attempts();
    const latest = rows.filter(r=>r.finished).at(-1);
    const next = Math.max(latest?.finished ? Date.parse(latest.finished)+300000 : Date.now(),recoveryCooldown);
    refresh();
    await post();
    while (!stop && Date.now()<next) {
      await Bun.sleep(Math.min(60000,next-Date.now())); refresh(); await post();
    }
    if (stop) break;
    const child = Bun.spawn(["bun",join(import.meta.dir,"scoreboard.ts"),"--run-next"],{cwd:root,stdout:"inherit",stderr:"inherit"});
    writeFileSync(join(lease,"child-pid"),String(child.pid));
    // Lock acquisition may take hours; hourly summaries continue independently.
    let done = false;
    const ended = child.exited.then(()=>{ done=true; });
    while (!done) { await Promise.race([ended,Bun.sleep(60000)]); refresh(); await post(); }
    rmSync(join(lease,"child-pid"),{force:true});
    await post();
  }
} finally { rmSync(lease,{recursive:true,force:true}); ownership.release(); }
