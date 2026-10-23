// ==== 0x442cc28  ArcSearchEngine::faviconUrl  (184 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcSearchEngine::faviconUrl(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  long lVar2;
  code *pcVar3;
  undefined8 *puVar4;
  ID IVar5;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    if (*(char *)(*(long *)(param_1 + 8) + 0x228) == '\0') {
      local_40 = (undefined1 *)0x0;
      lStack_38 = 0;
      local_30 = 0;
    }
    else {
      puVar4 = (undefined8 *)FUN_007cf864(*(long *)(param_1 + 8) + 0x210);
      if (*(char *)((long)puVar4 + 0x17) < '\0') {
        FUN_00060a60(&local_40,*puVar4,puVar4[1]);
      }
      else {
        lStack_38 = puVar4[1];
        local_40 = (undefined1 *)*puVar4;
        local_30 = puVar4[2];
      }
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar2 = lStack_38;
    if (-1 < (long)local_30) {
      lVar2 = (long)local_30._7_1_;
    }
    IVar5 = FUN_01c391bc(ppuVar1,lVar2);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar5;
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x442cce0);
  (*pcVar3)();
}


