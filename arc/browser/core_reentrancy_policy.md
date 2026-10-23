# Native production-mode reentrancy policy — UNAPPLIED / HOLD

Native proposal core_reentrancy_policy_chromium.patch SHA256
881bf2b4087ec87e2571eaee36ce3336747690d096c2c2f564d882119f4fcd07.
Shared app companion core_reentrancy_policy_app.patch SHA256
b2e27e1b1acf9bdab1ac01bdfcdf88f77ff23d91dd9d567ccf123cd4632bad23.
Both remain proposals. No actual native/shared/owned production edit or runtime.
Base is full eleven-patch stage (core11 includes BeforeUnload e57); earlier native
policy e292/f7 drafts are superseded. Exact current stage/hashes/argv/reviews:
logs/wp/core.reentrancy-policy/manifest.json. Do not add this HOLD proposal to the
ready ten/eleven-patch adoption queue without resolving the gates below.

Original422d7c0 ValidateNotReentrant keeps the transaction guard, emits diagnostic
on nonnull previous entry, then consults354e8e8. Exact public isProduction selector
-> nativeconfig58 -> mode1(YES)/2(NO) -> cached(mode!=2) proves normal production
returns even with active guard, while nonproduction traps. Cache is first-initializer
and never recomputes from later config replacement in bounded refs. Alternate
commandline mode0 also returns true; the proposal covers the public BOOL startup
route, not an invented default mode for all entrypoints. Actual accepted launch
constructor args[4]true and original getterseq22 are separately recorded evidence.

Proposal snapshots config.isProduction once before ChromeMain/profile setup.
A NoDestructor<unique_ptr<const bool>> keeps one immutable process-lifetime first
value; first false is engaged/cached, unconfigured reads false. Initial
NoDestructor<optional<bool>> was rejected by Chromium's trivial-destructor static
assert; that failed attempt is preserved under initial-compile/. A plain constinit
optional compiled, then was replaced to honor COMMON's explicit NoDestructor
rule. No mutable app delegate/wrapper/native peer is retained by this cache.

Native validator ALWAYS retains UI-thread CHECK. Only an initialized Arc-host
with actual production=true returns before fatal guard enforcement; nonhost,
nonproduction and unconfigured keep original current CHECK. Guard RAII statements
are unchanged. Nested inner scope currently clears boolfalse while outer scope
still exists; original trace guard likewise clears NULL rather than restoring
old pointer, but equivalence of Chromium156 mutation invariants is UNPROVEN.
No caller may use that bool as nesting depth or generic close-busy state.

HOLD gates:
- Original previous-entry crash key + genuine one-day DumpWithoutCrashing occurs
  BEFORE the production decision and can call the app synchronously. This patch
  intentionally supplies only policy propagation/decision, not that diagnostic.
  Current base dump function lacks Location arguments; current app block is stored
  but unwired. The app package owns diagnostic callback integration. Do not invent
  callback counts, padding, an empty-key clear notification or silent diagnostic
  suppression to make this appear complete.
- Chromium156 nested tab mutation invariants need review and a real scoped
  original/ours transaction test. Config-derived source proof is not evidence
  that arbitrary current nested mutations are safe or that webextras140398 is fixed.
- Lead owns native/shared adoption. No direct main-source edits or default
  production guess is authorized. Preserve source freezes for other workers.
- If sessions source+policy alternative is selected, bootstrap must combine one
  sessionRestorationPolicy snapshot AFTER isProduction/before loadProfiles and
  ArcSessionsConfigureRestorationPolicy before ChromeMain, with its declaration,
  configure implementation and native hook landed together. This standalone
  production companion adds no undefined configure call. Source-only sessions
  alternative needs no policy call. Board140953/140993.

Strict both-patch apply/exact3file reconstruction PASS. Actual native TSM and
shared application TUs both compiler0/empty for final881/b2 bytes. Current native
and shared production sources unchanged; final fresh review no concrete P1/P2
within the held policy scope. No linked build/runtime/whole-policy parity claim.

Diagnostic scope is now fully mapped: logs/wp/core.reentrancy-diagnostic/.
Original83774b4 is real DumpWithoutCrashing(Location,TimeDelta), per-location
one-day throttle. Its installed callback receives Location*, sync maps file/line
into the original ObjC block before mode evaluation. A fresh eligible UI case can
emit previous_entry, file, line crash keys then the dump block; actual callback
counts depend on last-value/2s coalescing state, never synthesize fixed counts.
Native Clear only zeros storedsize without observer emission. Original nested
entry deletes/replaces the old guard trace; inner cleanup clears NULL, no restore.
Current NoDestructor cache proposal still does not implement those diagnostics.
App owns its existing D/callback transport and reporter pending/throttle semantics.
