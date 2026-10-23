// Pairs dyld_info selector names with otool type encodings (same per-class order) and
// diffs the ObjC surface of the classes an app imports between two ArcCore builds.
import { readFileSync } from "node:fs";
type Facts = Map<string, string[]>;
const otoolTypes = (path: string): Map<string, { types: string[]; size?: string; sup?: string }> => {
  const out = new Map<string, { types: string[]; size?: string; sup?: string }>();
  let key: string | undefined;
  for (const line of readFileSync(path, "utf8").split("\n")) {
    const start = line.match(/^[0-9a-f]+ 0x[0-9a-f]+ _OBJC_(METACLASS|CLASS)_\$_(\S+)/);
    if (start) { key = `${start[1] === "METACLASS" ? "+" : "-"}${start[2]}`; out.set(key, { types: [] }); continue; }
    if (!key) continue;
    const entry = out.get(key)!;
    const size = line.match(/^\s+instanceSize\s+(\S+)/); if (size) entry.size = size[1];
    const sup = line.match(/^\s+superclass\s+0x[0-9a-f]+ _OBJC_(?:META)?CLASS_\$_(\S+)/); if (sup) entry.sup = sup[1];
    const types = line.match(/^\s+types\s+0x[0-9a-f]+(?: \(0x[0-9a-f]+\))? (\S+)$/); if (types) entry.types.push(types[1]!);
  }
  return out;
};
const dyldSelectors = (path: string): Map<string, string[]> => {
  const out = new Map<string, string[]>();
  let cls: string | undefined;
  for (const line of readFileSync(path, "utf8").split("\n")) {
    const iface = line.match(/^\s+@interface (\S+) :/); if (iface) { cls = iface[1]; continue; }
    if (/^\s+@interface /.test(line)) { cls = undefined; continue; } // categories
    const m = line.match(/^\s+0x[0-9A-F]+\s+([+-])\[(\S+) (\S+)\]$/);
    if (m && cls && m[2] === cls) { const key = `${m[1]}${cls}`; out.set(key, [...(out.get(key) ?? []), m[3]!]); }
  }
  return out;
};
const facts = (dyldPath: string, otoolPath: string): Facts => {
  const sels = dyldSelectors(dyldPath), types = otoolTypes(otoolPath), out: Facts = new Map();
  for (const [key, entry] of types) {
    const names = sels.get(key) ?? [];
    const list = names.length === entry.types.length ? names.map((n, i) => `${n} ${entry.types[i]}`) : [`<unpaired ${names.length} names/${entry.types.length} types>`, ...names];
    if (entry.size) list.push(`instanceSize=${entry.size}`);
    if (entry.sup) list.push(`super=${entry.sup}`);
    out.set(key, list);
  }
  return out;
};
const [, , d1, o1, d2, o2, importsPath] = Bun.argv as string[];
const imports = readFileSync(importsPath!, "utf8").split("\n").filter((s) => s.startsWith("_OBJC_CLASS_$_")).map((s) => s.slice(14));
const a = facts(d1!, o1!), b = facts(d2!, o2!);
let total = 0, unpaired = 0; const diffs: string[] = [];
for (const name of imports) for (const kind of ["-", "+"]) {
  const key = `${kind}${name}`, x = a.get(key), y = b.get(key);
  if (!x || !y) { if (x || y) diffs.push(`${key}: only in ${x ? "old" : "new"}`); continue; }
  if (x[0]?.startsWith("<unpaired") || y[0]?.startsWith("<unpaired")) unpaired++;
  total += x.length;
  const sx = new Set(x), sy = new Set(y);
  const removed = x.filter((f) => !sy.has(f)), added = y.filter((f) => !sx.has(f));
  if (removed.length || added.length) diffs.push(`${key}:\n${removed.map((f) => `   - ${f}`).join("\n")}${removed.length && added.length ? "\n" : ""}${added.map((f) => `   + ${f}`).join("\n")}`);
}
console.log(diffs.join("\n"));
console.log(`\ncompared ${imports.length} imported classes, ${total} method/layout facts; unpaired classes: ${unpaired}; classes with differences: ${diffs.length}`);
