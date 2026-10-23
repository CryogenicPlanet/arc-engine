// ==== 0x44169e8  ArcNavigation::isEqual:  (368 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

bool ArcNavigation::isEqual_(ID param_1,SEL param_2,ID param_3)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  bool bVar4;
  long local_48;
  long *local_40;
  byte local_38;
  
  if (param_1 == param_3) {
    return 1;
  }
  if (param_3 != 0) {
    IVar2 = _objc_msgSend((ID)&objc::class_t::ArcNavigation,"class");
    IVar2 = _objc_msgSend(param_3,"isKindOfClass:",IVar2);
    if ((int)IVar2 != 0) {
      _objc_msgSend(param_3,"nativeNavigation",&local_48);
      if ((local_38 & *(byte *)(param_1 + 0x20)) == 0) {
        bVar4 = (bool)(*(byte *)(param_1 + 0x20) == local_38);
        if (local_38 == 0) {
          return bVar4;
        }
      }
      else {
        if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
          pcVar3 = (code *)SoftwareBreakpoint(1,0x4416b58);
          (*pcVar3)();
        }
        bVar4 = (bool)(*(long *)(param_1 + 0x10) == local_48);
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
  }
  return 0;
}


