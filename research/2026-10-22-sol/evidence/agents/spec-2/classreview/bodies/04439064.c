// ==== 0x4439064  ArcAutocompleteMatch::matchType  (152 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAutocompleteMatch::matchType(ID param_1,SEL param_2)

{
  undefined1 **ppuVar1;
  uint uVar2;
  code *pcVar3;
  ID IVar4;
  undefined1 *local_40;
  long lStack_38;
  undefined7 local_30;
  char cStack_29;
  undefined1 uStack_21;
  
  if (((*(byte *)(param_1 + 0x28) & 1) != 0) &&
     (uVar2 = *(uint *)(*(long *)(param_1 + 0x18) + 0x3f4), uVar2 < 0x29)) {
    FUN_00b89b80(&local_40,(&PTR_s_url_what_you_typed_0e1dcfc8)[uVar2],&uStack_21);
    ppuVar1 = (undefined1 **)local_40;
    if (-1 < (long)cStack_29) {
      ppuVar1 = &local_40;
    }
    if (-1 < cStack_29) {
      lStack_38 = (long)cStack_29;
    }
    IVar4 = FUN_01c391bc(ppuVar1,lStack_38);
    if (cStack_29 < '\0') {
      FUN_00005810(local_40,CONCAT17(cStack_29,local_30) & 0x7fffffffffffffff);
    }
    return IVar4;
  }
                    /* WARNING: Does not return */
  pcVar3 = (code *)SoftwareBreakpoint(1,0x44390fc);
  (*pcVar3)();
}


