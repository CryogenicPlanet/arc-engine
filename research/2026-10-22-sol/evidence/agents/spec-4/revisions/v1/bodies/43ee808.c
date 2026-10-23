// ==== 0x43ee808  objc::Categories::ArcBrowserContext_Extensions_::getContentSettingForOrigin:withContentSettingType:  (628 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x20 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::
   getContentSettingForOrigin_withContentSettingType_
             (ID param_1,SEL param_2,ID param_3,unsigned_long_long param_4)

{
  ulong *puVar1;
  long *plVar2;
  long lVar3;
  ID IVar4;
  ulong *puVar5;
  code *pcVar6;
  ulong uVar7;
  ulong uVar8;
  long *plVar9;
  undefined8 local_78 [2];
  undefined7 local_68;
  char cStack_61;
  undefined8 local_60;
  long *local_58;
  byte local_50;
  undefined8 local_48;
  long *local_40;
  ulong local_38;
  
  local_48 = 0;
  local_40 = (long *)0x0;
  local_38 = 0;
  _objc_msgSend(param_1,"browser_context",&local_60);
  if ((local_50 & 1) == 0) goto LAB_043eea74;
  FUN_043abdf8(local_78,param_3);
  uVar7 = 0x25;
  puVar5 = &DAT_0c9d7d08;
  do {
    uVar8 = uVar7 >> 1;
    puVar1 = puVar5 + uVar8 * 2 + 2;
    uVar7 = uVar7 + ~(uVar7 >> 1);
    if (param_4 <= puVar5[uVar8 * 2]) {
      puVar1 = puVar5;
      uVar7 = uVar8;
    }
    puVar5 = puVar1;
  } while (uVar7 != 0);
  if ((puVar1 == &DAT_0c9d7f58) || (param_4 < *puVar1)) {
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(0,0x43eeb40);
    (*pcVar6)();
  }
  FUN_042c9600(&local_48,local_60,local_78,(int)puVar1[1]);
  if (cStack_61 < '\0') {
    FUN_00005810(local_78[0],CONCAT17(cStack_61,local_68) & 0x7fffffffffffffff);
  }
  if ((local_50 != 0) && (local_58 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_58[1];
    local_58[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      (**(code **)(*local_58 + 0x10))(local_58);
      if (local_58[2] != 0) {
        LOAcquire();
        lVar3 = local_58[2];
        local_58[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043ee8f8;
      }
      pcVar6 = *(code **)(*local_58 + 0x20);
      if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
        FUN_00005810(local_58,0x20);
      }
      else {
        (*pcVar6)(local_58);
      }
    }
  }
LAB_043ee8f8:
  if ((local_38 & 1) == 0) {
    IVar4 = 0;
  }
  else {
    IVar4 = _objc_alloc(&class_t::ArcSiteSettingInfo);
    if ((local_38 & 1) == 0) {
LAB_043eea74:
                    /* WARNING: Does not return */
      pcVar6 = (code *)SoftwareBreakpoint(1,0x43eea78);
      (*pcVar6)();
    }
    IVar4 = ArcSiteSettingInfo::initWithSiteSettingInfo_(IVar4,"initWithSiteSettingInfo:",&local_48)
    ;
    plVar2 = local_40;
    if (((char)local_38 != '\0') && (local_40 != (long *)0x0)) {
      LOAcquire();
      lVar3 = local_40[1];
      local_40[1] = lVar3 + -1;
      LORelease();
      if (lVar3 == 0) {
        plVar9 = local_40 + 2;
        (**(code **)(*local_40 + 0x10))(local_40);
        if (*plVar9 != 0) {
          LOAcquire();
          lVar3 = plVar2[2];
          plVar2[2] = lVar3 + -1;
          LORelease();
          if (lVar3 != 0) {
            return IVar4;
          }
        }
        pcVar6 = *(code **)(*plVar2 + 0x20);
        if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
          FUN_00005810(plVar2,0x20);
        }
        else {
          (*pcVar6)(plVar2);
        }
      }
    }
  }
  return IVar4;
}


