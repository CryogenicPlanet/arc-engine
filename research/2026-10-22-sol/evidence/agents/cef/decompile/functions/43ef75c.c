// ==== 0x43ef75c  objc::Categories::ArcBrowserContext_Extensions_::getBookmarksTree  (476 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::getBookmarksTree(ID param_1,SEL param_2)

{
  long *plVar1;
  long lVar2;
  ID IVar3;
  code *pcVar4;
  long *plVar5;
  undefined8 local_60;
  long *local_58;
  byte local_50;
  undefined8 local_48;
  long *local_40;
  ulong local_38;
  
  local_48 = 0;
  local_40 = (long *)0x0;
  local_38 = 0;
  _objc_msgSend(param_1,"browser_context",&local_60);
  if ((local_50 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43ef934);
    (*pcVar4)();
  }
  FUN_042c9f90(&local_48,local_60);
  if ((local_50 != 0) && (local_58 != (long *)0x0)) {
    LOAcquire();
    lVar2 = local_58[1];
    local_58[1] = lVar2 + -1;
    LORelease();
    if (lVar2 == 0) {
      (**(code **)(*local_58 + 0x10))(local_58);
      if (local_58[2] != 0) {
        LOAcquire();
        lVar2 = local_58[2];
        local_58[2] = lVar2 + -1;
        LORelease();
        if (lVar2 != 0) goto LAB_043ef7bc;
      }
      pcVar4 = *(code **)(*local_58 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_58,0x20);
      }
      else {
        (*pcVar4)(local_58);
      }
    }
  }
LAB_043ef7bc:
  if ((char)local_38 == '\0') {
    IVar3 = 0;
  }
  else {
    IVar3 = _objc_alloc(&class_t::ArcBookmarkTreeNode);
    if ((local_38 & 1) == 0) {
                    /* WARNING: Subroutine does not return */
      FUN_04b08a00();
    }
    IVar3 = ArcBookmarkTreeNode::initWithBookmarkTreeNode_
                      (IVar3,"initWithBookmarkTreeNode:",&local_48);
    plVar1 = local_40;
    if (((char)local_38 != '\0') && (local_40 != (long *)0x0)) {
      LOAcquire();
      lVar2 = local_40[1];
      local_40[1] = lVar2 + -1;
      LORelease();
      if (lVar2 == 0) {
        plVar5 = local_40 + 2;
        (**(code **)(*local_40 + 0x10))(local_40);
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
  }
  return IVar3;
}


