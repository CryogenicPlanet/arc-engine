// ==== 0x882fd0  FUN_00882fd0  (152 bytes)

byte FUN_00882fd0(long param_1)

{
  code *pcVar1;
  byte bVar2;
  long *plVar3;
  long lVar4;
  long *plVar5;
  
  if (*(char *)(param_1 + 0x9a) == '\0') {
    plVar3 = *(long **)(*(long *)(param_1 + 0x18) + 0x8d8);
    if (plVar3 == (long *)0x0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x883054);
      (*pcVar1)();
    }
    if (*(undefined1 **)(*plVar3 + 0xd0) == &LAB_04152228) {
      do {
        plVar5 = plVar3;
        plVar3 = (long *)plVar5[0x2f];
      } while ((long *)plVar5[0x2f] != (long *)0x0);
    }
    else {
      do {
        plVar5 = plVar3;
        plVar3 = (long *)plVar5[0x2f];
      } while ((long *)plVar5[0x2f] != (long *)0x0);
    }
    if ((*(byte *)(plVar5 + 0x219) & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(1,0x88305c);
      (*pcVar1)();
    }
    lVar4 = *(long *)(plVar5[0x1de] + 0x188);
    if (lVar4 == 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x883068);
      (*pcVar1)();
    }
    if (*(long *)(lVar4 + 0x178) != 0) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x883074);
      (*pcVar1)();
    }
    bVar2 = *(float *)("/System/Library/Frameworks/IOKit.framework/Versions/A/IOKit" + lVar4 + 0x1c)
            == 1.0;
  }
  else {
    bVar2 = *(byte *)(param_1 + 0x99);
  }
  return bVar2 & 1;
}


