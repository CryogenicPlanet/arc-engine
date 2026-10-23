#!/usr/bin/env python3
"""UNSEALED Native13 held-slot proposal; no lock acquisition or signal policy.

Parent must seal/review config after material adoption, then invoke under the
ordinary shared locked.sh owner. Parent alone queues, reaps and reconciles.
"""
import datetime
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
sys.dont_write_bytecode=True
ROOT=Path(__file__).resolve().parents[2]
CONFIG=ROOT/'logs/wp/webextras-native13-reference-proposal/config.UNSEALED.json'
STEM=ROOT/'logs/wp/webextras.native.run13'
OUTPUT=ROOT/'results/webextras/native-run13'
WRAPPER='arc/browser/webextras_native_resources_current_run.sh'
CHECKER='arc/browser/webextras_frontend_resource_checked.py'
CHECKER_SHA='aee01b900c0bfaa527995f893167a525a1a9984f2f5e5e8f76370dc98ce99b74'
REFERENCE_SHA='b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d'
REFERENCE_SIZE=501736176
RESOURCE_IDS={'DEVTOOLS_COMPATIBILITY_JS','PANELS_ELEMENTS_ELEMENTS_JS','UI_LEGACY_LEGACY_JS'}

def require(value,message):
    if not value:raise RuntimeError(message)

def file_pin(path):
    path=Path(path);real=path.resolve(strict=True);before=real.stat()
    require(real.is_file(),'not regular: '+str(path))
    with real.open('rb') as stream:digest=hashlib.file_digest(stream,'sha256').hexdigest()
    after=real.stat()
    fields=('st_dev','st_ino','st_size','st_mtime_ns','st_ctime_ns')
    require(path.resolve(strict=True)==real and all(getattr(before,k)==getattr(after,k) for k in fields),'input changed: '+str(path))
    return {'path':str(path),'realpath':str(real),'sha256':digest,'size':after.st_size,**{k:str(getattr(after,k)) for k in fields}}

def write_new(path,value):
    with Path(path).open('x') as stream:stream.write(json.dumps(value,indent=2)+'\n')
    Path(path).chmod(0o444)

def ancestor_owner():
    owner=int((ROOT/'logs/arccore-build.lock/pid').read_text());ancestor=os.getpid()
    for _ in range(20):
        if ancestor==owner:return owner
        require(ancestor>1,'shared lock owner is not an ancestor')
        ancestor=int(subprocess.check_output(['/bin/ps','-p',str(ancestor),'-o','ppid='],text=True))
    raise RuntimeError('shared lock owner is not an ancestor')

def check_config(config):
    require(config.get('status')=='SEALED_AFTER_ADOPTION','UNSEALED: material adoption and fresh parent review required')
    require(config.get('materialAdoptionEvidenceSHA256') and len(config['materialAdoptionEvidenceSHA256'])==64,'missing adoption evidence pin')
    require(config.get('expectedFiles') and set(config['requiredFiles'])<=set(config['expectedFiles']),'incomplete selected source closure')
    require(config['expectedFiles'].get(CHECKER)==CHECKER_SHA,'wrong reviewed resource checker')
    require(config['expectedFiles'].get('arc/browser/webextras_native_resources_probe.ts')=='46dfcca2b690df487ee7170d9c469ba6bedcde5b94c366620172037f18330ae4','returned-identity correction not sealed')
    for path,digest in config['expectedFiles'].items():
        require(isinstance(digest,str) and len(digest)==64,'unsealed source pin: '+path)
        require(file_pin(ROOT/path)['sha256']==digest,'source pin drift: '+path)

def bind_resource_receipt(receipt,proof,before,after):
    require(receipt.get('passed') is True and receipt.get('status')=='RESOURCE_BYTES_VERIFIED' and receipt.get('expectHostGate')=='present' and receipt.get('builtPackagedPakExactMatch') is True,'resource checker did not verify ours host gate')
    require(proof.get('contentEqual') is True and proof['packaged']==before==after,'resource proof/journal identity mismatch')
    runtime=before['files']['resources.pak']['sha256'];built=proof['built']['files']['resources.pak']['sha256']
    require(receipt.get('runtimePakSHA256')==runtime==built==receipt.get('builtPakSHA256'),'resource checker receipt not tied to actual ours launch/cleanup PAK')
    resources=receipt.get('resources',[])
    require(len(resources)==3 and {x.get('textualId') for x in resources}==RESOURCE_IDS and all(x.get('expectHostGate')=='present' and all(x.get(k) is True for k in ('generatedExactMatch','grdInputExactMatch','intermediateExactMatch')) for x in resources),'incomplete three-resource proof')
    sanity=receipt.get('nativePackSanity',{})
    require(set(sanity)=={'intermediate','built','packaged'} and all(x.get('version')==5 and x.get('nativeLoaderChecksPassed') is True and x.get('writerLookupDeterminismPolicyPassed') is True for x in sanity.values()),'missing native V5 lookup proof')
    return runtime

def main():
    require(len(sys.argv)==1,'fixed Native13 control accepts no arguments')
    config_pin=file_pin(CONFIG);config=json.loads(CONFIG.read_text());check_config(config)
    require(file_pin(CONFIG)==config_pin,'config changed')
    owner=ancestor_owner();os.chdir(ROOT)
    resource_output=ROOT/'logs/wp/webextras.native.run13.resource-check'
    fresh=[OUTPUT,Path(str(STEM)+'.source-before.json'),Path(str(STEM)+'.native-reference-pin.json'),Path(str(STEM)+'.resource-tie.json'),resource_output]
    require(all(not p.exists() and not p.is_symlink() for p in fresh),'Native13 output namespace already exists')
    reference=ROOT.parent/'arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore'
    pin=file_pin(reference);require(pin['sha256']==REFERENCE_SHA and pin['size']==REFERENCE_SIZE,'original B70 reference drift')
    write_new(str(STEM)+'.native-reference-pin.json',{'capturedUTC':datetime.datetime.now(datetime.timezone.utc).isoformat(),'reference':pin,'config':config_pin,'command':file_pin(__file__),'lockOwnerPID':owner,'contract':'ancestor-verified ordinary shared slot; parent sole queue/reap authority'})
    # Do not use check=True: ordinary measurement exit1 must still collect the
    # fixed reviewed resource proof before this child returns to the lock owner.
    probe=subprocess.run(['/bin/bash',WRAPPER,'--locked','--run-number','13','--devtools-frontend-events']).returncode
    require(ancestor_owner()==owner,'ancestor lock changed after wrapper')
    check_config(config);require(file_pin(CONFIG)==config_pin,'config changed after wrapper')
    with Path(str(STEM)+'.resource-check.log').open('x') as log:
        checker=subprocess.run([sys.executable,'-B',CHECKER,'--output',str(resource_output),'--expect-host-gate','present'],stdout=log,stderr=subprocess.STDOUT).returncode
    require(ancestor_owner()==owner,'ancestor lock changed after resource proof')
    require(checker==0,'resource checker failed; inspect receipt/log before parent reaping')
    receipt=json.loads((resource_output/'receipt.json').read_text())
    proof=json.loads(Path(str(STEM)+'.resources-package.json').read_text())
    before=json.loads((OUTPUT/'ours156/binary-identity.json').read_text())['resources']
    after=json.loads((OUTPUT/'ours156/resources-after-cleanup.json').read_text())
    runtime=bind_resource_receipt(receipt,proof,before,after)
    # Reuse the reviewed actual resource verifier, not a second PAK decoder.
    verified_proof=subprocess.run([sys.executable,'-B','arc/browser/webextras_resource_identity.py','verify-proof',str(STEM)+'.resources-package.json'],check=True,capture_output=True,text=True)
    verified_journal=subprocess.run([sys.executable,'-B','arc/browser/webextras_resource_identity.py','verify-journal',str(OUTPUT/'ours156/binary-identity.json')],check=True,capture_output=True,text=True)
    require(json.loads(verified_proof.stdout)==proof and json.loads(verified_journal.stdout)==before,'final actual resource verifier returned different identity')
    require(file_pin(reference)==pin,'original B70 changed after pin')
    original=json.loads((OUTPUT/'trace156/binary-identity.json').read_text())['engine']
    require(original['sha256']==REFERENCE_SHA and original['size']==REFERENCE_SIZE and Path(original['realpath'])==Path(pin['realpath']),'runtime original engine differs from B70 pin')
    check_config(config);require(file_pin(CONFIG)==config_pin and ancestor_owner()==owner,'final config/slot drift')
    write_new(str(STEM)+'.resource-tie.json',{'status':'OURS_RESOURCE_BYTES_TIED_TO_LAUNCH_AND_CLEANUP','runtimePakSHA256':runtime,'receipt':file_pin(resource_output/'receipt.json'),'probeExitCode':probe,'resourceCheckerExitCode':checker,'originalReference':pin,'scope':'resources proof only; no acceptance/OS-exit/cleanup guarantee'})
    return probe

if __name__=='__main__':
    try:sys.exit(main())
    except (RuntimeError,OSError,ValueError,KeyError,subprocess.SubprocessError) as error:
        print(str(error),file=sys.stderr);sys.exit(2)
