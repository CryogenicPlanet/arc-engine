#!/usr/bin/env python3
"""Read the engine's export trie; compare supplied export-name snapshots."""
import argparse
import collections
import json
import struct
from pathlib import Path
from inventory import MachO,digest,tsv


def main():
    ap=argparse.ArgumentParser()
    ap.add_argument('--inputs',type=Path,default=Path('/home/exedev/work/inputs'))
    ap.add_argument('--out',type=Path,default=Path('initial'))
    a=ap.parse_args();m=MachO(a.inputs/'binaries/ArcCore-1.170.0.arm64')
    pos=32;trie=None
    for cmd in m.commands:
        c,n=struct.unpack('<II',m.read(pos,8))
        if c==0x80000033:
            off,size=struct.unpack('<II',m.read(pos+8,8));trie=m.read(off,size)
        pos+=n
    assert trie is not None
    def uleb(pos):
        v=0;shift=0
        while True:
            b=trie[pos];pos+=1;v|=(b&127)<<shift
            if not b&128:return v,pos
            shift+=7
            assert shift<70
    found={}
    def visit(pos,prefix,depth):
        assert depth<100
        term,pos=uleb(pos);end=pos+term
        if term:
            flags,p=uleb(pos)
            assert not flags&8, 'reexport needs separate address treatment'
            address,p=uleb(p)
            found[prefix]=(flags,address+m.imagebase)
        n=trie[end];p=end+1
        for i in range(n):
            e=trie.index(0,p);edge=trie[p:e].decode();target,p=uleb(e+1)
            visit(target,prefix+edge,depth+1)
    visit(0,'',0)
    data=a.inputs/'context/analysis/data'
    names={v:set((data/f'core{v}-exports.txt').read_text().splitlines()) for v in ('168','169','170')}
    assert set(found)==names['170'],(len(found),len(names['170']))
    imports=set((data/'app168-imports.txt').read_text().splitlines())
    fn=collections.defaultdict(list)
    for l in open(a.inputs/'metadata/decomp/arccore-1.170.0-functions.tsv'):
        va,end,size,name=l.rstrip('\n').split('\t',3)
        if name in found:fn[name].append(va)
    rows=[]
    for symbol,(flags,address) in sorted(found.items()):
        kind='objc_class' if symbol.startswith('_OBJC_CLASS_$_') else 'objc_metaclass' if symbol.startswith('_OBJC_METACLASS_$_') else 'function' if symbol in fn else 'nonfunction_or_unnamed'
        assert symbol not in fn or hex(address) in fn[symbol],symbol
        rows.append(dict(symbol=symbol,kind=kind,va=hex(address),file_offset=hex(m.offset(address)),export_flags=hex(flags),
                         present_168=symbol in names['168'],present_169=symbol in names['169'],
                         app168_import=symbol in imports,ghidra_function_va=hex(address) if symbol in fn else '',
                         all_ghidra_name_anchors=','.join(fn.get(symbol,[]))))
    tsv(a.out/'exported-interface.tsv',list(rows[0]),rows)
    stats=dict(exports=len(rows),kinds=dict(collections.Counter(r['kind'] for r in rows)),
               name_sets_equal_168_169_170=names['168']==names['169']==names['170'],
               app168_imports=len(imports),app168_imports_not_exported=sorted(imports-set(found)),
               sha256={f'core{v}-exports.txt':digest(data/f'core{v}-exports.txt') for v in names})
    (a.out/'export-counts.json').write_text(json.dumps(stats,indent=2)+'\n')
    print(json.dumps(stats,indent=2))


if __name__=='__main__':main()
