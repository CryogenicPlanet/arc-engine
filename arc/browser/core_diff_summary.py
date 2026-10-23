#!/usr/bin/env python3
"""Extract core-owned rows without changing or normalizing the raw diff."""

import argparse
import json
import re
from pathlib import Path


CORE_CLASSES = set("""
ArcBrowser ArcBrowserCreateParams ArcWebContents ArcWebContentsConfiguration
ArcWebContentsNavigationController ArcNavigation ArcNavigationAction
ArcBackForwardList ArcBackForwardListItem ArcSessionID ArcRedirectEntry
ArcSecureContext ArcKeyEventData ArcKeyboardAccelerator ArcOverscrollBehavior
ArcOverscrollParams ArcFindResult ArcFindTextWithDirectiveResult ArcTabGroupInfo
ArcTabGroupProperties ArcTabGroupResult
""".split())
CORE_CALLBACKS = {
    "webContents:decidePolicyForNavigationAction:",
    "webContentsClosingImminent:withSessionID:",
    "webContents:didCloseWithSessionID:",
    "webContents:didChangeStatusText:",
    "webContents:didChangeVisibleSecurityState:",
    "webContents:didStartProvisionalNavigation:",
    "webContents:didCommitNavigation:",
    "webContents:didFinishNavigation:",
    "webContents:didReceiveServerRedirectForProvisionalNavigation:",
    "webContents:didFailNavigation:",
    "webContents:didFailProvisionalNavigation:withError:",
    "webContents:didStartInPageNavigation:",
    "webContents:didFinishInPageNavigation:",
    "browser:didCreateNewWebContents:isActive:",
}


def owned(method):
    match = re.fullmatch(r"(->engine|engine->app) [+-](\S+) (\S+)", method)
    if not match:
        return False
    direction, class_name, selector = match.groups()
    if direction == "engine->app":
        return selector in CORE_CALLBACKS
    if class_name in CORE_CLASSES:
        return True
    return (class_name, selector) in {
        ("ArcBrowserContext", "createWebContents:"),
        ("ArcBrowserApplication", "createBrowser:withDelegate:forWindow:"),
        ("ArcBrowserApplication", "createPopupBrowser:withDelegate:"),
        ("ArcBrowserApplication", "restoreBrowser:withSessionID:browserCreatedCallback:finishedCallback:"),
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("diff", type=Path)
    parser.add_argument("--json", type=Path)
    args = parser.parse_args()
    raw = json.loads(args.diff.read_text())
    result = {category: [row for row in raw.get(category, []) if owned(row["method"])]
              for category in ("missing", "extra", "return_mismatch", "arg_mismatch", "count_mismatch", "internal")}
    print("Core-owned raw rows: " + ", ".join(f"{key} {len(rows)}" for key, rows in result.items()))
    print("Ownership filter only; no normalization or parity claim. Raw callback sequence remains in input diff.")
    for category, rows in result.items():
        for row in rows:
            print(f"{category}: {json.dumps(row, ensure_ascii=False)}")
    if args.json:
        args.json.write_text(json.dumps({"raw_diff": str(args.diff), "rows": result}, indent=2) + "\n")


if __name__ == "__main__":
    main()
