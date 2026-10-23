// ==== 0x4447030  FUN_04447030  (312 bytes)

/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

void FUN_04447030(long param_1,undefined8 param_2,const_void__ param_3,undefined8 *param_4)

{
  ID self;
  ID IVar1;
  undefined8 uVar2;
  undefined *local_78;
  undefined8 uStack_70;
  undefined1 *local_68;
  undefined *puStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  long lStack_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  local_78 = PTR___NSConcreteStackBlock_0d824120;
  uStack_70 = 0xc6000000;
  local_68 = &LAB_043e4e50;
  puStack_60 = &DAT_0df6b568;
  uStack_50 = param_4[1];
  local_58 = *param_4;
  lStack_40 = param_4[3];
  local_48 = param_4[2];
  if (*(code **)param_4[3] != (code *)0x0) {
    local_58 = (**(code **)param_4[3])(*param_4);
  }
  self = *(ID *)(param_1 + 0x10);
  uVar2 = *(undefined8 *)(param_1 + 8);
  IVar1 = _objc_alloc(&objc::class_t::ArcExtensionInfo);
  IVar1 = ArcExtensionInfo::initWithExtensionInfo_(IVar1,"initWithExtensionInfo:",param_3);
  _objc_msgSend(self,"webContents:askForExtensionInstallationConsent:completion:",uVar2,IVar1,
                &local_78);
  uVar2 = 0;
  if (*(code **)(lStack_40 + 8) != (code *)0x0) {
    uVar2 = (**(code **)(lStack_40 + 8))(local_58);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(uVar2);
}


