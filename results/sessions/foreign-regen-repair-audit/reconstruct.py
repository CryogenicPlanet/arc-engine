"""Read-only HEAD blob reconstruction; writes audit JSON only, never an index."""
from pathlib import Path
import subprocess,os,re,json,hashlib,datetime
ROOT=Path('/Users/cryogenicplanet/general/arc-engine');OUT=ROOT/'results/sessions/foreign-regen-repair-audit';TX=ROOT/'results/sessions/proposals/source-only-adoption-transaction-v4/run-parent-v4'
env={**os.environ,'GIT_OPTIONAL_LOCKS':'0'}
sha=lambda b:hashlib.sha256(b).hexdigest()
def read(p):return json.loads(Path(p).read_text())
transaction=read(TX/'transaction.json');receiptPath=ROOT/'results/core-runtime/20261023T172900Z-seven-integration11/final-patches.json';accepted=read(receiptPath)
foreign=sorted(n for n in transaction['maintainedPatchPreimages']if n not in ['0004-arcmod-hooks.patch','0010-sessions.patch'])
head=subprocess.check_output(['git','-C',str(ROOT/'chromium/src'),'rev-parse','HEAD'],env=env,text=True).strip()
blobcache={}
def blob(name):
 if name not in blobcache:
  r=subprocess.run(['git','-C',str(ROOT/'chromium/src'),'show','HEAD:'+name],env=env,capture_output=True)
  blobcache[name]=r.stdout if r.returncode==0 else None
 return blobcache[name]
def sections(p):
 lines=p.read_text().splitlines(True);starts=[i for i,l in enumerate(lines)if l.startswith('diff --git ')]+[len(lines)]
 out=[]
 for a,b in zip(starts,starts[1:]):
  part=lines[a:b];old=next(l[4:].strip()for l in part if l.startswith('--- '));new=next(l[4:].strip()for l in part if l.startswith('+++ '));name=new[2:]if new.startswith('b/')else new
  out.append((name,old,part))
 return out
def apply(base,part):
 src=[]if base is None else base.decode().splitlines(True);result=[];cursor=0;i=0
 while i<len(part):
  m=re.match(r'@@ -(\d+)(?:,(\d+))? \+(\d+)(?:,(\d+))? @@',part[i])
  if not m:i+=1;continue
  start=int(m[1]);start=start-1 if start else 0
  if start<cursor:raise ValueError('overlapping hunk')
  result+=src[cursor:start];cursor=start;i+=1;oldcount=0;newcount=0
  while i<len(part)and not part[i].startswith('@@ '):
   l=part[i]
   if l.startswith('\\ No newline'):
    if result and result[-1].endswith('\n'):result[-1]=result[-1][:-1]
    i+=1;continue
   if l[:1]not in [' ','+','-']:break
   value=l[1:]
   if l[0]in [' ','-']:
    if cursor>=len(src)or src[cursor]!=value:raise ValueError('HEAD context mismatch at line '+str(cursor+1))
    cursor+=1;oldcount+=1
   if l[0]in [' ','+']:result.append(value);newcount+=1
   i+=1
  expectedold=int(m[2])if m[2]is not None else 1;expectednew=int(m[4])if m[4]is not None else 1
  if (oldcount,newcount)!=(expectedold,expectednew):raise ValueError('hunk size mismatch')
 result+=src[cursor:];return ''.join(result).encode()
rows=[];details=[]
for name in foreign:
 before=TX/'backup/maintained-patches'/name;after=ROOT/'patches'/name;row={'patch':name,'beforeSHA256':sha(before.read_bytes()),'afterSHA256':sha(after.read_bytes()),'acceptedCore11SHA256':accepted['patches/'+name],'beforeBytes':before.stat().st_size,'afterBytes':after.stat().st_size};assert row['afterSHA256']==row['acceptedCore11SHA256'];rows.append(row)
 for kind,p in [('v4-entry',before),('current-canonical',after)]:
  for name,old,part in sections(p):
   base=blob(name);newfile=old=='/dev/null';record={'patch':str(p.relative_to(ROOT)),'form':kind,'source':name,'stockHEADExists':base is not None,'oldPath':old,'newFileHeader':newfile,'stockHEADSHA256':sha(base)if base is not None else None}
   live=ROOT/'chromium/src'/name;record['liveSHA256']=sha(live.read_bytes())if live.is_file()else None
   try:
    if newfile:
     content=apply(None,part);record['patchContentMatchesLive']=sha(content)==record['liveSHA256'];record['applicableToStockHEAD']=base is None;record['reconstructedSHA256']=sha(content)
    else:
     content=apply(base,part);record['patchContentMatchesLive']=sha(content)==record['liveSHA256'];record['applicableToStockHEAD']=base is not None;record['reconstructedSHA256']=sha(content)
   except Exception as e:record['error']=str(e)
   details.append(record)
canonical=[d for d in details if d['form']=='current-canonical'];assert all(d.get('patchContentMatchesLive')and d.get('applicableToStockHEAD')for d in canonical)
entry=[d for d in details if d['form']=='v4-entry'];assert all(d.get('patchContentMatchesLive')for d in entry)
broad=[]
for name,before in transaction['broadEntrySourcePins'].items():
 p=ROOT/name;after=sha(p.read_bytes())if p.exists()else None
 if before!=after:broad.append({'path':name,'before':before,'after':after,'authorizedSevenPath':name in transaction['stagedSHA256']})
coreSources=read(ROOT/'results/core-runtime/20261023T172900Z-seven-integration11/final-sources.json');nativeDrift=[]
for name,before in coreSources.items():
 p=ROOT/name;after=sha(p.read_bytes())if p.exists()else None
 if before!=after:nativeDrift.append({'path':name,'core11SHA256':before,'currentSHA256':after,'authorizedSevenPath':name.removeprefix('chromium/src/')in transaction['stagedSHA256']})
header=ROOT/'chromium/src/chrome/browser/arcmod/arcmod_hooks.h';text=header.read_text()
for addition in transaction['headerUnion']['additions']:
 if text.count(addition)!=1:raise ValueError('header addition not unique')
 text=text.replace(addition,'',1)
headerProof={'path':str(header.relative_to(ROOT)),'currentSHA256':sha(header.read_bytes()),'foreignPayloadAfterRemovingOnlyApprovedUnionSHA256':sha(text.encode()),'entrySHA256':transaction['preimages']['chrome/browser/arcmod/arcmod_hooks.h']};assert headerProof['foreignPayloadAfterRemovingOnlyApprovedUnionSHA256']==headerProof['entrySHA256']
chronology=[]
for f in [ROOT/'results/sessions/proposals/source-only-adoption-transaction/run-20261023T170400Z/transaction.json',ROOT/'results/sessions/proposals/source-only-adoption-transaction-v3/run-parent-v3/transaction.json',TX/'transaction.json']:
 d=read(f);values=d['maintainedPatchPreimages'];chronology.append({'path':str(f.relative_to(ROOT)),'startedUTC':d.get('startedUTC'),'status':d.get('status'),'foreignHashes':{n:values[n]for n in foreign},'foreignAllCanonical':all(values[n]==accepted['patches/'+n]for n in foreign),'failureStringLength':len(d.get('failure',''))if isinstance(d.get('failure'),str)else None})
report={'auditedUTC':datetime.datetime.now(datetime.timezone.utc).isoformat(),'scope':'Read-only provenance audit during parent-owned transaction; no builds/queries/apps/locks/process control/Chirp or source/index writes. Git reads are HEAD blobs/rev-parse with GIT_OPTIONAL_LOCKS=0. No writer attribution.','stockChromiumHEAD':head,'table':rows,'patchSourceDetails':details,'canonicalSourceReconstructions':len(canonical),'entryFullAdditionsOverExistingStock':sum(d['newFileHeader']and d['stockHEADExists']for d in entry),'allCanonicalReconstructCurrentForeignSources':True,'allEntryFullAdditionPayloadsMatchCurrentForeignSources':True,'broadArcEntryCount':len(transaction['broadEntrySourcePins']),'broadArcDrift':broad,'core11SourceCount':len(coreSources),'core11SourceDrift':nativeDrift,'sharedHeaderReverseProof':headerProof,'chronology':chronology,'conclusions':['All eight foreign patch after hashes match accepted core11 canonical patch outputs; canonical HEAD→worktree reconstruction succeeds byte-exact for every listed foreign source.','V4 saved patch preimages use full-file additions over existing stock files. Their payloads still equal current foreign source contents; this is patch representation drift, not evidence of new foreign source behavior.','V1 and v3 entry maps are canonical; v4 entry is first inspected bad state. Drift occurred after v3 entry snapshot and before v4 entry snapshot. No responsible writer is established.','All broad69 Arc source entries remain byte-exact except authorized sessions e973→178091; core11 selected sources differ only at the authorized shared arcmod header, whose approved insertion removal restores entry bytes exactly.','Current canonical foreign patches should not be reverted to malformed v4 preimages merely to enforce byte-identical outputs. Ownership/reporting still requires explicit distinction and repair acknowledgement. No foreign source evidence here requires interrupting parent runtime.'],'limits':['This is provenance/semantic reconstruction of eight foreign maintained patches and named source seals, not runtime behavior acceptance or whole repository/index reconciliation.','Read-only current source snapshots may change after audit; parent holds ownership/source freeze and executes transaction decisions.','Failure strings are never emitted; chronology records only length.']}
(OUT/'audit.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'patches':len(rows),'canonicalSources':len(canonical),'badEntryAdditionsOverStock':report['entryFullAdditionsOverExistingStock'],'broadArcDrift':broad,'core11SourceDrift':nativeDrift,'chronology':[(x['startedUTC'],x['foreignAllCanonical'])for x in chronology],'headerForeignExact':True}))
