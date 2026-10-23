// ==== 0x4434900  ArcCommandBarRankingCandidate::initWithCommandBarRankingCandidate:  (116 bytes)

/* Function Stack Size: 0x18 bytes */

ID ArcCommandBarRankingCandidate::initWithCommandBarRankingCandidate_
             (ID param_1,SEL param_2,const_void__ param_3)

{
  ID IVar1;
  ID local_30;
  class_t *pcStack_28;
  
  pcStack_28 = &objc::class_t::ArcCommandBarRankingCandidate;
  local_30 = param_1;
  IVar1 = NSObject::init((ID)&local_30,"init");
  if (IVar1 != 0) {
    if (*(char *)(IVar1 + 0xb0) != '\0') {
      FUN_042a3ff4(IVar1 + 8);
      *(undefined1 *)(IVar1 + 0xb0) = 0;
    }
    FUN_042a3ea8(IVar1 + 8,param_3);
    *(undefined1 *)(IVar1 + 0xb0) = 1;
  }
  return IVar1;
}


