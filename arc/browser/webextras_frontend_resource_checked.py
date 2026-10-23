#!/usr/bin/env python3
"""Read-only V5 frontend resource proof with native-loader/lookup sanity checks.

Fixed ours156 paths; creates only a new logs/wp receipt directory. No build,
resource mutation, app execution, profile access or lock acquisition. Importable
core accepts private fixture paths for tests; the CLI never accepts source paths.
"""
import argparse
import dataclasses
import hashlib
import ast
import collections
import struct
import types
import json
import os
from pathlib import Path
import subprocess
import sys
import zlib

sys.dont_write_bytecode = True
ROOT = Path(__file__).resolve().parents[2]
SRC = ROOT / 'chromium/src'
BUILD = SRC / 'out/Base'
FRONT = BUILD / 'gen/third_party/devtools-frontend/src/front_end'
MAX_PAK = 256 * 1024 * 1024
MAX_JS = 32 * 1024 * 1024
MAX_META = 4 * 1024 * 1024
ITEMS = (
    ('DEVTOOLS_COMPATIBILITY_JS', 'devtools_compatibility.js', ('arcmodHost', 'inspectModeChanged', 'requestDockPosition')),
    ('PANELS_ELEMENTS_ELEMENTS_JS', 'panels/elements/elements.js', ('arcmodHost', 'inspectModeChanged')),
    ('UI_LEGACY_LEGACY_JS', 'ui/legacy/legacy.js', ('arcmodHost', 'requestDockPosition')),
)


def require(condition, message):
    if not condition:
        raise RuntimeError(message)


def digest(data):
    return hashlib.sha256(data).hexdigest()


def identity(path):
    path = Path(path)
    real = path.resolve(strict=True)
    st = real.stat()
    require(real.is_file(), f'not a regular input: {path}')
    return {'path': str(path), 'realpath': str(real), 'dev': st.st_dev, 'ino': st.st_ino,
            'size': st.st_size, 'mtimeNs': st.st_mtime_ns, 'ctimeNs': st.st_ctime_ns}


def read_stable(path, limit, identities):
    before = identity(path)
    require(before['size'] <= limit, f'oversized input: {path}')
    with open(path, 'rb') as stream:
        data = stream.read(limit + 1)
    require(len(data) == before['size'] and len(data) <= limit and identity(path) == before,
            f'input changed while reading: {path}')
    item = dict(before, sha256=digest(data))
    if str(path) in identities:
        require(identities[str(path)] == item, f'input changed between reads: {path}')
    identities[str(path)] = item
    return data


def official_grit(identities, output):
    # Compile only fixed official parser definitions from freshly sealed source;
    # Python import caches/unchecked .pyc cannot substitute executed Grit code.
    source_path = SRC / 'tools/grit/grit/format/data_pack.py'
    constants_path = SRC / 'tools/grit/grit/constants.py'
    source = read_stable(source_path, MAX_META, identities)
    constants = read_stable(constants_path, MAX_META, identities)
    names = {'WrongFileVersion', 'DataPackSizes', 'DataPackContents',
             'ReadDataPackFromString', 'ReadGrdInfo', 'GrdInfoItem'}
    selected = []
    for node in ast.parse(source, filename=str(source_path)).body:
        if isinstance(node, (ast.FunctionDef, ast.ClassDef)) and node.name in names:
            selected.append(node)
        elif isinstance(node, ast.Assign) and any(isinstance(t, ast.Name) and t.id == 'GrdInfoItem' for t in node.targets):
            selected.append(node)
    extracted_names = {n.name if isinstance(n, (ast.FunctionDef, ast.ClassDef)) else n.targets[0].id for n in selected}
    require(extracted_names == names and len(selected) == len(names), 'official parser definitions changed')
    namespace = {'struct': struct, 'collections': collections}
    exec(compile(ast.Module(body=selected, type_ignores=[]), str(source_path), 'exec'), namespace)
    markers = [n.value for n in ast.parse(constants).body if isinstance(n, ast.Assign)
               and any(isinstance(t, ast.Name) and t.id == 'BROTLI_CONST' for t in n.targets)]
    require(len(markers) == 1, 'official Brotli constant definition changed')
    marker = ast.literal_eval(markers[0])
    require(isinstance(marker, bytes) and len(marker) == 2, 'unsupported Brotli magic')
    (output / 'inputs/data_pack.py').write_bytes(source)
    (output / 'inputs/grit_constants.py').write_bytes(constants)
    (output / 'inputs/executed_parser.py').write_text('\n\n'.join(ast.get_source_segment(source.decode('utf-8'), n) for n in selected) + '\n')
    return types.SimpleNamespace(**{name: namespace[name] for name in names}), marker


# Exact native C156 sources reviewed for the checks below. Reject drift rather
# than claiming a later loader still has the same acceptance/lookup conditions.
NATIVE_SOURCES = {
    'ui/base/resource/resource_handle.h': '3e18062fb8452ac6a0177e3e66ec434945caa9d0d2315a2df21bb8ac673b7e20',
    'ui/base/resource/data_pack.cc': '11dcac3de60b8f007c6cc1b1f4be4ca420e6e347bb825ebdcacb9c08405e1c57',
    'ui/base/resource/data_pack.h': '97e9285a0b748b673fd586c88d1b6a1c8ad8e5294aa27e3ab9eb58925d6bd8ef',
}


def native_v5_sanity(data):
    # Native LoadImpl supports V4 too. This fixed C156 generated-build checker
    # deliberately supports V5 only (both reviewed writers emit V5).
    require(len(data) >= 12, 'native DataPack incomplete V5 header')
    version, = struct.unpack_from('<I', data)
    require(version == 5, 'resource checker supports generated DataPack V5 only')
    encoding, count, alias_count = struct.unpack_from('<BxxxHH', data, 4)
    require(encoding in (0, 1, 2), 'native DataPack bad encoding')
    table_end = 12 + (count + 1) * 6 + alias_count * 4
    require(table_end <= len(data), 'native DataPack tables exceed file size')
    entries = [struct.unpack_from('<HI', data, 12 + i * 6) for i in range(count + 1)]
    require(all(offset <= len(data) for _, offset in entries), 'native DataPack entry offset past EOF')
    require(all(entries[i][1] <= entries[i + 1][1] for i in range(count)),
            'native DataPack offsets not monotone')
    alias_start = 12 + (count + 1) * 6
    aliases = [struct.unpack_from('<HH', data, alias_start + i * 4) for i in range(alias_count)]
    require(all(index < count for _, index in aliases), 'native DataPack alias index out of bounds')
    # Native SanityCheck does NOT reject unsorted/duplicate IDs. Its bsearch
    # lookup assumes ordering; Grit's dictionary semantics differ otherwise.
    # Require both official writers' sorted unique/disjoint key invariant so
    # dictionary-selected bytes identify the same resource as native lookup.
    ids = [number for number, _ in entries[:-1]]
    alias_ids = [number for number, _ in aliases]
    require(all(a < b for a, b in zip(ids, ids[1:])), 'writer/native-lookup policy: canonical IDs not sorted unique')
    require(all(a < b for a, b in zip(alias_ids, alias_ids[1:])), 'writer/native-lookup policy: alias IDs not sorted unique')
    require(set(ids).isdisjoint(alias_ids), 'writer/native-lookup policy: canonical and alias IDs overlap')
    return {'version': version, 'encoding': encoding, 'resourceCount': count, 'aliasCount': alias_count,
            'nativeLoaderChecksPassed': True, 'writerLookupDeterminismPolicyPassed': True}


def decode(payload, brotli_const, node):
    layers = []
    for _ in range(3):
        prior = payload
        if payload.startswith(b'\x1f\x8b'):
            codec = 'gzip'
            decoder = zlib.decompressobj(16 + zlib.MAX_WBITS)
            payload = decoder.decompress(payload, MAX_JS + 1)
            require(len(payload) <= MAX_JS and decoder.eof and not decoder.unconsumed_tail and not decoder.unused_data,
                    'oversized, truncated or concatenated gzip payload')
            declared = None
        elif payload.startswith(brotli_const):
            codec = 'grit-brotli'
            require(len(payload) >= 8, 'short GRIT Brotli header')
            declared = int.from_bytes(payload[2:8], 'little')
            require(declared <= MAX_JS, 'Brotli declared output exceeds limit')
            # Node's bounded standard decoder matches the official frontend
            # compressor (scripts/build/compress_files.js). No shell or PATH lookup.
            code = "const z=require('node:zlib'),f=require('node:fs');process.stdout.write(z.brotliDecompressSync(f.readFileSync(0),{maxOutputLength:33554432}));"
            result = subprocess.run([str(node), '-e', code], input=payload[8:],
                                    stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=20)
            require(result.returncode == 0 and len(result.stdout) <= MAX_JS,
                    'Brotli decoder failed or exceeded limit')
            payload = result.stdout
            require(len(payload) == declared, 'Brotli decoded size does not match GRIT header')
        else:
            require(len(payload) <= MAX_JS, 'plain payload exceeds limit')
            return payload, layers
        layers.append({'codec': codec, 'inputBytes': len(prior), 'inputSha256': digest(prior),
                       'outputBytes': len(payload), 'outputSha256': digest(payload), 'declaredOutputBytes': declared})
    require(not payload.startswith((b'\x1f\x8b', brotli_const)), 'too many compression layers')
    return payload, layers


@dataclasses.dataclass(frozen=True)
class Layout:
    front: Path
    intermediate_pak: Path
    info: Path
    built_pak: Path
    packaged_pak: Path
    node: Path


def check(layout, output, expect_host_gate):
    import xml.etree.ElementTree as ET
    require(expect_host_gate in ('present', 'absent'), 'explicit host gate expectation required')
    identities = {}
    require(not output.exists() and not output.is_symlink(), 'receipt directory already exists')
    output.mkdir(parents=True, exist_ok=False)
    (output / 'inputs').mkdir()
    (output / 'payloads').mkdir()
    # Parent phase seals these fixed sources before invocation; the official
    # parser body is compiled from the exact source bytes archived below.
    read_stable(Path(__file__).resolve(), MAX_META, identities)
    for relative, expected in NATIVE_SOURCES.items():
        source = read_stable(SRC / relative, MAX_META, identities)
        require(digest(source) == expected, 'native DataPack reference source drift: ' + relative)
        (output / 'inputs' / Path(relative).name).write_bytes(source)
    data_pack, brotli_const = official_grit(identities, output)
    read_stable(layout.node, MAX_PAK, identities)
    grd = read_stable(layout.front / 'devtools_resources.grd', MAX_META, identities)
    info = read_stable(layout.info, MAX_META, identities)
    (output / 'inputs/devtools_resources.grd').write_bytes(grd)
    snapshot_info = output / 'inputs/devtools_resources.pak.info'
    snapshot_info.write_bytes(info)
    table = data_pack.ReadGrdInfo(str(snapshot_info)[:-5])
    require(len(table) == len(info.decode('utf-8').splitlines()), 'duplicate .info resource IDs')
    document = ET.fromstring(grd)
    intermediate_bytes = read_stable(layout.intermediate_pak, MAX_PAK, identities)
    built_bytes = read_stable(layout.built_pak, MAX_PAK, identities)
    packaged_bytes = read_stable(layout.packaged_pak, MAX_PAK, identities)
    sanity = {name: native_v5_sanity(data) for name, data in [('intermediate', intermediate_bytes), ('built', built_bytes), ('packaged', packaged_bytes)]}
    intermediate = data_pack.ReadDataPackFromString(intermediate_bytes)
    require(digest(built_bytes) == digest(packaged_bytes) and built_bytes == packaged_bytes,
            'built framework resource PAK differs from packaged ours156 PAK')
    built = data_pack.ReadDataPackFromString(built_bytes)
    results = []
    for textual, relative, markers in ITEMS:
        entries = [e for e in table.values() if e.textual_id == textual]
        require(len(entries) == 1, f'missing/duplicate .info name: {textual}')
        entry = entries[0]
        require(entry.path == 'gen/third_party/devtools-frontend/src/front_end/devtools_resources.grd',
                f'unexpected .info generator source for {textual}: {entry.path}')
        includes = [e for e in document.iter('include') if e.attrib.get('name') == textual]
        require(len(includes) == 1, f'missing/duplicate GRD entry: {textual}')
        attributes = includes[0].attrib
        require(attributes.get('resource_path') == relative and attributes.get('type') == 'BINDATA'
                and attributes.get('compress') == 'false'
                and attributes.get('file') in (relative, relative + '.compressed'), f'unsupported GRD declaration: {textual}')
        number = int(entry.id)
        require(number in built.resources and number in intermediate.resources, f'resource absent from PAK: {textual}')
        raw = built.resources[number]
        require(raw == intermediate.resources[number], f'intermediate/final resource mismatch: {textual}')
        require(raw == read_stable(layout.front / attributes['file'], MAX_JS, identities),
                f'PAK payload differs from exact GRD input: {textual}')
        generated = read_stable(layout.front / relative, MAX_JS, identities)
        decoded, layers = decode(raw, brotli_const, layout.node)
        require(decoded == generated, f'decoded PAK bytes differ from generated bundle: {textual}')
        if expect_host_gate == 'present':
            require(all(marker.encode('ascii') in generated for marker in markers), f'host capability/notification missing: {textual}')
        else:
            require(b'arcmodHost' not in generated, f'host capability unexpectedly present: {textual}')
        leaf = textual.lower()
        (output / 'payloads' / (leaf + '.raw')).write_bytes(raw)
        (output / 'payloads' / (leaf + '.decoded.js')).write_bytes(decoded)
        (output / 'inputs' / (leaf + '.generated.js')).write_bytes(generated)
        results.append({'textualId': textual, 'resourceId': number, 'generatedRelativePath': relative,
                        'grdAttributes': attributes, 'rawSha256': digest(raw), 'rawBytes': len(raw),
                        'decodedSha256': digest(decoded), 'decodedBytes': len(decoded), 'compressionLayers': layers,
                        'generatedExactMatch': True, 'grdInputExactMatch': True, 'intermediateExactMatch': True,
                        'expectHostGate': expect_host_gate, 'requiredMarkers': markers if expect_host_gate == 'present' else [], 'canonicalId': built.aliases.get(number, number)})
    # Rehash, not merely stat, every source after decoding and archival.
    for path, expected in list(identities.items()):
        current = {}
        read_stable(Path(path), MAX_PAK, current)
        require(current[path] == expected, f'input changed during resource check: {path}')
    artifacts = {str(p.relative_to(output)): digest(p.read_bytes()) for p in output.rglob('*') if p.is_file()}
    receipt = {'status': 'RESOURCE_BYTES_VERIFIED', 'passed': True,
               'expectHostGate': expect_host_gate, 'builtPakSHA256': digest(built_bytes), 'runtimePakSHA256': digest(packaged_bytes), 'scope': 'three generated frontend resources and built/package PAK identity; not runtime/parity proof',
               'builtPackagedPakExactMatch': True, 'nativePackSanity': sanity, 'pakVersion': built.version, 'resources': results,
               'inputs': identities, 'artifacts': artifacts}
    (output / 'receipt.json').write_text(json.dumps(receipt, indent=2) + '\n')
    return receipt


def verify_lock():
    owner = int((ROOT / 'logs/arccore-build.lock/pid').read_text())
    ancestor = os.getpid()
    for _ in range(32):
        if ancestor == owner:
            return
        require(ancestor > 1, 'requires ancestor holding shared integration lock')
        ancestor = int(subprocess.check_output(['ps', '-p', str(ancestor), '-o', 'ppid='], text=True))
    raise RuntimeError('requires ancestor holding shared integration lock')


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument('--output', required=True, type=Path)
    parser.add_argument('--expect-host-gate', required=True, choices=('present', 'absent'))
    args = parser.parse_args()
    verify_lock()
    output = args.output.resolve()
    require(output.is_relative_to(ROOT / 'logs/wp') and output != ROOT / 'logs/wp', 'output must be a fresh logs/wp subdirectory')
    framework = 'Contents/Frameworks/ArcCore.framework/Versions/A/Resources/resources.pak'
    layout = Layout(FRONT, BUILD / 'gen/content/browser/devtools/devtools_resources.pak',
                    BUILD / 'gen/content/browser/devtools/devtools_resources.pak.info',
                    BUILD / 'Chromium.app' / framework,
                    ROOT.parent / 'arc/build/ArcModAgent-ours156.app' / framework,
                    Path('/opt/homebrew/bin/node').resolve(strict=True))
    receipt = check(layout, output, args.expect_host_gate)
    print(json.dumps({'status': receipt['status'], 'receipt': str(output / 'receipt.json')}))


if __name__ == '__main__':
    try:
        main()
    except (RuntimeError, OSError, ValueError, subprocess.SubprocessError) as error:
        print(str(error), file=sys.stderr)
        sys.exit(1)
