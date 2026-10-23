#!/usr/bin/env python3
"""Actual Source retirement + observer Info forwarder, with a mock Mojo receipt.

The mock's receiver/binding observations do not establish CORE's public binding
or recursive replacement semantics. --expect-old-failure is a bounded control.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
OBSERVER = r'''
class ArcMediaObserver;
static ArcMediaObserver* receiver=nullptr;
class ArcMediaObserver {
 public:
  ArcMediaObserver(content::WebContents* c,content::MediaSession*,std::weak_ptr<WebextrasMediaSource> source)
      :source_(source),contents(c){++observers;receiver=this;}
  ~ArcMediaObserver(){--observers;if(receiver==this)receiver=nullptr;}
  void Detach(){++detachments;attached=false;contents=nullptr;}
  void MediaSessionInfoChanged(media_session::mojom::MediaSessionInfoPtr info);
  void ReceiveInfo(media_session::mojom::MediaSessionInfoPtr info){
    if(attached)MediaSessionInfoChanged(std::move(info));
  }
  std::weak_ptr<WebextrasMediaSource> source_;
  content::WebContents* contents;
  bool attached=true;
};
'''
TESTS = r'''
int main(){@autoreleasepool{
  events=[NSMutableArray array]; content::WebContents physical;
  auto* physical_identity=&physical;
  ArcWebContents* owner=[[ArcWebContents alloc]init];
  auto source=std::make_shared<WebextrasMediaSource>(owner);source->AttachInitial(&physical);
  ArcWebContentsMediaSession* service=Service(source,owner);WebextrasTestDelegate* a=Delegate(@"A");[service setDelegate:a];
  source->Info(Info());Drain();Expect(@[@"A:state:3",@"A:camera:0",@"A:mic:0"]);
  source->PiPCreated();Expect(@[@"A:created:0"]);
  __block bool sawReceiver=false,sawBinding=false;
  @autoreleasepool{
    __weak WebextrasTestDelegate* weakA=a;
    a.onCallback=^{
      weakA.onCallback=nil;
      sawReceiver=receiver&&receiver->attached&&detachments==0;
      sawBinding=receiver&&receiver->contents==physical_identity&&binding_clears==0;
      REQUIRE(service->_pipController==nil);
      // This is an observer Info receipt with real Source cache/dispatch logic,
      // not a synthetic delegate callback. A nested loop delivers it while the
      // physical receiver is still attached, then drains its genuine tasks.
      PostWebextrasMediaTask(base::BindOnce([]{
        if(receiver)receiver->ReceiveInfo(Info(1,2,1));
      }));
      Drain();
      source->VolumeChanged(.5);  // Accepted before the physical tail, left queued.
    };
    source->RetirePhysical();
    if(!sawReceiver||!sawBinding){
      std::puts("EXPECTED OLD FAILURE: PiP app callout saw detached receiver or cleared Source binding");return 71;
    }
    REQUIRE(detachments==1&&binding_clears==1&&receiver&&!receiver->attached&&receiver->contents==nullptr);
    Expect(@[@"A:destroyed:0",@"A:state:1",@"A:camera:2",@"A:mic:1"]);
    REQUIRE(tasks.size()==1);Drain();Expect(@[@"A:volume:50"]);
    // The comparison cache retains the receipt processed during the callout.
    source->Info(Info(1,2,1));REQUIRE(tasks.empty());
    receiver->ReceiveInfo(Info(2,1,2));REQUIRE(tasks.empty());

  }
  service=nil;source.reset();REQUIRE(observers==0&&receiver==nullptr&&tasks.empty());
  std::puts("PASS actual retirement/Info observer path: PiP before detach/Source clear, nested receipt/cache/tasks, postcallback detach; CORE public-binding/rebind not modeled");
}}
'''


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--source', type=Path, required=True)
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('--expect-old-failure', action='store_true')
    ap.add_argument('--regression-suite', action='store_true')
    args = ap.parse_args()
    spec = importlib.util.spec_from_file_location('delivery_base', ROOT / 'arc/browser/webextras_media_delivery_test.py')
    base = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(base)
    source = args.source.read_text()
    prelude = base.PRELUDE.replace('namespace base {', 'static int binding_clears=0;\nnamespace base {', 1).replace('void reset(){ptr=nullptr;}', 'void reset(){++binding_clears;ptr=nullptr;}')
    a = prelude.index('class ArcMediaObserver {')
    b = prelude.index('\n};', a) + 3
    prelude = prelude[:a] + OBSERVER + prelude[b:]
    prelude = prelude.replace('static NSString* ArcNSString', '[[maybe_unused]] static NSString* ArcNSString')
    protocol = source[source.index('@protocol ArcMediaDelegate'):source.index('@end', source.index('@protocol ArcMediaDelegate')) + 4]
    helper = source[source.index('// BEGIN WEBEXTRAS_MEDIA_DELIVERY_HELPER'):source.index('// END WEBEXTRAS_MEDIA_DELIVERY_HELPER')]
    a = source.index('class WebextrasMediaSource final')
    cls = source[a:source.index('\nBOOL ArcMediaFailure', a)]
    a = source.index('- (void)setDelegate:(id<ArcMediaDelegate>)delegate {')
    method = source[a:source.index('- (NSIndexSet*)getMediaContentTypes:', a)]
    a = source.index('- (ArcMediaPosition*)position {')
    getter = source[a:source.index('\n#pragma clang diagnostic pop', a)]
    a = source.index('void WebextrasMediaSource::PiPCreated()')
    pip = source[a:source.index('ArcMediaObserver::ArcMediaObserver', a)]
    a = source.index('void ArcMediaObserver::MediaSessionInfoChanged(')
    forwarder = source[a:source.index('void ArcMediaObserver::MediaSessionActionsChanged(', a)]
    overlay = ''
    tests = TESTS
    if args.regression_suite:
        methods = []
        for signature in ['void Close()', 'void ShowInactive()', 'void Hide()', 'void UpdateNaturalSize(', 'void SetPlaybackState(', 'void SetMediaPosition(', 'void SetSourceTitle(', 'void SetFaviconImages(', 'void Feature(']:
            start = source.index('  ' + signature, source.index('class ArcVideoOverlay final'))
            opening = source.index('{', start)
            depth, end = 1, opening + 1
            while depth:
                depth += (source[end] == '{') - (source[end] == '}')
                end += 1
            methods.append(source[start:end].replace(' override', ''))
        overlay = ('class WebextrasTestOverlay { public: '\
                   'explicit WebextrasTestOverlay(std::shared_ptr<WebextrasMediaSource> s):source_(s) {} '\
                   'using PlaybackState=uint64_t; std::shared_ptr<WebextrasMediaSource> source_; '\
                   'bool created_=false,visible_=false; struct C { void Unsuspend(){} } c; C* compositor_=&c; '\
                   'struct W { bool GetWeakPtr(){return true;} } weak_factory_;\n' + '\n'.join(methods) + '\n};\n')
        old = 'a.onCallback=^{ retireDelegate.onCallback=nil; REQUIRE(detachments==1); };'
        assert base.TESTS.count(old) == 1
        tests = base.TESTS.replace(old, 'a.onCallback=^{ retireDelegate.onCallback=nil; REQUIRE(detachments==0); };')
    service = base.SERVICE.replace('SETDELEGATE', method).replace('GETPOSITION', getter).replace('PIPDEFINITIONS', pip).replace('OVERLAY', overlay)
    text = prelude + protocol + '\n' + helper + '\n' + cls + '\n' + service + '\n' + forwarder + tests
    args.out.mkdir(parents=True, exist_ok=True)
    (args.out / 'test.mm').write_text(text)
    with tempfile.TemporaryDirectory(prefix='webextras-media-retire-test-') as tmp:
        cmd = ['xcrun', 'clang++', '-std=c++20', '-fobjc-arc', '-fno-exceptions', '-O2', '-Wall', '-Wextra', '-Werror', '-framework', 'Foundation', '-framework', 'CoreGraphics', str(args.out.resolve() / 'test.mm'), '-o', str(Path(tmp) / 'test')]
        p = subprocess.run(cmd, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        output, compile_code = p.stdout, p.returncode
        run_code = None
        if not compile_code:
            p = subprocess.run([str(Path(tmp) / 'test')], text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            output += p.stdout
            run_code = p.returncode
    result = {'source_sha256': hashlib.sha256(source.encode()).hexdigest(), 'extracted_test_sha256': hashlib.sha256(text.encode()).hexdigest(), 'compile_command': cmd, 'compile_exit_code': compile_code, 'run_exit_code': run_code, 'expected_old_failure': args.expect_old_failure, 'derived_full_delivery_suite': args.regression_suite, 'base_test_sha256': hashlib.sha256((ROOT/'arc/browser/webextras_media_delivery_test.py').read_bytes()).hexdigest(), 'output': output, 'limits': 'Foundation mock Mojo receipt/receiver/native WeakPtr; actual Source retirement/cache/delivery and Info observer forwarder; no CORE public binding or recursive attachment'}
    (args.out / 'test-result.json').write_text(json.dumps(result, indent=2) + '\n')
    print(output, end='')
    if args.expect_old_failure:
        raise SystemExit(0 if compile_code == 0 and run_code == 71 else 1)
    raise SystemExit(compile_code or run_code or 0)


if __name__ == '__main__':
    main()
