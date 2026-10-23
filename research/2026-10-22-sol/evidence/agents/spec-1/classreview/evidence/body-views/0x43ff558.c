// ==== 0x43ff558  ArcAnnotatedVisit::relatedSearches  (228 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAnnotatedVisit::relatedSearches(ID param_1,SEL param_2)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID self;
  undefined8 uVar5;
  undefined *puVar6;
  long lVar7;
  undefined8 *puVar8;
  
  puVar6 = PTR____NSArray0__struct_0d824938;
  if (*(char *)(param_1 + 0x10) != '\0') {
    self = _objc_alloc_init(&_OBJC_CLASS___NSMutableArray);
    if ((*(byte *)(param_1 + 0x10) & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar4 = (code *)SoftwareBreakpoint(1,0x43ff63c);
      (*pcVar4)();
    }
    lVar7 = *(long *)(*(long *)(param_1 + 8) + 0xe0) * 0x18;
    if (lVar7 != 0) {
      puVar8 = *(undefined8 **)(*(long *)(param_1 + 8) + 0xd8);
      do {
        cVar3 = *(char *)((long)puVar8 + 0x17);
        puVar1 = (undefined8 *)*puVar8;
        if (-1 < (long)cVar3) {
          puVar1 = puVar8;
        }
        lVar2 = puVar8[1];
        if (-1 < cVar3) {
          lVar2 = (long)cVar3;
        }
        uVar5 = FUN_01c391bc(puVar1,lVar2);
        NSMutableArray::addObject_(self,"addObject:",uVar5);
        puVar8 = puVar8 + 3;
        lVar7 = lVar7 + -0x18;
      } while (lVar7 != 0);
    }
    puVar6 = (undefined *)NSMutableArray::copy(self,"copy");
  }
  return (ID)puVar6;
}


