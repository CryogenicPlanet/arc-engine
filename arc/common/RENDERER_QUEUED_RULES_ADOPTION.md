# Renderer queued-rule correction and integration route

Owned correction5475 and its official interface are **applied and syntax-verified**.
The transaction completed/reaped0 under the shared lock; all14 generated outputs
match the reviewed private bytes, both prescribed checks pass, and all6 actual
compiler checks pass with empty diagnostics. Build graph/nonpermitted checked
inputs are unchanged. Exact receipt:
`logs/wp/renderer-queued-rules-adoption/transaction.json`, SHA256
`c27c1833f8356c8d00f8b06df715bd3efa9914c2915812c55587454fb163ef23`.
A fresh full Base build/package and unchanged37-step CSS/extraction regression
now pass (tool96238 completed/reaped0), with225 declared source/tool/golden/stock
inputs and runtime hashes stable. Output: `results/renderer-queued-rules-canonical-run2`;
status: `logs/wp/renderer-canonical-post-queued-rules/run/status.json` SHA256
`8d27f36d55925673c23f958d5175bd8d8191e2e66a65b01cc90459a491402523`.
This is an owned-source regression result. Global57 differences remain, including
app themeCSS3/5; full shared-stack activation and the queued disable race are
unverified. New Boost lifecycle first OURS fails new-document application; its
shared document-start hook is still absent.

After the adopted exact eight-file owned patch5475, the current
integration route is:

1. `renderer_all_post_queued_rules_current.patch`, SHA256
   `43914cf5ba13444c24f3512633e3c686a393b0e626fddc750fb631c74b33578b`.
2. Unchanged `renderer_gradient_followup_proposal.patch`, SHA256
   `66179056ee5ae3a5c76f226e805ca86a8652279ea9f8fd15ac7ec799bf129e80`.
3. `renderer_top_band_post_queued_rules_current.patch`, SHA256
   `6360353c131f710981de4b20212c9ab2e1a71840beccd22dcf4c7b9ffa86ef90`.

Alternatively, `renderer_base_post_queued_rules_aggregate_alternative.patch`,
SHA256 `129c38a49f24a67cfa0813b403fcb95489ef33a2c9b1e009ddddbf19ece3c24f`,
replaces all three steps. Never apply both routes. The old ALLbd0, TopBand9d and
historical queue follow-up7a12 must not be applied after owned5475. Their bytes
remain preserved for their historical baselines.

The new patches are proposals only. Shared Arc files and Chromium source still
require lead adoption. The maintained-root equivalent pair is now published and independently reviewed:
`renderer_native_post_queued_rules_current.patch` (Chromium-root,49 files), SHA256
`f67c298537798a5013e0ee090d1f5b94d889f589aa2550ccda2b880302c2951e`, and
`renderer_arc_post_queued_rules_current.patch` (repository-root,24 files), SHA256
`a04411c358e8ba20e3238198dec6e6d5020090cb5d83e9a2520c56786ed068c3`.
Together they replace the aggregate or three-step chain. Both are required as a
coordinated source adoption; no numbered maintained patch slot is reserved.
The kit proves both forward orders and exact reversal, with all80 current baseline
paths matching. Its manifest/review are under
`logs/wp/renderer-post-owned-adoption-kit/` and `renderer-post-owned-kit-review/`.
The current pair targets bare post-owned production. After the separately ready
core11 queue, the Arc companion has one strict WebContents context conflict;
a minimal context-only variant is being prepared. Do not force the bare patch
through that conflict or replace shared whole files. Optional startup746, activation-refreshf18 and discard723 remain
separate and HELD under their existing lifecycle/runtime conditions.

The rebase preserves current shared WebContents sourceff957 and app flagsECC.
Its19 routes/84 Git operations are exact; an independent review is clean.
Fresh composition checks for the core WebContents, app flags and queued producer
all pass with the actual compiler flags and empty diagnostics. Four unique-error
negative controls verify selection of the staged sources and shared/generated
headers. The full target, dependency hashes, commands and logs are under
`logs/wp/renderer-queued-rules-current-rebase/`, with compiler evidence under
`compiler-checks/`. These are private syntax/composition checks, not a combined
link or runtime result.

Owned5475 is pinned in
`logs/wp/renderer-queued-rules-implementation/patch-manifest.json`, SHA256
`26095c517fba297bed970b69474deb05b7a9cd953a2b1921bb2527709a10e274`.
It separates cosmetic and URL update/reset commands, queued per-profile state,
URL cancellation/identity and current-URL activation checks. Ordinary global
disable still sends nothing. Fifteen production-helper assertions execute;
four real-code service/receiver/network fixtures remain compile-only. The
existing public scenario has no fence that deterministically positions disable
between compile completion and queued C/U notification delivery. This race and
native registration/pre-bind alignment remain runtime-unverified.
