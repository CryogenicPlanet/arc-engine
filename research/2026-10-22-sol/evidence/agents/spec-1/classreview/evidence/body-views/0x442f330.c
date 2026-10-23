// ==== 0x442f330  ArcExtensionInfo::permissions  (192 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcExtensionInfo::permissions(ID param_1,SEL param_2)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  code *pcVar4;
  ID self;
  undefined8 uVar5;
  long lVar6;
  undefined8 *puVar7;
  
  if ((*(byte *)(param_1 + 0xe0) & 1) != 0) {
    self = NSMutableArray::arrayWithCapacity_
                     ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",
                      *(undefined8 *)(param_1 + 0x70));
    lVar6 = *(long *)(param_1 + 0x70) * 0x18;
    if (lVar6 != 0) {
      puVar7 = *(undefined8 **)(param_1 + 0x68);
      do {
        cVar3 = *(char *)((long)puVar7 + 0x17);
        puVar1 = (undefined8 *)*puVar7;
        if (-1 < (long)cVar3) {
          puVar1 = puVar7;
        }
        lVar2 = puVar7[1];
        if (-1 < cVar3) {
          lVar2 = (long)cVar3;
        }
        uVar5 = FUN_01d9fb90(puVar1,lVar2);
        NSMutableArray::addObject_(self,"addObject:",uVar5);
        puVar7 = puVar7 + 3;
        lVar6 = lVar6 + -0x18;
      } while (lVar6 != 0);
    }
    return self;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(1,0x442f3f0);
  (*pcVar4)();
}


