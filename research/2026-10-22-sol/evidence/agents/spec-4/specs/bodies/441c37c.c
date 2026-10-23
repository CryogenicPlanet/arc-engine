// ==== 0x441c37c  ArcWebContentsMediaSessionPiPController::sourceElementBounds  (68 bytes)

/* Function Stack Size: 0x10 bytes */

CGRect ArcWebContentsMediaSessionPiPController::sourceElementBounds(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  CGRect CVar5;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    lVar2 = *(long *)(*(long *)(param_1 + 8) + 0x10);
    if (lVar2 == 0) {
      uVar4 = 0;
      uVar3 = 0;
    }
    else {
      lVar2 = *(long *)(lVar2 + 0x28);
      uVar4 = *(undefined8 *)(lVar2 + 0x110);
      uVar3 = *(undefined8 *)(lVar2 + 0x118);
    }
    CVar5.field0_0x0.field0_0x0 = (double)(int)uVar4;
    CVar5.field0_0x0.field1_0x8 = (double)(int)((ulong)uVar4 >> 0x20);
    CVar5.field1_0x10.field0_0x0 = (double)(int)uVar3;
    CVar5.field1_0x10.field1_0x8 = (double)(int)((ulong)uVar3 >> 0x20);
    return CVar5;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x441c3c0);
  (*pcVar1)();
}


