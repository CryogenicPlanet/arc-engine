// ==== 0x7c6a394  FUN_07c6a394  (164 bytes)

void FUN_07c6a394(long param_1,undefined8 param_2,ulong param_3)

{
  code *pcVar1;
  int iVar2;
  ulong uVar3;
  long lVar4;
  
  iVar2 = FUN_01460670(param_1 + 0x20);
  if ((iVar2 != 0) && (*(long *)(param_1 + 0x28) != 0)) {
    uVar3 = FUN_01460670(param_1 + 0x20);
    if ((uVar3 & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x7c6a438);
      (*pcVar1)();
    }
    lVar4 = *(long *)(param_1 + 0x28);
    if ((param_3 & 0xfffffffc00000000) == DAT_0e2f2208) {
      FUN_0000ffa8(param_3);
    }
    FUN_04215b20(lVar4 + -0x50,param_2,param_3);
  }
  if ((param_3 & 0xfffffffc00000000) != DAT_0e2f2208) {
    return;
  }
  FUN_0001fee4(param_3);
  return;
}


