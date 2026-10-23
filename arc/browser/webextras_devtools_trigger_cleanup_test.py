#!/usr/bin/env python3
"""Run bounded actual Root/peer, actual raw-client and facade metadata fixtures.

The explicit private stage is the only mutation target. Native host/queue/JSON
and WeakPtr implementations are mocks; Foundation ARC and actual shared Root,
peer/producer/client bodies run. Negative controls must compile and fail their
specific behavioral assertion. This is not an engine runtime/golden comparison.
"""
from pathlib import Path
import argparse,hashlib,json,shutil,subprocess,tempfile
parser=argparse.ArgumentParser();parser.add_argument('--stage',type=Path,required=True);parser.add_argument('--baseline-stage',type=Path,required=True);parser.add_argument('--negative-controls',action='store_true');args=parser.parse_args();stage=args.stage.resolve();baseline=args.baseline_stage.resolve();root=Path.cwd()
peer=root/'arc/browser/webextras_devtools_trigger_cleanup_peer_test.py';client=root/'arc/browser/webextras_devtools_trigger_cleanup_client_test.py'
def run(script,folder):
 r=subprocess.run(['python3',str(script),'--stage',str(folder)],text=True,capture_output=True);assert r.returncode==0,r.stdout+r.stderr;return r.stdout
print(run(peer,stage).strip());print(run(client,stage).strip())
# Reuse only the cache test's ACTUAL facade/runtime fixture, preserving both
# frozen tests byte-for-byte. Its old peer adapter is superseded for this delta.
metadata=(root/'arc/browser/webextras_devtools_facade_cache_test.py').read_text();assert hashlib.sha256(metadata.encode()).hexdigest()=='e7d2d065f256470e9db8cc3fccdb1006866df983f67a33884226baf30ee7ec36'
metadata=metadata.split('# The original frozen peer test remains immutable.',1)[0]
p=stage/'trigger-metadata-test.py';p.write_text(metadata)
r=subprocess.run(['python3',str(p),'--stage',str(stage),'--baseline-stage',str(baseline)],text=True,capture_output=True);(stage/'trigger-metadata-test.log').write_text(r.stdout+r.stderr);assert r.returncode==0,r.stdout+r.stderr;print(r.stdout.strip())
source=(stage/'arc/api/arc_webextras_devtools.mm').read_text();header=(stage/'arc/browser/webextras_devtools_logical_peer.h').read_text()
# No unconditional per-target weak guard invalidation or extra alive object.
assert header.count('base::WeakPtrFactory<WebextrasDevToolsPeer>')==2
assert 'command_weak_factory_.InvalidateWeakPtrs();\n  impl_->DestroyFinalProtocolState();\n  weak_factory_.InvalidateWeakPtrs();' in source
close=source.split('  void Close() {',1)[1].split('  void DispatchProtocolMessage(',1)[0]
after=close.split('host->DetachClient(this);',1)[1]
for forbidden in ['host_','callback_','attached_','weak_factory_','this->']:assert forbidden not in after,forbidden
observed=source.split('  void WebContentsDestroyed() override {',2)[2].split('  void RegisterBridge',1)[0] if False else source.split('    DeliverObservedHostClosed(pending, subscriptions);',1)[1].split('  void RegisterBridge',1)[0]
assert 'state.' not in observed and 'peer->' not in observed
helper=source.split('void DeliverObservedHostClosed(',1)[1].split('// Keep retired panel blocks alive',1)[0]
assert 'sorted' not in helper and 'NotifySubscription' not in helper
print('PASS structural actual-peer/client tail, separate guards, direct unsorted terminal helper checks')
controls=[]
if args.negative_controls:
 original=(stage/'arc/api/arc_webextras_devtools.mm').read_text()
 # A private source perturbation is evidence only if its fixture compiles and
 # fails the intended runtime assertion. No production/proposal mutation.
 cases=[
  ('missing-both-map-snapshot','peer',
   '    ProtocolSubscriptions subscriptions = std::exchange(\n        state._subscriptions, [NSMutableDictionary dictionary]);\n    DeliverObservedHostClosed(pending, subscriptions);',
   '    ProtocolSubscriptions subscriptions = state._subscriptions;\n    DeliverObservedHostClosed(pending, subscriptions);\n    state._subscriptions = [NSMutableDictionary dictionary];',
   'FAILED maps not both empty before first callback'),
  ('async-terminal-subscriptions','peer',
   '        callback(subscription[@"identifier"], nil,\n                 DevToolsError(10, @"DevTools agent host closed"));',
   '        NotifySubscription(callback, subscription[@"identifier"], nil,\n                 DevToolsError(10, @"DevTools agent host closed"));',
   'FAILED commands did not precede subscriptions'),
  ('late-command-router-guard','peer',
   '  command_weak_factory_.InvalidateWeakPtrs();\n  impl_->DestroyFinalProtocolState();',
   '  impl_->DestroyFinalProtocolState();\n  command_weak_factory_.InvalidateWeakPtrs();',
   'FAILED router remained valid during final raw detach'),
  ('early-delegate-guard','peer',
   '  impl_->DestroyFinalProtocolState();\n  weak_factory_.InvalidateWeakPtrs();',
   '  weak_factory_.InvalidateWeakPtrs();\n  impl_->DestroyFinalProtocolState();',
   'FAILED distinct final guard/destruction phases changed'),
  ('strong-root-during-observed-errors','peer',
   '  void WebContentsDestroyed() override {\n    auto peer = _peer;',
   '  void WebContentsDestroyed() override {\n    [[maybe_unused]] auto held_root = _logical.lock();\n    auto peer = _peer;',
   'FAILED direct error held strong peer/root'),
  ('strong-oversized-task','client',
   '          base::BindOnce([](base::WeakPtr<ProtocolClient> client) {\n            if (client) { client->Close(); }\n          }, weak_client));',
   '          base::BindOnce([](std::shared_ptr<ProtocolClient> client) {\n            client->Close();\n          }, shared_from_this()));',
   'FAILED oversize task retained actual client/deferred close'),
  ('strong-client-during-direct-completion','client',
   '      ProtocolNotification callback = callback_;\n      callback(message, nil);',
   '      auto held_client = shared_from_this();\n      ProtocolNotification callback = callback_;\n      callback(message, nil);',
   'FAILED internal direct completion retained client')]
 for name,suite,before,after,marker in cases:
  assert original.count(before)==1,(name,'anchor count')
  d=Path(tempfile.mkdtemp(prefix='webextras-devtools-trigger-negative-'+name+'-'));shutil.copytree(stage,d,dirs_exist_ok=True);changed=original.replace(before,after)
  if name in ['strong-oversized-task','strong-client-during-direct-completion']:
   changed=changed.replace('class ProtocolClient final : public content::DevToolsAgentHostClient {','class ProtocolClient final : public content::DevToolsAgentHostClient, public std::enable_shared_from_this<ProtocolClient> {')
   if name=='strong-oversized-task':changed=changed.replace('      auto weak_client = weak_factory_.GetWeakPtr();\n','')
  (d/'arc/api/arc_webextras_devtools.mm').write_text(changed)
  executable=peer if suite=='peer'else client;r=subprocess.run(['python3',str(executable),'--stage',str(d)],text=True,capture_output=True);output=r.stdout+r.stderr
  compile_log=d/('lifetime-compiler.log'if suite=='peer'else'trigger-client-compiler.log')
  assert r.returncode!=0 and marker in output and compile_log.read_bytes()==b'',(name,r.returncode,output[-10000:])
  controls.append({'name':name,'suite':suite,'directory':str(d),'compilerDiagnosticBytes':0,'expectedAssertion':marker,'detected':True,'output':output})
  print('PASS negative control detected:',name)
 (stage/'trigger-negative-controls.json').write_text(json.dumps(controls,indent=2)+'\n')
summary={'peerGroups':25,'actualClientGroups':8,'metadataNegativeAndCorrected':'PASS','structuralChecks':'PASS','negativeControls':len(controls),'nativeDependencies':'mocks; no app/native runtime or golden comparison'}
(stage/'trigger-tests-summary.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary))
