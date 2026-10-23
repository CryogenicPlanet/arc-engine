"""Diff two ArcEngineRecorder transcripts (record.jsonl): Arc's engine (golden) vs ours.

Object names <Class#n> are renumbered per class by first appearance, so runs compare structurally.
Reports, per boundary method (dir, kind, class, selector):
  - missing: called in golden, never in ours (and the reverse, extra)
  - return mismatches: the i-th call's normalized return differs
  - argument mismatches: the i-th call's normalized arguments differ (engine->app callbacks, and the
    engine's own property updates)
and the ordered engine->app callback sequence (first divergence).

  python3 tests/recorder/diff.py golden/record.jsonl ours/record.jsonl [--json out.json] [--max 40] [--fixture-image BASENAME ...] [--omnibox-receivers] [--omnibox-google-query] [--compare-boundary-arguments]
  --compare-boundary-arguments independently compares normalized arguments of selected boundary methods;
  it requires caller attribution on every call/return and excludes existing whole-method <ptr> constructors.
"""
import argparse
import collections
from functools import partial
import json
import re
import sys

from download_identity import DownloadIdentityAliases

# Exact recorder identities only; these are not selector-prefix allowlists.
SESSION_IDENTIFIER = ("->engine", "-", "ArcSessionID", "identifier")
SESSION_GROUP_CALLBACK = (
    "engine->app", "-", "ADK.ChromiumBrowser",
    "browser:extensionGroupIdForTabWithSessionId:",
)

OBJ = re.compile(r"<([A-Za-z_][A-Za-z0-9_]*)#(\d+)>")


# Lead allowlist: class tag only, not a new object-identity namespace.
NATIVE_VIEW_RETURN = ("->engine", "-", "ArcWebContents", "nativeView", "ret")
NATIVE_VIEW_OBJECT = re.compile(r"<(WebContentsHostView|WebContentsViewCocoa)#([1-9][0-9]*)>")


def native_view_return_class(events, scope=None):
    """One approved class per admitted return lane; mixed classes fail closed."""
    classes = set()
    for event in events:
        if tuple(event.get(k) for k in ("dir", "kind", "cls", "sel", "ev")) != NATIVE_VIEW_RETURN:
            continue
        value = event.get("ret")
        match = NATIVE_VIEW_OBJECT.fullmatch(value) if type(value) is str else None
        if match:
            classes.add(match.group(1))
    if scope is not None:
        scope["mixed_approved_classes"] = len(classes) > 1
    return next(iter(classes)) if len(classes) == 1 else None


def normalize_native_view_return(event, value, normalized_value, approved_class):
    """Alias only the tag; preserve the ordinal already assigned by norm().

    The non-JSON marker cannot be impersonated by a malformed literal string.
    This establishes neither WebContents-to-view ownership nor physical hosting.
    """
    if tuple(event.get(k) for k in ("dir", "kind", "cls", "sel", "ev")) != NATIVE_VIEW_RETURN:
        return normalized_value
    match = NATIVE_VIEW_OBJECT.fullmatch(value) if type(value) is str else None
    if not match or match.group(1) != approved_class:
        return normalized_value
    normalized = NATIVE_VIEW_OBJECT.fullmatch(json.loads(normalized_value))
    if not normalized:
        return normalized_value
    return f"@typed:nativeView:anNSView#{normalized.group(2)}@"


def favicon_header_args(event, args):
    # Exact recorded callback identity; other URL clients retain strict comparison.
    if ((event.get("ev"), event.get("dir"), event.get("kind"),
         event.get("cls"), event.get("sel")) != (
            "call", "engine->app", "-",
            "_TtCC15URLLoaderClient19URLLoaderClientImpl12DataDelegate",
            "didReceiveHeaders:withFinalURL:withCode:withHeaders:")
            or not isinstance(args, list) or len(args) != 4
            or not isinstance(args[1], str) or not isinstance(args[3], dict)
            or not re.match(r"^https://t[0-9]+\.gstatic\.com/faviconV2\?", args[1])):
        return args
    args = list(args)
    args[1] = re.sub(r"^https://t[0-9]+\.gstatic\.com/",
                     "https://t0.gstatic.com/", args[1])
    headers = args[3] = dict(args[3])
    age = headers.get("age")
    if isinstance(age, str) and re.fullmatch(r"[0-9]+", age):
        headers["age"] = "0"
    location = headers.get("content-location")
    if isinstance(location, str):
        headers["content-location"] = re.sub(
            r"^https://www\.youtube\.com/s/desktop/[0-9a-f]{8}/img/favicon_144x144\.png\Z",
            "https://www.youtube.com/s/desktop/00000000/img/favicon_144x144.png",
            location)
    # Canonical values themselves satisfy the accepted field grammar. Invalid
    # values cannot impersonate a wildcard marker, and all other fields survive.
    return args


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


LAST_VISIT_CLASS = "ArcAutocompleteMatchAdditionalData"
LAST_VISIT_KEY = "last visit since epoch"
# Opaque comparison text, deliberately outside JSON: a malformed literal string
# equal to this marker still normalizes to a quoted JSON string and differs.
LAST_VISIT_MARKER = "@volatile:last-visit-epoch@"
POSITIVE_EPOCH_STRING = re.compile(r"[1-9][0-9]*\Z")


def last_visit_epoch_objects(events):
    """Resolve exact object keys from raw evidence, without changing events.

    Read before caller filtering: v2 may omit internal initializers from the
    comparison, while their key evidence remains useful. A paired public -key
    return also establishes identity when no initializer was recorded. Any
    conflicting or malformed key evidence disables normalization for that object.
    """
    keys = collections.defaultdict(set)
    ambiguous = set()
    pending_keys = collections.Counter()
    for event in events:
        if (event.get("dir"), event.get("kind"), event.get("cls")) != (
                "->engine", "-", LAST_VISIT_CLASS):
            continue
        identity = event.get("self")
        match = OBJ.fullmatch(identity) if isinstance(identity, str) else None
        if not match or match.group(1) != LAST_VISIT_CLASS:
            continue
        selector, phase = event.get("sel"), event.get("ev")
        if selector == "init:value:" and phase == "call":
            args = event.get("args")
            if isinstance(args, list) and len(args) == 2 and isinstance(args[0], str):
                keys[identity].add(args[0])
            else:
                ambiguous.add(identity)
        elif selector == "key":
            thread = event.get("thread")
            if thread is not None and not isinstance(thread, str):
                ambiguous.add(identity)
                continue
            pending = (identity, thread)
            if phase == "call":
                pending_keys[pending] += 1
            elif phase == "ret" and pending_keys[pending]:
                pending_keys[pending] -= 1
                key = event.get("ret")
                if isinstance(key, str):
                    keys[identity].add(key)
                else:
                    ambiguous.add(identity)
    return {identity for identity, observed in keys.items()
            if identity not in ambiguous and observed == {LAST_VISIT_KEY}}


def is_last_visit_epoch(event, identities, phase, selector, value):
    return ((event.get("dir"), event.get("kind"), event.get("cls"),
             event.get("ev"), event.get("sel")) ==
            ("->engine", "-", LAST_VISIT_CLASS, phase, selector)
            and isinstance(event.get("self"), str) and event.get("self") in identities
            and isinstance(value, str) and POSITIVE_EPOCH_STRING.fullmatch(value))


def load(path, by_caller=False, fixture_images=(), fixture_counts=None, object_ids=None, google_q=False, google_q_status=None, require_callers=False, native_view_scope=None):
    events = []
    raw_events = []
    boundary_rows = 0
    attributed_calls = 0
    validate_callers = bool(fixture_images) or require_callers
    fixture_stacks = collections.defaultdict(list)
    if fixture_counts is None:
        fixture_counts = {image: {"call": 0, "ret": 0} for image in fixture_images}
    for line in open(path):
        try:
            e = json.loads(line)
        except ValueError:
            if validate_callers:
                scope = "attributed" if require_callers else "fixture"
                raise ValueError(f"{path}: malformed JSON in {scope} transcript")
            if google_q:
                raise ValueError(f"{path}: malformed JSON in Google query transcript")
            continue
        if validate_callers:
            if not isinstance(e, dict):
                scope = "attributed" if require_callers else "fixture"
                raise ValueError(f"{path}: {scope} transcript row is not an object")
            if e.get("ev") in ("call", "ret"):
                boundary_rows += 1
                attributed_calls += e["ev"] == "call"
                if not isinstance(e.get("from"), str) or not e["from"] or e["from"].strip() != e["from"] or any(ord(c) < 32 for c in e["from"]):
                    mode = "boundary-argument mode" if require_callers else "fixture mode"
                    raise ValueError(f"{path}: {mode} requires caller attribution on every call and return")
        if google_q and not isinstance(e, dict):
            raise ValueError(f"{path}: Google query mode requires object rows")
        raw_events.append(e)
    # Establish exact raw receiver/key evidence before any caller admission.
    # This prepass never changes raw events, fixture stacks, or fixture counts.
    epoch_objects = last_visit_epoch_objects(raw_events)
    # Build eligibility from ALL original event objects, before caller admission
    # or comparison-value transforms. This never edits an event or raw input.
    google_return = lambda event: event.get("ret")
    if google_q:
        from google_q import prepare_google_q_normalizer
        try:
            google_return = prepare_google_q_normalizer(raw_events, enabled=True)
        except ValueError as error:
            if google_q_status is not None:
                google_q_status.update(status="rejected", reason=str(error), valuesChanged=0)
        else:
            if google_q_status is not None:
                google_q_status.update(status="eligible", valuesChanged=0)
    for e in raw_events:
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
    if require_callers and not attributed_calls:
        raise ValueError(f"{path}: boundary-argument mode requires at least one attributed call per transcript")
    if fixture_images and (any(fixture_stacks.values()) or not sum(c["call"] for c in fixture_counts.values())):
        raise ValueError(f"{path}: fixture mode requires at least one complete admitted fixture call/return pair and no incomplete admitted calls")
    if fixture_images and not boundary_rows:
        raise ValueError(f"{path}: fixture transcript contains no attributed calls or returns")
    view_class = native_view_return_class(events, native_view_scope)
    if native_view_scope is not None and native_view_scope.get("enabled") is False:
        view_class = None
    aliases = DownloadIdentityAliases(raw_events, events)
    ids = dict(object_ids or {})
    per_class = collections.Counter()
    for (cls, _), number in ids.items():
        per_class[cls] = max(per_class[cls], number)
    session_ids = {}

    def session_id(value):
        # SessionID::id_type is int32_t. bool is an int subclass in Python,
        # so require the exact JSON integer type and preserve invalid values.
        if type(value) is int and 0 < value <= 0x7fffffff:
            if value not in session_ids:
                session_ids[value] = len(session_ids) + 1
            return {"native_session_id": session_ids[value]}
        # Wrap literals too: a malformed dict/string must never impersonate an
        # alias. These values bypass the existing string volatility masks.
        return {"native_session_id_literal": value}


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
            args = favicon_header_args(e, aliases.rewrite_field(e, "args", e.get("args")))
            native_session_id = None
            if key == SESSION_GROUP_CALLBACK and isinstance(args, list) and len(args) > 1:
                args = list(args)
                native_session_id = session_id(args[1])
                args[1] = None
            if (isinstance(args, list) and len(args) == 2 and args[0] == LAST_VISIT_KEY
                    and is_last_visit_epoch(e, epoch_objects, "call", "init:value:", args[1])):
                normalized_args = LAST_VISIT_MARKER + " " + norm([args[0], None])
            else:
                normalized_args = norm(args)
            normalized_args = volatile.sub("*", normalized_args)
            if native_session_id is not None:
                normalized_args = json.loads(normalized_args)
                normalized_args[1] = native_session_id
                normalized_args = json.dumps(normalized_args, sort_keys=True)
            calls[key].append({"args": normalized_args, "ret": None})
            if e["dir"] == "engine->app":
                callbacks.append("%s %s" % (e["cls"].split("C3ADK")[-1], e["sel"]))
        else:
            for c in reversed(calls[key]):
                if c["ret"] is None:
                    value = google_return(e)
                    if google_q_status is not None and value != e.get("ret"):
                        google_q_status["valuesChanged"] += 1
                    value = aliases.rewrite_field(e, "ret", value)
                    if key == SESSION_IDENTIFIER:
                        c["ret"] = json.dumps(session_id(value), sort_keys=True)
                    else:
                        normalized_ret = (LAST_VISIT_MARKER
                                          if is_last_visit_epoch(e, epoch_objects, "ret", "value", value)
                                          else norm(value))
                        normalized_ret = normalize_native_view_return(e, value, normalized_ret, view_class)
                        c["ret"] = volatile.sub("*", normalized_ret)
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
    parser.add_argument("--omnibox-receivers", action="store_true",
                        help="conservatively align paired v2 app omnibox DTO getters by observed receiver graph; rejects unsafe scopes")
    # Preserve the existing error-usage bytes when the new mode is absent.
    # --help still lists the new option in its option descriptions.
    parser.usage = parser.format_usage().removeprefix("usage: ").rstrip("\n")
    parser.add_argument("--omnibox-google-query", action="store_true",
                        help="normalize only canonical Google q encoding of the sole paired v2 app fixture query port; rejects unsafe scopes")
    parser.add_argument("--compare-boundary-arguments", action="store_true",
                        help="independently compare arguments of selected metadata boundary methods and callbacks; requires complete caller labels")
    args = parser.parse_args()
    golden, ours, limit = args.golden, args.ours, args.limit
    fixture_images = frozenset(args.fixture_image)
    by_caller = args.compare_boundary_arguments or bool(fixture_images) or (has_callers(golden) and has_callers(ours))
    fixture_counts = {name: {image: {"call": 0, "ret": 0} for image in sorted(fixture_images)} for name in ("golden", "ours")}
    google_scope = None
    google_status = {name: {} for name in ("golden", "ours")} if args.omnibox_google_query else {}
    effective_google_q = args.omnibox_google_query
    native_view_scope = {name: {} for name in ("golden", "ours")}
    effective_native_view_tags = True
    try:
        g, gcb = load(golden, by_caller, fixture_images, fixture_counts["golden"],
                      google_q=effective_google_q, google_q_status=google_status.get("golden"), require_callers=args.compare_boundary_arguments,
                      native_view_scope=native_view_scope["golden"])
        o, ocb = load(ours, by_caller, fixture_images, fixture_counts["ours"],
                      google_q=effective_google_q, google_q_status=google_status.get("ours"), require_callers=args.compare_boundary_arguments,
                      native_view_scope=native_view_scope["ours"])
        if args.omnibox_google_query:
            rejected = any(scope.get("status") == "rejected" for scope in google_status.values())
            if rejected:
                # Pair transaction: invalid scope on EITHER side applies no
                # Google value changes on EITHER side. Retain the baseline.
                effective_google_q = False
                fixture_counts = {name: {image: {"call": 0, "ret": 0} for image in sorted(fixture_images)} for name in ("golden", "ours")}
                g, gcb = load(golden, by_caller, fixture_images, fixture_counts["golden"], require_callers=args.compare_boundary_arguments,
                              native_view_scope=native_view_scope["golden"])
                o, ocb = load(ours, by_caller, fixture_images, fixture_counts["ours"], require_callers=args.compare_boundary_arguments,
                              native_view_scope=native_view_scope["ours"])
                for scope in google_status.values():
                    scope["valuesChanged"] = 0
            google_scope = {"status": "rejected" if rejected else "applied",
                            "transcripts": google_status, "baselineDiffRetained": rejected}
        if any(scope.get("mixed_approved_classes") for scope in native_view_scope.values()):
            # Pair fail-closed: retain BOTH baseline lanes, including any earlier
            # equal return. Reloading must not create/move its first mismatch.
            effective_native_view_tags = False
            fixture_counts = {name: {image: {"call": 0, "ret": 0} for image in sorted(fixture_images)} for name in ("golden", "ours")}
            g, gcb = load(golden, by_caller, fixture_images, fixture_counts["golden"],
                          google_q=effective_google_q, google_q_status=google_status.get("golden"),
                          require_callers=args.compare_boundary_arguments, native_view_scope={"enabled": False})
            o, ocb = load(ours, by_caller, fixture_images, fixture_counts["ours"],
                          google_q=effective_google_q, google_q_status=google_status.get("ours"),
                          require_callers=args.compare_boundary_arguments, native_view_scope={"enabled": False})
    except ValueError as error:
        parser.error(str(error))
    receiver_scope = None
    if args.omnibox_receivers:
        from omnibox_receivers import align
        try:
            if not by_caller or fixture_images:
                raise ValueError("omnibox receiver mode requires v2 app attribution and no fixture-image admission")
            g, o, receiver_scope = align(golden, ours, g, o, partial(load, google_q=effective_google_q, require_callers=args.compare_boundary_arguments,
                                                         native_view_scope={"enabled": effective_native_view_tags}))
        except ValueError as error:
            receiver_scope = {"status": "rejected", "reason": str(error), "baselineDiffRetained": True}
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
            if not args.compare_boundary_arguments and a["args"] != b["args"] and key[0] == "engine->app":
                report["arg_mismatch"].append({"method": name, "call": i, "golden": a["args"], "ours": b["args"]})
                break
        if args.compare_boundary_arguments and (
                key[0] == "engine->app" or (
                    (key[1], key[2], key[3]) in boundary and
                    not all('"<ptr>"' in c["args"] for c in gc + oc))):
            # Existing callbacks stay eligible even when an ordinary argument contains literal "<ptr>".
            # Apply the native-constructor rule only to the additional calls into the engine.
            # Argument and return evidence remain independent in opt-in mode.
            for i, (a, b) in enumerate(zip(gc, oc)):
                if a["args"] != b["args"]:
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
    if receiver_scope is not None:
        report["omnibox_receivers"] = receiver_scope
    if google_scope is not None:
        report["omnibox_google_query"] = google_scope
    if args.compare_boundary_arguments:
        report["boundary_arguments_comparison"] = {"mode": "selected-boundary-methods", "caller_attribution": True}
    total = sum(len(v) for k, v in report.items() if isinstance(v, list) and k not in ("internal", "internal_ours"))
    print("methods differing: %d (missing %d, extra %d, returns %d, callback args %d, counts %d; internal-only %d, ours-internal %d)" % (
        total, len(report["missing"]), len(report["extra"]), len(report["return_mismatch"]),
        len(report["arg_mismatch"]), len(report["count_mismatch"]), len(report["internal"]), len(report["internal_ours"])))
    print("callback sequence:", json.dumps(report["callback_sequence"])[:600])
    print("caller attribution:", "on (app calls + engine setters)" if by_caller else "off (a transcript predates it)")
    for section in ("missing", "return_mismatch", "arg_mismatch", "extra"):
        for item in report[section][:limit]:
            print(section, json.dumps(item)[:300])
    if receiver_scope is not None:
        print("omnibox receivers:", json.dumps(receiver_scope, sort_keys=True))
    if google_scope is not None:
        print("omnibox Google query:", json.dumps(google_scope, sort_keys=True))
    if args.json_path:
        json.dump(report, open(args.json_path, "w"), indent=1)
    if google_scope is not None and google_scope["status"] == "rejected":
        raise SystemExit(2)
    if receiver_scope is not None or google_scope is not None:
        if receiver_scope is not None and receiver_scope["status"] == "rejected":
            raise SystemExit(2)
        if total or first_div is not None:
            raise SystemExit(1)


if __name__ == "__main__":
    main()
