// ==== 0x42ae8ac  FUN_042ae8ac  (52 bytes)

long FUN_042ae8ac(long param_1)

{
  uint uVar1;
  code *pcVar2;
  
  if ((*(long *)(param_1 + 0x18) != 0) && (uVar1 = *(uint *)(param_1 + 0x50), -1 < (int)uVar1)) {
    if ((ulong)uVar1 < *(ulong *)(param_1 + 0x40)) {
      return *(long *)(param_1 + 0x38) + (ulong)uVar1 * 0x10;
    }
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x42ae8e0);
    (*pcVar2)();
  }
  return param_1 + 0x58;
}


