// ==== 0x43f65a8  FUN_043f65a8  (448 bytes)

/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */

undefined8 FUN_043f65a8(long param_1,long *param_2,undefined8 param_3,__shared_weak_count *param_4)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  ID IVar4;
  code *pcVar5;
  long *plVar6;
  long lVar7;
  long *plVar8;
  SessionWindow SVar9;
  long local_70;
  long *plStack_68;
  
  IVar3 = _objc_alloc_init(&_OBJC_CLASS___NSMutableArray);
  lVar7 = param_2[1] << 4;
  if (lVar7 != 0) {
    plVar8 = (long *)(*param_2 + 8);
    do {
      IVar4 = _objc_alloc(&objc::class_t::ArcSessionWindow);
      plStack_68 = (long *)*plVar8;
      local_70 = plVar8[-1];
      if (*plVar8 != 0) {
        plVar1 = (long *)(*plVar8 + 8);
        *plVar1 = *plVar1 + 1;
      }
      SVar9.field0_0x0.field1_0x8 = param_4;
      SVar9.field0_0x0.field0_0x0 = (Impl *)&local_70;
      IVar4 = ArcSessionWindow::initWithSessionWindow_(IVar4,"initWithSessionWindow:",SVar9);
      NSMutableArray::addObject_(IVar3,"addObject:",IVar4);
      plVar1 = plStack_68;
      if (plStack_68 != (long *)0x0) {
        LOAcquire();
        lVar2 = plStack_68[1];
        plStack_68[1] = lVar2 + -1;
        LORelease();
        if (lVar2 == 0) {
          plVar6 = plStack_68 + 2;
          (**(code **)(*plStack_68 + 0x10))(plStack_68);
          if (*plVar6 != 0) {
            LOAcquire();
            lVar2 = plVar1[2];
            plVar1[2] = lVar2 + -1;
            LORelease();
            if (lVar2 != 0) goto LAB_043f6690;
          }
          pcVar5 = *(code **)(*plVar1 + 0x20);
          if (((pcVar5 == FUN_00f01bc0) || (pcVar5 == FUN_00f01bc0)) || (pcVar5 == FUN_013efb40)) {
            FUN_00005810(plVar1,0x20);
          }
          else {
            (*pcVar5)(plVar1);
          }
        }
      }
LAB_043f6690:
      plVar8 = plVar8 + 2;
      lVar7 = lVar7 + -0x10;
    } while (lVar7 != 0);
  }
  IVar4 = *(ID *)(param_1 + 8);
  IVar3 = NSMutableArray::copy(IVar3,"copy");
  _objc_msgSend(IVar4,"didImportSessionWindows:",IVar3);
  return 0;
}


