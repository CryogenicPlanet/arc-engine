// Import a folder Safari exported (File → Export Browsing Data) into the Default profile: bookmarks, history
// and passwords, through ArcBrowserContext's public import API with a type-12 source, driven in-process by
// chromium-import-driver.m. A first launch saves one password; the second launch imports an export whose
// CSV conflicts with it (the saved password must win) and whose History.json has entries Chromium's Safari
// importer skips. The folder also has a second JSON file, so the import has to pick History.json.
// Ours only: no recording of Arc's engine has a full Safari export yet.
import type { ScenarioContext } from "../launch-slice/run";
import { Database } from "bun:sqlite";
import { createDecipheriv, pbkdf2Sync } from "node:crypto";
import { copyFileSync, existsSync, mkdirSync, readFileSync, rmSync, writeFileSync } from "node:fs";
import { join } from "node:path";

const BOOKMARK = "https://bookmark.example.com/safari-7f3a";
const VISITED = "https://history.example.com/safari-7f3a";
const REALM = "https://safari.example.com/";
const DAY = 24 * 3600 * 1000;
// Chrome's time: microseconds since 1601-01-01.
const chromeTime = (ms: number) => BigInt(ms + 11644473600000) * 1000n;
// Agent builds run with --use-mock-keychain, whose password is Chromium's fixed "mock_password".
const key = pbkdf2Sync("mock_password", "saltysalt", 1003, 16, "sha1");
const decrypt = (blob: Uint8Array): string => {
  const bytes = Buffer.from(blob);
  if (bytes.subarray(0, 3).toString() !== "v10") return "";
  const decipher = createDecipheriv("aes-128-cbc", key, Buffer.alloc(16, 0x20));
  return Buffer.concat([decipher.update(bytes.subarray(3)), decipher.final()]).toString();
};
const passwordsCSV = (rows: [string, string][]) =>
  "Title,URL,Username,Password,Notes,OTPAuth\n" + rows.map(([user, pass]) => `safari.example.com (${user}),${REALM},${user},${pass},,\n`).join("");

function makeSeed(dir: string) {
  rmSync(dir, { recursive: true, force: true });
  mkdirSync(dir, { recursive: true });
  writeFileSync(join(dir, "Passwords.csv"), passwordsCSV([["safari-user-7f3a", "kept-7f3a"]]));
}

function makeExport(dir: string, visitedAt: number) {
  rmSync(dir, { recursive: true, force: true });
  mkdirSync(dir, { recursive: true });
  writeFileSync(join(dir, "Bookmarks.html"), `<!DOCTYPE NETSCAPE-Bookmark-file-1>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=UTF-8">
<TITLE>Bookmarks</TITLE>
<H1>Bookmarks</H1>
<DL><p>
<DT><H3>Safari Fixture Folder</H3>
<DL><p>
<DT><A HREF="${BOOKMARK}">Safari Bookmark 7f3a</A>
</DL><p>
</DL><p>
`);
  const usec = (ms: number) => ms * 1000;
  writeFileSync(join(dir, "History.json"), JSON.stringify({
    metadata: { browser_name: "Safari", browser_version: "18.0", data_type: "history", export_time_usec: usec(Date.now()), schema_version: 1 },
    history: [
      { url: VISITED, title: "Safari History 7f3a", time_usec: usec(visitedAt), visit_count: 4 },
      { title: "Skipped: no URL", time_usec: usec(visitedAt) },
      { url: "not a url", title: "Skipped: bad URL", time_usec: usec(visitedAt) },
      { url: "ftp://files.example.com/safari-7f3a", title: "Skipped: not http", time_usec: usec(visitedAt) },
      { url: "https://history.example.com/no-time-7f3a", title: "Skipped: no time" },
      { url: "https://history.example.com/old-7f3a", title: "Skipped: too old", time_usec: usec(Date.now() - 120 * DAY) },
      { url: "http://redirect.example.com/7f3a", title: "Skipped: redirect", time_usec: usec(visitedAt),
        source_url: "http://redirect.example.com/7f3a", destination_url: "https://redirect.example.com/7f3a" },
    ],
  }));
  // Sorts before History.json and isn't history.
  writeFileSync(join(dir, "Credit Cards.json"), JSON.stringify({ metadata: { data_type: "payment_cards" }, payment_cards: [] }));
  writeFileSync(join(dir, "Passwords.csv"), passwordsCSV([["safari-user-7f3a", "replaced-7f3a"], ["safari-new-7f3a", "new-7f3a"]]));
}

type Event = { event: string; category?: number; error?: string };
type Result = { complete?: boolean; accepted?: boolean; immediateError?: string; failure?: string; events?: Event[] };

// Reads a profile database through a private copy, so the live files are never opened.
function query<T>(store: string, copy: string, sql: string): T[] {
  copyFileSync(store, copy);
  if (existsSync(`${store}-wal`)) copyFileSync(`${store}-wal`, `${copy}-wal`);
  const db = new Database(copy);
  const rows = db.query(sql).all() as T[];
  db.close();
  for (const file of [copy, `${copy}-wal`, `${copy}-shm`]) rmSync(file, { force: true });
  return rows;
}

export default async function scenario(c: ScenarioContext) {
  const root = join(c.outDir, "safari-export-driver");
  const visitedAt = Date.now() - 60000;
  mkdirSync(root, { recursive: true });
  const library = join(root, "ChromiumImportDriver.dylib");
  const compile = Bun.spawnSync(["xcrun", "clang", "-dynamiclib", "-fobjc-arc", "-Wall", "-Wextra", "-Werror",
    "-framework", "Foundation", "-framework", "AppKit", join(import.meta.dir, "chromium-import-driver.m"), "-o", library]);
  if (!compile.success) throw new Error(`driver build failed: ${compile.stderr.toString()}`);

  // One launch, one import through the driver; returns the delegate events.
  const importIn = async (name: string, source: string, categories: number): Promise<Event[] | undefined> => {
    const directory = join(root, name);
    mkdirSync(directory, { recursive: true });
    const l = c.launch({ libraries: [library], environment: { ARC_SCENARIO_CHROMIUM_IMPORT_DIRECTORY: directory } });
    let events: Event[] | undefined;
    if (await c.ready(l, `${name} launch`)) {
      await c.step(`${name} import`, async () => {
        writeFileSync(join(directory, "go"), JSON.stringify({ source, type: 12, name: "Safari", appPath: "", categories, extensions: [], skipExisting: true }));
        const resultPath = join(directory, "result.json");
        const result = await c.waitFor(() => {
          if (!existsSync(resultPath)) return undefined;
          const r = JSON.parse(readFileSync(resultPath, "utf8")) as Result;
          return r.complete ? r : undefined;
        }, 60000);
        if (!result) throw new Error("driver did not complete");
        c.assert(`${name} import accepted`, result.accepted === true && !result.failure, `${result.immediateError} ${result.failure ?? ""}`);
        events = result.events ?? [];
        // Let the password store commit before quitting. History (committed every 10 s) and bookmarks (saved
        // after 2.5 s) must survive the quit on their own.
        await c.sleep(3000);
      });
    }
    await c.quit(l, `${name} quit`);
    return events;
  };

  const seed = join(root, "Safari Export Seed");
  makeSeed(seed);
  const seeded = await importIn("seed", seed, 8);
  c.assert("seed password imported", seeded?.some(e => e.event === "category-end" && e.category === 8 && e.error === undefined) === true, JSON.stringify(seeded));

  const source = join(root, "Safari Export 7f3a");
  makeExport(source, visitedAt);
  // History, bookmarks, passwords and cards; a Safari export's cards aren't imported.
  const events = await importIn("export", source, 1 | 2 | 8 | 256);
  if (events) {
    const all = JSON.stringify(events);
    c.assert("one initialize, then one start", events[0]?.event === "initialize" && events[1]?.event === "start" && events.filter(e => e.event === "start").length === 1, all);
    for (const [category, name] of [[1, "history"], [2, "bookmarks"], [8, "passwords"]] as const) {
      const starts = events.filter(e => e.event === "category-start" && e.category === category);
      const ends = events.filter(e => e.event === "category-end" && e.category === category);
      c.assert(`${name} starts and ends once, without error`, starts.length === 1 && ends.length === 1 && ends[0]!.error === undefined, all);
    }
    const cards = events.filter(e => e.event === "category-end" && e.category === 256);
    c.assert("cards end once with an error", cards.length === 1 && cards[0]!.error !== undefined, all);
    c.assert("import ends last, without error", events.at(-1)?.event === "end" && events.at(-1)?.error === undefined && events.filter(e => e.event === "end").length === 1, all);
  }

  await c.step("imported data saved to the profile", () => {
    const profile = join(c.data, "Agent Chromium Data", "Default");
    const bookmarks = readFileSync(join(profile, "Bookmarks"), "utf8");
    c.assert("bookmark imported", bookmarks.includes(BOOKMARK) && bookmarks.includes("Safari Fixture Folder"), "");
    const urls = query<{ url: string; title: string; visit_count: number; last_visit_time: bigint | number }>(
      join(profile, "History"), join(root, "History copy"), "SELECT url, title, visit_count, last_visit_time FROM urls");
    const visited = urls.find(r => r.url === VISITED);
    const drift = visited ? Number(BigInt(visited.last_visit_time) - chromeTime(visitedAt)) / 1e6 : Infinity;
    c.assert("history entry imported with its title and time", visited?.title === "Safari History 7f3a" && visited.visit_count >= 1 && Math.abs(drift) < 1, JSON.stringify(visited ?? urls.slice(0, 5), (_, v) => typeof v === "bigint" ? String(v) : v));
    const skipped = urls.filter(r => r.title.startsWith("Skipped:"));
    c.assert("history entries Chromium's Safari importer skips are skipped", skipped.length === 0, JSON.stringify(skipped.map(r => r.title)));
    const logins = query<{ username_value: string; signon_realm: string; password_value: Uint8Array }>(
      join(profile, "Login Data"), join(root, "Login Data copy"), "SELECT username_value, signon_realm, password_value FROM logins");
    const user = (name: string) => logins.filter(r => r.username_value === name && r.signon_realm === REALM);
    c.assert("saved password kept over the export's conflicting one", user("safari-user-7f3a").length === 1 && decrypt(user("safari-user-7f3a")[0]!.password_value) === "kept-7f3a", String(user("safari-user-7f3a").length));
    c.assert("non-conflicting login imported", user("safari-new-7f3a").length === 1 && decrypt(user("safari-new-7f3a")[0]!.password_value) === "new-7f3a", String(user("safari-new-7f3a").length));
  });
}
