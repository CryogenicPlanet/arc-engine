// ==== 0x42ccef8  FUN_042ccef8  (1192 bytes)

/* WARNING: Removing unreachable block (ram,0x042ccfd4) */
/* WARNING: Removing unreachable block (ram,0x042cd328) */

void FUN_042ccef8(long param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                 undefined8 *param_5)

{
  char cVar1;
  code *pcVar2;
  int iVar3;
  long *plVar4;
  undefined8 *puVar5;
  long lVar6;
  undefined8 uVar7;
  undefined4 *puVar8;
  undefined8 uVar9;
  int *piVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  undefined8 *local_228;
  undefined1 local_220;
  undefined7 uStack_21f;
  ulong local_218;
  undefined8 local_210;
  char local_208;
  long local_200;
  ulong uStack_1f8;
  undefined8 local_1f0;
  ulong local_1e8;
  long local_1d8;
  int local_1d0;
  long local_1c8;
  long local_1c0;
  ulong uStack_1b8;
  undefined8 local_1b0;
  ulong local_1a8;
  undefined8 local_1a0;
  undefined7 local_190;
  char cStack_189;
  undefined8 local_168;
  undefined7 local_158;
  char cStack_151;
  undefined8 local_150;
  undefined7 local_140;
  char cStack_139;
  undefined8 local_118;
  undefined7 local_108;
  char cStack_101;
  undefined8 local_100;
  undefined7 local_f0;
  char cStack_e9;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  ulong local_60;
  
  if (*(long *)(param_1 + 0x88) == 0) {
                    /* WARNING: Could not recover jumptable at 0x042ccfbc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)param_5[2])(param_5,0);
    return;
  }
  local_60 = 0;
  uStack_78 = 0;
  local_80 = 0;
  uStack_68 = 0;
  uStack_70 = 0;
  uStack_98 = 0;
  local_a0 = 0;
  uStack_88 = 0;
  local_90 = 0;
  uStack_a8 = 0;
  local_b0 = 0;
  FUN_042c9420(param_2);
  if ((local_60 & 1) == 0) {
LAB_042ccf54:
    (*(code *)param_5[2])(param_5,0);
    return;
  }
  FUN_0119426c(*(undefined8 *)(param_1 + 0x88));
  plVar4 = (long *)FUN_067688b0();
  if (plVar4 == (long *)0x0) goto LAB_042ccf54;
  puVar5 = (undefined8 *)FUN_00008790(0x30);
  puVar5[1] = 0;
  *puVar5 = 0;
  puVar5[3] = 0;
  puVar5[2] = 0;
  puVar5[5] = 0;
  puVar5[4] = 0;
  FUN_01996f10(&local_1c0,&local_b0);
  FUN_07a82090(&local_1d8,puVar5,0);
  (**(code **)(*plVar4 + 0x10))(plVar4,&local_1c0,&local_1d8);
  if ((local_1d0 == 1) && (local_1c8 != 0)) {
    FUN_084a7784();
  }
  lVar6 = local_1c8;
  local_1d0 = 0;
  local_1c8 = 0;
  if (lVar6 != 0) {
    piVar10 = (int *)(lVar6 + 8);
    LOAcquire();
    iVar3 = *piVar10;
    *piVar10 = iVar3 + -1;
    LORelease();
    if (iVar3 == 1) {
      uVar7 = FUN_084a76fc();
      FUN_00005810(uVar7,0x38);
    }
    if (local_1c8 != 0) {
                    /* WARNING (jumptable): Read-only address (ram,0x00000008) is written */
      LOAcquire();
      iVar3 = *(int *)(local_1c8 + 8);
      *(int *)(local_1c8 + 8) = iVar3 + -1;
      LORelease();
      if (iVar3 == 1) {
        uVar7 = FUN_084a76fc();
        FUN_00005810(uVar7,0x38);
      }
    }
  }
  if (local_1d8 != 0) {
    FUN_005a359c();
  }
  if (cStack_e9 < '\0') {
    FUN_00005810(local_100,CONCAT17(cStack_e9,local_f0) & 0x7fffffffffffffff);
  }
  if (cStack_101 < '\0') {
    FUN_00005810(local_118,CONCAT17(cStack_101,local_108) & 0x7fffffffffffffff);
  }
  if (cStack_139 < '\0') {
    FUN_00005810(local_150,CONCAT17(cStack_139,local_140) & 0x7fffffffffffffff);
  }
  if (cStack_151 < '\0') {
    FUN_00005810(local_168,CONCAT17(cStack_151,local_158) & 0x7fffffffffffffff);
  }
  if (cStack_189 < '\0') {
    FUN_00005810(local_1a0,CONCAT17(cStack_189,local_190) & 0x7fffffffffffffff);
  }
  if ((long)local_1a8 < 0) {
    FUN_00005810(uStack_1b8,local_1a8 & 0x7fffffffffffffff);
  }
  uStack_1b8 = 0;
  local_1c0 = 0;
  local_1a8 = 0;
  local_1b0 = 0;
  FUN_042ccc54(&local_1c0,param_3);
  uStack_1f8 = 0;
  local_200 = 0;
  local_1e8 = 0;
  local_1f0 = 0;
  FUN_042ccc54(&local_200,param_4);
  cVar1 = (char)local_1e8;
  if (((char)local_1a8 == '\0') || ((char)local_1e8 == '\0')) {
    (*(code *)param_5[2])(param_5,0);
    if (cVar1 != '\0') goto LAB_042cd114;
  }
  else {
    FUN_07a821a4(puVar5);
    uVar7 = puVar5[5];
    local_220 = 0;
    local_208 = '\0';
    local_228 = (undefined8 *)0x0;
    piVar10 = *(int **)(param_1 + 0x310);
    if ((piVar10 != (int *)0x0) && (iVar3 = *piVar10, *piVar10 = iVar3 + 1, iVar3 == 0x7fffffff)) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(0,0x42cd37c);
      (*pcVar2)();
    }
    uVar9 = *(undefined8 *)(param_1 + 0x318);
    puVar8 = (undefined4 *)FUN_00008790(0x68);
    *puVar8 = 1;
    *(undefined1 **)(puVar8 + 2) = &LAB_042da9b0;
    *(code **)(puVar8 + 4) = FUN_042daa90;
    *(undefined1 **)(puVar8 + 6) = &LAB_04169bc0;
    *(code **)(puVar8 + 8) = FUN_042cd3a8;
    *(undefined8 *)(puVar8 + 10) = 0;
    *(int **)(puVar8 + 0xc) = piVar10;
    *(undefined8 *)(puVar8 + 0xe) = uVar9;
    lVar6 = param_5[3];
    uVar9 = *param_5;
    uVar12 = param_5[3];
    uVar11 = param_5[2];
    *(undefined8 *)(puVar8 + 0x12) = param_5[1];
    *(undefined8 *)(puVar8 + 0x10) = uVar9;
    *(undefined8 *)(puVar8 + 0x16) = uVar12;
    *(undefined8 *)(puVar8 + 0x14) = uVar11;
    if (*(long *)(lVar6 + 8) != 0) {
      param_5[2] = 0;
      param_5[3] = &DAT_0c4b3500;
    }
    *(undefined8 **)(puVar8 + 0x18) = puVar5;
    FUN_0578cd84(uVar7,&local_1c0,&local_200,&local_220,&local_228);
    puVar5 = local_228;
    local_228 = (undefined8 *)0x0;
    if (puVar5 != (undefined8 *)0x0) {
      FUN_000282bc(puVar5 + 0xe,0);
      FUN_0002827c(puVar5 + 4);
      if (*(char *)((long)puVar5 + 0x17) < '\0') {
        FUN_00005810(*puVar5,puVar5[2] & 0x7fffffffffffffff);
      }
      FUN_00005810(puVar5,0x88);
    }
    if ((local_208 != '\0') && (lVar6 = CONCAT71(uStack_21f,local_220), lVar6 != 0)) {
      if ((ulong)-lVar6 < local_218) goto LAB_042cd12c;
      local_218 = 0;
      FUN_00005810(lVar6,local_210);
    }
    puVar5 = (undefined8 *)0x0;
    if ((local_1e8 & 1) != 0) {
LAB_042cd114:
      if (local_200 != 0) {
        if ((ulong)-local_200 < uStack_1f8) goto LAB_042cd12c;
        uStack_1f8 = 0;
        FUN_00005810(local_200,local_1f0);
      }
    }
  }
  if (((char)local_1a8 != '\0') && (local_1c0 != 0)) {
    if ((ulong)-local_1c0 < uStack_1b8) {
LAB_042cd12c:
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(1,0x42cd130);
      (*pcVar2)();
    }
    uStack_1b8 = 0;
    FUN_00005810(local_1c0,local_1b0);
  }
  if (puVar5 == (undefined8 *)0x0) {
    return;
  }
  lVar6 = puVar5[5];
  puVar5[5] = 0;
  if (lVar6 != 0) {
    FUN_00005810(lVar6,0x10);
  }
  uVar7 = FUN_00111f30(puVar5);
  FUN_00005810(uVar7,0x30);
  return;
}


