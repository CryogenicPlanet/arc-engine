#!/usr/bin/env python3
"""Supplement imported-class inventory with layout, callback and request evidence."""
import collections,csv,json,re,struct
from inventory import ROOT,OUT,DATA,MachO,write_tsv,dump_blocks,fields_at,value_text,otool
from calls import functions

def read_methods(b,va):
    if not va:return []
    flags,n=struct.unpack('<II',b.read(va,8));size=flags&0xffff
    assert n<10000 and size in [12,24],(hex(va),hex(flags),n)
    result=[]
    for i in range(n):
        p=va+8+i*size
        if flags&0x80000000:
            a,t,imp=struct.unpack('<iii',b.read(p,12))
            ref=p+a
            sel=b.string(ref if flags&0x40000000 else b.pointer(ref))
            enc=b.string(p+4+t);implementation=p+8+imp
        else:
            sel=b.string(b.pointer(p));enc=b.string(b.pointer(p+8));implementation=b.pointer(p+16)
        result.append(dict(selector=sel,type_encoding=enc,imp=hex(implementation),entry_va=hex(p),list_va=hex(va)))
    return result

def protocol_refs(b,va):
    if not va:return []
    n=b.u64(va);assert n<1000
    return [b.pointer(va+8+8*i) for i in range(n)]

def dump_detail(version):
    details=[]
    for block in dump_blocks(version,'__objc_classlist)'):
        cut=next((i for i,(_,l) in enumerate(block) if l=='Meta Class'),len(block));block=block[:cut]
        f=fields_at(block,8)
        if 'name' not in f:continue
        name=value_text(f['name'][0])
        if not name.startswith('Arc'):continue
        entry=dict(class_name=name,otool_line=block[0][0],ivars=[],properties=[],protocols=[],strong_map=[],weak_map=[])
        top=fields_at(block,4)
        # Superclass symbol when present; local pointer addresses are not ABI
        # differences. Every Arc class here has a named NSObject superclass.
        entry['superclass']=value_text(top.get('superclass',('',0))[0])
        for k in ['instanceStart','instanceSize','flags']:entry[k]=f.get(k,('',0))[0]
        active='';row=None
        for ln,line in block:
            n=len(line)-len(line.lstrip());s=line.strip()
            if n==8:
                field=s.split()[0] if s else ''
                if field=='layout':
                    entry['weak_map' if active=='weakIvarLayout' else 'strong_map'].append(s.split('map',1)[1].strip());continue
                active=field
            if n==16 and active=='baseProtocols' and s.startswith('name '):entry['protocols'].append(value_text(s.split(None,1)[1]))
            if n!=12:continue
            if active=='ivars':
                if s.startswith('offset '):
                    row={'offset':s.split()[-1]};entry['ivars'].append(row)
                elif row:
                    k,_,v=s.partition(' ');v=v.strip()
                    if k in ['name','type']:row[k]=value_text(v)
                    elif k in ['size','alignment']:row[k]=v
            if active=='baseProperties':
                if s.startswith('name '):row=dict(name=value_text(s.split(None,1)[1]));entry['properties'].append(row)
                elif row and s.startswith('attributes '):row['attributes']=value_text(s.split(None,1)[1])
        details.append(entry)
    return details

def category_detail(version):
    _,layouts=otool(version);names={x['class_va']:x['class_name'] for x in layouts};rows=[]
    for block in dump_blocks(version,'__objc_catlist)'):
        f=fields_at(block,4)
        if 'cls' not in f:continue
        cv=f['cls'][0];m=re.search(r'_OBJC_CLASS_\$_(\w+)',cv)
        name=m[1] if m else names.get(cv.split()[0],'')
        if not name.startswith('Arc'):continue
        r=dict(class_name=name,category=value_text(f['name'][0]),otool_line=block[0][0],protocols=[],instance_properties=[],class_properties=[])
        active='';prop=None
        for ln,line in block:
            n=len(line)-len(line.lstrip());s=line.strip()
            if n==4:active=s.split()[0]
            if active=='protocols' and n==12 and s.startswith('name '):r['protocols'].append(value_text(s.split(None,1)[1]))
            if active not in ['instanceProperties','classProperties'] or n!=8:continue
            if s.startswith('name '):
                prop=dict(name=value_text(s.split(None,1)[1]));r['instance_properties' if active=='instanceProperties' else 'class_properties'].append(prop)
            if prop and s.startswith('attributes '):prop['attributes']=value_text(s.split(None,1)[1])
        rows.append(r)
    return rows

def main():
    b=MachO(ROOT/'binaries/Arc-1.168.0.arm64')
    pva,psz,_=b.sections[('__DATA_CONST','__objc_protolist')]
    protos={};proto_methods=[]
    def parse_proto(p):
        if p in protos:return protos[p]
        name=b.string(b.pointer(p+8))
        info=dict(name=name,va=hex(p),inherited=[],methods=[]);protos[p]=info
        info['inherited']=[parse_proto(q)['name'] for q in protocol_refs(b,b.pointer(p+16))]
        for off,kind,required in [(24,'-',True),(32,'+',True),(40,'-',False),(48,'+',False)]:
            for m in read_methods(b,b.pointer(p+off)):
                m.update(protocol=name,protocol_va=hex(p),kind=kind,required=required)
                info['methods'].append(m)
                if name.startswith('Arc'):proto_methods.append(m)
        return info
    for a in range(pva,pva+psz,8):parse_proto(b.pointer(a))
    byname={p['name']:p for p in protos.values()}
    callbacks=[];app_classes=[]
    cva,csz,_=b.sections[('__DATA_CONST','__objc_classlist')]
    for a in range(cva,cva+csz,8):
        cls=b.pointer(a);ro=b.pointer(cls+32)&~7
        name=b.string(b.pointer(ro+24))
        if not name.startswith('_TtC3ADK'):continue
        ps=[parse_proto(q)['name'] for q in protocol_refs(b,b.pointer(ro+40))]
        arcps=[p for p in ps if p.startswith('Arc')]
        if not arcps:continue
        full=set()
        def inherit(n):
            if n in full:return
            full.add(n)
            for q in byname[n]['inherited']:inherit(q)
        for p in arcps:inherit(p)
        candidate=collections.defaultdict(list)
        for p in full:
            for m in byname[p]['methods']:candidate[(m['kind'],m['selector'])].append(m)
        app_classes.append(dict(class_name=name,class_va=hex(cls),ro_va=hex(ro),protocols=';'.join(ps),
            instanceStart=struct.unpack('<I',b.read(ro+4,4))[0],instanceSize=struct.unpack('<I',b.read(ro+8,4))[0]))
        for kind,mp in [('-',b.pointer(ro+32)),('+',b.pointer((b.pointer(b.pointer(cls)+32)&~7)+32))]:
            for m in read_methods(b,mp):
                matches=candidate.get((kind,m['selector']),[])
                if not matches:continue
                callbacks.append(dict(app_version='1.168.0',app_class=name,class_va=hex(cls),kind=kind,**m,
                    protocols=';'.join(sorted({q['protocol'] for q in matches})),
                    protocol_type_encodings=';'.join(sorted({q['type_encoding'] for q in matches})),
                    protocol_encoding_match=all(q['type_encoding']==m['type_encoding'] for q in matches),
                    encoding_comparison='raw_equal' if all(q['type_encoding']==m['type_encoding'] for q in matches) else 'raw_difference_including_object_or_block_annotations;ABI_equivalence_not_established',
                    evidence_status='app_binary_method+protocol_declaration;engine_invocation_unproven',
                    app_pseudocode_file='',app_pseudocode_line='',app_pseudocode_name=''))
    # Source address lookup preserves exact app VA and does not confuse it with
    # engine low VAs. Only headers for known callback implementations retained.
    cb_byva=collections.defaultdict(list)
    for r in callbacks:cb_byva[r['imp']].append(r)
    for path in sorted((ROOT/'app168/ghidra/decomp').glob('*.c')):
        for h,body in functions(path):
            for r in cb_byva.get(h['va'],[]):
                r.update(app_pseudocode_file=h['path'],app_pseudocode_line=h['header_line'],app_pseudocode_name=h['name'])
    write_tsv(OUT/'app-arc-protocol-methods.tsv',proto_methods)
    write_tsv(OUT/'app-callbacks.tsv',callbacks)
    write_tsv(OUT/'app-callback-classes.tsv',app_classes)
    details={v:dump_detail(v) for v in ['168','170']}
    for v,rows in details.items():(OUT/f'arc-class-details-{v}.json').write_text(json.dumps(rows,indent=2)+'\n')
    d168={x['class_name']:x for x in details['168']};changes=[]
    for x in details['170']:
        prev=d168.get(x['class_name'])
        if not prev:changes.append(dict(class_name=x['class_name'],field='class',old='',new='added'));continue
        for k in ['superclass','instanceStart','instanceSize','flags','ivars','properties','protocols','strong_map','weak_map']:
            if prev[k]!=x[k]:changes.append(dict(class_name=x['class_name'],field=k,old=json.dumps(prev[k]),new=json.dumps(x[k])))
    write_tsv(OUT/'arc-layout-semantic-changes.tsv',changes,['class_name','field','old','new'])
    cats={v:category_detail(v) for v in ['168','170']}
    for v,rows in cats.items():(OUT/f'arc-category-details-{v}.json').write_text(json.dumps(rows,indent=2)+'\n')
    normalized_cats=lambda rows:{(r['class_name'],r['category']):{k:v for k,v in r.items() if k!='otool_line'} for r in rows}
    summary=dict(app_arc_protocols=len({r['protocol'] for r in proto_methods}),app_arc_protocol_method_rows=len(proto_methods),
        callback_classes=len(app_classes),app_callback_method_rows=len(callbacks),callback_protocol_raw_encoding_differences=sum(not r['protocol_encoding_match'] for r in callbacks),
        callback_methods_with_app_pseudocode=sum(bool(r['app_pseudocode_file']) for r in callbacks),
        arc_engine_classes_170=len(details['170']),arc_semantic_metadata_changes=len(changes),
        arc_engine_categories_170=len(cats['170']),arc_category_metadata_equal=normalized_cats(cats['168'])==normalized_cats(cats['170']))
    (OUT/'boundaries-summary.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary,indent=2))

if __name__=='__main__':main()
