# ArcImageRep

ArcCore 1.170.0 arm64. 17 declared rows; superclass `NSObject`. Class metadata `0xe37c488`, class_ro `0xe2a7250`. Addresses are unslid engine VAs. This is a bounded research contract, not proven runtime equivalence.

Native image value with raw pixel, codec and asynchronous decode adapters.

## State and layout

Metadata instance start 8, size 32 bytes. This is object layout, not code size. Exact full ivar encodings and property records are in `../evidence/layout.json`.

| Ivar | Offset | Bytes | Encoding | Metadata record |
|---|---:|---:|---|---|
| `_imageRep` | 0x8 | 24 | `{ImageRep="_vptr$ImageRep"^^?"impl_"{shared_ptr<arc::ImageRepImpl>="__ptr_"^{ImageRepImpl}"__cntrl_"^{__shared_weak_count}}}` | `0xe2a71e8` |

Declared properties (metadata facts; `&` strong, `W` weak, `C` copy, `R` readonly, `N` nonatomic):

- `imageRep`: `T{ImageRep=^^?{shared_ptr<arc::ImageRepImpl>=^{ImageRepImpl}^{__shared_weak_count}}},N,V_imageRep` at `0xe2a7210`.
- `isNull`: `TB,R,N` at `0xe2a7220`.
- `pixelWidth`: `Tq,R,N` at `0xe2a7230`.
- `pixelHeight`: `Tq,R,N` at `0xe2a7240`.

## Lifecycle, ownership, callbacks and threading

init 0x440539c constructs native empty image; 0x440549c copies supplied ImageRep through 0x43380d4. Pixel constructor 0x44054f4 forwards width/height/scale/rowBytes and NSData.bytes to 0x43378d8. dangerouslyDecodeFromData 0x4405700 returns nil on empty native optional. Async decode 0x4405930 captures block and calls 0x4337e00: null/empty data or scale<=0 invokes an empty result inline; valid input is copied and posted to an unresolved task runner, default decode budget 0x8000000 bytes when argument is zero. Callback 0x4405db4 passes nil or a new ArcImageRep.

0x4337e00 proves mixed inline-failure and task-posting behavior. It does not establish the reply sequence or a main-thread guarantee. See evidence/bridges-services.txt and callback trampoline 0x4405db4 in evidence/bridges-callbacks.txt.

## Relations and engine route

copyPixelDataBGRA8888 0x4405ad8 allocates width*height*4 mutable data, returning nil if copy helper fails. Codec calls 0x4405ba8→0x43385c0 and 0x8371eb8→0x8b0218c return NSData; toNSImage 0x4405b64 uses native image conversion. These are source/codec candidates, not exact Chromium symbol matches. CEF image APIs would require parity tests for dimensions, alpha, color, scale and failures.

## Method matrix

Each evidence entry names the exact ArcCore IMP, binary method-record VA, raw dyld line and local indexed-body extract. `local_verified` means the stated local operation is supported, not that all downstream semantics or ABI behavior are proven. `reviewed_partial` retains a consequential open question. `unresolved` makes no semantic claim. Shared IMP names in the corpus may belong to another class. Category rows are independent identities.

| Category | Kind / selector | Type encoding | IMP VA | Behavior | Backend / callees | Evidence | Confidence / status | Unresolved question |
|---|---|---|---|---|---|---|---|---|
| (base) | -init | `@16@0:8` | 0x440539c | Calls super init and constructs an empty native ImageRep through 0x4337780/0x43381c0. | 0x4337780,0x43381c0,0x5810 | [0x440539c](../bodies/0x440539c.c); record 0xd7793d8; dyld L1811 | medium / reviewed_partial / available | Verify null storage safety, codec error/enum handling and aggregate ABI. |
| (base) | -initWithArcImageRep: | `@24@0:8r^v16` | 0x440549c | Calls super init then copies native ImageRep using 0x43380d4. | 0x43380d4 | [0x440549c](../bodies/0x440549c.c); record 0xd7793e4; dyld L1812 | medium / reviewed_partial / available | Verify null storage safety, codec error/enum handling and aggregate ABI. |
| (base) | -isNull | `B16@0:8` | 0x4405a94 | Checks native image storage/null state; exact empty-state representation requires backend validation. | No direct anonymous call recorded / indirect possible | [0x4405a94](../bodies/0x4405a94.c); record 0xd7793f0; dyld L1813 | medium / reviewed_partial / available | Verify null storage safety, codec error/enum handling and aggregate ABI. |
| (base) | -pixelWidth | `q16@0:8` | 0x4405ac0 | Returns native image width integer from shared storage. | No direct anonymous call recorded / indirect possible | [0x4405ac0](../bodies/0x4405ac0.c); record 0xd7793fc; dyld L1814 | medium / reviewed_partial / available | Verify null storage safety, codec error/enum handling and aggregate ABI. |
| (base) | -pixelHeight | `q16@0:8` | 0x4405acc | Returns native image height integer from shared storage. | No direct anonymous call recorded / indirect possible | [0x4405acc](../bodies/0x4405acc.c); record 0xd779408; dyld L1815 | medium / reviewed_partial / available | Verify null storage safety, codec error/enum handling and aggregate ABI. |
| (base) | -copyPixelDataBGRA8888 | `@16@0:8` | 0x4405ad8 | Allocates width*height*4 mutable data; calls 0x4338370 to copy pixels; returns nil if data allocation/copy fails. | 0x4338370 ; ObjC selectors dataWithLength: | [0x4405ad8](../bodies/0x4405ad8.c); record 0xd779414; dyld L1816 | medium / reviewed_partial / available | Verify null storage safety, codec error/enum handling and aggregate ABI. |
| (base) | -toNSImage | `@16@0:8` | 0x4405b64 | Converts native image through 0x22f4af4 and 0x85eac30. | 0x22f4af4,0x85eac30 | [0x4405b64](../bodies/0x4405b64.c); record 0xd779420; dyld L1817 | medium / reviewed_partial / available | Verify null storage safety, codec error/enum handling and aggregate ABI. |
| (base) | -encodeToPNGWithMethod: | `@24@0:8Q16` | 0x4405ba8 | Forwards method enum to 0x43385c0; packages resulting byte buffer as NSData and frees temporary bytes. | 0x43385c0,0x5810 ; ObjC selectors dataWithBytes:length: | [0x4405ba8](../bodies/0x4405ba8.c); record 0xd77942c; dyld L1818 | medium / reviewed_partial / available | Verify null storage safety, codec error/enum handling and aggregate ABI. |
| (base) | -encodeToJPEGWithQuality: | `@20@0:8f16` | 0x8371eb8 | Forwards native image/quality through 0x22f4af4 and 0x8b0218c; packages bytes as NSData. | 0x22f4af4,0x8b0218c,0x5810 ; ObjC selectors dataWithBytes:length: | [0x8371eb8](../bodies/0x8371eb8.c); record 0xd779438; dyld L1819 | medium / reviewed_partial / available | Verify null storage safety, codec error/enum handling and aggregate ABI. |
| (base) | -imageRep | `{ImageRep=^^?{shared_ptr<arc::ImageRepImpl>=^{ImageRepImpl}^{__shared_weak_count}}}16@0:8` | 0x4405c98 | Returns native ImageRep aggregate with shared-count handling; return ABI needs validation. | No direct anonymous call recorded / indirect possible | [0x4405c98](../bodies/0x4405c98.c); record 0xd779444; dyld L1820 | medium / reviewed_partial / available | Verify null storage safety, codec error/enum handling and aggregate ABI. |
| (base) | -setImageRep: | `v40@0:8{ImageRep=^^?{shared_ptr<arc::ImageRepImpl>=^{ImageRepImpl}^{__shared_weak_count}}}16` | 0x440505c | Assigns native ImageRep through 0x43380d4. | 0x43380d4 | [0x440505c](../bodies/0x440505c.c); record 0xd779450; dyld L1821 | medium / reviewed_partial / available | Verify null storage safety, codec error/enum handling and aggregate ABI. |
| (base) | -.cxx_destruct | `v16@0:8` | 0x4405cc4 | Destroys backing storage with shared-count decrement and last-owner virtual destruction. ARC injection may hide retain/release operations. | 0x5810 | [0x4405cc4](../bodies/0x4405cc4.c); record 0xd77945c; dyld L1822 | medium / local_verified / available | Indirect destructor targets and thread of destruction are not exhaustively recovered. |
| (base) | -.cxx_construct | `@16@0:8` | 0x4405d8c | Initializes C++ backing storage; zero/empty assignments and any constructor helpers are preserved in the exact body. | 0x4337780 | [0x4405d8c](../bodies/0x4405d8c.c); record 0xd779468; dyld L1823 | medium / local_verified / available | Full C++ aggregate construction ABI remains to be validated. |
| (base) | +createFromPixelDataBGRA8888:height:scale:rowBytes:pixelData: | `@56@0:8q16q24d32Q40@48` | 0x44054f4 | Passes dimensions/scale/rowBytes/NSData.bytes to 0x43378d8; wraps returned native value in ArcImageRep. | 0x43378d8,0x5810 ; ObjC selectors initWithArcImageRep: | [0x44054f4](../bodies/0x44054f4.c); record 0xd7793a0; dyld L1824 | medium / reviewed_partial / available | Does native helper validate NSData.length against rowBytes*height? |
| (base) | +dangerouslyDecodeFromData:scale: | `@28@0:8@16f24` | 0x4405700 | Passes NSData bytes/length and scale to 0x4337b04; returns nil if native optional absent, otherwise wraps native ImageRep. | 0x4337b04,0x4b08a00,0x5810 ; ObjC selectors initWithArcImageRep: | [0x4405700](../bodies/0x4405700.c); record 0xd7793ac; dyld L1825 | medium / reviewed_partial / available | Recover codecs, byte/size limits and synchronous decoder isolation. |
| (base) | +decodeFromData:scale:desiredWidth:desiredHeight:maxDecodedBytes:completion: | `v60@0:8@16f24q28q36Q44@?52` | 0x4405930 | Captures completion; 0x4337e00 fails inline on null/empty bytes or nonpositive scale, otherwise copies bytes and posts decode task; zero maxDecodedBytes defaults to 128 MiB. Callback 0x4405db4 delivers nil or new wrapper. | 0x8790,0x4337e00 | [0x4405930](../bodies/0x4405930.c); record 0xd7793b8; dyld L1826; evidence/bridges-services.txt | medium / reviewed_partial / available | Resolve reply sequence, cancellation, dimension bounds and decode error details. |
| (base) | +decodeFromData:scale:completion: | `v36@0:8@16f24@?28` | 0x4404fd0 | Forwards to full decode overload with desiredWidth=0, desiredHeight=0, maxDecodedBytes=0. |  ; ObjC selectors decodeFromData:scale:desiredWidth:desiredHeight:maxDecodedBytes:completion: | [0x4404fd0](../bodies/0x4404fd0.c); record 0xd7793c4; dyld L1827 | medium / local_verified / available | Full overload defines async/failure delivery semantics. |

## Remaining implementation questions

Does valid async decode reply on caller sequence, how are dimensions clamped, and what are codec enums/quality limits and input-byte bounds?
