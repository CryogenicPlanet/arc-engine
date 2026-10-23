// ==== 0x43ff064  ArcAnnotatedVisit::visitTime  (88 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcAnnotatedVisit::visitTime(ID param_1,SEL param_2)

{
  ID IVar1;
  
  if ((*(byte *)(param_1 + 0x10) & 1) == 0) {
    IVar1 = NSDate::date((ID)&_OBJC_CLASS___NSDate,"date");
  }
  else {
    IVar1 = NSDate::dateWithTimeIntervalSince1970_
                      ((ID)&_OBJC_CLASS___NSDate,"dateWithTimeIntervalSince1970:",
                       *(undefined8 *)(*(long *)(param_1 + 8) + 0x30));
  }
  return IVar1;
}


