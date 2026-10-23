// ==== 0x43e99a8  ArcBrowserApplication::loadUrl:withDelegate:  (268 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* WARNING: Removing unreachable block (ram,0x043e9a7c) */
/* Function Stack Size: 0x20 bytes */

ID ArcBrowserApplication::loadUrl_withDelegate_(ID param_1,SEL param_2,ID param_3,ID param_4)

{
  undefined8 *puVar1;
  ID IVar2;
  undefined8 *puVar3;
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
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined8 local_48;
  undefined8 uStack_40;
  undefined8 local_38;
  
  local_48 = 0;
  uStack_40 = 0;
  local_38 = 0;
  puVar3 = *(undefined8 **)(param_1 + 0x20);
  if (param_3 == 0) {
    uStack_68 = 0;
    local_70 = 0;
    uStack_58 = 0;
    uStack_60 = 0;
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
  }
  else {
    _objc_msgSend(param_3,"config",&local_f0);
  }
  puVar1 = (undefined8 *)FUN_00008790(0x10);
  *puVar1 = &PTR_FUN_0da87ed8;
  puVar1[1] = param_4;
  FUN_042bdb68(&local_48,*puVar3,&local_f0,puVar1);
  FUN_0836de40(&local_f0);
  IVar2 = _objc_alloc(&objc::class_t::ArcURLLoadCancellationToken);
  IVar2 = ArcURLLoadCancellationToken::initWithToken_(IVar2,"initWithToken:",&local_48);
  return IVar2;
}


