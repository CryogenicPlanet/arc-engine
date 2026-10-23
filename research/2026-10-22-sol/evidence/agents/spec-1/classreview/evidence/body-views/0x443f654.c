// ==== 0x443f654  ArcContextMenuElementInfo::dictionarySuggestions  (340 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcContextMenuElementInfo::dictionarySuggestions(ID param_1,SEL param_2)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  undefined8 *puVar4;
  code *pcVar5;
  ID self;
  undefined8 uVar6;
  undefined8 *puVar7;
  long lVar8;
  undefined8 *local_58;
  long local_50;
  long local_48;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    FUN_043342a4(&local_58,param_1 + 8);
    self = NSMutableArray::arrayWithCapacity_
                     ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",local_50);
    puVar4 = local_58;
    puVar1 = local_58;
    for (lVar8 = local_50 * 0x18; local_58 = puVar1, lVar8 != 0; lVar8 = lVar8 + -0x18) {
      cVar3 = *(char *)((long)puVar4 + 0x17);
      puVar1 = (undefined8 *)*puVar4;
      if (-1 < (long)cVar3) {
        puVar1 = puVar4;
      }
      lVar2 = puVar4[1];
      if (-1 < cVar3) {
        lVar2 = (long)cVar3;
      }
      uVar6 = FUN_01d9fb90(puVar1,lVar2);
      NSMutableArray::addObject_(self,"addObject:",uVar6);
      puVar4 = puVar4 + 3;
      puVar1 = local_58;
    }
    if (puVar1 != (undefined8 *)0x0) {
      if (local_50 * 0x18 != 0) {
        lVar8 = local_50 * -0x18;
        puVar4 = puVar1 + local_50 * 3;
        do {
          puVar7 = puVar4 + -3;
          if (puVar7 == (undefined8 *)0x0) goto LAB_0443f724;
          if (*(char *)((long)puVar4 + -1) < '\0') {
            FUN_00005810(*puVar7,puVar4[-1] << 1);
          }
          lVar8 = lVar8 + 0x18;
          puVar4 = puVar7;
        } while (lVar8 != 0);
      }
      local_50 = ((long)puVar1 - (long)local_58 >> 3) * -0x5555555555555555;
      FUN_00005810(local_58,local_48 * 0x18);
    }
    return self;
  }
LAB_0443f724:
                    /* WARNING: Does not return */
  pcVar5 = (code *)SoftwareBreakpoint(1,0x443f728);
  (*pcVar5)();
}


