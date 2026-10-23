// ==== 0x43f6444  FUN_043f6444  (324 bytes)

/* WARNING: Function: _objc_loadWeakRetained replaced with injection: _objc_load_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */

void FUN_043f6444(long param_1,undefined8 *param_2,undefined8 *param_3,undefined8 *param_4)

{
  undefined8 *puVar1;
  long lVar2;
  char cVar3;
  ID self;
  undefined8 uVar4;
  undefined8 uVar5;
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
  self = *(ID *)(param_1 + 8);
  cVar3 = *(char *)((long)param_2 + 0x17);
  puVar1 = (undefined8 *)*param_2;
  if (-1 < (long)cVar3) {
    puVar1 = param_2;
  }
  lVar2 = param_2[1];
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  uVar4 = FUN_01d9fb90(puVar1,lVar2);
  cVar3 = *(char *)((long)param_3 + 0x17);
  puVar1 = (undefined8 *)*param_3;
  if (-1 < (long)cVar3) {
    puVar1 = param_3;
  }
  lVar2 = param_3[1];
  if (-1 < cVar3) {
    lVar2 = (long)cVar3;
  }
  uVar5 = FUN_01d9fb90(puVar1,lVar2);
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
  _objc_msgSend(self,"didImportLockDialogShow:withDialogBody:completion:",uVar4,uVar5,&local_78);
  uVar4 = 0;
  if (*(code **)(lStack_40 + 8) != (code *)0x0) {
    uVar4 = (**(code **)(lStack_40 + 8))(local_58);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(uVar4);
}


