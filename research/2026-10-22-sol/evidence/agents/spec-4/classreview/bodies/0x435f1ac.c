// ==== 0x435f1ac  FUN_0435f1ac  (120 bytes)

void FUN_0435f1ac(long param_1)

{
  code *pcVar1;
  int iVar2;
  int *piVar3;
  
  piVar3 = *(int **)(param_1 + 0x18);
  if (piVar3 != (int *)0x0) {
    iVar2 = *piVar3;
    *piVar3 = iVar2 + 1;
    if (iVar2 < 1) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x435f220);
      (*pcVar1)();
    }
    if (iVar2 == 0x7fffffff) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x435f22c);
      (*pcVar1)();
    }
    (**(code **)(*(long *)(param_1 + 0x18) + 8))(piVar3);
    LOAcquire();
    iVar2 = *piVar3;
    *piVar3 = iVar2 + -1;
    LORelease();
    if (iVar2 == 1) {
                    /* WARNING: Could not recover jumptable at 0x0435f218. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(piVar3 + 4))(piVar3);
      return;
    }
  }
  return;
}


