// ==== 0x4374c00  FUN_04374c00  (780 bytes)

void FUN_04374c00(long param_1)

{
  long lVar1;
  long *plVar2;
  bool bVar3;
  int iVar4;
  undefined8 uVar5;
  long lVar6;
  code *pcVar7;
  int *piVar8;
  long *plVar9;
  undefined8 uVar10;
  undefined8 *puVar11;
  undefined8 *local_180;
  undefined8 uStack_178;
  undefined8 auStack_170 [37];
  undefined1 *local_48;
  
  if (*(long *)(param_1 + 0x130) == 0) {
    return;
  }
  if (DAT_0e53ea40 == 0) {
    if (2 < DAT_0e4a3078) {
      return;
    }
    FUN_00f195d8(&local_180,"../../arc/browser/web_contents/web_contents_impl.cc",0x799,2);
    FUN_00040060(auStack_170,"PerformanceManager is not available.",0x24);
    FUN_00f1950c(&local_180);
    return;
  }
  uVar5 = FUN_07af18ac(DAT_0e53ea40 + 8);
  lVar6 = FUN_000805e0(*(long *)(param_1 + 0x130) + 8,&DAT_0ccc03ac);
  if (lVar6 == 0 || lVar6 == 0x20) {
    piVar8 = (int *)0x0;
LAB_04374de0:
    local_180 = (undefined8 *)FUN_00008790(8);
    uVar10 = 0;
  }
  else {
    lVar6 = *(long *)(lVar6 + 0x10);
    piVar8 = *(int **)(lVar6 + 0x370);
    if ((piVar8 != (int *)0x0) && (iVar4 = *piVar8, *piVar8 = iVar4 + 1, iVar4 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar7 = (code *)SoftwareBreakpoint(0,0x4374f0c);
      (*pcVar7)();
    }
    uVar10 = *(undefined8 *)(lVar6 + 0x378);
    if (piVar8 == (int *)0x0) goto LAB_04374de0;
    iVar4 = piVar8[1];
    local_180 = (undefined8 *)FUN_00008790(8);
    if ((char)iVar4 != '\0') {
      uVar10 = 0;
    }
  }
  uStack_178 = 1;
  auStack_170[0] = 1;
  *local_180 = uVar10;
  uVar5 = FUN_07af7468(uVar5,&local_180,2,1);
  local_48 = (undefined1 *)&local_180;
  FUN_0267ce40(&local_48);
  if (*(long *)(param_1 + 0x130) == 0) goto LAB_04374db8;
  plVar9 = *(long **)(param_1 + 0x10);
  if (plVar9 == (long *)0x0) {
LAB_04374ddc:
                    /* WARNING: Subroutine does not return */
    FUN_050d342c();
  }
  uVar10 = *(undefined8 *)(param_1 + 0x10);
  puVar11 = *(undefined8 **)(param_1 + 8);
  lVar6 = plVar9[1];
  do {
    if (lVar6 == -1) goto LAB_04374ddc;
    lVar1 = lVar6 + 1;
    plVar2 = plVar9 + 1;
    bVar3 = *plVar2 != lVar6;
    lVar6 = *plVar2;
  } while (bVar3);
  *plVar2 = lVar1;
  lVar6 = *plVar2;
  *plVar2 = lVar6 + 1;
  local_180 = puVar11;
  uStack_178 = uVar10;
  FUN_04378120(lVar6,param_1 + 0x348,"arc::WebContentsUIDelegate::FinalizeDiscardRequest",0xb0,1,
               &local_180,uVar5);
  LOAcquire();
  lVar6 = *plVar2;
  *plVar2 = lVar6 + -1;
  LORelease();
  if (lVar6 == 0) {
    (**(code **)(*plVar9 + 0x10))(plVar9);
    if (plVar9[2] != 0) {
      LOAcquire();
      lVar6 = plVar9[2];
      plVar9[2] = lVar6 + -1;
      LORelease();
      if (lVar6 != 0) goto LAB_04374dac;
    }
    pcVar7 = *(code **)(*plVar9 + 0x20);
    if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40)) {
      FUN_00005810(plVar9,0x20);
    }
    else {
      (*pcVar7)(plVar9);
    }
  }
LAB_04374dac:
  LOAcquire();
  lVar6 = plVar9[1];
  plVar9[1] = lVar6 + -1;
  LORelease();
  if (lVar6 == 0) {
    (**(code **)(*plVar9 + 0x10))(plVar9);
    if (plVar9[2] != 0) {
      LOAcquire();
      lVar6 = plVar9[2];
      plVar9[2] = lVar6 + -1;
      LORelease();
      if (lVar6 != 0) goto LAB_04374db8;
    }
    pcVar7 = *(code **)(*plVar9 + 0x20);
    if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40)) {
      FUN_00005810(plVar9,0x20);
    }
    else {
      (*pcVar7)(plVar9);
    }
  }
LAB_04374db8:
  if (piVar8 != (int *)0x0) {
    LOAcquire();
    iVar4 = *piVar8;
    *piVar8 = iVar4 + -1;
    LORelease();
    if (iVar4 == 1) {
      FUN_00005810(piVar8,8);
    }
  }
  return;
}


