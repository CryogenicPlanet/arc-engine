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


def has_callers(path):
    for line in open(path):
        if '"from":' in line:
            return True
    return False


def load(path, by_caller=False):
    events = []
    for line in open(path):
        try:
            e = json.loads(line)
        except ValueError:
            continue
        # With caller attribution on both sides, compare what the app sees: its own calls into the engine,
        # engine->app callbacks, and the engine's own setters (state the app observes with KVO). The engine's
        # internal getter reads are implementation detail.
        if by_caller and e.get("dir") == "->engine" and e.get("from") != "app" and not (
                e.get("from") == "engine" and e.get("sel", "").startswith("set")):
            continue
        events.append(e)
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
    golden, ours = sys.argv[1], sys.argv[2]
    limit = int(sys.argv[sys.argv.index("--max") + 1]) if "--max" in sys.argv else 40
    by_caller = has_callers(golden) and has_callers(ours)
    g, gcb = load(golden, by_caller)
    o, ocb = load(ours, by_caller)
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
    total = sum(len(v) for k, v in report.items() if isinstance(v, list) and k not in ("internal", "internal_ours"))
    print("methods differing: %d (missing %d, extra %d, returns %d, callback args %d, counts %d; internal-only %d, ours-internal %d)" % (
        total, len(report["missing"]), len(report["extra"]), len(report["return_mismatch"]),
        len(report["arg_mismatch"]), len(report["count_mismatch"]), len(report["internal"]), len(report["internal_ours"])))
    print("callback sequence:", json.dumps(report["callback_sequence"])[:600])
    print("caller attribution:", "on (app calls + engine setters)" if by_caller else "off (a transcript predates it)")
    for section in ("missing", "return_mismatch", "arg_mismatch", "extra"):
        for item in report[section][:limit]:
            print(section, json.dumps(item)[:300])
    if "--json" in sys.argv:
        json.dump(report, open(sys.argv[sys.argv.index("--json") + 1], "w"), indent=1)


if __name__ == "__main__":
    main()
