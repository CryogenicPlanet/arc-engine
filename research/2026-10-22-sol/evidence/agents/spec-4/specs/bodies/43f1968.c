// ==== 0x43f1968  objc::Categories::ArcBrowserContext_Extensions_::addBookmarkModelObserver:  (1456 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::addBookmarkModelObserver_
               (ID param_1,SEL param_2,ID param_3)

{
  ulong uVar1;
  bool bVar2;
  long lVar3;
  long *plVar4;
  ulong uVar5;
  code *pcVar6;
  ulong uVar7;
  long *plVar8;
  long *plVar9;
  long *plVar10;
  long lVar11;
  size_t sVar12;
  long *plVar13;
  long *local_a8;
  ulong local_a0;
  ulong local_98;
  undefined1 auStack_90 [8];
  long *local_88;
  byte local_80;
  long *local_78;
  ulong local_70;
  ulong local_68;
  
  local_78 = (long *)0x0;
  local_70 = 0;
  local_68 = 0;
  _objc_msgSend(param_1,"bookmarkObservers",&local_78);
  plVar10 = local_78;
  for (lVar11 = local_70 << 4; lVar11 != 0; lVar11 = lVar11 + -0x10) {
    if (*(ID *)(*plVar10 + 8) == param_3) goto LAB_043f1d08;
    plVar10 = plVar10 + 2;
  }
  plVar4 = (long *)FUN_00008790(0x28);
  plVar8 = plVar4 + 1;
  *plVar8 = 0;
  plVar4[2] = 0;
  *plVar4 = (long)&PTR_viewWillStartLiveResize_0da87a98;
  plVar10 = plVar4 + 3;
  if (plVar10 == (long *)0x0) goto LAB_043f1bd8;
  plVar4[3] = (long)&PTR_FUN_0da87ad0;
  _objc_initWeak(plVar4 + 4,param_3);
  _objc_msgSend(param_1,"browser_context",auStack_90);
  uVar5 = (ulong)local_80;
  if ((local_80 & 1) == 0) goto LAB_043f1bd8;
  if (plVar4 != (long *)0x0) {
    uVar5 = plVar4[2];
    plVar4[2] = uVar5 + 1;
  }
  FUN_04333c0c(uVar5,auStack_90,plVar10,plVar4);
  if ((local_80 != 0) && (local_88 != (long *)0x0)) {
    LOAcquire();
    lVar11 = local_88[1];
    local_88[1] = lVar11 + -1;
    LORelease();
    if (lVar11 == 0) {
      (**(code **)(*local_88 + 0x10))(local_88);
      if (local_88[2] != 0) {
        LOAcquire();
        lVar11 = local_88[2];
        local_88[2] = lVar11 + -1;
        LORelease();
        if (lVar11 != 0) goto LAB_043f1a94;
      }
      pcVar6 = *(code **)(*local_88 + 0x20);
      if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
        FUN_00005810(local_88,0x20);
      }
      else {
        (*pcVar6)(local_88);
      }
    }
  }
LAB_043f1a94:
  uVar5 = local_70;
  if (local_70 == local_68) {
    uVar1 = local_70 + 1;
    if (uVar1 >> 0x3c != 0) goto LAB_043f2028;
    uVar7 = local_70 * 2;
    if (uVar7 < uVar1 || uVar7 - uVar1 == 0) {
      uVar7 = uVar1;
    }
    if (0x7fffffffffffffe < local_70) {
      uVar7 = 0xfffffffffffffff;
    }
    if (uVar7 == 0) {
      lVar11 = 0;
    }
    else {
      if (uVar7 >> 0x3c != 0) {
                    /* WARNING: Subroutine does not return */
        FUN_04b087f0();
      }
      lVar11 = FUN_00008790(uVar7 << 4);
    }
    plVar13 = (long *)(lVar11 + uVar5 * 0x10);
    if (plVar13 == (long *)0x0) goto LAB_043f1bd8;
    *plVar13 = (long)plVar10;
    plVar13[1] = (long)plVar4;
    if (plVar4 != (long *)0x0) {
      *plVar8 = *plVar8 + 1;
    }
    plVar9 = plVar13 + local_70 * -2;
    sVar12 = local_70 * 0x10;
    _memcpy(plVar9,local_78,sVar12);
    plVar10 = local_78;
    local_70 = (long)(sVar12 + 0x10) >> 4;
    bVar2 = local_78 != (long *)0x0;
    local_78 = plVar9;
    if (bVar2) {
      lVar11 = local_68 << 4;
      local_68 = uVar7;
      FUN_00005810(plVar10,lVar11);
      uVar7 = local_68;
    }
  }
  else {
    plVar13 = local_78 + local_70 * 2;
    if (plVar13 == (long *)0x0) goto LAB_043f1bd8;
    *plVar13 = (long)plVar10;
    plVar13[1] = (long)plVar4;
    uVar7 = local_68;
    if (plVar4 != (long *)0x0) {
      *plVar8 = *plVar8 + 1;
    }
  }
  local_68 = uVar7;
  plVar10 = local_78;
  lVar11 = (long)plVar13 + (0x10 - (long)local_78);
  uVar5 = lVar11 >> 4;
  local_70 = uVar5;
  if (lVar11 == 0) {
LAB_043f1bd8:
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(1,0x43f1bdc);
    (*pcVar6)();
  }
  local_a8 = (long *)0x0;
  local_a0 = 0;
  local_98 = 0;
  if (uVar5 >> 0x3c != 0) {
LAB_043f2028:
                    /* WARNING: Subroutine does not return */
    FUN_040416c0();
  }
  local_a8 = (long *)FUN_00008790();
  plVar9 = local_a8;
  do {
    local_98 = uVar5;
    if (plVar9 == (long *)0x0) goto LAB_043f1bd8;
    *plVar9 = *plVar10;
    lVar11 = plVar10[1];
    plVar9[1] = lVar11;
    if (lVar11 != 0) {
      *(long *)(lVar11 + 8) = *(long *)(lVar11 + 8) + 1;
    }
    plVar9 = plVar9 + 2;
    bVar2 = plVar10 != plVar13;
    plVar10 = plVar10 + 2;
  } while (bVar2);
  local_a0 = (long)plVar9 - (long)local_a8 >> 4;
  _objc_msgSend(param_1,"setBookmarkObservers:",&local_a8);
  plVar10 = local_a8;
  if (local_a8 != (long *)0x0) {
    if ((local_a0 & 0xfffffffffffffff) != 0) {
      lVar11 = local_a0 * -0x10;
      plVar13 = local_a8 + local_a0 * 2;
      do {
        if (plVar13 + -2 == (long *)0x0) goto LAB_043f1bd8;
        plVar9 = (long *)plVar13[-1];
        if (plVar9 != (long *)0x0) {
          LOAcquire();
          lVar3 = plVar9[1];
          plVar9[1] = lVar3 + -1;
          LORelease();
          if (lVar3 == 0) {
            (**(code **)(*plVar9 + 0x10))(plVar9);
            if (plVar9[2] != 0) {
              LOAcquire();
              lVar3 = plVar9[2];
              plVar9[2] = lVar3 + -1;
              LORelease();
              if (lVar3 != 0) goto LAB_043f1be8;
            }
            pcVar6 = *(code **)(*plVar9 + 0x20);
            if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40))
            {
              FUN_00005810(plVar9,0x20);
            }
            else {
              (*pcVar6)(plVar9);
            }
          }
        }
LAB_043f1be8:
        lVar11 = lVar11 + 0x10;
        plVar13 = plVar13 + -2;
      } while (lVar11 != 0);
    }
    local_a0 = (long)plVar10 - (long)local_a8 >> 4;
    FUN_00005810(local_a8,local_98 << 4);
  }
  if (plVar4 != (long *)0x0) {
    LOAcquire();
    lVar11 = *plVar8;
    *plVar8 = lVar11 + -1;
    LORelease();
    if (lVar11 == 0) {
      (**(code **)(*plVar4 + 0x10))(plVar4);
      if (plVar4[2] != 0) {
        LOAcquire();
        lVar11 = plVar4[2];
        plVar4[2] = lVar11 + -1;
        LORelease();
        if (lVar11 != 0) goto LAB_043f1d08;
      }
      pcVar6 = *(code **)(*plVar4 + 0x20);
      if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
        FUN_00005810(plVar4,0x20);
      }
      else {
        (*pcVar6)(plVar4);
      }
    }
  }
LAB_043f1d08:
  plVar10 = local_78;
  if (local_78 != (long *)0x0) {
    if ((local_70 & 0xfffffffffffffff) != 0) {
      lVar11 = local_70 * -0x10;
      plVar4 = local_78 + local_70 * 2;
      do {
        if (plVar4 + -2 == (long *)0x0) goto LAB_043f1bd8;
        plVar8 = (long *)plVar4[-1];
        if (plVar8 != (long *)0x0) {
          LOAcquire();
          lVar3 = plVar8[1];
          plVar8[1] = lVar3 + -1;
          LORelease();
          if (lVar3 == 0) {
            (**(code **)(*plVar8 + 0x10))(plVar8);
            if (plVar8[2] != 0) {
              LOAcquire();
              lVar3 = plVar8[2];
              plVar8[2] = lVar3 + -1;
              LORelease();
              if (lVar3 != 0) goto LAB_043f1da0;
            }
            pcVar6 = *(code **)(*plVar8 + 0x20);
            if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40))
            {
              FUN_00005810(plVar8,0x20);
            }
            else {
              (*pcVar6)(plVar8);
            }
          }
        }
LAB_043f1da0:
        lVar11 = lVar11 + 0x10;
        plVar4 = plVar4 + -2;
      } while (lVar11 != 0);
    }
    local_70 = (long)plVar10 - (long)local_78 >> 4;
    FUN_00005810(local_78,local_68 << 4);
  }
  return;
}


