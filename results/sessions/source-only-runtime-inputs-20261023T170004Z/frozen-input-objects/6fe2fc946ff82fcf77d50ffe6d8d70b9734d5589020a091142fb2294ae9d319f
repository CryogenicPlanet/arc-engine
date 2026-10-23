// Adapted unchanged from the batch6 SHA256-frozen storage helper.
// Opaque storage-presence barrier for a fresh disposable session test profile.
// This neither decrypts data nor claims a file contains the fixture; actual
// post-relaunch restoration and exact callback/history assertions prove that.
import {existsSync,lstatSync,realpathSync,readdirSync,readFileSync} from 'node:fs';
import {join,resolve,basename} from 'node:path';
import {createHash} from 'node:crypto';
function demand(ok:unknown,why:string):asserts ok {if(!ok)throw Error('opaque sessions snapshot: '+why)}
function directory(path:string){demand(existsSync(path),'missing directory '+path);const s=lstatSync(path);demand(s.isDirectory()&&!s.isSymbolicLink()&&realpathSync(path)===resolve(path),'nonregular/symlinked directory '+path)}
export function snapshotSessionStorage(agent:string,contextKey:string){
 demand(['trace156','ours156'].includes(agent),'unapproved disposable agent');
 const m=/^([^/]+)\/0$/.exec(contextKey);demand(m,'nonpersistent/invalid context');const context=m[1]!;
 demand(basename(context)===context&&context!=='.'&&context!=='..'&&!context.includes('\\'),'unsafe context basename');
 const app=join(process.env.HOME!,'Library/Application Support','ArcModAgent-'+agent),userData=join(app,agent==='trace156'?'User Data':'Agent Chromium Data'),profile=join(userData,context);
 directory(app);directory(userData);directory(profile);
 const files:{path:string;name:string;generation:string;size:number;mtime:string;sha256:string;version:number;encrypted:boolean}[]=[];
 for(const folder of ['Sessions','Sessions_Encrypted']){
  const dir=join(profile,folder);if(!existsSync(dir))continue;directory(dir);
  for(const name of readdirSync(dir)){
   const match=/^Tabs_([0-9]+)$/.exec(name);if(!match)continue;
   const path=join(dir,name),before=lstatSync(path);
   demand(before.isFile()&&!before.isSymbolicLink()&&realpathSync(path)===path,'nonregular/symlinked Tabs file');
   demand(before.size>8&&before.size<=64*1024*1024,'empty/excessive Tabs file');
   const data=readFileSync(path),after=lstatSync(path);
   demand(before.ino===after.ino&&before.size===after.size&&before.mtimeMs===after.mtimeMs&&data.length===before.size,'Tabs file changed while snapshotting');
   demand(data.readUInt32LE(0)===0x53534e53,'invalid SNSS signature');const version=data.readUInt32LE(4);
   demand((folder==='Sessions'&&version===3)||(folder==='Sessions_Encrypted'&&version===5),'unknown storage folder/version pair');
   files.push({path,name,generation:match[1]!,size:data.length,mtime:before.mtime.toISOString(),sha256:createHash('sha256').update(data).digest('hex'),version,encrypted:version===5});
  }
 }
 demand(files.length>0,'no nonempty Tabs file saved yet');
 files.sort((a,b)=>BigInt(a.generation)>BigInt(b.generation)?-1:BigInt(a.generation)<BigInt(b.generation)?1:a.path.localeCompare(b.path));
 return {scope:'opaque storage presence only; no payload inspection, decryption, or fixture-content claim',agent,contextKey,observedAt:new Date().toISOString(),selected:files[0],files};
}
