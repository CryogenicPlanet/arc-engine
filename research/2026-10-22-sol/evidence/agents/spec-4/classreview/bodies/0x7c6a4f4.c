// ==== 0x7c6a4f4  FUN_07c6a4f4  (108 bytes)

void FUN_07c6a4f4(long param_1)

{
  code *pcVar1;
  int iVar2;
  ulong uVar3;
  long lVar4;
  
  iVar2 = FUN_01460670(param_1 + 0x20);
  if ((iVar2 != 0) && (*(long *)(param_1 + 0x28) != 0)) {
    uVar3 = FUN_01460670(param_1 + 0x20);
    if ((uVar3 & 1) != 0) {
      lVar4 = *(long *)(param_1 + 0x28);
      FUN_07c5a310(lVar4 + -0x50);
      *(undefined1 *)(lVar4 + 0x12) = 0;
                    /* WARNING: Could not recover jumptable at 0x07c6a558. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(*(long *)(lVar4 + -0x50) + 0x80))(lVar4 + -0x50);
      return;
    }
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x7c6a560);
    (*pcVar1)();
  }
  return;
}


