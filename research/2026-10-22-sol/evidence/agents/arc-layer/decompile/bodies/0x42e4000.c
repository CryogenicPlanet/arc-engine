// ==== 0x42e4000  FUN_042e4000  (176 bytes)

void FUN_042e4000(long param_1,ulong param_2)

{
  ulong uVar1;
  
  uVar1 = *(ulong *)(param_1 + 0x30);
  if (uVar1 != 0) {
    *(undefined8 *)(param_1 + 0x30) = 0;
    FUN_01eb7c9c(uVar1 + 0x88,*(undefined8 *)(param_1 + 0x28));
    if ((uVar1 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4(uVar1);
    }
  }
  if (param_2 != 0) {
    if ((param_2 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(param_2);
    }
    if ((*(ulong *)(param_1 + 0x30) & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4();
    }
    *(ulong *)(param_1 + 0x30) = param_2;
    FUN_00575e00(param_2 + 0x88,*(undefined8 *)(param_1 + 0x28));
    return;
  }
  return;
}


