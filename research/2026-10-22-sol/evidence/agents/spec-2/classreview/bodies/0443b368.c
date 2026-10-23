// ==== 0x443b368  ArcConfirmInfobar::messageText  (160 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcConfirmInfobar::messageText(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  long lVar2;
  code *pcVar3;
  cfstringStruct *pcVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined8 local_30;
  undefined8 local_28;
  
  if ((*(byte *)(param_1 + 0x20) & 1) != 0) {
    lStack_38 = 0;
    local_40 = (undefined1 *)0x0;
    local_28 = 0;
    local_30 = 0;
    FUN_0430dbcc(&local_40,*(undefined8 *)(param_1 + 0x10));
    if ((char)local_28 == '\0') {
      pcVar4 = &cf___;
    }
    else {
      ppuVar1 = (undefined1 **)local_40;
      if (-1 < (long)local_30._7_1_) {
        ppuVar1 = &local_40;
      }
      lVar2 = lStack_38;
      if (-1 < local_30) {
        lVar2 = (long)local_30._7_1_;
      }
      pcVar4 = (cfstringStruct *)FUN_01d9fb90(ppuVar1,lVar2);
      if (((char)local_28 != '\0') && (local_30 < 0)) {
        FUN_00005810(local_40,local_30 << 1);
      }
    }
    return (ID)pcVar4;
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x443b408);
  (*pcVar3)();
}


