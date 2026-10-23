Role: Arc-specific engine inventory and targeted reverse engineering. Read common constraints above.

Measure named Arc namespaces/methods and sizes in saved engine functions.tsv, inventory ObjC class/protocol metadata, and correlate Arc-only strings and compiled source paths from context/results. Reproduce counts rather than inheriting 146/142/1524 from prior prose. Distinguish classes, namespaces, functions, methods, thunks and category methods.

Create a subsystem inventory for browser app/context/browser/web contents, UI controllers, importer, extensions, IPC/mojom, AI/content conversion, ad blocking/media, and any other evidenced Arc layer. Small streaming binary/string/xref tools are allowed; do not load Ghidra or attempt a full decompile on this pool. Record exact VAs/file offsets/image hash and symbol anchors where possible. Explicitly state that named Arc methods are a lower bound, not all custom native code.

Deliver initial/report.md, initial/arc-functions.tsv, initial/subsystems.tsv, initial/decompile-requests.tsv with address, expected shard (address >>20 formatted 0x%x.c), reason and priority, plus scripts reproducing counts. Identify unnamed function candidates with evidenced Arc string/call references if practical. Prioritize paths necessary for launching an Arc-compatible framework; separate optional service features from essential behavior without guessing.
