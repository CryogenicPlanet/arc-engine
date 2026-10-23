// ==== 0x443b4a8  ArcConfirmInfobar::.cxx_destruct  (208 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcConfirmInfobar::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  code *UNRECOVERED_JUMPTABLE;
  long *plVar2;
  
  if (*(char *)(param_1 + 0x20) != '\0') {
    *(undefined ***)(param_1 + 8) = &PTR_FUN_0da7a7d8;
    plVar2 = *(long **)(param_1 + 0x18);
    if (plVar2 != (long *)0x0) {
      LOAcquire();
      lVar1 = plVar2[1];
      plVar2[1] = lVar1 + -1;
      LORelease();
      if (lVar1 == 0) {
        (**(code **)(*plVar2 + 0x10))(plVar2);
        if (plVar2[2] != 0) {
          LOAcquire();
          lVar1 = plVar2[2];
          plVar2[2] = lVar1 + -1;
          LORelease();
          if (lVar1 != 0) {
            return;
          }
        }
        UNRECOVERED_JUMPTABLE = *(code **)(*plVar2 + 0x20);
        if (((UNRECOVERED_JUMPTABLE != FUN_00f01bc0) && (UNRECOVERED_JUMPTABLE != FUN_00f01bc0)) &&
           (UNRECOVERED_JUMPTABLE != FUN_013efb40)) {
                    /* WARNING: Could not recover jumptable at 0x0443b574. Too many branches */
                    /* WARNING: Treating indirect jump as call */
          (*UNRECOVERED_JUMPTABLE)(plVar2);
          return;
        }
        FUN_00005810(plVar2,0x20);
        return;
      }
    }
  }
  return;
}


