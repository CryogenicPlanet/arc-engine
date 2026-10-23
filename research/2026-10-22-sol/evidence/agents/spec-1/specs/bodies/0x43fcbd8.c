// ==== 0x43fcbd8  ArcBrowserContextExtensions::setDelegate:  (552 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_storeWeak replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserContextExtensions::setDelegate_(ID param_1,SEL param_2,ID param_3)

{
  ulong *puVar1;
  code *pcVar2;
  long lVar3;
  undefined8 *puVar4;
  undefined8 uVar5;
  ulong uVar6;
  long *plVar7;
  
  if (param_3 == 0) {
    plVar7 = *(long **)(param_1 + 0x18);
    *(undefined8 *)(param_1 + 0x10) = 0;
    *(undefined8 *)(param_1 + 0x18) = 0;
    if (plVar7 == (long *)0x0) {
      return;
    }
    LOAcquire();
    lVar3 = plVar7[1];
    plVar7[1] = lVar3 + -1;
    LORelease();
    if (lVar3 != 0) {
      return;
    }
    (**(code **)(*plVar7 + 0x10))(plVar7);
    if (plVar7[2] != 0) {
      LOAcquire();
      lVar3 = plVar7[2];
      plVar7[2] = lVar3 + -1;
      LORelease();
      if (lVar3 != 0) {
        return;
      }
    }
    pcVar2 = *(code **)(*plVar7 + 0x20);
    if (((pcVar2 != FUN_00f01bc0) && (pcVar2 != FUN_00f01bc0)) && (pcVar2 != FUN_013efb40)) {
      (*pcVar2)(plVar7);
      return;
    }
    FUN_00005810(plVar7,0x20);
    return;
  }
  if (*(long *)(param_1 + 0x10) != 0) {
    *(ID *)(*(long *)(param_1 + 0x10) + 8) = param_3;
    return;
  }
  puVar4 = (undefined8 *)FUN_00008790(0x30);
  puVar4[1] = 0;
  puVar4[2] = 0;
  *puVar4 = &PTR_viewWillStartLiveResize_0da87d18;
  if (puVar4 + 3 == (undefined8 *)0x0) goto LAB_043fcdfc;
  uVar5 = *(undefined8 *)(param_1 + 8);
  puVar4[3] = &PTR_FUN_0da87d50;
  _objc_initWeak(puVar4 + 4,param_3);
  _objc_initWeak(puVar4 + 5,uVar5);
  plVar7 = *(long **)(param_1 + 0x18);
  *(undefined8 **)(param_1 + 0x10) = puVar4 + 3;
  *(undefined8 **)(param_1 + 0x18) = puVar4;
  if (plVar7 != (long *)0x0) {
    LOAcquire();
    lVar3 = plVar7[1];
    plVar7[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      (**(code **)(*plVar7 + 0x10))(plVar7);
      if (plVar7[2] != 0) {
        LOAcquire();
        lVar3 = plVar7[2];
        plVar7[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043fcd24;
      }
      pcVar2 = *(code **)(*plVar7 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plVar7,0x20);
      }
      else {
        (*pcVar2)(plVar7);
      }
    }
  }
LAB_043fcd24:
  uVar6 = (ulong)*(byte *)(param_1 + 0x30);
  if ((*(byte *)(param_1 + 0x30) & 1) != 0) {
    uVar5 = *(undefined8 *)(param_1 + 0x10);
    if (*(long *)(param_1 + 0x18) != 0) {
      puVar1 = (ulong *)(*(long *)(param_1 + 0x18) + 0x10);
      uVar6 = *puVar1;
      *puVar1 = uVar6 + 1;
    }
    FUN_04332a34(uVar6,param_1 + 0x20,uVar5);
    return;
  }
LAB_043fcdfc:
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x43fce00);
  (*pcVar2)();
}


