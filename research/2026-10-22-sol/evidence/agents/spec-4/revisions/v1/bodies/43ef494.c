// ==== 0x43ef494  objc::Categories::ArcBrowserContext_Extensions_::clearBookmarks  (540 bytes)

/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x10 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::clearBookmarks(ID param_1,SEL param_2)

{
  ID self;
  long lVar1;
  code *pcVar2;
  undefined1 auStack_80 [8];
  long *local_78;
  byte local_70;
  long local_68;
  long *local_60;
  byte local_58;
  undefined1 auStack_50 [32];
  
  self = _objc_msgSend(param_1,"getBookmarksTree");
  if (self == 0) {
    return;
  }
  _objc_msgSend(param_1,"browser_context",&local_68);
  if ((local_58 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43ef6ac);
    (*pcVar2)();
  }
  _objc_msgSend(self,"node",auStack_80);
  if ((local_70 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
    FUN_04b08a00();
  }
  if (((*(long *)(local_68 + 0x88) != 0) && (lVar1 = FUN_00881874(), lVar1 != 0)) &&
     (*(char *)(lVar1 + 0x21) != '\0')) {
    FUN_00029e40(auStack_50,"ClearBookmarks",
                 "../../arc/browser/browser_context/browser_context_impl.cc",0x2c7);
    FUN_09f4e2b8(lVar1,auStack_50);
  }
  if ((local_70 != 0) && (local_78 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_78[1];
    local_78[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_78 + 0x10))(local_78);
      if (local_78[2] != 0) {
        LOAcquire();
        lVar1 = local_78[2];
        local_78[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043ef564;
      }
      pcVar2 = *(code **)(*local_78 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_78,0x20);
      }
      else {
        (*pcVar2)(local_78);
      }
    }
  }
LAB_043ef564:
  if ((local_58 != 0) && (local_60 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_60[1];
    local_60[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_60 + 0x10))(local_60);
      if (local_60[2] != 0) {
        LOAcquire();
        lVar1 = local_60[2];
        local_60[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) {
          return;
        }
      }
      pcVar2 = *(code **)(*local_60 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_60,0x20);
      }
      else {
        (*pcVar2)(local_60);
      }
    }
  }
  return;
}


