#!/usr/bin/env python3
"""Compare retained Arc method declarations and instance layouts, not full ABI."""
import argparse
import csv
import hashlib
import json
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--evidence', type=Path, required=True)
    args = parser.parse_args()
    methods, layouts, pins = {}, {}, {}
    fields = ('class_name', 'category', 'kind', 'selector')
    for version in ('168', '170'):
        method_path = args.evidence / f'engine{version}-implementations.tsv'
        layout_path = args.evidence / f'engine{version}-layouts.json'
        for path in (method_path, layout_path):
            pins[path.name] = hashlib.sha256(path.read_bytes()).hexdigest()
        with method_path.open(newline='') as stream:
            records = list(csv.DictReader(stream, delimiter='\t'))
        assert all(r['class_name'].startswith('Arc') for r in records)
        methods[version] = {tuple(r[k] for k in fields): r['type_encoding'] for r in records}
        assert len(records) == len(methods[version]) == 1964
        raw = json.loads(layout_path.read_text())
        assert len(raw) == 146 and all(c.startswith('Arc') for c in raw)
        assert all(type(i['offset']) is int for r in raw.values() for i in r['ivars'])
        layouts[version] = {
            c: {k: ([{ik: iv for ik, iv in i.items() if ik != 'entry_va'} for i in v]
                    if k == 'ivars' else v)
                for k, v in record.items() if k not in ('image', 'class_va', 'ro_va')}
            for c, record in raw.items()}
    old, new = methods['168'], methods['170']
    common = old.keys() & new.keys()
    result = {
        'source_namespace': 'agents/protocols/initial/evidence',
        'source_sha256': pins,
        'images': ['ArcCore-1.168.0.arm64', 'ArcCore-1.170.0.arm64'],
        'declaration_key_fields': list(fields),
        'method_rows_per_image': {v: len(m) for v, m in methods.items()},
        'added_method_keys': sorted(new.keys() - old.keys()),
        'removed_method_keys': sorted(old.keys() - new.keys()),
        'raw_encoding_differences': [dict(key=key, older=old[key], newer=new[key])
                                     for key in sorted(common) if old[key] != new[key]],
        'classes_per_image': {v: len(m) for v, m in layouts.items()},
        'ivars_per_image': {v: sum(len(r['ivars']) for r in m.values()) for v, m in layouts.items()},
        'added_layout_classes': sorted(layouts['170'].keys() - layouts['168'].keys()),
        'removed_layout_classes': sorted(layouts['168'].keys() - layouts['170'].keys()),
        'retained_instance_layout_differences': [c for c in sorted(layouts['168'].keys() & layouts['170'].keys())
                                               if layouts['168'][c] != layouts['170'][c]],
        'scope': 'Retained method declaration identities/raw encodings and instance-layout fields; address anchors excluded. Does not compare every property, protocol conformance, metaclass layouts, C++ ABI, implementations, runtime fixups or behavior. Tables are not independently reparsed binaries.'}
    print(json.dumps(result, indent=2) + '\n', end='')


if __name__ == '__main__':
    main()
