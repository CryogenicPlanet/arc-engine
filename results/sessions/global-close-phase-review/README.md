# Original global browser-close phase: bounded static proof

The original global close producer is called during destruction of the factory-created native browser object, after collection removal on the inspected posted normal-close path. It is not the earlier synchronous close-observer phase. This strengthens the earlier `destructor-shaped` description to a concrete deleting-destructor/vtable/allocator chain. No runtime was executed and no production patch is proposed.

## Exact object and destructor identity

- Factory3510610 allocates **0x110 bytes** at35106b4..bc. It installs primary vtabledd0c070 and secondary interface vtable **dd0c250** at object+0x28 (35106cc..f8). x22 is that +0x28 interface;3510744..48 selects it into x21. Factory3510b30..3c obtains the profile collection and calls3510ea4 with this interface.3511060..c0 appends it into the collection's+0x50 pointer list.
- Secondary vtabledd0c250 contains nondeleting destructor **c1e156c** and deleting destructor **36a8904** at virtual+8 (dd0c258). c1e156c subtracts0x28 and branches through **c4a3420**, which jumps to **36a8920**.36a8904 also subtracts0x28, calls36a8920, and then deallocates **0x110 bytes** at36a8914..1c. These bytes establish destructor identity independently of a guessed C++ class name. The interface's virtual+0x68 targets471fe8c, which returns the same object's weak reference with +0x28 interface adjustment.
- Destructor36a8920 installs the same vtables and sets byte+0x91. It obtains userdata keydf6a920 and calls **42feb24 at36a89f4** when present. The global callback route is inline inside this call, as documented in the earlier shutdown audit.

## Posted normal-close to physical destruction

The previously verified window-close route43cbe38→35ea198 marks closing/notifies observers synchronously, then **35ea500..564** captures the native browser weak reference and posts closure **3b26570**. That closure validates weak liveness and calls **33ca620 at3b265d8**.33ca620 obtains the profile collection and tail-calls **33ca660**. The collection removes the pointer from its public list and moves its owned record to a local at33ca6e0..710; it commits the reduced owned-record count at33ca784..78c, then calls the removed interface's **virtual+8 at33ca7a4..ac**. For the factory object's dd0c250 vtable, that dispatch is **36a8904→36a8920→42feb24→42babd0→43eb200→ObjC didCloseBrowser**.

Thus, on this path the native browser is already absent from the collection when the global callback executes, but its allocation and destructor's later members still exist. The callback is within the posted task's destruction work, rather than the earlier inline native close notification. This explains the established outside-performClose runtime ordering without prescribing an arbitrary task post in the replacement.

## Lifetime order and limits

After42feb24 returns, destructor36a8920 continues native-window/member work:35ea198 may be reached at36a8b94 if the native closing bit was not already set;0x48/0xf0 owned fields are torn down at36a8b98..bc4; weak state+0x100/+0x108 is invalidated at36a8c28..58; further fields are destroyed afterward. The deleting thunk frees the allocation only after all of36a8920 returns. Producer42feb24 sets its Arc closed byte+0x178 before calling observers, performs observer delivery inline at42fee98, then calls42f9f84 at42fef08 after observer iteration. These facts do not imply that callback-time public isInitialized or arbitrary getters return a particular value; their accessor guards remain separate.

No universal shutdown/nested-runloop/profile-teardown ordering is proved. A static pointerda7d048 to42feb24 remains an additional possible indirect entry; this bounded investigation does not enumerate every invocation of that pointer. No universal number of main-loop turns, required dispatch mechanism for Chromium156, session-history durability barrier, or observer/delegate survival across unrelated teardown is established. The class's source spelling is not needed for the proven object/interface/destructor identity and is deliberately not guessed. No speculative posting patch, source edits, app/build/queue/process-control/Chirp/credential operations occurred.

Evidence: original-disassembly.txt; prior results/sessions/global-close-shutdown-audit/README.md and results/sessions/native-evidence/cancellation.md. Static binary identity is recorded in manifest.json.
