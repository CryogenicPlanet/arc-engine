// ==== 0x43ee15c  objc::Categories::ArcBrowserContext_Extensions_::cancelUrlLoad:  (388 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::cancelUrlLoad_
               (ID param_1,SEL param_2,ID param_3)

{
  int iVar1;
  code *pcVar2;
  long lVar3;
  void *local_60;
  long local_58;
  ulong local_50;
  long local_48;
  long *local_40;
  byte local_38;
  
  _objc_msgSend(param_1,"browser_context",&local_48);
  if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43ee2e0);
    (*pcVar2)();
  }
  if (param_3 == 0) {
    local_60 = (void *)0x0;
    local_58 = 0;
    local_50 = 0;
  }
  else {
    _objc_msgSend(param_3,"token",&local_60);
  }
  lVar3 = *(long *)(local_48 + 0x288);
  if ((lVar3 != 0) &&
     (((-1 < (long)local_50 || (local_58 != 0x24)) ||
      (iVar1 = _memcmp(local_60,&DAT_0c9d6718,0x24), iVar1 != 0)))) {
    FUN_0434f448(lVar3,&local_60);
  }
  if ((long)local_50 < 0) {
    FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
  }
  if ((local_38 != 0) && (local_40 != (long *)0x0)) {
    LOAcquire();
    lVar3 = local_40[1];
    local_40[1] = lVar3 + -1;
    LORelease();
    if (lVar3 == 0) {
      (**(code **)(*local_40 + 0x10))(local_40);
      if (local_40[2] != 0) {
        LOAcquire();
        lVar3 = local_40[2];
        local_40[2] = lVar3 + -1;
        LORelease();
        if (lVar3 != 0) {
          return;
        }
      }
      pcVar2 = *(code **)(*local_40 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_40,0x20);
      }
      else {
        (*pcVar2)(local_40);
      }
    }
  }
  return;
}


