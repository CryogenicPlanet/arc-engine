// ==== 0x43755a4  FUN_043755a4  (1380 bytes)

void FUN_043755a4(long *param_1,long param_2)

{
  int *piVar1;
  bool bVar2;
  long lVar3;
  long lVar4;
  int iVar5;
  long *plVar6;
  undefined8 *puVar7;
  undefined8 *puVar8;
  undefined8 uVar9;
  long lVar10;
  code *pcVar11;
  long lVar12;
  long lVar13;
  long local_100;
  int local_f8;
  long local_f0;
  undefined8 local_e8 [2];
  undefined7 local_d8;
  char cStack_d1;
  char local_d0;
  long local_c8;
  long local_c0;
  long lStack_b8;
  long local_b0;
  long *plStack_a8;
  ulong local_a0;
  undefined8 *local_98;
  long local_88;
  long local_80;
  long lStack_78;
  long local_70;
  long *plStack_68;
  ulong local_60;
  undefined8 uStack_58;
  
  if (*(long *)(param_2 + 0x130) == 0) {
    param_1[3] = 0;
    param_1[2] = 0;
    param_1[5] = 0;
    param_1[4] = 0;
    param_1[1] = 0;
    *param_1 = 0;
    return;
  }
  lVar13 = *(long *)(*(long *)(param_2 + 0x130) + 0x8d8);
  plStack_a8 = (long *)0x0;
  local_b0 = 0;
  local_98 = (undefined8 *)0x0;
  local_a0 = 0;
  lStack_b8 = 0;
  local_c0 = 0;
  if ((*(long *)(lVar13 + 0x128) == 0) || (*(char *)(*(long *)(lVar13 + 0x128) + 4) != '\0')) {
                    /* WARNING: Does not return */
    pcVar11 = (code *)SoftwareBreakpoint(0,0x4375a40);
    (*pcVar11)();
  }
  lVar12 = *(long *)(*(long *)(lVar13 + 0x130) + 0x40);
  if (lVar12 == 0) {
                    /* WARNING: Does not return */
    pcVar11 = (code *)SoftwareBreakpoint(0,0x4375a64);
    (*pcVar11)();
  }
  local_88 = 0;
  local_80 = 0;
  FUN_003ad358(0,&local_80,&local_88);
  lVar4 = local_80;
  lVar3 = local_88;
  puVar7 = local_98;
  plVar6 = plStack_a8;
  lVar10 = local_b0;
  if (local_80 == 0) {
    uStack_58 = 0;
    local_98 = (undefined8 *)0x0;
    lStack_78 = lStack_b8;
    local_80 = local_c0;
    lStack_b8 = 0;
    local_c0 = 0;
    plStack_a8 = (long *)0x0;
    local_b0 = 0;
    local_70 = lVar10;
    plStack_68 = plVar6;
    local_60 = local_a0 & 0xffffffff;
    local_a0 = local_a0 & 0xffffffff00000000;
    if (puVar7 != (undefined8 *)0x0) {
      FUN_00005810(puVar7,0x10);
    }
    FUN_00111f30(&local_80);
  }
  else {
    if (local_b0 != 0) {
      FUN_005a359c();
    }
    local_a0 = local_a0 & 0xffffffff00000000;
    local_b0 = lVar4;
    plVar6 = (long *)(*(code *)PTR___tlv_bootstrap_0e379840)(&PTR___tlv_bootstrap_0e379840);
    if (((long *)*plVar6 == (long *)0x0) || (plVar6 = *(long **)*plVar6, plVar6 == (long *)0x0)) {
                    /* WARNING: Does not return */
      pcVar11 = (code *)SoftwareBreakpoint(0,0x4375a4c);
      (*pcVar11)();
    }
    iVar5 = *(int *)((long)plVar6 + 8);
    *(int *)((long)plVar6 + 8) = iVar5 + 1;
    if (iVar5 == 0x7fffffff) {
                    /* WARNING: Does not return */
      pcVar11 = (code *)SoftwareBreakpoint(0,0x4375a70);
      (*pcVar11)();
    }
    if (plStack_a8 != (long *)0x0) {
      LOAcquire();
      iVar5 = (int)plStack_a8[1];
      *(int *)(plStack_a8 + 1) = iVar5 + -1;
      LORelease();
      if (iVar5 == 1) {
        pcVar11 = *(code **)(*plStack_a8 + 0x18);
        if (pcVar11 == (code *)&LAB_044511c4) {
          pcVar11 = *(code **)(*plStack_a8 + 0x10);
        }
        plStack_a8 = plVar6;
        (*pcVar11)();
        plVar6 = plStack_a8;
      }
    }
    plStack_a8 = plVar6;
    if (local_98 == (undefined8 *)0x0) {
      puVar7 = (undefined8 *)FUN_00008790(8);
      *puVar7 = &PTR_viewWillStartLiveResize_0da8f868;
      iVar5 = FUN_009180c0(&local_c0,0,0,0,puVar7,"arc.mojom.RenderProcessAgent",&LAB_0442ad24,
                           &LAB_04051dc0);
      lVar10 = lStack_b8;
      if (iVar5 != 0) {
        puVar8 = (undefined8 *)FUN_00008790(0x10);
        puVar7 = local_98;
        *puVar8 = &PTR__cxx_destruct_0da887f8;
        puVar8[1] = lVar10;
        bVar2 = local_98 != (undefined8 *)0x0;
        local_98 = puVar8;
        if (bVar2) {
          FUN_00005810(puVar7,0x10);
        }
      }
    }
  }
  local_80 = lVar3;
  FUN_0077a848(local_e8,"arc.mojom.RenderProcessAgent",0x1c,&local_80);
  if (local_80 != 0) {
    FUN_005a359c();
  }
  FUN_028bbd60(lVar12,local_e8);
  if (local_c8 != 0) {
    FUN_005a359c();
  }
  if ((local_d0 != '\0') && (cStack_d1 < '\0')) {
    FUN_00005810(local_e8[0],CONCAT17(cStack_d1,local_d8) & 0x7fffffffffffffff);
  }
  param_1[3] = 0;
  param_1[2] = 0;
  param_1[5] = 0;
  param_1[4] = 0;
  param_1[1] = 0;
  *param_1 = 0;
  if (local_98 == (undefined8 *)0x0) {
    puVar7 = (undefined8 *)FUN_00008790(8);
    *puVar7 = &PTR_viewWillStartLiveResize_0da8f868;
    iVar5 = FUN_009180c0(&local_c0,0,0,0,puVar7,"arc.mojom.RenderProcessAgent",&LAB_0442ad24,
                         &LAB_04051dc0);
    lVar12 = lStack_b8;
    if (iVar5 != 0) {
      puVar8 = (undefined8 *)FUN_00008790(0x10);
      puVar7 = local_98;
      *puVar8 = &PTR__cxx_destruct_0da887f8;
      puVar8[1] = lVar12;
      bVar2 = local_98 != (undefined8 *)0x0;
      local_98 = puVar8;
      if (bVar2) {
        FUN_00005810(puVar7,0x10);
      }
    }
  }
  puVar7 = local_98;
  local_88 = 0;
  local_80 = 0;
  FUN_003ad358(0,&local_80,&local_88);
  lVar10 = local_80;
  lVar12 = local_88;
  if (local_80 == 0) {
    uStack_58 = 0;
    lVar10 = param_1[5];
    param_1[5] = 0;
    lStack_78 = param_1[1];
    local_80 = *param_1;
    local_70 = param_1[2];
    plStack_68 = (long *)param_1[3];
    param_1[1] = 0;
    *param_1 = 0;
    param_1[3] = 0;
    param_1[2] = 0;
    local_60 = (ulong)*(uint *)(param_1 + 4);
    *(undefined4 *)(param_1 + 4) = 0;
    if (lVar10 != 0) {
      FUN_00005810(lVar10,0x10);
    }
    FUN_00111f30(&local_80);
  }
  else {
    if (param_1[2] != 0) {
      FUN_005a359c();
    }
    param_1[2] = lVar10;
    *(undefined4 *)(param_1 + 4) = 0;
    plVar6 = (long *)(*(code *)PTR___tlv_bootstrap_0e379840)(&PTR___tlv_bootstrap_0e379840);
    if (((long *)*plVar6 == (long *)0x0) || (lVar10 = *(long *)*plVar6, lVar10 == 0)) {
                    /* WARNING: Does not return */
      pcVar11 = (code *)SoftwareBreakpoint(0,0x4375a58);
      (*pcVar11)();
    }
    iVar5 = *(int *)(lVar10 + 8);
    *(int *)(lVar10 + 8) = iVar5 + 1;
    if (iVar5 == 0x7fffffff) {
                    /* WARNING: Does not return */
      pcVar11 = (code *)SoftwareBreakpoint(0,0x4375a7c);
      (*pcVar11)();
    }
    plVar6 = (long *)param_1[3];
    param_1[3] = lVar10;
    if (plVar6 != (long *)0x0) {
      LOAcquire();
      iVar5 = (int)plVar6[1];
      *(int *)(plVar6 + 1) = iVar5 + -1;
      LORelease();
      if (iVar5 == 1) {
        pcVar11 = *(code **)(*plVar6 + 0x18);
        if (pcVar11 == (code *)&LAB_044511c4) {
          pcVar11 = *(code **)(*plVar6 + 0x10);
        }
        (*pcVar11)();
      }
    }
    if (param_1[5] == 0) {
      puVar8 = (undefined8 *)FUN_00008790(8);
      *puVar8 = &PTR__cxx_destruct_0da88798;
      iVar5 = FUN_009180c0(param_1,0,0,0,puVar8,"arc.mojom.RenderFrameAgent",&LAB_04429bcc,
                           &LAB_04051dc0);
      if (iVar5 != 0) {
        lVar10 = param_1[1];
        puVar8 = (undefined8 *)FUN_00008790(0x10);
        *puVar8 = &PTR__cxx_destruct_0da88710;
        puVar8[1] = lVar10;
        lVar10 = param_1[5];
        param_1[5] = (long)puVar8;
        if (lVar10 != 0) {
          FUN_00005810(lVar10,0x10);
        }
      }
    }
  }
  local_100 = lVar12;
  local_f8 = 0;
  local_f0 = 0;
  FUN_0442ad7c(puVar7,lVar13 + 0xbf8,&local_100);
  if (local_f8 == 1) {
    if (local_f0 == 0) {
      local_f8 = 0;
      goto LAB_04375990;
    }
    FUN_084a7784();
  }
  lVar13 = local_f0;
  local_f8 = 0;
  local_f0 = 0;
  if (lVar13 != 0) {
    piVar1 = (int *)(lVar13 + 8);
    LOAcquire();
    iVar5 = *piVar1;
    *piVar1 = iVar5 + -1;
    LORelease();
    if (iVar5 == 1) {
      uVar9 = FUN_084a76fc();
      FUN_00005810(uVar9,0x38);
    }
    if (local_f0 != 0) {
      LOAcquire();
      iVar5 = *(int *)(local_f0 + 8);
      *(int *)(local_f0 + 8) = iVar5 + -1;
      LORelease();
      if (iVar5 == 1) {
        uVar9 = FUN_084a76fc();
        FUN_00005810(uVar9,0x38);
      }
    }
  }
LAB_04375990:
  if (local_100 != 0) {
    FUN_005a359c();
  }
  puVar7 = local_98;
  local_98 = (undefined8 *)0x0;
  if (puVar7 != (undefined8 *)0x0) {
    FUN_00005810(puVar7,0x10);
  }
  FUN_00111f30(&local_c0);
  return;
}


