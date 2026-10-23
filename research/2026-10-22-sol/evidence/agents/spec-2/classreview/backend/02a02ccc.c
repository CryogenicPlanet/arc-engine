// ==== 0x2a02ccc  FUN_02a02ccc  (340 bytes)

/* WARNING: Function: _objc_retainAutoreleaseReturnValue replaced with injection: _objc_retain_fixup
    */

undefined8 FUN_02a02ccc(long *param_1)

{
  code *pcVar1;
  long *plVar2;
  long *plVar3;
  long lVar4;
  long *plVar5;
  
  lVar4 = *param_1;
  if (lVar4 == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x2a02e1c);
    (*pcVar1)();
  }
  plVar3 = (long *)(lVar4 + 0x10);
  plVar2 = (long *)*plVar3;
  for (plVar5 = plVar2; plVar5 != (long *)0x0; plVar5 = (long *)*plVar5) {
    while ((int)plVar5[4] < 1) {
      if (-1 < (int)plVar5[4]) {
        if ((plVar5 == plVar3) || (plVar5[5] == 0)) goto LAB_02a02d14;
        goto LAB_02a02d70;
      }
      plVar5 = (long *)plVar5[1];
      if (plVar5 == (long *)0x0) goto LAB_02a02d14;
    }
  }
LAB_02a02d14:
  if (*(int *)(lVar4 + 4) != 2) {
    if (*(int *)(lVar4 + 4) != 3) {
                    /* WARNING: Does not return */
      pcVar1 = (code *)SoftwareBreakpoint(0,0x2a02e4c);
      (*pcVar1)();
    }
    for (; plVar2 != (long *)0x0; plVar2 = (long *)*plVar2) {
      while ((int)plVar2[4] < 4) {
        if ((int)plVar2[4] == 3) {
          if (plVar2 == plVar3) goto LAB_02a02e0c;
          if (*(int *)(plVar2[5] + 8) != 3) {
                    /* WARNING: Does not return */
            pcVar1 = (code *)SoftwareBreakpoint(0,0x2a02e58);
            (*pcVar1)();
          }
          FUN_02a03b58(plVar2[5] + 0x10);
          goto LAB_02a02d58;
        }
        plVar2 = (long *)plVar2[1];
        if (plVar2 == (long *)0x0) goto LAB_02a02e0c;
      }
    }
LAB_02a02e0c:
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0,0x2a02e10);
    (*pcVar1)();
  }
  for (; plVar2 != (long *)0x0; plVar2 = (long *)*plVar2) {
    while ((int)plVar2[4] < 3) {
      if ((int)plVar2[4] == 2) {
        if (plVar2 != plVar3) {
          if (*(int *)(plVar2[5] + 8) != 2) {
                    /* WARNING: Does not return */
            pcVar1 = (code *)SoftwareBreakpoint(0,0x2a02e40);
            (*pcVar1)();
          }
          FUN_02a02ef4(plVar2[5] + 0x10);
LAB_02a02d58:
          lVar4 = FUN_02a02e78();
          if (lVar4 == 0) {
                    /* WARNING: Does not return */
            pcVar1 = (code *)SoftwareBreakpoint(0,0x2a02e28);
            (*pcVar1)();
          }
          if (*param_1 == 0) {
                    /* WARNING: Does not return */
            pcVar1 = (code *)SoftwareBreakpoint(0,0x2a02e34);
            (*pcVar1)();
          }
          FUN_007b7790(*param_1,lVar4);
LAB_02a02d70:
          lVar4 = FUN_02a02e60();
          return *(undefined8 *)(lVar4 + 0x10);
        }
        goto LAB_02a02df8;
      }
      plVar2 = (long *)plVar2[1];
      if (plVar2 == (long *)0x0) goto LAB_02a02df8;
    }
  }
LAB_02a02df8:
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0,0x2a02dfc);
  (*pcVar1)();
}


