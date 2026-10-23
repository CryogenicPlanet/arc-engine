"""Pure private-artifact forward/reverse verifier. No Git/compiler/source writes."""
from pathlib import Path
import hashlib,json,re
O=Path(__file__).resolve().parent
H=lambda b:hashlib.sha256(b).hexdigest()
m=json.loads((O/'manifest.json').read_text())
base={p:(O/'before'/p).read_bytes() for p in m['five_files']}
target={p:(O/'after'/p).read_bytes() for p in m['five_files']}
for p,row in m['five_files'].items():
    assert H(base[p])==row['base'] and H(target[p])==row['target']
def replay(files,patch,reverse=False,native=False):
    files=dict(files);lines=patch.splitlines(keepends=True);i=0;hunks=0
    while i<len(lines):
        assert lines[i].startswith('--- a/');name=lines[i][6:].rstrip('\n');i+=1
        assert lines[i]=='+++ b/'+name+'\n';i+=1
        if native:name='chromium/src/'+name
        src=files[name].decode().splitlines(keepends=True);out=[];cursor=0
        while i<len(lines) and lines[i].startswith('@@ '):
            match=re.fullmatch(r'@@ -(\d+)(?:,(\d+))? \+(\d+)(?:,(\d+))? @@.*\n',lines[i]);assert match;i+=1;old=[];new=[]
            while i<len(lines) and not lines[i].startswith(('@@ ','--- ')):
                tag=lines[i][0];body=lines[i][1:];assert tag in ' +-'
                if tag in ' -':old.append(body)
                if tag in ' +':new.append(body)
                i+=1
            assert len(old)==int(match[2] or 1) and len(new)==int(match[4] or 1)
            start,expected,replacement=(int(match[3])-1,new,old) if reverse else (int(match[1])-1,old,new)
            assert start>=cursor and src[start:start+len(expected)]==expected,(name,start)
            out+=src[cursor:start]+replacement;cursor=start+len(expected);hunks+=1
        out+=src[cursor:];files[name]=''.join(out).encode()
    return files,hunks
cp=(O/'inputs/core-endpoint.patch').read_text();np=(O/'inputs/physical-native.patch').read_text();op=(O/'inputs/physical-owned-v1.patch').read_text();rp=(O/'sessions-pipeline-refinement.patch').read_text();combined=(O/'combined-five-file.patch').read_text()
assert H(cp.encode())==m['core_patch_preserved_exact']
assert H(np.encode())==m['native_patch_preserved_exact']
assert H(op.encode())==m['owned_v1_patch_preserved_exact']
for p,h in m['patches'].items():assert H((O/p).read_bytes())==h
s1,h1=replay(base,cp);s2,h2=replay(s1,np,native=True);s3,h3=replay(s2,op);s4,h4=replay(s3,rp);assert s4==target
r3,_=replay(target,rp,True);assert r3==s3
r2,_=replay(r3,op,True);r1,_=replay(r2,np,True,True);r0,_=replay(r1,cp,True);assert r0==base
f,hf=replay(base,combined);r,hr=replay(target,combined,True);assert f==target and r==base and hf==hr
assert all(m['guards'].values())
print(json.dumps({'result':'PASS','files':len(base),'chain_hunks':[h1,h2,h3,h4],'combined_forward_hunks':hf,'combined_reverse_hunks':hr,'forward_and_reverse_exact':True,'live_or_compiler_actions':False},indent=2))
