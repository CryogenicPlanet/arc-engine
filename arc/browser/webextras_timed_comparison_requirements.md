# Timed media comparison requirements after24437

**No sound elapsed-time normalizer can be derived from the existing canonical bare records.** Keep the numeric mismatch visible. Lead policy must classify an unverifiable timing difference; neither a broad numeric mask nor an invented tolerance supplies missing evidence. No comparator/recorder/source code, lock, build or runtime was changed.

| Capture | Bound call/return | DTO receiver | Raw currentTime |
|---|---|---|---:|
| Canonical gold `results/golden/arc-1.170-media-pip/launch-1/record.jsonl` |4818/4819|ArcMediaPosition#164|0.322324|
| Frozen current run1 `results/webextras/media-pip-current-run1/launch-1/record.jsonl` |4754/4755|ArcMediaPosition#145|0.422503|

The raw difference is0.100179seconds. Generic `results/ours/media-pip` is a DIFFERENT capture (4756/4757,0.413762); it must not substitute for current-run1.

Both canonical record field unions lack timestamps, ticks, elapsed windows and position anchors. Each contains only one currentTime sample; no playbackRate/duration/endOfMedia getter evidence accompanies this DTO. Its initializer arguments are only `["<ptr>"]`. Position-return→DTO-getter receiver linkage is visible (gold4814–4819; ours4748–4755), but the opaque native initializer does not reveal copied position/rate/duration/last-update time. `native-1.json` is an accessibility snapshot. `steps.json` reports64×64, advancing=true, paused=false and title, without getter-bound timestamps or position values. Coverage is selector coverage; runner runtimeAccepted and run start/end, binary mtimes and input-capture times cannot reconstruct this getter's age. There is no justified retrospective elapsed interval.

Native9 is a separate service-only experiment. Its frozen `media-clock.json` on each side records six complete phases, five measured attempts per phase, heldToken/dtoTag/owner, Mach before/after getter ticks, integer timebase125/3, raw byte offsets and ABI; its evaluated producer receipts retain exact public setPositionState inputs and host/renderer brackets. The independent audit verifies115 public pairs, six releases, same-held-DTO slopes and distinct paused anchors1.25/2.25/7.25. Its predeclared4us×(1+abs(rate)) allowance is a bounded quantization policy for that experiment, **not a canonical-PiP tolerance**. It supplies neither these canonical getter timings nor cross-run clock-origin alignment; even that experiment explicitly leaves PLAYING absolute offset/alignment uncovered.

An optional future explicit-sidecar mode needs all of the following before it can report timing capability:

1. Hash-bind each sidecar to the exact transcript, binary/recorder/producer and policy version; bind every measurement to its actual caller-aware call/return, selector/ABI, owner, DTO creation/position return, receiver and byte/seq span. Require nonempty complete coverage; no inferred ordinal matches or arbitrary renumbering.
2. Preserve the copied source position, rate, duration, end flag and native last-update anchor, plus proved receipt/update generation. Bind the sample to brackets in the SAME clock domain/timebase as that anchor, or provide measured conversion bounds. Host hrtime, renderer performance.now and native Mach origins cannot be equated. The same semantic stimulus/phase, source generation at each copy, and timing of intervening seek/rate/pause/source replacement must be established independently on both runs. Later Source changes do not mutate the retained copied value.
3. Derive each result's allowed interval from that state and measured elapsed bounds under a reviewed clock/clamp/serialization model. For the finite stable C156 case, the copied value evolves as clamp(position+rate×max(0,now−lastUpdate),0,duration); its end flag remains a separate field. Proven rounding/quantization bounds come from the exact implementation and recorder format, not the observed mismatch or fitted jitter. Unsupported rates/states/clock mappings fail closed. Matching intervals alone do not prove the same source generation or initial offset.
4. Keep raw values and strict counts/callbacks/arguments/other returns intact. Report explicit policy-backed timing evidence separately from exact payload parity. Missing, stale, malformed, wrong-receiver or mismatched sidecars remain unsupported/failing, never a blank verified capability. Test nonempty support using actual timing/identity failures and wrong offset/rate, not a zero self-diff.

**Source timing remains separate from immutable DTO math.** Frozen current-run source copies the native MediaPosition in initWithMediaPosition (:65–68), then currentTime calls GetPosition (:73). Its service getter queries the current native source before constructing the DTO (:188–191). A proposed asynchronously cached Source.position can supply an older generation even when that copied DTO's elapsed-rate math is perfect. Same-held-DTO slope cannot prove cache freshness, the correct source update, or absolute playing offset. Do not normalize that separate getter/cache timing issue away.

PiP identity is also separate: these records show gold creation wrapper#148/getter wrapper#150 versus ours reusing#132, but both native initializers contain only `<ptr>`. Any approved canonicalization needs explicit same-underlying-controller/lifecycle evidence and the lead's selected identity policy; class, proximity, one visible PiP window and opaque pointers alone do not establish it. No additional PiP identity work or normalization is proposed here.

Read-only independent factcheck agreed. No architecture acceptance or numerical normalization is claimed. Exact inspected inputs (rechecked unchanged after writing):

```json
{
  "results/golden/arc-1.170-media-pip/launch-1/record.jsonl": "672f405866b7904b0ae0b996de12ef6496c0121b495450fede4a92acdb1d954f",
  "results/golden/arc-1.170-media-pip/steps.json": "238427e8c8dff0a860c783d12971c199f3f774cf1f4cf2f4f5589a1e5f272bb2",
  "results/golden/arc-1.170-media-pip/native-1.json": "6f523614b03ae29ec0ce900d315ff465dcc47acb830403897a636de2946ff279",
  "results/golden/arc-1.170-media-pip/coverage.json": "b93af055da5839f0ec46ff280befae6eda21446161f161af4cc4c4d21083a5a6",
  "results/webextras/media-pip-current-run1/launch-1/record.jsonl": "5760c41eef96ce0bfd05477cc86477fda80f687fc20a66e3952c207472696ff1",
  "results/webextras/media-pip-current-run1/steps.json": "a6f7cc5f0e1028f2e75725546366ec2e9add18e2190db8aa79c23f608a1f0bd1",
  "results/webextras/media-pip-current-run1/native-1.json": "88154d69109add949759806b0220fdb3ca0586cf4ed707e4f53fd200fb6f2d09",
  "results/webextras/media-pip-current-run1/coverage.json": "f7fa2c4991edc84ce50b11575a55537dd851c76853271b061908cd398ff8cbac",
  "results/webextras/media-pip-current-run1/runner-summary.json": "299e96c6b09b1b18c493883165554ee227b48393c96c51cf2299c9e237eddd73",
  "results/webextras/media-pip-current-run1/inputs/arc-engine/arc/api/arc_webextras_media.mm": "a529476a08a40798823a1b56a0888a85bf9e89415d7f41fdd7c6e974c2e29177",
  "results/webextras/native-run9-offline-diagnostic/frozen-recordings/trace156/media-clock.json": "7508b6b5e81b2858b7be28f42b05fc2a9cec7626d64394df886934670be884cc",
  "results/webextras/native-run9-offline-diagnostic/frozen-recordings/ours156/media-clock.json": "59ab6c62c852170057f0416fda085b7a376ba3cffe04756018d38f8da98c7789",
  "results/webextras/native-run9-offline-diagnostic/independent-checks.json": "098eb0f51f26b1a351ba4142d1c89830ea1e80bccaecc2952451fe5cf296bc2a",
  "chromium/src/services/media_session/public/cpp/media_position.cc": "9ee5a46cd95b066e53f95ab144be2feec53c5b7317a20a7839b39a4b1dec9813",
  "logs/wp/webextras-boundary-arguments-callback-correction/staged/tests/recorder/diff.py": "24437bed30fddc8b94ab5c9dcc7cd20e755b06c0b05a4d8620e487d3c7f612cf"
}
```
