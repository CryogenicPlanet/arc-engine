// ==== 0x43dec3c  ArcAccessibilityActionData::action  (20 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_long_long ArcAccessibilityActionData::action(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if (*(uint *)(param_1 + 8) < 4) {
    return (ulong)*(uint *)(param_1 + 8);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0,0x43dec50);
  (*pcVar1)();
}


