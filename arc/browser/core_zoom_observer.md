# Genuine zoom observer publication — UNAPPLIED companion

Apply AFTER four-part native/WC/display/observer bundle + close primitive +
core_close_accounting_adapters.patch. Patch SHA256
37637ae2799d04681f52a0a782ab53e01d720d436c246f16df26c7cce450dcf5.
One owned WC file; no new deps/native/shared changes. Exact staged WC SHA256
 eb1ca0f75e01dd4f2336d8d73d4e44dd135264cac3133104844915a5b8bb3ca2.

Original producer4398940 takes event new zoom level(+18), computes pow(1.2,level)
at receipt, captures double factor, posts a real WCObserver batch, converts to
float before weak ObjC owner delivery4413834, and posts a separate finalizer.
Initial setters are direct/uncounted and precede observer registration. Current
manual synchronous per-commit zoom refresh has no equivalent timing/accounting.

The candidate subscribes via current primary ZoomController's ZoomObserver API,
captures factor at receipt and uses one typed WC notification+trailer per event.
It adds no WC/frame/commit/same-document filter: original checks ONLY live logical
shared owner before factor capture. ScopedObservation unregisters from the actual
passed controller on destruction, before any wrapper close callout; no lookup of
a controller that FromWebContents already reports null. Observer destruction
also resets through ScopedObservation. Missing controller skips registration.
The manual per-commit refresh, private declaration and helper are removed;
initial direct snapshot remains unchanged.

Native constructor vtable+100 slot18 ->4399120 ->4398940. Association4373cc4
uses SAME controller lookup882ba0 as initial zoom getter4597a58, registers
Impl+100 in controller+48; initializer43705b4 and native transfer437b308 call it.
Controller destruction4399150 removes from the passed controller directly.
Original commit-to-event sender remains untraced. Current156 committed navigation
DidFinish ->UpdateState ->OnZoomChanged is verified current source, separately.
Do not call that a recovered original commit stack.

Checks: actual WC compiler0/no diagnostics, strict incremental apply0 and exact
reconstruction, fresh current-byte source review clean. Evidence/staged source
logs/wp/core.zoom-observer/; bounded original/current source audit under
logs/wp/core.initial-zoom-accounting/.

No runtime/link coverage for this composition; public Close remains unchanged.
Logical discard/rebind/native replacement still requires its complete lifetime
model and native association counterpart; no partial fast-close activation or
claim of full zoom order/count parity is made.
