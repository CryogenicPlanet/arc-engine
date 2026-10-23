#!/usr/bin/env python3
"""Extract actual staged DevTools bodies into bounded native/task-runner mocks.

Reads the explicit private stage; writes fixtures/logs only into that stage.
No engine build, app run, native browser integration or golden acceptance.
"""
from pathlib import Path
import argparse,ast,subprocess,hashlib
parser=argparse.ArgumentParser();parser.add_argument('--stage',type=Path,required=True);args=parser.parse_args();stage=args.stage.resolve();root=Path.cwd();source=(stage/'arc/api/arc_webextras_devtools.mm').read_text();constants={}
frozen_test=(root/'arc/browser/webextras_devtools_logical_peer_test.py').read_text()
assert hashlib.sha256(frozen_test.encode()).hexdigest()=='66c6c510f179450784e92cd7a4c898e324cc0128a55fe30aaae7e06c855a9ccd'
for node in ast.parse(frozen_test).body:
 if isinstance(node,ast.Assign)and isinstance(node.value,ast.Constant)and isinstance(node.value.value,str):
  for target in node.targets:
   if isinstance(target,ast.Name):constants[target.id]=node.value.value
preamble=constants['preamble'].replace('class ProtocolClient{','class UnusedFixtureProtocolClient{').replace('public:explicit ProtocolClient(','public:explicit UnusedFixtureProtocolClient(')
preamble=preamble.replace('#include <memory>','#include <memory>\n#include <span>')
preamble=preamble.replace('inline constexpr int JSON_PARSE_CHROMIUM_EXTENSIONS=0;','''inline constexpr int JSON_PARSE_CHROMIUM_EXTENSIONS=0;
inline constexpr int JSON_PARSE_RFC=0;
template<class T>using span=std::span<T>;
static span<const uint8_t>as_byte_span(const std::string&s){return {reinterpret_cast<const uint8_t*>(s.data()),s.size()};}''')
preamble=preamble.replace('void PostTask(int,dispatch_block_t b){[tasks addObject:[b copy]];}','void PostTask(int,dispatch_block_t b){[tasks addObject:[b copy]];}template<class F>void PostTask(int,F f){[tasks addObject:^{f();}];}')
native=r'''
template<class T>using scoped_refptr=std::shared_ptr<T>;
namespace content {
class DevToolsAgentHost;
class DevToolsAgentHostClient{public:virtual~DevToolsAgentHostClient()=default;virtual void DispatchProtocolMessage(DevToolsAgentHost*,base::span<const uint8_t>)=0;virtual void AgentHostClosed(DevToolsAgentHost*)=0;};
class DevToolsAgentHost:public std::enable_shared_from_this<DevToolsAgentHost>{public:
 inline static std::shared_ptr<DevToolsAgentHost>latest;
 static std::shared_ptr<DevToolsAgentHost>GetOrCreateFor(WebContents*){latest=std::make_shared<DevToolsAgentHost>();return latest;}
 bool AttachClient(DevToolsAgentHostClient*c){clients.push_back(c);return true;}
 bool DetachClient(DevToolsAgentHostClient*c){++detachCount;clients.erase(std::remove(clients.begin(),clients.end(),c),clients.end());auto callback=onDetach;if(callback)callback();return true;}
 void DispatchProtocolMessage(DevToolsAgentHostClient*,base::span<const uint8_t>s){sent.emplace_back(reinterpret_cast<const char*>(s.data()),s.size());}
 void Deliver(DevToolsAgentHostClient*c,const std::string&s){c->DispatchProtocolMessage(this,base::as_byte_span(s));}
 std::vector<DevToolsAgentHostClient*>clients;std::vector<std::string>sent;unsigned detachCount=0;std::function<void()>onDetach;
};
}
constexpr size_t kMaximumProtocolBytes=8*1024*1024;
'''
helper=source[source.index('NSError* DevToolsError('):source.index('class ProtocolClient final')]
client=source[source.index('class ProtocolClient final'):source.index('using RawProtocolConnections')]
checks=r'''
int main(){@autoreleasepool{tasks=[NSMutableArray array];unsigned cases=0;
 // Outgoing envelope guards and first raw close notice remain exact and async.
 {content::WebContents wc;__block unsigned notices=0;auto c=std::make_shared<ProtocolClient>(^(NSString*m,NSError*e){Check(!m&&e.code==10&&[e.localizedDescription isEqual:@"DevTools connection closed"],"close error shape");++notices;});Check(c->Attach(&wc),"attach failed");auto host=content::DevToolsAgentHost::latest;NSError*e=nil;Check(!c->Send(@"[]",&e)&&e.code==2,"invalid envelope accepted");Check(c->Send(@"{\"id\":1,\"method\":\"Runtime.ready\"}",&e)&&host->sent.size()==1,"valid envelope failed");c->Close();Check(!notices&&tasks.count==1&&host->detachCount==1,"raw close inline or missing");c->Close();c.reset();Drain();Check(notices==1&&host->detachCount==1,"raw duplicate close");}++cases;
 // Oversized receive task retains ONLY weak actual client; final raw Close
 // immediately copies/posts its error and older task becomes inert.
 {content::WebContents wc;__block unsigned notices=0;auto c=std::make_shared<ProtocolClient>(^(NSString*,NSError*e){Check(e.code==10,"oversize close code");++notices;});c->Attach(&wc);auto host=content::DevToolsAgentHost::latest;std::weak_ptr<ProtocolClient>w=c;host->Deliver(c.get(),std::string(kMaximumProtocolBytes+1,'x'));Check(tasks.count==1&&!notices,"oversize acceptance changed");c.reset();Check(w.expired()&&host->detachCount==1&&tasks.count==2,"oversize task retained actual client/deferred close");RunOne();Check(!notices&&tasks.count==1,"expired oversize task invoked raw callback");RunOne();Check(notices==1&&!tasks.count,"copied final raw close was canceled");}++cases;
 // Explicit close before older oversized task cannot duplicate the notice.
 {content::WebContents wc;__block unsigned notices=0;auto c=std::make_shared<ProtocolClient>(^(NSString*,NSError*){++notices;});c->Attach(&wc);auto host=content::DevToolsAgentHost::latest;host->Deliver(c.get(),std::string(kMaximumProtocolBytes+1,'x'));c->Close();Drain();Check(notices==1&&host->detachCount==1,"explicit+oversize double close");c.reset();Check(!tasks.count,"closed destructor notified twice");}++cases;
 // Old copied successful raw notification and terminal raw error both survive
 // final client release, without a client/peer/service guard.
 {content::WebContents wc;NSMutableArray*events=[NSMutableArray array];auto c=std::make_shared<ProtocolClient>(^(NSString*m,NSError*e){[events addObject:e?@"error":m];});c->Attach(&wc);auto host=content::DevToolsAgentHost::latest;host->Deliver(c.get(),"raw event");c.reset();Check(!events.count&&tasks.count==2,"raw notifications inline or lost");Drain();Check([events isEqual:@[@"raw event",@"error"]],"accepted raw callback canceled at final release");}++cases;
 // Internal direct completion may finally release actual client. No hidden
 // shared_from_this lease or member access follows the app callback.
 {content::WebContents wc;std::shared_ptr<ProtocolClient>c;std::weak_ptr<ProtocolClient>w;auto*cp=&c;auto*wp=&w;c=std::make_shared<ProtocolClient>(^(NSString*m,NSError*e){if(e)return;Check(m!=nil,"internal message missing");cp->reset();Check(wp->expired(),"internal direct completion retained client");},false);w=c;c->Attach(&wc);auto host=content::DevToolsAgentHost::latest;host->Deliver(c.get(),"{}");Check(w.expired()&&host->detachCount==1,"internal final release failed");Drain();}++cases;
 // Real Close has no this access after C156 backend DetachClient reentry can
 // release its last owner. The copied raw notice still posts exactly once.
 {content::WebContents wc;__block unsigned notices=0;auto c=std::make_shared<ProtocolClient>(^(NSString*,NSError*){++notices;});c->Attach(&wc);auto host=content::DevToolsAgentHost::latest;std::weak_ptr<ProtocolClient>w=c;auto*cp=&c;host->onDetach=[cp,w]{cp->reset();Check(w.expired(),"Close held client across backend detach");};auto*raw=c.get();raw->Close();host->onDetach={};Check(w.expired()&&!notices&&tasks.count==1,"backend last release corrupted close notice");Drain();Check(notices==1&&host->detachCount==1,"backend release double close");}++cases;
 // Native-style AgentHostClosed is distinct from explicit DetachClient; one
 // copied notice survives later destruction and there is no forced reconnect.
 {content::WebContents wc;__block unsigned notices=0;auto c=std::make_shared<ProtocolClient>(^(NSString*,NSError*){++notices;});c->Attach(&wc);auto host=content::DevToolsAgentHost::latest;host->DetachClient(c.get());c->AgentHostClosed(host.get());c->AgentHostClosed(host.get());c.reset();Drain();Check(notices==1&&host->detachCount==1,"host-closed duplicate/error behavior");}++cases;
 // Client weak guard remains eligible through destructor Close, then poisons.
 // Nested loop may run an older oversize task while Close is already inert.
 {content::WebContents wc;__block unsigned notices=0;auto c=std::make_shared<ProtocolClient>(^(NSString*,NSError*){++notices;});c->Attach(&wc);auto host=content::DevToolsAgentHost::latest;host->Deliver(c.get(),std::string(kMaximumProtocolBytes+1,'x'));host->onDetach=[]{RunOne();};c.reset();host->onDetach={};Check(!notices&&tasks.count==1&&host->detachCount==1,"client guard/close poisoning order changed");Drain();Check(notices==1,"raw final notice suppressed");}++cases;
 Check(!tasks.count&&cases==8,"client test case/task count");puts("8 actual ProtocolClient raw/copy/weak-oversize/direct-completion/backend-reentry groups PASS; native host and task runner mocked");
}}
'''
fixture=preamble+native+helper+client+checks;p=stage/'trigger-client-fixture.mm';p.write_text(fixture);exe=stage/'trigger-client-fixture';cmd=['xcrun','clang++','-std=c++20','-fobjc-arc','-O1','-Wall','-Wextra','-Werror','-Wno-unused-function','-Wno-unused-parameter','-Wno-unused-private-field','-framework','Foundation',str(p),'-o',str(exe)];r=subprocess.run(cmd,text=True,capture_output=True);(stage/'trigger-client-compiler.log').write_text(r.stdout+r.stderr);print(r.returncode,(r.stdout+r.stderr)[-12000:]);assert r.returncode==0
r=subprocess.run([str(exe)],text=True,capture_output=True);(stage/'trigger-client-run.log').write_text(r.stdout+r.stderr);print('RUN',r.returncode,r.stdout,r.stderr);assert r.returncode==0
