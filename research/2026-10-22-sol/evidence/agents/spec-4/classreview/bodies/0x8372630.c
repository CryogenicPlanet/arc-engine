// ==== 0x8372630  objc::Categories::ArcWebContents_Extensions_::basicPrint  (284 bytes)

/* Function Stack Size: 0x10 bytes */

void objc::Categories::ArcWebContents_Extensions_::basicPrint(ID param_1,SEL param_2)

{
  long lVar1;
  ulong uVar2;
  code *pcVar3;
  long local_38;
  long *local_30;
  byte local_28;
  
  _objc_msgSend(param_1,"web_contents",&local_38);
  if ((local_28 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x837274c);
    (*pcVar3)();
  }
  if (*(long *)(local_38 + 0x130) == 0) {
    uVar2 = FUN_04b37104(0x25,"managed webcontents is null",0x1b,0x6fb,
                         "../../arc/browser/web_contents/web_contents_impl.cc");
    if ((uVar2 & 1) == 0) {
      FUN_04b35898();
    }
  }
  else {
    FUN_07f73fa0();
  }
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
      pcVar3 = *(code **)(*local_30 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        thunk_FUN_00005810(local_30,0x20);
      }
      else {
        (*pcVar3)(local_30);
      }
    }
  }
  return;
}


