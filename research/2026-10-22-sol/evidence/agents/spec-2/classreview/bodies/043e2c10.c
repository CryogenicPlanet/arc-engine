// ==== 0x43e2c10  ArcBrowser::restoreForeignTab:  (488 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcBrowser::restoreForeignTab_(ID param_1,SEL param_2,ID param_3)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  code *pcVar4;
  long *plVar5;
  undefined1 auStack_58 [8];
  long *local_50;
  byte local_48;
  undefined8 local_40;
  long *local_38;
  
  if (param_3 == 0) {
    return 0;
  }
  local_40 = 0;
  local_38 = (long *)0x0;
  if (((*(byte *)(param_1 + 0x38) & 1) == 0) ||
     (_objc_msgSend(param_3,"session_tab",auStack_58), (local_48 & 1) == 0)) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43e2df8);
    (*pcVar4)();
  }
  FUN_042fb100(&local_40,*(undefined8 *)(param_1 + 0x28),auStack_58);
  if ((local_48 != 0) && (local_50 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_50[1];
    local_50[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_50 + 0x10))(local_50);
      if (local_50[2] != 0) {
        LOAcquire();
        lVar2 = local_50[2];
        local_50[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043e2c90;
      }
      pcVar4 = *(code **)(*local_50 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_50,0x20);
      }
      else {
        (*pcVar4)(local_50);
      }
    }
  }
LAB_043e2c90:
  IVar3 = _objc_alloc(&objc::class_t::ArcWebContents);
  IVar3 = ArcWebContents::initWithWebContents_(IVar3,"initWithWebContents:",&local_40);
  plVar1 = local_38;
  if (local_38 != (long *)0x0) {
    LOAcquire();
    lVar2 = local_38[1];
    local_38[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      plVar5 = local_38 + 2;
      (**(code **)(*local_38 + 0x10))(local_38);
      if (*plVar5 != 0) {
        LOAcquire();
        lVar2 = plVar1[2];
        plVar1[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) {
          return IVar3;
        }
      }
      pcVar4 = *(code **)(*plVar1 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(plVar1,0x20);
      }
      else {
        (*pcVar4)(plVar1);
      }
    }
  }
  return IVar3;
}


