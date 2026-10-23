// ==== 0x43deae8  ArcAccessibilityActionData::initWithAction:  (84 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcAccessibilityActionData::initWithAction_(ID param_1,SEL param_2,unsigned_long_long param_3)

{
  code *pcVar1;
  ID IVar2;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcAccessibilityActionData;
  local_30 = param_1;
  IVar2 = NSObject::init((ID)&local_30,"init");
  if (IVar2 != 0) {
    if (3 < param_3) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x43deb3c);
      (*pcVar1)();
    }
    *(int *)(IVar2 + 8) = (int)param_3;
  }
  return IVar2;
}


