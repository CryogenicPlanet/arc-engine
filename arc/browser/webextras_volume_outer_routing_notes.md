# Held incremental volume routing correction

Apply `webextras_volume_outer_routing.patch` **after** the reviewed notification
proposal SHA256
`399b42ef1823a4603c5157c5f542ffedcc9d3b563b7073c8f50080507b372a81`.
The earlier proposal, its test and its evidence remain unchanged. This separate
increment supersedes its documented local/outer routing gap: the original root
policy is proven, rather than a proposed extra aggregation behavior.

## Exact native and C156 correspondence

Original binary SHA256
`fe0f512702094c7c1067576bd0dd84bdc18e2ab5a5ef9953ac1edd26a50ccbce`:

- `415f0b4..415f0bc` walks the receiver's pointer at `+1e0` until null, retaining
  only the final root in x8. `415f0c0..415f0e0` queries Arc userdata on that
  root only and forwards the preserved double v8 to `4395d74`. There is no
  intermediate observer call, aggregation or dedup.
- `415e890` is the direct `ldr x0,[x0,+1e0]; ret` getter. `415e89c..415e8ac`
  is an identical root traversal of this same pointer. Adjacent native vtable
  slots `dd314e0` and `dd314e8` point to these getter/root-walk bodies. This
  disambiguates the field traversal from an arbitrary logical-owner pointer.
- C156 `WebContentsImpl::GetOuterWebContents` at source10126 returns
  `node_.outer_web_contents()`. `GetOutermostWebContents` at10231 repeatedly
  invokes that getter and returns self when no outer exists. Attachment sets
  this node field to the actual outer WebContents at source1041. The native
  direct getter + adjacent root-walk and the C156 public getter/root-walk agree.

The change calls `GetOutermostWebContents()->observers_.NotifyObservers(...)`
with the exact input double. Only root observers are notified. There is no
recursion/fanout through each inner WebContents, cached root, player/tab value
cache, boolean conversion or new task posting. The owned async consumer stays
unchanged; it must observe the actual root owner for this path, as the native
root Arc userdata does. A public observer comment now states the root policy.
The source evidence concerns traditional outer/inner WebContents chains, not a
new policy for SurfaceEmbed, GuestPageHolder or unrelated responsible-content
relations. No such policy is added.

## Tests and evidence

The incremental browser unittest creates actual `TestWebContents`, real child
RenderFrameHosts and real two- then three-level ownership attachments. It checks
middle-only delivery before the middle is attached, then root-only delivery
from inner/middle/root, preserving equal repeated values and a nonzero precision
value in exact order; inner/middle observers stay empty after the root attach.
A derived fixture disables `kGuestViewMPArch` before harness SetUp and keeps the
override alive through TearDown, because the existing legacy attachment API
explicitly requires it disabled. Containers are real subframes initialized by
the existing tester API; they are not accessed after attachment. There were no
existing unit-test attachment call sites to copy, so runtime setup remains a
limitation. This new test compiles with the actual browser unittest flags but
has **not executed**; `content_unittests` is absent.

The actual WebContentsImpl and browser unittest translation units compile with
actual out/Base flags, new real generated Mojo bindings and private VFS sources:
exit0, empty diagnostics. `webextras_volume_outer_routing_test.py` strictly
applies both held patches in a temporary source copy and executes extracted
actual `GetOutermostWebContents` and corrected `MediaVolumeChanged` bodies with
mock parent/observer dependencies. Four groups pass: real two/three-level root
routing (including changed association), and runtime rejection of local-only,
local+root duplicate fanout, and bool-value mutations. These are actual-body
execution tests, not native browser/real Mojo runtime evidence.

`logs/wp/webextras-volume-outer-routing-validation/manifest.json` pins the
bounded native getter dump, resolved vtable slots, C156 excerpts, compiler
commands/diagnostics, strict sequential apply/reconstruction and standalone log.
No actual Chromium/production source or shared patch was edited, no full engine
build or app was run, and no runtime lock was taken.

The initial notification proposal's unconditional internal transport/nonhost
extra IPC is still a **lead policy decision**, not an invented Blink host flag.
This incremental root routing changes neither that transport choice nor any
existing media/mute route. Default public observers are no-op unless a consumer
opts in. Alternate player producers remain unproven. The separate owned
consumer and separate MEDIA pending accounting proposal remain integration
requirements; this patch alone makes no app-boundary acceptance claim.

The existing held multiplier proposal's browser unittest include hunk still
requires coalescing/rebase when integrating it with the notification proposal.
This incremental patch does not remove or conceal that earlier overlap.
