// ==== 0x442e5a4  ArcHistoryDeletion::timeRangeStart  (36 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcHistoryDeletion::timeRangeStart(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  
  if ((*(byte *)(param_1 + 0x88) & 1) != 0) {
    IVar2 = NSDate::dateWithTimeIntervalSince1970_
                      ((ID)&_OBJC_CLASS___NSDate,"dateWithTimeIntervalSince1970:",
                       *(undefined8 *)(param_1 + 0x18));
    return IVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x442e5c8);
  (*pcVar1)();
}


