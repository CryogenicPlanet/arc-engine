// Compares each imported class's otool -ov block between two builds with addresses removed,
// and its dyld_info selector list, so signatures, layout, ivars, properties and names all count.
import { readFileSync } from "node:fs";
const blocks = (path: string): Map<string, string> => {
  const out = new Map<string, string>(); let key: string | undefined; let lines: string[] = [];
  const flush = () => { if (key && lines.length > (out.get(key)?.split("\n").length ?? -1)) out.set(key, lines.join("\n")); };
  for (const line of readFileSync(path, "utf8").split("\n")) {
    const start = line.match(/^[0-9a-f]+ 0x[0-9a-f]+ _OBJC_CLASS_\$_(\S+)/);
    if (start || /^[0-9a-f]+ 0x[0-9a-f]+ /.test(line) || /^Contents of /.test(line)) { flush(); key = start?.[1]; lines = []; continue; }
    if (key) lines.push(line.replace(/\(?0x[0-9a-f]+\)?/g, "").replace(/^[0-9a-f]{16}/, "").replace(/\s+/g, " ").trim());
  }
  flush(); return out;
};
const selectors = (path: string): Map<string, string> => {
  const out = new Map<string, string[]>(); let cls: string | undefined;
  for (const line of readFileSync(path, "utf8").split("\n")) {
    const iface = line.match(/^\s+@interface (\S+?)(?:\(\S*\))? :?/); if (iface) { cls = iface[1]; continue; }
    const m = line.match(/^\s+0x[0-9A-F]+\s+([+-]\[\S+ \S+\])$/); if (m && cls) out.set(cls, [...(out.get(cls) ?? []), m[1]!]);
  }
  return new Map([...out].map(([k, v]) => [k, v.join("\n")]));
};
const [, , o1, d1, o2, d2, importsPath] = Bun.argv as string[];
const imports = process.env.ALL ? [...new Set([...blocks(o1!).keys()])] : readFileSync(importsPath!, "utf8").split("\n").filter((s) => s.startsWith("_OBJC_CLASS_$_")).map((s) => s.slice(14));
const [b1, b2, s1, s2] = [blocks(o1!), blocks(o2!), selectors(d1!), selectors(d2!)];
let lines = 0; const diffs: string[] = [];
for (const name of imports) {
  const x = b1.get(name), y = b2.get(name);
  if (x === undefined || y === undefined) { diffs.push(`${name}: block missing (${x === undefined ? "old" : "new"})`); continue; }
  lines += x.split("\n").length;
  if (x !== y) { const xs = x.split("\n"), ys = new Set(y.split("\n")); diffs.push(`${name}: layout/signature block differs, e.g. ${xs.find((l) => !ys.has(l))}`); }
  if ((s1.get(name) ?? "") !== (s2.get(name) ?? "")) diffs.push(`${name}: selector list differs`);
}
console.log(diffs.join("\n") || "no differences");
console.log(`compared ${imports.length} imported classes: ${lines} normalized metadata lines plus selector lists`);
