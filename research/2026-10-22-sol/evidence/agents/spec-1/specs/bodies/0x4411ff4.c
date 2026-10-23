// ==== 0x4411ff4  objc::Categories::ArcWebContents_Extensions_::sessionId  (264 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcWebContents_Extensions_::sessionId(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined8 local_40;
  long *local_38;
  byte local_30;
  SessionID local_24;
  
  _objc_msgSend(param_1,"web_contents",&local_40);
  if ((local_30 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x44120fc);
    (*pcVar3)();
  }
  local_24.field0_0x0 = FUN_04376480(local_40);
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
        if (lVar1 != 0) goto LAB_04412048;
      }
      pcVar3 = *(code **)(*local_38 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_38,0x20);
      }
      else {
        (*pcVar3)(local_38);
      }
    }
  }
LAB_04412048:
  IVar2 = _objc_alloc(&class_t::ArcSessionID);
  IVar2 = ArcSessionID::initWithSessionId_(IVar2,"initWithSessionId:",&local_24);
  return IVar2;
}


