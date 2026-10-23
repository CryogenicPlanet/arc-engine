# Deferred native popup keyboard forwarding

**APPLIED after validation8 completed.** The adjacent historical `extensions_popup_keyboard.patch` changes
only `arc/api/arc_extensions_browser.mm`, against frozen source SHA256
`08b4caeedfed9b9659498d950f45c71853751ae47e1ea32d19a83203597cf136`.
Validation8's source snapshot stayed unchanged; this change followed that batch.
The official browser check-file passed, with diagnostics retained in
`logs/wp/extensions.browser.popup-keyboard.check-file.log`. A fresh build and
runtime check are still pending. This implements only the popup's native unhandled-key route; it
does not implement action-shortcut registration or a core keyboard dispatcher.

## Exact native behavior

The original popup ExtensionView-shaped adapter calls the menu-forwarding
helper at `0x43b45d4 -> 0x473502c`. Full bounded instructions, byte hashes and
selector/import decoding are retained under
`logs/wp/extensions-action-command-recon/{native-disassembly.txt,native-evidence.json}`.
The original image SHA256 is
`b70bb16c1d10890c5f5913a58bdfcb8fc34f4147b05480c2b359b6fc0bd9050d`.

| Original instructions | Observable branch preserved by proposal |
| --- | --- |
| `0x4735040-0x4735078` | Read the supplied native event and require type 10, keyDown. A nil event receives nil type=0 and returns false; the proposal's explicit nil fast path is equivalent. |
| `0x47350bc-0x47350d8` | Require `(modifierFlags & 0xffff0000) != 0`. This is any device-independent modifier, not a Command-key-only rule. |
| `0x47350dc-0x47350e8` | Require NSApp. The referenced GOT slot `0xd8240e8` binds to `_NSApp`, chained import ordinal 28. |
| `0x47350ec-0x4735104` | Require a nonnil first mainMenu lookup. |
| `0x4735108-0x4735164` | Obtain mainMenu again and return its `performKeyEquivalent:` result for the same supplied event. A missing second menu or an unhandled equivalent returns false. |
| `0x473507c-0x4735090` | Rejected event type, absent modifiers/app/menu, or other unhandled path returns false. |

The actual selected SDK confirms `NSEventTypeKeyDown=10` and
`NSEventModifierFlagDeviceIndependentFlagsMask=0xffff0000UL` in
`out/Base/sdk/xcode_links/MacOSX26.0.sdk/System/Library/Frameworks/AppKit.framework/Headers/NSEvent.h:34,178`.
The patch uses those exact named constants and retains the two native menu
lookups. It forwards no alternative event and adds no app delegate fallback.

## Current Chromium event ownership

`components/input/native_web_keyboard_event.h` declares the supplied
`NativeWebKeyboardEvent::os_event`. On Mac, `gfx::NativeEvent` is
`base::apple::OwnedNSEvent` (`ui/gfx/native_ui_types.h:143`); the ObjC-only
`Get()` API returns its held NSEvent (`base/apple/owned_objc.h:49-52`,
`owned_objc.mm:39-40`).
The patch adds the direct native-event header and AppKit import, then reads
`event.os_event.Get()` synchronously inside the existing ExtensionView method.

No NSEvent is constructed, posted, copied into a synthetic input stream, or
sent to another app/window. This forwards whatever NSEvent Chromium provided;
it makes no claim that every producer's event originated from physical input.
The native path does not inspect `skip_if_unhandled`, blink modifiers, keycode,
popup ownership, or action state, so no such extra filters are introduced.

The only execution is the normal menu key-equivalent method on NSApp's actual
main menu; its BOOL becomes the handler's bool. No fake success, callback,
action invocation, keyboard registration or OS input automation is added.

## Historical verification before application

Temporary copies and exact compiler/overlay evidence:

`/var/folders/2j/z4115t_51pv7sxxrq1_j7_xh0000gn/T/arc-extensions-popup-keyboard-9he3h4bd`

- `git apply --check --whitespace=error` passed against the then-current TU.
- Applying only to a temporary copied original yields the proposed copy
  byte-for-byte.
- Proposed browser TU passes its own actual Base compiler command with
  `-fsyntax-only`: exit 0, no diagnostics. Original warning/error/plugin flags
  are retained; object and dependency output flags are stripped. The only
  added warning suppression is `-Wno-unused-command-line-argument` for the
  syntax-only invocation.
- A VFS overlay maps canonical and Chromium-symlink source paths with
  `use-external-names: false`. The negative overlay exits 1 with the unique
  `ARC_POPUP_KEYBOARD_VFS_SENTINEL`, confirming the compiler reads the overlay.
- Actual source SHA256 before/after that proposal check remained `08b4ca...`.
  Proposed-copy SHA256 is
  `84d532be49b5123068ed33ff340d741ef346a02234f3f58c43205b930f992b1e`.
  `logs/wp/extensions-action-command-recon/popup-proposal-validation.json`
  retains the concise results; the temporary directory retains exact argv,
  compiler logs and overlay files.

Fresh read-only review independently confirmed the complete native conditions,
return mapping, actual-event use, apply-check and retained VFS evidence; no
blocking issue was found. During that proposal check no product TU, helper,
compiled runtime artifact, app, profile, build graph, lock or board was changed
or used. The actual application after validation8 is recorded at the top.

Runtime popup-keyboard behavior remains **uncovered**. No current-engine
parity or golden cleanliness is claimed. The separate native action-command
map/consumer, initial RegisterCommand call and no-op delegate registration
remain unresolved implementation work described in the recon report.
