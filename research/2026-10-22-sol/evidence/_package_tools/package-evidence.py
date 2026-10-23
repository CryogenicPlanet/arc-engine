#!/usr/bin/env python3
"""Stage (never publish) the reviewed Arc engine evidence; stdlib, offline only.

Usage: package-evidence.py stage --collected collected --output /new/package
       package-evidence.py verify /new/package

The collected directory mirrors /home/exedev/work, including readiness, revision
completion proofs, frozen inputs and prompts. No readiness/proof is manufactured.
"""
import argparse
import csv
import gzip
import hashlib
import io
import json
from pathlib import Path, PurePosixPath
import posixpath
import re
import sys
from urllib.parse import quote, unquote

WORK = '/home/exedev/work'
EVIDENCE = 'research/2026-10-22-sol/evidence'
CHECKSUMS = EVIDENCE + '/SHA256SUMS'
BUNDLE_README = EVIDENCE + '/BUNDLE-README.md'
ROLES = tuple(f'spec-{n}' for n in range(1, 5))
ENGINE_ROLES = ('abi', 'cef', 'arc-layer', 'matcher', 'quality', 'protocols')
OMIT = 'agents/cef/initial/sources/cef-build-index.json'
COMPRESS = 'agents/quality/initial/warning-bodies.tsv'
ASSIGN = 'inputs/context/class-assignments.json'
EMPTY_REVIEW_ARTIFACTS = {
    'agents/spec-1/revision-reviews/v1/9',
    'agents/review/9',
}
FIELDS = ('class_name', 'category', 'kind', 'selector', 'type_encoding')
TEXT_SUFFIXES = {'.md', '.tsv', '.csv', '.json', '.txt', '.c', '.cc', '.cpp',
                 '.h', '.hpp', '.py', '.sh', '.bash', '.js', '.ts', '.html',
                 '.xml', '.log', '.yaml', '.yml', '.sha256', '.sql', '.patch',
                 '.diff', '.rst', '.gn', '.gni', '.toml', '.def', '.inc',
                 '.m', '.mm', '.s', '.jsonl'}
BLOCKED_PARTS = {'__pycache__', '.git', '.venv', 'venv', 'node_modules',
                 'ghidra', 'profiles', 'profile', 'credentials', 'tokens',
                 'decompile-all', 'decompile-extra', 'binaries'}
# Markdown destinations, including reference definitions. Inline code and fenced
# code are deliberately not treated as links. Nested parentheses are supported
# to one level (the checked corpus uses ordinary Markdown paths).
LINK = re.compile(r'(!?\[[^\]\n]*\]\(\s*)(<[^>\n]+>|(?:[^\s()]|\([^()\n]*\))+)([^\n]*?\))'
                  r'|(^[ \t]{0,3}\[[^\]\n]+\]:[ \t]*)(<[^>\n]+>|\S+)', re.M)
FENCE = re.compile(r'(^[ \t]*```[^\n]*\n.*?^[ \t]*```[^\n]*$|`[^`\n]+`)', re.M | re.S)


def require(condition, message):
    if not condition:
        raise ValueError(message)


def sha_bytes(data):
    return hashlib.sha256(data).hexdigest()


def sha_file(path):
    h = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            h.update(block)
    return h.hexdigest()


def relative(name):
    """Require a canonical work-relative path; reject traversal and aliases."""
    require(isinstance(name, str) and name and '\\' not in name, f'Invalid path: {name!r}')
    path = PurePosixPath(name)
    require(not path.is_absolute() and all(p not in ('.', '..') for p in name.split('/')),
            f'Noncanonical path: {name}')
    return name


def instruction_excerpt(name):
    """Only bounded protocol-audit instruction bytes, never complete images."""
    return re.fullmatch(r'agents/protocols/initial/bodies/(?:engine|app)-0x[0-9a-f]+\.bin', name) is not None


def check_excerpt_size(name, size):
    require(not instruction_excerpt(name) or 0 < size <= 4096,
            f'Instruction excerpt exceeds the 4096-byte scope limit: {name}')


def check_artifact_size(name, size):
    check_excerpt_size(name, size)
    require(name not in EMPTY_REVIEW_ARTIFACTS or size == 0,
            f'Known empty review artifact unexpectedly has content: {name}')


def json_bytes(data):
    return (json.dumps(data, indent=2, sort_keys=True) + '\n').encode()


class Local:
    def __init__(self, root):
        self.root = root.resolve()

    def path(self, name):
        relative(name)
        p = self.root / name
        require(p.is_file() and not p.is_symlink(), f'Missing regular collected file: {name}')
        require(p.resolve().is_relative_to(self.root), f'Escaping collected path: {name}')
        check_artifact_size(name, p.stat().st_size)
        return p

    def read(self, name):
        return self.path(name).read_bytes()

    def inventory(self, directory):
        base = self.root / relative(directory)
        require(base.is_dir() and not base.is_symlink(), f'Missing directory: {directory}')
        result = {}
        for p in sorted(base.rglob('*')):
            require(not p.is_symlink(), f'Symlink in frozen evidence: {p}')
            if p.is_file() and '__pycache__' not in p.parts:
                result[p.relative_to(self.root).as_posix()] = sha_file(p)
        return result


class Packaged:
    """Original virtual namespace backed only by verified relocation records."""
    def __init__(self, root, records):
        self.root = root
        self.records = {r['source_relative']: r for r in records if r['output_path']}

    def read(self, name):
        relative(name)
        require(name in self.records, f'Unpackaged required input: {name}')
        record = self.records[name]
        p = self.root / record['output_path']
        data = gzip.decompress(p.read_bytes()) if record['transform'] == 'gzip' else p.read_bytes()
        check_artifact_size(name, len(data))
        return data

    def inventory(self, directory):
        return {name: sha_bytes(self.read(name)) for name in self.records
                if name.startswith(directory + '/')}


def load(store, name):
    return json.loads(store.read(name))


def rows(store, name):
    return list(csv.DictReader(io.StringIO(store.read(name).decode()), delimiter='\t'))


def identities(records, address):
    values = [tuple(row[f] for f in FIELDS) + (int(row[address], 0),) for row in records]
    require(len(values) == len(set(values)), 'Duplicate declared method identity')
    return set(values)


def previous(role):
    return f'spec-{(int(role[-1]) - 2) % 4 + 1}'


def check_hash_map(store, mapping, prefix=None):
    require(isinstance(mapping, dict) and mapping, 'Empty hash manifest')
    for name, expected in mapping.items():
        relative(name)
        require(not excluded(name), f'Sensitive or excluded required proof artifact: {name}')
        require(prefix is None or name.startswith(prefix + '/'), f'Wrong manifest namespace: {name}')
        require(re.fullmatch('[0-9a-f]{64}', expected or '') is not None, f'Invalid SHA256: {name}')
        require(sha_bytes(store.read(name)) == expected, f'Hash mismatch: {name}')
    if prefix is not None:
        require(mapping == store.inventory(prefix), f'Incomplete or extra frozen files: {prefix}')


def validate_ready(store):
    """Recheck publisher invariants, proofs, raw immutability and raw metadata."""
    ready = load(store, 'CLASS-REVISIONS-READY')
    require(ready.get('version') == 1 and ready.get('classes') == 146 and
            ready.get('declared_methods') == 1964, 'Wrong readiness scope/version')
    selected = ready.get('selected', [])
    require(len(selected) == 4 and {s['writer'] for s in selected} == set(ROLES),
            'Readiness must select exactly four distinct writers')
    groups = load(store, ASSIGN)['groups']
    classes = [c for s in selected for c in s['classes']]
    require(len(classes) == len(set(classes)) == 146, 'Class coverage is not exactly 146')
    metadata_classes = {r['class_name'] for r in rows(store, 'agents/arc-layer/initial/objc-classes.tsv')
                        if r['class_name'].startswith('Arc')}
    require(set(classes) == metadata_classes, 'Readiness differs from raw Arc-prefixed metadata classes')
    metadata = rows(store, 'agents/arc-layer/initial/objc-methods.tsv')
    expected_global = identities([r for r in metadata if r['class_name'] in classes
                                  and not r['protocol']], 'imp')
    require(len(expected_global) == 1964, 'Raw metadata does not cover exactly 1964 identities')
    actual_global = set()
    for item in selected:
        role = item['writer']
        base = f'agents/{role}/revisions/v1'
        review = f'agents/{role}/revision-reviews/v1'
        require(item['specs'] == WORK + '/' + base and item['review'] == WORK + '/' + review,
                f'Unexpected revision namespace: {role}')
        require(set(item['classes']) == set(groups[role]['classes']), f'Assignment drift: {role}')
        for field, directory in (('spec_sha256', base), ('review_sha256', review)):
            check_hash_map(store, item[field], directory)
        class_paths = {n for n in item['spec_sha256']
                       if n.startswith(base + '/classes/') and n.endswith('.md')}
        require(class_paths == {base + '/classes/' + c + '.md' for c in item['classes']},
                f'Noncanonical class paths: {role}')
        actual_classes = {PurePosixPath(n).stem for n in class_paths}
        require(actual_classes == set(item['classes']), f'Class files mismatch: {role}')
        require(all(re.fullmatch(r'Arc\w+', c) for c in actual_classes), 'Unsafe class filename')
        original = f'agents/{role}/specs'
        current = identities(rows(store, base + '/methods.tsv'), 'imp_va')
        require(len(current) == 491 and current == identities(rows(store, original + '/methods.tsv'), 'imp_va'),
                f'Original/revised identities differ: {role}')
        expected = {i for i in expected_global if i[0] in actual_classes}
        require(current == expected, f'Raw metadata/type identity mismatch: {role}')
        require(not actual_global.intersection(current), f'Cross-writer identity overlap: {role}')
        actual_global.update(current)
        for name, digest in store.inventory(original).items():
            tail = name[len(original) + 1:]
            if tail.split('/')[0] in ('bodies', 'evidence', 'sources'):
                require(sha_bytes(store.read(base + '/' + tail)) == digest,
                        f'Original raw evidence changed in revision: {name}')
        initial_review = f'agents/{previous(role)}/classreview'
        findings = rows(store, initial_review + '/findings.tsv')
        expected_rows = {str(n) for n in range(1, len(findings) + 1)}
        for name in (base + '/corrections.tsv', review + '/finding-dispositions.tsv'):
            ledger = rows(store, name)
            numbers = [r['finding_row'] for r in ledger]
            require(len(numbers) == len(set(numbers)) and set(numbers) == expected_rows,
                    f'Incomplete correction/review ledger: {name}')
            require(all(r.get('evidence', '').strip() for r in ledger), f'Missing ledger evidence: {name}')
            if name.endswith('corrections.tsv'):
                require(all(r.get('disposition', '').strip() for r in ledger), f'Missing disposition: {name}')
            else:
                require(all(r['status'] in ('resolved', 'resolved_with_limits') for r in ledger),
                        f'Unresolved correction: {role}')
        reviewed = [r['class_name'] for r in rows(store, review + '/class-review.tsv')]
        require(len(reviewed) == len(set(reviewed)) and set(reviewed) == actual_classes,
                f'Fresh review class coverage mismatch: {role}')
        require(all(r['severity'].strip().lower() == 'low' for r in rows(store, review + '/findings.tsv')),
                f'Material or invalid fresh review finding: {role}')
        for phase, directory in (('', base), ('-review', review)):
            job = role + '-revision-v1' + phase
            store.read(job + '.done')
            proof = load(store, job + '.completion.json')
            require(proof.get('terminal_event', {}).get('type') == 'turn.completed',
                    f'No terminal completion proof: {job}')
            require(proof['outputs'] == store.inventory(directory), f'Completion output drift: {job}')
            require(proof['inputs'] == load(store, job + '.run.json'), f'Run/proof inputs differ: {job}')
            expected_inputs = store.inventory(original)
            expected_inputs.update(store.inventory(initial_review))
            if phase:
                expected_inputs.update(store.inventory(base))
            for name in ('class-followups.py', ASSIGN, 'prompts/' + job + '.md'):
                expected_inputs[name] = sha_bytes(store.read(name))
            require(proof['inputs'] == expected_inputs, f'Incomplete frozen proof inputs: {job}')
            check_hash_map(store, proof['inputs'])
            require(store.read(directory + '/report.md').strip(), f'Empty report: {directory}')
    require(actual_global == expected_global, 'Declared identity coverage differs from metadata')
    # Reports from all required non-writer roles must be present; completion
    # proofs for their runs remain documentary evidence, not semantic assurance.
    for role in ENGINE_ROLES:
        stages = ('initial', 'decompile') if role in ENGINE_ROLES[:4] else ('initial',)
        for stage in stages:
            require(store.read(f'agents/{role}/{stage}/report.md').strip(), f'Empty {role}/{stage} report')
            store.read(f'{role}-{stage}.done')
    for role in ROLES:
        for stage in ('specs', 'classreview'):
            store.read(f'{role}-{stage}.done')
    validate_synthesis(store, ready)
    return ready


def validate_synthesis(store, ready):
    """A final package also requires the genuine completed global synthesis."""
    job = 'review-synthesis'
    directory = 'agents/review/synthesis'
    store.read(job + '.done')
    proof = load(store, job + '.completion.json')
    require(proof.get('terminal_event', {}).get('type') == 'turn.completed',
            'No successful global synthesis terminal event')
    require(proof['inputs'] == load(store, job + '.run.json'), 'Synthesis run/proof inputs differ')
    required = {'CLASS-REVISIONS-READY', 'prompts/review-synthesis.md', ASSIGN,
                'inputs/context/lead-review.md', 'inputs/context/ENGINE.md',
                'inputs/FULL-CORPUS-READY', 'inputs/INDEX-READY', 'inputs/CHROMIUM-READY'}
    require(required.issubset(proof['inputs']), 'Missing frozen synthesis inputs')
    for item in ready['selected']:
        for field in ('spec_sha256', 'review_sha256'):
            require(all(proof['inputs'].get(name) == digest for name, digest in item[field].items()),
                    'Synthesis input map does not freeze every selected spec/review')
    check_hash_map(store, proof['inputs'])
    require(proof['outputs'] == store.inventory(directory), 'Synthesis completed output drift')
    check_hash_map(store, proof['outputs'], directory)
    for name in ('report.md', 'engine-feasibility.md', 'spec-index.tsv'):
        require(store.read(directory + '/' + name).strip(), 'Missing/empty synthesis artifact: ' + name)
    index = [r['class_name'] for r in rows(store, directory + '/spec-index.tsv')]
    expected = {c for item in ready['selected'] for c in item['classes']}
    require(len(index) == len(set(index)) == 146 and set(index) == expected,
            'Global synthesis class coverage mismatch')
    final = proof['final_output']
    require(final['path'] == 'out/review-synthesis.md', 'Unexpected synthesis final-output path')
    require(store.read(final['path']).strip() and
            sha_bytes(store.read(final['path'])) == final['sha256'], 'Synthesis final-output hash mismatch')


def metadata_input(name):
    """Only compact frozen metadata under inputs, never source/corpus bodies."""
    return (name in {'inputs/FULL-CORPUS-READY', 'inputs/INDEX-READY', 'inputs/CHROMIUM-READY'}
            or (name.startswith('inputs/context/') and Path(name).suffix in TEXT_SUFFIXES)
            or re.fullmatch(r'inputs/decompile[^/]*/(?:[^/]+/)*MANIFEST\.json', name) is not None)


def excluded(name):
    parts = {p.lower() for p in PurePosixPath(name).parts}
    # Copied public headers such as cef_cookie.h and profiles/profile.h are
    # research sources, not browser profile stores or credential material.
    public_code = '/sources/' in name and Path(name).suffix.lower() in {
        '.h', '.hpp', '.c', '.cc', '.cpp', '.m', '.mm', '.s', '.gn', '.gni'}
    sensitive = any((not public_code and re.search(r'(?:^|[._-])(?:credentials?|tokens?|secrets?|passwords?|cookies?|keychain)(?:[._-]|$)', p))
                    or p in {'auth.json', 'auth.yaml', 'auth.yml', '.env', 'id_rsa', 'id_ed25519'}
                    or p.endswith(('.pem', '.key', '.p12', '.pfx')) or p.startswith('.env.')
                    for p in parts)
    if sensitive:
        return True
    if instruction_excerpt(name):
        return False
    if name.startswith('inputs/'):
        return not metadata_input(name)
    blocked = BLOCKED_PARTS - {'profile', 'profiles'} if public_code else BLOCKED_PARTS
    return bool(parts & blocked) or Path(name).suffix.lower() in {
        '.sqlite', '.sqlite3', '.db', '.dylib', '.so', '.o', '.a', '.exe',
        '.tar', '.gz', '.zip', '.bin', '.arm64', '.pyc'}


def scope_files(store):
    """Explicit research namespaces only. Never follow links or copy inputs wholesale."""
    names = set()
    roles = ROLES + ENGINE_ROLES + ('review',)
    for role in roles:
        root = store.root / 'agents' / role
        require(root.is_dir() and not root.is_symlink(), f'Missing/linked role: {role}')
        for p in root.rglob('*'):
            require(not p.is_symlink(), f'No symlinks allowed in role evidence: {p}')
            if not p.is_file():
                continue
            name = p.relative_to(store.root).as_posix()
            if excluded(name):
                continue
            excerpt = instruction_excerpt(name)
            check_artifact_size(name, p.stat().st_size)
            require(excerpt or name in EMPTY_REVIEW_ARTIFACTS or p.suffix.lower() in TEXT_SUFFIXES or p.name in ('README', 'SHA256SUMS', 'READY', 'VERSION'),
                    f'Unclassified file; inspect scope before packaging: {name}')
            require(p.stat().st_size <= 16 * 1024 * 1024 or name in (COMPRESS, OMIT),
                    f'Unexpected large evidence file: {name}')
            with p.open('rb') as stream:
                header = stream.read(8192)
            require(excerpt or b'\x00' not in header, f'Binary content excluded: {name}')
            names.add(name)
    names.update(('CLASS-REVISIONS-READY', 'class-followups.py', ASSIGN))
    for p in (store.root / 'inputs/context').glob('*'):
        if p.is_file() and p.suffix in TEXT_SUFFIXES:
            name = p.relative_to(store.root).as_posix()
            if not excluded(name):
                names.add(name)
    for p in store.root.iterdir():
        if p.is_file() and (p.suffix in ('.py', '.sh', '.tsv') or
                            re.fullmatch(r'[\w-]+\.(?:completion(?:-recovery)?\.json|run\.json|done)', p.name)):
            if not excluded(p.name):
                names.add(p.name)
    for folder in ('prompts', 'out', 'requests'):
        base = store.root / folder
        if base.is_dir():
            for p in base.rglob('*'):
                if p.is_file() and p.suffix in TEXT_SUFFIXES:
                    name = p.relative_to(store.root).as_posix()
                    if not excluded(name):
                        names.add(name)
    # Include every frozen correction-proof input, without expanding input scope.
    for role in ROLES:
        for suffix in ('', '-review'):
            proof = load(store, role + '-revision-v1' + suffix + '.completion.json')
            names.update(proof['inputs'])
            names.update(proof['outputs'])
    proof = load(store, 'review-synthesis.completion.json')
    names.update(proof['inputs'])
    names.update(proof['outputs'])
    for name in names:
        relative(name)
        require(not excluded(name), f'Required proof input falls outside compact scope: {name}')
        p = store.path(name)
        require(p.stat().st_size <= 16 * 1024 * 1024 or name in (COMPRESS, OMIT),
                f'Unexpected large control/input file: {name}')
    return sorted(names)


def split_destination(value):
    """Retain :LINE[:COLUMN], query and #fragment exactly after relocation."""
    m = re.match(r'^(.*?)(:[0-9]+(?::[0-9]+)?)?([?#].*)?$', value)
    return m.group(1), (m.group(2) or '') + (m.group(3) or '')


def external_link(value):
    # urlsplit treats "body.c:42" as a URI scheme. It is a local line citation.
    return bool(re.match(r'^[a-z][a-z0-9+.-]*:(?![0-9]+(?:[:#?]|$))', value, re.I))


def markdown_links(text, callback):
    protected = [(m.start(), m.end()) for m in FENCE.finditer(text)]
    def replace(m):
        if any(start <= m.start() < end for start, end in protected):
            return m.group(0)
        group = 2 if m.group(2) is not None else 5
        raw = m.group(group)
        angle = raw.startswith('<') and raw.endswith('>')
        destination = callback(raw[1:-1] if angle else raw)
        if angle:
            destination = '<' + destination + '>'
        a, b = m.span(group)
        return m.group(0)[:a - m.start()] + destination + m.group(0)[b - m.start():]
    return LINK.sub(replace, text)


def pinned_source(name, chromium_pin):
    prefix = 'inputs/chromium-156/'
    if chromium_pin and name.startswith(prefix):
        tail = name[len(prefix):]
        return 'https://chromium.googlesource.com/chromium/src/+/' + chromium_pin + '/' + quote(tail, safe='/')
    return None


def chromium_pin(store):
    """Derive the pin from retained manifests/context, never guess from version."""
    candidates = []
    for name in ('inputs/context/ENGINE.md', 'inputs/context/lead-review.md',
                 'agents/quality/initial/source-provenance.tsv',
                 'agents/cef/initial/sources/chromium-manifest.json'):
        try:
            text = store.read(name).decode()
        except (ValueError, FileNotFoundError):
            continue
        candidates.extend(re.findall(r'(?i)\b[0-9a-f]{40}\b', text))
    # Only the exact pin explicitly documented in the research handoff is eligible;
    # its presence in collected provenance is required before generating a URL.
    known = 'd0c02bfad08ec92a6ffcf989e53cd995e6381a4c'
    return known if known in candidates else None


def rebase_markdown(text, source, output, relocation, pin, issues):
    def destination(value):
        if not value or value.startswith('#') or external_link(value):
            return value
        path, suffix = split_destination(value)
        path = unquote(path)
        if path.startswith(WORK + '/'):
            name = path[len(WORK) + 1:]
        elif path.startswith('/'):
            issues.append({'source': source, 'destination': value, 'reason': 'external absolute path'})
            return value
        else:
            name = posixpath.normpath(posixpath.join(posixpath.dirname(source), path))
        record = relocation.get(name)
        if record and record['output_path']:
            result = posixpath.relpath(record['output_path'], posixpath.dirname(output))
            return quote(result, safe='/.-_') + suffix
        upstream = pinned_source(name, pin)
        if upstream:
            line = re.fullmatch(r':([0-9]+)(?::[0-9]+)?', suffix)
            if line:
                suffix = '#'+line.group(1)
            elif suffix.startswith(':'):
                issues.append({'source': source, 'destination': value, 'resolved_source': name,
                               'reason': 'combined upstream line/fragment suffix requires manual resolution',
                               'pinned_upstream': upstream})
                return value
            return upstream + suffix
        issues.append({'source': source, 'destination': value, 'resolved_source': name,
                       'reason': 'omitted input' if record else 'unresolved local link'})
        return value
    return markdown_links(text, destination)


def write_file(root, name, data):
    relative(name)
    path = root / name
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open('xb') as stream:
        stream.write(data)


def bundle_name(name):
    """Task-owned paths only; existing repository files are outside this scope."""
    return name.startswith(EVIDENCE + '/') or re.fullmatch(r'specs/Arc[^/]*\.md', name) is not None


def bundle_files(root):
    """Inventory only owned namespaces, rejecting links inside either namespace."""
    evidence = root / EVIDENCE
    specs = root / 'specs'
    require(evidence.is_dir() and specs.is_dir(), 'Missing owned evidence/specs directories')
    for name in (EVIDENCE, 'specs'):
        current = root
        for component in PurePosixPath(name).parts:
            current /= component
            require(not current.is_symlink(), f'Linked package namespace: {name}')
    files = []
    for path in evidence.rglob('*'):
        require(not path.is_symlink(), f'Package contains symlink: {path}')
        if path.is_file() and path != root / CHECKSUMS:
            files.append(path)
    for path in specs.glob('Arc*.md'):
        require(path.is_file() and not path.is_symlink(), f'Invalid class document: {path}')
        files.append(path)
    return sorted(files)


def stage(args):
    store = Local(args.collected)
    require(not args.output.exists(), f'Refusing to overwrite output: {args.output}')
    ready = validate_ready(store)  # Nothing is staged before genuine readiness.
    names = scope_files(store)
    output = args.output.resolve()
    require(not output.is_relative_to(store.root), 'Output cannot be inside collected input')
    output.mkdir(parents=True, exist_ok=False)
    records = []
    try:
        for name in names:
            if name == OMIT:
                source = store.path(name)
                records.append({'source_path': WORK + '/' + name, 'source_relative': name,
                                'source_sha256': sha_file(source), 'source_bytes': source.stat().st_size,
                                'output_path': None, 'output_sha256': None, 'output_bytes': 0,
                                'transform': 'omit',
                                'reason': 'Large CEF build catalog; pinned provenance retained, no behavioral evidence removed'})
                continue
            raw = store.read(name)
            transform = 'copy'
            target = EVIDENCE + '/' + name
            reason = ''
            if name == COMPRESS:
                target, transform = target + '.gz', 'gzip'
            data = gzip.compress(raw, mtime=0) if transform == 'gzip' else raw
            if target:
                write_file(output, target, data)
            records.append({'source_path': WORK + '/' + name, 'source_relative': name,
                            'source_sha256': sha_bytes(raw), 'source_bytes': len(raw),
                            'output_path': target, 'output_sha256': sha_bytes(data) if target else None,
                            'output_bytes': len(data) if target else 0, 'transform': transform,
                            'reason': reason})
        mapping = {r['source_relative']: r for r in records}
        pin = chromium_pin(store)
        issues = []
        raw_issues = []
        for record in records:
            if record['source_relative'].endswith('.md') and record['output_path']:
                rebase_markdown(store.read(record['source_relative']).decode(),
                                record['source_relative'], record['output_path'],
                                mapping, pin, raw_issues)
        for item in ready['selected']:
            for class_name in item['classes']:
                source = f'agents/{item["writer"]}/revisions/v1/classes/{class_name}.md'
                target = 'specs/' + class_name + '.md'
                raw = store.read(source)
                text = rebase_markdown(raw.decode(), source, target, mapping, pin, issues)
                text += ('\n\nOriginal reviewed source: [' + class_name + '.md](' +
                         posixpath.relpath(mapping[source]['output_path'], 'specs') + ').\n')
                data = text.encode()
                write_file(output, target, data)
                records.append({'source_path': WORK + '/' + source, 'source_relative': source,
                                'source_sha256': sha_bytes(raw), 'source_bytes': len(raw),
                                'output_path': target, 'output_sha256': sha_bytes(data),
                                'output_bytes': len(data), 'transform': 'presentation-markdown',
                                'reason': 'Relative links rebased; exact original retained in evidence'})
        manifest = {'version': 1, 'classes': 146, 'declared_methods': 1964,
                    'readiness_sha256': sha_bytes(store.read('CLASS-REVISIONS-READY')),
                    'files': records, 'chromium_pin': pin, 'link_issues': issues,
                    'raw_link_issues': raw_issues,
                    'excluded_inputs': [
                        'Pristine Arc/ArcCore binaries, app bundles and original browser profiles',
                        'Full 199-shard Ghidra export, project, SQLite corpus index and profiling captures',
                        '31 GB Chromium source tree, build outputs and external fetched dependencies',
                        'Credentials, private agent/session logs and unrelated filesystem contents'],
                    'reproduction_limit': 'Offline integrity/coverage verification only; original research tools require Linux paths, pristine binaries, corpus/source inputs and sometimes network fetches.'}
        write_file(output, EVIDENCE + '/relocation-manifest.json', json_bytes(manifest))
        write_file(output, EVIDENCE + '/link-issues.json', json_bytes(issues))
        write_file(output, EVIDENCE + '/raw-link-issues.json', json_bytes(raw_issues))
        write_file(output, BUNDLE_README, readme(manifest).encode())
        # Retain the verifier beside the package without requiring the checkout.
        write_file(output, EVIDENCE + '/_package_tools/package-evidence.py', Path(__file__).read_bytes())
        sums = ''.join(sha_file(p) + '  ' + p.relative_to(output).as_posix() + '\n'
                       for p in bundle_files(output))
        write_file(output, CHECKSUMS, sums.encode())
        result = verify(output)
        print(json.dumps({'staged': str(output), **result}, indent=2))
    except Exception:
        # Keep a failed partial stage for inspection; never delete/replace user paths.
        print(f'Incomplete stage retained for inspection: {output}', file=sys.stderr)
        raise


def readme(manifest):
    return '''# Arc engine reviewed evidence stage

This is a newly staged package, pending lead review and publication. Its paths
mirror the intended arc-engine repository: specs/<Class>.md and
research/2026-10-22-sol/evidence/. No final destination was modified.

All 146 class documents cover 1,964 exact declared identities, including category,
kind, encoding and IMP. Coverage is distinct from behavioral/runtime equivalence.
The readiness manifest, eight correction/review completion proofs, completed
global synthesis with frozen input/output/final-output proof, frozen inputs,
original drafts/reviews, corrected revisions and role evidence are retained.
Raw TSV/JSON provenance and original Markdown are byte-exact. Only specs/*.md are
presentation copies with relocated links. Address-named bodies retain their
writer, stage and revision namespaces; equal addresses are never deduplicated.

relocation-manifest.json records original Linux paths/hashes and output hashes,
including explicit omissions and link issues. BUNDLE-README.md and SHA256SUMS
live inside research/2026-10-22-sol/evidence/. Checksum paths are relative to the
repository root. Verification covers exactly specs/Arc*.md and every file under
that evidence directory except SHA256SUMS itself. It ignores unrelated repository
files, including the repository README.md and optional specs/README.md index.
The same verify command works in a new stage or the existing repository after
the lead publishes these two trees. Hash consistency is not an independent
authenticity signature.

Offline verification (Python 3.9+):
    python3 research/2026-10-22-sol/evidence/_package_tools/package-evidence.py verify .

The quality warning table is losslessly compressed. To inspect without mutating
the package:
    gzip -dc research/2026-10-22-sol/evidence/agents/quality/initial/warning-bodies.tsv.gz > /tmp/warning-bodies.tsv
Its original SHA256 is recorded and checked on decompression. The large CEF
build-index catalog is omitted with its original SHA256 and reason recorded.

This package supports offline integrity and declared-coverage checks. It is not a
self-contained rerun environment: original scripts use /home/exedev/work Linux
paths, pristine image binaries, full decompile shards/index, source trees, and
some network fetches. Large binaries, full corpus, SQLite caches, Ghidra projects,
profiles and credentials are excluded.
Bounded protocol-audit instruction excerpts under agents/protocols/initial/bodies/
are retained byte-for-byte with SHA256; only engine/app VA filenames up to 4096
bytes each qualify. These are evidence snippets, not complete executable images.
Two exact zero-byte reviewer artifacts, agents/review/9 and
agents/spec-1/revision-reviews/v1/9, are retained and hashed as original files.
Only those paths qualify, and verification requires that both remain empty.
Chromium source links are pinned upstream
when retained provenance establishes the pin; other absent links are reported in
link-issues.json. Original historical Markdown can have Linux paths and broken
relative links. Missing raw-link dependencies are recorded in raw-link-issues.json,
never silently changed in machine evidence.

The verifier checks local presentation links; unresolved links explicitly listed
in the manifest are reported as limitations. Raw Markdown is provenance, not a
relocated presentation. Review every reported unresolved link before publishing.
'''


def verify(root):
    root = root.resolve()
    require(root.is_dir(), 'Package directory missing')
    sums = {}
    for line in (root / CHECKSUMS).read_text().splitlines():
        digest, sep, name = line.partition('  ')
        require(sep and re.fullmatch('[0-9a-f]{64}', digest), 'Invalid SHA256SUMS line')
        relative(name)
        require(bundle_name(name) and name != CHECKSUMS, f'Checksum path outside owned scope/self: {name}')
        require(name not in sums, 'Duplicate checksum')
        sums[name] = digest
    actual = set()
    for p in bundle_files(root):
        name = p.relative_to(root).as_posix()
        actual.add(name)
        require(sums.get(name) == sha_file(p), f'Package checksum mismatch/unlisted file: {name}')
    require(actual == set(sums), 'Missing packaged file')
    manifest = json.loads((root / EVIDENCE / 'relocation-manifest.json').read_text())
    require(manifest['version'] == 1, 'Unknown package format')
    raw_records = []
    targets = set()
    originals = set()
    for record in manifest['files']:
        name = relative(record['source_relative'])
        require(record['source_path'] == WORK + '/' + name, 'Original provenance path drift')
        require(re.fullmatch('[0-9a-f]{64}', record.get('source_sha256', '')) is not None
                and isinstance(record.get('source_bytes'), int) and record['source_bytes'] >= 0,
                f'Invalid original hash/size record: {name}')
        check_artifact_size(name, record['source_bytes'])
        target = record['output_path']
        transform = record['transform']
        if target:
            relative(target)
            require(target in sums, f'Relocation target outside verified owned files: {target}')
            require(target not in targets, f'Duplicate output: {target}')
            targets.add(target)
            data = (root / target).read_bytes()
            require(sha_bytes(data) == record['output_sha256'] and len(data) == record['output_bytes'],
                    f'Relocation output mismatch: {target}')
            if transform != 'presentation-markdown':
                raw = gzip.decompress(data) if transform == 'gzip' else data
                require(transform in ('copy', 'gzip') and sha_bytes(raw) == record['source_sha256']
                        and len(raw) == record['source_bytes'], f'Original content mismatch: {name}')
        else:
            require(name == OMIT and transform == 'omit' and record['reason'], 'Unexpected omission')
        if transform != 'presentation-markdown':
            require(name not in originals, f'Duplicate raw provenance: {name}')
            originals.add(name)
            raw_records.append(record)
    store = Packaged(root, raw_records)
    require(sha_bytes(store.read('CLASS-REVISIONS-READY')) == manifest['readiness_sha256'],
            'Readiness identity changed')
    ready = validate_ready(store)
    specs = {p.stem for p in (root / 'specs').glob('Arc*.md')}
    require(specs == {c for s in ready['selected'] for c in s['classes']}, 'Final class file coverage mismatch')
    presentations = [r for r in manifest['files'] if r['transform'] == 'presentation-markdown']
    expected_presentations = {
        'specs/' + c + '.md': f'agents/{item["writer"]}/revisions/v1/classes/{c}.md'
        for item in ready['selected'] for c in item['classes']}
    require(len(presentations) == 146 and
            {r['output_path']: r['source_relative'] for r in presentations} == expected_presentations,
            'Presentation provenance/coverage mismatch')
    mapping = {r['source_relative']: r for r in raw_records}
    raw_issues = []
    for record in raw_records:
        if record['source_relative'].endswith('.md') and record['output_path']:
            rebase_markdown(store.read(record['source_relative']).decode(),
                            record['source_relative'], record['output_path'],
                            mapping, manifest['chromium_pin'], raw_issues)
    require(raw_issues == manifest['raw_link_issues'], 'Raw missing-link inventory drift')
    issues = []
    for record in manifest['files']:
        if record['transform'] != 'presentation-markdown':
            continue
        source, target = record['source_relative'], record['output_path']
        original = store.read(source)
        require(sha_bytes(original) == record['source_sha256'] and
                len(original) == record['source_bytes'], f'Presentation original identity mismatch: {source}')
        expected = rebase_markdown(original.decode(), source, target, mapping,
                                   manifest['chromium_pin'], issues)
        expected += ('\n\nOriginal reviewed source: [' + PurePosixPath(source).name + '](' +
                     posixpath.relpath(mapping[source]['output_path'], 'specs') + ').\n')
        require((root / target).read_text() == expected, f'Presentation differs from original: {target}')
    require(issues == manifest['link_issues'], 'Unresolved link inventory drift')
    known_bad = {i['destination'] for i in issues}
    broken = []
    def check(value, source):
        if value.startswith('#') or external_link(value) or value in known_bad:
            return value
        path, _ = split_destination(value)
        path = unquote(path)
        resolved = (root / source).parent / path
        if path.startswith('/') or not resolved.resolve().is_relative_to(root) or not resolved.exists():
            broken.append({'source': source, 'destination': value})
        return value
    for path in (root / 'specs').glob('Arc*.md'):
        name = path.relative_to(root).as_posix()
        markdown_links(path.read_text(), lambda value: check(value, name))
    require(not broken, f'Unexpected broken presentation links: {broken[:10]}')
    return {'verified_files': len(sums), 'classes': len(specs), 'declared_methods': 1964,
            'unresolved_links': len(issues), 'raw_unresolved_links': len(raw_issues),
            'omitted_inputs': [r['source_path'] for r in raw_records
                                                               if r['transform'] == 'omit'],
            'scope': 'specs/Arc*.md and task evidence directory only; offline integrity/coverage, no runtime or self-contained rerun claim'}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest='action', required=True)
    prepare = commands.add_parser('stage', help='Create a NEW stage; never publish or overwrite')
    prepare.add_argument('--collected', type=Path, required=True)
    prepare.add_argument('--output', type=Path, required=True)
    check = commands.add_parser('verify', help='Verify a completed package entirely offline')
    check.add_argument('package', type=Path)
    args = parser.parse_args()
    try:
        if args.action == 'stage':
            stage(args)
        else:
            print(json.dumps(verify(args.package), indent=2))
    except (OSError, ValueError, KeyError, TypeError, csv.Error) as error:
        parser.exit(1, 'Evidence package refused: ' + str(error) + '\n')


if __name__ == '__main__':
    main()
