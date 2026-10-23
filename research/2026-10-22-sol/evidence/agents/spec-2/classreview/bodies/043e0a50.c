// ==== 0x43e0a50  ArcBrowser::sessionId  (76 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID ArcBrowser::sessionId(ID param_1,SEL param_2)

{
  code *pcVar1;
  ID IVar2;
  SessionID local_14;
  
  if ((*(byte *)(param_1 + 0x38) & 1) != 0) {
    local_14.field0_0x0 = FUN_042fb310(*(undefined8 *)(param_1 + 0x28));
    IVar2 = _objc_alloc(&objc::class_t::ArcSessionID);
    IVar2 = ArcSessionID::initWithSessionId_(IVar2,"initWithSessionId:",&local_14);
    return IVar2;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x43e0a9c);
  (*pcVar1)();
}


