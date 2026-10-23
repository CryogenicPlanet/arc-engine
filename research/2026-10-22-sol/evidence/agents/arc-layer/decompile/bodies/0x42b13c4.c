// ==== 0x42b13c4  FUN_042b13c4  (1028 bytes)

void FUN_042b13c4(ulong *param_1,char *param_2,undefined8 param_3,undefined8 param_4,
                 undefined8 *param_5)

{
  long lVar1;
  long lVar2;
  int iVar3;
  undefined4 *puVar4;
  long *plVar5;
  undefined4 *puVar6;
  ulong uVar7;
  long lVar8;
  code *pcVar9;
  long *plVar10;
  undefined8 *puVar11;
  bool bVar12;
  ulong uVar13;
  int *piVar14;
  undefined8 uVar15;
  undefined8 local_a8;
  char *local_a0;
  char *local_98;
  char *pcStack_90;
  undefined1 local_88;
  ulong local_78;
  undefined4 local_70;
  char *local_68;
  
  plVar10 = (long *)param_1[1];
  if (plVar10 == (long *)0x0) {
LAB_042b15dc:
    bVar12 = true;
  }
  else {
    lVar8 = plVar10[1];
    do {
      if (lVar8 == -1) {
        plVar10 = (long *)0x0;
        goto LAB_042b15dc;
      }
      lVar1 = lVar8 + 1;
      lVar2 = plVar10[1];
      bVar12 = lVar2 != lVar8;
      lVar8 = lVar2;
    } while (bVar12);
    plVar10[1] = lVar1;
    uVar13 = *param_1;
    bVar12 = false;
    if (uVar13 != 0) {
      uVar7 = param_1[3];
      param_1[3] = uVar7 + 1;
      local_a8 = 10;
      if (DAT_0e4a445a != '\0') {
        local_68 = "AsyncInvocation";
        local_a0 = "NULL";
        if (param_2 != (char *)0x0) {
          local_a0 = param_2;
        }
        local_88 = 0;
        local_70 = 2;
        local_98 = "callback";
        pcStack_90 = param_2;
        local_78 = uVar7 + 1;
        FUN_042c0d74(DAT_0e4a445a,&local_a8,&local_68,0,&DAT_0c4b50d8,0x62,0,0,&pcStack_90,&local_98
                     ,&local_a0);
      }
      if ((uVar13 & 0xfffffffc00000000) == DAT_0e2f2208) {
        FUN_0000ffa8(uVar13);
      }
      puVar4 = (undefined4 *)FUN_00008790(0x48);
      *puVar4 = 1;
      *(undefined1 **)(puVar4 + 2) = &LAB_04146f80;
      *(code **)(puVar4 + 4) = FUN_042c1618;
      *(code **)(puVar4 + 6) = FUN_00b77080;
      *(undefined8 *)(puVar4 + 8) = param_3;
      *(undefined8 *)(puVar4 + 10) = param_4;
      *(ulong *)(puVar4 + 0xc) = uVar13;
      lVar8 = param_5[1];
      uVar15 = *param_5;
      *(undefined8 *)(puVar4 + 0x10) = param_5[1];
      *(undefined8 *)(puVar4 + 0xe) = uVar15;
      if (lVar8 != 0) {
        plVar5 = (long *)(lVar8 + 8);
        *plVar5 = *plVar5 + 1;
      }
      plVar5 = (long *)(*(code *)PTR___tlv_bootstrap_0e379858)();
      if (((undefined8 *)*plVar5 == (undefined8 *)0x0) ||
         (puVar11 = *(undefined8 **)*plVar5, puVar11 == (undefined8 *)0x0)) {
                    /* WARNING: Does not return */
        pcVar9 = (code *)SoftwareBreakpoint(0,0x42b17c8);
        (*pcVar9)();
      }
      FUN_00029e40(&pcStack_90,"InvokeAsync","../../arc/common/utils/delegate_wrapper.h",0x76);
      piVar14 = (int *)param_1[4];
      if ((piVar14 != (int *)0x0) && (iVar3 = *piVar14, *piVar14 = iVar3 + 1, iVar3 == 0x7fffffff))
      {
                    /* WARNING: Does not return */
        pcVar9 = (code *)SoftwareBreakpoint(0,0x42b17e0);
        (*pcVar9)();
      }
      uVar7 = param_1[5];
      lVar8 = plVar10[2];
      plVar10[2] = lVar8 + 1;
      puVar6 = (undefined4 *)FUN_00008790(lVar8,0x60);
      *puVar6 = 1;
      *(undefined1 **)(puVar6 + 2) = &LAB_042c1740;
      *(code **)(puVar6 + 4) = FUN_042c17f0;
      *(undefined1 **)(puVar6 + 6) = &LAB_04169bc0;
      *(undefined1 **)(puVar6 + 8) = &LAB_042c0840;
      *(undefined8 *)(puVar6 + 10) = 0;
      *(int **)(puVar6 + 0xc) = piVar14;
      *(ulong *)(puVar6 + 0xe) = uVar7;
      *(ulong *)(puVar6 + 0x10) = uVar13;
      *(long **)(puVar6 + 0x12) = plVar10;
      if (((ulong)param_2 & 0xfffffffc00000000) == DAT_0e2f2208) {
        FUN_0000ffa8(param_2);
      }
      *(char **)(puVar6 + 0x14) = param_2;
      *(undefined4 **)(puVar6 + 0x16) = puVar4;
      (**(code **)*puVar11)(puVar11,&pcStack_90,puVar6,0);
      goto LAB_042b15a8;
    }
  }
  plVar5 = (long *)(*(code *)PTR___tlv_bootstrap_0e379858)();
  if (((undefined8 *)*plVar5 == (undefined8 *)0x0) ||
     (puVar11 = *(undefined8 **)*plVar5, puVar11 == (undefined8 *)0x0)) {
                    /* WARNING: Does not return */
    pcVar9 = (code *)SoftwareBreakpoint(0,0x42b17bc);
    (*pcVar9)();
  }
  FUN_00029e40(&pcStack_90,"InvokeAsync","../../arc/common/utils/delegate_wrapper.h",0x68);
  piVar14 = (int *)param_1[4];
  if ((piVar14 != (int *)0x0) && (iVar3 = *piVar14, *piVar14 = iVar3 + 1, iVar3 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar9 = (code *)SoftwareBreakpoint(0,0x42b17d4);
    (*pcVar9)();
  }
  uVar13 = param_1[5];
  puVar4 = (undefined4 *)FUN_00008790(0x50);
  *puVar4 = 1;
  *(undefined1 **)(puVar4 + 2) = &LAB_042c0b1c;
  *(code **)(puVar4 + 4) = FUN_042c0c80;
  *(undefined1 **)(puVar4 + 6) = &LAB_04169bc0;
  *(code **)(puVar4 + 8) = PiPNSView::viewWillStartLiveResize;
  *(undefined8 *)(puVar4 + 10) = 0;
  *(int **)(puVar4 + 0xc) = piVar14;
  *(ulong *)(puVar4 + 0xe) = uVar13;
  lVar8 = param_5[1];
  uVar15 = *param_5;
  *(undefined8 *)(puVar4 + 0x12) = param_5[1];
  *(undefined8 *)(puVar4 + 0x10) = uVar15;
  if (lVar8 != 0) {
    plVar5 = (long *)(lVar8 + 8);
    *plVar5 = *plVar5 + 1;
  }
  (**(code **)*puVar11)(puVar11,&pcStack_90,puVar4,0);
  if (bVar12) {
    return;
  }
LAB_042b15a8:
  LOAcquire();
  lVar8 = plVar10[1];
  plVar10[1] = lVar8 + -1;
  LORelease();
  if (lVar8 == 0) {
    (**(code **)(*plVar10 + 0x10))(plVar10);
    if (plVar10[2] != 0) {
      LOAcquire();
      lVar8 = plVar10[2];
      plVar10[2] = lVar8 + -1;
      LORelease();
      if (lVar8 != 0) {
        return;
      }
    }
    pcVar9 = *(code **)(*plVar10 + 0x20);
    if (((pcVar9 == FUN_00f01bc0) || (pcVar9 == FUN_00f01bc0)) || (pcVar9 == FUN_013efb40)) {
      FUN_00005810(plVar10,0x20);
    }
    else {
      (*pcVar9)(plVar10);
    }
  }
  return;
}


