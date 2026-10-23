// ==== 0x4410cd0  objc::Categories::ArcWebContents_Extensions_::attachToPaneHostView:  (316 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcWebContents_Extensions_::attachToPaneHostView_
               (ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  code *pcVar2;
  long local_50;
  long *local_48;
  byte local_40;
  undefined8 local_38;
  
  if (param_3 != 0) {
    _objc_msgSend(param_1,"web_contents",&local_50);
    if ((local_40 & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar2 = (code *)SoftwareBreakpoint(1,0x4410e0c);
      (*pcVar2)();
    }
    FUN_04376580(local_50);
    local_38 = *(undefined8 *)(local_50 + 0x288);
    FUN_04739114(param_3,&local_38);
    if ((local_40 != 0) && (local_48 != (long *)0x0)) {
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
  }
  return;
}


