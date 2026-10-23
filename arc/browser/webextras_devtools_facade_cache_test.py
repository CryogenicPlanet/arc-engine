#!/usr/bin/env python3
"""Verify actual extracted facade property/ivar/cache; run peer regressions privately."""
from pathlib import Path
import argparse,hashlib,json,subprocess,tempfile
parser=argparse.ArgumentParser();parser.add_argument('--stage',type=Path,required=True);parser.add_argument('--baseline-stage',type=Path,required=True);args=parser.parse_args();stage=args.stage.resolve();old=args.baseline_stage.resolve()
ROOT=Path.cwd()
def method(t,anchor):
 a=t.index(anchor);op=t.index('{',a);d=0
 for i in range(op,len(t)):
  if t[i]=='{':d+=1
  elif t[i]=='}':
   d-=1
   if not d:return t[a:i+1]
 raise AssertionError(anchor)
pre=r'''
#import <Foundation/Foundation.h>
#import <objc/runtime.h>
#include <memory>
#include <functional>
#include <string>
#define ARC_EXPORT
#define CHECK(v) do{if(!(v))abort();}while(0)
static void Check(bool b,const char*s){if(!b){fprintf(stderr,"FAILED %s\n",s);abort();}}
@class ArcWebContentsDevTools;
@protocol WebExtrasDevToolsDelegate<NSObject>@end
static void CacheDestructing();
@interface ArcWebContents:NSObject@end
@implementation ArcWebContents
- (void)dealloc{CacheDestructing();}
@end
'''
for label,folder in [('baseline',old),('corrected',stage)]:
 s=(folder/'arc/api/arc_webextras_devtools.mm').read_text();fac=s[s.index('ARC_EXPORT @interface ArcWebContentsDevTools : NSObject'):s.index('ARC_EXPORT @interface ArcBrowserContextDevTools')];interface=fac.split('@implementation ArcWebContentsDevTools')[0];impl=fac.split('@implementation ArcWebContentsDevTools\n')[1].split('\n@end')[0]
 bridge=s[s.index('@interface WebextrasDevToolsDelegateBridge'):s.index('static NSUInteger DevToolsDisplayMode')]
 peer=r'''
static bool observeTeardown=false;
static __weak WebextrasDevToolsDelegateBridge* observedBridge;
static __weak ArcWebContents* observedCache;
static NSMutableArray*destroyOrder;
static void CacheDestructing(){if(observeTeardown){Check(!observedBridge,"cache released before bridge");[destroyOrder addObject:@"cache"];}}
namespace arc{class WebextrasDevToolsPeer{public:
 ~WebextrasDevToolsPeer(){if(observeTeardown){Check(observedBridge&&observedCache,"peer teardown did not precede bridge/cache");[destroyOrder addObject:@"peer"];}}
 struct State{void RegisterBridge(WebextrasDevToolsDelegateBridge*b){observedBridge=b;}ArcWebContents*toolboxWebContents(){return nil;}};State&state(){return state_;}private:State state_;};using WebextrasDevToolsPeerHandle=std::shared_ptr<WebextrasDevToolsPeer>;}
'''
 bodies='\n'.join(method(impl,a)for a in ['- (instancetype)initWithLogicalDevToolsPeer:','- (void)setDelegate:','- (ArcWebContents*)toolboxWebContents'])
 synth='@synthesize toolboxWebContents = _toolboxWebContents;\n'if '@synthesize toolboxWebContents' in impl else ''
 category=r'''
@interface ArcWebContentsDevTools(FixtureCache)
- (void)fixtureCache:(ArcWebContents*)value;
@end
@implementation ArcWebContentsDevTools(FixtureCache)
- (void)fixtureCache:(ArcWebContents*)value{_toolboxWebContents=value;}
@end
'''if label=='corrected'else ''
 check=r'''
int main(){@autoreleasepool{
 auto prop=class_getProperty(ArcWebContentsDevTools.class,"toolboxWebContents");
 auto ivar=class_getInstanceVariable(ArcWebContentsDevTools.class,"_toolboxWebContents");
 std::string attrs=prop?property_getAttributes(prop):"";
 printf("property=%s\n",attrs.c_str());
 if(attrs!="T@\"ArcWebContents\",R,N,V_toolboxWebContents"||!ivar){puts("DETECTED metadata P2: missing true facade backing ivar/attribute");return 42;}
 Check(ivar_getOffset(ivar)==8,"cache ivar offset differs from native8");
 Check(std::string(ivar_getTypeEncoding(ivar))=="@\"ArcWebContents\"","cache ivar type differs");
'''
 if label=='corrected':check+=r'''
 auto peer=std::make_shared<arc::WebextrasDevToolsPeer>();
 ArcWebContentsDevTools*a=[[ArcWebContentsDevTools alloc]initWithLogicalDevToolsPeer:peer];
 ArcWebContentsDevTools*b=[[ArcWebContentsDevTools alloc]initWithLogicalDevToolsPeer:peer];
 Check(!a.toolboxWebContents&&!b.toolboxWebContents,"initially-nil bounded behavior changed");
 __weak ArcWebContents*wa;__weak ArcWebContents*wb;
 @autoreleasepool{ArcWebContents*x=[ArcWebContents new];ArcWebContents*y=[ArcWebContents new];wa=x;wb=y;[a fixtureCache:x];[b fixtureCache:y];Check(a.toolboxWebContents==x&&b.toolboxWebContents==y,"getter not using actual per-facade cache");x=nil;y=nil;Check(wa&&wb,"facade cache not strong");}
 a=nil;Check(!wa&&wb,"facade lifetime did not independently release genuine cache");
 b=nil;Check(!wb,"second facade cache retained by peer");
 @autoreleasepool{auto finalPeer=std::make_shared<arc::WebextrasDevToolsPeer>();ArcWebContentsDevTools*service=[[ArcWebContentsDevTools alloc]initWithLogicalDevToolsPeer:finalPeer];auto*delegate=(id<WebExtrasDevToolsDelegate>)[NSObject new];[service setDelegate:delegate];@autoreleasepool{auto*cache=[ArcWebContents new];observedCache=cache;[service fixtureCache:cache];}destroyOrder=[NSMutableArray array];observeTeardown=true;finalPeer.reset();service=nil;Check([destroyOrder isEqual:@[@"peer",@"cache"]]&&!observedBridge&&!observedCache,"facade teardown order changed");observeTeardown=false;}
 puts("PASS actual facade metadata/type/offset8/public getter/independent strong cache ownership/native peer-bridge-cache release order; cache seeded only by test category");
'''
 check+='return 0;}}\n'
 fixture=pre+bridge+peer+interface+'@implementation ArcWebContentsDevTools\n'+synth+bodies+'\n@end\n'+category+check
 p=stage/(label+'-facade-cache-fixture.mm');p.write_text(fixture);binary=stage/(label+'-facade-cache-fixture');cmd=['xcrun','clang++','-std=c++20','-fobjc-arc','-O1','-Wall','-Wextra','-Werror','-framework','Foundation',str(p),'-o',str(binary)];c=subprocess.run(cmd,text=True,capture_output=True);(stage/(label+'-metadata-compiler.log')).write_text(c.stdout+c.stderr);assert c.returncode==0,c.stdout+c.stderr
 r=subprocess.run([str(binary)],text=True,capture_output=True);(stage/(label+'-metadata-run.log')).write_text(r.stdout+r.stderr);expected=42 if label=='baseline'else 0;assert r.returncode==expected,(label,r.returncode,r.stdout,r.stderr);print(label,'exit',r.returncode,r.stdout.strip())
# The original frozen peer test remains immutable. Adapt only its mock retired
# state shape and extraction of the actual new @synthesize in a PRIVATE copy.
original=(ROOT/'arc/browser/webextras_devtools_logical_peer_test.py').read_text()
assert hashlib.sha256(original.encode()).hexdigest()=='66c6c510f179450784e92cd7a4c898e324cc0128a55fe30aaae7e06c855a9ccd'
modified=original.replace('struct FrontendRetiredState{ArcWebContents*toolbox;NSArray<ProtocolNotification>*panel_queries;};','struct FrontendRetiredState{NSArray<ProtocolNotification>*panel_queries;};')
needle="+'@implementation ArcWebContentsDevTools\\n'+'\\n'.join(kept)"
assert needle in modified
modified=modified.replace(needle,"+'@implementation ArcWebContentsDevTools\\n@synthesize toolboxWebContents = _toolboxWebContents;\\n'+'\\n'.join(kept)")
# Frozen baseline's lifetime tests include a fake State getter which no longer
# exists; all public forwarding is now the actual facade ivar getter.
p=stage/'adapted-peer-test.py';p.write_text(modified)
r=subprocess.run(['python3',str(p),'--stage',str(stage)],text=True,capture_output=True);(stage/'adapted-peer-test.log').write_text(r.stdout+r.stderr);assert r.returncode==0,r.stdout+r.stderr;print(r.stdout.strip())
source=(stage/'arc/api/arc_webextras_devtools.mm').read_text();actual_impl=source.split('class WebextrasDevToolsPeer::Impl final')[1].split('ARC_EXPORT @interface ArcWebContentsDevTools')[0]
assert '_toolboxWebContents' not in actual_impl
assert 'ArcWebContents* toolbox;' not in source
assert 'FrontendRetiredState retired{_panelQueries}' in source
print('PASS no peer duplicate cache, cache reset/callout or retired-cache lease; actual panel-state lease remains')
