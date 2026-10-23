# Fresh narrow review: PASS

Reviewed executor SHA256 `8701261ae1af7e765bad2207741df67f590a91ae2920118701e0f4652fc2e4fd` against v3 `b3627435ecb682d2221a7f64fc85df50c2373abb0bc9f381f90b938ad57391c0`. The evidence copy and `/tmp` executor match byte-for-byte. No blocking issue found in this delta.

The only operational change is compile-command selection from preserved Siso query bytes. The selector filters candidate lines before decoding; it requires one exact output token and one requested source token (exact or path-suffix), rejects zero or multiple candidates, strictly decodes only the unique candidate, and independently verifies both arguments after `shlex.split`. The query log SHA256 is recorded in compiler evidence. Unrelated invalid UTF8 therefore cannot defeat a valid selected command, while invalid bytes in the selected command remain an error. The selector is deliberately narrow for the existing trusted Siso command format; it is not a general shell parser.

The three exception serialization sites now use exception type plus the first 4096 message characters instead of `repr(error)`. For the observed UnicodeDecodeError this avoids serializing the full input byte buffer. Formatting does not alter rollback, cleanup, or lock behavior. Private-index regeneration, regen-tool pins, runtime-runner alias normalization, and publication guards are unchanged by the inspected diff.

Inspected all eleven reported pure AST-isolated controls. Independently extracted only the two added functions and verified the actual preserved 20,858,643-byte query (SHA256 `2ec2cf5e16b6c6d544dec727ef5b91ae3fb33c591db36d98fe1e065c3a157158`), selected 264 arguments with `-Werror`, accepted unrelated invalid-byte noise, rejected duplicate/missing/output-lookalike/wrong-source candidates, rejected selected invalid UTF8, and checked bounded long-error formatting. Independent controls passed.

No actual executor import, constructor, entrypoint, live query, compiler, index, lock, application, runtime, queue, or Chirp operation was performed. Only this review.md and review.json were written. This review covers the v4 delta only; no adoption or runtime acceptance claim.
