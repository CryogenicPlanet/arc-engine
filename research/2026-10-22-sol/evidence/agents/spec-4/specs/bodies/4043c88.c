// ==== 0x4043c88  objc::Categories::ArcBrowserContextHistory_Extensions_::addVisit:completion:  (300 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContextHistory_Extensions_::addVisit_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,uint param_5)

{
  code *pcVar1;
  undefined8 local_78;
  undefined1 auStack_70 [16];
  byte local_60;
  ID *local_58 [2];
  undefined1 *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  _objc_msgSend(param_1,"browser_context_history",auStack_70,param_3,param_4,(ulong)param_5);
  if ((local_60 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4043db0);
    (*pcVar1)();
  }
  if (param_3 == 0) {
    local_78 = 0;
  }
  else {
    _objc_msgSend(param_3,"toAnnotatedVisit",&local_78);
  }
  local_48 = &LAB_04045100;
  local_40 = &PTR_FUN_0da87c08;
  local_58[0] = (ID *)FUN_00008790(8);
  *local_58[0] = param_4;
  FUN_04332fc0(auStack_70,&local_78,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  FUN_043364d8(&local_78);
  if (local_60 != 0) {
    FUN_04332750(auStack_70);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


