#!/usr/bin/env python3
"""Compile/execute actual proposed delivery/cache/Source code with mock tasks.

This is an owned standalone Foundation test, not a Chromium runtime test.
Default reconstruction applies only the owned proposal to private temporary files.
"""
from pathlib import Path
import argparse
import hashlib
import json
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
PRELUDE = r'''
#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#include <array>
#include <deque>
#include <functional>
#include <memory>
#include <optional>
#include <vector>
#include <tuple>
#include <cstdio>
#include <cstdlib>
namespace base {
using OnceClosure = std::function<void()>;
template<class F, class... A> OnceClosure BindOnce(F f, A... a) {
  return [f, args=std::make_tuple(std::move(a)...)]() mutable {
    std::apply([&](auto&... v) { f(std::move(v)...); },args);
  };
}
template<class T> struct WeakPtr { T* ptr=nullptr; void reset(){ptr=nullptr;} };
}
static std::deque<base::OnceClosure> tasks;
static void PostWebextrasMediaTask(base::OnceClosure f) { tasks.push_back(std::move(f)); }
static void DrainOne() { auto f=std::move(tasks.front()); tasks.pop_front(); f(); }
static void Drain() { while(!tasks.empty()) DrainOne(); }
static int observers=0, detachments=0, dto_allocations=0;
static void (^onControllerInit)(void);
static void (^onImageInit)(void);
static void (^onPositionInit)(void);
static void (^onTitleConvert)(void);
static int image_allocations=0, title_conversions=0;
static NSMutableArray<NSString*>* events;
#define REQUIRE(x) do { if(!(x)) { std::fprintf(stderr,"failed line %d: %s\n",__LINE__,#x); std::abort(); } } while(0)
namespace content {
struct WebContents { base::WeakPtr<WebContents> GetWeakPtr() { return {this}; } };
struct MediaSession { static MediaSession* Get(WebContents*) { return nullptr; } };
}
namespace media_session {
struct MediaPosition { std::array<uint64_t,5> words; };
struct MediaImage { uint64_t token=0; };
namespace mojom {
using MediaSessionAction=uint32_t;
struct MediaSessionInfo { uint64_t state,camera_state,microphone_state; };
using MediaSessionInfoPtr=std::unique_ptr<MediaSessionInfo>;
}
}
@interface ArcWebContents : NSObject @end
@implementation ArcWebContents @end
@interface ArcMediaPosition : NSObject {
 @public media_session::MediaPosition copied;
}
- (instancetype)initWithMediaPosition:(const void*)p;
@end
@implementation ArcMediaPosition
- (instancetype)initWithMediaPosition:(const void*)p {
  if((self=[super init])) {
    void (^callback)(void)=onPositionInit; if(callback) callback();
    copied=*static_cast<const media_session::MediaPosition*>(p); ++dto_allocations;
  }
  return self;
}
@end
@interface ArcWebContentsMediaSessionPiPController : NSObject
- (instancetype)initWithArcWebContents:(ArcWebContents*)owner;
@end
@implementation ArcWebContentsMediaSessionPiPController
- (instancetype)initWithArcWebContents:(ArcWebContents*)owner {
  (void)owner; void (^callback)(void)=onControllerInit; if(callback) callback(); return [super init];
}
@end
@interface ArcMediaImage : NSObject {
 @public uint64_t token;
}
- (instancetype)initWithChromiumMediaImage:(const media_session::MediaImage&)image;
@end
@implementation ArcMediaImage
- (instancetype)initWithChromiumMediaImage:(const media_session::MediaImage&)image {
  void (^callback)(void)=onImageInit; if(callback) callback();
  if((self=[super init])) { token=image.token; ++image_allocations; } return self;
}
@end
static NSString* ArcNSString(const std::u16string& s) {
  void (^callback)(void)=onTitleConvert; if(callback) callback(); ++title_conversions;
  return [[NSString alloc] initWithCharacters:(const unichar*)s.data() length:s.size()]; }
namespace gfx { struct Size { int width() const { return 4; } int height() const { return 5; } }; }
@class ArcWebContentsMediaSession;
class WebextrasMediaSource;
class ArcMediaObserver {
 public:
  ArcMediaObserver(content::WebContents*, content::MediaSession*, std::weak_ptr<WebextrasMediaSource>) { ++observers; }
  ~ArcMediaObserver() { --observers; }
  void Detach() { ++detachments; }
};
'''

SERVICE = r'''
@interface ArcWebContentsMediaSession : NSObject {
 @public
  std::shared_ptr<WebextrasMediaDelegateBridge> _bridge;
  std::shared_ptr<WebextrasMediaSource> _source;
  __weak ArcWebContents* _owner;
  ArcWebContentsMediaSessionPiPController* _pipController;
}
- (void)setDelegate:(id<ArcMediaDelegate>)delegate;
- (ArcMediaPosition*)position;
- (void)webextrasCachePiPController:(ArcWebContentsMediaSessionPiPController*)controller;
@end
@implementation ArcWebContentsMediaSession
GETPOSITION
SETDELEGATE
- (void)webextrasCachePiPController:(ArcWebContentsMediaSessionPiPController*)controller { _pipController=controller; }
@end
PIPDEFINITIONS
OVERLAY
@interface WebextrasTestDelegate : NSObject <ArcMediaDelegate>
@property(nonatomic, copy) NSString* name;
@property(nonatomic, copy) void (^onCallback)(void);
@property(nonatomic) BOOL sawNilOwner;
@property(nonatomic, strong) NSIndexSet* actions;
@property(nonatomic, strong) ArcMediaPosition* position;
@property(nonatomic, strong) ArcMediaPosition* pipPosition;
@property(nonatomic, copy) NSString* title;
@property(nonatomic, strong) NSArray* images;
@end
@implementation WebextrasTestDelegate
- (void)record:(NSString*)kind owner:(id)owner value:(uint64_t)v {
  _sawNilOwner=!owner;
  [events addObject:[NSString stringWithFormat:@"%@:%@:%llu",_name,kind,(unsigned long long)v]];
  void (^callback)(void)=_onCallback; if(callback) callback();
}
- (void)webContents:(id)o didChangeMediaSessionState:(uint64_t)v { [self record:@"state" owner:o value:v]; }
- (void)webContents:(id)o didChangeCameraState:(uint64_t)v { [self record:@"camera" owner:o value:v]; }
- (void)webContents:(id)o didChangeMicrophoneState:(uint64_t)v { [self record:@"mic" owner:o value:v]; }
- (void)webContents:(id)o didChangeAvailableMediaActions:(NSIndexSet*)v { self.actions=v; [self record:@"actions" owner:o value:v.count]; }
- (void)webContents:(id)o didChangeMediaPosition:(ArcMediaPosition*)v { self.position=v; [self record:@"position" owner:o value:v->copied.words[3]]; }
- (void)webContents:(id)o didChangeVolume:(double)v { [self record:@"volume" owner:o value:(uint64_t)(v*100)]; }
- (void)webContents:(id)o pipControllerCreated:(id)c { (void)c; [self record:@"created" owner:o value:0]; }
- (void)webContentsPiPControllerDestroyed:(id)o { [self record:@"destroyed" owner:o value:0]; }
- (void)webContents:(id)o pipVideoResolutionChanged:(CGSize)s { (void)s; [self record:@"size" owner:o value:0]; }
- (void)webContentsShowPiP:(id)o { [self record:@"show" owner:o value:0]; }
- (void)webContentsHidePiP:(id)o { [self record:@"hide" owner:o value:0]; }
- (void)webContentsClosePiP:(id)o { [self record:@"close" owner:o value:0]; }
- (void)webContents:(id)o pipPlaybackStateChanged:(uint64_t)v { [self record:@"playback" owner:o value:v]; }
- (void)webContents:(id)o pipFeatureStateChanged:(uint64_t)v turnedOn:(BOOL)b { (void)b; [self record:@"feature" owner:o value:v]; }
- (void)webContents:(id)o pipMediaPositionChanged:(id)p { self.pipPosition=p; [self record:@"pipPosition" owner:o value:0]; }
- (void)webContents:(id)o pipSourceTitleChanged:(NSString*)s { self.title=s; [self record:@"title" owner:o value:0]; }
- (void)webContents:(id)o pipFaviconImagesChanged:(NSArray*)a { self.images=a; [self record:@"icons" owner:o value:0]; }
@end
static WebextrasTestDelegate* Delegate(NSString* name) {
  WebextrasTestDelegate* d=[[WebextrasTestDelegate alloc] init]; d.name=name; return d;
}
static ArcWebContentsMediaSession* Service(std::shared_ptr<WebextrasMediaSource> source, ArcWebContents* owner) __attribute__((ns_returns_retained)) {
  ArcWebContentsMediaSession* s=[[ArcWebContentsMediaSession alloc] init];
  s->_source=source; s->_owner=owner; source->service=s; return s;
}
static auto Info(uint64_t s=3,uint64_t c=0,uint64_t m=0) {
  return std::make_unique<media_session::mojom::MediaSessionInfo>(media_session::mojom::MediaSessionInfo{s,c,m});
}
static void Expect(NSArray* expected) { REQUIRE([events isEqualToArray:expected]); [events removeAllObjects]; }
'''

TESTS = r'''
int main() { @autoreleasepool {
  events=[NSMutableArray array];
  content::WebContents physical;
  ArcWebContents* owner=[[ArcWebContents alloc] init];
  WebextrasTestDelegate* a=Delegate(@"A"); WebextrasTestDelegate* b=Delegate(@"B");
  auto source=std::make_shared<WebextrasMediaSource>(owner); source->AttachInitial(&physical);
  ArcWebContentsMediaSession* service=Service(source,owner); [service setDelegate:a];
  // Unset first info includes 3/0/0, receipt order, never inline. Equality suppresses later info.
  source->Info(Info()); REQUIRE(events.count==0 && tasks.size()==3); Drain();
  Expect(@[@"A:state:3",@"A:camera:0",@"A:mic:0"]);
  source->Info(Info()); REQUIRE(tasks.empty());
  source->Info(Info(1,2,3)); source->Info(Info(4,5,6)); Drain();
  Expect(@[@"A:state:1",@"A:camera:2",@"A:mic:3",@"A:state:4",@"A:camera:5",@"A:mic:6"]);
  // Actions snapshot/filter and allocation only after a live registration is locked at delivery.
  std::vector<media_session::mojom::MediaSessionAction> actions={2,20,21,30};
  source->Actions(actions); actions={7}; REQUIRE(!a.actions); Drain();
  REQUIRE([a.actions containsIndex:2] && [a.actions containsIndex:20] && a.actions.count==2);
  Expect(@[@"A:actions:2"]);
  std::optional<media_session::MediaPosition> position=media_session::MediaPosition{{1,2,3,4,5}};
  source->Position(position); position->words={9,9,9,9,9}; REQUIRE(dto_allocations==0);
  source->Position(std::nullopt); REQUIRE(tasks.size()==1); Drain(); REQUIRE(dto_allocations==1);
  REQUIRE((a.position->copied.words==std::array<uint64_t,5>{1,2,3,4,5})); Expect(@[@"A:position:4"]);
  // Getter follows the latest native RECEIPT, even while an older callback is queued.
  position=media_session::MediaPosition{{6,7,8,9,10}}; source->Position(position);
  ArcMediaPosition* newer=[service position]; ArcMediaPosition* newerAgain=[service position];
  REQUIRE(newer!=newerAgain && newer->copied.words[3]==9 && a.position->copied.words[3]==4);
  source->Position(std::nullopt); REQUIRE([service position]==nil); Drain();
  REQUIRE(newer->copied.words[3]==9 && newerAgain->copied.words[3]==9);
  Expect(@[@"A:position:9"]); const int allocatedBeforeCancel=dto_allocations;
  // Non-nil replacement preserves bridge identity and current weak delegate at delivery.
  auto* original=service->_bridge.get(); source->VolumeChanged(.25); [service setDelegate:b];
  REQUIRE(service->_bridge.get()==original); Drain(); Expect(@[@"B:volume:25"]);
  // Nil cancels an expired selected bridge; DTO is not allocated for it.
  source->Position(position); [service setDelegate:nil]; Drain(); REQUIRE(dto_allocations==allocatedBeforeCancel); Expect(@[]);
  [service setDelegate:a]; source->VolumeChanged(.30); [service setDelegate:nil]; [service setDelegate:b];
  Drain(); Expect(@[]); source->VolumeChanged(.45); Drain(); Expect(@[@"B:volume:45"]);
  // Registration changes never reset observer or logical cache; no replay on re-add.
  const int before=observers; [service setDelegate:nil]; source->Info(Info(9,8,7)); Drain(); Expect(@[]);
  [service setDelegate:a]; source->Info(Info(9,8,7)); REQUIRE(tasks.empty() && observers==before);
  @autoreleasepool {
  // Sync keeps OLD bridge alive through app return. Nested drain can call A after nil -> B.
  source->VolumeChanged(.60); auto* old=service->_bridge.get();
  __weak WebextrasTestDelegate* weakA=a;
  a.onCallback=^{
    weakA.onCallback=nil; [service setDelegate:nil]; [service setDelegate:b];
    REQUIRE(service->_bridge.get()!=old); source->VolumeChanged(.70); Drain();
  };
  WebextrasTestOverlay overlay(source); overlay.ShowInactive();
  Expect(@[@"A:show:0",@"A:volume:60",@"B:volume:70"]); REQUIRE(tasks.empty());
  // Nilling without replacement inside Sync still permits old registered receipts while leased.
  [service setDelegate:a];
  a.onCallback=^{ weakA.onCallback=nil; [service setDelegate:nil]; source->VolumeChanged(.80); Drain(); };
  overlay.Hide();
  overlay.source_.reset();
  Expect(@[@"A:hide:0",@"A:volume:80"]); source->VolumeChanged(.81); Drain(); Expect(@[]);
  }
  // The weak-loaded app delegate is leased through return, even when the last
  // external app reference disappears before a nested old-registration drain.
  __weak WebextrasTestDelegate* expiredLease;
  @autoreleasepool {
    __block WebextrasTestDelegate* sole=Delegate(@"lease"); expiredLease=sole;
    [service setDelegate:sole];
    sole.onCallback=^{
      expiredLease.onCallback=nil; sole=nil; [service setDelegate:nil];
      REQUIRE(expiredLease!=nil); source->VolumeChanged(.88); Drain();
    };
    WebextrasTestOverlay leased(source); leased.ShowInactive(); leased.source_.reset();
    Expect(@[@"lease:show:0",@"lease:volume:88"]);
  }
  REQUIRE(expiredLease==nil);
  // Creation selects/leases its bridge BEFORE DTO construction. Reentrant
  // allocation cannot migrate it to a new registration, and old queued work
  // stays eligible while that selected bridge is leased.
  @autoreleasepool {
    [service setDelegate:a]; source->VolumeChanged(.87);
    onControllerInit=^{
      onControllerInit=nil; [service setDelegate:nil]; [service setDelegate:b];
      source->VolumeChanged(.89); Drain();
    };
    source->PiPCreated();
    Expect(@[@"A:volume:87",@"B:volume:89",@"A:created:0"]);
    // Favicon bridge weak-loads precede image DTO construction. Updating the
    // same bridge during construction affects only the later invocation.
    [service setDelegate:a];
    onImageInit=^{ onImageInit=nil; [service setDelegate:b]; };
    WebextrasTestOverlay images(source); images.SetFaviconImages({media_session::MediaImage{}});
    source->VolumeChanged(.86); Drain(); images.source_.reset();
    Expect(@[@"A:icons:0",@"B:volume:86"]);
  }
  // Synchronous native inputs are copied before a reentrant conversion can
  // mutate/free the controller's referenced value, string or vector storage.
  @autoreleasepool {
    [service setDelegate:a]; WebextrasTestOverlay snapshots(source);
    __block media_session::MediaPosition borrowedPosition{{11,12,13,14,15}};
    onPositionInit=^{ onPositionInit=nil; borrowedPosition.words={91,92,93,94,95}; };
    snapshots.SetMediaPosition(borrowedPosition);
    REQUIRE((a.pipPosition->copied.words==std::array<uint64_t,5>{11,12,13,14,15}));
    __block std::u16string borrowedTitle=u"original-title";
    onTitleConvert=^{ onTitleConvert=nil; borrowedTitle=std::u16string(4096,u'x'); };
    snapshots.SetSourceTitle(borrowedTitle); REQUIRE([a.title isEqualToString:@"original-title"]);
    __block std::vector<media_session::MediaImage> borrowedImages={{41},{42}};
    onImageInit=^{ onImageInit=nil; borrowedImages.clear(); borrowedImages.shrink_to_fit(); borrowedImages.push_back({99}); };
    snapshots.SetFaviconImages(borrowedImages);
    REQUIRE(a.images.count==2 && ((ArcMediaImage*)a.images[0])->token==41 && ((ArcMediaImage*)a.images[1])->token==42);
    Expect(@[@"A:pipPosition:0",@"A:title:0",@"A:icons:0"]); snapshots.source_.reset();
  }
  // Weak app delegate expiration, no extra owner prerequisite.
  @autoreleasepool { WebextrasTestDelegate* ephemeral=Delegate(@"expired"); [service setDelegate:ephemeral]; }
  source->VolumeChanged(.90); Drain(); Expect(@[]);
  // A live registration with expired app delegate still allocates PiP DTOs;
  // absence of the registration itself skips that bridge invocation entirely.
  WebextrasTestOverlay expired(source);
  const int beforePiPDto=dto_allocations, beforeTitle=title_conversions, beforeImage=image_allocations;
  expired.SetMediaPosition(*position); expired.SetSourceTitle(u"expired"); expired.SetFaviconImages({{17}});
  REQUIRE(dto_allocations==beforePiPDto+1 && title_conversions==beforeTitle+1 && image_allocations==beforeImage+1);
  [service setDelegate:nil]; expired.SetMediaPosition(*position); expired.SetSourceTitle(u"missing"); expired.SetFaviconImages({{18}});
  REQUIRE(dto_allocations==beforePiPDto+1 && title_conversions==beforeTitle+1 && image_allocations==beforeImage+1);
  expired.source_.reset(); Expect(@[]);
  [service setDelegate:a]; owner=nil; source->VolumeChanged(.91); Drain(); REQUIRE(a.sawNilOwner);
  Expect(@[@"A:volume:91"]);
  // Functional Source survives service + physical retirement through accepted payload.
  // An independently held bridge models native shared bridge exposure, without retaining app objects.
  auto selected=service->_bridge; std::weak_ptr<WebextrasMediaSource> weak=source;
  __weak ArcWebContentsMediaSession* weakService=service;
  __weak WebextrasTestDelegate* retireDelegate=a;
  a.onCallback=^{ retireDelegate.onCallback=nil; REQUIRE(detachments==1); };
  source->VolumeChanged(.92); source->RetirePhysical(); REQUIRE(detachments==1);
  Expect(@[@"A:destroyed:0"]); service=nil; source.reset(); REQUIRE(weakService==nil && !weak.expired() && observers==before);
  Drain(); Expect(@[@"A:volume:92"]); REQUIRE(weak.expired() && observers==before-1); selected.reset();
  // Initial receipts without a delegate update caches, and re-add emits nothing synthetic.
  auto early=std::make_shared<WebextrasMediaSource>(nil); early->AttachInitial(&physical);
  early->Info(Info()); early->Position(position); Drain();
  ArcWebContentsMediaSession* late=Service(early,nil); REQUIRE([late position]->copied.words[3]==9); [late setDelegate:a];
  early->Info(Info()); REQUIRE(tasks.empty()); early->Info(Info(2)); Drain(); Expect(@[@"A:state:2"]);
  // PiP created/destroyed are synchronous and maintain only the service cache. Destroy latch is logical.
  early->Position(position); Drain(); Expect(@[@"A:position:9"]);
  ArcMediaPosition* held=[late position]; early->RetirePhysical(); Expect(@[@"A:destroyed:0"]);
  REQUIRE([late position]!=held && [late position]->copied.words==held->copied.words);
  early->PiPCreated(); REQUIRE(late->_pipController!=nil && tasks.empty());
  early->PiPDestroyed(); REQUIRE(late->_pipController==nil); early->PiPDestroyed();
  Expect(@[@"A:created:0",@"A:destroyed:0"]);
  WebextrasTestOverlay all(early);
  all.SetPlaybackState(2); all.UpdateNaturalSize(gfx::Size{}); all.ShowInactive(); all.Hide(); all.Close();
  all.Feature(4,true); all.SetMediaPosition(media_session::MediaPosition{{1,2,3,4,5}});
  all.SetSourceTitle(u"title"); all.SetFaviconImages({media_session::MediaImage{}}); early->PiPDestroyed();
  REQUIRE(tasks.empty());
  Expect(@[@"A:created:0",@"A:playback:2",@"A:size:0",@"A:show:0",@"A:hide:0",@"A:close:0",@"A:feature:4",@"A:pipPosition:0",@"A:title:0",@"A:icons:0",@"A:destroyed:0"]);
  all.source_.reset();
  early->PiPCreated(); Expect(@[@"A:created:0"]);
  __weak ArcWebContentsMediaSessionPiPController* weakController=late->_pipController;
  late=nil; REQUIRE(weakController==nil); early.reset(); REQUIRE(observers==0 && tasks.empty());
  // Even uncounted/no-registration native work retains its logical payload.
  auto unregistered=std::make_shared<WebextrasMediaSource>(nil); unregistered->AttachInitial(&physical);
  unregistered->Info(Info()); unregistered->Position(position); unregistered->Actions({1});
  REQUIRE(tasks.size()==5); std::weak_ptr<WebextrasMediaSource> weakUnregistered=unregistered; unregistered.reset();
  REQUIRE(!weakUnregistered.expired() && observers==1);
  for(int i=0;i<4;++i) { DrainOne(); REQUIRE(!weakUnregistered.expired()); }
  DrainOne(); REQUIRE(weakUnregistered.expired() && observers==0 && tasks.empty()); Expect(@[]);
  std::puts("PASS actual proposed delivery/cache/Source: async order, payload copies, late DTOs, bridge transitions, weak expiry/nil owner, functional lifetime/cache/getter snapshots, nested sync, all 11 PiP selectors/latch");
} }
'''


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--source', type=Path)
    parser.add_argument('--out', type=Path)
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='webextras-media-delivery-test-') as tmp:
        work = Path(tmp)
        if args.source:
            source = args.source.read_text()
        else:
            path = work / 'arc/api/arc_webextras_media.mm'
            path.parent.mkdir(parents=True)
            path.write_bytes((ROOT / 'arc/api/arc_webextras_media.mm').read_bytes())
            subprocess.run(['git', 'apply', '--check', str(ROOT / 'arc/browser/webextras_media_delivery.patch')], cwd=work, check=True)
            subprocess.run(['git', 'apply', str(ROOT / 'arc/browser/webextras_media_delivery.patch')], cwd=work, check=True)
            source = path.read_text()
        protocol = source[source.index('@protocol ArcMediaDelegate'):source.index('@end', source.index('@protocol ArcMediaDelegate')) + 4]
        helper = source[source.index('// BEGIN WEBEXTRAS_MEDIA_DELIVERY_HELPER'):source.index('// END WEBEXTRAS_MEDIA_DELIVERY_HELPER')]
        start = source.index('class WebextrasMediaSource final')
        cls = source[start:source.index('\nBOOL ArcMediaFailure', start)]
        start = source.index('- (void)setDelegate:(id<ArcMediaDelegate>)delegate {')
        method = source[start:source.index('- (NSIndexSet*)getMediaContentTypes:', start)]
        start = source.index('void WebextrasMediaSource::PiPCreated()')
        pip = source[start:source.index('ArcMediaObserver::ArcMediaObserver', start)]
        methods = []
        for signature in ['void Close()', 'void ShowInactive()', 'void Hide()', 'void UpdateNaturalSize(', 'void SetPlaybackState(', 'void SetMediaPosition(', 'void SetSourceTitle(', 'void SetFaviconImages(', 'void Feature(']:
            start = source.index('  ' + signature, source.index('class ArcVideoOverlay final'))
            opening = source.index('{', start)
            depth, end = 1, opening + 1
            while depth:
                depth += (source[end] == '{') - (source[end] == '}')
                end += 1
            methods.append(source[start:end].replace(' override', ''))
        overlay = ('class WebextrasTestOverlay { public: '                   'explicit WebextrasTestOverlay(std::shared_ptr<WebextrasMediaSource> s):source_(s) {} '                   'using PlaybackState=uint64_t; std::shared_ptr<WebextrasMediaSource> source_; '                   'bool created_=false,visible_=false; struct C { void Unsuspend(){} } c; C* compositor_=&c; '                   'struct W { bool GetWeakPtr(){return true;} } weak_factory_;\n' + '\n'.join(methods) + '\n};\n')
        start = source.index('- (ArcMediaPosition*)position {')
        getter = source[start:source.index('\n#pragma clang diagnostic pop', start)]
        test = PRELUDE + protocol + '\n' + helper + '\n' + cls + '\n' + SERVICE.replace('SETDELEGATE', method).replace('GETPOSITION', getter).replace('PIPDEFINITIONS', pip).replace('OVERLAY', overlay) + TESTS
        (work / 'test.mm').write_text(test)
        cmd = ['xcrun', 'clang++', '-std=c++20', '-fobjc-arc', '-O2', '-Wall', '-Wextra', '-Werror', '-framework', 'Foundation', '-framework', 'CoreGraphics', str(work / 'test.mm'), '-o', str(work / 'test')]
        compile_result = subprocess.run(cmd, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        output = compile_result.stdout
        result = compile_result.returncode
        if result == 0:
            run = subprocess.run([str(work / 'test')], text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            output += run.stdout
            result = run.returncode
        manifest = {'source_sha256': hashlib.sha256(source.encode()).hexdigest(), 'extracted_test_sha256': hashlib.sha256(test.encode()).hexdigest(), 'compile_command': cmd, 'exit_code': result, 'output': output, 'limits': 'Foundation mock tasks/observer/native value types; no Chromium Mojo/runtime/physical rebind integration test'}
        if args.out:
            args.out.mkdir(parents=True, exist_ok=True)
            (args.out / 'test.mm').write_text(test)
            (args.out / 'test-result.json').write_text(json.dumps(manifest, indent=2) + '\n')
        print(output, end='')
        raise SystemExit(result)


if __name__ == '__main__':
    main()
