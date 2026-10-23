// ==== 0x42fc76c  FUN_042fc76c  (172 bytes)

void FUN_042fc76c(long param_1,undefined8 param_2,undefined8 param_3,uint param_4)

{
  code *pcVar1;
  long lVar2;
  undefined1 uVar3;
  
  if (((*(long *)(param_1 + 0xf0) != 0) && (*(char *)(*(long *)(param_1 + 0xf0) + 4) == '\0')) &&
     (*(long *)(param_1 + 0xf8) != 0)) {
    if ((*(long *)(param_1 + 0xf0) == 0) || (*(char *)(*(long *)(param_1 + 0xf0) + 4) != '\0')) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x42fc818);
      (*pcVar1)();
    }
    lVar2 = (**(code **)(**(long **)(param_1 + 0xf8) + 0x78))();
    if ((*(long *)(lVar2 + 0x90) != 0) &&
       (lVar2 = *(long *)(*(long *)(lVar2 + 0x90) + 0x40), lVar2 != 0)) {
      if (param_4 < 0xb) {
        uVar3 = (&DAT_0c9d6830)[param_4];
      }
      else {
        uVar3 = 1;
      }
      FUN_07c91d4c(lVar2,param_2,param_3,uVar3);
      return;
    }
  }
  return;
}


