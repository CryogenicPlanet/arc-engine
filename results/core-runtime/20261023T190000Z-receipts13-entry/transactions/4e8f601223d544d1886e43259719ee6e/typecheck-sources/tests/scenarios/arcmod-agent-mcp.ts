import type { ScenarioContext } from "../launch-slice/run";
import type { ElementState, PageState } from "../../../arc/tools/agent-browser";
import { join, resolve } from "node:path";
import { writeFileSync } from "node:fs";
import { run, ensureNativeWindow } from "./native";

export const fixture = (request: Request) => {
  if (new URL(request.url).pathname !== "/mcp") return;
  return new Response(`<!doctype html><title>ArcCore MCP fixture 7f3a</title>
<label>Fixture text<input id="text"></label><label>Fixture password<input type="password" value="fixture-secret-7f3a"></label>
<button id="commit">Commit</button><output id="result">waiting</output>
<script>let trustedInput=false;document.querySelector('#text').addEventListener('input',e=>trustedInput=e.isTrusted);
document.querySelector('#commit').onclick=e=>document.querySelector('#result').textContent=
 'committed:'+document.querySelector('#text').value+':'+trustedInput+':'+e.isTrusted;</script>`, {headers:{"content-type":"text/html"}});
};

type Reply = {id?:number; result?:{isError?:boolean;structuredContent?:Record<string,unknown>;content?:{type:string;data?:string}[]};error?:unknown};
// Actual newline-delimited JSON-RPC stdio, not a direct adapter method call.
// Attach explicitly: this child may neither build nor launch another application.
class MCP {
  private next = 0;
  private pending = new Map<number,{resolve:(value:Reply)=>void;reject:(error:Error)=>void;timer:ReturnType<typeof setTimeout>}>();
  private readonly process = Bun.spawn([process.execPath, resolve(import.meta.dir, "../../../arc/tools/agent-mcp.ts")], {stdin:"pipe",stdout:"pipe",stderr:"pipe"});
  private readonly errors = new Response(this.process.stderr).text();
  private readonly reading = this.read();
  private async read() {
    const reader = this.process.stdout.getReader(), decoder = new TextDecoder(); let buffer = "";
    try {
      for (;;) {
        const item = await reader.read(); if (item.done) break;
        buffer += decoder.decode(item.value, {stream:true});
        let end: number;
        while ((end = buffer.indexOf("\n")) >= 0) {
          const line = buffer.slice(0,end); buffer = buffer.slice(end+1); if (!line) continue;
          const reply = JSON.parse(line) as Reply, entry = reply.id === undefined ? undefined : this.pending.get(reply.id);
          if (entry) { clearTimeout(entry.timer); this.pending.delete(reply.id!); entry.resolve(reply); }
        }
      }
    } catch (error) { this.rejectAll(new Error(`MCP stdout: ${error}`)); }
    finally { reader.releaseLock(); this.rejectAll(new Error("MCP stdout ended")); }
  }
  private rejectAll(error: Error) {
    for (const entry of this.pending.values()) { clearTimeout(entry.timer); entry.reject(error); }
    this.pending.clear();
  }
  async rpc(method: string, params: Record<string,unknown> = {}): Promise<Reply> {
    const id = ++this.next;
    const reply = new Promise<Reply>((resolve,reject) => {
      const timer = setTimeout(() => { this.pending.delete(id); reject(new Error(`MCP request timeout: ${method}`)); },30000);
      this.pending.set(id,{resolve,reject,timer});
    });
    this.process.stdin.write(JSON.stringify({jsonrpc:"2.0",id,method,params})+"\n");
    return reply;
  }
  initialized() { this.process.stdin.write(JSON.stringify({jsonrpc:"2.0",method:"notifications/initialized"})+"\n"); }
  async tool(name: string, args: Record<string,unknown>, expectedError?: string) {
    const reply = await this.rpc("tools/call",{name,arguments:args});
    if (reply.error || !reply.result) throw new Error(`MCP protocol failure for ${name}`);
    const result = reply.result;
    const body = result.structuredContent ?? {};
    if (expectedError) {
      if (!result.isError || (body.error as {code?:string})?.code !== expectedError) throw new Error(`MCP ${name} did not reject with ${expectedError}`);
    } else if (result.isError) throw new Error(`MCP ${name}: ${JSON.stringify(body.error)}`);
    return {body,result};
  }
  async stop() {
    this.process.stdin.end();
    const code = await Promise.race([this.process.exited, Bun.sleep(5000).then(() => undefined)]);
    if (code === undefined) { this.process.kill("SIGTERM"); await this.process.exited; }
    await this.reading;
    const errors = await this.errors;
    if (code !== 0) throw new Error(`MCP shutdown failed: ${errors.slice(-1000)}`);
  }
}
const element = (state: PageState, name: string): ElementState => {
  const nodes = state.elements.filter(n => n.name === name && n.actionable);
  if (nodes.length !== 1) throw new Error(`Expected one actionable MCP element named ${name}, got ${nodes.length}`);
  return nodes[0]!;
};

export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    await ensureNativeWindow(c);
    const initial = c.native().elements.filter(n=>n.role === "AXWindow");
    c.assert("one native window before MCP attachment", initial.length === 1);
    if (initial.length !== 1) throw new Error("MCP native action test requires exactly one disposable window");
    const mcp = new MCP(); let session: string | undefined;
    const calls: string[] = [];
    const call = async (name: string,args:Record<string,unknown> = {},error?:string) => {
      calls.push(name); return mcp.tool(name,{...(session ? {session} : {}),...args},error);
    };
    try {
      const initialized = await mcp.rpc("initialize",{protocolVersion:"2025-11-25",capabilities:{},clientInfo:{name:"ArcCore scenario",version:"1"}});
      c.assert("MCP initialize protocol version", (initialized.result as Record<string,unknown>)?.protocolVersion === "2025-11-25");
      mcp.initialized();
      const listed = await mcp.rpc("tools/list");
      const expected = ["open_session","close_session","list_tabs","open_tab","read_page","screenshot","navigate","back","forward","click","type","wait_for","close_tab","native_state","native_action"].sort();
      c.assert("MCP advertises exactly fifteen expected tools", JSON.stringify(((listed.result as Record<string,unknown>)?.tools as {name:string}[] ?? []).map(t=>t.name).sort()) === JSON.stringify(expected));
      const attached = await call("open_session",{agent:c.slug,port:c.port,attach:true});
      session = typeof attached.body.session === "string" ? attached.body.session : undefined;
      const exactOwner = session !== undefined && attached.body.agent === c.slug && attached.body.pid === c.native().pid && attached.body.port === c.port;
      c.assert("MCP attached to exact harness agent PID", exactOwner);
      if (!exactOwner) throw new Error("Refusing page/native actions without exact attached ownership");
      const a = (await call("open_tab",{url:`${c.base}/a`})).body as unknown as PageState;
      let b = (await call("open_tab",{url:`${c.base}/mcp`})).body as unknown as PageState;
      c.assert("MCP owns two distinct exact page handles", a.page !== b.page && a.title === c.EXPECT.aTitle && b.title === "ArcCore MCP fixture 7f3a");
      c.assert("MCP password is redacted", b.elements.some(n=>n.name === "Fixture password" && n.value === "[redacted]"));
      const stale = element(b,"Fixture text").handle;
      b = (await call("read_page",{page:b.page})).body as unknown as PageState;
      await call("type",{page:b.page,element:stale,text:"do-not-type"},"stale_element");
      c.assert("MCP rejects stale element", true);
      b = (await call("type",{page:b.page,element:element(b,"Fixture text").handle,text:"mcp-value-7f3a"})).body as unknown as PageState;
      b = (await call("click",{page:b.page,element:element(b,"Commit").handle})).body as unknown as PageState;
      b = (await call("wait_for",{page:b.page,text:"committed:mcp-value-7f3a:true:true"})).body as unknown as PageState;
      c.assert("MCP trusted click and type reach correct page", b.text.includes("committed:mcp-value-7f3a:true:true"));
      const unchanged = (await call("read_page",{page:a.page})).body as unknown as PageState;
      c.assert("MCP sentinel page remains unchanged", unchanged.url === `${c.base}/a` && unchanged.title === c.EXPECT.aTitle && unchanged.text.includes("fixture A"));
      const navigated = (await call("navigate",{page:b.page,url:`${c.base}/b`})).body as unknown as PageState;
      c.assert("MCP navigate exact page", navigated.url === `${c.base}/b` && navigated.title === c.EXPECT.bTitle);
      const back = (await call("back",{page:b.page})).body as unknown as PageState;
      c.assert("MCP back restores fixture", back.url === `${c.base}/mcp` && back.title === "ArcCore MCP fixture 7f3a");
      const forward = (await call("forward",{page:b.page})).body as unknown as PageState;
      c.assert("MCP forward restores fixture B", forward.url === `${c.base}/b` && forward.title === c.EXPECT.bTitle);
      const shot = await call("screenshot",{page:b.page});
      const png = shot.result.content?.find(item=>item.type === "image")?.data;
      c.assert("MCP screenshot contains PNG", typeof png === "string" && Buffer.from(png,"base64").subarray(0,8).equals(Buffer.from([137,80,78,71,13,10,26,10])));
      if (png) writeFileSync(join(c.outDir,"mcp-fixture.png"),Buffer.from(png,"base64"));
      type Native = {pid:number;elements:{role:string;identifier:string;enabled:boolean;actions:string[];handle:string}[]};
      const native = (await call("native_state")).body as unknown as Native;
      c.assert("MCP native state exact agent and badge", native.pid === attached.body.pid && native.elements.some(n=>n.identifier === "ArcModBadge"));
      const old = native.elements.filter(n=>n.identifier === "newWindow" && n.enabled && n.actions.includes("AXPress"));
      if (old.length !== 1) throw new Error("New Window not uniquely actionable in MCP native snapshot");
      await call("native_state");
      await call("native_action",{element:old[0]!.handle,action:"press"},"stale_native_element");
      c.assert("MCP rejects stale native handle", true);
      const fresh = (await call("native_state")).body as unknown as Native;
      const next = fresh.elements.filter(n=>n.identifier === "newWindow" && n.enabled && n.actions.includes("AXPress"));
      if (next.length !== 1) throw new Error("New Window not uniquely actionable after refresh");
      const before = fresh.elements.filter(n=>n.role === "AXWindow").length;
      const action = await call("native_action",{element:next[0]!.handle,action:"press"});
      c.assert("MCP native action receipt belongs to exact PID", (action.body.receipt as {pid?:number;dispatched?:boolean})?.pid === attached.body.pid && (action.body.receipt as {dispatched?:boolean})?.dispatched === true);
      c.assert("MCP begins native window operation with one window", before === 1);
      c.assert("MCP native action creates second full window", await c.waitFor(() => {
        const windows = c.native().elements.filter(n=>n.role === "AXWindow");
        return windows.length === 2 && windows.every(n=>n.identifier.startsWith("bigBrowserWindow-")) ? true : undefined;
      },8000));
      const listedPages = (await call("list_tabs")).body.tabs as {page:string}[];
      c.assert("MCP list contains both exact owned handles", listedPages.some(t=>t.page === a.page) && listedPages.some(t=>t.page === b.page));
      await call("close_tab",{page:b.page});
      await call("read_page",{page:b.page},"stale_page");
      c.assert("MCP closes exact page and rejects old handle", true);
      const closed = await call("close_session"); session = undefined;
      c.assert("MCP attach detach preserves external app ownership", closed.body.closed === true && closed.body.appStopped === false);
      // MCP page handles are deliberately not CDP target IDs. Verify actual
      // surviving target by exact fixture URL/title after the controller detaches.
      c.assert("harness still owns live sentinel after MCP detach", (c.cdp("list") as {url:string;title:string}[]).some(t=>t.url === `${c.base}/a` && t.title === c.EXPECT.aTitle));
    } finally {
      if (session) await mcp.tool("close_session",{session}).catch(()=>undefined);
      writeFileSync(join(c.outDir,"mcp-calls.json"),JSON.stringify(calls,null,2));
      await mcp.stop();
    }
  });
}
