# PiP destruction during discard detach: bounded static adjunct

Original trace156 ArcCore arm64, static-only. Raw ranges and pointer reads: `/tmp/arccore-sessions-discard-impl-pip-disassembly.txt`. No app/build/attach/production change.

The recovered native route invokes `webContentsPiPControllerDestroyed:` synchronously during the old attachment retirement, BEFORE clearing `Impl+0x130`, before removing the old WC Arc shared-Impl UserData, and before attaching replacement WC. This establishes old **native Impl binding at callback entry** on this route; it does not establish every public getter remains valid, all helpers still attached, or behavior after arbitrary delegate reentry.

Exact chain:

1. Detach4372b6c records old observation engagement from `[Impl+0xc0]` into x24 at4372c50. At4372cc0 it sets w2=(x24!=nil), then4372cc8 loads mediahelper `[Impl+0x2c8]`,x1=nil, calls435f9c8 at4372cd0.
2. In435f9c8, if its previous WC weak observation at+0x30 is still engaged and valid, w2 true selects435fb30. That uses old nativeWC to lookup UserData keyc9d6f08 at435fb50..5c. If userdata+0x10 has a native media peer,435fb68..78 invokes its vtable+8 **synchronously before returning to detach**.
3. Media peer constructor435f34c..435f360 installs primary vtableda80620. Raw slotda80628 points43639e8. That function clears its PiP-controller peer `[mediaImpl+0xa0]->+0x10` at4363a5c, captures same outer WebContents shared peer via42c8400, and calls4355b30 at4363a84 with native member pointer offset0x30/virtual flag1. Literald3817b3 is exactly `arc::WebContents::MediaSession::Delegate::PiPControllerDestroyed`.
4. Dispatcher4355b30 may submit instrumentation work first, then weak-locks the native delegate and synchronously invokes the selected member at4355cb0..4355cc4 (`ldr x21,[vtable,w21,uxtw]`;`blr x21`). It is not exclusively a queued app callback.
5. Public MediaSession setDelegate441a5b4..bc installs native delegatebridge vtableda88368. Raw slotda88398 (+0x30) points441b718. That bridge loads retained weak delegate/owner, reads selector e290f98, and directly objc_msgSend at441b750. The selector pointer resolves exactly `webContentsPiPControllerDestroyed:`. There is no additional task post inside this bridge.
6. Only later in detach,4372ed0..4372ee0 calls138f698 to remove oldWC UserData keye4a2f38 (the Arc shared-Impl map), and4373054..4373058 clears Impl+0x128/+0x130. Replacement attach stores newWC at437039c, after detach has returned through43980b0..43980c4.

Therefore the native pointer `[sameImpl+0x130]` still equals oldWC when this synchronous callback starts (assuming no earlier external/reentrant mutation). The old Arc UserData removal is later in this exact local sequence, but no complete ObjC wrapper cache/map or arbitrary getter validity audit was performed. Other native services have already begun detachment before the media helper, and the media PiP-controller peer is specifically cleared before the callback: do not model this as a completely live old service graph or treat it as replacement-attached callback receipt.

This matches the separate recorder observation of a depth2 PiP-destroyed callback inside discard, while adding native binding dataflow that recording did not itself expose. No runtime parity acceptance is claimed.
