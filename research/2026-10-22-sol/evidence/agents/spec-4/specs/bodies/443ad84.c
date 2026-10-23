// ==== 0x443ad84  ArcBreadcrumb::data  (228 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBreadcrumb::data(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  ID self;
  undefined8 uVar4;
  undefined8 uVar5;
  long *plVar6;
  
  self = NSMutableDictionary::dictionaryWithCapacity_
                   ((ID)&_OBJC_CLASS___NSMutableDictionary,"dictionaryWithCapacity:",
                    *(undefined8 *)(param_1 + 0x50));
  for (plVar6 = *(long **)(param_1 + 0x48); plVar6 != (long *)0x0; plVar6 = (long *)*plVar6) {
    cVar3 = *(char *)((long)plVar6 + 0x3f);
    plVar1 = (long *)plVar6[5];
    if (-1 < (long)cVar3) {
      plVar1 = plVar6 + 5;
    }
    lVar2 = plVar6[6];
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    uVar4 = FUN_01c391bc(plVar1,lVar2);
    cVar3 = *(char *)((long)plVar6 + 0x27);
    plVar1 = (long *)plVar6[2];
    if (-1 < (long)cVar3) {
      plVar1 = plVar6 + 2;
    }
    lVar2 = plVar6[3];
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    uVar5 = FUN_01c391bc(plVar1,lVar2);
    NSMutableDictionary::setObject_forKeyedSubscript_
              (self,"setObject:forKeyedSubscript:",uVar4,uVar5);
  }
  return self;
}


