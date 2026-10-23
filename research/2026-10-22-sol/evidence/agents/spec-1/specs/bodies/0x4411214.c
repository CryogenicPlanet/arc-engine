// ==== 0x4411214  objc::Categories::ArcWebContents_Extensions_::devTools  (712 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcWebContents_Extensions_::devTools(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  __shared_weak_count *in_x3;
  code *pcVar4;
  long *plVar5;
  long lVar6;
  undefined8 uVar7;
  long *plVar8;
  DevTools DVar9;
  undefined8 local_88;
  long *local_80;
  long local_78;
  long *local_70;
  byte local_68;
  undefined8 local_60;
  long *plStack_58;
  
  IVar3 = _objc_getAssociatedObject(param_1,"devTools");
  if (IVar3 != 0) {
    return IVar3;
  }
  _objc_msgSend(param_1,"web_contents",&local_78);
  if ((local_68 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x44114dc);
    (*pcVar4)();
  }
  lVar6 = *(long *)(local_78 + 0x2b8);
  if (lVar6 == 0) {
    plVar5 = (long *)0x0;
    uVar7 = 0;
LAB_044112bc:
    if (local_70 != (long *)0x0) {
      LOAcquire();
      lVar2 = local_70[1];
      local_70[1] = lVar2 + -1;
      LORelease();
      if (lVar2 == 0) {
        (**(code **)(*local_70 + 0x10))(local_70);
        if (local_70[2] != 0) {
          LOAcquire();
          lVar2 = local_70[2];
          local_70[2] = lVar2 + -1;
          LORelease();
          if (lVar2 != 0) goto LAB_044112d4;
        }
        pcVar4 = *(code **)(*local_70 + 0x20);
        if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
          FUN_00005810(local_70,0x20);
        }
        else {
          (*pcVar4)(local_70);
        }
      }
    }
LAB_044112d4:
    if (lVar6 == 0) {
      return 0;
    }
  }
  else {
    FUN_042c8400(&local_60,lVar6);
    plVar5 = plStack_58;
    uVar7 = local_60;
    if ((local_68 & 1) != 0) goto LAB_044112bc;
  }
  IVar3 = _objc_alloc(&class_t::ArcWebContentsDevTools);
  if (plVar5 != (long *)0x0) {
    plVar5[1] = plVar5[1] + 1;
  }
  DVar9.field0_0x0.field1_0x8 = in_x3;
  DVar9.field0_0x0.field0_0x0 = (Impl *)&local_88;
  local_88 = uVar7;
  local_80 = plVar5;
  IVar3 = ArcWebContentsDevTools::init_(IVar3,"init:",DVar9);
  plVar1 = local_80;
  if (local_80 != (long *)0x0) {
    LOAcquire();
    lVar6 = local_80[1];
    local_80[1] = lVar6 + -1;
    LORelease();
    if (lVar6 == 0) {
      plVar8 = local_80 + 2;
      (**(code **)(*local_80 + 0x10))(local_80);
      if (*plVar8 != 0) {
        LOAcquire();
        lVar6 = plVar1[2];
        plVar1[2] = lVar6 + -1;
        LORelease();
        if (lVar6 != 0) goto LAB_04411324;
      }
      pcVar4 = *(code **)(*plVar1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar4)(plVar1);
      }
    }
  }
LAB_04411324:
  _objc_setAssociatedObject(param_1,"devTools",IVar3,1);
  if (plVar5 != (long *)0x0) {
    LOAcquire();
    lVar6 = plVar5[1];
    plVar5[1] = lVar6 + -1;
    LORelease();
    if (lVar6 == 0) {
      (**(code **)(*plVar5 + 0x10))(plVar5);
      if (plVar5[2] != 0) {
        LOAcquire();
        lVar6 = plVar5[2];
        plVar5[2] = lVar6 + -1;
        LORelease();
        if (lVar6 != 0) {
          return IVar3;
        }
      }
      pcVar4 = *(code **)(*plVar5 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar5,0x20);
      }
      else {
        (*pcVar4)(plVar5);
      }
    }
  }
  return IVar3;
}


