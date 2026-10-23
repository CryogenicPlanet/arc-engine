// ==== 0x44013f0  ArcBrowserSidePanel::initWithSidePanel:  (268 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcBrowserSidePanel::initWithSidePanel_(ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcBrowserSidePanel;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 != 0) {
    uVar6 = *(undefined8 *)((long)param_3 + 8);
    uVar5 = *(undefined8 *)param_3;
    if (*(long *)((long)param_3 + 8) != 0) {
      plVar4 = (long *)(*(long *)((long)param_3 + 8) + 8);
      *plVar4 = *plVar4 + 1;
    }
    plVar4 = *(long **)(IVar2 + 0x10);
    *(undefined8 *)(IVar2 + 0x10) = uVar6;
    *(undefined8 *)(IVar2 + 8) = uVar5;
    if (plVar4 != (long *)0x0) {
      LOAcquire();
      lVar1 = plVar4[1];
      plVar4[1] = lVar1 + -1;
      LORelease();
      if (lVar1 == 0) {
        (**(code **)(*plVar4 + 0x10))(plVar4);
        if (plVar4[2] != 0) {
          LOAcquire();
          lVar1 = plVar4[2];
          plVar4[2] = lVar1 + -1;
          LORelease();
          if (lVar1 != 0) {
            return IVar2;
          }
        }
        pcVar3 = *(code **)(*plVar4 + 0x20);
        if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
          FUN_00005810(plVar4,0x20);
        }
        else {
          (*pcVar3)(plVar4);
        }
      }
    }
  }
  return IVar2;
}


