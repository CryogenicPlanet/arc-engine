// ==== 0x440ab4c  ArcSecureContext::state  (60 bytes)

/* Function Stack Size: 0x10 bytes */

unsigned_long_long ArcSecureContext::state(ID param_1,SEL param_2)

{
  unsigned_long_long uVar1;
  unsigned_long_long uVar2;
  code *pcVar3;
  int iVar4;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    iVar4 = FUN_03c2b700(**(undefined8 **)(param_1 + 8));
    uVar1 = 1;
    if (iVar4 != 3) {
      uVar1 = 2;
    }
    uVar2 = 0;
    if (iVar4 != 0) {
      uVar2 = uVar1;
    }
    return uVar2;
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x440ab88);
  (*pcVar3)();
}


