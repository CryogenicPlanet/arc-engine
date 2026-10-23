import hashlib,json,shutil
from pathlib import Path
ROOT=Path('/Users/cryogenicplanet/general/arc-engine');OUT=ROOT/'results/webextras/native-run9-offline-diagnostic';RUN=ROOT/'results/webextras/native-run9'
def load(p):return json.loads(p.read_text())
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
state=load(OUT/'input-manifest.json');saved={k:sha(Path(v['source']['path'])) for k,v in state['files'].items()}
assert all(saved[k]==v['source']['sha256'] for k,v in state['files'].items())
guards={phase:load(ROOT/f'logs/wp/webextras.native.run9.source-{phase}.json') for phase in ['before','after-build','after-run']}
assert guards['before']['files']==guards['after-build']['files']==guards['after-run']['files']
assert all(x['probeMode']=='media-service-clock' and x['mediaResourceUsed'] is False for x in guards.values())
validation=load(OUT/'validation-and-projection.json');summ=load(OUT/'diagnostic-summary.json');shape_clean=payload_clean=comparator_clean=0
for action,diff in zip(summ['perActionShapeAndSemanticDifferences'],summ['perActionComparator']):
 shape_clean+=not action['projectedShapeDifferences'];comparator_clean+=not any(diff['mismatchCounts'].values()) and diff['callbackSequence']['first_divergence'] is None
 payload=[d for d in action['semanticDifferences'] if not d['path'].startswith('/traceShape')]
 action['outcomePayloadDifferences']=payload;action['outcomePayloadEqual']=not payload;payload_clean+=not payload
checks={}
for agent in ['trace156','ours156']:
 p=OUT/'frozen-recordings'/agent;obs=load(p/'observations.json');clock=load(p/'media-clock.json');rows=[json.loads(s)for s in (p/'record.jsonl').read_text().splitlines()];clockactions=obs['actions'][11:]
 fixture=[r for r in rows if r.get('dir')=='->engine' and r.get('from')=='WebextrasNativeProbe.dylib']
 assert sum(r['ev']=='call'for r in fixture)==115==sum(r['ev']=='ret'for r in fixture)
 assert len({a['ownerTag']for a in obs['actions']})==1
 positions=[r['self']for r in fixture if r['ev']=='call'and r['sel']=='position'];assert len(positions)==18 and len(set(positions))==1
 durations=[];slopes=[];dtotags=[]
 for i,phase in enumerate(clock['phases']):
  aa=clockactions[i*5:i*5+5];rate=[0,1,2,0,0,1.5][i];base=[1.25,1.25,1.25,2.25,7.25,7.25][i]
  dtotags.extend(a['clock']['dtoTag']for a in aa[:3]);assert len({a['clock']['dtoTag']for a in aa[2:]})==1
  for a in aa:
   assert a['ownerTag']==obs['actions'][0]['ownerTag'];factor=a['clock']['timebaseNumer']/a['clock']['timebaseDenom']/1e9
   for c in a['publicCalls']:
    b=int(c['clockBeforeTicks']);e=int(c['clockAfterTicks']);assert 0<=e-b;durations.append((e-b)*factor)
  for first,second in [(aa[2],aa[3]),(aa[3],aa[4]),(aa[2],aa[4])]:
   x=next(c for c in first['publicCalls']if c['selector']=='currentTime');y=next(c for c in second['publicCalls']if c['selector']=='currentTime')
   low=(int(y['clockBeforeTicks'])-int(x['clockAfterTicks']))*factor;high=(int(y['clockAfterTicks'])-int(x['clockBeforeTicks']))*factor;delta=y['value']-x['value'];epsilon=.000004*(1+abs(rate))
   assert low>=.2 and rate*low-epsilon<=delta<=rate*high+epsilon
   slopes.append({'phase':phase['phase'],'rate':rate,'first':first['actionKey'],'second':second['actionKey'],'delta':delta,'elapsedBracketSeconds':[low,high],'fixedEpsilon':epsilon,'marginsSeconds':[delta-(rate*low-epsilon),(rate*high+epsilon)-delta]})
  if rate==0:assert all(next(c['value']for c in a['publicCalls']if c['selector']=='currentTime')==base for a in aa)
 assert len(set(dtotags))==18 and len(durations)==102 and max(durations)<=.025
 evals=[json.loads(s)for s in (p/'clock-evaluations.jsonl').read_text().splitlines()];assert len(evals)==66
 assert all(e['label']=='prepare' or e['label'].startswith('attempt-')for e in evals)
 assert not (RUN/agent/'clock-activation.json').exists() and not (RUN/'media-clock-identity.json').exists() and not (RUN/'webextras-media-clock.webm').exists()
 checks[agent]={'fullRecordFixturePairs':115,'stableOwnerTag':obs['actions'][0]['ownerTag'],'rawSessionReceiver':positions[0],'freshSnapshotDtoTags':dtotags,'getterBracketsCount':len(durations),'maximumGetterBracketSeconds':max(durations),'slopes':slopes,'pausedExactAnchors':[1.25,2.25,7.25],'rawClockEvaluations':66,'activationArtifact':False,'mediaAssetArtifact':False,'noAssetActivationStimulus':'source branch and all raw evaluated expressions, not independent network monitoring','releases':6,'runtimeStatus':obs['runtimeStatus']}
summ['counts']={'actions':41,'comparatorClean':comparator_clean,'comparatorMismatch':41-comparator_clean,'projectedShapeClean':shape_clean,'projectedShapeMismatch':41-shape_clean,'outcomePayloadClean':payload_clean,'outcomePayloadMismatch':41-payload_clean}
(OUT/'diagnostic-summary.json').write_text(json.dumps(summ,indent=2)+'\n')
# Archive exact execution source, owned audit scripts and the runtime guard/pin input bytes.
execution=OUT/'archived-temp-execution';shutil.copytree(Path(state['temporarySource']),execution)
extra={}
for source in [OUT/'audit.py',OUT/'offline-harness.ts',OUT/'independent_checks.py',RUN/'summary.json',RUN/'recorder-identity.json',RUN/'ArcEngineRecorder.dylib',*[(ROOT/f'logs/wp/webextras.native.run9.source-{phase}.json')for phase in guards],ROOT/'logs/wp/webextras.native.run9.native-reference-pin.json']:
 destination=OUT/'additional-frozen-inputs'/source.relative_to(ROOT);destination.parent.mkdir(parents=True,exist_ok=True);before=sha(source);shutil.copyfile(source,destination);assert sha(source)==before==sha(destination);destination.chmod(0o444);extra[str(source.relative_to(ROOT))]={'sha256':before,'frozen':str(destination)}
assert all(sha(Path(v['source']['path']))==saved[k]==v['source']['sha256'] and sha(Path(v['frozen']['path']))==saved[k] for k,v in state['files'].items())
(OUT/'independent-checks.json').write_text(json.dumps({'status':'pass','runtimeGuardFiles':len(guards['before']['files']),'runtimeOwnedInputsStable':True,'checks':checks,'counts':summ['counts'],'additionalFrozenInputs':extra,'integrityRechecked':True},indent=2)+'\n')
print(json.dumps({'status':'pass','counts':summ['counts'],'additionalFrozenInputs':len(extra),'maxBrackets':{a:r['maximumGetterBracketSeconds']for a,r in checks.items()}},indent=2))
