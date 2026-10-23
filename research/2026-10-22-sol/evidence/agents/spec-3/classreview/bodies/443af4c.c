// ==== 0x443af4c  ArcBreadcrumb::line  (56 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x10 bytes */

ID ArcBreadcrumb::line(ID param_1,SEL param_2)

{
  ID IVar1;
  
  if ((*(ulong *)(param_1 + 0x80) >> 0x20 & 1) == 0) {
    return 0;
  }
  IVar1 = NSNumber::numberWithInt_((ID)&_OBJC_CLASS___NSNumber,"numberWithInt:");
  return IVar1;
}


