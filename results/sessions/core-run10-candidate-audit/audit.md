# Core integration10 candidate: sessions audit

**Candidate only; index guard failed and core/parent reports owned-source rollback. Not accepted live integration.** No new sessions P1/P2 established by this bounded collection.

The candidate's copied sessions source is e973b371ff612fcb85132ee992af900937f92fbc325d06a97ec9e4b17a9c668d; sessions.gni is6f983408cd5c903507a5ae093cc6bee7700a0aa69e83d0c017581bcb3a8a9830. All122 Arc build-input entries are equal before/after/final, as are46 selected source,40 native and56 runtime-input entries. This establishes candidate source provenance, not live promotion. Older core9 used a647 source and the same sessions GNI.

Candidate launch-slice9/9 and tabs-navigation11/11 pass, harness0, coverageaccepted/errors=[]; Browser.close receipts report8/6/9 helper exits and fatal app-log signature lists are empty. Packaged framework38cd05e579e2e84e4206b71f112c87b58a80a1f463abff2ad5eb3abe67f5460e. No OS normal-exit status was captured.

Default diff headers(candidate):launch1 methods43/callback106vs137; launch2 methods61/callback47vs62; tabs methods61/callback162vs200, all first divergence1. Older core9:methods106/57/56; callbacks106vs128,47vs61,162vs182. Verbatim firsttwo lines and complete raw hashes/pairs are adjacent. Diff exits0 indicate successful comparison execution, not parity.

Sessions raw shape: candidate ID getter counts8/3/14 versus older5/3/14; configured policy scalar0 at initializerseq1 in all records, no policygetter call. Positive ID values are generated per run and retained verbatim. Candidate global didCloseBrowser counts0/0/0 versus older1/0/0. The older launch1 callback6040(type0,nil restorationID) occurs BEFORE tryToTerminate6148; candidate has none and tryToTerminate7256. All terminate calls return2. This is window-lifetime observation, not a quit-time callback regression or proof of global parity.

The raw records contain no public close selector invocation, restore selector, discard selector or session DTO class beyond ArcSessionID. Public synchronous-close, historical restore IDs, full-window restore, navigation setter guards/publication, selected-adapter lifetime, discard/rebind and startuprestore remain uncovered. No acceptance can be inferred from these two functional scenarios.

Only this audit directory was written. No build/compiler/app/git-status/Chirp/process actions or production changes; queued4709 was untouched.
