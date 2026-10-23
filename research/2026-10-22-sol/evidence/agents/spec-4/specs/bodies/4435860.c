// ==== 0x4435860  ArcBrowserContextPreferences::creditCardsSavingEnabled  (68 bytes)

/* Function Stack Size: 0x10 bytes */

bool ArcBrowserContextPreferences::creditCardsSavingEnabled(ID param_1,SEL param_2)

{
  code *pcVar1;
  bool bVar2;
  long *plVar3;
  undefined8 uVar4;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x44358a4);
    (*pcVar1)();
  }
  plVar3 = *(long **)(*(long *)(param_1 + 0x10) + 0x70);
  if (plVar3 != (long *)0x0) {
    uVar4 = (**(code **)(*plVar3 + 0x168))();
    bVar2 = FUN_003bc2f4(uVar4,"autofill.credit_card_enabled",0x1c);
    return bVar2;
  }
  return 0;
}


