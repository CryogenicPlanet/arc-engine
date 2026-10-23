// ==== 0x4415838  ArcWebContentsDevTools::subscribe:autoEnable:notification:  (560 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x24 bytes */

ID ArcWebContentsDevTools::subscribe_autoEnable_notification_
             (ID param_1,SEL param_2,ID param_3,bool param_4,ID param_5,undefined4 param_6)

{
  code *pcVar1;
  ID IVar2;
  ID *local_c0;
  undefined8 uStack_b8;
  ulong local_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  ulong local_98;
  ulong local_90;
  ulong local_88;
  undefined8 local_80;
  undefined8 uStack_78;
  ulong local_70;
  ID *local_68;
  undefined8 uStack_60;
  undefined1 *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  if ((*(byte *)(param_1 + 0x30) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4415a64);
    (*pcVar1)();
  }
  local_90 = 0;
  local_88 = 0;
  IVar2 = _objc_msgSend(param_3,"domain");
  if (IVar2 == 0) {
    local_68 = (ID *)0x0;
    uStack_60 = 0;
    local_58 = (undefined1 *)0x0;
  }
  else {
    FUN_01b356b8(&local_68,IVar2);
  }
  IVar2 = _objc_msgSend(param_3,"name");
  if (IVar2 == 0) {
    local_80 = 0;
    uStack_78 = 0;
    local_70 = 0;
  }
  else {
    FUN_01b356b8(&local_80,IVar2);
  }
  uStack_b8 = uStack_60;
  local_c0 = local_68;
  local_b0 = (ulong)local_58;
  uStack_a0 = uStack_78;
  local_a8 = local_80;
  local_98 = local_70;
  local_68 = (ID *)FUN_00008790(8);
  *local_68 = param_5;
  local_58 = &LAB_04416474;
  local_50 = &PTR_FUN_0df6bc68;
  FUN_0435b784(&local_90,*(undefined8 *)(param_1 + 0x20),&local_c0,param_4,&local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68);
  }
  if ((long)local_98 < 0) {
    FUN_00005810(local_a8,local_98 & 0x7fffffffffffffff);
  }
  if ((long)local_b0 < 0) {
    FUN_00005810(local_c0,local_b0 & 0x7fffffffffffffff);
  }
  if (local_90 == 1) {
    IVar2 = NSNumber::numberWithInt_
                      ((ID)&_OBJC_CLASS___NSNumber,"numberWithInt:",local_88 & 0xffffffff);
  }
  else {
    IVar2 = 0;
  }
  if ((local_90 & 1) == 0) {
    FUN_04b35898();
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
    return IVar2;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


