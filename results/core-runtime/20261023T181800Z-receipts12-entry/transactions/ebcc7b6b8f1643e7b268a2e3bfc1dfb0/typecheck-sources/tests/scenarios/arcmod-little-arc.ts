// ISOLATED WRITE-only draft. NOT ready or authorized for native recording.
// Policy request142744 is NOT an ACK; missing reviewed policy/helper/user-native
// authorization receipt emits inconclusive before any app/action. No raw call or
// NSPanel-class/numeric AX-window-ID inference is claimed. Runtime unverified.
// Profile setup below is locally duplicated from arcmod-profiles.ts SHA
// 2a40293dceb99b6b575f943e7cd0aac95af0509b83bdc146a88477e33efe4b10.
// Copied profile setup is a prior WRITE-only plan, not Little Arc runtime evidence.
// Native menu evidence: accepted context-menus/native-1.json exposes File
// _NS:2190 / New Profile _NS:393 and Spaces _NS:1674 / Change Profile _NS:84.
// ../arc/ghidra/decomp/0x1015.c:55051 attaches the name-field ID;
// :39865-39867 attaches spacePicker; :31370-31398 defines New Profile option.
// :42789-42848 gives Next/Create Profile and enabled-state validity;
// :30175-30233 requires a valid name AND nonempty selectedSpaces.
// :57263-57309 binds each Toggle to Item.isSelected; :56210-56266 renders title
// and selection image. Disassembly 0x10153cf04-0x10153cf24 identifies its button
// callback FUN_100a830ec; 0x100a.c:100449-100475 flips the bound Boolean once.
// :31800-32020 constructs the exact destination/single-Space warning and
// Switch it up; :32250-32312 supplies confirm/Cancel. No private selectors used.
// Custom buttons need not expose AXCheckBox/value: the public selection proof
// is disabled->enabled Create Profile with only one exact eligible Space row.
import type { ScenarioContext, NativeNode, NativeSnapshot } from "../launch-slice/run";
import { createHash } from "node:crypto";
import { pathToFileURL } from "node:url";
import { verifyAgentEndpoint } from "../../../arc/tools/agent-cdp";
import { nativeState } from "../../../arc/tools/agent-native";
import { appendFileSync, chmodSync, existsSync, lstatSync, mkdirSync, readFileSync, readdirSync, realpathSync, statSync, writeFileSync } from "node:fs";
import { join, resolve } from "node:path";

const NAME="ArcCore Little Arc Profile",KEY="profileMarker",OLD="arccore-original-7f3a",NEW="arccore-created-7f3a";
export const morningControls={nameField:"importMenuFeatureNewProfileTextField",changePicker:"spacePicker",newProfileOption:"New Profile",
  submitLabels:["Create Profile","Change Profile","Next","Back","Cancel"],confirmation:"Switch it up"} as const;

export const fixture=(request:Request)=>{const m=new URL(request.url).pathname.match(/^\/little-arc-7f3a\/([ab])$/);if(!m)return;return new Response(`<!doctype html><title>Little Arc fixture ${m[1]!.toUpperCase()} 7f3a</title><script>window.__arcLittleArcFixtureDocument=crypto.randomUUID()</script><p>Disposable Little Arc ${m[1]!.toUpperCase()}</p>`,{headers:{"content-type":"text/html; charset=utf-8","cache-control":"no-store"}});};

export const profileSetupPlan=[
  "Fresh exact disposable profile/app; prove one Space and one original profile using owned spacePicker; set fixture A original marker.",
  "Set/read back ArcCore Fixture Profile through exact name ID; follow only observed known Create or Next step.",
  "If Next requests Space selection, press only the exact sole observed Space title row once and require disabled->enabled Create Profile.",
  "Handle only the exact owned destination/single-Space warning; observe creation assignment, otherwise select exact new profile and submit Change Profile once.",
  "Reacquire unique fixture target; require null/new localStorage marker, switch back to captured original and require original marker.",
  "Standard graceful quit and same-profile/new-PID restart; require both names and both markers to persist through repeat switches.",
];
type Node=NativeNode&{fingerprint:string};
type Snapshot=Omit<NativeSnapshot,"elements">&{bundle?:string;webContentExcluded?:boolean;elements:Node[]};
type Target={id:string;url:string;title:string};
type Identity={pid:number;executable:string;start:string};

export const morningPlan=[
  "BLOCKED: require explicit lead opt-in ACK responding to142744, reviewed native variant/helper/full script, default-build-unchanged proof and separate native-recording authorization; WRITE-only currently.",
  "Duplicate guarded fresh one-Space/two-profile setup locally; seed fixed OLD/NEW same-origin markers, return original and preserve its full main window/document.",
  "Send exact-PID external A then B once per fixed logical step; require one observed littleBrowserWindow identifier and A→B reuse, actual URL/title/document and original marker.",
  "Observe exact littleBrowserProfileIndicator plus advertised AXPress; only owned Change profile: menu with exactly the two known profiles may dispatch exact destination once. Unknown controls/confirmation stop inconclusive.",
  "Require native profile replacement closes old panel identifier, preserves main and reads fixed NEW marker; switch back through exact menu and verify OLD.",
  "Close the observed panel through its advertised standard AXCloseButton; external A once for a new logical reopen step must create a new identifier, leaving main unchanged.",
  "Normal graceful quit, same disposable directory/new PID restart, persisted profile inventory and both fixed markers; repeat A/B reuse and native profile replacement. No callback ordering/parity claim until recorded ground truth exists.",
];
const shaBytes=(bytes:Uint8Array|string)=>createHash("sha256").update(bytes).digest("hex"),shaFile=(path:string)=>shaBytes(readFileSync(path));
const SHA=/^[a-f0-9]{64}$/;
type Artifact={path:string;sha256:string};
type ApprovedPolicy={schemaVersion:number;requestSeq:number;status:string;nativeRecordingAuthorized:boolean;
  leadAck:{seq:number;body:string;bodySHA256:string;approvesOptIn:boolean};
  userAuthorization:{instruction:string;instructionSHA256:string;authorizesNativeRecording:boolean};
  parentReview:{scenarioSHA256:string;fullScenarioApproved:boolean;helperApproved:boolean;policyImplementationApproved:boolean};
  defaultBuildUnchanged:boolean;defaultBuildProof:Artifact;policySources:Artifact[];
  variants:Record<string,{info:Artifact;executable:Artifact;arcMod:Artifact}>;
  helper:{source:Artifact;binary:Artifact;productionTransport:boolean;expectedArcRoot:string}};
function readPolicy(c:ScenarioContext){
  const workspace=resolve(import.meta.dir,"../.."),arc=resolve(workspace,"../arc"),receiptPath=join(workspace,"logs/wp/scenarios-little-arc-policy.approved.json");
  const blocked=(why:string)=>{c.record("true single Little Arc prerequisites","inconclusive","BLOCKED/WRITE-only: "+why+". No app/native event/action attempted; request142744 is not an ACK; not ready-to-record.");return undefined;};
  if(!existsSync(receiptPath))return blocked("reviewed policy/helper/native-recording receipt absent");
  try{
    const st=lstatSync(receiptPath);if(st.isSymbolicLink()||!st.isFile()||st.size>65536||realpathSync(receiptPath)!==receiptPath)throw new Error("policy receipt must be a confined bounded no-follow file");
    const p=JSON.parse(readFileSync(receiptPath,"utf8")) as ApprovedPolicy;
    if(p.schemaVersion!==1||p.requestSeq!==142744||p.status!=="approved"||p.nativeRecordingAuthorized!==true||p.defaultBuildUnchanged!==true)throw new Error("receipt is not approved for this native recording");
    if(!Number.isSafeInteger(p.leadAck?.seq)||p.leadAck.seq<=142744||p.leadAck.approvesOptIn!==true||typeof p.leadAck.body!=="string"||!p.leadAck.body.length||!SHA.test(p.leadAck.bodySHA256)||shaBytes(p.leadAck.body)!==p.leadAck.bodySHA256)throw new Error("actual later lead response/body/hash required, not request number");
    if(p.userAuthorization?.authorizesNativeRecording!==true||typeof p.userAuthorization.instruction!=="string"||!p.userAuthorization.instruction.length||!SHA.test(p.userAuthorization.instructionSHA256)||shaBytes(p.userAuthorization.instruction)!==p.userAuthorization.instructionSHA256)throw new Error("WRITE-only instruction does not authorize recording");
    if(p.parentReview?.fullScenarioApproved!==true||p.parentReview.helperApproved!==true||p.parentReview.policyImplementationApproved!==true||p.parentReview.scenarioSHA256!==shaFile(import.meta.path))throw new Error("full exact scenario/helper/policy independent parent review missing");
    const fixedSources=["tools/agent-session.ts","tools/build-mod.ts","mods/hello/ArcMod.m","tools/patch-local-mode.ts"].map(n=>join(arc,n));
    if(!Array.isArray(p.policySources)||p.policySources.length!==fixedSources.length||fixedSources.some(path=>p.policySources.filter(a=>a.path===path).length!==1))throw new Error("exact four policy source artifacts required");
    if(p.helper?.productionTransport!==true||p.helper.expectedArcRoot!==arc||p.helper.source.path!==join(workspace,"tests/scenarios/little-arc-url-delivery.m")||p.helper.source.sha256!=="54804c2a86daf0d743b8666b912bf58da0f7401241934bfa0903e491917d581e")throw new Error("reviewed production helper source/compile-root contract missing");
    const artifact=(a:Artifact,expected?:string)=>{if(!a||typeof a.path!=="string"||resolve(a.path)!==a.path||!SHA.test(a.sha256)||expected!==undefined&&a.path!==expected)throw new Error("artifact path/hash missing or unexpected");const allowed=a.path.startsWith(join(workspace,"tests/scenarios")+"/")||a.path.startsWith(join(workspace,"logs/wp")+"/")||a.path.startsWith(arc+"/");const s=lstatSync(a.path);if(!allowed||s.isSymbolicLink()||!s.isFile()||realpathSync(a.path)!==a.path||shaFile(a.path)!==a.sha256)throw new Error("artifact is foreign/symlink/changed");return {artifact:a,dev:s.dev,ino:s.ino,size:s.size,mtime:s.mtimeMs};};
    const artifacts=p.policySources.map(a=>artifact(a));artifacts.push(artifact(p.defaultBuildProof),artifact(p.helper.source),artifact(p.helper.binary));if((lstatSync(p.helper.binary.path).mode&0o111)===0)throw new Error("reviewed production helper is not executable");
    if(!p.defaultBuildProof.path.startsWith(join(workspace,"logs/wp")+"/"))throw new Error("default build proof must be owned reviewed evidence");
    const variant=p.variants?.[c.slug],app=join(arc,`build/ArcModAgent-${c.slug}.app`);if(!variant)throw new Error("exact slug variant missing");
    artifacts.push(artifact(variant.info,join(app,"Contents/Info.plist")),artifact(variant.executable,join(app,"Contents/MacOS/Arc")),artifact(variant.arcMod,join(app,"Contents/Frameworks/ArcMod.dylib")));
    const checkMarker=()=>{const converted=Bun.spawnSync(["/usr/bin/plutil","-convert","json","-o","-",variant.info.path],{stdout:"pipe",stderr:"pipe",timeout:3000});if(!converted.success||converted.stderr.length)throw new Error("typed variant plist conversion failed");const v=JSON.parse(converted.stdout.toString()) as Record<string,unknown>;
      if(v.ArcModAgentSingleLittleArcParity!==true||v.ArcModLocalMode!==true||v.ArcModAgentSession!==c.slug||v.CFBundleName!==`ArcModAgent-${c.slug}`||v.CFBundleIdentifier!==`company.thebrowser.Browser.mod.agent.${c.slug}`||typeof v.ArcModHostResolverRules!=="string"||v.ArcModHostResolverRules.length===0)throw new Error("typed CFBoolean opt-in/local mode plus exact canonical identity/service rules required");c.bundleIdentity();};
    const receiptIdentity={dev:st.dev,ino:st.ino,size:st.size,mtime:st.mtimeMs,sha256:shaFile(receiptPath)};
    const recheck=()=>{const s=lstatSync(receiptPath);if(s.isSymbolicLink()||!s.isFile()||s.dev!==receiptIdentity.dev||s.ino!==receiptIdentity.ino||s.size!==receiptIdentity.size||s.mtimeMs!==receiptIdentity.mtime||shaFile(receiptPath)!==receiptIdentity.sha256)throw new Error("approved policy receipt changed");for(const a of artifacts){const v=lstatSync(a.artifact.path);if(v.isSymbolicLink()||!v.isFile()||realpathSync(a.artifact.path)!==a.artifact.path||v.dev!==a.dev||v.ino!==a.ino||v.size!==a.size||v.mtimeMs!==a.mtime)throw new Error("pinned policy/helper/native variant changed");}checkMarker();};recheck();
    return {receipt:p,receiptPath,receiptSHA256:receiptIdentity.sha256,recheck,helper:p.helper.binary.path};
  }catch(error){return blocked(String(error));}
}
type DeliveryStart={seconds:number;microseconds:number};
type DeliveryIdentity={pid:number;uid:number;seconds:number;microseconds:number;executablePath:string;appExecutablePath:string;appPath:string;bundleID:string;terminated:boolean};
type DeliveryReceipt={version:number;productionTransport:boolean;ok:boolean;operation:string;agent:string;pid:number;nonce:string;sequence:number;appPath:string;executablePath:string;bundleID:string;url:string;start:DeliveryStart;attempts:number;sendCalls:number;processQueries:number;permissionQueries:number;retryAllowed:boolean;deliveryOutcome:string;identityVerified:boolean;sendReturned:boolean;replyReceived?:boolean;eventErrorNumber?:number;permissionStatus?:number;runtimePanelOrNavigationVerified:boolean;identityBefore:DeliveryIdentity;identityImmediatelyBeforeSend?:DeliveryIdentity;identityAfter:DeliveryIdentity;preparedEvent?:{eventClass:number;eventID:number;targetType:number;targetPID:number;directObject:string;sendOptions:number;timeoutSeconds:number;askUserIfNeeded:boolean};error?:string};

export default async function scenario(c:ScenarioContext){
  const policy=readPolicy(c);if(!policy)return;
  const assert=(name:string,ok:unknown,detail="")=>{c.assert(name,ok,detail);if(!ok)throw new Error(name+(detail?": "+detail:""));};
  assert("Profiles uses caller-aware standard disposable recording",c.recorded&&!c.traced&&["trace156","ours156"].includes(c.slug));
  const app=resolve(import.meta.dir,`../../../arc/build/ArcModAgent-${c.slug}.app`),executable=join(app,"Contents/MacOS/Arc"),bundle=`company.thebrowser.Browser.mod.agent.${c.slug}`;
  assert("Profiles uses exact canonical disposable app",c.app===app&&realpathSync(c.app)===app);
  assert("Little Arc exact literal loopback fixture origin",new URL(c.base).protocol==="http:"&&new URL(c.base).hostname==="127.0.0.1"&&Number(new URL(c.base).port)>=1024&&Number(new URL(c.base).port)<=65535);
  const support=join(process.env.HOME!,"Library/Application Support"),profile=join(support,`ArcModAgent-${c.slug}`);
  assert("Profiles uses exact disposable profile path",c.data===profile);
  const supportCanonical=realpathSync(support),profileCanonical=join(supportCanonical,`ArcModAgent-${c.slug}`);
  const info=(path:string)=>{try{return lstatSync(path);}catch(error){if((error as NodeJS.ErrnoException).code==="ENOENT")return;throw error;}};
  let profileIdentity:{device:number;inode:number}|undefined;
  const inspectProfile=(phase:string,required:boolean,fresh=false)=>{
    assert(`Profiles ${phase}: canonical parent unchanged`,realpathSync(support)===supportCanonical);
    const st=info(profile);
    if(!st){assert(`Profiles ${phase}: only an uncreated profile may be absent`,!required&&profileIdentity===undefined);return;}
    assert(`Profiles ${phase}: no-follow real profile directory`,!st.isSymbolicLink()&&st.isDirectory());
    assert(`Profiles ${phase}: exact canonical parent`,realpathSync(profile)===profileCanonical);
    if(fresh)assert(`Profiles ${phase}: only empty disposable storage may preexist`,readdirSync(profile).every(n=>["User Data","Agent Chromium Data"].includes(n)));
    for(const name of ["User Data","Agent Chromium Data"]){
      const path=join(profile,name),child=info(path);
      assert(`Profiles ${phase}: ${name} absent or non-symlink directory`,child===undefined||(!child.isSymbolicLink()&&child.isDirectory()));
      if(fresh)assert(`Profiles ${phase}: ${name} initially empty`,child===undefined||readdirSync(path).length===0);
    }
    const after=info(profile);
    assert(`Profiles ${phase}: inspection retains directory identity`,after!==undefined&&!after.isSymbolicLink()&&after.isDirectory()&&after.dev===st.dev&&after.ino===st.ino);
    assert(`Profiles ${phase}: original inode retained`,profileIdentity===undefined||(profileIdentity.device===st.dev&&profileIdentity.inode===st.ino));
    return {device:st.dev,inode:st.ino};
  };
  profileIdentity=inspectProfile("prelaunch",false,true);
  const directory=join(c.outDir,"little-arc-evidence"),prior=info(directory);
  assert("Profiles evidence is fresh and not a symlink",prior===undefined||(!prior.isSymbolicLink()&&prior.isDirectory()&&readdirSync(directory).length===0));
  mkdirSync(directory,{recursive:true,mode:0o700});chmodSync(directory,0o700);
  assert("Profiles evidence confined to this canonical output",realpathSync(directory)===join(realpathSync(c.outDir),"little-arc-evidence"));
  const save=(name:string,value:unknown)=>writeFileSync(join(directory,name),JSON.stringify(value,null,2)+"\n",{mode:0o600});
  const helper=resolve(import.meta.dir,"../../../arc/build/agent-native"),source=resolve(import.meta.dir,"../../../arc/tools/agent-native.swift");
  assert("Profiles native helper already built/current",existsSync(helper)&&statSync(helper).mtimeMs>=statSync(source).mtimeMs);
  const helperIdentity=statSync(helper),nonce=crypto.randomUUID();
  let sequence=0,firstPID:number|undefined,originalProfile:string|undefined,spaceTitle:string|undefined,sidebarName:string|undefined;
  const helperCurrent=()=>{const st=statSync(helper);if(st.dev!==helperIdentity.dev||st.ino!==helperIdentity.ino||st.mtimeMs!==helperIdentity.mtimeMs||st.mtimeMs<statSync(source).mtimeMs)throw new Error("Profiles helper changed/stale; auto-build forbidden");};
  const command=(args:string[])=>{const r=Bun.spawnSync(args,{stdout:"pipe",stderr:"pipe",timeout:3000});if(!r.success||r.stderr.length)throw new Error("Profiles process identity observation failed");return r.stdout.toString().trim();};
  const identity=(pid:number):Identity=>{
    const exe=command(["/bin/ps","-ww","-p",String(pid),"-o","comm="]),start=command(["/bin/ps","-p",String(pid),"-o","lstart="]);
    if(exe!==executable||!start)throw new Error("Profiles exact browser executable/start unavailable");return {pid,executable:exe,start};
  };
  const controls=(launch:ReturnType<ScenarioContext["launch"]>,id:Identity)=>{
    const pid=id.pid;
    const owner=()=>{policy.recheck();const now=identity(pid);if(launch.pid()!==pid||now.start!==id.start||verifyAgentEndpoint(c.slug,c.port)!==pid)throw new Error("Profiles PID/start/endpoint ownership changed");};
    const state=()=>{owner();helperCurrent();const s=c.native() as Snapshot;owner();if(s.pid!==pid||s.bundle!==bundle||s.truncated!==false||s.webContentExcluded!==true)throw new Error("Profiles native identity/scope/truncation gap");return s;};
    const under=(n:Node,p:Node)=>n.path.startsWith(p.path+".");
    const unique=(s:Snapshot,predicate:(n:Node)=>boolean,what:string)=>{const nodes=s.elements.filter(predicate);if(nodes.length!==1)throw new Error(`${what}: expected one observed node, got ${nodes.length}`);return nodes[0]!;};
    const browser=(s:Snapshot)=>{const windows=s.elements.filter(n=>n.role==="AXWindow"),nodes=windows.filter(n=>n.identifier.startsWith("bigBrowserWindow-")),little=windows.filter(n=>n.identifier.startsWith("littleBrowserWindow-"));if(nodes.length!==1||little.length>1||windows.length!==nodes.length+little.length)throw new Error("Little Arc requires one exact full main window and at most one observed littleBrowserWindow identifier");return nodes[0]!;};
    const sheets=(s:Snapshot)=>{const w=browser(s),nodes=s.elements.filter(n=>n.role==="AXSheet");if(nodes.length>1||nodes.some(n=>!under(n,w)))throw new Error("Profiles extra/foreign sheet");return nodes;};
    const text=(n:Node)=>typeof n.value==="string"?n.value:n.label;
    const dispatch=(node:Node,kind:"press"|"set"|"perform"="press",value?:string)=>{
      owner();helperCurrent();
      if(++sequence>256||!node.enabled||!node.fingerprint||(kind==="set"?!node.valueSettable:!node.actions.includes(kind==="perform"?value!:"AXPress")))throw new Error("Profiles action missing enabled/advertised/settable identity or budget");
      const intent={nonce,pid,sequence,identifier:node.identifier,label:node.label,path:node.path,fingerprint:node.fingerprint,kind,value,attempts:1,retries:false};
      appendFileSync(join(directory,"actions.jsonl"),JSON.stringify({event:"intent",...intent})+"\n",{mode:0o600});
      try{
        const receipt=nativeState(c.slug,c.port,{pid,path:node.path,fingerprint:node.fingerprint,kind,...(value===undefined?{}:{value})});
        appendFileSync(join(directory,"actions.jsonl"),JSON.stringify({event:"receipt",...intent,receipt})+"\n",{mode:0o600});
        if(receipt.pid!==pid||receipt.bundle!==bundle||receipt.action!==kind||receipt.dispatched!==true)throw new Error("Profiles native receipt mismatch");owner();
      }catch(error){appendFileSync(join(directory,"actions.jsonl"),JSON.stringify({event:"uncertain-no-retry",...intent,error:String(error)})+"\n",{mode:0o600});throw error;}
    };
    // Repeat observations only. Ownership/protocol/mutation failures propagate.
    const poll=async<T>(read:()=>T|undefined,ms=8000)=>{const end=Date.now()+ms;while(Date.now()<end){owner();const v=read();owner();if(v!==undefined)return v;await c.sleep(150);}return;};
    const noSheet=(s:Snapshot)=>{if(sheets(s).length)throw new Error("Profiles expected no modal sheet");};
    const ensureWindow=async()=>{
      let s=state();if(s.elements.some(n=>n.role==="AXWindow")){browser(s);return;}
      dispatch(unique(s,n=>n.role==="AXMenuItem"&&n.identifier==="newWindow","sole New Window setup"));
      assert("Profiles owned browser window appears",await poll(()=>{s=state();if(!s.elements.some(n=>n.role==="AXWindow"))return;browser(s);return true;},5000));
    };
    const closeMenu=(s:Snapshot,parent:Node)=>dispatch(unique(s,n=>under(n,parent)&&n.role==="AXMenu"&&n.actions.includes("AXCancel"),"owned cancellable menu"),"perform","AXCancel");
    const openMenu=(label:"File"|"Spaces")=>{
      const s=state();noSheet(s);
      dispatch(unique(s,n=>n.role==="AXMenuBarItem"&&n.identifier===(label==="File"?"_NS:2190":"_NS:1674")&&n.label===label,`exact ${label} menu`));
      const fresh=state(),bar=unique(fresh,n=>n.role==="AXMenuBarItem"&&n.label===label,"opened menu bar");
      return {s:fresh,bar,items:fresh.elements.filter(n=>under(n,bar)&&n.role==="AXMenuItem")};
    };
    const oneSpace=()=>{
      const s=state();noSheet(s);if(s.elements.filter(n=>n.role==="AXWindow").length!==1)throw new Error("Little Arc profile setup requires sole full main and no panel");const w=browser(s),side=unique(s,n=>under(n,w)&&n.identifier==="sidebarSpaceName"&&n.role==="AXStaticText","sole Space sidebar name");
      if(typeof side.value!=="string")throw new Error("Space sidebar name lacks string value");
      const menu=openMenu("Spaces"),spaces=menu.items.filter(n=>n.identifier==="focusSpace:");
      assert("Profiles has exactly one nonempty Space menu title",spaces.length===1&&spaces[0]!.label.length>0);
      const value={title:spaces[0]!.label,sidebar:side.value};closeMenu(menu.s,menu.bar);
      assert("Profiles preserves captured sole Space identity",spaceTitle===undefined||(spaceTitle===value.title&&sidebarName===value.sidebar));return value;
    };
    const changeSheet=(s:Snapshot)=>{
      const nodes=sheets(s);if(!nodes.length)return;
      const sheet=nodes[0]!,pickers=s.elements.filter(n=>under(n,sheet)&&n.identifier===morningControls.changePicker);
      if(pickers.length!==1)throw new Error("Profiles unknown sheet instead of exact picker editor");
      unique(s,n=>under(n,sheet)&&n.role==="AXButton"&&n.label==="Cancel","Change Profile Cancel");
      unique(s,n=>under(n,sheet)&&n.role==="AXButton"&&n.label==="Change Profile","Change Profile submit");
      return {s,sheet,picker:pickers[0]!};
    };
    const openChange=async()=>{
      oneSpace();const menu=openMenu("Spaces");
      dispatch(unique(menu.s,n=>under(n,menu.bar)&&n.role==="AXMenuItem"&&n.identifier==="_NS:84"&&n.label==="Change Profile","exact Change Profile command"));
      const v=await poll(()=>changeSheet(state()),5000);assert("owned exact Change Profile editor appears",v!==undefined);return v!;
    };
    const pickerLabel=(s:Snapshot,picker:Node,names:readonly string[])=>{
      const matches=names.filter(name=>picker.label===name||picker.value===name||s.elements.some(n=>under(n,picker)&&n.role==="AXStaticText"&&text(n)===name));
      if(matches.length!==1)throw new Error("Profiles picker lacks one exact observed display-name readback");return matches[0]!;
    };
    const profileMenu=(s:Snapshot)=>{
      const editor=changeSheet(s);if(!editor)throw new Error("Profiles picker editor disappeared");
      const menus=s.elements.filter(n=>under(n,editor.picker)&&n.role==="AXMenu");if(!menus.length)return;
      if(menus.length!==1)throw new Error("Profiles picker menu ambiguous");
      const menu=menus[0]!,rows=s.elements.filter(n=>under(n,menu)&&["AXMenuItem","AXButton"].includes(n.role)&&n.actions.includes("AXPress"));
      if(rows.filter(n=>n.label==="New Profile").length!==1)throw new Error("Profiles picker lacks exact unique New Profile option");
      return {s,editor,menu,profiles:rows.filter(n=>n.label!=="New Profile")};
    };
    const inventory=async(expected:number)=>{
      const editor=changeSheet(state());if(!editor)throw new Error("Profiles inventory missing editor");dispatch(editor.picker);
      const menu=await poll(()=>profileMenu(state()),3000);assert("Profiles exact owned picker menu appears",menu!==undefined);
      const names=menu!.profiles.map(n=>n.label);
      assert(`Profiles inventory contains exactly ${expected} actual profiles`,names.length===expected&&menu!.profiles.every(n=>n.enabled&&n.label.length>0)&&new Set(names).size===expected);
      closeMenu(menu!.s,menu!.editor.picker);
      const fresh=changeSheet(state());if(!fresh)throw new Error("Profiles editor disappeared after menu cancellation");
      return {...fresh,names,selected:pickerLabel(fresh.s,fresh.picker,names)};
    };
    const cancelChange=()=>{const v=changeSheet(state());if(!v)throw new Error("Profiles editor missing before Cancel");dispatch(unique(v.s,n=>under(n,v.sheet)&&n.role==="AXButton"&&n.label==="Cancel","fresh editor Cancel"));};
    const waitNoSheet=async()=>assert("Profiles owned modal closes",await poll(()=>sheets(state()).length===0?true:undefined,5000));
    const expectedNames=(names:readonly string[],count:number)=>assert("Profiles inventory matches exact captured names",names.length===count&&new Set(names).size===count&&names.includes(originalProfile!)&&(count===1||names.includes(NAME)));
    const confirm=(s:Snapshot,destination:string)=>{
      const nodes=sheets(s);if(!nodes.length)return;
      const sheet=nodes[0]!,title=`Switch to your ${destination} Profile?`;
      unique(s,n=>under(n,sheet)&&n.role==="AXStaticText"&&text(n)===title,"exact destination confirmation title");
      const message=`Each Profile has different browser data, so your ${spaceTitle} Space might look a little different!\n\nFor example, you might get logged out of some websites, see different Favorites up top, or need to re-install an extension.\n\nThis action cannot be undone.`;
      unique(s,n=>under(n,sheet)&&n.role==="AXStaticText"&&text(n)===message,"exact single captured Space warning");
      unique(s,n=>under(n,sheet)&&n.role==="AXButton"&&n.label==="Cancel","warning Cancel");
      const buttons=s.elements.filter(n=>under(n,sheet)&&n.role==="AXButton");
      if(buttons.length!==2||s.elements.some(n=>under(n,sheet)&&n.role==="AXCheckBox"))throw new Error("Profiles warning has extra/suppression controls");
      return unique(s,n=>under(n,sheet)&&n.role==="AXButton"&&n.label==="Switch it up","single-Space affirmative");
    };
    const confirmChange=async(destination:string)=>{
      const shown=await poll(()=>{
        const s=state(),values=sheets(s);if(!values.length)return;
        // A returned submit may precede removal of its known editor. Observe
        // that exact transition without replaying Change Profile or accepting
        // an unknown sheet as a pending warning.
        if(s.elements.some(n=>under(n,values[0]!)&&n.identifier===morningControls.changePicker)){changeSheet(s);return;}
        return confirm(s,destination);
      },5000);
      assert("exact owned destination profile warning appears",shown!==undefined);
      const fresh=confirm(state(),destination);if(!fresh)throw new Error("Profiles confirmation disappeared before sole action");dispatch(fresh);await waitNoSheet();
    };
    const selectedProfile=async()=>{await openChange();const v=await inventory(2);expectedNames(v.names,2);cancelChange();await waitNoSheet();return v.selected;};
    const switchFromEditor=async(destination:string,current:string)=>{
      if(destination===current)throw new Error("Profiles switch requires distinct observed destination");
      const editor=changeSheet(state());if(!editor)throw new Error("Profiles switch editor missing");dispatch(editor.picker);
      const row=await poll(()=>{
        const menu=profileMenu(state());if(!menu)return;expectedNames(menu.profiles.map(n=>n.label),2);
        return unique(menu.s,n=>under(n,menu.menu)&&["AXMenuItem","AXButton"].includes(n.role)&&n.label===destination,"exact destination profile row");
      },3000);
      assert("Profiles exact destination row appears",row!==undefined);dispatch(row!);
      assert("Profiles picker reads back selected destination",await poll(()=>{const v=changeSheet(state());if(!v)throw new Error("Profiles editor disappeared before submit");return pickerLabel(v.s,v.picker,[originalProfile!,NAME])===destination?true:undefined;},3000));
      const submit=changeSheet(state());if(!submit)throw new Error("Profiles submit editor missing");
      dispatch(unique(submit.s,n=>under(n,submit.sheet)&&n.role==="AXButton"&&n.label==="Change Profile","fresh Change Profile submit"));
      await confirmChange(destination);assert("Profiles applies exact destination",await selectedProfile()===destination);
    };
    const switchTo=async(destination:string)=>{await openChange();const v=await inventory(2);expectedNames(v.names,2);await switchFromEditor(destination,v.selected);};
    const targets=()=>{owner();const values=c.cdp("list") as Target[];owner();return values;};
    const fixture=async(label:string,initial=false)=>{
      let values=targets().filter(t=>t.url===c.base+"/a");if(initial&&values.length===0){c.open(c.base+"/a");owner();}
      const v=await poll(()=>{values=targets().filter(t=>t.url===c.base+"/a");if(values.length>1)throw new Error("Profiles duplicate fixture A CDP targets");return values.length===1&&values[0]!.title===c.EXPECT.aTitle?values[0]:undefined;},10000);
      assert(`Profiles ${label} has unique exact fixture URL/title target`,v!==undefined);
      assert(`Profiles ${label} reads exact fixture document`,c.evaluate(v!.id,"location.href")===c.base+"/a"&&c.evaluate(v!.id,"document.title")===c.EXPECT.aTitle);owner();
      const s=state(),w=browser(s);noSheet(s);
      const tab=unique(s,n=>under(n,w)&&n.role==="AXButton"&&n.label===c.EXPECT.aTitle,"sole owned fixture tab"),action=tab.actions.find(a=>a==="AXPress"||a.startsWith("Name:press"));
      if(!action)throw new Error("Profiles fixture lacks advertised native selection");dispatch(tab,"perform",action);
      assert(`Profiles ${label} native address matches fixture`,await poll(()=>{const s=state(),w=browser(s);return s.elements.filter(n=>under(n,w)&&n.identifier==="contentPanelTextField"&&n.value===c.base+"/a").length===1?true:undefined;},5000));return v!.id;
    };
    const marker=(target:string,expected:string|null,label:string)=>{owner();const v=c.evaluate(target,`localStorage.getItem(${JSON.stringify(KEY)})`);owner();assert(`Profiles ${label} marker equals fixed expectation`,v===expected);};
    const setMarker=(target:string,value:string)=>{
      owner();const intent={nonce,pid,target,url:c.base+"/a",key:KEY,value,attempts:1,retries:false};
      appendFileSync(join(directory,"actions.jsonl"),JSON.stringify({event:"fixture-marker-intent",...intent})+"\n",{mode:0o600});
      try{
        const result=c.evaluate(target,`localStorage.setItem(${JSON.stringify(KEY)},${JSON.stringify(value)});true`);owner();
        appendFileSync(join(directory,"actions.jsonl"),JSON.stringify({event:"fixture-marker-returned",...intent,result})+"\n",{mode:0o600});
        if(result!==true)throw new Error("Profiles marker setter lacks true return");marker(target,value,"written fixture");
      }catch(error){appendFileSync(join(directory,"actions.jsonl"),JSON.stringify({event:"fixture-marker-uncertain-no-retry",...intent,error:String(error)})+"\n",{mode:0o600});throw error;}
    };
    return {owner,state,under,unique,browser,sheets,text,dispatch,poll,ensureWindow,openMenu,oneSpace,openChange,inventory,cancelChange,waitNoSheet,expectedNames,confirm,confirmChange,switchFromEditor,switchTo,fixture,marker,setMarker};
  };
  type UI=ReturnType<typeof controls>;
  let helperSequence=0,deliveryIndex=0;
  const deliverySchedule=[{phase:"first-A",fixture:"a"},{phase:"first-B",fixture:"b"},{phase:"first-reopen-A",fixture:"a"},{phase:"restart-A",fixture:"a"},{phase:"restart-B",fixture:"b"}] as const;
  const helperRequest=(id:Identity,operation:"inspect-only"|"deliver-once",fixture:"a"|"b",start:DeliveryStart|null)=>({operation,agent:c.slug,pid:id.pid,start,appURL:pathToFileURL(app+"/").href,executablePath:executable,bundleID:bundle,appName:`ArcModAgent-${c.slug}`,fixturePort:new URL(c.base).port?Number(new URL(c.base).port):0,fixture,nonce,sequence:++helperSequence});
  const helperCall=(id:Identity,operation:"inspect-only"|"deliver-once",fixture:"a"|"b",start:DeliveryStart|null)=>{
    policy.recheck();helperCurrent();if(helperSequence>=64)throw new Error("Little Arc helper request budget exhausted");const request=helperRequest(id,operation,fixture,start),expectedURL=`${c.base}/little-arc-7f3a/${fixture}`;
    const intent={nonce,pid:id.pid,operation,sequence:request.sequence,request,sendAttemptsAllowed:operation==="deliver-once"?1:0,retries:false};
    appendFileSync(join(directory,"external-url-intents.jsonl"),JSON.stringify({event:"intent",...intent})+"\n",{mode:0o600});
    let receipt:DeliveryReceipt|undefined;
    try{
      const result=Bun.spawnSync([policy.helper],{stdin:Buffer.from(JSON.stringify(request)),stdout:"pipe",stderr:"pipe",timeout:10000});
      if(result.stdout.length>16384||result.stderr.length>4096)throw new Error("Little Arc helper response exceeds bounded schema");
      try{receipt=JSON.parse(result.stdout.toString()) as DeliveryReceipt;}catch{throw new Error("Little Arc helper malformed/absent receipt; delivery uncertain, no retry");}
      appendFileSync(join(directory,"external-url-intents.jsonl"),JSON.stringify({event:"receipt",...intent,exit:result.exitCode,receipt})+"\n",{mode:0o600});
      if(!result.success||result.stderr.length||receipt.error)throw new Error("Little Arc helper failed/uncertain; never resend: "+(receipt.error??"nonzero/stderr"));
      const r=receipt,validStart=(v:DeliveryStart)=>v&&Number.isSafeInteger(v.seconds)&&v.seconds>0&&Number.isSafeInteger(v.microseconds)&&v.microseconds>=0&&v.microseconds<1000000;
      if(r.version!==1||r.productionTransport!==true||r.ok!==true||r.operation!==operation||r.agent!==c.slug||r.pid!==id.pid||r.nonce!==nonce||r.sequence!==request.sequence||r.appPath!==app||r.executablePath!==executable||r.bundleID!==bundle||r.url!==expectedURL||r.retryAllowed!==false||r.runtimePanelOrNavigationVerified!==false||r.identityVerified!==true||!validStart(r.start)||start!==null&&(r.start.seconds!==start.seconds||r.start.microseconds!==start.microseconds))throw new Error("Little Arc helper exact owned receipt/start mismatch");
      const identityMatches=(v:DeliveryIdentity|undefined)=>v?.pid===id.pid&&v.uid===process.getuid!()&&v.seconds===r.start.seconds&&v.microseconds===r.start.microseconds&&v.appPath===app&&v.executablePath===executable&&v.appExecutablePath===executable&&v.bundleID===bundle&&v.terminated===false;
      if(!identityMatches(r.identityBefore)||!identityMatches(r.identityAfter))throw new Error("Little Arc native PID/start/path changed");
      if(operation==="inspect-only"){
        if(r.attempts!==0||r.sendCalls!==0||r.processQueries!==2||r.permissionQueries!==0||r.sendReturned!==false||r.deliveryOutcome!=="not-attempted")throw new Error("Little Arc inspect must not send or ask permission");
      }else{
        const e=r.preparedEvent;
        if(r.attempts!==1||r.sendCalls!==1||r.processQueries!==3||r.permissionQueries!==1||r.permissionStatus!==0||r.sendReturned!==true||r.replyReceived!==true||r.eventErrorNumber!==0||r.deliveryOutcome!=="reply-received"||!identityMatches(r.identityImmediatelyBeforeSend)||e?.eventClass!==0x4755524c||e.eventID!==0x4755524c||e.targetType!==0x6b706964||e.targetPID!==id.pid||e.directObject!==expectedURL||e.sendOptions!==0x21093||e.timeoutSeconds!==3||e.askUserIfNeeded!==false)throw new Error("Little Arc delivery did not return exact one-send/no-prompt receipt");
      }
      policy.recheck();save(`external-helper-${request.sequence}.json`,{request,receipt});return r.start;
    }catch(error){appendFileSync(join(directory,"external-url-intents.jsonl"),JSON.stringify({event:"uncertain-no-retry",...intent,receipt,error:String(error)})+"\n",{mode:0o600});throw error;}
  };
  const panelControls=(ui:UI,id:Identity,mainTarget:string)=>{
    ui.owner();const initial=ui.state(),main=ui.browser(initial),mainIdentifier=main.identifier;
    if(initial.elements.some(n=>n.role==="AXWindow"&&n.identifier.startsWith("littleBrowserWindow-")))throw new Error("Little Arc phase requires no preexisting panel");
    const nativeStart=helperCall(id,"inspect-only","a",null);
    const mainDocument=c.evaluate(mainTarget,"({url:location.href,title:document.title,timeOrigin:performance.timeOrigin,marker:localStorage.getItem('profileMarker')})") as {url:string;title:string;timeOrigin:number;marker:string|null};
    assert("Little Arc phase pins original main document and marker",mainDocument.url===c.base+"/a"&&mainDocument.title===c.EXPECT.aTitle&&Number.isFinite(mainDocument.timeOrigin)&&mainDocument.marker===OLD);
    const noUnknownModal=(s:Snapshot)=>{if(s.elements.some(n=>n.role==="AXSheet"||n.role==="AXDialog"))throw new Error("Little Arc unobserved profile/URL confirmation; no blind affirmative action");};
    const windows=(s:Snapshot)=>{const all=s.elements.filter(n=>n.role==="AXWindow"),full=all.filter(n=>n.identifier.startsWith("bigBrowserWindow-")),little=all.filter(n=>n.identifier.startsWith("littleBrowserWindow-"));if(full.length!==1||full[0]!.identifier!==mainIdentifier||little.length>1||all.length!==full.length+little.length)throw new Error("Little Arc full main ownership/sole observed panel invariant failed");return little;};
    const preserveMain=(s=ui.state())=>{
      ui.owner();noUnknownModal(s);windows(s);const w=ui.browser(s);
      const address=s.elements.filter(n=>ui.under(n,w)&&n.identifier==="contentPanelTextField"&&n.value===c.base+"/a");
      assert("Little Arc preserves same main AX identifier and selected address",w.identifier===mainIdentifier&&address.length===1);
      const listed=c.cdp("list") as Target[];if(listed.filter(t=>t.id===mainTarget&&t.url===c.base+"/a"&&t.title===c.EXPECT.aTitle).length!==1)throw new Error("Little Arc original main target disappeared/changed");
      const doc=c.evaluate(mainTarget,"({url:location.href,title:document.title,timeOrigin:performance.timeOrigin,marker:localStorage.getItem('profileMarker')})") as typeof mainDocument;
      assert("Little Arc preserves main profile/document/marker",doc.url===mainDocument.url&&doc.title===mainDocument.title&&doc.timeOrigin===mainDocument.timeOrigin&&doc.marker===OLD);ui.owner();return s;
    };
    type Panel={identifier:string;target:string;fixture:"a"|"b";documentNonce:string;profileMarker:string;url:string;title:string};
    const observedPanel=async(fixture:"a"|"b",marker:string,relation?:{same?:string;different?:string})=>{
      const url=`${c.base}/little-arc-7f3a/${fixture}`,title=`Little Arc fixture ${fixture.toUpperCase()} 7f3a`;
      const observed=await ui.poll(()=>{
        const s=preserveMain(),little=windows(s);if(little.length!==1)return;
        const window=little[0]!;if(relation?.same&&window.identifier!==relation.same||relation?.different&&window.identifier===relation.different)return;
        const exactAddress=s.elements.filter(n=>ui.under(n,window)&&["AXTextField","AXStaticText"].includes(n.role)&&(n.value===url||n.label===url));
        const titleVisible=window.label===title||s.elements.some(n=>ui.under(n,window)&&n.role==="AXStaticText"&&(n.value===title||n.label===title));
        if(exactAddress.length!==1||!titleVisible)return;
        const targets=(c.cdp("list") as Target[]).filter(t=>t.url===url);if(targets.length!==1||targets[0]!.title!==title)return;
        const doc=c.evaluate(targets[0]!.id,"({url:location.href,title:document.title,documentNonce:window.__arcLittleArcFixtureDocument,marker:localStorage.getItem('profileMarker')})") as {url:string;title:string;documentNonce:string;marker:string|null};
        if(doc.url!==url||doc.title!==title||!/^[a-f0-9-]{36}$/.test(doc.documentNonce)||doc.marker!==marker)return;
        preserveMain();return {identifier:window.identifier,target:targets[0]!.id,fixture,documentNonce:doc.documentNonce,profileMarker:marker,url,title} satisfies Panel;
      },12000);
      if(!observed){c.record("Little Arc exact owned AX/DOM association","inconclusive","Unobserved control/DOM association; no NSPanel class or numeric AX-window-ID inference, no retry");throw new Error("Little Arc exact observed panel association unavailable");}assert("Little Arc exact observed AX URL/title, sole panel, DOM and fixed profile marker",true,JSON.stringify(observed));return observed;
    };
    const deliver=async(phase:typeof deliverySchedule[number]["phase"],fixture:"a"|"b",marker:string,relation?:{same?:string;different?:string})=>{
      const expected=deliverySchedule[deliveryIndex];if(!expected||expected.phase!==phase||expected.fixture!==fixture)throw new Error("Little Arc external logical delivery schedule changed/replayed");preserveMain();++deliveryIndex;
      // Exactly one helper invocation per journaled logical delivery. A later
      // explicit closed/reopen or new-PID phase is a separate fixed operation.
      helperCall(id,"deliver-once",fixture,nativeStart);preserveMain();const panel=await observedPanel(fixture,marker,relation);save(`${phase}.json`,{nonce,pid:id.pid,mainIdentifier,mainTarget,mainDocument,panel,scope:"Observed AX identifier/URL/title and CDP document/storage only; helper reply is not navigation proof."});return panel;
    };
    const switchProfile=async(before:Panel,destination:string,expectedMarker:string)=>{
      if(![originalProfile,NAME].includes(destination)||expectedMarker===before.profileMarker)throw new Error("Little Arc switch requires exact known distinct destination/marker");
      const s=preserveMain(),window=ui.unique(s,n=>n.role==="AXWindow"&&n.identifier===before.identifier,"fresh exact panel before profile menu");
      if(s.elements.some(n=>n.role==="AXMenu"&&!s.elements.some(b=>b.role==="AXMenuBar"&&ui.under(n,b))&&n.actions.includes("AXCancel")))throw new Error("Little Arc requires no preexisting popup menu");
      const indicator=ui.unique(s,n=>ui.under(n,window)&&n.identifier==="littleBrowserProfileIndicator"&&n.role==="AXButton","observed static Little Arc profile indicator candidate");
      if(!indicator.enabled||!indicator.actions.includes("AXPress"))throw new Error("Little Arc profile indicator has no advertised press; unsupported/unobserved, no action");ui.dispatch(indicator);
      const menuState=()=>{const s=ui.state();noUnknownModal(s);windows(s);const menus=s.elements.filter(n=>n.role==="AXMenu"&&n.label==="Change profile:"&&!s.elements.some(b=>b.role==="AXMenuBar"&&ui.under(n,b)));if(menus.length===0)return;if(menus.length!==1)throw new Error("Little Arc profile popup ambiguous");const menu=menus[0]!,rows=s.elements.filter(n=>ui.under(n,menu)&&n.role==="AXMenuItem");if(rows.length!==2||new Set(rows.map(n=>n.label)).size!==2||!rows.some(n=>n.label===originalProfile)||!rows.some(n=>n.label===NAME)||rows.some(n=>!n.enabled||!n.actions.includes("AXPress")))throw new Error("Little Arc native profile menu must contain only the two exact known enabled profiles");return {s,menu,rows};};
      assert("Little Arc exact owned static profile menu appears",await ui.poll(menuState,4000)!==undefined);
      const fresh=menuState();if(!fresh)throw new Error("Little Arc menu disappeared before sole destination press");const row=ui.unique(fresh.s,n=>ui.under(n,fresh.menu)&&n.role==="AXMenuItem"&&n.label===destination,"fresh exact Little Arc destination profile");ui.dispatch(row);
      const after=await observedPanel(before.fixture,expectedMarker,{different:before.identifier});const final=preserveMain();assert("Little Arc native profile replacement closes old observed panel",!final.elements.some(n=>n.role==="AXWindow"&&n.identifier===before.identifier)&&after.identifier!==before.identifier&&after.documentNonce!==before.documentNonce);save(`profile-replacement-${sequence}.json`,{before,after,destination,scope:"Native replacement/marker behavior remains unrecorded. Unknown confirmations are never accepted."});return after;
    };
    const close=async(panel:Panel)=>{
      const s=preserveMain(),window=ui.unique(s,n=>n.role==="AXWindow"&&n.identifier===panel.identifier,"fresh exact panel for close"),button=ui.unique(s,n=>ui.under(n,window)&&n.role==="AXButton"&&n.subrole==="AXCloseButton","observed standard panel close button");
      if(!button.enabled||!button.actions.includes("AXPress"))throw new Error("Little Arc close lacks advertised standard action");ui.dispatch(button);
      assert("Little Arc closing panel leaves no panel and same full main",await ui.poll(()=>{const s=preserveMain();if(windows(s).length!==0)return;const pages=c.cdp("list") as Target[];return pages.every(t=>t.id!==panel.target)?true:undefined;},8000));
    };
    const round=async(phase:"first"|"restart",reopen:boolean)=>{
      const a=await deliver(phase==="first"?"first-A":"restart-A","a",OLD),b=await deliver(phase==="first"?"first-B":"restart-B","b",OLD,{same:a.identifier});
      assert("Little Arc A to B reuses actual panel and replaces document",a.identifier===b.identifier&&a.documentNonce!==b.documentNonce);
      const created=await switchProfile(b,NAME,NEW),returned=await switchProfile(created,originalProfile!,OLD);await close(returned);
      if(reopen){const next=await deliver("first-reopen-A","a",OLD,{different:returned.identifier});assert("Little Arc explicit close/reopen creates fresh observed identifier",next.identifier!==returned.identifier);await close(next);}
      preserveMain();save(`${phase}-panel-summary.json`,{nonce,pid:id.pid,mainIdentifier,mainTarget,a,b,created,returned,reopen,scope:"Fixed markers/one-panel identity/replacement and main isolation only. No native class inference, numeric window identity, raw callback sequence or boundary-parity claim."});
    };
    return {round};
  };
  policy.recheck();const first=c.launch();
  try{
    assert("Profiles initial launch ready",await c.ready(first,"Profiles initial launch"));
    firstPID=first.pid();assert("Profiles initial actual PID exists",firstPID!==undefined);
    profileIdentity=inspectProfile("initial launch",true);const ui=controls(first,identity(firstPID!));await ui.ensureWindow();
    const firstFixture=await ui.fixture("original",true),space=ui.oneSpace();spaceTitle=space.title;sidebarName=space.sidebar;
    await ui.openChange();const initial=await ui.inventory(1);originalProfile=initial.names[0]!;
    assert("Profiles sole original selected and distinct fixed new name",initial.selected===originalProfile&&originalProfile!==NAME&&originalProfile!=="New Profile");
    ui.cancelChange();await ui.waitNoSheet();ui.marker(firstFixture,null,"fresh original");ui.setMarker(firstFixture,OLD);
    save("initial.json",{nonce,pid:firstPID,profile:profileCanonical,profileIdentity,originalProfile,spaceTitle,sidebarName,firstFixture});
    const menu=ui.openMenu("File");
    ui.dispatch(ui.unique(menu.s,n=>ui.under(n,menu.bar)&&n.role==="AXMenuItem"&&n.identifier==="_NS:393"&&n.label==="New Profile","exact New Profile command"));
    const creation=()=>{
      const s=ui.state(),sheets=ui.sheets(s);if(!sheets.length)return;
      const sheet=sheets[0]!,fields=s.elements.filter(n=>ui.under(n,sheet)&&n.identifier===morningControls.nameField);
      if(fields.length!==1)throw new Error("Profiles unknown creation editor instead of exact name field");return {s,sheet,field:fields[0]!};
    };
    const editor=await ui.poll(creation,5000);assert("owned exact profile name editor appears",editor!==undefined);
    assert("profile name field enabled nonsecure actually settable",editor!.field.role==="AXTextField"&&editor!.field.subrole!=="AXSecureTextField"&&editor!.field.enabled&&editor!.field.valueSettable&&typeof editor!.field.value==="string");
    ui.dispatch(editor!.field,"set",NAME);
    assert("profile name setter reads back fixed name",await ui.poll(()=>{const v=creation();return v?.field.value===NAME?true:undefined;},3000));
    // Name readback can precede asynchronous validation/AXPress readiness. Only
    // observe the exact unchanged editor and known submit; never replay input.
    const readySubmit=await ui.poll(()=>{
      const v=creation();if(!v)return;
      if(v.field.value!==NAME)throw new Error("Profiles fixed name changed before submit readiness");
      const submits=v.s.elements.filter(n=>ui.under(n,v.sheet)&&n.role==="AXButton"&&["Next","Create Profile"].includes(n.label));
      if(submits.length>1)throw new Error("Profiles ambiguous Next-or-Create submit step");
      const submit=submits[0];return submit?.enabled&&submit.actions.includes("AXPress")?submit:undefined;
    },5000);
    assert("Profiles has one ready enabled advertised Next-or-Create step",readySubmit!==undefined);
    if(readySubmit!.label==="Next"){
      ui.dispatch(readySubmit!);
      const selection=()=>{
        const s=ui.state(),sheets=ui.sheets(s);if(!sheets.length)return;
        const sheet=sheets[0]!;
        if(s.elements.some(n=>ui.under(n,sheet)&&n.identifier===morningControls.nameField&&n.value===NAME)&&
          s.elements.filter(n=>ui.under(n,sheet)&&n.role==="AXButton"&&n.label==="Next").length===1)return;
        ui.unique(s,n=>ui.under(n,sheet)&&n.role==="AXStaticText"&&ui.text(n)==="Which Spaces should use this Profile?","exact Space selection title");
        const submit=ui.unique(s,n=>ui.under(n,sheet)&&n.role==="AXButton"&&n.label==="Create Profile","selection Create Profile");
        const rows=s.elements.filter(n=>ui.under(n,sheet)&&["AXButton","AXCheckBox"].includes(n.role)&&!["Back","Cancel","Create Profile"].includes(n.label)&&n.actions.includes("AXPress"));
        assert("Profiles selection has only exact sole observed Space title row",rows.length===1&&rows[0]!.label===spaceTitle&&rows[0]!.enabled);
        return {s,sheet,submit,row:rows[0]!};
      };
      const step=await ui.poll(selection,5000);assert("Profiles sole Space selection step appears",step!==undefined);
      assert("Profiles initially no selected Space by submit validity",step!.submit.enabled===false);ui.dispatch(step!.row);
      const selectedSubmit=await ui.poll(()=>{const v=selection();return v?.submit.enabled&&v.submit.actions.includes("AXPress")?v.submit:undefined;},3000);
      assert("sole bound Space press enables advertised Create Profile",selectedSubmit!==undefined);
      ui.dispatch(selectedSubmit!);
    }else ui.dispatch(readySubmit!);
    // Known creation editor may remain while completion is in flight. Only its
    // exact destination warning may intervene; no unknown sheet is dismissed.
    const completed=await ui.poll(()=>{
      const s=ui.state(),sheets=ui.sheets(s);if(!sheets.length)return {warning:false};
      const sheet=sheets[0]!;
      if(s.elements.some(n=>ui.under(n,sheet)&&n.identifier===morningControls.nameField&&n.value===NAME)&&
        s.elements.filter(n=>ui.under(n,sheet)&&n.role==="AXButton"&&["Create Profile","Next"].includes(n.label)).length===1)return;
      if(s.elements.filter(n=>ui.under(n,sheet)&&n.role==="AXStaticText"&&ui.text(n)==="Which Spaces should use this Profile?").length===1&&
        s.elements.filter(n=>ui.under(n,sheet)&&n.role==="AXButton"&&n.label==="Create Profile").length===1&&
        s.elements.filter(n=>ui.under(n,sheet)&&["AXButton","AXCheckBox"].includes(n.role)&&n.label===spaceTitle).length===1)return;
      return ui.confirm(s,NAME)?{warning:true}:undefined;
    },8000);
    assert("Profiles creation closes or shows only exact destination warning",completed!==undefined);
    if(completed!.warning)await ui.confirmChange(NAME);
    await ui.waitNoSheet();await ui.openChange();const after=await ui.inventory(2);ui.expectedNames(after.names,2);
    assert("Profiles creation assignment is one of two known profiles",[originalProfile,NAME].includes(after.selected));
    if(after.selected===NAME){ui.cancelChange();await ui.waitNoSheet();}else await ui.switchFromEditor(NAME,after.selected);
    const newFixture=await ui.fixture("created profile");assert("new profile fixture target differs from original document",newFixture!==firstFixture);
    ui.marker(newFixture,null,"new isolated profile");ui.setMarker(newFixture,NEW);
    await ui.switchTo(originalProfile);const oldFixture=await ui.fixture("returned original");
    assert("returned original fixture differs from new profile target",oldFixture!==newFixture);ui.marker(oldFixture,OLD,"returned original");
    save("first-launch.json",{nonce,pid:firstPID,originalProfile,newProfile:NAME,spaceTitle,sidebarName,targets:{initial:firstFixture,new:newFixture,returned:oldFixture},scope:"Native create/assign/switch and isolated fixture localStorage. No private context, data import, callback or boundary-parity claim."});

    const panelFlow=panelControls(ui,identity(firstPID!),oldFixture);await panelFlow.round("first",true);

  }finally{await c.quit(first,"Little Arc first standard graceful quit");}
  assert("Little Arc setup captured both exact profiles and directory",originalProfile!==undefined&&spaceTitle!==undefined&&sidebarName!==undefined&&profileIdentity!==undefined);
  inspectProfile("before restart",true);policy.recheck();const second=c.launch();
  try{
    assert("Little Arc restart ready",await c.ready(second,"Little Arc restart launch"));const pid=second.pid();assert("Little Arc restart genuinely new PID",pid!==undefined&&pid!==firstPID);
    inspectProfile("second launch",true);const ui=controls(second,identity(pid!));await ui.ensureWindow();ui.oneSpace();
    await ui.openChange();const restored=await ui.inventory(2);ui.expectedNames(restored.names,2);assert("Little Arc restart retains original main profile selection",restored.selected===originalProfile);ui.cancelChange();await ui.waitNoSheet();
    const originalTarget=await ui.fixture("restarted main original");ui.marker(originalTarget,OLD,"restarted original main");
    const panelFlow=panelControls(ui,identity(pid!),originalTarget);await panelFlow.round("restart",false);
    assert("Little Arc fixed five logical external deliveries and two zero-send inspections",deliveryIndex===5&&helperSequence===7);
    inspectProfile("completed restart",true);ui.owner();policy.recheck();save("restart.json",{nonce,profile:profileCanonical,profileIdentity,firstPID,secondPID:pid,originalProfile,newProfile:NAME,spaceTitle,sidebarName,originalTarget,policyReceiptSHA256:policy.receiptSHA256,scope:"WRITE-only runtime-unverified native flow. Future actual control exposure/confirmation behavior, external profile defaults, panel replacement and marker persistence may fail/inconclusive. Complete standard recorder bytes must be retained; no invented callback/raw parity gate."});
  }finally{await c.quit(second,"Little Arc restart standard graceful quit");}
}
