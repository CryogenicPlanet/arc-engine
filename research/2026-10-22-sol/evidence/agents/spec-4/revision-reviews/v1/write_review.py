#!/usr/bin/env python3
"""Write this independent review's deliverables, never the reviewed artifacts."""
import csv,json,re
from pathlib import Path
H=Path(__file__).resolve().parent
R=Path('/home/exedev/work/agents/spec-4/revisions/v1')
V=Path('/home/exedev/work/agents/spec-3/classreview')
def rows(p):
    with p.open() as f:return list(csv.DictReader(f,delimiter='\t'))
def write(name,fields,rs):
    with (H/name).open('w') as f:
        w=csv.DictWriter(f,fields,delimiter='\t');w.writeheader();w.writerows(rs)
identity='ArcCore 1.170.0 arm64; unslid VAs'
c=rows(R/'corrections.tsv')
evidence={
1:'Revised classes/ArcBrowserContext.md:20,56 and methods.tsv data row 431 remove stored-clear claim. finding-bodies.txt:1 getter-returned control-pointer cleanup; instructions.tsv 0x43ed9c0/0x43ed9e4/0x43ed9e8 -> 0x43eda98 bypasses nonnil weak store 0x43eda20. Exact binary method record 0xd7784bc reproduced.',
2:'Revised classes/ArcBrowserContextHistory.md:18,54 and methods.tsv data row 418 distinguish temporary cleanup; question no longer presupposes clearing. finding-bodies.txt:196; instructions.tsv 0x4043a04/0x4043a08 -> 0x4043a94 bypasses nonnil weak store 0x4043a40. Exact binary method record 0xd773ef0 reproduced.',
3:'Revised classes/ArcSearchEngine.md:15,43,44 and methods.tsv data rows 427/428 distinguish pointer equality from native content-derived hash. finding-bodies.txt:322,375,443: accessor, optional check, two native-string fields +0x3b8/+0x3d0, two 0x18f3c calls, arithmetic combination and temporary release. No preference lookup appears.',
4:'Revised classes/ArcWebContentsBoostIDE.md:20,34-36,45 and methods.tsv data rows 148-150 consistently describe string payloads. boostide-bodies.txt:1,62,105,276,447,502,518,534,591,649 verifies all three wrappers, byte-copy/tag-4 constructor, pending-ID matching, string views and success/error calls. boostide-boundary-bodies.txt:1 verifies Boolean tag 1; instructions.tsv 0xc87ca8/0xc87cc0/0xc87cc4 corroborates copy/tag. Pinned source-excerpts.txt reproduces base/values.h:746-754, base/values.cc:182-191 and web_ui_message_handler.h:76-105.',
5:'Revised classes/ArcBrowserContextPreferences.md:57 and methods.tsv data row 272 describe vector assignment and synchronization limits. finding-bodies.txt:547 at 0x4418ce4 has destination self+0x28, self-assignment guard, capacity/reallocation branches and per-element copy/release. Binary record 0xd77baa8 retains vector<shared_ptr<...>> encoding. Analogous bookmark vector at 0x443808c verified in analogous-bodies.txt:215.',
6:'Revised classes/ArcBrowserApplicationConfiguration.md:23,39,53 and methods.tsv data row 9 flag malformed stack/prototype recovery. finding-bodies.txt:841 retains invalid bool-byte/CONCAT44 expressions. instructions.tsv 0x43eca0c/0x43eca28 establishes entry SP = x29+16; 0x43eca88/0x43eca8c loads bytes +1/+0, 0x43ecab4/0x43ecab8 stores self+9/+10, 0x43eca48 loads pointer entry+8 forwarded at 0x43ecabc-0x43ecac4. Binary record 0xd778314 preserves declared encoding. Status counts reproduce: 482 ordinary, 4 malformed, 4 raw-corrected, 1 suspect.',
7:'Revised classes/ArcBrowserContext.md:20 now cites 0x43f1968/0x43f21a8. analogous-bodies.txt:509,791 exact original entries; binary-method-check.tsv independently decodes records 0xd778594/0xd7785a0 to those IMPs. All class-file occurrences of the two erroneous addresses removed. Original method order/IMPs unchanged; data rows 449/450 only extend evidence annotations.',
8:'Revised classes/ArcMenuItemModel.md:24,48 and methods.tsv data row 400 distinguish native from wrapper slots. finding-bodies.txt:879,1021; instructions.tsv 0x444011c getter/temporary cleanup, 0x4440140-0x444014c loads native pointer and passes zero x1/x2 to 0x432bf9c; 0x432c06c/0x432c070 loads prior control then stp x1,xzr at native+0x20/+0x28. No wrapper clear is claimed.',
9:'Revised classes/ArcSessionID.md:23 cites pinned components/sessions/core/session_types.h:185. Independently read and hashed source: source-check.tsv and source-excerpts.txt; line 185 is SessionID window_id, line 203 is int selected_tab_index. No declaration or method identity changes.'
}
dispositions=[]
for i,x in enumerate(c,1):
    dispositions.append(dict(finding_row=i,status='resolved' if i in (7,9) else 'resolved_with_limits',evidence=identity+'; '+evidence[i],remaining_unknown=x['remaining_unknown']))
write('finding-dispositions.tsv',['finding_row','status','evidence','remaining_unknown'],dispositions)
write('findings.tsv',['severity','class_name','selector','claim','evidence','required_correction'],[])

notes={
'ArcAutocompleteMatchAdditionalData':'Nil-to-empty conversion and inline optional string storage remain supported; borrowed-pointer caveat retained.',
'ArcBreadcrumb':'Dictionary iteration/string conversion and optional source fields remain bounded data observations.',
'ArcBrowserApplicationConfiguration':'Finding 6 corrected; declared encoding and raw stack-to-field mapping distinguished from malformed recovered prototype.',
'ArcBrowserContext':'Findings 1/7 corrected; nil only described as temporary cleanup; bookmark IMP typos fixed; category definitions remain separate.',
'ArcBrowserContextEffects':'Sampled stylesheet conversion, engagement gate and native mode-0 forwarding support the retained service description.',
'ArcBrowserContextHistory':'Finding 2 corrected; nil detachment is explicitly unknown; existing/new nonnil observer paths kept distinct.',
'ArcBrowserContextPreferences':'Finding 5 corrected; vector assignment at self+0x28 and per-element ownership replace single-slot claim; observer creation remains separately described.',
'ArcBrowserSidePanel':'Nil wrapper stores and close-request forwarding checked; closure-completion and native-registration behavior remain separate.',
'ArcBrowserSidePanelEntryInfo':'Shared byte setter is interpreted using this class metadata, rather than the unrelated Ghidra class name.',
'ArcCommandBarRankingModel':'Descriptor construction/engagement/version stores support container description; no model execution inferred.',
'ArcCommandBarRankingTrainingResult':'Constructor storage is supported; delta cleanup-before-read remains expressly suspect and unresolved, confirmed by fresh original-body reads.',
'ArcCookie':'Engaged shared native record construction is supported; direct mutations remain distinguished from cookie-store persistence.',
'ArcCpuInfo':'Snapshot calls and explicit zeroing support arm64-only observations; swapped-looking ARM getter identities remain metadata facts.',
'ArcCrashKeysReporter':'Global construction, nil empty callback and nonnil weak observer capture remain supported; guard is not generalized to thread safety.',
'ArcCreditCardInfoSaveRequest':'Optional shared request copying and control-count handling checked; outcome/controller interpretations remain bounded by prior review.',
'ArcExtensionCommand':'Absent shortcut returns nil; engaged shortcut copies native string representation. No dispatch behavior inferred.',
'ArcExtensionRuntimeErrorStackFrame':'Numeric field copy and two native UTF-16 string copies/engagement checked; source-coordinate meaning remains open.',
'ArcExternalProtocolDialog':'Close recovery is visibly malformed and already flagged; no reset, single-shot or mutual-exclusion guarantee inferred.',
'ArcFindTextWithDirectiveResult':'Native string/result record copy supports container scope; producer text-fragment semantics remain separate.',
'ArcImportSourceProfile':'Safari initializer performs path/name conversion and native record construction; archive/import execution stays outside wrapper claims.',
'ArcLocalState':'Registration callback adaptation and unsigned token result visible; malformed error-pointer recovery remains honestly flagged.',
'ArcLoginCredentialsSaveRequest':'Engagement gate and native update byte checked; prior review of save/cancel asymmetry remains applicable to unchanged text.',
'ArcMemoryDumpMetric':'Name/units copy and unsigned scalar storage checked; shared getter meanings remain metadata-based.',
'ArcMenuItemModel':'Finding 8 corrected; nil native slot stores corroborated by instructions, wrapper shared-slot clearing expressly unproved; category query recovery remains qualified.',
'ArcMetaTagEntry':'Initializer sends copy for both strings and stores fields; producer normalization and mutable-string edge cases remain open.',
'ArcMostVisitedURL':'Long/short byte URL and UTF-16 title copying plus optional engagement checked; ranking/canonicalization equivalence not claimed.',
'ArcNomicEmbedTextModel':'Descriptor construction parallels ranking descriptor and supports storage-only scope; mutable return lifetime remains bounded.',
'ArcPerformanceController':'Guarded singleton setup and provider active byte checked; no whole-provider synchronization implied.',
'ArcReference':'Initializer sends copy for text and stores signed count; consumer character-count semantics remain unknown.',
'ArcSearchEngine':'Finding 3 corrected in lifecycle, equality/hash matrices and questions; optional checks/pointer equality/content-derived hash are separate.',
'ArcSessionID':'Finding 9 corrected to exact source line; fresh constructor confirms 32-bit narrowing; CEF ID correspondence remains unproved.',
'ArcSessionTabGroup':'Engaged shared record copying/control-count increment checked; pinned session structure remains an analogue.',
'ArcSessionWindow':'Type guard/remapping checked; full enum tables, ordering and restore behavior remain bounded unknowns.',
'ArcSpake2Context':'Role!=0 normalization and native allocation checked; pinned BoringSSL header corroborates API family, not runtime crypto behavior.',
'ArcTracingController':'Fresh nil path visibly zeroes wrapper bridge slots and nonnil captures weak references; no callback suppression inferred.',
'ArcURLUtilities':'Empty/IP-like gate, registrable-domain helper and copied output checked; helper names remain source-backed hypotheses.',
'ArcWebContentsBoostIDE':'Finding 4 corrected across all outgoing methods/questions; string payloads and Boolean response flags verified; wrapper nil clear independently sampled.',
'ArcWebContentsMediaSessionPiPController':'Nil/absent videoView branches and nested view pointer checked; returned view ownership/main-thread usage remain open.'
}
limits={
'ArcBrowserApplicationConfiguration':'Startup consumers, restoration-policy meanings, invalid argc/argv and suite-name nil semantics remain unverified; recovered C is not a callable ABI.',
'ArcBrowserContext':'Nil detachment/native registration lifetime, queued callbacks, profile shutdown and missing exact-entry delegate adapters remain unresolved.',
'ArcBrowserContextHistory':'Native observer ownership, nil detachment, DB intervals, delivery queue, synchronization and cancellation remain unproved.',
'ArcBrowserContextPreferences':'Observer registration/removal and concurrent/reentrant vector mutation are separate from assignment; policy/service loading and persistence not end-to-end verified.',
'ArcMenuItemModel':'Wrapper bridge retention/reuse, complete native cleanup and queued/in-flight callback effects remain unknown; three helper instruction words undecoded.',
'ArcSearchEngine':'Native string field names and exact upstream hash symbol, mutation stability and service-reload lifetimes remain unproved.',
'ArcSessionID':'Sentinels/restore mapping, old-engine ambiguity and numeric correspondence to CEF IDs remain unproved.',
'ArcWebContentsBoostIDE':'Delegate-slot ownership, receiving JavaScript, downstream serialization and end-to-end JS-allowed/pending-ID lifetimes remain open.'
}
transcripts={}
for p in H.glob('*bodies.txt'):
    for j,line in enumerate(p.read_text().splitlines(),1):
        m=re.match(r'INDEX (0x[0-9a-f]+)',line)
        if m:transcripts[m[1]]=f'{p.name}:{j}'
methods=rows(R/'methods.tsv');prior=rows(V/'class-review.tsv');classes=[]
for x in prior:
    name=x['class_name'];a=re.search(r'Bodies: (0x[0-9a-f]+)',x['evidence'])[1]
    assert a in transcripts and name in notes
    n=sum(y['class_name']==name for y in methods)
    extra=''
    nums=[str(i) for i,z in enumerate(c,1) if z['class_name']==name]
    if nums:extra=' finding-dispositions.tsv original data row(s) '+','.join(nums)+';'
    classes.append(dict(class_name=name,status='accepted_incomplete' if name=='ArcCommandBarRankingTrainingResult' else 'accepted_with_limits',evidence=f'{n} method identities/matrices and raw binary layout verified (coverage-check.tsv; binary-method-check.tsv); {identity} {a}, {transcripts[a]};'+extra+' '+notes[name],limits=limits.get(name,x['limits'])))
write('class-review.tsv',['class_name','status','evidence','limits'],classes)
assert len(classes)==38 and len({x['class_name'] for x in classes})==38
assert len(dispositions)==9 and [x['finding_row'] for x in dispositions]==list(range(1,10))
print('Wrote 9 finding dispositions, 38 class reviews and header-only findings.tsv.')
