// ==== 0x440b464  ArcSessionWindow::tabGroups  (640 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcSessionWindow::tabGroups(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  long lVar4;
  ID self;
  ID IVar5;
  __shared_weak_count *in_x3;
  code *pcVar6;
  long *plVar7;
  long *plVar8;
  long lVar9;
  SessionTabGroup SVar10;
  long local_90;
  long *plStack_88;
  long local_78;
  long local_70;
  long local_68;
  
  if ((*(byte *)(param_1 + 0x18) & 1) == 0) {
LAB_0440b6e0:
                    /* WARNING: Does not return */
    pcVar6 = (code *)SoftwareBreakpoint(1,0x440b6e4);
    (*pcVar6)();
  }
  local_78 = 0;
  local_70 = 0;
  local_68 = 0;
  FUN_043413d0(&local_78,*(undefined8 *)(param_1 + 8));
  self = NSMutableArray::arrayWithCapacity_
                   ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",local_70);
  lVar9 = local_70 << 4;
  if (lVar9 != 0) {
    plVar7 = (long *)(local_78 + 8);
    do {
      IVar5 = _objc_alloc(&objc::class_t::ArcSessionTabGroup);
      plStack_88 = (long *)*plVar7;
      local_90 = plVar7[-1];
      if (*plVar7 != 0) {
        plVar1 = (long *)(*plVar7 + 8);
        *plVar1 = *plVar1 + 1;
      }
      SVar10.field0_0x0.field1_0x8 = in_x3;
      SVar10.field0_0x0.field0_0x0 = (Impl *)&local_90;
      IVar5 = ArcSessionTabGroup::initWithSessionTabGroup_(IVar5,"initWithSessionTabGroup:",SVar10);
      NSMutableArray::addObject_(self,"addObject:",IVar5);
      plVar1 = plStack_88;
      if (plStack_88 != (long *)0x0) {
        LOAcquire();
        lVar4 = plStack_88[1];
        plStack_88[1] = lVar4 + -1;
        LORelease();
        if (lVar4 == 0) {
          plVar8 = plStack_88 + 2;
          (**(code **)(*plStack_88 + 0x10))(plStack_88);
          if (*plVar8 != 0) {
            LOAcquire();
            lVar4 = plVar1[2];
            plVar1[2] = lVar4 + -1;
            LORelease();
            if (lVar4 != 0) goto LAB_0440b608;
          }
          pcVar6 = *(code **)(*plVar1 + 0x20);
          if (((pcVar6 == FUN_00f01bc0) || (pcVar6 == FUN_00f01bc0)) || (pcVar6 == FUN_013efb40)) {
            FUN_00005810(plVar1,0x20);
          }
          else {
            (*pcVar6)(plVar1);
          }
        }
      }
LAB_0440b608:
      plVar7 = plVar7 + 2;
      lVar9 = lVar9 + -0x10;
    } while (lVar9 != 0);
  }
  lVar9 = local_78;
  if (local_78 != 0) {
    if (local_70 * 0x10 != 0) {
      lVar4 = local_70 * -0x10;
      lVar2 = local_70 * 0x10 + local_78;
      do {
        if (lVar2 + -0x10 == 0) goto LAB_0440b6e0;
        plVar7 = *(long **)(lVar2 + -8);
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
              if (lVar3 != 0) goto LAB_0440b580;
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
LAB_0440b580:
        lVar4 = lVar4 + 0x10;
        lVar2 = lVar2 + -0x10;
      } while (lVar4 != 0);
    }
    local_70 = lVar9 - local_78 >> 4;
    FUN_00005810(local_78,local_68 << 4);
  }
  return self;
}


