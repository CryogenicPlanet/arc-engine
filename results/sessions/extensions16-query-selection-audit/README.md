# Saved Extensions bootstrap command selection audit

**Confirmed source-path spelling rejection, not UTF-8, quoting, object-target ambiguity or compiler failure.** The saved query exited0. Controller select_compile refuses at line273 before its UTF-8/shlex stage274 and canonical argv-identity stage276.

The 164,827,289-byte saved command031 query contains 115,673 lines. Its sole line naming both arc_browser_application.mm and arc_browser_application.o is line115673 (10,447 bytes; SHA256 f90e56f9be7dd5bdb2060b02e29d89bd042b6deaea56faba77dadcd5cfcfa978). Bounded argument fragment:

    -c ../../arc/api/arc_browser_application.mm -o obj/arc/arc_core/arc_browser_application.o

It decodes strictly as UTF-8 and shlex parses386 argv, with exactly one -c and one -o. Both operands resolve to the configured exact source/object under the recorded build cwd. Sourcealias chromium/src/arc currently resolves to the repository arc directory. The configured bootstrap source_argument itself uses ../../arc/api/arc_browser_application.mm.

Controller262–278 instead canonicalizes expected_source with resolve() at269, then builds the source regex from only its canonical relative and absolute spellings270:

    ../../../../arc/api/arc_browser_application.mm
    /Users/cryogenicplanet/general/arc-engine/arc/api/arc_browser_application.mm

Neither literal spelling appears as the candidate's -c operand. The source regex matches0 query lines, the exact object regex matches1, and their intersection is0. Thus the valid symlink-relative spelling is rejected by the prefilter, before canonical validation could accept it. No guard weakening or tool modification was performed or proposed for automatic execution. Any selector correction remains the owner's separately reviewed change; preserve exact canonical source/object uniqueness and failure behavior.

Saved RECOVERY_REQUIRED.json independently records rollback_error `RecoveryRequired('real index drift across working-tree observation')`. This audit does not inspect/reconcile the index and does not establish that drift's cause or recovery safety. GUARDIAN_HOLD remains the owner's responsibility. No release, retry, process or lock action is authorized by this finding.

Scope: saved command/query artifacts plus controller/config source inspection and read-only current path resolution. No controller/import execution, compiler/build/graph query, Git/index access, apps, signals, queues, lock changes, Chirp or source/tool edits. Only this new evidence directory was written. Full query/command contents are not duplicated; inputs, line hash and bounded facts are pinned in manifest.json/selection-facts.json. Current symlink resolution is not an independent historical query-time symlink seal.
