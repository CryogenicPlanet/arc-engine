import type { ScenarioContext } from "../launch-slice/run";
import { existsSync, mkdirSync, writeFileSync } from "node:fs";
import { join } from "node:path";
import { run, page, observed } from "./native";
const name = "arccore-scenario-prompt-7f3a.txt";
export function fixture(req: Request) {
  switch (new URL(req.url).pathname) {
    case "/download-prompt": return new Response('<!doctype html><title>download prompt fixture</title><a href="/attachment">Download fixture</a>', {headers:{"content-type":"text/html"}});
    case "/attachment": return new Response("ArcCore prompt download 7f3a\n", {headers:{"content-type":"text/plain","content-disposition":`attachment; filename="${name}"`}});
  }
}
export default async function scenario(c: ScenarioContext) {
  // Seed a fresh, disposable profile before its first launch. The reviewed preferences spec maps
  // promptForDownloadPath to this exact Chromium key; no running profile is edited.
  const prefs = join(c.data, "User Data/Default/Preferences"), directory = join(c.outDir, "downloads");
  if (existsSync(prefs)) throw new Error("download-prompt requires a fresh agent profile");
  if (existsSync(join(process.env.HOME!, "Downloads", name))) throw new Error("fixture filename already exists in Downloads");
  mkdirSync(join(c.data, "User Data/Default"), {recursive:true}); mkdirSync(directory, {recursive:true});
  writeFileSync(prefs, JSON.stringify({ download: {prompt_for_download:true, default_directory:directory} }));
  await run(c, async () => {
    const t = await page(c, "/download-prompt", "download prompt fixture");
    c.cdp("click", t, "60", "15");
    c.assert("native save prompt has fixed filename stem", await c.waitFor(() => c.native().elements.some(n => n.identifier === "saveAsNameTextField" && n.value === "arccore-scenario-prompt-7f3a") ? true : undefined, 10000));
    c.assert("save prompt uses isolated fixture directory", c.native().elements.some(n => n.role === "AXPopUpButton" && n.label === "Where:" && n.value === "downloads"));
    const state = c.native(), sheets = state.elements.filter(n => n.role === "AXSheet");
    if (sheets.length !== 1) throw new Error(`expected one native save sheet; observed ${sheets.length}`);
    c.action(observed(c, n => n.label === "Cancel" && n.actions.includes("AXPress") && n.path.startsWith(sheets[0]!.path + ".")));
    c.assert("save prompt cancelled without output", await c.waitFor(() => !c.native().elements.some(n => n.role === "AXSheet") ? true : undefined, 8000)
      && !existsSync(join(directory, name)) && !existsSync(join(process.env.HOME!, "Downloads", name)));
  });
}
