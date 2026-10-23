// WRITE-only. NOT ready until the lead's complete original type13 factory/IPC
// path audit and independent safety review close. Never record while locked.
// Public API import; no source discovery, import wizard, private ABI or ADK
// delegate construction. Own protocol callback observations are SIDECARS, not
// canonical ADK callback rows, block instrumentation or boundary parity proof.
import type { ScenarioContext } from "../launch-slice/run";
import { verifyAgentEndpoint } from "../../../arc/tools/agent-cdp";
import { createHash } from "node:crypto";
import { chmodSync, closeSync, constants, fstatSync, lstatSync, mkdirSync, openSync, readFileSync, readSync, readdirSync, realpathSync, renameSync, writeFileSync } from "node:fs";
import { isAbsolute, join, relative, resolve } from "node:path";
import { validateDisposableImportRaw } from "./arcmod-disposable-import-raw";

const PATH="/arc-disposable-import-7f3a",IMAGE="ScenarioDisposableImportDriver.dylib";
// Original tables: VA0xc9d8168 bytes020304000000060708090a0b0c maps
// native13->public12; VA0xc9d8128 ushort table maps category bit1->mask2.
const ORIGINAL_SHA="b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d";
export const importRouteReceipt="logs/wp/scenarios-import-route-approved.json";
export const fixtureBookmarkHTML=`<!DOCTYPE NETSCAPE-Bookmark-file-1>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=UTF-8">
<TITLE>ArcCore disposable import</TITLE>
<H1>ArcCore disposable import</H1>
<DL><p>
<DT><H3>ArcCore Fixture Folder</H3>
<DL><p>
<DT><A HREF="http://127.0.0.1:8765/arccore-import-fixture" ADD_DATE="1700000000">ArcCore Imported Bookmark 7f3a</A>
</DL><p>
</DL><p>
`;
export const morningPlan=[
  "Mandatory prelaunch: lead-approved complete type13 route receipt, original framework hash and evidence hashes; no discovery applies only to this adapter route.",
  "Typed bundle/profile guard each launch; one fixture-only directory/HTML; create distinct stage fixture after driver readiness and bind regular Default context.",
  "Create two dedicated live bookmark target folders; explicit Safari-export descriptor public type12/mask2; one bookmark-only import call.",
  "Require own delegate sidecar initialize/start/category2-start/category2-end(nil)/end(nil), exact scalar profiles and no lock/session/extra callbacks.",
  "Require exactly one fixed folder and fixed title/URL leaf under exactly one target, the other empty; retain dates without claiming date conversion parity.",
  "Standard graceful quit, same-profile/new-PID restart and fixed read-only persistence; gate real public calls independently of sidecars.",
];
export const fixture=(request:Request)=>{
  const match=new URL(request.url).pathname.match(/^\/arc-disposable-import-7f3a\/([12])$/);if(!match)return;
  return new Response(`<!doctype html><title>Disposable import fixture 7f3a stage${match[1]}</title><p id="import-stage-marker">ArcCore import stage${match[1]}</p>`,{headers:{"content-type":"text/html; charset=utf-8","cache-control":"no-store"}});
};
type Profile={name:string;profileName:string;sourcePath:string;appPath:string;type:number;supportedCategories:number;constraints:number};
type Event={ordinal:number;kind:string;category:number|null;error:null|{domain:string;code:number;description:string};mainThread:boolean;thread:string;profile:Profile|null};
type Tree={id:string;uuid:string;parentID:string;title:string;url:string|null;folder:boolean;valid:boolean;added:number|null;children:Tree[]};
type Reply={id:string;operation:string;pid:number;nonce:string;agent:string;url:string;stage:number;complete:boolean;error?:string;main:boolean;recorderAliasesBefore:boolean;recorderAliasesAfter:boolean;importAttempts:number;accepted:boolean;immediateError:unknown;profile?:Profile;events:Event[];targets?:{bar:Tree;other:Tree};callbackScope:string};
const readJSON=<T>(path:string):T|undefined=>{try{return JSON.parse(readFileSync(path,"utf8")) as T;}catch(error){if((error as NodeJS.ErrnoException).code==="ENOENT")return;throw error;}};
const sha=(bytes:Buffer)=>createHash("sha256").update(bytes).digest("hex");

export default async function scenario(c:ScenarioContext){
  const assert=(name:string,ok:unknown,detail="")=>{c.assert(name,ok,detail);if(!ok)throw new Error(name+(detail?": "+detail:""));};
  assert("disposable import requires caller-aware standard agent recording",c.recorded&&!c.traced&&["trace156","ours156"].includes(c.slug));
  const workspace=resolve(import.meta.dir,"../.."),app=resolve(workspace,`../arc/build/ArcModAgent-${c.slug}.app`);
  assert("disposable import exact canonical agent app",c.app===app&&realpathSync(app)===app);
  const info=(path:string)=>{try{return lstatSync(path);}catch(error){if((error as NodeJS.ErrnoException).code==="ENOENT")return;throw error;}};
  const regular=(path:string)=>{const st=lstatSync(path);assert("import provenance/evidence is a no-follow regular file",!st.isSymbolicLink()&&st.isFile(),path);return readFileSync(path);};
  // This receipt authorizes only the explicit test-owned source path/category.
  // It says nothing about ordinary app startup discovery or ADK callback rows.
  const receiptPath=join(workspace,importRouteReceipt);
  const receipt=JSON.parse(regular(receiptPath).toString("utf8")) as {schema:string;scope:string;routeReviewClosed:boolean;frameworkSHA256:string;nativeSourceType:number;publicSourceType:number;categoryMask:number;safeExplicitFixtureDirectory:boolean;noDiscovery:boolean;evidence:{path:string;sha256:string}[]};
  assert("import requires reviewed explicit fixture route receipt before launch",receipt.schema==="arc-disposable-import-route-v1"&&receipt.scope==="explicit-test-owned-import-route"&&receipt.routeReviewClosed===true&&receipt.frameworkSHA256===ORIGINAL_SHA&&receipt.nativeSourceType===13&&receipt.publicSourceType===12&&receipt.categoryMask===2&&receipt.safeExplicitFixtureDirectory===true&&receipt.noDiscovery===true&&Array.isArray(receipt.evidence)&&receipt.evidence.length>0);
  const original=resolve(workspace,"../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore");
  const originalInfo=lstatSync(original);assert("import original framework is a no-follow regular file",!originalInfo.isSymbolicLink()&&originalInfo.isFile());
  const fd=openSync(original,constants.O_RDONLY|constants.O_NOFOLLOW),digest=createHash("sha256"),chunk=Buffer.alloc(1024*1024);try{const opened=fstatSync(fd);assert("import original framework opened exact pinned inode",opened.dev===originalInfo.dev&&opened.ino===originalInfo.ino);let count:number;while((count=readSync(fd,chunk,0,chunk.length,null))>0)digest.update(chunk.subarray(0,count));const after=fstatSync(fd);assert("import original framework remains unchanged while hashing",after.dev===opened.dev&&after.ino===opened.ino&&after.size===opened.size&&after.mtimeMs===opened.mtimeMs);}finally{closeSync(fd);}
  assert("import route original framework hash matches on either engine",digest.digest("hex")===ORIGINAL_SHA);
  const seen=new Set<string>();
  for(const evidence of receipt.evidence){
    const path=resolve(workspace,evidence.path),child=relative(workspace,path);
    assert("import route evidence stays canonical in checkout",typeof evidence.path==="string"&&!isAbsolute(evidence.path)&&child!==""&&child!==".."&&!child.startsWith("../")&&realpathSync(path)===path&&!seen.has(path)&&/^[a-f0-9]{64}$/.test(evidence.sha256));seen.add(path);
    assert("import route evidence hash matches reviewed bytes",sha(regular(path))===evidence.sha256,evidence.path);
  }
  const support=join(process.env.HOME!,"Library/Application Support"),supportCanonical=realpathSync(support),profile=join(support,`ArcModAgent-${c.slug}`);
  assert("import uses exact disposable profile path",c.data===profile);
  let profileIdentity:{dev:number;ino:number}|undefined;
  const inspectProfile=(required:boolean,fresh=false)=>{
    assert("import canonical profile anchor unchanged",realpathSync(support)===supportCanonical);
    const st=info(profile);if(!st){assert("import only uncreated disposable profile may be absent",!required&&!profileIdentity);return;}
    assert("import profile is exact no-follow directory",!st.isSymbolicLink()&&st.isDirectory()&&realpathSync(profile)===join(supportCanonical,`ArcModAgent-${c.slug}`));
    if(fresh)assert("import profile contains only empty disposable storage",readdirSync(profile).every(n=>["User Data","Agent Chromium Data"].includes(n)));
    for(const n of ["User Data","Agent Chromium Data"]){const path=join(profile,n),child=info(path);assert("import storage child is no-follow directory or absent",child===undefined||(!child.isSymbolicLink()&&child.isDirectory()));if(fresh)assert("import storage initially empty",child===undefined||readdirSync(path).length===0);}
    const after=lstatSync(profile);assert("import retains exact profile inode",!after.isSymbolicLink()&&after.dev===st.dev&&after.ino===st.ino&&(!profileIdentity||(st.dev===profileIdentity.dev&&st.ino===profileIdentity.ino)));return {dev:st.dev,ino:st.ino};
  };
  profileIdentity=inspectProfile(false,true);
  const root=realpathSync(c.outDir);assert("import output itself has no final symlink",!lstatSync(c.outDir).isSymbolicLink()&&lstatSync(c.outDir).isDirectory());
  const directory=join(root,"disposable-import-driver"),source=join(directory,"fixture-source");
  assert("import owned output directory is absent",info(directory)===undefined);mkdirSync(directory,{mode:0o700});mkdirSync(source,{mode:0o700});chmodSync(directory,0o700);chmodSync(source,0o700);
  const html=fixtureBookmarkHTML.replace("http://127.0.0.1:8765",c.base),sourceFile=join(source,"import-bookmarks.html");writeFileSync(sourceFile,html,{mode:0o600,flag:"wx"});
  const dirIdentity=lstatSync(directory),sourceIdentity=lstatSync(source),htmlIdentity=lstatSync(sourceFile),htmlSHA=sha(Buffer.from(html));
  const inspectSource=()=>{
    for(const [path,id] of [[directory,dirIdentity],[source,sourceIdentity]] as const){const st=lstatSync(path);assert("import source/output retains private canonical directory identity",!st.isSymbolicLink()&&st.isDirectory()&&st.dev===id.dev&&st.ino===id.ino&&(st.mode&0o077)===0&&realpathSync(path)===path);}
    const st=lstatSync(sourceFile);assert("import source is exactly one unchanged regular HTML file",readdirSync(source).join("\n")==="import-bookmarks.html"&&!st.isSymbolicLink()&&st.isFile()&&st.dev===htmlIdentity.dev&&st.ino===htmlIdentity.ino&&sha(readFileSync(sourceFile))===htmlSHA);
  };
  const nonce=crypto.randomUUID(),barUUID=crypto.randomUUID(),otherUUID=crypto.randomUUID(),leafURL=c.base+"/arccore-import-fixture",name="ArcCore Disposable Safari Export",profileName="ArcCore Fixture Source";
  const expectedProfile:Profile={name,profileName,sourcePath:source,appPath:"",type:12,supportedCategories:2,constraints:0};
  const profileMatches=(value:Profile|undefined|null)=>value!==undefined&&value!==null&&Object.keys(value).length===Object.keys(expectedProfile).length&&(Object.keys(expectedProfile) as (keyof Profile)[]).every(key=>value[key]===expectedProfile[key]);
  const library=join(directory,IMAGE),compile=Bun.spawnSync(["xcrun","clang","-dynamiclib","-fobjc-arc","-Wall","-Wextra","-Werror","-framework","Foundation","-framework","AppKit",join(import.meta.dir,"arcmod-disposable-import-driver.m"),"-o",library],{stdout:"pipe",stderr:"pipe"});
  writeFileSync(join(directory,"compile.log"),compile.stdout.toString()+compile.stderr.toString(),{mode:0o600});assert("import driver compiles with strict warnings",compile.success,compile.stderr.toString().slice(-1500));
  const librarySHA=sha(regular(library));writeFileSync(join(directory,"provenance.json"),JSON.stringify({receiptPath,receipt,originalFrameworkSHA256:ORIGINAL_SHA,htmlSHA,librarySHA,scope:"Public bookmark-only import plus own public-protocol callback SIDEcars; no canonical ADK callback coverage, date conversion parity, general startup discovery or full boundary parity claim.",runtimeUnverified:true},null,2),{mode:0o600});
  const command=(args:string[])=>{const r=Bun.spawnSync(args,{stdout:"pipe",stderr:"pipe",timeout:3000});if(!r.success||r.stderr.length)throw new Error("import process identity observation failed");return r.stdout.toString().trim();};
  const identity=(pid:number)=>{const exe=command(["/bin/ps","-ww","-p",String(pid),"-o","comm="]),start=command(["/bin/ps","-p",String(pid),"-o","lstart="]);if(exe!==join(app,"Contents/MacOS/Arc")||!start)throw new Error("import exact PID/executable/start required");return start;};
  const checkTree=(reply:Reply)=>{
    const targets=reply.targets;assert("import returns two exact live dedicated targets",targets!==undefined&&targets.bar.uuid===barUUID&&targets.other.uuid===otherUUID&&targets.bar.id!==targets.other.id&&[targets.bar,targets.other].every(n=>n.valid&&n.folder&&n.parentID==="1"));
    const trees=[targets!.bar,targets!.other],populated=trees.filter(n=>n.children.length!==0);assert("import populates exactly one target and leaves other empty",populated.length===1&&populated[0]!.children.length===1);
    const folder=populated[0]!.children[0]!,leaf=folder.children[0];assert("import has exact folder and sole fixed URL/title leaf",folder.valid&&folder.folder&&folder.title==="ArcCore Fixture Folder"&&folder.url===null&&folder.parentID===populated[0]!.id&&folder.children.length===1&&leaf!==undefined&&leaf.valid&&!leaf.folder&&leaf.title==="ArcCore Imported Bookmark 7f3a"&&leaf.url===leafURL&&leaf.parentID===folder.id&&leaf.children.length===0);
    return {destination:populated[0]!.uuid,folderID:folder.id,folderUUID:folder.uuid,leafID:leaf!.id,leafUUID:leaf!.uuid};
  };
  let firstPID:number|undefined,firstTarget:string|undefined,firstTree:ReturnType<typeof checkTree>|undefined;
  for(const stage of [1,2]){
    inspectSource();inspectProfile(stage===2);assert("import driver library remains exact confined bytes",sha(regular(library))===librarySHA);
    const stageDirectory=join(directory,`launch-${stage}`);mkdirSync(stageDirectory,{mode:0o700});
    const url=`${c.base}${PATH}/${stage}`;
    const bundleIdentity=c.bundleIdentity();assert("import per-launch typed bundle metadata pins actual disposable storage",bundleIdentity.CFBundleName===`ArcModAgent-${c.slug}`&&bundleIdentity.ArcModAgentSession===c.slug&&bundleIdentity.CFBundleIdentifier===`company.thebrowser.Browser.mod.agent.${c.slug}`&&bundleIdentity.ArcModLocalMode===true);writeFileSync(join(stageDirectory,"bundle-identity.json"),JSON.stringify(bundleIdentity,null,2),{mode:0o600});
    const launch=c.launch({libraries:[library],environment:{ARC_SCENARIO_IMPORT_APP:app,ARC_SCENARIO_IMPORT_LIBRARY:library,ARC_SCENARIO_IMPORT_ROOT:root,ARC_SCENARIO_IMPORT_DIRECTORY:stageDirectory,ARC_SCENARIO_IMPORT_SOURCE:source,ARC_SCENARIO_IMPORT_HTML:html,ARC_SCENARIO_IMPORT_AGENT:c.slug,ARC_SCENARIO_IMPORT_NONCE:nonce,ARC_SCENARIO_IMPORT_BAR_UUID:barUUID,ARC_SCENARIO_IMPORT_OTHER_UUID:otherUUID,ARC_SCENARIO_IMPORT_URL:url,ARC_SCENARIO_IMPORT_STAGE:String(stage)}});
    let response:Reply|undefined;
    try{
      assert(`import launch${stage} becomes ready`,await c.ready(launch,`disposable import launch${stage}`));const pid=launch.pid();assert("import exposes exact browser PID",pid!==undefined);const start=identity(pid!);
      const owner=()=>{if(launch.pid()!==pid||identity(pid!)!==start||verifyAgentEndpoint(c.slug,c.port)!==pid)throw new Error("import PID/start/endpoint ownership changed");};
      const poll=async<T>(read:()=>T|undefined,ms=15000)=>{const end=Date.now()+ms;while(Date.now()<end){owner();const v=read();owner();if(v!==undefined)return v;await c.sleep(150);}return;};
      if(stage===1)firstPID=pid;else assert("import restart uses a new process",pid!==firstPID);profileIdentity=inspectProfile(true);
      const ready=await poll(()=>{const failure=readJSON<{error:string}>(join(stageDirectory,"startup-error.json"));if(failure)throw new Error(failure.error);const v=readJSON<{pid:number;nonce:string;agent:string;stage:number;url:string;installed:boolean}>(join(stageDirectory,"ready.json"));if(!v)return;if(v.pid!==pid||v.nonce!==nonce||v.agent!==c.slug||v.stage!==stage||v.url!==url)throw new Error("import driver ready identity mismatch");return v;});assert("import exact driver aliases ready",ready?.installed===true);
      // This stage-specific URL never existed before this driver's ready
      // receipt. Restored pages may remain; always create a NEW exact target.
      const beforeTargets=c.cdp("list") as {id:string;url:string;title:string}[];assert("import new stage URL did not preexist driver-ready target creation",beforeTargets.every(t=>t.url!==url));owner();const target=c.open(url);assert("import creates a genuinely new stage target ID",!beforeTargets.some(t=>t.id===target)&&(stage===1||target!==firstTarget));
      assert("import exact new stage fixture and document marker load",await c.titleIs(target,`Disposable import fixture 7f3a stage${stage}`)&&c.evaluate(target,"location.href")===url&&c.evaluate(target,"document.getElementById('import-stage-marker')?.textContent")===`ArcCore import stage${stage}`);owner();
      const afterTargets=c.cdp("list") as {id:string;url:string;title:string}[];assert("import exact stage URL belongs only to new target",afterTargets.filter(t=>t.url===url).length===1&&afterTargets.some(t=>t.id===target&&t.url===url));if(stage===1)firstTarget=target;writeFileSync(join(stageDirectory,"fixture-target.json"),JSON.stringify({pid,nonce,stage,url,target,createdAfterDriverReady:true,priorTargetIDs:beforeTargets.map(t=>t.id),priorURLs:beforeTargets.map(t=>t.url)},null,2),{mode:0o600});inspectSource();
      const id=`${nonce}-${stage}`,operation=stage===1?"import":"verify",request=join(stageDirectory,"request.json");writeFileSync(request+".tmp",JSON.stringify({id,operation,pid,nonce,url,stage,source,barUUID,otherUUID}),{mode:0o600,flag:"wx"});renameSync(request+".tmp",request);
      response=await poll(()=>{const v=readJSON<Reply>(join(stageDirectory,"response.json"));if(!v)return;if(v.id!==id||v.operation!==operation||v.pid!==pid||v.nonce!==nonce||v.agent!==c.slug||v.url!==url||v.stage!==stage)throw new Error("import response identity mismatch");if(v.error)throw new Error(v.error);return v.complete?v:undefined;});
      assert("import fixed request completes without replay",response?.complete===true&&response.main===true&&response.recorderAliasesBefore===true&&response.recorderAliasesAfter===true&&response.callbackScope==="own-public-protocol-sidecar-only");
      assert("import attempts match fixed launch scope",response!.importAttempts===(stage===1?1:0));
      if(stage===1){assert("import accepted exactly once with nil immediate error",response!.accepted===true&&response!.immediateError===null&&profileMatches(response!.profile));
        const events=response!.events;assert("import sidecar has exact five lifecycle events",events.length===5&&events.map(e=>e.kind).join(",")==="initialize,start,category-start,category-end,end");
        assert("import sidecar profiles mask categories errors and actual threads are exact",events.every((e,i)=>e.ordinal===i+1&&profileMatches(e.profile)&&e.error===null&&e.category===(i===2||i===3?2:null)&&typeof e.mainThread==="boolean"&&typeof e.thread==="string"&&e.thread.length>0));
      }else assert("import restart performs no import and receives no import callbacks",response!.events.length===0&&response!.accepted===false&&response!.immediateError===null);
      const tree=checkTree(response!);if(stage===1)firstTree=tree;else assert("import folder leaf destination and identity persist",JSON.stringify(tree)===JSON.stringify(firstTree));inspectSource();owner();
    }finally{await c.quit(launch,`disposable import launch${stage} standard graceful quit`);}
    const final=readJSON<Reply>(join(stageDirectory,"response.json"));assert("import final sidecar retains exact result and no late callbacks",final!==undefined&&JSON.stringify(final)===JSON.stringify(response));inspectProfile(true);inspectSource();
    const gate=validateDisposableImportRaw(join(root,`launch-${stage}`,"record.jsonl"),{image:IMAGE,stage,source,name,profileName,nonce,barUUID,otherUUID,fixtureURL:url});writeFileSync(join(stageDirectory,"raw-gate.json"),JSON.stringify(gate,null,2),{mode:0o600});assert("import real public API receiver argument return and create-forwarding raw gate passes",true);
  }
}
