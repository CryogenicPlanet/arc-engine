// ==== 0x442fa20  ArcDownloadItem::startTime  (80 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* Function Stack Size: 0x10 bytes */

ID ArcDownloadItem::startTime(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  undefined1 auVar3 [16];
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x442fa70);
    (*pcVar1)();
  }
  IVar2 = 0;
  if (*(long *)(param_1 + 0x10) != 0) {
    auVar3 = FUN_0430def0();
    if ((auVar3._8_8_ & 1) == 0) {
      IVar2 = 0;
    }
    else {
      IVar2 = NSDate::dateWithTimeIntervalSince1970_
                        ((ID)&_OBJC_CLASS___NSDate,"dateWithTimeIntervalSince1970:",auVar3._0_8_);
    }
  }
  return IVar2;
}


