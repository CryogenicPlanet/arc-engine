# Batch7 evidence review

Build/package and hash stability pass. Canonical restart replay fails at the first close; recorded replacement discard contract passes. No new fixture P1/P2 found within those scopes.

- Canonical restart: close call 8796 returns 8799; genuine main-thread didClose callback arrives 8858 and returns 9205. Accepted original closes inline. Immediate public isInitialized remains true at 8800/8801; driver response has no close callback yet. The callback later reports historical ID 187627389 for source 187627387.
- Only launch 1 exists and there are zero restore requests. Restart/persistence restoration is a cutoff, not a separately observed failed restore. Cleanup reports Browser.close with browser plus 9 helpers exited.
- Recorded discard: 6670 discard →6681 return →6738 finalize(true) →6739 callback return, main and asynchronous, exactly one selected-wrapper finalization. Probe forwarding attribution remains explicit.
- Selected wrapper 0x11c0fc44880 and live SessionID 1555513126 remain unchanged after true discard and reload. Earlier original supplemental result instead changed 2138339481→2138339482 on the same wrapper. This is an observed identity difference; the ours contract does not assert original identity parity.
- Exact A/B/C URLs/titles with B selected survive both discard and reload. AppKit shutdown is acknowledged for exact PID/nonce, parent PID 50633 exits without TERM. No helper-exit enumeration claim for this supplemental probe.
- All 7 batch before/after hashes match and match review-time files. All 6 frozen v8 artifacts match their preparation manifest. Framework used: 6b6f7cd5e50a88f2f2c09e0fc46ce33dd1b30e539a609eff66695ff298557ad5; sessions source: 6e8f6484e501ea1a37142955d7074dcd1511176adce7ca0e5302e7c14efacc17.

Default diff launch 1 reports 99 differing methods; the comparison includes cutoff effects. Neither replacement restart acceptance nor canonical discard/service parity is established by this batch.
