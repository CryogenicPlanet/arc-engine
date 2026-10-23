#!/usr/bin/env python3
"""Execute proposed Media delivery with the actual held CoreCloseState primitive.

Foundation task/bind/check/sequence scaffolding is mocked. No Chromium runtime,
public Close activation, or exception behavior is claimed.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
BIND = r'''
namespace base {
class OnceClosure {
 public:
  OnceClosure()=default;
  template<class F> OnceClosure(F f): fn_(std::move(f)) {}
  OnceClosure(OnceClosure&&)=default;
  OnceClosure& operator=(OnceClosure&&)=default;
  OnceClosure(const OnceClosure&)=delete;
  OnceClosure& operator=(const OnceClosure&)=delete;
  explicit operator bool() const { return bool(fn_); }
  void Run() && { auto f=std::move(fn_); if(f) f(); }
 private: std::function<void()> fn_;
};
template<class F,class...A> OnceClosure BindOnce(F f,A...a) {
  auto args=std::make_shared<std::tuple<A...>>(std::move(a)...);
  return [f,args]() mutable { std::apply([&](auto&...v){ f(std::move(v)...); },*args); };
}
template<class T> struct WeakPtr { T* ptr=nullptr; void reset(){ptr=nullptr;} };
}
'''
CHECKS = r'''
#include <limits>
#include <cstdlib>
#include <utility>
struct TestCheckStream { template<class T> TestCheckStream& operator<<(T&&){return *this;} };
static TestCheckStream CheckStream(){return {};}
[[noreturn]] static void AbortCheck(){std::abort();}
#define CHECK(x) ((x)?CheckStream():(AbortCheck(),CheckStream()))
#define CHECK_LT(a,b) CHECK((a)<(b))
#define CHECK_GT(a,b) CHECK((a)>(b))
#define SEQUENCE_CHECKER(name) int name=0
#define DCHECK_CALLED_ON_VALID_SEQUENCE(name) ((void)name)
'''
TESTS = r'''
int main(){ @autoreleasepool {
  events=[NSMutableArray array]; content::WebContents physical;
  ArcWebContents* owner=[[ArcWebContents alloc] init];
  active=arc::CreateCoreCloseState(); owner->canonical=active;
  auto source=std::make_shared<WebextrasMediaSource>(owner);
  REQUIRE(canonical_lookups==1); source->AttachInitial(&physical);
  ArcWebContentsMediaSession* service=Service(source,owner);
  WebextrasTestDelegate* a=Delegate(@"A"); WebextrasTestDelegate* b=Delegate(@"B");
  [service setDelegate:a]; REQUIRE(!arc::CoreCloseStateIsBusy(active));
  // Three genuine invocations acquire at receipt, not at selector delivery.
  source->Info(Info()); REQUIRE(tasks.size()==3 && arc::CoreCloseStateIsBusy(active));
  DrainOne(); REQUIRE(arc::CoreCloseStateIsBusy(active));
  DrainOne(); REQUIRE(arc::CoreCloseStateIsBusy(active));
  DrainOne(); REQUIRE(!arc::CoreCloseStateIsBusy(active));
  Expect(@[@"A:state:3",@"A:camera:0",@"A:mic:0"]);
  // Accepted work decrements after conditional return, including expired bridge.
  source->VolumeChanged(.1); [service setDelegate:nil];
  REQUIRE(arc::CoreCloseStateIsBusy(active)); Drain(); REQUIRE(!arc::CoreCloseStateIsBusy(active)); Expect(@[]);
  [service setDelegate:a]; source->VolumeChanged(.2); [service setDelegate:b]; Drain();
  Expect(@[@"B:volume:20"]); REQUIRE(!arc::CoreCloseStateIsBusy(active));
  source->VolumeChanged(.3); [service setDelegate:nil]; [service setDelegate:a]; Drain(); Expect(@[]);
  // Outer async remains pending while a nested run loop drains the old bridge
  // after nil/re-add. New selected work is counted separately on the SAME state.
  @autoreleasepool {
    source->VolumeChanged(.4); source->VolumeChanged(.5);
    __weak WebextrasTestDelegate* weakA=a;
    a.onCallback=^{ weakA.onCallback=nil; [service setDelegate:nil]; [service setDelegate:b];
      source->VolumeChanged(.6); Drain(); REQUIRE(arc::CoreCloseStateIsBusy(active)); };
    Drain(); Expect(@[@"A:volume:40",@"A:volume:50",@"B:volume:60"]);
    REQUIRE(!arc::CoreCloseStateIsBusy(active));
  }
  // Sync starts after bridge selection, before DTO preparation/weak ObjC loads,
  // and nested same-domain scopes restore the previous Boolean.
  @autoreleasepool {
    [service setDelegate:a];
    onControllerInit=^{ onControllerInit=nil; REQUIRE(arc::CoreCloseStateIsBusy(active));
      source->delivery.Sync([](id<ArcMediaDelegate> d,ArcWebContents* o){[d webContentsHidePiP:o];});
      REQUIRE(arc::CoreCloseStateIsBusy(active)); };
    expectedSync=true; source->PiPCreated(); expectedSync=false; REQUIRE(!arc::CoreCloseStateIsBusy(active));
    Expect(@[@"A:hide:0",@"A:created:0"]);
    __weak WebextrasTestDelegate* weakA=a;
    a.onCallback=^{ weakA.onCallback=nil; source->VolumeChanged(.7); Drain();
      REQUIRE(arc::CoreCloseStateIsBusy(active)); };
    expectedSync=true; source->delivery.Sync([](id<ArcMediaDelegate> d,ArcWebContents* o){[d webContentsShowPiP:o];}); expectedSync=false;
    REQUIRE(!arc::CoreCloseStateIsBusy(active)); Expect(@[@"A:show:0",@"A:volume:70"]);
  }
  // A live bridge counts even with an expired app weak delegate.
  @autoreleasepool { WebextrasTestDelegate* expired=Delegate(@"expired"); [service setDelegate:expired]; }
  source->VolumeChanged(.8); REQUIRE(arc::CoreCloseStateIsBusy(active)); Drain();
  REQUIRE(!arc::CoreCloseStateIsBusy(active)); Expect(@[]);
  // A bridge whose app object implements no optional selector still counts.
  NSObject* partial=[[NSObject alloc]init]; [service setDelegate:(id<ArcMediaDelegate>)partial];
  source->VolumeChanged(.81); REQUIRE(arc::CoreCloseStateIsBusy(active)&&!MediaSync()); Drain();
  REQUIRE(!arc::CoreCloseStateIsBusy(active)); Expect(@[]);
  // Absence of the bridge has no synchronous flag and no pending acquisition.
  [service setDelegate:nil]; source->VolumeChanged(.9); REQUIRE(!arc::CoreCloseStateIsBusy(active));
  source->delivery.Sync([](id<ArcMediaDelegate>,ArcWebContents*){REQUIRE(false);}); Drain();
  REQUIRE(!arc::CoreCloseStateIsBusy(active));
  // Source captured the original canonical state ONCE, before subscribing.
  auto replacement=arc::CreateCoreCloseState(); owner->canonical=replacement;
  [service setDelegate:a]; source->VolumeChanged(.11);
  REQUIRE(arc::CoreCloseStateIsBusy(active) && !arc::CoreCloseStateIsBusy(replacement));
  owner=nil; Drain(); REQUIRE(a.sawNilOwner && canonical_lookups==1); Expect(@[@"A:volume:11"]);
  expectedSync=true; source->RetirePhysical(); expectedSync=false; Expect(@[@"A:destroyed:0"]);
  REQUIRE(!arc::CoreCloseStateIsBusy(active)); service=nil; source.reset(); REQUIRE(observers==0);
  // Inert tasks retain functional source state but do not count it as pending.
  ArcWebContents* inertOwner=[[ArcWebContents alloc] init]; inertOwner->canonical=active;
  auto inert=std::make_shared<WebextrasMediaSource>(inertOwner); inert->AttachInitial(&physical);
  inert->Info(Info()); std::weak_ptr<WebextrasMediaSource> weakInert=inert; inert.reset();
  REQUIRE(!weakInert.expired() && !arc::CoreCloseStateIsBusy(active));
  Drain(); REQUIRE(weakInert.expired() && !arc::CoreCloseStateIsBusy(active));
  // Destruction without Run intentionally does NOT decrement native pending.
  ArcWebContents* cancelledOwner=[[ArcWebContents alloc] init];
  auto cancelledState=arc::CreateCoreCloseState(); cancelledOwner->canonical=cancelledState;
  auto cancelled=std::make_shared<WebextrasMediaSource>(cancelledOwner); cancelled->AttachInitial(&physical);
  ArcWebContentsMediaSession* cancelledService=Service(cancelled,cancelledOwner); [cancelledService setDelegate:a];
  cancelled->VolumeChanged(.12); REQUIRE(arc::CoreCloseStateIsBusy(cancelledState));
  tasks.clear(); REQUIRE(arc::CoreCloseStateIsBusy(cancelledState));
  cancelledService=nil; cancelled.reset(); REQUIRE(observers==0 && tasks.empty());
  std::puts("PASS actual held CoreCloseState + Media delivery/Source: enqueue pending, postreturn decrement, nested sync/async, replacement/nil/expired/nil-owner, same canonical capture, inert lifetime, unrun-task pending policy");
} }
'''


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--stage', type=Path, required=True)
    ap.add_argument('--out', type=Path, required=True)
    args = ap.parse_args()
    spec = importlib.util.spec_from_file_location('delivery_base', ROOT / 'arc/browser/webextras_media_delivery_test.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    source = (args.stage / 'arc/api/arc_webextras_media.mm').read_text()
    header = (args.stage / 'arc/browser/core_close_state.h').read_text()
    primitive = (args.stage / 'arc/browser/core_close_state.mm').read_text()
    declarations = header[header.index('enum class CoreCloseDelegateChannel'):header.index('// Call immediately at enqueue')]
    implementation = primitive[primitive.index('namespace arc {'):primitive.index('CoreCloseObserverBatch::CoreCloseObserverBatch')]
    prelude = module.PRELUDE
    start, end = prelude.index('namespace base {'), prelude.index('static std::deque<base::OnceClosure>')
    prelude = prelude[:start] + BIND + prelude[end:]
    prelude = prelude.replace('f(); }\nstatic void Drain()', 'std::move(f).Run(); }\nstatic void Drain()')
    original_owner = '@interface ArcWebContents : NSObject @end\n@implementation ArcWebContents @end'
    owner = '''namespace arc { class CoreCloseState; using CoreCloseStateHandle=std::shared_ptr<CoreCloseState>;\n''' + declarations + '\n}\n' + implementation + '\n}\n' + '''
static int canonical_lookups=0;
static arc::CoreCloseStateHandle active;
static bool expectedSync=false;
static bool MediaSync(){
  const bool previous=active->EnterSync(arc::CoreCloseDelegateChannel::kMedia);
  active->LeaveSync(arc::CoreCloseDelegateChannel::kMedia,previous);return previous;
}
@interface ArcWebContents : NSObject { @public arc::CoreCloseStateHandle canonical; } @end
@implementation ArcWebContents @end
namespace arc {
CoreCloseStateHandle CoreCloseStateForWebContents(ArcWebContents* owner) {
  REQUIRE(owner && owner->canonical); ++canonical_lookups; return owner->canonical;
}
}
'''
    prelude = CHECKS + prelude.replace(original_owner, owner)
    prelude = prelude.replace('static NSString* ArcNSString', '[[maybe_unused]] static NSString* ArcNSString')
    protocol = source[source.index('@protocol ArcMediaDelegate'):source.index('@end', source.index('@protocol ArcMediaDelegate')) + 4]
    helper = source[source.index('// BEGIN WEBEXTRAS_MEDIA_DELIVERY_HELPER'):source.index('// END WEBEXTRAS_MEDIA_DELIVERY_HELPER')]
    start = source.index('class WebextrasMediaSource final')
    cls = source[start:source.index('\nBOOL ArcMediaFailure', start)]
    start = source.index('- (void)setDelegate:(id<ArcMediaDelegate>)delegate {')
    method = source[start:source.index('- (NSIndexSet*)getMediaContentTypes:', start)]
    start = source.index('- (ArcMediaPosition*)position {')
    getter = source[start:source.index('\n#pragma clang diagnostic pop', start)]
    start = source.index('void WebextrasMediaSource::PiPCreated()')
    pip = source[start:source.index('ArcMediaObserver::ArcMediaObserver', start)]
    service = module.SERVICE.replace('SETDELEGATE', method).replace('GETPOSITION', getter).replace('PIPDEFINITIONS', pip).replace('OVERLAY', '')
    service = service.replace('  _sawNilOwner=!owner;', '  REQUIRE(arc::CoreCloseStateIsBusy(active)); REQUIRE(MediaSync()==expectedSync);\n  _sawNilOwner=!owner;')
    text = prelude + protocol + '\n' + helper + '\n' + cls + '\n' + service + TESTS
    args.out.mkdir(parents=True, exist_ok=True)
    (args.out / 'test.mm').write_text(text)
    with tempfile.TemporaryDirectory(prefix='webextras-media-accounting-test-') as tmp:
        cmd = ['xcrun', 'clang++', '-std=c++20', '-fobjc-arc', '-fno-exceptions', '-O2', '-Wall', '-Wextra', '-Werror', '-framework', 'Foundation', '-framework', 'CoreGraphics', str(args.out.resolve() / 'test.mm'), '-o', str(Path(tmp) / 'test')]
        p = subprocess.run(cmd, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        output, code = p.stdout, p.returncode
        if not code:
            p = subprocess.run([str(Path(tmp) / 'test')], text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            output += p.stdout
            code = p.returncode
    result = {'source_sha256': hashlib.sha256(source.encode()).hexdigest(), 'primitive_sha256': hashlib.sha256(primitive.encode()).hexdigest(), 'header_sha256': hashlib.sha256(header.encode()).hexdigest(), 'extracted_test_sha256': hashlib.sha256(text.encode()).hexdigest(), 'compile_command': cmd, 'exit_code': code, 'output': output, 'limits': 'Foundation mock bind/task/check/sequence scaffolding; actual extracted primitive and proposed Media delivery/cache/Source; no C++ exceptions, Chromium runtime or public Close'}
    (args.out / 'test-result.json').write_text(json.dumps(result, indent=2) + '\n')
    print(output, end='')
    raise SystemExit(code)


if __name__ == '__main__':
    main()
