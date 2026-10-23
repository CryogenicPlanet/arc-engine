// ==== 0x42df0b8  FUN_042df0b8  (308 bytes)

void FUN_042df0b8(long param_1,ulong param_2)

{
  long lVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  ulong uVar4;
  undefined1 local_31;
  
  if ((param_2 & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(param_2);
  }
  if ((*(ulong *)(param_1 + 8) & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0001fee4();
  }
  *(ulong *)(param_1 + 8) = param_2;
  if (param_2 == 0) {
    lVar1 = *(long *)(param_1 + 0x30);
    *(undefined8 *)(param_1 + 0x30) = 0;
    if (lVar1 != 0) {
      uVar2 = FUN_0275b6f0();
      FUN_00005810(uVar2,0x58);
    }
    uVar4 = *(ulong *)(param_1 + 0xb0);
    if (uVar4 != 0) {
      *(undefined8 *)(param_1 + 0xb0) = 0;
      FUN_02374d94(uVar4 + 0xe0,*(undefined8 *)(param_1 + 0xa8));
      if ((uVar4 & 0xfffffffc00000000) == DAT_0e2f2208) {
        FUN_0001fee4(uVar4);
        return;
      }
    }
    return;
  }
  lVar1 = *(long *)(param_2 + 0x18);
  local_31 = 0;
  FUN_000f71cc(&local_31,0,0x9f);
  uVar2 = FUN_02bf70d0(*(undefined8 *)(lVar1 + 0x18));
  uVar3 = FUN_00008790(0x58);
  FUN_02831118(uVar3,uVar2,param_1);
  lVar1 = *(long *)(param_1 + 0x30);
  *(undefined8 *)(param_1 + 0x30) = uVar3;
  if (lVar1 != 0) {
    uVar3 = FUN_0275b6f0();
    FUN_00005810(uVar3,0x58);
  }
  FUN_080fe1d8(param_1 + 0xa8,uVar2);
  return;
}


