// ==== 0x42b7738  FUN_042b7738  (292 bytes)

void FUN_042b7738(undefined1 (*param_1) [16],long param_2)

{
  code *pcVar1;
  long lVar2;
  long *plVar3;
  undefined1 auVar4 [16];
  
  if (*(char *)(param_2 + 0xb0) != '\0') {
    auVar4 = *(undefined1 (*) [16])(param_2 + 0xa0);
    goto LAB_042b77a8;
  }
  auVar4 = FUN_04341f9c();
  if ((*(char *)(param_2 + 0xb0) != '\0') &&
     (plVar3 = *(long **)(param_2 + 0xa8), plVar3 != (long *)0x0)) {
    LOAcquire();
    lVar2 = plVar3[1];
    plVar3[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*plVar3 + 0x10))(plVar3);
      if (plVar3[2] != 0) {
        LOAcquire();
        lVar2 = plVar3[2];
        plVar3[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_042b7790;
      }
      pcVar1 = *(code **)(*plVar3 + 0x20);
      if (((pcVar1 == FUN_00f01bc0) || (pcVar1 == FUN_00f01bc0)) || (pcVar1 == FUN_013efb40)) {
        FUN_00005810(plVar3,0x20);
      }
      else {
        (*pcVar1)(plVar3);
      }
    }
  }
LAB_042b7790:
  if (param_2 == -0xa0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x42b785c);
    (*pcVar1)();
  }
  *(undefined1 (*) [16])(param_2 + 0xa0) = auVar4;
  *(undefined1 *)(param_2 + 0xb0) = 1;
LAB_042b77a8:
  *param_1 = auVar4;
  if (auVar4._8_8_ != 0) {
    plVar3 = (long *)(auVar4._8_8_ + 8);
    *plVar3 = *plVar3 + 1;
  }
  return;
}


