// ==== 0x4412b7c  objc::Categories::ArcWebContents_Extensions_::setMuted:  (288 bytes)

/* Function Stack Size: 0x14 bytes */

void objc::Categories::ArcWebContents_Extensions_::setMuted_(ID param_1,SEL param_2,bool param_3)

{
  long lVar1;
  code *pcVar2;
  long local_50;
  long *local_48;
  byte local_40;
  undefined8 local_38;
  undefined8 uStack_30;
  ulong local_28;
  
  _objc_msgSend(param_1,"web_contents",&local_50);
  if ((local_40 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x4412c9c);
    (*pcVar2)();
  }
  if (*(long *)(local_50 + 0x130) != 0) {
    local_38 = 0;
    uStack_30 = 0;
    local_28 = 0;
    FUN_07876aa4(*(long *)(local_50 + 0x130),param_3,2,&local_38);
    if ((long)local_28 < 0) {
      FUN_00005810(local_38,local_28 & 0x7fffffffffffffff);
    }
    if (local_40 == 0) {
      return;
    }
  }
  if (local_48 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_48[1];
    local_48[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_48 + 0x10))(local_48);
      if (local_48[2] != 0) {
        LOAcquire();
        lVar1 = local_48[2];
        local_48[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      pcVar2 = *(code **)(*local_48 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_48,0x20);
      }
      else {
        (*pcVar2)(local_48);
      }
    }
  }
  return;
}


