// ==== 0x43fd5b0  ArcBrowserContextExtensions::getExtensionInfo:  (288 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_autoreleaseReturnValue replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x18 bytes */

ID ArcBrowserContextExtensions::getExtensionInfo_(ID param_1,SEL param_2,ID param_3)

{
  code *pcVar1;
  ulong uVar2;
  ID IVar3;
  undefined8 local_128;
  undefined8 uStack_120;
  ulong local_118;
  undefined8 local_110;
  undefined8 uStack_108;
  undefined8 local_100;
  undefined8 uStack_f8;
  undefined8 uStack_f0;
  undefined8 uStack_e8;
  undefined8 local_e0;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  undefined8 uStack_c8;
  undefined8 local_c0;
  undefined8 uStack_b8;
  undefined8 uStack_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  ulong local_40;
  
  local_40 = 0;
  uStack_58 = 0;
  local_60 = 0;
  uStack_48 = 0;
  uStack_50 = 0;
  uStack_78 = 0;
  local_80 = 0;
  uStack_68 = 0;
  local_70 = 0;
  uStack_98 = 0;
  local_a0 = 0;
  uStack_88 = 0;
  uStack_90 = 0;
  uStack_b8 = 0;
  local_c0 = 0;
  uStack_a8 = 0;
  uStack_b0 = 0;
  uStack_d8 = 0;
  local_e0 = 0;
  uStack_c8 = 0;
  uStack_d0 = 0;
  uStack_f8 = 0;
  local_100 = 0;
  uStack_e8 = 0;
  uStack_f0 = 0;
  uStack_108 = 0;
  local_110 = 0;
  FUN_043386a0(&uStack_98);
  local_40 = local_40 & 0xffffffffffffff00;
  uStack_58 = 0;
  local_60 = 0;
  uStack_48 = 0;
  uStack_50 = 0;
  uStack_68 = 0;
  local_70 = 0;
  if ((*(byte *)(param_1 + 0x30) & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x43fd6d0);
    (*pcVar1)();
  }
  if (param_3 == 0) {
    local_128 = 0;
    uStack_120 = 0;
    local_118 = 0;
  }
  else {
    FUN_01b356b8(&local_128,param_3);
  }
  uVar2 = FUN_042e7df4(*(undefined8 *)(param_1 + 0x20),&local_128,&local_110);
  if ((long)local_118 < 0) {
    FUN_00005810(local_128,local_118 & 0x7fffffffffffffff);
  }
  if ((uVar2 & 1) == 0) {
    IVar3 = 0;
  }
  else {
    IVar3 = _objc_alloc(&objc::class_t::ArcExtensionInfo);
    IVar3 = ArcExtensionInfo::initWithExtensionInfo_(IVar3,"initWithExtensionInfo:",&local_110);
  }
  FUN_04334d2c(&local_110);
  return IVar3;
}


