#!/usr/bin/env python3
"""Execute actual gated SetVolume bodies against original ordinary source bodies.

Reads an explicit private Chromium source stage. Only that private stage receives
fixture/log writes. Pipeline/watch/client/delegate and CommandLine are mocks;
there is no browser, Mojo runtime, native callback or acceptance claim.
"""
from pathlib import Path
import argparse,hashlib,json,subprocess
p=argparse.ArgumentParser();p.add_argument('--stage',type=Path,required=True);p.add_argument('--negative-controls',action='store_true');args=p.parse_args();stage=args.stage.resolve();root=Path.cwd();rel='third_party/blink/renderer/platform/media/web_media_player_impl.cc'
def body(text,signature):
 start=text.index(signature);opening=text.index('{',start);depth=1;end=opening+1
 while depth:
  depth+=(text[end]=='{')-(text[end]=='}');end+=1
 return text[start:end]
current=(stage/rel).read_text();ordinary=(root/'chromium/src'/rel).read_text();signatures=['void WebMediaPlayerImpl::SetVolume(double volume)','void WebMediaPlayerImpl::SetVolumeMultiplier(double multiplier)'];now='\n'.join(body(current,s) for s in signatures);old='\n'.join(body(ordinary,s) for s in signatures)
assert 'DidPlayerVolumeChange' not in old,'ordinary baseline no longer original; audit/rebase explicitly'
assert body(current,signatures[1])==body(ordinary,signatures[1]),'multiplier body changed'
# Removing only the added old-value snapshot and guarded callback must recreate
# the complete original ordinary producer body, including every early return.
addition='''  const double previous_volume = volume_;
'''
conditional='''  if (previous_volume != volume &&
      base::CommandLine::ForCurrentProcess()->HasSwitch("arcmod-host-app")) {
    client_->DidPlayerVolumeChange(volume);
  }
'''
assert now.count(addition)==1 and now.count(conditional)==1
assert now.replace(addition,'').replace(conditional,'')==old,'ordinary arithmetic/order/tail drift'
preamble=r'''
#include <cassert>
#include <optional>
#include <set>
#include <string>
#include <vector>
#include <iostream>
struct Log{template<class T>Log&operator<<(const T&){return *this;}};
#define DVLOG(x) if(false) Log()
#define DCHECK(x) assert(x)
namespace base {class CommandLine {public:
 static CommandLine*ForCurrentProcess(){static CommandLine line;return &line;}
 bool HasSwitch(const char* name)const{assert(std::string(name)=="arcmod-host-app");return switches.contains(name);}
 std::set<std::string>switches;
};}
struct Event{char kind;double value;bool operator==(const Event&)const=default;};
struct Pipeline{std::vector<Event>*events;double value=1;void SetVolume(double v){value=v;events->push_back({'p',v});}};
struct Watch{std::vector<Event>*events;void OnVolumeChange(double v){events->push_back({'w',v});}};
struct Client{std::vector<Event>*events;void DidPlayerMutedStatusChange(bool muted){events->push_back({'m',double(muted)});}void DidPlayerVolumeChange(double v){events->push_back({'v',v});}void ResumePlayback(){events->push_back({'r',0});}};
struct Runner{bool BelongsToCurrentThread(){return true;}};
struct PlayerState{
 double volume_=1,volume_multiplier_=1;Pipeline*pipeline_controller_;Watch*watch_time_reporter_;Client*client_;Runner*main_task_runner_;std::vector<Event>*events;
 bool delegate_has_audio_=false,has_audio=false,hidden=false;std::optional<int>visibility_pause_reason_;
 bool HasUnmutedAudio(){return has_audio&&volume_>0;}
 bool IsPausedBecausePageHidden(){return hidden&&visibility_pause_reason_.has_value();}
 void DidMediaMetadataChange(){events->push_back({'d',double(delegate_has_audio_)});}
 void UpdatePlayState(){events->push_back({'s',0});}
};
'''
def make_fixture(functions):
 source=preamble
 for name,text in [('gated',functions),('ordinary',old)]:source+='\nnamespace '+name+'{struct WebMediaPlayerImpl:PlayerState{void SetVolume(double);void SetVolumeMultiplier(double);};\n'+text+'\n}\n'
 return source+r'''
constexpr double precise=.123456789123;
template<class Player>struct Rig{
 std::vector<Event>events;Pipeline pipeline{&events};Watch watch{&events};Client client{&events};Runner runner;Player player;
 Rig(bool watched,bool audio,bool hidden){player.pipeline_controller_=&pipeline;player.watch_time_reporter_=watched?&watch:nullptr;player.client_=&client;player.main_task_runner_=&runner;player.events=&events;player.has_audio=audio;player.hidden=hidden;if(hidden)player.visibility_pause_reason_=99;}
 void Run(){player.SetVolume(.625);player.SetVolume(.625);player.SetVolume(.375);player.SetVolumeMultiplier(.25);player.SetVolume(0);player.SetVolumeMultiplier(.5);player.SetVolume(.375);player.SetVolume(precise);}
};
int main(){unsigned ordinaryCases=0,hostCases=0;
 for(bool watched:{false,true})for(bool audio:{false,true})for(bool hidden:{false,true}){
  Rig<ordinary::WebMediaPlayerImpl>baseline(watched,audio,hidden);baseline.Run();
  for(auto flags:{std::set<std::string>{},std::set<std::string>{"arcmod-host-app-other"}}){
   base::CommandLine::ForCurrentProcess()->switches=flags;Rig<gated::WebMediaPlayerImpl>current(watched,audio,hidden);current.Run();
   assert(current.events==baseline.events&&"ordinary full producer behavior changed or extra IPC client call");
   assert(current.pipeline.value==baseline.pipeline.value&&current.player.volume_==baseline.player.volume_&&current.player.volume_multiplier_==baseline.player.volume_multiplier_&&current.player.delegate_has_audio_==baseline.player.delegate_has_audio_&&current.player.visibility_pause_reason_==baseline.player.visibility_pause_reason_);++ordinaryCases;
  }
  base::CommandLine::ForCurrentProcess()->switches={"arcmod-host-app"};Rig<gated::WebMediaPlayerImpl>host(watched,audio,hidden);host.Run();std::vector<Event>unchanged;std::vector<double>volumes;
  for(size_t i=0;i<host.events.size();++i){auto event=host.events[i];if(event.kind=='v'){assert(i&&host.events[i-1].kind=='m'&&"volume notification precedes mute");volumes.push_back(event.value);}else unchanged.push_back(event);}
  assert((volumes==std::vector<double>{.625,.375,0,.375,precise})&&"host double input/dedup/multiplier policy changed");
  assert(unchanged==baseline.events&&host.pipeline.value==precise*.5&&"host altered ordinary call order/arithmetic");++hostCases;
 }
 assert(ordinaryCases==16&&hostCases==8);std::cout<<"PASS 16 ordinary full-source controls +8 host changed-input/order/precision controls\n";
}
'''
def run(source,name,expected=0,marker=None):
 cc=stage/(name+'.cc');binary=stage/name;cc.write_text(source);command=['/usr/bin/clang++','-std=c++20','-Wall','-Wextra','-Werror',str(cc),'-o',str(binary)];c=subprocess.run(command,capture_output=True,text=True);(stage/(name+'-compiler.log')).write_text(c.stdout+c.stderr);assert c.returncode==0,c.stderr;r=subprocess.run([str(binary)],capture_output=True,text=True);(stage/(name+'-run.log')).write_text(r.stdout+r.stderr)
 if expected==0:assert r.returncode==0,r.stdout+r.stderr
 else:assert r.returncode!=0 and marker in r.stderr,(r.returncode,r.stdout,r.stderr)
 return {'name':name,'compilerExit':c.returncode,'runExit':r.returncode,'expectedAssertion':marker,'sourceSHA256':hashlib.sha256(source.encode()).hexdigest(),'output':r.stdout+r.stderr}
results=[run(make_fixture(now),'actual-host-scope')]
if args.negative_controls:
 unconditional=now.replace('previous_volume != volume &&\n      base::CommandLine::ForCurrentProcess()->HasSwitch("arcmod-host-app")','previous_volume != volume');assert unconditional!=now
 early=now.replace('  volume_ = volume;','  if (!base::CommandLine::ForCurrentProcess()->HasSwitch("arcmod-host-app")) return;\n  volume_ = volume;');assert early!=now
 multiplied=now.replace('client_->DidPlayerVolumeChange(volume);','client_->DidPlayerVolumeChange(volume * volume_multiplier_);')
 results+=[run(make_fixture(unconditional),'negative-unconditional',1,'ordinary full producer behavior changed or extra IPC client call'),run(make_fixture(early),'negative-nonhost-early-return',1,'ordinary full producer behavior changed or extra IPC client call'),run(make_fixture(multiplied),'negative-multiplied-input',1,'host double input/dedup/multiplier policy changed')]
(stage/'host-gate-standalone-results.json').write_text(json.dumps({'actualBodies':{s:{'ordinary':hashlib.sha256(body(ordinary,s).encode()).hexdigest(),'hostGated':hashlib.sha256(body(current,s).encode()).hexdigest()} for s in signatures},'results':results,'nativeIPC':'not executed; full-source-body client emission controls only'},indent=2)+'\n');print(json.dumps(results,indent=2))
