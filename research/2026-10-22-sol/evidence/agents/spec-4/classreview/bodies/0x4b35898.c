// ==== 0x4b35898  FUN_04b35898  (136 bytes)

void FUN_04b35898(int *param_1)

{
  int iVar1;
  
  if (*param_1 == 1) {
    if (param_1 == (int *)0x0) {
      return;
    }
  }
  else {
    LOAcquire();
    iVar1 = *param_1;
    *param_1 = iVar1 + -1;
    LORelease();
    if (iVar1 != 1 || param_1 == (int *)0x0) {
      return;
    }
  }
  FUN_04b36288(param_1 + 0xe);
  if ((param_1[8] & 1U) != 0) {
    FUN_00005810(*(undefined8 *)(param_1 + 10),*(long *)(param_1 + 0xc) << 4);
  }
  if (*(char *)((long)param_1 + 0x1f) < '\0') {
    FUN_00005810(*(undefined8 *)(param_1 + 2),*(ulong *)(param_1 + 6) & 0x7fffffffffffffff);
  }
  FUN_00005810(param_1,0x40);
  return;
}


