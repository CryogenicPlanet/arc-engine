// WRITE-only morning scenario: ready for native recording, runtime unverified.
// Static evidence: ../arc/dump/strings.txt:46651-46657 names the creation IDs;
// ../arc/ghidra/decomp/0x1025.c:100541-100594 constructs the Create Space button
// and binds accessibility ID enum0x4a; 0x100b.c:28611-28616 maps it to the ID below.
// The name binding/Space name… placeholder is at0x1025.c:107328-107380.
// Field-ID catalog evidence is not proof of a live settable editor: require it.
// Accepted context-menus/native-1.json:547-555 exposes an EMPTY sidebarSpaceName;
// :3902/:3919 expose Previous/Next Space, initially disabled. No adjacency or
// default-tab count is inferred without the checked one-to-two Space guards.
import type { ScenarioContext, NativeNode, NativeSnapshot } from "../launch-slice/run";
import { verifyAgentEndpoint } from "../../../arc/tools/agent-cdp";
import { isTabFallback } from "./native";
import { appendFileSync, existsSync, lstatSync, mkdirSync, readdirSync, realpathSync, statSync, writeFileSync } from "node:fs";
import { join, resolve } from "node:path";

const NAME="ArcCore Fixture Space";
export const morningPlan = [
  "Require one initial disposable Space and preserve its observed name, including an empty name.",
  "Create ArcCore Fixture Space once through its exact owned, settable native editor and advertised Create Space button.",
  "Require exactly two Space menu entries, select fixture B in the new Space, then Previous returns original/A and Next returns new/B.",
  "Quit normally, relaunch the same disposable profile in a new PID, and repeat both Space/tab checks using fresh native handles.",
  "Fail closed on absent controls, uncertain actions, unexpected Space counts or placement; do not infer default-tab count or profile assignment.",
];
type Target={id:string;url:string;title:string};
type Snapshot=NativeSnapshot&{bundle?:string;webContentExcluded?:boolean};
type Receipt={pid?:number;bundle?:string;action?:string;dispatched?:boolean};

export default async function scenario(c:ScenarioContext) {
  const assert=(name:string,ok:unknown,detail="")=>{c.assert(name,ok,detail);if(!ok)throw new Error(name+(detail?": "+detail:""));};
  assert("Space scenario uses exact disposable standard agent",["trace156","ours156"].includes(c.slug)&&!c.traced);
  const support=join(process.env.HOME!,"Library/Application Support"),profile=join(support,`ArcModAgent-${c.slug}`);
  assert("Space scenario uses exact disposable profile path",c.data===profile);
  // Resolve only the known Application Support anchor. Never resolve a profile
  // or storage-child symlink and then decide whether its target is disposable.
  const supportCanonical=realpathSync(support),profileCanonical=join(supportCanonical,`ArcModAgent-${c.slug}`);
  let profileIdentity:{device:number;inode:number}|undefined;
  const directoryInfo=(path:string)=>{
    try{return lstatSync(path);}
    catch(error){if((error as NodeJS.ErrnoException).code==="ENOENT")return;throw error;}
  };
  const inspectProfile=(phase:string,required:boolean,empty=false)=>{
    assert(`Space ${phase} retains canonical Application Support anchor`,realpathSync(support)===supportCanonical);
    const info=directoryInfo(profile);
    if(!info){assert(`Space ${phase} may omit only an uncreated disposable profile`,!required&&profileIdentity===undefined);return;}
    assert(`Space ${phase} profile is a real directory without symlink`,!info.isSymbolicLink()&&info.isDirectory());
    assert(`Space ${phase} canonical profile is below exact known parent`,realpathSync(profile)===profileCanonical);
    for(const name of ["User Data","Agent Chromium Data"]){
      const path=join(profile,name),child=directoryInfo(path);
      assert(`Space ${phase} ${name} is absent or a real directory`,child===undefined||(!child.isSymbolicLink()&&child.isDirectory()));
      if(empty)assert(`Space ${phase} ${name} starts empty`,child===undefined||readdirSync(path).length===0);
    }
    const current=directoryInfo(profile);
    assert(`Space ${phase} profile identity survives no-follow inspection`,current!==undefined&&!current.isSymbolicLink()&&current.isDirectory()&&current.dev===info.dev&&current.ino===info.ino);
    const identity={device:info.dev,inode:info.ino};
    assert(`Space ${phase} retains disposable directory identity`,profileIdentity===undefined||(identity.device===profileIdentity.device&&identity.inode===profileIdentity.inode));
    return identity;
  };
  profileIdentity=inspectProfile("prelaunch",false,true);
  const nativeHelper=resolve(import.meta.dir,"../../../arc/build/agent-native"),nativeSource=resolve(import.meta.dir,"../../../arc/tools/agent-native.swift");
  assert("Space scenario has current native helper without auto-build",existsSync(nativeHelper)&&statSync(nativeHelper).mtimeMs>=statSync(nativeSource).mtimeMs);
  const directory=join(c.outDir,"spaces-evidence");
  assert("Space evidence directory is fresh",!existsSync(directory)||readdirSync(directory).length===0);
  mkdirSync(directory,{recursive:true,mode:0o700});
  const nonce=crypto.randomUUID(),bundle=`company.thebrowser.Browser.mod.agent.${c.slug}`;
  let actionSequence=0;
  let originalName:string|undefined,firstPID:number|undefined;
  let firstA:string|undefined,firstB:string|undefined;
  const controls=(launch:ReturnType<ScenarioContext["launch"]>,pid:number)=>{
    const owner=()=>{if(launch.pid()!==pid||verifyAgentEndpoint(c.slug,c.port)!==pid)throw new Error("Space scenario PID/endpoint ownership changed");};
    const state=()=>{
      owner();const value=c.native() as Snapshot;owner();
      if(value.pid!==pid||value.bundle!==bundle||value.truncated!==false||value.webContentExcluded!==true)throw new Error("Space snapshot identity/scope/truncation gap");
      return value;
    };
    const window=(s:Snapshot)=>{
      const values=s.elements.filter(n=>n.role==="AXWindow");
      if(values.length!==1||!values[0]!.identifier.startsWith("bigBrowserWindow-"))throw new Error("Space scenario requires one observed full browser window");
      return values[0]!;
    };
    const under=(node:NativeNode,parent:NativeNode)=>node.path.startsWith(parent.path+".");
    const unique=(s:Snapshot,predicate:(n:NativeNode)=>boolean,what:string)=>{
      const values=s.elements.filter(predicate);if(values.length!==1)throw new Error(`${what}: expected one observed native control, got ${values.length}`);return values[0]!;
    };
    const dispatch=(node:NativeNode,kind:"press"|"set"|"perform",value?:string)=>{
      owner();if(++actionSequence>32)throw new Error("Space native action budget exhausted");
      if(!node.enabled||(kind==="set"?!node.valueSettable:!node.actions.includes(kind==="perform"?value!:"AXPress")))throw new Error("Space native action is not enabled/advertised/settable");
      const intent={nonce,pid,sequence:actionSequence,identifier:node.identifier,path:node.path,kind,value,maximumAttempts:1,retries:false};
      appendFileSync(join(directory,"actions.jsonl"),JSON.stringify({event:"intent",...intent})+"\n");
      // No native snapshot between selection and dispatch: opaque handles expire
      // whenever NativeController.state() runs. Uncertain dispatch is not retried.
      try{
        const result=(kind==="perform"?c.perform(node,value!):c.action(node,kind,value)) as {receipt?:Receipt};
        appendFileSync(join(directory,"actions.jsonl"),JSON.stringify({event:"receipt",...intent,receipt:result.receipt??null})+"\n");
        owner();const receipt=result.receipt;
        // A sidebar tab press falls back to DevTools (native.ts isTabFallback); that receipt has no AX
        // pid/bundle fields, and owner() above already re-verified the endpoint belongs to this PID.
        if(kind==="perform"&&isTabFallback(value,receipt))return;
        if(!receipt||receipt.pid!==pid||receipt.bundle!==bundle||receipt.action!==kind||receipt.dispatched!==true)throw new Error("Space native action receipt identity/dispatch mismatch");
      }catch(error){appendFileSync(join(directory,"actions.jsonl"),JSON.stringify({event:"uncertain-no-retry",...intent,error:String(error)})+"\n");throw error;}
    };
    // Retry observations only; ownership, transport and malformed state errors
    // propagate rather than being swallowed by the harness's generic waitFor.
    const poll=async<T>(read:()=>T|undefined,ms=8000):Promise<T|undefined>=>{
      const end=Date.now()+ms;while(Date.now()<end){owner();const value=read();owner();if(value!==undefined)return value;await c.sleep(250);}return;
    };
    const ensureWindow=async()=>{
      // Arc creates its first window asynchronously after the readiness record, and AX can report the window
      // before exposing it as AXWindow. Wait for it instead of opening a second one.
      const s=(await poll(()=>{const v=state();return v.elements.some(n=>n.role==="AXWindow")?v:undefined;},5000))??state();
      if(s.elements.some(n=>n.role==="AXWindow")){window(s);return;}
      const item=unique(s,n=>n.identifier==="newWindow"&&n.role==="AXMenuItem"&&n.enabled&&n.actions.includes("AXPress"),"bounded New Window setup");
      dispatch(item,"press");
      if(!await poll(()=>{const value=state();if(!value.elements.some(n=>n.role==="AXWindow"))return;window(value);return true;},5000))throw new Error("Space browser window did not appear after sole New Window attempt");
    };
    // Arc's sidebar is a paged list that keeps every Space's page (its sidebarSpaceName and tab list) in the AX
    // tree, with no AX selection state (observed on Arc's engine with two Spaces). So with several pages, the
    // current Space is the page holding the tab shown in the content panel; with no tab selected (a new, empty
    // Space) the window title is the current Space's name. Returns undefined when that cannot be determined.
    const fixtureTitles:Record<string,string>={[`${c.base}/a`]:c.EXPECT.aTitle,[`${c.base}/b`]:c.EXPECT.bTitle};
    const sidebarName=(s:Snapshot):string|undefined=>{
      const browser=window(s),names=s.elements.filter(n=>under(n,browser)&&n.identifier==="sidebarSpaceName"&&n.role==="AXStaticText");
      if(!names.length||names.some(n=>typeof n.value!=="string"))throw new Error("Space pages lack string sidebar names");
      if(names.length===1)return names[0]!.value as string;
      const pages=names.map(n=>({name:n.value as string,page:n.path.slice(0,n.path.lastIndexOf("."))}));
      const address=s.elements.filter(n=>under(n,browser)&&n.identifier==="contentPanelTextField");
      if(!address.length){const titled=pages.filter(p=>p.name===browser.label);return titled.length===1?titled[0]!.name:undefined;}
      const title=address.length===1&&typeof address[0]!.value==="string"?fixtureTitles[address[0]!.value]:undefined;
      const tabs=title===undefined?[]:s.elements.filter(n=>under(n,browser)&&n.role==="AXButton"&&n.label===title);
      const owners=tabs.length===1?pages.filter(p=>tabs[0]!.path.startsWith(p.page+".")):[];
      return owners.length===1?owners[0]!.name:undefined;
    };
    const selected=(s:Snapshot,name:string,url:string)=>{
      const browser=window(s),address=s.elements.filter(n=>under(n,browser)&&n.identifier==="contentPanelTextField"&&n.value===url);
      return sidebarName(s)===name&&address.length===1;
    };
    const menu=()=>{
      let s=state();window(s);
      const bar=unique(s,n=>n.role==="AXMenuBarItem"&&n.label==="Spaces"&&n.enabled&&n.actions.includes("AXPress"),"Spaces menu");
      dispatch(bar,"press");s=state();
      const opened=unique(s,n=>n.role==="AXMenuBarItem"&&n.label==="Spaces","opened Spaces menu");
      return {s,items:s.elements.filter(n=>under(n,opened)&&n.role==="AXMenuItem")};
    };
    const spaceMenu=async(count:number)=>{
      menu();
      const inventory=await poll(()=>{
        const s=state(),bar=unique(s,n=>n.role==="AXMenuBarItem"&&n.label==="Spaces","opened Spaces menu");
        const items=s.elements.filter(n=>under(n,bar)&&n.role==="AXMenuItem"),spaces=items.filter(n=>n.identifier==="focusSpace:");
        return spaces.length===count?{s,items,spaces}:undefined;
      },5000);
      assert(`Space menu has exactly ${count} entries`,inventory!==undefined);
      assert("Space menu labels are unique",new Set(inventory!.spaces.map(n=>n.label)).size===inventory!.spaces.length);
      return inventory!;
    };
    const closeSpaceMenu=(s:Snapshot)=>{
      const bar=unique(s,n=>n.role==="AXMenuBarItem"&&n.label==="Spaces","owned Spaces menu bar");
      const active=unique(s,n=>under(n,bar)&&n.role==="AXMenu"&&n.enabled&&n.actions.includes("AXCancel"),"owned cancellable Spaces menu");
      dispatch(active,"perform","AXCancel");
    };
    const targets=()=>{owner();const values=c.cdp("list") as Target[];owner();return values;};
    const exactTargets=()=>{
      const values=targets(),a=values.filter(t=>t.url===`${c.base}/a`&&t.title===c.EXPECT.aTitle),b=values.filter(t=>t.url===`${c.base}/b`&&t.title===c.EXPECT.bTitle);
      assert("both exact Space fixtures have unique CDP targets",a.length===1&&b.length===1&&a[0]!.id!==b[0]!.id);return {a:a[0]!,b:b[0]!};
    };
    const nativePage=async(path:"/a"|"/b",title:string)=>{
      owner();const url=c.base+path,target=c.open(url);owner();
      assert(`Space fixture ${path} loads exact document`,await c.titleIs(target,title)&&c.evaluate(target,"location.href")===url);owner();
      await ensureWindow();let s=state(),browser=window(s);
      const tab=unique(s,n=>under(n,browser)&&n.role==="AXButton"&&n.enabled&&n.label===title,"owned fixture tab");
      const action=tab.actions.find(a=>a==="AXPress"||a.startsWith("Name:press"));
      if(!action)throw new Error("owned Space fixture tab has no advertised press action");dispatch(tab,"perform",action);
      assert(`Space fixture ${path} is selected in native address`,await poll(()=>{const value=state(),win=window(value);return value.elements.filter(n=>under(n,win)&&n.identifier==="contentPanelTextField"&&n.value===url).length===1?true:undefined;},5000));
      // Same own-window advertised AXRaise used by native.ts:nativePage. Never
      // activate an application, synthesize keys or target a foreign window.
      s=state();browser=window(s);if(browser.actions.includes("AXRaise"))dispatch(browser,"perform","AXRaise");
      return target;
    };
    const switchTo=async(label:"Previous Space"|"Next Space",name:string,url:string)=>{
      const before=sidebarName(state());assert(`${label} starts in distinct known Space`,before!==undefined&&before!==name);
      const {items,spaces}=await spaceMenu(2);
      assert("created Space remains uniquely named in menu",spaces.filter(n=>n.label===NAME).length===1);
      const matches=items.filter(n=>n.label===label&&n.identifier===(label==="Previous Space"?"_NS:1108":"_NS:808")&&n.enabled&&n.actions.includes("AXPress"));
      if(matches.length!==1)throw new Error(`${label} lacks one enabled exact observed menu action`);
      dispatch(matches[0]!,"press");
      assert(`${label} returns exact Space and selected fixture`,await poll(()=>selected(state(),name,url)?true:undefined));
    };
    return {owner,state,window,under,unique,dispatch,poll,ensureWindow,sidebarName,selected,menu,spaceMenu,closeSpaceMenu,targets,exactTargets,nativePage,switchTo};
  };
  const first=c.launch();
  try{
    if(!await c.ready(first,"Space initial launch"))return;
    firstPID=first.pid();assert("Space initial launch exposes actual PID",firstPID!==undefined);
    const ui=controls(first,firstPID!);
    profileIdentity=inspectProfile("initial launched profile",true);
    firstA=await ui.nativePage("/a",c.EXPECT.aTitle);
    originalName=ui.sidebarName(ui.state());
    assert("original Space name is distinct from fixed new name",originalName!==undefined&&originalName!==NAME);
    const initial=await ui.spaceMenu(1);writeFileSync(join(directory,"initial-space.json"),JSON.stringify({nonce,pid:firstPID,profile:profileCanonical,profileIdentity,originalName,menuLabels:initial.spaces.map(n=>n.label)},null,2));
    const createItems=initial.items.filter(n=>n.identifier==="_NS:1195"&&n.label==="New Space…"&&n.enabled&&n.actions.includes("AXPress"));
    assert("Space creation has one exact enabled New Space menu action",createItems.length===1);ui.dispatch(createItems[0]!,"press");
    // Arc's SwiftUI name field does not expose its spaceCreationSpaceNameTextField ID over AX (observed on
    // Arc's engine); only the Create Space button carries its ID. The name field is the text field inside the
    // same creation view as that button.
    const nameFields=(s:Snapshot,browser:NativeNode)=>{
      const create=s.elements.filter(n=>ui.under(n,browser)&&n.identifier==="spaceCreationCreateSpaceButton"&&n.role==="AXButton");
      if(create.length!==1)return [];
      const view=create[0]!.path.slice(0,create[0]!.path.lastIndexOf("."));
      return s.elements.filter(n=>n.path.startsWith(view+".")&&(n.identifier==="spaceCreationSpaceNameTextField"||n.role==="AXTextField"));
    };
    const editor=await ui.poll(()=>{const s=ui.state(),browser=ui.window(s),matches=nameFields(s,browser);return matches.length?{s,browser,matches}:undefined;},5000);
    assert("Space name editor appears in owned browser window",editor!==undefined);
    const fields=editor!.matches;
    assert("Space name editor is unique enabled nonsecure and actually settable",fields.length===1&&fields[0]!.role==="AXTextField"&&fields[0]!.subrole!=="AXSecureTextField"&&fields[0]!.enabled&&fields[0]!.valueSettable&&typeof fields[0]!.value==="string");
    ui.dispatch(fields[0]!,"set",NAME);
    assert("Space name setter has exact native readback",await ui.poll(()=>{const s=ui.state(),browser=ui.window(s),matches=nameFields(s,browser);return matches.length===1&&matches[0]!.value===NAME?true:undefined;},5000));
    const button=await ui.poll(()=>{
      const s=ui.state(),browser=ui.window(s),matches=s.elements.filter(n=>ui.under(n,browser)&&n.identifier==="spaceCreationCreateSpaceButton"&&n.role==="AXButton");
      if(matches.length>1)throw new Error("Create Space button became ambiguous");
      return matches.length===1&&matches[0]!.enabled&&matches[0]!.actions.includes("AXPress")?matches[0]:undefined;
    },5000);
    assert("owned Create Space button becomes enabled and advertises AXPress",button!==undefined);
    ui.dispatch(button!,"press");
    assert("Space creation selects fixed new Space name",await ui.poll(()=>ui.sidebarName(ui.state())===NAME?true:undefined));
    const created=await ui.spaceMenu(2);
    assert("Space creation adds one uniquely fixed menu name",created.spaces.filter(n=>n.label===NAME).length===1);
    ui.closeSpaceMenu(created.s);
    firstB=await ui.nativePage("/b",c.EXPECT.bTitle);
    assert("fixture B is selected inside fixed new Space",ui.selected(ui.state(),NAME,`${c.base}/b`));
    const targets=ui.exactTargets();assert("native Space fixtures retain their created CDP IDs",targets.a.id===firstA&&targets.b.id===firstB);
    // OPEN (not passing on Arc's engine): after Previous Space, Space 1 shows no selected tab (window title
    // "Space 1", no content-panel address) although fixture A was selected there before creating the new Space,
    // whether A was selected via the DevTools fallback or via Tabs > Next Tab.
    await ui.switchTo("Previous Space",originalName!,`${c.base}/a`);
    await ui.switchTo("Next Space",NAME,`${c.base}/b`);
    ui.exactTargets();ui.owner();
  }finally{await c.quit(first,"Space initial standard graceful quit");}
  assert("Space initial launch established disposable directory identity",profileIdentity!==undefined);
  inspectProfile("before restart",true);
  const second=c.launch();
  try{
    if(!await c.ready(second,"Space restart launch"))return;
    const pid=second.pid();assert("Space restart is a genuinely new owned PID",pid!==undefined&&pid!==firstPID);
    assert("Space restart keeps exact disposable lexical profile path",c.data===profile);
    inspectProfile("second launched profile",true);
    const ui=controls(second,pid!);await ui.ensureWindow();
    // CDP target IDs may change across real restart. Require fixed URL/title
    // identities, not equality with the previous process's transient IDs.
    // Arc restores tabs lazily: a tab gets a page (CDP target) only once it is shown (observed on Arc's engine
    // in arcmod-profiles; this restart branch has not run yet). So check right away that both tabs are restored in their own Space pages, and check their CDP
    // documents after the Previous/Next round trip below has shown both Spaces.
    assert("both Space fixture tabs survive real process restart in their own Spaces",await ui.poll(()=>{
      const s=ui.state(),browser=ui.window(s),pages=s.elements.filter(n=>ui.under(n,browser)&&n.identifier==="sidebarSpaceName");
      const page=(title:string)=>{const tabs=s.elements.filter(n=>ui.under(n,browser)&&n.role==="AXButton"&&n.label===title);
        return tabs.length===1?pages.filter(p=>tabs[0]!.path.startsWith(p.path.slice(0,p.path.lastIndexOf("."))+".")).map(p=>p.value):[];};
      const a=page(c.EXPECT.aTitle),b=page(c.EXPECT.bTitle);
      return a.length===1&&a[0]===originalName&&b.length===1&&b[0]===NAME?true:undefined;
    },10000));
    const inventory=await ui.spaceMenu(2),named=inventory.spaces.filter(n=>n.label===NAME&&n.enabled&&n.actions.includes("AXPress"));
    assert("fixed new Space menu entry persists uniquely after restart",named.length===1);
    // A newly opened agent window need not restore the last selected Space.
    // Selecting the exact observed named entry is one deliberate action, never
    // a retry of an uncertain Previous/Next operation.
    ui.dispatch(named[0]!,"press");
    assert("restarted named Space restores selected fixture B",await ui.poll(()=>ui.selected(ui.state(),NAME,`${c.base}/b`)?true:undefined));
    await ui.switchTo("Previous Space",originalName!,`${c.base}/a`);
    await ui.switchTo("Next Space",NAME,`${c.base}/b`);
    await ui.poll(()=>{const v=ui.targets();return ["/a","/b"].every(path=>v.some(t=>t.url===c.base+path))?true:undefined;},10000);
    const targets=ui.exactTargets();
    assert("restarted Space fixture documents have fixed content",c.evaluate(targets.a.id,"location.href")===`${c.base}/a`&&c.evaluate(targets.a.id,"document.title")===c.EXPECT.aTitle&&c.evaluate(targets.b.id,"location.href")===`${c.base}/b`&&c.evaluate(targets.b.id,"document.title")===c.EXPECT.bTitle);ui.owner();
    writeFileSync(join(directory,"restart.json"),JSON.stringify({nonce,profile:profileCanonical,profileIdentity,firstPID,secondPID:pid,originalName,newName:NAME,firstTargets:{a:firstA,b:firstB},restoredTargets:targets,scope:"Two checked Spaces; native creation/name, previous/next selected fixture persistence. No default-tab count, profile assignment or boundary-parity claim."},null,2));
  }finally{await c.quit(second,"Space restart standard graceful quit");}
}
