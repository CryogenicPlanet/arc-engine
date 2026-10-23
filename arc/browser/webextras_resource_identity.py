#!/usr/bin/env python3
"""Owned resource identity proposal; file evidence only, never a PAK decoder."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import stat
import subprocess
import sys

PAKS = ('resources.pak', 'chrome_100_percent.pak', 'chrome_200_percent.pak')
ROLES = ('built-ours156', 'ours156', 'trace156')
FIELDS = ('st_dev', 'st_ino', 'st_size', 'st_mtime_ns', 'st_ctime_ns')


def require(value, message):
    if not value:
        raise ValueError(message)


def stable_fields(info):
    # Nanosecond epochs exceed JavaScript's exact integer range. The identities
    # are embedded in the actual TS binary journal, so preserve them as strings.
    return {name: str(getattr(info, name)) if name.endswith('_ns') else getattr(info, name) for name in FIELDS}


def inventory(root):
    """Record every directory/file, rejecting links and special entries."""
    entries = {}
    def walk(directory):
        for path in sorted(directory.iterdir()):
            info = path.lstat()
            require(not stat.S_ISLNK(info.st_mode), 'symlink resource entry: '+str(path))
            kind = 'directory' if stat.S_ISDIR(info.st_mode) else 'file' if stat.S_ISREG(info.st_mode) else None
            require(kind, 'nonregular resource entry: '+str(path))
            entries[path.relative_to(root).as_posix()] = {'kind': kind, **stable_fields(info)}
            if kind == 'directory':
                walk(path)
    walk(root)
    return entries


def capture(root, role):
    require(role in ROLES, 'invalid resource role')
    root = Path(root)
    require(not root.is_symlink() and stat.S_ISDIR(root.lstat().st_mode), 'resource root must be a direct directory')
    root = root.resolve(strict=True)
    root_before = stable_fields(root.stat())
    before = inventory(root)
    files = {}
    for relative, item in before.items():
        if item['kind'] != 'file':
            continue
        path = root/relative
        with path.open('rb') as source:
            require(stable_fields(os.fstat(source.fileno())) == {k: item[k] for k in FIELDS}, 'resource changed before hashing: '+relative)
            digest = hashlib.file_digest(source, 'sha256').hexdigest()
            require(stable_fields(os.fstat(source.fileno())) == {k: item[k] for k in FIELDS}, 'resource changed while hashing: '+relative)
        require(not path.is_symlink() and path.resolve(strict=True).is_relative_to(root), 'resource path changed while hashing')
        files[relative] = {'sha256': digest, 'size': item['st_size'], **{k: item[k] for k in FIELDS}}
    require(before == inventory(root) and root_before == stable_fields(root.stat()), 'resource inventory changed while hashing')
    if role in ('built-ours156', 'ours156'):
        require('resources.pak' in files and files['resources.pak']['size'] > 0, 'ours resources.pak is missing or empty')
    # Scale packs are literal inventory, not an assertion of JS dependencies.
    return {'schema': 1, 'role': role, 'root': str(root), 'rootIdentity': root_before,
            'directories': sorted(name for name, item in before.items() if item['kind'] == 'directory'),
            'files': files, 'observedPaks': [name for name in PAKS if name in files]}


def verify_identity(identity):
    require(identity.get('schema') == 1, 'unknown resource identity schema')
    current = capture(identity['root'], identity['role'])
    require(current == identity, 'resource identity changed: '+identity['root'])
    return current


def content(identity):
    return {'directories': identity['directories'], 'files': {
        name: {key: value[key] for key in ('sha256', 'size')} for name, value in identity['files'].items()}}


def lock_owner(lock_file):
    owner = int(Path(lock_file).read_text())
    ancestor = os.getpid()
    for _ in range(20):
        if ancestor == owner:
            return owner
        require(ancestor > 1, 'resource checker lock owner is not an ancestor')
        ancestor = int(subprocess.check_output(['/bin/ps', '-p', str(ancestor), '-o', 'ppid='], text=True))
    raise ValueError('resource checker lock owner is not an ancestor')


def package_proof(built, packaged_root, lock_file):
    owner = lock_owner(lock_file)
    require(built['role'] == 'built-ours156', 'built proof role must be ours156')
    verify_identity(built)
    packaged = capture(packaged_root, 'ours156')
    require(content(built) == content(packaged), 'built and packaged resource bytes/inventory differ')
    require(lock_owner(lock_file) == owner, 'resource proof lock owner changed')
    return {'schema': 1, 'kind': 'ours156-built-package-resources', 'lockFile': str(Path(lock_file).resolve()),
            'lockOwnerPID': owner, 'built': built, 'packaged': packaged, 'contentEqual': True}


def verify_proof(proof):
    require(proof.get('schema') == 1 and proof.get('kind') == 'ours156-built-package-resources' and
            proof.get('contentEqual') is True, 'invalid resource package proof')
    require(lock_owner(proof['lockFile']) == proof['lockOwnerPID'], 'resource package proof belongs to another lock owner')
    require(proof['built']['role'] == 'built-ours156' and proof['packaged']['role'] == 'ours156', 'resource proof role mismatch')
    verify_identity(proof['built'])
    verify_identity(proof['packaged'])
    require(content(proof['built']) == content(proof['packaged']), 'resource package proof content mismatch')
    return proof


def write_new(path, value):
    with Path(path).open('x') as output:
        output.write(json.dumps(value, sort_keys=True, indent=2)+'\n')
    Path(path).chmod(0o444)


def main():
    parser = argparse.ArgumentParser(allow_abbrev=False)
    operations = parser.add_subparsers(dest='operation', required=True)
    snapshot = operations.add_parser('capture'); snapshot.add_argument('role', choices=ROLES); snapshot.add_argument('root'); snapshot.add_argument('--out')
    pair = operations.add_parser('package-proof'); pair.add_argument('built'); pair.add_argument('packaged'); pair.add_argument('lock_file'); pair.add_argument('--out', required=True)
    proof = operations.add_parser('verify-proof'); proof.add_argument('proof')
    journal = operations.add_parser('verify-journal'); journal.add_argument('journal')
    args = parser.parse_args()
    try:
        if args.operation == 'capture':
            result = capture(args.root, args.role)
        elif args.operation == 'package-proof':
            result = package_proof(json.loads(Path(args.built).read_text()), args.packaged, args.lock_file)
        elif args.operation == 'verify-proof':
            result = verify_proof(json.loads(Path(args.proof).read_text()))
        else:
            result = verify_identity(json.loads(Path(args.journal).read_text())['resources'])
        if getattr(args, 'out', None):
            write_new(args.out, result)
        print(json.dumps(result, sort_keys=True))
    except (ValueError, KeyError, TypeError, OSError) as error:
        parser.error(str(error))


if __name__ == '__main__':
    main()
