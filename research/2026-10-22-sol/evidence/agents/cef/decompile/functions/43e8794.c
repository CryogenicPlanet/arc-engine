// ==== 0x43e8794  ArcBrowserApplication::loadBrowserContext:type:completion:  (320 bytes)

/* WARNING: Function: _objc_retain replaced with injection: _objc_retain_fixup */
/* WARNING: Function: _objc_release replaced with injection: _objc_release_fixup */
/* WARNING: Function: _objc_retainBlock replaced with injection: _objc_retain_fixup */
/* Function Stack Size: 0x28 bytes */

void ArcBrowserApplication::loadBrowserContext_type_completion_
               (ID param_1,SEL param_2,ID param_3,unsigned_long_long param_4,ID param_5,
               undefined4 param_6)

{
  code *pcVar1;
  SEL extraout_x1;
  undefined8 *puVar2;
  undefined8 local_80;
  undefined8 uStack_78;
  ulong local_70;
  ID *local_68 [2];
  undefined1 *local_58;
  undefined **local_50;
  long local_48;
  
  local_48 = *(long *)PTR____stack_chk_guard_0d824020;
  puVar2 = *(undefined8 **)(param_1 + 0x20);
  if ((param_4 != 0) && (param_4 != 1)) {
                    /* WARNING: Does not return */
    pcVar1 = (code *)SoftwareBreakpoint(0x550b,0x43e88d4);
    (*pcVar1)();
  }
  if (param_3 == 0) {
    local_80 = 0;
    uStack_78 = 0;
    local_70 = 0;
  }
  else {
    FUN_01b356b8(&local_80,param_3);
    param_2 = extraout_x1;
  }
  local_68[0] = (ID *)FUN_00008790(8,param_2);
  *local_68[0] = param_5;
  local_58 = &LAB_043eb7a0;
  local_50 = &PTR_FUN_0df6b768;
  FUN_042b5c28(*puVar2,param_4,&local_80,local_68);
  if ((code *)local_50[1] != (code *)0x0) {
    (*(code *)local_50[1])(local_68[0]);
  }
  if ((long)local_70 < 0) {
    FUN_00005810(local_80,local_70 & 0x7fffffffffffffff);
  }
  if (*(long *)PTR____stack_chk_guard_0d824020 == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  ___stack_chk_fail(0);
}


