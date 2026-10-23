// ==== 0x4412e8c  objc::Categories::ArcWebContents_Extensions_::shareable  (236 bytes)

/* Function Stack Size: 0x10 bytes */

bool objc::Categories::ArcWebContents_Extensions_::shareable(ID param_1,SEL param_2)

{
  long lVar1;
  code *pcVar2;
  long lVar3;
  long local_48;
  long *plStack_40;
  byte local_38;
  
  _objc_msgSend(param_1,"web_contents",&local_48);
  if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x4412f78);
    (*pcVar2)();
  }
  lVar3 = *(long *)(local_48 + 0x600);
  if (plStack_40 != (long *)0x0) {
    LOAcquire();
    lVar1 = plStack_40[1];
    plStack_40[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*plStack_40 + 0x10))(plStack_40);
      if (plStack_40[2] != 0) {
        LOAcquire();
        lVar1 = plStack_40[2];
        plStack_40[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04412ed4;
      }
      pcVar2 = *(code **)(*plStack_40 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(plStack_40,0x20);
      }
      else {
        (*pcVar2)(plStack_40);
      }
    }
  }
LAB_04412ed4:
  return (bool)(lVar3 != 0);
}


