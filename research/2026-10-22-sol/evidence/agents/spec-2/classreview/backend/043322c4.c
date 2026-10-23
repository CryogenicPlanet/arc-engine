// ==== 0x43322c4  FUN_043322c4  (176 bytes)

void FUN_043322c4(undefined8 *param_1)

{
  ulong uVar1;
  long lVar2;
  undefined8 local_40;
  ulong uStack_38;
  ulong local_30;
  
  lVar2 = FUN_02682c64();
  local_40 = 0;
  uStack_38 = 0;
  local_30 = 0;
  FUN_02af463c();
  if (*(char *)(lVar2 + 0x37) < '\0') {
    FUN_00060a60(&local_40,*(undefined8 *)(lVar2 + 0x20),*(undefined8 *)(lVar2 + 0x28));
  }
  else {
    uStack_38 = *(ulong *)(lVar2 + 0x28);
    local_40 = *(undefined8 *)(lVar2 + 0x20);
    local_30 = *(ulong *)(lVar2 + 0x30);
  }
  uVar1 = uStack_38;
  if (-1 < (long)local_30) {
    uVar1 = local_30 >> 0x38;
  }
  if (uVar1 == 0) {
    *(undefined1 *)param_1 = 0;
    *(undefined1 *)(param_1 + 3) = 0;
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
  }
  else {
    param_1[1] = uStack_38;
    *param_1 = local_40;
    param_1[2] = local_30;
    *(undefined1 *)(param_1 + 3) = 1;
  }
  return;
}


