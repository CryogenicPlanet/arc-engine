#!/usr/bin/env python3
"""Actual staged Source+logical Root+CloseState with Foundation/mock transport.

No Chromium runtime, real DevTools factory, facade construction transaction,
rebind, native ownership, or public Close is modeled.
"""
from pathlib import Path
import argparse,hashlib,importlib.util,json,subprocess,tempfile
ROOT=Path(__file__).resolve().parents[2]
BIND=r'''
namespace base {
template<class Signature> class OnceCallback;
template<class R,class... A> class OnceCallback<R(A...)> {
 public:
  OnceCallback()=default;
  template<class F> OnceCallback(F f):fn_(std::move(f)){}
  OnceCallback(OnceCallback&&)=default;
  OnceCallback& operator=(OnceCallback&&)=default;
  OnceCallback(const OnceCallback&)=delete;
  explicit operator bool()const{return bool(fn_);}
  R Run(A...a)&&{auto f=std::move(fn_);return f(std::forward<A>(a)...);}
 private: std::function<R(A...)>fn_;
};
using OnceClosure=OnceCallback<void()>;
template<class F,class...A> auto BindOnce(F f,A...a){
 return [f,args=std::make_shared<std::tuple<A...>>(std::move(a)...)](auto&&...tail)mutable{
   return std::apply([&](auto&...v){return f(std::move(v)...,std::forward<decltype(tail)>(tail)...);},*args);
 };
}
}
'''
SUPPORT=r'''
#import <objc/runtime.h>
#include <map>
namespace base {
template<class T>struct WeakPtr {
 T*ptr=nullptr; T*get()const{return ptr;}
 T*operator->()const{return ptr;} explicit operator bool()const{return ptr;}
 void reset(){ptr=nullptr;}
};
class SupportsUserData { public:
 struct Data {virtual ~Data()=default;};
 Data*GetUserData(const void*k){auto i=data.find(k);return i==data.end()?nullptr:i->second.get();}
 void SetUserData(const void*k,std::unique_ptr<Data>v){data[k]=std::move(v);}
 void RemoveUserData(const void*k){data.erase(k);}
 private:std::map<const void*,std::unique_ptr<Data>>data;
};
template<class T>class DeleteHelper {public:static void Delete(T*p){delete p;}};
}
namespace content {
struct BrowserThread{enum ID{UI};static bool CurrentlyOn(ID){return true;}};
struct TestRunner{template<class T>void DeleteSoon(int,T*p){base::DeleteHelper<T>::Delete(p);}};
inline TestRunner*GetUIThreadTaskRunner(std::initializer_list<int>){static TestRunner r;return &r;}
}
#define FROM_HERE 0
'''
TESTS=r'''
// A stateful sibling fixture tests the real Root's ownership slot. This is NOT
// a production DevTools implementation or a substitute factory in the proposal.
static int sibling_alive=0,sibling_attaches=0;
class TestDevToolsPeer final:public arc::CoreDevToolsPeer{
 public:
 explicit TestDevToolsPeer(arc::CoreLogicalWebContentsWeakHandle r):root_(r){++sibling_alive;}
 ~TestDevToolsPeer()override{--sibling_alive;}
 void AttachToWebContents(base::WeakPtr<content::WebContents>c)override{
   auto r=root_.lock();REQUIRE(r&&r->GetNative()==c.get());++sibling_attaches;
 }
 void RetireFromWebContents()override{}
 private:arc::CoreLogicalWebContentsWeakHandle root_;
};
static arc::CoreLogicalWebContentsHandle NewRoot(content::WebContents&physical){
 return arc::CreateCoreLogicalWebContents(physical.GetWeakPtr(),active,
  arc::WebextrasMediaPeerFactory(),base::BindOnce([](arc::CoreLogicalWebContentsWeakHandle r)
    ->std::shared_ptr<arc::CoreDevToolsPeer>{
   REQUIRE(observers==0&&tasks.empty());return std::make_shared<TestDevToolsPeer>(r);
  }));
}
int main(){@autoreleasepool{
 events=[NSMutableArray array];active=arc::CreateCoreCloseState();
 content::WebContents physical;
 // Factory is nonobserving/nonpublishing, same canonical state, both slots ready.
 auto root=NewRoot(physical);auto weak=arc::CoreLogicalWebContentsWeakHandle(root);
 auto source=std::static_pointer_cast<WebextrasMediaSource>(root->GetMediaPeer());
 REQUIRE(observers==0&&tasks.empty()&&source->Native()==&physical&&sibling_alive==1);
 REQUIRE(!GetWebextrasMediaSourceForNative(&physical));
 root->GetMediaPeer()->AttachToWebContents(physical.GetWeakPtr());
 root->GetDevToolsPeer()->AttachToWebContents(physical.GetWeakPtr());
 REQUIRE(observers==1&&sibling_attaches==1&&GetWebextrasMediaSourceForNative(&physical)==source);
 ArcWebContents* owner=[[ArcWebContents alloc]init];
 arc::AssociateWebextrasMediaFacade(owner,root);
 ArcWebContentsMediaSession* service=arc::CreateWebextrasMediaService(owner);
 REQUIRE(service->_source==source&&GetWebextrasMediaSource(owner)==source);
 auto d=Delegate(@"A");[service setDelegate:d];
 source->Position(media_session::MediaPosition{{1,2,3,41,5}});
 source->Info(Info(1,2,1));source->Actions({2,3});
 REQUIRE(tasks.size()==5&&arc::CoreCloseStateIsBusy(active));
 auto snapshot=[service position];REQUIRE(snapshot&&snapshot->copied.words[3]==41);
 // Every accepted task retains acquired Root and its real sibling ownership slot.
 owner=nil;root.reset();REQUIRE(!weak.expired()&&sibling_alive==1);
 REQUIRE(source->Native()==&physical&&[service nativeSession]); // facade absence is not native death.
 Drain();REQUIRE(weak.expired()&&sibling_alive==0&&source->Native()==nullptr&&![service nativeSession]);
 REQUIRE(d.sawNilOwner&&!arc::CoreCloseStateIsBusy(active));
 Expect(@[@"A:position:41",@"A:state:1",@"A:camera:2",@"A:mic:1",@"A:actions:2"]);
 // Retained service/physical observer cannot accept new work after Root death.
 source->Info(Info(2,0,0));source->Actions({1});source->Position(std::nullopt);
 source->VolumeChanged(.8);REQUIRE(tasks.empty()&&source->PositionSnapshot()->words[3]==41);
 auto later=[service position];REQUIRE(later!=snapshot&&later->copied.words[3]==41);
 service=nil;source.reset();REQUIRE(observers==0);
 // No facade/service/registration: actual native weak route selects Source and
 // inert tasks retain acquired Root+sibling while remaining uncounted.
 auto inert=NewRoot(physical);auto weakInert=arc::CoreLogicalWebContentsWeakHandle(inert);
 inert->GetMediaPeer()->AttachToWebContents(physical.GetWeakPtr());
 auto actual=GetWebextrasMediaSourceForNative(&physical);REQUIRE(actual);
 actual->Info(Info());actual->Actions({1});
 REQUIRE(tasks.size()==4&&!arc::CoreCloseStateIsBusy(active));
 std::weak_ptr<WebextrasMediaSource> weakSource=actual;actual.reset();inert.reset();
 REQUIRE(!weakInert.expired()&&!weakSource.expired()&&sibling_alive==1);
 Drain();REQUIRE(weakInert.expired()&&weakSource.expired()&&sibling_alive==0&&observers==0);
 // A retained peer alone is not a logical owner: no new cache or task changes.
 auto dead=NewRoot(physical);auto retained=std::static_pointer_cast<WebextrasMediaSource>(dead->GetMediaPeer());
 dead->GetMediaPeer()->AttachToWebContents(physical.GetWeakPtr());
 retained->Position(media_session::MediaPosition{{1,2,3,7,5}});Drain();
 auto absent=arc::CoreLogicalWebContentsWeakHandle(dead);dead.reset();
 REQUIRE(absent.expired());retained->Position(std::nullopt);retained->Info(Info());retained->Actions({1});
 REQUIRE(tasks.empty()&&retained->PositionSnapshot()->words[3]==7);
 retained.reset();REQUIRE(observers==0&&tasks.empty()&&sibling_alive==0);
 // Root completion, not an arbitrary attach-first rule, is the association gate.
 auto before=NewRoot(physical);ArcWebContents* earlyOwner=[[ArcWebContents alloc]init];
 arc::AssociateWebextrasMediaFacade(earlyOwner,before);
 ArcWebContentsMediaSession* earlyService=arc::CreateWebextrasMediaService(earlyOwner);
 REQUIRE(observers==0&&tasks.empty()&&earlyService->_source.get()==before->GetMediaPeer().get());
 before->GetMediaPeer()->AttachToWebContents(physical.GetWeakPtr());REQUIRE(observers==1);
 earlyOwner=nil;earlyService=nil;before.reset();REQUIRE(observers==0&&sibling_alive==0);
 std::puts("PASS actual Source+Root+CloseState: nonobserving factory, completed attach, same-peer facade adoption, root-before-cache, live native without facade, accepted/inert root+sibling retention, service-only dead-root rejection, cached snapshots survive root death");
}}
'''
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--stage',type=Path,required=True);ap.add_argument('--out',type=Path,required=True);ap.add_argument('--negative-source-only',action='store_true');ap.add_argument('--negative-root-guard',action='store_true');args=ap.parse_args()
 def module(name,file):
  spec=importlib.util.spec_from_file_location(name,ROOT/file);m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m);return m
 b=module('base','arc/browser/webextras_media_delivery_test.py');c=module('counter','arc/browser/webextras_media_close_accounting_held_test.py')
 src=(args.stage/'arc/api/arc_webextras_media.mm').read_text();primitive=(args.stage/'arc/browser/core_close_state.mm').read_text();header=(args.stage/'arc/browser/core_close_state.h').read_text();rh=(args.stage/'arc/browser/core_logical_web_contents.h').read_text();rm=(args.stage/'arc/browser/core_logical_web_contents.mm').read_text()
 decl=header[header.index('enum class CoreCloseDelegateChannel'):header.index('// Call immediately at enqueue')]
 impl=primitive[primitive.index('namespace arc {'):primitive.index('CoreCloseObserverBatch::CoreCloseObserverBatch')]
 pre=b.PRELUDE;start=pre.index('namespace base {');end=pre.index('static std::deque<base::OnceClosure>');pre=pre[:start]+BIND+SUPPORT+pre[end:]
 pre=pre.replace('f(); }\nstatic void Drain()', 'std::move(f).Run(); }\nstatic void Drain()')
 pre=pre.replace('struct WebContents {','class WebContents : public base::SupportsUserData { public:')
 pre=pre.replace('struct MediaSession { static MediaSession* Get(WebContents*) { return nullptr; } };','struct MediaSession { static MediaSession* Get(WebContents*) { static MediaSession s; return &s; } static MediaSession* GetIfExists(WebContents*c) { return c?Get(c):nullptr; } };')
 pre=pre.replace('static NSString* ArcNSString','[[maybe_unused]] static NSString* ArcNSString')
 pre=pre.replace('@interface ArcWebContents : NSObject @end','namespace arc {class CoreCloseState;using CoreCloseStateHandle=std::shared_ptr<CoreCloseState>;'+decl+'\n}\n'+impl+'\n}\nstatic arc::CoreCloseStateHandle active;\n@interface ArcWebContents : NSObject @end')
 # Remove fake observer; root/Source actual Attach uses a recording mock transport.
 start=pre.index('class ArcMediaObserver {');pre=pre[:start]+'''class ArcMediaObserver {public:
 ArcMediaObserver(content::WebContents*,content::MediaSession*,std::weak_ptr<WebextrasMediaSource>){++observers;}
 ~ArcMediaObserver(){--observers;}void Detach(){++detachments;}
};
'''
 rootheader=rh[rh.index('namespace base {'):rh.rindex('#endif')]
 rootimpl=rm[rm.index('namespace arc {'):]
 protocol=src[src.index('@protocol ArcMediaDelegate'):src.index('@end',src.index('@protocol ArcMediaDelegate'))+4]
 helper=src[src.index('// BEGIN WEBEXTRAS_MEDIA_DELIVERY_HELPER'):src.index('// END WEBEXTRAS_MEDIA_DELIVERY_HELPER')]
 start=src.index('// The constructor is nonobserving:');cls=src[start:src.index('\nBOOL ArcMediaFailure',start)]
 start=src.index('// Facade association retains only');associate=src[start:src.index('ARC_EXPORT @interface ArcWebContentsMediaSession :',start)]
 # Strip anonymous namespace braces only for extraction's shared namespace.
 associate=associate.replace('namespace {\n','').replace('\n}\n\nnamespace arc {','\n\nnamespace arc {',1)
 start=associate.index('  arcmod::GetVideoOverlayWindowHook()');end=associate.index('\n',start);associate=associate[:start]+associate[end+1:]
 methods=''
 for a,z in [('- (instancetype)initWithArcWebContents:(ArcWebContents*)owner {','- (content::MediaSession*)nativeSession'),('- (void)setDelegate:(id<ArcMediaDelegate>)delegate {','- (NSIndexSet*)getMediaContentTypes:'),('- (content::MediaSession*)nativeSession __attribute__((objc_direct)) {','- (content::MediaSession*)requireSession:')]:
  start=src.index(a,src.index('@implementation ArcWebContentsMediaSession {'));methods+=src[start:src.index(z,start)]
 start=src.index('- (ArcMediaPosition*)position {');getter=src[start:src.index('\n#pragma clang diagnostic pop',start)]
 start=src.index('void WebextrasMediaSource::PiPCreated()');pip=src[start:src.index('ArcMediaObserver::ArcMediaObserver',start)]
 service=b.SERVICE.replace('SETDELEGATE',methods).replace('GETPOSITION',getter).replace('PIPDEFINITIONS',pip).replace('OVERLAY','')
 service=service.replace('- (void)setDelegate:(id<ArcMediaDelegate>)delegate;','- (content::MediaSession*)nativeSession __attribute__((objc_direct));\n- (instancetype)initWithArcWebContents:(ArcWebContents*)owner;\n- (void)setDelegate:(id<ArcMediaDelegate>)delegate;')
 service=service.replace('static ArcWebContentsMediaSession* Service(', '[[maybe_unused]] static ArcWebContentsMediaSession* Service(')
 # Negative controls mutate extracted code only; production proposal is unchanged.
 if args.negative_source_only:
  cls=cls.replace('delivery.Async(logical,','delivery.Async(shared_from_this(),').replace('delivery.AsyncPrepared(logical,','delivery.AsyncPrepared(shared_from_this(),')
 if args.negative_root_guard:
  cls=cls.replace('if (!logical || !info)', 'if (!info)').replace('    if (!logical) { return; }\n', '')
 pre=pre.replace('std::abort();', 'std::exit(73);')
 # Define Root before Source (prelude's fake observer only requires forward Source).
 factory_start=src.index('namespace arc {\nArcWebContentsMediaSession* CreateWebextrasMediaService');factory=src[factory_start:src.index('namespace {\nvoid WebextrasMediaSource::PiPCreated()',factory_start)]
 text=c.CHECKS+pre+rootheader+rootimpl+protocol+'\n'+helper+'\n'+cls+'\n'+associate+'\n'+service+factory+TESTS
 args.out.mkdir(parents=True,exist_ok=True);(args.out/'test.mm').write_text(text)
 with tempfile.TemporaryDirectory(prefix='webextras-logical-test-')as tmp:
  cmd=['xcrun','clang++','-std=c++23','-fobjc-arc','-fno-exceptions','-O2','-Wall','-Wextra','-Werror','-framework','Foundation','-framework','CoreGraphics',str(args.out.resolve()/'test.mm'),'-o',str(Path(tmp)/'test')]
  p=subprocess.run(cmd,text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT);output=p.stdout;code=p.returncode
  if not code:
   p=subprocess.run([str(Path(tmp)/'test')],text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT);output+=p.stdout;code=p.returncode
 result={'source_sha256':hashlib.sha256(src.encode()).hexdigest(),'root_header_sha256':hashlib.sha256(rh.encode()).hexdigest(),'root_impl_sha256':hashlib.sha256(rm.encode()).hexdigest(),'primitive_sha256':hashlib.sha256(primitive.encode()).hexdigest(),'extracted_test_sha256':hashlib.sha256(text.encode()).hexdigest(),'command':cmd,'exit_code':code,'negative_source_only':args.negative_source_only,'negative_root_guard':args.negative_root_guard,'output':output,'limits':'Actual extracted Source/association/adoption/Root/CloseState; Foundation tasks/native WeakPtr/userdata/observer transport mocked; stateful sibling fixture tests Root strong slot, not production DevTools; no runtime/ownership/rebind/public Close'}
 (args.out/'test-result.json').write_text(json.dumps(result,indent=2)+'\n');print(output,end='');raise SystemExit(code)
if __name__=='__main__':main()
