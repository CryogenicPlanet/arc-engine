# ArcBookmarkImportTargets

ArcCore 1.170.0 arm64. 6 declared rows; superclass `NSObject`. Class metadata `0xe37bf88`, class_ro `0xe2a56e0`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Two bookmark-node destinations, one for bookmark-bar entries and one for other bookmarks.

## State and layout

Metadata instance start 8, size 24 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_targetNodeForBookmarkBarBookmarks` | 0x8 | 8 | `@"ArcBookmarkTreeNode"` | `0xe2a5678` |
| `_targetNodeForOtherBookmarks` | 0x10 | 8 | `@"ArcBookmarkTreeNode"` | `0xe2a5698` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `targetNodeForBookmarkBarBookmarks`: `T@"ArcBookmarkTreeNode",&,N,V_targetNodeForBookmarkBarBookmarks` at `0xe2a56c0`.
- `targetNodeForOtherBookmarks`: `T@"ArcBookmarkTreeNode",&,N,V_targetNodeForOtherBookmarks` at `0xe2a56d0`.

## Lifecycle, ownership, callbacks and threading

0x43ed544 stores the two supplied objects. Metadata property records 0xe2a56c0 / 0xe2a56d0 declare strong nonatomic ownership; 0x43dd5a4 clears both fields. Retain/store operations have been injected away in pseudocode. No import is started here.

No Arc thread-affinity assertion is established in these examined wrappers. Callback delivery and calling sequence require a separate task-runner/caller audit.

## Relations and engine route

An import adapter must preserve destination identity; node creation and Chrome bookmark-model ownership lie outside these six methods. The class does not establish a persistence transaction or a CEF bookmark capability.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -initWithBookmarkBarBookmarksTargetNode:withOtherBookmarksTargetNode: | `@32@0:8@16@24` | 0x43ed544 | Calls super init and stores the two destination objects at self+8 and self+0x10. | No direct anonymous call recorded / indirect possible | [0x43ed544](../bodies/0x43ed544.c); record 0xd778454; dyld L1539 | medium / local_verified / available | Validate nil handling in import consumer. |
| (base) | -targetNodeForBookmarkBarBookmarks | `@16@0:8` | 0x2d0fb40 | Returns 64-bit field _targetNodeForBookmarkBarBookmarks at self+0x8; no backend call. | No direct anonymous call recorded / indirect possible | [0x2d0fb40](../bodies/0x2d0fb40.c); record 0xd778460; dyld L1540 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -setTargetNodeForBookmarkBarBookmarks: | `v24@0:8@16` | 0x43eae2c | Stores the supplied value in the declared backing field; see layout/property attributes for ownership declaration. | No direct anonymous call recorded / indirect possible | [0x43eae2c](../bodies/0x43eae2c.c); record 0xd77846c; dyld L1541 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -targetNodeForOtherBookmarks | `@16@0:8` | 0x2cb140 | Returns 64-bit field _targetNodeForOtherBookmarks at self+0x10; no backend call. | No direct anonymous call recorded / indirect possible | [0x2cb140](../bodies/0x2cb140.c); record 0xd778478; dyld L1542 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -setTargetNodeForOtherBookmarks: | `v24@0:8@16` | 0x420b588 | Stores the supplied value in the declared backing field; see layout/property attributes for ownership declaration. | No direct anonymous call recorded / indirect possible | [0x420b588](../bodies/0x420b588.c); record 0xd778484; dyld L1543 | high / local_verified / available | No additional local question; runtime parity untested. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x43dd5a4 | Destroys backing storage; clears object/backing fields. ARC injection may hide retain/release operations. | No direct anonymous call recorded / indirect possible | [0x43dd5a4](../bodies/0x43dd5a4.c); record 0xd778490; dyld L1544 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |

## Remaining implementation questions

Validate aggregate ABI, nil/error cases and producer/caller lifetime before treating this local wrapper contract as complete.
