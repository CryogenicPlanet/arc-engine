# Sessions source provenance + startup policy proposals (UNAPPLIED)

Current narrow source-only pair remains independently usable:
- /tmp/arccore-sessions-native-source-hooks.patch (Chromium cwd)
- /tmp/arccore-sessions-source-registration.patch (engine cwd)

Combined source+policy alternative, instead of source-only pair:
- /tmp/arccore-sessions-native-source-policy.patch (Chromium cwd): exact WebContents insertion source plus startup preference type hook.
- /tmp/arccore-sessions-source-policy-registration.patch (engine cwd): exact source-ID registration, own ArcSessionsConfigureRestorationPolicy and shared declaration.

Both combined patches apply against current production. Incremental policy-only pair /tmp/arccore-sessions-native-policy.patch and /tmp/arccore-sessions-policy-registration.patch is based on source-only proposal already applied, if lead chooses that order. Do not apply both alternatives. These do NOT include full-context reuse, full browser factory routing, or discard identity changes.

LEAD BOOTSTRAP WIRING IS REQUIRED, intentionally not guessed here: in arc/api/arc_browser_application.mm initialize:, snapshot configuration.sessionRestorationPolicy once after isProduction and before shouldLoadAllProfilesAtStartup, as original43ec404 and current launch golden show. Call ArcSessionsConfigureRestorationPolicy(snapshot_policy) BEFORE ChromeMainInitializeHostedBrowser/profile initialization. Bootstrap is lead-owned and other config getter ordering/snapshots remain their scope. Without this call, hook is empty and new policy behavior does not execute. No behavior-complete acceptance claim without live verification of this wiring.

Native original273d694 maps policy0 to SessionStartupPref::LAST(2), policy1 to DEFAULT(0), THEN still reads/preserves startup URLs from prefs. Do not write raw enum values to restore_on_startup: Chromium serialized PrefValue is different. Hook modifies only the in-memory preference type producer and leaves URLListToPref and guest Profile overload untouched. Explicit TabRestoreService eligibility/restore is independent and remains enabled. Configobject constructor stores the policy; validation occurs later during conversion/initialize43ec720..28, not initial object creation. Own configure CHECK_LE(policy,1) lives at that bootstrap conversion point.

Hosted native call is guarded by arcmod-host-app plus installed optional<bool> hook. Engagedfalse selects DEFAULT; it must not fall through to stored prefs. No-hook/nonhost keeps Chrome's current preference-type behavior. Original config-disengaged function traps; fallback here is for Chrome/no-hook, not a claim that original Arc had a type-pref fallback.

Stages /tmp/arccore-sessions-policy-staged (no production edits). Actual compiler command checker /tmp/arccore-sessions-check-policy.py {owned,pref}; source-only three TUs were previously checked. Combined policy prefs/owned actual Siso compiler checks exit0, both combined apply-checks0, and fresh policy-only review found no P1/P2. Source-only provenance three TUs were previously checked0. App runs/builds require shared lock; no runtime policy1 proof yet.
