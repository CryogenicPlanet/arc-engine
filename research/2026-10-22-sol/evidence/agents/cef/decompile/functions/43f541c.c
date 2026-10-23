// ==== 0x43f541c  objc::Categories::ArcBrowserContext_Extensions_::id  (360 bytes)

/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x10 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::id(ID param_1,SEL param_2)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  long local_68;
  long *local_60;
  byte local_58;
  undefined8 local_50;
  undefined8 uStack_48;
  ulong local_40;
  undefined4 local_38;
  
  IVar2 = _objc_alloc(&class_t::ArcBrowserContextID);
  _objc_msgSend(param_1,"browser_context",&local_68);
  if ((local_58 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43f5584);
    (*pcVar3)();
  }
  if (*(char *)(local_68 + 0xa7) < '\0') {
    FUN_00060a60(&local_50,*(undefined8 *)(local_68 + 0x90),*(undefined8 *)(local_68 + 0x98));
  }
  else {
    uStack_48 = *(undefined8 *)(local_68 + 0x98);
    local_50 = *(undefined8 *)(local_68 + 0x90);
    local_40 = *(ulong *)(local_68 + 0xa0);
  }
  local_38 = *(undefined4 *)(local_68 + 0xa8);
  IVar2 = ArcBrowserContextID::initWithBrowserContextID_
                    (IVar2,"initWithBrowserContextID:",&local_50);
  if ((long)local_40 < 0) {
    FUN_00005810(local_50,local_40 & 0x7fffffffffffffff);
  }
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
          return IVar2;
        }
      }
      pcVar3 = *(code **)(*local_60 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_60,0x20);
      }
      else {
        (*pcVar3)(local_60);
      }
    }
  }
  return IVar2;
}


