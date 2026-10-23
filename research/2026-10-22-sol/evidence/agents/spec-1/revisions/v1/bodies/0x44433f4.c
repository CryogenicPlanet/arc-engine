// ==== 0x44433f4  ArcWebContents::rootFrameAccessibilityTreeId  (124 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcWebContents::rootFrameAccessibilityTreeId(ID param_1,SEL param_2)

{
  undefined8 ***pppuVar1;
  code *pcVar2;
  ID IVar3;
  undefined8 **local_38;
  long lStack_30;
  undefined7 local_28;
  char cStack_21;
  
  if ((*(byte *)(param_1 + 0x128) & 1) != 0) {
    FUN_0439d26c(&local_38,*(undefined8 *)(param_1 + 0x118));
    pppuVar1 = (undefined8 ***)local_38;
    if (-1 < (long)cStack_21) {
      pppuVar1 = &local_38;
    }
    if (-1 < cStack_21) {
      lStack_30 = (long)cStack_21;
    }
    IVar3 = FUN_01c391bc(pppuVar1,lStack_30);
    if (cStack_21 < '\0') {
      FUN_00005810(local_38,CONCAT17(cStack_21,local_28) & 0x7fffffffffffffff);
    }
    return IVar3;
  }
                    /* WARNING: Does not return */
  pcVar2 = (code *)SoftwareBreakpoint(1,0x4443470);
  (*pcVar2)();
}


