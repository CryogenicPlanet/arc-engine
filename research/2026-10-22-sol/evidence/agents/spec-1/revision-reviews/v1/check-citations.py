"""Recheck original review citations through the mandated bounded body reader."""
import hashlib,json,re,subprocess
from audit import EV, PRIOR, REV, IN, readtsv, table, digest

citations=readtsv(PRIOR/'evidence/citation-checks.tsv')
vas=[r['va'] for r in citations if r['status']=='indexed']
assert len(vas)==633
result=subprocess.run(['python3','-B','/home/exedev/work/read-function.py',*vas,'--max-bytes','65536'],capture_output=True,check=True)
assert b'TRUNCATED:' not in result.stdout and b'NO_EXPORTED_BODY' not in result.stdout
(EV/'citation-original-bodies.txt').write_bytes(result.stdout)
parts=re.split(rb'(?m)^INDEX ',result.stdout)[1:]
found={}
for part in parts:
    header,body=part.split(b'\n',1)
    va=header.split(b' ',1)[0].decode()
    # read-function.py adds one print newline after the original bytes.
    found[va]=(header.decode(),body[:-1])
assert len(found)==633
checked=[]
for r in citations:
    if r['status']!='indexed':
        assert r['va']=='0x4410bb4';continue
    header,body=found[r['va']]
    assert hashlib.sha256(body).hexdigest()==r['sha256'] and len(body)==int(r['length'])
    assert f"offset={r['offset']} " in header and f"path={r['path']} " in header
    checked.append(dict(unslid_va=r['va'],path=r['path'],offset=r['offset'],length=r['length'],sha256=r['sha256'],result='exact original body through bounded reader'))
table(EV/'citation-checks.tsv',checked)
source_checks=[]
for r in readtsv(PRIOR/'evidence/source-checks.tsv'):
    p=__import__('pathlib').Path(r['evidence'])
    assert digest(p)==r['sha256']
    input_copy=IN/'chromium-156'/r['path'] if r['repo']=='chromium' else None
    if input_copy and input_copy.is_file():assert digest(input_copy)==r['sha256']
    source_checks.append(dict(project=r['repo'],pin=r['revision'],path=str(p),sha256=r['sha256']))
table(EV/'prior-source-hash-checks.tsv',source_checks)
analogs=(REV/'revision-evidence/analog-original-bodies.txt').read_bytes()
for part in re.split(rb'(?m)^INDEX ',analogs)[1:]:
    header,body=part.split(b'\n',1); va=header.split(b' ',1)[0].decode()
    assert found[va]==(header.decode(),body[:-1]),va
# Hash the existing documented input identities, including both referenced images.
input_checks=[]
for rel,want in json.loads((REV/'evidence/input-hashes.json').read_text()).items():
    actual=digest(IN/rel);assert actual==want,rel
    input_checks.append(dict(path=str(IN/rel),sha256=actual))
table(EV/'input-hash-checks.tsv',input_checks)
print(json.dumps(dict(result='pass',prior_code_citations=len(citations),exact_original_bodies=len(checked),verified_no_body_veneer='0x4410bb4',prior_source_hashes=len(source_checks),input_hashes=len(input_checks),truncations=0),indent=2))
