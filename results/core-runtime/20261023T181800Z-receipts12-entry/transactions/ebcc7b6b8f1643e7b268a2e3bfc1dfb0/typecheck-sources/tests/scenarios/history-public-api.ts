// WRITE/static candidate only. Original recording is held until the owner posts
// applied host-only history DCHECK tolerance plus Debug9/9. Release is canonical judge.
// Public history-only fixture: no cookies/storage/type29/native input/private DTO ABI.
import type {ScenarioContext} from "../launch-slice/run";
import {verifyAgentEndpoint,type CDPResult} from "../../../arc/tools/agent-cdp";
import {createHash} from "node:crypto";
import {lstatSync,mkdirSync,readFileSync,readdirSync,realpathSync,renameSync,writeFileSync} from "node:fs";
import {join,resolve} from "node:path";
import {validateHistoryPublicRaw} from "./history-public-api.raw";
const PATH="/history-public-api-7f3a/fixture",IMAGE="HistoryPublicAPIDriver.dylib",ORACLE="logs/wp/profile.batch13-release-history",OBS_SHA="e519e695b4a0f506b8398437028c038a2d0aae44dee0696cd63ed1fb60b833cd",RAW_SHA="ee55ff3b998d0174aaf399b53e60d9c87bb36c4806d6a15b0f24bd0dfa8022cc";
export const fixture=(request:Request)=>new URL(request.url).pathname===PATH?new Response("<!doctype html><title>History public fixture 7f3a</title><script>window.__arcHistoryDocumentNonce=crypto.randomUUID()</script><p>Disposable public history fixture</p>",{headers:{"content-type":"text/html; charset=utf-8","cache-control":"no-store"}}):undefined;
const ORACLE_FILES={
  "probe.mm": "20216b0c8772991087f0f93567aef132a8af9c7527468f5f12dd43c85cde58e5",
  "run-diff.py": "0732f260dd8d3c76cd078d9ff20b4661474fea2ad0de9cb87aba177a03283f9f",
  "media-run.ts": "3fe7958134c467416127cfb2dbacc25236401a7ecc07f59908b82357e6d9b2b9",
  "scenario.ts": "d1463537203207acb6794157e5faa1697efe41c8c444503ece5513886d2c6dc3",
  "coverage.py": "cac3bee7f0307e2c48add1db4cc4cd4de21b1cd62c2aa838c562f82a4f75b217",
  "observed-compare.py": "ac4cf314b012d3865d0b61568e3fd5ddd4f956e8659a175563e84e35afbeca4f",
  "ArcEngineRecorder.dylib": "fee95e1339e8c1aa2ecf1fcb0e9ebf1137efe64193031b60ead9c110677bba9a",
  "diff.py": "f8df579e6ea23b4eb531f357fdfe0460fa57b563daf44f9d6858c73abf84b184",
  "ArcEngineRecorder.m": "9ae39687fdebcf4115b8c974af1b799aeee65265185a2c512ff56e5ca287d81e",
  "run.ts": "998acbaf535a9bb05c0bc6f756cfcf7db14788a0507bae82f78659a003c2b5fe",
  "probe.dylib": "6db81bc286f674bc6db2ccc0f129dc4bac26f9354ade756dbb80b678f3a7808f",
  "app_callbacks.inc": "52b3687bd7f574acb34f8c3c5fcf6750e0fc044c9bf159a6971531a952830748"
} as const;
type Callback={stage:number;main:boolean;thread:string;insideCall:boolean;count:number;countRows?:number;urls?:string[];status?:number};
type Deletion={index:number;main:boolean;thread:string;isFromExpiration:boolean;isAllHistory:boolean;contextDirectory:string;deletedURLs?:string[]};
type Reply={id:string;pid:number;nonce:string;agent:string;url:string;complete:boolean;step:number;forwardingBefore:boolean;forwardingAfter:boolean;callbacks:Callback[];deletions:Deletion[];historyItem:{url:string;urlSpec:string;title:string;visitCount:number;typedCount:number;hasID:boolean;hasLastVisitTime:boolean;hasVisitTime:boolean};error?:string};
const readJSON=<T>(path:string):T|undefined=>{try{return JSON.parse(readFileSync(path,"utf8")) as T;}catch(error){if((error as NodeJS.ErrnoException).code==="ENOENT")return;throw error;}};
const sha=(path:string)=>createHash("sha256").update(readFileSync(path)).digest("hex");
export default async function scenario(c:ScenarioContext){
  const assert=(name:string,ok:unknown,detail="")=>{c.assert(name,ok,detail);if(!ok)throw new Error(name+(detail?": "+detail:""));};
  assert("History public API requires caller-aware standard disposable recording",c.recorded&&!c.traced&&["trace156","ours156"].includes(c.slug));
  const workspace=resolve(import.meta.dir,"../.."),app=resolve(workspace,`../arc/build/ArcModAgent-${c.slug}.app`),exe=join(app,"Contents/MacOS/Arc");
  assert("History public API exact canonical agent app",c.app===app&&realpathSync(app)===app);
  for(const [file,expected] of Object.entries(ORACLE_FILES))assert("History pins frozen oracle "+file,sha(join(workspace,ORACLE,"fixture",file))===expected);assert("History pins original observation and raw",sha(join(workspace,ORACLE,"trace156/profile-probe.observed.json"))===OBS_SHA&&sha(join(workspace,ORACLE,"trace156/launch-1/record.jsonl"))===RAW_SHA);
  const host=new URL(c.base).hostname;assert("History public API uses literal loopback host without resolver changes",host==="127.0.0.1"&&new URL(c.base).protocol==="http:");
  const info=(path:string)=>{try{return lstatSync(path);}catch(error){if((error as NodeJS.ErrnoException).code==="ENOENT")return;throw error;}};
  const support=join(process.env.HOME!,"Library/Application Support"),anchor=realpathSync(support),profile=join(support,`ArcModAgent-${c.slug}`);assert("History public API exact disposable profile path",c.data===profile);
  let profileIdentity:{dev:number;ino:number}|undefined;
  const inspectProfile=(required:boolean,fresh=false)=>{assert("History public API profile anchor unchanged",realpathSync(support)===anchor);const st=info(profile);if(!st){assert("History public API only uncreated profile may be absent",!required&&!profileIdentity);return;}
    assert("History public API no-follow canonical profile directory",!st.isSymbolicLink()&&st.isDirectory()&&realpathSync(profile)===join(anchor,`ArcModAgent-${c.slug}`));
    if(fresh)assert("History public API only fresh storage may preexist",readdirSync(profile).every(n=>["User Data","Agent Chromium Data"].includes(n)));
    for(const n of ["User Data","Agent Chromium Data"]){const path=join(profile,n),child=info(path);assert("History public API storage children no-follow",child===undefined||(!child.isSymbolicLink()&&child.isDirectory()));if(fresh)assert("History public API storage starts empty",child===undefined||readdirSync(path).length===0);}
    const after=lstatSync(profile);assert("History public API profile identity retained",!after.isSymbolicLink()&&after.dev===st.dev&&after.ino===st.ino&&(!profileIdentity||(profileIdentity.dev===st.dev&&profileIdentity.ino===st.ino)));return {dev:st.dev,ino:st.ino};};
  profileIdentity=inspectProfile(false,true);
  const root=realpathSync(c.outDir);assert("History public API output is no-follow directory",!lstatSync(c.outDir).isSymbolicLink()&&lstatSync(c.outDir).isDirectory());
  const directory=join(root,"history-public-api-driver");assert("History public API evidence is fresh",info(directory)===undefined);mkdirSync(directory,{mode:0o700});const dstat=lstatSync(directory);
  const scoped=()=>{const st=lstatSync(directory);if(st.isSymbolicLink()||!st.isDirectory()||realpathSync(directory)!==directory||st.dev!==dstat.dev||st.ino!==dstat.ino||(st.mode&0o077)!==0)throw new Error("History evidence directory changed/exposed");};
  const library=join(directory,IMAGE),source=join(import.meta.dir,"history-public-api.m"),compile=Bun.spawnSync(["xcrun","clang","-dynamiclib","-fobjc-arc","-Wall","-Wextra","-Werror","-framework","Foundation",source,"-o",library],{stdout:"pipe",stderr:"pipe"});writeFileSync(join(directory,"compile.log"),compile.stdout.toString()+compile.stderr.toString(),{mode:0o600});assert("History public API driver compiles strictly",compile.success,compile.stderr.toString().slice(-1500));
  const nonce=crypto.randomUUID(),url=c.base+PATH,mainURL=c.base+"/history-public-api-7f3a/history/"+nonce,sentinelURL=mainURL+"-sentinel",librarySHA=sha(library),metadata=c.bundleIdentity();
  assert("History fixture page does not match private query nonce",!url.includes(nonce));
  writeFileSync(join(directory,"provenance.json"),JSON.stringify({oracle:ORACLE,oracleFiles:ORACLE_FILES,oracleObservationSHA256:OBS_SHA,oracleRawSHA256:RAW_SHA,sourceSHA256:sha(source),librarySHA,metadata,scope:"History-only exact2→sentinel1→0, statuses0/0/1, public scalar snapshots. Completion thread/inline facts are sidecar observations, no universal queue guarantee. Public NSDate presence and nonempty opaque historyID only; no numeric epoch/ID parity/private DTO access. Direct driver calls need separate raw gate. New recording held until confirmed host-only Debug tolerance APPLIED+9/9; Release is canonical judge."},null,2),{mode:0o600});
  const launch=c.launch({libraries:[library],environment:{ARC_SCENARIO_HISTORY_ROOT:root,ARC_SCENARIO_HISTORY_DIRECTORY:directory,ARC_SCENARIO_HISTORY_APP:app,ARC_SCENARIO_HISTORY_LIBRARY:library,ARC_SCENARIO_HISTORY_NONCE:nonce,ARC_SCENARIO_HISTORY_AGENT:c.slug,ARC_SCENARIO_HISTORY_URL:url}});
  let ownedPID:number|undefined;
  try{
    assert("History public agent ready",await c.ready(launch,"History public agent launch"));const pid=launch.pid();ownedPID=pid;assert("History owns actual PID",pid!==undefined);
    const command=(args:string[])=>{const r=Bun.spawnSync(args,{stdout:"pipe",stderr:"pipe",timeout:3000});if(!r.success||r.stderr.length)throw new Error("History identity observation failed");return r.stdout.toString().trim();};
    const identity=()=>{if(command(["/bin/ps","-ww","-p",String(pid),"-o","comm="])!==exe)throw new Error("History executable identity changed");const value=command(["/bin/ps","-p",String(pid),"-o","lstart="]);if(!value)throw new Error("History process start missing");return value;},start=identity();
    const owner=()=>{scoped();if(launch.pid()!==pid||identity()!==start||verifyAgentEndpoint(c.slug,c.port)!==pid)throw new Error("History PID/start/endpoint changed");};
    const poll=async<T>(read:()=>T|undefined,timeout=10000)=>{const end=Date.now()+timeout;while(Date.now()<end){owner();const value=read();owner();if(value!==undefined)return value;await c.sleep(150);}return;};
    const owned=<T extends {pid:number;nonce:string;agent:string;url:string}>(v:T)=>{if(v.pid!==pid||v.nonce!==nonce||v.agent!==c.slug||v.url!==url)throw new Error("History sidecar identity mismatch");return v;};
    const ready=await poll(()=>{const v=readJSON<{pid:number;nonce:string;agent:string;url:string;installed:boolean}>(join(directory,"ready.json"));return v?owned(v):undefined;});assert("History genuine callback aliases ready",ready?.installed===true);profileIdentity=inspectProfile(true);
    const client=await c.client(),before=await client.request("Target.getTargets");owner();const prior=before.targetInfos as {targetId:string;url:string}[];assert("History fixture absent before post-ready target creation",Array.isArray(prior)&&prior.every(t=>t.url!==url));const created=await client.request("Target.createTarget",{url,background:true});owner();assert("History fresh target creation owns exact new ID",typeof created.targetId==="string"&&prior.every(t=>t.targetId!==created.targetId));const target=created.targetId as string;
    const observe=async()=>{owner();try{const result=await client.targetCommand(target,"Runtime.evaluate",{expression:`(() => {if(typeof document==='undefined'||document.readyState!=='complete')return null;return {url:location.href,title:document.title,documentNonce:window.__arcHistoryDocumentNonce};})()`,returnByValue:true});owner();if(result.exceptionDetails!==undefined)throw new Error("History DOM observation exception "+JSON.stringify(result.exceptionDetails));return (result.result as CDPResult)?.value as {url:string;title:string;documentNonce:string}|null;}catch(error){owner();if(error instanceof Error&&error.message.startsWith("CDP error: ")){let e:{code?:unknown;message?:unknown};try{e=JSON.parse(error.message.slice(11));}catch{throw error;}if(e.code===-32000&&typeof e.message==="string"&&/^(?:Execution context was destroyed(?:, most likely because of a navigation)?|Cannot find (?:default execution context|context with specified id))\.?$/.test(e.message))return;}throw error;}};
    let document:Awaited<ReturnType<typeof observe>>;const end=Date.now()+10000;while(Date.now()<end){document=await observe();if(document?.url===url&&document.title==="History public fixture 7f3a"&&/^[a-f0-9-]{36}$/.test(document.documentNonce))break;await c.sleep(150);}assert("History exact fresh fixture document loaded",document?.url===url&&document.title==="History public fixture 7f3a"&&/^[a-f0-9-]{36}$/.test(document.documentNonce));const documentNonce=document!.documentNonce;
    const id="history-"+nonce,request=join(directory,"request.json");owner();writeFileSync(request+".tmp",JSON.stringify({id,operation:"run",pid,nonce,url}),{mode:0o600,flag:"wx"});renameSync(request+".tmp",request);
    const response=await poll(()=>{const v=readJSON<Reply>(join(directory,"response.json"));if(!v)return;owned(v);if(v.error)throw new Error(v.error);if(v.id!==id)throw new Error("History response request ID differs");return v.complete?v:undefined;},35000);
    assert("History one public chain completes with canonical forwarding",response?.complete===true&&response.forwardingBefore===true&&response.forwardingAfter===true&&response.step===6&&!response.error);const r=response!;
    assert("History exactly six completion sidecars with typed actual scheduling",Array.isArray(r.callbacks)&&r.callbacks.length===6&&r.callbacks.every((v,i)=>v.stage===i&&v.count===1&&typeof v.main==="boolean"&&typeof v.insideCall==="boolean"&&typeof v.thread==="string"&&/^[1-9][0-9]*$/.test(v.thread)));
    const same=(a:unknown,b:unknown)=>JSON.stringify(a)===JSON.stringify(b),initial=r.callbacks[0]!,remaining=r.callbacks[2]!,final=r.callbacks[4]!;
    assert("History fixed exact two added URLs",initial.countRows===2&&Array.isArray(initial.urls)&&same([...initial.urls].sort(),[mainURL,sentinelURL].sort()));
    const item=r.historyItem;assert("History fixed public Unicode title URL counts and NSDate presence",item?.url===mainURL&&item.urlSpec===mainURL&&item.title==="Profile ε🪐 "+nonce&&item.visitCount===1&&item.typedCount===0&&item.hasID===true&&item.hasLastVisitTime===true&&item.hasVisitTime===true);
    assert("History scoped expiry preserves exactly sentinel",r.callbacks[1]?.status===0&&remaining.countRows===1&&same(remaining.urls,[sentinelURL]));assert("History sentinel delete and final empty search",r.callbacks[3]?.status===0&&final.countRows===0&&same(final.urls,[]));assert("History invalid relative URL fixed failure status",r.callbacks[5]?.status===1);
    assert("History exactly two genuine deletion receipts with typed scheduling",Array.isArray(r.deletions)&&r.deletions.length===2&&r.deletions.every((v,i)=>v.index===i&&typeof v.main==="boolean"&&typeof v.thread==="string"&&/^[1-9][0-9]*$/.test(v.thread)&&v.contextDirectory==="Default"&&v.isFromExpiration===false&&v.isAllHistory===(i===0)));
    assert("History native allHistory expiry metadata retained despite sentinel survival",r.deletions[0]!.isAllHistory===true&&remaining.countRows===1);assert("History delete notification contains exact sentinel",same(r.deletions[1]!.deletedURLs,[sentinelURL]));
    const after=await observe();assert("History public chain preserves exact actual fixture document",after?.url===url&&after.title==="History public fixture 7f3a"&&after.documentNonce===documentNonce);owner();inspectProfile(true);assert("History driver bytes retained",sha(library)===librarySHA);writeFileSync(join(directory,"observed.json"),JSON.stringify({target,pid,start,documentNonce,response},null,2),{mode:0o600});
  }finally{await c.quit(launch,"History public standard graceful quit");}
  const final=readJSON<Reply>(join(directory,"response.json"));assert("History no late/duplicate callback failure through graceful shutdown",final!==undefined&&final.pid===ownedPID&&final.nonce===nonce&&final.complete===true&&!final.error&&final.callbacks.length===6&&final.deletions.length===2);
  const gate=validateHistoryPublicRaw(join(root,"launch-1/record.jsonl"),{image:IMAGE,url,nonce});writeFileSync(join(directory,"raw-gate.json"),JSON.stringify(gate,null,2),{mode:0o600});assert("History raw exact public calls values pairing context and genuine callbacks",true);
}
