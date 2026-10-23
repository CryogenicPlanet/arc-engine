# Owned sessions close-accounting companion review

PASS: no new P1/P2 against the exact staged core prerequisites. Unapplied production remains SHA b7c5c201a2395ecf9cd1bd736958432ed42076f2b970cdbaf627fd18e45e5206; staged source594b1dca2d070ec8354d420d79832b2deb9a7e7b6048c686f875bdf97446ae48. Patch exactly matches their diff.

- Capture remains after PM discard, surviving-current-binding check and selection of the current registration. The adapter getter returns the exact wrapper-owned logical state, initialized before its first setter; no fresh/native-pointer state lookup or registration retarget is introduced.
- Existing current-default runner, FROM_HERE and one PostTask remain. The Make adapter adds pending synchronously and wraps the existing delivery rather than posting a second task.
- Weak wrapper/adapter captures remain. Normal app callback, expired wrapper, expired adapter and missing delegate paths all return through compatibility decrement and then core UI decrement. Core pending remains nonzero through app callback return. No new owner cycle is added.
- Local SessionsDiscard.pending remains the compatibility predicate; it is not another native accounting channel and has no new reset. Rejected/dropped work gains no synthetic decrement; no rebind/cancel retarget or reset is introduced.
- Ordering is consistent with original discard-completion-capture.md: operation →current registration selection →pending increment/post →captured registration callback or expired skip →decrement after return.

Recorded compiler.log reports compiler_exit0 using the staged typed headers; this reviewer did not run a build/compiler. No linked/runtime acceptance is claimed. Setter wiring, core close decisions, canonical rebind and cross-service migration remain separate prerequisites. No exception/cancellation/reentry runtime policy is inferred, and missing-finalize results are not repaired by accounting alone.

Only review.md/json were written; no production, shared source, app, probe, build, or Chirp actions occurred.
