# Normal restoreForeignTab static audit

**No concrete normal-route metadata mismatch found** in current `arc/api/arc_sessions_restore.mm:1096–1115` against original backend42fb100 and C156 AddRestoredTab. This is static argument/dataflow evidence for a supported populated ArcSessionTab and initialized target browser. It is not foreign-session/profile/group runtime acceptance.

## Arguments

| Item | Original address/dataflow | Current mapping |
|---|---|---|
| History |42fb190 loads the navigation range pair from tab+0xa8/+0xb0;42fb264/268 passes it to421e280|native->navigations|
| Insert position |42fb1b0 reads active-tab member; absent →w25=0 at42fb1d0; otherwise403d24 finds its flattened model index, and42fb1c4..c8 selects found index+1 or0|active_index()+1; empty/no active index-1 →0|
| Selected navigation |42fb1e0 reads current index at tab+0xc;42fb210..21c computes max(0,min(index,history count-1));42fb270 passesw26|normalized_navigation_index(), C156 session_types.h:48–51|
| Extension app ID |42fb24c x5=tab+0x48|extension_app_id|
| Group |42fb1e4..1f0 copies the optional group beginning tab+0x10;42fb250 x6 points to that copy|native->group|
| Select |42fb274 w7=0|false|
| Pin |42fb1fc reads byte tab+0x40;42fb258 stores it to caller stack+0|native->pinned|
| Monotonic last active |42fb254 storeszero to stack+8|base::TimeTicks()|
| Wall-clock last active |42fb200 loads tab+0xa0;42fb248 stores to stack+0x10|native->last_active_time|
| Session storage |42fb248 storeszero at stack+0x18|nullptr|
| UA override |42fb20c takes tab+0x60;42fb244 stores its pointer at stack+0x20|full native->user_agent_override|
| Extra data |42fb240 takes tab+0x108;42fb244 stores pointer at stack+0x28|native->extra_data|
| From session restore |42fb23c storesfalse at stack+0x30|false|

These original call-stack offsets are relative to42fb100's SP at its421e280 call, not the callee's frame pointer. The C156 parameter order is `chrome/browser/ui/browser_tabrestore.h:49–66` and `.cc:268–292`. The target browser is the requesting browser; no TabRestoreService historical-entry lookup or new browser factory is used here. Pin/group index corrections remain inside native AddRestoredTab; the raw active+1 value is not a promise of the final index after native constraints.

The index-search403d24 distinguishes found/absent with bit0 ofx1, returns the flattened index inw0, and42fb1c8 adds1 only when found. It is not a general “append after count” policy. Current insertion uses that same after-active convention. For nonempty history the current normalization formula and original signed min/zero clamp agree; this audit does not infer malformed/empty peer behavior.

## Loading, return and scoped insertion

Original42fb278 calls the AddRestoredTab analogue421e280, keeps returned native WC inx22 at42fb27c, and immediately calls6661c88 on its controller atWC+0x260 (42fb280..284). Current code likewise calls `GetController().LoadIfNecessary()` after AddRestoredTab and returns its ArcWebContents wrapper. C156 CreateRestoredTab restores serialized entries and UA, applies extension app ID/extra data, creates without an immediate renderer, and passes session storage through the native namespace map (`browser_tabrestore.cc:52–102`). The normal path returns the newly inserted WebContents. Current's null-result conditional is outside this supported successful result path.

One structural argument difference is retained explicitly: original42fb220..238 obtains a browser virtual boolean, marks its optional engaged with0x100, and42fb25c stores it at caller stack+0x38. Current supplies `std::nullopt` for `is_active_browser`. No normal C156 Mac behavior mismatch follows from the audited helper: `.cc:247–256` replaces `is_active_browser.value_or(true)` with a browser-type test on Mac, and select=false also takes the hidden-tab branch at219–221. Both routes then explicitly load afterward. Original421e280's examined body does not read this caller-stack+0x38 optional; do not infer its virtual boolean's semantic name solely from the native vtable offset.

Original42fb124 checks a byte at browserImpl+0x179,42fb168 sets it for this operation and42fb2ac restores its saved value after the native wrap. Current `ScopedSessionInsertion(self,nil)` installs/restores a browser-local insertion state; `ArcSessionsDidInsertWebContents` suppresses ordinary creation without emitting a restore callback when that state's session is nil. This audit verifies the current suppression mechanism and original scoped flag, but does not newly prove every original flag consumer, callback suppression edge or arbitrary reentrancy outcome. Native helper side effects remain native; no synthetic didRestore callback is introduced by the current foreign-tab path.

Original ObjC43e2c10 requests the native session_tab peer and allocates a new ArcWebContents wrapper at43e2c90 onward. Current returns its canonical wrapper. That already-known wrapper identity difference is not a new metadata finding and is outside this bounded input-mapping conclusion. No malformed private peer, subclass, disengaged DTO, cross-profile storage, native group destination or cancellation fixture is invented here. No code fix is justified by the reviewed mapping alone.

Independent read-only review by `/root/sessions_recon/full_probe_review` agrees on the metadata argument choices, explicit load and lack of an identified Mac active-optional consequence. No apps/probes/builds, source/shared edits, board or credential actions were performed.
