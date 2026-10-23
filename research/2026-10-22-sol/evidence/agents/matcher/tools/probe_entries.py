#!/usr/bin/env python3
"""Supplemental diagnostic on named method entry windows, NOT full bodies.

Windows stop at the next Ghidra entry, stock LC start, or 256 bytes. This avoids
using the enormous TSV spans as bodies, but does not establish body membership.
Names supply held-out identity labels; byte features do not use names. No result
from this experiment promotes an unknown-code match in the main pipeline.
"""
import argparse
import bisect
import collections
import csv
import hashlib
import json
from pathlib import Path
import resource
import sqlite3
import struct

from engine_match import identities, normalize, scan_symbols, write_tsv
from macho import MachO, sha256_file


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--out',required=True)
    p.add_argument('--arc',required=True)
    p.add_argument('--stock',required=True)
    p.add_argument('--objc-dyld',required=True)
    args=p.parse_args()
    out=Path(args.out)
    validation=json.loads((out/'validation.json').read_text())
    for label,path in [('arc',args.arc),('stock',args.stock)]:
        if sha256_file(path)!=validation['image_hashes'][label]:
            raise ValueError('image changed since primary experiment')
    db=sqlite3.connect(f'file:{out}/evidence.sqlite?mode=ro',uri=True)
    arc,stock=MachO(args.arc),MachO(args.stock)
    aid=identities(args.objc_dyld)
    wanted={name for names in aid.values() for name in names}
    known,_,_,stats=scan_symbols(stock,wanted)
    arc_bounds={s:(e,b,n) for s,e,b,n in db.execute('SELECT start,end,body,name FROM arc') if s in aid}
    # Compact starts alone are sufficient for window boundaries.
    starts=[r[0] for r in db.execute('SELECT start FROM arc ORDER BY start') if arc.in_text(r[0],4)]
    ss=stock.starts()
    records=[]
    seen=set()
    for a,names in sorted(aid.items()):
        if a not in arc_bounds:
            continue
        e,b,ghidra_name=arc_bounds[a]
        ai=bisect.bisect_right(starts,a)
        ae=min(e, starts[ai] if ai<len(starts) else e, a+256)
        for name in sorted(names):
            possible=known.get(name,set())
            if len(possible)!=1:
                continue
            s=next(iter(possible))
            si=bisect.bisect_left(ss,s)
            if si==len(ss) or ss[si]!=s or si+1==len(ss):
                continue
            if (a,s) in seen:
                continue
            seen.add((a,s))
            length=min(ae-a,ss[si+1]-s)
            if length<16:
                continue
            length-=length%4
            ar=arc.bytes_at(a,length);sr=stock.bytes_at(s,length)
            aw=struct.unpack('<'+'I'*(length//4),ar);sw=struct.unpack('<'+'I'*(length//4),sr)
            an,sn=normalize(aw),normalize(sw)
            ac,sc=collections.Counter(an),collections.Counter(sn)
            rec=dict(identity=name,arc_va=hex(a),stock_va=hex(s),arc_tsv_end_exclusive=hex(e),
                     arc_tsv_body_bytes=b,ghidra_name=ghidra_name,window_bytes=length,
                     raw_equal=ar==sr,normalized_equal=an==sn,
                     instruction_multiset_similarity_not_semantics=round(2*sum((ac&sc).values())/(len(an)+len(sn)),6),
                     normalized_arc=an,normalized_stock=sn)
            records.append(rec)
    # Blind closed-set lookup: signatures at fixed widths, collisions count as
    # ambiguity even if a true name match is among the colliding methods.
    retrieval={}
    negatives=[]
    for width in (16,64,128):
        n=width//4
        eligible=[r for r in records if r['window_bytes']>=width]
        index=collections.defaultdict(set)
        for r in eligible:
            index[tuple(r['normalized_stock'][:n])].add(r['stock_va'])
        count=collections.Counter()
        ambiguous=[]
        wrong=[]
        for r in eligible:
            targets=index.get(tuple(r['normalized_arc'][:n]),set())
            if len(targets)>1:
                count['ambiguous']+=1
                if len(ambiguous)<10:
                    ambiguous.append(dict(identity=r['identity'],arc_va=r['arc_va'],true_stock_va=r['stock_va'],candidate_stock_vas=sorted(targets)))
            elif not targets:
                count['no_exact_masked_prefix_candidate']+=1
            elif r['stock_va'] in targets:
                count['unique_correct_identity']+=1
            else:
                count['unique_wrong_identity']+=1
                if len(wrong)<10:wrong.append(dict(identity=r['identity'],arc_va=r['arc_va'],true_stock_va=r['stock_va'],predicted_stock_vas=sorted(targets)))
        # Rotated named identities are explicit negatives at equal fixed length.
        false_equal=0
        for i,r in enumerate(eligible):
            q=eligible[(i+1)%len(eligible)]
            if q['stock_va']==r['stock_va']:
                continue
            eq=r['normalized_arc'][:n]==q['normalized_stock'][:n]
            false_equal+=eq
            if eq and len(negatives)<20:
                negatives.append(dict(width=width,arc_identity=r['identity'],stock_wrong_identity=q['identity'],arc_va=r['arc_va'],stock_va=q['stock_va']))
        correct=count['unique_correct_identity'];incorrect=count['unique_wrong_identity']
        retrieval[str(width)]=dict(eligible_identity_pairs=len(eligible),counts=dict(count),
            closed_set_unique_identity_precision=correct/(correct+incorrect) if correct+incorrect else None,
            rotated_wrong_identity_equal_prefixes=false_equal,ambiguous_examples=ambiguous,wrong_unique_examples=wrong)
    rows=[]
    for r in records:
        rows.append({k:(json.dumps(v) if isinstance(v,(list,dict)) else v) for k,v in r.items()})
    write_tsv(out/'entry-pairs.tsv',list(rows[0]) if rows else [],rows)
    output=dict(method='named_identity_entry_window_diagnostic_only',
        image_hashes=validation['image_hashes'],objc_dyld_sha256=sha256_file(args.objc_dyld),
        stock_symbol_stats=stats,identity_pairs=len(records),
        full_window_raw_equal=sum(r['raw_equal'] for r in records),
        full_window_normalized_equal=sum(r['normalized_equal'] for r in records),
        full_window_normalized_different=sum(not r['normalized_equal'] for r in records),
        fixed_width_closed_set_retrieval=retrieval,negative_equal_prefix_examples=negatives,
        peak_rss_kib=resource.getrusage(resource.RUSAGE_SELF).ru_maxrss,
        limitations=['Explicit class, selector and method kind come from engine 1.170 dyld names and stock nlist names; no type-encoding pairing attempted',
            'Entry windows are executable bytes, not proven Ghidra body blocks; no full-function exact/semantic claim',
            'Closed set includes only known identity controls, not all stock functions; apparent precision is optimistic',
            'No unknown-code matches are promoted using these numbers'])
    (out/'entry-validation.json').write_text(json.dumps(output,indent=2,sort_keys=True)+'\n')
    print(json.dumps({k:output[k] for k in ['identity_pairs','full_window_raw_equal','full_window_normalized_equal','full_window_normalized_different','peak_rss_kib']},sort_keys=True))


if __name__=='__main__':main()
