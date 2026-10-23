// ==== 0x43df3bc  ArcImportSourceProfile::name  (156 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcImportSourceProfile::name(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  long lVar2;
  code *pcVar3;
  ID IVar4;
  undefined8 *puVar5;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    puVar5 = *(undefined8 **)(param_1 + 8);
    if (*(char *)((long)puVar5 + 0x17) < '\0') {
      FUN_00054420(&local_40,*puVar5,puVar5[1]);
    }
    else {
      lStack_38 = puVar5[1];
      local_40 = (undefined1 *)*puVar5;
      local_30 = puVar5[2];
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar2 = lStack_38;
    if (-1 < local_30) {
      lVar2 = (long)local_30._7_1_;
    }
    IVar4 = FUN_01d9fb90(ppuVar1,lVar2);
    if (local_30 < 0) {
      FUN_00005810(local_40,local_30 << 1);
    }
    return IVar4;
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x43df458);
  (*pcVar3)();
}


