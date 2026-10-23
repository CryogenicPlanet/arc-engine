// ==== 0x42f8300  FUN_042f8300  (1656 bytes)

undefined8 * FUN_042f8300(undefined8 *param_1,long *param_2,uint param_3,undefined8 *param_4)

{
  byte bVar1;
  int iVar2;
  long lVar3;
  int *piVar4;
  ulong uVar5;
  long *plVar6;
  long lVar7;
  code *pcVar8;
  undefined8 *puVar9;
  long *plVar10;
  undefined8 *puVar11;
  undefined8 *puVar12;
  undefined8 *puVar13;
  undefined8 *puVar14;
  undefined1 auVar15 [16];
  undefined1 auStack_320 [232];
  undefined1 auStack_238 [232];
  undefined8 local_150;
  undefined8 uStack_148;
  ulong local_140;
  undefined8 uStack_138;
  undefined8 local_130;
  undefined8 uStack_128;
  undefined8 uStack_120;
  undefined8 uStack_118;
  undefined8 local_110;
  undefined8 uStack_108;
  undefined8 uStack_100;
  undefined8 uStack_f8;
  undefined8 local_f0;
  undefined8 uStack_e8;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined8 uStack_c8;
  long local_c0;
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  undefined8 local_70;
  
  *param_1 = &PTR_FUN_0da7d860;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = &PTR_FUN_0da7d8e8;
  param_1[4] = &PTR_viewWillStartLiveResize_0da821d8;
  puVar9 = param_1 + 5;
  *puVar9 = &PTR_FUN_0da8a590;
  piVar4 = (int *)FUN_00008790(8);
  *piVar4 = 0;
  *(undefined1 *)(piVar4 + 1) = 0;
  if ((piVar4 != (int *)0x0) && (iVar2 = *piVar4, *piVar4 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar8 = (code *)SoftwareBreakpoint(0,0x42f8960);
    (*pcVar8)();
  }
  param_1[8] = &PTR_FUN_0da8a590;
  param_1[6] = piVar4;
  param_1[7] = puVar9;
  param_1[5] = &PTR_LAB_0da7d958;
  piVar4 = (int *)FUN_00008790(8);
  *piVar4 = 0;
  *(undefined1 *)(piVar4 + 1) = 0;
  if ((piVar4 != (int *)0x0) && (iVar2 = *piVar4, *piVar4 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar8 = (code *)SoftwareBreakpoint(0,0x42f896c);
    (*pcVar8)();
  }
  puVar12 = param_1 + 0xc;
  *puVar12 = &PTR_FUN_0da8a590;
  param_1[8] = &PTR_LAB_0d882d88;
  param_1[9] = piVar4;
  param_1[10] = param_1 + 8;
  param_1[0xb] = 0;
  piVar4 = (int *)FUN_00008790(8);
  *piVar4 = 0;
  *(undefined1 *)(piVar4 + 1) = 0;
  if ((piVar4 != (int *)0x0) && (iVar2 = *piVar4, *piVar4 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar8 = (code *)SoftwareBreakpoint(0,0x42f8978);
    (*pcVar8)();
  }
  puVar11 = param_1 + 0xf;
  *puVar11 = &PTR_viewWillStartLiveResize_0da7d9f0;
  param_1[0xd] = piVar4;
  param_1[0xe] = puVar12;
  param_1[0xc] = &PTR_FUN_0da7d9a0;
  param_1[0x13] = 0;
  param_1[0x12] = 0;
  param_1[0x10] = &PTR__cxx_destruct_0da1fa18;
  param_1[0x11] = param_1 + 0x12;
  puVar13 = param_1 + 0x14;
  *puVar13 = &PTR_viewWillStartLiveResize_0da7da58;
  puVar14 = param_1 + 0x15;
  *puVar14 = &PTR_FUN_0da8a590;
  piVar4 = (int *)FUN_00008790(8);
  *piVar4 = 0;
  *(undefined1 *)(piVar4 + 1) = 0;
  if ((piVar4 != (int *)0x0) && (iVar2 = *piVar4, *piVar4 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar8 = (code *)SoftwareBreakpoint(0,0x42f8984);
    (*pcVar8)();
  }
  param_1[0x16] = piVar4;
  param_1[0x17] = puVar14;
  *param_1 = &PTR_FUN_0da7d038;
  param_1[3] = &PTR_LAB_0da7d170;
  param_1[4] = &PTR_LAB_0da7d1e0;
  param_1[5] = &PTR_LAB_0da7d240;
  param_1[8] = &PTR_LAB_0da7d288;
  param_1[0xc] = &PTR_LAB_0da7d6a0;
  param_1[0xf] = &PTR_LAB_0da7d6f0;
  param_1[0x10] = &PTR_LAB_0da7d758;
  param_1[0x14] = &PTR_FUN_0da7d7f8;
  param_1[0x15] = &PTR_LAB_0da7d830;
  param_1[0x18] = 0;
  param_1[0x1b] = 0;
  param_1[0x19] = 0;
  *(undefined1 *)(param_1 + 0x1a) = 0;
  piVar4 = (int *)FUN_00008790(8);
  *piVar4 = 0;
  *(undefined1 *)(piVar4 + 1) = 0;
  if ((piVar4 != (int *)0x0) && (iVar2 = *piVar4, *piVar4 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar8 = (code *)SoftwareBreakpoint(0,0x42f8990);
    (*pcVar8)();
  }
  param_1[0x1c] = piVar4;
  param_1[0x1d] = param_1 + 0x18;
  param_1[0x1f] = 0;
  param_1[0x1e] = 0;
  param_1[0x21] = 0;
  param_1[0x20] = 0;
  param_1[0x24] = FUN_040490c0;
  param_1[0x25] = &DAT_0c4b3500;
  param_1[0x28] = FUN_040490c0;
  param_1[0x29] = &DAT_0c4b3500;
  piVar4 = (int *)FUN_00008790(8);
  *piVar4 = 0;
  *(undefined1 *)(piVar4 + 1) = 0;
  if ((piVar4 != (int *)0x0) && (iVar2 = *piVar4, *piVar4 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar8 = (code *)SoftwareBreakpoint(0,0x42f899c);
    (*pcVar8)();
  }
  param_1[0x2a] = piVar4;
  param_1[0x2b] = param_1 + 0x22;
  param_1[0x30] = 0;
  param_1[0x2d] = 0;
  param_1[0x2e] = 0;
  param_1[0x2c] = 0;
  *(undefined2 *)(param_1 + 0x2f) = 0;
  if (((ulong)puVar14 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(puVar14);
  }
  uVar5 = DAT_0e2f2208;
  param_1[0x31] = puVar14;
  param_1[0x32] = 0;
  if (((ulong)puVar12 & 0xfffffffc00000000) == uVar5) {
    FUN_0000ffa8(puVar12);
    uVar5 = DAT_0e2f2208;
  }
  param_1[0x33] = puVar12;
  param_1[0x34] = 0;
  if (((ulong)puVar13 & 0xfffffffc00000000) == uVar5) {
    FUN_0000ffa8(puVar13);
    uVar5 = DAT_0e2f2208;
  }
  param_1[0x35] = puVar13;
  param_1[0x36] = 0;
  if (((ulong)puVar11 & 0xfffffffc00000000) == uVar5) {
    FUN_0000ffa8(puVar11);
    uVar5 = DAT_0e2f2208;
  }
  param_1[0x38] = 0;
  param_1[0x39] = 0;
  param_1[0x37] = puVar11;
  if (((ulong)puVar9 & 0xfffffffc00000000) == uVar5) {
    FUN_0000ffa8(puVar9);
  }
  param_1[0x3a] = puVar9;
  param_1[0x3b] = 0;
  param_1[0x3c] = *param_2;
  lVar7 = param_2[1];
  param_1[0x3d] = lVar7;
  lVar3 = 0;
  if (lVar7 != 0) {
    lVar3 = *(long *)(lVar7 + 8);
    *(long *)(lVar7 + 8) = lVar3 + 1;
  }
  auVar15 = FUN_043074c0(lVar3);
  *(undefined1 (*) [16])(param_1 + 0x3e) = auVar15;
  *(undefined1 *)(param_1 + 0x44) = 0;
  *(undefined1 *)(param_1 + 0x4b) = 0;
  param_1[0x41] = 0;
  param_1[0x40] = 0;
  *(undefined1 *)(param_1 + 0x42) = 0;
  param_1[0x4c] = 0;
  param_1[0x4e] = 0;
  param_1[0x4d] = 0;
  param_1[0x4f] = param_1 + 0x4f;
  param_1[0x50] = param_1 + 0x4f;
  param_1[0x51] = 0;
  *(undefined4 *)(param_1 + 0x52) = 0;
  piVar4 = (int *)FUN_00008790(8);
  *piVar4 = 0;
  *(undefined1 *)(piVar4 + 1) = 0;
  if ((piVar4 != (int *)0x0) && (iVar2 = *piVar4, *piVar4 = iVar2 + 1, iVar2 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar8 = (code *)SoftwareBreakpoint(0,0x42f89a8);
    (*pcVar8)();
  }
  param_1[0x53] = piVar4;
  param_1[0x54] = param_1;
  plVar10 = *(long **)(*param_2 + 0x88);
  if (plVar10 == (long *)0x0) {
    return param_1;
  }
  if (DAT_0e481618 == 0) {
    return param_1;
  }
  if ((*(byte *)(DAT_0e481618 + 0x128) & 1) != 0) {
    return param_1;
  }
  if ((*(byte *)(DAT_0e481618 + 0x129) & 1) != 0) {
    return param_1;
  }
  iVar2 = FUN_03510d90(plVar10);
  if (iVar2 == 0) {
    return param_1;
  }
  iVar2 = FUN_0270f548(plVar10);
  if (iVar2 == 0) {
    return param_1;
  }
  (**(code **)(*plVar10 + 0x30))(&local_150,plVar10);
  uVar5 = FUN_0270f5dc(&local_150);
  if ((long)local_140 < 0) {
    FUN_00005810(local_150,local_140 & 0x7fffffffffffffff);
  }
  if ((uVar5 & 1) != 0) {
    return param_1;
  }
  local_70 = 0;
  uStack_88 = 0;
  local_90 = 0;
  uStack_78 = 0;
  uStack_80 = 0;
  uStack_a8 = 0;
  local_b0 = 0;
  uStack_98 = 0;
  uStack_a0 = 0;
  uStack_c8 = 0;
  local_d0 = 0;
  uStack_b8 = 0;
  local_c0 = 0;
  uStack_e8 = 0;
  local_f0 = 0;
  uStack_d8 = 0;
  uStack_e0 = 0;
  uStack_108 = 0;
  local_110 = 0;
  uStack_f8 = 0;
  uStack_100 = 0;
  uStack_128 = 0;
  local_130 = 0;
  uStack_118 = 0;
  uStack_120 = 0;
  uStack_148 = 0;
  local_150 = 0;
  uStack_138 = 0;
  local_140 = 0;
  FUN_03b1dd10(&local_150,*(undefined8 *)(*param_2 + 0x88),1);
  bVar1 = *(byte *)((long)param_4 + 0x17);
  if (local_c0 < 0) {
    uVar5 = param_4[1];
    puVar9 = (undefined8 *)*param_4;
    if (-1 < (char)bVar1) {
      uVar5 = (ulong)bVar1;
      puVar9 = param_4;
    }
    FUN_00359ad8(&local_d0,puVar9,uVar5);
  }
  else if ((char)bVar1 < '\0') {
    FUN_00359998(&local_d0,*param_4,param_4[1]);
  }
  else {
    uStack_c8 = param_4[1];
    local_d0 = *param_4;
    local_c0 = param_4[2];
  }
  if (3 < param_3) {
                    /* WARNING: Does not return */
    pcVar8 = (code *)SoftwareBreakpoint(0x550b,0x42f895c);
    (*pcVar8)();
  }
  local_150 = CONCAT44(local_150._4_4_,(uint)(byte)(&DAT_0d7420a0)[param_3]);
  FUN_03510c88(auStack_238,&local_150);
  plVar6 = (long *)FUN_03510498(auStack_238);
  FUN_024708d0(auStack_238);
  lVar3 = (**(code **)(*plVar6 + 0x18))(plVar6);
  for (plVar10 = *(long **)(lVar3 + 8); plVar10 != (long *)0x0; plVar10 = (long *)*plVar10) {
    while ((undefined **)plVar10[4] < (undefined **)0xdf3bb11) {
      if ((undefined **)plVar10[4] == &PTR_s_BrowserInitState_kDataKeyImpl_0df3bb10) {
        if (plVar10 == (long *)(lVar3 + 8)) goto LAB_042f88ac;
        uVar5 = plVar10[5];
        if ((uVar5 & 0xfffffffc00000000) == DAT_0e2f2208) {
          FUN_0000ffa8(uVar5);
        }
        goto LAB_042f8724;
      }
      plVar10 = (long *)plVar10[1];
      if (plVar10 == (long *)0x0) goto LAB_042f88ac;
    }
  }
LAB_042f88ac:
  uVar5 = 0;
LAB_042f8724:
  if ((uVar5 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4(uVar5);
  }
  FUN_07c80940(auStack_320,uVar5);
  auVar15 = FUN_04307700(auStack_320);
  plVar10 = (long *)param_1[0x41];
  *(undefined1 (*) [16])(param_1 + 0x40) = auVar15;
  if (plVar10 != (long *)0x0) {
    LOAcquire();
    lVar3 = plVar10[1];
    plVar10[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      (**(code **)(*plVar10 + 0x10))(plVar10);
      if (plVar10[2] != 0) {
        LOAcquire();
        lVar3 = plVar10[2];
        plVar10[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_042f8770;
      }
      pcVar8 = *(code **)(*plVar10 + 0x20);
      if (((pcVar8 == FUN_00f01bc0) || (pcVar8 == FUN_00f01bc0)) || (pcVar8 == FUN_013efb40)) {
        FUN_00005810(plVar10,0x20);
      }
      else {
        (*pcVar8)(plVar10);
      }
    }
  }
LAB_042f8770:
  FUN_024708d0(auStack_320);
  FUN_042f7cc0(param_1,plVar6);
  FUN_024708d0(&local_150);
  return param_1;
}


