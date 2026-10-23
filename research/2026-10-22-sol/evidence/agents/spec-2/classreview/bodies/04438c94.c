// ==== 0x4438c94  ArcAutocompleteMatch::destinationUrl  (140 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::destinationUrl(ID param_1,SEL param_2)

{
  code *pcVar1;
  undefined8 *puVar2;
  ID IVar3;
  undefined8 local_40;
  undefined8 uStack_38;
  ulong local_30;
  
  if ((*(byte *)(param_1 + 0x28) & 1) != 0) {
    puVar2 = (undefined8 *)FUN_007cf864(*(long *)(param_1 + 0x18) + 0x68);
    if (*(char *)((long)puVar2 + 0x17) < '\0') {
      FUN_00060a60(&local_40,*puVar2,puVar2[1]);
    }
    else {
      uStack_38 = puVar2[1];
      local_40 = *puVar2;
      local_30 = puVar2[2];
    }
    IVar3 = FUN_043abd34(&local_40);
    if ((long)local_30 < 0) {
      FUN_00005810(local_40,local_30 & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x4438d20);
  (*pcVar1)();
}


