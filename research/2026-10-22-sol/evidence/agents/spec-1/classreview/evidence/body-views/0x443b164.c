// ==== 0x443b164  ArcColor::initWithColor:  (76 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcColor::initWithColor_(ID param_1,SEL param_2,const_Color__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcColor;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    *(Color *)(IVar1 + 8) = *param_3;
  }
  return IVar1;
}


