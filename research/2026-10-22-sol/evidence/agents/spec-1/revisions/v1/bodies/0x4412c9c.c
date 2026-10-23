// ==== 0x4412c9c  objc::Categories::ArcWebContents_Extensions_::audible  (256 bytes)

/* Function Stack Size: 0x10 bytes */

bool objc::Categories::ArcWebContents_Extensions_::audible(ID param_1,SEL param_2)

{
  long lVar1;
  code *pcVar2;
  uint uVar3;
  long local_48;
  long *local_40;
  byte local_38;
  
  _objc_msgSend(param_1,"web_contents",&local_48);
  if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x4412d9c);
    (*pcVar2)();
  }
  if (*(long *)(local_48 + 0x130) == 0) {
    uVar3 = 0;
  }
  else {
    uVar3 = (uint)(byte)s__System_Library_Frameworks_CoreF_00000fe0
                        [*(long *)(local_48 + 0x130) + 0x4a];
  }
  if (local_40 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_40[1];
    local_40[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_40 + 0x10))(local_40);
      if (local_40[2] != 0) {
        LOAcquire();
        lVar1 = local_40[2];
        local_40[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_04412cf4;
      }
      pcVar2 = *(code **)(*local_40 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_40,0x20);
      }
      else {
        (*pcVar2)(local_40);
      }
    }
  }
LAB_04412cf4:
  return uVar3 & 1;
}


