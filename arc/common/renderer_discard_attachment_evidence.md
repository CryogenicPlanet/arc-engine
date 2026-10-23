# Renderer attachments during canonical WebContents replacement

Read-only source audit, 2026-10-23. This records the current implementation and
a proposed integration contract; no production rebind implementation is present.
The later original-native SAME-Impl audit establishes count retention through discard; see the correction below and the canonical HELD v2 REVIEW. Attachment activation and TopBand-enabled handoff remain held.

## Attachment inventory

- `arc/api/arc_renderer_web_contents.mm:28-30`: `RendererBridge` is both a
  `WebContentsObserver` and `WebContentsUserData`. Its owner is one native
  WebContents, not the canonical ObjC wrapper. `For` creates it at lines 32-38.
- Its frame connections contain associated renderer remotes and host receiver
  IDs (lines 98-100); the maps/receiver set are at lines 166-167. Those endpoints
  refer to the old native frames and cannot be transferred to replacement frames.
- The profile-effects subscription invokes a bridge WeakPtr (lines 89-91,
  168-170). Destroying the old bridge removes the subscription and invalidates its
  weak callbacks. A fresh bridge subscribes to the replacement's actual profile.
- The bridge's blocked-resource counter is native-WebContents-local in our
  implementation (lines 67-79, 169). Count publication calls
  `wrapperForWebContents:web_contents()` (lines 69 and 78), rather than retaining
  a wrapper. Once the old canonical mapping is detached, leaving this bridge
  active can recreate an old wrapper through that getter.
- The `SetRendererHost` acknowledgement captures only a global frame ID
  (lines 128-134), not the bridge or wrapper. Destroying the bridge does not
  cancel that acknowledgement. Its remaining effect is process-agent readiness;
  it does not migrate or publish wrapper state.

Do not move the old helper with `Observe(new_contents)`: in addition to its old
frame endpoints, `WebContentsUserData` retains a const native-owner pointer
(`content/public/browser/web_contents_user_data.h:93-105`). Destroy the old helper
and use `For(new_contents)` to create or reuse the replacement's own helper.
Fresh attachment enumerates current replacement frames (lines 92-95), establishes
their host endpoints, and sends current profile effects (lines 116-152).

## State that must remain at its existing scope

- Frame stylesheet identifiers/Blink insertion keys and effects configuration
  live in `RenderFrameAgent` (`arc/renderer/render_frame_agent.cc:220-223`). New
  document creation clears insertion keys (lines 169-173). Do not copy old frame
  keys, remotes, direct-insertion CSS, or document state into new frames.
- Profile effects configuration and service identity belong to
  `ProfileEffectsData` (`arc/api/arc_renderer_effects.mm:29-44`); ObjC service
  wrappers hold a profile WeakPtr and service identity (lines 126-145). Profile
  configuration can be read by the new bridge; the service itself needs no
  WebContents migration.
- Profile ad-block descriptors and process-agent rules/cosmetic cache retain
  their existing profile/process lifetimes. Rebinding a wrapper is not a reason
  to clear, duplicate into another process, or recreate that process cache.
- Extraction has no persistent WebContents attachment. An issued native request
  captures the requested WebContents WeakPtr and primary frame ID
  (`arc/api/arc_renderer_extraction.mm:326-329`); completion validates those
  original identities (lines 166-175). Do not retarget an in-flight request to
  the replacement. Destruction of its original native object yields nil. Whether
  native discard cancels a request while the original object remains alive is
  unproved and should not be invented as part of attachment migration.

The original discard audit
`results/sessions/native-evidence/discard-impl/arccore-sessions-discard-impl-evidence.md`
and its hash manifest prove that the SAME Arc WebContents::Impl survives replacement
of its Chromium WebContents. Count is decimal1484 = hex0x5cc in that retained Impl;
constructor zero is not reentered, detach/attach do not reset it, and Observe(new)
does not synthesize PrimaryPageChanged. Actual later PrimaryPageChanged resets it
through the existing equality guard. Therefore the earlier fresh-helper-zero
policy was wrong: snapshot the old bridge's backend count before removing it,
and seed the new helper before frame/configuration publication. The wrapper's
ObjC count is a mirror, not an authoritative substitute for an existing backend.
The reviewed implementation is the canonical HELD v2 proposal; no activation or
runtime acceptance is implied.

## Proposed shared entrypoint and synchronous ordering

Proposed declaration for the lead; not yet implemented:

```objc
namespace arc {
void RendererWebContentsRebound(ArcWebContents* wrapper,
                               content::WebContents* old_contents,
                               content::WebContents* new_contents);
}
```

Recommended contract for a canonical old-to-new replacement:

1. Run on UI with the canonical wrapper held strongly and both native objects
   valid for the handoff. Dispose/recreate core's native-bound observers too:
   `_observer` and `_renderStateObserver` are currently initialized once at
   `arc/api/arc_web_contents.mm:669-670`. The latter additionally observes old
   widget/metadata-provider objects and resets them during destruction
   (`arc/browser/core_render_state.mm:136-156`). Preserve wrapper delegates and
   identity; do not invoke the wrapper's ordinary initializer again.
2. Core installs the replacement's canonical mapping and wrapper `_contents`,
   and detaches the old mapping. There must be **no app callback, setter/KVO
   notification, task hop, or reentrant operation** between detaching the old
   mapping and this renderer hook removing the old helper. Otherwise an old
   bridge count callback can create a second wrapper for the detached old object.
3. Call the hook synchronously before new navigation or public replacement
   notifications. Its first action removes old renderer userdata via
   `old_contents->RemoveUserData(RendererBridge::UserDataKey())`. This disposes
   old observations, frame receivers, remotes, and effects subscription. Handle
   `old_contents == new_contents` as a no-op; never transplant the helper.
4. Snapshot the old helper's backend count before step 3 removes it. Retire any
   preattached replacement helper after core has reconciled its prior canonical
   owner; create a new replacement helper seeded with the retained count before
   frame/configuration publication. No synthetic zero is published. If the old
   helper was absent, v2 defaults its untracked backend to zero; count continuity
   requires core to ensure the old helper existed throughout the counted lifetime.
   Reset only on actual PrimaryPageChanged, not the handoff itself.
5. Any wrapper count setter/KVO publication is last. Accept the wrapper argument
   directly rather than rediscovering it with `wrapperForWebContents`. A callback
   may close the wrapper/native object; retain a wrapper for the call and avoid
   further raw native/helper access afterward (or revalidate WeakPtrs).

Core must also prevent its detached old native observers from calling destruction
or state callbacks into the newly rebound wrapper. The renderer hook cannot repair
core's canonical mapping or migrate unrelated cached services. If the no-callback
window in step 2 cannot be guaranteed, detach the renderer helper before clearing
the old mapping through an explicit pre-handoff phase instead of allowing a gap.

The separate native-bound TopBand holder is outside this bridge hook. A
TopBand-enabled discard handoff remains an explicit integration gap/future variant.
Core still owns global retirement/attachment callback phases and stale-old-native
protection; this renderer-only held proposal does not activate those phases.

No runtime discard trace, build, application run, or native mutation was performed.
