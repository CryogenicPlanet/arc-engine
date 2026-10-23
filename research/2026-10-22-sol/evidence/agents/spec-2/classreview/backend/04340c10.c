// ==== 0x4340c10  FUN_04340c10  (512 bytes)

void FUN_04340c10(undefined8 *param_1,undefined8 *param_2)

{
  undefined8 *puVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  code *pcVar5;
  long lVar6;
  undefined8 *puVar7;
  long *plVar8;
  long lVar9;
  long lVar10;
  long local_78;
  long local_70;
  long local_68;
  
  lVar6 = param_2[1];
  if (lVar6 == 0) {
                    /* WARNING: Does not return */
    pcVar5 = (code *)SoftwareBreakpoint(0,0x4340e10);
    (*pcVar5)();
  }
  plVar8 = (long *)*param_1;
  puVar7 = (undefined8 *)*param_2;
  *param_2 = 0;
  param_2[1] = 0;
  lVar10 = param_2[2];
  param_2[2] = 0;
  local_78 = 0;
  local_70 = 0;
  local_68 = 0;
  puVar1 = puVar7;
  lVar4 = local_78;
  lVar2 = local_70;
  lVar3 = local_68;
  for (lVar9 = lVar6 * 0x10; local_78 = lVar4, local_70 = lVar2, local_68 = lVar3, lVar9 != 0;
      lVar9 = lVar9 + -0x10) {
    if (lVar2 == lVar3) {
      lVar4 = FUN_04340f60(&local_78,*(undefined8 *)*puVar1);
    }
    else {
      if (lVar4 + lVar2 * 0x298 == 0) goto LAB_04340cd8;
      lVar4 = FUN_06c7effc();
      lVar4 = lVar4 + 0x298;
    }
    local_70 = (lVar4 - local_78 >> 3) * -0x5c87b5f9d4d1bc25;
    if (lVar4 - local_78 == 0) goto LAB_04340cd8;
    puVar1 = puVar1 + 2;
    lVar4 = local_78;
    lVar2 = local_70;
    lVar3 = local_68;
  }
  lVar9 = *plVar8;
  FUN_06c87d0c(lVar9 + 0xa8);
  *(long *)(lVar9 + 0xa8) = lVar4;
  *(long *)(lVar9 + 0xb0) = lVar2;
  *(long *)(lVar9 + 0xb8) = lVar3;
  if (puVar7 == (undefined8 *)0x0) {
    return;
  }
  if (lVar6 * 0x10 != 0) {
    lVar9 = lVar6 * -0x10;
    puVar1 = puVar7 + lVar6 * 2;
    do {
      if (puVar1 + -2 == (undefined8 *)0x0) {
LAB_04340cd8:
                    /* WARNING: Does not return */
        pcVar5 = (code *)SoftwareBreakpoint(1,0x4340cdc);
        (*pcVar5)();
      }
      plVar8 = (long *)puVar1[-1];
      if (plVar8 != (long *)0x0) {
        LOAcquire();
        lVar6 = plVar8[1];
        plVar8[1] = lVar6 + -1;
        LORelease();
        if (lVar6 == 0) {
          (**(code **)(*plVar8 + 0x10))(plVar8);
          if (plVar8[2] != 0) {
            LOAcquire();
            lVar6 = plVar8[2];
            plVar8[2] = lVar6 + -1;
            LORelease();
            if (lVar6 != 0) goto LAB_04340d3c;
          }
          pcVar5 = *(code **)(*plVar8 + 0x20);
          if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
            FUN_00005810(plVar8,0x20);
          }
          else {
            (*pcVar5)(plVar8);
          }
        }
      }
LAB_04340d3c:
      lVar9 = lVar9 + 0x10;
      puVar1 = puVar1 + -2;
    } while (lVar9 != 0);
  }
  FUN_00005810(puVar7,lVar10 << 4);
  return;
}


