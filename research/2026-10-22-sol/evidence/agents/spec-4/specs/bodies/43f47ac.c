// ==== 0x43f47ac  objc::Categories::ArcBrowserContext_Extensions_::favicon:iconTypes:sizeInPixels:fallbackToHost:completion:  (508 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x34 bytes */

void objc::Categories::ArcBrowserContext_Extensions_::
     favicon_iconTypes_sizeInPixels_fallbackToHost_completion_
               (ID param_1,SEL param_2,ID param_3,long_long param_4,long_long param_5,bool param_6,
               ID param_7,undefined4 param_8)

{
  long lVar1;
  code *pcVar2;
  undefined8 local_a8 [2];
  undefined7 local_98;
  char cStack_91;
  undefined8 local_90;
  long *local_88;
  byte local_80;
  ID *local_78 [2];
  code *local_68;
  undefined **local_60;
  long local_58;
  
  local_58 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context",&local_90);
  if ((local_80 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar2 = (code *)SoftwareBreakpoint(1,0x43f49a4);
    (*pcVar2)();
  }
  FUN_043abdf8(local_a8,param_3);
  local_68 = FUN_043f8954;
  local_60 = &PTR_FUN_0df6ba28;
  local_78[0] = (ID *)FUN_00008790(8);
  *local_78[0] = param_7;
  FUN_042d6548(local_90,local_a8,(uint)param_4 & 0x1f,param_5,param_6,local_78);
  if ((code *)local_60[1] != (code *)0x0) {
    (*(code *)local_60[1])(local_78[0]);
  }
  if (cStack_91 < '\0') {
    FUN_00005810(local_a8[0],CONCAT17(cStack_91,local_98) & 0x7fffffffffffffff);
  }
  if ((local_80 != 0) && (local_88 != (long *)0x0)) {
    LOAcquire();
    lVar1 = local_88[1];
    local_88[1] = lVar1 + -1;
    LORelease();
    if (lVar1 == 0) {
      (**(code **)(*local_88 + 0x10))(local_88);
      if (local_88[2] != 0) {
        LOAcquire();
        lVar1 = local_88[2];
        local_88[2] = lVar1 + -1;
        LORelease();
        if (lVar1 != 0) goto LAB_043f48d8;
      }
      pcVar2 = *(code **)(*local_88 + 0x20);
      if (((pcVar2 == FUN_00f01bc0) || (pcVar2 == FUN_00f01bc0)) || (pcVar2 == FUN_013efb40)) {
        FUN_00005810(local_88,0x20);
      }
      else {
        (*pcVar2)(local_88);
      }
    }
  }
LAB_043f48d8:
  if (*(long *)PTR____stack_chk_guard_0d824020 != local_58) {
                    /* WARNING: Subroutine does not return */
    ___stack_chk_fail(0);
  }
  return;
}


