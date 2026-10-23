// ==== 0x4434f38  ArcBrowserContextPreferences::addObserver:  (684 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void ArcBrowserContextPreferences::addObserver_(ID param_1,SEL param_2,ID param_3)

{
  long *plVar1;
  ulong uVar2;
  long lVar3;
  ID IVar4;
  long *plVar5;
  long lVar6;
  ulong uVar7;
  code *pcVar8;
  ulong uVar9;
  size_t sVar10;
  long *plVar11;
  long *plVar12;
  undefined8 local_60;
  undefined1 auStack_58 [8];
  
  IVar4 = ImmersiveModeMapper::originalHostingWindow(param_1,"owner");
  plVar5 = (long *)FUN_00008790(0x30);
  plVar11 = plVar5 + 1;
  *plVar11 = 0;
  plVar5[2] = 0;
  *plVar5 = (long)&PTR_viewWillStartLiveResize_0da88da0;
  plVar1 = plVar5 + 3;
  if (plVar1 != (long *)0x0) {
    _objc_initWeak(&local_60,IVar4);
    _objc_initWeak(auStack_58,local_60);
    plVar5[3] = (long)&PTR_FUN_0da88ef0;
    _objc_copyWeak(plVar5 + 4,auStack_58);
    _objc_initWeak(plVar5 + 5,param_3);
    _objc_destroyWeak(auStack_58);
    _objc_destroyWeak(&local_60);
    uVar7 = (ulong)*(byte *)(param_1 + 0x20);
    if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
      if (plVar5 != (long *)0x0) {
        uVar7 = plVar5[2];
        plVar5[2] = uVar7 + 1;
      }
      FUN_04333598(uVar7,param_1 + 0x10,plVar1,plVar5);
      uVar7 = *(ulong *)(param_1 + 0x30);
      if (uVar7 == *(ulong *)(param_1 + 0x38)) {
        uVar2 = uVar7 + 1;
        if (uVar2 >> 0x3c != 0) {
                    /* WARNING: Subroutine does not return */
          FUN_040416c0();
        }
        uVar9 = uVar7 * 2;
        if (uVar9 < uVar2 || uVar9 - uVar2 == 0) {
          uVar9 = uVar2;
        }
        if (0x7fffffffffffffe < uVar7) {
          uVar9 = 0xfffffffffffffff;
        }
        if (uVar9 == 0) {
          lVar3 = 0;
        }
        else {
          if (uVar9 >> 0x3c != 0) {
                    /* WARNING: Subroutine does not return */
            FUN_04b087f0();
          }
          lVar3 = FUN_00008790(uVar9 << 4);
        }
        plVar12 = (long *)(lVar3 + uVar7 * 0x10);
        if (plVar12 == (long *)0x0) goto LAB_0443513c;
        *plVar12 = (long)plVar1;
        plVar12[1] = (long)plVar5;
        if (plVar5 != (long *)0x0) {
          *plVar11 = *plVar11 + 1;
        }
        lVar3 = *(long *)(param_1 + 0x30);
        sVar10 = lVar3 * 0x10;
        _memcpy(plVar12 + lVar3 * -2,*(void **)(param_1 + 0x28),sVar10);
        lVar6 = *(long *)(param_1 + 0x28);
        *(long **)(param_1 + 0x28) = plVar12 + lVar3 * -2;
        *(long *)(param_1 + 0x30) = (long)(sVar10 + 0x10) >> 4;
        lVar3 = *(long *)(param_1 + 0x38);
        *(ulong *)(param_1 + 0x38) = uVar9;
        if (lVar6 != 0) {
          FUN_00005810(lVar6,lVar3 << 4);
        }
      }
      else {
        plVar12 = (long *)(*(long *)(param_1 + 0x28) + uVar7 * 0x10);
        if (plVar12 == (long *)0x0) goto LAB_0443513c;
        *plVar12 = (long)plVar1;
        plVar12[1] = (long)plVar5;
        if (plVar5 != (long *)0x0) {
          *plVar11 = *plVar11 + 1;
        }
      }
      lVar3 = (long)plVar12 + (0x10 - *(long *)(param_1 + 0x28));
      *(long *)(param_1 + 0x30) = lVar3 >> 4;
      if (lVar3 != 0) {
        if (plVar5 != (long *)0x0) {
          LOAcquire();
          lVar3 = *plVar11;
          *plVar11 = lVar3 + -1;
          LORelease();
          if (lVar3 == 0) {
            (**(code **)(*plVar5 + 0x10))(plVar5);
            if (plVar5[2] != 0) {
              LOAcquire();
              lVar3 = plVar5[2];
              plVar5[2] = lVar3 + -1;
              LORelease();
              if (lVar3 != 0) {
                return;
              }
            }
            pcVar8 = *(code **)(*plVar5 + 0x20);
            if (((pcVar8 == FUN_00f01bc0) || (pcVar8 == FUN_00f01bc0)) || (pcVar8 == FUN_013efb40))
            {
              FUN_00005810(plVar5,0x20);
            }
            else {
              (*pcVar8)(plVar5);
            }
          }
        }
        return;
      }
    }
  }
LAB_0443513c:
                    /* WARNING: Does not return */
  pcVar8 = (code *)SoftwareBreakpoint(1,0x4435140);
  (*pcVar8)();
}


