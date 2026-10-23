"""Read-only guard for this checkout's Git SHA-1 index v3 (TREE, UNTR only).

Bounds intentionally exclude other index versions/extensions. Format source:
/Library/Developer/CommandLineTools/usr/share/man/man5/gitformat-index.5
UNTR structural validation is delegated to core_untr_run11 (exact Git 2.50.1).
No function writes an index or asks Git to refresh one.
"""
from dataclasses import dataclass
import hashlib
import os
from pathlib import Path
import re
import stat
import struct
import subprocess

from core_untr_run11 import validate_untr

MAX_INDEX_BYTES = 1024 * 1024 * 1024


class IndexGuardError(RuntimeError):
    pass


def git_environment():
    # These redirects could otherwise make a purported native HEAD/index pin refer
    # to a different repository. Alternate-index regeneration passes its own env.
    for key in ('GIT_DIR', 'GIT_COMMON_DIR', 'GIT_WORK_TREE', 'GIT_INDEX_FILE'):
        if os.environ.get(key):
            raise IndexGuardError('refuse inherited Git repository/index redirect: ' + key)
    return {**os.environ, 'GIT_OPTIONAL_LOCKS': '0'}


def native_head(native):
    env = git_environment()
    fmt = subprocess.check_output(['git', 'rev-parse', '--show-object-format'],
                                  cwd=native, env=env, text=True).strip()
    oid = subprocess.check_output(['git', 'rev-parse', '--verify', 'HEAD'],
                                  cwd=native, env=env, text=True).strip()
    if fmt != 'sha1' or not re.fullmatch('[0-9a-f]{40}', oid):
        raise IndexGuardError('only pinned SHA-1 native HEAD is supported')
    return oid


def read_regular_bytes(path):
    """No symlink following; reject a detectable replacement/in-place write race."""
    path = Path(path)
    with os.fdopen(os.open(path, os.O_RDONLY | os.O_NOFOLLOW), 'rb') as stream:
        before = os.fstat(stream.fileno())
        if not stat.S_ISREG(before.st_mode) or before.st_size > MAX_INDEX_BYTES:
            raise IndexGuardError('index must be a bounded regular file')
        data = stream.read(MAX_INDEX_BYTES + 1)
        after = os.fstat(stream.fileno())
        named = path.stat(follow_symlinks=False)
    def identity(s):
        return (s.st_dev, s.st_ino, s.st_mode, s.st_size, s.st_mtime_ns, s.st_ctime_ns)
    if identity(before) != identity(after) or identity(after) != identity(named) or len(data) != after.st_size:
        raise IndexGuardError('index changed while being read; retry requires parent review')
    return data


@dataclass(frozen=True)
class Extension:
    kind: bytes
    offset: int
    size: int


@dataclass(frozen=True)
class IndexImage:
    data: bytes
    version: int
    entries: int
    entries_end: int
    extensions: tuple
    sha256: str

    def proof(self):
        return {
            'raw_sha256': self.sha256,
            'raw_bytes': len(self.data),
            'version': self.version,
            'entries': self.entries,
            'header_entries_bytes': self.entries_end,
            'header_entries_sha256': hashlib.sha256(memoryview(self.data)[:self.entries_end]).hexdigest(),
            'checksum_sha1_valid': True,
            'ordered_extensions': [
                {'type': e.kind.decode('ascii'), 'size': e.size,
                 'payload_sha256': hashlib.sha256(memoryview(self.data)[e.offset + 8:e.offset + 8 + e.size]).hexdigest()}
                for e in self.extensions],
            'non_UNTR_framed_sha256': hashlib.sha256(b''.join(
                self.data[e.offset:e.offset + 8 + e.size] for e in self.extensions if e.kind != b'UNTR')).hexdigest(),
        }


def validate_tree(payload, entry_count):
    # Cache-tree DFS records: component NUL, entry_count SP child_count LF,
    # then a SHA-1 only for non-invalidated (entry_count >= 0) nodes.
    offset = 0
    pending = 1
    nodes = 0
    while pending:
        nul = payload.find(b'\0', offset)
        if nul < 0:
            raise IndexGuardError('unterminated TREE component')
        name = payload[offset:nul]
        if (nodes == 0 and name) or (nodes and (not name or b'/' in name or name in (b'.', b'..', b'.git'))):
            raise IndexGuardError('invalid TREE component')
        newline = payload.find(b'\n', nul + 1)
        if newline < 0 or newline - nul > 30:
            raise IndexGuardError('invalid TREE count record')
        match = re.fullmatch(rb'(-1|0|[1-9][0-9]*) (0|[1-9][0-9]*)', payload[nul + 1:newline])
        if not match:
            raise IndexGuardError('invalid TREE decimal counts')
        entries, children = map(int, match.groups())
        if entries > entry_count or children > len(payload) // 5:
            raise IndexGuardError('TREE counts outside supported bounds')
        offset = newline + 1 + (20 if entries >= 0 else 0)
        if offset > len(payload):
            raise IndexGuardError('truncated TREE object ID')
        nodes += 1
        pending += children - 1
        if pending > (len(payload) - offset) // 5:
            raise IndexGuardError('TREE child count exceeds remaining payload')
    if offset != len(payload):
        raise IndexGuardError('trailing TREE bytes')


def parse_index(data):
    if not isinstance(data, bytes) or not 32 <= len(data) <= MAX_INDEX_BYTES:
        raise IndexGuardError('invalid/bounded index length')
    if hashlib.sha1(data[:-20]).digest() != data[-20:]:
        raise IndexGuardError('invalid Git SHA-1 index checksum')
    signature, version, count = struct.unpack_from('>4sII', data)
    if signature != b'DIRC' or version != 3:
        raise IndexGuardError('only actual DIRC index version 3 is supported')
    end = len(data) - 20
    if count > (end - 12) // 64:
        raise IndexGuardError('index entry count exceeds available bytes')
    offset = 12
    previous = None
    for _ in range(count):
        start = offset
        if offset + 62 > end:
            raise IndexGuardError('truncated index entry')
        mode = struct.unpack_from('>I', data, offset + 24)[0]
        flags = struct.unpack_from('>H', data, offset + 60)[0]
        offset += 62
        if flags & 0x3000:
            raise IndexGuardError('unmerged/staged index stages are unsupported')
        if mode not in (0o100644, 0o100755, 0o120000, 0o160000):
            raise IndexGuardError('unknown/sparse index entry mode')
        if flags & 0x4000:
            if offset + 2 > end:
                raise IndexGuardError('truncated extended entry flags')
            extended = struct.unpack_from('>H', data, offset)[0]
            if extended & ~0x6000:
                raise IndexGuardError('unknown extended entry flags')
            offset += 2
        nul = data.find(b'\0', offset, end)
        if nul < 0:
            raise IndexGuardError('unterminated index pathname')
        name = data[offset:nul]
        if not name or name.startswith(b'/') or any(part in (b'', b'.', b'..', b'.git') for part in name.split(b'/')):
            raise IndexGuardError('invalid index pathname')
        length = flags & 0x0fff
        if (length != 0x0fff and length != len(name)) or (length == 0x0fff and len(name) < 0x0fff):
            raise IndexGuardError('pathname length/flags mismatch')
        if previous is not None and name <= previous:
            raise IndexGuardError('index entries are not strictly sorted stage-zero paths')
        previous = name
        offset = start + ((nul + 1 - start + 7) // 8) * 8
        if offset > end or any(data[nul:offset]):
            raise IndexGuardError('invalid index entry padding')
    entries_end = offset
    extensions = []
    while offset < end:
        if offset + 8 > end:
            raise IndexGuardError('truncated extension header')
        kind, size = struct.unpack_from('>4sI', data, offset)
        if kind not in (b'TREE', b'UNTR') or any(e.kind == kind for e in extensions):
            raise IndexGuardError('unknown/duplicate index extension')
        if size > end - offset - 8:
            raise IndexGuardError('extension exceeds index payload')
        extensions.append(Extension(kind, offset, size))
        if kind == b'TREE':
            validate_tree(data[offset + 8:offset + 8 + size], count)
        if kind == b'UNTR':
            try:
                validate_untr(data[offset + 8:offset + 8 + size])
            except (ValueError, OverflowError, struct.error) as exc:
                raise IndexGuardError('malformed UNTR payload: ' + str(exc)) from exc
        offset += 8 + size
    if tuple(e.kind for e in extensions) != (b'TREE', b'UNTR'):
        raise IndexGuardError('only actual ordered TREE,UNTR extension layout is supported')
    return IndexImage(data, version, count, entries_end, tuple(extensions), hashlib.sha256(data).hexdigest())


def compare_index(baseline, current, baseline_head, current_head):
    if not re.fullmatch('[0-9a-f]{40}', baseline_head) or current_head != baseline_head:
        raise IndexGuardError('native HEAD changed; never restoring it')
    if (baseline.version != current.version or baseline.entries != current.entries or
            baseline.entries_end != current.entries_end or
            memoryview(baseline.data)[:baseline.entries_end] != memoryview(current.data)[:current.entries_end]):
        raise IndexGuardError('real Chromium index header/full entries changed; never restoring it')
    if tuple(e.kind for e in baseline.extensions) != tuple(e.kind for e in current.extensions):
        raise IndexGuardError('real Chromium index extension order/types changed')
    for old, new in zip(baseline.extensions, current.extensions):
        if old.kind != b'UNTR' and (old.size != new.size or
                memoryview(baseline.data)[old.offset:old.offset + 8 + old.size] !=
                memoryview(current.data)[new.offset:new.offset + 8 + new.size]):
            raise IndexGuardError('real Chromium non-UNTR extension changed; never restoring it')
    return {
        'native_HEAD': baseline_head,
        'real_index_unchanged': baseline.data == current.data,
        'real_index_entries_unchanged': True,
        'real_index_written_by_runner': False,
        'real_index_auxiliary_only_drift_allowed': True,
        'baseline': baseline.proof(),
        'current': current.proof(),
    }
