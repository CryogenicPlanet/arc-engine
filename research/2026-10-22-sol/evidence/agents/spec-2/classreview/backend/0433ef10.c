// ==== 0x433ef10  FUN_0433ef10  (128 bytes)

void FUN_0433ef10(long *param_1)

{
  long lVar1;
  undefined8 uVar2;
  long lVar3;
  undefined8 local_40;
  undefined8 uStack_38;
  ulong local_30;
  
  lVar1 = *param_1;
  lVar3 = *(long *)(lVar1 + 0x370);
  if (lVar3 != 0) {
    uVar2 = *(undefined8 *)(lVar1 + 0x368);
    if (*(char *)(lVar1 + 0x27) < '\0') {
      FUN_00060a60(&local_40,*(undefined8 *)(lVar1 + 0x10),*(undefined8 *)(lVar1 + 0x18));
    }
    else {
      uStack_38 = *(undefined8 *)(lVar1 + 0x18);
      local_40 = *(undefined8 *)(lVar1 + 0x10);
      local_30 = *(ulong *)(lVar1 + 0x20);
    }
    FUN_0432e02c(lVar3 + -8,uVar2);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
  }
  return;
}


