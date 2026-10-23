#!/usr/bin/env python3
import csv,json,sqlite3
from pathlib import Path
O=Path(__file__).resolve().parents[1];db=sqlite3.connect('file:/home/exedev/work/inputs/corpus-index.sqlite?mode=ro',uri=True)
D=[
('ArcHSLAMatrix','0x43dee04','Empty/lost scalar return','Independently repaired by LDR S0,[X0,#8]; RET; do not implement empty C body.'),
('ArcHSLAMatrix','0x43dee9c','Numeric conversions in SIMD aggregate C','Independently repaired by exact byte-move simulation: self+8..+87 -> X8+0..79.'),
('ArcHistogramSample','0x40b1230','Empty/lost scalar return','Independently repaired by LDRB W0,[X0,#8]; RET.'),
('ArcContentNode','0x4430bdc','Ghidra omits NSString format vararg','Recovered format #%08X at 0xe244ab8 and color from W8/STR X8,[SP]; F02 changes field-return claim.'),
('ArcCommandBarRankingCandidate','0x4434540','Feature arguments recovered as in_stack_00000004, split param_10 bytes and CONCAT44(...,param_12)','Partial initializer row is a feature/layout observation; native argument-to-field correspondence still unverified.'),
('ArcBrowserContextPageContentExtraction','0x42f0254','CoreServices framework string added to native object pointer','Do not accept as object layout; successful continuation and ownership remain unresolved.'),
('ArcMediaPosition','0x32b5dd0','Helper double signature versus integer use by currentTime caller','Source-backed behavior hypothesis only; raw calling/return ABI and saturation need instructions.'),
('ArcHungRendererDialog','0x4403cb8','pid_t return/int* self despite void ObjC metadata','Byte engagement offset is +0x18, not +6; signature not adopted.'),
('ArcHungRendererDialog','0x435f1ac','Unrecovered indirect-jump warning on last-owner callback cleanup','Local refcount/invocation observed; indirect destruction not exhaustively resolved.'),
('ArcWebContentsPDFController','0x443e0b4','Extra undefined4/ID* arguments in C relative to metadata B32@0:8@?16^@24','Use exact metadata identity; do not implement recovered C prototype; immediate status/callback distinction still supported.'),
('ArcBrowserContextDownloads','0x836ba70','Callee param_3 used while displayed caller 0x43328cc passes two arguments','Do not use decompiler signature as calling contract; detached/missing-manager messages supported.'),
('ArcWebContents','0x4440dec','in_x4 and shifted aggregate argument in named setter C','Weak captures/registration local evidence supported; aggregate ABI and bridge callback prototypes unknown.')]
rows=[]
for cls,a,problem,conclusion in D:
 x=db.execute('select name,path,offset,length from functions where va=?',(int(a,16),)).fetchone();assert x
 rows.append(dict(class_name=cls,va=a,problem=problem,review_disposition=conclusion,body_name=x[0],source=x[1],byte_offset=x[2],c_bytes=x[3]))
with (O/'evidence/decompiler-limits.tsv').open('w') as f:
 w=csv.DictWriter(f,list(rows[0]),delimiter='\t');w.writeheader();w.writerows(rows)
