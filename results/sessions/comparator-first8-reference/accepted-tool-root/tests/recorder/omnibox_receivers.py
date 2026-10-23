"""Conservative, opt-in alignment of observed public omnibox DTO getters.

Raw transcripts are never changed. A rejection leaves the ordinary diff intact.
This observes app-consumed graphs; it does not infer completion blocks or done.
"""
import collections
import json
import re

OBJECT = re.compile(r"<([A-Za-z_][A-Za-z0-9_]*)#(\d+)>")
RESULT = "ArcAutocompleteResult"
MATCH = "ArcAutocompleteMatch"
DATA = "ArcAutocompleteMatchAdditionalData"
GETTERS = {
    RESULT: frozenset(("combinedMatches", "perProviderMatches")),
    MATCH: frozenset(("destinationUrl", "destinationUrlSpec", "imageUrl", "imageUrlSpec",
                      "relevance", "contents", "matchType", "providerType", "inlineAutocompleteText",
                      "description", "additionalData", "hasTabMatch", "allowedToBeDefaultMatch",
                      "deletable", "swapContentsAndDescription", "isFromPrevious", "isFromKeyword",
                      "keyword", "fillIntoEdit", "fillIntoEditAdditionalText", "transition",
                      "duplicates", "associatedKeyword")),
    DATA: frozenset(("key", "value")),
}
QUERY = "autocomplete:cursorPosition:preventInlineAutocomplete:startFresh:keyword:error:completion:"
PAIR_FIELDS = ("from", "thread", "depth", "dir", "kind", "cls", "sel", "self")


def selected(event):
    return (event.get("from") == "app" and event.get("dir") == "->engine"
            and event.get("kind") == "-" and event.get("sel") in GETTERS.get(event.get("cls"), ()))


def identity(value, cls):
    match = OBJECT.fullmatch(value) if isinstance(value, str) else None
    if not match or match.group(1) != cls:
        raise ValueError("omnibox graph contains an invalid raw receiver/member token")
    return value


def references(value):
    if isinstance(value, str):
        yield from (match.group(0) for match in OBJECT.finditer(value) if match.group(1) in GETTERS)
    elif isinstance(value, list):
        for member in value:
            yield from references(member)
    elif isinstance(value, dict):
        for key, member in value.items():
            yield from references(key)
            yield from references(member)


def graph(path):
    rows, stack, boundary, queries = [], [], [], []
    last_seq = 0
    seen_seq = set()
    with open(path) as source:
        for line in source:
            try:
                event = json.loads(line)
            except ValueError as error:
                raise ValueError("omnibox mode requires valid JSON on every row") from error
            if not isinstance(event, dict):
                raise ValueError("omnibox mode requires object rows")
            if event.get("ev") not in ("call", "ret"):
                continue
            boundary.append(event)
            caller = event.get("from")
            if not isinstance(caller, str) or not caller or caller.strip() != caller or any(ord(c) < 32 for c in caller):
                raise ValueError("omnibox mode requires strict v2 attribution on every call and return")
            seq = event.get("seq")
            if type(seq) is not int or seq <= 0:
                raise ValueError("omnibox mode requires positive integer sequence numbers")
            if seq in seen_seq:
                raise ValueError("omnibox mode found duplicate boundary sequence numbers")
            seen_seq.add(seq)
            if selected(event) and event.get("thread") != "main":
                raise ValueError("omnibox selected getter is not on main")
            is_query = (caller == "app" and event.get("dir") == "->engine"
                        and event.get("cls") == "ArcBrowser" and event.get("sel") == QUERY)
            if (caller == "app" and event.get("dir") == "->engine"
                    and event.get("sel", "").startswith("autocomplete:") and not is_query):
                raise ValueError("omnibox mode excludes other app autocomplete requests")
            if is_query and event.get("thread") != "main":
                raise ValueError("omnibox query is not on main")
            if event.get("thread") != "main":
                continue
            if seq <= last_seq:
                raise ValueError("omnibox mode requires increasing main-thread sequence numbers")
            last_seq = seq
            if (not all(field in event for field in PAIR_FIELDS) or type(event["depth"]) is not int
                    or event["depth"] < 0):
                raise ValueError("omnibox mode requires complete main-thread pairing identities")
            if event["ev"] == "call":
                if not isinstance(event.get("args"), list):
                    raise ValueError("omnibox main call lacks argument array")
                if stack and event["depth"] <= stack[-1]["call"]["depth"]:
                    raise ValueError("omnibox observed nested call depth does not increase")
                record = {"call": event}
                rows.append(record)
                stack.append(record)
                if is_query:
                    queries.append(record)
            else:
                if ("ret" not in event or not stack
                        or any(stack[-1]["call"][field] != event[field] for field in PAIR_FIELDS)):
                    raise ValueError("omnibox mode found unmatched/inconsistent main return")
                record = stack.pop()
                record["return"] = event
    root = stack[0]["call"] if len(stack) == 1 else None
    if stack:
        if (root is None or (root.get("from"), root.get("dir"), root.get("kind"),
                root.get("cls"), root.get("sel"), root.get("depth"), root.get("args"))
                != ("app", "->engine", "-", "ArcBrowserApplication", "run", 0, [])):
            raise ValueError("omnibox mode found an invalid/incomplete open run root")
        try:
            identity(root["self"], "ArcBrowserApplication")
        except ValueError as error:
            raise ValueError("omnibox open run root has an invalid receiver") from error
    if len(queries) != 1 or "return" not in queries[0]:
        raise ValueError("omnibox mode requires exactly one paired app browser query")
    query = queries[0]
    if root and root["seq"] >= query["call"]["seq"]:
        raise ValueError("omnibox open run root must precede the query")
    args = query["call"]["args"]
    if (len(args) != 7 or not isinstance(args[0], str) or type(args[1]) is not int
            or type(args[2]) is not bool or type(args[3]) is not bool):
        raise ValueError("omnibox query arguments have an unsupported shape")
    reads = [record for record in rows if selected(record["call"])]
    by_receiver = collections.defaultdict(lambda: collections.defaultdict(list))
    methods = collections.defaultdict(list)
    for record in reads:
        call = record["call"]
        if "return" not in record or call["args"] != []:
            raise ValueError("omnibox getter is incomplete or has nonempty arguments")
        obj = identity(call["self"], call["cls"])
        by_receiver[obj][call["sel"]].append(record)
        methods[(call["dir"], call["kind"], call["cls"], call["sel"])].append(record)
    result_ids = list(dict.fromkeys(record["call"]["self"] for record in reads
                                   if record["call"]["cls"] == RESULT))
    if not result_ids:
        raise ValueError("omnibox mode found no app-consumed result graph")
    if any(record["call"]["seq"] < query["call"]["seq"] for record in reads):
        raise ValueError("omnibox DTO reads precede the sole query")
    paths = {}

    def add(path_key, token, cls):
        paths[path_key] = identity(token, cls)

    def collection(obj, selector, required=False):
        observations = by_receiver[obj].get(selector, [])
        if required and not observations:
            raise ValueError("omnibox result graph lacks a public collection getter")
        if not observations:
            return None
        values = [row["return"]["ret"] for row in observations]
        if any(value != values[0] for value in values[1:]):
            raise ValueError("omnibox object has changing collection snapshots")
        return values[0]

    for ordinal, obj in enumerate(result_ids):
        add(("result", ordinal), obj, RESULT)
        combined = collection(obj, "combinedMatches", required=True)
        providers = collection(obj, "perProviderMatches", required=True)
        if (not isinstance(combined, list) or not isinstance(providers, dict)
                or any(not isinstance(key, str) or not isinstance(value, list) for key, value in providers.items())):
            raise ValueError("omnibox public result collection has an invalid shape")
        for index, member in enumerate(combined):
            add(("result", ordinal, "combined", index), member, MATCH)
        for key, members in sorted(providers.items()):
            for index, member in enumerate(members):
                add(("result", ordinal, "provider", key, index), member, MATCH)
    match_paths = [(key, token) for key, token in paths.items() if OBJECT.fullmatch(token).group(1) == MATCH]
    if not match_paths:
        raise ValueError("omnibox mode requires a nonempty observed member graph")
    for parent, token in match_paths:
        values = collection(token, "additionalData")
        if values is not None:
            if not isinstance(values, list):
                raise ValueError("omnibox additionalData getter did not return an array")
            for index, member in enumerate(values):
                add(parent + ("additionalData", index), member, DATA)
    aliases = collections.defaultdict(list)
    for anchor, token in paths.items():
        aliases[token].append(anchor)
    anchors = {token: min(members) for token, members in aliases.items()}
    for record in reads:
        if record["call"]["self"] not in anchors:
            raise ValueError("omnibox getter receiver is outside the observed graph")
        if any(token not in anchors for token in references(record["return"]["ret"])):
            raise ValueError("omnibox getter references an unanchored DTO object")
    for event in boundary:
        # Exact baseline app-mode admission. Excluded engine-internal
        # constructors may establish raw evidence but are not comparison rows.
        admitted = not (event.get("dir") == "->engine" and event.get("from") != "app"
                        and not (event.get("from") == "engine" and event.get("sel", "").startswith("set")))
        if admitted and not selected(event):
            values = (event.get("self"), event.get("args"), event.get("ret"))
            if any(token in anchors for value in values for token in references(value)):
                raise ValueError("omnibox anchored DTO crosses into an admitted nonselected record (cross-scope alias)")
    signature = sorted(tuple(sorted(members)) for members in aliases.values())
    counts = collections.Counter((anchors[row["call"]["self"]], row["call"]["cls"], row["call"]["sel"])
                                 for row in reads)
    preseed = {}
    per_class = collections.Counter()
    for token in sorted(anchors, key=anchors.get):
        cls, number = OBJECT.fullmatch(token).groups()
        per_class[cls] += 1
        preseed[(cls, number)] = per_class[cls]
    return {"paths": set(paths), "aliases": signature, "counts": counts, "anchors": anchors,
            "preseed": preseed, "methods": methods, "results": len(result_ids),
            "queryKey": ("->engine", "-", "ArcBrowser", QUERY)}


def align(golden, ours, g, o, load):
    """Return replacement maps only after both complete scopes pass every gate."""
    left, right = graph(golden), graph(ours)
    if left["paths"] != right["paths"]:
        raise ValueError("omnibox ordered graph shapes differ (including AdditionalData positions)")
    if left["aliases"] != right["aliases"]:
        raise ValueError("omnibox raw-object alias partitions differ")
    if left["counts"] != right["counts"]:
        raise ValueError("omnibox per-anchor getter counts differ")
    if g.get(left["queryKey"]) != o.get(right["queryKey"]):
        raise ValueError("omnibox sole query arguments/return differ under baseline normalization")
    replacements = []
    for path, baseline, scope in ((golden, g, left), (ours, o, right)):
        # Reuse the existing loader's strict raw-key epoch prepass and value
        # normalizer after raw graph reconstruction. Do not duplicate its rules.
        stable, _ = load(path, by_caller=True, object_ids=scope["preseed"])
        replacement = dict(baseline)
        for key, raw_calls in scope["methods"].items():
            values = stable.get(key, [])
            if len(values) != len(raw_calls) or len(baseline.get(key, [])) != len(raw_calls):
                raise ValueError("omnibox admitted getter counts do not match paired graph evidence")
            # Stable sort preserves each receiver's chronological repeated calls.
            indices = sorted(range(len(raw_calls)), key=lambda i: scope["anchors"][raw_calls[i]["call"]["self"]])
            replacement[key] = [values[index] for index in indices]
        if {key: len(value) for key, value in replacement.items()} != {key: len(value) for key, value in baseline.items()}:
            raise ValueError("omnibox alignment changed global method counts")
        replacements.append(replacement)
    return (*replacements, {"status": "aligned", "results": left["results"],
                            "getterCalls": sum(left["counts"].values()),
                            "callbackBlockIdentityAndDone": "unobserved",
                            "postReturnResultAssociation": "temporal only",
                            "pairingEvidence": "observed main records only; omitted ancestry not inferred"})
