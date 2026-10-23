// ==== 0x43ece08  ArcBrowserContextAutocomplete::stop  (96 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserContextAutocomplete::stop(ID param_1,SEL param_2)

{
  code *UNRECOVERED_JUMPTABLE;
  long lVar1;
  long lVar2;
  
  lVar2 = **(long **)(param_1 + 8);
  if ((lVar2 != 0) && (*(long *)(lVar2 + 0x70) != 0)) {
    FUN_024e39c0(*(undefined8 *)(lVar2 + 0x30),0);
    lVar1 = *(long *)(lVar2 + 0x50);
    *(undefined8 *)(lVar2 + 0x48) = 0;
    *(undefined8 **)(lVar2 + 0x50) = &DAT_0c4b3500;
    UNRECOVERED_JUMPTABLE = *(code **)(lVar1 + 8);
    if (UNRECOVERED_JUMPTABLE != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x043ece58. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      (*UNRECOVERED_JUMPTABLE)(*(undefined8 *)(lVar2 + 0x38));
      return;
    }
  }
  return;
}


