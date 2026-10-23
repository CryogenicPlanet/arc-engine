#!/usr/bin/env python3
"""Private synthetic PAK tests; never reads actual generated/package resources."""
import ast
import gzip
import hashlib
import types
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]
path = ROOT / 'arc/browser/webextras_frontend_resource_check.py'
m = types.ModuleType('resource_check')
sys.modules[m.__name__] = m
m.__file__ = str(path)
exec(compile(path.read_bytes(), str(path), 'exec'), m.__dict__)
NODE = Path('/opt/homebrew/bin/node').resolve(strict=True)
SOURCE = ROOT / 'chromium/src/tools/grit/grit/format/data_pack.py'
text = SOURCE.read_bytes()
tree = ast.parse(text)
writer = next(n for n in tree.body if isinstance(n, ast.FunctionDef) and n.name == 'WriteDataPackToString')
version = next(n for n in tree.body if isinstance(n, ast.Assign) and any(isinstance(t, ast.Name) and t.id == 'PACK_FILE_VERSION' for t in n.targets))
ns = {'struct': struct}
exec(compile(ast.Module(body=[version, writer], type_ignores=[]), str(SOURCE), 'exec'), ns)
write = ns['WriteDataPackToString']
MAGIC = b'\x1e\x9b'
PROOF = Path(tempfile.mkdtemp(prefix='webextras-frontend-resource-unit-'))
results = []


def brotli(data, declared=None):
    code = "const z=require('node:zlib'),f=require('node:fs');process.stdout.write(z.brotliCompressSync(f.readFileSync(0)));"
    packed = subprocess.run([str(NODE), '-e', code], input=data, capture_output=True, check=True).stdout
    return MAGIC + (len(data) if declared is None else declared).to_bytes(6, 'little') + packed


def fixture(name, codec='plain', alias=False, marker=True):
    root = PROOF / name
    front = root / 'front'
    front.mkdir(parents=True)
    values, info, includes = {}, [], []
    for offset, (textual, relative, markers) in enumerate(m.ITEMS):
        data = ('// fixture ' + relative + '\n' + '\n'.join(markers if marker else markers[1:])).encode()
        generated = front / relative
        generated.parent.mkdir(parents=True, exist_ok=True)
        generated.write_bytes(data)
        raw = data if codec == 'plain' else gzip.compress(data, mtime=0) if codec == 'gzip' else brotli(data) if codec == 'brotli' else gzip.compress(brotli(data), mtime=0)
        file = relative if codec == 'plain' else relative + '.compressed'
        (front / file).write_bytes(raw)
        number = [52490, 52853, 53015][offset]
        values[number] = raw
        info.append(f'{textual},{number},gen/third_party/devtools-frontend/src/front_end/devtools_resources.grd\n')
        includes.append(f'<include name="{textual}" resource_path="{relative}" file="{file}" type="BINDATA" compress="false"/>')
    if alias:
        values[1] = values[52490]
    (front / 'devtools_resources.grd').write_text('<grit><includes>' + ''.join(includes) + '</includes></grit>')
    (root / 'devtools_resources.pak.info').write_text(''.join(info))
    for name in ['intermediate.pak', 'built.pak', 'packaged.pak']:
        (root / name).write_bytes(write(values, 1))
    return m.Layout(front, root / 'intermediate.pak', root / 'devtools_resources.pak.info', root / 'built.pak', root / 'packaged.pak', NODE), root


def success(name, codec='plain', alias=False):
    layout, root = fixture(name, codec, alias)
    receipt = m.check(layout, root / 'receipt', 'present')
    assert receipt['builtPackagedPakExactMatch'] and len(receipt['resources']) == 3
    assert all(r['generatedExactMatch'] and r['grdInputExactMatch'] and r['intermediateExactMatch'] for r in receipt['resources'])
    assert all(len(r['compressionLayers']) == {'plain': 0, 'gzip': 1, 'brotli': 1, 'mixed': 2}[codec] for r in receipt['resources'])
    if alias:
        assert receipt['resources'][0]['canonicalId'] == 1
    results.append({'name': name, 'status': 'pass'})


def reject(name, change, message, codec='plain', marker=True):
    layout, root = fixture(name, codec, marker=marker)
    change(layout, root)
    try:
        m.check(layout, root / 'receipt', 'present')
    except RuntimeError as error:
        assert message in str(error), (name, str(error))
        assert not (root / 'receipt/receipt.json').exists()
        results.append({'name': name, 'status': 'expected rejection', 'reason': str(error)})
    else:
        raise AssertionError(name + ' was accepted')


def replace_all_payloads(layout, raw):
    values = {52490: raw, 52853: raw, 53015: raw}
    for p in [layout.intermediate_pak, layout.built_pak, layout.packaged_pak]:
        p.write_bytes(write(values, 1))
    for _, relative, _ in m.ITEMS:
        (layout.front / (relative + '.compressed')).write_bytes(raw)


for codec in ['plain', 'gzip', 'brotli', 'mixed']:
    success(codec, codec)
success('alias', alias=True)
reject('different-package', lambda l, r: l.packaged_pak.write_bytes(l.packaged_pak.read_bytes() + b'x'), 'differs from packaged')
reject('different-generated', lambda l, r: (l.front / 'panels/elements/elements.js').write_bytes(b'arcmodHost inspectModeChanged changed'), 'exact GRD input')
reject('decoded-generated-mismatch', lambda l, r: (l.front / 'panels/elements/elements.js').write_bytes(b'arcmodHost inspectModeChanged changed'), 'decoded PAK bytes differ', codec='gzip')
reject('missing-marker', lambda l, r: None, 'capability/notification missing', marker=False)
reject('missing-info', lambda l, r: l.info.write_text(l.info.read_text().replace('DEVTOOLS_COMPATIBILITY_JS', 'OTHER_JS')), 'missing/duplicate .info name')
reject('wrong-info-source', lambda l, r: l.info.write_text(l.info.read_text().replace('gen/third_party/devtools-frontend/src/front_end/devtools_resources.grd', 'gen/other.grd')), 'unexpected .info generator source')
reject('duplicate-info-id', lambda l, r: l.info.write_text(l.info.read_text() + l.info.read_text().splitlines()[0] + '\n'), 'duplicate .info resource IDs')
reject('bad-grd-path', lambda l, r: (l.front / 'devtools_resources.grd').write_text((l.front / 'devtools_resources.grd').read_text().replace('file="devtools_compatibility.js"', 'file="../escape.js"')), 'unsupported GRD declaration')
reject('intermediate-mismatch', lambda l, r: l.intermediate_pak.write_bytes(write({52490: b'wrong', 52853: b'wrong', 53015: b'wrong'}, 1)), 'intermediate/final resource mismatch')
reject('bad-brotli-size', lambda l, r: replace_all_payloads(l, brotli(b'abc', declared=1)), 'does not match GRIT header', codec='brotli')
reject('bad-brotli-limit', lambda l, r: replace_all_payloads(l, MAGIC + (m.MAX_JS + 1).to_bytes(6, 'little') + b'x'), 'declared output exceeds limit', codec='brotli')
reject('truncated-brotli', lambda l, r: replace_all_payloads(l, MAGIC + b'\x00'), 'short GRIT Brotli header', codec='brotli')
reject('excessive-layers', lambda l, r: replace_all_payloads(l, gzip.compress(gzip.compress(gzip.compress(gzip.compress(b'x'))))), 'too many compression layers', codec='gzip')
reject('concatenated-gzip', lambda l, r: replace_all_payloads(l, gzip.compress(b'x') + gzip.compress(b'y')), 'concatenated gzip', codec='gzip')
# Recovery uses the same exact-byte chain with an explicit absent gate.
layout, root = fixture('rollback-absent')
for _, relative, _ in m.ITEMS:
    p = layout.front / relative
    p.write_bytes(p.read_bytes().replace(b'arcmodHost', b'oldHost'))
values = {number: (layout.front / relative).read_bytes() for number, (_, relative, _) in zip([52490, 52853, 53015], m.ITEMS)}
for p in [layout.intermediate_pak, layout.built_pak, layout.packaged_pak]:
    p.write_bytes(write(values, 1))
r = m.check(layout, root / 'receipt', 'absent')
assert r['expectHostGate'] == 'absent' and r['passed']
results.append({'name': 'rollback-absent-exact-bytes', 'status': 'pass'})
layout, root = fixture('candidate-rejected-as-absent')
try:
    m.check(layout, root / 'receipt', 'absent')
except RuntimeError as error:
    assert 'unexpectedly present' in str(error)
    results.append({'name': 'candidate-rejected-as-absent', 'status': 'expected rejection', 'reason': str(error)})
else:
    raise AssertionError('candidate accepted as absent')
# Output-bounded decoder failures, independent of declared compressed sizes.
for codec in ['gzip', 'brotli']:
    raw = gzip.compress(b'x' * (m.MAX_JS + 1), mtime=0) if codec == 'gzip' else brotli(b'x' * (m.MAX_JS + 1), declared=1)
    try:
        m.decode(raw, MAGIC, NODE)
    except RuntimeError as error:
        expected = 'oversized' if codec == 'gzip' else 'decoder failed or exceeded limit'
        assert expected in str(error)
        results.append({'name': codec + '-actual-output-bound', 'status': 'expected rejection', 'reason': str(error)})
    else:
        raise AssertionError(codec + ' output limit bypassed')
# Source drift after initial reads is rejected by the final rehash.
layout, root = fixture('during-decode-drift', 'gzip')
original = m.decode
changed = False

def drift(*args):
    global changed
    value = original(*args)
    if not changed:
        changed = True
        layout.info.write_text(layout.info.read_text() + '\n')
    return value

m.decode = drift
try:
    m.check(layout, root / 'receipt', 'present')
except RuntimeError as error:
    assert 'input changed during resource check' in str(error)
    results.append({'name': 'during-decode-drift', 'status': 'expected rejection', 'reason': str(error)})
else:
    raise AssertionError('input drift accepted')
finally:
    m.decode = original
report = {'scope': 'private synthetic fixtures only; official writer/reader source AST and bounded real Node Brotli',
          'proof': str(PROOF), 'writerSourceSha256': hashlib.sha256(text).hexdigest(), 'results': results}
(PROOF / 'tests.json').write_text(json.dumps(report, indent=2) + '\n')
print(json.dumps({'groups': len(results), 'status': 'PASS', 'proof': str(PROOF)}))
