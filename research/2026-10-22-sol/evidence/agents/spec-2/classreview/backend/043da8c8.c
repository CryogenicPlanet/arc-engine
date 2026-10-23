// ==== 0x43da8c8  FUN_043da8c8  (264 bytes)

/* WARNING: Removing unreachable block (ram,0x043da924) */
/* WARNING: Removing unreachable block (ram,0x043da944) */

void FUN_043da8c8(undefined8 *param_1)

{
  code *pcVar1;
  long *plVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  
  plVar2 = (long *)*param_1;
  lVar3 = *plVar2;
  if (lVar3 == 0) {
    return;
  }
  lVar4 = plVar2[1] * 0x30;
  lVar5 = lVar3;
  if (lVar4 != 0) {
    lVar5 = 0;
    lVar6 = lVar3;
    do {
      if (lVar6 + lVar4 == 0x30) {
                    /* WARNING: Does not return */
        pcVar1 = (code *)SoftwareBreakpoint(1,0x43da9d0);
        (*pcVar1)();
      }
      lVar6 = lVar6 + -0x30;
      lVar5 = lVar5 + 0x30;
    } while (lVar4 - lVar5 != 0);
    lVar5 = *plVar2;
  }
  plVar2[1] = (lVar3 - lVar5 >> 4) * -0x5555555555555555;
  FUN_00005810(*(undefined8 *)*param_1,((undefined8 *)*param_1)[2] * 0x30);
  return;
}


