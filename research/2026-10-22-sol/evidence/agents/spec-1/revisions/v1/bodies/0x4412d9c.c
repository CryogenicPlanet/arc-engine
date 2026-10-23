// ==== 0x4412d9c  objc::Categories::ArcWebContents_Extensions_::applyColorScheme:  (240 bytes)

/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcWebContents_Extensions_::applyColorScheme_
               (ID param_1,SEL param_2,long_long param_3)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_38;
  long *local_30;
  byte local_28;
  
  _objc_msgSend(param_1,"web_contents",&local_38);
  if ((local_28 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x4412e8c);
    (*pcVar2)();
  }
  FUN_043794f0(local_38,param_3);
  if ((local_28 != 0) && (local_30 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_30[1];
    local_30[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_30 + 0x10))(local_30);
      if (local_30[2] != 0) {
        LOAcquire();
        lVar1 = local_30[2];
        local_30[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      pcVar2 = *(code **)(*local_30 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_30,0x20);
      }
      else {
        (*pcVar2)(local_30);
      }
    }
  }
  return;
}


