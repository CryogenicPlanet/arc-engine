# Media retirement order correction — separate held delta

This supersedes the detach-before-PiP interpretation in frozen Media audit
ad88d9a2 and delivery-v2 notes8004e9ba. Those files, delivery patchd6ba6caf,
accounting patch11898e3d, their tests and freeze manifests remain unchanged.
`webextras_media_retirement_order_held.patch` is a separate incremental proposal
against delivery-v2 source
`cf829322d53d3a316e8916b838ced02cec10f5fd41bda5139feb7858e619c283`.
It changes no actual API/shared/Chromium source and activates nothing.

The native raw file already contained the decisive branch. For original thin
arm64 image SHAfe0f5127, function435f9c8's old-engaged path branches
435f9f0→435fb30. After the existing argument/binding/userdata guards, virtual
call435fb78 invokes PiPDestroyed. The tail reloads userdata435fb7c, clears the old
userdata peer435fb98, then branches BACK at435fb9c to435f9f4. Only then does
435f9f8 reset the receiver and435fa00/18 clear physical weak state. Reading only
the reset block's earlier address had reversed actual control flow. Raw source
SHA256 is2c95e580b2fb8c726b786901c0dcf1d032d11de1f13e07e2b50cba0bb4be7792.

The complete corrected `core_logical_web_contents_plan.md` was read, including
its qualified-discard and ownership gates. It agrees that DevTools detaches
first, then Media PiP callout occurs while Media receiver/userdata/weak binding
remain, Media clears afterward, and logical WCO detaches later. No native
post-call expected-old/generation revalidation was recovered in the helper,
DeInitialize or replacement tail. The stored raw branch and a plan snapshot are
retained with this correction's evidence.

The minimal owned correction moves PiPDestroyed before observer Detach and Source
weak-binding reset. It preserves the existing destruction latch, cache update,
selected bridge, sync/async delivery and accounting. It adds no expected-old,
generation, Retiring-phase filter, rollback, replay or ownership rule. The
existing null-observer check remains; it is not attachment-identity revalidation.

This is only the unchanged-attachment normal path. Current v2 Source exposes no
public rebind operation. Future recursive attach/retire or stale overlay mutation
cannot be declared safe from this ordering fix. Native resumes its tail without
the recovered revalidation that an adapter might want; any proposed safeguards
would be explicit adaptations requiring fresh review. This delta makes no such
adaptation or promise. In particular, it does not make the late
WebContentsDestroyed fallback a source-equivalent early retirement endpoint.

The owned test executes actual Source retirement, PiP delivery, Info observer
forwarding, cache and async dispatch with mock native receiver/WeakPtr/transport.
During the PiP app callout it verifies the mock receiver remains attached and
Source weak binding has not been reset. A nested delivered Info receipt uses
valid C156 values state1/camera2/microphone1, advances the real comparison caches
and drains its genuine queued delegate tasks. Work accepted before callback
return is still delivered after physical detach; after return the receiver is
detached and Source binding clears. The frozen v2 source fails this same control
with deliberate exit71, without crashing. This tests native-shaped receipt/data
flow, not a real Mojo/browser runtime or a synthetic production callback.

A derived full delivery regression suite imports frozen v2 test scaffolding and
actual corrected bodies. Only its now-obsolete in-call expectation changes from
detach count1 to0; its post-call expectation remains1. All other payload-copy,
DTO, bridge/nil/expiry, cache/getter, Source lifetime and all-eleven-PiP checks
pass. The actual-primitive accounting suite also passes against the composed
correction+counter+volume source. No frozen test was edited.

Strict reconstruction passes for the baseline delta and all six orders of
retirement/counter/volume consumer:38 apply/check commands, identical composed
bytes. Baseline and fully composed Media translation units pass real out/Base
flags/plugins through private VFS, with notification399b42ef and root routing
9161ec2b headers/generated Mojo in the latter. Evidence and exact hashes are in
`logs/wp/webextras-media-retirement-held/`.

Activation still requires CORE's early retirement endpoint and authoritative
public binding. Source.contents_ is not owner.webContents; observing its mock
WeakPtr does not prove public old binding survives a separate CORE clear. Root
construction/association, incoming WasDiscarded qualification, whole logical-WC
receipt locks before cache work, accepted root/sibling lifetime, ownership and
recursive cutover remain the existing shared holds. Retaining Media Source alone
is not whole native logical-WC parity. No engine build/link/app/profile/lock or
scenario run was performed, and no new boundary acceptance status is asserted.
