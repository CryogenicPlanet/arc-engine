# Historical run1 recomparison with accepted comparator144325

OFFLINE recompare of historical frozen current-run1 records using accepted shared comparator144325; NO new engine/runtime/app/lock/source adoption and NOT current source behavior/acceptance

The isolated seven-file comparator closure and every raw input were checked unchanged. Default CLI only; no fixture/receiver/Google/argument opt-ins. These are offline reports, not a new engine capture.

launch-slice / launch-1
```text
methods differing: 103 (missing 5, extra 3, returns 12, callback args 3, counts 80; internal-only 0, ours-internal 1)
callback sequence: {"golden": 106, "ours": 127, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObser
```

launch-slice / launch-2
```text
methods differing: 55 (missing 6, extra 0, returns 11, callback args 3, counts 35; internal-only 0, ours-internal 1)
callback sequence: {"golden": 47, "ours": 62, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserve
```

devtools / launch-1
```text
methods differing: 104 (missing 22, extra 2, returns 11, callback args 3, counts 66; internal-only 10, ours-internal 20)
callback sequence: {"golden": 56, "ours": 61, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserve
```

devtools / launch-2
```text
methods differing: 107 (missing 25, extra 2, returns 14, callback args 3, counts 63; internal-only 9, ours-internal 20)
callback sequence: {"golden": 58, "ours": 62, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserve
```

media / launch-1
```text
methods differing: 54 (missing 6, extra 1, returns 10, callback args 3, counts 34; internal-only 0, ours-internal 1)
callback sequence: {"golden": 49, "ours": 69, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserve
```

pdf / launch-1
```text
methods differing: 63 (missing 5, extra 3, returns 9, callback args 2, counts 44; internal-only 0, ours-internal 1)
callback sequence: {"golden": 64, "ours": 85, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserve
```

media-pip / launch-1
```text
methods differing: 48 (missing 8, extra 1, returns 11, callback args 3, counts 25; internal-only 0, ours-internal 1)
callback sequence: {"golden": 172, "ours": 170, "first_divergence": 1, "golden_from": ["ADK.ADKApplication didInitializeBrowserProcess", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:"], "ours_from": ["ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObserverBridge onCrashKeyEmitted:value:", "ADK.CrashKeysObser
```

Owned residuals remain: unavailable native DevTools controls before public open calls; missing volume callbacks in media/media-pip; thumbnail501x341 extra and two missing DevTools thumbnails; opaque MediaActions NSIndexSet ordinal mismatch caused by earlier Core tabTraits allocation; PiP currentTime and canonical getter alias; PiP feature92/72/title2/1; historical launch1 two native owners versus one ours. Root/Core/producer integrations and narrow approved PiP alias are still pending.

The PDF scenario still records zero PDFController methods/callbacks. New accepted shared comparator reduces two global rows per pair; it does not establish new engine behavior.
