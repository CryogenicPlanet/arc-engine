# Synchronous BeforeUnload transport — UNAPPLIED

core_beforeunload_transport_chromium.patch SHA256
 e57ccbd991e538d860ed62f496d4f40046afccc27fea3e54ca3fcff0dd170625.
Depends on native close transport41de166 (and native2f423 prerequisites). Two
native files only, EMPTY default/host-gated/no consumer or public Close activation.

Original4383ea4 is void(thisImpl, source, proceed, BORROWED bool* output). It
conditionally sets15a and sends imminent synchronously, then reloads its current
proxy original-delegate weak target and forwards exact source/proceed/SAMEoutput.
Absent original delegate leaves output untouched. No160token/post is involved.
Current RFHManager owns that output on its stack and reads it immediately after
return; CloseContents-style delayed output continuations are unsafe/unproved.
Exact source/raw proof: logs/wp/core.beforeunload-native-interface/.

New hook receives only source/proceed and returns explicit transport enum
ContinueOriginal or SkipOriginal. Output storage is NEVER exposed. Skip/dead
captured exact weak source/delegate returns with output unchanged; Continue
forwards the same native arguments/output synchronously through the unchanged
Unloader policy and immediately returns. Callback copied before app callout;
no raw-this member use afterward. Empty/nonhost/null paths preserve originalbody.
Extra weak guards are safety adaptations, not recovered originalinstructions.

Future consumer MUST check logical association/original-target validity after
app callouts; weak liveness alone does not prove old browser membership after a
transfer. Original proxy reloads CURRENT target, so captured Chrome pair is not
itself full rebind semantics. No consumer installation, flags, imminent, Busy,
source token, bulk policy, async BeforeUnload response, or Close activation is
supplied. Keep native Unloader mutation/return handling and same borrowed output.

Strict incremental apply/exact reconstruction against full ten-patch core stage
PASS (thus eleven patch composition); actual native delegate TU compiler0/empty;
fresh independent review no concrete P1/P2 under EMPTY/no-consumer contract.
Evidence exactstages/hashes/argv/logs/review: logs/wp/core.beforeunload-transport/.
No full link/build/runtime/cross-platform or behavioral-equivalence claim.
