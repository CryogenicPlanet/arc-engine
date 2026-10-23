# HELD initial DevTools userdata-before-observation correction

This DISTINCT small increment applies AFTER focus payload correctiond03834a8
(and thus initial8dcc → cachef702 → cleanupe00 → focusd038). Only the held
`Impl::Attach` ordering changes. Actual source/shared files, old proposals/freeze
artifacts, GNI/build graph, maintained patches, native recordings, apps/profiles,
locks and board were not changed. This is not lifecycle activation.

## Frozen native evidence

`webextras_devtools_lifetime_audit_notes.md` SHA
f456392c0fd789d75079556cada97608be8661480d5c1676e8f77c7f4dc1ed7a
already describes helper4358934. Its full bounded disassembly
logs/wp/webextras-devtools-lifetime-audit/devtools-rebind-full.asm SHA
05d54b103aa45f555ecd700e91b45fe92bda8a35f4caf3f4d131421954f66495
is pinned by that audit's manifest. That audit pins original arm64 image SHAfe0f5127.
No new disassembly or native app execution was needed.

The actual incoming weak flag/pointer is stored at4358a08/4358a10. Native
userdata keyc9d7108 is installed at4358a70 if absent, its peer+10 is assigned the
SAME DevToolsImpl at4358ac0, and only then4358ad8 tailcalls Observe(newWC).
The userdata peer is a tagged raw/non-owning route, not a new shared Impl/Root
owner. The frozen e00/focus held Attach instead observed before installing its
weak actual-peer userdata. This increment corrects that bounded local order.

## Implementation and ownership

The patch swaps only the two existing statements:

1. Existing `_contents = contents` remains first.
2. Existing weak actual-peer DevToolsBinding userdata is installed.
3. Existing Observe(contents.get()) runs.
4. Existing InstallHooks remains last.

The UI/live-contents/refuse-implicit-replacement checks, real weak Root lock,
matching Root native pointer, SAME DevTools peer and complete Media slot checks
are byte-unchanged. There is no new Root, facade/public wrapper, peer allocation,
retention, hook, callback, method, observer or callout. The existing local Root
lease remains exactly as before. The userdata continues to hold only the actual
peer WeakPtr; physical contents does not acquire Root/peer strong ownership.

This remains an INITIAL-only transaction. It does not implement native old-target
retirement/rebind, raw-client cleanup phase, exclusive native ownership, final
teardown, callback pending accounting or broader registration timing. Those
contracts and prior C156 safety/order holds are unchanged.

## Verification and limits

`webextras_devtools_attach_order_test.py` executes the actual composed functional
Root/peer bodies with the previous33 groups plus one registration-boundary
control. Its mocked Observe entry inspects that actual `_contents`, SAME weak
userdata route, native identity and BOTH real Root peer slots already exist.
It verifies one initial observation and that dropping external handles destroys
Root/peer rather than native userdata keeping them alive. The exact old ordering
compiles and fails `userdata missing before Observe registration`. All34 positive
groups pass. Both frozen prior tests and the focus test remain byte-unchanged;
only explicit private test adapters are generated.

The registration probe is a MOCK phase assertion. Actual current C156
WebContentsObserver::Observe29–39 and WebContentsImpl::AddObserver4753–4757 add
to the observer list without an inline app notification. This audit therefore
establishes a source-order mismatch and a meaningful publication precondition,
not an observed runtime bug or a new assertion about app reentry from Observe.
No production callout was added to manufacture such a case.

The full composed private `.mm` passes the prior real Chromium/macOS arm64 SDK,
ARC/C++23/plugin/Werror syntax command: exit0, empty diagnostics. Both prior Core
Root composition orders strictly apply this one-file increment and reconstruct
identical `.mm`/peer-header bytes. All source other than this statement swap,
including the entire facade, Root checks, focus payload, cleanup and public ABI,
is byte-identical to the precedingd038 candidate. No GN, engine build/link, app,
native runtime or scenario/golden comparison was performed.

Fresh independent code/test review is bounded-CLEAR, no P1/P2 findings:
logs/wp/webextras-devtools-attach-order/review.md SHA780a4303. The reviewer
independently ran34 groups and the compiled intended old-order negative.
Readiness is for held integration review only. Core ownership/factory/facade activation,
physical replacement/retirement, pending accounting and all existing scenario
acceptance gaps remain HOLD. Existing diffs/coverage are unchanged.
