#!/bin/bash
# Fixture-only transport probe; refuses non-disposable bundles and shares build/run lock.
set -euo pipefail
E="$(cd "$(dirname "$0")/../.." && pwd)"
if [ "${1:-}" != --locked ]; then exec "$E/arc/tools/locked.sh" bash "$0" --locked "$@"; fi
shift
mode="${1:-cold}"
agent="${2:-trace156}"
case "$agent" in trace156|ours156) ;; *) echo 'only trace156/ours156 allowed' >&2; exit 2;; esac
build="$E/logs/wp/ai-service-probe-build"
out="$E/results/ai-service-probe/$agent/$mode"
if [ "${3:-}" = --embeddings-only ]; then
  export ARC_AI_PROBE_EMBEDDINGS_ONLY=1
  out="$out-embeddings-only"
elif [ "${3:-}" = --stream-only ]; then
  export ARC_AI_PROBE_STREAM_ONLY=1
  out="$out-stream-only"
fi
mkdir -p "$build" "$out"
clang++ -dynamiclib -fobjc-arc -Wall -Wextra -Werror -Wno-unused-variable -framework Foundation \
  "$E/arc/browser/ai_service_fixture.mm" "$E/arc/browser/ai_conversion_fixture.mm" -o "$build/ArcAIServiceFixture.dylib"
codesign --force --sign - "$build/ArcAIServiceFixture.dylib"
clang++ -dynamiclib -fobjc-arc -Wall -Wextra -Werror -Wno-unused-function -framework Foundation \
  "$E/arc/browser/ai_service_probe.mm" -o "$build/ArcAIServiceProbe.dylib"
clang++ -DARC_AI_PROBE_STANDALONE -fobjc-arc -Wall -Wextra -Werror -Wno-unused-function -framework Foundation \
  "$E/arc/browser/ai_service_probe.mm" -o "$build/probe"
app="$E/../arc/build/ArcModAgent-$agent.app"
exe="$app/Contents/MacOS/Arc"
framework="$app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore"
export ARC_AI_FIXTURE_LOG="$out/fixture-direct.jsonl"
export ARC_AI_PROBE_FIXTURE="$build/ArcAIServiceFixture.dylib"
export ARC_AI_PROBE_OUTPUT="$out/events.json"
# Original utility sandbox permits executable mappings inside its disposable bundle.
# This unique test file is removed before releasing the shared lock.
if [ "$mode" = injected ]; then
  bundle_fixture="$app/Contents/Frameworks/ArcCore.framework/Versions/A/Libraries/ArcAIServiceFixture-ai-probe.dylib"
  if [ -e "$bundle_fixture" ]; then echo "fixture path already exists; refusing overwrite: $bundle_fixture" >&2; exit 2; fi
  mkdir -p "$(dirname "$bundle_fixture")"
  cp "$build/ArcAIServiceFixture.dylib" "$bundle_fixture"
  trap 'rm -f "$bundle_fixture"' EXIT
  export ARC_AI_PROBE_FIXTURE="$bundle_fixture"
fi
rm -f "$ARC_AI_FIXTURE_LOG" "$out/fixture.jsonl" "$ARC_AI_PROBE_OUTPUT" "$ARC_AI_PROBE_OUTPUT.ready.json" "$out/run.json" "$out/termination.json" "$out/utility-processes.json"
: > "$out/app.log"
if [ "$mode" = build ]; then echo "$build"; exit 0; fi
if [ "$mode" = cold ]; then
  python3 - "$build/probe" "$framework" "$ARC_AI_PROBE_FIXTURE" "$ARC_AI_PROBE_OUTPUT" "$out/app.log" <<'PY'
import os,subprocess,sys
with open(sys.argv[5],'w') as log:
    try:
        result=subprocess.run(sys.argv[1:5],stdout=log,stderr=log,timeout=35)
        print('cold probe exit:',result.returncode)
    except subprocess.TimeoutExpired:
        print('cold probe timeout after 35 seconds; use injected mode with initialized browser')
PY
elif [ "$mode" = injected ]; then
  python3 - "$app" "$out" "$build/ArcAIServiceProbe.dylib" <<'PY'
import json,os,pathlib,plistlib,re,subprocess,sys,time
app=pathlib.Path(sys.argv[1]).resolve();out=pathlib.Path(sys.argv[2]);exe=str(app/'Contents/MacOS/Arc')
plist=plistlib.loads((app/'Contents/Info.plist').read_bytes());agent=plist['ArcModAgentSession']
assert agent in ('trace156','ours156') and plist['CFBundleIdentifier']=='company.thebrowser.Browser.mod.agent.'+agent
embeddings_only='ARC_AI_PROBE_EMBEDDINGS_ONLY' in os.environ
expect_fatal=embeddings_only and agent=='ours156'
track_utilities=embeddings_only and agent=='trace156'
utility_prefix=str(app/'Contents')+'/'
utility_flag=re.compile(r'(?:^|\s)--utility-sub-type=ai\.mojom\.AIService(?:\s|$)')
utility_started=time.monotonic()
utility_timeline={'mode':'injected-embeddings-only','agent':agent,'app':str(app),'poll_interval_seconds':.02,'samples':[]}
def processes():
    rows=subprocess.check_output(['ps','-axo','pid=,command='],text=True).splitlines()
    return {int(parts[0]):parts[1] for row in rows if len(parts:=row.strip().split(maxsplit=1))==2}
def owns(command):return command==exe or command.startswith(exe+' ')
def utility_processes(current):
    return {pid:command for pid,command in current.items() if command.startswith(utility_prefix) and utility_flag.search(command)}
def utility_sample(current,events,main_pid):
    if not track_utilities:return
    found=utility_processes(current)
    last={key:events[-1][key] for key in ('method','phase')} if events else None
    row={'elapsed_monotonic_seconds':round(time.monotonic()-utility_started,6),'utility_pids':sorted(found),'commands':{str(pid):command for pid,command in found.items()},'boundary_last':last,'embeddings_call_seen':any(e['method']=='embeddings' and e['phase']=='call' for e in events),'main_pid':main_pid,'main_alive':main_pid is not None and owns(current.get(main_pid,''))}
    previous=utility_timeline['samples'][-1] if utility_timeline['samples'] else None
    if previous is None or any(row[key]!=previous[key] for key in ('utility_pids','boundary_last','main_pid','main_alive')):
        utility_timeline['samples'].append(row)
        (out/'utility-processes.json').write_text(json.dumps(utility_timeline,indent=2))
def verify_utility_turnover():
    samples=utility_timeline['samples'];initial=None;death=None;replacement=None
    for index,row in enumerate(samples):
        if initial is None and row['utility_pids'] and not row['embeddings_call_seen'] and row['main_alive']:
            initial=(index,set(row['utility_pids']))
        if initial is not None and row['embeddings_call_seen'] and row['main_alive']:
            removed=initial[1]-set(row['utility_pids'])
            if removed and death is None:death=(index,removed)
            added=set(row['utility_pids'])-initial[1]
            if death is not None and added and index>=death[0]:
                replacement=(index,added);break
    errors=[]
    if initial is None:errors.append('unclear: initial scoped AI utility not sampled before embeddings')
    if death is None:errors.append('unclear: initial AI utility exit after embeddings not observed while main app alive')
    if replacement is None:errors.append('unclear: replacement AI utility not observed while main app alive')
    summary={'verified':not errors,'initial_sample':initial[0] if initial else None,'initial_pids':sorted(initial[1]) if initial else [],'exit_sample':death[0] if death else None,'exited_pids':sorted(death[1]) if death else [],'replacement_sample':replacement[0] if replacement else None,'replacement_pids':sorted(replacement[1]) if replacement else [],'errors':errors}
    utility_timeline['turnover']=summary
    (out/'utility-processes.json').write_text(json.dumps(utility_timeline,indent=2))
    assert not errors,'; '.join(errors)

def read_events():
    try:return json.loads(pathlib.Path(os.environ['ARC_AI_PROBE_OUTPUT']).read_text())
    except (FileNotFoundError,json.JSONDecodeError):return []
def verified_ready(pid):
    try:
        ready=json.loads((out/'events.json.ready.json').read_text())
        return ready.get('pid')==pid and ready.get('installed') is True
    except (FileNotFoundError,json.JSONDecodeError):return False
def fatal_checkpoint(events):
    initial=[e for e in events if e['method']=='initialize' and e['phase']=='callback']
    assert len(initial)==1 and initial[0]['value']==-1, 'fatal probe: initialization did not succeed'
    assert any(e['method']=='isAvailable.after' and e['value'] is True for e in events), 'fatal probe: unavailable service'
    for phase in ('call','return'):
        assert sum(e['method']=='embeddings' and e['phase']==phase for e in events)==1, 'fatal probe: missing or duplicate embeddings '+phase
    assert not any(e['method'].startswith('embeddings') and e['phase']=='callback' for e in events), 'fatal probe: unexpected embeddings callback'
    assert not any(e['method']=='embeddings.error' for e in events), 'fatal probe: termination occurred after a different operation'
    assert not any(e['method']=='embeddings' and e['phase']=='timeout' for e in events), 'fatal probe: first operation timed out before exit'
    assert not any(e['method']=='probe' and e['phase']=='finished' for e in events), 'fatal probe: completed without expected termination'
before_launch=processes()
assert not any(owns(command) for command in before_launch.values()),'agent already running; refusing to attach'
if track_utilities:
    assert not utility_processes(before_launch),'preexisting scoped AI utility; refusing ambiguous ownership'
    utility_sample(before_launch,[],None)
data=pathlib.Path.home()/'Library/Application Support'/('ArcModAgent-'+agent)/'Agent Chromium Data'
data.mkdir(parents=True,exist_ok=True)
cmd=['open','-n','-g','-a',str(app),'--env','DYLD_INSERT_LIBRARIES='+sys.argv[3]]
for key in ('ARC_AI_FIXTURE_LOG','ARC_AI_PROBE_FIXTURE','ARC_AI_PROBE_OUTPUT','ARC_AI_PROBE_EMBEDDINGS_ONLY','ARC_AI_PROBE_STREAM_ONLY'):
    if key in os.environ:cmd+=['--env',key+'='+os.environ[key]]
cmd+=['--stdout',str(out/'app.log'),'--stderr',str(out/'app.log'),'--args','--use-mock-keychain','--no-default-browser-check','--disable-sync','--disable-background-networking','--host-resolver-rules='+plist['ArcModHostResolverRules'],'--user-data-dir='+str(data)]
subprocess.run(cmd,check=True)
pid=None
try:
    deadline=time.monotonic()+100
    while time.monotonic()<deadline:
        current=processes();matches=[candidate for candidate,command in current.items() if owns(command)]
        if pid is None and matches:
            assert len(matches)==1, 'multiple newly launched agent processes'
            pid=matches[0]
            (out/'run.json').write_text(json.dumps({'pid':pid,'agent':agent,'app':str(app),'fixture':os.environ['ARC_AI_PROBE_FIXTURE']}))
        if pid is not None and pid in current and not owns(current[pid]):
            raise SystemExit('owned PID was reused; refusing termination acceptance or cleanup')
        events=read_events()
        utility_sample(current,events,pid)
        gone=pid is not None and pid not in current
        if gone:
            if not expect_fatal:raise SystemExit('unexpected exit of probe-owned app')
            assert verified_ready(pid), 'fatal probe: PID was not verified by injected ready file'
            fatal_checkpoint(events)
            (out/'termination.json').write_text(json.dumps({'mode':'injected-embeddings-only','pid':pid,'agent':agent,'observedexit':True,'checkpoint':'initialized embeddings call/return; no callback'}))
            print('verified owned embeddings process exit:',pid)
            break
        if any(e['method']=='probe' and e['phase']=='finished' for e in events):
            assert pid is not None and verified_ready(pid), 'completed probe has no verified owned PID'
            if expect_fatal:raise SystemExit('ours embeddings completed without the expected process exit')
            if track_utilities:verify_utility_turnover()
            if embeddings_only:
                (out/'termination.json').write_text(json.dumps({'mode':'injected-embeddings-only','pid':pid,'agent':agent,'observedexit':False,'checkpoint':'original app remained alive through finished marker'}))
            break
        time.sleep(.02 if track_utilities else .2)
    else:raise SystemExit('injected probe timeout; inspect app.log and partial events')
    print('injected fixture probe complete:',out)
finally:
    # Never signal a PID unless it still names the exact process we launched.
    if pid and owns(processes().get(pid,'')):
        subprocess.run(['kill','-TERM',str(pid)],check=False)
        deadline=time.monotonic()+5
        while time.monotonic()<deadline:
            current=processes()
            if pid not in current or not owns(current[pid]):break
            time.sleep(.1)
        else:
            if owns(processes().get(pid,'')):subprocess.run(['kill','-KILL',str(pid)],check=False)
PY
else echo 'usage: ai_service_probe.sh [build|cold|injected] [trace156|ours156]' >&2; exit 2
fi
# Fail clearly when only unavailable-state behavior was observed.
python3 - "$out" "$agent" <<'PY'
import json,pathlib,sys
p=pathlib.Path(sys.argv[1]);events=json.loads((p/'events.json').read_text())
def callback(method):
    rows=[e['value'] for e in events if e['method']==method and e['phase']=='callback']
    if len(rows)!=1:raise SystemExit(f'{method}: expected one callback, got {len(rows)}')
    return rows[0]
assert callback('initialize')==-1, 'library initialize did not succeed; see partial events and app.log'
assert next(e['value'] for e in events if e['method']=='isAvailable.after') is True
if 'ARC_AI_PROBE_STREAM_ONLY' in __import__('os').environ:
    rows=[e for e in events if e['method']=='stream.spaced' and e['phase'] in ('token','partial','callback')]
    assert [(e['phase'],e['value']) for e in rows]==[('token',0),('partial','spaced-first-π'),('partial','spaced-second-東京'),('callback',10)], 'spaced stream ordering/count/value mismatch'
    assert all(e['mainThread'] is True for e in rows), 'spaced stream delivered off main thread'
    assert rows[0]['synchronous'] is True and all(e['synchronous'] is False for e in rows[1:]), 'spaced stream delivery timing mismatch'
    print('spaced stream: exact token, two partials, completion, thread and asynchronous delivery verified')
    raise SystemExit(0)
if 'ARC_AI_PROBE_EMBEDDINGS_ONLY' in __import__('os').environ:
    termination=json.loads((p/'termination.json').read_text())
    run=json.loads((p/'run.json').read_text())
    assert termination['mode']=='injected-embeddings-only' and termination['pid']==run['pid'] and termination['agent']==sys.argv[2]
    if sys.argv[2]=='ours156':
        assert termination['observedexit'] is True, 'ours embeddings process did not exit'
        for phase in ('call','return'):
            assert sum(e['method']=='embeddings' and e['phase']==phase for e in events)==1
        assert not any(e['method'].startswith('embeddings') and e['phase']=='callback' for e in events), 'unexpected embeddings callback'
        assert not any(e['method']=='embeddings.error' for e in events)
        print('ours legacy embeddings: verified owned process exited after initialized call/return with no callback')
    else:
        assert termination['observedexit'] is False, 'original app unexpectedly exited'
        for method in ('embeddings','embeddings.error'):
            assert sum(e['method']==method and e['phase']=='timeout' for e in events)==1, method+': expected one timeout'
            assert not any(e['method']==method and e['phase']=='callback' for e in events), method+': callback unexpectedly delivered'
            for phase in ('call','return'):
                assert sum(e['method']==method and e['phase']==phase for e in events)==1
        assert any(e['method']=='probe' and e['phase']=='finished' for e in events)
        timeline=json.loads((p/'utility-processes.json').read_text())
        turnover=timeline['turnover']
        assert turnover['verified'] is True and not turnover['errors'], 'utility death/replacement is unclear'
        assert turnover['exited_pids'] and turnover['replacement_pids']
        assert not set(turnover['initial_pids']) & set(turnover['replacement_pids'])
        assert turnover['initial_sample']<turnover['exit_sample']<=turnover['replacement_sample']
        for index in ('initial_sample','exit_sample','replacement_sample'):
            assert timeline['samples'][turnover[index]]['main_alive'] is True
        assert timeline['samples'][turnover['exit_sample']]['embeddings_call_seen'] is True
        available=next(e['value'] for e in events if e['method']=='embeddings.afterFirst.available')
        print('original legacy embeddings: both callbacks dropped; app survived; scoped AI utility turnover verified; after-first availability=',available)
    raise SystemExit(0)
if 'ARC_AI_PROBE_EMBEDDINGS_ONLY' not in __import__('os').environ:
    assert callback('type')==[4,-1]
    assert callback('type.beforeInitialize')==[-1,3]
    assert callback('setUser.beforeInitialize')==3
    assert callback('initialize.again')==4
    assert callback('initialize.error')==1
    assert next(e['value'] for e in events if e['method']=='isAvailable.afterError') is True
    assert callback('type.afterError')==[4,-1]
    assert callback('setUser.afterError')==-1
    assert callback('stream.error')==10
    assert next(e['value'] for e in events if e['method']=='released.owner') is True
    assert next(e['value'] for e in events if e['method']=='released.setUser' and e['phase']=='callbackObserved') is False
    assert callback('classification')==[[.125,-7.5,31.25],-1]
    assert callback('nomic')==[[2,3],[.125,-7.5,31.25,4,5,6],-1]
    assert callback('rank')==[[{'candidateID':'fixture-B','score':-7.5},{'candidateID':'fixture-A','score':.125}],-1]
    assert callback('rank.async')==callback('rank')
    result,error=callback('conversion.convert')
    assert error==-1 and result['body']=='markdown-π' and result['title']=='title-東京'
    assert result['references']==[{'referenceText':'reference-π','minimumCharacterCount':37}]
    assert result['selectorsApplied']==[] and result['selectorsContentPct']==73.125
    assert result['metaTags']==[{'key':'og:title','value':'tag-東京'},{'key':'description','value':'fixture-description'}]
    print('conversion async observation:',callback('conversion.async'))
    assert callback('conversion.async')==[None,8]
    assert callback('conversion.afterAsync.setUser')==-1
    records=[json.loads(line) for line in (p/'fixture.jsonl').read_text().splitlines()]
    rank=next(r['args'] for r in records if r['method']=='RankCommandBar')
    assert rank['query']==[.125,-7.5,31.25]
    assert rank['candidates'][0]=={'id':'fixture-A','archetype':'tab','destination':'current','title':'title-π','url':'https://example.invalid/π','lastNavSeconds':17,'freqEver':29,'typedCount':31,'siteEngagement':.125,'isExactUrlMatch':True,'isBaseDomainMatch':False,'isTitlePrefix':True,'embedding':[.125,-7.5,31.25]}
    conversion=next(r['args'] for r in records if r['method']=='Conversion.ConvertHtmlToMarkdown')
    assert conversion=={'html':'<main>fixture-π</main>','url':'https://example.invalid/path','linkStyle':1,'strategy':2,'urlTransform':1,'skipNoscript':True,'passThroughInputElements':False,'deduplicateMetaTagContent':True,'metaTags':[{'kind':0,'value':'og:title'},{'kind':1,'value':'description'}]}
    required={'Initialize','Type','SetUser','EnableDataDogTracing','LoadInferenceModel','GetStreamedInferenceFor','LoadClassifier','IsClassifierLoaded','GetClassificationFor','LoadNomicEmbedText','NomicEmbed','LoadCommandBarRanking','RankCommandBar','EvaluateCommandBarRanking','TrainCommandBarRanking','Crash','Conversion.Initialize','Conversion.SetUser','Conversion.ConvertHtmlToMarkdown','RankCommandBar.asyncCopy'}
    observed={r['method'] for r in records}
    assert not required-observed, f'missing fixture calls: {required-observed}'
    assert not any(r['method'] in ('UpdateFeatureFlags','WaitForDebuggerToAttach') for r in records), 'original engine does not forward these legacy ABI methods'
    print(sys.argv[2]+': fixture scalar/nested payloads and request arguments verified; callback ordering/thread observations in events.json')
PY
