import type { ScenarioContext } from "../launch-slice/run";
import { writeFileSync } from "node:fs";
import { join } from "node:path";

export const fixtureBookmarkHTML = `<!DOCTYPE NETSCAPE-Bookmark-file-1>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=UTF-8">
<TITLE>ArcCore disposable import</TITLE><H1>ArcCore disposable import</H1>
<DL><p><DT><H3>ArcCore Fixture Folder</H3><DL><p>
<DT><A HREF="http://127.0.0.1:8765/arccore-import-fixture" ADD_DATE="1700000000">ArcCore Imported Bookmark 7f3a</A>
</DL><p></DL><p>
`;
export const morningPlan = [
  "Lead must expose an importer path selecting this explicit file or a disposable browser profile source root.",
  "Use only import-bookmarks.html emitted under this run's outDir. Do not select CHROME/SAFARI discovered profiles.",
  "Before import assert no ArcCore Fixture Folder or ArcCore Imported Bookmark 7f3a exists in the disposable target.",
  "Import fixture; assert one folder and one exact URL/title bookmark, with no extra imported history/passwords/cookies.",
  "Quit/relaunch the disposable agent; assert exact bookmark/folder persistence, then compare transcripts.",
];
export default async function scenario(c: ScenarioContext) {
  writeFileSync(join(c.outDir, "import-bookmarks.html"), fixtureBookmarkHTML.replace("http://127.0.0.1:8765", c.base));
  c.record("disposable data import", "inconclusive", "UNSUPPORTED: existing import wizard discovers real CHROME/SAFARI profiles and has no observed explicit fixture source selection. Fixture written under outDir only; no browser source opened or imported. Lead must add disposable source injection/file chooser support.");
}
