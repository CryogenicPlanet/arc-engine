import type { ScenarioContext } from "../launch-slice/run";
import { verifyAgentEndpoint } from "../../../arc/tools/agent-cdp";
import { spawnSync } from "bun";
import { mkdirSync, readFileSync, renameSync, writeFileSync } from "node:fs";
import { join, resolve } from "node:path";
import { page } from "./native";

// Foundation/libc results are native driver sidecars: the boundary recorder only
// instruments ArcCore/app callbacks, so these are not claimed as engine selectors.
type HTTPResult = {mainThread:boolean;errorDomain:string|null;errorCode:number;policyMessageMatches:boolean;hasResponse:boolean;status:number;bytes:number;fixtureControlMatches:boolean};
type NativeState = {started:boolean;dnsDone:boolean;completions:number;completionCounts:Record<string,number>;protocolInstalled:boolean;
  results:{dns?:{blockedHost:string;blockedCode:number;blockedIsEAINONAME:boolean;blockedReturnedNull:boolean;numericControlCode:number;numericControlIsLoopback:boolean};blockedHTTP?:HTTPResult;controlHTTP?:HTTPResult}};
type Reply = {id:string;operation:string;pid:number;nonce:string;main:boolean;ok?:boolean;error?:string;state:NativeState};

export default async function scenario(c: ScenarioContext) {
  const assert=(name:string,pass:unknown,detail="")=>{c.assert(name,pass,detail);if(!pass)throw new Error(name);};
  assert("local services probe uses disposable engine test agent",["trace156","ours156"].includes(c.slug)&&!c.traced);
  const policy=JSON.parse(readFileSync(resolve(import.meta.dir,"../../../arc/resources/local-service-policy.json"),"utf8")) as {native:string[]};
  assert("native service probe host remains explicitly blocked by source policy",policy.native.includes("firestore.googleapis.com"));
  const directory=join(c.outDir,"native-services-driver");mkdirSync(directory,{recursive:true,mode:0o700});
  const dylib=join(directory,"ScenarioNativeServices.dylib");
  const compile=spawnSync(["xcrun","clang","-dynamiclib","-fobjc-arc","-Wall","-Wextra","-Werror","-framework","Foundation",
    join(import.meta.dir,"arcmod-native-services-driver.m"),"-o",dylib],{stdout:"pipe",stderr:"pipe"});
  writeFileSync(join(directory,"compile.log"),compile.stdout.toString()+compile.stderr.toString());
  assert("native services driver compiles with strict warnings",compile.success,compile.stderr.toString().slice(-1500));
  const nonce=`arcmod-services-${crypto.randomUUID()}`,controlURL=`${c.base}/a`;
  const launch=c.launch({libraries:[dylib],environment:{ARC_SCENARIO_SERVICES_DIRECTORY:directory,ARC_SCENARIO_SERVICES_NONCE:nonce,
    ARC_SCENARIO_SERVICES_AGENT:c.slug,ARC_SCENARIO_SERVICES_CONTROL_URL:controlURL}});
  const observations:Reply[]=[];
  try {
    if(!await c.ready(launch,"local services agent launch"))return;
    const pid=launch.pid();assert("native services agent exposes an owned PID",pid!==undefined);
    const owner=()=>{if(launch.pid()!==pid||verifyAgentEndpoint(c.slug,c.port)!==pid)throw new Error("native services agent PID/endpoint changed");};
    const ready=await c.waitFor(()=>{const value=JSON.parse(readFileSync(join(directory,"ready.json"),"utf8")) as {pid?:number;nonce?:string;agent?:string;installed?:boolean};
      return value.pid===pid&&value.nonce===nonce&&value.agent===c.slug&&value.installed===true?value:undefined;},10000);
    assert("native services driver ready in current PID",ready!==undefined);
    let sequence=0;
    const action=async(operation:string)=>{
      owner();const id=`${nonce}-${++sequence}`,request=join(directory,"request.json");
      writeFileSync(request+".tmp",JSON.stringify({id,operation,pid,nonce,url:controlURL}),{mode:0o600});renameSync(request+".tmp",request);
      const reply=await c.waitFor(()=>{const value=JSON.parse(readFileSync(join(directory,"response.json"),"utf8")) as Reply;
        return value.id===id&&value.pid===pid&&value.nonce===nonce&&value.operation===operation?value:undefined;},5000);
      if(!reply)throw new Error(`native services ${operation} timed out; requests will not be replayed`);
      owner();observations.push(reply);writeFileSync(join(directory,"observations.json"),JSON.stringify(observations,null,2));
      if(reply.ok!==true||reply.main!==true)throw new Error(reply.error??"native services request failed");
      return reply.state;
    };
    const tab = await page(c);
    for (const key of ["shouldShareUsageData", "arcMaxAutoOptInEnabled", "tidyTabsEnabled", "instantLinksEnabled", "askOnPageEnabled", "renameDownloadsEnabled"]) {
      const result = spawnSync(["defaults", "read", `company.thebrowser.Browser.mod.agent.${c.slug}`, key]);
      c.assert(`local-mode ${key} disabled`, result.success && result.stdout.toString().trim() === "0");
    }
    let native=await action("start");
    const end=Date.now()+15000;
    while(Date.now()<end&&(!native.dnsDone||native.completions<2)){await c.sleep(100);native=await action("status");}
    assert("native policy HTTP and DNS probes complete once",native.started&&native.dnsDone&&native.completions===2&&native.completionCounts.blockedHTTP===1&&native.completionCounts.controlHTTP===1);
    const dns=native.results.dns,blocked=native.results.blockedHTTP,control=native.results.controlHTTP;
    assert("native DNS blocks policy host with EAI_NONAME and null result",dns?.blockedHost==="firestore.googleapis.com"&&dns.blockedIsEAINONAME&&dns.blockedReturnedNull);
    assert("native DNS numeric loopback control still resolves",dns?.numericControlCode===0&&dns.numericControlIsLoopback);
    assert("native NSURLSession installs ArcMod protocol and returns exact local-policy failure",native.protocolInstalled&&blocked?.mainThread&&blocked.errorDomain==="NSURLErrorDomain"&&blocked.errorCode===-1009&&blocked.policyMessageMatches&&!blocked.hasResponse&&blocked.bytes===0);
    assert("native NSURLSession loopback HTTP200 control still loads fixture",control?.mainThread&&control.errorDomain===null&&control.errorCode===0&&control.hasResponse&&control.status===200&&control.fixtureControlMatches);
    const client = await c.client();
    const failures: { errorText?: unknown; type?: unknown }[] = [];
    const responses: unknown[] = [];
    const blockedRequests = new Set<string>();
    await client.targetCommand(tab, "Network.enable");
    const dispose = client.onEvent(event => {
      if (event.method === "Network.requestWillBeSent" && event.params.type === "Document" &&
        (event.params.request as {url?:string})?.url === "https://api.arc.net/arccore-disposable-probe" && typeof event.params.requestId === "string")
        blockedRequests.add(event.params.requestId);
      if (event.method === "Network.loadingFailed" && typeof event.params.requestId === "string" && blockedRequests.has(event.params.requestId)) failures.push(event.params);
      if (event.method === "Network.responseReceived" && (event.params.response as {url?:string})?.url === "https://api.arc.net/arccore-disposable-probe") responses.push(event.params);
    });
    try {
      // A fixed Arc-owned host listed in resources/local-service-policy.json.
      const result = await client.targetCommand(tab, "Page.navigate", {url:"https://api.arc.net/arccore-disposable-probe"});
      c.assert("Chromium service hostname fails resolution", result.errorText === "net::ERR_NAME_NOT_RESOLVED");
      c.assert("service failure event is DNS rejection", await c.waitFor(() => failures.some(e => e.type === "Document" && e.errorText === "net::ERR_NAME_NOT_RESOLVED") ? true : undefined, 5000));
      c.assert("blocked service receives no HTTP response", responses.length === 0);
      await client.targetCommand(tab, "Page.navigate", {url:`${c.base}/b`});
      c.assert("ordinary loopback browsing survives service rejection", await c.titleIs(tab, c.EXPECT.bTitle));
    } finally { dispose(); }
  } finally {
    writeFileSync(join(directory,"observations.json"),JSON.stringify(observations,null,2));
    await c.quit(launch,"local services agent quit");
  }
}
