// ==== 0x44438cc  ArcWebContents::supportedTranslateLanguages  (380 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcWebContents::supportedTranslateLanguages(ID param_1,SEL param_2)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  undefined8 *puVar4;
  code *pcVar5;
  ID self;
  undefined8 uVar6;
  long lVar7;
  undefined8 *puVar8;
  undefined8 *local_58;
  long local_50;
  long local_48;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    lVar7 = *(long *)(*(long *)(param_1 + 0x118) + 0x130);
    if (lVar7 == 0) {
      local_58 = (undefined8 *)0x0;
      local_50 = 0;
      local_48 = 0;
    }
    else {
      FUN_000805e0(lVar7 + 8,&DAT_0c9d7100);
      FUN_0434e3c0(&local_58);
    }
    self = NSMutableArray::arrayWithCapacity_
                     ((ID)&_OBJC_CLASS___NSMutableArray,"arrayWithCapacity:",local_50);
    puVar4 = local_58;
    puVar1 = local_58;
    for (lVar7 = local_50 * 0x18; local_58 = puVar1, lVar7 != 0; lVar7 = lVar7 + -0x18) {
      cVar3 = *(char *)((long)puVar4 + 0x17);
      puVar1 = (undefined8 *)*puVar4;
      if (-1 < (long)cVar3) {
        puVar1 = puVar4;
      }
      lVar2 = puVar4[1];
      if (-1 < cVar3) {
        lVar2 = (long)cVar3;
      }
      uVar6 = FUN_01c391bc(puVar1,lVar2);
      NSMutableArray::addObject_(self,"addObject:",uVar6);
      puVar4 = puVar4 + 3;
      puVar1 = local_58;
    }
    if (puVar1 != (undefined8 *)0x0) {
      if (local_50 * 0x18 != 0) {
        lVar7 = local_50 * -0x18;
        puVar4 = puVar1 + local_50 * 3;
        do {
          puVar8 = puVar4 + -3;
          if (puVar8 == (undefined8 *)0x0) goto LAB_04443a44;
          if (*(char *)((long)puVar4 + -1) < '\0') {
            FUN_00005810(*puVar8,puVar4[-1] & 0x7fffffffffffffff);
          }
          lVar7 = lVar7 + 0x18;
          puVar4 = puVar8;
        } while (lVar7 != 0);
      }
      local_50 = ((long)puVar1 - (long)local_58 >> 3) * -0x5555555555555555;
      FUN_00005810(local_58,local_48 * 0x18);
    }
    return self;
  }
LAB_04443a44:
                    /* WARNING: Does not return */
  pcVar5 = (code *)SoftwareBreakpoint(1,0x4443a48);
  (*pcVar5)();
}


