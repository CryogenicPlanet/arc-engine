// ==== 0x4417210  ArcWebContentsNavigationController::removeObserver:  (628 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcWebContentsNavigationController::removeObserver_(ID param_1,SEL param_2,ID param_3)

{
  ulong *puVar1;
  long *plVar2;
  ulong uVar3;
  code *pcVar4;
  long *plVar5;
  long *plVar6;
  long *plVar7;
  long lVar8;
  long lVar9;
  
  plVar6 = *(long **)(param_1 + 0x28);
  lVar8 = *(long *)(param_1 + 0x30) << 4;
  plVar7 = plVar6;
  if (lVar8 != 0) {
    plVar5 = plVar6 + *(long *)(param_1 + 0x30) * 2;
    do {
      plVar7 = plVar6;
      if (*(ID *)(*plVar6 + 8) == param_3) break;
      plVar6 = plVar6 + 2;
      lVar8 = lVar8 + -0x10;
      plVar7 = plVar5;
    } while (lVar8 != 0);
  }
  if (plVar7 == (long *)(*(long *)(param_1 + 0x28) + *(long *)(param_1 + 0x30) * 0x10)) {
    return;
  }
  uVar3 = (ulong)*(byte *)(param_1 + 0x20);
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    lVar8 = *plVar7;
    if (plVar7[1] != 0) {
      puVar1 = (ulong *)(plVar7[1] + 0x10);
      uVar3 = *puVar1;
      *puVar1 = uVar3 + 1;
    }
    FUN_04333650(uVar3,param_1 + 0x10,lVar8);
    lVar8 = *(long *)(param_1 + 0x28);
    lVar9 = *(long *)(param_1 + 0x30);
    plVar6 = (long *)(lVar8 + lVar9 * 0x10);
    if (plVar7 != plVar6) {
      plVar5 = plVar7;
      if ((long)plVar7 + (0x10 - lVar8) != lVar9 * 0x10) {
        do {
          lVar9 = plVar7[3];
          lVar8 = plVar7[2];
          plVar7[3] = 0;
          plVar7[2] = 0;
          plVar5 = (long *)plVar7[1];
          plVar7[1] = lVar9;
          *plVar7 = lVar8;
          if (plVar5 != (long *)0x0) {
            LOAcquire();
            lVar8 = plVar5[1];
            plVar5[1] = lVar8 + -1;
            LORelease();
            if (lVar8 == 0) {
              (**(code **)(*plVar5 + 0x10))(plVar5);
              if (plVar5[2] != 0) {
                LOAcquire();
                lVar8 = plVar5[2];
                plVar5[2] = lVar8 + -1;
                LORelease();
                if (lVar8 != 0) goto LAB_0441730c;
              }
              pcVar4 = *(code **)(*plVar5 + 0x20);
              if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)
                 ) {
                FUN_00005810(plVar5,0x20);
              }
              else {
                (*pcVar4)(plVar5);
              }
            }
          }
LAB_0441730c:
          plVar5 = plVar7 + 2;
          plVar2 = plVar7 + 4;
          plVar7 = plVar5;
        } while (plVar2 != plVar6);
        lVar8 = *(long *)(param_1 + 0x28);
        lVar9 = *(long *)(param_1 + 0x30);
      }
      plVar6 = (long *)(lVar8 + lVar9 * 0x10);
      if (plVar5 != plVar6) {
        do {
          plVar7 = plVar6 + -2;
          if (plVar7 == (long *)0x0) goto LAB_04417480;
          plVar6 = (long *)plVar6[-1];
          if (plVar6 != (long *)0x0) {
            LOAcquire();
            lVar8 = plVar6[1];
            plVar6[1] = lVar8 + -1;
            LORelease();
            if (lVar8 == 0) {
              (**(code **)(*plVar6 + 0x10))(plVar6);
              if (plVar6[2] != 0) {
                LOAcquire();
                lVar8 = plVar6[2];
                plVar6[2] = lVar8 + -1;
                LORelease();
                if (lVar8 != 0) goto LAB_04417400;
              }
              pcVar4 = *(code **)(*plVar6 + 0x20);
              if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)
                 ) {
                FUN_00005810(plVar6,0x20);
              }
              else {
                (*pcVar4)(plVar6);
              }
            }
          }
LAB_04417400:
          plVar6 = plVar7;
        } while (plVar7 != plVar5);
        lVar8 = *(long *)(param_1 + 0x28);
      }
      *(long *)(param_1 + 0x30) = (long)plVar5 - lVar8 >> 4;
      return;
    }
  }
LAB_04417480:
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x4417484);
  (*pcVar4)();
}


