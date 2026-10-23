// ==== 0x9278c  FUN_0009278c  (876 bytes)

undefined8 FUN_0009278c(long *param_1,undefined8 param_2,int param_3)

{
  long *plVar1;
  undefined8 *puVar2;
  code *pcVar3;
  long *plVar4;
  undefined8 *puVar5;
  ulong uVar6;
  ulong uVar7;
  undefined8 *puVar8;
  long lVar9;
  ulong uVar10;
  ulong uVar11;
  undefined8 uVar12;
  bool bVar13;
  long *local_c0;
  undefined4 local_b8;
  long *local_b0;
  undefined8 uStack_a8;
  undefined1 local_a0;
  long *local_98;
  undefined4 local_90;
  long *local_88;
  undefined8 uStack_80;
  undefined1 local_78;
  undefined8 local_70;
  long *plStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  
  local_70 = 0x1b;
  if (DAT_0e4a446b != '\0') {
    local_98 = (long *)0xd006234;
    local_c0 = param_1;
    FUN_04acc3c8(DAT_0e4a446b,&local_70,&local_98,&local_c0);
  }
  FUN_00092b20(param_1,param_2);
  plVar4 = (long *)(**(code **)(*param_1 + 0x18))(param_1,param_2);
  if (plVar4 == (long *)0x0) {
LAB_000928cc:
    uVar12 = 0;
    goto LAB_00092898;
  }
  puVar8 = (undefined8 *)param_1[4];
  lVar9 = param_1[5];
  uVar7 = (lVar9 * 0x28 >> 3) * -0x3333333333333333;
  puVar2 = puVar8;
  uVar6 = uVar7;
  uVar10 = lVar9 * 0x28;
  while (puVar5 = puVar2, uVar10 != 0) {
    uVar11 = uVar6 >> 1;
    uVar6 = uVar6 + ~uVar11;
    puVar2 = puVar5 + uVar11 * 5 + 5;
    uVar10 = uVar6;
    if (plVar4 <= (long *)puVar5[uVar11 * 5]) {
      puVar2 = puVar5;
      uVar6 = uVar11;
      uVar10 = uVar11;
    }
  }
  if ((puVar5 == puVar8 + lVar9 * 5) || (plVar4 < (long *)*puVar5)) {
    uStack_58 = 0;
    local_70 = 1;
    local_b8 = 1;
    plStack_68 = (long *)0x0;
    uStack_60 = 0;
    local_a0 = 0;
    local_90 = 1;
    local_88 = (long *)0x0;
    uStack_80 = 0;
    local_b0 = (long *)0x0;
    uStack_a8 = 0;
    local_78 = 0;
    puVar2 = puVar8;
    uVar6 = lVar9 * 0x28;
    while (puVar5 = puVar2, uVar6 != 0) {
      uVar10 = uVar7 >> 1;
      uVar7 = uVar7 + ~(uVar7 >> 1);
      puVar2 = puVar5 + uVar10 * 5 + 5;
      uVar6 = uVar7;
      if (plVar4 <= (long *)puVar5[uVar10 * 5]) {
        puVar2 = puVar5;
        uVar7 = uVar10;
        uVar6 = uVar10;
      }
    }
    local_c0 = plVar4;
    local_98 = plVar4;
    if ((puVar5 == puVar8 + lVar9 * 5) || (plVar4 < (long *)*puVar5)) {
      puVar5 = (undefined8 *)FUN_00d5ffe0(param_1 + 4,puVar5,&local_98);
      bVar13 = true;
    }
    else {
      bVar13 = false;
    }
    FUN_00d5ff9c(&uStack_80);
    plVar1 = local_88;
    local_88 = (long *)0x0;
    if (plVar1 != (long *)0x0) {
      (**(code **)(*plVar1 + 8))();
    }
    FUN_00d5ff9c(&uStack_a8);
    plVar1 = local_b0;
    local_b0 = (long *)0x0;
    if (plVar1 != (long *)0x0) {
      (**(code **)(*plVar1 + 8))();
    }
    if (!bVar13) {
                    /* WARNING: Does not return */
      pcVar3 = (code *)SoftwareBreakpoint(0,0x92aa0);
      (*pcVar3)();
    }
    FUN_00d5ff9c(&uStack_60);
    plVar1 = plStack_68;
    plStack_68 = (long *)0x0;
    if (plVar1 != (long *)0x0) {
      (**(code **)(*plVar1 + 8))();
    }
    puVar8 = (undefined8 *)param_1[4];
    lVar9 = param_1[5];
  }
  if (puVar8 + lVar9 * 5 == puVar5) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(0,0x929e0);
    (*pcVar3)();
  }
  if (3 < *(int *)(puVar5 + 1)) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(0,0x929ec);
    (*pcVar3)();
  }
  if (*(int *)(puVar5 + 1) < 2) {
    if (puVar5[2] != 0) {
                    /* WARNING: Does not return */
      pcVar3 = (code *)SoftwareBreakpoint(0,0x92a34);
      (*pcVar3)();
    }
    if (param_3 == 0) goto LAB_000928cc;
    *(undefined4 *)(puVar5 + 1) = 2;
    if (puVar5[3] == 0) {
      if ((*(char *)(puVar5 + 4) == '\0') ||
         (uVar6 = (**(code **)(*param_1 + 0x30))(param_1), (uVar6 & 1) == 0)) {
        uVar12 = (**(code **)(*param_1 + 0x68))(param_1,plVar4);
        plVar4 = (long *)puVar5[2];
        puVar5[2] = uVar12;
        goto joined_r0x00092a68;
      }
    }
    else {
      local_98 = (long *)puVar5[3];
      puVar5[3] = 0;
      uVar12 = (*(code *)local_98[1])(local_98,plVar4);
      FUN_00d5ff9c(&local_98);
      plVar4 = (long *)puVar5[2];
      puVar5[2] = uVar12;
joined_r0x00092a68:
      if (plVar4 != (long *)0x0) {
        (**(code **)(*plVar4 + 8))();
      }
    }
    if (*(int *)(puVar5 + 1) < 2) {
                    /* WARNING: Does not return */
      pcVar3 = (code *)SoftwareBreakpoint(0,0x92928);
      (*pcVar3)();
    }
  }
  uVar12 = puVar5[2];
LAB_00092898:
  local_c0 = (long *)0x1b;
  if (DAT_0e4a446b != '\0') {
    local_98 = (long *)0x0;
    FUN_0000a280(DAT_0e4a446b,&local_c0,&local_98,2);
  }
  return uVar12;
}


