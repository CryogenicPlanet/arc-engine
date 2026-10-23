#!/usr/bin/env python3
"""Actual initial Attach ordering against a mocked Observe registration seam.

Reads an explicit private stage and writes fixtures only there. No source, native
observer callback, app/build/lock/runtime or acceptance behavior is fabricated.
The mock hook inspects registration-time state; C156 Observe is not claimed to
call the app inline. All prior functional peer/focus groups remain executed.
"""
from pathlib import Path
import argparse,hashlib,json,subprocess,tempfile,shutil
p=argparse.ArgumentParser();p.add_argument('--stage',type=Path,required=True);p.add_argument('--negative-controls',action='store_true');a=p.parse_args();stage=a.stage.resolve();root=Path.cwd()
old=root/'arc/browser/webextras_devtools_focus_payload_test.py';script=old.read_text();assert hashlib.sha256(script.encode()).hexdigest()=='ac6ad8ad4b02a2eb8cad48d2421a67c056cd2d0a39174fef57b4b751f00d7f25'
# Adapt the mock only. Its Observe entry verifies the actual Attach body's
# publication order; the production method remains extracted unchanged.
observe='void WebContentsObserver::Observe(WebContents*c){if(c_){auto&v=c_->observers;v.erase(std::remove(v.begin(),v.end(),this),v.end());}c_=c;if(c_)c_->observers.push_back(this);}'
replacement='static std::function<void(WebContents*,WebContentsObserver*)>registration_probe;\nvoid WebContentsObserver::Observe(WebContents*c){if(c&&registration_probe)registration_probe(c,this);if(c_){auto&v=c_->observers;v.erase(std::remove(v.begin(),v.end(),this),v.end());}c_=c;if(c_)c_->observers.push_back(this);}'
extra=r'''
 // The actual state and weak userdata route must already be installed at the
 // incoming Observe boundary, with BOTH real Root peer slots complete.
 {content::WebContents c;auto r=MakeRoot(&c);auto p=Peer(r);auto*raw=p.get();auto*logical=r.get();auto weak=p->GetWeakPtr();auto rootWeak=std::weak_ptr<arc::CoreLogicalWebContents>(r);Check(!PeerForContents(&c),"factory published userdata before initial Attach");unsigned observed=0;content::registration_probe=[&](content::WebContents*incoming,content::WebContentsObserver*observer){Check(incoming==&c&&observer==&raw->state(),"wrong observer/physical identity at registration");Check(PeerForContents(incoming).get()==raw,"userdata missing before Observe registration");Check(raw->state()._contents.get()==incoming&&logical->GetNative()==incoming,"physical state missing before Observe registration");Check(logical->GetDevToolsPeer().get()==raw&&logical->GetMediaPeer(),"incomplete or substitute Root before Observe registration");++observed;};p->AttachToWebContents(c.GetWeakPtr());content::registration_probe=nullptr;Check(observed==1&&PeerForContents(&c).get()==raw,"initial Attach observation count/route changed");p.reset();r.reset();Check(rootWeak.expired()&&!weak&&!PeerForContents(&c),"native userdata acquired ownership of Root/peer");}++cases;
 Check(!tasks.count&&cases==34,"fixture count/queue mismatch");'''
marker="adapter=stage/'focus-peer-test.py';adapter.write_text(text)"
insertion="attach_observe="+repr(observe)+"\nattach_replacement="+repr(replacement)+"\nattach_extra="+repr(extra)+"\nattach_marker=\"p=stage/'trigger-peer-test.py';p.write_text(s)\"\nattach_insertion=\"assert s.count(\"+repr(attach_observe)+\")==1\\ns=s.replace(\"+repr(attach_observe)+\",\"+repr(attach_replacement)+\")\\nassert s.count(' Check(!tasks.count&&cases==33,\\\"fixture count/queue mismatch\\\");')==1\\ns=s.replace(' Check(!tasks.count&&cases==33,\\\"fixture count/queue mismatch\\\");',\"+repr(attach_extra)+\").replace('33 actual Root/peer extracted functional, cleanup and focus payload groups PASS','34 actual Root/peer functional, focus and initial Attach ordering groups PASS')\\n\"\nassert text.count(attach_marker)==1\ntext=text.replace(attach_marker,attach_insertion+attach_marker)\n"
assert script.count(marker)==1;script=script.replace(marker,insertion+marker)
adapter=stage/'attach-order-focus-test.py';adapter.write_text(script)
def execute(folder):
 r=subprocess.run(['python3',str(folder/'attach-order-focus-test.py'),'--stage',str(folder)],capture_output=True,text=True);(folder/'attach-order-test.log').write_text(r.stdout+r.stderr);return r
r=execute(stage);print(r.stdout,r.stderr);assert r.returncode==0,'actual Attach fixture failed'
source=(stage/'arc/api/arc_webextras_devtools.mm').read_text();baseline=Path('logs/wp/webextras-devtools-focus-payload/staged/arc/api/arc_webextras_devtools.mm').read_text();before='    _contents = contents;\n    Observe(contents.get());\n    contents->SetUserData(&kDevToolsBindingKey, std::make_unique<DevToolsBinding>(_peer));\n    InstallHooks();';after='    _contents = contents;\n    contents->SetUserData(&kDevToolsBindingKey, std::make_unique<DevToolsBinding>(_peer));\n    Observe(contents.get());\n    InstallHooks();';assert baseline.count(before)==source.count(after)==1;assert source.replace(after,before)==baseline,'unrelated production-body drift'
controls=[]
if a.negative_controls:
 folder=Path(tempfile.mkdtemp(prefix='webextras-attach-order-negative-'));shutil.copytree(stage,folder,dirs_exist_ok=True);(folder/'arc/api/arc_webextras_devtools.mm').write_text(source.replace(after,before));r=execute(folder);output=r.stdout+r.stderr
 assert r.returncode!=0 and 'userdata missing before Observe registration' in output and (folder/'lifetime-compiler.log').read_bytes()==b'',output[-8000:];controls.append({'name':'Observe-before-userdata','directory':str(folder),'compiled':True,'expectedAssertion':'userdata missing before Observe registration','detected':True});print('PASS compiled negative: old Observe-before-userdata order')
(stage/'attach-order-summary.json').write_text(json.dumps({'groups':34,'newOrderGroups':1,'negativeControls':controls,'sourceDelta':'only SetUserData/Observe ordering; all checks and other source bytes unchanged','registrationHook':'mock phase assertion only; no claim that real Observe invokes callbacks'},indent=2)+'\n')
