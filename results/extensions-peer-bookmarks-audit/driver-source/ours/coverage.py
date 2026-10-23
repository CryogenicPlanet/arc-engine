"""Independent raw public fixture coverage; rejects vacuous caller-filtered diffs.
Usage: coverage.py record.jsonl response.json dates.json coverage.json
"""
import json,pathlib,sys,collections,math
record,response,dates,out=map(pathlib.Path,sys.argv[1:5])
rows=[json.loads(s) for s in record.read_text().splitlines() if s]
r=json.loads(response.read_text());d=json.loads(dates.read_text());fixture='profile-probe.dylib'
exact={('ArcBrowserContext','getContentSettingForOrigin:withContentSettingType:'):1,('ArcBrowserContext','getLocalStorage:key:completion:'):2,
 ('ArcBrowserContext','setLocalStorage:key:value:completion:'):1,('ArcBrowser','setCookie:withCookie:'):1,
 ('ArcBrowser','removeCookie:withName:'):1,('ArcBrowser','saveCookiesToDisk'):1,
 ('ArcBrowserContext','getBookmarksTree'):2,('ArcBrowserContext','addBookmark:withUrl:withParent:withIndexInParent:withUuid:'):1,
 ('ArcBrowserContext','renameBookmark:withTitle:'):1,('ArcBrowserContext','editBookmark:withNewURL:'):1,
 ('ArcBrowserContext','removeBookmark:withParent:'):1,('ArcBrowserContext','isBookmarked:'):4,
 ('ArcBookmarkTreeNode','liveChildren'):6}
minimum={('ArcBookmarkTreeNode',s):1 for s in ('isFolder','isValid','nodeID','uuid','parentNodeID','title','url','dateAdded','dateFolderModified')}
selected=[e for e in rows if e.get('ev') in ('call','ret') and e.get('dir')=='->engine' and e.get('from')==fixture]
calls=collections.Counter((e.get('cls'),e.get('sel')) for e in selected if e['ev']=='call')
returns=collections.Counter((e.get('cls'),e.get('sel')) for e in selected if e['ev']=='ret')
errors=[]
for key,n in exact.items():
 if calls[key]!=n:errors.append(f'{key}: expected exactly {n}, got {calls[key]}')
for key,n in minimum.items():
 if calls[key]<n:errors.append(f'{key}: expected at least {n}, got {calls[key]}')
for key,n in calls.items():
 if returns[key]!=n:errors.append(f'{key}: call/return mismatch {n}/{returns[key]}')
if returns.keys()!=calls.keys():errors.append('orphan returns')
if not selected or any(e.get('thread')!='main' for e in selected):errors.append('missing fixture rows or non-main public call/return')
cookie_calls=calls[('ArcBrowser','getAllCookies:completion:')]
if not 2<=cookie_calls<=162 or r.get('cookieQueryCount')!=cookie_calls:errors.append('cookie polling count missing/outside allowance or differs from response')
if r.get('callbacksOnMain')!=[True]*(3+cookie_calls):errors.append('callbacks not exactly three storage plus each cookie query, all main')
bookmarked=[e.get('ret') for e in selected if e['ev']=='ret' and e.get('cls')=='ArcBrowserContext' and e.get('sel')=='isBookmarked:']
if len(bookmarked)!=4 or any(a is not b for a,b in zip(bookmarked,[True,False,True,False])):errors.append('raw isBookmarked result sequence differs from true,false,true,false')
mutation_selectors={s for (c,s) in exact if c=='ArcBrowserContext' and s.startswith(('addBookmark:','renameBookmark:','editBookmark:','removeBookmark:'))}
for e in selected:
 if e['ev']=='ret' and e.get('cls')=='ArcBrowserContext' and e.get('sel') in mutation_selectors and e.get('ret') is not True:errors.append('bookmark mutation did not return true: '+e['sel'])
if not r.get('complete') or not r.get('main') or r.get('pid')!=d.get('pid') or r.get('nonce')!=d.get('nonce'):errors.append('response/date sidecar identity or completion missing')
number=lambda v:isinstance(v,(int,float)) and not isinstance(v,bool) and math.isfinite(v)
start,end=d.get('windowStart'),d.get('windowEnd')
if not number(start) or not number(end) or end<start:errors.append('raw date sidecar lacks a valid add window')
else:
 for field in ('freshParentModified','leafAdded'):
  value=d.get(field)
  if not number(value) or not start<=value<=end:errors.append('raw date outside add window: '+field)
if number(d.get('freshParentModified')) and number(d.get('retainedParentBefore')) and not d['freshParentModified']>d['retainedParentBefore']:errors.append('fresh parent date did not advance beyond retained snapshot')
if not number(d.get('retainedParentBefore')) or d.get('retainedParentBefore')!=d.get('retainedParentAfter') or d.get('leafFolderModified') is not None:errors.append('raw retained date changed or leaf folder date nonnil')
expected={'added','retainedParentDateUnchanged','freshRootDistinctSameIdentity','freshParentDistinctSameIdentity','freshParentDateWithinAddWindow',
 'freshParentDateAdvanced','leafFolderModifiedNil','leafAddedWithinAddWindow','oldURLInitiallyBookmarked','renamed','edited','retainedLeafOriginalValues',
 'freshLeafDistinctSameIdentity','freshLeafChangedValues','oldURLNotBookmarked','newURLBookmarked','removedUsingOldDTO','bothLeafDTOsInvalid',
 'removedOldDTOOriginalValues','removedFreshDTOChangedValues','parentStillValid','fixtureAbsentFromRetainedParent','newURLNoLongerBookmarked'}
if set(r.get('bookmarks',{}))!=expected or any(v is not True for v in r.get('bookmarks',{}).values()):errors.append('fixed bookmark assertions missing or failed')
result={'fixture':fixture,'counts':{str(k):v for k,v in sorted(calls.items())},'exact_mutations':{str(k):calls[k] for k in exact if k[1] in mutation_selectors},'cookie_queries':cookie_calls,'callback_count':len(r.get('callbacksOnMain',[])),'errors':errors,'accepted':not errors}
out.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2));sys.exit(bool(errors))
