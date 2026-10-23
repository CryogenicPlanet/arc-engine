# Core typed close-accounting adapters — UNAPPLIED composition

This is concrete staged code, not an activated synchronous-close repair.
Apply AFTER the existing four-part native/WC/display/observer bundle and
core_close_accounting_primitive.patch. Incremental adapter patch SHA256
bec65249a39939a247b3fe48a46d1b58fce040eab632fba4f49f507d0e2729b5.
Rebased against production quit suppression ff957456; prior c63b artifact is
historical. Closed UI scope is INSIDE !trying_to_quit, so suppressed delivery
acquires no invocation. Current strict reconstruction/current+full7 WC compilers
exit0/empty + fresh review: logs/wp/core.quit-suppression/. Other producer logic
and private API are unchanged.
No native/shared production source changed. Exact reconstruction, real compiler
commands/logs, staged files and fresh review: logs/wp/core.accounting-adapters/.

One state is created immediately after successful ArcWebContents super init,
before initializer setters; direct private access never creates a wrapper or
substitutes a state. Contents/display observers capture that exact handle at
construction. Retained navigation controller captures its logical state ONCE at
init; public weak owner changes/expiration do not migrate it. Existing observer
registrations retain their own receipt-time weak owner contexts independently.

Explicit typed producers preserve notification grouping: URL+ID together,
loading+back+forward together, metadata viewport+root together; title, favicon,
traits, progress, visibility, theme, focus, history and readiness are separate
WC batches. Each posts one distinct same-runner trailer after its delivery.
Navigation notify creates ONE batch before registration enumeration and one
trailer after all posts, even with no observers. No per-recipient RAII decrement.

LOAD status/security use UI pending acquisition at enqueue and decrement after
conditional weak delivery returns. Direct target URL uses UI synchronous scope;
policy uses Navigation synchronous scope; imminent/didClose use UI synchronous
scope. Focus's first weak-native hop captures exact state WITHOUT counting;
its second-hop actual notification acquires the batch. No automatic retry/Close,
wrapper/delegate/native retention, delivery-time state lookup, rejected-post
rollback or destructor decrement is added. Existing default close timing1 and
all production integrations/tails remain unchanged.

Initial direct setters are now source-proven uncounted; do not synthesize a
scope around them. Original subsequent zoom uses a genuine queued WCObserver notification+trailer.
The separate compiled/reviewed UNAPPLIED ZoomObserver companion37637ae supplies
that event source AFTER this adapter; this adapter stage alone still retains the
old per-commit refresh, so apply the full documented composition.

Remaining activation gates: lead-approved shared declaration/state access for
UI/media packages; their exact producer coverage on SAME logical state; real zoom
producer; exact public/single-native forwarding/conditional imminent and the
source-proven Closed/query phase, plus lifetime/validation. Timing0 continuation,
unattached ownership and original bulk behavior remain separate bounded scopes;
no native/bulk Busy gate is inferred. Core's new private
header is only a same-package adapter until the lead approves cross-package use.
No public Close query or synchronous fast path is activated by this composition.

Validation: four actual real-flag compiler exits0/no diagnostics (WC, navigation,
render and primitive), strict sequential apply0 plus exact reconstructed hashes,
fresh current-byte code review clean. No build/link or runtime check of this
composition has been performed. Do not transfer window/run6 runtime claims.
