// ==== 0x442f548  ArcDownloadItem::initWithDownloadItem:  (292 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcDownloadItem::initWithDownloadItem_(ID param_1,SEL param_2,const_void__ param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_40;
  class_t *pcStack_38;
  
  pcStack_38 = &objc::class_t::ArcDownloadItem;
  local_40 = param_1;
  IVar2 = NSObject::init((ID)&local_40,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x20) == '\0') goto LAB_0442f5a8;
  plVar4 = *(long **)(IVar2 + 0x18);
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
        if (lVar1 != 0) goto LAB_0442f5a4;
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
LAB_0442f5a4:
  *(undefined1 *)(IVar2 + 0x20) = 0;
LAB_0442f5a8:
  *(undefined8 *)(IVar2 + 0x10) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x18) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x20) = 1;
  return IVar2;
}


