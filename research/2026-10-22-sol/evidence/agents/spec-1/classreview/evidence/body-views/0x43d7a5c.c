// ==== 0x43d7a5c  objc::Categories::ArcAIService_Extensions_::streamInference:configuration:model:partialResultCallback:completion:  (688 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x38 bytes */

unsigned_short
objc::Categories::ArcAIService_Extensions_::
streamInference_configuration_model_partialResultCallback_completion_
          (ID param_1,SEL param_2,ID param_3,ID param_4,ID param_5,ID param_6,uint param_7,
          ID param_8,undefined4 param_9)

{
  long lVar1;
  unsigned_short uVar2;
  undefined8 uVar3;
  code *pcVar4;
  undefined1 auStack_e8 [8];
  undefined8 local_e0;
  undefined7 local_d0;
  char cStack_c9;
  undefined8 local_c8;
  undefined8 uStack_c0;
  ulong local_b8;
  undefined1 auStack_b0 [8];
  long *local_a8;
  byte local_a0;
  ulong *local_98 [2];
  undefined1 *local_88;
  undefined **local_80;
  ID *local_78 [2];
  undefined1 *local_68;
  undefined **local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"ai_service",auStack_b0);
  if ((local_a0 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar4 = (code *)SoftwareBreakpoint(1,0x43d7d08);
    (*pcVar4)();
  }
  if (param_3 == 0) {
    local_c8 = 0;
    uStack_c0 = 0;
    local_b8 = 0;
  }
  else {
    FUN_01b356b8(&local_c8,param_3);
  }
  uVar3 = FUN_0443a40c(param_4);
  FUN_0443a21c(auStack_e8,param_5);
  local_68 = &LAB_043db694;
  local_60 = &PTR_FUN_0df6b378;
  local_78[0] = (ID *)FUN_00008790(8);
  *local_78[0] = param_6;
  local_88 = &LAB_043db42c;
  local_80 = &PTR_FUN_0df6b398;
  local_98[0] = (ulong *)FUN_00008790(8);
  *local_98[0] = (ulong)param_7;
  uVar2 = FUN_043310bc(auStack_b0,&local_c8,uVar3,auStack_e8,local_78,local_98);
  if ((code *)local_80[1] != (code *)0x0) {
    (*(code *)local_80[1])(local_98[0]);
  }
  if ((code *)local_60[1] != (code *)0x0) {
    (*(code *)local_60[1])(local_78[0]);
  }
  if (cStack_c9 < '\0') {
    FUN_00005810(local_e0,CONCAT17(cStack_c9,local_d0) & 0x7fffffffffffffff);
  }
  if ((long)local_b8 < 0) {
    FUN_00005810(local_c8,local_b8 & 0x7fffffffffffffff);
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
        if (lVar1 != 0) goto LAB_043d7c20;
      }
      pcVar4 = *(code **)(*local_a8 + 0x20);
      if (((pcVar4 == FUN_00f01bc0) || (pcVar4 == FUN_00f01bc0)) || (pcVar4 == FUN_013efb40)) {
        FUN_00005810(local_a8,0x20);
      }
      else {
        (*pcVar4)(local_a8);
      }
    }
  }
LAB_043d7c20:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_58) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return uVar2;
}


