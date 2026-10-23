// Offline caller-aware public-boundary gate. The numeric return is deliberately
// unnamed: neither an enum interpretation nor execution of <block> is observed.
import { createHash } from "node:crypto";

type Event = Record<string, unknown>;
type Row = { line:number; offset:number; end:number; event:Event };
export type QuitPhase = { start:number; end:number };
const IDENTITY = ["thread","depth","from","dir","cls","kind","sel","self"] as const;
const require = (ok:unknown,message:string):void => { if(!ok)throw new Error(message); };

/** Live snapshots must extend the last COMPLETE observed bytes, even if the
 * same inode was truncated and rewritten. Never normalize recorder contents. */
export function assertTerminationRawPrefix(current:Uint8Array,previous:Uint8Array):void {
  require(current.length<=64*1024*1024&&current.length>=previous.length&&
    Buffer.from(current).subarray(0,previous.length).equals(Buffer.from(previous)),
    "termination recorder truncated or rewrote an observed complete prefix");
}

/** Accepts one archived phase for offline evidence checks, or the two actual
 * cursor-bounded phases of the morning scenario. Never manufactures a phase. */
export function validateTerminationPhases(input:Uint8Array, phases:readonly QuitPhase[]) {
  const raw=Buffer.from(input);
  require(raw.length>0&&raw.length<=64*1024*1024&&raw.at(-1)===10,"termination raw size/newline bound");
  require(phases.length===1||phases.length===2,"expected one archived or two actual Quit phases");
  const rows:Row[]=[];let offset=0;
  for(const line of new TextDecoder("utf-8",{fatal:true,ignoreBOM:true}).decode(raw).split("\n").slice(0,-1)){
    const event:unknown=JSON.parse(line),end=offset+Buffer.byteLength(line)+1;
    require(event!==null&&typeof event==="object"&&!Array.isArray(event),"nonobject recorder row");
    rows.push({line:rows.length+1,offset,end,event:event as Event});offset=end;
  }
  require(offset===raw.length,"termination raw byte accounting changed");
  const boundary=(n:number)=>Number.isSafeInteger(n)&&n>=0&&(n===raw.length||rows.some(r=>r.offset===n));
  for(let i=0;i<phases.length;i++){
    const p=phases[i]!;
    require(boundary(p.start)&&boundary(p.end)&&p.start<p.end,"Quit phase is not a nonempty exact byte range");
    require(i===0||phases[i-1]!.end<=p.start,"Quit phases overlap or reverse");
  }
  const hooks=rows.filter(r=>r.event.ev==="hooked");
  require(hooks.length===1&&hooks[0]!.offset===0&&hooks[0]!.event.seq===0&&Number.isSafeInteger(hooks[0]!.event.engine_classes)&&Number(hooks[0]!.event.engine_classes)>0,"expected one positive initial hook marker");
  const stack:Row[]=[],pairs=new Map<number,Row>(),sequences=new Set<number>();let lastMain=0,appCalls=0;
  for(const row of rows){
    const e=row.event;if(e.ev==="hooked")continue;
    require((e.ev==="call"||e.ev==="ret")&&IDENTITY.every(k=>Object.hasOwn(e,k))&&
      Number.isSafeInteger(e.seq)&&Number(e.seq)>0&&!sequences.has(Number(e.seq))&&
      typeof e.from==="string"&&e.from.trim().length>0&&typeof e.thread==="string"&&e.thread.length>0&&
      Number.isSafeInteger(e.depth)&&Number(e.depth)>=0&&typeof e.cls==="string"&&typeof e.sel==="string"&&
      (e.kind==="-"||e.kind==="+")&&(e.dir==="->engine"||e.dir==="engine->app")&&
      (typeof e.self==="string"||(e.kind==="+"&&e.self===null)),"invalid caller-aware recorder metadata");
    sequences.add(Number(e.seq));
    if(e.ev==="call"&&e.from==="app"&&e.dir==="->engine")appCalls++;
    if(e.thread!=="main")continue;
    require(Number(e.seq)>lastMain,"main recorder sequence reversed");lastMain=Number(e.seq);
    if(e.ev==="call"){
      require(e.depth===stack.length,"main call depth mismatch");stack.push(row);
    }else{
      const call=stack.at(-1);
      require(call&&IDENTITY.every(k=>call.event[k]===e[k]),"main return receiver/caller/thread/depth/order mismatch");
      pairs.set(call!.offset,row);stack.pop();
    }
  }
  require(appCalls>0,"no genuine app-origin call");
  require(sequences.size>0&&[...sequences].reduce((a,b)=>Math.max(a,b),0)===sequences.size,"global recorder sequence gap");
  // A process can disappear with its public -run still open. Do not impose a
  // fictitious shutdown return, or require unrelated final calls to return.
  const terminationCalls=rows.filter(r=>r.event.ev==="call"&&r.event.cls==="ArcBrowserApplication"&&r.event.sel==="tryToTerminate:");
  require(terminationCalls.length===phases.length,"complete transcript has extra/missing public termination calls");
  require(terminationCalls.every(call=>phases.filter(p=>call.offset>=p.start&&call.offset<p.end).length===1),
    "public termination call lies outside the declared Quit phases");
  const proof=phases.map((phase,index)=>{
    const calls=terminationCalls.filter(r=>r.offset>=phase.start&&r.offset<phase.end);
    require(calls.length===1,`Quit phase ${index+1} requires exactly one public termination call`);
    const call=calls[0]!,e=call.event,returned=pairs.get(call.offset);
    require(e.thread==="main"&&e.from==="app"&&e.dir==="->engine"&&e.kind==="-"&&
      typeof e.self==="string"&&/^<ArcBrowserApplication#[1-9][0-9]*>$/.test(e.self)&&
      Array.isArray(e.args)&&e.args.length===1&&e.args[0]==="<block>","termination call is not the exact genuine main app public block call");
    require(returned&&returned.offset>call.offset&&returned.end<=phase.end&&
      Number.isInteger(returned.event.ret)&&Number(returned.event.ret)>=0&&Number(returned.event.ret)<=2,
      "termination call lacks its exact matched unsigned 0..2 return within phase");
    return {phase:index+1,...phase,call,returned};
  });
  require(proof.every(p=>p.call.event.self===proof[0]!.call.event.self),"Quit phases changed application receiver");
  return {passed:true,rawBytes:raw.length,rawSHA256:createHash("sha256").update(raw).digest("hex"),rawRows:rows.length,
    appOriginCalls:appCalls,phases:proof,blockExecution:"unobserved",returnSemantics:"unsigned representation only; no enum names",
    scope:"Exact app-origin public termination call/return per cursor-bounded Quit; all raw rows retained. No block completion or OS exit-status inference."};
}
