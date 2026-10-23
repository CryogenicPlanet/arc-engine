# HELD DevTools facade cache metadata correction

This is a DISTINCT unapplied incremental proposal after the frozen initial peer
patch `8dccf5c9c239c41a6a9edf8626be0ae3e7e082e7f656cbf5e4427b3b93c5b649`.
It does not rewrite that snapshot, its notes, test, evidence or freeze manifest.
Only owned proposal/test/notes/evidence files were published; actual production,
shared headers, Chromium, apps/profiles, build graph and board were untouched.

The initial extraction has a **P2 regression**: retaining a readonly property
without its actual facade backing cache removes the runtime
`V_toolboxWebContents` attribute. This attribute is already exercised and passing
in values-run2. It is not newly uncovered. That initial proposal must not be
adopted alone. The correction restores a real facade-owned cache and its actual
public getter, rather than a dummy ivar, duplicate peer cache or injected runtime
metadata. All initial peer integration/lifecycle HOLD gates still apply.

## Native ownership evidence

The independent retirement/cache audit is
`logs/wp/webextras-devtools-retirement-audit/`, specifically
`objc-frontend-toolbox-getters.asm`, `objc-destruct-and-bridges.asm` and its
backend close/toolbox producer excerpts. Original arm64 image SHA256 remains
`fe0f512702094c7c1067576bd0dd84bdc18e2ab5a5ef9953ac1edd26a50ccbce`.

Original service getter4415204 reads DevToolsImpl's **toolbox** handle+70 at
4415228..522c. This is distinct from the frontend handle+68 used by the public
webContents getter. If toolbox is absent,44152e0 returns nil without clearing or
releasing service cache+8. If present, it loads the cache4415234; for a cached
wrapper it calls its native aggregate accessor4415248, reloads the CURRENT
native toolbox identity441525c..5264, and compares cached native pointer x22 to
current x21 at4415280. Equality reuses the same cached ObjC wrapper. Absence or
mismatch allocates/wraps the current native handle4415288..52ac, stores the NEW
wrapper at service+8 first44152b4, then releases the OLD wrapper44152bc. Service
.cxx_destruct4415ec0..ed4 clears service+8 after native handle/bridge release.

Backend close4357fd0 calls native CloseWindow4358008, DidClose4358240 at4358064,
then clears frontend handle+68 at4358080/84. Distinct toolbox callback4358b84
clears the native toolbox handle+70 at4358c48/4c on nil input. ObjC DidClose
bridge44160b8..6108 only weak-loads delegate/service and sends the app callback;
it does not clear or release the service cache. A reentrant app getter can
separately update the cache through the getter algorithm. No eager peer
close/reset write to the ObjC cache follows from these bodies.

## Bounded implementation

The incremental patch changes only the held `.mm`:

- Places actual strong `ArcWebContents* _toolboxWebContents` first in the ObjC
  facade's ivar list, supplying native offset8.
- Restores `@synthesize toolboxWebContents = _toolboxWebContents` and the genuine
  public getter returning that cache. It restores exact public property
  attributes `T@"ArcWebContents",R,N,V_toolboxWebContents` and the ivar's ObjC
  type encoding. Declaration order cache, bridge, shared peer also preserves
  native final-release order peer, bridge, cache. This cache is used by the real public getter, not hidden state
  added only to satisfy reflection.
- Removes the peer's duplicate cache field/getter, its reset-to-nil assignment,
  and the retired toolbox lease. The retired panel-query state remains real and
  retained through reset/close reentry. Neither peer nor queued work acquires a
  strong service reference to clear a facade cache.

This deliberately preserves the PREEXISTING BOUNDED production behavior: its
cache is never populated, so the getter currently returns nil. No new C156
private toolbox accessor, backend creation hook, native wrapper constructor or
identity substitute is added. C156 `toolbox_web_contents_` is private;
`GetInTabWebContents` would return the frontend when docked, so it is not an
exact substitute for original toolbox+70. Full native getter population,
retained-cache-on-native-absence behavior, current logical-handle revalidation,
new-before-old release reentry and native fresh-versus-canonical wrapper policy
require a separate reviewed interface/implementation. This correction must not
be described as completing that native algorithm.

## Verification

The new actual held `.mm` passes the Chromium macOS arm64 real-flags
`-fsyntax-only` compiler command: exit0, zero diagnostic bytes. No build/link,
app launch, permission/activation, native UI or scenario diff was performed.

The owned test extracts the actual corrected facade/interface, synthesize,
initializer, delegate setter and public getter into a Foundation/runtime
fixture. Native peer dependencies are mocks. It verifies exact
`property_getAttributes`, `class_getInstanceVariable`, `ivar_getOffset == 8`,
`ivar_getTypeEncoding`, initial nil returns, real getter reads, native peer-then-
bridge-then-cache final release, independently
strong caches for two services retaining the SAME peer, and independent cache
release with each facade. A test-only category seeds actual cache storage to
check ARC ownership; it is not a production population path or native parity
claim. The unchanged original8dcc source is a negative control: it reports
`T@"ArcWebContents",R,N`, detects missing backing ivar/V, and exits42 as expected.
Corrected source reports the full native attributes and exits0.

The original frozen 18-group peer regression test is unchanged. A PRIVATE copy
adapts only its mock retired-state shape and includes the new actual synthesize
in its extraction; all18 actual Root/peer functional groups still pass. The
original test hash is pinned before adapting. Native/CDP/JSON/Chromium WeakPtr
and task-runner dependencies remain mocks. Source checks confirm no duplicate
peer cache, peer cache reset or retired-cache lease remains.

Strict incremental apply/reconstruction and both Core library composition
orders are recorded with complete hashes in the separate freeze manifest.
Fresh review must accept this correction before any adoption; the original
initial peer remains blocked without it.

## Requests / remaining HOLD gates

The parent must compose the original shared hooks + original two owned held
companions + initial logical-peer delta + THIS incremental correction, after
review. Core factory/facade/root ownership, native lifecycle cleanup, typed close
accounting, final silent ProtocolClient destruction, protocol HostClosed versus
explicit detach, real toolbox backend access/population and native logical
cache identity remain separate gates. No default wrapper identity exception is
authorized here. Existing devtools/media/pdf/launch-slice diffs are unchanged;
there are no new scenario diff lines for an unapplied proposal.
