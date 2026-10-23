// ==== 0x40448f8  objc::Categories::ArcBrowserContextHistory_Extensions_::getLocalCacheGUID:  (228 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x18 bytes */

void objc::Categories::ArcBrowserContextHistory_Extensions_::getLocalCacheGUID_
               (ID param_1,SEL param_2,ID param_3,uint param_4)

{
  code *pcVar1;
  undefined1 auStack_60 [16];
  byte local_50;
  ID *local_48 [2];
  code *local_38;
  undefined **local_30;
  long local_28;
  
  local_28 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context_history",auStack_60,param_3,(ulong)param_4);
  if ((local_50 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x40449d8);
    (*pcVar1)();
  }
  local_38 = FUN_04044ef4;
  local_30 = &PTR_FUN_0da87cc8;
  local_48[0] = (ID *)FUN_00008790(8);
  *local_48[0] = param_3;
  FUN_04333378(auStack_60,local_48);
  if ((code *)local_30[1] != (code *)0x0) {
    (*(code *)local_30[1])(local_48[0]);
  }
  if (local_50 != 0) {
    FUN_04332750(auStack_60);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


