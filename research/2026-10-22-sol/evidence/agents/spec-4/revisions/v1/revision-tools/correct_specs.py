#!/usr/bin/env python3
"""Apply reviewed prose/annotation corrections only to this revision directory."""
import csv, collections, json
from pathlib import Path
R=Path(__file__).resolve().parents[1]
assert R==Path('/home/exedev/work/agents/spec-4/revisions/v1')
O=Path('/home/exedev/work/agents/spec-4/specs')
rows=list(csv.DictReader((O/'methods.tsv').open(),delimiter='\t'));fields=list(rows[0])
docs={p.stem:p.read_text() for p in (O/'classes').glob('*.md')}
changes=[]
def method(cls,sel,behavior=None,question=None,flag=None,status=None,evidence=None,cat=None):
 found=[r for r in rows if r['class_name']==cls and r['selector']==sel and (cat is None or r['category']==cat)]
 assert len(found)==1,(cls,sel)
 r=found[0];old=dict(r)
 if behavior:
  assert r['behavior'] in docs[cls]
  docs[cls]=docs[cls].replace(r['behavior'],behavior);r['behavior']=behavior
 if question:r['unresolved_question']=question
 if flag:r['decompiler_flags']='; '.join(filter(None,[r['decompiler_flags'],flag]))
 if status:
  docs[cls]=docs[cls].replace(f"M; {r['body_status']}; {r['semantic_status']}",f"M; {status}; {r['semantic_status']}",1)
  r['body_status']=status
 if evidence:r['evidence']+='; '+evidence
 changes.append((old,dict(r)))
def replace(cls,old,new):
 assert old in docs[cls],(cls,old)
 docs[cls]=docs[cls].replace(old,new)
method('ArcBrowserContext','setDelegate:',
 'Creates a bridge with weak delegate/self and installs it through 0x4333868 on nonnil creation; updates an existing bridge for nonnil delegate. Nil follows cleanup of the getter-returned shared-handle temporary; no stored bridge clear or unregister call is visible on that path.',
 'Nil detachment, native registration lifetime and effects on in-flight callbacks remain unestablished; observed temporary cleanup does not prove callback suppression.',
 evidence='revision-evidence/analysis.md findings 1-2; revision-evidence/instructions.tsv (nil branch 0x43ed9e8 -> 0x43eda98)',cat='Extensions')
method('ArcBrowserContextHistory','setVisitObserver:',
 'Creates shared visit-observer bridge with weak observer/owner and installs through 0x4332f08 for nonnil creation; updates an existing bridge for nonnil observer. Nil releases the getter-returned shared-handle temporary; no stored bridge clear or unregister call is visible on that path.',
 'Nil detachment and native observer ownership remain unestablished; queued/in-flight delivery and shutdown cancellation need separate evidence.',
 evidence='revision-evidence/analysis.md findings 1-2; revision-evidence/instructions.tsv (nil branch 0x4043a08 -> 0x4043a94)',cat='Extensions')
method('ArcSearchEngine','hash',
 'Calls search_engine_template_url accessor to copy the optional native handle; requires engagement, calls 0x4340180 to combine hashes of two native string fields at Impl+0x3b8/+0x3d0, then releases the temporary shared owner.',
 'Native string field names, exact hash-helper symbol and hash stability across content mutation/service reload are unproved; equality separately compares native pointers.',
 evidence='revision-evidence/rechecked-original-bodies.txt entries 0x43ed260 and 0x4340180; revision-evidence/analysis.md finding 3',cat='Extensions')
method('ArcSearchEngine','isEqual:',
 'Checks receiver identity, nil and class compatibility; compares optional engagement and, when both are engaged, underlying native pointers. This does not perform a fieldwise content comparison.',
 'Native pointer lifetime, content mutation and hash/equality behavior across service reload remain unverified.',
 evidence='revision-evidence/rechecked-original-bodies.txt entry 0x43ecf8c',cat='Extensions')
method('ArcWebContentsBoostIDE','sendEvent:jsonString:',
 'Converts event name/payload bytes and forwards to 0x8371aa8; 0xc87c40 constructs string-valued arguments for cr.webUIListenerCallback. No JSON parsing appears in this native path.',
 'Receiving JavaScript interpretation, event naming and JS lifecycle gating remain untraced; no native JSON parser/error contract is established.',
 evidence='revision-evidence/rechecked-original-bodies.txt entries 0x8371aa8/0xc87c40; revision-evidence/primary-sources.tsv')
method('ArcWebContentsBoostIDE','sendResponse:jsonString:',
 'Converts command/request ID and payload bytes (nil to empty), checks native handler and forwards to 0x43d35c8. For a matching pending ID, string-valued payload construction through 0xc87c40 precedes 0x68122a8 and cr.webUIResponse true.',
 'Pending-ID matching/removal, JS-allowed gate, response ID format and receiving JavaScript payload interpretation need end-to-end validation.',
 evidence='revision-evidence/rechecked-original-bodies.txt entries 0x43d35c8/0xc87c40; revision-evidence/primary-sources.tsv')
method('ArcWebContentsBoostIDE','sendErrorResponse:jsonString:',
 'Converts ID/payload bytes and forwards to 0x43d38cc when native handler is live. For a matching pending ID, string-valued payload construction through 0xc87c40 precedes 0x6812404 and cr.webUIResponse false.',
 'Pending-ID matching/removal, JS-allowed gate, response ID format and receiving JavaScript payload interpretation need end-to-end validation.',
 evidence='revision-evidence/rechecked-original-bodies.txt entries 0x43d38cc/0xc87c40; revision-evidence/primary-sources.tsv')
method('ArcBrowserContextPreferences','setBrowser_context_preferences_observer_bridges:',
 'Assigns the vector of 16-byte shared observer handles at self+0x28, skipping self-assignment. Reuses or reallocates capacity, increments copied element control counts and releases replaced/removed elements and obsolete allocation; metadata identifies this shared IMP despite its Ghidra class name.',
 'Vector mutation synchronization, element lifetime under reentrancy and observer registration/detachment remain unproved by storage assignment.',
 evidence='revision-evidence/rechecked-original-bodies.txt entry 0x4418ce4; declared vector encoding retained')
method('ArcBrowserApplicationConfiguration','init:argv:profilePath:applicationID:isProduction:sessionRestorationPolicy:shouldLoadAllProfilesAtStartup:quitRunLoopOnTermination:userDefaultsSuiteName:',
 'Initializes NSObject and stores configuration fields. Ghidra stack-argument recovery is malformed (second byte of a bool; pointer assembled from bool). Raw instructions load entry-SP bytes +0/+1 into self+9/+10, and entry-SP pointer +8 into the object-store call for self+0x30; this corroborates the final fields without validating the recovered C prototype.',
 'Declared encoding is retained; raw stack loads/stores corroborate final fields, but startup consumers, restoration-policy meanings, argc/argv consistency and suite-name nil behavior remain unverified.',
 flag='malformed_stack_argument_recovery; entry_stack_mapping_raw_corroborated',status='available_prototype_malformed',
 evidence='revision-evidence/instructions.tsv 0x43eca48, 0x43eca88-0x43ecac4; revision-evidence/analysis.md finding 6')
for sel in ['addBookmarkModelObserver:','removeBookmarkModelObserver:']:
 method('ArcBrowserContext',sel,evidence='revision-evidence/nearby-original-bodies.txt; revision-evidence/analysis.md finding 7 (correct unslid ArcCore 1.170.0 IMP; lifecycle typo corrected)',cat='Extensions')
method('ArcMenuItemModel','setDelegate:',
 'Creates weak model/delegate bridge and registers via 0x433cc10 for nonnil delegate. Nil releases a getter-returned temporary and calls 0x432bf9c(native,0,0), clearing native delegate slots at +0x20/+0x28. No wrapper shared-bridge clear is visible on that path.',
 'Wrapper bridge retained lifetime/reuse, native weak-handle cleanup and in-flight changed/removed callbacks remain unproved; native slot clearing does not establish callback suppression.',
 evidence='revision-evidence/rechecked-original-bodies.txt entries 0x443ff88/0x432bf9c; revision-evidence/instructions.tsv 0x4440110-0x444014c and 0x432c06c-0x432c074')
replace('ArcBrowserContext','`0x443f1968`, `0x443f21a8`','`0x43f1968`, `0x43f21a8`')
replace('ArcBrowserContext','Cached wrappers and native Profile shutdown are distinct lifetimes.',
 'Cached wrappers and native Profile shutdown are distinct lifetimes. The nil delegate path cleans up a returned temporary; stored-bridge removal and callback suppression are not established. See [revision evidence](../revision-evidence/analysis.md).')
replace('ArcBrowserContextHistory','and changes stored observer for existing bridge.',
 'and changes the observer for an existing bridge when the supplied observer is nonnil. Nil takes a temporary-handle cleanup path; stored clear/unregister and callback suppression are not established. See [revision evidence](../revision-evidence/analysis.md).')
replace('ArcSearchEngine','Category equality/hash use native record identity, not a proven fieldwise comparison.',
 'Category equality compares native pointers after optional checks (`0x43ecf8c`); hash calls `0x4340180` to combine hashes from native string fields +0x3b8/+0x3d0 (`0x43ed260`). String field names and exact upstream hash symbol remain unproved. See [revision evidence](../revision-evidence/analysis.md).')
replace('ArcWebContentsBoostIDE','Callback queue, JavaScript lifecycle and parse errors remain unresolved.',
 'The native outgoing paths construct string-valued payloads through `0xc87c40`; they do not show JSON parsing. Parsing or other interpretation by receiving JavaScript remains untraced. Callback queue and JavaScript lifecycle remain unresolved. See [revision evidence](../revision-evidence/analysis.md).')
replace('ArcWebContentsBoostIDE','native handler availability/JS-allowed conditions, JSON parse failure and command ID format.',
 'native handler availability/JS-allowed conditions, receiving JavaScript interpretation of string payloads and command ID format.')
replace('ArcBrowserApplicationConfiguration','`0x43eca0c` calls NSObject init and stores constructor arguments in metadata-listed ivars;',
 '`0x43eca0c` calls NSObject init and stores configuration fields, but its recovered stack-argument expressions are malformed. At entry-SP+0/+1 raw byte loads feed self+9/+10; entry-SP+8 supplies the pointer passed to the object-store call for self+0x30. This corroborates the final field stores while the C prototype remains unreliable ([instruction analysis](../revision-evidence/analysis.md));')
replace('ArcBrowserApplicationConfiguration','validate argc/argv consistency and suite-name nil behavior.',
 'validate argc/argv consistency and suite-name nil behavior. Keep the malformed recovered prototype distinct from the declared metadata encoding and raw stack stores.')
replace('ArcMenuItemModel','`0x443ff88` installs native bridge and nil path unregisters through `0x432bf9c`.',
 '`0x443ff88` installs a native bridge; its nil path clears native delegate slots through `0x432bf9c(native,0,0)`. The wrapper getter supplies a temporary that is released; no wrapper stored-bridge clear is visible. Retained lifetime/reuse and queued callback effects remain unknown ([instruction analysis](../revision-evidence/analysis.md)).')
replace('ArcSessionID','`components/sessions/core/session_types.h:203` references SessionIDs in session data',
 '`components/sessions/core/session_types.h:185` declares `SessionID window_id` in session data')
# No other baseline files are regenerated.
with (R/'methods.tsv').open('w') as f:
 w=csv.DictWriter(f,fields,delimiter='\t');w.writeheader();w.writerows(rows)
for cls,doc in docs.items():
 if doc!=(O/'classes'/f'{cls}.md').read_text():(R/'classes'/f'{cls}.md').write_text(doc)
cov=json.loads((O/'coverage.json').read_text())
cov['body_status_counts']=dict(collections.Counter(r['body_status'] for r in rows))
cov['semantic_status_counts']=dict(collections.Counter(r['semantic_status'] for r in rows))
cov['revision']=dict(writer='spec-4',name='v1',findings=9,correction_rows=9,changed_class_files=8,annotated_method_rows=len(changes),new_evidence='revision-evidence/',validation='revision-evidence/validation.json',baseline_evidence='evidence/ remains the unchanged original snapshot; revised annotations/counts are in methods.tsv and this file',cef_commit='426920c33ef22551e0b8f48cf343d623ac25642d')
(R/'coverage.json').write_text(json.dumps(cov,indent=2)+'\n')
(R/'revision-evidence/method-annotation-changes.json').write_text(json.dumps([dict(before=a,after=b) for a,b in changes],indent=2)+'\n')
