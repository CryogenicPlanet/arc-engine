// ==== 0x4438580  ArcBrowserContext::.cxx_destruct  (652 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcBrowserContext::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
  SEL extraout_x1;
  SEL extraout_x1_00;
  SEL extraout_x1_01;
  code *pcVar2;
  long lVar3;
  long *plVar4;
  long lVar5;
  long lVar6;
  
  lVar5 = *(long *)(param_1 + 0x48);
  if (lVar5 != 0) {
    lVar3 = *(long *)(param_1 + 0x50) * 0x10;
    lVar6 = lVar5;
    if (lVar3 != 0) {
      lVar6 = *(long *)(param_1 + 0x50) * -0x10;
      lVar3 = lVar3 + lVar5;
      do {
        if (lVar3 + -0x10 == 0) {
                    /* WARNING: Does not return */
          pcVar2 = (code *)SoftwareBreakpoint(1,0x44387e0);
          (*pcVar2)();
        }
        plVar4 = *(long **)(lVar3 + -8);
        if (plVar4 != (long *)0x0) {
          LOAcquire();
          lVar1 = plVar4[1];
          plVar4[1] = lVar1 + -1;
          LORelease();
          if (lVar1 == 0) {
            (**(code **)(*plVar4 + 0x10))(plVar4,param_2);
            if (plVar4[2] != 0) {
              LOAcquire();
              lVar1 = plVar4[2];
              plVar4[2] = lVar1 + -1;
              LORelease();
              param_2 = extraout_x1;
              if (lVar1 != 0) goto LAB_044385ec;
            }
            pcVar2 = *(code **)(*plVar4 + 0x20);
            if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40))
            {
              FUN_00005810(plVar4,0x20);
              param_2 = extraout_x1_00;
            }
            else {
              (*pcVar2)(plVar4);
              param_2 = extraout_x1_01;
            }
          }
        }
LAB_044385ec:
        lVar6 = lVar6 + 0x10;
        lVar3 = lVar3 + -0x10;
      } while (lVar6 != 0);
      lVar6 = *(long *)(param_1 + 0x48);
    }
    *(long *)(param_1 + 0x50) = lVar5 - lVar6 >> 4;
    FUN_00005810(lVar6,*(long *)(param_1 + 0x58) << 4);
  }
  if ((*(char *)(param_1 + 0x40) != '\0') &&
     (plVar4 = *(long **)(param_1 + 0x38), plVar4 != (long *)0x0)) {
    LOAcquire();
    lVar5 = plVar4[1];
    plVar4[1] = lVar5 + -1;
    LORelease();
    if (lVar5 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar5 = plVar4[2];
        plVar4[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_044386ac;
      }
      pcVar2 = *(code **)(*plVar4 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar2)(plVar4);
      }
    }
  }
LAB_044386ac:
  plVar4 = *(long **)(param_1 + 0x28);
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar5 = plVar4[1];
    plVar4[1] = lVar5 + -1;
    LORelease();
    if (lVar5 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar5 = plVar4[2];
        plVar4[2] = lVar5 + -1;
        LORelease();
        if (lVar5 != 0) goto LAB_044386c4;
      }
      pcVar2 = *(code **)(*plVar4 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar2)(plVar4);
      }
    }
  }
LAB_044386c4:
  *(undefined8 *)(param_1 + 0x18) = 0;
  *(undefined8 *)(param_1 + 0x10) = 0;
  *(undefined8 *)(param_1 + 8) = 0;
  return;
}


