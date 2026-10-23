// ==== 0x42bdc48  FUN_042bdc48  (204 bytes)

void FUN_042bdc48(undefined8 *param_1,undefined8 param_2,undefined8 param_3)

{
  int iVar1;
  undefined1 auStack_d0 [40];
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined8 uStack_70;
  int *local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  ulong uStack_40;
  undefined8 local_38;
  undefined8 local_30;
  undefined8 uStack_28;
  
  local_38 = 0;
  uStack_40 = 0;
  local_48 = 0;
  uStack_50 = 0;
  local_58 = 0;
  uStack_60 = 0;
  local_68 = (int *)0x0;
  uStack_70 = 0;
  local_78 = 0;
  uStack_80 = 0;
  local_88 = 0;
  local_30 = 0;
  uStack_28 = 0;
  FUN_02526c2c(auStack_d0,0xffffffffffffffff,&local_30);
  iVar1 = FUN_08b8891c(DAT_0e4abb58,param_3,auStack_d0);
  if (iVar1 != 0) {
    param_1[1] = uStack_a0;
    *param_1 = local_a8;
  }
  else {
    *(undefined1 *)param_1 = 0;
  }
  *(bool *)(param_1 + 2) = iVar1 != 0;
  if ((long)uStack_40 < 0) {
    FUN_00005810(uStack_50,uStack_40 & 0x7fffffffffffffff);
  }
  if (local_68 != (int *)0x0) {
    LOAcquire();
    iVar1 = *local_68;
    *local_68 = iVar1 + -1;
    LORelease();
    if (iVar1 == 1) {
      FUN_00005810(local_68,500);
    }
  }
  return;
}


