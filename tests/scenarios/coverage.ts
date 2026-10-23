import { existsSync, readFileSync, readdirSync, writeFileSync } from "node:fs";
import { basename, join } from "node:path";
// Counts are observed identities for the entire launch, including startup. They are lower bounds,
// not claims that a scenario exercised every feature of a class or that all calls crossed the boundary.
export function summarize(out: string) {
  if (basename(out) === "arc-1.170-launch-slice") throw new Error("original launch-slice golden is immutable");
  const engineClasses = new Set(readFileSync(join(import.meta.dir, "../../arc/tools/arccore-1.170.0-objc.tsv"), "utf8")
    .split("\n").filter(line => line.startsWith("C\t")).map(line => line.split("\t")[1]!));
  const steps = JSON.parse(readFileSync(join(out, "steps.json"), "utf8"));
  const launches = readdirSync(out).filter(n => /^launch-\d+$/.test(n)).sort();
  const methods = new Map<string, number>();
  const errors: string[] = [];
  for (const launch of launches) {
    const file = join(out, launch, "record.jsonl");
    if (!existsSync(file)) { errors.push(`${launch}: no transcript`); continue; }
    const rows = readFileSync(file, "utf8").trim().split("\n").map(line => JSON.parse(line));
    if (!rows.some(r => r.ev === "hooked" && r.engine_classes > 0)) errors.push(`${launch}: recorder did not hook`);
    let calls = 0;
    for (const r of rows) if (r.ev === "call") {
      calls++;
      const key = `${r.dir} ${r.kind}[${r.cls} ${r.sel}]`;
      methods.set(key, (methods.get(key) ?? 0) + 1);
    }
    if (!calls) errors.push(`${launch}: no calls`);
  }
  if (!launches.length) errors.push("no launches");
  const classes = new Map<string, { selectors: Set<string>; calls: number }>();
  for (const [key, calls] of methods) {
    const cls = key.match(/\[([^ ]+) /)![1]!;
    const group = classes.get(cls) ?? { selectors: new Set<string>(), calls: 0 };
    group.selectors.add(key); group.calls += calls; classes.set(cls, group);
  }
  const accepted = steps.steps.length > 0 && steps.steps.every((s: {outcome:string}) => s.outcome === "pass") && !errors.length;
  const engineMethods = [...methods].filter(([key]) => key.startsWith("->engine ") && engineClasses.has(key.match(/\[([^ ]+) /)![1]!));
  const callbackMethods = [...methods].filter(([key]) => key.startsWith("engine->app "));
  const summary = { engineClassCount: new Set(engineMethods.map(([key]) => key.match(/\[([^ ]+) /)![1]!)).size,
    engineSelectorCount: engineMethods.length, callbackSelectorCount: callbackMethods.length,
    scenario: steps.scenario, accepted, errors, launches, classCount: classes.size, selectorCount: methods.size,
    callCount: [...methods.values()].reduce((a,b)=>a+b,0),
    classes: Object.fromEntries([...classes].sort().map(([cls,g]) => [cls, {calls:g.calls,selectorCount:g.selectors.size,selectors:[...g.selectors].sort()}])),
    methods: Object.fromEntries([...methods].sort()),
  };
  writeFileSync(join(out, "coverage.json"), JSON.stringify(summary, null, 2));
  return summary;
}
if (import.meta.main) for (const out of Bun.argv.slice(2)) {
  const s = summarize(out); console.log(`${s.scenario}: accepted=${s.accepted} classes=${s.classCount} selectors=${s.selectorCount} calls=${s.callCount}`);
  if (!s.accepted) process.exitCode = 1;
}
