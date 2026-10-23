// ==== 0x42ae8e0  FUN_042ae8e0  (56 bytes)

long FUN_042ae8e0(long param_1)

{
  int iVar1;
  
  if (((*(long *)(param_1 + 0x18) != 0) && (-1 < *(int *)(param_1 + 0x50))) &&
     (iVar1 = *(int *)(param_1 + 0x50) + 1, (ulong)(long)iVar1 < *(ulong *)(param_1 + 0x40))) {
    return *(long *)(param_1 + 0x38) + (long)iVar1 * 0x10;
  }
  return param_1 + 0x58;
}


