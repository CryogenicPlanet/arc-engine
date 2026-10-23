// ==== 0x7afc3a8  FUN_07afc3a8  (36 bytes)

void FUN_07afc3a8(long param_1,uint param_2)

{
  code *pcVar1;
  long lVar2;
  
  lVar2 = *(long *)(param_1 + 0x18);
  if (lVar2 == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x7afc3cc);
    (*pcVar1)();
  }
  if (*(byte *)(lVar2 + 0x29) == param_2) {
    return;
  }
  *(char *)(lVar2 + 0x29) = (char)param_2;
  FUN_04208b34();
  return;
}


