// Bookmark-only adaptation of logs/wp/profile.batch11/bookmark-fixture/scenario.ts
// SHA256 6e20ad06230b2bcf302e40763536756a9f0fe2a98769daff69b5578d3811fdc5.
// Frozen combined fixture had35 passing steps on both engines; this narrower
// scenario requires its own original recording. It excludes storage/cookie APIs.
import type { ScenarioContext } from "../launch-slice/run";
import { verifyAgentEndpoint } from "../../../arc/tools/agent-cdp";
import { existsSync, mkdirSync, readFileSync, readdirSync, renameSync, writeFileSync } from "node:fs";
import { join } from "node:path";
import { validateBookmarkTranscript } from "./bookmarks-public-api-raw";
const PATH="/arc-bookmarks-public-7f3a",IMAGE="BookmarksPublicAPIDriver.dylib";
const FACTS=["added","retainedParentDateUnchanged","freshRootDistinctSameIdentity","freshParentDistinctSameIdentity","freshParentDateWithinAddWindow","freshParentDateAdvanced","leafFolderModifiedNil","leafAddedWithinAddWindow","oldURLInitiallyBookmarked","renamed","edited","retainedLeafOriginalValues","freshLeafDistinctSameIdentity","freshLeafChangedValues","oldURLNotBookmarked","newURLBookmarked","removedUsingOldDTO","bothLeafDTOsInvalid","removedOldDTOOriginalValues","removedFreshDTOChangedValues","parentStillValid","fixtureAbsentFromRetainedParent","newURLNoLongerBookmarked"];
type Response={id:string;pid:number;nonce:string;agent:string;url:string;main:boolean;complete:boolean;error?:string;recorderAliasesBefore:boolean;recorderAliasesAfter:boolean;bookmarks:Record<string,boolean>;bookmarkValues:{uuid:string;oldTitle:string;freshTitle:string;oldURL:string;freshURL:string}};
type Dates={pid:number;nonce:string;leafUUID:string;parentUUID:string;parentID:string;windowStart:number;windowEnd:number;retainedParentBefore:number;retainedParentAfter:number;freshParentModified:number;leafAdded:number;leafFolderModified:null};
export const fixture=(request:Request)=>new URL(request.url).pathname===PATH?new Response("<!doctype html><title>Bookmark public fixture 7f3a</title><p>Disposable bookmark fixture</p>",{headers:{"content-type":"text/html; charset=utf-8","cache-control":"no-store"}}):undefined;
export default async function scenario(c:ScenarioContext){
  const assert=(name:string,ok:unknown,detail="")=>{c.assert(name,ok,detail);if(!ok)throw new Error(name+(detail?": "+detail:""));};
  assert("bookmark scenario requires standard caller-aware recording",c.recorded&&!c.traced);
  assert("bookmark scenario uses exact disposable engine agent",["trace156","ours156"].includes(c.slug));
  assert("bookmark scenario starts with empty disposable browser storage",["User Data","Agent Chromium Data"].every(name=>!existsSync(join(c.data,name))||readdirSync(join(c.data,name)).length===0));
  const directory=join(c.outDir,"bookmarks-public-api-driver");mkdirSync(directory,{recursive:true,mode:0o700});
  const request=join(directory,"request.json"),library=join(directory,IMAGE),nonce=crypto.randomUUID(),url=c.base+PATH;
  const compile=Bun.spawnSync(["xcrun","clang","-dynamiclib","-fobjc-arc","-Wall","-Wextra","-Werror","-framework","Foundation","-framework","AppKit",join(import.meta.dir,"bookmarks-public-api-driver.m"),"-o",library],{stdout:"pipe",stderr:"pipe"});
  writeFileSync(join(directory,"compile.log"),compile.stdout.toString()+compile.stderr.toString());assert("bookmark driver compiles with strict warnings",compile.success,compile.stderr.toString().slice(-1500));
  writeFileSync(join(directory,"provenance.json"),JSON.stringify({source:"logs/wp/profile.batch11/bookmark-fixture/probe.mm",sha256:"9ba51ecf20030ab5c370e25c4050ed83feb25181eaad7105a6693839094d0505",scenario:"logs/wp/profile.batch11/bookmark-fixture/scenario.ts",scenarioSHA256:"6e20ad06230b2bcf302e40763536756a9f0fe2a98769daff69b5578d3811fdc5",gate:"logs/wp/profile.batch11/bookmark-fixture/coverage.py",gateSHA256:"03707c596db55f628f7fc63f599b1236385285d3ca37828e768eae540e6b6a8a",scope:"Bookmark-only public API: immutable scalar DTO snapshots, live identity/mutations, Unicode values and raw add-call date window. Standard v2 diff excludes driver-origin calls; raw gate separately requires exact74 bookmark calls, matching per-receiver returns/order and genuine create callback. No import/observer/undo/relaunch coverage."},null,2));
  const launch=c.launch({libraries:[library],environment:{ARC_SCENARIO_BOOKMARKS_DIRECTORY:directory,ARC_SCENARIO_BOOKMARKS_NONCE:nonce,ARC_SCENARIO_BOOKMARKS_AGENT:c.slug,ARC_SCENARIO_BOOKMARKS_URL:url}});
  try{
    if(!await c.ready(launch,"bookmark public agent launch"))return;
    const pid=launch.pid();assert("bookmark launch has owned actual PID",pid!==undefined);
    const owner=()=>{if(launch.pid()!==pid||verifyAgentEndpoint(c.slug,c.port)!==pid)throw new Error("bookmark PID/endpoint ownership changed");};
    const ready=await c.waitFor(()=>{const value=JSON.parse(readFileSync(join(directory,"ready.json"),"utf8")) as {pid:number;nonce:string;agent:string;installed:boolean;recorderAliases:boolean};return value.pid===pid&&value.nonce===nonce&&value.agent===c.slug&&value.installed===true&&value.recorderAliases===true?value:undefined;},10000);
    assert("bookmark driver ready with exact identity and recorder forwarding",ready!==undefined);owner();
    const target=c.open(url);assert("bookmark fixture page loads exact title",await c.titleIs(target,"Bookmark public fixture 7f3a"));owner();
    const id=`bookmarks-${nonce}`;writeFileSync(request+".tmp",JSON.stringify({id,operation:"run",url,nonce,pid}),{mode:0o600});renameSync(request+".tmp",request);
    // One mutation chain only; read-only observation retries never replay it.
    const response=await c.waitFor(()=>{const value=JSON.parse(readFileSync(request+".response.json","utf8")) as Response;return value.id===id&&value.pid===pid&&value.nonce===nonce&&value.agent===c.slug&&value.url===url?value:undefined;},10000);
    owner();assert("public bookmark chain completes on main through canonical forwarding",response?.complete===true&&response.main===true&&response.recorderAliasesBefore===true&&response.recorderAliasesAfter===true&&!response.error,response?.error??(response?"exact owned response observed":"response missing"));
    for(const fact of FACTS)assert("bookmark "+fact,response!.bookmarks?.[fact]===true);
    assert("bookmark facts have exact schema",Object.keys(response!.bookmarks).sort().join("\n")===[...FACTS].sort().join("\n"));
    const values=response!.bookmarkValues;
    assert("retained and fresh Unicode scalar values remain exact",values?.uuid===nonce&&values.oldTitle===`Bookmark ε🪐 ${nonce}`&&values.freshTitle===`Renamed 改ε🪐 ${nonce}`&&values.oldURL===new URL(`${url}/bookmarks/${nonce}/ε🪐`).href&&values.freshURL===new URL(`${url}/bookmarks/${nonce}/改ε🪐`).href);
    const dates=JSON.parse(readFileSync(request+".bookmark-dates.json","utf8")) as Dates;
    const finite=(value:unknown):value is number=>typeof value==="number"&&Number.isFinite(value);
    assert("raw date sidecar retains exact process UUID and parent identity",dates.pid===pid&&dates.nonce===nonce&&dates.leafUUID===nonce&&typeof dates.parentUUID==="string"&&dates.parentUUID.length>0&&dates.parentID==="1");
    assert("raw add-call dates are finite ordered wallclock values",[dates.windowStart,dates.windowEnd,dates.retainedParentBefore,dates.retainedParentAfter,dates.freshParentModified,dates.leafAdded].every(finite)&&dates.windowEnd>=dates.windowStart);
    assert("retained parent date stays frozen while fresh native date advances within add window",dates.retainedParentBefore===dates.retainedParentAfter&&dates.freshParentModified>dates.retainedParentBefore&&dates.freshParentModified>=dates.windowStart&&dates.freshParentModified<=dates.windowEnd);
    assert("native leaf creation date lies in add window and leaf folder date is nil",dates.leafAdded>=dates.windowStart&&dates.leafAdded<=dates.windowEnd&&dates.leafFolderModified===null);
  }finally{await c.quit(launch,"bookmark public agent graceful quit");}
  const gate=validateBookmarkTranscript(join(c.outDir,"launch-1","record.jsonl"),IMAGE,nonce,url);writeFileSync(join(directory,"raw-gate.json"),JSON.stringify(gate,null,2));assert("raw recorder proves exact driver API calls returns receiver order and genuine callback",true);
}
