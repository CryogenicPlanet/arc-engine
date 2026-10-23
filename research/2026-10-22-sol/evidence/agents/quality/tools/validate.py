#!/usr/bin/env python3
"""Focused independent cross-check against exact indexed failure bodies and counts."""
import collections,csv,hashlib,json,re,sqlite3
from pathlib import Path
I=Path('/home/exedev/work/inputs');O=Path('initial');j=json.loads((O/'coverage.json').read_text());D=sqlite3.connect(O/'audit.sqlite');idx=sqlite3.connect(f'file:{I}/corpus-index.sqlite?mode=ro',uri=True)
def rows(n):return list(csv.DictReader((O/n).open(),delimiter='\t'))
assert j['manifest_files']==3 and j['manifest_records']==j['unique_shards']==j['verified_manifest_records']==199 and not j['manifest_mismatches']
actual={str(p) for base in [I/'decompile',I/'decompile-extra'] for p in base.rglob('*.c')};listed={r['path'] for r in rows('manifest-verification.tsv')};assert actual==listed
assert j['C_headers']==567959 and j['TSV_entries']==607294 and j['failures']==79 and j['C_headers']-j['failures']==567880
assert D.execute('select count(*) from bodies').fetchone()[0]==idx.execute('select count(*) from functions').fetchone()[0]
f=rows('failures.tsv');assert len(f)==len({r['va'] for r in f})==79
categories=collections.Counter();independent_fail=set()
for va,path,offset,length in idx.execute('select va,path,offset,length from functions where failed=1'):
 assert length<32768
 with Path(path).open('rb') as fh:fh.seek(offset);b=fh.read(length).decode()
 err=b.split('/* decompile failed: ',1)[1].split(' */',1)[0].strip();rr=next(r for r in f if int(r['va'],16)==va);assert rr['error']==err;categories[rr['category']]+=1;independent_fail.add(va)
assert independent_fail=={r[0] for r in D.execute('select va from bodies where failed=1')}
assert categories=={'timeout':48,'varnode_hash_nonunique':24,'address_space_range':3,'input_varnode_adjustment':3,'forced_merge_intersection':1}
assert j['handoff']['exact_reproduction'] and j['handoff']['supplied_rows']==5846 and j['handoff']['zero_name_join_sizes']==524
for va,name,n,path in D.execute('select va,name,n,path from bodies'):
 assert int(Path(path).stem,16)==va>>20
 rr=idx.execute('select name,reported_size from functions where va=?',(va,)).fetchone();assert rr==(name,n)
# Full row comparison is streamed; bounded database caches and one Python CPU.
assert j['bounds']['isolated_spans_within_TEXT_text']==24528
assert j['engine170_binary_export_trie_matches_supplied_names'] and j['engine170_binary_export_trie_names']==259
inventory=json.loads((O/'inventory.json').read_text());assert inventory['app_import_list_binary_verified'] and inventory['app_imported_classes']==63
assert inventory['versions']['170']['arc_metadata_classes']==146 and inventory['versions']['170']['arc_method_rows']==1964
assert len(rows('method-coverage.tsv'))==1964 and len(rows('class-coverage.tsv'))==146 and len(rows('spec-expectations.tsv'))==146
assert all(r['identity_status']=='match' for r in rows('spec-inventory-checks.tsv'))
assert len(rows('old-encoding-unknowns.tsv'))==6
aper=json.loads((O/'aperitif.json').read_text());assert aper['sha256']==hashlib.sha256(Path(aper['file']).read_bytes()).hexdigest()
for a in aper['architectures']:assert len(a['symtab_imports'])==59 and a['symtab_external_definitions']==[e['name'] for e in a['export_trie']]==['_AperitifCheckInitialized']
result={'status':'PASS','manifest_record_hashes':199,'C_header_entries_cross_checked_against_read_only_index':567959,'failure_bodies_re_read':79,'failure_categories':dict(categories),'TSV_rows':607294,'class_assignments':146,'method_declarations':1964,'spec_inventory_groups_identity_checked':4,'scope':'Artifact consistency and structural triage only; no behavioral assurance.'}
(O/'validation.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
