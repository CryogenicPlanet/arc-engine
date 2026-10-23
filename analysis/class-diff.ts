import { readFileSync } from "node:fs";
const block = (path: string, want: string): string[] => {
  const lines = readFileSync(path, "utf8").split("\n"); let best: string[] = [];
  lines.forEach((l, i) => {
    if (!new RegExp(`^[0-9a-f]+ 0x[0-9a-f]+ _OBJC_CLASS_\\$_${want}$`).test(l)) return;
    const end = lines.findIndex((m, j) => j > i && /^([0-9a-f]+ 0x[0-9a-f]+ |Contents of )/.test(m));
    const body = lines.slice(i + 1, end).map((m) => m.replace(/\(?0x[0-9a-f]+\)?/g, "").replace(/\s+/g, " ").trim());
    if (body.length > best.length) best = body;
  });
  return best;
};
for (const c of Bun.argv.slice(4)) {
  const a = block(Bun.argv[2]!, c), b = block(Bun.argv[3]!, c);
  const count = (xs: string[]) => xs.reduce((m, x) => m.set(x, (m.get(x) ?? 0) + 1), new Map<string, number>());
  const [ca, cb] = [count(a), count(b)];
  console.log(`== ${c} (${a.length} vs ${b.length} lines)`);
  for (const [k, n] of ca) if ((cb.get(k) ?? 0) < n) console.log(`  - ${k}${n - (cb.get(k) ?? 0) > 1 ? ` (x${n - (cb.get(k) ?? 0)})` : ""}`);
  for (const [k, n] of cb) if ((ca.get(k) ?? 0) < n) console.log(`  + ${k}${n - (ca.get(k) ?? 0) > 1 ? ` (x${n - (ca.get(k) ?? 0)})` : ""}`);
}
