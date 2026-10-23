// Pure offline gate adapted from the frozen omnibox.real-ui-pair2 causal proof.
// No recorder rows are removed, relabeled, or normalized. Background thread
// names are not unique IDs, so complete stack pairing is asserted for main only.
import { createHash } from "node:crypto";

export const COMMAND_BAR_QUERY = "autocomplete:cursorPosition:preventInlineAutocomplete:startFresh:keyword:error:completion:";
const IDENTITY = ["thread", "depth", "from", "dir", "cls", "kind", "sel", "self"] as const;
type Event = Record<string, unknown>;
type Row = {line:number;offset:number;endOffset:number;event:Event};
export class CommandBarSnapshotPending extends Error {}
const require = (ok:unknown, message:string):void => { if (!ok) throw new Error(message); };
const pending = (message:string):never => { throw new CommandBarSnapshotPending(message); };
const same = (a:unknown,b:unknown) => JSON.stringify(a) === JSON.stringify(b);

/** A byte-exact live snapshot, taken BEFORE AXConfirm/quit. Only an unchanged
 * already-open app -run may remain on main's stack. Missing post-action returns
 * are pending, never accepted. Final saved proof still contains every row. */
export function validateCommandBarRaw(input:Uint8Array, cursor:number, url:string) {
  const raw=Buffer.from(input);
  require(raw.length>0&&raw.length<=16*1024*1024,"command-bar raw size outside bound");
  if(raw.at(-1)!==10)pending("command-bar live snapshot ends within a recorder line");
  const rows:Row[]=[];let offset=0;
  for(const line of new TextDecoder("utf-8",{fatal:true}).decode(raw).split("\n").slice(0,-1)) {
    const event:unknown=JSON.parse(line),length=Buffer.byteLength(line)+1;
    require(event!==null&&typeof event==="object"&&!Array.isArray(event),"nonobject recorder row");
    rows.push({line:rows.length+1,offset,endOffset:offset+length,event:event as Event});offset+=length;
  }
  require(Number.isSafeInteger(cursor)&&cursor>=0&&(cursor===raw.length||rows.some(row=>row.offset===cursor)),"pre-setter cursor is not an exact raw byte boundary");
  const hooks=rows.filter(row=>row.event.ev==="hooked");
  require(hooks.length===1&&hooks[0]!.event.seq===0&&Number.isSafeInteger(hooks[0]!.event.engine_classes)&&Number(hooks[0]!.event.engine_classes)>0,"expected one positive engine hook marker");
  const events=rows.filter(row=>row.event.ev!=="hooked");
  const sequences=events.map(row=>row.event.seq);
  require(sequences.length>0&&sequences.every(seq=>Number.isSafeInteger(seq)&&Number(seq)>0)&&new Set(sequences).size===sequences.length,"invalid duplicate/reset recorder sequence");
  const maximum=sequences.reduce<number>((maximum,seq)=>Math.max(maximum,Number(seq)),0);
  if(maximum!==sequences.length)pending("command-bar live snapshot has an uncommitted global sequence gap");
  const stack:Row[]=[],pairs=new Map<number,Row>();let prefix:Row[]|undefined;
  let lastMainSequence=0,backgroundEvents=0;
  for(const row of rows) {
    if(prefix===undefined&&row.offset>=cursor)prefix=[...stack];
    const e=row.event;if(e.ev==="hooked")continue;
    require((e.ev==="call"||e.ev==="ret")&&IDENTITY.every(key=>Object.hasOwn(e,key))&&
      typeof e.from==="string"&&e.from.trim().length>0&&typeof e.thread==="string"&&e.thread.length>0&&
      Number.isSafeInteger(e.depth)&&Number(e.depth)>=0&&(typeof e.self==="string"||(e.kind==="+"&&e.self===null))&&typeof e.cls==="string"&&
      typeof e.sel==="string"&&(e.kind==="-"||e.kind==="+")&&(e.dir==="->engine"||e.dir==="engine->app"),"missing/invalid caller-aware recorder metadata");
    if(e.thread!=="main"){backgroundEvents++;continue;}
    require(Number(e.seq)>lastMainSequence,"main recorder sequence order changed");lastMainSequence=Number(e.seq);
    if(e.ev==="call"){
      require(e.depth===stack.length,"main call depth disagrees with complete recorder stack");stack.push(row);
    }else{
      const call=stack.at(-1);require(call&&IDENTITY.every(key=>call.event[key]===e[key]),"main return identity/order mismatch");
      stack.pop();pairs.set(call!.offset,row);
    }
  }
  prefix??=[...stack];
  require(prefix.length===1,"pre-setter prefix must have exactly one preexisting main invocation");
  const root=prefix[0]!,e=root.event;
  require(root.offset<cursor&&e.ev==="call"&&e.thread==="main"&&e.depth===0&&e.from==="app"&&e.dir==="->engine"&&
    e.cls==="ArcBrowserApplication"&&e.kind==="-"&&e.sel==="run"&&same(e.args,[])&&
    typeof e.self==="string"&&/^<ArcBrowserApplication#[1-9][0-9]*>$/.test(e.self),"prefix is not the exact known live app ArcBrowserApplication -run");
  require(stack[0]===root&&!pairs.has(root.offset),"preexisting -run invocation changed/returned in live proof");
  if(stack.length!==1)pending("post-setter main invocation remains open");
  const post=rows.filter(row=>row.offset>=cursor&&row.event.thread==="main");
  require(post.every(row=>row.event.ev!=="call"||pairs.has(row.offset)),"post-setter main invocation lacks a paired return");
  const queries=post.filter(row=>{const e=row.event;return e.ev==="call"&&e.dir==="->engine"&&e.cls==="ArcBrowser"&&e.sel===COMMAND_BAR_QUERY;});
  if(queries.length===0)pending("new app autocomplete has not been observed");
  require(queries.length===1,"expected exactly one post-setter browser autocomplete query");
  const query=queries[0]!,args=query.event.args;
  require(query.event.from==="app"&&query.event.kind==="-"&&typeof query.event.self==="string"&&/^<ArcBrowser#[1-9][0-9]*>$/.test(query.event.self)&&
    same(args,[url,url.length,false,true,"","<ptr>","<block>"]),"new query caller/receiver/canonical arguments mismatch");
  const returned=pairs.get(query.offset);
  require(returned&&returned.offset>query.offset&&returned.event.ret===true,"new exact-fixture query lacks matching true return");
  // Prove genuine app consumption rather than direct test-driver construction.
  // These are actual getter rows, not evidence that the uninstrumented block ran.
  const consumed=post.filter(row=>row.event.ev==="call"&&row.event.from==="app"&&row.event.dir==="->engine"&&row.event.cls==="ArcAutocompleteResult"&&row.event.sel==="combinedMatches"&&row.offset>query.offset);
  if(consumed.length===0)pending("app result consumption has not been observed");
  require(consumed.every(row=>{const ret=pairs.get(row.offset)?.event.ret;return same(row.event.args,[])&&Array.isArray(ret)&&ret.length>0;}),"app combinedMatches consumption lacks a nonempty matched return");
  return {passed:true,rawBytes:raw.length,rawSha256:createHash("sha256").update(raw).digest("hex"),rawEventCount:rows.length,
    preSetterCursor:cursor,sequenceGaps:0,mainIdentityDepthOrderMismatches:0,backgroundEvents,
    pairedMainInvocations:pairs.size,preexistingRun:root,targetCall:query,targetReturn:returned,
    genuineAppResultReads:consumed.length,prefixEqualsFinalExactInvocation:true,allPostCursorMainCallsPaired:true,
    blockInvocationObserved:false,completionDoneObserved:false,
    scope:"New exact app autocomplete/true return, complete main pairing and genuine app result consumption. No block-callback, navigation, parity or shutdown inference."};
}
