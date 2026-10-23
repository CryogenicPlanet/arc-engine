#!/usr/bin/env python3
"""Actual held primitive + PDFReady/DeliverPDF bodies, with mock observer/tasks."""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
PRELUDE = r'''
#import <Foundation/Foundation.h>
#include <array>
#include <deque>
#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>
#include <cstdint>
#include <cstdio>
#define REQUIRE(x) do { if(!(x)){std::fprintf(stderr,"failure %d: %s\n",__LINE__,#x);std::abort();} } while(0)
'''
SCAFFOLD = r'''
static std::deque<base::OnceClosure> tasks;
static void Drain(){while(!tasks.empty()){auto task=std::move(tasks.front());tasks.pop_front();std::move(task).Run();}}
struct Runner { bool PostTask(int,base::OnceClosure task){tasks.push_back(std::move(task));return true;} };
namespace content {
struct WebContents {};
static Runner* GetUIThreadTaskRunner(std::initializer_list<int>){static Runner r;return &r;}
}
namespace pdf {
struct PDFDocumentHelper {
  bool complete=true;
  static PDFDocumentHelper* MaybeGetForFullPagePdf(content::WebContents&){static PDFDocumentHelper h;return &h;}
  bool IsDocumentLoadComplete(){return complete;}
};
}
#define FROM_HERE 0
static arc::CoreCloseStateHandle active;
static bool UISync(){const bool previous=active->EnterSync(arc::CoreCloseDelegateChannel::kUI);
  active->LeaveSync(arc::CoreCloseDelegateChannel::kUI,previous);return previous;}
static int canonical_lookups=0, published=0, callbacks=0;
static std::function<void()> onSetter;
@interface ArcWebContents : NSObject {
 @public arc::CoreCloseStateHandle canonical;
}
@property(nonatomic,weak) id uiDelegate;
@property(nonatomic,strong) id pdfController;
@end
@implementation ArcWebContents
@synthesize uiDelegate=_uiDelegate,pdfController=_pdfController;
- (void)setPdfController:(id)value {
  REQUIRE(arc::CoreCloseStateIsBusy(active)&&!UISync()); _pdfController=value; ++published;
  auto callback=onSetter;if(callback)callback();
}
@end
namespace arc {
CoreCloseStateHandle CoreCloseStateForWebContents(ArcWebContents* owner){REQUIRE(owner&&owner->canonical);++canonical_lookups;return owner->canonical;}
}
@interface ArcWebContentsPDFController : NSObject
- (instancetype)initWithWebContentsPDFController:(const void*)contents;
@end
@implementation ArcWebContentsPDFController
- (instancetype)initWithWebContentsPDFController:(const void*)contents{(void)contents;return [super init];}
@end
@interface WebextrasPDFTestDelegate : NSObject <ArcPDFUI> @end
@implementation WebextrasPDFTestDelegate
- (void)webContents:(id)owner didFinishPDFRendering:(id)controller {
  REQUIRE(arc::CoreCloseStateIsBusy(active)&&!UISync()); REQUIRE(owner&&controller);++callbacks;
}
@end
class ArcPDFObserver {
 public:
  ArcPDFObserver(content::WebContents* contents,ArcWebContents* owner)
    :owner_(owner),close_state_(arc::CoreCloseStateForWebContents(owner)),contents_(contents){}
  content::WebContents* web_contents(){return contents_;}
  void PDFReady(uint64_t generation);
  void DeliverPDF();
  __weak ArcWebContents* owner_;
  arc::CoreCloseStateHandle close_state_;
  bool has_controller_=false,readiness_posted_=false;
  uint64_t generation_=0;
  content::WebContents* contents_;
  base::WeakPtrFactory<ArcPDFObserver> weak_factory_{this};
};
'''
TESTS = r'''
int main(){@autoreleasepool{
  content::WebContents physical; active=arc::CreateCoreCloseState();
  ArcWebContents* owner=[[ArcWebContents alloc]init];owner->canonical=active;
  WebextrasPDFTestDelegate* delegate=[[WebextrasPDFTestDelegate alloc]init];owner.uiDelegate=delegate;
  auto observer=std::make_unique<ArcPDFObserver>(&physical,owner);REQUIRE(canonical_lookups==1);
  // Stale receipt guard acquires nothing; valid readiness enqueues ONE UI
  // invocation. Duplicate receipt creates no extra task or observer trailer.
  observer->PDFReady(1);REQUIRE(tasks.empty()&&!arc::CoreCloseStateIsBusy(active));
  observer->PDFReady(0);observer->PDFReady(0);REQUIRE(tasks.size()==1&&arc::CoreCloseStateIsBusy(active));
  ++observer->generation_;Drain();
  REQUIRE(!arc::CoreCloseStateIsBusy(active)&&published==1&&callbacks==1);
  // A genuine accepted invocation keeps the exact captured state through setter
  // KVO/reentry and delegate return, even if the observer dies in the setter.
  observer=std::make_unique<ArcPDFObserver>(&physical,owner);observer->PDFReady(0);
  auto other=arc::CreateCoreCloseState();owner->canonical=other;
  onSetter=[&]{REQUIRE(arc::CoreCloseStateIsBusy(active));REQUIRE(!arc::CoreCloseStateIsBusy(other));
    observer.reset();owner.uiDelegate=nil;onSetter=nullptr;};
  Drain();REQUIRE(!arc::CoreCloseStateIsBusy(active)&&published==2&&callbacks==2);
  // Expired observer and nil owner are conditional delivery skips INSIDE the
  // accounting wrapper. Both still complete their accepted pending invocation.
  owner->canonical=active;observer=std::make_unique<ArcPDFObserver>(&physical,owner);
  observer->PDFReady(0);observer.reset();REQUIRE(arc::CoreCloseStateIsBusy(active));Drain();
  REQUIRE(!arc::CoreCloseStateIsBusy(active)&&published==2&&callbacks==2);
  observer=std::make_unique<ArcPDFObserver>(&physical,owner);observer->PDFReady(0);owner=nil;Drain();
  REQUIRE(!arc::CoreCloseStateIsBusy(active)&&published==2&&callbacks==2);observer.reset();
  // Native rejected/unrun policy has no destructor rollback.
  ArcWebContents* cancelledOwner=[[ArcWebContents alloc]init];auto cancelled=arc::CreateCoreCloseState();cancelledOwner->canonical=cancelled;
  observer=std::make_unique<ArcPDFObserver>(&physical,cancelledOwner);observer->PDFReady(0);
  tasks.clear();REQUIRE(arc::CoreCloseStateIsBusy(cancelled));observer.reset();
  std::puts("PASS placement-only actual CoreCloseState + PDFReady/DeliverPDF (native UI-registration eligibility NOT modeled): single UI enqueue, setter/reentry/delegate return, exact captured state, expired observer/nil owner, unrun-task pending; no WCO trailer");
}}
'''


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--stage', type=Path, required=True)
    ap.add_argument('--out', type=Path, required=True)
    args = ap.parse_args()
    spec = importlib.util.spec_from_file_location('held_media', ROOT / 'arc/browser/webextras_media_close_accounting_held_test.py')
    media = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(media)
    source = (args.stage / 'arc/api/arc_webextras_capture.mm').read_text()
    header = (args.stage / 'arc/browser/core_close_state.h').read_text()
    primitive = (args.stage / 'arc/browser/core_close_state.mm').read_text()
    declarations = header[header.index('enum class CoreCloseDelegateChannel'):header.index('// Call immediately at enqueue')]
    implementation = primitive[primitive.index('namespace arc {'):primitive.index('CoreCloseObserverBatch::CoreCloseObserverBatch')]
    bind = media.BIND.replace('std::apply([&](auto&...v){ f(std::move(v)...); },*args);', '''
    if constexpr(std::is_member_function_pointer_v<F>) {
      std::apply([&](auto& first,auto&...rest){if(auto* target=first.get())std::invoke(f,target,std::move(rest)...);},*args);
    } else { std::apply([&](auto&...v){ f(std::move(v)...); },*args); }''')
    bind = bind.replace('template<class T> struct WeakPtr { T* ptr=nullptr; void reset(){ptr=nullptr;} };', '''
template<class T> struct WeakPtr {T* ptr;std::weak_ptr<int> validity;T* get()const{return validity.expired()?nullptr:ptr;}};
template<class T> class WeakPtrFactory {
 T* ptr_;std::shared_ptr<int> validity_=std::make_shared<int>();
 public:explicit WeakPtrFactory(T* p):ptr_(p){} WeakPtr<T> GetWeakPtr(){return {ptr_,validity_};}
};''')
    p = source.index('@protocol ArcPDFUI')
    protocol = source[p:source.index('@end', p) + 4]
    p = source.index('void ArcPDFObserver::PDFReady(')
    bodies = source[p:source.rfind('\n}')]
    text = PRELUDE + media.CHECKS + bind + 'namespace arc {class CoreCloseState;using CoreCloseStateHandle=std::shared_ptr<CoreCloseState>;\n' + declarations + '\n}\n' + implementation + '\n}\n@class ArcWebContents;\n@class ArcWebContentsPDFController;\n' + protocol + '\n' + SCAFFOLD + bodies + TESTS
    args.out.mkdir(parents=True, exist_ok=True)
    (args.out / 'test.mm').write_text(text)
    with tempfile.TemporaryDirectory(prefix='webextras-pdf-accounting-test-') as tmp:
        cmd = ['xcrun', 'clang++', '-std=c++20', '-fobjc-arc', '-fno-exceptions', '-O2', '-Wall', '-Wextra', '-Werror', '-framework', 'Foundation', str(args.out.resolve() / 'test.mm'), '-o', str(Path(tmp) / 'test')]
        p = subprocess.run(cmd, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        output, code = p.stdout, p.returncode
        if not code:
            p = subprocess.run([str(Path(tmp) / 'test')], text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            output += p.stdout
            code = p.returncode
    (args.out / 'test-result.json').write_text(json.dumps({'source_sha256': hashlib.sha256(source.encode()).hexdigest(), 'primitive_sha256': hashlib.sha256(primitive.encode()).hexdigest(), 'header_sha256': hashlib.sha256(header.encode()).hexdigest(), 'extracted_test_sha256': hashlib.sha256(text.encode()).hexdigest(), 'compile_command': cmd, 'exit_code': code, 'output': output, 'native_ui_registration_eligibility_modelled': False, 'limits': 'PLACEMENT PROTOTYPE ONLY; mock observer construction/weak binding/tasks/check/sequence; actual primitive + PDFReady/DeliverPDF; no Chromium PDF emitter/runtime or public Close'}, indent=2) + '\n')
    print(output, end='')
    raise SystemExit(code)


if __name__ == '__main__':
    main()
