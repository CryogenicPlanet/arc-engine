// ==== 0x41a7920  FUN_041a7920  (268 bytes)

void FUN_041a7920(long param_1,undefined8 *param_2)

{
  undefined4 *puVar1;
  undefined8 *puVar2;
  long lVar3;
  long lVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  long lStack_98;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 local_40;
  
  puVar1 = (undefined4 *)FUN_00008790(0x48);
  uVar5 = *param_2;
  uVar7 = param_2[3];
  uVar6 = param_2[2];
  *(undefined8 *)(puVar1 + 0xc) = param_2[1];
  *(undefined8 *)(puVar1 + 10) = uVar5;
  *puVar1 = 1;
  *(undefined1 **)(puVar1 + 2) = &LAB_041a82c0;
  *(code **)(puVar1 + 4) = FUN_041a82e0;
  *(code **)(puVar1 + 6) = FUN_00b77080;
  lVar3 = param_2[3];
  *(undefined8 *)(puVar1 + 0x10) = uVar7;
  *(undefined8 *)(puVar1 + 0xe) = uVar6;
  if (*(long *)(lVar3 + 8) != 0) {
    param_2[2] = 0;
    param_2[3] = &DAT_0c4b3500;
  }
  FUN_06f88120(param_1 + 0x118);
  lVar4 = *(long *)(param_1 + 0x140);
  local_40 = 0;
  uStack_58 = 0;
  local_60 = 0;
  uStack_48 = 0;
  uStack_50 = 0;
  uStack_78 = 0;
  local_80 = 0;
  uStack_68 = 0;
  uStack_70 = 0;
  lStack_98 = 0;
  local_a0 = 0;
  uStack_88 = 0;
  uStack_90 = 0;
  uStack_a8 = 0;
  local_b0 = 0;
  FUN_00115df8(&local_b0,0x23b754f8,1,0);
  lVar3 = FUN_0002c16c((ulong)&local_b0 | 8,8);
  *(undefined8 *)(lStack_98 + lVar3) = 8;
  puVar2 = (undefined8 *)FUN_00008790(0x10);
  *puVar2 = &PTR_FUN_0d943700;
  puVar2[1] = puVar1;
  FUN_00c409f8(*(undefined8 *)(lVar4 + 8),&local_b0,puVar2);
  FUN_0019ec98(&local_b0);
  return;
}


