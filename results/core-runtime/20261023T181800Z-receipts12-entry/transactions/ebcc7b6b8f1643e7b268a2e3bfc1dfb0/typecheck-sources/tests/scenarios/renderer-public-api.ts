// CSS lifecycle and annotated extraction only. Completion timing is asserted in
// the driver sidecar; the standard recorder does not emit block-invocation rows.
import type { ScenarioContext } from "../launch-slice/run";
import { verifyAgentEndpoint, type CDPResult } from "../../../arc/tools/agent-cdp";
import { mkdirSync, readFileSync, renameSync, writeFileSync } from "node:fs";
import { join } from "node:path";
import { isDeepStrictEqual } from "node:util";

const PREFIX = "/arc-renderer-probe-7f3a/";
const TITLE = "Renderer boundary fixture";
const rich = `<main><h1>Heading</h1><p>Paragraph <strong>bold</strong> <em>emphasis</em>
  <a href="https://example.invalid/path">link</a></p><div style="display:none">hidden</div>
  <img alt="fixture image" width="16" height="16" src="data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='16' height='16'%3E%3Crect width='16' height='16'/%3E%3C/svg%3E">
  <ul><li>first</li><li>second</li></ul><ol><li>ordered</li></ol>
  <form name="test-form" action="/submit"><input name="text-field" value="public-fixture" placeholder="placeholder" required>
  <input type="password" name="password-field" value="fixture-only-password"><input type="checkbox" name="check-field" checked>
  <select name="select-field"><option value="a" selected>Alpha</option><option value="b" disabled>Beta</option></select>
  <textarea name="text-area">area text</textarea><button>Submit</button></form>
  <table><caption>fixture table</caption><tr><th>header</th></tr><tr><td>cell</td></tr></table></main>`;
export const fixture = (request: Request) => {
  const path = new URL(request.url).pathname;
  if (!path.startsWith(PREFIX)) return;
  const name = path.slice(PREFIX.length), frame = name.startsWith("frame");
  if (!["a","b","frame","frame-dynamic","frame-navigated"].includes(name)) return new Response("not found",{status:404});
  return new Response(`<!doctype html><meta charset="utf-8"><title>${TITLE}</title>
    <style>#probe{color:rgb(0,0,0);background-color:rgb(255,255,255)}</style><div id="probe">fixture</div><div class="arc-probe-ad">ad fixture</div>
    ${frame ? "" : rich + `<iframe id="child" src="${PREFIX}frame"></iframe>`}`,
    {headers:{"content-type":"text/html; charset=utf-8","cache-control":"no-store"}});
};
type Tree = {type:number;children:Tree[];[key:string]:unknown};
const node = (type:number,children:Tree[] = [],fields:Record<string,unknown> = {}):Tree => ({
  type,children,textContent:null,hasEmphasis:false,textSize:2,textColor:null,linkURL:null,imageAlt:null,
  iframeOrigin:null,formName:null,formActionURL:null,tableName:null,formControlInfo:null,...fields,
});
const text = (value:string,fields:Record<string,unknown> = {}) => node(3,[],{textContent:value,textColor:"#FF000000",...fields});
const control = (type:number,name:string,children:Tree[] = [],fields:Record<string,unknown> = {}) => node(18,children,{formControlInfo:{
  controlType:type,fieldName:name,fieldValue:null,placeholder:null,isChecked:false,isRequired:false,isRedacted:false,selectOptions:[],...fields,
}});
// Literal fixture contract, never learned from the candidate recording.
export const expectedExtractionTree = (base:string,marker = "fixture"):Tree => node(0,[
  text(marker),text("ad fixture"),node(1,[
    node(5,[text("Heading",{hasEmphasis:true,textSize:4})]),
    node(4,[text("Paragraph "),text("bold",{hasEmphasis:true}),text("emphasis",{hasEmphasis:true}),
      node(6,[text("link",{hasEmphasis:true,textColor:"#FF0000EE"})],{linkURL:"https://example.invalid/path"})]),
    node(7,[],{imageAlt:"fixture image"}),
    node(12,[node(13,[text("first")]),node(13,[text("second")])]),node(11,[node(13,[text("ordered")])]),
    node(17,[
      control(25,"text-field",[node(1,[text("public-fixture")])],{fieldValue:"public-fixture",placeholder:"placeholder",isRequired:true}),
      control(18,"password-field",[],{isRedacted:true}),control(8,"check-field",[],{isChecked:true}),
      control(30,"select-field",[text("Alpha")],{selectOptions:[
        {value:"a",text:"Alpha",isSelected:true,isDisabled:false},
        // Native Arc's annotation reports false even though HTML marks Beta disabled.
        {value:"b",text:"Beta",isSelected:false,isDisabled:false},
      ]}),control(33,"text-area",[text("area text")],{fieldValue:"area text"}),control(2,"",[text("Submit")]),
    ],{formName:"test-form",formActionURL:`${base}/submit`}),
    node(14,[text("fixture table"),node(15,[node(16,[text("header",{hasEmphasis:true})])]),node(15,[node(16,[text("cell")])])],{tableName:"fixture table"}),
  ]),node(2,[node(0,[text("fixture"),text("ad fixture")])],{iframeOrigin:base}),
]);
const flatten = (tree:Tree):Tree[] => [tree,...tree.children.flatMap(flatten)];
type State = {insertCalls:number;removeCalls:number;extractionCalls:number;completionCalls:number;completionCounts:number[];pending:boolean};
type DriverResponse = {id:string;operation:string;pid:number;nonce:string;main:boolean;ok?:boolean;error?:string;
  recorderForwarding?:boolean;state:State;mainThread?:boolean;synchronous?:boolean;completionCount?:number;
  result?:{url:string;title:string;rootNode:Tree};counts?:{nodes:number;controls:number;options:number}};
type RecordRow = {ev:string;dir?:string;cls?:string;sel?:string;args?:unknown[];thread?:string;depth?:number;self?:string;from?:string};
type Style = {color:string;background:string;old:string;current:string;second:string};
const baseline:Style = {color:"rgb(0, 0, 0)",background:"rgb(255, 255, 255)",old:"",current:"",second:""};
const first:Style = {...baseline,color:"rgb(19, 53, 91)",old:"old"};
const replacement:Style = {...baseline,color:"rgb(113, 29, 47)",current:"current"};
const green = {background:"rgb(7, 101, 23)",second:"second"};
const ONE = "arc-renderer-probe-one", TWO = "arc-renderer-probe-two", MISSING = "arc-renderer-probe-missing";
const FIRST = "#probe{color:rgb(19,53,91)!important;--arc-probe-old:old}";
const REPLACEMENT = "#probe{color:rgb(113,29,47)!important;--arc-probe-current:current}";
const SECOND = "#probe{background-color:rgb(7,101,23)!important;--arc-probe-second:second}";
const styleExpression = `(() => {
  const inspect=d=>{const e=d?.getElementById('probe');if(!e||d.readyState!=='complete')return null;
    const s=d.defaultView.getComputedStyle(e);return {color:s.color,background:s.backgroundColor,
      old:s.getPropertyValue('--arc-probe-old').trim(),current:s.getPropertyValue('--arc-probe-current').trim(),second:s.getPropertyValue('--arc-probe-second').trim()};};
  const main=inspect(document),frames=[...document.querySelectorAll('iframe')].map(f=>inspect(f.contentDocument));
  return main&&frames.length&&frames.every(Boolean)?{main,frames}:null;
})()`;

export default async function scenario(c: ScenarioContext) {
  const assert = (name:string,pass:unknown,detail = "") => {c.assert(name,pass,detail);if(!pass)throw new Error(name);};
  assert("renderer public API requires standard boundary recording",c.recorded&&!c.traced);
  assert("renderer public API uses disposable engine test agent",["trace156","ours156"].includes(c.slug));
  const directory = join(c.outDir,"renderer-driver");mkdirSync(directory,{recursive:true,mode:0o700});
  const dylib = join(directory,"RendererPublicAPIDriver.dylib");
  const compile = Bun.spawnSync(["xcrun","clang","-dynamiclib","-fobjc-arc","-Wall","-Wextra","-Werror","-framework","Foundation",
    join(import.meta.dir,"renderer-public-api-driver.m"),"-o",dylib],{stdout:"pipe",stderr:"pipe"});
  writeFileSync(join(directory,"compile.log"),compile.stdout.toString()+compile.stderr.toString());
  assert("renderer driver compiles with strict warnings",compile.success,compile.stderr.toString().slice(-1500));
  const nonce = `renderer-public-api-${crypto.randomUUID()}`;
  let currentURL = `${c.base}${PREFIX}a`;
  const launch = c.launch({libraries:[dylib],environment:{ARC_SCENARIO_RENDERER_DIRECTORY:directory,
    ARC_SCENARIO_RENDERER_NONCE:nonce,ARC_SCENARIO_RENDERER_AGENT:c.slug,ARC_SCENARIO_RENDERER_URL:currentURL}});
  const acknowledgments:DriverResponse[] = [], observations:{step:string;styles:unknown}[] = [];
  try {
    if(!await c.ready(launch,"renderer public API agent launch"))return;
    const pid=launch.pid();assert("renderer agent exposes an owned PID",pid!==undefined);
    const owner=()=>{if(launch.pid()!==pid||verifyAgentEndpoint(c.slug,c.port)!==pid)throw new Error("renderer agent PID/endpoint changed");};
    const ready=await c.waitFor(()=>{const value=JSON.parse(readFileSync(join(directory,"ready.json"),"utf8")) as {pid?:number;nonce?:string;agent?:string;installed?:boolean;recorderAliases?:boolean};
      return value.pid===pid&&value.nonce===nonce&&value.agent===c.slug&&value.installed===true&&value.recorderAliases===true?value:undefined;},10000);
    assert("renderer driver ready with current PID and recorder aliases",ready!==undefined);
    let sequence=0;
    const action=async(operation:string,values:Record<string,unknown> = {})=>{
      owner();const id=`${nonce}-${++sequence}`,request=join(directory,"request.json");
      writeFileSync(request+".tmp",JSON.stringify({id,operation,pid,nonce,url:currentURL,...values}),{mode:0o600});renameSync(request+".tmp",request);
      const response=await c.waitFor(()=>{const value=JSON.parse(readFileSync(join(directory,"response.json"),"utf8")) as DriverResponse;
        return value.id===id&&value.pid===pid&&value.nonce===nonce&&value.operation===operation?value:undefined;},25000);
      if(!response)throw new Error(`renderer ${operation} response timed out; action will not be replayed`);
      owner();acknowledgments.push(response);writeFileSync(join(directory,"observations.json"),JSON.stringify({acknowledgments,observations},null,2));
      if(response.ok!==true||response.main!==true)throw new Error(`renderer ${operation}: ${response.error??"not main-thread/success"}`);
      return response;
    };
    const until=async<T>(fn:()=>Promise<T|undefined>):Promise<T>=>{
      // Only read-only observations belong here; mutations are dispatched once.
      const end=Date.now()+10000;let lastError:unknown;
      while(Date.now()<end){try{const result=await fn();if(result!==undefined)return result;}catch(error){lastError=error;}await c.sleep(100);}
      throw new Error(`renderer fixture condition timed out${lastError?`: ${String(lastError).slice(-200)}`:""}`);
    };
    const client=await c.client();
    const created=await client.request("Target.createTarget",{url:currentURL,background:true});
    assert("renderer fixture has exact target ID",typeof created.targetId==="string");const target=created.targetId as string;
    const evaluate=async(expression:string)=>{owner();const result=await client.targetCommand(target,"Runtime.evaluate",{expression,returnByValue:true,awaitPromise:true});
      if(result.exceptionDetails)throw new Error("renderer fixture JS failed");return(result.result as CDPResult)?.value;};
    await until(async()=>await evaluate(`location.href===${JSON.stringify(currentURL)}&&document.readyState==='complete'`)?true:undefined);
    const selected=await action("select");assert("renderer driver owns exact fixture and canonical forwarding",selected.state!==undefined&&selected.recorderForwarding===true);
    const snapshot=async(step:string,main:Style,frames=1)=>{
      const expected={main,frames:Array.from({length:frames},()=>baseline)};let consecutive=0,last:unknown;
      await until(async()=>{last=await evaluate(styleExpression);consecutive=isDeepStrictEqual(last,expected)?consecutive+1:0;return consecutive>=3?true:undefined;});
      observations.push({step,styles:last});assert(`CSS ${step}`,isDeepStrictEqual(last,expected));
    };
    await snapshot("baseline",baseline);
    for(const marker of ["fixture","mutated fixture"]){
      if(marker!=="fixture")await evaluate("document.getElementById('probe').textContent='mutated fixture'");
      const response=await action("extract");
      assert(`annotated extraction ${marker} callback is asynchronous main-thread once`,response.mainThread===true&&response.synchronous===false&&response.completionCount===1&&response.recorderForwarding===true);
      assert(`annotated extraction ${marker} has fixed bounded counts`,isDeepStrictEqual(response.counts,{nodes:45,controls:6,options:2}));
      const expected=expectedExtractionTree(c.base,marker),actual=response.result;
      assert(`annotated extraction ${marker} exact public tree`,actual?.url===`${c.base}${PREFIX}a`&&actual.title===TITLE&&isDeepStrictEqual(actual.rootNode,expected));
      const nodes=flatten(actual!.rootNode),controls=nodes.map(n=>n.formControlInfo).filter((v):v is Record<string,unknown>=>typeof v==="object"&&v!==null);
      const password=controls.filter(v=>v.fieldName==="password-field");
      assert(`annotated extraction ${marker} password redacted and hidden text omitted`,password.length===1&&password[0]!.isRedacted===true&&password[0]!.fieldValue===null&&!nodes.some(n=>n.textContent==="hidden")&&!JSON.stringify(actual).includes("fixture-only-password"));
    }
    await evaluate("document.getElementById('probe').textContent='fixture'");
    await action("insert",{identifier:ONE,stylesheet:FIRST});await snapshot("insert-one",first);
    await action("insert",{identifier:ONE,stylesheet:REPLACEMENT});await snapshot("replace-same-identifier",replacement);
    await action("insert",{identifier:TWO,stylesheet:SECOND});await snapshot("insert-second-identifier",{...replacement,...green});
    await action("remove",{identifier:MISSING});await snapshot("remove-missing",{...replacement,...green});
    await action("remove",{identifier:ONE});await snapshot("remove-one",{...baseline,...green});
    await action("remove",{identifier:TWO});await snapshot("remove-two",baseline);
    await action("insert",{identifier:ONE,stylesheet:FIRST});await snapshot("insert-before-document-navigation",first);
    currentURL=`${c.base}${PREFIX}b`;await client.targetCommand(target,"Page.navigate",{url:currentURL});
    await until(async()=>await evaluate(`location.href===${JSON.stringify(currentURL)}&&document.readyState==='complete'`)?true:undefined);
    await action("bind-after-navigation");await snapshot("after-document-navigation",baseline);
    await action("remove",{identifier:ONE});await snapshot("remove-after-document-navigation",baseline);
    await action("insert",{identifier:ONE,stylesheet:REPLACEMENT});await snapshot("insert-after-document-navigation",replacement);
    await evaluate(`(()=>{const f=document.createElement('iframe');f.id='dynamic';f.src=${JSON.stringify(`${c.base}${PREFIX}frame-dynamic`)};document.body.append(f);})()`);
    await snapshot("new-subframe-after-insertion",replacement,2);
    await evaluate(`document.getElementById('child').src=${JSON.stringify(`${c.base}${PREFIX}frame-navigated`)}`);
    await until(async()=>await evaluate(`document.getElementById('child').contentWindow.location.href===${JSON.stringify(`${c.base}${PREFIX}frame-navigated`)}`)?true:undefined);
    await snapshot("existing-subframe-navigation",replacement,2);
    await action("remove",{identifier:ONE});await snapshot("remove-with-subframes",baseline,2);
    assert("exactly fourteen fixed CSS observations",observations.length===14);
    const final=await action("status");assert("two extraction requests finish once and ten CSS operations complete",final.recorderForwarding===true&&final.state.insertCalls===5&&final.state.removeCalls===5&&final.state.extractionCalls===2&&final.state.completionCalls===2&&isDeepStrictEqual(final.state.completionCounts,[1,1])&&final.state.pending===false);
    const rows=readFileSync(join(c.outDir,"launch-1","record.jsonl"),"utf8").trim().split("\n").map(line=>JSON.parse(line) as RecordRow);
    const allCalls=rows.filter(row=>row.ev==="call"),attributed=allCalls.some(row=>Object.hasOwn(row,"from"));
    if(attributed)assert("caller-aware transcript fully attributes fixture calls",allCalls.length>0&&allCalls.every(row=>typeof row.from==="string"&&row.from.trim().length>0));
    // Preserve full raw rows. Fixture getters are caller-visible even though the
    // default shared comparator excludes this explicit test-peer image.
    const calls=attributed?allCalls.filter(row=>row.from==="RendererPublicAPIDriver.dylib"):allCalls;
    const css:RecordRow[]=[],pending=new Set<string>();let returns=0;
    const key=(row:RecordRow)=>JSON.stringify([row.thread,row.depth,row.self,row.cls,row.sel,attributed?row.from:undefined]);
    for(const row of rows){
      if(row.ev==="call"&&(!attributed||row.from==="RendererPublicAPIDriver.dylib")&&row.dir==="->engine"&&row.cls==="ArcWebContents"&&["insertStylesheet:stylesheet:","removeStylesheet:"].includes(row.sel??"")&&[ONE,TWO,MISSING].includes(String(row.args?.[0]))){
        if(pending.has(key(row)))throw new Error("nested duplicate fixture CSS call");pending.add(key(row));css.push(row);
      }else if(row.ev==="ret"&&pending.delete(key(row)))returns++;
    }
    const expectedCSS=[
      {sel:"insertStylesheet:stylesheet:",args:[ONE,FIRST]},{sel:"insertStylesheet:stylesheet:",args:[ONE,REPLACEMENT]},
      {sel:"insertStylesheet:stylesheet:",args:[TWO,SECOND]},{sel:"removeStylesheet:",args:[MISSING]},
      {sel:"removeStylesheet:",args:[ONE]},{sel:"removeStylesheet:",args:[TWO]},
      {sel:"insertStylesheet:stylesheet:",args:[ONE,FIRST]},{sel:"removeStylesheet:",args:[ONE]},
      {sel:"insertStylesheet:stylesheet:",args:[ONE,REPLACEMENT]},{sel:"removeStylesheet:",args:[ONE]},
    ];
    assert("unmodified boundary transcript contains exactly five fixture insertions and five removals",isDeepStrictEqual(css.map(row=>({sel:row.sel,args:row.args})),expectedCSS)&&returns===10&&pending.size===0);
    const count=(cls:string,sel:string)=>calls.filter(row=>row.dir==="->engine"&&row.cls===cls&&row.sel===sel).length;
    assert("unmodified transcript records both extraction requests and document getters",count("ArcBrowserContextPageContentExtraction","getExtractedContentForWebContents:completion:")===2&&["url","title","rootNode"].every(sel=>count("ArcExtractedPageContent",sel)===2));
    assert("unmodified transcript records both complete annotated trees",["type","children","textContent","hasEmphasis","textSize","textColor","linkURL","imageAlt","iframeOrigin","formName","formActionURL","tableName","formControlInfo"].every(sel=>count("ArcContentNode",sel)===90)&&["controlType","fieldName","fieldValue","placeholder","isChecked","isRequired","isRedacted","selectOptions"].every(sel=>count("ArcFormControlInfo",sel)===12)&&["value","text","isSelected","isDisabled"].every(sel=>count("ArcSelectOption",sel)===4));
  }finally{
    writeFileSync(join(directory,"observations.json"),JSON.stringify({acknowledgments,observations},null,2));
    // Keep the common harness's graceful-exit assertion, including its SIGTERM failure.
    await c.quit(launch,"renderer public API agent quit");
  }
}
