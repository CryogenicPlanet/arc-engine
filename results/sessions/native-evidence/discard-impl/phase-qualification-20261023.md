# Qualification: PiP receipt precedes Media reset; only qualified replacement

The frozen sessions PiP/main notes remain correct within their original bounds. Their word “detach” names the enclosing4372b6c retirement operation; it must **not** be read as saying Media's receiver/physical binding has already been reset before PiP receipt. No old file is rewritten.

Exact local order for the guarded Media route:

1.4372cd0 calls existing Media helper435f9c8. With live old observation and engagement,435f9f0 branches to435fb30; old-WC userdata lookup leads to synchronous peer vslot+8 at435fb78 and PiP route43639e8.
2.4363a5c clears the **cached PiP-controller peer** before the app callback (native dispatcher4355cc4 / ObjC441b750). This is distinct from Media's receiver and physical weak binding.
3.Only after that call returns,435fb7c reloads userdata's peer;435fb98 clears it;435fb9c branches back to435f9f4. Receiver reset at435f9f8 (receiver storage+0x70) and physical weak/control clears435fa00/+0x30 and435fa18/+0x38 follow. Root WCO detachment is later4372d74..7c atImpl+0xa8. Old logical binding+0x130/userdata clear remains later still as the existing PiP note states.

The same-Impl/count result is **not for every kReplaced**. Original4398088..94 first requires replacement old payload to match current logical+0x130, then439809c..a4 requires incoming+0x5b0 true. New bounded producer evidence in `/tmp/core-replacement-gate-identity.md/.raw` identifies that incoming marker semantically: named FinishDiscard81d8518 copies controller state81d8600..0c, writes literaltrue to the SAME incoming object's+0x5b0 at81d861c, and passes it to tab-strip replacement81d862c. This supports the receipt-time WasDiscarded mapping; it does not broaden to all replacement or later marker-reset semantics.

The frozen main note's statement that native-WC-bound helpers need recreation refers to **physical endpoints/attachments**, not reconstruction of retained logical Media/DevTools peers. The existing peer slotsImpl+0x2b8/+0x2c8 are loaded during retirement4372ca8/4372cc8 and reattachment4370624/4370638; this qualification supplies no general ownership-transfer approval.

Neither the shown Media post-call tail nor the qualified root replacement tail43980a8..d4 supplies an expected-old/generation revalidation after this app callout. Earlier counter and old-binding conclusions retain their **no earlier external/reentrant mutation** qualification. They do not prove all helpers usable, accepted-work cancellation semantics, recursive reentry safety or a recovered generation guard. No new phase, safety guard, transport/ownership approval or activation recommendation follows.

References: `/tmp/core-logical-replacement-phase.md/.raw`, `/tmp/core-replacement-gate-identity.md/.raw`, and existing frozen sessions PiP disassembly. Static reconciliation only; no app/probe/build, source/shared edits, board or credential access.

Independent read-only second review agrees, including this physical-attachment versus retained-logical-peer distinction.
