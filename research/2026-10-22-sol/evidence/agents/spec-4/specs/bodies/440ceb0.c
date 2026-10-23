// ==== 0x440ceb0  ArcTracingController::abort  (20 bytes)

/* Function Stack Size: 0x10 bytes */

void __thiscall ArcTracingController::abort(ArcTracingController *this)

{
  code *pcVar1;
  
  if (((byte)this[0x30] & 1) != 0) {
    FUN_0434b818(*(undefined8 *)(this + 0x20));
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440cec4);
  (*pcVar1)();
}


