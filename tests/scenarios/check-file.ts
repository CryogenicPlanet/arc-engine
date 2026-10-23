// TypeScript counterpart to arc/tools/check-file.sh (which accepts Chromium ObjC++ only).
import { readFileSync } from "node:fs";
const transpiler = new Bun.Transpiler({ loader: "ts", target: "bun" });
for (const file of Bun.argv.slice(2)) {
  transpiler.transformSync(readFileSync(file, "utf8"));
  console.log(`check done: ${file}`);
}
