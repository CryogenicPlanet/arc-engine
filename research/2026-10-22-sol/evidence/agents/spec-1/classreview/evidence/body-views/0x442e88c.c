// ==== 0x442e88c  ArcHistoryDeletion::deletedVisitIds  (192 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcHistoryDeletion::deletedVisitIds(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID self;
  ID IVar2;
  long lVar3;
  undefined8 *puVar4;
  
  if ((*(byte *)(param_1 + 0x88) & 1) != 0) {
    self = NSMutableArray::arrayWithCapacity_
                     ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",
                      *(undefined8 *)(param_1 + 0x78));
    lVar3 = *(long *)(param_1 + 0x78) << 3;
    if (lVar3 != 0) {
      puVar4 = *(undefined8 **)(param_1 + 0x70);
      do {
        IVar2 = NSNumber::numberWithLongLong_
                          ((ID)&_OBJC_CLASS___NSNumber,"numberWithLongLong:",*puVar4);
        NSMutableArray::addObject_(self,"addObject:",IVar2);
        lVar3 = lVar3 + -8;
        puVar4 = puVar4 + 1;
      } while (lVar3 != 0);
    }
    return self;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442e94c);
  (*pcVar1)();
}


