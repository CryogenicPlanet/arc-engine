#!/usr/bin/env python3
"""Native12 frozen-data-only diagnostic. No runtime or acceptance authority."""
import copy
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import types

ROOT = Path(__file__).resolve().parents[3]
OUT = Path(__file__).resolve().parent
FROZEN = OUT/'frozen-inputs'
NATIVE = FROZEN/'results/webextras/native-run12'
IMAGE = 'WebextrasNativeProbe.dylib'
SECTIONS = ('missing','extra','return_mismatch','arg_mismatch','count_mismatch')

def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def write(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2)+'\n')

def load(path, name):
    module=types.ModuleType(name);module.__file__=str(path)
    exec(compile(path.read_bytes(),str(path),'exec'),module.__dict__)
    return module

def inputs_unchanged():
    manifest=json.loads((OUT/'input-manifest.json').read_text())
    for entry in manifest:
        assert sha(ROOT/entry['path'])==entry['sha256'],entry['path']
        assert sha(FROZEN/entry['path'])==entry['sha256'],entry['path']
    return len(manifest)

def rows(path):
    return [json.loads(line) for line in path.read_text().splitlines()]

def pairs(data):
    stacks={};result=[]
    for index,event in enumerate(data):
        stack=stacks.setdefault(event['thread'],[])
        if event['ev']=='call':stack.append(index)
        else:
            start=stack.pop();call=data[start]
            assert all(call.get(k)==event.get(k) for k in ('dir','kind','cls','sel','self','thread','from','depth'))
            result.append((start,index))
    assert not any(stacks.values())
    return result

def save_rows(path,data):
    path.parent.mkdir(parents=True,exist_ok=True)
    path.write_text(''.join(json.dumps(row,separators=(',',':'))+'\n' for row in data))

def execute(helper,snapshot,left,right,name):
    directory=OUT/'comparisons'/name;directory.mkdir(parents=True,exist_ok=True)
    result=helper.compare(snapshot,IMAGE,left,right,directory/'opt-in-report.json')
    write(directory/'helper-result.json',result)
    (directory/'opt-in-stdout.txt').write_text(result['stdout'])
    command=[sys.executable,'-B',str(snapshot/'tests/recorder/diff.py'),str(left),str(right),'--json',str(directory/'default-report.json')]
    process=subprocess.run(command,text=True,capture_output=True,timeout=20)
    write(directory/'default-invocation.json',dict(argv=command,exitCode=process.returncode,stdout=process.stdout,stderr=process.stderr))
    assert process.returncode==0,process.stderr
    (directory/'default-stdout.txt').write_text(process.stdout)
    return dict(name=name,status=result['status'],verified=result['verified'],counts=result['counts'],
                sections={key:len(result['report'][key]) for key in SECTIONS},
                callbackSequence=result['report']['callback_sequence'],
                firstTwoLines=result['stdout'].splitlines()[:2],
                defaultFirstTwoLines=process.stdout.splitlines()[:2])

def main():
    count=inputs_unchanged()
    helper=load(NATIVE/'frontend-comparison.py','native12_frozen_frontend')
    freezer=load(FROZEN/'arc/browser/webextras_fixture_inputs.py','native12_frozen_freezer')
    assert sha(NATIVE/'frontend-comparison.py')=='1a2c226c853e43b1d74babde939320707d34ec69802f93f2eb2976d33f23cc9e'
    assert sha(FROZEN/'arc/browser/webextras_fixture_inputs.py')=='f60537a31119b2b50f802f0ce0b9257ee12a09dd0f1edc3a639c03a3e5a64fd9'
    legacy=ROOT/'results/webextras/native-run12/fixture-comparator'
    legacy_state=helper.verify(legacy)
    assert len(legacy_state['files'])==4
    write(OUT/'legacy-verified-manifest.json',legacy_state)
    results=[];provenance=[];inventory=[]
    events=json.loads((NATIVE/'trace156/frontend-events.json').read_text())
    raw=(NATIVE/'trace156/record.jsonl').read_bytes()
    with tempfile.TemporaryDirectory(prefix='native12-offline-comparator-') as temp:
        temp=Path(temp);source=temp/'source';snapshot=temp/'snapshot'
        stage=FROZEN/'logs/wp/webextras-boundary-arguments-callback-correction/staged'
        for relative in ('tests/recorder/diff.py','tests/recorder/download_identity.py','tests/recorder/google_q.py','tests/recorder/omnibox_receivers.py','tests/recorder/app_callbacks.inc','arc/tools/arccore-1.170.0-objc.tsv'):
            target=source/relative;target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(stage/relative,target)
        target=source/'arc/browser/webextras_fixture_inputs.py';target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(FROZEN/'arc/browser/webextras_fixture_inputs.py',target)
        assert sha(source/'tests/recorder/diff.py')=='24437bed30fddc8b94ab5c9dcc7cd20e755b06c0b05a4d8620e487d3c7f612cf'
        state=freezer.freeze(source,snapshot,IMAGE)
        assert len(state['files'])==7 and state['capability']['verified']
        write(OUT/'supported-temp-manifest.json',state)
        support=helper.capability(snapshot,IMAGE);assert support['verified'];write(OUT/'supported-four-canaries.json',support)
        for span in events['spans']:
            kind=span['kind'];left=NATIVE/'trace156'/('frontend-'+kind+'.callbacks.record.jsonl')
            observation=span['observation'];begin=observation['recordBegin'];end=observation['recordEnd']
            assert left.read_bytes()==raw[begin:end],kind
            data=rows(left);matched=pairs(data);known=helper.callback_metadata(snapshot)
            typed=helper.gate(left,IMAGE,known_callbacks=known)
            calls=[row for row in data if row['ev']=='call']
            owned=[row for row in calls if helper.owned_callback(row)]
            assert len(owned)==1 and owned[0]['args'][1]==span['expectedMode']
            assert span['rawValidated'] and typed['callbacks']==span['rawCallbackCounts']['total']
            app_calls=[row for row in calls if row['dir']=='->engine' and row['from']=='app']
            app_nested=[row for row in app_calls if row['seq']>owned[0]['seq'] and row['depth']>owned[0]['depth']]
            setters=[(a,b) for a,b in matched if data[a]['dir']=='->engine' and data[a]['sel'].startswith('set') and data[a]['sel'].endswith(':') and data[a]['depth']>owned[0]['depth']]
            inventory.append(dict(kind=kind,rawRecordBegin=begin,rawRecordEnd=end,rawSliceSHA256=sha(left),typed=typed,mode=owned[0]['args'][1],nestedAppCalls=len(app_nested),appCallsTotal=len(app_calls),nestedAppSetters=sum(data[a]['from']=='app' for a,b in setters),nestedEngineSetters=sum(data[a]['from']=='engine' for a,b in setters)))
            legacy_result=execute(helper,legacy,left,left,'legacy-'+kind+'-self')
            assert not legacy_result['verified'] and not any(legacy_result['sections'].values())
            results.append(legacy_result)
            clean=execute(helper,snapshot,left,left,'supported-'+kind+'-self')
            assert clean['verified'] and not any(clean['sections'].values())
            assert clean['callbackSequence']['golden']==typed['callbacks']
            results.append(clean)
            getters=[(a,b) for a,b in matched if data[a]['dir']=='->engine' and data[a]['from']=='app' and data[a]['cls']=='ArcSearchEngine' and data[a]['sel']=='url' and data[a]['depth']>owned[0]['depth'] and b==a+1]
            a,b=getters[0]
            mutations=[]
            changed=copy.deepcopy(data);old=changed[b]['ret'];changed[b]['ret']='https://webextras-diagnostic.invalid/query?q=%s'
            mutations.append(('nested-getter-return',changed,'return_mismatch',dict(rowIndex=b,seq=data[b]['seq'],old=old,new=changed[b]['ret'])))
            count_a,count_b=getters[-1]
            changed=[row for index,row in enumerate(data) if index not in (count_a,count_b)]
            mutations.append(('nested-call-count',changed,'count_mismatch',dict(removedRowIndices=[count_a,count_b],removedSeq=[data[count_a]['seq'],data[count_b]['seq']])))
            if setters:
                a,b=next(((a,b) for a,b in setters if data[a]['sel']=='setVisibility:'),setters[0])
                changed=copy.deepcopy(data);old=copy.deepcopy(changed[a]['args']);changed[a]['args'][0]=old[0]+1 if type(old[0]) is int else str(old[0])+' [offline diagnostic]'
                mutations.append(('nested-engine-setter-argument',changed,'arg_mismatch',dict(rowIndex=a,seq=data[a]['seq'],caller=data[a]['from'],selector=data[a]['sel'],old=old,new=changed[a]['args'])))
            ambient=[i for i,row in enumerate(data) if row['ev']=='call' and row['dir']=='engine->app' and not helper.owned_callback(row)]
            if ambient:
                a=ambient[0];changed=copy.deepcopy(data);old=copy.deepcopy(changed[a]['args']);changed[a]['args'][-1]=str(old[-1])+' [offline diagnostic]'
                mutations.append(('ambient-callback-argument',changed,'arg_mismatch',dict(rowIndex=a,seq=data[a]['seq'],selector=data[a]['sel'],old=old,new=changed[a]['args'])))
            for label,changed,expected,change in mutations:
                name=kind+'-'+label;right=OUT/'negative-recordings'/(name+'.jsonl');save_rows(right,changed)
                # Original seq IDs, callers, receiver tokens, ordering and unedited values remain verbatim.
                helper.gate(right,IMAGE,known_callbacks=known)
                result=execute(helper,snapshot,left,right,'supported-'+name)
                assert result['verified'] and result['sections']=={key:int(key==expected) for key in SECTIONS},result
                assert result['callbackSequence']['golden']==typed['callbacks'] and result['callbackSequence']['ours']==typed['callbacks']
                assert result['callbackSequence']['first_divergence'] is None
                results.append(result);provenance.append(dict(name=name,source=str(left.relative_to(OUT)),sourceSHA256=sha(left),derivedSHA256=sha(right),change=change,expectedSection=expected))
        ours_event=json.loads((NATIVE/'ours156/frontend-events.json').read_text())['spans'][0]
        observation=ours_event['observation'];begin=observation['recordBegin'];end=observation['recordEnd']
        ours=OUT/'ours-inspect-on-actual-raw-slice.jsonl';ours.write_bytes((NATIVE/'ours156/record.jsonl').read_bytes()[begin:end])
        ours_rows=rows(ours);pairs(ours_rows)
        callbacks=[row for row in ours_rows if row['ev']=='call' and row['dir']=='engine->app']
        assert len(callbacks)==6 and not any(helper.owned_callback(row) for row in callbacks)
        try:helper.gate(ours,IMAGE,known_callbacks=helper.callback_metadata(snapshot))
        except ValueError as error:failure=str(error)
        else:raise AssertionError('ambient-only ours evidence admitted')
        assert failure=='no owned DevTools callback evidence: total=6, ambient=6 (all raw callbacks retained)',failure
        write(OUT/'ours-coverage-rejection.json',dict(recordBegin=begin,recordEnd=end,sha256=sha(ours),rows=len(ours_rows),ownedCallbacks=0,ambientCallbacks=6,error=failure,expired=observation['expired'],producer=ours_event['producer'],rawError=ours_event.get('rawError')))
        assert freezer.verify(snapshot)==state and helper.verify(legacy)==legacy_state
        shutil.copytree(source,OUT/'archived-temp-source')
        shutil.copytree(snapshot,OUT/'archived-temp-snapshot')
    # Archived manifests preserve the original TEMP identities and are evidence,
    # not relocated runnable snapshots. Fresh replay always freezes a new TEMP.
    assert inputs_unchanged()==count
    summary=dict(scope='conditional comparator and call-shape proof only; no Native12 boundary acceptance',complete=False,parity=False,inputFiles=count,
                 legacy=dict(files=4,verified=False,status='pending-callback-boundary-comparison-support'),
                 supportedTEMP=dict(files=7,verified=True,comparatorSHA256='24437bed30fddc8b94ab5c9dcc7cd20e755b06c0b05a4d8620e487d3c7f612cf',fourCanaries=True),
                 originalSpans=inventory,comparisons=results,negativeProvenance=provenance,oursCoverageRejected=True,
                 limitations=['No nested app setter exists in these spans; actual engine setters tested where present.',
                              'Original raw recordings retained byte-for-byte; negative controls are distinct derivatives with original seq IDs.',
                              'No added normalization, filtering, receiver renumbering, timing repair, app/runtime invocation, or shared source change.'])
    write(OUT/'diagnostic-summary.json',summary)
    lines=[]
    for result in results:
        lines.extend([result['name']+' [opt-in comparator standard output]',*result['firstTwoLines'],result['name']+' [absent opt-in / default output]',*result['defaultFirstTwoLines'],''])
    (OUT/'first-two-lines.txt').write_text('\n'.join(lines))
    print(json.dumps(dict(comparisons=len(results),negativeControls=len(provenance),inputsUnchanged=count,legacySupport=False,supportedTEMP=True,oursCoverageRejected=True)))

if __name__=='__main__':main()
