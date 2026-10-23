// ==== 0x42bf5ac  FUN_042bf5ac  (124 bytes)

void FUN_042bf5ac(long param_1)

{
  int iVar1;
  int *piVar2;
  code *pcVar3;
  
  if (param_1 != 0) {
    pcVar3 = *(code **)(*(long *)(param_1 + 0x78) + 8);
    if (pcVar3 != (code *)0x0) {
      (*pcVar3)(*(undefined8 *)(param_1 + 0x60));
    }
    if (*(char *)(param_1 + 0x5f) < '\0') {
      FUN_00005810(*(undefined8 *)(param_1 + 0x48),*(ulong *)(param_1 + 0x58) & 0x7fffffffffffffff);
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
    FUN_00005810(param_1,0x80);
    return;
  }
  return;
}


