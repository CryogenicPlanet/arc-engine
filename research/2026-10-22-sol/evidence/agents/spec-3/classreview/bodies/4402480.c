// ==== 0x4402480  objc::Categories::ArcCommandBarRankingTrainingResult_Extensions_::delta  (200 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcCommandBarRankingTrainingResult_Extensions_::delta(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID self;
  undefined8 uVar2;
  undefined1 auStack_b8 [8];
  long local_b0;
  long local_a8;
  byte local_48;
  
  _objc_msgSend(param_1,"command_bar_ranking_training_result",auStack_b8);
  if ((local_48 & 1) != 0) {
    FUN_042a43ac(auStack_b8);
    self = NSMutableArray::arrayWithCapacity_
                     ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",local_a8);
    for (local_a8 = local_a8 * 0x48; local_a8 != 0; local_a8 = local_a8 + -0x48) {
      uVar2 = FUN_044022d8(local_b0);
      NSMutableArray::addObject_(self,"addObject:",uVar2);
      local_b0 = local_b0 + 0x48;
    }
    return self;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4402548);
  (*pcVar1)();
}


