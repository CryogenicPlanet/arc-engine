#!/usr/bin/env python3
"""Record local snapshot provenance for discovery metadata and immutable pins."""
import datetime,hashlib,json,pathlib
ROOT=pathlib.Path(__file__).resolve().parents[1]
URLS={
 'cef-target.json':'https://api.github.com/repos/chromiumembedded/cef/commits/426920c33ef22551e0b8f48cf343d623ac25642d',
 'cef-tree.json':'https://api.github.com/repos/chromiumembedded/cef/git/trees/426920c33ef22551e0b8f48cf343d623ac25642d?recursive=1',
 'cef-8078-branch.json':'https://api.github.com/repos/chromiumembedded/cef/git/refs/heads/8078',
 'cef-build-index.json':'https://cef-builds.spotifycdn.com/index.json',
 'cef-published-target.json':'https://api.github.com/repos/chromiumembedded/cef/commits/47dbc1d',
 'cef-published-tree.json':'https://api.github.com/repos/chromiumembedded/cef/git/trees/47dbc1df023e99f006fb9e850a8d83cc19b4fdd9?recursive=1',
 'cef-master.json':'https://api.github.com/repos/chromiumembedded/cef/commits/master',
 'cef-master-compatibility.txt':'https://raw.githubusercontent.com/chromiumembedded/cef/7f8a5ec28e932aa62b583c8248e3df2e4dfe4807/CHROMIUM_BUILD_COMPATIBILITY.txt',
 'cef-8078-tip-compatibility.txt':'https://raw.githubusercontent.com/chromiumembedded/cef/426920c33ef22551e0b8f48cf343d623ac25642d/CHROMIUM_BUILD_COMPATIBILITY.txt',
 'chromium-tag.json':'https://chromium.googlesource.com/chromium/src/+/refs/tags/156.0.8078.48?format=JSON',
 'cef-architecture.html':'https://chromiumembedded.github.io/cef/architecture',
 'cef-branches.json':'https://api.github.com/repos/chromiumembedded/cef/branches?per_page=100',
}
rows=[]
for name,url in URLS.items():
    path=ROOT/'sources'/name
    rows.append(dict(path='sources/'+name,url=url,sha256=hashlib.sha256(path.read_bytes()).hexdigest(),
                     local_snapshot_mtime_utc=datetime.datetime.fromtimestamp(path.stat().st_mtime,datetime.timezone.utc).isoformat(),
                     timestamp_basis='local file mtime, not an upstream revision timestamp'))
(ROOT/'sources/discovery-provenance.json').write_text(json.dumps(rows,indent=2)+'\n')
print(f'{len(rows)} source discovery snapshots recorded')
