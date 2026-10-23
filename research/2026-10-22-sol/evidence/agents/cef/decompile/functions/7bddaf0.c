// ==== 0x7bddaf0  FUN_07bddaf0  (180 bytes)

void FUN_07bddaf0(long param_1)

{
  int iVar1;
  int *piVar2;
  
  if (param_1 != 0) {
    piVar2 = *(int **)(param_1 + 0x60);
    if (piVar2 != (int *)0x0) {
      LOAcquire();
      iVar1 = *piVar2;
      *piVar2 = iVar1 + -1;
      LORelease();
      if (iVar1 == 1) {
        (**(code **)(piVar2 + 4))();
      }
    }
    piVar2 = *(int **)(param_1 + 0x58);
    if (piVar2 != (int *)0x0) {
      LOAcquire();
      iVar1 = *piVar2;
      *piVar2 = iVar1 + -1;
      LORelease();
      if (iVar1 == 1) {
        (**(code **)(piVar2 + 4))();
      }
    }
    if (*(char *)(param_1 + 0x57) < '\0') {
      FUN_00005810(*(undefined8 *)(param_1 + 0x40),*(ulong *)(param_1 + 0x50) & 0x7fffffffffffffff);
    }
    piVar2 = *(int **)(param_1 + 0x30);
    if (piVar2 != (int *)0x0) {
      LOAcquire();
      iVar1 = *piVar2;
      *piVar2 = iVar1 + -1;
      LORelease();
      if (iVar1 == 1) {
        FUN_00005810(piVar2,8);
      }
    }
    FUN_00005810(param_1,0x68);
    return;
  }
  return;
}


