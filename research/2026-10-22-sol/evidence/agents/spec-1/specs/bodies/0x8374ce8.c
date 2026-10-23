// ==== 0x8374ce8  ArcSessionNavigation::virtualURL  (136 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcSessionNavigation::virtualURL(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  undefined8 local_40;
  undefined8 uStack_38;
  ulong local_30;
  undefined8 local_28;
  
  if ((*(byte *)(param_1 + 0x18) & 1) != 0) {
    uStack_38 = 0;
    local_40 = 0;
    local_28 = 0;
    local_30 = 0;
    FUN_05533b24(&local_40,*(long *)(param_1 + 8) + 0x28);
    if ((char)local_28 == '\0') {
      IVar2 = 0;
    }
    else {
      IVar2 = FUN_043abd34(&local_40);
      if (((char)local_28 != '\0') && ((long)local_30 < 0)) {
        thunk_FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
      }
    }
    return IVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x8374d70);
  (*pcVar1)();
}


