// ==== 0x42aa7ec  FUN_042aa7ec  (692 bytes)

undefined8
FUN_042aa7ec(ulong *param_1,ulong *param_2,long param_3,undefined8 param_4,undefined8 param_5,
            undefined8 *param_6)

{
  code *pcVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  ulong uVar4;
  undefined8 uVar5;
  undefined1 extraout_w1;
  ulong uVar6;
  ulong local_b0;
  undefined4 local_a8;
  ulong local_a0;
  ulong uStack_98;
  ulong local_90;
  ulong local_88;
  ulong uStack_80;
  long local_78;
  undefined8 local_70;
  undefined8 uStack_68;
  ulong local_60;
  
  if (DAT_0e4a2fd8 == (undefined8 *)0x0) {
    uVar4 = param_2[1];
    uVar6 = param_2[2];
    puVar2 = (undefined8 *)FUN_00008790(0x38);
    puVar3 = (undefined8 *)FUN_00008790(0x18);
    *puVar2 = puVar3;
    puVar2[2] = 1;
    if (puVar3 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x42aaaa0);
      (*pcVar1)();
    }
    *puVar3 = 0;
    puVar3[1] = 0;
    puVar3[2] = 0;
    puVar2[1] = 1;
    puVar2[4] = 0;
    puVar2[3] = puVar2 + 4;
    puVar2[5] = 0;
    puVar2[6] = 1;
    DAT_0e4a2fd8 = puVar2;
    FUN_01b2da94(puVar2,(int)uVar4,uVar6);
  }
  FUN_040462ac();
  if ((param_1[5] & 1) == 0) {
    uVar4 = FUN_043abf0c(0);
    param_1[4] = uVar4;
    *(undefined1 *)(param_1 + 5) = extraout_w1;
  }
  if (param_3 == 0) {
    param_3 = (*(code *)PTR_FUN_0e2f1ea8)();
  }
  local_78 = param_3;
  uVar4 = FUN_00008790(0x130);
  if (*(char *)((long)param_6 + 0x17) < '\0') {
    FUN_00060a60(&local_70,*param_6,param_6[1]);
  }
  else {
    uStack_68 = param_6[1];
    local_70 = *param_6;
    local_60 = param_6[2];
  }
  FUN_0429608c(uVar4,param_1,&local_78,&local_70);
  if ((long)local_60 < 0) {
    FUN_00005810(local_70,local_60 & 0x7fffffffffffffff);
  }
  puVar2 = (undefined8 *)*param_1;
  *param_1 = uVar4;
  if (puVar2 != (undefined8 *)0x0) {
    *puVar2 = &PTR_FUN_0da7a7f8;
    if (*(char *)((long)puVar2 + 0x12f) < '\0') {
      FUN_00005810(puVar2[0x23],puVar2[0x25] & 0x7fffffffffffffff);
    }
    FUN_03da8400(puVar2 + 0x15);
    if ((puVar2[0x14] & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4();
    }
    puVar2[0x14] = 0;
    uVar5 = FUN_02e75440(puVar2);
    FUN_00005810(uVar5,0x130);
    uVar4 = *param_1;
  }
  if ((uVar4 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(uVar4);
  }
  if ((*param_2 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4();
  }
  *param_2 = uVar4;
  local_70 = _objc_autoreleasePoolPush();
  param_2[6] = (ulong)&local_70;
  puVar2 = (undefined8 *)FUN_00008790(0x100);
  *puVar2 = &PTR_LAB_0da06428;
  *(undefined1 *)(puVar2 + 1) = 0;
  *(undefined1 *)(puVar2 + 2) = 0;
  *(undefined1 *)(puVar2 + 0xc) = 0;
  *(undefined1 *)(puVar2 + 0x1b) = 0;
  *(undefined1 *)(puVar2 + 0x1c) = 0;
  *(undefined1 *)(puVar2 + 0x1f) = 0;
  puVar2[0xe] = 0;
  puVar2[0xd] = 0;
  puVar2[0x10] = 0;
  puVar2[0xf] = 0;
  *(undefined2 *)(puVar2 + 0x11) = 0;
  puVar2[0x12] = 0;
  puVar2[0x13] = 0;
  *(undefined1 *)(puVar2 + 0x14) = 0;
  uVar4 = param_1[2];
  param_1[2] = (ulong)puVar2;
  if (uVar4 != 0) {
    uVar5 = FUN_02d2f978();
    FUN_00005810(uVar5,0x100);
  }
  local_b0 = *param_2;
  *param_2 = 0;
  local_a8 = (undefined4)param_2[1];
  uStack_98 = param_2[3];
  local_a0 = param_2[2];
  param_2[2] = 0;
  param_2[3] = 0;
  local_90 = param_2[4];
  param_2[4] = 0;
  uStack_80 = param_2[6];
  local_88 = param_2[5];
  uVar5 = FUN_02d2f3cc(&local_b0,param_1[2]);
  FUN_02440774(&local_b0);
  _objc_autoreleasePoolPop(local_70);
  return uVar5;
}


