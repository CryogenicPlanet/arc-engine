// ==== 0x4430bdc  ArcContentNode::textColor  (88 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcContentNode::textColor(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  
  if ((*(byte *)(param_1 + 0x1b8) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4430c34);
    (*pcVar1)();
  }
  if (*(int *)(param_1 + 0x60) != 0) {
    IVar2 = NSString::stringWithFormat_((ID)&_OBJC_CLASS___NSString,"stringWithFormat:",&cf___08X);
    return IVar2;
  }
  return 0;
}


