// ==== 0x7d641d0  FUN_07d641d0  (120 bytes)

void FUN_07d641d0(long param_1)

{
  code *pcVar1;
  int iVar2;
  ulong uVar3;
  
  if (*(int *)(param_1 + 0x78) != 7 && *(int *)(param_1 + 0x78) != 0xd) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x7d64244);
    (*pcVar1)();
  }
  *(undefined4 *)(param_1 + 0x8e4) = 2;
  iVar2 = FUN_01460670(param_1 + 0x10);
  if ((iVar2 != 0) && (*(long *)(param_1 + 0x18) != 0)) {
    uVar3 = FUN_01460670(param_1 + 0x10);
    if ((uVar3 & 1) != 0) {
                    /* WARNING: Could not recover jumptable at 0x07d6423c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (**(code **)(**(long **)(*(long *)(param_1 + 0x18) + 0x70) + 0x90))();
      return;
    }
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x7d64250);
    (*pcVar1)();
  }
  return;
}


