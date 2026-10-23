// ==== 0x4330cdc  FUN_04330cdc  (92 bytes)

void FUN_04330cdc(undefined8 *param_1)

{
  long lVar1;
  long *plVar2;
  
  plVar2 = (long *)*param_1;
  lVar1 = *plVar2;
  if ((lVar1 + 0x30U & 0xfffffffc00000000) == DAT_0e2f2208) {
    FUN_0000ffa8(lVar1 + 0x30);
  }
  FUN_07c6a394(lVar1,2,lVar1 + 0x30U);
  FUN_07c6a4f4(*plVar2);
  return;
}


