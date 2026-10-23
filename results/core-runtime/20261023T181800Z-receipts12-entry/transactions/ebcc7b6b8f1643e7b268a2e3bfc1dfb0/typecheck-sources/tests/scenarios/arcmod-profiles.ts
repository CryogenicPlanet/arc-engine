// WRITE-only morning scenario: ready for native recording, runtime unverified.
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
import { verifyAgentEndpoint } from "../../../arc/tools/agent-cdp";
import { nativeState } from "../../../arc/tools/agent-native";
import { appendFileSync, chmodSync, existsSync, lstatSync, mkdirSync, readdirSync, realpathSync, statSync, writeFileSync } from "node:fs";
import { join, resolve } from "node:path";

const NAME="ArcCore Fixture Profile",KEY="profileMarker",OLD="arccore-original-7f3a",NEW="arccore-created-7f3a";
export const morningControls={nameField:"importMenuFeatureNewProfileTextField",changePicker:"spacePicker",newProfileOption:"New Profile",
  submitLabels:["Create Profile","Change Profile","Next","Back","Cancel"],confirmation:"Switch it up"} as const;
export const morningPlan=[
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

export default async function scenario(c:ScenarioContext){
  const assert=(name:string,ok:unknown,detail="")=>{c.assert(name,ok,detail);if(!ok)throw new Error(name+(detail?": "+detail:""));};
  assert("Profiles uses caller-aware standard disposable recording",c.recorded&&!c.traced&&["trace156","ours156"].includes(c.slug));
  const app=resolve(import.meta.dir,`../../../arc/build/ArcModAgent-${c.slug}.app`),executable=join(app,"Contents/MacOS/Arc"),bundle=`company.thebrowser.Browser.mod.agent.${c.slug}`;
  assert("Profiles uses exact canonical disposable app",c.app===app&&realpathSync(c.app)===app);
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
  const directory=join(c.outDir,"profiles-evidence"),prior=info(directory);
  assert("Profiles evidence is fresh and not a symlink",prior===undefined||(!prior.isSymbolicLink()&&prior.isDirectory()&&readdirSync(directory).length===0));
  mkdirSync(directory,{recursive:true,mode:0o700});chmodSync(directory,0o700);
  assert("Profiles evidence confined to this canonical output",realpathSync(directory)===join(realpathSync(c.outDir),"profiles-evidence"));
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
    const owner=()=>{const now=identity(pid);if(launch.pid()!==pid||now.start!==id.start||verifyAgentEndpoint(c.slug,c.port)!==pid)throw new Error("Profiles PID/start/endpoint ownership changed");};
    const state=()=>{owner();helperCurrent();const s=c.native() as Snapshot;owner();if(s.pid!==pid||s.bundle!==bundle||s.truncated!==false||s.webContentExcluded!==true)throw new Error("Profiles native identity/scope/truncation gap");return s;};
    const under=(n:Node,p:Node)=>n.path.startsWith(p.path+".");
    const unique=(s:Snapshot,predicate:(n:Node)=>boolean,what:string)=>{const nodes=s.elements.filter(predicate);if(nodes.length!==1)throw new Error(`${what}: expected one observed node, got ${nodes.length}`);return nodes[0]!;};
    const browser=(s:Snapshot)=>{const nodes=s.elements.filter(n=>n.role==="AXWindow");if(nodes.length!==1||!nodes[0]!.identifier.startsWith("bigBrowserWindow-"))throw new Error("Profiles requires sole owned full browser window");return nodes[0]!;};
    const sheets=(s:Snapshot)=>{const w=browser(s),nodes=s.elements.filter(n=>n.role==="AXSheet");if(nodes.length>1||nodes.some(n=>!under(n,w)))throw new Error("Profiles extra/foreign sheet");return nodes;};
    const text=(n:Node)=>typeof n.value==="string"?n.value:n.label;
    const dispatch=(node:Node,kind:"press"|"set"|"perform"="press",value?:string)=>{
      owner();helperCurrent();
      if(++sequence>128||!node.enabled||!node.fingerprint||(kind==="set"?!node.valueSettable:!node.actions.includes(kind==="perform"?value!:"AXPress")))throw new Error("Profiles action missing enabled/advertised/settable identity or budget");
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
      const s=state();noSheet(s);const w=browser(s),side=unique(s,n=>under(n,w)&&n.identifier==="sidebarSpaceName"&&n.role==="AXStaticText","sole Space sidebar name");
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
  const first=c.launch();
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
  }finally{await c.quit(first,"Profiles first standard graceful quit");}
  assert("Profiles first phase captured exact names and directory",originalProfile!==undefined&&spaceTitle!==undefined&&sidebarName!==undefined&&profileIdentity!==undefined);
  inspectProfile("before restart",true);const second=c.launch();
  try{
    assert("Profiles restart ready",await c.ready(second,"Profiles restart launch"));
    const pid=second.pid();assert("Profiles restart genuinely new PID",pid!==undefined&&pid!==firstPID);
    inspectProfile("second launch",true);const ui=controls(second,identity(pid!));await ui.ensureWindow();ui.oneSpace();
    await ui.openChange();const restored=await ui.inventory(2);ui.expectedNames(restored.names,2);
    assert("Profiles restart restores original profile selection",restored.selected===originalProfile);ui.cancelChange();await ui.waitNoSheet();
    const originalTarget=await ui.fixture("restarted original");ui.marker(originalTarget,OLD,"restarted original");
    await ui.switchTo(NAME);const newTarget=await ui.fixture("restarted new");
    assert("restarted new profile reacquires distinct fixture",newTarget!==originalTarget);ui.marker(newTarget,NEW,"restarted new");
    await ui.switchTo(originalProfile!);const returnedTarget=await ui.fixture("restarted returned original");
    assert("restarted original reacquires distinct fixture from new",returnedTarget!==newTarget);ui.marker(returnedTarget,OLD,"restarted returned original");
    inspectProfile("completed restart",true);ui.owner();save("restart.json",{nonce,profile:profileCanonical,profileIdentity,firstPID,secondPID:pid,originalProfile,newProfile:NAME,spaceTitle,sidebarName,targets:{original:originalTarget,new:newTarget,returned:returnedTarget},runtimeScope:"Only recorded execution establishes this written plan. Native row/menu/display exposure, exact warning text, unique target replacement and restoration remain runtime assumptions."});
  }finally{await c.quit(second,"Profiles restart standard graceful quit");}
}
