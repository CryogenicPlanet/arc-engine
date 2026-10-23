// ==== 0x4434190  ArcCommandBarRankingEvaluation::metrics  (244 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcCommandBarRankingEvaluation::metrics(ID param_1,SEL param_2)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID self;
  ID IVar5;
  undefined8 uVar6;
  long lVar7;
  undefined8 *puVar8;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    self = NSMutableDictionary::dictionaryWithCapacity_
                     ((ID)&_OBJC_CLASS___NSMutableDictionary,"dictionaryWithCapacity:",
                      *(undefined8 *)(param_1 + 0x10));
    lVar7 = *(long *)(param_1 + 0x10) << 5;
    if (lVar7 != 0) {
      puVar8 = *(undefined8 **)(param_1 + 8);
      do {
        IVar5 = NSNumber::numberWithDouble_
                          ((ID)&_OBJC_CLASS___NSNumber,"numberWithDouble:",puVar8[3]);
        cVar3 = *(char *)((long)puVar8 + 0x17);
        puVar1 = (undefined8 *)*puVar8;
        if (-1 < (long)cVar3) {
          puVar1 = puVar8;
        }
        lVar2 = puVar8[1];
        if (-1 < cVar3) {
          lVar2 = (long)cVar3;
        }
        uVar6 = FUN_01c391bc(puVar1,lVar2);
        NSMutableDictionary::setObject_forKeyedSubscript_
                  (self,"setObject:forKeyedSubscript:",IVar5,uVar6);
        puVar8 = puVar8 + 4;
        lVar7 = lVar7 + -0x20;
      } while (lVar7 != 0);
    }
    return self;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x4434284);
  (*pcVar4)();
}


