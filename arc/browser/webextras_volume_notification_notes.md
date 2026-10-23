# Volume notification proposal — held, not integrated

This proposal adds the missing double-valued player-volume notification. It does
not apply any shared source change or claim boundary parity. The accepted current
media-pip original records `webContents:didChangeVolume:` at call 4108 / return
4147, receiver owner `<ArcWebContents#118>`, value `0`, before PiP is requested.
The current-run1 replacement records none. The existing volume-bridge proposal
only controls the player multiplier and does not produce this notification.

Original evidence is pinned in
`logs/wp/webextras-volume-notification-audit/manifest.json` and its eight bounded
raw disassemblies. Binary SHA256:
`fe0f512702094c7c1067576bd0dd84bdc18e2ab5a5ef9953ac1edd26a50ccbce`.
Golden record SHA256:
`672f405866b7904b0ae0b996de12ef6496c0121b495450fede4a92acdb1d954f`.
Current-run1 record SHA256:
`5760c41eef96ce0bfd05477cc86477fda80f687fc20a66e3952c207472696ff1`.

## Native contract and implementation

At original `37ca62c` SetVolume saves old double `+740`, stores new input at
`37ca630`, and applies new input times multiplier `+748` to the pipeline. The
existing muted-status callback precedes `fcmp old,new` at `37ca71c`; equality
skips the new double callback, otherwise `37ca72c` calls client `4610800` with
the unmultiplied input. This is the input after HTML element mute, not a
conversion of the muted-status boolean. Multiplier-only changes call SetVolume
with the unchanged input and therefore emit no double notification. No clamp or
cross-player dedup is evidenced.

Client `4610800` fans out message `0x145074ca` with the double at parameter `+8`;
browser dispatcher `352b06c..352b094` routes it to `415f084`, whose source string
is `WebContentsImpl::MediaVolumeChanged`. That method walks outer WebContents
before forwarding to Arc userdata. Logical observer `4395bc8` / adjusted
`4395d74` forwards to `435fd88`. The latter captures the value and registered
MEDIA bridge through `435ff54`, increments MEDIA pending `+58` before posting
at `4360140`; there is no inline delivery branch. Inner `4365e70` dispatches
virtual slot `+80`. ObjC `441bef8` sends the unchanged double to
`webContents:didChangeVolume:` at `441bf3c`. Generic guard/decrement lives in
`42c0840`. The shared proposal stops at a genuine public Chromium observer
callback; the separate owned media-source proposal supplies async delivery.
MEDIA pending accounting is a separate required integration dependency, explicitly
absent from the frozen delivery helper. Both are needed before claiming the
original pending-counter contract, and the owned consumer is needed for runtime
volume delivery.

Ten production files implement this C156 route:

1. `WebMediaPlayerImpl::SetVolume` saves previous input and emits only on
   `previous != input`, immediately after existing client muted notification.
2. `MediaPlayerClient` gains default-noop `DidPlayerVolumeChange(double)`.
3. `HTMLMediaElement` implements that callback and fans out to existing remotes.
4. `MediaPlayerObserver` appends `OnVolumeChanged(double)` at its end, preserving
   every existing implicit ordinal.
5. `MediaPlayerObserverHostImpl` forwards to `WebContentsImpl`.
6. `WebContentsImpl` notifies new default-noop
   `WebContentsObserver::MediaVolumeChanged(double)` observers.

The ten-file count includes the relevant header/implementation pairs. There is
no Chrome dependency, new feature flag, host check in Blink, global value cache,
or mutation of existing mute/player-state paths. Internal changed-volume
transport is unconditional; nonhost consumers inherit the default-noop public
observer. This deliberately preserves content/Blink layering. Internal Mojo
traffic for normal nonhost volume changes is a known cost. Normal generated
bindings must be regenerated when the lead integrates the IDL change.

## Coverage and limits

Three test files add six test definitions (eight parameter-expanded cases):

- Real WMPI fixture: changed/equal nonzero inputs, muted zero, multiplier-only
  calls, existing muted-before-volume order, pipeline product, double precision.
- Real HTML fixture: actual associated Mojo observer fanout to two receivers,
  and real element volume/mute effective inputs passed to the mock player.
- Real browser fixture: real associated Mojo remotes from two players, exact
  nonzero double values and repeated messages without browser-global dedup,
  muted-status order, and WebContents audio mute producing no volume event.
  Each cross-player send is explicitly flushed; no unordered cross-pipe order
  guarantee is asserted.

All ten affected production/test/generated translation units compile with the
actual out/Base flags and a private VFS overlay. The changed IDL was parsed by
the real Chromium Mojom parser, then the real generator produced normal, Blink,
shared and message-ID bindings privately; no generated header was hand-written.
Commands, generated hashes, full diagnostics and source hashes are retained in
`logs/wp/webextras-volume-notification-validation/manifest.json`. All final
compiler logs are empty. `blink_unittests` and `content_unittests` binaries are
absent: the added Chromium test cases were **not executed**.

`webextras_volume_notification_test.py` strictly applies the patch to a private
copy of the current sources and compiles/runs actual extracted SetVolume,
SetVolumeMultiplier, HTML fanout/effective-volume, browser forwarder and
WebContents observer-notification bodies with small mock dependencies. Five
standalone groups pass, including runtime rejection of equal-input duplicate,
boolean-value substitution, and volume-before-muted mutations. This is execution
coverage of these actual bodies, not a running browser or real Mojo transport.
Real Mojo coverage here is compilation only. Strict apply and byte reconstruction
checks additionally pin the proposal to its private staged source bytes.

Alternate player emitters, including WebMediaPlayerMS, remain unproven. The new
client default does not invent their volume production. The original adjusted
client `46106bc` also sends the double message, but its concrete class has not
been established. Original outer-WebContents forwarding is proved; this initial
public observer route reports on the player's local WebContents, like nearby
C156 media notifications. Inner-WebContents owner routing remains a separate
integration question; no speculative outer fanout has been added. The proposal
adds neither a player ID to the app callback nor a tab aggregate volume.

No app, engine build, runtime lock, production source edit, or shared patch
integration was performed. A fresh reader and lead adoption are still required.
The shared proposal alone cannot repair the accepted golden callback until the
owned media observer is connected to the separately staged async source.

The existing held multiplier proposal `webextras_volume_bridge.patch` and this
proposal apply individually but currently conflict in either order at the
browser unittest's system-include hunk (both add `<vector>`). Production changes
are disjoint. `multiplier-composition.json` records both strict failures. Lead
integration must coalesce that include/rebase the test hunk; this proposal does
not overwrite or silently modify the other proposal.

## Owned incremental consumer

`webextras_volume_notification_consumer_held.patch` applies **after** the frozen
media-delivery candidate `d6ba6caf28a403a87fb8d9e8417c0f164c09bcaed9b3f158e9454f37d1c3acda`
(base staged media SHA256
`cf829322d53d3a316e8916b838ced02cec10f5fd41bda5139feb7858e619c283`).
It adds only the new public observer override and forwards the original double
through `source_.lock()->VolumeChanged(volume)`. That existing source entry
owns async bridge selection and post semantics; the override adds no scheduling,
dedup or new state. The frozen source does not implement MEDIA pending accounting;
that separate proposal must be integrated for the native counter contract. The candidate also already declares the
private ObjC `webContents:didChangeVolume:` selector. Do not apply this consumer
against the active old media source or without the new public observer API.

The combined consumer/media-header/shared-volume VFS translation unit compiles
with actual flags, exit0 and empty diagnostics. Strict application against the
pinned candidate and exact byte reconstruction pass; evidence is under
`logs/wp/webextras-volume-notification-validation/consumer/`. This is compile
and composition evidence, not runtime callback evidence. Neither candidate nor
this incremental patch has been applied to the actual production media source.

Consumer base is now the corrected delivery-v2 candidate (synchronous payload
copies). Exact old v1 patch, notes and evidence remain under
`logs/wp/webextras-volume-notification-validation/consumer-v1/`; that archive
is not an activation contract. The v2 incremental override is otherwise unchanged.
