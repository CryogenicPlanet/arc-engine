// ==== 0x43f96c8  objc::Categories::ArcBrowserContextPageContentExtraction_Extensions_::isEqual:  (552 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

bool objc::Categories::ArcBrowserContextPageContentExtraction_Extensions_::isEqual_
               (ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  bool bVar4;
  long local_60;
  long *local_58;
  byte local_50;
  long local_48;
  long *local_40;
  byte local_38;
  
  if (param_1 == param_3) {
    return 1;
  }
  if (param_3 == 0) {
    return 0;
  }
  IVar2 = _objc_msgSend((ID)&class_t::ArcBrowserContextPageContentExtraction,"class");
  IVar2 = _objc_msgSend(param_3,"isKindOfClass:",IVar2);
  if ((int)IVar2 == 0) {
    return 0;
  }
  _objc_msgSend(param_1,"browser_context_page_content_extraction",&local_48);
  _objc_msgSend(param_3,"browser_context_page_content_extraction",&local_60);
  if ((local_50 & local_38) == 0) {
    bVar4 = (bool)(local_38 == local_50);
    if (local_50 == 0) goto LAB_043f97c4;
  }
  else {
    if ((local_50 & 1) == 0) {
                    /* WARNING: Does not return */
      pcVar3 = (code *)SoftwareBreakpoint(1,0x43f98f0);
      (*pcVar3)();
    }
    bVar4 = (bool)(local_48 == local_60);
  }
  if (local_58 != (long *)0x0) {
    LOAcquire();
    lVar1 = local_58[1];
    local_58[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_58 + 0x10))(local_58);
      if (local_58[2] != 0) {
        LOAcquire();
        lVar1 = local_58[2];
        local_58[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f97c4;
      }
      pcVar3 = *(code **)(*local_58 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_58,0x20);
      }
      else {
        (*pcVar3)(local_58);
      }
    }
  }
LAB_043f97c4:
  if (local_38 == 0) {
    return bVar4;
  }
  if (local_40 == (long *)0x0) {
    return bVar4;
  }
  LOAcquire();
  lVar1 = local_40[1];
  local_40[1] = lVar1 + -1;
  LORelease();
  if (lVar1 != 0) {
    return bVar4;
  }
  (**(code **)(*local_40 + 0x10))(local_40);
  if (local_40[2] != 0) {
    LOAcquire();
    lVar1 = local_40[2];
    local_40[2] = lVar1 + -1;
    LORelease();
    if (lVar1 != 0) {
      return bVar4;
    }
  }
  pcVar3 = *(code **)(*local_40 + 0x20);
  if (((pcVar3 != FUN_00f01bc0) && (pcVar3 != FUN_00f01bc0)) && (pcVar3 != FUN_013efb40)) {
    (*pcVar3)(local_40);
    return bVar4;
  }
  FUN_00005810(local_40,0x20);
  return bVar4;
}


