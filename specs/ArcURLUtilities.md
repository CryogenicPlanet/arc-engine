# ArcURLUtilities

ArcCore 1.170.0 arm64; Chromium 156.0.8078.48. 2 declared method identities (base and category definitions counted separately). All VAs below are unslid engine VAs, not app 0x100… addresses.

## State and layout

Superclass `NSObject`; instanceStart 8, instanceSize 8 bytes; class metadata `0xe37e300`, class_ro `0xe2ae420`. Declared protocols: none. Raw binary read and independent dyld reconciliation: [inventory evidence](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/inventory-validation.json). Full ivar encodings/record addresses: [layout.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/layout.json). These are metadata facts, not an ownership inference.

| Ivar | Byte offset | Size | Type |
|---|---:|---:|---|
| No declared ivars | — | — | — |

## Lifecycle, ownership and callbacks

No instance ivars; both methods are class methods. `0x443dd58` converts host and returns nil for empty native registrable-domain result; `0x443de84` converts formatted display host. Native bridges `0x4345480`/`0x434563c` distinguish empty/IP-like hosts and canonicalization paths.

## Threading and caller/callee relations

Registry helper call `0x4345480` → `0xda58a0` and IDN conversion `0x434563c` → `0x788520` are bounded source-backed hypotheses. Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `net/base/registry_controlled_domains/registry_controlled_domain.h:162`/:193 (fetched under specs/sources) defines registrable-domain behavior/private filters; Chromium `d0c02bfad08ec92a6ffcf989e53cd995e6381a4c`, `components/url_formatter/url_formatter.h:178`/:185 defines IDN display conversion and fallback. No locale/main-thread assumption follows from these synchronous wrappers.

## Chrome / CEF route implications

Can use pinned Chromium registry/IDN utilities in a Chrome route or reproduce identical public-suffix/IDN behavior in a shim. Return nil versus empty and private-registry filtering are compatibility gates; CEF URL utilities are not yet compared.

## Method matrix

Each M anchor is the binary ObjC method-record VA; D is the raw `inputs/context/analysis/data/dobjc170.txt` line. Every B link is the exact indexed IMP body copied under `bodies/`; original shard path/byte offset/length are in [body-index.json](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/evidence/body-index.json). Ghidra function names may describe another class sharing the IMP. H means high confidence only for local field/ABI facts, M is local or partial bridge review, U is unresolved. No row claims dynamic equivalence or exhaustive backend verification. Full unresolved questions and decompiler flags are in [methods.tsv](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/methods.tsv).

| Category | Kind/selector | Type encoding | IMP VA / body | Behavior | Backend/callees | Metadata evidence | Confidence/status |
|---|---|---|---|---|---|---|---|
| (base) | `+effectiveTopLevelDomainFromHost:includePrivateRegistries:` | `@28@0:8@16B24` | [0x443dd58](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/443dd58.c) | Converts host to native string and calls 0x4345480; returns nil for empty native result, NSString otherwise. Backend rejects empty/IP-like host and calls registry helper with private-registry flag. | 0x1b356b8; 0x1c391bc; 0x4345480 | M `0xd77ca50`; D 3075 | M; available; bridge_reviewed_partial |
| (base) | `+formatHostForDisplay:` | `@24@0:8@16` | [0x443de84](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/bodies/443de84.c) | Converts host, calls 0x434563c and returns string. Backend canonicalizes, distinguishes IP-like host and conditionally performs IDN display conversion with original-host fallback. | 0x1b356b8; 0x1c391bc; 0x434563c | M `0xd77ca5c`; D 3076 | M; available; bridge_reviewed_partial |

## Remaining consequential questions

Test empty, IP/IPv6, trailing dots, private registries, unknown suffixes, Unicode/punycode spoof checks and invalid hosts against original engine.

Further per-method questions are in `methods.tsv`. Header availability is distinct from body correctness. No timing, ownership or thread guarantee is inferred solely from a selector name or type encoding.


Original reviewed source: [ArcURLUtilities.md](../research/2026-10-22-sol/evidence/agents/spec-4/revisions/v1/classes/ArcURLUtilities.md).
