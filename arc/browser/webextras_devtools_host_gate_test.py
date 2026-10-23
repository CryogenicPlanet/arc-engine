#!/usr/bin/env python3
"""Private initial5 host gate reconstruction and actual extracted-body regressions.
Never edits production/shared sources, locks, build graph, apps or profiles.
"""
from pathlib import Path
import difflib,hashlib,json,subprocess,tempfile
ROOT=Path(__file__).resolve().parents[2]
OUT=ROOT/'logs/wp/webextras-devtools-host-gate'
H=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
PATCHES=[('webextras_devtools_hooks.patch','dcbf472a06bd6a722cfaf591c3a0c4662b8759196d77babeb7fec4ad14bebd17'),('webextras_devtools_host_docking.patch','d9295b5cd7faea6251b4845d196323c2c8f5dfe31bd25bf26b78b5bfa8f1eac9'),('webextras_devtools_embedder_events.patch','2fae816858d06e2a78eddb7c32caa0cea0bdd27634adaa0a23dcc67e8944d24f')]
OWNED=[('webextras_devtools_integration_held.patch','a2a9d232a57adc19daaa13c182c719523c4eb2a315f93002c1a08ca1d3c18cad'),('webextras_devtools_embedder_companion_held.patch','ed9a828d55029381d8b899711f3459b52ed71369acdab881323173d7c69f6bfb')]
GATE='base::CommandLine::ForCurrentProcess()->HasSwitch("arcmod-host-app")'

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
 raise AssertionError(anchor)

def reconstruct():
 OUT.mkdir(parents=True,exist_ok=True)
 for name,pin in PATCHES+OWNED:assert H(ROOT/'arc/browser'/name)==pin,name
 paths=sorted({l[6:] for name,_ in PATCHES for l in (ROOT/'arc/browser'/name).read_text().splitlines() if l.startswith('+++ b/')})
 actual={n:ROOT/'chromium/src'/n for n in paths};actual['arc/api/arc_webextras_devtools.mm']=ROOT/'arc/api/arc_webextras_devtools.mm'
 before={n:H(p)for n,p in actual.items()}
 stage=Path(tempfile.mkdtemp(prefix='webextras-devtools-host-gate-'));base=stage/'base';initial=stage/'initial5';final=stage/'gated'
 for n,p in actual.items():
  for dest in [base,initial]:q=dest/n;q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 commands=[]
 def run_apply(dest,patch):
  for check in [True,False]:
   argv=['git','apply','--whitespace=error']+(['--check']if check else[])+[str(patch)];r=subprocess.run(argv,cwd=dest,text=True,capture_output=True);commands.append({'argv':argv,'cwd':str(dest),'exit':r.returncode,'stderr':r.stderr});assert r.returncode==0,r.stderr
 for name,_ in PATCHES+OWNED:run_apply(initial,ROOT/'arc/browser'/name)
 for n in actual:q=final/n;q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes((initial/n).read_bytes())
 def edit(n,old,new):
  p=final/n;s=p.read_text();assert s.count(old)==1,(n,old,s.count(old));p.write_text(s.replace(old,new))
 w='chrome/browser/devtools/devtools_window.cc';u='chrome/browser/devtools/devtools_ui_bindings.cc';d='chrome/browser/devtools/devtools_embedder_message_dispatcher.cc';f='third_party/devtools-frontend/src/front_end/'
 edit(w,'      auto weak_window = window->weak_factory_.GetWeakPtr();\n      window->ActivateWindow();',f'''      if (!{GATE}) {{
        window->ActivateWindow();
        return window;
      }}
      auto weak_window = window->weak_factory_.GetWeakPtr();
      window->ActivateWindow();''')
 edit(w,'  auto weak_window = window->weak_factory_.GetWeakPtr();\n  window->ScheduleShow(DevToolsToggleAction::Show());',f'''  if (!{GATE}) {{
    window->ScheduleShow(DevToolsToggleAction::Show());
    return window;
  }}
  auto weak_window = window->weak_factory_.GetWeakPtr();
  window->ScheduleShow(DevToolsToggleAction::Show());''')
 edit(w,'    auto weak_inspected = inspected_web_contents->GetWeakPtr();',f'''    const bool arc_host = {GATE};
    auto weak_inspected = arc_host ? inspected_web_contents->GetWeakPtr()
                                   : base::WeakPtr<content::WebContents>();''')
 edit(w,'      if (weak_inspected) {\n        DevToolsPolicyDialog::Show(weak_inspected.get());\n      }','''      if (!arc_host) {
        DevToolsPolicyDialog::Show(inspected_web_contents);
      } else if (weak_inspected) {
        DevToolsPolicyDialog::Show(weak_inspected.get());
      }''')
 # Preserve the baseline constructor/return path in ordinary Chrome. No newly
 # added weak guard is consulted there, including unusual constructor reentry.
 old='  auto weak_inspected = inspected_web_contents\n                            ? inspected_web_contents->GetWeakPtr()'
 baseline=method((base/w).read_text(),'DevToolsWindow* DevToolsWindow::Create(')
 original_return=baseline[baseline.index('  return new DevToolsWindow('):baseline.rfind('\n}')]
 edit(w,old,f'''  if (!{GATE}) {{
'''+''.join('  '+l+'\n'for l in original_return.splitlines())+'''  }
  auto weak_inspected = inspected_web_contents
                            ? inspected_web_contents->GetWeakPtr()''')
 edit(w,'  if (auto hook = arcmod::GetDevToolsFocusHook(); !hook.is_null()) {',f'''  if (auto hook = arcmod::GetDevToolsFocusHook();
      {GATE} && !hook.is_null()) {{''')
 # This callback is already inside the host-only factory tail; retain explicit
 # switch check to make its authority visible if the tail moves in a later rebase.
 edit(w,'  if (auto hook = arcmod::GetDevToolsFrontendCreatedHook(); !hook.is_null()) {',f'''  if (auto hook = arcmod::GetDevToolsFrontendCreatedHook();
      {GATE} && !hook.is_null()) {{''')
 edit(w,'  auto weak_this = weak_factory_.GetWeakPtr();\n  Show(action_on_load_);\n  if (!weak_this || weak_this->life_stage_ == kClosing) {\n    return;\n  }',f'''  if ({GATE}) {{
    auto weak_this = weak_factory_.GetWeakPtr();
    Show(action_on_load_);
    if (!weak_this || weak_this->life_stage_ == kClosing) {{
      return;
    }}
  }} else {{
    Show(action_on_load_);
  }}''')
 edit(w,'  return DevToolsUIBindings::SanitizeFrontendURL(GURL(url));',f'''  if ({GATE}) {{
    url += "&arcmodHost=true";
  }}
  return DevToolsUIBindings::SanitizeFrontendURL(GURL(url));''')
 edit(u,'  // Convert boolean flags to true.',f'''  // Only hosted Arc frontends may advertise the additional notifications.
  if (key == "arcmodHost") {{
    return {GATE} && value == "true"
               ? "true"
               : std::string();
  }}

  // Convert boolean flags to true.''')
 edit(u,'  auto weak_this = weak_factory_.GetWeakPtr();\n  FrontendLoaded();\n  if (!weak_this) {\n    return;\n  }',f'''  if ({GATE}) {{
    auto weak_this = weak_factory_.GetWeakPtr();
    FrontendLoaded();
    if (!weak_this) {{
      return;
    }}
  }} else {{
    FrontendLoaded();
  }}''')
 edit(u,'  auto weak_this = weak_factory_.GetWeakPtr();\n  delegate_->OnLoadCompleted();\n  if (!weak_this) {\n    return;\n  }',f'''  if ({GATE}) {{
    auto weak_this = weak_factory_.GetWeakPtr();
    delegate_->OnLoadCompleted();
    if (!weak_this) {{
      return;
    }}
  }} else {{
    delegate_->OnLoadCompleted();
  }}''')
 for name,hook in [('InspectModeChanged','InspectMode'),('RequestDockPosition','DockPosition')]:
  edit(u,f'  if (auto hook = arcmod::GetDevTools{hook}Hook(); !hook.is_null()) {{',f'''  if (auto hook = arcmod::GetDevTools{hook}Hook();
      {GATE} && !hook.is_null()) {{''')
 edit(d,'#include "base/functional/bind.h"','#include "base/command_line.h"\n#include "base/functional/bind.h"')
 old='''  d->RegisterHandler("inspectModeChanged", &Delegate::InspectModeChanged,
                     delegate);
  d->RegisterHandler("requestDockPosition", &Delegate::RequestDockPosition,
                     delegate);'''
 edit(d,old,f'''  if ({GATE}) {{
    d->RegisterHandler("inspectModeChanged", &Delegate::InspectModeChanged,
                       delegate);
    d->RegisterHandler("requestDockPosition", &Delegate::RequestDockPosition,
                       delegate);
  }}''')
 for n,call in [(f+'panels/elements/InspectElementModeController.ts','inspectModeChanged(mode)'),(f+'ui/legacy/DockController.ts','requestDockPosition(dockSide)')]:
  edit(n,'    Host.InspectorFrontendHost.InspectorFrontendHostInstance.'+call+';',"    if (Root.Runtime.Runtime.queryParam('arcmodHost') === 'true') {\n      Host.InspectorFrontendHost.InspectorFrontendHostInstance."+call+';\n    }')
 edit(f+'ui/legacy/DockController.ts',"import * as i18n from '../../core/i18n/i18n.js';","import * as i18n from '../../core/i18n/i18n.js';\nimport * as Root from '../../core/root/root.js';")
 edit(f+'ui/legacy/DockController.ts','      Host.InspectorFrontendHost.InspectorFrontendHostInstance.requestDockPosition(DockState.UNDOCKED);',"      if (Root.Runtime.Runtime.queryParam('arcmodHost') === 'true') {\n        Host.InspectorFrontendHost.InspectorFrontendHostInstance.requestDockPosition(DockState.UNDOCKED);\n      }")
 for call in ["DevToolsAPI.sendMessageToEmbedder('inspectModeChanged', [mode], null);","DevToolsAPI.sendMessageToEmbedder('requestDockPosition', [side]);"]:
  edit(f+'devtools_compatibility.js','      '+call,"      if (new URLSearchParams(window.location.search).get('arcmodHost') === 'true') {\n        "+call+'\n      }')
 # Bound the added long conditions to Chromium's usual line width without
 # reformatting any unrelated source context.
 for n in [u,w]:
  p=final/n;s=p.read_text()
  s=s.replace('return '+GATE+' && value == "true"', 'return '+GATE+' &&\n                   value == "true"')
  s=s.replace(GATE+' && !hook.is_null()',GATE+' &&\n          !hook.is_null()')
  s=s.replace('    const bool arc_host = '+GATE+';', '    const bool arc_host =\n        '+GATE+';')
  s=s.replace('    auto weak_inspected = arc_host ? inspected_web_contents->GetWeakPtr()\n                                   : base::WeakPtr<content::WebContents>();','    auto weak_inspected = arc_host\n                              ? inspected_web_contents->GetWeakPtr()\n                              : base::WeakPtr<content::WebContents>();')
  p.write_text(s)
 patch=''.join('diff --git a/'+n+' b/'+n+'\n'+''.join(difflib.unified_diff((initial/n).read_text().splitlines(True),(final/n).read_text().splitlines(True),fromfile='a/'+n,tofile='b/'+n))for n in actual if(initial/n).read_bytes()!=(final/n).read_bytes())
 candidate=ROOT/'arc/browser/webextras_devtools_host_gate_held.patch'
 if candidate.exists():assert candidate.read_text()==patch,'published candidate differs'
 else:candidate.write_text(patch)
 # Strict fresh replay proves the published delta, not only the mutation script.
 replay=stage/'replay'
 for n in actual:q=replay/n;q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes((initial/n).read_bytes())
 run_apply(replay,candidate)
 assert all((replay/n).read_bytes()==(final/n).read_bytes()for n in actual)
 assert {n:H(p)for n,p in actual.items()}==before,'actual source changed during private stage'
 receipt={'scope':'PRIVATE after-initial5 gate; no production/shared/lock/runtime mutation','stage':str(stage),'prerequisites':{str(ROOT/'arc/browser'/n):p for n,p in PATCHES+OWNED},'candidate':{'path':str(candidate),'sha256':H(candidate)},'base':before,'initial5':{n:H(initial/n)for n in actual},'gated':{n:H(final/n)for n in actual},'commands':commands}
 (OUT/'reconstruction.json').write_text(json.dumps(receipt,indent=2)+'\n')
 return stage


def producer_regressions(stage):
 front='third_party/devtools-frontend/src/front_end/'
 anchors={
 'inspect':('panels/elements/InspectElementModeController.ts',['private setMode(','modelAdded(','private isInInspectElementMode(','private suspendStateChanged(','private showDetailedInspectTooltipChanged(']),
 'dock':('ui/legacy/DockController.ts',['initialize():','private dockSideChanged():','setDockSide(dockSide:','private setIsDockedResponse('])}
 payload={'ts':str(ROOT/'chromium/src/third_party/devtools-frontend/src/node_modules/typescript/lib/typescript.js')}
 for variant in ['base','initial5','gated']:
  payload[variant]={kind:'\n'.join(method((stage/variant/front/name).read_text(),a)for a in methods)for kind,(name,methods)in anchors.items()}
  if variant!='base':payload[variant]['compat']='\n'.join(method((stage/variant/front/'devtools_compatibility.js').read_text(),a)for a in ['inspectModeChanged(mode)','requestDockPosition(side)'])
 fixture=stage/'producer.json';fixture.write_text(json.dumps(payload))
 js=r'''
const fs=require('fs'),vm=require('vm'),assert=require('assert/strict');
const p=JSON.parse(fs.readFileSync(process.argv[2])),ts=require(p.ts);
let log=[],suspended=false,models=[],query=null;
const mode={None:'none',SearchForNode:'searchForNode',SearchForUAShadowDOM:'searchForUAShadowDOM',CaptureAreaScreenshot:'captureAreaScreenshot'};
const manager={allTargetsSuspended:()=>suspended,models:()=>models};
const host={inspectModeChanged:v=>log.push(['inspect',v]),requestDockPosition:v=>log.push(['dock',v]),setIsDocked:(v,cb)=>{log.push(['setIsDocked',v]);host.ack=cb;}};
const SDK={TargetManager:{TargetManager:{instance:()=>manager}},OverlayModel:{OverlayModel:{}}};
const Host={InspectorFrontendHost:{InspectorFrontendHostInstance:host}},Root={Runtime:{Runtime:{queryParam:name=>{assert.equal(name,'arcmodHost');return query;}}}};
const Protocol={Overlay:{InspectMode:mode}},states=['right','bottom','left','undocked'];
const DockState={RIGHT:'right',BOTTOM:'bottom',LEFT:'left',UNDOCKED:'undocked'};
const Events={BEFORE_DOCK_SIDE_CHANGED:'BeforeDockSideChanged',DOCK_SIDE_CHANGED:'DockSideChanged',AFTER_DOCK_SIDE_CHANGED:'AfterDockSideChanged'};
const document={body:{classList:{remove:v=>log.push(['css-remove',v]),add:v=>log.push(['css-add',v])}}};
const consoleMock={timeStamp:v=>log.push(['timestamp',v])};
function compile(code){const r=ts.transpileModule(code,{compilerOptions:{target:ts.ScriptTarget.ES2022,module:ts.ModuleKind.CommonJS},reportDiagnostics:true});assert.equal(r.diagnostics.length,0);return r.outputText;}
function cls(name,fields,methods){const ctx={SDK,Host,Root,Protocol,states,DockState,Events,document,console:consoleMock,record:v=>log.push(JSON.parse(JSON.stringify(v)))};vm.createContext(ctx);return vm.runInContext(compile(`class ${name}{${fields}\n${methods}}\n${name}`),ctx);}
const ifields=`mode='none';toggleSearchAction={setToggled:v=>record(['toggle',v])};showDetailedInspectTooltipSetting={get:()=>true};`;
const dfields=`#canDock;#dockSide;constructor(can){this.#canDock=can;this.#dockSide=can?undefined:'undocked';}closeButton={setVisible:v=>record(['visible',v])};currentDockStateSetting={get:()=>this.setting,set:v=>{this.setting=v;record(['current',v]);}};lastDockStateSetting={set:v=>record(['last',v])};setting='right';dispatchEventToListeners(n,v){record([n,v]);}announceDockLocation(){record(['announce']);}`;
function model(n){return {setInspectMode:(v,t)=>{log.push(['model',n,v,t]);return new Promise(()=>{});}};}
function inspect(C){const c=new C;log=[];suspended=false;models=[model(1),model(2)];c.setMode('searchForNode');c.showDetailedInspectTooltipChanged();models=[];c.setMode('none');c.setMode('none');suspended=true;c.setMode('captureAreaScreenshot');c.suspendStateChanged();suspended=false;c.setMode('searchForUAShadowDOM');c.modelAdded(model(3));return JSON.parse(JSON.stringify(log));}
function dock(C){log=[];new C(false).initialize();let c=new C(true);c.initialize();host.ack();c.setDockSide('right');c.setDockSide('bottom');host.ack();c.setDockSide('invalid');host.ack();return JSON.parse(JSON.stringify(log));}
const classes={};for(const v of ['base','initial5','gated'])classes[v]={inspect:cls(v+'I',ifields,p[v].inspect),dock:cls(v+'D',dfields,p[v].dock)};
for(const q of [null,'false','','1','TRUE','true']){query=q;for(const [kind,suite]of [['inspect',inspect],['dock',dock]]){const got=suite(classes.gated[kind]);const expected=suite(classes[q==='true'?'initial5':'base'][kind]);assert.deepEqual(got,expected,kind+' query='+q);}}
// Inspect ordering remains models -> toolbar toggle -> one notification; repeated
// mode/tooltips still notify, suspension/modelAdded remain quiet in hosted mode.
query='true';const it=inspect(classes.gated.inspect);assert.deepEqual(it.slice(0,4).map(x=>x[0]),['model','model','toggle','inspect']);assert.equal(it.filter(x=>x[0]==='inspect').length,5);
const dt=dock(classes.gated.dock);assert.deepEqual(dt[0],['dock','undocked']);assert.equal(dt.filter(x=>x[0]==='dock').length,4);
for(const q of ['', '?arcmodHost=false','?arcmodHost=TRUE','?arcmodHost=true','?arcmodHost=true&arcmodHost=false']){
 const sent=[],ctx={URLSearchParams,window:{location:{search:q}},DevToolsAPI:{sendMessageToEmbedder:(...a)=>sent.push(a)}};vm.createContext(ctx);const c=vm.runInContext(`new(class {${p.gated.compat}})`,ctx);c.inspectModeChanged('none');c.requestDockPosition('left');assert.deepEqual(JSON.parse(JSON.stringify(sent)),q.startsWith('?arcmodHost=true')?[['inspectModeChanged',['none'],null],['requestDockPosition',['left']]]:[]);
}
console.log('12 actual producer host/ordinary comparisons + 5 compatibility gates passed');
'''
 (stage/'producer.cjs').write_text(js)
 subprocess.run(['node',str(stage/'producer.cjs'),str(fixture)],check=True)

def native_regressions(stage):
 w='chrome/browser/devtools/devtools_window.cc';u='chrome/browser/devtools/devtools_ui_bindings.cc'
 common=r'''
#include <cassert>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#define BUILDFLAG(x) 0
static bool hosted=false;
namespace base {struct CommandLine {static CommandLine* ForCurrentProcess(){static CommandLine c;return &c;}bool HasSwitch(const char* key){assert(std::string(key)=="arcmod-host-app");return hosted;}};}
static std::vector<std::string> events;
template<class T>struct Weak {std::weak_ptr<T*> p;T* get()const{auto s=p.lock();return s?*s:nullptr;}T* operator->()const{assert(get());return get();}explicit operator bool()const{return get()!=nullptr;}};
template<class T>struct Factory{std::shared_ptr<T*>p;int queries=0;explicit Factory(T*t):p(std::make_shared<T*>(t)){}Weak<T>GetWeakPtr(){++queries;return{p};}void Invalidate(){*p=nullptr;}~Factory(){Invalidate();}};
'''
 preamble=r'''
struct Closure {std::function<void()>fn;bool is_null()const{return !fn;}void Run(){auto f=std::move(fn);f();}};
struct DevToolsToggleAction {int value;static DevToolsToggleAction NoOp(){return {0};}static DevToolsToggleAction Show(){return {1};}};
struct Profile{};enum class DevToolsOpenedByAction{Any};
constexpr int kFrontendNode=7,kClosing=3;
struct DevToolsAgentHost {static int CreateForDiscovery(){return 1;}};
struct Binding{void AttachTo(int){events.push_back("attach");}};
struct DevToolsWindow{
 Factory<DevToolsWindow>weak_factory_{this};int frontend_type_=kFrontendNode,life_stage_=0;
 DevToolsToggleAction action_on_load_{1};Closure load_completed_callback_;
 std::function<void()>during;Binding b;Binding*bindings_=&b;
 void ActivateWindow(){events.push_back("activate");if(during)during();}
 void ScheduleShow(DevToolsToggleAction){events.push_back("schedule");if(during)during();}
 void Show(DevToolsToggleAction){events.push_back("show");if(during)during();}
 void LoadCompleted();
 static DevToolsWindow*Create(Profile*,void*,int,std::string,bool,std::string,std::string,bool,bool,DevToolsOpenedByAction);
 static DevToolsWindow*OpenNodeFrontendWindow(Profile*,DevToolsOpenedByAction);
};
static DevToolsWindow*created=nullptr;static std::vector<DevToolsWindow*>instances;
auto&GetDevToolsWindowInstances(){return instances;}
DevToolsWindow*DevToolsWindow::Create(Profile*,void*,int,std::string,bool,std::string,std::string,bool,bool,DevToolsOpenedByAction){events.push_back("create");return created;}
struct Delegate{std::function<void()>fn;void OnLoadCompleted(){events.push_back("delegate");if(fn)fn();}};
struct DevToolsUIBindings{
 Factory<DevToolsUIBindings>weak_factory_{this};bool frontend_loaded_=false;
 Delegate d;Delegate*delegate_=&d;std::string initial_target_id_="";
 void FrontendLoaded();void LoadCompleted();
 void CallClientMethod(const char*,const char*,std::string){events.push_back("target");}
 void AddDevToolsExtensionsToClient(){events.push_back("extensions");}
};
'''
 code=common
 for variant in ['base','gated']:
  b=(stage/variant/u).read_text();win=(stage/variant/w).read_text()
  methods='\n'.join([method(b,'void DevToolsUIBindings::LoadCompleted()'),method(b,'void DevToolsUIBindings::FrontendLoaded()'),method(win,'void DevToolsWindow::LoadCompleted()'),method(win,'DevToolsWindow* DevToolsWindow::OpenNodeFrontendWindow(')])
  # The single active non-Android initial_target_id_ branch needs only a value
  # copy. This mock mirrors that value constructor, not any callback body.
  methods=methods.replace('base::Value(initial_target_id_)','initial_target_id_')
  code+='namespace '+variant+' {\n'+preamble+methods+r'''
std::vector<std::string> Run(bool invalidate){
 events.clear();
 {DevToolsUIBindings b;b.initial_target_id_="x";b.d.fn=[&]{if(invalidate)b.weak_factory_.Invalidate();};b.LoadCompleted();events.push_back("binding-weak="+std::to_string(b.weak_factory_.queries));}
 {DevToolsWindow w;w.load_completed_callback_.fn=[] {events.push_back("loaded-callback");};w.during=[&]{if(invalidate){w.life_stage_=kClosing;w.weak_factory_.Invalidate();}};w.LoadCompleted();events.push_back("action="+std::to_string(w.action_on_load_.value));events.push_back("window-weak="+std::to_string(w.weak_factory_.queries));}
 {DevToolsWindow w;instances={&w};w.during=[&]{if(invalidate){w.life_stage_=kClosing;w.weak_factory_.Invalidate();}};auto*p=DevToolsWindow::OpenNodeFrontendWindow(nullptr,DevToolsOpenedByAction::Any);events.push_back(p?"node-live":"node-null");events.push_back("node-weak="+std::to_string(w.weak_factory_.queries));instances.clear();}
 {DevToolsWindow w;created=&w;w.during=[&]{if(invalidate){w.life_stage_=kClosing;w.weak_factory_.Invalidate();}};auto*p=DevToolsWindow::OpenNodeFrontendWindow(nullptr,DevToolsOpenedByAction::Any);events.push_back(p?"new-live":"new-null");events.push_back("new-weak="+std::to_string(w.weak_factory_.queries));created=nullptr;}
 return events;
}
}
'''
 # Capability prefix is extracted verbatim; the remainder intentionally delegates
 # to an unchanged-baseline sentinel, rather than reimplementing its sanitizer.
 sanitizer=method((stage/'gated'/u).read_text(),'std::string SanitizeFrontendQueryParam(')
 prefix=sanitizer[sanitizer.index('{')+1:sanitizer.index('  // Convert boolean flags to true.')]
 url=method((stage/'gated'/w).read_text(),'GURL DevToolsWindow::GetDevToolsURL(')
 urlbranch=url[url.index('  if (base::CommandLine::ForCurrentProcess()->HasSwitch("arcmod-host-app"))'):url.rfind('  return DevToolsUIBindings::SanitizeFrontendURL(GURL(url));')]
 code+='std::string Capability(const std::string&key,const std::string&value){'+prefix+'return "baseline";}\n'
 code+='std::string URL(std::string url){'+urlbranch+'return url;}\n'
 code+=r'''
int main(){
 // Actual methods match ordinary baseline even when a weak anchor has expired;
 // objects remain allocated here to observe baseline behavior without UAF.
 hosted=false;for(bool invalidate:{false,true})assert(base::Run(invalidate)==gated::Run(invalidate));
 hosted=true;auto alive=gated::Run(false),dead=gated::Run(true);
 auto has=[](auto&v,const char*s){return std::find(v.begin(),v.end(),s)!=v.end();};
 assert(has(alive,"extensions")&&has(alive,"loaded-callback")&&has(alive,"node-live")&&has(alive,"new-live"));
 assert(!has(dead,"extensions")&&!has(dead,"target")&&!has(dead,"loaded-callback")&&has(dead,"action=1")&&has(dead,"node-null")&&has(dead,"new-null"));
 for(bool host:{false,true}){hosted=host;for(auto v:{"true","false","TRUE","1",""})assert(Capability("arcmodHost",v)==(host&&std::string(v)=="true"?"true":""));assert(Capability("can_dock","true")=="baseline");assert(URL("devtools://devtools/bundled/devtools_app.html?remoteBase=x")==std::string("devtools://devtools/bundled/devtools_app.html?remoteBase=x")+(host?"&arcmodHost=true":""));
 // Sanitizer visits each duplicate query entry independently (actual enclosing
 // SanitizeFrontendURL loop); no combination can advertise host in Chrome.
 for(auto first:{"true","false","TRUE",""})for(auto second:{"true","false","TRUE",""}){bool accepted=!Capability("arcmodHost",first).empty()||!Capability("arcmodHost",second).empty();assert(!accepted||host);}}
}
'''
 # std::find used only in fixture assertions.
 code=code.replace('#include <cassert>','#include <algorithm>\n#include <cassert>')
 fixture=stage/'native.cc';fixture.write_text(code)
 subprocess.run(['xcrun','clang++','-std=c++20','-O2','-Wall','-Wextra','-Werror',str(fixture),'-o',str(stage/'native')],check=True)
 subprocess.run([str(stage/'native')],check=True,timeout=15)
 # Verbatim ordinary constructor/return path plus exact normal policy call.
 before=method((stage/'base'/w).read_text(),'DevToolsWindow* DevToolsWindow::Create(');after=method((stage/'gated'/w).read_text(),'DevToolsWindow* DevToolsWindow::Create(')
 ordinary=after[after.index('  if (!'+GATE+') {'):after.index('  auto weak_inspected = inspected_web_contents')]
 assert ''.join(l[2:]+'\n'for l in ordinary.splitlines()[1:-1]).strip()==before[before.index('  return new DevToolsWindow('):before.rfind('\n}')].strip()
 print('actual native ordinary/host method comparisons, capability values/duplicates and baseline constructor branch passed')

def factory_and_dispatch_regressions(stage):
 import ast
 old=ast.parse((ROOT/'arc/browser/webextras_devtools_embedder_regression.py').read_text())
 assignment=next(n for n in old.body if isinstance(n,ast.Assign)and any(isinstance(t,ast.Name)and t.id=='factory'for t in n.targets))
 create=method((stage/'gated/chrome/browser/devtools/devtools_window.cc').read_text(),'DevToolsWindow* DevToolsWindow::Create(')
 tail=create[create.index('  auto weak_window = window->weak_factory_.GetWeakPtr();'):create.rfind('\n}')]
 fixture=eval(compile(ast.Expression(assignment.value),'<existing-factory-fixture>','eval'),{'tail':tail})
 fixture=fixture.replace('#include <cassert>','#include <cassert>\nnamespace base{struct CommandLine{static CommandLine*ForCurrentProcess(){static CommandLine c;return &c;}bool HasSwitch(const char*){return true;}};}')
 path=stage/'factory.cc';path.write_text(fixture)
 subprocess.run(['xcrun','clang++','-std=c++20','-O2','-Wall','-Wextra','-Werror',str(path),'-o',str(stage/'factory')],check=True)
 subprocess.run([str(stage/'factory')],check=True,timeout=15)
 u=(stage/'gated/chrome/browser/devtools/devtools_ui_bindings.cc').read_text()
 d=(stage/'gated/chrome/browser/devtools/devtools_embedder_message_dispatcher.cc').read_text()
 start=d.index('  if ('+GATE+') {',d.index('d->RegisterHandler("inspectedURLChanged"'))
 registration=method(d[start:],'  if (')
 code=r'''
#include <cassert>
#include <functional>
#include <memory>
#include <string>
#include <vector>
static bool hosted=false;namespace base{struct CommandLine{static CommandLine*ForCurrentProcess(){static CommandLine c;return &c;}bool HasSwitch(const char*){return hosted;}};}
struct Contents{};
namespace arcmod{struct Hook{std::function<void(Contents*,Contents*,const std::string&)>fn;bool is_null()const{return !fn;}void Run(Contents*a,Contents*b,const std::string&s){fn(a,b,s);}};static Hook inspect,dock;Hook&GetDevToolsInspectModeHook(){return inspect;}Hook&GetDevToolsDockPositionHook(){return dock;}}
struct Delegate{Contents*GetInspectedWebContents(){return nullptr;}void InspectModeChanged(){}void RequestDockPosition(){}};
struct DevToolsUIBindings{Delegate owned;Delegate*delegate_=&owned;Contents*web_contents_=nullptr;void InspectModeChanged(const std::string&mode);void RequestDockPosition(const std::string&side);};
'''+method(u,'void DevToolsUIBindings::InspectModeChanged(')+'\n'+method(u,'void DevToolsUIBindings::RequestDockPosition(')+r'''
struct Dispatcher{std::vector<std::string>names;void RegisterHandler(const char*n,void(Delegate::*)(),Delegate*){names.emplace_back(n);}};
void Register(Dispatcher*d,Delegate*delegate){
'''+registration+r'''
}
int main(){
 for(bool host:{false,true}){hosted=host;Dispatcher d;Delegate target;Register(&d,&target);assert(d.names.size()==(host?2:0));}
 unsigned calls=0;
 for(bool host:{false,true}){hosted=host;auto b=std::make_unique<DevToolsUIBindings>();arcmod::inspect.fn=[&](Contents*,Contents*,const std::string&s){assert(s=="searchForNode");++calls;arcmod::inspect.fn=nullptr;b.reset();};b->InspectModeChanged("searchForNode");assert(bool(b)==!host);}
 assert(calls==1);
 for(bool host:{false,true}){hosted=host;auto b=std::make_unique<DevToolsUIBindings>();arcmod::dock.fn=[&](Contents*,Contents*,const std::string&s){assert(s=="left");++calls;arcmod::dock.fn=nullptr;b.reset();};b->RequestDockPosition("left");assert(bool(b)==!host);}
 assert(calls==2);
}
'''
 path=stage/'dispatch.cc';path.write_text(code)
 subprocess.run(['xcrun','clang++','-std=c++20','-O2','-Wall','-Wextra','-Werror',str(path),'-o',str(stage/'dispatch')],check=True)
 subprocess.run([str(stage/'dispatch')],check=True,timeout=15)
 print('5 actual hosted factory-tail cases + native handler registration/copy/self-destruction gates passed')

def syntax_checks(stage):
 import shlex
 src=ROOT/'chromium/src';build=src/'out/Base';proof=OUT/'syntax';proof.mkdir(exist_ok=True)
 receipt=json.loads((OUT/'reconstruction.json').read_text());roots=[]
 for name in receipt['gated']:
  base=ROOT if name.startswith('arc/')else src
  roots.append({'type':'file','name':str(base/name),'external-contents':str(stage/'gated'/name)})
  if name.startswith('arc/'):roots.append({'type':'file','name':str(src/name),'external-contents':str(stage/'gated'/name)})
 # Capture current shared header and add only the authorized declaration privately;
 # no equality requirement against the obsolete historical header overlay.
 header=ROOT/'arc/api/arc_internal.h';data=header.read_text();header_pin=H(header)
 if 'bool ArcRuntimeFeatureEnabled(NSUInteger identifier);'not in data:
  data+='\n// webextras private integration prerequisite; shared application is parent-owned.\nbool ArcRuntimeFeatureEnabled(NSUInteger identifier);\n'
 private=stage/'arc_internal.h';private.write_text(data)
 for base in [ROOT,src]:roots.append({'type':'file','name':str(base/'arc/api/arc_internal.h'),'external-contents':str(private)})
 overlay=stage/'overlay.json';overlay.write_text(json.dumps({'version':0,'use-external-names':False,'roots':roots},indent=2))
 (proof/'overlay.json').write_text(overlay.read_text());checks=[]
 for name,path,target in [(n,'../../chrome/browser/devtools/'+n+'.cc','obj/chrome/browser/devtools/devtools/'+n+'.o')for n in ['devtools_window','devtools_ui_bindings','devtools_embedder_message_dispatcher']]+[('companion','../../arc/api/arc_webextras_devtools.mm','obj/arc/arc_core/arc_webextras_devtools.o')]:
  r=subprocess.run([str(src/'third_party/siso/cipd/siso'),'query','commands','-C','.',target],cwd=build,text=True,errors='replace',capture_output=True,check=True)
  (proof/(name+'.query.json')).write_text(json.dumps({'stdoutBytes':len(r.stdout.encode()),'stderrBytes':len(r.stderr.encode()),'stdoutSha256':hashlib.sha256(r.stdout.encode()).hexdigest(),'exit':r.returncode}))
  lines=[l for l in r.stdout.splitlines()if' -c '+path+' 'in l];assert len(lines)==1,(name,len(lines))
  args=shlex.split(lines[0]);(proof/(name+'.original-command.json')).write_text(json.dumps(args,indent=2));filtered=[];i=0
  while i<len(args):
   if args[i]in['-o','-MF']:i+=2;continue
   if args[i]in['-c','-MD','-MMD']:i+=1;continue
   filtered.append(args[i]);i+=1
  filtered+=['-fsyntax-only','-Wno-unused-command-line-argument','-ivfsoverlay',str(overlay)]
  (proof/(name+'.syntax-command.json')).write_text(json.dumps(filtered,indent=2))
  r=subprocess.run(filtered,cwd=build,text=True,errors='replace',capture_output=True)
  (proof/(name+'.syntax.log')).write_text(r.stdout+r.stderr)
  checks.append({'name':name,'exit':r.returncode,'diagnostic_bytes':len(r.stdout+r.stderr)});print(checks[-1],flush=True)
 assert H(header)==header_pin,'shared header changed during private syntax; rerun capture required'
 (proof/'inputs.json').write_text(json.dumps({'actualHeaderBeforeAfter':header_pin,'privateHeader':H(private),'overlay':H(overlay),'stage':str(stage)},indent=2))
 (proof/'checks.json').write_text(json.dumps(checks,indent=2))
 assert all(c['exit']==0 and c['diagnostic_bytes']==0 for c in checks)

def negative_controls(stage):
 import shutil,sys
 cases=[('inspect-ungated','third_party/devtools-frontend/src/front_end/panels/elements/InspectElementModeController.ts',"if (Root.Runtime.Runtime.queryParam('arcmodHost') === 'true')",'if (true)','producer_regressions','inspect query=null'),('compat-ungated','third_party/devtools-frontend/src/front_end/devtools_compatibility.js',"if (new URLSearchParams(window.location.search).get('arcmodHost') === 'true')",'if (true)','producer_regressions','AssertionError'),('node-global-guard','chrome/browser/devtools/devtools_window.cc','if (!'+GATE+')','if (false)','native_regressions','base::Run(invalidate)==gated::Run(invalidate)')]
 results=[]
 for name,path,old,new,function,expected in cases:
  dest=stage/('negative-'+name)
  for v in ['base','initial5','gated']:shutil.copytree(stage/v,dest/v)
  p=dest/'gated'/path;s=p.read_text();assert old in s;p.write_text(s.replace(old,new,1))
  code="import importlib.util;from pathlib import Path;s=importlib.util.spec_from_file_location('gate',"+repr(str(Path(__file__).resolve()))+");m=importlib.util.module_from_spec(s);s.loader.exec_module(m);m."+function+"(Path("+repr(str(dest))+"))"
  r=subprocess.run([sys.executable,'-c',code],text=True,capture_output=True)
  assert r.returncode!=0 and expected in r.stderr,(name,r.returncode,r.stdout,r.stderr)
  results.append({'name':name,'exit':r.returncode,'specificExpectedFailure':expected,'stdout':r.stdout,'stderr':r.stderr})
 (OUT/'negative-controls.json').write_text(json.dumps(results,indent=2)+'\n')
 print('3 specific extracted-body negative controls failed as expected')

if __name__=='__main__':
 import argparse
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--proof-dir',type=Path)
 args=parser.parse_args()
 if args.proof_dir:OUT=args.proof_dir.resolve()
 assert not (OUT/'freeze.json').exists(),'frozen evidence: select a fresh --proof-dir'
 stage=reconstruct()
 producer_regressions(stage)
 native_regressions(stage)
 factory_and_dispatch_regressions(stage)
 negative_controls(stage)
 syntax_checks(stage)
 assert all(H(ROOT/n if n.startswith('arc/')else ROOT/'chromium/src'/n)==pin for n,pin in json.loads((OUT/'reconstruction.json').read_text())['base'].items()),'actual inputs changed during private tests'
 print('PRIVATE gate candidate tests passed:',stage)
