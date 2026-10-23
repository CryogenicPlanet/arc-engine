// ==== 0x440a40c  ArcMenuModel::.cxx_destruct  (208 bytes)

/* WARNING: Function: _objc_storeStrong replaced with injection: _objc_store_fixup */
/* Function Stack Size: 0x10 bytes */

void ArcMenuModel::_cxx_destruct(ID param_1,SEL param_2)

{
  long lVar1;
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
        if (lVar1 != 0) goto LAB_0440a440;
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
LAB_0440a440:
  *(undefined8 *)(param_1 + 8) = 0;
  return;
}


