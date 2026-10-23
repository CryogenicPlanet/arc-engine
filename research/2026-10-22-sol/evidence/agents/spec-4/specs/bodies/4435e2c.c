// ==== 0x4435e2c  ArcBrowserContextPageContentExtraction::.cxx_destruct  (204 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserContextPageContentExtraction::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  SEL extraout_x1;
  SEL extraout_x1_00;
  SEL extraout_x1_01;
  code *pcVar2;
  long *plVar3;
  
  if ((*(char *)(param_1 + 0x20) != '\0') &&
     (plVar3 = *(long **)(param_1 + 0x18), plVar3 != (long *)0x0)) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        param_2 = extraout_x1;
        if (lVar1 != 0) goto LAB_04435e60;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
        param_2 = extraout_x1_00;
      }
      else {
        (*pcVar2)(plVar3);
        param_2 = extraout_x1_01;
      }
    }
  }
LAB_04435e60:
  _objc_destroyWeak(param_1 + 8,param_2);
  return;
}


