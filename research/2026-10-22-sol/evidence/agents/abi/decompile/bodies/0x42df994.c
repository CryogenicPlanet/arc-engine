// ==== 0x42df994  FUN_042df994  (1176 bytes)

void FUN_042df994(ulong *param_1,undefined8 *param_2,undefined8 *param_3)

{
  long lVar1;
  undefined8 *puVar2;
  long lVar3;
  int iVar4;
  undefined4 *puVar5;
  undefined8 *puVar6;
  long *plVar7;
  undefined4 *puVar8;
  ulong uVar9;
  long lVar10;
  code *pcVar11;
  long *plVar12;
  ulong uVar13;
  undefined8 *puVar14;
  bool bVar15;
  int *piVar16;
  undefined8 uVar17;
  undefined8 local_a8;
  char *local_a0;
  char *local_98;
  char *pcStack_90;
  undefined1 local_88;
  ulong local_78;
  undefined4 local_70;
  char *local_68;
  
  plVar12 = (long *)param_1[1];
  if (plVar12 == (long *)0x0) {
LAB_042dfb14:
    bVar15 = true;
  }
  else {
    lVar10 = plVar12[1];
    do {
      if (lVar10 == -1) {
        plVar12 = (long *)0x0;
        goto LAB_042dfb14;
      }
      lVar1 = lVar10 + 1;
      lVar3 = plVar12[1];
      bVar15 = lVar3 != lVar10;
      lVar10 = lVar3;
    } while (bVar15);
    plVar12[1] = lVar1;
    uVar13 = *param_1;
    if (uVar13 != 0) {
      uVar9 = param_1[3];
      param_1[3] = uVar9 + 1;
      local_a8 = 10;
      if (DAT_0e4a445a != '\0') {
        local_68 = "AsyncInvocation";
        local_88 = 0;
        local_70 = 2;
        local_98 = "callback";
        pcStack_90 = "Delegate::OnManagerInitialized";
        local_a0 = "Delegate::OnManagerInitialized";
        local_78 = uVar9 + 1;
        FUN_042c0d74(DAT_0e4a445a,&local_a8,&local_68,0,&DAT_0c4b50d8,0x62,0,0,&pcStack_90,&local_98
                     ,&local_a0);
      }
      if ((uVar13 & 0xfffffffc00000000) == DAT_0e2f2208) {
        FUN_0000ffa8(uVar13);
      }
      puVar5 = (undefined4 *)FUN_00008790(0x60);
      *puVar5 = 1;
      *(undefined1 **)(puVar5 + 2) = &LAB_042e0ff4;
      *(code **)(puVar5 + 4) = FUN_042e116c;
      *(code **)(puVar5 + 6) = FUN_00b77080;
      *(undefined8 *)(puVar5 + 10) = 1;
      *(undefined8 *)(puVar5 + 8) = 0x10;
      *(ulong *)(puVar5 + 0xc) = uVar13;
      lVar10 = param_2[1];
      uVar17 = *param_2;
      *(undefined8 *)(puVar5 + 0x10) = param_2[1];
      *(undefined8 *)(puVar5 + 0xe) = uVar17;
      if (lVar10 != 0) {
        plVar7 = (long *)(lVar10 + 8);
        *plVar7 = *plVar7 + 1;
      }
      *(undefined8 *)(puVar5 + 0x12) = 0;
      *(undefined8 *)(puVar5 + 0x14) = 0;
      *(undefined8 *)(puVar5 + 0x16) = 0;
      uVar9 = param_3[1];
      if (uVar9 != 0) {
        if (uVar9 >> 0x3c != 0) {
                    /* WARNING: Subroutine does not return */
          FUN_040416c0();
        }
        param_3 = (undefined8 *)*param_3;
        puVar2 = param_3 + uVar9 * 2;
        puVar6 = (undefined8 *)FUN_00008790(uVar9 << 4);
        *(undefined8 **)(puVar5 + 0x12) = puVar6;
        *(ulong *)(puVar5 + 0x16) = uVar9;
        puVar14 = puVar6;
        do {
          if (puVar14 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
            pcVar11 = (code *)SoftwareBreakpoint(1,0x42dfda8);
            (*pcVar11)();
          }
          *puVar14 = *param_3;
          lVar10 = param_3[1];
          puVar14[1] = lVar10;
          if (lVar10 != 0) {
            *(long *)(lVar10 + 8) = *(long *)(lVar10 + 8) + 1;
          }
          param_3 = param_3 + 2;
          puVar14 = puVar14 + 2;
        } while (param_3 != puVar2);
        *(long *)(puVar5 + 0x14) = (long)puVar14 - (long)puVar6 >> 4;
      }
      plVar7 = (long *)(*(code *)PTR___tlv_bootstrap_0e379858)();
      if (((undefined8 *)*plVar7 == (undefined8 *)0x0) ||
         (puVar14 = *(undefined8 **)*plVar7, puVar14 == (undefined8 *)0x0)) {
                    /* WARNING: Does not return */
        pcVar11 = (code *)SoftwareBreakpoint(0,0x42dfe2c);
        (*pcVar11)();
      }
      FUN_00029e40(&pcStack_90,"InvokeAsync","../../arc/common/utils/delegate_wrapper.h",0x76);
      piVar16 = (int *)param_1[4];
      if ((piVar16 != (int *)0x0) && (iVar4 = *piVar16, *piVar16 = iVar4 + 1, iVar4 == 0x7fffffff))
      {
                    /* WARNING: Does not return */
        pcVar11 = (code *)SoftwareBreakpoint(0,0x42dfe44);
        (*pcVar11)();
      }
      uVar9 = param_1[5];
      lVar10 = plVar12[2];
      plVar12[2] = lVar10 + 1;
      puVar8 = (undefined4 *)FUN_00008790(lVar10,0x60);
      *puVar8 = 1;
      *(undefined1 **)(puVar8 + 2) = &LAB_042c1740;
      *(code **)(puVar8 + 4) = FUN_042c17f0;
      *(undefined1 **)(puVar8 + 6) = &LAB_04169bc0;
      *(undefined1 **)(puVar8 + 8) = &LAB_042c0840;
      *(undefined8 *)(puVar8 + 10) = 0;
      *(int **)(puVar8 + 0xc) = piVar16;
      *(ulong *)(puVar8 + 0xe) = uVar9;
      *(ulong *)(puVar8 + 0x10) = uVar13;
      *(long **)(puVar8 + 0x12) = plVar12;
      if (DAT_0e2f2208 == 0) {
        FUN_0000ffa8("Delegate::OnManagerInitialized");
      }
      *(char **)(puVar8 + 0x14) = "Delegate::OnManagerInitialized";
      *(undefined4 **)(puVar8 + 0x16) = puVar5;
      (**(code **)*puVar14)(puVar14,&pcStack_90,puVar8,0);
      goto LAB_042dfcf0;
    }
    bVar15 = false;
  }
  plVar7 = (long *)(*(code *)PTR___tlv_bootstrap_0e379858)();
  if (((undefined8 *)*plVar7 == (undefined8 *)0x0) ||
     (puVar14 = *(undefined8 **)*plVar7, puVar14 == (undefined8 *)0x0)) {
                    /* WARNING: Does not return */
    pcVar11 = (code *)SoftwareBreakpoint(0,0x42dfe20);
    (*pcVar11)();
  }
  FUN_00029e40(&pcStack_90,"InvokeAsync","../../arc/common/utils/delegate_wrapper.h",0x68);
  piVar16 = (int *)param_1[4];
  if ((piVar16 != (int *)0x0) && (iVar4 = *piVar16, *piVar16 = iVar4 + 1, iVar4 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar11 = (code *)SoftwareBreakpoint(0,0x42dfe38);
    (*pcVar11)();
  }
  uVar13 = param_1[5];
  puVar5 = (undefined4 *)FUN_00008790(0x50);
  *puVar5 = 1;
  *(undefined1 **)(puVar5 + 2) = &LAB_042db590;
  *(code **)(puVar5 + 4) = FUN_042c0c80;
  *(undefined1 **)(puVar5 + 6) = &LAB_04169bc0;
  *(code **)(puVar5 + 8) = PiPNSView::viewWillStartLiveResize;
  *(undefined8 *)(puVar5 + 10) = 0;
  *(int **)(puVar5 + 0xc) = piVar16;
  *(ulong *)(puVar5 + 0xe) = uVar13;
  lVar10 = param_2[1];
  uVar17 = *param_2;
  *(undefined8 *)(puVar5 + 0x12) = param_2[1];
  *(undefined8 *)(puVar5 + 0x10) = uVar17;
  if (lVar10 != 0) {
    plVar7 = (long *)(lVar10 + 8);
    *plVar7 = *plVar7 + 1;
  }
  (**(code **)*puVar14)(puVar14,&pcStack_90,puVar5,0);
  if (bVar15) {
    return;
  }
LAB_042dfcf0:
  LOAcquire();
  lVar10 = plVar12[1];
  plVar12[1] = lVar10 + -1;
  LORelease();
  if (lVar10 == 0) {
    (**(code **)(*plVar12 + 0x10))(plVar12);
    if (plVar12[2] != 0) {
      LOAcquire();
      lVar10 = plVar12[2];
      plVar12[2] = lVar10 + -1;
      LORelease();
      if (lVar10 != 0) {
        return;
      }
    }
    pcVar11 = *(code **)(*plVar12 + 0x20);
    if (((pcVar11 == FUN_00f01bc0) || (pcVar11 == FUN_00f01bc0)) || (pcVar11 == FUN_013efb40)) {
      FUN_00005810(plVar12,0x20);
    }
    else {
      (*pcVar11)(plVar12);
    }
  }
  return;
}


