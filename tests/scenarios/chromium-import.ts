// Import from a Chromium-family profile (Chrome) into the Default profile: history, bookmarks, cookies,
// passwords, saved cards and extensions, through ArcBrowserContext's public import API, driven in-process by
// chromium-import-driver.m. The fixture is a Chrome profile made here (schema-24 cookie store with
// SHA-256-prefixed values, Login Data, Web Data, History, Bookmarks), encrypted like Chrome on macOS with a test
// keychain password passed by switch, so the real keychain is never read. Ours only: Arc's engine needs its own
// ArcImportSourceProfile for a Chrome source, which only its discovery can create.
import type { ScenarioContext } from "../launch-slice/run";
import { Database } from "bun:sqlite";
import { createCipheriv, createHash, pbkdf2Sync } from "node:crypto";
import { copyFileSync, existsSync, mkdirSync, readFileSync, rmSync, writeFileSync } from "node:fs";
import { join } from "node:path";

const PASSWORD = "arcmod-import-7f3a";
const EXTENSION = "cjpalhdlnbpafiamejdnhcphjbkeiagm";
export const chromiumArgs = [`--arcmod-import-test-keychain-password=${PASSWORD}`];

const key = pbkdf2Sync(PASSWORD, "saltysalt", 1003, 16, "sha1");
const encrypt = (plain: Buffer): Buffer => {
  const cipher = createCipheriv("aes-128-cbc", key, Buffer.alloc(16, 0x20));
  return Buffer.concat([Buffer.from("v10"), cipher.update(plain), cipher.final()]);
};
// Chrome's time: microseconds since 1601-01-01.
const chromeTime = (ms: number) => BigInt(ms + 11644473600000) * 1000n;

function makeFixture(dir: string) {
  rmSync(dir, { recursive: true, force: true });
  mkdirSync(join(dir, "Network"), { recursive: true });
  const now = Date.now(), year = 365 * 24 * 3600 * 1000;
  const cookies = new Database(join(dir, "Network", "Cookies"));
  cookies.run("CREATE TABLE meta(key LONGVARCHAR NOT NULL UNIQUE PRIMARY KEY, value LONGVARCHAR)");
  cookies.run("INSERT INTO meta VALUES ('version', '24'), ('last_compatible_version', '24')");
  cookies.run(`CREATE TABLE cookies(creation_utc INTEGER NOT NULL, host_key TEXT NOT NULL, top_frame_site_key TEXT NOT NULL,
    name TEXT NOT NULL, value TEXT NOT NULL, encrypted_value BLOB NOT NULL, path TEXT NOT NULL, expires_utc INTEGER NOT NULL,
    is_secure INTEGER NOT NULL, is_httponly INTEGER NOT NULL, last_access_utc INTEGER NOT NULL, has_expires INTEGER NOT NULL,
    is_persistent INTEGER NOT NULL, priority INTEGER NOT NULL, samesite INTEGER NOT NULL, source_scheme INTEGER NOT NULL,
    source_port INTEGER NOT NULL, last_update_utc INTEGER NOT NULL, source_type INTEGER NOT NULL,
    has_cross_site_ancestor INTEGER NOT NULL)`);
  const addCookie = (host: string, name: string, value: string, secure: number, httponly: number, expires: number) =>
    cookies.run("INSERT INTO cookies VALUES (?, ?, '', ?, '', ?, '/', ?, ?, ?, ?, 1, 1, 1, -1, ?, ?, ?, 0, 0)", [
      chromeTime(now - 1000), host, name,
      encrypt(Buffer.concat([createHash("sha256").update(host).digest(), Buffer.from(value)])),
      chromeTime(expires), secure, httponly, chromeTime(now - 1000), secure ? 2 : 1, secure ? 443 : 80, chromeTime(now - 1000)]);
  addCookie("127.0.0.1", "imported_7f3a", "v1", 0, 0, now + year);
  addCookie("127.0.0.1", "existing_7f3a", "theirs", 0, 0, now + year);
  addCookie(".example.com", "session_7f3a", "secret", 1, 1, now + year);
  addCookie("127.0.0.1", "expired_7f3a", "old", 0, 0, now - year);
  cookies.close();

  const logins = new Database(join(dir, "Login Data"));
  logins.run(`CREATE TABLE logins(origin_url VARCHAR NOT NULL, action_url VARCHAR, username_element VARCHAR,
    username_value VARCHAR, password_element VARCHAR, password_value BLOB, signon_realm VARCHAR NOT NULL,
    date_created INTEGER NOT NULL, blacklisted_by_user INTEGER NOT NULL, scheme INTEGER NOT NULL, times_used INTEGER,
    date_last_used INTEGER NOT NULL DEFAULT 0, date_password_modified INTEGER NOT NULL DEFAULT 0,
    display_name VARCHAR, federation_url VARCHAR)`);
  const addLogin = (user: string, password: string, blocked: number) =>
    logins.run("INSERT INTO logins VALUES ('https://accounts.example.com/login', 'https://accounts.example.com/session', 'user', ?, 'pass', ?, 'https://accounts.example.com/', ?, ?, 0, 3, ?, ?, '', '')", [
      user, encrypt(Buffer.from(password)), chromeTime(now - year), blocked, chromeTime(now - 1000), chromeTime(now - year)]);
  addLogin("fixture-user-7f3a", "fixture-pass-7f3a", 0);
  addLogin("never-saved-7f3a", "", 1);
  logins.close();

  const web = new Database(join(dir, "Web Data"));
  web.run(`CREATE TABLE credit_cards(guid VARCHAR PRIMARY KEY, name_on_card VARCHAR, expiration_month INTEGER,
    expiration_year INTEGER, card_number_encrypted BLOB, date_modified INTEGER NOT NULL DEFAULT 0,
    origin VARCHAR DEFAULT '', use_count INTEGER NOT NULL DEFAULT 0, use_date INTEGER NOT NULL DEFAULT 0,
    billing_address_id VARCHAR, nickname VARCHAR)`);
  web.run("INSERT INTO credit_cards (guid, name_on_card, expiration_month, expiration_year, card_number_encrypted, nickname) VALUES ('00000000-0000-4000-8000-000000007f3a', 'Card Holder 7f3a', 4, 2031, ?, 'Fixture card')", [encrypt(Buffer.from("4111111111111111"))]);
  web.close();

  const history = new Database(join(dir, "History"));
  history.run("CREATE TABLE urls(id INTEGER PRIMARY KEY, url LONGVARCHAR, title LONGVARCHAR, visit_count INTEGER, typed_count INTEGER, last_visit_time INTEGER, hidden INTEGER)");
  history.run("INSERT INTO urls VALUES (1, 'https://history.example.com/7f3a', 'History 7f3a', 3, 1, ?, 0)", [chromeTime(now - 60000)]);
  history.close();

  writeFileSync(join(dir, "Bookmarks"), JSON.stringify({ version: 1, roots: {
    bookmark_bar: { type: "folder", name: "Bookmarks bar", children: [{ type: "url", name: "Bookmark 7f3a", url: "https://bookmark.example.com/7f3a", date_added: String(chromeTime(now)) }] },
    other: { type: "folder", name: "Other bookmarks", children: [] },
    synced: { type: "folder", name: "Mobile bookmarks", children: [] } } }));
}

type Event = { event: string; category?: number; error?: string };
type Result = { complete?: boolean; accepted?: boolean; immediateError?: string; failure?: string; events?: Event[] };
type Cookie = { name: string; value: string; domain: string; httpOnly: boolean; secure: boolean };

export default async function scenario(c: ScenarioContext) {
  const directory = join(c.outDir, "chromium-import-driver");
  const source = join(directory, "Chrome Fixture Profile");
  mkdirSync(directory, { recursive: true });
  makeFixture(source);
  const library = join(directory, "ChromiumImportDriver.dylib");
  const compile = Bun.spawnSync(["xcrun", "clang", "-dynamiclib", "-fobjc-arc", "-Wall", "-Wextra", "-Werror",
    "-framework", "Foundation", "-framework", "AppKit", join(import.meta.dir, "chromium-import-driver.m"), "-o", library]);
  if (!compile.success) throw new Error(`driver build failed: ${compile.stderr.toString()}`);
  const l = c.launch({ libraries: [library], environment: { ARC_SCENARIO_CHROMIUM_IMPORT_DIRECTORY: directory } });
  if (await c.ready(l, "launch")) {
    await c.step("import from Chrome", async () => {
      const client = await c.client();
      // A cookie the profile already has; skipExistingCookies keeps ours.
      await client.request("Storage.setCookies", { cookies: [{ name: "existing_7f3a", value: "ours", domain: "127.0.0.1", path: "/", expires: Date.now() / 1000 + 3600 }] });
      writeFileSync(join(directory, "go"), JSON.stringify({ source, categories: 1 | 2 | 4 | 8 | 128 | 256, extensions: [EXTENSION], skipExisting: true }));
      const resultPath = join(directory, "result.json");
      const result = await c.waitFor(() => {
        if (!existsSync(resultPath)) return undefined;
        const r = JSON.parse(readFileSync(resultPath, "utf8")) as Result;
        return r.complete ? r : undefined;
      }, 60000);
      if (!result) throw new Error("driver did not complete");
      c.assert("import accepted", result.accepted === true && !result.failure, `${result.immediateError} ${result.failure ?? ""}`);
      const events = result.events ?? [];
      const ended = (category: number) => events.find(e => e.event === "category-end" && e.category === category);
      for (const [category, name] of [[1, "history"], [2, "bookmarks"], [4, "cookies"], [8, "passwords"], [128, "extensions"], [256, "cards"]] as const) {
        const end = ended(category);
        c.assert(`${name} category ends without error`, end !== undefined && end.error === undefined, JSON.stringify(end));
      }
      c.assert("import ends without error", events.at(-1)?.event === "end" && events.at(-1)?.error === undefined, JSON.stringify(events.at(-1)));
      const cookies = ((await client.request("Storage.getCookies", {})) as { cookies: Cookie[] }).cookies;
      const cookie = (name: string) => cookies.find(k => k.name === name);
      c.assert("cookie imported and decrypted", cookie("imported_7f3a")?.value === "v1", JSON.stringify(cookie("imported_7f3a")));
      c.assert("secure httponly cookie keeps its flags", cookie("session_7f3a")?.value === "secret" && cookie("session_7f3a")?.httpOnly === true && cookie("session_7f3a")?.secure === true, JSON.stringify(cookie("session_7f3a")));
      c.assert("existing cookie kept (skipExistingCookies)", cookie("existing_7f3a")?.value === "ours", JSON.stringify(cookie("existing_7f3a")));
      c.assert("expired cookie not imported", cookie("expired_7f3a") === undefined, JSON.stringify(cookie("expired_7f3a")));
      const page = c.open(`${c.base}/a`);
      c.assert("imported cookie sent to the site", await c.titleIs(page, c.EXPECT.aTitle) && String(c.evaluate(page, "document.cookie")).includes("imported_7f3a=v1"), String(c.evaluate(page, "document.cookie")));
      // Let the password store commit before quitting.
      await c.sleep(3000);
    });
  }
  await c.quit(l, "quit");
  await c.step("password and card saved to the profile", () => {
    const store = join(c.data, "Agent Chromium Data", "Default", "Login Data");
    const copy = join(directory, "Login Data copy");
    copyFileSync(store, copy);
    if (existsSync(`${store}-wal`)) copyFileSync(`${store}-wal`, `${copy}-wal`);
    const db = new Database(copy);
    const rows = db.query("SELECT username_value, signon_realm, length(password_value) AS n FROM logins").all() as { username_value: string; signon_realm: string; n: number }[];
    db.close();
    c.assert("login imported", rows.some(r => r.username_value === "fixture-user-7f3a" && r.signon_realm === "https://accounts.example.com/" && r.n > 0), JSON.stringify(rows));
    c.assert("never-save entry not imported", !rows.some(r => r.username_value === "never-saved-7f3a"), JSON.stringify(rows));
    const webStore = join(c.data, "Agent Chromium Data", "Default", "Web Data");
    const webCopy = join(directory, "Web Data copy");
    copyFileSync(webStore, webCopy);
    if (existsSync(`${webStore}-wal`)) copyFileSync(`${webStore}-wal`, `${webCopy}-wal`);
    const web = new Database(webCopy);
    const cards = web.query("SELECT name_on_card, expiration_month, expiration_year, nickname, length(card_number_encrypted) AS n FROM credit_cards").all() as { name_on_card: string; expiration_month: number; expiration_year: number; nickname: string; n: number }[];
    web.close();
    c.assert("card imported", cards.some(k => k.name_on_card === "Card Holder 7f3a" && k.expiration_month === 4 && k.expiration_year === 2031 && k.nickname === "Fixture card" && k.n > 0), JSON.stringify(cards));
  });
}
