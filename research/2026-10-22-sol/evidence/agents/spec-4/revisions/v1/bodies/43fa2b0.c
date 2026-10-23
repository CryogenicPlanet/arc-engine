// ==== 0x43fa2b0  objc::Categories::ArcBrowserContextPreferences_Extensions_::getSearchEngines  (840 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcBrowserContextPreferences_Extensions_::getSearchEngines
             (ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  ID IVar5;
  ID IVar6;
  code *pcVar7;
  long *plVar8;
  long *plVar9;
  long lVar10;
  long local_a0;
  long *plStack_98;
  undefined8 local_90;
  long *local_88;
  byte local_80;
  long local_78;
  long local_70;
  long local_68;
  
  IVar5 = NSMutableArray::new((ID)&_OBJC_CLASS___NSMutableArray,"new");
  _objc_msgSend(param_1,"browser_context_preferences",&local_90);
  if ((local_80 & 1) == 0) {
LAB_043fa5f4:
                    /* WARNING: Does not return */
    pcVar7 = (code *)SoftwareBreakpoint(1,0x43fa5f8);
    (*pcVar7)();
  }
  FUN_042f2854(&local_78,local_90);
  lVar10 = local_70 << 4;
  if (lVar10 != 0) {
    plVar8 = (long *)(local_78 + 8);
    do {
      plStack_98 = (long *)*plVar8;
      local_a0 = plVar8[-1];
      lVar3 = 0;
      if (*plVar8 != 0) {
        plVar1 = (long *)(*plVar8 + 8);
        lVar3 = *plVar1;
        *plVar1 = lVar3 + 1;
      }
      IVar6 = _objc_alloc(lVar3,&class_t::ArcSearchEngine);
      IVar6 = _objc_msgSend(IVar6,"initWithSearchEngineTemplateURL:",&local_a0);
      NSMutableArray::addObject_(IVar5,"addObject:",IVar6);
      plVar1 = plStack_98;
      if (plStack_98 != (long *)0x0) {
        LOAcquire();
        lVar3 = plStack_98[1];
        plStack_98[1] = lVar3 + -1;
        LORelease();
        if (lVar3 == 0) {
          plVar9 = plStack_98 + 2;
          (**(code **)(*plStack_98 + 0x10))(plStack_98);
          if (*plVar9 != 0) {
            LOAcquire();
            lVar3 = plVar1[2];
            plVar1[2] = lVar3 + -1;
            LORelease();
            if (lVar3 != 0) goto LAB_043fa37c;
          }
          pcVar7 = *(code **)(*plVar1 + 0x20);
          if (((pcVar7 == FUN_00f01bc0) || (pcVar7 == FUN_00f01bc0)) || (pcVar7 == FUN_013efb40)) {
            FUN_00005810(plVar1,0x20);
          }
          else {
            (*pcVar7)(plVar1);
          }
        }
      }
LAB_043fa37c:
      plVar8 = plVar8 + 2;
      lVar10 = lVar10 + -0x10;
    } while (lVar10 != 0);
  }
  lVar10 = local_78;
  if (local_78 != 0) {
    if (local_70 * 0x10 != 0) {
      lVar3 = local_70 * -0x10;
      lVar2 = local_70 * 0x10 + local_78;
      do {
        if (lVar2 + -0x10 == 0) goto LAB_043fa5f4;
        plVar8 = *(long **)(lVar2 + -8);
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
              if (lVar4 != 0) goto LAB_043fa4dc;
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
LAB_043fa4dc:
        lVar3 = lVar3 + 0x10;
        lVar2 = lVar2 + -0x10;
      } while (lVar3 != 0);
    }
    local_70 = lVar10 - local_78 >> 4;
    FUN_00005810(local_78,local_68 << 4);
  }
  if ((local_80 != 0) && (local_88 != (long *)0x0)) {
    LOAcquire();
    lVar10 = local_88[1];
    local_88[1] = lVar10 + -1;
    LORelease();
    if (lVar10 == 0) {
      (**(code **)(*local_88 + 0x10))(local_88);
      if (local_88[2] != 0) {
        LOAcquire();
        lVar10 = local_88[2];
        local_88[2] = lVar10 + -1;
        LORelease();
        if (lVar10 != 0) goto LAB_043fa490;
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
LAB_043fa490:
  IVar5 = NSMutableArray::copy(IVar5,"copy");
  return IVar5;
}


