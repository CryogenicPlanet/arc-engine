// Disposable filesystem fault checks only. No real lock, app, profile or golden is used.
// bun tests/scenarios/upgrade-recorder-check.ts
import {mkdtempSync,mkdirSync,writeFileSync,readFileSync,cpSync,existsSync,renameSync,readdirSync,rmSync} from 'node:fs';
import {join,resolve} from 'node:path';
import {tmpdir} from 'node:os';
import {createHash} from 'node:crypto';
const source=readFileSync(join(resolve(import.meta.dir,'../..'),'tests/scenarios/upgrade-recorder.ts'),'utf8');
const area=mkdtempSync(join(tmpdir(),'scenarios-preservation-faults-'));
const sha=(p:string)=>createHash('sha256').update(readFileSync(p)).digest('hex');
const v1='{"ev":"hooked","engine_classes":1}\n{"ev":"call","dir":"->engine","fromLegacy":true}\n';
const valid='{"ev":"hooked","engine_classes":1}\n{"ev":"call","dir":"->engine","from":"app"}\n';
let count=0;
const check=(name:string,value:unknown)=>{if(!value)throw new Error(name);count++;console.log(`PASS ${name}`)};
function setup(name:string,candidate=valid,canonical=v1){
 const root=join(area,name);for(const d of ['tests/scenarios','tests/launch-slice','tests/recorder','logs/wp','logs/arccore-build.lock','results/golden/arc-1.170-launch-slice/launch-1'])mkdirSync(join(root,d),{recursive:true});
 // Isolate the profile reset in this copy. No production source or real profile path is executed.
 const code=source.replace('join(process.env.HOME!,"Library/Application Support/ArcModAgent-trace156")','join(root,"disposable-profile")');
 if(code===source)throw new Error('profile isolation replacement did not match');
 writeFileSync(join(root,'tests/scenarios/upgrade-recorder.ts'),code);
 writeFileSync(join(root,'tests/scenarios/scoreboard.ts'),`import {existsSync} from 'node:fs';export const acceptedScenarios=()=>existsSync(new URL('../../results/golden/arc-1.170-launch-slice/steps.json',import.meta.url))?['launch-slice']:[];`);
 writeFileSync(join(root,'logs/arccore-build.lock/pid'),String(process.pid));
 writeFileSync(join(root,'tests/recorder/ArcEngineRecorder.dylib'),'not an executable fixture');
 const golden=join(root,'results/golden/arc-1.170-launch-slice');
 writeFileSync(join(golden,'steps.json'),JSON.stringify({steps:[{outcome:'pass'}]}));
 writeFileSync(join(golden,'coverage.json'),JSON.stringify({accepted:true}));
 writeFileSync(join(golden,'launch-1/record.jsonl'),canonical);
 writeFileSync(join(golden,'launch-1/app.log'),'original full log\n');
 writeFileSync(join(root,'tests/launch-slice/run.ts'),`import {mkdirSync,writeFileSync} from 'node:fs';import{join}from'node:path';let out=Bun.argv[Bun.argv.indexOf('--out')+1];mkdirSync(join(out,'launch-1'),{recursive:true});writeFileSync(join(out,'steps.json'),JSON.stringify({steps:[{outcome:'pass'}]}));writeFileSync(join(out,'coverage.json'),JSON.stringify({accepted:true}));writeFileSync(join(out,'launch-1/record.jsonl'),${JSON.stringify(candidate)});writeFileSync(join(out,'harness-ran'),'yes');`);
 return {root,golden};
}
async function run(f:{root:string,golden:string}){const p=Bun.spawn(['bun',join(f.root,'tests/scenarios/upgrade-recorder.ts'),'launch-slice','--lead-recorder-upgrade','--locked'],{cwd:f.root,stdout:'pipe',stderr:'pipe'});const [code,out,err]=await Promise.all([p.exited,new Response(p.stdout).text(),new Response(p.stderr).text()]);return{code,out,err};}
const normal=setup('normal');let r=await run(normal);check('valid candidate promotes',r.code===0&&readFileSync(join(normal.golden,'launch-1/record.jsonl'),'utf8')===valid);check('original transcript bytes preserved',readFileSync(join(normal.golden,'launch-1/record.v1.jsonl'),'utf8')===v1);check('full snapshot log and integrity manifest preserved',existsSync(join(normal.golden+'.recorder-v1','snapshot-manifest.json'))&&readFileSync(join(normal.golden+'.recorder-v1','launch-1/app.log'),'utf8')==='original full log\n');
r=await run(normal);check('fully attributed rerun is idempotent',r.code===0&&r.out.includes('already caller-aware'));
const incomplete=setup('incomplete-candidate','{"ev":"hooked","engine_classes":1}\n{"ev":"call","dir":"->engine","from":"app"}\n{"ev":"call","dir":"->engine"}\n');r=await run(incomplete);check('partial caller coverage rejected without changing canonical',r.code===1&&readFileSync(join(incomplete.golden,'launch-1/record.jsonl'),'utf8')===v1);
const malformed=setup('malformed-candidate',valid+'{"ev":');r=await run(malformed);check('truncated JSON rejected with v1 intact',r.code===1&&readFileSync(join(malformed.golden,'launch-1/record.jsonl'),'utf8')===v1);
const mixed=setup('mixed-canonical',valid,valid+'{"ev":"call","dir":"->engine"}\n');r=await run(mixed);check('mixed canonical is rerecorded rather than skipped',r.code===0&&existsSync(join(mixed.golden,'harness-ran'))&&!r.out.includes('already caller-aware'));
const interrupted=setup('previous-only');renameSync(interrupted.golden,interrupted.golden+'.migration-previous');r=await run(interrupted);check('missing canonical restored before acceptance check then upgraded',r.code===0&&existsSync(join(interrupted.golden,'launch-1/record.v1.jsonl'))&&!existsSync(interrupted.golden+'.migration-previous'));
const both=setup('both-versions');r=await run(both);if(r.code!==0)throw new Error(r.err);cpSync(both.golden+'.recorder-v1',both.golden+'.migration-previous',{recursive:true});r=await run(both);check('verified completed promotion cleans interrupted backup',r.code===0&&!existsSync(both.golden+'.migration-previous'));
const corrupt=setup('corrupt-snapshot');mkdirSync(corrupt.golden+'.recorder-v1');writeFileSync(join(corrupt.golden+'.recorder-v1','record.jsonl'),'partial');const before=sha(join(corrupt.golden,'launch-1/record.jsonl'));r=await run(corrupt);check('incomplete permanent snapshot fails closed before harness',r.code!==0&&sha(join(corrupt.golden,'launch-1/record.jsonl'))===before&&!existsSync(join(corrupt.golden+'.recorder-v2.candidate','harness-ran')));
const partial=setup('abandoned-partial');mkdirSync(partial.golden+'.recorder-v1.partial-abandoned');writeFileSync(join(partial.golden+'.recorder-v1.partial-abandoned','partial'),'diagnostic');r=await run(partial);check('abandoned partial retained and complete snapshot separately published',r.code===0&&existsSync(join(partial.golden+'.recorder-v1.partial-abandoned','partial'))&&existsSync(join(partial.golden+'.recorder-v1','snapshot-manifest.json')));
console.log(`${count} filesystem fault/recovery checks passed. Fixture area: ${area}`);
