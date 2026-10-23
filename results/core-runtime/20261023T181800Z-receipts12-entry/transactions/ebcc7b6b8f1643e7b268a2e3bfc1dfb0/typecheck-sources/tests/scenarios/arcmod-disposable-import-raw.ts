// Independent public-call proof; own delegate callback events remain sidecars.
// Standard v2 comparison excludes driver-origin calls: startup coverage alone
// cannot accept this scenario. No canonical ADK import callback claim is made.
import { readFileSync } from "node:fs";
import { createHash } from "node:crypto";
type Row={seq:number;ev:string;from?:string;dir?:string;cls?:string;sel?:string;self?:unknown;args?:unknown[];ret?:unknown;thread?:string;depth?:number;engine_classes?:number};
export type DisposableImportRawOptions={image:string;stage:number;source:string;name:string;profileName:string;nonce:string;barUUID:string;otherUUID:string;fixtureURL:string};
const same=(a:unknown,b:unknown)=>JSON.stringify(a)===JSON.stringify(b);
function demand(ok:unknown,why:string):asserts ok{if(!ok)throw new Error("disposable import raw: "+why);}
export function validateDisposableImportRaw(path:string,o:DisposableImportRawOptions){
  const bytes=readFileSync(path),text=bytes.toString("utf8");demand(bytes.length>0&&text.endsWith("\n"),"nonempty complete recorder required");
  const rows=text.trimEnd().split("\n").map((line,i)=>{try{return JSON.parse(line) as Row;}catch{throw new Error(`disposable import raw: malformed line${i+1}`);}});
  demand(o.stage===1||o.stage===2,"fixed stage1/2 required");demand(new URL(o.fixtureURL).pathname===`/arc-disposable-import-7f3a/${o.stage}`,"exact fresh stage-specific fixture URL required");demand(rows.some(r=>r.ev==="hooked"&&Number(r.engine_classes)>0),"no real engine hooks");
  const seen=new Set<number>();for(const r of rows){demand(Number.isSafeInteger(r.seq)&&r.seq>=0&&!seen.has(r.seq),"invalid/duplicate sequence");seen.add(r.seq);}
  const boundary=rows.filter(r=>r.ev==="call"||r.ev==="ret"),calls=boundary.filter(r=>r.ev==="call");
  demand(calls.length>0&&boundary.every(r=>typeof r.from==="string"&&r.from.trim().length>0&&r.from!=="?"),"fully attributed v2 boundaries required");
  const own=calls.filter(r=>r.from===o.image&&r.dir==="->engine"),paired=new Map<Row,Row>();demand(own.length>0,"no exact driver-image calls");
  const pair=(call:Row)=>{const next=calls.find(r=>r.seq>call.seq&&r.thread===call.thread&&r.depth===call.depth);const matches=boundary.filter(r=>r.ev==="ret"&&r.seq>call.seq&&(!next||r.seq<next.seq)&&r.from===call.from&&r.dir===call.dir&&r.cls===call.cls&&r.sel===call.sel&&same(r.self,call.self)&&r.thread===call.thread&&r.depth===call.depth);demand(matches.length===1,"missing/duplicate pair "+call.sel);return matches[0]!;};
  for(const call of own){demand(call.thread==="main","public fixture calls must be on main");paired.set(call,pair(call));}
  const returns=boundary.filter(r=>r.ev==="ret"&&r.from===o.image&&r.dir==="->engine");demand(returns.length===own.length&&new Set(paired.values()).size===own.length&&returns.every(r=>[...paired.values()].includes(r)),"orphan/duplicate driver returns");
  const allowed:Record<string,string[]>={ArcBrowser:["context","isInitialized"],ArcWebContents:["url","isInitialized"],ArcBrowserContextID:["directoryBasename","isIncognito"],ArcBrowserContext:["id","getBookmarksTree","addBookmarkFolder:withParent:withIndexInParent:withUuid:","import:withCategories:withAllowedExtensions:withBookmarkImportTargets:skipExistingCookies:withDelegate:error:"],ArcImportSourceProfile:["initWithSafariExportedData:profileName:name:categories:","name","profileName","sourcePath","appPath","type","supportedCategories","constraints"],ArcBookmarkImportTargets:["initWithBookmarkBarBookmarksTargetNode:withOtherBookmarksTargetNode:","targetNodeForBookmarkBarBookmarks","targetNodeForOtherBookmarks"],ArcBookmarkTreeNode:["isValid","isFolder","nodeID","uuid","parentNodeID","title","url","dateAdded","liveChildren"]};
  demand(own.every(r=>r.cls!==undefined&&r.sel!==undefined&&allowed[r.cls]?.includes(r.sel)),"unapproved driver API/discovery/private selector");
  const find=(cls:string,sel:string)=>own.filter(r=>r.cls===cls&&r.sel===sel),ret=(r:Row)=>paired.get(r)!.ret;
  const exact=(cls:string,sel:string,n:number)=>{const found=find(cls,sel);demand(found.length===n,`exact ${cls}.${sel} count${n}`);return found;};
  const one=(cls:string,sel:string)=>exact(cls,sel,1)[0]!;
  const contextCall=one("ArcBrowser","context"),context=ret(contextCall),contextIDCall=one("ArcBrowserContext","id"),contextID=ret(contextIDCall);
  demand(same(contextIDCall.self,context)&&find("ArcBrowserContext","getBookmarksTree").every(r=>same(r.self,context)),"one owned context receiver");
  const directory=one("ArcBrowserContextID","directoryBasename"),incognito=one("ArcBrowserContextID","isIncognito");demand(same(directory.self,contextID)&&same(incognito.self,contextID)&&ret(directory)==="Default"&&ret(incognito)===false,"regular exact Default context");
  const urls=find("ArcWebContents","url").filter(r=>ret(r)==="url:"+o.fixtureURL);demand(urls.length===1,"one real fixture URL getter");
  const creates=calls.filter(r=>r.dir==="engine->app"&&r.from==="engine"&&r.sel==="browser:didCreateNewWebContents:isActive:"&&same(r.args?.[0],contextCall.self)&&same(r.args?.[1],urls[0]!.self));demand(creates.length===1,"exact fixture genuine create callback missing/ambiguous");pair(creates[0]!);
  const importSelector="import:withCategories:withAllowedExtensions:withBookmarkImportTargets:skipExistingCookies:withDelegate:error:";
  const adds=exact("ArcBrowserContext","addBookmarkFolder:withParent:withIndexInParent:withUuid:",o.stage===1?2:0),imports=exact("ArcBrowserContext",importSelector,o.stage===1?1:0);
  exact("ArcBrowserContext","getBookmarksTree",o.stage===1?3:1);
  const initProfiles=exact("ArcImportSourceProfile","initWithSafariExportedData:profileName:name:categories:",o.stage===1?1:0),initTargets=exact("ArcBookmarkImportTargets","initWithBookmarkBarBookmarksTargetNode:withOtherBookmarksTargetNode:",o.stage===1?1:0);
  const profileValues:Record<string,unknown>={name:o.name,profileName:o.profileName,sourcePath:o.source,appPath:"",type:12,supportedCategories:2,constraints:0};
  for(const [selector,value] of Object.entries(profileValues)){const getters=exact("ArcImportSourceProfile",selector,o.stage===1?6:0);demand(getters.every(r=>same(ret(r),value)),"fixed profile getter "+selector);if(o.stage===1)demand(new Set(getters.map(r=>JSON.stringify(r.self))).size===6,"initial descriptor plus five fresh callback profile wrappers "+selector);}
  let importCall:Row|undefined;
  if(o.stage===1){
    const init=initProfiles[0]!,profile=ret(init),targetInit=initTargets[0]!,targets=ret(targetInit);demand(typeof profile==="string"&&typeof targets==="string"&&profile!==targets,"non-nil public descriptor/targets");demand(same(init.args,[o.source,o.profileName,o.name,2]),"exact explicit source directory/name/profile/mask2");
    const a=adds[0]!,b=adds[1]!;demand(a.seq<b.seq&&b.seq<targetInit.seq&&targetInit.seq<init.seq&&same(a.self,context)&&same(b.self,context)&&ret(a)===true&&ret(b)===true,"two live target creations before descriptor/import");
    demand(same(a.args,["ArcCore Import Bar "+o.nonce,a.args?.[1],0,o.barUUID])&&same(b.args,["ArcCore Import Other "+o.nonce,a.args?.[1],1,o.otherUUID]),"fixed target titles UUIDs parent indices");
    const [bar,other]=targetInit.args??[];demand(typeof bar==="string"&&typeof other==="string"&&bar!==other&&same(targetInit.args,[bar,other]),"distinct real target DTO arguments");
    const barRead=one("ArcBookmarkImportTargets","targetNodeForBookmarkBarBookmarks"),otherRead=one("ArcBookmarkImportTargets","targetNodeForOtherBookmarks");demand(same(barRead.self,targets)&&same(otherRead.self,targets)&&same(ret(barRead),bar)&&same(ret(otherRead),other),"target getter identity readback");
    for(const label of [bar,other])for(const selector of ["isValid","isFolder"]){const facts=find("ArcBookmarkTreeNode",selector).filter(r=>same(r.self,label)&&r.seq<targetInit.seq);demand(facts.length>0&&facts.every(r=>ret(r)===true),"live valid folder before import targets");}
    const ids=[bar,other].map(label=>{const facts=find("ArcBookmarkTreeNode","nodeID").filter(r=>same(r.self,label)&&r.seq<targetInit.seq);demand(facts.length>0&&typeof ret(facts[0]!)==="string","live target nodeID");return ret(facts[0]!);});demand(ids[0]!==ids[1],"distinct native target nodeIDs");
    importCall=imports[0]!;demand(same(importCall.self,context)&&importCall.seq>init.seq&&ret(importCall)===true,"one owned accepted import after descriptor");const args=importCall.args;demand(args?.length===7&&same(args[0],profile)&&args[1]===2&&same(args[2],[])&&same(args[3],targets)&&args[4]===true&&typeof args[5]==="string"&&/^<DisposableImportDelegate#\d+>$/.test(args[5])&&args[6]==="<ptr>","exact bookmark mask2 empty extensions real targets own delegate skip true NSError**");
    for(const [selector] of Object.entries(profileValues)){const before=find("ArcImportSourceProfile",selector).filter(r=>same(r.self,profile)&&r.seq<importCall!.seq);demand(before.length===1,"descriptor scalar preflight before sole import "+selector);}
  }else{exact("ArcBookmarkImportTargets","targetNodeForBookmarkBarBookmarks",0);exact("ArcBookmarkImportTargets","targetNodeForOtherBookmarks",0);}
  // Fixed final model structure; either explicit target may receive the parser's
  // single non-toolbar folder. This asserts counts/values rather than learning
  // a destination, imported wrapper or date conversion from the observed run.
  const scalar=(node:unknown,selector:string)=>{const found=find("ArcBookmarkTreeNode",selector).filter(r=>same(r.self,node));demand(found.length>0&&found.every(r=>same(ret(r),ret(found[0]!))),"consistent scalar "+selector);return ret(found[0]!);};
  const titled=(title:string)=>{const matches=find("ArcBookmarkTreeNode","title").filter(r=>ret(r)===title),labels=[...new Set(matches.map(r=>JSON.stringify(r.self)))];demand(labels.length===1,"exact final model title "+title);return matches[0]!.self;};
  const bar=titled("ArcCore Import Bar "+o.nonce),other=titled("ArcCore Import Other "+o.nonce),folder=titled("ArcCore Fixture Folder"),leaf=titled("ArcCore Imported Bookmark 7f3a");demand(new Set([bar,other,folder,leaf].map(v=>JSON.stringify(v))).size===4,"four distinct final node DTOs");
  const children=(node:unknown)=>{const found=find("ArcBookmarkTreeNode","liveChildren").filter(r=>same(r.self,node));demand(found.length===1&&Array.isArray(ret(found[0]!)),"one final live children read");return ret(found[0]!) as unknown[];};
  const aChildren=children(bar),bChildren=children(other);demand(same(aChildren,[folder])&&same(bChildren,[])||same(aChildren,[])&&same(bChildren,[folder]),"one populated target one empty");demand(same(children(folder),[leaf])&&same(children(leaf),[]),"single imported folder and leaf only");
  demand(scalar(bar,"uuid")===o.barUUID&&scalar(other,"uuid")===o.otherUUID&&scalar(bar,"parentNodeID")==="1"&&scalar(other,"parentNodeID")==="1","fixed target UUIDs and public parent");
  for(const node of [bar,other,folder,leaf]){demand(scalar(node,"isValid")===true&&scalar(node,"isFolder")===(node!==leaf),"final validity/folder shape");demand(typeof scalar(node,"nodeID")==="string"&&typeof scalar(node,"uuid")==="string","real final node identity");}
  const destination=aChildren.length?bar:other;demand(scalar(folder,"parentNodeID")===scalar(destination,"nodeID")&&scalar(leaf,"parentNodeID")===scalar(folder,"nodeID"),"fixed imported parent relationships");demand(scalar(folder,"url")===null&&scalar(leaf,"url")==="url:"+new URL("/arccore-import-fixture",o.fixtureURL).href,"fixed folder/leaf URL");
  if(importCall)demand(find("ArcBookmarkTreeNode","title").filter(r=>[folder,leaf].some(n=>same(n,r.self))).every(r=>r.seq>paired.get(importCall!)!.seq),"final model observed after import returned");
  return {sha256:createHash("sha256").update(bytes).digest("hex"),stage:o.stage,driverImage:o.image,ownPublicCalls:own.length,pairedDriverReturns:paired.size,importCalls:imports.length,categoryMask:2,publicSourceType:12,genuineCreateCallback:creates[0],callbackScope:"own-public-protocol-sidecar-only",rawLabelsPreserved:true};
}
