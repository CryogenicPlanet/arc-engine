import { writeFileSync } from "node:fs";
import type { ScenarioContext } from "../launch-slice/run";
import { run } from "./native";
const stream = "BT /F1 18 Tf 20 70 Td (ArcCore PDF 7f3a) Tj ET\n";
const objects = ["<< /Type /Catalog /Pages 2 0 R >>", "<< /Type /Pages /Kids [3 0 R] /Count 1 >>",
  "<< /Type /Page /Parent 2 0 R /MediaBox [0 0 300 144] /Contents 4 0 R /Resources << /Font << /F1 5 0 R >> >> >>",
  `<< /Length ${stream.length} >>\nstream\n${stream}endstream`, "<< /Type /Font /Subtype /Type1 /BaseFont /Helvetica >>"];
let pdf = "%PDF-1.4\n";
const offsets = objects.map((o, i) => { const offset = pdf.length; pdf += `${i + 1} 0 obj\n${o}\nendobj\n`; return offset; });
const xref = pdf.length;
pdf += `xref\n0 6\n0000000000 65535 f \n${offsets.map(o => `${String(o).padStart(10, "0")} 00000 n \n`).join("")}trailer\n<< /Size 6 /Root 1 0 R >>\nstartxref\n${xref}\n%%EOF\n`;
export const fixture = (req: Request) => new URL(req.url).pathname === "/fixture.pdf"
  ? new Response(pdf, { headers: { "content-type": "application/pdf" } }) : undefined;
export default async function scenario(c: ScenarioContext) {
  await run(c, async () => {
    const t = c.open(`${c.base}/fixture.pdf`);
    c.assert("PDF document MIME", await c.waitFor(() => c.evaluate(t, "document.contentType") === "application/pdf" ? true : undefined, 15000));
    const client = await c.client();
    const tree = await client.targetCommand(t, "DOM.getDocument", {depth:-1, pierce:true});
    writeFileSync(`${c.outDir}/pdf-dom.json`, JSON.stringify(tree, null, 2));
    const frames: Record<string, unknown>[] = [];
    const walk = (node: Record<string, unknown>) => {
      if (node.nodeName === "IFRAME") frames.push(node);
      for (const key of ["children", "shadowRoots"]) for (const child of (node[key] ?? []) as Record<string, unknown>[]) walk(child);
    };
    walk(tree.root as Record<string, unknown>);
    const mimeFrames = frames.filter(frame => {
      const attributes = frame.attributes as string[]; const i = attributes.indexOf("type");
      return i >= 0 && attributes[i+1] === "application/pdf";
    });
    c.assert("PDF MIME viewer iframe attached", mimeFrames.length === 1);
    c.cdp("screenshot", t, `${c.outDir}/pdf.png`);
    c.cdp("close", t);
    c.assert("PDF target closed", !(c.cdp("list") as { id: string }[]).some(n => n.id === t));
  });
}
