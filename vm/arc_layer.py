"""Measures Arc's own layer in the ArcCore decompile (runbook §5.1).

Seeds: Arc* ObjC methods, the C exports, and functions whose pseudo-C mentions an arc/ source path or
arc.mojom. Then follows calls into anonymous FUN_ functions up to depth 2, not counting functions
that are named outside Arc (other ObjC classes, imports, stubs).
usage: arc_layer.py <functions.tsv> <decomp-dir> <out.tsv>
"""
import collections
import glob
import re
import sys

tsv_path, decomp_dir, out_path = sys.argv[1:4]
size = {}
for line in open(tsv_path):
    entry, _end, n, name = line.rstrip("\n").split("\t", 3)
    size[name] = int(n)

header = re.compile(r"^// ==== (0x[0-9a-f]+)  (.+?)  \((\d+) bytes\)$")
bodies: dict[str, str] = {}
for shard in sorted(glob.glob(f"{decomp_dir}/*.c")):
    name, buf = None, []
    for line in open(shard, errors="replace"):
        m = header.match(line.rstrip("\n"))
        if m:
            if name:
                bodies[name] = "".join(buf)
            name, buf = m.group(2).rstrip(":"), []
        elif name:
            buf.append(line)
    if name:
        bodies[name] = "".join(buf)

exports = {"_ChromeMain", "_ChromeAppModeStart_v8", "_ChromeWebAppShortcutCopierMain", "_SetLogMessageHandler",
           "_CallWithEHFrame", "_emitTraceEvent", "_arcCategoryEnabledFlag", "_ArcErrorDomain"}
seeds: dict[str, str] = {}
for name, body in bodies.items():
    if re.match(r"^Arc[A-Za-z0-9_]*::", name):
        seeds[name] = "arc-objc"
    elif name in exports:
        seeds[name] = "export"
    elif "../../arc/" in body or "arc.mojom" in body:
        seeds[name] = "arc-path"

call = re.compile(r"\b(FUN_[0-9a-f]{8})\s*\(")
depth = dict.fromkeys(seeds, 0)
frontier = list(seeds)
for d in (1, 2):
    nxt = []
    for f in frontier:
        for callee in set(call.findall(bodies.get(f, ""))):
            if callee not in depth:
                depth[callee] = d
                nxt.append(callee)
    frontier = nxt

with open(out_path, "w") as out:
    for name, d in sorted(depth.items(), key=lambda kv: (kv[1], kv[0])):
        out.write(f"{name}\t{d}\t{seeds.get(name, 'callee')}\t{size.get(name, 0)}\n")

by = collections.defaultdict(lambda: [0, 0])
for name, d in depth.items():
    key = f"depth {d}" + (f" ({seeds[name]})" if d == 0 else " (anonymous callees)")
    by[key][0] += 1
    by[key][1] += size.get(name, 0)
print(f"decompiled bodies parsed: {len(bodies)}")
for key in sorted(by):
    n, b = by[key]
    print(f"{key:34s} {n:7d} functions  {b / 1048576:7.2f} MB code")
total_n, total_b = len(depth), sum(size.get(n, 0) for n in depth)
print(f"{'total (seeds + depth<=2)':34s} {total_n:7d} functions  {total_b / 1048576:7.2f} MB code")
