// ==== 0x4415520  ArcWebContentsDevTools::execute:completion:error:  (712 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_claimAutoreleasedReturnValue replaced with injection: _objc_retain_fixup
    */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x28 bytes */

bool ArcWebContentsDevTools::execute_completion_error_
               (ID param_1,SEL param_2,ID param_3,ID param_4,undefined4 param_5,ID *param_6)

{
  code *pcVar1;
  uint uVar2;
  bool bVar3;
  ID IVar4;
  uint *puVar5;
  uint *local_c0;
  uint *local_b8;
  ID *local_b0;
  undefined8 uStack_a8;
  ulong local_a0;
  ID *local_98;
  undefined8 uStack_90;
  ulong uStack_88;
  ID *local_80;
  undefined8 uStack_78;
  ulong local_70;
  ID *local_68;
  undefined8 uStack_60;
  undefined1 *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  local_70 = 0;
  uStack_88 = 0;
  uStack_90 = 0;
  uStack_78 = 0;
  local_80 = (ID *)0x0;
  uStack_a8 = 0;
  local_b0 = (ID *)0x0;
  local_98 = (ID *)0x0;
  local_a0 = 0;
  IVar4 = _objc_msgSend(param_3,"domain");
  if (IVar4 == 0) {
    local_68 = (ID *)0x0;
    uStack_60 = 0;
    local_58 = (undefined1 *)0x0;
  }
  else {
    FUN_01b356b8(&local_68,IVar4);
  }
  if ((long)local_a0 < 0) {
    FUN_00005810(local_b0,local_a0 & 0x7fffffffffffffff);
  }
  uStack_a8 = uStack_60;
  local_b0 = local_68;
  local_a0 = (ulong)local_58;
  IVar4 = _objc_msgSend(param_3,"name");
  if (IVar4 == 0) {
    local_68 = (ID *)0x0;
    uStack_60 = 0;
    local_58 = (undefined1 *)0x0;
  }
  else {
    FUN_01b356b8(&local_68,IVar4);
  }
  if ((long)uStack_88 < 0) {
    FUN_00005810(local_98,uStack_88 & 0x7fffffffffffffff);
  }
  uStack_90 = uStack_60;
  local_98 = local_68;
  uStack_88 = (ulong)local_58;
  IVar4 = _objc_msgSend(param_3,"params");
  if (IVar4 != 0) {
    IVar4 = _objc_msgSend(param_3,"params");
    if (IVar4 == 0) {
      local_68 = (ID *)0x0;
      uStack_60 = 0;
      local_58 = (undefined1 *)0x0;
    }
    else {
      FUN_01b356b8(&local_68,IVar4);
    }
    if ((long)local_70 < 0) {
      FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
    }
    uStack_78 = uStack_60;
    local_80 = local_68;
    local_70 = (ulong)local_58;
  }
  local_b8 = (uint *)0x0;
  if ((*(byte *)(param_1 + 0x30) & 1) != 0) {
    local_68 = (ID *)FUN_00008790(8);
    *local_68 = param_4;
    local_58 = &LAB_044162ac;
    local_50 = &PTR_FUN_0df6bc48;
    FUN_0435b114(&local_b8,*(undefined8 *)(param_1 + 0x20),&local_b0,&local_68);
    if ((code *)local_50[1] != (code *)0x0) {
      (*(code *)local_50[1])(local_68);
    }
    local_c0 = local_b8;
    puVar5 = local_b8;
    if (((ulong)local_b8 & 1) == 0) {
      uVar2 = *local_b8;
      *local_b8 = uVar2 + 1;
      puVar5 = (uint *)(ulong)uVar2;
    }
    bVar3 = FUN_04402548(puVar5,&local_c0,param_5);
    if (((ulong)local_c0 & 1) == 0) {
      FUN_04b35898();
    }
    if (((ulong)local_b8 & 1) == 0) {
      FUN_04b35898();
    }
    FUN_04052260(&local_b0);
    if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
      return bVar3;
    }
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(1,0x44157e4);
  (*pcVar1)();
}


