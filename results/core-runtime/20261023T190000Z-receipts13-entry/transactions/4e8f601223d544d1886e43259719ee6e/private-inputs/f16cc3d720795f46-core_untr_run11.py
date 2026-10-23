"""Pure, bounded UNTR/SHA-1 structural validation; no Git/filesystem operations.

API: validate_untr(payload: bytes, *, hash_size=20) -> dict; raises
UNTRFormatError on unsupported/malformed input. Caller must independently
validate the index checksum, version/header/entries, extension framing/order,
and exact non-UNTR identity. This does not establish cache freshness or intent.

Sources: installed Apple Git 2.50.1 gitformat-index(5), UNTRACKED CACHE;
exact primary upstream v2.50.1 source (local C source was unavailable):
https://github.com/git/git/blob/v2.50.1/dir.c#L3320-L3441
https://github.com/git/git/blob/v2.50.1/dir.c#L3496-L3666
https://github.com/git/git/blob/v2.50.1/statinfo.h#L15-L23
https://github.com/git/git/blob/v2.50.1/varint.c
https://github.com/git/git/blob/v2.50.1/ewah/ewah_io.c
https://github.com/git/git/blob/v2.50.1/ewah/ewok_rlw.h

Deliberately limited: SHA-1 only, <=128MiB payload, <=1,000,000 directories,
<=4096 directory depth. Rejects empty env-string members, noncanonical EWAH
word coverage, and non-final RLW pointers even if a permissive Git reader
could tolerate them. Names remain opaque bytes, not UTF-8. Stat words,
dir_flags, environment identities and OIDs are opaque cache metadata; no
assertion about their truth is made. No checksum exists inside UNTR itself.
"""

import struct


class UNTRFormatError(ValueError):
    pass


def validate_untr(payload, *, hash_size=20):
    if not isinstance(payload, bytes):
        raise UNTRFormatError("payload must be immutable bytes")
    if hash_size != 20:
        raise UNTRFormatError("only known SHA-1 format supported")
    if not 2 <= len(payload) <= 128 * 1024 * 1024:
        raise UNTRFormatError("payload length outside supported bounds")
    if payload[-1] != 0:
        raise UNTRFormatError("missing terminal NUL")
    end = len(payload) - 1
    pos = 0

    def take(n):
        nonlocal pos
        if n < 0 or n > end - pos:
            raise UNTRFormatError("truncated payload")
        start = pos
        pos += n
        return payload[start:pos]

    def varint(limit):
        # Git OFS_DELTA encoding: continuation adds one before shifting;
        # this is not LEB128. Bound both bytes and decoded value.
        nonlocal pos
        value = 0
        for index in range(10):
            c = take(1)[0]
            value = ((value + 1) << 7) + (c & 127) if index else c & 127
            if value > limit:
                raise UNTRFormatError("varint exceeds field bounds")
            if not c & 128:
                return value
        raise UNTRFormatError("overlong varint")

    def cstring():
        nonlocal pos
        z = payload.find(b"\0", pos, end)
        if z < 0:
            raise UNTRFormatError("unterminated string")
        value = payload[pos:z]
        pos = z + 1
        return value

    env_len = varint(end)
    env = take(env_len)
    if not env or not env.endswith(b"\0") or any(not x for x in env[:-1].split(b"\0")):
        raise UNTRFormatError("invalid environment string sequence")
    # struct stat_data is nine uint32 words (NO mode field): 36 bytes.
    fixed_offset = pos
    take(2 * 36)
    flags = struct.unpack(">I", take(4))[0]
    take(2 * hash_size)
    exclude = cstring()
    if not exclude:
        raise UNTRFormatError("empty per-directory exclude name")
    # Git's no-root writer emits just one final zero, simultaneously the
    # zero-directory encoding and safe string-list terminator. Also allow the
    # documented explicit zero count followed by a separate final terminator.
    if pos == end:
        return {"directories": 0, "untracked": 0, "dir_flags": flags,
                "fixed_offset": fixed_offset, "bytes": len(payload),
                "empty_root_encoding": "writer-terminal-zero"}
    count_offset = pos
    directories = varint(min(1_000_000, end - pos))
    if directories == 0:
        if pos != end:
            raise UNTRFormatError("trailing data after zero directories")
        return {"directories": 0, "untracked": 0, "dir_flags": flags,
                "fixed_offset": fixed_offset, "bytes": len(payload),
                "empty_root_encoding": "explicit-count-and-terminal"}

    # Iterative DFS prevents attacker-controlled Python recursion. Each stack
    # counter is the remaining direct children of an open directory.
    stack = [1]
    seen = 0
    total_untracked = 0
    while stack:
        if stack[-1] == 0:
            stack.pop()
            continue
        stack[-1] -= 1
        if seen >= directories:
            raise UNTRFormatError("directory tree exceeds declared count")
        untracked = varint(end - pos)
        children = varint(directories - seen - 1)
        name = cstring()
        if seen == 0 and name != b"":
            raise UNTRFormatError("root directory name must be empty")
        if seen and not name:
            raise UNTRFormatError("empty child directory name")
        for _ in range(untracked):
            if not cstring():
                raise UNTRFormatError("empty untracked name")
        total_untracked += untracked
        seen += 1
        if children:
            if len(stack) >= 4096:
                raise UNTRFormatError("directory depth outside supported bounds")
            stack.append(children)
    if seen != directories:
        raise UNTRFormatError("directory tree shorter than declared count")

    def bitmap():
        # Serialized EWAH: bit_size:u32, word_count:u32, words:u64[],
        # rlw_position:u32. RLW bit0=run value, bits1..32=run length,
        # bits33..63=number of immediately following literal words.
        start = pos
        bits, words = struct.unpack(">II", take(8))
        if bits > directories or words < 1 or words > (end - pos - 4) // 8:
            raise UNTRFormatError("invalid bitmap size/count")
        data = take(words * 8)
        rlw_position = struct.unpack(">I", take(4))[0]
        index = 0
        covered = 0
        ones = 0
        last_rlw = None
        word_limit = (bits + 63) // 64
        while index < words:
            last_rlw = index
            word = struct.unpack_from(">Q", data, index * 8)[0]
            index += 1
            run_value = word & 1
            run_length = (word >> 1) & 0xFFFFFFFF
            literals = word >> 33
            if literals > words - index or covered + run_length + literals > word_limit:
                raise UNTRFormatError("bitmap run/literal bounds")
            if run_value and run_length:
                if (covered + run_length) * 64 > bits:
                    raise UNTRFormatError("bitmap run sets padding bits")
                ones += run_length * 64
            covered += run_length
            for _ in range(literals):
                literal = struct.unpack_from(">Q", data, index * 8)[0]
                index += 1
                available = min(64, bits - covered * 64)
                if available < 0 or literal >> available:
                    raise UNTRFormatError("bitmap literal sets padding bits")
                ones += literal.bit_count()
                covered += 1
            if not run_length and not literals and words != 1:
                raise UNTRFormatError("empty internal bitmap RLW")
        if covered != word_limit or rlw_position != last_rlw:
            raise UNTRFormatError("bitmap coverage or final RLW pointer")
        return {"offset": start, "bits": bits, "words": words,
                "ones": ones, "rlw_position": rlw_position}

    maps = [bitmap(), bitmap(), bitmap()]
    # IMPORTANT: valid bitmap #1 selects STAT records; sha1_valid bitmap #3
    # selects HASH records. They do not share a selection bitmap.
    stats_offset = pos
    take(maps[0]["ones"] * 36)
    hashes_offset = pos
    take(maps[2]["ones"] * hash_size)
    if pos != end:
        raise UNTRFormatError("unconsumed bytes before final NUL")
    return {"directories": directories, "untracked": total_untracked,
            "dir_flags": flags, "fixed_offset": fixed_offset,
            "count_offset": count_offset, "bitmaps": maps,
            "stats_offset": stats_offset, "hashes_offset": hashes_offset,
            "bytes": len(payload)}


if __name__ == "__main__":
    import json
    import pathlib
    import sys
    for argument in sys.argv[1:]:
        print(json.dumps({"path": argument,
                          "validation": validate_untr(pathlib.Path(argument).read_bytes())},
                         sort_keys=True))
