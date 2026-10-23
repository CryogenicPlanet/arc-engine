// ==== 0x43edeb0  objc::Categories::ArcBrowserContext_Extensions_::loadUrl:withDelegate:  (468 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Removing unreachable block (ram,0x043edfc0) */
/* Function Stack Size: 0x20 bytes */

ID objc::Categories::ArcBrowserContext_Extensions_::loadUrl_withDelegate_
             (ID param_1,SEL param_2,ID param_3,ID param_4)

{
  long lVar1;
  undefined8 *puVar2;
  ID IVar3;
  code *pcVar4;
  undefined8 local_110;
  undefined8 uStack_108;
  undefined8 uStack_100;
  undefined8 uStack_f8;
  undefined8 local_f0;
  undefined8 uStack_e8;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined8 uStack_c8;
  undefined8 uStack_c0;
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  undefined8 local_70;
  long *local_68;
  byte local_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  
  local_58 = 0;
  uStack_50 = 0;
  local_48 = 0;
  _objc_msgSend(param_1,"browser_context",&local_70);
  if ((local_60 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43ee084);
    (*pcVar4)();
  }
  if (param_3 == 0) {
    uStack_88 = 0;
    local_90 = 0;
    uStack_78 = 0;
    uStack_80 = 0;
    uStack_a8 = 0;
    local_b0 = 0;
    uStack_98 = 0;
    uStack_a0 = 0;
    uStack_c8 = 0;
    local_d0 = 0;
    uStack_b8 = 0;
    uStack_c0 = 0;
    uStack_e8 = 0;
    local_f0 = 0;
    uStack_d8 = 0;
    uStack_e0 = 0;
    uStack_108 = 0;
    local_110 = 0;
    uStack_f8 = 0;
    uStack_100 = 0;
  }
  else {
    _objc_msgSend(param_3,"config",&local_110);
  }
  puVar2 = (undefined8 *)FUN_00008790(0x10);
  *puVar2 = &PTR_FUN_0da87ed8;
  puVar2[1] = param_4;
  FUN_042c8c30(&local_58,local_70,&local_110,puVar2);
  FUN_0836de40(&local_110);
  if ((local_60 != 0) && (local_68 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_68[1];
    local_68[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_68 + 0x10))(local_68);
      if (local_68[2] != 0) {
        LOAcquire();
        lVar1 = local_68[2];
        local_68[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043edf98;
      }
      pcVar4 = *(code **)(*local_68 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_68,0x20);
      }
      else {
        (*pcVar4)(local_68);
      }
    }
  }
LAB_043edf98:
  IVar3 = _objc_alloc(&class_t::ArcURLLoadCancellationToken);
  IVar3 = ArcURLLoadCancellationToken::initWithToken_(IVar3,"initWithToken:",&local_58);
  return IVar3;
}


