// ==== 0x42e712c  FUN_042e712c  (464 bytes)

void FUN_042e712c(long param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,uint param_5
                 )

{
  ulong uVar1;
  long *plVar2;
  code *pcVar3;
  long lVar4;
  long *plVar5;
  ulong uVar6;
  ulong uVar7;
  ulong uVar8;
  long *plVar9;
  ulong uVar10;
  undefined1 uVar11;
  long lVar12;
  
  lVar4 = FUN_042dda40(param_1 + 0x70);
  if (param_5 < 0xf) {
    uVar11 = (&DAT_0c9d67cc)[param_5];
  }
  else {
    uVar11 = 2;
  }
  if (lVar4 == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x42e72fc);
    (*pcVar3)();
  }
  lVar12 = *(long *)(lVar4 + 0x38) << 5;
  if (lVar12 != 0) {
    lVar4 = *(long *)(lVar4 + 0x30);
    do {
      (**(code **)(lVar4 + 0x10))(lVar4,param_2,uVar11);
      lVar4 = lVar4 + 0x20;
      lVar12 = lVar12 + -0x20;
    } while (lVar12 != 0);
  }
  plVar5 = (long *)FUN_042dda40(param_1 + 0x70,param_2);
  if (plVar5 == (long *)0x0) {
    return;
  }
  uVar7 = *(ulong *)(param_1 + 0x78);
  uVar6 = plVar5[1];
  uVar8 = uVar7 - 1;
  if ((uVar7 & uVar8) == 0) {
    uVar6 = uVar6 & uVar8;
  }
  else if (uVar7 <= uVar6) {
    uVar10 = 0;
    if (uVar7 != 0) {
      uVar10 = uVar6 / uVar7;
    }
    uVar6 = uVar6 - uVar10 * uVar7;
  }
  plVar2 = *(long **)(*(long *)(param_1 + 0x70) + uVar6 * 8);
  do {
    plVar9 = plVar2;
    plVar2 = (long *)*plVar9;
  } while ((long *)*plVar9 != plVar5);
  if (plVar9 != (long *)(param_1 + 0x80)) {
    uVar10 = plVar9[1];
    if ((uVar7 & uVar8) == 0) {
      uVar10 = uVar10 & uVar8;
    }
    else if (uVar7 <= uVar10) {
      uVar1 = 0;
      if (uVar7 != 0) {
        uVar1 = uVar10 / uVar7;
      }
      uVar10 = uVar10 - uVar1 * uVar7;
    }
    if (uVar10 == uVar6) goto LAB_042e7280;
  }
  if (*plVar5 != 0) {
    uVar10 = *(ulong *)(*plVar5 + 8);
    if ((uVar7 & uVar8) == 0) {
      uVar10 = uVar10 & uVar8;
    }
    else if (uVar7 <= uVar10) {
      uVar1 = 0;
      if (uVar7 != 0) {
        uVar1 = uVar10 / uVar7;
      }
      uVar10 = uVar10 - uVar1 * uVar7;
    }
    if (uVar10 == uVar6) goto LAB_042e7280;
  }
  *(undefined8 *)(*(long *)(param_1 + 0x70) + uVar6 * 8) = 0;
LAB_042e7280:
  lVar4 = *plVar5;
  if (lVar4 != 0) {
    uVar10 = *(ulong *)(lVar4 + 8);
    if ((uVar7 & uVar8) == 0) {
      uVar10 = uVar10 & uVar8;
    }
    else if (uVar7 <= uVar10) {
      uVar8 = 0;
      if (uVar7 != 0) {
        uVar8 = uVar10 / uVar7;
      }
      uVar10 = uVar10 - uVar8 * uVar7;
    }
    if (uVar10 != uVar6) {
      *(long **)(*(long *)(param_1 + 0x70) + uVar10 * 8) = plVar9;
      lVar4 = *plVar5;
    }
  }
  *plVar9 = lVar4;
  *plVar5 = 0;
  *(long *)(param_1 + 0x88) = *(long *)(param_1 + 0x88) + -1;
  FUN_042ec96c(plVar5 + 2);
  FUN_00005810(plVar5,0x48);
  return;
}


