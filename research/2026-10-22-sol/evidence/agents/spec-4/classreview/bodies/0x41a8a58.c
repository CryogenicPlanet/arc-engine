// ==== 0x41a8a58  FUN_041a8a58  (192 bytes)

void FUN_041a8a58(long param_1,undefined4 param_2,undefined8 param_3)

{
  long lVar1;
  undefined8 *puVar2;
  undefined8 local_c0;
  undefined8 uStack_b8;
  undefined8 local_b0;
  long lStack_a8;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined8 local_50;
  
  local_50 = 0;
  uStack_68 = 0;
  local_70 = 0;
  uStack_58 = 0;
  uStack_60 = 0;
  uStack_88 = 0;
  local_90 = 0;
  uStack_78 = 0;
  uStack_80 = 0;
  lStack_a8 = 0;
  local_b0 = 0;
  uStack_98 = 0;
  uStack_a0 = 0;
  uStack_b8 = 0;
  local_c0 = 0;
  FUN_00115df8(&local_c0,0x1c652318,1,0);
  lVar1 = FUN_0002c16c((ulong)&local_c0 | 8,0x10);
  *(undefined8 *)(lStack_a8 + lVar1) = 0x10;
  *(undefined4 *)(lStack_a8 + lVar1 + 8) = param_2;
  puVar2 = (undefined8 *)FUN_00008790(0x10);
  *puVar2 = &PTR_FUN_0d9437c0;
  puVar2[1] = param_3;
  FUN_00c409f8(*(undefined8 *)(param_1 + 8),&local_c0,puVar2);
  FUN_0019ec98(&local_c0);
  return;
}


