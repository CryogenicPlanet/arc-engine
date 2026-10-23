// ==== 0x4408564  ArcNavigationAction::navigationDisposition  (20 bytes)

/* Function Stack Size: 0x10 bytes */

long_long ArcNavigationAction::navigationDisposition(ID param_1,SEL param_2)

{
  code *pcVar1;
  
  if (*(uint *)(param_1 + 0xc) < 0xc) {
    return (ulong)*(uint *)(param_1 + 0xc);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x4408578);
  (*pcVar1)();
}


