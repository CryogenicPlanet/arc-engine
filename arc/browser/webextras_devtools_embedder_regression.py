#!/usr/bin/env python3
"""Compare extracted original/staged frontend producers; run staged close mocks.

Requires the hash-pinned original JS extraction; no app, GN or Chromium build.
Controller constructors, Chromium IPC and the task runner are not executed.
"""
from pathlib import Path
import argparse,hashlib,json,subprocess,tempfile

ROOT=Path(__file__).resolve().parents[2]
parser=argparse.ArgumentParser()
parser.add_argument('--original-resource-dir',type=Path,required=True)
args=parser.parse_args()
proof=Path(tempfile.mkdtemp(prefix='webextras-embedder-regression-'))
stage=proof/'stage'
shared_paths=['chrome/browser/arcmod/arcmod_hooks.h','chrome/browser/devtools/devtools_window.cc','chrome/browser/devtools/devtools_ui_bindings.cc','chrome/browser/devtools/devtools_ui_bindings.h','chrome/browser/devtools/devtools_embedder_message_dispatcher.cc','chrome/browser/devtools/devtools_embedder_message_dispatcher.h','third_party/devtools-frontend/src/front_end/core/host/InspectorFrontendHostAPI.ts','third_party/devtools-frontend/src/front_end/core/host/InspectorFrontendHostStub.ts','third_party/devtools-frontend/src/front_end/devtools_compatibility.js','third_party/devtools-frontend/src/front_end/panels/elements/InspectElementModeController.ts','third_party/devtools-frontend/src/front_end/ui/legacy/DockController.ts']
for name in shared_paths:
 p=stage/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes((ROOT/'chromium/src'/name).read_bytes())
for name in ['webextras_devtools_hooks.patch','webextras_devtools_host_docking.patch','webextras_devtools_embedder_events.patch']:
 subprocess.run(['git','apply','--check','--whitespace=error',str(ROOT/'arc/browser'/name)],cwd=stage,check=True)
 subprocess.run(['git','apply',str(ROOT/'arc/browser'/name)],cwd=stage,check=True)
p=stage/'arc/api/arc_webextras_devtools.mm';p.parent.mkdir(parents=True);p.write_bytes((ROOT/'arc/api/arc_webextras_devtools.mm').read_bytes())
for name in ['webextras_devtools_integration_held.patch','webextras_devtools_embedder_companion_held.patch']:
 subprocess.run(['git','apply','--check','--whitespace=error',str(ROOT/'arc/browser'/name)],cwd=stage,check=True)
 subprocess.run(['git','apply',str(ROOT/'arc/browser'/name)],cwd=stage,check=True)
original={}
for resource,digest in [('52853','2616bfe4be3a3ce9eab925f250452f2f1e21398c763cb1a0c790787092e32702'),('53015','723be0b9811a6686918ea2889e72a69466d07197fcb3313804f86fa86884853e'),('52490','5155e65c618a6776d555abd85bcafdabe1d4fc606aaeeaa6292cea1eb3c6217c')]:
 data=(args.original_resource_dir/resource).read_bytes()
 assert hashlib.sha256(data).hexdigest()==digest,resource+' original identity mismatch'
 original[resource]=data.decode()
# Balanced extraction ignores braces within strings/comments. The pinned minified
# original methods below contain no template literal interpolation or regex braces.
def method(text,anchor):
 start=text.index(anchor); opening=text.index('{',start); depth=0;quote=None;comment=None;i=opening
 while i<len(text):
  c=text[i];two=text[i:i+2]
  if comment=='//':
   if c=='\n':comment=None
  elif comment=='/*':
   if two=='*/':comment=None;i+=1
  elif quote:
   if c=='\\':i+=1
   elif c==quote:quote=None
  elif two in ('//','/*'):comment=two;i+=1
  elif c in ('"',"'",'`'):quote=c
  elif c=='{':depth+=1
  elif c=='}':
   depth-=1
   if not depth:return text[start:i+1]
  i+=1
 raise AssertionError('unclosed '+anchor)
front=stage/'third_party/devtools-frontend/src/front_end'
inspect=(front/'panels/elements/InspectElementModeController.ts').read_text()
dock=(front/'ui/legacy/DockController.ts').read_text()
oinspect=original['52853'][original['52853'].index('setMode(e){if(!Nt.TargetManager'):]
# Earlier methods occur before setMode; use exact unique anchors in the full file.
oi='\n'.join(method(original['52853'],a) for a in ['setMode(e){if(!Nt.TargetManager','modelAdded(e){this.mode!==Ut','isInInspectElementMode(){return this.mode!==Ut','suspendStateChanged(){Nt.TargetManager','showDetailedInspectTooltipChanged(){this.setMode'])
si='\n'.join(method(inspect,a) for a in ['private setMode(','modelAdded(','private isInInspectElementMode(','private suspendStateChanged(','private showDetailedInspectTooltipChanged('])
od='\n'.join(method(original['53015'],a) for a in ['initialize(){if(!this.#e)','dockSideChanged(){this.setDockSide','setDockSide(e){if(dt.indexOf','setIsDockedResponse(e){this.dispatchEventToListeners'])
sd='\n'.join(method(dock,a) for a in ['initialize():','private dockSideChanged():','setDockSide(dockSide:','private setIsDockedResponse('])
compat=(front/'devtools_compatibility.js').read_text()
ocompat='\n'.join(method(original['52490'],a) for a in ['inspectModeChanged(inspectMode)','requestDockPosition(dockPosition)'])
scompat='\n'.join(method(compat,a) for a in ['inspectModeChanged(mode)','requestDockPosition(side)'])
payload={'oi':oi,'si':si,'od':od,'sd':sd,'oc':ocompat,'sc':scompat,'ts':str(ROOT/'chromium/src/third_party/devtools-frontend/src/node_modules/typescript/lib/typescript.js')}
(proof/'methods.json').write_text(json.dumps(payload,indent=2))
js=r'''
const fs=require('fs'),vm=require('vm'),assert=require('assert/strict');
const p=JSON.parse(fs.readFileSync(process.argv[2])),ts=require(p.ts);
let log=[],suspended=false,models=[];
const mode={None:'none',SearchForNode:'searchForNode',SearchForUAShadowDOM:'searchForUAShadowDOM',CaptureAreaScreenshot:'captureAreaScreenshot'};
const manager={allTargetsSuspended:()=>suspended,models:()=>models};
const host={inspectModeChanged:v=>log.push(['inspect',v]),requestDockPosition:v=>log.push(['dock',v]),setIsDocked:(v,cb)=>{log.push(['setIsDocked',v]);host.ack=cb;}};
const target={TargetManager:{instance:()=>manager}};
const SDK={TargetManager:target,OverlayModel:{OverlayModel:{}}};
const Host={InspectorFrontendHost:{InspectorFrontendHostInstance:host}};
const Protocol={Overlay:{InspectMode:mode}};
const states=['right','bottom','left','undocked'];
const DockState={RIGHT:'right',BOTTOM:'bottom',LEFT:'left',UNDOCKED:'undocked'};
const Events={BEFORE_DOCK_SIDE_CHANGED:'BeforeDockSideChanged',DOCK_SIDE_CHANGED:'DockSideChanged',AFTER_DOCK_SIDE_CHANGED:'AfterDockSideChanged'};
const document={body:{classList:{remove:v=>log.push(['css-remove',v]),add:v=>log.push(['css-add',v])}}};
const consoleMock={timeStamp:v=>log.push(['timestamp',v])};
function compile(code){const r=ts.transpileModule(code,{compilerOptions:{target:ts.ScriptTarget.ES2022,module:ts.ModuleKind.CommonJS},reportDiagnostics:true});assert.equal(r.diagnostics.length,0);return r.outputText;}
function makeClass(name,fields,methods){const context={Nt:SDK,Ut:{InspectMode:mode},Gc:Host,SDK,Host,Protocol,Dt:Host,dt:states,states,DockState,Events,document,console:consoleMock};vm.createContext(context);return vm.runInContext(compile(`class ${name}{${fields}\n${methods}}\n${name}`),context);}
const inspectFields=`mode='none';toggleSearchAction={setToggled:v=>record(['toggle',v])};showDetailedInspectTooltipSetting={get:()=>true};`;
// Inject logging function through the realm, rather than replacing any producer body.
function inspectClass(name,methods){const context={Nt:SDK,Ut:{InspectMode:mode},Gc:Host,SDK,Host,Protocol,record:v=>log.push(JSON.parse(JSON.stringify(v)))};vm.createContext(context);return vm.runInContext(compile(`class ${name}{${inspectFields}${methods}}\n${name}`),context);}
const OI=inspectClass('OI',p.oi),SI=inspectClass('SI',p.si);
function model(n){return {setInspectMode:(v,t)=>{log.push(['model',n,v,t]);return new Promise(()=>{});}};}
function inspectSuite(C){const c=new C;log=[];suspended=false;models=[model(1),model(2)];c.setMode('searchForNode');assert.equal(log.filter(x=>x[0]==='inspect').length,1);assert.deepEqual(log.map(x=>x[0]),['model','model','toggle','inspect']);const out=[log];log=[];c.showDetailedInspectTooltipChanged();assert.equal(log.filter(x=>x[0]==='inspect').length,1);out.push(log);log=[];models=[];c.setMode('none');c.setMode('none');assert.equal(log.filter(x=>x[0]==='inspect').length,2);out.push(log);log=[];suspended=true;c.setMode('captureAreaScreenshot');assert.equal(log.length,0);c.suspendStateChanged();assert.deepEqual(log,[['toggle',false]]);out.push(log);suspended=false;log=[];c.setMode('searchForUAShadowDOM');log=[];c.modelAdded(model(3));assert.deepEqual(log.map(x=>x[0]),['model']);out.push(log);return JSON.parse(JSON.stringify(out));}
assert.deepEqual(inspectSuite(OI),inspectSuite(SI));
const dockFields=`closeButton={setVisible:v=>record(['visible',v])};currentDockStateSetting={get:()=>this.setting,set:v=>{this.setting=v;record(['current',v]);}};lastDockStateSetting={set:v=>record(['last',v])};setting='right';dispatchEventToListeners(n,v){record([n,v]);}announceDockLocation(){record(['announce']);}`;
function dockClass(name,priv,methods){const context={Dt:Host,Host,dt:states,states,DockState,Events,document,console:consoleMock,record:v=>log.push(JSON.parse(JSON.stringify(v)))};vm.createContext(context);return vm.runInContext(compile(`class ${name}{${priv}${dockFields}${methods}}\n${name}`),context);}
const OD=dockClass('OD',`#e;#t;constructor(can){this.#e=can;this.#t=can?undefined:'undocked';}`,p.od);
const SD=dockClass('SD',`#canDock;#dockSide;constructor(can){this.#canDock=can;this.#dockSide=can?undefined:'undocked';}`,p.sd);
function dockSuite(C){let c=new C(false);log=[];c.initialize();assert.deepEqual(log,[['dock','undocked']]);const out=[log];c=new C(true);log=[];c.initialize();assert.equal(log[0][0],'dock');assert.equal(log[1][0],'css-add');assert(log.findIndex(x=>x[0]==='BeforeDockSideChanged')<log.findIndex(x=>x[0]==='setIsDocked'));assert(!log.some(x=>x[0]==='AfterDockSideChanged'));host.ack();assert.equal(log.at(-1)[0],'announce');out.push(log);log=[];c.setDockSide('right');assert.equal(log.length,0);out.push(log);log=[];c.setDockSide('bottom');assert.deepEqual(log[0],['dock','bottom']);out.push(log);log=[];c.setDockSide('invalid');assert.deepEqual(log[0],['dock','right']);out.push(log);return JSON.parse(JSON.stringify(out));}
assert.deepEqual(dockSuite(OD),dockSuite(SD));
function compat(methods){const sent=[];const context={DevToolsAPI:{sendMessageToEmbedder:(...a)=>sent.push(a)}};vm.createContext(context);const c=vm.runInContext(`new (class {${methods}})`,context);c.inspectModeChanged('none');c.requestDockPosition('left');return JSON.parse(JSON.stringify(sent));}
assert.deepEqual(compat(p.oc),compat(p.sc));
assert.deepEqual(compat(p.sc),[['inspectModeChanged',['none'],null],['requestDockPosition',['left']]]);
process.stdout.write('11 original/staged inspect, dock and compatibility producer cases passed; extracted methods/TypeScript syntax only\n');
'''
(proof/'producer.cjs').write_text(js)
subprocess.run(['node',str(proof/'producer.cjs'),str(proof/'methods.json')],check=True)
# Reuse the existing actual-production extraction harness on the composed staged
# companion, adding a pending natural-destruction case absent from the old harness.
close=(ROOT/'arc/browser/webextras_devtools_close_regression.py').read_text()
needle='''    // 1. Typed native close precedes synchronous app notification.'''
extra=r'''
    // An associated peer can die before Show. Observer presence is the native
    // peer proxy (original43586ac); WeakPtr validity is intentionally irrelevant.
    {
      content::WebContents inspected, front1, front2;
      ArcWebContents* owner=Owner(&inspected); MockDelegate* delegate=[MockDelegate new];
      ArcWebContentsDevTools* tools=[[ArcWebContentsDevTools alloc] initForOwner:owner delegate:delegate];
      [tools fixtureFrontend:&front1 shown:NO opening:YES];
      front1.QueueDestruction(); events=[NSMutableArray array]; content::Drain();
      Exact(@[@"did-close"]); Check(delegate.count==1,@"pending peer did not notify exactly once");
      [tools frontendClosed]; Check(delegate.count==1,@"pending close repeated");
      [tools fixtureFrontend:&front1 shown:NO opening:YES]; front1.QueueDestruction();
      [tools fixtureFrontend:&front2 shown:NO opening:YES]; content::Drain();
      [tools checkFrontend:&front2 shown:NO opening:YES queries:0];
      Check(delegate.count==1,@"retired pending observer closed replacement");
    }
'''
assert close.count(needle)==1;close=close.replace(needle,extra+needle)
q=stage/'arc/browser/webextras_devtools_close_regression.py';q.parent.mkdir(parents=True,exist_ok=True);q.write_text(close)
subprocess.run(['python3',str(q)],check=True)
print('Additional staged pending-natural-close/replacement case passed; Foundation mocks only')
print('Proof:',proof)
# Reuse only mock declarations from the existing display harness; every bridge,
# mapping, receipt and Show/reset body below is extracted from the staged source.
import ast
constants={}
for node in ast.parse((ROOT/'arc/browser/webextras_devtools_display_regression.py').read_text()).body:
 if isinstance(node,ast.Assign) and isinstance(node.value,ast.Constant) and isinstance(node.value.value,str):
  for target in node.targets:
   if isinstance(target,ast.Name):constants[target.id]=node.value.value
source=(stage/'arc/api/arc_webextras_devtools.mm').read_text()
helper=source.split('// BEGIN WEBEXTRAS_DEVTOOLS_DISPLAY\n')[1].split('// END WEBEXTRAS_DEVTOOLS_DISPLAY')[0]
helper+='\n'+method(source,'static void PostDevToolsInspectMode(')
implementation=source.split('@implementation ArcWebContentsDevTools\n')[1].split('\n@end')[0]
bodies='\n'.join(method(implementation,a) for a in ['- (void)setDelegate:','- (void)didCreateFrontend:','- (void)didShowFrontend:','- (void)requestedDockPosition:','- (void)inspectModeChanged:','- (void)resetFrontendState {'])
preamble=constants['preamble'].replace('- (void)webContentsDevTools:(id)tools displayAs:(NSUInteger)mode;','- (void)webContentsDevTools:(id)tools displayAs:(NSUInteger)mode;\n- (void)webContentsDevTools:(id)tools inspectModeDidChange:(NSUInteger)mode;')
interface=constants['interface'].replace('- (void)setDelegate:(id<WebExtrasDevToolsDelegate>)delegate;','''- (void)setDelegate:(id<WebExtrasDevToolsDelegate>)delegate;
- (void)didCreateFrontend:(content::WebContents*)frontend;
- (void)requestedDockPosition:(const std::string&)side;
- (void)inspectModeChanged:(const std::string&)mode;
- (void)checkPending:(content::WebContents*)frontend;''')
interface+='\n- (void)checkPending:(content::WebContents*)frontend {Check(!_shown && _opening && _frontend.get()==frontend && _frontendObserver!=nullptr,@"early frontend association/readiness lost");}\n'
checks=r'''
@end
@interface MockDelegate : NSObject <WebExtrasDevToolsDelegate>
@property(nonatomic,strong) NSMutableArray* modes;
@property(nonatomic,strong) NSMutableArray* inspect;
@end
@implementation MockDelegate
- (instancetype)init {if((self=[super init])){_modes=[NSMutableArray array];_inspect=[NSMutableArray array];}return self;}
- (void)webContentsDevTools:(id)tools displayAs:(NSUInteger)mode {(void)tools;[_modes addObject:@(mode)];}
- (void)webContentsDevTools:(id)tools inspectModeDidChange:(NSUInteger)mode {(void)tools;[_inspect addObject:@(mode)];}
@end
static ArcWebContents* Owner(content::WebContents* native){auto* owner=[ArcWebContents new];owner->native=native;return owner;}
int main(){@autoreleasepool{
 tasks=[NSMutableArray array];
 // An explicit request can deliver before Show. Show must not duplicate it.
 {content::WebContents native,front;auto* owner=Owner(&native);auto* tools=[[ArcWebContentsDevTools alloc]initForOwner:owner];auto* delegate=[MockDelegate new];[tools setDelegate:delegate];
 [tools didCreateFrontend:&front];[tools checkPending:&front];[tools requestedDockPosition:"left"];
 Check(tasks.count==1 && !delegate.modes.count,@"dock request was lost/inline");Drain();
 Check([delegate.modes isEqual:@[@2]],@"requested side was reconstructed from current settings");[tools checkPending:&front];
 [tools seedQuery];unsigned before=queryCalls;[tools didShowFrontend:&front docked:YES];[tools checkShown:&front];
 Check(tasks.count==0 && queryCalls==before+1 && delegate.modes.count==1,@"Show duplicated display or failed query work");}
 // Exact native string mappings, including fallback, are captured at receipt.
 {content::WebContents native,front;auto* owner=Owner(&native);auto* tools=[[ArcWebContentsDevTools alloc]initForOwner:owner];auto* delegate=[MockDelegate new];[tools setDelegate:delegate];[tools didCreateFrontend:&front];
 for(const auto* side:{"undocked","left","right","bottom","other"}){[tools requestedDockPosition:side];}
 for(const auto* mode:{"searchForNode","searchForUAShadowDOM","captureAreaScreenshot","showDistances","none"}){[tools inspectModeChanged:mode];}
 Check(!delegate.modes.count && !delegate.inspect.count,@"event delivered inline");Drain();
 Check([delegate.modes isEqual:@[@1,@2,@3,@4,@0]] && [delegate.inspect isEqual:@[@1,@2,@3,@4,@0]],@"native scalar mapping mismatch");}
 // Accepted tasks retain the old registration across nil/new and frontend reset;
 // they must neither be epoch-cancelled nor overwrite replacement frontend state.
 {content::WebContents native,front1,front2;auto* owner=Owner(&native);auto* tools=[[ArcWebContentsDevTools alloc]initForOwner:owner];auto* old=[MockDelegate new];auto* fresh=[MockDelegate new];[tools setDelegate:old];[tools didCreateFrontend:&front1];
 [tools requestedDockPosition:"bottom"];[tools inspectModeChanged:"captureAreaScreenshot"];[tools setDelegate:nil];[tools setDelegate:fresh];[tools resetFrontendState];[tools didCreateFrontend:&front2];Drain();
 Check([old.modes isEqual:@[@4]] && [old.inspect isEqual:@[@3]] && !fresh.modes.count && !fresh.inspect.count,@"accepted old-registration tasks canceled/retargeted");[tools checkPending:&front2];}
 // A nonnil replacement updates the same native bridge before delivery.
 {content::WebContents native;auto* owner=Owner(&native);auto* tools=[[ArcWebContentsDevTools alloc]initForOwner:owner];auto* old=[MockDelegate new];auto* fresh=[MockDelegate new];[tools setDelegate:old];[tools inspectModeChanged:"searchForNode"];[tools setDelegate:fresh];Drain();Check(!old.inspect.count && [fresh.inspect isEqual:@[@1]],@"nonnil replacement did not update queued bridge");}
 // Native owner anchor remains a proxy; ordinary owner destruction cancels work.
 {auto native=std::make_unique<content::WebContents>();auto* owner=Owner(native.get());auto* tools=[[ArcWebContentsDevTools alloc]initForOwner:owner];auto* delegate=[MockDelegate new];[tools setDelegate:delegate];[tools inspectModeChanged:"searchForNode"];native.reset();Drain();Check(!delegate.inspect.count,@"dead native owner delivered inspect event");}
 Check(tasks.count==0,@"event tasks remained");puts("5 staged event receipt/association/queue groups passed; Foundation mocks, native runner and Impl proxy unverified");
}}
'''
fixture=proof/'events.mm';binary=proof/'events'
fixture.write_text(preamble+'\n'+helper+'\n'+interface+'\n'+bodies+'\n'+checks)
subprocess.run(['xcrun','clang++','-std=c++20','-fobjc-arc','-O2','-Wall','-Wextra','-Werror','-framework','Foundation',str(fixture),'-o',str(binary)],check=True)
subprocess.run([str(binary)],check=True,timeout=15)
print('Staged event extraction SHA256:',hashlib.sha256((helper+bodies).encode()).hexdigest())
# Execute the actual post-construction factory tail against expiring weak anchors.
# Construction/ClosePage themselves stay mocks; the retirement branch is real.
window=(stage/'chrome/browser/devtools/devtools_window.cc').read_text()
create=method(window,'DevToolsWindow* DevToolsWindow::Create(')
tail=create[create.index('  auto weak_window = window->weak_factory_.GetWeakPtr();'):create.rfind('\n}')]
factory=r'''
#include <cassert>
#include <functional>
#include <memory>
template<class T> struct Weak {std::weak_ptr<T*> state;T* get()const{auto s=state.lock();return s?*s:nullptr;}T* operator->()const{return get();}explicit operator bool()const{return get()!=nullptr;}};
template<class T> struct Factory {std::shared_ptr<T*> state;explicit Factory(T* p):state(std::make_shared<T*>(p)){}Weak<T> GetWeakPtr(){return {state};}~Factory(){*state=nullptr;}};
struct Contents {Factory<Contents> weak_factory_{this};};
constexpr int kClosing=2;
struct Window {Factory<Window> weak_factory_{this};int life_stage_=0;Contents* main_web_contents_=nullptr;std::function<void()> retire;void InspectedContentsClosing(){auto copied=retire;copied();}};
namespace arcmod {struct Hook {std::function<void(Contents*,Contents*)> fn;bool is_null()const{return !fn;}void Run(Contents*a,Contents*b){fn(a,b);}};static Hook hook;Hook& GetDevToolsFrontendCreatedHook(){return hook;}}
static Window* Tail(Window* window,Contents* inspected_web_contents,Weak<Contents> weak_inspected){
'''+tail+r'''
}
int main(){
 unsigned calls=0,retired=0;
 // Live normal route calls once with actual anchors, before attachment/show.
 {Contents target,front;auto w=std::make_unique<Window>();w->main_web_contents_=&front;arcmod::hook.fn=[&](Contents*t,Contents*f){assert(t==&target&&f==&front);++calls;};assert(Tail(w.get(),&target,target.weak_factory_.GetWeakPtr())==w.get());}
 // Constructor-side target destruction is noticed before notifying the hook.
 {auto t=std::make_unique<Contents>();auto weak=t->weak_factory_.GetWeakPtr();auto* raw=t.get();t.reset();auto w=std::make_unique<Window>();w->retire=[&]{++retired;w.reset();};assert(Tail(w.get(),raw,weak)==nullptr);assert(retired==1&&calls==1&&!w);}
 // Hook target destruction retires through native path, which may delete window.
 {auto t=std::make_unique<Contents>();auto weak=t->weak_factory_.GetWeakPtr();auto* raw=t.get();auto w=std::make_unique<Window>();w->retire=[&]{++retired;w.reset();};arcmod::hook.fn=[&](Contents*,Contents*){++calls;t.reset();};assert(Tail(w.get(),raw,weak)==nullptr);assert(retired==2&&calls==2&&!w);}
 // A hook that destroys the window leaves no post-hook member access/retirement.
 {Contents t;auto w=std::make_unique<Window>();arcmod::hook.fn=[&](Contents*,Contents*){++calls;w.reset();};assert(Tail(w.get(),&t,t.weak_factory_.GetWeakPtr())==nullptr);assert(retired==2&&calls==3);}
 // Existing closing window is not associated or retired a second time.
 {Contents t;auto w=std::make_unique<Window>();w->life_stage_=kClosing;assert(Tail(w.get(),&t,t.weak_factory_.GetWeakPtr())==nullptr);assert(retired==2&&calls==3);}
}
'''
q=proof/'factory.cc';q.write_text(factory);binary=proof/'factory'
subprocess.run(['xcrun','clang++','-std=c++20','-O2','-Wall','-Wextra','-Werror',str(q),'-o',str(binary)],check=True)
subprocess.run([str(binary)],check=True,timeout=15)
print('5 extracted factory-tail weak/reentry/retirement cases passed; constructor and native ClosePage mocked')
