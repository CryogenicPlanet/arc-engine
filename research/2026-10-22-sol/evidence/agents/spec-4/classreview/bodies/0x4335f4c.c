// ==== 0x4335f4c  FUN_04335f4c  (976 bytes)

undefined8 * FUN_04335f4c(undefined8 *param_1,ulong param_2,int param_3)

{
  char cVar1;
  int iVar2;
  int *piVar3;
  ulong uVar4;
  undefined *puVar5;
  int *piVar6;
  undefined8 uVar7;
  long *plVar8;
  long lVar9;
  long lVar10;
  code *pcVar11;
  undefined8 uVar12;
  undefined8 *puVar13;
  long *plVar14;
  ulong uVar15;
  
  *param_1 = &PTR_FUN_0da8a590;
  piVar3 = (int *)FUN_00008790(8);
  *piVar3 = 0;
  *(undefined1 *)(piVar3 + 1) = 0;
  if ((piVar3 != (int *)0x0) && (iVar2 = *piVar3, *piVar3 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar11 = (code *)SoftwareBreakpoint(0,0x4336340);
    (*pcVar11)();
  }
  param_1[1] = piVar3;
  param_1[2] = param_1;
  *param_1 = &PTR_LAB_0da7ed48;
  *(undefined1 *)(param_1 + 3) = 0;
  *(undefined1 *)((long)param_1 + 0x1c) = 0;
  param_1[4] = 0;
  if ((param_2 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(param_2);
  }
  puVar13 = param_1 + 6;
  *puVar13 = 0;
  param_1[5] = param_2;
  param_1[7] = 0;
  param_1[8] = 0;
  piVar3 = (int *)FUN_00008790(8);
  *piVar3 = 0;
  *(undefined1 *)(piVar3 + 1) = 0;
  if ((piVar3 != (int *)0x0) && (iVar2 = *piVar3, *piVar3 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar11 = (code *)SoftwareBreakpoint(0,0x433634c);
    (*pcVar11)();
  }
  param_1[9] = piVar3;
  param_1[10] = param_1;
  lVar9 = DAT_0e53ea40;
  if (DAT_0e53ea40 == 0) {
                    /* WARNING: Does not return */
    pcVar11 = (code *)SoftwareBreakpoint(0,0x4336310);
    (*pcVar11)();
  }
  uVar15 = DAT_0e53ea40 + 8;
  if ((uVar15 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(DAT_0e53ea40 + 8);
    uVar4 = param_1[8];
  }
  else {
    uVar4 = 0;
  }
  if ((uVar4 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4();
  }
  param_1[8] = uVar15;
  if (uVar15 == 0) {
                    /* WARNING: Does not return */
    pcVar11 = (code *)SoftwareBreakpoint(0,0x433631c);
    (*pcVar11)();
  }
  plVar8 = *(long **)(lVar9 + 600);
  lVar10 = *(long *)(lVar9 + 0x260);
  plVar14 = plVar8;
  if (lVar10 << 3 != 0) {
    uVar15 = (lVar10 << 3) >> 3;
    do {
      uVar4 = uVar15 >> 1;
      puVar5 = (undefined *)(**(code **)(*(long *)plVar8[uVar4] + 0x10))();
      plVar14 = plVar8 + uVar4 + 1;
      uVar15 = uVar15 + ~(uVar15 >> 1);
      if (&UNK_0c9b52df < puVar5) {
        plVar14 = plVar8;
        uVar15 = uVar4;
      }
      plVar8 = plVar14;
    } while (uVar15 != 0);
    plVar8 = *(long **)(lVar9 + 600);
    lVar10 = *(long *)(lVar9 + 0x260);
  }
  if (plVar14 == plVar8 + lVar10) {
LAB_043362e4:
    plVar14 = plVar8 + lVar10;
  }
  else {
    pcVar11 = *(code **)(*(long *)*plVar14 + 0x10);
    if (pcVar11 == (code *)&LAB_0037d0bc) {
      puVar5 = &DAT_0ccc0158;
    }
    else if (pcVar11 == (code *)&LAB_021661bc) {
      puVar5 = &DAT_0ccc01ac;
    }
    else if (pcVar11 == (code *)&LAB_021661c8) {
      puVar5 = &DAT_0ccc01b0;
    }
    else {
      puVar5 = (undefined *)(*pcVar11)();
      plVar8 = *(long **)(lVar9 + 600);
      lVar10 = *(long *)(lVar9 + 0x260);
    }
    if (&DAT_0c9b52e0 < puVar5) goto LAB_043362e4;
  }
  if (plVar8 + lVar10 == plVar14) {
    lVar9 = 0;
  }
  else {
    pcVar11 = *(code **)(*(long *)*plVar14 + 0x10);
    if (((pcVar11 == (code *)&LAB_021661c8 || pcVar11 == (code *)&LAB_0037d0bc) ||
         pcVar11 == (code *)&LAB_021661bc) ||
       (puVar5 = (undefined *)(*pcVar11)(), puVar5 != &DAT_0c9b52e0)) {
                    /* WARNING: Does not return */
      pcVar11 = (code *)SoftwareBreakpoint(0,0x4336304);
      (*pcVar11)();
    }
    lVar9 = *plVar14;
  }
  uVar15 = 0;
  if (lVar9 != 0) {
    uVar15 = lVar9 - 0x50;
  }
  if ((uVar15 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(uVar15);
  }
  if ((param_1[4] & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4();
  }
  param_1[4] = uVar15;
  if (uVar15 == 0) {
                    /* WARNING: Does not return */
    pcVar11 = (code *)SoftwareBreakpoint(0,0x4336328);
    (*pcVar11)();
  }
  lVar9 = FUN_000805e0(param_1[5] + 8,&DAT_0ccc03ac);
  piVar3 = (int *)0x0;
  uVar12 = 0;
  if ((lVar9 != 0) && (lVar9 != 0x20)) {
    lVar9 = *(long *)(lVar9 + 0x10);
    piVar3 = *(int **)(lVar9 + 0x370);
    if ((piVar3 != (int *)0x0) && (iVar2 = *piVar3, *piVar3 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar11 = (code *)SoftwareBreakpoint(0,0x4336334);
      (*pcVar11)();
    }
    uVar12 = *(undefined8 *)(lVar9 + 0x378);
  }
  piVar6 = (int *)*puVar13;
  *puVar13 = piVar3;
  if (piVar6 != (int *)0x0) {
    LOAcquire();
    iVar2 = *piVar6;
    *piVar6 = iVar2 + -1;
    LORelease();
    if (iVar2 == 1) {
      FUN_00005810(piVar6,8);
    }
    piVar3 = (int *)*puVar13;
  }
  param_1[7] = uVar12;
  if (piVar3 == (int *)0x0) {
    return param_1;
  }
  if ((char)piVar3[1] != '\0') {
    return param_1;
  }
  if (param_1[7] == 0) {
    return param_1;
  }
  FUN_00575e00(param_1[8] + 0x150,param_1);
  *(int *)(param_1 + 3) = param_3;
  *(undefined1 *)((long)param_1 + 0x1c) = 1;
  if (param_1[6] == 0) {
    uVar7 = 0;
    uVar12 = 0;
    if (param_3 == 0) goto LAB_043362d4;
  }
  else {
    cVar1 = *(char *)(param_1[6] + 4);
    if (param_3 == 0) {
      if (cVar1 == '\0') {
        uVar7 = param_1[7];
      }
      else {
        uVar7 = 0;
      }
LAB_043362d4:
      FUN_09fc7ee0(param_1[4],uVar7);
      return param_1;
    }
    if (cVar1 == '\0') {
      uVar12 = param_1[7];
    }
    else {
      uVar12 = 0;
    }
  }
  FUN_0459a040(param_1[4],uVar12,0x15);
  return param_1;
}


