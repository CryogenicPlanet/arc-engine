#!/usr/bin/env python3
"""Owned fixture-input freezer/contract verifier. Inactive until driver proposal adoption."""
import argparse
import json
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile

FILES = ('tests/recorder/diff.py', 'arc/tools/arccore-1.170.0-objc.tsv',
         'tests/recorder/app_callbacks.inc', 'arc/browser/webextras_fixture_inputs.py')
SECTIONS = ('missing', 'extra', 'return_mismatch', 'arg_mismatch', 'count_mismatch')
FIELDS = ('from', 'thread', 'depth', 'dir', 'kind', 'cls', 'sel', 'self')


def require(value, message):
    if not value:
        raise ValueError(message)


def image_name(image):
    require(re.fullmatch(r'[A-Za-z0-9_][A-Za-z0-9_.+\-]*', image) and image not in ('app', 'engine'), 'invalid fixture image')
    return image


def identity(path):
    path = Path(path)
    real = path.resolve(strict=True)
    before = real.stat()
    sha = subprocess.check_output(['/usr/bin/shasum', '-a', '256', str(real)], text=True).split()[0]
    after = real.stat()
    fields = ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')
    require(path.resolve(strict=True) == real and all(getattr(before, key) == getattr(after, key) for key in fields), 'input changed while hashing')
    return dict(path=str(path), realpath=str(real), sha256=sha, size=after.st_size,
                mtimeNs=after.st_mtime_ns, ctimeNs=after.st_ctime_ns, dev=after.st_dev, ino=after.st_ino)


def metadata(image, count):
    return {'mode': 'exact-image', 'admitted_images': [image], 'retained_fixture_events': {
        side: {image: {'call': count, 'ret': count}} for side in ('golden', 'ours')}}


def run_diff(snapshot, image, left, right, report):
    return subprocess.run([sys.executable, '-B', str(Path(snapshot)/FILES[0]), str(left), str(right),
                           '--fixture-image', image, '--json', str(report)], text=True, capture_output=True)


def capability(snapshot, image):
    # The old CLI exits zero for an unknown --fixture-image and drops both calls.
    # Require a real differing known-public result AND the exact fixture metadata.
    with tempfile.TemporaryDirectory(prefix='webextras-fixture-capability-') as temporary:
        root = Path(temporary)
        common = dict(thread='main', depth=0, dir='->engine', kind='-', cls='ArcMediaPosition',
                      sel='playbackRate', self='<ArcMediaPosition#1>', **{'from': image})
        for name, value in (('golden', 1), ('ours', 2)):
            rows = [dict(common, seq=1, ev='call', args=[]), dict(common, seq=2, ev='ret', ret=value)]
            (root/name).write_text(''.join(json.dumps(row, separators=(',', ':'))+'\n' for row in rows))
        report_path = root/'report.json'
        result = run_diff(snapshot, image, root/'golden', root/'ours', report_path)
        try:
            report = json.loads(report_path.read_text()) if report_path.exists() else None
        except (ValueError, OSError):
            report = None
        expected = {'method': '->engine -ArcMediaPosition playbackRate', 'call': 0,
                    'args': '[]', 'golden': '1', 'ours': '2'}
        verified = (result.returncode == 0 and isinstance(report, dict) and
                    report.get('fixture_comparison') == metadata(image, 1) and
                    report.get('return_mismatch') == [expected] and
                    all(report.get(key) == [] for key in SECTIONS if key != 'return_mismatch') and
                    report.get('callback_sequence', {}).get('first_divergence') is None)
        return dict(verified=verified, status='verified' if verified else 'pending-fixture-comparison-support',
                    image=image, stdout=result.stdout, stderr=result.stderr, exitCode=result.returncode, report=report)


def freeze(source_root, snapshot, image):
    source_root, snapshot = Path(source_root).resolve(), Path(snapshot).resolve()
    image_name(image)
    snapshot.mkdir(parents=True, exist_ok=True)
    require(not any(snapshot.iterdir()), 'comparison snapshot must be empty')
    before = {relative: identity(source_root/relative) for relative in FILES}
    copied = {}
    for relative in FILES:
        target = snapshot/relative
        target.parent.mkdir(parents=True, exist_ok=True)
        with target.open('xb') as output, Path(before[relative]['realpath']).open('rb') as source:
            shutil.copyfileobj(source, output)
        target.chmod(0o444)
        copied[relative] = identity(target)
    for relative in FILES:
        require(identity(source_root/relative) == before[relative], 'comparison source changed while freezing')
        require(all(copied[relative][key] == before[relative][key] for key in ('sha256', 'size')), 'frozen comparison bytes differ')
    state = dict(snapshot=str(snapshot), image=image, files={relative: {'source': before[relative], 'frozen': copied[relative]} for relative in FILES})
    state['capability'] = capability(snapshot, image)
    manifest = snapshot/'comparison-inputs.json'
    manifest.write_text(json.dumps(state, indent=2)+'\n')
    manifest.chmod(0o444)
    verify(snapshot)
    return state


def verify(snapshot):
    snapshot = Path(snapshot).resolve()
    state = json.loads((snapshot/'comparison-inputs.json').read_text())
    require(state['snapshot'] == str(snapshot) and set(state['files']) == set(FILES), 'snapshot manifest mismatch')
    for relative in FILES:
        require(identity(snapshot/relative) == state['files'][relative]['frozen'], 'frozen comparison input changed: '+relative)
    return state


def fixture_counts(rows, image):
    counts = {'call': 0, 'ret': 0}
    stacks = {}
    for event in rows:
        if event.get('ev') not in ('call', 'ret'):
            continue
        caller = event.get('from')
        require(isinstance(caller, str) and caller and caller.strip() == caller and not any(ord(c)<32 for c in caller), 'missing/mixed caller attribution')
        if event.get('dir') != '->engine' or caller != image:
            continue
        require(all(field in event for field in FIELDS), 'incomplete fixture identity')
        stack = stacks.setdefault(event['thread'], [])
        counts[event['ev']] += 1
        if event['ev'] == 'call':
            require(isinstance(event.get('args'), list), 'missing fixture arguments')
            stack.append(event)
        else:
            require(stack and 'ret' in event and all(stack[-1][field] == event[field] for field in FIELDS), 'unmatched fixture return')
            stack.pop()
    require(counts['call'] > 0 and counts['call'] == counts['ret'] and not any(stacks.values()), 'zero/incomplete fixture evidence')
    return counts


def compare(snapshot, image, left, right, count, report_path):
    state = verify(snapshot)
    require(state['image'] == image and state['capability']['verified'] is True, 'pending fixture-comparison support')
    require(count > 0, 'invalid expected fixture coverage')
    for path in (left, right):
        rows = [json.loads(line) for line in Path(path).read_text().splitlines()]
        require(fixture_counts(rows, image) == {'call': count, 'ret': count}, 'fixture coverage differs from expected '+str(count))
    result = run_diff(snapshot, image, left, right, report_path)
    verify(snapshot)
    require(result.returncode == 0, 'frozen comparator failed: '+result.stderr)
    report = json.loads(Path(report_path).read_text())
    require(report.get('fixture_comparison') == metadata(image, count), 'comparator did not retain exact expected fixture coverage')
    return result.stdout, report


def main():
    parser = argparse.ArgumentParser(allow_abbrev=False)
    commands = parser.add_subparsers(dest='operation', required=True)
    freeze_parser = commands.add_parser('freeze')
    freeze_parser.add_argument('source_root'); freeze_parser.add_argument('snapshot'); freeze_parser.add_argument('image')
    compare_parser = commands.add_parser('compare')
    compare_parser.add_argument('snapshot'); compare_parser.add_argument('image'); compare_parser.add_argument('left'); compare_parser.add_argument('right')
    compare_parser.add_argument('count', type=int); compare_parser.add_argument('report')
    verify_parser = commands.add_parser('verify'); verify_parser.add_argument('snapshot')
    args = parser.parse_args()
    try:
        if args.operation == 'freeze':
            print(json.dumps(freeze(args.source_root, args.snapshot, args.image)))
        elif args.operation == 'verify':
            print(json.dumps(verify(args.snapshot)))
        else:
            stdout, _ = compare(args.snapshot, args.image, args.left, args.right, args.count, args.report)
            print(stdout, end='')
    except (ValueError, KeyError, OSError, json.JSONDecodeError) as error:
        parser.error(str(error))


if __name__ == '__main__':
    main()
