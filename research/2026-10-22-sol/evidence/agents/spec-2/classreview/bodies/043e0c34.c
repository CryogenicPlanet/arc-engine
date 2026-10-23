// ==== 0x43e0c34  ArcBrowser::initWithBrowser:  (828 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcBrowser::initWithBrowser_(ID param_1,SEL param_2,Browser param_3)

{
  long lVar1;
  long lVar2;
  bool bVar3;
  long lVar4;
  ID IVar5;
  ID IVar6;
  undefined8 uVar7;
  Impl *pIVar8;
  undefined8 *puVar9;
  long lVar10;
  code *pcVar11;
  long *plVar12;
  long *plVar13;
  undefined1 auVar14 [16];
  undefined1 auStack_50 [8];
  long *local_48;
  ID local_40;
  class_t *pcStack_38;
  
  pIVar8 = param_3.field0_0x0.field0_0x0;
  pcStack_38 = &objc::class_t::ArcBrowser;
  local_40 = param_1;
  IVar5 = NSObject::init((ID)&local_40,"init",pIVar8,param_3.field0_0x0.field1_0x8);
  if (IVar5 == 0) {
    return 0;
  }
  if (*(char *)(IVar5 + 0x38) != '\0') {
    plVar12 = *(long **)(IVar5 + 0x30);
    if (plVar12 != (long *)0x0) {
      LOAcquire();
      lVar4 = plVar12[1];
      plVar12[1] = lVar4 + -1;
      LORelease();
      if (lVar4 == 0) {
        (**(code **)(*plVar12 + 0x10))(plVar12);
        if (plVar12[2] != 0) {
          LOAcquire();
          lVar4 = plVar12[2];
          plVar12[2] = lVar4 + -1;
          LORelease();
          if (lVar4 != 0) goto LAB_043e0c90;
        }
        pcVar11 = *(code **)(*plVar12 + 0x20);
        if (((pcVar11 == FUN_00f01bc0) || (pcVar11 == FUN_00f01bc0)) || (pcVar11 == FUN_013efb40)) {
          FUN_00005810(plVar12,0x20);
        }
        else {
          (*pcVar11)(plVar12);
        }
      }
    }
LAB_043e0c90:
    *(undefined1 *)(IVar5 + 0x38) = 0;
  }
  *(undefined8 *)(IVar5 + 0x28) = *(undefined8 *)pIVar8;
  lVar4 = *(long *)(pIVar8 + 8);
  *(long *)(IVar5 + 0x30) = lVar4;
  if (lVar4 != 0) {
    *(long *)(lVar4 + 8) = *(long *)(lVar4 + 8) + 1;
  }
  *(undefined1 *)(IVar5 + 0x38) = 1;
  IVar6 = _objc_alloc(&objc::class_t::ArcBrowserSidePanel);
  if ((*(byte *)(IVar5 + 0x38) & 1) == 0) goto LAB_043e0f68;
  FUN_042c8400(auStack_50,*(undefined8 *)(*(long *)(IVar5 + 0x28) + 0x1f0));
  IVar6 = ArcBrowserSidePanel::initWithSidePanel_(IVar6,"initWithSidePanel:",auStack_50);
  plVar12 = local_48;
  *(ID *)(IVar5 + 8) = IVar6;
  if (local_48 != (long *)0x0) {
    LOAcquire();
    lVar4 = local_48[1];
    local_48[1] = lVar4 + -1;
    LORelease();
    if (lVar4 == 0) {
      plVar13 = local_48 + 2;
      (**(code **)(*local_48 + 0x10))(local_48);
      if (*plVar13 != 0) {
        LOAcquire();
        lVar4 = plVar12[2];
        plVar12[2] = lVar4 + -1;
        LORelease();
        if (lVar4 != 0) goto LAB_043e0d20;
      }
      pcVar11 = *(code **)(*plVar12 + 0x20);
      if (((pcVar11 == FUN_00f01bc0) || (pcVar11 == FUN_00f01bc0)) || (pcVar11 == FUN_013efb40)) {
        FUN_00005810(plVar12,0x20);
      }
      else {
        (*pcVar11)(plVar12);
      }
    }
  }
LAB_043e0d20:
  auVar14 = _objc_alloc(&objc::class_t::ArcBrowserCreateParams);
  uVar7 = auVar14._8_8_;
  if ((*(byte *)(IVar5 + 0x38) & 1) != 0) {
    puVar9 = *(undefined8 **)(*(long *)(IVar5 + 0x28) + 0x200);
    if (puVar9 == (undefined8 *)0x0) {
                    /* WARNING: Does not return */
      pcVar11 = (code *)SoftwareBreakpoint(0,0x43e1018);
      (*pcVar11)();
    }
    lVar4 = puVar9[1];
    if (lVar4 != 0) {
      uVar7 = *puVar9;
      lVar10 = *(long *)(lVar4 + 8);
      while (lVar10 != -1) {
        lVar1 = lVar10 + 1;
        lVar2 = *(long *)(lVar4 + 8);
        bVar3 = lVar2 == lVar10;
        lVar10 = lVar2;
        if (bVar3) {
          *(long *)(lVar4 + 8) = lVar1;
          FUN_043334c8(auStack_50);
          IVar6 = _objc_msgSend(auVar14._0_8_,"initWithBrowserCreateParams:",auStack_50);
          *(ID *)(IVar5 + 0x10) = IVar6;
          if (local_48 != (long *)0x0) {
            LOAcquire();
            lVar4 = local_48[1];
            local_48[1] = lVar4 + -1;
            LORelease();
            if (lVar4 == 0) {
              (**(code **)(*local_48 + 0x10))(local_48);
              if (local_48[2] != 0) {
                LOAcquire();
                lVar4 = local_48[2];
                local_48[2] = lVar4 + -1;
                LORelease();
                if (lVar4 != 0) {
                  return IVar5;
                }
              }
              pcVar11 = *(code **)(*local_48 + 0x20);
              if (((pcVar11 == FUN_00f01bc0) || (pcVar11 == FUN_00f01bc0)) ||
                 (pcVar11 == FUN_013efb40)) {
                FUN_00005810(local_48,0x20);
              }
              else {
                (*pcVar11)(local_48);
              }
            }
          }
          return IVar5;
        }
      }
    }
                    /* WARNING: Subroutine does not return */
    FUN_050d342c(auVar14._0_8_,uVar7);
  }
LAB_043e0f68:
                    /* WARNING: Does not return */
  pcVar11 = (code *)SoftwareBreakpoint(1,0x43e0f6c);
  (*pcVar11)();
}


