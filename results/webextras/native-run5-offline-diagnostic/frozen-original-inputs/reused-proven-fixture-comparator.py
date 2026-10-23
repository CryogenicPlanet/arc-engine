"""Diff two ArcEngineRecorder transcripts (record.jsonl): Arc's engine (golden) vs ours.

Object names <Class#n> are renumbered per class by first appearance, so runs compare structurally.
Reports, per boundary method (dir, kind, class, selector):
  - missing: called in golden, never in ours (and the reverse, extra)
  - return mismatches: the i-th call's normalized return differs
  - argument mismatches: the i-th call's normalized arguments differ (engine->app callbacks, and the
    engine's own property updates)
and the ordered engine->app callback sequence (first divergence).

  python3 tests/recorder/diff.py golden/record.jsonl ours/record.jsonl [--json out.json] [--max 40] [--fixture-image BASENAME ...]
"""
import argparse
import collections
import json
import re
import sys

OBJ = re.compile(r"<([A-Za-z_][A-Za-z0-9_]*)#(\d+)>")


def has_callers(path):
    for line in open(path):
        if '"from":' in line:
            return True
    return False


def fixture_image(value):
    # This is a literal recorder image basename, never a path, glob or caller alias.
    if not re.fullmatch(r"[A-Za-z0-9_][A-Za-z0-9_.+\-]*", value) or value in ("app", "engine"):
        raise argparse.ArgumentTypeError("fixture image must be a literal image basename, not a path, glob or caller alias")
    return value


def load(path, by_caller=False, fixture_images=(), fixture_counts=None):
    events = []
    boundary_rows = 0
    fixture_stacks = collections.defaultdict(list)
    if fixture_counts is None:
        fixture_counts = {image: {"call": 0, "ret": 0} for image in fixture_images}
    for line in open(path):
        try:
            e = json.loads(line)
        except ValueError:
            if fixture_images:
                raise ValueError(f"{path}: malformed JSON in fixture transcript")
            continue
        if fixture_images:
            if not isinstance(e, dict):
                raise ValueError(f"{path}: fixture transcript row is not an object")
            if e.get("ev") in ("call", "ret"):
                boundary_rows += 1
                if not isinstance(e.get("from"), str) or not e["from"] or e["from"].strip() != e["from"] or any(ord(c) < 32 for c in e["from"]):
                    raise ValueError(f"{path}: fixture mode requires caller attribution on every call and return")
        # With caller attribution on both sides, compare what the app sees: its own calls into the engine,
        # engine->app callbacks, and the engine's own setters (state the app observes with KVO). The engine's
        # internal getter reads are implementation detail.
        if by_caller and e.get("dir") == "->engine" and e.get("from") != "app" and e.get("from") not in fixture_images and not (
                e.get("from") == "engine" and e.get("sel", "").startswith("set")):
            continue
        events.append(e)
        if fixture_counts is not None and e.get("dir") == "->engine" and e.get("from") in fixture_images and e.get("ev") in ("call", "ret"):
            fixture_counts[e["from"]][e["ev"]] += 1
            fields = ("from", "thread", "depth", "dir", "kind", "cls", "sel", "self")
            if not all(field in e for field in fields) or not isinstance(e["thread"], str):
                raise ValueError(f"{path}: incomplete admitted fixture identity")
            stack = fixture_stacks[e["thread"]]
            if e["ev"] == "call":
                if not isinstance(e.get("args"), list):
                    raise ValueError(f"{path}: admitted fixture call has no argument array")
                stack.append(e)
            else:
                if "ret" not in e:
                    raise ValueError(f"{path}: admitted fixture return has no return value")
                if not stack or any(stack[-1][field] != e[field] for field in fields):
                    raise ValueError(f"{path}: unmatched or inconsistent admitted fixture return")
                stack.pop()
    if fixture_images and (any(fixture_stacks.values()) or not sum(c["call"] for c in fixture_counts.values())):
        raise ValueError(f"{path}: fixture mode requires at least one complete admitted fixture call/return pair and no incomplete admitted calls")
    if fixture_images and not boundary_rows:
        raise ValueError(f"{path}: fixture transcript contains no attributed calls or returns")
    ids = {}
    per_class = collections.Counter()

    def norm(s):
        def sub(m):
            key = (m.group(1), m.group(2))
            if key not in ids:
                per_class[m.group(1)] += 1
                ids[key] = per_class[m.group(1)]
            return "<%s#%d>" % (m.group(1), ids[key])
        return OBJ.sub(sub, json.dumps(s, sort_keys=True))

    # Port numbers and temp paths differ run to run.
    volatile = re.compile(r"127\.0\.0\.1:\d+|/var/folders/[^\"]*|[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}")
    calls = collections.defaultdict(list)
    callbacks = []
    for e in events:
        if e.get("ev") not in ("call", "ret"):
            continue
        key = (e["dir"], e["kind"], e["cls"], e["sel"])
        if e["ev"] == "call":
            calls[key].append({"args": volatile.sub("*", norm(e.get("args"))), "ret": None})
            if e["dir"] == "engine->app":
                callbacks.append("%s %s" % (e["cls"].split("C3ADK")[-1], e["sel"]))
        else:
            for c in reversed(calls[key]):
                if c["ret"] is None:
                    c["ret"] = volatile.sub("*", norm(e.get("ret")))
                    break
    return calls, callbacks


def boundary_methods():
    """(kind, class, selector) of Arc's interface: ArcCore 1.170.0 metadata plus the app's callback methods."""
    import os
    root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    keys = set()
    for line in open(os.path.join(root, "arc/tools/arccore-1.170.0-objc.tsv")):
        r = line.rstrip("\n").split("\t")
        if r[0] == "M":
            keys.add((r[2], r[1], r[3]))
    for line in open(os.path.join(root, "tests/recorder/app_callbacks.inc")):
        m = re.match(r'\{"([^"]+)", "([^"]+)"\}', line.strip())
        if m:
            keys.add(("-", m.group(1), m.group(2)))
    return keys


def main():
    parser = argparse.ArgumentParser(description=__doc__, allow_abbrev=False)
    parser.add_argument("golden")
    parser.add_argument("ours")
    parser.add_argument("--max", type=int, default=40, dest="limit")
    parser.add_argument("--json", dest="json_path")
    parser.add_argument("--fixture-image", action="append", type=fixture_image, default=[], metavar="BASENAME",
                        help="also admit calls from this exact recorder image basename (repeatable; requires caller fields on both transcripts)")
    args = parser.parse_args()
    golden, ours, limit = args.golden, args.ours, args.limit
    fixture_images = frozenset(args.fixture_image)
    by_caller = bool(fixture_images) or (has_callers(golden) and has_callers(ours))
    fixture_counts = {name: {image: {"call": 0, "ret": 0} for image in sorted(fixture_images)} for name in ("golden", "ours")}
    try:
        g, gcb = load(golden, by_caller, fixture_images, fixture_counts["golden"])
        o, ocb = load(ours, by_caller, fixture_images, fixture_counts["ours"])
    except ValueError as error:
        parser.error(str(error))
    boundary = boundary_methods()
    report = {"internal": [], "internal_ours": [], "missing": [], "extra": [], "return_mismatch": [], "arg_mismatch": [], "count_mismatch": []}
    for key in sorted(set(g) | set(o)):
        name = "%s %s%s %s" % (key[0], key[1], key[2], key[3])
        gc, oc = g.get(key, []), o.get(key, [])
        if gc and not oc:
            # Arc's engine-internal constructors take a native pointer ("<ptr>"); ours builds wrappers its own way.
            section = "internal" if all('"<ptr>"' in c["args"] for c in gc) else "missing"
            report[section].append({"method": name, "golden_calls": len(gc), "example": gc[0]})
            continue
        if oc and not gc:
            # Selectors outside Arc's interface are our engine's own helpers, not boundary behavior.
            cls = key[2].replace("ADK.", "_TtC3ADK") if key[0] == "engine->app" else key[2]
            in_boundary = (key[1], key[2], key[3]) in boundary or (key[0] == "engine->app")
            report["extra" if in_boundary else "internal_ours"].append({"method": name, "our_calls": len(oc), "example": oc[0]})
            continue
        if len(gc) != len(oc):
            report["count_mismatch"].append({"method": name, "golden": len(gc), "ours": len(oc)})
        for i, (a, b) in enumerate(zip(gc, oc)):
            if a["ret"] != b["ret"]:
                report["return_mismatch"].append({"method": name, "call": i, "args": a["args"], "golden": a["ret"], "ours": b["ret"]})
                break
            if a["args"] != b["args"] and key[0] == "engine->app":
                report["arg_mismatch"].append({"method": name, "call": i, "golden": a["args"], "ours": b["args"]})
                break
    first_div = next((i for i, (a, b) in enumerate(zip(gcb, ocb)) if a != b), None)
    if first_div is None and len(gcb) != len(ocb):
        first_div = min(len(gcb), len(ocb))
    report["callback_sequence"] = {
        "golden": len(gcb), "ours": len(ocb), "first_divergence": first_div,
        "golden_from": gcb[first_div:first_div + 5] if first_div is not None else [],
        "ours_from": ocb[first_div:first_div + 5] if first_div is not None else [],
    }
    if fixture_images:
        report["fixture_comparison"] = {"mode": "exact-image", "admitted_images": sorted(fixture_images),
                                        "retained_fixture_events": fixture_counts}
    total = sum(len(v) for k, v in report.items() if isinstance(v, list) and k not in ("internal", "internal_ours"))
    print("methods differing: %d (missing %d, extra %d, returns %d, callback args %d, counts %d; internal-only %d, ours-internal %d)" % (
        total, len(report["missing"]), len(report["extra"]), len(report["return_mismatch"]),
        len(report["arg_mismatch"]), len(report["count_mismatch"]), len(report["internal"]), len(report["internal_ours"])))
    print("callback sequence:", json.dumps(report["callback_sequence"])[:600])
    print("caller attribution:", "on (app calls + engine setters)" if by_caller else "off (a transcript predates it)")
    for section in ("missing", "return_mismatch", "arg_mismatch", "extra"):
        for item in report[section][:limit]:
            print(section, json.dumps(item)[:300])
    if args.json_path:
        json.dump(report, open(args.json_path, "w"), indent=1)


if __name__ == "__main__":
    main()
