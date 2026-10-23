// ==== 0x44395e0  ArcAutocompleteMatch::keyword  (164 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::keyword(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    lVar4 = *(long *)(param_1 + 0x18);
    if (*(char *)(lVar4 + 0x44f) < '\0') {
      FUN_00054420(&local_40,*(undefined8 *)(lVar4 + 0x438),*(undefined8 *)(lVar4 + 0x440));
    }
    else {
      lStack_38 = *(long *)(lVar4 + 0x440);
      local_40 = *(undefined1 **)(lVar4 + 0x438);
      local_30 = *(long *)(lVar4 + 0x448);
    }
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)local_30._7_1_) {
      ppuVar1 = &local_40;
    }
    lVar4 = lStack_38;
    if (-1 < local_30) {
      lVar4 = (long)local_30._7_1_;
    }
    IVar3 = FUN_01d9fb90(ppuVar1,lVar4);
    if (local_30 < 0) {
      FUN_00005810(local_40,local_30 << 1);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4439684);
  (*pcVar2)();
}


