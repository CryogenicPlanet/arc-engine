#!/usr/bin/env python3
"""Compare bounded original/ours extraction probes with explicit volatile fields.

The complete unmodified response and recorder files remain in each run. Only
fixture authority/token and extraction wall-clock values are excluded from the
tree comparison; each timestamp must lie within its own request interval.
"""
import json
import argparse
from pathlib import Path
import sys
from urllib.parse import urlsplit

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('result')
parser.add_argument('--allow-unrelated-run-errors', action='store_true')
parser.add_argument('--expected-count', type=int)
args = parser.parse_args()
if args.allow_unrelated_run_errors and not args.expected_count:
    parser.error('subset validation requires --expected-count')
result = Path(args.result).resolve()
run_errors = {}
def extracts(name):
    data = json.loads((result / name / 'observations.json').read_text())
    if data.get('error'):
        if not args.allow_unrelated_run_errors:
            raise SystemExit(f'{name}: {data["error"]}')
        run_errors[name] = data['error']
    rows = [row for row in data['acknowledgments'] if row['operation'] == 'extract']
    if not rows:
        raise SystemExit(f'{name}: no extraction')
    if args.expected_count is not None and len(rows) != args.expected_count:
        raise SystemExit(f'{name}: expected {args.expected_count} extractions, got {len(rows)}')
    normalized = []
    for row in rows:
        if not row.get('ok') or not row.get('mainThread'):
            raise SystemExit(f'{name}: invalid completion {row}')
        content = row.get('result')
        if content is None:
            normalized.append({'result': None, 'synchronous': row.get('synchronous'), 'surface': row.get('surface')})
            continue
        timestamp = content['extractionTime']
        if not row['sentAt'] - 0.01 <= timestamp <= row['receivedAt'] + 0.01:
            raise SystemExit(f'{name}: extractionTime outside its request interval')
        url = urlsplit(content['url'])
        origin = f'{url.scheme}://{url.netloc}'
        token = url.path.split('/')[1]
        if not token.startswith('arc-renderer-probe-'):
            raise SystemExit(f'{name}: nonfixture extraction URL')
        def normalize(value):
            if isinstance(value, str):
                return value.replace(origin, 'http://fixture').replace(token, 'fixture')
            if isinstance(value, list):
                return [normalize(x) for x in value]
            if isinstance(value, dict):
                return {k: normalize(v) for k, v in value.items() if k != 'extractionTime'}
            return value
        normalized.append({'result': normalize(content), 'synchronous': row.get('synchronous'), 'surface': row.get('surface')})
    return normalized
original = extracts('trace156')
ours = extracts('ours156')
(result / 'extraction-normalized.json').write_text(json.dumps(
    {'original': original, 'ours': ours, 'unrelated_run_errors': run_errors}, indent=2) + '\n')
if original != ours:
    raise SystemExit('extraction values/metadata/callback shape differ; inspect extraction-normalized.json')
scope = 'extraction subset only' if run_errors else 'extraction'
print(f'PASS ({scope}): {len(original)} extraction(s), exact trees/public metadata, main-thread callback shape, bounded wall-clock timestamps')
