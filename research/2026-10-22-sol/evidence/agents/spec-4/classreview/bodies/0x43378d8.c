// ==== 0x43378d8  FUN_043378d8  (556 bytes)

void FUN_043378d8(undefined8 param_1,undefined8 param_2,ulong param_3,long param_4,
                 undefined8 param_5,long param_6)

{
  bool bVar1;
  bool bVar2;
  int iVar3;
  ulong uVar4;
  float fVar5;
  undefined1 auStack_2c0 [432];
  undefined1 local_110;
  undefined1 local_108;
  undefined8 local_100;
  undefined4 local_f8;
  undefined8 local_f0;
  undefined8 uStack_e8;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined8 uStack_c8;
  undefined4 local_c0;
  long local_b8;
  undefined8 uStack_b0;
  int *local_a8;
  undefined8 local_a0;
  ulong uStack_98;
  long *local_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  int *local_78;
  undefined8 local_70;
  undefined8 uStack_68;
  int *local_60;
  undefined8 local_58;
  ulong uStack_50;
  
  bVar1 = false;
  bVar2 = false;
  if (param_6 != 0) {
    bVar1 = false;
    bVar2 = true;
    fVar5 = (float)param_2;
    if (!NAN(fVar5)) {
      bVar1 = fVar5 == 0.0;
      bVar2 = 0.0 <= fVar5;
    }
  }
  if (((!bVar2 || bVar1) || param_4 == 0) || param_3 == 0) {
    FUN_00434000(auStack_2c0);
    local_110 = 0;
    local_108 = 0;
    local_100 = 1;
    local_f8 = 0;
    uStack_e8 = 0;
    local_f0 = 0;
    uStack_d8 = 0;
    uStack_e0 = 0;
    uStack_c8 = 0;
    local_d0 = 0;
    local_c0 = 0;
  }
  else {
    uVar4 = -(param_3 >> 0x1f & 1) & 0xfffffffc00000000 | (param_3 & 0xffffffff) << 2;
    uStack_50 = param_3 & 0xffffffff | param_4 << 0x20;
    uStack_68 = 0;
    local_70 = 0;
    local_58 = 0x200000006;
    local_60 = (int *)0x0;
    if (uVar4 >> 0x1f != 0) {
      uVar4 = 0;
    }
    uStack_88 = 0;
    local_90 = (long *)0x0;
    local_78 = (int *)0x0;
    uStack_80 = 0;
    uVar4 = FUN_01265cfc(&local_90,&local_60,uVar4);
    if ((uVar4 & 1) == 0) {
      FUN_00434000(auStack_2c0);
      local_110 = 0;
      local_108 = 0;
      local_100 = 1;
      local_f8 = 0;
      uStack_e8 = 0;
      local_f0 = 0;
      uStack_d8 = 0;
      uStack_e0 = 0;
      uStack_c8 = 0;
      local_d0 = 0;
      local_c0 = 0;
    }
    else {
      if (local_60 != (int *)0x0) {
        *local_60 = *local_60 + 1;
      }
      local_a8 = local_60;
      uStack_98 = uStack_50;
      local_a0 = local_58;
      local_b8 = param_6;
      uStack_b0 = param_5;
      uVar4 = FUN_005cfe40(&local_90,&local_b8,0,0);
      if ((uVar4 & 1) == 0) {
        FUN_00434000(auStack_2c0);
        local_110 = 0;
        local_108 = 0;
        local_100 = 1;
        local_f8 = 0;
        uStack_e8 = 0;
        local_f0 = 0;
        uStack_d8 = 0;
        uStack_e0 = 0;
        uStack_c8 = 0;
        local_d0 = 0;
        local_c0 = 0;
      }
      else {
        if (local_90 != (long *)0x0) {
          *(undefined1 *)((long)local_90 + 0x61) = 2;
        }
        FUN_02544f38(param_2,auStack_2c0,&local_90);
      }
      if (local_a8 != (int *)0x0) {
        LOAcquire();
        iVar3 = *local_a8;
        *local_a8 = iVar3 + -1;
        LORelease();
        if (iVar3 == 1) {
          FUN_00005810(local_a8,0x90);
        }
      }
    }
    if (local_78 != (int *)0x0) {
      LOAcquire();
      iVar3 = *local_78;
      *local_78 = iVar3 + -1;
      LORelease();
      if (iVar3 == 1) {
        FUN_00005810(local_78,0x90);
      }
    }
    if (local_90 != (long *)0x0) {
      LOAcquire();
      iVar3 = (int)local_90[1];
      *(int *)(local_90 + 1) = iVar3 + -1;
      LORelease();
      if (iVar3 == 1) {
        (**(code **)(*local_90 + 0x10))();
      }
    }
    if (local_60 != (int *)0x0) {
      LOAcquire();
      iVar3 = *local_60;
      *local_60 = iVar3 + -1;
      LORelease();
      if (iVar3 == 1) {
        FUN_00005810(local_60,0x90);
      }
    }
  }
  FUN_04338b28(param_1,auStack_2c0);
  FUN_01c8dcf8(auStack_2c0);
  return;
}


