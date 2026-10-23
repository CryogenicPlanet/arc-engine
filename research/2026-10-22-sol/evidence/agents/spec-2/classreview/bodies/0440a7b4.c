// ==== 0x440a7b4  ArcOverscrollBehavior::init  (60 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcOverscrollBehavior::init(ID param_1,SEL param_2)

{
  ID IVar1;
  ID local_20;
  class_t *pcStack_18;
  
  pcStack_18 = &objc::class_t::ArcOverscrollBehavior;
  local_20 = param_1;
  IVar1 = NSObject::init((ID)&local_20,"init");
  if (IVar1 != 0) {
    *(undefined8 *)(IVar1 + 8) = 0;
    *(undefined8 *)(IVar1 + 0x10) = 0;
  }
  return IVar1;
}


