#!/usr/bin/env python3
import csv,json
from pathlib import Path
R=Path(__file__).resolve().parents[1]
review=Path('/home/exedev/work/agents/spec-3/classreview')
findings=list(csv.DictReader((review/'findings.tsv').open(),delimiter='\t'))
assert len(findings)==9
changes=json.loads((R/'revision-evidence/method-annotation-changes.json').read_text())
after=[
'Nil follows getter-returned temporary shared-handle cleanup; no visible stored-bridge clear/unregister. Fresh raw branch bypasses the nonnil weak store.',
'Nil releases the returned temporary; stored clear/unregister and callback suppression remain unestablished. Removed the question premised on clearing.',
'Accessor copies optional native handle, engagement is checked, and helper combines two native string hashes before temporary cleanup. Equality separately compares native pointers; preference-key and pointer-hash claims removed.',
'All three outgoing native paths pass string-valued payloads through 0xc87c40; response helpers send true/false. Receiving-JavaScript interpretation remains unknown. Native-parser questions removed.',
'Vector of shared observer handles at self+0x28 is assigned with self-assignment guard, capacity reuse/reallocation and per-element ownership cleanup. Registration and synchronization are separate unknowns.',
'Malformed stack-argument/prototype recovery explicitly flagged; body status becomes available_prototype_malformed. Raw entry-SP+0/+1 bytes feed self+9/+10 and entry-SP+8 pointer feeds self+0x30 store. Declared encoding unchanged; startup behavior unverified.',
'Lifecycle VAs corrected to ArcCore 1.170.0 arm64 unslid 0x43f1968 and 0x43f21a8; already-correct matrices retained and evidence annotations extended.',
'Nil clears native delegate slots +0x20/+0x28 via 0x432bf9c(native,0,0); only temporary cleanup is visible for wrapper getter result. Wrapper bridge clearing/lifetime and queued delivery remain unproved.',
'Pinned source citation corrected to components/sessions/core/session_types.h:185 (SessionID window_id); no method identity or behavior change needed.'
]
unknown=[
'Nil detachment, native registration lifetime, complete cleanup callees, queued/in-flight callback behavior and shutdown cancellation.',
'Nil detachment, native observer ownership, delivery queue, synchronization and cancellation.',
'Native string field names, exact upstream helper symbol, mutable content/hash stability and service-reload lifetime.',
'Receiving JavaScript parsing/interpretation, downstream serialization, end-to-end JS-allowed state and pending-ID behavior.',
'Observer registration/detachment, concurrent/reentrant vector mutation and external element consumers.',
'Consumer interpretation of startup fields, restoration policy, invalid argc/argv, suite-name nil semantics and callable runtime compatibility.',
'Observer callback ordering, native registration cleanup, concurrency and shutdown behavior remain outside this typo correction.',
'Wrapper bridge retained lifetime/reuse, complete weak-handle cleanup semantics and suppression of already queued callbacks.',
'Session ID sentinel/restore semantics, old-engine ambiguity and numeric correspondence with CEF IDs.'
]
evidence=[
'revision-evidence/analysis.md findings 1-2; revision-evidence/instructions.tsv ArcCore 1.170.0 arm64 unslid 0x43ed9e8 -> 0x43eda98; revision-evidence/rechecked-original-bodies.txt entry 0x43ed9a8',
'revision-evidence/analysis.md findings 1-2; revision-evidence/instructions.tsv ArcCore 1.170.0 arm64 unslid 0x4043a08 -> 0x4043a94; revision-evidence/rechecked-original-bodies.txt entry 0x40439c4',
'revision-evidence/analysis.md finding 3; revision-evidence/rechecked-original-bodies.txt ArcCore 1.170.0 arm64 unslid entries 0x43ed260, 0x4340180, 0x43ecf8c',
'revision-evidence/analysis.md finding 4; revision-evidence/rechecked-original-bodies.txt ArcCore 1.170.0 arm64 unslid 0x8371aa8/0xc87c40/0x43d35c8/0x43d38cc; revision-evidence/boostide-boundary-bodies.txt; revision-evidence/primary-sources.tsv at Chromium d0c02bfad08ec92a6ffcf989e53cd995e6381a4c',
'revision-evidence/analysis.md finding 5; revision-evidence/rechecked-original-bodies.txt ArcCore 1.170.0 arm64 unslid entry 0x4418ce4; unchanged evidence/declared-methods.tsv encoding',
'revision-evidence/analysis.md finding 6; revision-evidence/instructions.tsv ArcCore 1.170.0 arm64 unslid 0x43eca0c-0x43ecb00; unchanged ObjC method record 0xd778314',
'revision-evidence/analysis.md finding 7; revision-evidence/nearby-original-bodies.txt ArcCore 1.170.0 arm64 unslid entries 0x43f1968/0x43f21a8; revision-evidence/validation.json binary method-list/record checks',
'revision-evidence/analysis.md finding 8; revision-evidence/instructions.tsv ArcCore 1.170.0 arm64 unslid 0x4440140-0x444014c and 0x432c06c-0x432c074; revision-evidence/rechecked-original-bodies.txt entries 0x443ff88/0x432bf9c',
'revision-evidence/analysis.md finding 9; revision-evidence/source-excerpts.txt; revision-evidence/primary-sources.tsv Chromium d0c02bfad08ec92a6ffcf989e53cd995e6381a4c components/sessions/core/session_types.h:185 and :203'
]
rows=[]
for i,f in enumerate(findings):
 rows.append(dict(finding_row=i+1,class_name=f['class'],selector=f['selector'],disposition='corrected',before=f['claim'],after=after[i],evidence=evidence[i],remaining_unknown=unknown[i]))
with (R/'corrections.tsv').open('w') as f:
 w=csv.DictWriter(f,['finding_row','class_name','selector','disposition','before','after','evidence','remaining_unknown'],delimiter='\t');w.writeheader();w.writerows(rows)
