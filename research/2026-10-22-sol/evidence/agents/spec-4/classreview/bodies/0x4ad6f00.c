// ==== 0x4ad6f00  FUN_04ad6f00  (120 bytes)

undefined8 FUN_04ad6f00(long *param_1)

{
  code *pcVar1;
  int iVar2;
  undefined8 uVar3;
  int *piVar4;
  
  piVar4 = (int *)*param_1;
  if (piVar4 == (int *)0x0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x4ad6f70);
    (*pcVar1)();
  }
  iVar2 = *piVar4;
  *piVar4 = iVar2 + 1;
  if (0 < iVar2) {
    if (iVar2 != 0x7fffffff) {
      uVar3 = (**(code **)(*param_1 + 8))(piVar4);
      LOAcquire();
      iVar2 = *piVar4;
      *piVar4 = iVar2 + -1;
      LORelease();
      if (iVar2 == 1) {
        FUN_00dffad0(piVar4);
      }
      return uVar3;
    }
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x4ad6f88);
    (*pcVar1)();
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0,0x4ad6f7c);
  (*pcVar1)();
}


