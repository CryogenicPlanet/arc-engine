// ==== 0x433e440  FUN_0433e440  (256 bytes)

void FUN_0433e440(undefined8 *param_1,long param_2)

{
  undefined1 **ppuVar1;
  long lVar2;
  ulong uVar3;
  undefined8 *puVar4;
  ulong uVar5;
  undefined8 uVar6;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  uVar5 = (ulong)*(char *)(param_2 + 0x247);
  uVar3 = uVar5;
  if ((long)uVar5 < 0) {
    uVar3 = *(ulong *)(param_2 + 0x238);
  }
  if ((uVar3 == 0) && (*(char *)(param_2 + 0xb0) != '\0')) {
    uVar3 = FUN_00d1f8d4(param_2 + 0x98);
    if ((uVar3 & 1) != 0) {
      puVar4 = (undefined8 *)FUN_007cf864(param_2 + 0x98);
      if (*(char *)((long)puVar4 + 0x17) < '\0') {
        FUN_00060a60(&local_40,*puVar4,puVar4[1]);
      }
      else {
        lStack_38 = puVar4[1];
        local_40 = (undefined1 *)*puVar4;
        local_30 = puVar4[2];
      }
      ppuVar1 = (undefined1 **)local_40;
      if (-1 < (long)local_30._7_1_) {
        ppuVar1 = &local_40;
      }
      lVar2 = lStack_38;
      if (-1 < (long)local_30) {
        lVar2 = (long)local_30._7_1_;
      }
      FUN_007cf8e0(param_1,ppuVar1,lVar2);
      if (-1 < (long)local_30) {
        return;
      }
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
      return;
    }
    uVar5 = (ulong)*(byte *)(param_2 + 0x247);
  }
  if (((uint)uVar5 >> 7 & 1) == 0) {
    uVar6 = *(undefined8 *)(param_2 + 0x230);
    param_1[1] = *(undefined8 *)(param_2 + 0x238);
    *param_1 = uVar6;
    param_1[2] = *(undefined8 *)(param_2 + 0x240);
    return;
  }
  FUN_00054420(param_1,*(undefined8 *)(param_2 + 0x230),*(undefined8 *)(param_2 + 0x238));
  return;
}


