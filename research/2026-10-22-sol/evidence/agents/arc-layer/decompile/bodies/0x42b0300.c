// ==== 0x42b0300  FUN_042b0300  (1304 bytes)

undefined8 * FUN_042b0300(undefined8 *param_1)

{
  long *plVar1;
  long *plVar2;
  undefined8 *puVar3;
  int iVar4;
  long lVar5;
  int *piVar6;
  long *plVar7;
  code *pcVar8;
  ulong uVar9;
  undefined8 *puVar10;
  undefined8 *puVar11;
  undefined8 *puVar12;
  
  param_1[3] = 0;
  param_1[4] = 0;
  *param_1 = &PTR_FUN_0da8a590;
  piVar6 = (int *)FUN_00008790(8);
  *piVar6 = 0;
  *(undefined1 *)(piVar6 + 1) = 0;
  if ((piVar6 != (int *)0x0) && (iVar4 = *piVar6, *piVar6 = iVar4 + 1, iVar4 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar8 = (code *)SoftwareBreakpoint(0,0x42b0808);
    (*pcVar8)();
  }
  param_1[1] = piVar6;
  param_1[2] = param_1;
  *param_1 = &PTR_thunk_FUN_02d6dee4_0dd03af8;
  param_1[5] = &PTR__cxx_destruct_0da7bd30;
  puVar11 = param_1 + 6;
  *puVar11 = &PTR_FUN_0da8a590;
  piVar6 = (int *)FUN_00008790(8);
  *piVar6 = 0;
  *(undefined1 *)(piVar6 + 1) = 0;
  if ((piVar6 != (int *)0x0) && (iVar4 = *piVar6, *piVar6 = iVar4 + 1, iVar4 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar8 = (code *)SoftwareBreakpoint(0,0x42b0814);
    (*pcVar8)();
  }
  puVar10 = param_1 + 9;
  *puVar10 = &PTR_viewWillStartLiveResize_0da7cf18;
  param_1[7] = piVar6;
  param_1[8] = puVar11;
  param_1[6] = &PTR_FUN_0da7bd60;
  puVar12 = param_1 + 10;
  *puVar12 = &PTR_FUN_0da8a590;
  piVar6 = (int *)FUN_00008790(8);
  *piVar6 = 0;
  *(undefined1 *)(piVar6 + 1) = 0;
  if ((piVar6 != (int *)0x0) && (iVar4 = *piVar6, *piVar6 = iVar4 + 1, iVar4 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar8 = (code *)SoftwareBreakpoint(0,0x42b0820);
    (*pcVar8)();
  }
  param_1[0xb] = piVar6;
  param_1[0xc] = puVar12;
  *param_1 = &PTR_LAB_0da7bb60;
  param_1[5] = &PTR__cxx_destruct_0da7bbc8;
  param_1[6] = &PTR_LAB_0da7bbf8;
  param_1[9] = &PTR_LAB_0da7bc20;
  param_1[10] = &PTR_LAB_0da7bcd0;
  param_1[0xd] = 0;
  param_1[0x10] = 0;
  param_1[0xe] = 0;
  *(undefined1 *)(param_1 + 0xf) = 0;
  piVar6 = (int *)FUN_00008790(8);
  *piVar6 = 0;
  *(undefined1 *)(piVar6 + 1) = 0;
  if ((piVar6 != (int *)0x0) && (iVar4 = *piVar6, *piVar6 = iVar4 + 1, iVar4 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar8 = (code *)SoftwareBreakpoint(0,0x42b07fc);
    (*pcVar8)();
  }
  param_1[0x11] = piVar6;
  param_1[0x12] = param_1 + 0xd;
  param_1[0x13] = 0;
  *(undefined1 *)(param_1 + 0x14) = 0;
  *(undefined1 *)(param_1 + 0x16) = 0;
  param_1[0x17] = 0;
  param_1[0x18] = 0;
  plVar7 = (long *)FUN_00008790(0xe0);
  plVar7[1] = 0;
  plVar7[2] = 0;
  *plVar7 = (long)&PTR_viewWillStartLiveResize_0da7bd88;
  plVar1 = plVar7 + 3;
  if (plVar1 == (long *)0x0) {
                    /* WARNING: Does not return */
    pcVar8 = (code *)SoftwareBreakpoint(1,0x42b0804);
    (*pcVar8)();
  }
  plVar7[9] = 0;
  plVar7[6] = 0;
  plVar7[5] = 0;
  plVar7[8] = 0;
  plVar7[7] = 0;
  plVar7[4] = 0;
  *plVar1 = 0;
  plVar7[0x16] = (long)&PTR_FUN_0da8a590;
  *(undefined4 *)(plVar7 + 10) = 0x3f800000;
  plVar7[0xc] = 0;
  plVar7[0xb] = 0;
  plVar7[0xe] = 0;
  plVar7[0xd] = 0;
  *(undefined4 *)(plVar7 + 0xf) = 0x3f800000;
  plVar7[0x11] = 0;
  plVar7[0x10] = 0;
  plVar7[0x13] = 0;
  plVar7[0x12] = 0;
  *(undefined4 *)(plVar7 + 0x14) = 0x3f800000;
  *(undefined4 *)(plVar7 + 0x15) = 0;
  piVar6 = (int *)FUN_00008790(8);
  *piVar6 = 0;
  *(undefined1 *)(piVar6 + 1) = 0;
  if ((piVar6 != (int *)0x0) && (iVar4 = *piVar6, *piVar6 = iVar4 + 1, iVar4 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar8 = (code *)SoftwareBreakpoint(0,0x42b082c);
    (*pcVar8)();
  }
  plVar7[0x16] = (long)&PTR_FUN_0dabb640;
  plVar7[0x17] = (long)piVar6;
  plVar7[0x18] = (long)(plVar7 + 0x16);
  plVar7[0x19] = 0;
  plVar7[0x1b] = 0;
  if (plVar7 != (long *)0xffffffffffffffe0) {
    if (plVar7 == (long *)0x0) {
      segment_command_00000020.cmd = 0x18;
      segment_command_00000020.cmdsize = 0;
      segment_command_00000020.segname[0] = '\0';
      segment_command_00000020.segname[1] = '\0';
      segment_command_00000020.segname[2] = '\0';
      segment_command_00000020.segname[3] = '\0';
      segment_command_00000020.segname[4] = '\0';
      segment_command_00000020.segname[5] = '\0';
      segment_command_00000020.segname[6] = '\0';
      segment_command_00000020.segname[7] = '\0';
    }
    else {
      plVar2 = plVar7 + 1;
      *plVar2 = *plVar2 + 1;
      plVar7[2] = plVar7[2] + 1;
      plVar7[4] = (long)plVar1;
      plVar7[5] = (long)plVar7;
      LOAcquire();
      lVar5 = *plVar2;
      *plVar2 = lVar5 + -1;
      LORelease();
      if (lVar5 == 0) {
        (**(code **)(*plVar7 + 0x10))(plVar7);
        if (plVar7[2] != 0) {
          LOAcquire();
          lVar5 = plVar7[2];
          plVar7[2] = lVar5 + -1;
          LORelease();
          if (lVar5 != 0) goto LAB_042b05b8;
        }
        pcVar8 = *(code **)(*plVar7 + 0x20);
        if (((pcVar8 == FUN_00f01bc0) || (pcVar8 == FUN_00f01bc0)) || (pcVar8 == FUN_013efb40)) {
          FUN_00005810(plVar7,0x20);
        }
        else {
          (*pcVar8)(plVar7);
        }
      }
    }
  }
LAB_042b05b8:
  param_1[0x19] = plVar1;
  param_1[0x1a] = plVar7;
  param_1[0x1c] = 0;
  param_1[0x1b] = 0;
  param_1[0x1e] = 0;
  param_1[0x1d] = 0;
  *(undefined4 *)(param_1 + 0x1f) = 0x3f800000;
  param_1[0x21] = 0;
  param_1[0x20] = 0;
  param_1[0x23] = 0;
  param_1[0x22] = 0;
  *(undefined4 *)(param_1 + 0x24) = 0x3f800000;
  *(undefined1 *)(param_1 + 0x25) = 0;
  *(undefined1 *)(param_1 + 0x28) = 0;
  *(undefined2 *)(param_1 + 0x29) = 0;
  param_1[0x2c] = FUN_040490c0;
  param_1[0x2d] = &DAT_0c4b3500;
  param_1[0x30] = FUN_040490c0;
  param_1[0x31] = &DAT_0c4b3500;
  *(undefined4 *)(param_1 + 0x32) = 0;
  param_1[0x35] = 0;
  *(undefined4 *)(param_1 + 0x36) = 0;
  param_1[0x34] = 0;
  param_1[0x33] = 0;
  param_1[0x3a] = 0;
  param_1[0x39] = 0;
  param_1[0x38] = 0;
  *(undefined4 *)(param_1 + 0x3b) = 0xffffffff;
  param_1[0x3d] = 0;
  param_1[0x3c] = 0;
  param_1[0x3f] = 0;
  param_1[0x3e] = 0;
  param_1[0x41] = 0;
  param_1[0x40] = 0;
  param_1[0x37] = &PTR_FUN_0da8bbe8;
  param_1[0x42] = 0;
  param_1[0x46] = 0;
  param_1[0x45] = 0;
  param_1[0x44] = 0;
  *(undefined4 *)(param_1 + 0x47) = 0xffffffff;
  param_1[0x49] = 0;
  param_1[0x48] = 0;
  param_1[0x4b] = 0;
  param_1[0x4a] = 0;
  param_1[0x4d] = 0;
  param_1[0x4c] = 0;
  param_1[0x43] = &PTR_FUN_0da8bbe8;
  param_1[0x4e] = 0;
  param_1[0x52] = 0;
  param_1[0x51] = 0;
  param_1[0x50] = 0;
  *(undefined4 *)(param_1 + 0x53) = 0xffffffff;
  param_1[0x55] = 0;
  param_1[0x54] = 0;
  param_1[0x57] = 0;
  param_1[0x56] = 0;
  param_1[0x4f] = &PTR_FUN_0da8bc48;
  param_1[0x5e] = 0;
  param_1[0x5d] = 0;
  param_1[0x59] = 0;
  param_1[0x58] = 0;
  param_1[0x5b] = 0;
  param_1[0x5a] = 0;
  param_1[0x5c] = param_1 + 0x5d;
  *(undefined4 *)(param_1 + 0x5f) = 0;
  puVar3 = param_1 + 0x69;
  *(undefined2 *)(param_1 + 0x68) = 0;
  param_1[0x61] = 0;
  param_1[0x60] = 0;
  param_1[99] = 0;
  param_1[0x62] = 0;
  param_1[0x65] = 0;
  param_1[100] = 0;
  param_1[0x67] = 0;
  param_1[0x66] = 0;
  param_1[0x69] = &PTR_viewWillStartLiveResize_0da7bcf8;
  *(undefined1 *)(param_1 + 0x6a) = 0;
  *(undefined1 *)(param_1 + 0x6e) = 0;
  param_1[0x70] = 0;
  param_1[0x6f] = 0;
  param_1[0x72] = 0;
  param_1[0x71] = 0;
  *(undefined4 *)(param_1 + 0x73) = 0x3f800000;
  param_1[0x75] = 0;
  param_1[0x74] = 0;
  param_1[0x77] = 0;
  param_1[0x76] = 0;
  *(undefined4 *)(param_1 + 0x78) = 0x3f800000;
  if (((ulong)puVar3 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(param_1 + 0x69);
  }
  param_1[0x79] = puVar3;
  param_1[0x7a] = 1;
  piVar6 = (int *)FUN_00008790(8);
  *piVar6 = 0;
  *(undefined1 *)(piVar6 + 1) = 0;
  if ((piVar6 != (int *)0x0) && (iVar4 = *piVar6, *piVar6 = iVar4 + 1, iVar4 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar8 = (code *)SoftwareBreakpoint(0,0x42b0838);
    (*pcVar8)();
  }
  param_1[0x7c] = piVar6;
  param_1[0x7d] = puVar3;
  if (((ulong)puVar11 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(puVar11);
  }
  uVar9 = DAT_0e2f2208;
  param_1[0x7e] = puVar11;
  param_1[0x7f] = 1;
  if (((ulong)puVar12 & 0xfffffffc00000000) == uVar9) {
    FUN_0000ffa8(puVar12);
    uVar9 = DAT_0e2f2208;
  }
  param_1[0x81] = puVar12;
  param_1[0x82] = 1;
  if (((ulong)puVar10 & 0xfffffffc00000000) == uVar9) {
    FUN_0000ffa8(puVar10);
  }
  param_1[0x84] = puVar10;
  param_1[0x85] = 0;
  if (((ulong)param_1 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(param_1);
  }
  param_1[0x86] = param_1;
  param_1[0x88] = 0;
  param_1[0x87] = 0;
  param_1[0x8a] = 0;
  param_1[0x89] = 0;
  param_1[0x8c] = 0;
  param_1[0x8b] = 0;
  piVar6 = (int *)FUN_00008790(8);
  *piVar6 = 0;
  *(undefined1 *)(piVar6 + 1) = 0;
  if ((piVar6 != (int *)0x0) && (iVar4 = *piVar6, *piVar6 = iVar4 + 1, iVar4 == 0x7fffffff)) {
                    /* WARNING: Does not return */
    pcVar8 = (code *)SoftwareBreakpoint(0,0x42b0844);
    (*pcVar8)();
  }
  param_1[0x8d] = piVar6;
  param_1[0x8e] = param_1;
  return param_1;
}


