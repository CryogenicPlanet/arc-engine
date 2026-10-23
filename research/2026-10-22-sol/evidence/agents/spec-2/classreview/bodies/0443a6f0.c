// ==== 0x443a6f0  ArcAIDistilBertClassifier::initWithAIDistilBertClassifier:  (116 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcAIDistilBertClassifier::initWithAIDistilBertClassifier_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcAIDistilBertClassifier;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x70) != '\0') {
      FUN_042a4a04(IVar1 + 8);
      *(undefined1 *)(IVar1 + 0x70) = 0;
    }
    FUN_0443a9fc(IVar1 + 8,param_3);
    *(undefined1 *)(IVar1 + 0x70) = 1;
  }
  return IVar1;
}


