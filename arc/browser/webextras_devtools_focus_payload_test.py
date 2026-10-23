#!/usr/bin/env python3
"""Bounded SAME-peer focus payload tests; mutates only an explicit private stage.

Actual functional Root, peer, facade and producer bodies run with native/queue/
WeakPtr/transport mocks. Native delegate pending accounting is NOT implemented by
this held chain; no counter is invented here. No app/build/runtime acceptance.
"""
from pathlib import Path
import argparse, hashlib, json, subprocess, tempfile, shutil
parser=argparse.ArgumentParser();parser.add_argument('--stage',type=Path,required=True);parser.add_argument('--negative-controls',action='store_true');args=parser.parse_args();stage=args.stage.resolve();root=Path.cwd()
base_driver=root/'arc/browser/webextras_devtools_trigger_cleanup_peer_test.py'
text=base_driver.read_text();assert hashlib.sha256(text.encode()).hexdigest()=='4391a664393359447e69c5a5bd63cca49a2ba05fdd84be80c8dea9b75862cd12'
# The older final-guard test used focus as a weak-only canary. Replace ONLY that
# receipt with another unchanged weak-only display producer: focus's newly
# proven self payload must postpone final peer destruction. Its exact behavior
# is exercised separately below. Neither frozen prior test is edited.
before='p->state().frontendRequestedFocus();auto weak=p->GetWeakPtr();service=nil;p.reset();r.reset();Check(!weak&&!outer&&[phases isEqual:@[@"raw",@"subscriptions",@"pending"]]&&[d.events isEqual:@[@"d2",@"i4",@"focus"]]&&!tasks.count'
after='p->state().requestedDockPosition("bottom");auto weak=p->GetWeakPtr();service=nil;p.reset();r.reset();Check(!weak&&!outer&&[phases isEqual:@[@"raw",@"subscriptions",@"pending"]]&&[d.events isEqual:@[@"d2",@"i4",@"d4"]]&&!tasks.count'
assert text.count(before)==1;text=text.replace(before,after)
extra=r'''
 // Focus owns the SAME real peer/control, not Root/facade/physical WC. Its
 // functional state remains readable while every external peer owner is gone.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);p->AttachToWebContents(c.GetWeakPtr());auto*d=[TestDelegate new];auto*service=WebextrasCreateDevToolsService(p);[service setDelegate:d];auto self=p->weak_from_this().lock();Check(self.get()==p.get()&&!self.owner_before(p)&&!p.owner_before(self),"focus self control identity changed");self.reset();auto weak=p->GetWeakPtr();auto rootWeak=std::weak_ptr<arc::CoreLogicalWebContents>(r);auto*raw=p.get();raw->state()._nextConnection=41;d.onCall=^{Check(weak&&rootWeak.expired()&&raw->state()._nextConnection==41,"focus did not retain functional peer through app return");};p->state().frontendRequestedFocus();__weak ArcWebContentsDevTools*facade=service;service=nil;p.reset();r.reset();Check(weak&&rootWeak.expired()&&!facade&&tasks.count==1,"focus payload did not retain same peer without Root/service");RunOne();Check(!weak&&[d.events isEqual:@[@"focus"]]&&[d.owners isEqual:@[NSNull.null]],"focus payload release/nil service delivery changed");d.onCall=nil;}++cases;
 // Same registration, nonnil replacement updates already accepted focus.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);auto*service=WebextrasCreateDevToolsService(p);auto*a=[TestDelegate new];auto*b=[TestDelegate new];[service setDelegate:a];p->state().frontendRequestedFocus();[service setDelegate:b];auto weak=p->GetWeakPtr();service=nil;p.reset();r.reset();Drain();Check(!weak&&!a.events.count&&[b.events isEqual:@[@"focus"]]&&[b.owners isEqual:@[NSNull.null]],"focus nonnil replacement/selected bridge changed");}++cases;
 // Nil/re-add cannot retarget the older selected strong bridge.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);auto*service=WebextrasCreateDevToolsService(p);auto*a=[TestDelegate new];auto*z=[TestDelegate new];[service setDelegate:a];p->state().frontendRequestedFocus();[service setDelegate:nil];[service setDelegate:z];p->state().frontendRequestedFocus();auto weak=p->GetWeakPtr();service=nil;p.reset();r.reset();RunOne();Check(weak&&[a.events isEqual:@[@"focus"]]&&!z.events.count,"focus nil/new redirected old selected bridge");RunOne();Check(!weak&&[z.events isEqual:@[@"focus"]],"focus nil/new second lease/order changed");}++cases;
 // No selected bridge still posts one inert payload task; canceling that task
 // releases the peer without executing a delegate or retaining Root.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);auto weak=p->GetWeakPtr();auto rootWeak=std::weak_ptr<arc::CoreLogicalWebContents>(r);p->state().frontendRequestedFocus();Check(tasks.count==1,"focus inert task not posted");p.reset();r.reset();Check(weak&&rootWeak.expired(),"focus inert task did not retain real peer or retained Root");[tasks removeAllObjects];Check(!weak,"destroyed inert task retained peer");}++cases;
 // Executing no-registration work likewise releases its actual payload.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);auto weak=p->GetWeakPtr();p->state().frontendRequestedFocus();p.reset();r.reset();Check(weak&&tasks.count==1,"inert focus lost peer before invocation");RunOne();Check(!weak&&!tasks.count,"inert focus task release changed");}++cases;
 // Expired weak delegate does not remove selected bridge/self payload lifetime.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);auto*service=WebextrasCreateDevToolsService(p);auto*d=[TestDelegate new];[service setDelegate:d];__weak TestDelegate*delegate=d;p->state().frontendRequestedFocus();d=nil;Check(!delegate,"focus task retained app delegate");auto weak=p->GetWeakPtr();service=nil;p.reset();r.reset();Check(bool(weak),"expired app delegate canceled native payload early");RunOne();Check(!weak,"expired delegate focus payload leaked");}++cases;
 // Root may ALREADY be gone when the service/peer receives a new focus request.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);auto*service=WebextrasCreateDevToolsService(p);auto*d=[TestDelegate new];[service setDelegate:d];auto rootWeak=std::weak_ptr<arc::CoreLogicalWebContents>(r);r.reset();Check(rootWeak.expired(),"service held Root before focus");p->state().frontendRequestedFocus();auto weak=p->GetWeakPtr();service=nil;p.reset();Check(bool(weak),"focus wrongly requires live Root");RunOne();Check(!weak&&[d.events isEqual:@[@"focus"]],"service-only focus real peer payload failed");}++cases;
 // Do not strengthen display/inspect. Their accepted weak-only work still
 // cancels once focus has finished and the last actual peer owner is gone.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);auto*service=WebextrasCreateDevToolsService(p);auto*d=[TestDelegate new];[service setDelegate:d];p->state().frontendRequestedFocus();p->state().requestedDockPosition("left");p->state().inspectModeChanged("searchForNode");auto weak=p->GetWeakPtr();service=nil;p.reset();r.reset();RunOne();Check(!weak&&tasks.count==2,"focus payload strengthened other notifications");Drain();Check([d.events isEqual:@[@"focus"]],"display/inspect unexpectedly own actual peer");}++cases;
 Check(!tasks.count&&cases==33,"fixture count/queue mismatch");'''
marker="p=stage/'trigger-peer-test.py';p.write_text(s)"
assert text.count(marker)==1
insertion="focus_extra="+repr(extra)+"\nneedle_focus=' Check(!tasks.count&&cases==25,\"fixture count/queue mismatch\");'\nassert s.count(needle_focus)==1\ns=s.replace(needle_focus,focus_extra).replace('25 actual Root/peer extracted functional and typed-trigger cleanup groups PASS','33 actual Root/peer extracted functional, cleanup and focus payload groups PASS')\n"
text=text.replace(marker,insertion+marker)
adapter=stage/'focus-peer-test.py';adapter.write_text(text)
def execute(folder):
 script=folder/'focus-peer-test.py'
 result=subprocess.run(['python3',str(script),'--stage',str(folder)],text=True,capture_output=True);(folder/'focus-peer-test.log').write_text(result.stdout+result.stderr);return result
result=execute(stage);print(result.stdout,result.stderr);assert result.returncode==0,'actual focus peer fixture failed'
source=(stage/'arc/api/arc_webextras_devtools.mm').read_text();header=(stage/'arc/browser/webextras_devtools_logical_peer.h').read_text()
assert 'public std::enable_shared_from_this<WebextrasDevToolsPeer>' in header
focus=source.split('static void PostDevToolsFocus(',1)[1].split('static void PostDevToolsInspectMode(',1)[0]
assert 'weak_from_this().lock()' in focus and 'std::move(payload)' in focus
for forbidden in ['_logical','CoreLogicalWebContentsHandle','wrapperFor','ArcWebContents* __strong','GetCloseState','ScopedCoreClose','MakeCoreClose']:
 assert forbidden not in focus,forbidden
# Native pending accounting is deliberately not implemented by this chain.
assert 'delegatePending' not in focus
controls=[]
if args.negative_controls:
 old=Path('logs/wp/webextras-devtools-trigger-cleanup/arc/api/arc_webextras_devtools.mm').read_text();start=source.index('static void PostDevToolsFocus(');end=source.index('\nstatic void PostDevToolsInspectMode(',start);oldstart=old.index('static void PostDevToolsFocus(');oldend=old.index('\nstatic void PostDevToolsInspectMode(',oldstart)
 cases=[('weak-peer-only',source[:start]+old[oldstart:oldend]+source[end:],'focus payload did not retain same peer without Root/service'),('skip-inert',source.replace('  // Native focus carries the SAME actual peer as its argument payload.','  if (!bridge) { return; }\n  // Native focus carries the SAME actual peer as its argument payload.'),'focus inert task not posted')]
 for name,changed,expected in cases:
  folder=Path(tempfile.mkdtemp(prefix='webextras-focus-negative-'+name+'-'));shutil.copytree(stage,folder,dirs_exist_ok=True);(folder/'arc/api/arc_webextras_devtools.mm').write_text(changed);r=execute(folder);output=r.stdout+r.stderr
  assert r.returncode!=0 and expected in output and (folder/'lifetime-compiler.log').read_bytes()==b'',(name,r.returncode,output[-8000:]);controls.append({'name':name,'directory':str(folder),'compiled':True,'expectedAssertion':expected,'detected':True});print('PASS negative:',name)
(stage/'focus-tests-summary.json').write_text(json.dumps({'groups':33,'newFocusGroups':8,'negativeControls':controls,'nativeDelegatePending':'NOT IMPLEMENTED; no invented counter; native accounting parity HOLD','nativeDependencies':'mocks; no runtime or acceptance'},indent=2)+'\n')
