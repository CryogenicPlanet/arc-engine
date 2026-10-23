// ==== 0x41e7c00  FUN_041e7c00  (464 bytes)

undefined8 * FUN_041e7c00(undefined8 *param_1,undefined8 *param_2)

{
  ulong uVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  long lVar4;
  undefined8 *puVar5;
  undefined8 *puVar6;
  code *pcVar7;
  undefined8 *puVar8;
  ulong uVar9;
  undefined8 *puVar10;
  long lVar11;
  long *plVar12;
  
  lVar11 = param_1[1];
  uVar1 = lVar11 + 1;
  if (uVar1 >> 0x3c != 0) {
                    /* WARNING: Subroutine does not return */
    FUN_040416c0();
  }
  uVar9 = param_1[2] * 2;
  if (uVar9 < uVar1 || uVar9 - uVar1 == 0) {
    uVar9 = uVar1;
  }
  if (0x7fffffffffffffe < (ulong)param_1[2]) {
    uVar9 = 0xfffffffffffffff;
  }
  if (uVar9 == 0) {
    lVar4 = 0;
  }
  else {
    if (uVar9 >> 0x3c != 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04b087f0();
    }
    lVar4 = FUN_00008790(uVar9 << 4);
  }
  puVar2 = (undefined8 *)(lVar4 + lVar11 * 0x10);
  if (puVar2 == (undefined8 *)0x0) {
LAB_041e7cd4:
                    /* WARNING: Does not return */
    pcVar7 = (code *)SoftwareBreakpoint(1,0x41e7cd8);
    (*pcVar7)();
  }
  *puVar2 = *param_2;
  puVar2[1] = param_2[1];
  *param_2 = 0;
  param_2[1] = 0;
  puVar10 = (undefined8 *)*param_1;
  uVar1 = param_1[1];
  if ((uVar1 & 0xfffffffffffffff) != 0) {
    puVar3 = puVar10 + uVar1 * 2;
    puVar5 = puVar10;
    puVar8 = puVar2 + uVar1 * -2;
    do {
      if (puVar8 == (undefined8 *)0x0) goto LAB_041e7cd4;
      *puVar8 = *puVar5;
      puVar8[1] = puVar5[1];
      puVar6 = puVar5 + 2;
      *puVar5 = 0;
      puVar5[1] = 0;
      puVar8 = puVar8 + 2;
      puVar5 = puVar6;
    } while (puVar6 != puVar3);
    do {
      if (puVar10 == (undefined8 *)0x0) goto LAB_041e7cd4;
      plVar12 = (long *)puVar10[1];
      if (plVar12 != (long *)0x0) {
        LOAcquire();
        lVar11 = plVar12[1];
        plVar12[1] = lVar11 + -1;
        LORelease();
        if (lVar11 == 0) {
          (**(code **)(*plVar12 + 0x10))(plVar12);
          if (plVar12[2] != 0) {
            LOAcquire();
            lVar11 = plVar12[2];
            plVar12[2] = lVar11 + -1;
            LORelease();
            if (lVar11 != 0) goto LAB_041e7ce4;
          }
          pcVar7 = *(code **)(*plVar12 + 0x20);
          if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40)) {
            FUN_00005810(plVar12,0x20);
          }
          else {
            (*pcVar7)(plVar12);
          }
        }
      }
LAB_041e7ce4:
      puVar10 = puVar10 + 2;
    } while (puVar10 != puVar3);
    puVar10 = (undefined8 *)*param_1;
  }
  *param_1 = puVar2 + uVar1 * -2;
  param_1[1] = (long)(uVar1 * 0x10 + 0x10) >> 4;
  lVar11 = param_1[2];
  param_1[2] = uVar9;
  if (puVar10 != (undefined8 *)0x0) {
    FUN_00005810(puVar10,lVar11 << 4);
  }
  return puVar2 + 2;
}


