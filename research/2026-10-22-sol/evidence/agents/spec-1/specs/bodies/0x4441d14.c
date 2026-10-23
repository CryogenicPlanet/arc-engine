// ==== 0x4441d14  ArcWebContents::diagnosticId  (100 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcWebContents::diagnosticId(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  long lVar3;
  
  if ((*(byte *)(param_1 + 0x128) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4441d74);
    (*pcVar1)();
  }
  lVar3 = *(long *)(*(long *)(param_1 + 0x118) + 0x130);
  if (lVar3 == 0) {
    IVar2 = 0;
  }
  else {
    lVar3 = FUN_000805e0(lVar3 + 8,&DAT_0c9d7104);
    if (lVar3 == 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x4441d78);
      (*pcVar1)();
    }
    IVar2 = NSNumber::numberWithUnsignedLongLong_
                      ((ID)&_OBJC_CLASS___NSNumber,"numberWithUnsignedLongLong:",
                       *(undefined8 *)(lVar3 + 0x10));
  }
  return IVar2;
}


