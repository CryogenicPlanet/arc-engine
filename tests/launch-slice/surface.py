"""Join traced first entries (results/launch-slice/<run>/launch-*/trace.tsv) with specs/methods.tsv and
the app callback joins. Writes <run>/launch-surface.tsv. An entry is a lower bound: it says the IMP was
entered in this scenario, not that the method is required or that unentered ones are unused.

  python3 tests/launch-slice/surface.py results/launch-slice/<run>
"""
import csv, glob, os, sys

run = sys.argv[1]
root = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
methods = list(csv.DictReader(open(os.path.join(root, "specs/methods.tsv")), delimiter="\t"))
launches = sorted(glob.glob(os.path.join(run, "launch-*")))
hits = {}  # (module, va) -> {launch: (rank, selector, match, main)}
for l in launches:
    for row in csv.DictReader(open(os.path.join(l, "trace.tsv")), delimiter="\t"):
        if row["rank"] == "#":
            continue
        hits.setdefault((row["module"], int(row["file_va"], 16)), {})[os.path.basename(l)] = (
            row["rank"], row["x1_selector"], row["selector_match"], row["main_thread"])

names = [os.path.basename(l) for l in launches]
out = open(os.path.join(run, "launch-surface.tsv"), "w")
cols = ["class_name", "category", "kind", "selector", "imp_va", "shared_imp_rows"] + \
       ["%s_rank" % n for n in names] + ["x1_selector", "attribution", "main_thread",
       "source_writer", "confidence", "semantic_status", "unresolved_question"]
out.write("\t".join(cols) + "\n")
by_imp = {}
for m in methods:
    by_imp.setdefault(int(m["imp_va"], 16), []).append(m)
entered = 0
for m in methods:
    va = int(m["imp_va"], 16)
    h = hits.get(("ArcCore", va))
    if not h:
        continue
    entered += 1
    sels = {x[1] for x in h.values()}
    shared = len(by_imp[va])
    if any(x[2] == "yes" and x[1] == m["selector"] for x in h.values()):
        attribution = "selector_confirmed" if shared == 1 else "selector_matches_shared_imp"
    else:
        attribution = "imp_only"
    out.write("\t".join([m["class_name"], m["category"], m["kind"], m["selector"], m["imp_va"], str(shared)] +
        [h[n][0] if n in h else "" for n in names] +
        [",".join(sorted(s for s in sels if s)), attribution, ",".join(sorted({x[3] for x in h.values()})),
         m["source_writer"], m["confidence"], m["semantic_status"], m["unresolved_question"].replace("\t", " ")]) + "\n")
app_hits = sum(1 for k in hits if k[0] == "Arc")
eng_imps = sum(1 for k in hits if k[0] == "ArcCore")
classes = {m["class_name"] for m in methods if ("ArcCore", int(m["imp_va"], 16)) in hits}
print("engine IMPs entered: %d of 1566; declaration rows on them: %d of 1964; classes: %d of 146; app callback IMPs entered: %d of 270"
      % (eng_imps, entered, len(classes), app_hits))
