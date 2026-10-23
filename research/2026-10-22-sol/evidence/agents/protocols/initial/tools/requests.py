#!/usr/bin/env python3
"""Exact function repair requests and metadata checks for unresolved sends."""
from audit import BASE,INPUT,load,tsv
from macho import MachO
import json
def main():
    coverage=json.loads((BASE/'coverage.json').read_text());unresolved=coverage['candidate_identities_without_binary_send'];sels={r['selector'] for r in unresolved};refs=[]
    for label,file in [('engine168','ArcCore-1.168.0.arm64'),('engine170','ArcCore-1.170.0.arm64')]:
        b=MachO(INPUT/'binaries'/file);by={s:[] for s in sels}
        for (_,sec),(va,n,off) in b.sections.items():
            if sec!='__objc_selrefs':continue
            for p in range(va,va+n,8):
                v=b.pointer(p);s=b.string(v)
                if s in sels:by[s].append((p,v))
        for r in unresolved:
            found=by[r['selector']]
            refs.append(dict(image=label,**r,selector_refs=';'.join(hex(p) for p,v in found),selector_string_vas=';'.join(hex(v) for p,v in found),status='selector_ref_present;send_unresolved' if found else 'not_found_in_parsed_selrefs;dynamic_or_other_forms_not_excluded'))
    tsv(BASE/'evidence/unresolved-selector-refs.tsv',refs)
    requests=[]
    names={0x4042728:'filename delegate dispatch',0x40428ac:'filename completion adapter',0x43eb7a0:'profile creation ObjC completion adapter',0x4413b74:'UI vtable entry;purpose unresolved',0x4413c7c:'artifact link permission bridge candidate',0x44462cc:'JavaScript alert bridge',0x4446470:'JavaScript confirm bridge',0x4446608:'JavaScript text input bridge',0x4446944:'before-unload bridge',0x4446bc8:'permission prompt bridge',0x4447188:'extension uninstall consent bridge',0x4447d88:'background CPU notification candidate',0x44482d0:'frame/chat association candidate',0x44484c8:'frame/chat disassociation candidate',0x4448634:'frame/chat data candidate'}
    for r in load(BASE/'evidence/focus-body-index.tsv'):
        if r['status']!='no_exact_exported_header':continue
        va=int(r['va'],16)
        requests.append(dict(request_kind='function_boundary_repair_not_shard_transfer',engine_va=r['va'],selector='',purpose=names.get(va,'vtable-selected callback candidate'),expected_bucket=f'{va>>20:#x}.c',evidence='evidence/focus-body-index.tsv;evidence/focus-vtables.tsv;evidence/focus-instructions.tsv',question='Recover coherent native/ObjC callback boundaries and adjacent invoke adapters; all 199 shards are already present.'))
    for r in unresolved:requests.append(dict(request_kind='selector_send_and_receiver_route',engine_va='',selector=r['selector'],purpose=r['protocol_name'],expected_bucket='unknown;do_not_guess',evidence='evidence/unresolved-selector-refs.tsv',question='Find a coherent actual send, availability check and receiver registration path; uncorroborated does not mean unused.'))
    tsv(BASE/'next-requests.tsv',requests)
    print(dict(boundary_repairs=sum(r['request_kind'].startswith('function') for r in requests),unresolved_selectors=len(unresolved)))
if __name__=='__main__':main()
