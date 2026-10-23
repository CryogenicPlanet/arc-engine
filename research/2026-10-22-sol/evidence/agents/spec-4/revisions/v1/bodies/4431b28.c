// ==== 0x4431b28  ArcCommandBarRankingTrainingResult::lossesPerEpoch  (192 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcCommandBarRankingTrainingResult::lossesPerEpoch(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID self;
  ID IVar2;
  long lVar3;
  undefined4 *puVar4;
  
  if ((*(byte *)(param_1 + 0x78) & 1) != 0) {
    self = NSMutableArray::arrayWithCapacity_
                     ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",
                      *(undefined8 *)(param_1 + 0x50));
    lVar3 = *(long *)(param_1 + 0x50) << 2;
    if (lVar3 != 0) {
      puVar4 = *(undefined4 **)(param_1 + 0x48);
      do {
        IVar2 = NSNumber::numberWithFloat_((ID)&_OBJC_CLASS___NSNumber,"numberWithFloat:",*puVar4);
        NSMutableArray::addObject_(self,"addObject:",IVar2);
        lVar3 = lVar3 + -4;
        puVar4 = puVar4 + 1;
      } while (lVar3 != 0);
    }
    return self;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4431be8);
  (*pcVar1)();
}


