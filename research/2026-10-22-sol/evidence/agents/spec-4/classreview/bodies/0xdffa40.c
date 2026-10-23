// ==== 0xdffa40  FUN_00dffa40  (120 bytes)

void FUN_00dffa40(long *param_1)

{
  code *pcVar1;
  int iVar2;
  int *piVar3;
  
  piVar3 = (int *)*param_1;
  if (piVar3 == (int *)0x0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0xdffab0);
    (*pcVar1)();
  }
  iVar2 = *piVar3;
  *piVar3 = iVar2 + 1;
  if (iVar2 < 1) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0xdffabc);
    (*pcVar1)();
  }
  if (iVar2 == 0x7fffffff) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0xdffac8);
    (*pcVar1)();
  }
  (**(code **)(*param_1 + 8))(piVar3);
  LOAcquire();
  iVar2 = *piVar3;
  *piVar3 = iVar2 + -1;
  LORelease();
  if (iVar2 != 1) {
    return;
  }
  FUN_00dffad0(piVar3);
  return;
}


