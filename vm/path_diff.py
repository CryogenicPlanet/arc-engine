"""Classify source paths compiled into ArcCore against stock Chromium's tree for the same version.

usage: path_diff.py <engine-strings.txt> <upstream-files.txt> <DEPS> <out.md>
"""
import collections
import re
import sys

strings_path, upstream_path, deps_path, out_path = sys.argv[1:5]
paths = sorted({m.group(1) for m in re.finditer(r"\.\./\.\./([A-Za-z0-9_./+-]+\.(?:cc|mm|h|m|cpp|c))", open(strings_path, errors="replace").read())})
upstream = set(open(upstream_path).read().split("\n"))
deps = sorted({m.group(1).rstrip("/") for m in re.finditer(r"['\"]src/([^'\"]+)['\"]\s*:", open(deps_path).read())}, key=len, reverse=True)

def dep_of(path: str) -> str | None:
    return next((d for d in deps if path == d or path.startswith(d + "/")), None)

groups: dict[str, list[str]] = collections.defaultdict(list)
for p in paths:
    if p.startswith("arc/"):
        groups["arc"].append(p)
    elif p in upstream:
        groups["upstream"].append(p)
    elif (d := dep_of(p)) is not None:
        groups["dep:" + d].append(p)
    elif p.startswith("out/") or p.startswith("gen/"):
        groups["generated"].append(p)
    else:
        groups["added"].append(p)

dep_total = sum(len(v) for k, v in groups.items() if k.startswith("dep:"))
lines = [
    "# ArcCore 1.170.0 vs Chromium 156.0.8078.48: compiled-in source paths",
    "",
    f"- Paths found in the engine: {len(paths)} (only files that log or assert are visible)",
    f"- Browser Company `arc/` tree: {len(groups['arc'])}",
    f"- Present in stock Chromium src (unmodified or modified, unknown): {len(groups['upstream'])}",
    f"- From Chromium DEPS repos (v8, skia, ...; checked separately): {dep_total}",
    f"- Generated sources: {len(groups['generated'])}",
    f"- **Inside Chromium's own directories but absent from stock Chromium (added by the Browser Company): {len(groups['added'])}**",
    "",
    "## Added inside Chromium directories",
    "",
    *[f"- `{p}`" for p in groups["added"]],
    "",
    "## `arc/` tree by directory",
    "",
    *[f"- {n:4d} `{d}`" for d, n in collections.Counter("/".join(p.split("/")[:3]) for p in groups["arc"]).most_common()],
    "",
    "## DEPS repos referenced",
    "",
    *[f"- {len(v):4d} `{k[4:]}`" for k, v in sorted(groups.items(), key=lambda kv: -len(kv[1])) if k.startswith("dep:")],
]
open(out_path, "w").write("\n".join(lines) + "\n")
print("\n".join(lines[:9]))
