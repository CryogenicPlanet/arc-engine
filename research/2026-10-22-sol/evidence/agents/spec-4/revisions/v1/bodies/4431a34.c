// ==== 0x4431a34  ArcCommandBarRankingTrainingResult::initWithCommandBarRankingTrainingResult:  (116 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcCommandBarRankingTrainingResult::initWithCommandBarRankingTrainingResult_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcCommandBarRankingTrainingResult;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0x78) != '\0') {
      FUN_042a43ac(IVar1 + 8);
      *(undefined1 *)(IVar1 + 0x78) = 0;
    }
    FUN_043dc67c(IVar1 + 8,param_3);
    *(undefined1 *)(IVar1 + 0x78) = 1;
  }
  return IVar1;
}


