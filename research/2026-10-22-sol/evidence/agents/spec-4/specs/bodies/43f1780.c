// ==== 0x43f1780  objc::Categories::ArcBrowserContext_Extensions_::isBookmarked:  (316 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

bool objc::Categories::ArcBrowserContext_Extensions_::isBookmarked_
               (ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  bool bVar2;
  code *pcVar3;
  undefined8 local_60 [2];
  undefined7 local_50;
  char cStack_49;
  undefined8 local_48;
  long *local_40;
  byte local_38;
  
  _objc_msgSend(param_1,"browser_context",&local_48);
  if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43f18bc);
    (*pcVar3)();
  }
  FUN_043abdf8(local_60,param_3);
  bVar2 = FUN_042ca760(local_48,local_60);
  if (cStack_49 < '\0') {
    FUN_00005810(local_60[0],CONCAT17(cStack_49,local_50) & 0x7fffffffffffffff);
  }
  if ((local_38 != 0) && (local_40 != (long *)0x0)) {
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
        if (lVar1 != 0) {
          return bVar2;
        }
      }
      pcVar3 = *(code **)(*local_40 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_40,0x20);
      }
      else {
        (*pcVar3)(local_40);
      }
    }
  }
  return bVar2;
}


