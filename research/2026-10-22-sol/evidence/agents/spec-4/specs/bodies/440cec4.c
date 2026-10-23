// ==== 0x440cec4  ArcTracingController::stop  (20 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcTracingController::stop(ID param_1,SEL param_2)

{
  code *pcVar1;
  bool bVar2;
  
  if ((*(byte *)(param_1 + 0x30) & 1) != 0) {
    bVar2 = FUN_0434b768(*(undefined8 *)(param_1 + 0x20));
    return bVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x440ced8);
  (*pcVar1)();
}


