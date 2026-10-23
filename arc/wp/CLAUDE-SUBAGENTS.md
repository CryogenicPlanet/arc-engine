# Brief for Claude implementation subagents (lead: Claude, 2026-10-23)

You implement unimplemented ArcCore methods for our Arc engine: `ArcCore.framework` built from Chromium 156
source so Arc 1.168's unmodified app runs on it. The target is the same behavior as Arc's own engine at the
app↔engine boundary. The Sol workers that did this before are out of quota, so you replace them for one package.

## Read first
- `arc/ARCHITECTURE.md` (binding: identity, weak delegates, KVO setters, threads, errors, layout).
- `arc/api/arc_internal.h` (shared conversions and accessors: `ArcNSString`, `ArcUTF8`, `ArcGURL`, `ArcNSURL`,
  `-[ArcWebContents webContents]`, `+[ArcWebContents wrapperForWebContents:]`, `ArcProfileFrom(context)`,
  `+[ArcBrowserContext wrapperForProfile:]`, `ArcColor`...). Look at existing files in `arc/api/` for idiom.
- Per method: `specs/methods.tsv` (columns: class_name, category, kind, selector, type_encoding, imp_va,
  body_status, behavior, evidence, confidence, unresolved_question, semantic_status, backend_callees, ...) and
  `specs/<Class>.md`. They describe what Arc's engine does and roughly which Chromium code it calls.
- Exact selectors, type encodings and properties: `arc/tools/arccore-1.170.0-objc.tsv` (rows `C` class,
  `M` method, `R` property, `I` ivar). Selectors and encodings must match exactly. Methods with C++ types in
  their encoding are engine-internal: skip them (except `init...:` pointer constructors you need yourself).
- Block signatures are not in the metadata (`@?`). Find how the app calls a method in the app decompile:
  `~/general/arc/ghidra/decomp/*.c` (grep `objc_stub::<selector_with_underscores>(`; the block's invoke
  function shows how many arguments the app's block takes), and protocol declarations in
  `~/general/arc/dump/objc-classes.h`. If you can't determine a signature, choose the most plausible one and
  say so in your report.

## Rules
- Write only the new file(s) your prompt names. Extra surface on a class that already has a primary
  `@implementation` elsewhere goes in a category in your file (`@implementation ArcWebContents (PageTools)`).
  A class with no primary implementation yet (a "shell" class) gets its primary `@implementation` in your
  file, declared `ARC_EXPORT @interface X : NSObject` with exactly the metadata's selectors.
- Never edit other files, `patches/`, the Chromium tree, `arc_internal.h` or any `.gni`. If you need a shared
  declaration or a Chromium change, put it in your report as a proposal (exact code) instead.
- Use public Chromium APIs from `chromium/src` (read the headers there; this is Chromium 156.0.8078.48).
  Chromium style: `-Werror`, ARC, no exit-time destructors (`base::NoDestructor`), `std::string_view`,
  `base::SysNSStringToUTF8` etc. Everything on the UI thread; completion blocks on the main thread.
- Wrappers must tolerate the native object being gone (return the empty value, call the completion with the
  empty result, never crash).
- Compile-check with `arc/tools/check-file.sh <your file>` (real build flags, works for new files, no lock).
  It must print no `error:`/`warning:` lines. Do NOT build, run the app, take `arc/tools/locked.sh`, commit,
  or touch git. The lead builds, integrates, runs scenarios and commits.
- Don't implement methods outside your list. Don't add features or logging beyond what Arc's engine does.

## Report (your final message)
1. Files written. 2. Each method: implemented / skipped (why), the Chromium API used, and the spec rows or
app call sites it is based on. 3. Inferred block signatures and other guesses. 4. Proposed shared or Chromium
changes, with code. 5. Which scenario or manual check would prove each behavior.
