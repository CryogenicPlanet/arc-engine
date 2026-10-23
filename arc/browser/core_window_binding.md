# Retained browser native-window binding — staged candidate

Status: PROMOTED to owned production after app run2k source freeze ended.
No runtime equivalence claim yet. Historical promotion patch SHA256
c216df2b9f0278c31e8896250926e638de75004f80b0cbd2f69957895f44ea4b.
Only arc/browser/arc_browser_window.{h,mm}; no native/shared files or new deps.

Native source proves the existing weak/replacing setter is wrong. Both public
ArcBrowser.setWindow: and outer-host attachment reach43c8b18. While its actual
window268 is nonnull it ignores all replacements, including nil. Otherwise it
retains the supplied actual NSWindow and creates a separately retaining tracker.
Object-filtered NSWindowWillClose synchronously releases the adapter's window
hold, clears the visible window, releases/clears the registration, then releases
the tracker's window. Later rebinding is permitted. Initial nil leaves the actual window unbound but still creates a tracker.

The candidate uses ObjC-only PIMPL storage with independent strong window and
tracker references, and outward gfx::NativeWindow weak storage. A selector-based
CoreWindowBindingTracker removes its notification registration before invoking
a weak-bound OnceClosure. A precise-lifetime self guard preserves the tracker
through registration release; a local shared PIMPL reference protects storage
through release-time reentry without retaining the adapter. Clear keeps the
old outward window visible through adapter-held reference release, then clears
it before dropping the tracker. Destruction invalidates the last-member weak
factory before PIMPL teardown; reverse member destruction releases tracker,
strong window, then outward weak storage. Existing feature teardown is unchanged.

Creation remains unchanged. Bounded public/factory/adapter source proves the
forWindow argument is identifier string metadata and the adapter starts with
zero actual window/tracker. No identifier-to-NSWindow lookup is introduced.

This implements only window binding/lifetime. It does not activate held host
attachment, fullscreen suppression, key-window registry or keyboard dispatch.
No dummy key/fullscreen observers are added. AppKit notifications stay on the
existing UI stack; no generic extra task is introduced.

Evidence:
- logs/wp/core.host-window-clear-audit/: exact object-filtered WillClose clear.
- logs/wp/core.browser-window-creation-audit/: creation and destructor order.
- logs/wp/core.window-binding/: exact staged files, base hashes, actual compiler
  commands and logs, manifest and fresh code review.

Checks: strict git apply --check --whitespace=error PASS; actual staged compiler
exit0 with zero diagnostics for BOTH arc_browser_window.mm and dependent
arc_browser.mm, no production mutation during checks. No test app or build was
run for these bytes. Native windows replay still needs the sanctioned AX/unlock
path; launch/tabs regression will be rerun once promoted and packaged.


Nil-input correction: the earlier excerpt ended at43c8c2c; the next instruction
43c8c30 branches BACK to43c8b5c with x19=0 and continues tracker creation.
NativeWindowTrackerObject init4735278 has no nil-window guard, so object:nil
subscriptions are global within the app. An initial nil setter therefore leaves
actual window268 zero but DOES install/replace its tracker. The corrected owned
implementation creates that tracker for nil too; no tracker-identity guard is
added. Earlier claims that initial nil creates no tracker are superseded.
Exact raw proof: logs/wp/core.window-binding/native-nil-binding.txt.
