// ==== 0x43d9ab8  objc::Categories::ArcAIService_Extensions_::evaluateCommandBarRanking:completion:  (604 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Type propagation algorithm not settling */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcAIService_Extensions_::evaluateCommandBarRanking_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,uint param_5)

{
  long lVar1;
  ID IVar2;
  code *pcVar3;
  undefined8 local_b0;
  long *local_a8;
  byte local_a0;
  undefined *local_98;
  undefined8 uStack_90;
  undefined1 *local_88;
  undefined *puStack_80;
  ID *local_78;
  ID local_70;
  undefined8 uStack_68;
  undefined8 local_60;
  ID *local_58;
  ID *pIStack_50;
  undefined1 *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  local_70 = 0;
  uStack_68 = 0;
  local_60 = 0;
  IVar2 = _objc_msgSend(param_3,"count",param_3,param_4,(ulong)param_5);
  if (IVar2 != 0) {
    if (0x492492492492492 < IVar2) {
                    /* WARNING: Subroutine does not return */
      FUN_040416c0();
    }
    local_58 = (ID *)FUN_00008790(IVar2 * 0x38);
    local_48 = (undefined1 *)0x0;
    pIStack_50 = local_58;
    local_40 = (undefined **)IVar2;
    FUN_043dab74(&local_58,&local_70,&uStack_68,&local_60);
    FUN_043daef4(&local_58);
  }
  local_98 = PTR___NSConcreteGlobalBlock_0d824130;
  uStack_90 = 0xd0800000;
  local_88 = &LAB_043d9df4;
  puStack_80 = &DAT_0df6b278;
  local_78 = &local_70;
  _objc_msgSend(param_3,"enumerateObjectsUsingBlock:",&local_98);
  _objc_msgSend(param_1,"ai_service",&local_b0);
  if ((local_a0 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(1,0x43d9d0c);
    (*pcVar3)();
  }
  local_58 = (ID *)FUN_00008790(8);
  *local_58 = param_4;
  local_48 = &LAB_043dc050;
  local_40 = &PTR_FUN_0df6b4a8;
  FUN_0429d6b8(local_b0,&local_70,&local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58);
  }
  if ((local_a0 != 0) && (local_a8 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_a8[1];
    local_a8[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_a8 + 0x10))(local_a8);
      if (local_a8[2] != 0) {
        LOAcquire();
        lVar1 = local_a8[2];
        local_a8[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043d9c38;
      }
      pcVar3 = *(code **)(*local_a8 + 0x20);
      if (((pcVar3 == FUN_00f01bc0) || (pcVar3 == FUN_00f01bc0)) || (pcVar3 == FUN_013efb40)) {
        FUN_00005810(local_a8,0x20);
      }
      else {
        (*pcVar3)(local_a8);
      }
    }
  }
LAB_043d9c38:
  local_58 = &local_70;
  FUN_043db22c(&local_58);
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_38) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}


