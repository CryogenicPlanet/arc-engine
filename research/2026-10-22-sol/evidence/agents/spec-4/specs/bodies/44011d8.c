// ==== 0x44011d8  ArcBrowserSidePanel::setDelegate:  (536 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_storeWeak replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserSidePanel::setDelegate_(ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  undefined8 *puVar2;
  code *pcVar3;
  long *plVar4;
  undefined8 *puVar5;
  
  if (param_3 == 0) {
    plVar4 = *(long **)(param_1 + 0x20);
    *(undefined8 *)(param_1 + 0x18) = 0;
    *(undefined8 *)(param_1 + 0x20) = 0;
    if (plVar4 == (long *)0x0) {
      return;
    }
    LOAcquire();
    lVar1 = plVar4[1];
    plVar4[1] = lVar1 + -1;
    LORelease();
    if (lVar1 != 0) {
      return;
    }
    (**(code **)(*plVar4 + 0x10))(plVar4);
    if (plVar4[2] != 0) {
      LOAcquire();
      lVar1 = plVar4[2];
      plVar4[2] = lVar1 + -1;
      LORelease();
      if (lVar1 != 0) {
        return;
      }
    }
    pcVar3 = *(code **)(*plVar4 + 0x20);
    if (((pcVar3 != FUN_00f01bc0) && (pcVar3 != FUN_00f01bc0)) && (pcVar3 != FUN_013efb40)) {
      (*pcVar3)(plVar4);
      return;
    }
    FUN_00005810(plVar4,0x20);
    return;
  }
  if (*(long *)(param_1 + 0x18) != 0) {
    *(ID *)(*(long *)(param_1 + 0x18) + 8) = param_3;
    return;
  }
  puVar2 = (undefined8 *)FUN_00008790(0x30);
  puVar2[1] = 0;
  puVar2[2] = 0;
  *puVar2 = &PTR_viewWillStartLiveResize_0da87dc0;
  puVar5 = puVar2 + 3;
  if (puVar5 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x44013f0);
    (*pcVar3)();
  }
  puVar2[3] = &PTR_FUN_0da87df8;
  _objc_initWeak(puVar2 + 4,param_3);
  _objc_initWeak(puVar2 + 5,param_1);
  plVar4 = *(long **)(param_1 + 0x20);
  *(undefined8 **)(param_1 + 0x18) = puVar5;
  *(undefined8 **)(param_1 + 0x20) = puVar2;
  if (plVar4 == (long *)0x0) goto LAB_04401318;
  LOAcquire();
  lVar1 = plVar4[1];
  plVar4[1] = lVar1 + -1;
  LORelease();
  if (lVar1 == 0) {
    (**(code **)(*plVar4 + 0x10))(plVar4);
    if (plVar4[2] != 0) {
      LOAcquire();
      lVar1 = plVar4[2];
      plVar4[2] = lVar1 + -1;
      LORelease();
      if (lVar1 != 0) goto LAB_04401314;
    }
    pcVar3 = *(code **)(*plVar4 + 0x20);
    if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
      FUN_00005810(plVar4,0x20);
    }
    else {
      (*pcVar3)(plVar4);
    }
  }
LAB_04401314:
  puVar5 = *(undefined8 **)(param_1 + 0x18);
  puVar2 = *(undefined8 **)(param_1 + 0x20);
LAB_04401318:
  if (puVar2 != (undefined8 *)0x0) {
    puVar2[2] = puVar2[2] + 1;
  }
  FUN_043417bc(param_1 + 8,puVar5,puVar2);
  return;
}


