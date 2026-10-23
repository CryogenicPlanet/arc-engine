// ==== 0x43ee38c  objc::Categories::ArcBrowserContext_Extensions_::getChangedPermissionsForOrigin:  (732 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::getChangedPermissionsForOrigin_
             (ID param_1,SEL param_2,ID param_3)

{
  const_void__ pvVar1;
  const_void__ pvVar2;
  long lVar3;
  long lVar4;
  ID IVar5;
  ID IVar6;
  code *pcVar7;
  long *plVar8;
  undefined8 local_a8 [2];
  undefined7 local_98;
  char cStack_91;
  undefined8 local_90;
  long *local_88;
  byte local_80;
  const_void__ local_78;
  long local_70;
  long local_68;
  
  local_78 = (const_void__)0x0;
  local_70 = 0;
  local_68 = 0;
  _objc_msgSend(param_1,"browser_context",&local_90);
  if ((local_80 & 1) == 0) {
LAB_043ee638:
                    /* WARNING: Does not return */
    pcVar7 = (code *)SoftwareBreakpoint(1,0x43ee63c);
    (*pcVar7)();
  }
  FUN_043abdf8(local_a8,param_3);
  FUN_042c8da8(&local_78,local_90,local_a8);
  if (cStack_91 < '\0') {
    FUN_00005810(local_a8[0],CONCAT17(cStack_91,local_98) & 0x7fffffffffffffff);
  }
  if ((local_80 != 0) && (local_88 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_88[1];
    local_88[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      (**(code **)(*local_88 + 0x10))(local_88);
      if (local_88[2] != 0) {
        LOAcquire();
        lVar3 = local_88[2];
        local_88[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) goto LAB_043ee434;
      }
      pcVar7 = *(code **)(*local_88 + 0x20);
      if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40)) {
        FUN_00005810(local_88,0x20);
      }
      else {
        (*pcVar7)(local_88);
      }
    }
  }
LAB_043ee434:
  IVar5 = NSMutableArray::new((ID)&_OBJC_CLASS___NSMutableArray,"new");
  pvVar1 = local_78;
  for (lVar3 = local_70 << 4; lVar3 != 0; lVar3 = lVar3 + -0x10) {
    IVar6 = _objc_alloc(&class_t::ArcSiteSettingInfo);
    IVar6 = ArcSiteSettingInfo::initWithSiteSettingInfo_(IVar6,"initWithSiteSettingInfo:",pvVar1);
    NSMutableArray::addObject_(IVar5,"addObject:",IVar6);
    pvVar1 = (const_void__)((long)pvVar1 + 0x10);
  }
  IVar5 = NSMutableArray::copy(IVar5,"copy");
  pvVar1 = local_78;
  if (local_78 != (const_void__)0x0) {
    if (local_70 * 0x10 != 0) {
      lVar3 = local_70 * -0x10;
      pvVar2 = (const_void__)(local_70 * 0x10 + (long)local_78);
      do {
        if ((const_void__)((long)pvVar2 + -0x10) == (const_void__)0x0) goto LAB_043ee638;
        plVar8 = *(long **)((long)pvVar2 + -8);
        if (plVar8 != (long *)0x0) {
          LOAcquire();
          lVar4 = plVar8[1];
          plVar8[1] = lVar4 + -1;
          LORelease();
          if (lVar4 == 0) {
            (**(code **)(*plVar8 + 0x10))(plVar8);
            if (plVar8[2] != 0) {
              LOAcquire();
              lVar4 = plVar8[2];
              plVar8[2] = lVar4 + -1;
              LORelease();
              if (lVar4 != 0) goto LAB_043ee55c;
            }
            pcVar7 = *(code **)(*plVar8 + 0x20);
            if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40))
            {
              FUN_00005810(plVar8,0x20);
            }
            else {
              (*pcVar7)(plVar8);
            }
          }
        }
LAB_043ee55c:
        lVar3 = lVar3 + 0x10;
        pvVar2 = (const_void__)((long)pvVar2 + -0x10);
      } while (lVar3 != 0);
    }
    local_70 = (long)pvVar1 - (long)local_78 >> 4;
    FUN_00005810(local_78,local_68 << 4);
  }
  return IVar5;
}


