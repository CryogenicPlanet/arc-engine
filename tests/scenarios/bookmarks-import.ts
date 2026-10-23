import { writeFileSync } from "node:fs";
import { join } from "node:path";
import type { ScenarioContext } from "../launch-slice/run";
import { run, page, press, set } from "./native";
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    await page(c);
    const html = join(c.outDir, "bookmarks.html");
    writeFileSync(html, '<!DOCTYPE NETSCAPE-Bookmark-file-1><TITLE>Bookmarks</TITLE><H1>Bookmarks</H1><DL><DT><A HREF="' + c.base + '/a">ArcCore fixture bookmark 7f3a</A></DL>');
    press(c, /^Import from Another Browser…$/);
    const state = c.native();
    c.assert("import dialog", state.elements.some(n => n.role === "AXDialog" || /Import Bookmarks/.test(n.label)));
    // Importing the user's actual browsers is outside fixture scope. Require a file-only chooser.
    press(c, /^(Bookmarks HTML File|Import from HTML File)$/);
    set(c, n => n.role === "AXTextField" && /file|name|path/i.test(n.label), html);
    press(c, /^(Open|Import)$/);
    c.assert("fixture bookmark imported", c.native().elements.some(n => n.label === "ArcCore fixture bookmark 7f3a"));
  });
}
