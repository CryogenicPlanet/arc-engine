// ==== 0x4430960  ArcContentNode::initWithContentNode:  (116 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcContentNode::initWithContentNode_(ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcContentNode;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x1b8) != '\0') {
      FUN_04295208(IVar1 + 0x18);
      *(undefined1 *)(IVar1 + 0x1b8) = 0;
    }
    FUN_0429539c(IVar1 + 0x18,param_3);
    *(undefined1 *)(IVar1 + 0x1b8) = 1;
  }
  return IVar1;
}


