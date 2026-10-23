#!/usr/bin/env python3
"""Record the corrected revision without rewriting the copied original manifest."""
import hashlib
from pathlib import Path

R = Path(__file__).resolve().parents[1]
target = R/'revision-artifacts.sha256'
rows = []
for p in sorted(R.rglob('*')):
    if not p.is_file() or p == target or '__pycache__' in p.parts: continue
    h = hashlib.sha256()
    with p.open('rb') as f:
        for data in iter(lambda:f.read(1024*1024), b''): h.update(data)
    rows.append(h.hexdigest()+'  '+str(p.relative_to(R)))
target.write_text('\n'.join(rows)+'\n')
print(f'{len(rows)} revision files recorded (manifest itself and Python caches excluded).')
