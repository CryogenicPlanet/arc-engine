#!/usr/bin/env python3
"""Strict ordered comparison of isolated AI fixture boundary and ABI recordings.

Usage:
  python3 arc/browser/ai_compare_service.py ORIGINAL_DIR OURS_DIR --out DIFF.json

Both directories must contain events.json and fixture.jsonl. No apps are launched.
Exit: 0 equal, 1 differences, 2 malformed or missing coverage.
"""

import argparse
import collections
import copy
import difflib
import json
from pathlib import Path
import sys


EVENT_KEYS = {"method", "phase", "mainThread", "synchronous", "value"}
ABI_KEYS = {"method", "mainThread", "args"}
REQUIRED_OPERATIONS = {
    "initialize", "type", "setUser", "setUser.error", "setUser.async", "tracing",
    "flags", "loadInference", "loadClassifier", "isClassifierLoaded",
    "classification", "classification.error", "loadNomic", "nomic",
    "loadRanking", "rank", "rank.async", "evaluate", "train", "stream.error",
    "initialize.error", "conversion.initialize", "conversion.setUser",
    "conversion.convert", "conversion.error", "conversion.async",
}
REQUIRED_ABI = {
    "Create", "Initialize", "Type", "SetUser", "EnableDataDogTracing",
    "LoadInferenceModel", "GetStreamedInferenceFor", "LoadClassifier",
    "IsClassifierLoaded", "GetClassificationFor", "LoadNomicEmbedText",
    "NomicEmbed", "LoadCommandBarRanking", "RankCommandBar",
    "RankCommandBar.asyncCopy", "EvaluateCommandBarRanking",
    "TrainCommandBarRanking", "Crash", "Conversion.Create",
    "Conversion.Initialize", "Conversion.SetUser", "Conversion.ConvertHtmlToMarkdown",
}
NORMALIZATIONS = [
    "Only fixtureLog.ai/conversion diagnostic JSON bodies are masked; status and event order remain strict.",
    "Only Conversion.Initialize.args.libraryPath agent bundle name is normalized.",
    "Only GetStreamedInferenceFor.args listener and userDataID are erased; public stream tokens remain strict.",
]


def canonical(value):
    return json.dumps(value, sort_keys=True, ensure_ascii=False, allow_nan=False,
                      separators=(",", ":"))


def is_diagnostic_rows(value):
    return isinstance(value, list) and bool(value) and all(
        isinstance(row, dict) and set(row) == ABI_KEYS for row in value)


def mask_diagnostic_body(value):
    """Mask actual log content, never diagnostic status codes or ordinary results."""
    if is_diagnostic_rows(value):
        return "<fixture-log diagnostic body>"
    if isinstance(value, str):
        try:
            parsed = json.loads(value)
        except (ValueError, TypeError):
            return value
        if is_diagnostic_rows(parsed):
            return "<fixture-log diagnostic body>"
        return value
    if isinstance(value, list):
        return [mask_diagnostic_body(item) for item in value]
    if isinstance(value, dict):
        result = copy.deepcopy(value)
        if "body" in result:
            result["body"] = mask_diagnostic_body(result["body"])
        return result
    return value


def load_run(directory):
    events = json.loads((directory / "events.json").read_text())
    abi = [json.loads(line) for line in (directory / "fixture.jsonl").read_text().splitlines()
           if line.strip()]
    if not isinstance(events, list):
        raise ValueError("events.json must contain an array")
    for index, row in enumerate(events):
        if not isinstance(row, dict) or set(row) != EVENT_KEYS:
            raise ValueError(f"event {index}: expected exactly {sorted(EVENT_KEYS)}")
        if not isinstance(row["method"], str) or not isinstance(row["phase"], str):
            raise ValueError(f"event {index}: method and phase must be strings")
        if type(row["mainThread"]) is not bool or type(row["synchronous"]) is not bool:
            raise ValueError(f"event {index}: thread/synchronous fields must be booleans")
    for index, row in enumerate(abi):
        if not isinstance(row, dict) or set(row) != ABI_KEYS:
            raise ValueError(f"ABI row {index}: expected exactly {sorted(ABI_KEYS)}")
        if not isinstance(row["method"], str) or not isinstance(row["args"], dict):
            raise ValueError(f"ABI row {index}: invalid method or arguments")
        if type(row["mainThread"]) is not bool:
            raise ValueError(f"ABI row {index}: mainThread must be boolean")
    # Also reject non-finite values rather than silently treating them as JSON.
    canonical(events)
    canonical(abi)
    return events, abi


def coverage(events, abi):
    pairs = {(row["method"], row["phase"]) for row in events}
    missing = []
    for method in sorted(REQUIRED_OPERATIONS):
        for phase in ("call", "return", "callback"):
            if (method, phase) not in pairs:
                missing.append(f"event:{method}:{phase}")
    for method, phase in (("createAIService", "observation"),
                          ("isAvailable.before", "observation"),
                          ("isAvailable.after", "observation"),
                          ("stream", "call"), ("stream", "return"),
                          ("stream", "partial"), ("stream", "callback"),
                          ("stream", "token"), ("stream.error", "token"),
                          ("setUser.timeout", "timeout"),
                          ("released.owner", "observation"),
                          ("released.setUser", "callbackObserved"),
                          ("probe", "finished")):
        if (method, phase) not in pairs:
            missing.append(f"event:{method}:{phase}")
    methods = {row["method"] for row in abi}
    missing.extend(f"abi:{method}" for method in sorted(REQUIRED_ABI - methods))
    if len(events) < 100:
        missing.append(f"insufficient event coverage: {len(events)} < 100")
    if len(abi) < 30:
        missing.append(f"insufficient ABI coverage: {len(abi)} < 30")
    if not any(row["method"] == "probe" and row["phase"] == "finished" and
               row["value"] is True for row in events):
        missing.append("missing successful probe finished marker")
    return {
        "events": len(events), "abi_rows": len(abi),
        "event_counts": dict(sorted(collections.Counter(
            f'{row["method"]}:{row["phase"]}' for row in events).items())),
        "abi_counts": dict(sorted(collections.Counter(row["method"] for row in abi).items())),
        "missing": missing,
    }


def normalize_events(events):
    result = copy.deepcopy(events)
    for row in result:
        if row["method"] in ("fixtureLog.ai", "fixtureLog.conversion"):
            row["value"] = mask_diagnostic_body(row["value"])
    return result


def normalize_abi(abi):
    result = copy.deepcopy(abi)
    for row in result:
        args = row["args"]
        if row["method"] == "Conversion.Initialize" and isinstance(args.get("libraryPath"), str):
            parts = args["libraryPath"].split("/")
            parts = ["ArcModAgent-<agent>.app" if part in
                     ("ArcModAgent-trace156.app", "ArcModAgent-ours156.app") else part
                     for part in parts]
            args["libraryPath"] = "/".join(parts)
        if row["method"] == "GetStreamedInferenceFor":
            args.pop("listener", None)
            args.pop("userDataID", None)
    return result


def field_differences(original, ours, path=""):
    if canonical(original) == canonical(ours):
        return []
    if isinstance(original, dict) and isinstance(ours, dict):
        result = []
        for key in sorted(original.keys() | ours.keys()):
            child = f"{path}.{key}" if path else key
            if key not in original or key not in ours:
                result.append({"path": child, "original_present": key in original,
                               "ours_present": key in ours,
                               "original": original.get(key), "ours": ours.get(key)})
            else:
                result.extend(field_differences(original[key], ours[key], child))
        return result
    if isinstance(original, list) and isinstance(ours, list) and len(original) == len(ours):
        result = []
        for index, (left, right) in enumerate(zip(original, ours)):
            result.extend(field_differences(left, right, f"{path}[{index}]"))
        return result
    return [{"path": path, "original": original, "ours": ours}]


def ordered_differences(original, ours, event):
    # Align only by method/phase, never sort records or discard repeated calls.
    def identity(row):
        return (row["method"], row["phase"]) if event else (row["method"],)
    matcher = difflib.SequenceMatcher(a=[identity(row) for row in original],
                                    b=[identity(row) for row in ours], autojunk=False)
    result = []
    for tag, i, j, k, end in matcher.get_opcodes():
        if tag == "equal":
            for left_index, right_index in zip(range(i, j), range(k, end)):
                fields = field_differences(original[left_index], ours[right_index])
                if fields:
                    result.append({"kind": "record_mismatch", "original_index": left_index,
                                   "ours_index": right_index, "fields": fields})
        else:
            result.append({"kind": tag, "original_range": [i, j], "ours_range": [k, end],
                           "original": original[i:j], "ours": ours[k:end]})
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("original", type=Path)
    parser.add_argument("ours", type=Path)
    parser.add_argument("--out", required=True, type=Path)
    args = parser.parse_args()
    report = {"schema_version": 1, "original": str(args.original.resolve()),
              "ours": str(args.ours.resolve()), "normalizations": NORMALIZATIONS,
              "errors": [], "coverage": {}, "differences": {}}
    runs = {}
    for name, directory in (("original", args.original), ("ours", args.ours)):
        try:
            runs[name] = load_run(directory)
            report["coverage"][name] = coverage(*runs[name])
            report["errors"].extend(f"{name}: {item}" for item in
                                    report["coverage"][name]["missing"])
        except (OSError, ValueError, TypeError) as error:
            report["errors"].append(f"{name}: {error}")
    if len(runs) == 2:
        report["differences"]["events"] = ordered_differences(
            normalize_events(runs["original"][0]), normalize_events(runs["ours"][0]), True)
        report["differences"]["abi"] = ordered_differences(
            normalize_abi(runs["original"][1]), normalize_abi(runs["ours"][1]), False)
    report["difference_counts"] = {key: len(value) for key, value in report["differences"].items()}
    report["equal"] = not report["errors"] and not any(report["difference_counts"].values())
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(report, ensure_ascii=False, indent=2, allow_nan=False) + "\n")
    print(f"{'MATCH' if report['equal'] else 'DIFFER'}: "
          f"events={report['difference_counts'].get('events', 0)} "
          f"ABI={report['difference_counts'].get('abi', 0)} errors={len(report['errors'])}; {args.out}")
    for error in report["errors"]:
        print(error, file=sys.stderr)
    return 2 if report["errors"] else (0 if report["equal"] else 1)


if __name__ == "__main__":
    sys.exit(main())
