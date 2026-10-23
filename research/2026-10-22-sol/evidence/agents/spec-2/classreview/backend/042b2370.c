// ==== 0x42b2370  FUN_042b2370  (192 bytes)

undefined8 FUN_042b2370(long param_1)

{
  long lVar1;
  ulong uVar2;
  
  FUN_04325a04(*(undefined8 *)(param_1 + 200),0);
  uVar2 = *(ulong *)(param_1 + 0x428);
  if (uVar2 != 0) {
    *(undefined8 *)(param_1 + 0x428) = 0;
    FUN_01cdf600(uVar2 + 0x18,*(undefined8 *)(param_1 + 0x420));
    if ((uVar2 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4(uVar2);
    }
  }
  uVar2 = *(ulong *)(param_1 + 0x438);
  if (uVar2 != 0) {
    *(undefined8 *)(param_1 + 0x438) = 0;
    FUN_01fbf654(uVar2,*(undefined8 *)(param_1 + 0x430));
    if ((uVar2 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0001fee4(uVar2);
    }
  }
  lVar1 = *(long *)(param_1 + 0x310);
  *(undefined8 *)(param_1 + 0x310) = 0;
  if (lVar1 != 0) {
    FUN_04328224();
  }
  lVar1 = *(long *)(param_1 + 0x308);
  *(undefined8 *)(param_1 + 0x308) = 0;
  if (lVar1 != 0) {
    FUN_0436c31c();
  }
  FUN_042aae64(*(undefined8 *)(param_1 + 0x98));
  return 0;
}


