"""Execute extracted actual driver methods with deterministic timer/client inputs.
No Chromium build, application, profile, lock or production write.
"""
from pathlib import Path
import json,subprocess,hashlib
R=Path('/Users/cryogenicplanet/general/arc-engine');T=Path('/tmp/webextras-thumbnail-host-preflight');O=R/'logs/wp/webextras-thumbnail-host-preflight';D=Path('chrome/browser/ui/thumbnails');F=O/'snapshot';T.mkdir(exist_ok=True)
def body(text,start):
 i=text.index(start);a=text.index('{',i);n=1;z=a+1
 while n:
  n+=(text[z]=='{')-(text[z]=='}');z+=1
 return text[i:z]
header=(F/'base'/D/'thumbnail_capture_driver.h').read_text();enum=body(header,'  enum class CaptureState')+';'
methods=['UpdatePageReadiness','UpdatePageVisibility','UpdateThumbnailVisibility','SetCanCapture','GotFrame','SetCapturePermittedByScheduler','UpdateCaptureState','UpdateSchedulingPriority','StartCooldown','OnCooldownEnded']
preamble=r'''
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <cstdlib>
inline int checks_failed=0;
struct Check {template<class T> Check& operator<<(T){return *this;}};
#define DCHECK_EQ(a,b) ((a)==(b)?Check():(++checks_failed,Check()))
#define FROM_HERE 0
namespace base {
inline bool host=false;
struct CommandLine {static CommandLine* ForCurrentProcess(){static CommandLine c;return &c;}bool HasSwitch(const char*){return host;}};
using TimeDelta=int;
constexpr TimeDelta Milliseconds(int n){return n;}
template<class T>struct WeakPtrFactory {T* p;T* GetWeakPtr(){return p;}};
template<class T>std::function<void()> BindRepeating(void(T::*fn)(),T* p){return [=]{(p->*fn)();};}
struct RetainingOneShotTimer {
 int now=0,due=0,delay=0,stops=0,starts=0,resets=0;bool running=false;std::function<void()> fn;
 bool IsRunning(){return running;}void Stop(){++stops;running=false;}
 void Reset(){++resets;running=true;due=now+delay;}
 void Start(int,int d,std::function<void()> f){++starts;delay=d;fn=f;running=true;due=now+d;}
 void Advance(int d){const int end=now+d;int bound=30;while(running&&due<=end){if(--bound==0)std::abort();now=due;running=false;fn();}now=end;}
};
}
struct ThumbnailReadinessTracker{enum class Readiness{kNotReady,kReadyForInitialCapture,kReadyForFinalCapture};};
struct ThumbnailScheduler {
 enum class TabCapturePriority{kNone,kLow,kHigh};TabCapturePriority priority=TabCapturePriority::kNone;
 void SetTabCapturePriority(void*,TabCapturePriority p){priority=p;}
};
struct Client {std::string calls;void RequestCapture(){calls+='R';}void StartCapture(){calls+='S';}void StopCapture(){calls+='X';}};
'''
def driver(ns,source):
 definitions='\n'.join(body(source,'void ThumbnailCaptureDriver::'+m+'(') for m in methods)
 return '\nnamespace '+ns+' {\nclass ThumbnailCaptureDriver { public:\n'+enum+r'''
 using PageReadiness=ThumbnailReadinessTracker::Readiness;
 static constexpr int kCooldownDelay=500;static constexpr size_t kMaxCooldownRetries=3;
 Client client;ThumbnailScheduler scheduler;Client* client_=&client;ThumbnailScheduler* scheduler_=&scheduler;
 PageReadiness page_readiness_=PageReadiness::kNotReady;bool page_visible_=false,thumbnail_visible_=false,can_capture_=false,scheduled_=false;
 CaptureState capture_state_=CaptureState::kNoCapture;bool captured_cooldown_frame_=false;size_t cooldown_retry_count_=0;
 base::RetainingOneShotTimer cooldown_timer_;base::WeakPtrFactory<ThumbnailCaptureDriver> weak_ptr_factory_{this};
 void UpdatePageReadiness(PageReadiness);void UpdatePageVisibility(bool);void UpdateThumbnailVisibility(bool);void SetCanCapture(bool);void GotFrame();void SetCapturePermittedByScheduler(bool);void UpdateCaptureState();void UpdateSchedulingPriority();void StartCooldown();void OnCooldownEnded();
};
'''+definitions+'\n}\n'
tests=r'''
template<class T>std::string snapshot(T& t,int failures){std::ostringstream s;s<<t.client.calls<<'/'<<static_cast<int>(t.capture_state_)<<'/'<<static_cast<int>(t.scheduler.priority)<<'/'<<t.cooldown_timer_.running<<'/'<<t.cooldown_timer_.stops<<'/'<<t.cooldown_timer_.starts<<'/'<<t.cooldown_timer_.resets<<'/'<<t.captured_cooldown_frame_<<'/'<<t.cooldown_retry_count_<<'/'<<failures;return s.str();}
template<class T>void seed(T& t,int state,int readiness,bool scheduled,bool can,bool running){t.capture_state_=static_cast<typename T::CaptureState>(state);t.page_readiness_=static_cast<typename T::PageReadiness>(readiness);t.scheduled_=scheduled;t.can_capture_=can;t.cooldown_timer_.running=running;}
template<class T>bool hostFlow(){base::host=true;checks_failed=0;T t;t.UpdateThumbnailVisibility(true);t.UpdatePageReadiness(T::PageReadiness::kReadyForInitialCapture);t.SetCapturePermittedByScheduler(true);t.SetCanCapture(true);t.UpdatePageReadiness(T::PageReadiness::kReadyForFinalCapture);t.GotFrame();t.cooldown_timer_.Advance(250);t.client.calls.clear();t.UpdatePageReadiness(T::PageReadiness::kReadyForInitialCapture);
 if(checks_failed||t.client.calls!="XRS"||t.cooldown_timer_.running||t.scheduler.priority!=ThumbnailScheduler::TabCapturePriority::kLow)return false;
 t.client.calls.clear();t.cooldown_timer_.Advance(2000);if(!t.client.calls.empty())return false;
 t.UpdatePageReadiness(T::PageReadiness::kReadyForFinalCapture);if(!t.cooldown_timer_.running||t.captured_cooldown_frame_)return false;
 t.cooldown_timer_.Advance(500);if(!t.cooldown_timer_.running||!t.client.calls.empty()||t.scheduler.priority!=ThumbnailScheduler::TabCapturePriority::kHigh)return false;
 t.GotFrame();t.cooldown_timer_.Advance(500);return t.client.calls=="X"&&!t.cooldown_timer_.running&&t.scheduler.priority==ThumbnailScheduler::TabCapturePriority::kNone&&!checks_failed;
}
int main(int argc,char** argv){std::string mode=argc>1?argv[1]:"";
 if(mode=="host-negative"){if(!hostFlow<baseline::ThumbnailCaptureDriver>())return 73;return 0;}
 base::host=false;int controls=0;
 for(int state=0;state<5;++state)for(int readiness=0;readiness<3;++readiness)for(int scheduled=0;scheduled<2;++scheduled)for(int can=0;can<2;++can)for(int running=0;running<2;++running){baseline::ThumbnailCaptureDriver a;candidate::ThumbnailCaptureDriver b;seed(a,state,readiness,scheduled,can,running);seed(b,state,readiness,scheduled,can,running);checks_failed=0;a.UpdateCaptureState();auto av=snapshot(a,checks_failed);checks_failed=0;b.UpdateCaptureState();auto bv=snapshot(b,checks_failed);if(av!=bv){std::cerr<<"nonhost divergence state="<<state<<" readiness="<<readiness<<" original="<<av<<" candidate="<<bv<<'\n';return 74;}++controls;}
 if(mode=="nonhost-negative")return 0;
 if(!hostFlow<candidate::ThumbnailCaptureDriver>())return 75;
 std::cout<<"PASS "<<controls<<" exact nonhost actual-method controls + host timer/capture restart and fresh-frame lifecycle\n";return 0;
}
'''
base=(F/'base'/D/'thumbnail_capture_driver.cc').read_text();gated=(F/'candidate'/D/'thumbnail_capture_driver.cc').read_text();old=(F/'cooldown'/D/'thumbnail_capture_driver.cc').read_text();results=[]
for name,candidate,mode,expected in [('positive',gated,'',0),('old-source-host-control',gated,'host-negative',73),('ungated-nonhost-control',old,'nonhost-negative',74)]:
 p=O/(name+'.cc');p.write_text(preamble+driver('baseline',base)+driver('candidate',candidate)+tests);exe=T/(name+'.exe');cmd=['/usr/bin/clang++','-std=c++20','-Wall','-Wextra','-Werror',str(p),'-o',str(exe)];c=subprocess.run(cmd,text=True,capture_output=True);(O/(name+'.compile.log')).write_text(c.stdout+c.stderr);assert c.returncode==0,c.stderr
 c=subprocess.run([str(exe),mode],text=True,capture_output=True);(O/(name+'.run.log')).write_text(c.stdout+c.stderr);assert c.returncode==expected,(name,c.returncode,c.stdout,c.stderr);results.append({'case':name,'exit':c.returncode,'expected':expected,'tu_sha256':hashlib.sha256(p.read_bytes()).hexdigest(),'compile':cmd});print(name,c.returncode)
(O/'extracted-results.json').write_text(json.dumps({'scope':'actual extracted driver methods, mock timer/client/scheduler/CommandLine only; no Chromium unit-test executable or engine run','method_names':methods,'results':results},indent=2)+'\n')
