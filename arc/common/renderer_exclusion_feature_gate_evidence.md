# Feature11 gates exclusion publication, not installed filtering

Read-only native audit, 2026-10-23. Addresses refer to the arm64 slice of
`../arc/build/ArcModAgent-trace156.app/Contents/Frameworks/ArcCore.framework/Versions/A/ArcCore`.
This audit used bounded disassembly through
`arc/common/renderer_contract_evidence.py` and existing paired probe outputs.
No apps, builds, locks, production changes, or shared contract-file edits.

## Correction

The prior interpretation that **explicit SetExcludedDomains publishes while
feature11 is false was wrong**. The global setter always caches the latest
domains, but feature11 gates forwarding them to profile services. A second
feature check gates every renderer send when the queued notification runs.
Neither gate clears or disables an already installed renderer filter.

This corrects the earlier exclusion-chain summary which omitted branches
`0x4297274-0x429727c` and `0x4730fec-0x4730ff4`.

## Global cache is updated before the feature gate

`SetExcludedDomains` backend **0x42bde04** obtains the singleton manager through
`0x4296d5c`, moves its input vector, and invokes **0x42971f0** at `0x42bde3c`.
There is no feature check in this short outer wrapper.

Manager `0x42971f0` first stores/replaces the global raw-string vector:

- manager+48: vector storage;
- manager+56: size;
- manager+64: capacity;
- manager+72: optional engagement, set to1 when first populated.

The initial-store branch is `0x429721c-0x4297244`; the replacement branch
converges through `0x4297254-0x4297270`. Both reach:

```asm
0x4297274 mov w0, #11
0x4297278 bl 0x20cf42c         ; current global feature11 getter
0x429727c cbz w0, 0x429728c    ; false -> epilogue/return
0x4297280 ldr x21, [x19, #80]  ; only true starts profile iteration
...
0x429732c bl 0x4297370         ; forward exclusions to one profile
```

Therefore a false-feature setter retains its new value for later enable but
does **not** update any profile service, post any exclusion notification from
that service, mutate renderer exclusions, or clear renderer cosmetic caches.
An explicitly empty vector is still an engaged, retained global value.

## Profile service updates synchronously; notification reads latest state later

Profile-forwarder **0x4297370** requires manager engagement+72, obtains the
profile's service via `0x4296bcc`, clones the current global strings, and invokes
**0x4730480** at `0x42973c4`. It has no separate feature test of its own; its
callers determine whether forwarding occurs.

Service setter **0x4730480** normalizes domain strings into Origin values and
replaces the refcounted origin-vector pointer at **service+112**:

```asm
0x47304f0 ldr x20, [x19, #112] ; previous vector
0x47304f4 str x0, [x19, #112]  ; replacement vector
```

It then posts **0x4730fa0**, `NotifyExcludedDomainsChanged`. The posted binding
is constructed at `0x473055c-0x47305d4`: it retains the weak service state from
service+192/+200, and installs callback address `0x4730fa0` at `0x47305ac`.
The closure does **not** capture the exclusion-vector pointer or its contents.
Its lifetime is tied to the service's weak state.

The callback iterates the service's bound agents. For **each agent**, it performs
the current feature query and only then loads the current service vector:

```asm
0x4730fec mov w0, #11
0x4730ff0 bl 0x20cf42c         ; live value when this queued callback executes
0x4730ff4 cbz w0, 0x4731008    ; skip this agent, send no reset
0x4730ff8 ldr x0, [x21, #56]   ; agent Mojo proxy
0x4730ffc ldr x8, [x19, #112]  ; LATEST service origin-vector
0x4731000 add x1, x8, #8       ; vector payload
0x4731004 bl 0x441fb58         ; send excluded origins
```

These are two distinct gates. For example, a setter accepted while true can
update a profile service, then its task can run after disable and skip renderer
publication. The profile cache stays updated; old renderer state remains.
Conversely, several true setters before queued tasks execute can cause those
tasks to send the latest service vector repeatedly. Capturing an immutable
per-setter vector would change this native behavior.

## Re-enable uses the latest global cache

Feature observer **0x42978ac** handles only feature11. When disabling, it clears
the manager's global compiled-pair optional, but does not clear global excluded
domains. Its profile loop skips exclusions when current feature is false:

```asm
0x4297984 cbz w20, 0x4297994   ; w20 is feature value from 0x42978d0
0x4297988 mov x0, x19          ; manager
0x429798c mov x1, x21          ; profile
0x4297990 bl 0x4297370         ; re-enable forwards latest retained exclusions
```

In the long probe, false-feature exclusion was followed by false-feature clear.
The global cache was consequently empty at enable. Re-enable forwards that
empty vector, so blocking remains active. If an excluded domain were the last
false-feature setter before enable, these branches predict that enable would
publish that exclusion. That variant is a static prediction, not exercised by
the 12-stage run cited below.

## Renderer cache and throttle lifetime

Delivered exclusion receiver **0x43a6e54** copies the vector and replaces the
renderer agent's origin-vector at physical **+376**, under mutex **+312**, at
`0x43a6f14-0x43a6f54`. It then clears the cosmetic host-cache map at
**+208/+216/+224**, `0x43a6f58-0x43a6f78`, invalidates cached state, and
enumerates all existing frames at `0x43a6fb4` to reapply cosmetic CSS.
No delivered message means none of those operations occurs.

**Equal/repeated deliveries still invalidate.** Receiver `0x43a6e54` has no
revision argument, vector-equality test, or deduplication branch. Allocation at
`0x43a6e74-0x43a6e78` and the replacement/cache-clear path above occur on every
delivery, including an empty or identical vector. Native serializer `0x441fb58`
sends the origin vector, not our synthetic configuration revision. Thus several
queued callbacks which all read the same latest profile vector still cause
several renderer invalidations. In our transport, rejecting `version <= current`
would incorrectly suppress those equal-version deliveries; an optional stale
transport guard must reject only `version < current`. The latter guard itself
is our transport protection, not a recovered native versioning feature.

Native network throttle constructor **0x4733814** retains the shared ruleset
dealer through provider virtual+24 at `0x4733850-0x4733868`, then obtains an
immutable excluded-origin snapshot through provider virtual+16 at
`0x4733888-0x473389c`, storing the latter at **throttle+24**.
`WillStartRequest` reads that captured pointer at `0x4733c4c-0x4733c70`.
There is no live feature11 query in this request matcher. Existing throttles
retain their snapshots; future throttles obtain the latest renderer snapshot.
Updating rules uses the shared dealer separately and does not replace this
snapshot rule.

Thus the proper correction belongs in browser cache/publication handling.
Adding feature11 gating to installed renderer matchers or cosmetic application
would reproduce neither the native lifetime nor the observed behavior.

## Paired 12-stage observations

Files:

- `results/renderer-long-readiness-run2/trace156/adblock-observations.json`
- `results/renderer-long-readiness-run2/ours156/adblock-observations.json`

| Stage | Native script/count/cosmetic display | Ours before fix |
|---|---|---|
| baseline | loaded / 0 / block | loaded / 0 / block |
| rule-active | blocked / 1 / none | blocked / 1 / none |
| rule-active-new-document | blocked / 1 / none | blocked / 1 / block |
| feature-disabled | blocked / 2 / none | blocked / 2 / block |
| feature-disabled-excluded | **blocked / 3 / none** | **loaded / 2 / block** |
| feature-disabled-exclusions-cleared | blocked / 4 / none | blocked / 3 / none |
| feature-reenabled | blocked / 5 / none | blocked / 4 / none |
| exclude-ip-no-port | loaded / 5 / block | loaded / 4 / block |
| exclude-ip-with-port | loaded / 5 / block | loaded / 4 / block |
| exclusions-cleared | blocked / 6 / none | blocked / 5 / none |
| badfilter | loaded / 6 / none | loaded / 5 / none |
| lists-cleared | loaded / 6 / block | loaded / 5 / block |

The stage names retain the historical "ip" labels; this run uses
renderer-probe.test. Disabled debug responses are `Native ad blocker feature
disabled` in both engines. Native re-enable/clear debug responses are Success;
the pre-fix ours responses are Cached. Those payloads must still be compared
after publication is corrected, because the exclusion receiver invalidates
the shared cosmetic/debug cache.

The existing-document-navigation cosmetic difference predates the false-feature
setter and is separately uncovered/pending hook integration. Do not attribute
it to this exclusion gate. The one-count difference beginning at false-feature
exclusion is explained by ours allowing a request which native blocks.

## Implementation scope supported by this proof

- Always retain latest global exclusions, including while false.
- Forward/update profile exclusions only when the manager setter observes true.
- Preserve profile exclusion cache updates separately from queued renderer sends.
- At queued broadcast execution, read current feature state, then latest profile
  exclusions; skip sends while false without resets. Do not capture per-setter
  origins into that broadcast task.
- Re-enable forwards latest global exclusions and queues publication.
- Keep initial registration eligibility distinct: native registration
  `0x4731028` synchronously checks feature11 and sends its initial messages at
  registration. A blanket live-feature guard in a delayed fallback's
  `SendPendingConfiguration` can wrongly discard a message corresponding to an
  earlier enabled registration. The queued broadcast guard alone does not prove
  that all pending initial messages should be dropped.

Reproduce key branches:

```sh
python3 arc/common/renderer_contract_evidence.py 0x42971f0:0x160 0x4297370:0x70
python3 arc/common/renderer_contract_evidence.py 0x47304f0:0xe8 0x4730fa0:0x88
python3 arc/common/renderer_contract_evidence.py 0x42978ac:0x108
python3 arc/common/renderer_contract_evidence.py 0x4733814:0x8c 0x4733c4c:0x28 0x43a6f14:0xa4
```
