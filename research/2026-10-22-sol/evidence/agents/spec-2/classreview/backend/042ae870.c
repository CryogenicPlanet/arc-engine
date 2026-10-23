// ==== 0x42ae870  FUN_042ae870  (60 bytes)

long FUN_042ae870(long param_1)

{
  code *pcVar1;
  ulong uVar2;
  
  if ((*(long *)(param_1 + 0x18) != 0) && (0 < *(int *)(param_1 + 0x50))) {
    uVar2 = (ulong)(*(int *)(param_1 + 0x50) - 1);
    if (uVar2 < *(ulong *)(param_1 + 0x40)) {
      return *(long *)(param_1 + 0x38) + uVar2 * 0x10;
    }
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x42ae8ac);
    (*pcVar1)();
  }
  return param_1 + 0x58;
}


