#!/usr/bin/env python3
"""On research VM: verify full export manifests; index C bodies without loading shards."""
import hashlib
import json
from pathlib import Path
import re
import sqlite3

root = Path('/home/exedev/work/inputs')
manifests = [root/'decompile'/'MANIFEST.json'] + sorted((root/'decompile-extra').glob('batch-*/MANIFEST.json'))
files = {}
for manifest in manifests:
    if not (manifest.parent/'READY').exists():
        continue
    for entry in json.loads(manifest.read_text()):
        path = manifest.parent / entry['file']
        if entry['file'] in files:
            assert files[entry['file']][1] == entry['sha256'], 'Conflicting duplicate shard'
        else:
            files[entry['file']] = (path, entry['sha256'])
assert len(files) == 199, f'Expected 199 unique exported shards, found {len(files)}'
database = root / 'corpus-index.sqlite.new'
assert not database.exists(), 'Index attempt already exists; inspect before retry'
db = sqlite3.connect(database)
db.execute('CREATE TABLE functions(va INTEGER PRIMARY KEY,name TEXT,reported_size INTEGER,path TEXT,offset INTEGER,length INTEGER,failed INTEGER)')
header = re.compile(rb'^// ==== 0x([0-9a-fA-F]+)  (.*)  \(([0-9]+) bytes\)\r?\n$')
count = failures = 0
for path, expected in (files[name] for name in sorted(files)):
    digest = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda:stream.read(1048576), b''):
            digest.update(block)
    assert digest.hexdigest() == expected, path
    with path.open('rb') as stream:
        current = None
        failed = False
        while True:
            offset = stream.tell()
            line = stream.readline()
            match = header.match(line)
            if match or not line:
                if current is not None:
                    va, name, reported_size, start = current
                    db.execute('INSERT INTO functions VALUES(?,?,?,?,?,?,?)',
                               (va,name,reported_size,str(path),start,offset-start,int(failed)))
                    count += 1
                    failures += int(failed)
                current = None
                if not line: break
                current = (int(match[1],16),match[2].decode('utf-8','replace'),int(match[3]),offset)
                failed = False
            if b'decompile failed:' in line:
                failed = True
    db.commit()
assert count == 567959, f'Unexpected export header count {count}'
assert failures == 79, f'Unexpected failure placeholder count {failures}'
db.execute('CREATE INDEX names ON functions(name)')
db.commit(); db.close()
database.rename(root/'corpus-index.sqlite')
summary={'unique_shards':len(files),'function_headers':count,'failure_placeholders':failures,'successful_body_candidates':count-failures,'scope':'Export presence and hashes, not semantic validity or correct Ghidra body boundaries'}
(root/'FULL-CORPUS-READY').write_text(json.dumps(summary,indent=2)+'\n')
(root/'INDEX-READY').write_text(json.dumps(summary,indent=2)+'\n')
print(json.dumps(summary))
