"""Diff two ArcEngineRecorder transcripts (record.jsonl): Arc's engine (golden) vs ours.

Object names <Class#n> are renumbered per class by first appearance, so runs compare structurally.
Reports, per boundary method (dir, kind, class, selector):
  - missing: called in golden, never in ours (and the reverse, extra)
  - return mismatches: the i-th call's normalized return differs
  - argument mismatches: the i-th call's normalized arguments differ (engine->app callbacks, and the
    engine's own property updates)
and the ordered engine->app callback sequence (first divergence).

  python3 tests/recorder/diff.py golden/record.jsonl ours/record.jsonl [--json out.json] [--max 40]
"""
import collections
import json
import re
import sys

OBJ = re.compile(r"<([A-Za-z_][A-Za-z0-9_]*)#(\d+)>")


def load(path):
    events = []
    for line in open(path):
        try:
            events.append(json.loads(line))
        except ValueError:
            pass
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


def main():
    golden, ours = sys.argv[1], sys.argv[2]
    limit = int(sys.argv[sys.argv.index("--max") + 1]) if "--max" in sys.argv else 40
    g, gcb = load(golden)
    o, ocb = load(ours)
    report = {"missing": [], "extra": [], "return_mismatch": [], "arg_mismatch": [], "count_mismatch": []}
    for key in sorted(set(g) | set(o)):
        name = "%s %s%s %s" % (key[0], key[1], key[2], key[3])
        gc, oc = g.get(key, []), o.get(key, [])
        if gc and not oc:
            report["missing"].append({"method": name, "golden_calls": len(gc), "example": gc[0]})
            continue
        if oc and not gc:
            report["extra"].append({"method": name, "our_calls": len(oc), "example": oc[0]})
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
    total = sum(len(v) for k, v in report.items() if isinstance(v, list))
    print("methods differing: %d (missing %d, extra %d, returns %d, callback args %d, counts %d)" % (
        total, len(report["missing"]), len(report["extra"]), len(report["return_mismatch"]),
        len(report["arg_mismatch"]), len(report["count_mismatch"])))
    print("callback sequence:", json.dumps(report["callback_sequence"])[:600])
    for section in ("missing", "return_mismatch", "arg_mismatch", "extra"):
        for item in report[section][:limit]:
            print(section, json.dumps(item)[:300])
    if "--json" in sys.argv:
        json.dump(report, open(sys.argv[sys.argv.index("--json") + 1], "w"), indent=1)


if __name__ == "__main__":
    main()
