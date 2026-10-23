// ==== 0x4043800  objc::Categories::ArcBrowserContextHistory_Extensions_::expireHistory:completion:  (356 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* Function Stack Size: 0x20 bytes */

void objc::Categories::ArcBrowserContextHistory_Extensions_::expireHistory_completion_
               (ID param_1,SEL param_2,ID param_3,ID param_4,uint param_5)

{
  code *pcVar1;
  undefined8 **local_b0 [2];
  byte local_a0;
  undefined *local_98;
  undefined8 uStack_90;
  undefined1 *local_88;
  undefined *puStack_80;
  undefined8 **local_78;
  undefined8 *local_70;
  undefined8 local_68;
  undefined8 local_60;
  ID *local_58 [2];
  undefined1 *local_48;
  undefined **local_40;
  long local_38;
  
  local_38 = *(long *)PTR____stack_chk_guard_0d824020;
  local_78 = &local_70;
  local_70 = &local_68;
  local_68 = 0;
  local_98 = PTR___NSConcreteGlobalBlock_0d824130;
  uStack_90 = 0xd0800000;
  local_60 = 0;
  local_88 = &LAB_040451a0;
  puStack_80 = &DAT_0da87b48;
  _objc_msgSend(param_3,"enumerateObjectsUsingBlock:",&local_98,param_4,(ulong)param_5);
  _objc_msgSend(param_1,"browser_context_history",local_b0);
  if ((local_a0 & 1) == 0) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(1,0x4043960);
    (*pcVar1)();
  }
  local_48 = &LAB_04044e30;
  local_40 = &PTR_FUN_0da87be8;
  local_58[0] = (ID *)FUN_00008790(8);
  *local_58[0] = param_4;
  FUN_04332e70(local_b0,&local_70,local_58);
  if ((code *)local_40[1] != (code *)0x0) {
    (*(code *)local_40[1])(local_58[0]);
  }
  if (local_a0 != 0) {
    FUN_04332750(local_b0);
  }
  local_b0[0] = &local_70;
  FUN_0063242c(local_b0,local_68);
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


