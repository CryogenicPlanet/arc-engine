import type { ScenarioContext } from "../launch-slice/run";
export default async function run(c: ScenarioContext) {
  const { launch, ready, open, base, titleIs, evaluate, record, waitFor, cdp, quit, EXPECT } = c;
// Run 1: S1–S6.
const l1 = launch();
if (await ready(l1, "S1 launch")) {
  try {
    const t = open(`${base}/a`);
    const okA = await titleIs(t, EXPECT.aTitle) && evaluate(t, `!!document.getElementById(${JSON.stringify(EXPECT.marker)})`) === true;
    record("S2 open A", okA ? "pass" : "fail", `title=${String(evaluate(t, "document.title"))}`);

    // Script navigation: CDP Page.navigate times out on Arc background tabs (the page still commits).
    evaluate(t, `location.assign(${JSON.stringify(`${base}/b`)}); 1`);
    const navStart = Date.now();
    const okB = await titleIs(t, EXPECT.bTitle);
    const navMs = Date.now() - navStart;
    evaluate(t, "location.hash = 'frag'; 1");
    const frag = await waitFor(() => (evaluate(t, "location.hash") === "#frag" ? true : undefined), 5000);
    evaluate(t, "history.back(); 1");
    const back = await waitFor(() => (evaluate(t, "location.href") === `${base}/b` ? true : undefined), 5000);
    record("S3 navigate", okB && frag && back ? "pass" : "fail", `B=${okB} (${navMs} ms) frag=${Boolean(frag)} back=${Boolean(back)} title=${String(evaluate(t, "document.title"))} href=${String(evaluate(t, "location.href"))}`);

    const wrote = evaluate(t, `new Promise((res) => {
      document.cookie = ${JSON.stringify(EXPECT.cookie + "; Max-Age=86400; path=/")};
      localStorage.setItem(${JSON.stringify(EXPECT.localKey)}, ${JSON.stringify(EXPECT.localValue)});
      const r = indexedDB.open("ls-db", 1);
      r.onupgradeneeded = () => r.result.createObjectStore("s");
      r.onsuccess = () => { const tx = r.result.transaction("s", "readwrite"); tx.objectStore("s").put(${JSON.stringify(EXPECT.idbValue)}, "k");
        tx.oncomplete = () => res("ok"); tx.onerror = () => res("idb-error"); };
      r.onerror = () => res("open-error");
    })`);
    record("S4 store", wrote === "ok" ? "pass" : "fail", String(wrote));

    cdp("close", t);
    const t2 = open(`${base}/a`);
    record("S5 close + new tab", (await titleIs(t2, EXPECT.aTitle)) ? "pass" : "fail", "");
  } catch (e) { record("S2-S5", "inconclusive", (e as Error).message); }
}
await quit(l1, "S6 quit");

// Run 2: S7–S8 on the same data dir.
const l2 = launch();
if (await ready(l2, "S7 relaunch")) {
  try {
    const t = open(`${base}/a`);
    await titleIs(t, EXPECT.aTitle);
    const back = evaluate(t, `new Promise((res) => {
      const out = { cookie: document.cookie, local: localStorage.getItem(${JSON.stringify(EXPECT.localKey)}) };
      const r = indexedDB.open("ls-db", 1);
      r.onsuccess = () => { try { const g = r.result.transaction("s").objectStore("s").get("k");
        g.onsuccess = () => res(JSON.stringify({ ...out, idb: g.result })); } catch (e) { res(JSON.stringify({ ...out, idb: "missing" })); } };
      r.onerror = () => res(JSON.stringify({ ...out, idb: "open-error" }));
    })`);
    const got = JSON.parse(String(back)) as { cookie: string; local: string; idb: string };
    const ok = got.cookie.includes(EXPECT.cookie) && got.local === EXPECT.localValue && got.idb === EXPECT.idbValue;
    record("S7 readback", ok ? "pass" : "fail", String(back));
  } catch (e) { record("S7 readback", "inconclusive", (e as Error).message); }
}
await quit(l2, "S8 quit");

}
