# Native close transport proposal — UNAPPLIED

Patch `core_close_transport_chromium.patch` SHA256
`41de166d504e1a4e671af035398ad8940adc23be8bad313af9813935d72d42e0`.
Apply AFTER native `core_navigation_chromium.patch`2f423c8. Chrome files are
based on the current native checkout. This is transport only: both callbacks are
EMPTY by default, host-gated, and no core consumer/public Close policy is enabled.
Do not apply the old held ClosePage/reentrant-close patch as a replacement.

The physical destruction hook in `content/browser/web_contents/web_contents_impl.cc`
runs AFTER ALL ordinary WebContentsDestroyed observers, guarded by the EXISTING
captured outermost WC, BEFORE observer reset/delegate clearing/physical weak
invalidation. It copies the callback before invoking it. Original36926b0→3692ff0
has this phase; logical Closed handler438a18c is a custom userdata client at
Impl+70, not the content WCO+A8 (whose destroy callback is no-op). FrameTrees are
already shut down. Consumers must use existing logical ownership, never construct
another wrapper, retain a dying WC, or infer arbitrary API safety from WeakPtr.

The Chrome delegate hook intercepts CloseContents BEFORE CanCloseContents mutates
unload state. Its OnceClosure retains only exact weak delegate/source identity,
resumes the original body without reentering the hook, and can run inline.
Empty/nonhost paths preserve that body. Delayed consumers still need explicit
logical association/source-token/current-membership validation; weak liveness
alone does not establish that an old browser can close a transferred tab.

No five-domain gate is added to native CloseContents: original4383378 has none.
Public437854c has that gate. No bulk-close, BeforeUnloadFired, timing0 source-token,
logical cleanup/transfer, or public timing1 activation is implemented here.
Received transfer reason1 is a separate logical Closed producer; Arc-controlled
transfer suppresses its own listener. Original cleanup removes userdata callbacks.
The future consumer must replace the existing early WCO Closed sender, not wire
both and double-send.

A distinct original Closed suppression predicate is now resolved exactly as
browser_shutdown::IsTryingToQuit (audit logs/wp/core.close-suppression-flag/).
The webextras native-run7 ours FATAL proves app didClose can insert a tab during
CloseAllTabs ReentrancyCheck. Moving to post-WCO alone still runs inside that bulk
transaction and is NOT a fix. Preserve quit suppression and the actual app quit
flag transaction; no global reentrancy bypass or invented bulk Busy gate.

Validation: strict incremental apply + exact five-file reconstruction PASS,
actual WebContentsImpl and BrowserWebContentsDelegate TU compilers both exit0
with empty diagnostics; native production bases unchanged. Independent fresh
source review found no concrete P1/P2 under the empty-default transport contract.
Exact manifests, argv, logs and review: logs/wp/core.close-transport/. No link,
packaging, runtime, cross-platform or consumer behavioral equivalence is claimed.
