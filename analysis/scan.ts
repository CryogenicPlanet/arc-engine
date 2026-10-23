// Estimates porting effort: for each 1.168.0 patch site, search a newer thin arm64 binary
// for the original instruction bytes, exactly and with PC-relative/immediate fields masked.
import { readFileSync } from "node:fs";
import { patches } from "./patch-local-mode";
const target = readFileSync(Bun.argv[2]!);
const maskWord = (w: number): number => {
  if ((w & 0x7c000000) === 0x14000000) return w & 0xfc000000;           // B / BL imm26
  if ((w & 0x9f000000) === 0x90000000) return w & 0x9f00001f;           // ADRP
  if ((w & 0x9f000000) === 0x10000000) return w & 0x9f00001f;           // ADR
  if ((w & 0x7f800000) === 0x11000000) return w & 0xffc003ff;           // ADD/SUB imm
  if ((w & 0xff000010) === 0x54000000) return w & 0xff00001f;           // B.cond
  if ((w & 0x7e000000) === 0x34000000) return w & 0xff00001f;           // CBZ/CBNZ
  if ((w & 0x7e000000) === 0x36000000) return w & 0xfff8001f;           // TBZ/TBNZ
  if ((w & 0x3b000000) === 0x39000000) return w & 0xffc003ff;           // LDR/STR unsigned imm
  return w >>> 0;
};
const words = (b: Buffer): number[] => Array.from({ length: b.length >> 2 }, (_, i) => b.readUInt32LE(i * 4));
const tw = words(target).map(maskWord);
const exact = (p: Buffer): number => { let n = 0, i = target.indexOf(p); while (i >= 0) { if (i % 4 === 0) n++; i = target.indexOf(p, i + 1); } return n; };
const masked = (p: Buffer): number => {
  const pw = words(p).map(maskWord); let n = 0;
  for (let i = 0; i + pw.length <= tw.length; i++) { let ok = true; for (let j = 0; j < pw.length; j++) if (tw[i + j] !== pw[j]) { ok = false; break; } if (ok) n++; }
  return n;
};
for (const p of patches) {
  if (p.original.every((b) => b === 0)) { console.log(`0x${p.address.toString(16)}  (header padding bridge, ${p.replacement.length} bytes) — relocatable`); continue; }
  console.log(`0x${p.address.toString(16)}  ${p.original.length}B  exact=${exact(p.original)}  masked=${masked(p.original)}`);
}
