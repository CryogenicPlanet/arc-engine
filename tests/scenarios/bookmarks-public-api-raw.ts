// Independent raw gate adapted from frozen bookmark-fixture/coverage.py SHA256
// 03707c596db55f628f7fc63f599b1236385285d3ca37828e768eae540e6b6a8a.
// Strengthens selector totals with per-call returns and receiver/order checks.
import { readFileSync } from "node:fs";
import { createHash } from "node:crypto";
type Row={seq:number;ev:string;from?:string;dir?:string;cls?:string;sel?:string;self?:unknown;args?:unknown[];ret?:unknown;thread?:string;depth?:number;engine_classes?:number};
const same=(a:unknown,b:unknown)=>JSON.stringify(a)===JSON.stringify(b);
function demand(value:unknown,why:string):asserts value{if(!value)throw new Error("bookmark raw gate: "+why);}
export function validateBookmarkTranscript(path:string,image:string,nonce:string,fixtureURL:string){
  const text=readFileSync(path,"utf8");demand(text.endsWith("\n"),"incomplete final recorder line");
  const rows=text.trimEnd().split("\n").map((line,index)=>{try{return JSON.parse(line) as Row;}catch{throw new Error(`bookmark raw gate: malformed line ${index+1}`);}});
  demand(rows.some(r=>r.ev==="hooked"&&Number(r.engine_classes)>0),"no hooked engine");
  const boundary=rows.filter(r=>r.ev==="call"||r.ev==="ret"),calls=boundary.filter(r=>r.ev==="call"),sequences=new Set<number>();
  for(const r of rows){demand(Number.isSafeInteger(r.seq)&&r.seq>=0&&!sequences.has(r.seq),"invalid/duplicate sequence");sequences.add(r.seq);}
  demand(calls.length>0&&boundary.every(r=>typeof r.from==="string"&&r.from.trim().length>0&&r.from!=="?"),"caller-aware transcript required; mixed/unknown attribution");
  const own=calls.filter(r=>r.from===image&&r.dir==="->engine"),paired=new Map<Row,Row>();
  const pair=(call:Row)=>{const next=calls.find(r=>r.seq>call.seq&&r.thread===call.thread&&r.depth===call.depth);
    const returns=boundary.filter(r=>r.ev==="ret"&&r.seq>call.seq&&(!next||r.seq<next.seq)&&r.from===call.from&&r.dir===call.dir&&r.cls===call.cls&&r.sel===call.sel&&same(r.self,call.self)&&r.thread===call.thread&&r.depth===call.depth);
    demand(call.thread==="main"&&returns.length===1,`missing/ambiguous or non-main return ${call.cls}.${call.sel}`);return returns[0]!;};
  demand(own.length>0,"no exact driver-image public calls");for(const call of own)paired.set(call,pair(call));
  const ownReturns=boundary.filter(r=>r.ev==="ret"&&r.from===image&&r.dir==="->engine");demand(ownReturns.length===own.length&&new Set(paired.values()).size===own.length&&ownReturns.every(r=>[...paired.values()].includes(r)),"orphan/duplicate driver-image return");
  const matching=(cls:string,sel:string)=>own.filter(r=>r.cls===cls&&r.sel===sel);
  const ret=(call:Row)=>paired.get(call)!.ret;
  const exact:Record<string,number>={getBookmarksTree:2,"addBookmark:withUrl:withParent:withIndexInParent:withUuid:":1,"renameBookmark:withTitle:":1,"editBookmark:withNewURL:":1,"removeBookmark:withParent:":1,"isBookmarked:":4};
  for(const [sel,n] of Object.entries(exact))demand(matching("ArcBrowserContext",sel).length===n,`exact context selector ${sel} count ${n}`);
  const getters:Record<string,number>={dateAdded:5,dateFolderModified:5,isFolder:1,isValid:7,liveChildren:6,nodeID:12,parentNodeID:2,title:7,url:7,uuid:12};
  for(const [sel,n] of Object.entries(getters))demand(matching("ArcBookmarkTreeNode",sel).length===n,`exact node selector ${sel} count ${n}`);
  const selected=own.filter(r=>r.cls==="ArcBookmarkTreeNode"||r.cls==="ArcBrowserContext"&&Object.hasOwn(exact,r.sel??""));demand(selected.length===74,"exact 74 bookmark calls required");
  const one=(cls:string,sel:string)=>{const found=matching(cls,sel);demand(found.length===1,`one ${cls}.${sel}`);return found[0]!;};
  const add=one("ArcBrowserContext","addBookmark:withUrl:withParent:withIndexInParent:withUuid:"),rename=one("ArcBrowserContext","renameBookmark:withTitle:"),edit=one("ArcBrowserContext","editBookmark:withNewURL:"),remove=one("ArcBrowserContext","removeBookmark:withParent:");
  const oldTitle=`Bookmark ε🪐 ${nonce}`,newTitle=`Renamed 改ε🪐 ${nonce}`,oldURL=new URL(`${fixtureURL}/bookmarks/${nonce}/ε🪐`).href,newURL=new URL(`${fixtureURL}/bookmarks/${nonce}/改ε🪐`).href;
  const context=add.self,parent=add.args?.[2],leaf=rename.args?.[0];
  demand(matching("ArcBrowserContext","getBookmarksTree").every(r=>same(r.self,context)),"tree/mutation context receiver differs");
  demand(same(add.args,[oldTitle,"url:"+oldURL,parent,0,nonce]),"AddURL fixed Unicode arguments/index/UUID");
  demand(same(rename.args,[leaf,newTitle])&&same(edit.args,[leaf,"url:"+newURL])&&same(remove.args,[leaf,parent]),"rename/edit/remove must use retained DTO and original parent");
  demand([rename,edit,remove].every(r=>same(r.self,context))&&[add,rename,edit,remove].every(r=>ret(r)===true),"mutation context/true returns");
  demand(add.seq<rename.seq&&rename.seq<edit.seq&&edit.seq<remove.seq,"exact mutation order");
  const membership=matching("ArcBrowserContext","isBookmarked:");demand(same(membership.map(r=>r.args),[["url:"+oldURL],["url:"+oldURL],["url:"+newURL],["url:"+newURL]])&&same(membership.map(ret),[true,false,true,false])&&membership.every(r=>same(r.self,context)),"membership fixed argument/result order");
  const trees=matching("ArcBrowserContext","getBookmarksTree"),root=ret(trees[0]!),freshRoot=ret(trees[1]!),children=matching("ArcBookmarkTreeNode","liveChildren"),freshParent=children[3]?.self,edited=ret(children[4]!) as unknown[];
  demand(typeof root==="string"&&typeof freshRoot==="string"&&root!==freshRoot,"fresh root wrapper differs");
  demand(typeof parent==="string"&&typeof freshParent==="string"&&parent!==freshParent,"fresh parent wrapper differs");
  demand(Array.isArray(edited)&&edited.length===1,"exact fresh edited leaf array");const freshLeaf=edited[0];
  demand(typeof leaf==="string"&&typeof freshLeaf==="string"&&leaf!==freshLeaf,"fresh leaf wrapper differs");
  demand(new Set([root,freshRoot,parent,freshParent,leaf,freshLeaf]).size===6,"six distinct DTO wrappers required");
  demand(same(children.map(r=>r.self),[root,parent,freshRoot,freshParent,freshParent,parent]),"liveChildren exact receiver sequence");
  demand(Array.isArray(ret(children[0]!))&&(ret(children[0]!) as unknown[]).includes(parent)&&same(ret(children[1]!),[])&&Array.isArray(ret(children[2]!))&&(ret(children[2]!) as unknown[]).includes(freshParent)&&same(ret(children[3]!),[leaf])&&same(ret(children[5]!),[]),"root/parent/leaf live tree transitions");
  const values=(label:unknown,sel:string)=>matching("ArcBookmarkTreeNode",sel).filter(r=>same(r.self,label)).map(ret);
  const scalar=(label:unknown,sel:string)=>{const all=values(label,sel);demand(all.length>0&&all.every(v=>same(v,all[0])),`immutable ${sel} on ${String(label)}`);return all[0];};
  for(const [old,fresh] of [[root,freshRoot],[parent,freshParent],[leaf,freshLeaf]])for(const sel of ["nodeID","uuid"])demand(same(scalar(old,sel),scalar(fresh,sel)),`stable public ${sel} across fresh wrappers`);
  demand(scalar(parent,"nodeID")==="1"&&scalar(leaf,"uuid")===nonce,"public bar ID1 and exact leaf UUID");
  demand(scalar(parent,"parentNodeID")===scalar(root,"nodeID")&&scalar(leaf,"parentNodeID")===scalar(parent,"nodeID"),"public parent identity relationships");
  demand(scalar(leaf,"title")===oldTitle&&scalar(freshLeaf,"title")===newTitle&&scalar(leaf,"url")==="url:"+oldURL&&scalar(freshLeaf,"url")==="url:"+newURL,"retained and fresh scalar snapshots remain frozen through removal");
  demand(same(values(leaf,"isValid"),[true,false])&&same(values(freshLeaf,"isValid"),[false])&&values(parent,"isValid").every(v=>v===true)&&same(values(freshParent,"isValid"),[true]),"removed DTO validity versus retained parent");
  demand(values(leaf,"dateFolderModified").every(v=>v===null)&&values(parent,"dateFolderModified").every(v=>v==="<date>")&&same(values(freshParent,"dateFolderModified"),["<date>"])&&values(leaf,"dateAdded").every(v=>v==="<date>"),"raw native date shapes (numeric window separately validated)");
  const contextCall=one("ArcBrowser","context");demand(same(ret(contextCall),context),"genuine browser context result receiver");
  const callbacks=calls.filter(r=>r.dir==="engine->app"&&r.sel==="browser:didCreateNewWebContents:isActive:"&&same(r.args?.[0],contextCall.self));
  demand(callbacks.length===1&&callbacks[0]?.from==="engine", "one genuine create callback from engine for fixture browser");pair(callbacks[0]!);
  return {path,sha256:createHash("sha256").update(text).digest("hex"),driverImage:image,ownPublicCalls:own.length,bookmarkCalls:selected.length,bookmarkSelectors:16,pairedDriverReturns:paired.size,genuineCreateCallback:callbacks[0],rawLabelsPreserved:true};
}
