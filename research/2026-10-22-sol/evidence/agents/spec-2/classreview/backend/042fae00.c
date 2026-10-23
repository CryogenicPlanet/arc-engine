// ==== 0x42fae00  FUN_042fae00  (196 bytes)

undefined1 FUN_042fae00(long param_1,int *param_2)

{
  int iVar1;
  code *pcVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  long lVar5;
  long lVar6;
  
  if ((((*(long *)(param_1 + 0xf0) != 0) && (*(char *)(*(long *)(param_1 + 0xf0) + 4) == '\0')) &&
      (*(long *)(param_1 + 0xf8) != 0)) && (*(char *)(param_1 + 600) != '\x01')) {
    if ((*(long *)(param_1 + 0xf0) == 0) || (*(char *)(*(long *)(param_1 + 0xf0) + 4) != '\0')) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(0,0x42faec4);
      (*pcVar2)();
    }
    uVar3 = (**(code **)(**(long **)(param_1 + 0xf8) + 0x38))();
    uVar4 = FUN_02302728();
    lVar5 = FUN_0009278c(uVar4,uVar3,1);
    if (lVar5 == 0) {
      return 0;
    }
    iVar1 = *param_2;
    if (0 < iVar1) {
      lVar6 = lVar5 + 0x38;
      do {
        lVar6 = *(long *)(lVar6 + 8);
        if (lVar6 == lVar5 + 0x38) {
          return 0;
        }
      } while (*(int *)(*(long *)(lVar6 + 0x10) + 8) != iVar1 &&
               *(int *)(*(long *)(lVar6 + 0x10) + 0xc) != iVar1);
      return 1;
    }
  }
  return 0;
}


