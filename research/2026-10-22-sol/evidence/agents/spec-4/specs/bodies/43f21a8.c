// ==== 0x43f21a8  objc::Categories::ArcBrowserContext_Extensions_::removeBookmarkModelObserver:  (1460 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::removeBookmarkModelObserver_
               (ID param_1,SEL param_2,ID param_3)

{
  ulong *puVar1;
  long *plVar2;
  long lVar3;
  long *plVar4;
  ulong uVar5;
  code *pcVar6;
  long *plVar7;
  long *plVar8;
  long *plVar9;
  long lVar10;
  long *local_a8;
  ulong local_a0;
  ulong local_98;
  undefined1 auStack_90 [8];
  long *local_88;
  byte local_80;
  long *local_78;
  ulong local_70;
  long local_68;
  
  local_78 = (long *)0x0;
  local_70 = 0;
  local_68 = 0;
  _objc_msgSend(param_1,"bookmarkObservers",&local_78);
  lVar10 = local_70 << 4;
  plVar9 = local_78;
  if (lVar10 != 0) {
    plVar4 = local_78;
    do {
      plVar9 = plVar4;
      if (*(ID *)(*plVar4 + 8) == param_3) break;
      plVar4 = plVar4 + 2;
      lVar10 = lVar10 + -0x10;
      plVar9 = local_78 + local_70 * 2;
    } while (lVar10 != 0);
  }
  if (plVar9 == local_78 + local_70 * 2) goto LAB_043f2620;
  _objc_msgSend(param_1,"browser_context",auStack_90);
  uVar5 = (ulong)local_80;
  if ((local_80 & 1) == 0) goto LAB_043f256c;
  lVar10 = *plVar9;
  if (plVar9[1] != 0) {
    puVar1 = (ulong *)(plVar9[1] + 0x10);
    uVar5 = *puVar1;
    *puVar1 = uVar5 + 1;
  }
  FUN_04333cc4(uVar5,auStack_90,lVar10);
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
        if (lVar10 != 0) goto LAB_043f22a8;
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
LAB_043f22a8:
  plVar4 = local_78 + local_70 * 2;
  if (plVar9 == plVar4) {
LAB_043f256c:
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(1,0x43f2570);
    (*pcVar6)();
  }
  plVar7 = plVar9;
  if ((long)plVar9 + (0x10 - (long)local_78) != local_70 * 0x10) {
    do {
      lVar3 = plVar9[3];
      lVar10 = plVar9[2];
      plVar9[3] = 0;
      plVar9[2] = 0;
      plVar7 = (long *)plVar9[1];
      plVar9[1] = lVar3;
      *plVar9 = lVar10;
      if (plVar7 != (long *)0x0) {
        LOAcquire();
        lVar10 = plVar7[1];
        plVar7[1] = lVar10 + -1;
        LORelease();
        if (lVar10 == 0) {
          (**(code **)(*plVar7 + 0x10))(plVar7);
          if (plVar7[2] != 0) {
            LOAcquire();
            lVar10 = plVar7[2];
            plVar7[2] = lVar10 + -1;
            LORelease();
            if (lVar10 != 0) goto LAB_043f2348;
          }
          pcVar6 = *(code **)(*plVar7 + 0x20);
          if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
            FUN_00005810(plVar7,0x20);
          }
          else {
            (*pcVar6)(plVar7);
          }
        }
      }
LAB_043f2348:
      plVar7 = plVar9 + 2;
      plVar2 = plVar9 + 4;
      plVar9 = plVar7;
    } while (plVar2 != plVar4);
  }
  plVar4 = local_78 + local_70 * 2;
  plVar9 = local_78;
joined_r0x043f2478:
  local_78 = plVar9;
  if (plVar7 != plVar4) {
    plVar2 = plVar4 + -2;
    if (plVar2 == (long *)0x0) goto LAB_043f256c;
    plVar8 = (long *)plVar4[-1];
    plVar4 = plVar2;
    if (plVar8 != (long *)0x0) {
      LOAcquire();
      lVar10 = plVar8[1];
      plVar8[1] = lVar10 + -1;
      LORelease();
      if (lVar10 == 0) {
        (**(code **)(*plVar8 + 0x10))(plVar8);
        if (plVar8[2] != 0) {
          LOAcquire();
          lVar10 = plVar8[2];
          plVar8[2] = lVar10 + -1;
          LORelease();
          plVar9 = local_78;
          if (lVar10 != 0) goto joined_r0x043f2478;
        }
        pcVar6 = *(code **)(*plVar8 + 0x20);
        if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
          FUN_00005810(plVar8,0x20);
          plVar9 = local_78;
        }
        else {
          (*pcVar6)(plVar8);
          plVar9 = local_78;
        }
      }
    }
    goto joined_r0x043f2478;
  }
  local_a0 = 0;
  local_98 = 0;
  local_a8 = (long *)0x0;
  uVar5 = (long)plVar7 - (long)plVar9 >> 4;
  local_70 = uVar5;
  if ((long)plVar7 - (long)plVar9 != 0) {
    if (uVar5 >> 0x3c != 0) {
                    /* WARNING: Subroutine does not return */
      FUN_040416c0();
    }
    local_a8 = (long *)FUN_00008790();
    local_a0 = 0;
    plVar4 = local_a8;
    do {
      local_98 = uVar5;
      if (plVar4 == (long *)0x0) goto LAB_043f256c;
      *plVar4 = *plVar9;
      lVar10 = plVar9[1];
      plVar4[1] = lVar10;
      if (lVar10 != 0) {
        *(long *)(lVar10 + 8) = *(long *)(lVar10 + 8) + 1;
      }
      plVar9 = plVar9 + 2;
      plVar4 = plVar4 + 2;
    } while (plVar9 != plVar7);
    local_a0 = (long)plVar4 - (long)local_a8 >> 4;
  }
  _objc_msgSend(param_1,"setBookmarkObservers:",&local_a8);
  plVar9 = local_a8;
  if (local_a8 != (long *)0x0) {
    if ((local_a0 & 0xfffffffffffffff) != 0) {
      lVar10 = local_a0 * -0x10;
      plVar4 = local_a8 + local_a0 * 2;
      do {
        if (plVar4 + -2 == (long *)0x0) goto LAB_043f256c;
        plVar7 = (long *)plVar4[-1];
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
              if (lVar3 != 0) goto LAB_043f257c;
            }
            pcVar6 = *(code **)(*plVar7 + 0x20);
            if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40))
            {
              FUN_00005810(plVar7,0x20);
            }
            else {
              (*pcVar6)(plVar7);
            }
          }
        }
LAB_043f257c:
        lVar10 = lVar10 + 0x10;
        plVar4 = plVar4 + -2;
      } while (lVar10 != 0);
    }
    local_a0 = (long)plVar9 - (long)local_a8 >> 4;
    FUN_00005810(local_a8,local_98 << 4);
  }
LAB_043f2620:
  plVar9 = local_78;
  if (local_78 != (long *)0x0) {
    if ((local_70 & 0xfffffffffffffff) != 0) {
      lVar10 = local_70 * -0x10;
      plVar4 = local_78 + local_70 * 2;
      do {
        if (plVar4 + -2 == (long *)0x0) goto LAB_043f256c;
        plVar7 = (long *)plVar4[-1];
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
              if (lVar3 != 0) goto LAB_043f26b0;
            }
            pcVar6 = *(code **)(*plVar7 + 0x20);
            if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40))
            {
              FUN_00005810(plVar7,0x20);
            }
            else {
              (*pcVar6)(plVar7);
            }
          }
        }
LAB_043f26b0:
        lVar10 = lVar10 + 0x10;
        plVar4 = plVar4 + -2;
      } while (lVar10 != 0);
    }
    local_70 = (long)plVar9 - (long)local_78 >> 4;
    FUN_00005810(local_78,local_68 << 4);
  }
  return;
}


