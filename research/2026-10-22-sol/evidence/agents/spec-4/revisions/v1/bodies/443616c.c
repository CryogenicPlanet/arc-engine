// ==== 0x443616c  ArcBrowserContextHistory::.cxx_destruct  (516 bytes)

/* Function Stack Size: 0x10 bytes */

void ArcBrowserContextHistory::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  SEL extraout_x1;
  SEL extraout_x1_00;
  SEL extraout_x1_01;
  code *pcVar2;
  long *plVar3;
  
  if ((*(char *)(param_1 + 0x40) != '\0') &&
     (plVar3 = *(long **)(param_1 + 0x38), plVar3 != (long *)0x0)) {
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
        if (lVar1 != 0) goto LAB_044361a0;
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
LAB_044361a0:
  plVar3 = *(long **)(param_1 + 0x28);
  if (plVar3 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar3[1];
    plVar3[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3,param_2);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar1 = plVar3[2];
        plVar3[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_044361b8;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar2)(plVar3);
      }
    }
  }
LAB_044361b8:
  plVar3 = *(long **)(param_1 + 0x18);
  if (plVar3 != (long *)0x0) {
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
        if (lVar1 != 0) goto LAB_044361d0;
      }
      pcVar2 = *(code **)(*plVar3 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar2)(plVar3);
      }
    }
  }
LAB_044361d0:
  _objc_destroyWeak(param_1 + 8);
  return;
}


