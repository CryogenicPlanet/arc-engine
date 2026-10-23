// ==== 0x440e120  ArcFindResult::initWithFindResult:  (84 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcFindResult::initWithFindResult_(ID param_1,SEL param_2,const_FindResult__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcFindResult;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    *(long *)(IVar1 + 8) = (long)(int)param_3->field0_0x0;
    *(long *)(IVar1 + 0x10) = (long)(int)param_3->field1_0x4;
  }
  return IVar1;
}


