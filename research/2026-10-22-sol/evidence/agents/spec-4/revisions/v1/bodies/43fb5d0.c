// ==== 0x43fb5d0  objc::Categories::ArcBrowserContextPreferences_Extensions_::classifyPasteAndGoInput:maxDisplayCharacters:  (704 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x20 bytes */

ID objc::Categories::ArcBrowserContextPreferences_Extensions_::
   classifyPasteAndGoInput_maxDisplayCharacters_
             (ID param_1,SEL param_2,ID param_3,unsigned_long_long param_4)

{
  undefined8 *****pppppuVar1;
  long *plVar2;
  code *pcVar3;
  long lVar4;
  ID IVar5;
  long *plVar6;
  undefined8 local_a0;
  undefined8 uStack_98;
  ulong local_90;
  undefined8 ****local_88;
  long *plStack_80;
  ulong local_78;
  undefined8 ****local_70;
  long *plStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  ulong local_48;
  ulong local_40;
  
  local_40 = 0;
  uStack_58 = 0;
  local_60 = 0;
  local_48 = 0;
  uStack_50 = 0;
  plStack_68 = (long *)0x0;
  local_70 = (undefined8 *****)0x0;
  _objc_msgSend(param_1,"browser_context_preferences",&local_88);
  if ((local_78 & 1) == 0) goto LAB_043fb88c;
  if (param_3 == 0) {
    local_a0 = 0;
    uStack_98 = 0;
    local_90 = 0;
  }
  else {
    FUN_01b356b8(&local_a0,param_3);
  }
  FUN_042f4ddc(&local_70,local_88,&local_a0,param_4);
  if ((long)local_90 < 0) {
    FUN_00005810(local_a0,local_90 & 0x7fffffffffffffff);
  }
  plVar2 = plStack_80;
  if (((char)local_78 != '\0') && (plStack_80 != (long *)0x0)) {
    LOAcquire();
    lVar4 = plStack_80[1];
    plStack_80[1] = lVar4 + -1;
    LORelease();
    if (lVar4 == 0) {
      plVar6 = plStack_80 + 2;
      (**(code **)(*plStack_80 + 0x10))(plStack_80);
      if (*plVar6 != 0) {
        LOAcquire();
        lVar4 = plVar2[2];
        plVar2[2] = lVar4 + -1;
        LORelease();
        if (lVar4 != 0) goto LAB_043fb69c;
      }
      pcVar3 = *(code **)(*plVar2 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(plVar2,0x20);
      }
      else {
        (*pcVar3)(plVar2);
      }
    }
  }
LAB_043fb69c:
  if ((local_40 & 1) == 0) {
    IVar5 = 0;
  }
  else {
    lVar4 = FUN_043abd34(&uStack_58);
    if (lVar4 == 0) {
      IVar5 = 0;
      if ((char)local_40 != '\x01') {
        return 0;
      }
    }
    else {
      if ((local_40 & 1) == 0) {
LAB_043fb88c:
                    /* WARNING: Does not return */
        pcVar3 = (code *)SoftwareBreakpoint(1,0x43fb890);
        (*pcVar3)();
      }
      pppppuVar1 = (undefined8 *****)local_70;
      if (-1 < (long)local_60._7_1_) {
        pppppuVar1 = &local_70;
      }
      plVar2 = plStack_68;
      if (-1 < (long)local_60) {
        plVar2 = (long *)(long)local_60._7_1_;
      }
      FUN_007cf8e0(&local_a0,pppppuVar1,plVar2);
      lVar4 = FUN_01d9f82c(&local_a0);
      if (lVar4 == 0) {
        local_88 = (undefined8 *****)0x0;
        plStack_80 = (long *)0x0;
        local_78 = 0;
      }
      else {
        FUN_01b356b8(&local_88,lVar4);
      }
      if ((local_40 & 1) == 0) goto LAB_043fb88c;
      if ((long)local_60 < 0) {
        FUN_00005810(local_70,local_60 & 0x7fffffffffffffff);
      }
      plStack_68 = plStack_80;
      local_70 = local_88;
      local_60 = local_78;
      if ((long)local_90 < 0) {
        FUN_00005810(local_a0,local_90 << 1);
      }
      IVar5 = _objc_alloc(&class_t::ArcPasteAndGoClassification);
      if ((local_40 & 1) == 0) goto LAB_043fb88c;
      IVar5 = ArcPasteAndGoClassification::initWithPasteAndGoClassification_
                        (IVar5,"initWithPasteAndGoClassification:",&local_70);
      if ((local_40 & 1) == 0) {
        return IVar5;
      }
    }
    if ((long)local_48 < 0) {
      FUN_00005810(uStack_58,local_48 & 0x7fffffffffffffff);
    }
    if ((long)local_60 < 0) {
      FUN_00005810(local_70,local_60 & 0x7fffffffffffffff);
    }
  }
  return IVar5;
}


