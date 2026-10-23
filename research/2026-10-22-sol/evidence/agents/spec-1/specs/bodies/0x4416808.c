// ==== 0x4416808  ArcNavigation::init:  (480 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

ID ArcNavigation::init_(ID param_1,SEL param_2,Navigation param_3)

{
  long lVar1;
  ID IVar2;
  ID self;
  undefined8 uVar3;
  undefined8 uVar4;
  Impl *pIVar5;
  code *pcVar6;
  long *plVar7;
  long lVar8;
  ID local_50;
  class_t *pcStack_48;
  
  pIVar5 = param_3.field0_0x0.field0_0x0;
  pcStack_48 = &objc::class_t::ArcNavigation;
  local_50 = param_1;
  IVar2 = NSObject::init((ID)&local_50,"init",pIVar5,param_3.field0_0x0.field1_0x8);
  if (IVar2 == 0) {
    return 0;
  }
  if (*(char *)(IVar2 + 0x20) == '\0') goto LAB_0441686c;
  plVar7 = *(long **)(IVar2 + 0x18);
  if (plVar7 != (long *)0x0) {
    LOAcquire();
    lVar1 = plVar7[1];
    plVar7[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plVar7 + 0x10))(plVar7);
      if (plVar7[2] != 0) {
        LOAcquire();
        lVar1 = plVar7[2];
        plVar7[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04416868;
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
LAB_04416868:
  *(undefined1 *)(IVar2 + 0x20) = 0;
LAB_0441686c:
  lVar8 = *(long *)pIVar5;
  *(long *)(IVar2 + 0x10) = lVar8;
  lVar1 = *(long *)(pIVar5 + 8);
  *(long *)(IVar2 + 0x18) = lVar1;
  if (lVar1 != 0) {
    *(long *)(lVar1 + 8) = *(long *)(lVar1 + 8) + 1;
    lVar8 = *(long *)(IVar2 + 0x10);
  }
  *(undefined1 *)(IVar2 + 0x20) = 1;
  if (*(long *)(lVar8 + 0x38) != 0) {
    self = NSMutableDictionary::dictionaryWithCapacity_
                     ((ID)&_OBJC_CLASS___NSMutableDictionary,"dictionaryWithCapacity:");
    for (plVar7 = *(long **)(lVar8 + 0x30); plVar7 != (long *)0x0; plVar7 = (long *)*plVar7) {
      lVar1 = (long)*(char *)((long)plVar7 + 0x3f);
      if (lVar1 < 0) {
        lVar8 = plVar7[5];
        lVar1 = plVar7[6];
      }
      else {
        lVar8 = (long)(plVar7 + 5);
      }
      uVar3 = FUN_01c391bc(lVar8,lVar1);
      lVar1 = (long)*(char *)((long)plVar7 + 0x27);
      if (lVar1 < 0) {
        lVar8 = plVar7[2];
        lVar1 = plVar7[3];
      }
      else {
        lVar8 = (long)(plVar7 + 2);
      }
      uVar4 = FUN_01c391bc(lVar8,lVar1);
      NSMutableDictionary::setObject_forKeyedSubscript_
                (self,"setObject:forKeyedSubscript:",uVar3,uVar4);
    }
    *(ID *)(IVar2 + 8) = self;
  }
  return IVar2;
}


