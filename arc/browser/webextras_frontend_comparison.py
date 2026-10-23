#!/usr/bin/env python3
"""Owned nonempty callback-span comparator capability/runner. No record normalization."""
import argparse
import json
from pathlib import Path
import re
import subprocess
import sys
import tempfile
import types

SECTIONS=('missing','extra','return_mismatch','arg_mismatch','count_mismatch')
SELECTORS={'webContentsDevTools:displayAs:','webContentsDevTools:inspectModeDidChange:',
           'webContentsDevToolsDidClose:','webContentsDevToolsNeedsFocus:'}
IDENTITY=('dir','kind','cls','sel','self','thread','from')
ARGUMENT_METADATA={'mode':'selected-boundary-methods','caller_attribution':True}
FILENAME_CALLBACK='browserContext:onDeterminingFilename:defaultFilename:completionTimeout:completion:'
FILENAME_CLASS='ADK.BrowserContextDownloadDelegateBridge'
DOWNLOAD_GUID='00000000-0000-4000-8000-000000000001'


def require(value,message):
    if not value:
        raise ValueError(message)


def verify(snapshot):
    path=Path(snapshot)/'arc/browser/webextras_fixture_inputs.py'
    # Source-only: -B prevents writes but still reads unchecked-hash .pyc.
    # Never execute an unmanifested cache before the freezer validates its inputs.
    module=types.ModuleType('frozen_fixture_inputs');module.__file__=str(path)
    exec(compile(path.read_bytes(),str(path),'exec'),module.__dict__)
    return module.verify(snapshot)


def callback_metadata(snapshot):
    known=set()
    for line in (Path(snapshot)/'tests/recorder/app_callbacks.inc').read_text().splitlines():
        match=re.fullmatch(r'\{"([^"\s]+)", "([^"\s]+)"\},?',line.strip())
        if not match:continue
        native,selector=match.groups();known.add((native,selector))
        if not native.startswith('_TtC'):continue
        rest=native[5 if native.startswith('_TtCC') else 4:];parts=[]
        while rest:
            length=re.match(r'([1-9]\d*)',rest)
            if not length:parts=[];break
            count=int(length[1]);rest=rest[len(length[1]):]
            if count>len(rest):parts=[];break
            parts.append(rest[:count]);rest=rest[count:]
        if len(parts)>=2:known.add(('.'.join(parts),selector))
    require(known,'empty frozen callback metadata');return known


def owned_callback(event):
    return event.get('cls')=='ADK.WebContentChromium' and event.get('sel') in SELECTORS


def gate(path,image,filename_canary=False,known_callbacks=None):
    data=Path(path).read_bytes()
    require(data and data.endswith(b'\n'),'empty/incomplete callback span')
    rows=[json.loads(line) for line in data.splitlines()]
    seen=set();stacks={};bases={};callbacks=0;owned=0;ambient=[];service=None
    require(known_callbacks is not None,"frozen callback metadata required")
    for event in rows:
        require(type(event.get('seq')) is int and event['seq']>=0 and event['seq'] not in seen,'duplicate/invalid callback span ID');seen.add(event['seq'])
        require(event.get('ev') in ('call','ret'),'callback span contains non-boundary row')
        caller=event.get('from');require(isinstance(caller,str) and caller and caller.strip()==caller and not any(ord(c)<32 for c in caller),'missing callback caller')
        require(not(event.get('dir')=='->engine' and caller==image),'callback span contains fixture public calls')
        require(event.get('dir') in ('->engine','engine->app') and event.get('kind') in ('-','+') and isinstance(event.get('thread'),str) and bool(event['thread']) and type(event.get('depth')) is int and event['depth']>=0,'invalid callback boundary identity')
        require(isinstance(event.get('cls'),str) and isinstance(event.get('sel'),str) and (event.get('self') is None if event['kind']=='+' else isinstance(event.get('self'),str) and re.fullmatch(r'<[^<>]+#\d+>',event['self'])),'invalid callback receiver identity')
        stack=stacks.setdefault(event['thread'],[])
        if event['ev']=='call':
            bases.setdefault(event['thread'],event['depth'])
            require(event['depth']==bases[event['thread']]+len(stack) and isinstance(event.get('args'),list),'callback span call nesting invalid')
            stack.append(event)
            if event['dir']=='engine->app':
                require(caller=='engine' and event['kind']=='-' and (event['cls'],event['sel']) in known_callbacks,'unknown/wrong-origin callback')
                require(len(event['args'])==event['sel'].count(':'),'callback argument count differs from frozen selector')
                args=event['args']
                if filename_canary:
                    require(event.get('sel')==FILENAME_CALLBACK and event.get('cls')==FILENAME_CLASS and
                            len(args)==5 and args[0]=='<ArcBrowserContext#1>' and args[1]=='<ArcDownloadItem#1>' and
                            args[2]=='<ptr>' and type(args[3]) is int and args[3] in (15,16) and args[4]=='<block>',
                            'invalid literal filename callback canary')
                elif owned_callback(event):
                    require(event['thread']=='main','owned callback off main thread')
                    owned+=1
                    modeful=event['sel'] in ('webContentsDevTools:displayAs:','webContentsDevTools:inspectModeDidChange:')
                    require(len(args)==(2 if modeful else 1) and isinstance(args[0],str) and re.fullmatch(r'<ArcWebContentsDevTools#\d+>',args[0]),'invalid callback service/payload')
                    require(not modeful or(type(args[1]) is int and 0<=args[1]<=4),'invalid callback mode')
                    service=service or args[0];require(service==args[0],'callback service changed')
                else:
                    ambient.append({'cls':event['cls'],'selector':event['sel'],'thread':event['thread']})
                callbacks+=1
        else:
            require(stack and 'ret' in event,'callback span unmatched return')
            call=stack.pop();require(call['seq']<event['seq'] and event['depth']==bases[event['thread']]+len(stack) and all(call.get(key)==event.get(key) for key in IDENTITY),'callback span return identity mismatch')
            if event['dir']=='engine->app' and (owned_callback(event) or filename_canary):require(event['ret'] is None,'callback void return is not null')
    require(callbacks>0 and not any(stacks.values()),'zero/incomplete callback evidence')
    if filename_canary:
        require(len(rows)==4 and rows[0].get('dir')=='->engine' and rows[0].get('from')=='engine' and rows[0].get('cls')=='ArcDownloadItem' and
                rows[0].get('sel')=='id' and rows[0].get('self')=='<ArcDownloadItem#1>' and rows[0].get('args')==[] and rows[1].get('ret')==DOWNLOAD_GUID,
                'filename callback lacks exact raw download GUID proof')
    require(filename_canary or owned>0,f'no owned DevTools callback evidence: total={callbacks}, ambient={len(ambient)} (all raw callbacks retained)')
    return {'callbacks':callbacks,'ownedCallbacks':owned,'ambientCallbacks':len(ambient),'ambientIdentities':ambient,'rows':len(rows),'fixturePublicCalls':0,'service':service}


def run_diff(snapshot,left,right,report):
    # Explicit argument option negotiation; legacy CLI may ignore it. Canaries+
    # metadata must prove support. Never pass --fixture-image for zero-public spans.
    return subprocess.run([sys.executable,'-B',str(Path(snapshot)/'tests/recorder/diff.py'),str(left),str(right),
                           '--compare-boundary-arguments','--json',str(report)],capture_output=True,text=True,timeout=20)


def canary_rows(kind,value=None,count=1):
    rows=[]
    def event(ev,depth,cls,selector,receiver,caller,**fields):
        rows.append(dict(seq=len(rows)+1,ev=ev,depth=depth,cls=cls,sel=selector,self=receiver,thread='main',kind='-',dir='engine->app' if cls=='ADK.WebContentChromium' else '->engine',**{'from':caller},**fields))
    event('call',0,'ADK.WebContentChromium','webContentsDevTools:inspectModeDidChange:','<ADK.WebContentChromium#8>','engine',args=['<ArcWebContentsDevTools#9>',1])
    for _ in range(count):
        cls,selector,receiver=('ArcWebContents','setTitle:','<ArcWebContents#118>') if kind=='setter' else ('ArcWebContentsDevTools','webContents','<ArcWebContentsDevTools#9>')
        event('call',1,cls,selector,receiver,'app',args=[value] if kind=='setter' else [])
        event('ret',1,cls,selector,receiver,'app',ret=None if kind=='setter' else value)
    event('ret',0,'ADK.WebContentChromium','webContentsDevTools:inspectModeDidChange:','<ADK.WebContentChromium#8>','engine',ret=None)
    return rows


def filename_canary_rows(timeout):
    # Exact real callback signature; literal '<ptr>' is an NSString filename,
    # not a synthetic opaque DevTools argument. The native internal id read
    # supplies the raw GUID proof used by the existing download identity policy.
    common=dict(thread='main',depth=0,kind='-',**{'from':'engine'})
    item=dict(common,dir='->engine',cls='ArcDownloadItem',sel='id',self='<ArcDownloadItem#1>')
    callback=dict(common,dir='engine->app',cls=FILENAME_CLASS,sel=FILENAME_CALLBACK,self='<ADK.BrowserContextDownloadDelegateBridge#1>')
    return [dict(item,seq=1,ev='call',args=[]),dict(item,seq=2,ev='ret',ret=DOWNLOAD_GUID),
            dict(callback,seq=3,ev='call',args=['<ArcBrowserContext#1>','<ArcDownloadItem#1>','<ptr>',timeout,'<block>']),
            dict(callback,seq=4,ev='ret',ret=None)]


def capability(snapshot,image):
    state=verify(snapshot);require(state["image"]==image,"callback image differs from frozen snapshot");cases=[];known=callback_metadata(snapshot)
    require('M\tArcDownloadItem\t-\tid\t@16@0:8' in (Path(snapshot)/'arc/tools/arccore-1.170.0-objc.tsv').read_text().splitlines(),'filename canary id ABI absent from frozen metadata')
    require('{"_TtC3ADK36BrowserContextDownloadDelegateBridge", "'+FILENAME_CALLBACK+'"},' in (Path(snapshot)/'tests/recorder/app_callbacks.inc').read_text().splitlines(),'filename canary callback absent from frozen callback metadata')
    with tempfile.TemporaryDirectory(prefix='webextras-callback-capability-') as directory:
        root=Path(directory)
        specs=[('nested-getter-return',canary_rows('getter','<ArcWebContents#118>'),canary_rows('getter',None),'return_mismatch'),
               ('nested-app-setter-argument',canary_rows('setter','alpha'),canary_rows('setter','beta'),'arg_mismatch'),
               ('nested-public-call-count',canary_rows('getter','<ArcWebContents#118>'),canary_rows('getter','<ArcWebContents#118>',2),'count_mismatch'),
               ('literal-filename-callback-argument',filename_canary_rows(15),filename_canary_rows(16),'arg_mismatch')]
        for name,left,right,expected in specs:
            paths=[root/(name+suffix) for suffix in ('.golden','.ours','.json')]
            for path,rows in zip(paths,[left,right]):path.write_text(''.join(json.dumps(row,separators=(',',':'))+'\n' for row in rows))
            counts=[gate(path,image,filename_canary=name=='literal-filename-callback-argument',known_callbacks=known) for path in paths[:2]]
            process=run_diff(snapshot,*paths)
            try:report=json.loads(paths[2].read_text())
            except (ValueError,OSError):report=None
            observed={key:len(report.get(key,[])) for key in SECTIONS} if isinstance(report,dict) else None
            metadata=report.get('boundary_arguments_comparison') if isinstance(report,dict) else None
            expected_counts={key:int(key==expected) for key in SECTIONS}
            verified=(process.returncode==0 and observed==expected_counts and metadata==ARGUMENT_METADATA and
                      report.get('callback_sequence',{}).get('first_divergence') is None and
                      report.get('callback_sequence',{}).get('golden')==1 and report.get('callback_sequence',{}).get('ours')==1)
            cases.append(dict(name=name,verified=verified,expected=expected_counts,observed=observed,metadata=metadata,
                              typedCounts=counts,exitCode=process.returncode,stdout=process.stdout,stderr=process.stderr,report=report))
    verify(snapshot)
    return {'verified':all(case['verified'] for case in cases),'status':'verified' if all(case['verified'] for case in cases) else 'pending-callback-boundary-comparison-support',
            'option':'--compare-boundary-arguments','expectedMetadata':ARGUMENT_METADATA,'cases':cases}


def compare(snapshot,image,left,right,report_path):
    state=verify(snapshot);require(state["image"]==image,"callback image differs from frozen snapshot");counts=[gate(path,image,known_callbacks=callback_metadata(snapshot)) for path in (left,right)]
    support=capability(snapshot,image)
    result=run_diff(snapshot,left,right,report_path)
    verify(snapshot)
    require(result.returncode==0,'callback comparator failed: '+result.stderr)
    report=json.loads(Path(report_path).read_text())
    require(all(isinstance(report.get(key),list) for key in SECTIONS),'incomplete comparator report')
    require(report.get('callback_sequence',{}).get('golden')==counts[0]['callbacks'] and report.get('callback_sequence',{}).get('ours')==counts[1]['callbacks'],'comparator dropped nonempty typed callbacks')
    mismatches=[key for key in SECTIONS if report.get(key)]
    if report.get('callback_sequence',{}).get('first_divergence') is not None:mismatches.append('callback_sequence')
    verified=support['verified'] and report.get('boundary_arguments_comparison')==ARGUMENT_METADATA
    return {'status':'compared' if verified else 'pending-callback-boundary-comparison-support','verified':verified,'capability':support,
            'counts':counts,'mismatches':mismatches,'stdout':result.stdout,'stderr':result.stderr,'exitCode':result.returncode,'report':report}


def main():
    parser=argparse.ArgumentParser(allow_abbrev=False);commands=parser.add_subparsers(dest='operation',required=True)
    cap=commands.add_parser('capability');cap.add_argument('snapshot');cap.add_argument('image')
    comp=commands.add_parser('compare')
    for name in ('snapshot','image','left','right','report'):comp.add_argument(name)
    args=parser.parse_args()
    try:
        result=capability(args.snapshot,args.image) if args.operation=='capability' else compare(args.snapshot,args.image,args.left,args.right,args.report)
        print(json.dumps(result))
    except (ValueError,KeyError,OSError,json.JSONDecodeError,subprocess.SubprocessError) as error:parser.error(str(error))


if __name__=='__main__':main()
