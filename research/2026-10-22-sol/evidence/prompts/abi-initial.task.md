Role: app-to-engine ABI and call contract. Read the common constraints included above.

Regenerate the imported-class surface for Arc app 1.168.0 against engine 1.170.0. Preserve class vs instance methods, categories, superclass/protocol/layout, signature/type encoding and engine implementation address. Compare older engine metadata to find real compatibility differences. Validate dyld/otool pairing by selector and counts, recording unknowns instead of fabricated types.

Search the existing APP pseudocode, app stubs and binary metadata for call-site-confirmed engine usage. Dynamic ObjC dispatch may be unresolved: distinguish confirmed address/caller evidence, selector-only evidence and methods merely declared on imported classes. Prioritize lifecycle/bootstrap, browser context/window/web contents, delegate callbacks, downloads, devtools and ownership/threading boundaries.

Deliver initial/report.md, initial/contract.tsv (one row per declared method with evidence/status columns), initial/used-calls.tsv, scripts to regenerate them, and initial/decompile-requests.tsv with engine addresses/selectors and questions to answer when ENGINE pseudocode arrives. Document how 834/63 compares with the actual parsed inventory. Do not invent parameter names or treat Swift-facing types as proven by encoding alone.
