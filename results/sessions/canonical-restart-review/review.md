# Canonical original restart evidence review

No P1/P2 fixture issue found within the documented two-individually-closed-tab scope.

- Final golden coverage explicitly has `accepted=true`, `errors=[]`; 39/39 steps pass. Waiter and parent PIDs have exited.
- Actual app PIDs 20704 → 21563; unchanged `Default/0` disposable profile. Both standard `Browser.close` quit steps report browser and helper exits; no fallback is credited.
- Captured source IDs 758441851/758441852, historical IDs 758441853/758441854; new simultaneously initialized restored wrappers have live IDs 758441906/758441907. Historical eligibility is consumed exactly once.
- Raw close boundaries: 8034 → callback 8036 → callback return 8383 → request return 8389; 8420 → callback 8422 → callback return 8769 → request return 8775. Both are genuine main-thread engine callbacks inside close.
- Raw restore boundaries: request 4148/return 4177 then callback 4268/return 4427; request 4526/return 4527 then callback 4624/return 4787. Exact historical-ID getter values link requests to callbacks. Both are genuine main-thread engine callbacks after request return.
- Both histories retain exactly A/B/C URLs and titles, index 1 selected; sentinel survives. All gate rows match raw rows exactly, both raw hashes match, and all 74/159 driver-origin public calls have unique paired returns.

Limits: this is original acceptance, not replacement parity, window restore, or startup reopen coverage. Storage is opaque presence evidence only. Historical v6 provenance and current adapted source hashes are distinct. Embedded `.candidate` gate paths are pre-promotion paths; the final records match the hashes. Driver-origin calls remain excluded by the default comparator, so these independent gates remain necessary.
