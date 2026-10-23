// ==== 0x440abb4  ArcSessionID::initWithSessionId:  (96 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcSessionID::initWithSessionId_(ID param_1,SEL param_2,const_SessionID__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcSessionID;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0xc) != '\0') {
      *(undefined1 *)(IVar1 + 0xc) = 0;
    }
    *(int *)(IVar1 + 8) = param_3->field0_0x0;
    *(undefined1 *)(IVar1 + 0xc) = 1;
  }
  return IVar1;
}


