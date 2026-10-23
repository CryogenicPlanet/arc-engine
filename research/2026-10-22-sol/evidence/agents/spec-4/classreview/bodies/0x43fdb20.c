// ==== 0x43fdb20  ArcBrowserContextExtensions::getPinnedExtensionOrder  (384 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowserContextExtensions::getPinnedExtensionOrder(ID param_1,SEL param_2)

{
  undefined8 *puVar1;
  undefined8 *puVar2;
  long lVar3;
  char cVar4;
  code *pcVar5;
  ID IVar6;
  undefined8 uVar7;
  undefined8 *puVar8;
  long lVar9;
  undefined8 *local_58;
  long local_50;
  long local_48;
  
  if ((*(byte *)(param_1 + 0x30) & 1) != 0) {
    local_58 = (undefined8 *)0x0;
    local_50 = 0;
    local_48 = 0;
    FUN_042eb484(&local_58,*(undefined8 *)(param_1 + 0x20));
    IVar6 = NSMutableArray::arrayWithCapacity_
                      ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",local_50);
    puVar1 = local_58;
    for (lVar9 = local_50 * 0x18; lVar9 != 0; lVar9 = lVar9 + -0x18) {
      cVar4 = *(char *)((long)puVar1 + 0x17);
      puVar2 = (undefined8 *)*puVar1;
      if (-1 < (long)cVar4) {
        puVar2 = puVar1;
      }
      lVar3 = puVar1[1];
      if (-1 < cVar4) {
        lVar3 = (long)cVar4;
      }
      uVar7 = FUN_01c391bc(puVar2,lVar3);
      NSMutableArray::addObject_(IVar6,"addObject:",uVar7);
      puVar1 = puVar1 + 3;
    }
    IVar6 = NSMutableArray::copy(IVar6,"copy");
    puVar1 = local_58;
    if (local_58 != (undefined8 *)0x0) {
      if (local_50 * 0x18 != 0) {
        lVar9 = local_50 * -0x18;
        puVar2 = local_58 + local_50 * 3;
        do {
          puVar8 = puVar2 + -3;
          if (puVar8 == (undefined8 *)0x0) goto LAB_043fdc94;
          if (*(char *)((long)puVar2 + -1) < '\0') {
            FUN_00005810(*puVar8,puVar2[-1] & 0x7fffffffffffffff);
          }
          lVar9 = lVar9 + 0x18;
          puVar2 = puVar8;
        } while (lVar9 != 0);
      }
      local_50 = ((long)puVar1 - (long)local_58 >> 3) * -0x5555555555555555;
      FUN_00005810(local_58,local_48 * 0x18);
    }
    return IVar6;
  }
LAB_043fdc94:
                    /* WARNING: Does not return */
  pcVar5 = (code *)SoftwareBreakpoint(1,0x43fdc98);
  (*pcVar5)();
}


