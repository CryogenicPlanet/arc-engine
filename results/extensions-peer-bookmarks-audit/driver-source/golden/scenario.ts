import { randomUUID } from "node:crypto";
import { copyFileSync, writeFileSync, readFileSync, existsSync, rmSync } from "node:fs";
import { join, resolve } from "node:path";
export function fixture(req: Request) {
  if (new URL(req.url).pathname === "/arc-profile-public-7f3a")
    return new Response("<!doctype html><title>profile public probe 7f3a</title><p>profile fixture</p>", { headers:{"content-type":"text/html"} });
}
const bookmarkFacts=["added","retainedParentDateUnchanged","freshRootDistinctSameIdentity","freshParentDistinctSameIdentity",
  "freshParentDateWithinAddWindow","freshParentDateAdvanced","leafFolderModifiedNil","leafAddedWithinAddWindow","oldURLInitiallyBookmarked",
  "renamed","edited","retainedLeafOriginalValues","freshLeafDistinctSameIdentity","freshLeafChangedValues",
  "oldURLNotBookmarked","newURLBookmarked","removedUsingOldDTO","bothLeafDTOsInvalid","removedOldDTOOriginalValues",
  "removedFreshDTOChangedValues","parentStillValid","fixtureAbsentFromRetainedParent","newURLNoLongerBookmarked"];
export default async function scenario(c: any) {
  const request=resolve(c.outDir,"profile-probe.request.json"), library=resolve(c.outDir,"profile-probe.dylib");
  const nonce=process.env.ARC_PROFILE_PROBE_NONCE??randomUUID();
  if (!/^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$/.test(nonce)) throw new Error("invalid profile probe UUID nonce");
  for (const suffix of ["",".ready.json",".response.json",".bookmark-dates.json"]) rmSync(request+suffix,{force:true});
  copyFileSync("/tmp/arc-profile-bookmark-probe-v1/probe.dylib",library);
  const launch=c.launch({libraries:[library],environment:{ARC_SCENARIO_PROFILE_PROBE_REQUEST:request}});
  if (await c.ready(launch,"launch")) {
    const ready=await c.waitFor(()=>{
      if (!existsSync(request+".ready.json")) return undefined;
      const value=JSON.parse(readFileSync(request+".ready.json","utf8"));
      return value.installed===true && value.pid===launch.pid() ? true : undefined;
    },10000);
    c.assert("profile probe installed for exact PID",ready);
    if (ready) {
      const url=c.base+"/arc-profile-public-7f3a", target=c.open(url);
      c.assert("fixture page ready",await c.titleIs(target,"profile public probe 7f3a"));
      writeFileSync(request,JSON.stringify({url,nonce,pid:launch.pid()}));
      const response=await c.waitFor(()=>existsSync(request+".response.json") ? JSON.parse(readFileSync(request+".response.json","utf8")) : undefined,30000);
      writeFileSync(join(c.outDir,"profile-probe.observed.json"),JSON.stringify(response??{error:"no profile probe response"},null,2));
      c.assert("public profile API chain completes",response?.complete===true && response?.pid===launch.pid() && response?.nonce===nonce && response?.main===true,JSON.stringify(response));
      c.assert("type29 native default",response?.type29===0);
      c.assert("Unicode local storage roundtrip",response?.storageInitial===null && response?.storageReadback==="Latin＋ε🪐");
      c.assert("cookie write read remove",response?.cookie?.name===`profile_probe_${nonce}` && response?.cookie?.value===`v_${nonce}` && response?.cookie?.sameSite===2 && response?.cookieRemoved===true);
      for (const fact of bookmarkFacts) c.assert(`bookmark ${fact}`,response?.bookmarks?.[fact]===true);
      const values=response?.bookmarkValues;
      c.assert("bookmark retained and fresh Unicode values",values?.uuid===nonce && values?.oldTitle===`Bookmark ε🪐 ${nonce}` && values?.freshTitle===`Renamed 改ε🪐 ${nonce}`
        && values?.oldURL===new URL(`${url}/bookmarks/${nonce}/ε🪐`).href && values?.freshURL===new URL(`${url}/bookmarks/${nonce}/改ε🪐`).href);
      const n=response?.cookieQueryCount;
      c.assert("five or more profile callbacks delivered on main",Number.isInteger(n) && n>=2 && n<=162 && Array.isArray(response?.callbacksOnMain)
        && response.callbacksOnMain.length===3+n && response.callbacksOnMain.every((x:unknown)=>x===true));
      const dates=existsSync(request+".bookmark-dates.json") ? JSON.parse(readFileSync(request+".bookmark-dates.json","utf8")) : undefined;
      c.assert("raw date sidecar matches process nonce and add window",dates?.pid===launch.pid() && dates?.nonce===nonce
        && Number.isFinite(dates?.windowStart) && Number.isFinite(dates?.windowEnd) && dates.windowEnd>=dates.windowStart
        && Number.isFinite(dates?.retainedParentBefore) && Number.isFinite(dates?.retainedParentAfter)
        && dates.retainedParentBefore===dates.retainedParentAfter && Number.isFinite(dates?.freshParentModified)
        && dates.freshParentModified>dates.retainedParentBefore && dates.freshParentModified>=dates.windowStart && dates.freshParentModified<=dates.windowEnd
        && Number.isFinite(dates?.leafAdded) && dates.leafAdded>=dates.windowStart && dates.leafAdded<=dates.windowEnd && dates.leafFolderModified===null);
    }
  }
  await c.quit(launch,"quit");
  const coverage=Bun.spawnSync(["python3","/tmp/arc-profile-bookmark-probe-v1/coverage.py",
    join(c.outDir,"launch-1","record.jsonl"),request+".response.json",request+".bookmark-dates.json",
    join(c.outDir,"bookmark-public-coverage.json")],{stdout:"pipe",stderr:"pipe"});
  c.assert("independent bookmark raw coverage",coverage.success,coverage.success ? "exact mutations, getter returns, five main callbacks, and raw dates verified" : coverage.stdout.toString()+coverage.stderr.toString());
}
