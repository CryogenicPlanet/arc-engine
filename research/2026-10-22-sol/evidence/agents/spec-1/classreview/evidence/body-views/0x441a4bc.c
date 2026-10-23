// ==== 0x441a4bc  ArcWebContentsMediaSession::setDelegate:  (552 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_storeWeak replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcWebContentsMediaSession::setDelegate_(ID param_1,SEL param_2,ID param_3)

{
  ulong *puVar1;
  code *pcVar2;
  long lVar3;
  undefined8 *puVar4;
  undefined8 uVar5;
  ulong uVar6;
  long *plVar7;
  
  if (param_3 == 0) {
    plVar7 = *(long **)(param_1 + 0x28);
    *(undefined8 *)(param_1 + 0x20) = 0;
    *(undefined8 *)(param_1 + 0x28) = 0;
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
  if (*(long *)(param_1 + 0x20) != 0) {
    *(ID *)(*(long *)(param_1 + 0x20) + 8) = param_3;
    return;
  }
  puVar4 = (undefined8 *)FUN_00008790(0x30);
  puVar4[1] = 0;
  puVar4[2] = 0;
  *puVar4 = &PTR_viewWillStartLiveResize_0da88330;
  if (puVar4 + 3 == (undefined8 *)0x0) goto LAB_0441a6e0;
  uVar5 = *(undefined8 *)(param_1 + 8);
  puVar4[3] = &PTR_FUN_0da88368;
  _objc_initWeak(puVar4 + 4,param_3);
  _objc_initWeak(puVar4 + 5,uVar5);
  plVar7 = *(long **)(param_1 + 0x28);
  *(undefined8 **)(param_1 + 0x20) = puVar4 + 3;
  *(undefined8 **)(param_1 + 0x28) = puVar4;
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
        if (lVar3 != 0) goto LAB_0441a608;
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
LAB_0441a608:
  uVar6 = (ulong)*(byte *)(param_1 + 0x40);
  if ((*(byte *)(param_1 + 0x40) & 1) != 0) {
    uVar5 = *(undefined8 *)(param_1 + 0x20);
    if (*(long *)(param_1 + 0x28) != 0) {
      puVar1 = (ulong *)(*(long *)(param_1 + 0x28) + 0x10);
      uVar6 = *puVar1;
      *puVar1 = uVar6 + 1;
    }
    FUN_043458dc(uVar6,param_1 + 0x30,uVar5);
    return;
  }
LAB_0441a6e0:
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x441a6e4);
  (*pcVar2)();
}


