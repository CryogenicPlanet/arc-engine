// ==== 0x435f150  FUN_0435f150  (76 bytes)

void FUN_0435f150(long param_1)

{
  code *pcVar1;
  undefined4 *puVar2;
  long lVar3;
  
  lVar3 = *(long *)(*(long *)(param_1 + 0x10) + 0xf8);
  if (lVar3 == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x435f198);
    (*pcVar1)();
  }
  lVar3 = *(long *)(lVar3 + 0x40);
  if (lVar3 != 0) {
    puVar2 = (undefined4 *)FUN_013b3f18(lVar3);
    FUN_083727c4(*puVar2);
    FUN_0669e6e4(lVar3,2);
    return;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0,0x435f1a4);
  (*pcVar1)();
}


