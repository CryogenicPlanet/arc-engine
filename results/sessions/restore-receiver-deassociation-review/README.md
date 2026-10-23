# Queued restoration receiver versus browser deassociation

**The inspected original restoration delivery path does not test the logical Browser's native-binding fields f0/f8. Its captured weak receiver is a separately owned logical-Impl token298/2a0, not the physical Browser weak tokenf0/f8. Cleanup does not directly invalidate298/2a0; explicit invalidation is in the logical Impl destructor. The transitive helper/callout effects of cleanup are not exhaustively proved. These facts do not justify adding a facade/native-binding equality guard to the owned queued delegate notification.**

## Exact token producer and receiver

Logical construction allocates an8-byte refcount/invalid-flag token at42f7c0c..30, initializes flag+4 to0, stores it to **Impl+298 at42f7c34**, and stores **Impl itself to+2a0 at42f7c38**. Alternate constructor42f8660..8c repeats the same layout. This is distinct from physical binding initialized at f0/f8 by42f7cc0..7d00.

Insertion producer4306358 receives the logical Impl (x20=x0 at4306384).43064e8 increments its298 token refcount,4306504 loads2a0, and4306540..44 captures them into task+30/+38, with member target42fcf40 at4306534..3c. The shared Arc WebContents peer is separately retained into task+40/+48. Invoker4307290 tests captured token's invalid byte+4 and nonnull receiver, then invokes42fcf40 with that receiver at4307308..0c. It does not consult physical Browserf0/f8. Task destruction43073e0..7434 releases the WC shared count and receiver-token reference; the queued receiver reference itself does not strongly own logical Impl.

## Logical lifetime versus binding clear

The ObjC ArcBrowser facade stores native shared peer and its control atself+28/+30 (43e0c94..ca0), increments its shared count at43e0ca8..cb0, and releases it in.cxx_destruct43e3220..3c. A retained facade with its native shared handle unchanged can therefore outlive physical-browser deassociation while holding the logical Impl alive. This is not a physical Browser ownership claim. Original public setNativeBrowser or other reentrant ownership changes are separate cases.

Logical Impl destruction starts42f90b0, marks closing178 and may call cleanup42f9cf4 at42f9160. Later it clears **2a0 at42f9320**, loads298 and sets that token's **invalid byte+4 at42f9330**, then releases its token ref42f9334..50. If it enters without physical token,42f9c38..44 still routes into this invalidation. Those are positively identified logical-destructor operations, not operations inferred from a physical weak pointer becoming invalid.

Cleanup42f9cf4 directly resets subscriptions/backedges/helper ownership, then clears physical **f0 at42f9ec8** and **f8 at42f9ee0**. The complete extracted cleanup body contains no direct read/write of298/2a0, restoration state258 or selected delegatec0/c8. It does not itself write the captured logical token's invalid flag. This is a local body result, not a whole-call-graph absence theorem.

## Restoration routing and delegate call

For a retained task receiver,42fcf60..68 first requires **WC peer+130 nonnull**.42fcf78..42fd044 uses captured historical/original optionals, logical state258 and IDvector228/230 to choose restoration. The restoration route42fd4d8..42fd6c8 strongly locks logical shared peer8/10, retains the WC peer again, posts its ancillary invocation bookkeeping, weak-locks delegatec0/c8 and directly invokes delegate virtual+18. It does not load Browserf0/f8 or check closing178 on this inspected path. The trace-mode branches42fdd34..6c return to the same delegate path. The receiver's physical Browser is not queried to choose the restored callback.

After delivery, tracked-WC/explicit-ID completion uses logical restoration vectors/state. These local paths likewise contain no direct f0/f8 load. ObjC bridge43e3458 constructs value wrappers, weak-loads selected ObjC delegate and Browser references43e34b4..c8, then directly sends didRestore at43e34e4. A dead ObjC facade/delegate remains an independent delivery limit; it is not equivalent to a live retained facade whose physical browser binding is empty.

## Helpers108/180: what is and is not established

Cleanup moves out108 before calling **2f80cb0** (42f9e6c..7c), and moves out180 before **782ceb4** (42f9e98/42f9ef8).108 creation proof is42fc260..88: separately allocated0x928-byte object constructed through287b240, not an inline alias for298.2f80cb0 iterates checked observers and makes a virtual+30 call at2f80df4, then destroys further fields.180's destructor782ceb4 releases its own subscriptions/owned fields, invokes293a1b8 and tail-calls base6aef580.293a1b8 is a remove/release helper, not the token298 invalidation routine shown above;6aef580 has additional callbacks/base teardown beyond its inspected prefix.

No direct logical298 access was found in those helper prefixes, but arbitrary registered observers, virtual destructors and further callees are not exhaustively resolved. Accordingly **whether every supported cleanup invocation preserves298 until return remains UNKNOWN**. It is not proved that helper teardown invalidates298, nor proved that all helper teardown is incapable of doing so. This is the remaining exact evidence gap; do not replace it with a generic safety guard or assertion of unconditional post-deassociation delivery.

## Owned-code implication and limits

Current QueueRestorationCallback retains the WC facade and weak ObjC Browser, additionally captures a physical native Browser WeakPtr atarc_sessions_restore.mm766, and checks it at776. Its delegate-only branch777..779 does not otherwise dereference native Browser/Profile. The existing physical weak check and a proposed new facade/current-native equality check are **different predicates from original's logical receiver token**. Physical binding can clear while logical receiver survives; physical weak invalidation and logical-token invalidation need not coincide. A new equality guard would introduce an unproved suppression condition. This audit does not change the existing weak check or claim its parity; it exposes the distinction for owner review.

Conditional result: if logical receiver token remains valid, the retained WC has a nonnull current binding, restoration routing still matches, and delegate/facade references survive, the inspected original route has no additional Browserf0/f8 gate before didRestore. Reachability of that complete ordering after real browser destruction/deassociation, cleanup helper transitive effects, nested run-loop scheduling, full-finish cancellation semantics and runtime callback counts remain unmeasured. No live execution or speculative production change was performed.

Evidence and binary/input hashes are in evidence.txt and hashes.json. Prior restore-callback-liveness.md's phrase Browser weak receiver must be read as this **logical Impl token**, not proof of physical Browser liveness. Its retained-WC current-binding result remains valid.
