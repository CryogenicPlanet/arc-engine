// ==== 0x440af50  ArcSessionWindow::initWithSessionWindow:  (292 bytes)

/* Function Stack Size: 0x20 bytes */

ID ArcSessionWindow::initWithSessionWindow_(ID param_1,SEL param_2,SessionWindow param_3)

{
  long lVar1;
  ID IVar2;
  Impl *pIVar3;
  code *pcVar4;
  long *plVar5;
  ID local_40;
  class_t *pcStack_38;
  
  pIVar3 = param_3.field0_0x0.field0_0x0;
  pcStack_38 = &objc::class_t::ArcSessionWindow;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init",pIVar3,param_3.field0_0x0.field1_0x8);
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x18) == '\0') goto LAB_0440afb0;
  plVar5 = *(long **)(IVar2 + 0x10);
  if (plVar5 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar5[1];
    plVar5[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar5 + 0x10))(plVar5);
      if (plVar5[2] != 0) {
        LOAcquire();
        lVar1 = plVar5[2];
        plVar5[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_0440afac;
      }
      pcVar4 = *(code **)(*plVar5 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar5,0x20);
      }
      else {
        (*pcVar4)(plVar5);
      }
    }
  }
LAB_0440afac:
  *(undefined1 *)(IVar2 + 0x18) = 0;
LAB_0440afb0:
  *(undefined8 *)(IVar2 + 8) = *(undefined8 *)pIVar3;
  lVar1 = *(long *)(pIVar3 + 8);
  *(long *)(IVar2 + 0x10) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x18) = 1;
  return IVar2;
}


