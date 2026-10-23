// ==== 0x4597a58  FUN_04597a58  (72 bytes)

undefined1  [16] FUN_04597a58(long param_1)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined1 auVar3 [16];
  
  if (*(int *)(param_1 + 0x28) != 2) {
    uVar2 = *(undefined8 *)(param_1 + 0x18);
    FUN_00882dd0(uVar2,*(undefined4 *)(param_1 + 0x20));
    uVar1 = FUN_0259be70();
    auVar3 = FUN_008831d4(uVar2,uVar1);
    return auVar3;
  }
  auVar3._0_8_ = *(ulong *)(param_1 + 0x30);
  auVar3._8_8_ = 0;
  return auVar3;
}


