import type { ScenarioContext } from "../launch-slice/run";
import { verifyAgentEndpoint } from "../../../arc/tools/agent-cdp";
import { mkdirSync, readFileSync, renameSync, writeFileSync } from "node:fs";
import { join } from "node:path";
import { nativePage, page, observed, set, windows } from "./native";

export const morningPlan = [
  "After unlock, record this scenario's in-process NSApplication.sendEvent key-equivalent routing.",
  "Validate all seven fixed native UI outcomes and exact PID/window keyDown/keyUp receipts.",
  "Local monitor observations are retained when AppKit exposes them; no hardware/WindowServer delivery is claimed.",
];
const TARGET = "ArcCore shortcut target 7f3a", SENTINEL = "ArcCore shortcut sentinel 7f3a", NEW_TAB = "ArcCore shortcut new tab 7f3a";
export const fixture = (request:Request) => {
  const title:Record<string,string> = {"/keys-target":TARGET,"/keys-sentinel":SENTINEL,"/keys-created":NEW_TAB};
  const name=title[new URL(request.url).pathname];
  return name ? new Response(`<!doctype html><title>${name}</title><p>${name}</p>`,{headers:{"content-type":"text/html"}}) : undefined;
};
type Receipt = {pid:number;sequence:number;shortcut:string;windowIdentifier:string;windowNumber:number;mainThread:boolean;
  keyWindowMatchedBefore:boolean;downReturned:boolean;upReturned:boolean;localMonitorInstalled:boolean;localMonitorSawDown:boolean;localMonitorSawUp:boolean};
type Reply = {id:string;pid:number;nonce:string;shortcut:string;ok?:boolean;error?:string;receipt?:Receipt};
type Target = {id:string;url:string;title:string};

export default async function scenario(c: ScenarioContext) {
  const assert=(name:string,pass:unknown,detail="")=>{c.assert(name,pass,detail);if(!pass)throw new Error(name);};
  assert("app-event key routing uses disposable engine test agent",["trace156","ours156"].includes(c.slug)&&!c.traced);
  const directory=join(c.outDir,"key-driver");mkdirSync(directory,{recursive:true,mode:0o700});
  const dylib=join(directory,"ScenarioKeyEvents.dylib");
  const compile=Bun.spawnSync(["xcrun","clang","-dynamiclib","-fobjc-arc","-Wall","-Wextra","-Werror","-framework","Foundation","-framework","AppKit",
    join(import.meta.dir,"arcmod-key-driver.m"),"-o",dylib],{stdout:"pipe",stderr:"pipe"});
  writeFileSync(join(directory,"compile.log"),compile.stdout.toString()+compile.stderr.toString());
  assert("app-event key driver compiles with strict warnings",compile.success,compile.stderr.toString().slice(-1500));
  const nonce=`arcmod-keys-${crypto.randomUUID()}`,fixtureURL=`${c.base}/keys-target`;
  const launch=c.launch({libraries:[dylib],environment:{ARC_SCENARIO_KEYS_DIRECTORY:directory,ARC_SCENARIO_KEYS_NONCE:nonce,
    ARC_SCENARIO_KEYS_AGENT:c.slug,ARC_SCENARIO_KEYS_URL:fixtureURL}});
  const receipts:Receipt[]=[];
  try{
    if(!await c.ready(launch,"app-event key agent launch"))return;
    const pid=launch.pid();assert("app-event key launch exposes an owned PID",pid!==undefined);
    const owner=()=>{if(launch.pid()!==pid||verifyAgentEndpoint(c.slug,c.port)!==pid)throw new Error("key agent PID/endpoint changed");};
    const ready=await c.waitFor(()=>{const value=JSON.parse(readFileSync(join(directory,"ready.json"),"utf8")) as {pid?:number;nonce?:string;agent?:string;installed?:boolean};
      return value.pid===pid&&value.nonce===nonce&&value.agent===c.slug&&value.installed===true?value:undefined;},10000);
    assert("app-event key driver ready in current PID",ready!==undefined);
    const key=async(shortcut:string,windowIdentifier:string)=>{
      owner();const id=`${nonce}-${receipts.length+1}`,path=join(directory,"request.json");
      writeFileSync(path+".tmp",JSON.stringify({id,pid,nonce,url:fixtureURL,shortcut,windowIdentifier}),{mode:0o600});renameSync(path+".tmp",path);
      const reply=await c.waitFor(()=>{const value=JSON.parse(readFileSync(join(directory,"response.json"),"utf8")) as Reply;
        return value.id===id&&value.pid===pid&&value.nonce===nonce&&value.shortcut===shortcut?value:undefined;},15000);
      if(!reply)throw new Error(`key ${shortcut} timed out; uncertain input will not be replayed`);
      owner();if(reply.ok!==true||!reply.receipt)throw new Error(`key ${shortcut}: ${reply.error??"missing receipt"}`);
      const receipt=reply.receipt;receipts.push(receipt);writeFileSync(join(directory,"receipts.json"),JSON.stringify(receipts,null,2));
      assert(`${shortcut} sent local keyDown and keyUp to exact agent window`,receipt.pid===pid&&receipt.sequence===receipts.length&&receipt.shortcut===shortcut&&
        receipt.windowIdentifier===windowIdentifier&&receipt.windowNumber>0&&receipt.mainThread&&receipt.keyWindowMatchedBefore&&receipt.downReturned&&receipt.upReturned,
        `local monitor observed down=${receipt.localMonitorSawDown} up=${receipt.localMonitorSawUp}; app-event routing only`);
    };
    const targets=()=>c.cdp("list") as Target[];
    const waitNative=async(name:string,predicate:()=>boolean)=>assert(name,await c.waitFor(()=>predicate()?true:undefined,8000));
    const select=async(title:string,url:string)=>{
      owner();const state=c.native();assert("native fixture observation belongs to exact PID",state.pid===pid);
      const controls=state.elements.filter(n=>n.role==="AXButton"&&n.enabled&&n.label===title);
      if(controls.length!==1)throw new Error("fixture tab is ambiguous in native UI");
      const control=controls[0]!,action=control.actions.find(name=>name==="AXPress"||name.startsWith("Name:press"));
      if(!action)throw new Error("fixture tab has no advertised press action");c.perform(control,action);
      await waitNative("exact fixture URL visibly selected",()=>c.native().elements.some(n=>n.identifier==="contentPanelTextField"&&n.value===url));
      const selected=c.native(),address=selected.elements.filter(n=>n.identifier==="contentPanelTextField"&&n.value===url);
      if(address.length!==1)throw new Error("selected fixture URL has ambiguous native address controls");
      const owners=selected.elements.filter(n=>n.role==="AXWindow"&&address[0]!.path.startsWith(n.path+"."));
      if(owners.length!==1||!owners[0]!.identifier.startsWith("bigBrowserWindow-")||!owners[0]!.actions.includes("AXRaise"))throw new Error("fixture has no uniquely observed raisable full browser window");
      c.perform(owners[0]!,"AXRaise");return owners[0]!.identifier;
    };
    const commandField=()=>observed(c,n=>n.identifier==="commandBarTextField"&&n.role==="AXTextField");
    // Arc's command bar field advertises AXConfirm, but performing it neither navigates nor opens a tab (on Arc's
    // own engine the Cmd+T fixture never loaded). Commit it with Return through the same in-process key route.
    const confirmCommand=async(windowIdentifier:string)=>{observed(c,n=>n.identifier==="commandBarTextField"&&n.enabled);await key("return",windowIdentifier);};
    const sentinel=await page(c,"/keys-sentinel",SENTINEL);
    const target=await nativePage(c,"/keys-target",TARGET);
    assert("one initial full browser window",windows(c).length===1&&windows(c)[0]!.identifier.startsWith("bigBrowserWindow-"));
    const browser=await select(TARGET,fixtureURL);
    await key("cmd-t",browser);
    await waitNative("Cmd+T opens empty command bar",()=>c.native().elements.some(n=>n.identifier==="commandBarTextField"&&n.value===""));
    const newURL=`${c.base}/keys-created`;set(c,n=>n.identifier==="commandBarTextField",newURL);await confirmCommand(browser);
    const created=await c.waitFor(()=>{const matches=targets().filter(t=>t.url===newURL&&t.title===NEW_TAB);return matches.length===1?matches[0]:undefined;},10000);
    assert("Cmd+T creates exact fixture through confirmed command bar",created!==undefined&&created.id!==target&&created.id!==sentinel);
    await select(TARGET,fixtureURL);await key("cmd-l",browser);
    await waitNative("Cmd+L opens selected fixture URL in command bar",()=>c.native().elements.some(n=>n.identifier==="commandBarTextField"&&n.value===fixtureURL));
    assert("Cmd+L command field is enabled",commandField().enabled);await confirmCommand(browser);
    await waitNative("Cmd+L command bar closes after confirming same URL",()=>!c.native().elements.some(n=>n.identifier==="commandBarTextField"));
    // Confirming the same URL may reload it, and the title is briefly unset while the document commits.
    assert("Cmd+L confirmation preserves original exact target",await c.waitFor(()=>targets().some(t=>t.id===target&&t.url===fixtureURL&&t.title===TARGET)?true:undefined,8000));
    await select(TARGET,fixtureURL);await key("cmd-comma",browser);
    await waitNative("Cmd+comma opens one Settings window",()=>windows(c).length===2&&windows(c).filter(n=>n.identifier!==browser).length===1);
    const settings=windows(c).filter(n=>n.identifier!==browser);if(settings.length!==1)throw new Error("Settings window became ambiguous");
    c.action(observed(c,n=>n.path.startsWith(settings[0]!.path+".")&&n.subrole==="AXCloseButton"&&n.actions.includes("AXPress")));
    await waitNative("Settings close returns to sole browser window",()=>windows(c).length===1&&windows(c)[0]!.identifier===browser);
    // OPEN (not passing on Arc's engine): this in-process Cmd+W makes Arc dispatch closeWindow: (Close Window,
    // Shift+Cmd+W in the menu) rather than closeCurrentItem: (Archive Tab, Cmd+W), per --arcmod-shortcut-diagnostics,
    // so the window and all three tabs close. Both menu items were enabled with masks Cmd+Shift and Cmd. It
    // happened with Settings before or after Cmd+W and with the tab selected via DevTools or Tabs > Next Tab.
    // A hardware Cmd+W archives the tab (notes/shortcut-audit.md), as does the Archive Tab menu item (tabs-restore).
    await select(TARGET,fixtureURL);await key("cmd-w",browser);
    assert("Cmd+W closes selected exact fixture target",await c.waitFor(()=>!targets().some(t=>t.id===target)?true:undefined,8000));
    {const after=targets();assert("Cmd+W preserves sentinel and separately created fixture",after.some(t=>t.id===sentinel)&&after.some(t=>t.id===created!.id),
      JSON.stringify({sentinel,created:created!.id,after,windows:windows(c).map(n=>n.identifier),tabs:c.native().elements.filter(n=>n.role==="AXButton"&&n.label.startsWith("ArcCore shortcut")).map(n=>n.label)}));}
    await key("cmd-shift-t",browser);
    const restored=await c.waitFor(()=>{const matches=targets().filter(t=>t.url===fixtureURL&&t.title===TARGET);return matches.length===1?matches[0]:undefined;},10000);
    assert("Cmd+Shift+T restores exact document in a new target",restored!==undefined&&restored.id!==target);
    await select(TARGET,fixtureURL);await key("cmd-n",browser);
    await waitNative("Cmd+N creates second full browser window",()=>{const current=windows(c);return current.length===2&&current.every(n=>n.identifier.startsWith("bigBrowserWindow-"));});
    const additional=windows(c).filter(n=>n.identifier!==browser);if(additional.length!==1||!additional[0]!.actions.includes("AXRaise"))throw new Error("new full browser window is not uniquely raisable");
    c.perform(additional[0]!,"AXRaise");await key("cmd-option-n",additional[0]!.identifier);
    await waitNative("Cmd+Option+N follows agent redirect to third full window",()=>{const current=windows(c);return current.length===3&&current.every(n=>n.identifier.startsWith("bigBrowserWindow-"));});
    assert("all seven whitelisted app-event shortcuts completed",receipts.filter(r=>r.shortcut!=="return").length===7&&receipts.filter(r=>r.shortcut==="return").length===2);
  }finally{
    writeFileSync(join(directory,"receipts.json"),JSON.stringify(receipts,null,2));
    await c.quit(launch,"app-event key agent quit");
  }
}
