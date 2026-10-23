#!/usr/bin/env python3
"""Run the actual specific PiPDestroyed producer with actual Root/close state.

Reuses frozen initial candidate scaffolding. Access control is exposed ONLY in
extracted Source; a counter observes actual primitive EnterSync entry. No body
is replaced, and no generic PiP/lifecycle behavior is modeled.
"""
from pathlib import Path
import argparse,hashlib,importlib.util,inspect,json,sys
ROOT=Path(__file__).resolve().parents[2]
TESTS=r'''
int main(){@autoreleasepool{
 events=[NSMutableArray array];active=arc::CreateCoreCloseState();content::WebContents physical;
 auto d=Delegate(@"A");
 // Retained service/peer plus dead Root: do not mutate peer cache/latch or enter
 // any accounting scope. Native strong expiry is separate from later guards.
 auto root=NewRoot(physical);auto weak=arc::CoreLogicalWebContentsWeakHandle(root);
 auto source=std::static_pointer_cast<WebextrasMediaSource>(root->GetMediaPeer());
 root->GetMediaPeer()->AttachToWebContents(physical.GetWeakPtr());
 ArcWebContents* owner=[[ArcWebContents alloc]init];arc::AssociateWebextrasMediaFacade(owner,root);
 ArcWebContentsMediaSession* service=arc::CreateWebextrasMediaService(owner);[service setDelegate:d];
 source->PiPCreated();Expect(@[@"A:created:0"]);
 ArcWebContentsMediaSessionPiPController* cached=service->_pipController;REQUIRE(cached&&!source->pip_destroyed_);
 root.reset();REQUIRE(weak.expired()&&sibling_alive==0);
 const int scopes=arc::test_sync_entries;source->PiPDestroyed();
 REQUIRE(service->_pipController==cached&&!source->pip_destroyed_&&events.count==0);
 REQUIRE(arc::test_sync_entries==scopes&&!arc::CoreCloseStateIsBusy(active)&&tasks.empty());
 source.reset();service=nil;owner=nil;cached=nil;REQUIRE(observers==0);
 // Live Root: cache clears before callback, actual old observation/binding is
 // still intact. Only other Root owner is dropped DURING callback.
 __block auto live=NewRoot(physical);auto weakLive=arc::CoreLogicalWebContentsWeakHandle(live);
 auto media=std::static_pointer_cast<WebextrasMediaSource>(live->GetMediaPeer());
 live->GetMediaPeer()->AttachToWebContents(physical.GetWeakPtr());
 ArcWebContents* liveOwner=[[ArcWebContents alloc]init];arc::AssociateWebextrasMediaFacade(liveOwner,live);
 ArcWebContentsMediaSession* liveService=arc::CreateWebextrasMediaService(liveOwner);[liveService setDelegate:d];
 media->PiPCreated();Expect(@[@"A:created:0"]);const int beforeDetach=detachments;
 auto* rawMedia=media.get();auto* physicalPtr=&physical;__weak ArcWebContentsMediaSession* weakService=liveService;
 __weak WebextrasTestDelegate* weakD=d;
 d.onCallback=^{weakD.onCallback=nil;ArcWebContentsMediaSession* currentService=weakService;REQUIRE(currentService&&!currentService->_pipController&&rawMedia->pip_destroyed_);
   REQUIRE(observers==1&&detachments==beforeDetach&&rawMedia->contents_.get()==physicalPtr);
   REQUIRE(arc::CoreCloseStateIsBusy(active));live.reset();
   REQUIRE(!weakLive.expired()&&sibling_alive==1);rawMedia->Info(Info(1,0,0));Drain();
   REQUIRE(!weakLive.expired()&&sibling_alive==1&&arc::CoreCloseStateIsBusy(active));};
 const int beforeLive=arc::test_sync_entries;media->RetirePhysical();
 Expect(@[@"A:destroyed:0",@"A:state:1",@"A:camera:0",@"A:mic:0"]);REQUIRE(weakLive.expired()&&sibling_alive==0);
 REQUIRE(detachments==beforeDetach+1&&!media->contents_&&arc::test_sync_entries==beforeLive+1);
 REQUIRE(!arc::CoreCloseStateIsBusy(active));liveService=nil;liveOwner=nil;media.reset();REQUIRE(observers==0);
 // Nil ObjC facade is not logical expiry and must still deliver to live bridge.
 auto nilRoot=NewRoot(physical);auto nilSource=std::static_pointer_cast<WebextrasMediaSource>(nilRoot->GetMediaPeer());
 nilRoot->GetMediaPeer()->AttachToWebContents(physical.GetWeakPtr());
 ArcWebContents* nilOwner=[[ArcWebContents alloc]init];arc::AssociateWebextrasMediaFacade(nilOwner,nilRoot);
 ArcWebContentsMediaSession* nilService=arc::CreateWebextrasMediaService(nilOwner);[nilService setDelegate:d];
 nilOwner=nil;nilSource->PiPDestroyed();Expect(@[@"A:destroyed:0"]);REQUIRE(d.sawNilOwner);
 nilService=nil;nilSource.reset();nilRoot.reset();REQUIRE(observers==0);
 // A selected native bridge with nil app delegate is separately eligible for
 // cache clear/latch/sync accounting; absence of bridge has no sync scope.
 auto noApp=NewRoot(physical);auto noAppSource=std::static_pointer_cast<WebextrasMediaSource>(noApp->GetMediaPeer());
 noApp->GetMediaPeer()->AttachToWebContents(physical.GetWeakPtr());
 ArcWebContents* noAppOwner=[[ArcWebContents alloc]init];arc::AssociateWebextrasMediaFacade(noAppOwner,noApp);
 ArcWebContentsMediaSession* noAppService=arc::CreateWebextrasMediaService(noAppOwner);
 @autoreleasepool{auto gone=Delegate(@"gone");[noAppService setDelegate:gone];}
 noAppSource->PiPCreated();REQUIRE(noAppService->_pipController);const int appScopes=arc::test_sync_entries;
 noAppSource->PiPDestroyed();REQUIRE(!noAppService->_pipController&&noAppSource->pip_destroyed_&&events.count==0);
 REQUIRE(arc::test_sync_entries==appScopes+1&&!arc::CoreCloseStateIsBusy(active));
 [noAppService setDelegate:nil];noAppSource->PiPCreated();const int noBridgeScopes=arc::test_sync_entries;
 noAppSource->PiPDestroyed();REQUIRE(noAppSource->pip_destroyed_&&arc::test_sync_entries==noBridgeScopes);
 noAppService=nil;noAppOwner=nil;noAppSource.reset();noApp.reset();REQUIRE(observers==0&&tasks.empty());
 std::puts("PASS specific PiPDestroyed actual Source+Root+primitive: dead-root cache/latch/count unchanged, live old-binding order, acquired Root+sibling through return, nil-facade/nil-app distinct eligibility; repeat latch remains HOLD");
}}
'''
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--stage',type=Path,required=True);ap.add_argument('--out',type=Path,required=True);ap.add_argument('--negative-no-lease',action='store_true');args=ap.parse_args()
 spec=importlib.util.spec_from_file_location('initial',ROOT/'arc/browser/webextras_media_logical_initial_held_test.py');module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
 module.TEST_NO_LEASE=args.negative_no_lease
 module.TESTS=module.TESTS[:module.TESTS.index('int main()')]+TESTS
 src=inspect.getsource(module.main)
 src=src.replace("impl=primitive[primitive.index('namespace arc {'):primitive.index('CoreCloseObserverBatch::CoreCloseObserverBatch')]", "impl=primitive[primitive.index('namespace arc {'):primitive.index('CoreCloseObserverBatch::CoreCloseObserverBatch')]\n impl=impl.replace('namespace arc {','namespace arc {\\ninline int test_sync_entries=0;',1).replace('    return std::exchange(synchronous_[Index(channel)], true);','    ++test_sync_entries;\\n    return std::exchange(synchronous_[Index(channel)], true);',1)")
 src=src.replace("# Negative controls mutate extracted code only; production proposal is unchanged.", "# Private fields are observed only in this extracted Source test.\n cls=cls.replace(' private:\\n',' public:\\n')\n # Negative controls mutate extracted code only; production proposal is unchanged.")
 src=src.replace("service=b.SERVICE.replace('SETDELEGATE',methods)", "if TEST_NO_LEASE: pip=pip.replace('  auto logical = logical_.lock();\\n  if (!logical) { return; }', '  if (logical_.expired()) { return; }',1)\n service=b.SERVICE.replace('SETDELEGATE',methods)")
 exec(src,module.__dict__);sys.argv=[__file__,'--stage',str(args.stage),'--out',str(args.out)]
 try:module.main()
 except SystemExit as e:
  p=args.out/'test-result.json';j=json.loads(p.read_text());j['negative_no_lease']=args.negative_no_lease;j['scope']='Specific PiPDestroyed producer only; actual Source/Root/CloseState and actual RetirePhysical body; frozen initial scaffolding';j['instrumentation']='Source private access exposed only in extraction; counter added immediately before actual EnterSync std::exchange, no replacement accounting logic';j['fixture_sha256']=hashlib.sha256((ROOT/'arc/browser/webextras_media_logical_initial_held_test.py').read_bytes()).hexdigest();p.write_text(json.dumps(j,indent=2)+'\n');raise SystemExit(e.code)
if __name__=='__main__':main()
