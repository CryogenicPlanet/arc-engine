#!/usr/bin/env python3
"""Conservative ArcCore / stock arm64 correspondence experiment (not an edit detector).

Replaces the supplied failed prototype. Requires explicit Ghidra TSV for ArcCore.
Only adjacent ADRP+ADD references to exact __cstring starts are detected. Tables
and string contents live in SQLite; only compact address arrays are held in RAM.
Run substantial scans under flock /home/exedev/work/heavy.lock, one process.
"""
import argparse
import array
import bisect
import collections
import csv
import hashlib
import json
import platform
import re
import resource
import sqlite3
import struct
import sys
import time
from pathlib import Path

from macho import MachO, sha256_file

VERSION = 'initial-1'
NOP = 0xd503201f


def log(message):
    print(time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime()), message, flush=True)


def sx(value, bits):
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


def adrp_add(a, b, pc):
    if (a & 0x9f000000 == 0x90000000 and b & 0xff800000 == 0x91000000
            and a & 31 == (b >> 5) & 31):
        imm = sx(((a >> 5) & 0x7ffff) << 2 | ((a >> 29) & 3), 21)
        return (pc & ~4095) + (imm << 12) + (((b >> 10) & 4095) << (12 if b & (1 << 22) else 0))
    return None


def direct_branch(w, pc):
    if w & 0x7c000000 == 0x14000000:
        return pc + (sx(w & 0x3ffffff, 26) << 2), bool(w & 0x80000000)
    return None


def normalize(words):
    """Mask relocation-sensitive PC offsets; preserve registers and other constants.

    This is a syntactic signature, never a proof of semantic or ABI equivalence.
    Does not mask general integer constants, arbitrary ADDs, register allocation,
    stack offsets, or load/store offsets except adjacent ADRP-based ADD addresses.
    """
    out = []
    prev = None
    for w in words:
        n = w
        if w & 0x9f000000 in (0x10000000, 0x90000000):  # ADR/ADRP immlo+immhi
            n &= ~0x60ffffe0
        elif w & 0x7c000000 == 0x14000000:  # B / BL
            n &= ~0x03ffffff
        elif w & 0xff000010 == 0x54000000:  # B.cond
            n &= ~0x00ffffe0
        elif w & 0x7e000000 == 0x34000000:  # CBZ/CBNZ
            n &= ~0x00ffffe0
        elif w & 0x7e000000 == 0x36000000:  # TBZ/TBNZ
            n &= ~0x0007ffe0
        elif w & 0x3b000000 == 0x18000000:  # literal LDR / PRFM
            n &= ~0x00ffffe0
        elif prev is not None and adrp_add(prev, w, 0) is not None:
            n &= ~0x007ffc00  # shifted ADD imm12 and shift bit
        out.append(n)
        prev = w
    return out


class Bounds:
    def __init__(self, starts, ends, body=None):
        self.starts, self.ends = starts, ends
        self.body = body
        self.prefix = array.array('Q')
        maximum = 0
        for end in ends:
            self.prefix.append(maximum)
            maximum = max(maximum, end)

    def at(self, pc, size=8):
        i = bisect.bisect_right(self.starts, pc) - 1
        if i < 0 or pc + size > self.ends[i] or self.prefix[i] > pc:
            return None
        if i+1 < len(self.starts) and self.starts[i+1] < pc+size:
            return None
        if self.body is not None and self.body[i] != self.ends[i]-self.starts[i]:
            return None
        return self.starts[i]


def read_arc(path, image, db):
    starts, ends, body = (array.array('Q') for _ in range(3))
    stats = collections.Counter()
    batch = []
    previous_start = -1
    exclusions = collections.Counter()
    exclusion_examples = {}
    for line, row in enumerate(csv.reader(open(path), delimiter='\t'), 1):
        if len(row) != 4:
            raise ValueError(f'TSV row {line}: expected four fields')
        s, e, b = int(row[0], 16), int(row[1], 16), int(row[2])
        if not 0 <= s < e or b <= 0 or b > e-s:
            raise ValueError(f'TSV row {line}: invalid body bounds')
        if s <= previous_start:
            raise ValueError('TSV starts not strictly increasing')
        previous_start = s
        stats['rows'] += 1
        stats['body_bytes_sum_not_coverage'] += b
        stats['span_bytes_sum_not_coverage'] += e-s
        stats['gapped_rows' if b != e-s else 'body_equals_span_rows'] += 1
        stats['thunks' if row[3].startswith('thunk_') else 'other_rows'] += 1
        if image.in_text(s, e-s):
            if s % 4 or e % 4 or b % 4:
                raise ValueError(f'TSV row {line}: unaligned __text body')
            starts.append(s); ends.append(e); body.append(b)
            stats['text_rows'] += 1
        else:
            region = next((x['segment']+','+x['name'] for x in image.sections
                           if x['va'] <= s and e <= x['va']+x['size']), None)
            if region is None:
                try:
                    image.va_offset(s,e-s)
                    region = 'file_backed_without_section'
                except ValueError:
                    region = 'unmapped_or_not_file_backed_possible_Ghidra_external_space'
            exclusions[region] += 1
            if region not in exclusion_examples:
                exclusion_examples[region] = dict(row=line,start=hex(s),end_exclusive=hex(e),body=b,name=row[3])
        batch.append((s, e, b, row[3]))
        if len(batch) >= 4096:
            db.executemany('INSERT INTO arc VALUES (?,?,?,?)', batch); batch.clear()
    db.executemany('INSERT INTO arc VALUES (?,?,?,?)', batch)
    bounds = Bounds(starts, ends, body)
    stats['rows_starting_inside_prior_span'] = sum(p > s for p, s in zip(bounds.prefix, starts))
    # Full-span isolation is required when comparing bytes, not just a reference site.
    stats['isolated_body_equals_span_rows'] = sum(
        b == e-s and p <= s and (i+1 == len(starts) or starts[i+1] >= e)
        for i, (s, e, b, p) in enumerate(zip(starts, ends, body, bounds.prefix)))
    db.commit()
    return bounds, dict(stats, excluded_regions=dict(exclusions),excluded_region_examples=exclusion_examples)


def identities(path):
    out = collections.defaultdict(set)
    rx = re.compile(r'^\s*0x([0-9a-fA-F]+)\s+([+-]\[[^\]]+\])\s*$')
    with open(path) as f:
        for line in f:
            m = rx.match(line)
            if m:
                out[int(m[1], 16)].add(m[2])
    return out


def scan_symbols(image, wanted_identities, wanted_addresses=None):
    known = collections.defaultdict(set)
    undefined = {}
    names = collections.defaultdict(set)
    addresses = array.array('Q')
    stats = collections.Counter()
    text_idx = image.section('__TEXT', '__text')['index']
    for idx, va, nt, ns, name in image.symbols(with_index=True):
        stats['records'] += 1
        if nt & 0xe0:
            stats['stab_records'] += 1
            continue
        if nt & 0x0e == 0:
            undefined[idx] = name
        if nt & 0x0e == 0x0e and ns == text_idx:
            if not image.in_text(va, 4) or va % 4:
                raise ValueError('unaligned or out-of-text executable symbol')
            stats['text_N_SECT_records'] += 1
            addresses.append(va)
            identity = name[1:] if name.startswith('_Chrome') else name
            if identity in wanted_identities or name in wanted_identities:
                known[identity if identity in wanted_identities else name].add(va)
            if wanted_addresses is not None and va in wanted_addresses:
                names[va].add(name)
    unique = sorted(set(addresses))
    stats['distinct_text_symbol_addresses'] = len(unique)
    lc = image.starts()
    lcset = set(lc)
    stats['symbol_addresses_also_LC_FUNCTION_STARTS'] = sum(x in lcset for x in unique)
    stats['symbol_addresses_without_LC_FUNCTION_STARTS_not_used_as_body_bounds'] = sum(x not in lcset for x in unique)
    return known, image.stubs(undefined), names, dict(stats)


def index_strings(image, label, db):
    addresses, ids = array.array('Q'), array.array('I')
    n = 0
    for va, value in image.cstrings():
        if len(value) < 8:
            continue
        db.execute('INSERT OR IGNORE INTO strings(value) VALUES (?)', (value,))
        sid = db.execute('SELECT id FROM strings WHERE value=?', (value,)).fetchone()[0]
        addresses.append(va); ids.append(sid)
        db.execute('INSERT INTO locations VALUES (?,?,?)', (label, va, sid))
        n += 1
    db.commit()
    return addresses, ids, n


def scan_refs(image, label, bounds, string_addresses, string_ids, db):
    text = image.section('__TEXT', '__text')
    count = 0
    batch = []
    # Each chunk includes one extra instruction. PC sites are processed only once.
    step = 1024*1024
    for pos in range(0, text['size']-4, step):
        size = min(step+4, text['size']-pos)
        words = array.array('I', image.read(text['fileoff']+pos, size - size % 4))
        if sys.byteorder != 'little':
            words.byteswap()
        for i, a in enumerate(words[:-1]):
            if a & 0x9f000000 != 0x90000000:
                continue
            pc = text['va'] + pos + i*4
            target = adrp_add(a, words[i+1], pc)
            if target is None:
                continue
            j = bisect.bisect_left(string_addresses, target)
            if j == len(string_addresses) or string_addresses[j] != target:
                continue
            f = bounds.at(pc)
            if f is None:
                continue
            batch.append((label, f, pc, string_ids[j], target))
            count += 1
        if len(batch) >= 4096:
            db.executemany('INSERT INTO refs VALUES (?,?,?,?,?)', batch); batch.clear()
    db.executemany('INSERT INTO refs VALUES (?,?,?,?,?)', batch)
    db.commit()
    return count


def isolated(bounds, start):
    i = bisect.bisect_left(bounds.starts, start)
    return (i < len(bounds.starts) and bounds.starts[i] == start and bounds.prefix[i] <= start
            and (i+1 == len(bounds.starts) or bounds.starts[i+1] >= bounds.ends[i])
            and (bounds.body is None or bounds.body[i] == bounds.ends[i]-start))


def fingerprint(image, start, bounds, stubs, db, label):
    i = bisect.bisect_left(bounds.starts, start)
    if not isolated(bounds, start):
        return None
    span = bounds.ends[i]-start
    if not 4 <= span <= 32768:
        return None
    raw = image.bytes_at(start, span)
    words = list(struct.unpack('<' + 'I'*(len(raw)//4), raw))
    trimmed = 0
    while words and words[-1] == NOP:
        words.pop(); trimmed += 4
    norm = normalize(words)
    calls, tails = [], []
    for j, w in enumerate(words):
        branch = direct_branch(w, start + j*4)
        if branch:
            target, link = branch
            if link:
                calls.append((j, target, stubs.get(target)))
            elif not start <= target < start+span:
                tails.append((j, target, stubs.get(target)))
    refs = {row[0] for row in db.execute('SELECT DISTINCT string_id FROM refs WHERE image=? AND start=?', (label,start))}
    return dict(start=start, span=span, compared_size=len(words)*4, trailing_nop_bytes=trimmed,
                raw_hash=hashlib.sha256(raw).hexdigest(), norm=norm,
                norm_hash=hashlib.sha256(struct.pack('<'+'I'*len(norm), *norm)).hexdigest(),
                distinct_words=len(set(norm)), calls=calls, tails=tails, refs=refs)


def compare(a, b, db, anchor_ids):
    if a is None or b is None:
        return dict(status='unmatched', method='excluded_range', confidence='none', reasons=['Nonisolated, gapped, non-start, or >32768-byte range'])
    shared = sorted(a['refs'] & b['refs'])
    anchors = [x for x in shared if x in anchor_ids]
    long_anchors = [x for x in anchors if anchor_ids[x] >= 16]
    ac = [c[2] for c in sorted(a['calls'] + a['tails']) if c[2]]
    bc = [c[2] for c in sorted(b['calls'] + b['tails']) if c[2]]
    # Import context is independent of masked BL immediates. Require the entire
    # resolved import sequence to agree and at least one non-msgSend import.
    imports_agree = ac == bc and any(x not in ('_objc_msgSend','_objc_msgSendSuper2') for x in ac)
    norm_equal = a['norm_hash'] == b['norm_hash'] and a['norm'] == b['norm']
    exact = a['raw_hash'] == b['raw_hash'] and a['span'] == b['span']
    substantial = a['compared_size'] >= 96 and a['distinct_words'] >= 12
    ca, cb = collections.Counter(a['norm']), collections.Counter(b['norm'])
    similarity = round(2 * sum((ca & cb).values()) / max(1, len(a['norm']) + len(b['norm'])), 6)
    status, method, confidence = 'unmatched', 'anchor_candidate', 'none'
    if substantial and long_anchors and exact:
        status, method, confidence = 'exact_match', 'raw_bytes+unique_string_anchor', 'exact_bytes_only'
    elif substantial and long_anchors and norm_equal and imports_agree:
        status, method, confidence = 'high_confidence_hypothesis', 'reloc_masked_instructions+unique_string+import_context', 'high_correspondence_only'
    elif shared or norm_equal:
        status, method, confidence = 'ambiguous', 'strings_or_masked_signature_insufficient', 'low'
    return dict(status=status, method=method, confidence=confidence,
        arc_span=a['span'], stock_interval=b['span'], arc_compared_size=a['compared_size'], stock_compared_size=b['compared_size'],
        arc_trimmed_nops=a['trailing_nop_bytes'], stock_trimmed_nops=b['trailing_nop_bytes'],
        raw_equal=exact, normalized_equal=norm_equal, instruction_multiset_similarity_not_semantics=similarity,
        shared_string_ids=shared, unique_anchor_ids=anchors, long_unique_anchor_ids=long_anchors,
        arc_import_sequence=ac, stock_import_sequence=bc, import_context_agrees=imports_agree,
        arc_direct_calls=len(a['calls']), stock_direct_calls=len(b['calls']),
        arc_raw_sha256=a['raw_hash'], stock_raw_sha256=b['raw_hash'],
        arc_normalized_sha256=a['norm_hash'], stock_normalized_sha256=b['norm_hash'],
        reasons=['Arc body-byte count equals span; no overlapping candidate span',
                 'Stock range is LC_FUNCTION_STARTS interval, with trailing NOPs trimmed only for normalized signature',
                 'String uniqueness is scoped to detected adjacent-pair references in eligible ranges',
                 'No ABI or semantic-equivalence claim'])


def summary(fp):
    if fp is None:
        return None
    return {k:v for k,v in fp.items() if k not in ('norm','refs')}


def write_tsv(path, fields, rows):
    with open(path, 'w', newline='') as f:
        writer = csv.DictWriter(f, fields, delimiter='\t', lineterminator='\n')
        writer.writeheader(); writer.writerows(rows)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--arc', required=True); p.add_argument('--stock', required=True)
    p.add_argument('--ghidra-tsv', required=True); p.add_argument('--objc-dyld', required=True)
    p.add_argument('--out', required=True)
    p.add_argument('--controls', type=int, default=384)
    p.add_argument('--sample', type=int, default=512)
    p.add_argument('--requests', type=int, default=40)
    args = p.parse_args()
    out = Path(args.out); out.mkdir(parents=True, exist_ok=True)
    dbpath = out/'evidence.sqlite'
    if dbpath.exists():
        raise SystemExit(f'{dbpath} exists; use a fresh output directory')
    db = sqlite3.connect(dbpath)
    db.executescript('''PRAGMA cache_size=-24576; PRAGMA mmap_size=0; PRAGMA temp_store=FILE;
        CREATE TABLE arc(start INTEGER PRIMARY KEY,end INTEGER,body INTEGER,name TEXT);
        CREATE TABLE strings(id INTEGER PRIMARY KEY,value BLOB UNIQUE);
        CREATE TABLE locations(image TEXT,va INTEGER,string_id INTEGER);
        CREATE TABLE refs(image TEXT,start INTEGER,pc INTEGER,string_id INTEGER,target INTEGER);
    ''')
    log('validate images and hashes')
    arc, stock = MachO(args.arc), MachO(args.stock)
    images = dict(arc=arc.describe(), stock=stock.describe())
    (out/'images.json').write_text(json.dumps(images, indent=2)+'\n')
    log('validate all Arc Ghidra candidates')
    ab, coverage = read_arc(args.ghidra_tsv, arc, db)
    ss = stock.starts()
    if not ss:
        raise ValueError('stock LC_FUNCTION_STARTS required for this experiment')
    st = stock.section('__TEXT','__text')
    sb = Bounds(ss, array.array('Q', list(ss[1:]) + [st['va']+st['size']]))
    aid = identities(args.objc_dyld)
    wanted = {n for names in aid.values() for n in names}
    for va, nt, ns, name in arc.symbols():
        if not nt & 0xe0 and nt & 0x0e == 0x0e and arc.in_text(va,4):
            aid[va].add(name)
            wanted.add(name)
    log('read stock symbols, preserve explicit ObjC class/selector/kind identities')
    known, stock_stubs, _, stock_symbol_stats = scan_symbols(stock, wanted)
    _, arc_stubs, _, arc_symbol_stats = scan_symbols(arc, set())
    controls = []
    for a, ids in aid.items():
        if not isolated(ab, a):
            continue
        idx = bisect.bisect_left(ab.starts,a)
        if not 64 <= ab.ends[idx]-a <= 8192:
            continue
        for name in sorted(ids):
            partners = known.get(name, set())
            if len(partners) == 1:
                s = next(iter(partners))
                if isolated(sb,s):
                    controls.append((name,a,s))
    controls.sort(key=lambda r: hashlib.sha256(r[0].encode()).digest())
    seen, selected = set(), []
    for row in controls:
        if row[1] not in seen:
            selected.append(row); seen.add(row[1])
    controls = selected[:args.controls]
    log(f'{len(controls)} named identity controls selected without inspecting fingerprints')
    counts = {}
    for label, image, bounds in [('arc',arc,ab),('stock',stock,sb)]:
        log(f'index {label} strings')
        va, ids, n = index_strings(image,label,db)
        log(f'scan {label} __text for adjacent ADRP+ADD string references')
        refs = scan_refs(image,label,bounds,va,ids,db)
        counts[label] = dict(cstring_locations_length_ge_8=n, detected_string_reference_sites=refs)
        del va, ids
    log('index evidence and derive uniquely anchored candidate pairs')
    db.executescript('''CREATE INDEX refs_function ON refs(image,start,string_id);
        CREATE INDEX refs_string ON refs(string_id,image,start);
        CREATE INDEX locations_string ON locations(string_id,image);
        CREATE TABLE ref_functions AS SELECT DISTINCT image,start,string_id FROM refs;
        CREATE INDEX ref_functions_string ON ref_functions(string_id,image,start);
        CREATE TABLE anchors AS
          SELECT a.string_id,a.start arc_start,s.start stock_start,length(v.value) length
          FROM ref_functions a JOIN ref_functions s ON a.string_id=s.string_id
          JOIN strings v ON v.id=a.string_id
          WHERE a.image='arc' AND s.image='stock'
          AND (SELECT count(*) FROM ref_functions x WHERE x.string_id=a.string_id AND x.image='arc')=1
          AND (SELECT count(*) FROM ref_functions x WHERE x.string_id=s.string_id AND x.image='stock')=1;
        CREATE INDEX anchors_arc ON anchors(arc_start,stock_start);
    ''')
    db.commit()
    anchor_ids = dict(db.execute('SELECT string_id,length FROM anchors'))
    cache = {}
    def fp(label,start):
        key = label,start
        if key not in cache:
            image, bounds, stubs = (arc,ab,arc_stubs) if label=='arc' else (stock,sb,stock_stubs)
            cache[key] = fingerprint(image,start,bounds,stubs,db,label)
        return cache[key]
    def cmp(a,s):
        return compare(fp('arc',a),fp('stock',s),db,anchor_ids)
    accepted = {'exact_match','high_confidence_hypothesis'}
    log('evaluate named controls BEFORE classifying unnamed candidates')
    control_results, decoys = [], []
    for name,a,s in controls:
        r = cmp(a,s)
        candidates = sorted({x[0] for x in db.execute('SELECT stock_start FROM anchors WHERE arc_start=?',(a,))})
        predicted = [(q,cmp(a,q)['status']) for q in candidates]
        control_results.append(dict(identity=name,arc_va=hex(a),stock_va=hex(s),comparison=r,
            blind_unique_string_candidates=[dict(stock_va=hex(q),status=st) for q,st in predicted],
            wrong_accepted=[hex(q) for q,st in predicted if st in accepted and q!=s]))
    # Real decoys: shifted identities, plus nearest-size wrong identity with a
    # normalized signature or string resemblance. Duplicate byte controls below.
    for i, (name,a,s) in enumerate(controls):
        wrong = controls[(i+1) % len(controls)] if len(controls)>1 else None
        if wrong and wrong[2] != s:
            r = cmp(a,wrong[2])
            decoys.append(dict(kind='rotated_named_identity',arc_identity=name,
                stock_identity=wrong[0],arc_va=hex(a),stock_va=hex(wrong[2]),comparison=r))
    duplicate_controls = []
    # RET bodies and common getters must never pass even when byte-identical.
    ret = next((s for s,e in zip(ab.starts,ab.ends) if e-s==4 and arc.bytes_at(s,4)==bytes.fromhex('c0035fd6')),
               None)
    if ret is not None:
        synthetic = dict(start=0,span=4,compared_size=4,trailing_nop_bytes=0,
                         raw_hash=hashlib.sha256(bytes.fromhex('c0035fd6')).hexdigest(),
                         norm=[0xd65f03c0],norm_hash=hashlib.sha256(bytes.fromhex('c0035fd6')).hexdigest(),
                         distinct_words=1,calls=[],tails=[],refs=set())
        duplicate_controls.append(dict(kind='synthetic_identical_RET_from_real_Arc_body',arc_va=hex(ret),
            comparison=compare(synthetic,synthetic,db,anchor_ids)))
    # Find actual identical normalized bodies among different named stock controls.
    by_norm = collections.defaultdict(list)
    for name,a,s in controls:
        f = fp('stock',s)
        if f:
            by_norm[f['norm_hash']].append((name,a,s))
    for group in by_norm.values():
        if len({x[2] for x in group}) > 1:
            left,right = group[0],next(x for x in group if x[2]!=group[0][2])
            duplicate_controls.append(dict(kind='real_distinct_stock_identities_same_masked_signature',
                left_identity=left[0],right_identity=right[0],left_stock_va=hex(left[2]),right_stock_va=hex(right[2]),
                comparison=compare(fp('stock',left[2]),fp('stock',right[2]),db,{})))
    tiny_by_norm = collections.defaultdict(list)
    for name, addresses in sorted(known.items()):
        for s in sorted(addresses):
            i = bisect.bisect_left(sb.starts,s)
            if i < len(sb.starts) and sb.starts[i] == s and sb.ends[i]-s <= 32:
                f = fp('stock',s)
                if f:
                    tiny_by_norm[f['norm_hash']].append((name,s))
    for group in tiny_by_norm.values():
        if len({x[1] for x in group}) > 1:
            left,right = group[0],next(x for x in group if x[1]!=group[0][1])
            duplicate_controls.append(dict(kind='real_distinct_stock_tiny_functions_same_masked_signature',
                left_identity=left[0],right_identity=right[0],left_stock_va=hex(left[1]),right_stock_va=hex(right[1]),
                comparison=compare(fp('stock',left[1]),fp('stock',right[1]),db,{})))
            if sum(x['kind']=='real_distinct_stock_tiny_functions_same_masked_signature' for x in duplicate_controls)>=10:
                break
    naccepted = sum(x['comparison']['status'] in accepted for x in control_results)
    false_accepts = sum(x['comparison']['status'] in accepted for x in decoys+duplicate_controls)
    wrong_blind = sum(len(x['wrong_accepted']) for x in control_results)
    gate = naccepted >= 5 and false_accepts == 0 and wrong_blind == 0
    log(f'controls accepted={naccepted}/{len(controls)} false_accepts={false_accepts} wrong_blind={wrong_blind}; unknown high-confidence gate={gate}')
    # Unnamed sample uses strongest observed independent string anchors first;
    # not a random or representative sample of all engine functions.
    proposals = db.execute('''SELECT x.arc_start,x.stock_start,count(*),max(x.length)
        FROM anchors x JOIN arc a ON a.start=x.arc_start
        WHERE a.name LIKE 'FUN_%' AND a.body=a.end-a.start AND a.body BETWEEN 96 AND 8192
        GROUP BY x.arc_start,x.stock_start ORDER BY count(*) DESC,max(x.length) DESC,x.arc_start,x.stock_start LIMIT ?''',(args.sample,)).fetchall()
    results = []
    for a,s,n,longest in proposals:
        r = cmp(a,s)
        if not gate and r['status'] in accepted:
            r['ungated_status'] = r['status']
            r.update(status='ambiguous',confidence='unvalidated',method='validation_gate_closed')
        results.append(dict(arc_start=a,stock_start=s,selection_anchor_count=n,selection_longest_anchor=longest,comparison=r))
    # Do not assign multiple high-confidence counterparts to a single function.
    ax, sxcount = collections.Counter(), collections.Counter()
    for row in results:
        if row['comparison']['status'] in accepted:
            ax[row['arc_start']] += 1; sxcount[row['stock_start']] += 1
    for row in results:
        r = row['comparison']
        if r['status'] in accepted and (ax[row['arc_start']]>1 or sxcount[row['stock_start']]>1):
            r.update(status='ambiguous',confidence='duplicate_candidate',method='multiple_accepted_counterparts')
    need_names = {s for _,_,s in controls} | {r['stock_start'] for r in results}
    log(f'resolve stock names for {len(need_names)} sampled candidate addresses')
    _, _, names, _ = scan_symbols(stock,set(),need_names)
    rows = []
    for row in results:
        a,s,r = row['arc_start'],row['stock_start'],row['comparison']
        r.update(selection_anchor_count=row['selection_anchor_count'],selection_longest_anchor=row['selection_longest_anchor'])
        rows.append(dict(scope='unnamed_anchored_sample',arc_va=hex(a),arc_name=db.execute('SELECT name FROM arc WHERE start=?',(a,)).fetchone()[0],
            stock_va=hex(s),stock_names=json.dumps(sorted(names.get(s,()))),
            status=r['status'],method=r['method'],confidence=r['confidence'],reasons=json.dumps(r,sort_keys=True)))
    for row in control_results:
        r = row['comparison']; a,s=int(row['arc_va'],16),int(row['stock_va'],16)
        rows.append(dict(scope='named_identity_control',arc_va=hex(a),arc_name=row['identity'],stock_va=hex(s),
            stock_names=json.dumps(sorted(names.get(s,()))),status=r['status'],method=r['method'],
            confidence=r['confidence'],reasons=json.dumps(r,sort_keys=True)))
    write_tsv(out/'matches.tsv',['scope','arc_va','arc_name','stock_va','stock_names','status','method','confidence','reasons'],rows)
    # Reproducible call targets and string content evidence for every tested pair.
    write_tsv(out/'fingerprints.tsv',['image','start','fingerprint_json'],[
        dict(image=k[0],start=hex(k[1]),fingerprint_json=json.dumps(summary(v),sort_keys=True))
        for k,v in sorted(cache.items())])
    used_strings = sorted({x for row in rows for x in json.loads(row['reasons']).get('shared_string_ids',[])})
    write_tsv(out/'strings.tsv',['id','value_json','arc_cstring_vas','stock_cstring_vas'],[
        dict(id=sid,value_json=json.dumps(db.execute('SELECT value FROM strings WHERE id=?',(sid,)).fetchone()[0].decode('utf8','replace')),
            arc_cstring_vas=json.dumps([hex(x[0]) for x in db.execute("SELECT va FROM locations WHERE image='arc' AND string_id=?",(sid,))]),
            stock_cstring_vas=json.dumps([hex(x[0]) for x in db.execute("SELECT va FROM locations WHERE image='stock' AND string_id=?",(sid,))]))
        for sid in used_strings])
    priorities = sorted((r for r in results if r['comparison']['status'] not in accepted),
        key=lambda r: (-r['selection_anchor_count'],-r['comparison'].get('instruction_multiset_similarity_not_semantics',0),r['arc_start']))
    requests = []
    for row in priorities[:args.requests]:
        a,s = row['arc_start'],row['stock_start']
        e,b,n = db.execute('SELECT end,body,name FROM arc WHERE start=?',(a,)).fetchone()
        requests.append(dict(priority=len(requests)+1,image='ArcCore-1.170.0.arm64',address_basis='unslid engine VA',
            arc_start=hex(a),arc_end_exclusive=hex(e),body_bytes=b,arc_name=n,stock_start=hex(s),
            stock_names=json.dumps(sorted(names.get(s,()))),
            reason=f"{row['selection_anchor_count']} unique detected string anchors; masked instruction equality={row['comparison'].get('normalized_equal')}; optimization/configuration changes remain possible",
            request='Engine decompile shard containing this exact entry plus direct callees; include actual Ghidra body ranges, prototype and callers; compare with named stock function/source before attributing edits'))
    write_tsv(out/'decompile-requests.tsv',['priority','image','address_basis','arc_start','arc_end_exclusive','body_bytes','arc_name','stock_start','stock_names','reason','request'],requests)
    validation = dict(version=VERSION,argv=sys.argv,python=platform.python_version(),
        input_hashes={str(x):sha256_file(x) for x in [args.ghidra_tsv,args.objc_dyld]},
        image_hashes={k:v['sha256'] for k,v in images.items()},coverage=coverage,
        stock_symbol_stats=stock_symbol_stats,arc_symbol_stats=arc_symbol_stats,
        counts=counts,unique_detected_string_anchors=len(anchor_ids),
        candidate_pairs_total=db.execute('SELECT count(*) FROM (SELECT arc_start,stock_start FROM anchors GROUP BY arc_start,stock_start)').fetchone()[0],
        named_controls_available_before_cap=len(selected),named_control_count=len(controls),
        named_control_status_counts=dict(collections.Counter(x['comparison']['status'] for x in control_results)),
        positive_identity_controls_accepted=naccepted,negative_identity_controls=len(decoys),duplicate_controls=len(duplicate_controls),
        false_accepts_in_explicit_negative_controls=false_accepts,wrong_named_blind_accepts=wrong_blind,
        empirical_precision_among_tested_acceptances=(naccepted/(naccepted+false_accepts+wrong_blind) if naccepted+false_accepts+wrong_blind else None),
        precision_scope='Named identity correspondence on selected controls only; does not establish semantics, ABI, or population-wide precision',
        unknown_high_confidence_gate=gate,gate_rule='>=5 accepted named identity controls, zero accepted explicit decoys and zero wrong blind identity matches',
        unnamed_sample_pairs=len(results),unnamed_sample_distinct_arc_functions=len({r['arc_start'] for r in results}),
        unnamed_status_counts=dict(collections.Counter(r['comparison']['status'] for r in results)),
        named_controls=control_results,negative_controls=decoys,duplicate_signature_controls=duplicate_controls,
        peak_rss_kib=resource.getrusage(resource.RUSAGE_SELF).ru_maxrss,
        limitations=['No engine pseudocode consulted; files absent at start',
            'No full Ghidra AddressSet body map; gaps excluded and equality of body/span remains metadata evidence',
            'Stock LC_FUNCTION_STARTS intervals are inferred bounds and may contain padding/data',
            'Adjacent ADRP+ADD only, exact __cstring entry addresses, length >=8; no ownership or ABI inference',
            'Unknown sample selected by strongest observed anchors; unmatched is not proprietary/modified',
            'Different build/LTO/inlining/ICF can explain all mismatches; no byte-percentage estimate'])
    (out/'validation.json').write_text(json.dumps(validation,indent=2,sort_keys=True)+'\n')
    db.close()
    log(json.dumps({k:validation[k] for k in ['named_control_status_counts','unnamed_status_counts','peak_rss_kib']},sort_keys=True))


if __name__ == '__main__':
    main()
