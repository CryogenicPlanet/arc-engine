// ==== 0x443ee68  ArcCreditCardInfoSaveRequest::cancel  (148 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcCreditCardInfoSaveRequest::cancel(ID param_1,SEL param_2)

{
  code *pcVar1;
  long lVar2;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x443eefc);
    (*pcVar1)();
  }
  lVar2 = *(long *)(param_1 + 8);
  if ((((*(long *)(lVar2 + 0x20) != 0) && (*(char *)(*(long *)(lVar2 + 0x20) + 4) == '\0')) &&
      (*(long *)(lVar2 + 0x28) != 0)) &&
     (((*(long *)(lVar2 + 0x20) != 0 && (*(char *)(*(long *)(lVar2 + 0x20) + 4) == '\0')) &&
      (lVar2 = *(long *)(lVar2 + 0x28), lVar2 != 0)))) {
    FUN_07c72f90(lVar2);
    lVar2 = FUN_000805e0(lVar2 + 8,&DAT_0c9c03dc);
    if (lVar2 != 0 && lVar2 + -0x48 != 0) {
      FUN_07c7dca0(lVar2 + -0x48,2);
      return;
    }
  }
  return;
}


