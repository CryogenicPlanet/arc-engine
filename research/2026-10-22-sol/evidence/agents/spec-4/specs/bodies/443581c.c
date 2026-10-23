// ==== 0x443581c  ArcBrowserContextPreferences::canEnableCreditCardsSaving  (68 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcBrowserContextPreferences::canEnableCreditCardsSaving(ID param_1,SEL param_2)

{
  code *pcVar1;
  bool bVar2;
  long *plVar3;
  undefined8 uVar4;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4435860);
    (*pcVar1)();
  }
  plVar3 = *(long **)(*(long *)(param_1 + 0x10) + 0x70);
  if (plVar3 != (long *)0x0) {
    uVar4 = (**(code **)(*plVar3 + 0x168))();
    bVar2 = FUN_08bcd32c(uVar4,"autofill.credit_card_enabled",0x1c);
    return bVar2;
  }
  return 0;
}


