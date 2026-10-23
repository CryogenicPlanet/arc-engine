// ==== 0x440d9f8  ArcURLLoadConfiguration::headers  (252 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcURLLoadConfiguration::headers(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  char cVar3;
  ID IVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  long *plVar7;
  
  IVar4 = NSMutableDictionary::dictionary((ID)&_OBJC_CLASS___NSMutableDictionary,"dictionary");
  for (plVar7 = *(long **)(param_1 + 0x78); plVar7 != (long *)0x0; plVar7 = (long *)*plVar7) {
    cVar3 = *(char *)((long)plVar7 + 0x3f);
    plVar1 = (long *)plVar7[5];
    if (-1 < (long)cVar3) {
      plVar1 = plVar7 + 5;
    }
    lVar2 = plVar7[6];
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    uVar5 = FUN_01c391bc(plVar1,lVar2);
    cVar3 = *(char *)((long)plVar7 + 0x27);
    plVar1 = (long *)plVar7[2];
    if (-1 < (long)cVar3) {
      plVar1 = plVar7 + 2;
    }
    lVar2 = plVar7[3];
    if (-1 < cVar3) {
      lVar2 = (long)cVar3;
    }
    uVar6 = FUN_01c391bc(plVar1,lVar2);
    NSMutableDictionary::setValue_forKey_(IVar4,"setValue:forKey:",uVar5,uVar6);
  }
  IVar4 = NSMutableDictionary::copy(IVar4,"copy");
  return IVar4;
}


