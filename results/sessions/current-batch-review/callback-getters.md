# Callback-time getter audit of existing canonical recordings

**No callback-time selected-WebContents getter oracle is recorded.** The traces confirm synchronous original close versus asynchronous replacement close, but cannot prove a native binding/getter difference during the genuine didClose callback.

| Boundary | Original | Replacement |
|---|---|---|
| Public close call / return | 6136 / 6489 | 6380 / 6381 |
| Genuine same-WC didClose call / paired return | 6138 / 6483 | 6428 / 6773 |
| Interior rows | 344 | 344 |
| Selected-WC getter rows inside callback | 0 | 0 |
| Any ArcWebContents rows inside callback | 0 | 0 |
| First selected-WC navigationController getter, after callback | 6484 / 6485 | 6774 / 6775 |

Each callback interior records one ArcSessionID.identifier call and 171 ArcSearchEngine.url calls, with their returns. The SessionID access is to the callback identifier, not selectedWC.sessionId. There are no recorded isInitialized, nativeView, url, or sessionId accesses on the closed WC inside either callback interval.

Both traces read navigationController immediately after callback return and then remove its observer. Those accesses are outside the requested interval; cached wrapper return values do not establish callback-time native binding validity. Unrecorded driver alias reads or other activity cannot be ruled out by missing rows. The replacement fixture stops before restoration because its synchronous-close assertion fails.

[Exact rows, hashes, interval counts, and limits](callback-getters.json). No new app runs, builds, source changes, or coordination-board actions.
