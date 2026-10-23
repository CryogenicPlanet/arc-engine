// ==== 0x44125fc  objc::Categories::ArcWebContents_Extensions_::viewSource  (248 bytes)

/* Function Stack Size: 0x10 bytes */

void objc::Categories::ArcWebContents_Extensions_::viewSource(ID param_1,SEL param_2)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_40;
  long *local_38;
  byte local_30;
  ulong local_28;
  
  _objc_msgSend(param_1,"web_contents",&local_40);
  if ((local_30 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x44126f4);
    (*pcVar2)();
  }
  FUN_04377a08(&local_28,local_40);
  if ((local_28 & 1) == 0) {
    FUN_04b35898();
  }
  if ((local_30 != 0) && (local_38 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_38[1];
    local_38[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_38 + 0x10))(local_38);
      if (local_38[2] != 0) {
        LOAcquire();
        lVar1 = local_38[2];
        local_38[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      pcVar2 = *(code **)(*local_38 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_38,0x20);
      }
      else {
        (*pcVar2)(local_38);
      }
    }
  }
  return;
}


