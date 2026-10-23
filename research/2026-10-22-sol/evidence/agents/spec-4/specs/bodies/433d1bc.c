// ==== 0x433d1bc  FUN_0433d1bc  (1484 bytes)

undefined8 * FUN_0433d1bc(undefined8 *param_1)

{
  undefined1 **ppuVar1;
  int iVar2;
  int *piVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  undefined8 uVar6;
  undefined *puVar7;
  long *plVar8;
  long lVar9;
  long lVar10;
  code *pcVar11;
  long *plVar12;
  ulong *puVar13;
  undefined8 *puVar14;
  undefined8 uVar15;
  ulong uVar16;
  ulong uVar17;
  undefined8 uVar18;
  undefined1 *local_90;
  long lStack_88;
  undefined7 local_80;
  char cStack_79;
  undefined8 local_78;
  
  *param_1 = &PTR_FUN_0da8a590;
  piVar3 = (int *)FUN_00008790(8);
  *piVar3 = 0;
  *(undefined1 *)(piVar3 + 1) = 0;
  if ((piVar3 != (int *)0x0) && (iVar2 = *piVar3, *piVar3 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar11 = (code *)SoftwareBreakpoint(0,0x433d79c);
    (*pcVar11)();
  }
  param_1[1] = piVar3;
  param_1[2] = param_1;
  *param_1 = &PTR_LAB_0da7f500;
  param_1[5] = FUN_040490c0;
  param_1[6] = &DAT_0c4b3500;
  param_1[7] = 0;
  puVar4 = (undefined8 *)FUN_00008790(0x18);
  if (((ulong)param_1 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(param_1);
  }
  if (((ulong)param_1 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(param_1);
  }
  *puVar4 = param_1;
  piVar3 = (int *)FUN_00008790(8);
  *piVar3 = 0;
  *(undefined1 *)(piVar3 + 1) = 0;
  if ((piVar3 != (int *)0x0) && (iVar2 = *piVar3, *piVar3 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar11 = (code *)SoftwareBreakpoint(0,0x433d7a8);
    (*pcVar11)();
  }
  puVar4[1] = piVar3;
  puVar4[2] = puVar4;
  lVar9 = DAT_0e53ea40;
  if (DAT_0e53ea40 == 0) {
                    /* WARNING: Does not return */
    pcVar11 = (code *)SoftwareBreakpoint(0,0x433d784);
    (*pcVar11)();
  }
  local_78 = 0;
  FUN_00cad330(&local_90,DAT_0e4a2fd8,"cpu-usage-reporting-threshold",0x1d);
  ppuVar1 = (undefined1 **)local_90;
  if (-1 < (long)cStack_79) {
    ppuVar1 = &local_90;
  }
  if (-1 < cStack_79) {
    lStack_88 = (long)cStack_79;
  }
  iVar2 = FUN_00d71908(ppuVar1,lStack_88,&local_78);
  uVar18 = local_78;
  if (cStack_79 < '\0') {
    FUN_00005810(local_90,CONCAT17(cStack_79,local_80) & 0x7fffffffffffffff);
  }
  if (iVar2 == 0) {
    uVar18 = 0x3fb999999999999a;
  }
  piVar3 = (int *)puVar4[1];
  if ((piVar3 != (int *)0x0) && (iVar2 = *piVar3, *piVar3 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar11 = (code *)SoftwareBreakpoint(0,0x433d790);
    (*pcVar11)();
  }
  uVar15 = puVar4[2];
  puVar5 = (undefined8 *)FUN_00008790(0x28);
  *puVar5 = &PTR_FUN_0da7f550;
  uVar6 = (*(code *)PTR_FUN_0e2f1ea8)();
  puVar5[1] = uVar6;
  puVar5[2] = piVar3;
  if (piVar3 == (int *)0x0) {
    puVar5[3] = uVar15;
    puVar5[4] = uVar18;
  }
  else {
    iVar2 = *piVar3;
    *piVar3 = iVar2 + 1;
    if (iVar2 == 0x7fffffff) {
                    /* WARNING: Does not return */
      pcVar11 = (code *)SoftwareBreakpoint(0,0x433d7b4);
      (*pcVar11)();
    }
    puVar5[3] = uVar15;
    puVar5[4] = uVar18;
    LOAcquire();
    iVar2 = *piVar3;
    *piVar3 = iVar2 + -1;
    LORelease();
    if (iVar2 == 1) {
      FUN_00005810(piVar3,8);
    }
  }
  plVar8 = *(long **)(lVar9 + 600);
  lVar10 = *(long *)(lVar9 + 0x260);
  plVar12 = plVar8;
  if (lVar10 << 3 != 0) {
    uVar16 = (lVar10 << 3) >> 3;
    do {
      uVar17 = uVar16 >> 1;
      puVar7 = (undefined *)(**(code **)(*(long *)plVar8[uVar17] + 0x10))();
      plVar12 = plVar8 + uVar17 + 1;
      uVar16 = uVar16 + ~(uVar16 >> 1);
      if (&UNK_0c9d706b < puVar7) {
        plVar12 = plVar8;
        uVar16 = uVar17;
      }
      plVar8 = plVar12;
    } while (uVar16 != 0);
    plVar8 = *(long **)(lVar9 + 600);
    lVar10 = *(long *)(lVar9 + 0x260);
  }
  if (plVar12 == plVar8 + lVar10) {
LAB_0433d73c:
    plVar12 = plVar8 + lVar10;
  }
  else {
    pcVar11 = *(code **)(*(long *)*plVar12 + 0x10);
    if (pcVar11 == (code *)&LAB_0037d0bc) {
      puVar7 = &DAT_0ccc0158;
    }
    else if (pcVar11 == (code *)&LAB_021661bc) {
      puVar7 = &DAT_0ccc01ac;
    }
    else if (pcVar11 == (code *)&LAB_021661c8) {
      puVar7 = &DAT_0ccc01b0;
    }
    else {
      puVar7 = (undefined *)(*pcVar11)();
      plVar8 = *(long **)(lVar9 + 600);
      lVar10 = *(long *)(lVar9 + 0x260);
    }
    if (&DAT_0c9d706c < puVar7) goto LAB_0433d73c;
  }
  if (plVar8 + lVar10 == plVar12) {
    lVar9 = 0;
  }
  else {
    pcVar11 = *(code **)(*(long *)*plVar12 + 0x10);
    if (((pcVar11 == (code *)&LAB_021661c8 || pcVar11 == (code *)&LAB_0037d0bc) ||
         pcVar11 == (code *)&LAB_021661bc) ||
       (puVar7 = (undefined *)(*pcVar11)(), puVar7 != &DAT_0c9d706c)) {
                    /* WARNING: Does not return */
      pcVar11 = (code *)SoftwareBreakpoint(0,0x433d76c);
      (*pcVar11)();
    }
    lVar9 = *plVar12;
  }
  lVar10 = 0;
  if (lVar9 != 0) {
    lVar10 = lVar9 + -8;
  }
  puVar14 = *(undefined8 **)(lVar10 + 0x60);
  *(undefined8 **)(lVar10 + 0x60) = puVar5;
  if (puVar14 != (undefined8 *)0x0) {
    *puVar14 = &PTR_FUN_0da7f550;
    piVar3 = (int *)puVar14[2];
    if (piVar3 != (int *)0x0) {
      LOAcquire();
      iVar2 = *piVar3;
      *piVar3 = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        FUN_00005810(piVar3,8);
      }
    }
    FUN_00005810(puVar14,0x28);
  }
  if (((ulong)param_1 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4(param_1);
  }
  puVar13 = (ulong *)param_1[7];
  param_1[7] = puVar4;
  lVar9 = DAT_0e53ea40;
  if (puVar13 == (ulong *)0x0) {
    return param_1;
  }
  if (DAT_0e53ea40 == 0) {
                    /* WARNING: Does not return */
    pcVar11 = (code *)SoftwareBreakpoint(0,0x433d7c0);
    (*pcVar11)();
  }
  plVar8 = *(long **)(DAT_0e53ea40 + 600);
  lVar10 = *(long *)(DAT_0e53ea40 + 0x260);
  plVar12 = plVar8;
  if (lVar10 << 3 != 0) {
    uVar16 = (lVar10 << 3) >> 3;
    do {
      uVar17 = uVar16 >> 1;
      puVar7 = (undefined *)(**(code **)(*(long *)plVar8[uVar17] + 0x10))();
      plVar12 = plVar8 + uVar17 + 1;
      uVar16 = uVar16 + ~(uVar16 >> 1);
      if (&UNK_0c9d706b < puVar7) {
        plVar12 = plVar8;
        uVar16 = uVar17;
      }
      plVar8 = plVar12;
    } while (uVar16 != 0);
    plVar8 = *(long **)(lVar9 + 600);
    lVar10 = *(long *)(lVar9 + 0x260);
  }
  if (plVar12 != plVar8 + lVar10) {
    pcVar11 = *(code **)(*(long *)*plVar12 + 0x10);
    if (pcVar11 == (code *)&LAB_0037d0bc) {
      puVar7 = &DAT_0ccc0158;
    }
    else if (pcVar11 == (code *)&LAB_021661bc) {
      puVar7 = &DAT_0ccc01ac;
    }
    else if (pcVar11 == (code *)&LAB_021661c8) {
      puVar7 = &DAT_0ccc01b0;
    }
    else {
      puVar7 = (undefined *)(*pcVar11)();
      plVar8 = *(long **)(lVar9 + 600);
      lVar10 = *(long *)(lVar9 + 0x260);
    }
    if (puVar7 <= &DAT_0c9d706c) goto LAB_0433d568;
  }
  plVar12 = plVar8 + lVar10;
LAB_0433d568:
  if (plVar8 + lVar10 == plVar12) {
    lVar9 = 0;
  }
  else {
    pcVar11 = *(code **)(*(long *)*plVar12 + 0x10);
    if (((pcVar11 == (code *)&LAB_021661c8 || pcVar11 == (code *)&LAB_0037d0bc) ||
         pcVar11 == (code *)&LAB_021661bc) ||
       (puVar7 = (undefined *)(*pcVar11)(), puVar7 != &DAT_0c9d706c)) {
                    /* WARNING: Does not return */
      pcVar11 = (code *)SoftwareBreakpoint(0,0x433d778);
      (*pcVar11)();
    }
    lVar9 = *plVar12;
  }
  lVar10 = 0;
  if (lVar9 != 0) {
    lVar10 = lVar9 + -8;
  }
  puVar4 = *(undefined8 **)(lVar10 + 0x60);
  *(undefined8 *)(lVar10 + 0x60) = 0;
  if (puVar4 != (undefined8 *)0x0) {
    *puVar4 = &PTR_FUN_0da7f550;
    piVar3 = (int *)puVar4[2];
    if (piVar3 != (int *)0x0) {
      LOAcquire();
      iVar2 = *piVar3;
      *piVar3 = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        FUN_00005810(piVar3,8);
      }
    }
    FUN_00005810(puVar4,0x28);
  }
  puVar13[2] = 0;
  if (puVar13[1] != 0) {
    *(undefined1 *)(puVar13[1] + 4) = 1;
    piVar3 = (int *)puVar13[1];
    if (piVar3 != (int *)0x0) {
      LOAcquire();
      iVar2 = *piVar3;
      *piVar3 = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        FUN_00005810(piVar3,8);
      }
    }
  }
  if ((*puVar13 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4();
  }
  FUN_00005810(puVar13,0x18);
  return param_1;
}


