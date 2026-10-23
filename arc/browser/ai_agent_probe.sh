#!/bin/bash
# Native-event and DOM parity probe. Runs only disposable agents under shared lock.
set -euo pipefail
E="$(cd "$(dirname "$0")/../.." && pwd)"
if [ "${1:-}" = --locked ]; then shift
elif [ "${1:-run}" != build ]; then exec "$E/arc/tools/locked.sh" bash "$0" --locked "$@"
fi
build="$E/logs/wp/ai-agent-probe-build"
mkdir -p "$build"
xcrun clang++ -dynamiclib -fobjc-arc -Wall -Wextra -Werror -framework AppKit \
  "$E/arc/browser/ai_agent_probe.mm" -o "$build/AIAgentProbe.dylib"
if [ "${1:-run}" = build ]; then echo "$build/AIAgentProbe.dylib"; exit 0; fi
if [ "${1:-run}" != run ]; then echo 'usage: ai_agent_probe.sh build | run [trace156,ours156] [OUTPUT]' >&2; exit 2; fi
export ARC_AI_AGENT_PROBE_ROOT="$E"
export ARC_AI_AGENT_PROBE_AGENTS="${2:-trace156,ours156}"
export ARC_AI_AGENT_PROBE_OUT="${3:-$E/results/ai-agent-probe/$(date -u +%Y%m%dT%H%M%SZ)}"
bun run - <<'JS'
import {existsSync,mkdirSync,readFileSync,writeFileSync,renameSync} from 'node:fs';
import {join,resolve} from 'node:path';
const root=process.env.ARC_AI_AGENT_PROBE_ROOT;
const {AgentCDP,verifyAgentEndpoint}=await import(join(root,'../arc/tools/agent-cdp.ts'));
const agents=process.env.ARC_AI_AGENT_PROBE_AGENTS.split(',');
if(!agents.length||new Set(agents).size!==agents.length||agents.some(a=>!['trace156','ours156'].includes(a)))throw Error('only trace156/ours156 allowed');
const out=resolve(process.env.ARC_AI_AGENT_PROBE_OUT);mkdirSync(out,{recursive:true});
const exec=args=>{const r=Bun.spawnSync(args,{stdout:'pipe',stderr:'pipe'});if(!r.success)throw Error(r.stderr.toString());return r.stdout.toString().trim()};
const lockPID=Number(readFileSync(join(root,'logs/arccore-build.lock/pid'),'utf8').trim());let ancestor=process.pid;
for(let depth=0;ancestor!==lockPID&&ancestor>1&&depth<20;++depth)ancestor=Number(exec(['ps','-p',String(ancestor),'-o','ppid=']));
if(ancestor!==lockPID)throw Error('shared lock is required');
const pids=exe=>exec(['ps','-axo','pid=,command=']).split('\n').flatMap(r=>{const m=r.match(/^\s*(\d+)\s+(.+)$/);return m&&(m[2]===exe||m[2].startsWith(exe+' '))?[Number(m[1])]:[]});
const sleep=ms=>new Promise(r=>setTimeout(r,ms));const json=p=>JSON.parse(readFileSync(p,'utf8'));
const token='arc-ai-agent-probe-'+crypto.randomUUID();
const eventsScript=`window.events=[];for(const type of ['keydown','keypress','beforeinput','input','keyup'])document.addEventListener(type,e=>window.events.push({type,key:e.key??null,code:e.code??null,keyCode:e.keyCode??null,data:e.data??null,inputType:e.inputType??null,target:e.target.id,trusted:e.isTrusted}),true);`;
const server=Bun.serve({hostname:'127.0.0.1',port:0,fetch(req){const u=new URL(req.url);if(!u.pathname.startsWith('/'+token+'/'))return new Response('missing',{status:404});const child=u.pathname.endsWith('/frame');return new Response(`<!doctype html><meta charset="utf-8"><title>AI agent input fixture</title><input id="${child?'child':'main'}"><script>${eventsScript}</script>${child?'':`<iframe id="frame" src="/${token}/frame"></iframe>`}`,{headers:{'content-type':'text/html'}})}});
const url=`http://127.0.0.1:${server.port}/${token}/main`;const runs=[];
try{for(const agent of agents){
 const dir=join(out,agent);mkdirSync(dir,{recursive:true});const app=resolve(root,`../arc/build/ArcModAgent-${agent}.app`),exe=join(app,'Contents/MacOS/Arc'),port=9458;
 const run={agent,observations:[]};runs.push(run);let pid,cdp,target;const deadline=Date.now()+47000;
 const wait=async(fn,ms=12000)=>{let last;const end=Math.min(deadline,Date.now()+ms);while(Date.now()<end){try{const v=await fn();if(v)return v}catch(e){last=e}await sleep(75)}throw Error('probe wait timeout'+(last?': '+last:''))};
 try{
  const plist=k=>exec(['/usr/libexec/PlistBuddy','-c','Print :'+k,join(app,'Contents/Info.plist')]);
  if(plist('ArcModAgentSession')!==agent||plist('CFBundleIdentifier')!==`company.thebrowser.Browser.mod.agent.${agent}`)throw Error('bundle identity mismatch');
  if(pids(exe).length)throw Error('agent already running; refusing attach');
  if(Bun.spawnSync(['/usr/sbin/lsof','-nP',`-iTCP:${port}`,'-sTCP:LISTEN'],{stdout:'ignore',stderr:'ignore'}).success)throw Error('CDP port occupied');
  const request=join(dir,'request.json');const data=join(process.env.HOME,'Library/Application Support',`ArcModAgent-${agent}`,'Agent Chromium Data');mkdirSync(data,{recursive:true});
  exec(['open','-n','-g','-a',app,'--env','DYLD_INSERT_LIBRARIES='+join(root,'logs/wp/ai-agent-probe-build/AIAgentProbe.dylib'),'--env','ARC_AI_AGENT_PROBE_REQUEST='+request,'--stdout',join(dir,'app.log'),'--stderr',join(dir,'app.log'),'--args','--use-mock-keychain','--no-default-browser-check','--disable-sync','--disable-background-networking','--host-resolver-rules='+plist('ArcModHostResolverRules'),'--user-data-dir='+data,'--remote-debugging-address=127.0.0.1','--remote-debugging-port='+port]);
  pid=await wait(()=>pids(exe)[0]);run.pid=pid;
  await wait(()=>{const r=json(request+'.ready.json');const ready=json(join(process.env.HOME,'Library/Application Support',`ArcModAgent-${agent}`,'ArcModAgentReady.json'));return r.pid===pid&&r.installed&&ready.pid===pid&&ready.ready===true},20000);
  await wait(()=>verifyAgentEndpoint(agent,port)===pid,20000);cdp=await AgentCDP.connectAgent(agent,port);
  target=(await cdp.request('Target.createTarget',{url})).targetId;if(typeof target!=='string')throw Error('missing fixture target');
  const evaluate=async(expression)=>{const r=await cdp.targetCommand(target,'Runtime.evaluate',{expression,returnByValue:true,awaitPromise:true});if(r.exceptionDetails)throw Error(JSON.stringify(r.exceptionDetails));return r.result?.value};
  await wait(()=>evaluate("document.readyState==='complete'&&document.getElementById('frame')?.contentDocument?.readyState==='complete'"));
  let seq=0;
  const action=async(operation,extra={})=>{if(verifyAgentEndpoint(agent,port)!==pid)throw Error('endpoint changed');const id=token+'-'+(++seq);writeFileSync(request+'.tmp',JSON.stringify({id,url,operation,...extra}));renameSync(request+'.tmp',request);const response=await wait(()=>{const r=json(request+'.response.json');return r.id===id?r:null});if(!response.ok||!response.identity||response.matches!==1)throw Error(JSON.stringify(response));return response};
  await action('inspect');
  for(const focus of ['main','iframe']){
   await evaluate(`(()=>{const d=${focus==='main'?'document':"document.getElementById('frame').contentDocument"};document.getElementById('main').value='';document.getElementById('frame').contentDocument.getElementById('child').value='';window.events=[];document.getElementById('frame').contentWindow.events=[];d.querySelector('input').focus();return true})()`);
   const cases=[['text',{text:'Azπ東京😀'}],['nil-text',{}],['text',{text:''}],...[2,3,4,5,0,1,6,-1,4294967298,-4294967293].map(key=>['key',{key}])];
   for(const [operation,extra]of cases){
    await evaluate(`(()=>{const main=document.getElementById('main'),child=document.getElementById('frame').contentDocument.getElementById('child');main.value=child.value=${JSON.stringify(operation==='key'?'seed':'')};window.events=[];document.getElementById('frame').contentWindow.events=[];const input=${focus==='main'?'main':'child'};input.focus();input.setSelectionRange(input.value.length,input.value.length);return true})()`);
    const native=await action(operation,extra);if((operation==='key'||extra.text?.length)&&!native.events?.length)throw Error('no native keyDown captured for '+operation);
    let previous='',stable=0;const dom=await wait(async()=>{const value=await evaluate(`({main:document.getElementById('main').value,child:document.getElementById('frame').contentDocument.getElementById('child').value,activeMain:document.activeElement?.id,activeChild:document.getElementById('frame').contentDocument.activeElement?.id,mainEvents:window.events,childEvents:document.getElementById('frame').contentWindow.events})`);const serial=JSON.stringify(value);stable=serial===previous?stable+1:0;previous=serial;return stable>=2?value:null});run.observations.push({focus,operation,...extra,identity:native.identity,events:native.events,dom});
   }
  }
  run.frameTree=await cdp.targetCommand(target,'Page.getFrameTree',{});
  console.log(agent+': '+run.observations.length+' input observations');
 }catch(e){run.error=String(e);console.error(agent+': '+run.error)}finally{
  if(cdp&&target)try{await cdp.request('Target.closeTarget',{targetId:target})}catch{}
  if(cdp)await cdp.close();if(pid)Bun.spawnSync(['kill','-TERM',String(pid)],{stderr:'ignore'});await sleep(500);
  writeFileSync(join(dir,'observations.json'),JSON.stringify(run,null,2));
 }
}}finally{server.stop(true)}
const clean=r=>({observations:r.observations,error:r.error??null});
const match=runs.length===2&&!runs.some(r=>r.error)&&JSON.stringify(clean(runs[0]))===JSON.stringify(clean(runs[1]));
const differences=runs.length===2?Array.from({length:Math.max(...runs.map(r=>r.observations.length))},(_,index)=>{const a=runs[0].observations[index],b=runs[1].observations[index];return JSON.stringify(a)===JSON.stringify(b)?null:{index,focus:a?.focus??b?.focus,operation:a?.operation??b?.operation,key:a?.key??b?.key,nativeMatch:JSON.stringify(a?.events)===JSON.stringify(b?.events),domMatch:JSON.stringify(a?.dom)===JSON.stringify(b?.dom)}}).filter(Boolean):[];
writeFileSync(join(out,'comparison.json'),JSON.stringify({match,differences,runs:runs.map(r=>({agent:r.agent,error:r.error??null,observations:r.observations.length}))},null,2));
console.log('OUTPUT '+out);if(runs.some(r=>r.error)||(runs.length===2&&!match))process.exitCode=1;console.log('MATCH '+match);
JS
