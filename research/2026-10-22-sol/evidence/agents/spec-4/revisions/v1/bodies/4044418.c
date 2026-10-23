// ==== 0x4044418  objc::Categories::ArcBrowserContextHistory_Extensions_::getVisitsForURL:completion:  (296 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContextHistory_Extensions_::getVisitsForURL_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,uint param_5)

{
  code *pcVar1;
  undefined8 local_88 [2];
  undefined7 local_78;
  char cStack_71;
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
    pcVar1 = (code *)SoftwareBreakpoint(1,0x404453c);
    (*pcVar1)();
  }
  FUN_043abdf8(local_88,param_3);
  local_48 = &LAB_04044f38;
  local_40 = &PTR_FUN_0da87c68;
  local_58[0] = (ID *)FUN_00008790(8);
  *local_58[0] = param_4;
  FUN_04333190(auStack_70,local_88,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if (cStack_71 < '\0') {
    FUN_00005810(local_88[0],CONCAT17(cStack_71,local_78) & 0x7fffffffffffffff);
  }
  if (local_60 != 0) {
    FUN_04332750(auStack_70);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


