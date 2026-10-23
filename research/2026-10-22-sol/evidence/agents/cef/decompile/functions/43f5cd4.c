// ==== 0x43f5cd4  objc::Categories::ArcBrowserContext_Extensions_::productConnectionClosed:withReason:  (448 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::productConnectionClosed_withReason_
               (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  long lVar1;
  undefined8 uVar2;
  code *pcVar3;
  undefined8 local_78;
  undefined8 uStack_70;
  ulong local_68;
  undefined8 local_60;
  undefined8 uStack_58;
  ulong local_50;
  long local_48;
  long *local_40;
  byte local_38;
  
  _objc_msgSend(param_1,"browser_context",&local_48);
  if ((local_38 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43f5e94);
    (*pcVar3)();
  }
  if (param_3 == 0) {
    local_60 = 0;
    uStack_58 = 0;
    local_50 = 0;
  }
  else {
    FUN_01b356b8(&local_60,param_3);
  }
  if (param_4 == 0) {
    local_78 = 0;
    uStack_70 = 0;
    local_68 = 0;
  }
  else {
    FUN_01b356b8(&local_78,param_4);
  }
  if (*(long *)(local_48 + 0x88) != 0) {
    uVar2 = FUN_04135760(*(undefined8 *)(*(long *)(local_48 + 0x88) + 0x18));
    FUN_0416cc3c(uVar2,&local_60,&local_78);
  }
  if ((long)local_68 < 0) {
    FUN_00005810(local_78,local_68 & 0x7fffffffffffffff);
  }
  if ((long)local_50 < 0) {
    FUN_00005810(local_60,local_50 & 0x7fffffffffffffff);
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
          return;
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
  return;
}


