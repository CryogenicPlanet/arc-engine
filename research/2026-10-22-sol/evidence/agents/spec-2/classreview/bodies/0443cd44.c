// ==== 0x443cd44  ArcSessionTab::lastActiveTime  (164 bytes)

/* Function Stack Size: 0x10 bytes */

ID ArcSessionTab::lastActiveTime(ID param_1,SEL param_2)

{
  long lVar1;
  code *pcVar2;
  ID IVar3;
  long lVar4;
  double dVar5;
  
  if ((*(byte *)(param_1 + 0x20) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x443cde8);
    (*pcVar2)();
  }
  lVar4 = *(long *)(**(long **)(param_1 + 0x10) + 0xa0);
  if (lVar4 == 0) {
    dVar5 = 0.0;
  }
  else if (lVar4 + 0x8000000000000001U < 2) {
    dVar5 = -INFINITY;
    if (-1 < lVar4) {
      dVar5 = INFINITY;
    }
  }
  else {
    lVar1 = -0x8000000000000000;
    if (!SCARRY8(lVar4,-0x295e9648864000)) {
      lVar1 = lVar4 + -0x295e9648864000;
    }
    if (lVar1 == -0x8000000000000000) {
      dVar5 = -INFINITY;
    }
    else {
      dVar5 = (double)lVar1 / 1000000.0;
    }
  }
  IVar3 = NSDate::dateWithTimeIntervalSince1970_
                    ((ID)&_OBJC_CLASS___NSDate,"dateWithTimeIntervalSince1970:",dVar5);
  return IVar3;
}


