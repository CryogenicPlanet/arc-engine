# Public media getter audit

2026-10-23 UTC, bounded original-native and current-source audit only. No
implementation, shared file, existing frozen delivery evidence, probe, app, build,
or lock was changed. This note is a separate artifact from the delivery audit.

Original thin arm64 image `/tmp/arccore-webextras.arm64` was reverified SHA256
`fe0f512702094c7c1067576bd0dd84bdc18e2ab5a5ef9953ac1edd26a50ccbce`.
All native addresses below are unslid VAs. Raw bounded ranges, decoded property
metadata, selected source snapshots and hashes are recorded in
`logs/wp/webextras-media-getter-audit/manifest.json`.

The previous delivery note SHA256
`ad88d9a22031f421bb655979adf94a1a13655b97962111f6db1dbae53bd02ba9`
and manifest SHA256
`de019558a5b9bfbc5ac69124d88b57bbe763ff1990dfcc214e335871bb2c0f07`
were checked unchanged; this audit does not append to or rewrite them.

## position reads the native observer receipt cache

Public IMP441b074 first checks service native-handle engagement at service+40.
It loads the retained MediaImpl at441b098 from service+30 and tests that Impl's
cached-position engagement byte+c8 at441b09c..a0. Absence returns nil. Presence
copies the shared native position pair at Impl+b8/+c0 into a temporary at
441b0a4..ac and increments its strong control count441b0b4..bc. It allocates a NEW
ArcMediaPosition441b0c8..d0 and calls initWithMediaPosition:441b0d4..e0 with the
temporary shared native handle. It then releases the temporary shared count and
returns the newly allocated object via the autorelease-return path441b108..11c.
There is no call to a live content::MediaSession getter and no physical-WC or
weak ObjC-owner check in this route.

The actual getter does not store its result into ObjC _position. Its new result
is saved only in local x19 at441b0e4, then returned. The metadata property record
e2a9930 nevertheless says `T@"ArcMediaPosition",&,N,V_position`. The separate
synthesized setter420b588 moves its argument and stores strongly at self+10 through
8c7df80. Preserve that existing metadata/setter surface; it is not evidence that
the custom getter must return or overwrite the synthesized ivar. The owned getter
currently overwrites _position at media.mm191, unlike this original IMP.

ArcMediaPosition initializer443c1dc copies/retains the supplied native shared
position pair into its own optional storage443c23c..260. Thus each returned DTO
holds the selected native snapshot independently of later source-cache changes
or service/physical contents destruction. Its currentTime accessor443c36c calls
32b5dd0 on that held native value; copying a snapshot does not freeze wall-clock
progress. This audit does not redefine its clock, clamp, or endOfMedia semantics.

## Cache production and asynchronous callback timing

Constructor435f4c4 starts cached position engagement+c8 unset. Native Mojo receipt
4362c0c checks optional presence4362c70..74. A present value allocates a shared
native snapshot4362c78..ca0 and copies ALL40 bytes of the incoming MediaPosition
at4362ca4..cb0, including its timing base. It installs the shared pair at
Impl+b8/+c0 around4362ed0 and engages+c8 at4362ee0 (the alternate present branch
4362ea4 also sets engagement). This occurs before the selected delegate's async
post43630d8. The queued callback captures its own selected snapshot.

Absent receipt clears the cached optional at4362d20..d40 and does not publish a
position delegate callback. No app delegate registration is required for the
cache to advance: receipt-cache mutation precedes selected weak-bridge checking
4362ef4 onward. Physical attachment helper435f9c8 resets observation/binding but
does not itself reset+b8/+c0/+c8. A subsequent genuine receipt on a new attachment
can of course replace/clear the cache; this is not permission to retain stale
values against such events.

Consequences of this source ordering:

- Before a queued position delegate delivery, public position already reflects
  the newest processed receipt, which need not be that pending callback's payload.
- Before a new Mojo receipt arrives, public position continues to expose its last
  receipt cache even if the underlying content media session has already changed.
- A retained service can query that cache after physical detachment or ObjC-owner
  expiry, while its own native MediaImpl handle remains engaged.
- Every getter creates a fresh ObjC DTO from the selected cached native value;
  older returned DTOs retain their own snapshot.

Current owned media.mm188..192 instead finds a live session through weak owner,
calls GetMediaSessionPosition and constructs/stores a DTO. Current Chromium
MediaSessionImpl::GetMediaSessionPosition851..852 returns its live position_.
That is a real source gap in receipt timing, native-detach lifetime, and the
ObjC ivar write. It is independent of the separate native-clock probe results.

Candidate change boundary: read the shared logical MediaSource's optional native
RECEIPT cache and allocate a fresh ArcMediaPosition on each getter call, without
writing _position. Preserve its synthesized setter and metadata. Do not use the
last delivered delegate DTO, a live session query, synchronous seeded receipt,
hardcoded position, or an owner/physical-WC liveness filter. Correct eager source
creation and observation remain prerequisites from the delivery audit; merely
adding a cache to the lazy ObjC service does not recreate native lifetime.

## getMediaContentTypes remains a live synchronous native query

ObjC IMP441a848 allocates its mutable index-set builder, checks engaged native
handle, and calls43608d8 at441a88c. Native43608fc..0910 validates MediaImpl's
physical-WC weak binding+30/+38;4360928..0940 looks up the EXISTING content media
session userdata. Missing binding and missing session take separate error routes
4360a14 and4360a84, later converted through4402548 by the ObjC wrapper. This is
not an observer info-cache read or a native-session creation request.

For a live existing session,4360950 calls2dda64c. That function obtains the current
routed frame when present and constructs a result vector, then enumerates players
through2dda968. The latter walks its current normal-player map and one-shot-player
vector. Its bound function2ddaea8 filters by the routed frame, queries current
audio/video flags, and emits2 for audio+video,1 for audio only,3 for video only.
The Arc native helper excludes zero436099c, requires values<4 at43609a0..a4, and
returns copied vector storage. The ObjC wrapper adds valid indices and returns
a fresh copied immutable index set441a910..920, or nil after error conversion.
This path is synchronous and independent of pending regular media callbacks.

Current owned media.mm175..186 calls GetMediaSessionInfoSync and extracts
audio_video_states. Current Chromium GetMediaSessionInfoSync1168 invokes
GetMediaAudioVideoStates2189..2214, which likewise reads current players/routed
frame and emits1/2/3; ForAllPlayers2217..2223 covers normal and one-shot players.
Therefore keep this getter live when restoring regular async delivery. Do not
replace it with cached MediaSessionInfo from the new source. Current filtering
allows zero, but this current Chromium enumerator never emits it; no observed
runtime mismatch follows from that latent difference in this audit.

The owned query currently resolves session availability through weak ObjC owner;
the original resolves it through retained MediaImpl's native physical binding.
That existing ownership distinction belongs to the broader logical-source/rebind
integration, not a reason to cache content types. This audit confirms query
provenance and missing-binding/session ordering, not complete NSError numeric
parity or all side effects of the broader GetMediaSessionInfoSync call.

## Scope and verification limits

Raw native field loads, allocation/refcount instructions, property metadata,
receipt update ordering, live enumeration and immutable result-copy paths were
inspected. Selected current-source snapshots support only the stated C156
comparison. No candidate implementation, compiler, runtime, acceptance result,
or unobserved count/value is asserted. Do not transfer this evidence to PiP
controller caching, callback-domain accounting, or other service getters.
