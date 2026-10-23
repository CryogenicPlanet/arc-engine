#!/usr/bin/env python3
"""Fetch bounded public primary-source files, no repository checkout or build."""
import concurrent.futures, hashlib, json, pathlib, urllib.request

ROOT = pathlib.Path(__file__).resolve().parents[1]
REV = json.loads((ROOT/'sources/cef-target.json').read_text())['sha']
TREE = json.loads((ROOT/'sources/cef-tree.json').read_text())['tree']
paths = [x['path'] for x in TREE if x['type'] == 'blob' and (
    (x['path'].startswith('include/') and x['path'].endswith('.h') and
     x['path'].count('/') <= 2 and not '/capi/' in x['path']) or
    x['path'] in ['CHROMIUM_BUILD_COMPATIBILITY.txt', 'README.md',
                 'docs/architecture.md', 'docs/general_usage.md',
                 'docs/javascript_integration.md','docs/sandbox_setup.md',
                 'docs/branches_and_building.md',
                 'tests/cefclient/cefclient_mac.mm',
                 'tests/shared/process_helper_mac.cc',
                 'tests/cefsimple/cefsimple_mac.mm'])]

def fetch(path):
    url = f'https://raw.githubusercontent.com/chromiumembedded/cef/{REV}/{path}'
    dest = ROOT/'sources/cef'/path
    dest.parent.mkdir(parents=True, exist_ok=True)
    if not dest.exists():
        with urllib.request.urlopen(url, timeout=30) as response:
            data = response.read(2_000_000)
        dest.write_bytes(data)
    data = dest.read_bytes()
    return dict(path=path, url=url, revision=REV, bytes=len(data),
                sha256=hashlib.sha256(data).hexdigest())

with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:
    manifest = list(pool.map(fetch, paths))
(ROOT/'sources/cef-manifest.json').write_text(json.dumps(manifest, indent=2)+'\n')
print(f'{len(manifest)} files; {sum(x["bytes"] for x in manifest)} bytes; revision {REV}')
