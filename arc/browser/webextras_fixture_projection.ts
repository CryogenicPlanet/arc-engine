// Owned proposal helper. Raw nesting/identity validation must run before projection.
export class PublicTraceProjection {
  private readonly methods = new Set<string>();
  private readonly nativeConstructors = new Set<string>();
  private readonly identities = new Map<string, string>();
  private readonly counts = new Map<string, number>();
  constructor(metadata: string, readonly fixtureImage: string, readonly byCaller = true) {
    for (const line of metadata.split("\n")) {
      const row = line.split("\t");
      if (row[0] === "M") {
        const key = JSON.stringify([row[2], row[1], row[3]]);
        this.methods.add(key);
        if (row[3].startsWith("initWith") && row[4]?.includes("^")) this.nativeConstructors.add(key);
      }
    }
  }
  private selected(event: any): boolean {
    if (event.dir === "engine->app") return true;
    if (event.dir !== "->engine" || !this.methods.has(JSON.stringify([event.kind, event.cls, event.sel]))) return false;
    if (this.nativeConstructors.has(JSON.stringify([event.kind, event.cls, event.sel])) && event.args?.includes("<ptr>")) return false;
    return !this.byCaller || event.from === "app" || event.from === this.fixtureImage ||
      (event.from === "engine" && event.sel.startsWith("set"));
  }
  private receiver(value: any): any {
    if (typeof value !== "string") return value;
    return value.replace(/<([^<>]+)#(\d+)>/g, (full, cls) => {
      if (!this.identities.has(full)) {
        const count = (this.counts.get(cls) ?? 0) + 1;
        this.counts.set(cls, count); this.identities.set(full, `<${cls}#${count}>`);
      }
      return this.identities.get(full)!;
    });
  }
  project(rows: {event:any}[]): any[] {
    const stacks = new Map<string, { selected:boolean; depth:number }[]>();
    const result: any[] = [];
    for (const {event} of rows) {
      if (event.ev !== "call" && event.ev !== "ret") continue;
      const stack = stacks.get(event.thread) ?? []; stacks.set(event.thread, stack);
      let entry: { selected:boolean; depth:number };
      if (event.ev === "call") {
        entry = { selected:this.selected(event), depth:stack.filter(ancestor => ancestor.selected).length };
        stack.push(entry);
      } else {
        const matched = stack.pop();
        if (!matched) throw new Error("projection requires a complete raw action trace");
        entry = matched;
      }
      if (entry.selected) result.push({ ev:event.ev, thread:event.thread, depth:entry.depth, dir:event.dir,
        cls:event.cls, kind:event.kind, sel:event.sel, self:this.receiver(event.self), from:event.from });
    }
    if ([...stacks.values()].some(stack => stack.length)) throw new Error("projection received incomplete raw action trace");
    return result;
  }
}
