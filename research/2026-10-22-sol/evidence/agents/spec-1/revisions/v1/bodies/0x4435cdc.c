// ==== 0x4435cdc  ArcBrowserContextPageContentExtraction::initWithBrowserContextPageContentExtraction:forBrowserContext:  (336 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_storeWeak replaced with injection: _objc_store_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcBrowserContextPageContentExtraction::
   initWithBrowserContextPageContentExtraction_forBrowserContext_
             (ID param_1,SEL param_2,const_void__ param_3,ID param_4)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long *plVar4;
  ID local_50;
  class_t *pcStack_48;
  
  pcStack_48 = &objc::class_t::ArcBrowserContextPageContentExtraction;
  local_50 = param_1;
  IVar2 = NSObject::init((ID)&local_50,"init");
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x20) == '\0') goto LAB_04435d50;
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
        if (lVar1 != 0) goto LAB_04435d4c;
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
LAB_04435d4c:
  *(undefined1 *)(IVar2 + 0x20) = 0;
LAB_04435d50:
  *(undefined8 *)(IVar2 + 0x10) = *(undefined8 *)param_3;
  lVar1 = *(long *)((long)param_3 + 8);
  *(long *)(IVar2 + 0x18) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
  }
  *(undefined1 *)(IVar2 + 0x20) = 1;
  *(ID *)(IVar2 + 8) = param_4;
  return IVar2;
}


